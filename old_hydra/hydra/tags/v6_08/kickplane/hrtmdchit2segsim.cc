//*-- Author : M.Sanchez (15.09.2000)
#include "hrtmdchit2segsim.h"
#include "hmdcdetector.h"
#include "hdetector.h"
#include "hades.h"
#include "haddef.h"
#include "hmdcdef.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hlocation.h"
#include "hmdcgeompar.h"
#include "hevent.h"
#include "hspecgeompar.h"
#include "hgeomvolume.h"
#include "hmdcsimhit.h"

//////////////////////////////////////////////////////////
//HRtMdcHit2SegSim
//
//  This class converts 1 MDC Hit into 1 MDC Segment
//
///////////////////////////////////////////////////////////

HRtMdcHit2SegSim::HRtMdcHit2SegSim(Text_t name[],Text_t title[]) :
  HReconstructor(name,title) {
  fInput=0;
  fOutput=0;
  fMdcGeo=0;
  fInputIter=0;
  fUseTarget = kFALSE;
  fModule = 1;
}

HRtMdcHit2SegSim::~HRtMdcHit2SegSim(void) {
  if (fInputIter) delete fInputIter;
}

Int_t HRtMdcHit2SegSim::execute(void) {
  HMdcSimSeg *seg=0;
  TObject *slot=0;
  UChar_t nTimes=6;
  Int_t geTrack;
  HGeomVector r,alpha;

  fLoc[0]=0;
  fLoc[1]=0;
  HMdcSimHit *hit;
  fInputIter->Reset();

  while ( (hit = (HMdcSimHit *)fInputIter->Next()) != 0) {
    fLoc[0]=hit->getSector();
    fLoc[1]=hit->getModule() / 2;
    if (hit->getModule() == fModule) {
      slot = fOutput->getNewSlot(fLoc);
      if (slot) {
	seg = new(slot) HMdcSimSeg;
	transform(hit,r,alpha);
	fillData(seg,r,alpha);
	geTrack = hit->getTrack();
	seg->setNTracks(1,&geTrack,&nTimes);
	seg->setMomentum(hit->getMomentum());
     }
    }
  }
  return 0;
}

void HRtMdcHit2SegSim::transform(HMdcHit *hit, HGeomVector &r,
			    HGeomVector &alpha) {
  HGeomVector rLocal,alphaLocal;
  HModGeomPar *mod = fMdcGeo->getModule(hit->getSector(),hit->getModule());

  if (mod) {
    HGeomTransform &transMod = mod->getLabTransform();
    HGeomTransform &transSec = fSpecGeometry->getSector(hit->getSector())->getTransform();
    HGeomTransform trans(transMod);
    const HGeomVector &targetPos = fSpecGeometry->getTarget(0)->getTransform().getTransVector();
    trans.transTo(transSec);

    rLocal.setX(hit->getX());
    rLocal.setY(hit->getY());
    rLocal.setZ(0.);
    alphaLocal.setX(hit->getXDir());
    alphaLocal.setY(hit->getYDir());
    alphaLocal.setZ( TMath::Sqrt(1. - hit->getXDir()*hit->getXDir() -
				 hit->getYDir()*hit->getYDir()) );

    r = trans.transFrom(rLocal);
    if (usingTarget()) {
      alpha = r - targetPos;
      alpha/= alpha.length(); 
    } else {
      alpha = trans.getRotMatrix() * alphaLocal;
    }

    // Going to z=0
    r.setX( r.getX() - (alpha.getX() / alpha.getZ()) * r.getZ() );
    r.setY( r.getY() - (alpha.getY() / alpha.getZ()) * r.getZ() );
    r.setZ( 0. );
  } else Warning("transform","Module %i unknown",hit->getModule());
}

void HRtMdcHit2SegSim::fillData(HMdcSimSeg *mdcSeg,HGeomVector &r,
			   HGeomVector &alpha) {
  Float_t theta,phi;

  mdcSeg->setSec(fLoc[0]);
  mdcSeg->setIOSeg(fLoc[1]);
  theta=TMath::ACos(alpha.getZ());
  phi=TMath::ATan2(alpha.getY(),alpha.getX());
  mdcSeg->setR(r.getY()*TMath::Cos(phi) - r.getX()*TMath::Sin(phi),0.1);
  mdcSeg->setZ(-(r.getY()*TMath::Sin(phi) + r.getX()*TMath::Cos(phi))*
	       alpha.getZ()/TMath::Sin(theta),0.1);
  mdcSeg->setThetaPhi(theta,0.1,
		      phi,0.1);
}

Bool_t HRtMdcHit2SegSim::init(void) {
  Bool_t r=kTRUE;
  if (gHades) {
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *setup=gHades->getSetup();
    if (setup && rtdb) {
      HDetector *mdc=setup->getDetector("Mdc");
      if (mdc) {
	HEvent *event=gHades->getCurrentEvent();
	if (event) {  //Real code goes here
	  fInput=event->getCategory(catMdcHit);
	  if (!fInput) {
	    fInput=mdc->buildCategory(catMdcHit);
	    event->addCategory(catMdcHit,fInput,"Mdc");
	  }
	  fInputIter=(HIterator *)fInput->MakeIterator();
	  fOutput=event->getCategory(catMdcSeg);
	  if (!fOutput) {
	    fOutput=mdc->buildCategory(catMdcSeg);
	    event->addCategory(catMdcSeg,fOutput,"Mdc");
	  }
	  fMdcGeo=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
	  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

	} else {
	  Warning("init","Non existing event structure");
	  r=kFALSE;
	}
      } else {
	Warning("init","MDC detector not defined");
	r=kFALSE;
      }
    } else {
      Warning("init","Spectrometer or rtdb not defined");
      r=kFALSE;
    }
  } else {
    Warning("init","HYDRA not initialized");
    r=kFALSE;
  }
  fLoc.set(2,0,0);
  return kTRUE;
}

Bool_t HRtMdcHit2SegSim::finalize(void) {
  return kTRUE;
}


ClassImp(HRtMdcHit2SegSim)
