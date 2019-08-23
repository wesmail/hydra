//*-- Author : M. Sanchez (25.05.2000)
//*-- Modified : M.Sanchez (25.05.2000
#include "hkickdetdigi.h"
#include "hades.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofdetector.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "hmdcsimhit.h"
#include "hdetector.h"
#include "htofhitsim2.h"
#include "hgeantmdc.h"
#include "hgeanttof.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hgeomvector.h"
#include "hiterator.h"
#include "hgeomrotation.h"
#include "hmdcsimseg.h"
#include "hspecgeompar.h"
#include "hmdcdef.h"
#include <TRandom.h>

////////////////////////////////////////////
// HKickMdcDigi
//
//  Simple minded digitization GEEANT --> MDC Hits
//
/////////////////////////////////////////////

HKickDetDigi::HKickDetDigi(Text_t name[],Text_t title[]) :
  HReconstructor(name,title) {
  fMdcInput=0;
  fMdcOut=0;
  fMdcIter=0;
  fTofInput=0;
  fTofOut=0;
  fTofIter=0;
  fFillTof=kTRUE;
  fIsTofPerfect = fIsMdcPerfect = kFALSE;
}

HKickDetDigi::~HKickDetDigi(void) {
  if (fMdcIter) delete fMdcIter;
  if (fTofIter) delete fTofIter;
}

Int_t HKickDetDigi::execute(void) {
  if (fMdcInput) digiMdc2();
  if (fTofInput && isFillingTof()) digiTof();
  return 0;
}

Int_t HKickDetDigi::digiTof(void) {
  HGeantTof *geTof=0;
  HTofHitSim2 *tofHit=0;
  Float_t x,y,en,tof,tofResol=0.1; //100 ps
  HLocation loc;
  HGeantKine *geKine=0;
  Int_t trackN,pid;
  Float_t px=0,py=0,pz=0,mom,len;

  loc.set(3,0,0,0);
  fTofIter->Reset();
  while ( (geTof=(HGeantTof *)fTofIter->Next()) != 0) {
    geTof->getHit(en,x,y,tof,mom,len);
    if (!isTofPerfect()) x+=gRandom->Gaus(0,15.5);
    loc[0]=geTof->getSector();
    loc[1]=21 - geTof->getModule();
    loc[2]=7 - geTof->getCell();
 
    if (loc[1]<0) continue; //This is TOFINO. Skip
    if (tof>50) continue;
    tofHit=(HTofHitSim2 *)fTofOut->getObject(loc);
    if (!tofHit) {
      tofHit=(HTofHitSim2 *)fTofOut->getSlot(loc);
      if (tofHit) {
	tofHit=new (tofHit) HTofHitSim2;
	tofHit->setNHits(1);
	tofHit->setXpos(x);
	if (isTofPerfect()) tofHit->setY(y);
	tofHit->setTof(tof+gRandom->Gaus(0,tofResol));
	tofHit->setSector(loc[0]);
	tofHit->setModule(loc[1]);
	tofHit->setCell(loc[2]);
	tofHit->addTrack(geTof->getTrack());
	geKine=(HGeantKine *)fCatKine->getObject(geTof->getTrack()-1);
	if (geKine) {
	  geKine->getMomentum(px,py,pz);
	  geKine->getParticle(trackN,pid);
	  tofHit->setPid(pid);
	  tofHit->setMomentum(mom);
	}
      }
    } else { //Coger hit mas rapido
      //if (n > 1) Warning("digiTof","Demasiados hits %i",n);
      geKine=(HGeantKine *)fCatKine->getObject(geTof->getTrack()-1);
      if (geKine) {
	Int_t parent,medium,mech;
	geKine->getCreator(parent,medium,mech);
	//tofHit->setNHits(n+1);
	if (tof < tofHit->getTof()) { //FIXME:Select primary tracks
	  tofHit->setXpos(x);
	  tofHit->setTof(tof);
	  tofHit->setSector(loc[0]);
	  tofHit->setModule(loc[1]);
	  tofHit->setCell(loc[2]);
	  tofHit->addTrack(geTof->getTrack());
	  geKine->getMomentum(px,py,pz);
	  geKine->getParticle(trackN,pid);
	  tofHit->setPid(pid);
	  tofHit->setMomentum(TMath::Sqrt(px*px+py*py+pz*pz));
	}
      }	
    }
  }
  return 0;
}

Int_t HKickDetDigi::digiMdc2(void) {
  //Digitizes data from MDC
  const Int_t theModule=0,theLayer=6;
  const Float_t resolX=0.16, resolY=0.08;
  HGeantMdc *geMdc=0,*geMdc2;
  HGeantKine *geKine=0;
  Int_t mdcSector=0,mdcModule=0,mdcLayer=0;
  Int_t trackN,pid;

  Float_t x=0,y=0,tof=0,px=0,py=0,pz=0;
  Float_t x2=0,y2=0,mom=0,mom2=0;
  HGeomVector rLocal,r,alpha;

  fMdcIter->Reset();

  while ( (geMdc=(HGeantMdc *)fMdcIter->Next()) != 0) {
    mdcSector=geMdc->getSector();
    mdcModule=geMdc->getModule();
    mdcLayer=geMdc->getLayer();
    if (mdcModule % 2 == theModule && mdcLayer == theLayer) {
      geMdc2 = findPartner(geMdc,mdcSector,mdcModule+1,mdcLayer);
      if (geMdc2) { //If partner exists
	geMdc->getHit(x,y,tof,mom);
	geMdc2->getHit(x2,y2,tof,mom2);
	if (!isMdcPerfect()) {
	  x+=gRandom->Gaus(0,resolX);
	  y+=gRandom->Gaus(0,resolY);
	  x2+=gRandom->Gaus(0,resolX);
	  y2+=gRandom->Gaus(0,resolY);
	}
	calcPosDir(geMdc,geMdc2,x,y,x2,y2,r,alpha);
	geKine=(HGeantKine *)fCatKine->getObject(geMdc->getTrack()-1);
	if (geKine) {
	  geKine->getParticle(trackN,pid);
	  if (trackN != geMdc->getTrack())
	    Warning("digiMdc","Inconsistent trackN");
	  geKine->getMomentum(px,py,pz);
	  
	  fMdcSectorLoc[0]=mdcSector;
	  fMdcSecModLoc[0]=mdcSector; fMdcSecModLoc[1]=mdcModule / 2;
	  fillData(r,alpha,(mom + mom2) / 2.0, //TMath::Sqrt(px*px + py*py + pz*pz),
		   geMdc->getTrack());
	  fMdcSecModLoc[1]=geMdc->getModule();
          fillHit(geMdc, x, y);
          fMdcSecModLoc[1]=geMdc2->getModule();
          fillHit(geMdc2, x2, y2);             
	} else { Error("digiMdc","Inconsistent track number"); return 1;}
      }
    }
  }

  return 0;
}

void HKickDetDigi::fillHit(HGeantMdc *geMdc, Float_t x, Float_t y) {
  Float_t xDir, yDir;
  Float_t theta,phi;
  Float_t resTheta = 1. ; // Res theta = 1 degree
  Float_t resPhi = 2. ; // Res phi = 2 degrees
  Double_t k = TMath::Pi() / 180.;
  TObject *slot = 0;
  HMdcHit *hit;

  geMdc->getIncidence(theta,phi);


  if (!isMdcPerfect()) {
    theta += gRandom->Gaus(0,resTheta);
    phi += gRandom->Gaus(0,resPhi);
  }

  // Pass to radians
  theta *= k;
  phi *= k;

  slot = fMdcHitOut->getNewSlot(fMdcSecModLoc);
  if (slot != 0) {
    hit = new(slot) HMdcHit;

    xDir = TMath::Sin(theta) * TMath::Cos(phi);
    yDir = TMath::Sin(theta) * TMath::Sin(phi);

    hit->setX(x, .16); //FIXME: do properly
    hit->setY(y, .08); //FIXME: do properly
    hit->setXDir( xDir, .01);
    hit->setYDir( yDir, .01);
    hit->setSecMod( fMdcSecModLoc[0], fMdcSecModLoc[1]);
    hit->setFlag( geMdc->getTrack() );
  }

}                

void HKickDetDigi::fillData(HGeomVector &r,HGeomVector &alpha,
			    Double_t p,Int_t track) {
  HMdcSimSeg *mdcSeg=0;
  TObject *slot=0;
  Float_t theta,phi;

  if (fMdcSecModLoc[1]==0) {
    slot=fMdcOut->getNewSlot(fMdcSecModLoc);
    if (slot != 0) {
      mdcSeg = new(slot) HMdcSimSeg; 
      mdcSeg->setSec(fMdcSectorLoc[0]);
      mdcSeg->setIOSeg(fMdcSecModLoc[1]);
      theta=TMath::ACos(alpha.getZ());
      phi=TMath::ATan2(alpha.getY(),alpha.getX());
      mdcSeg->setR(r.getY()*TMath::Cos(phi) - r.getX()*TMath::Sin(phi),0.1);
      mdcSeg->setZ(-(r.getY()*TMath::Sin(phi) + r.getX()*TMath::Cos(phi))*
		   alpha.getZ()/TMath::Sin(theta),0.1);
      mdcSeg->setThetaPhi(theta,0.1,
			  phi,0.1);
      //Stupid errors
      mdcSeg->setMomentum(p);
      mdcSeg->setTrack(track);
    } else { Warning("fillData","No slot available"); fMdcSecModLoc.Dump(); }
  }
}


void HKickDetDigi::calcPosDir(HGeantMdc *data, HGeantMdc *data2,
			   Float_t x, Float_t y, Float_t x2, Float_t y2,
			   HGeomVector &r,HGeomVector &dir) {
  //Transforms a mdc hit from the MDC coordinate system into the LAB coordinate
  //system and smears the data according to the resolution.
  HGeomVector r1,r2,rLocal;
  
  HModGeomPar *module=fGeometry->getModule(data->getSector(), data->getModule());

  if (!module) Error("calcDir",
		     "Module %i:%i invalid\n",
		     data->getSector(),data->getModule());

  HGeomTransform &transMod = module->getLabTransform();
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
  HGeomTransform modTrans(transMod);
  modTrans.transTo(transSec);

  HModGeomPar *module2=fGeometry->
    getModule(data2->getSector(),data2->getModule());

  if (!module2) Error("calcDir","Module %i:%i invalid\n",
		      data2->getSector(),data2->getModule());

  HGeomTransform &transMod2 = module2->getLabTransform();
  HGeomTransform &transSec2 = fSpecGeometry->getSector(data2->getSector())->getTransform();
  HGeomTransform modTrans2(transMod2);
  modTrans2.transTo(transSec2);

//   modTrans2.print();
  //Double_t layerHalfWidth=TMath::Abs(layerVol->getPoint(0)->getZ());
  
  rLocal.setX(x);
  rLocal.setY(y);
  rLocal.setZ(0.0);  
  r1 = modTrans.transFrom(rLocal);

  rLocal.setX(x2);
  rLocal.setY(y2);
  rLocal.setZ(0.0);
  r2 = modTrans2.transFrom(rLocal);

  dir = r2 -r1;
  dir /= dir.length();

  //Coordinates at z=0
  r.setX( r1.getX() - (dir.getX() / dir.getZ()) * r1.getZ());
  r.setY( r1.getY() - (dir.getY() / dir.getZ()) * r1.getZ());
  r.setZ( 0. );
}

HGeantMdc *HKickDetDigi::findPartner(HGeantMdc *geMdc, Int_t sector,
				     Int_t mod, Int_t layer) {
  HGeantKine *geKine;
  HGeantMdc *gePart;
  geKine=(HGeantKine *)fCatKine->getObject(geMdc->getTrack()-1);
  geKine->resetMdcIter();
  while ( (gePart = (HGeantMdc *)geKine->nextMdcHit()) != 0) {
    if (gePart->getSector() == sector &&
	gePart->getModule() == mod &&
	gePart->getLayer() == layer) return gePart;
  }
  return 0;
}

Bool_t HKickDetDigi::init(void) {
  fMdcSectorLoc.set(1,0);
  fMdcSecModLoc.set(2,0,0);
  if (gHades) {
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *spec=gHades->getSetup();
    HEvent *event=gHades->getCurrentEvent();
    if (rtdb && spec && event) {
      HDetector *mdc=spec->getDetector("Mdc");
      HTofDetector *tof=(HTofDetector *)spec->getDetector("Tof");

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
      if (!fSpecGeometry) {
	fSpecGeometry = (HSpecGeomPar *)spec->createSpecGeomPar();
      }

      if (mdc) {
	fCatKine=event->getCategory(catGeantKine);
	if (!fCatKine) return kFALSE;
	fMdcInput=event->getCategory(catMdcGeantRaw);
	if (!fMdcInput) return kFALSE;
	if ( (fMdcIter=(HIterator *)fMdcInput->MakeIterator()) == 0 ) return kFALSE;
	fMdcOut=event->getCategory(catMdcSeg);
	if (!fMdcOut) {
	  Int_t sizes[]={6,2,30};
	  fMdcOut=new HMatrixCategory("HMdcSimSeg",3,sizes,0.5);
	  if (!event->addCategory(catMdcSeg,fMdcOut,"Mdc"))
	    return kFALSE;
	}

	fMdcHitOut = event->getCategory(catMdcHit);
        if (!fMdcHitOut) {
          fMdcHitOut = mdc->buildCategory(catMdcHit);
          if (!event->addCategory(catMdcHit,fMdcHitOut,"Mdc"))
            return kFALSE;
        }      

	fGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
	if (!fGeometry) {
	  fGeometry=(HMdcGeomPar *)spec->createDetGeomPar("Mdc");
	}
      } else fMdcInput=0;
      if (tof) {
	fTofInput=event->getCategory(catTofGeantRaw);
	if (!fTofInput) return kFALSE;
	if ((fTofIter=fTofInput->MakeIterator()) == 0) return kFALSE;

	if (isFillingTof()) {
	  fTofOut=event->getCategory(catTofHit);
	  if (!fTofOut) {
	    fTofOut=tof->buildMatrixCategory("HTofHitSim2",0.5);
	    if (!fTofOut) return kFALSE;
	    event->addCategory(catTofHit,fTofOut,"Tof");
	  }
	}
      } else fTofInput=0; //fTofInput=0;
    } else return kFALSE; //! (rtdb && spec && event)
  } else return kFALSE; //! (gHades)
  return kTRUE;
}

Bool_t HKickDetDigi::finalize(void) {
  return kTRUE;
}

ClassImp(HKickDetDigi)
