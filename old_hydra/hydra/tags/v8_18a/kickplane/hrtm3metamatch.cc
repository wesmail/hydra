#include "hrtm3metamatch.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hades.h"
#include "kickdef.h"
#include "hmdchit.h"
#include "hspecgeompar.h"
#include "hmdcgeompar.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hiterator.h"
#include "hmdchit.h"
#include "hrtmetaseg.h"
#include "hmdcdef.h"
#include "hgeomvolume.h"

HRtM3MetaMatch::HRtM3MetaMatch(Text_t name[],Text_t title[]) : 
  HReconstructor(name,title)
{
  fCatMdc = 0;
  fMdcIter = 0;
  fCatSeg = 0;
  fModule = 2;
}

HRtM3MetaMatch::~HRtM3MetaMatch(void) {
}

Bool_t HRtM3MetaMatch::init(void) {
  if (gHades) {
    HEvent *ev = gHades->getCurrentEvent();
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (!fCorrelator.init(ev,rtdb)) return kFALSE;
    
    fCatMdc = ev->getCategory(catMdcHit);
    if (!fCatMdc) {
      Error("init","Category with HMdcHit not found");
      return kFALSE;
    }
    fMdcIter = (HIterator *)fCatMdc->MakeIterator();

    fMdcGeometry = (HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
    fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
    if (!fMdcGeometry || !fSpecGeometry) return kFALSE;

    fCatSeg = ev->getCategory(catKickMetaSeg);
    if (!fCatSeg) {
      fCatSeg = makeOutputCategory(ev);
      ev->addCategory(catKickMetaSeg,fCatSeg,"Tracks"); 
    }
  }

  fHitLoc.set(2,0,fModule);
  fSecLoc.set(1,0);
  return kTRUE;
}

HCategory *HRtM3MetaMatch::makeOutputCategory(HEvent *ev) {
  HCategory *r;
  Int_t sizes[2]={6,60}; //Maximum 60 tracks per sector
  r = new HMatrixCategory("HRtMetaSeg",2,sizes,0.5);
  return r;
}

Bool_t HRtM3MetaMatch::reinit(void) {
  HModGeomPar *module;
  for (Int_t sector=0;sector<6;sector++) {
    module =  fMdcGeometry->getModule(sector,fModule);
    if (module) {
      HGeomTransform &TA = module->getLabTransform();
      HGeomTransform &TS = fSpecGeometry->getSector(sector)->getTransform();
    
      // Transformation from module to sector system.
      fMdc2SecTrans[sector] = new HGeomTransform(TA);
      fMdc2SecTrans[sector]->transTo(TS);
    } else {
      fMdc2SecTrans[sector] = 0;
    }
  }
  return kTRUE;
}

Bool_t HRtM3MetaMatch::finalize(void) {
  return kTRUE;
}

HRtMetaSeg *HRtM3MetaMatch::fillData(HMdcHit *hit, HGeomVector &pos, 
				     HGeomVector &alpha, HRtMetaPoint *point) 
{ 
  HRtMetaSeg *c=0;
  TObject *slot=0;
  HGeomVector dir;

  dir.setX(point->getX() - pos.getX());
  dir.setY(point->getY() - pos.getY());
  dir.setZ(point->getZ() - pos.getZ());
  dir /= dir.length();

  slot = fCatSeg->getNewSlot(fSecLoc);
  if (slot) {
    c = makeSegment(slot);
    c->setPosition(pos);
    c->setDirection(dir);
    c->setTof(point->getTof());
    c->setSystem(point->getSystem());
    c->setPull(fCorrelator.getDistance()); 
    c->setResXY(fCorrelator.getResX(),fCorrelator.getResY());
  } else {
    Error("fillData","No slots free");
    return 0;
  }
  return c;
}

Int_t HRtM3MetaMatch::execute(void) {
  HMdcHit *hit=0;
  HRtMetaPoint *point=0;
  HGeomVector pos, alpha;
  HGeomVector localPos,localAlpha;
  HGeomVector r;
  Float_t xdir,ydir,e;
  Float_t dX,dY,bX,bY;

  fHitLoc[1] = fModule; // Look at MDC3
  fHitLoc[0] = 0;
  for (fSecLoc[0] = 0; fSecLoc[0] < 1; fSecLoc[0]++) {
    if (fMdc2SecTrans[fSecLoc[0]]) { //MDC sector was defined in setup
      fHitLoc[0] = fSecLoc[0];
      fCorrelator.setSector(fSecLoc[0]);
      fCorrelator.readMetaPoints();
      
      fMdcIter->Reset();
      fMdcIter->gotoLocation(fHitLoc);
     
      while ( (hit = (HMdcHit *)fMdcIter->Next()) != 0) {

	// Convert to normal representation in SEC system
	localPos.setXYZ(hit->getX(),hit->getY(),0.);
	hit->getXYDir(xdir,e,ydir,e);
	localAlpha.setXYZ(xdir, ydir, sqrt(1-xdir*xdir-ydir*ydir));
	pos = fMdc2SecTrans[fSecLoc[0]]->transFrom(localPos);
	alpha = fMdc2SecTrans[fSecLoc[0]]->getRotMatrix() * localAlpha;

	for (Int_t i=0;i<fCorrelator.getNMetaPoints();i++) {
	  HRtMetaPoint &point = fCorrelator.getMetaPoint(i);
	  
	  r.setXYZ(point.getX(), point.getY(), point.getZ());

	  bX = alpha.getX() / alpha.getZ();
	  bY = alpha.getY() / alpha.getZ();

	  dX = pos.getX() + bX * (r.getZ() - pos.getZ());
	  dX -= r.getX();	  
	  dY = pos.getY() + bY * (r.getZ() - pos.getZ());
	  dY -= r.getY();
	  fillControl(hit,point,dX,dY);
	 //  if (dX<1000 && dY<1000) {
// 	    fillData(hit,pos,alpha,&point);
// 	  }
	}

	point = fCorrelator.findClosestPoint(pos,alpha,0.);
	if (point != 0) {
	  //fillControl(hit,*point,0,0);
	  fillData(hit,pos,alpha,point);
	}
      }
    }
  }
  return 0;  
}

void HRtM3MetaMatch::fillControl(HMdcHit *, HRtMetaPoint &,
				 Float_t ,Float_t ) {
}

HRtMetaSeg *HRtM3MetaMatch::makeSegment(TObject *s) { 
  return new(s) HRtMetaSeg;
}

ClassImp(HRtM3MetaMatch)
