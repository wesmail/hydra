#include "hrtmetacorrelator.h"
#include "hcategory.h"
#include "hlocation.h"
#include "htofgeompar.h"
#include "hiterator.h"
#include "hshowergeometry.h"
#include "hruntimedb.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hspecgeompar.h"
#include "hkicktrack.h"
#include "tofdef.h"
#include "kickdef.h"
#include "showertofinodef.h"
#include "haddef.h"
#include "htofhit.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hshowerhittof.h"
#include "hmdcdef.h"
#include "hrtmetatrack.h"
#include "hkickplane2.h"

#include "htofhitsim2.h"

static Float_t gSqrt12 = sqrt(12.);

//*-- Author : M. Sanchez

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////
//  HRtMetaCorrelator
//
// Matches a straight line with a hit in Meta
/////////////////////////////////////////////////////////////////////

HRtMetaCorrelator::HRtMetaCorrelator(void) {  
  fSecLoc.set(1,0); 
  fMetaPoints = new TClonesArray("HRtMetaPoint",50); // Array of HGeomVector
  fMaxDist = 1400.;
  fTofCat = 0;//!
  fShowerCat = 0;//!
  fTofGeometry = 0;//!
  fShowerGeometry = 0;//!
  fSpecGeometry = 0; //!Spectrometer geometry
  fTofIter = 0;//!
  fShowerIter = 0;//!
  fLatestDistance = 0.; //FIXME: Quick and dirty hack
  fResX = fResY = 0.;
}

Bool_t HRtMetaCorrelator::getShowerCategory(HEvent *event) {
	Bool_t r=kFALSE;
	fShowerCat = event->getCategory(catShowerHitTof);
  	if (!fShowerCat) {
		Error("init","Shower/Tofino hit category not found!");
		r = kFALSE;
  	} else r = kTRUE;
	return r;
}
	

Bool_t HRtMetaCorrelator::init(HEvent *event,HRuntimeDb *rtdb) {
  fTofCat = event->getCategory(catTofHit);
  if (!fTofCat) {
    Error("init","Tof Hit category not found!");
    return kFALSE;
  }
  fTofIter = dynamic_cast<HIterator *>(fTofCat->MakeIterator());
  
  fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");

  if (!getShowerCategory(event)) return kFALSE;
  fShowerIter = dynamic_cast<HIterator *>(fShowerCat->MakeIterator());

  fShowerGeometry= (HShowerGeometry *)rtdb->getContainer("ShowerGeometry");

  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

  fOut = new TFile("match.root","RECREATE");
  fControl = new TNtuple("cont","control","xf:yf:zf:d:dx:dy:valid:sys:xm:ym:zm:xk:yk:zk");

  return kTRUE;
}

Bool_t HRtMetaCorrelator::finalize(void) {
  fOut->cd();
  fControl->Write();
  fOut->Close();  
  return kTRUE;
}

HRtMetaPoint *HRtMetaCorrelator::findClosestPoint(HGeomVector &r0, 
						  HGeomVector &alpha,
						  Float_t leverArm) {
  // Finds closest hit in TOF/Shower to a given track within a distance limit.
  Int_t n = fMetaPoints->GetEntriesFast();
  Float_t dmin = fMaxDist;
  HRtMetaPoint *pMin = 0;
  Float_t d,dX,dY,bX,bY;
  HGeomVector r;

  for (Int_t i = 0; i < n; i++) {
    HRtMetaPoint &point = *((HRtMetaPoint *)fMetaPoints->UncheckedAt(i));

    r.setX(point.getX());
    r.setY(point.getY());
    r.setZ(point.getZ());
    
    d = (r0 - r).vectorProduct(alpha).length();
  
    bX = alpha.getX() / alpha.getZ();
    bY = alpha.getY() / alpha.getZ();
    dX = r0.getX() + bX * (r.getZ() - r0.getZ());
    dX -= r.getX();

    dY = r0.getY() + bY * (r.getZ() - r0.getZ());
    dY -= r.getY();

    if (d < dmin) {
	dmin = d;
	pMin = (HRtMetaPoint *)fMetaPoints->UncheckedAt(i);
    }

    fResX = dX;
    fResY = dY;

    fControl->Fill(r.getX(),r.getY(),r.getZ(),
		   d,dX,dY,(pMin == 0)?0:1,point.getSystem(),
		   r0.getX(),r0.getY(),r0.getZ(),
		   r0.getX() - leverArm * alpha.getX(),
		   r0.getY() - leverArm * alpha.getY(),
		   r0.getZ() - leverArm * alpha.getZ());
  }
  fLatestDistance = dmin; //FIXME: there are better ways.
  return pMin;
}

void HRtMetaCorrelator::readMetaPoints(void) {
  // Read points from TOF/Shower
  HTofHit *tofHit = 0;
  HShowerHitTof *shoHit = 0;

  fMetaPoints->Clear();
  
  fTofIter->Reset();
  while ( (tofHit = (HTofHit *)fTofIter->Next()) != 0) {
    if (tofHit->getSector() == fSecLoc[0] && tofHit->getModule()<8)
      if (tofHit->getTof()>0 && tofHit->getTof()<60)
	addPoint(tofHit);
  }

  fShowerIter->Reset();
  while ( (shoHit = (HShowerHitTof *)fShowerIter->Next()) != 0) {
    if (shoHit->getSector() == fSecLoc[0] && shoHit->getModule()==0)
      addPoint(shoHit);
  }
}

void HRtMetaCorrelator::addPoint(HTofHit *data) {
  // Transforms and initializes one point
  HGeomVector rLocal,errLocal;
  //  HRtMetaPoint &r = createMetaPoint();
  HGeomVector r;
  Float_t x,y,rodHeight,rodLength;
  //  const Float_t tofXResol=15.5; // TOF resolution in mm
  HModGeomPar *module=fTofGeometry->getModule(data->getSector(), //Sector
					      data->getModule());
  HGeomTransform &transMod = module->getLabTransform();
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())
    ->getTransform();

  HGeomTransform modTrans(transMod);
  modTrans.transTo(transSec);

  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();
  const HGeomRotation &rot=modTrans.getRotMatrix();
  const HGeomVector &transVec=modTrans.getTransVector(); 
  rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - rodVol->getPoint(1)->getY());
  rodLength = 2. * TMath::Abs(rodVol->getPoint(0)->getX());

  x = data->getXpos();

  y = 0.;

  r.setX(x);
  r.setY(y);
  r.setZ(0.);
  rLocal=rodTrans.transFrom(r); //Rod to modue system

  r=modTrans.transFrom(rLocal); //Module to LAB system
  r.setZ( r.getZ() );

  HRtMetaPoint &metaPoint = createMetaPoint();
  HGeomVector &errors = metaPoint.getErrors();
  metaPoint.setX(r.getX());
  metaPoint.setY(r.getY());
  metaPoint.setZ(r.getZ());
  metaPoint.setTof(data->getTof());
  metaPoint.setSystem(1+data->getModule()*8+data->getCell());
  metaPoint.setMetaHit(data);
  metaPoint.setSlope( rot(2*3+1) / rot(1*3+1) );
  metaPoint.setOffset(transVec.getZ() - metaPoint.getSlope()*transVec.getY());
  #warning "FIXME: get tof resolutio from somewhere"
  errors.setXYZ(16.,rodHeight/gSqrt12,0);
}


void HRtMetaCorrelator::addPoint(HShowerHitTof *data) {
  // Transforms and initializes one point from the Shower detector
  Float_t x,y;
  HGeomVector rLocal;
  data->getXY(&x,&y);

  //Get transformation from local coord. system to LAB coord system
  const HGeomTransform &transMod=fShowerGeometry->getTransform(data->getSector(),
							       data->getModule());
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
  HGeomTransform trans(transMod);
  trans.transTo(transSec);
  const HGeomRotation &rot = trans.getRotMatrix();
  const HGeomVector &transVec = trans.getTransVector();

  HShowerPad *pad = fShowerGeometry->getPadParam(0)->getPad(data->getRow(),
							    data->getCol());
  data->getXY(&x,&y);

  HGeomVector r;
  HRtMetaPoint &metaPoint = createMetaPoint();
  HGeomVector &errors = metaPoint.getErrors();
  rLocal.setX(x); 
  rLocal.setY(y);
  rLocal.setZ(0.);
  r = trans.transFrom(rLocal);
  metaPoint.setX(r.getX());
  metaPoint.setY(r.getY());
  metaPoint.setZ(r.getZ());
  if (data->getTofinoMult() == 1)
    metaPoint.setTof(data->getTof());
  else 
    metaPoint.setTof(0.);
  metaPoint.setSystem(0);
  metaPoint.setFlag( 0 ); //data->getTrack() );
  metaPoint.setMetaHit(data);
  metaPoint.setSlope( rot(2*3+1) / rot(1*3+1) );
  metaPoint.setOffset( transVec.getZ() - metaPoint.getSlope()*transVec.getY());
  errors.setXYZ( (pad->getXru_mm() - pad->getXlu_mm()) / gSqrt12,
		 (pad->getYlu_mm() - pad->getYrd_mm()) / gSqrt12,
		 0.);  
}

HRtMetaPoint &HRtMetaCorrelator::createMetaPoint(void) {
  Int_t n = fMetaPoints->GetEntriesFast();
  HRtMetaPoint *p = new (fMetaPoints->operator[](n)) HRtMetaPoint;
  return *p;
}

ClassImp(HRtMetaCorrelator)
ClassImp(HRtMetaPoint)
