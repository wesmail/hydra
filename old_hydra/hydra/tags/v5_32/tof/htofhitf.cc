#include "htofhitf.h"
#include "hades.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofcalpar.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"

#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hgeomtransform.h"
#include "htofgeompar.h"
#include "hdetgeompar.h"
#include "hgeomvector.h"
#include "hstarthit.h"
#include "hstartdef.h"
#include <TMath.h>

//*-- Author : 
//*-- Modified : 24/04/2001 by M. Sanchez
//*-- Modified : 08/03/2001 by M. Sanchez
//*-- Modified : 27/03/2000 by I. Koenig
//*-- Modified : 17/03/2000 by R. Holzmann
//*-- Modified : 30/11/99 by M. Sanchez
//*-- Modified : 02/11/99 by D. Vasiliev
//*-- Modified : 15/06/98 by M. Sanchez

/////////////////////////////////////////////////////////////////////////
// HTofHitF Tof hit finder
//
// adapted from /u/halo/packages/abase/new by Manuel Sanchez (17/06/98)
/////////////////////////////////////////////////////////////////////////

void HTofHitF::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the Hit Finder
  //spec is used to get information of the spectrometer setup.
  fCalPar=(HTofCalPar *)rtdb->getContainer("TofCalPar");
  if (!fCalPar) {
    fCalPar=new HTofCalPar;
    rtdb->addContainer(fCalPar);
  }
  fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");
  if (!fTofGeometry) {
    fTofGeometry=(HTofGeomPar *)spec->createDetGeomPar("Tof");
  }
}

HTofHitF::HTofHitF(void) {
  fCalPar = NULL;
  fRawCat = fHitCat = NULL;
  fLoc.set(3,0,0,0);
  iter = NULL;
}

HTofHitF::HTofHitF(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fCalPar = NULL;
  fRawCat = fHitCat = NULL;
  fLoc.set(3,0,0,0);
  iter = NULL;
}

HTofHitF::~HTofHitF(void) {
  if(iter) delete iter;
}

Bool_t HTofHitF::init(void) {
  // Initializes data levels and container parameters for operation of the
  //hit finder
  Bool_t r=kTRUE; // Function's return value
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HEvent *ev = gHades->getCurrentEvent(); // Event structure

  if (spec && rtdb && ev) {
    HDetector *tof = spec->getDetector("Tof");
    HDetector *start = spec->getDetector("Start");

    if (tof) {
      // The real work starts here
      initParContainer(spec,rtdb);

      fRawCat = ev->getCategory(catTofRaw);
      if (!fRawCat) {
	fRawCat= tof->buildCategory(catTofRaw);
	if (!fRawCat) return kFALSE;
	else ev->addCategory(catTofRaw,fRawCat,"Tof");
      }
      iter=(HIterator*)fRawCat->MakeIterator("native");
      
      fHitCat = ev->getCategory(catTofHit);
      if (!fHitCat) {
	fHitCat = spec->getDetector("Tof")->buildCategory(catTofHit);
	if (!fHitCat) return kFALSE;
	else ev->addCategory(catTofHit,fHitCat,"Tof");
      }

    } else {
      Error("init","TOF setup is not defined");
      r = kFALSE; // Notify error to task manager
    }

    // Get Start Hit category. If StartHit is not defined or the start detector
    //is not in the setup a Warning is displayed and fStartHitCat is set to 0
    if (start) {
      fStartHitCat = ev->getCategory(catStartHit);
      if (!fStartHitCat) 
	Warning("init","Start Hit level not defining: setting start time to 0");
    } else {
      Warning("init","Start detector not found. Setting start time to 0");
      fStartHitCat = 0;
    } 
    
  } else {
    Error("init","Setup, RuntimeDb or Event structure not found");
    r = kFALSE; // Notify error to task manager
  }
  
  return r;
}

Int_t HTofHitF::execute(void) {
  // Find the hits. 
  //See also HReconstructor::execute
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HTofHitF::execute");
#endif

  HTofRaw *raw=NULL;
  HTofHit *hit=NULL;

  Float_t atof;
  Float_t axpos;
  Float_t xtof;
  Float_t startTime;
  Int_t ytof;
  Int_t multTof;

  multTof=0;
  xtof = 0.0;
  ytof = 0;

  if (fStartHitCat) {
    startTime = ((HStartHit *)fStartHitCat->getObject(0))->getTime();
  } else {
    startTime = 0.0;
  }

  iter->Reset();

  while ( (raw=(HTofRaw *)iter->Next())!=NULL) {

    fLoc[0]=raw->getSector();
    fLoc[1]=raw->getModule();
    fLoc[2]=raw->getCell();

    // Hit level is only filled if both left and right time are set
    if(raw->getLeftTime() && raw->getRightTime()) {
      hit = (HTofHit *)fHitCat->getSlot(fLoc);

      if (hit) {
	hit=new(hit) HTofHit;
	
        HTofCalParCell& cell=(*fCalPar)[fLoc[0]][fLoc[1]][fLoc[2]];
	
	atof = (raw->getLeftTime() * cell.getLeftK() +
      	        raw->getRightTime()*cell.getRightK())/2.0 - cell.getTimK();
	
 	//Substract start time. Zero if no start hit level is filled
	atof -= startTime;

	axpos = cell.getVGroup()*(raw->getLeftTime() * cell.getLeftK() -
		raw->getRightTime()*cell.getRightK())/2.0 -cell.getPosK();

	multTof++;
	xtof = -axpos;   // x axis is inverted
	ytof = 8*fLoc[1]+fLoc[2];

	hit->setTof(atof);
	hit->setXpos(xtof);
	hit->setSector((Char_t) fLoc[0]);
	hit->setModule((Char_t) fLoc[1]);
	hit->setCell((Char_t) fLoc[2]);
	

	fillGeometry(hit);

      }
    }
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HTofHitF::execute");
#endif
  return 0;
}


void HTofHitF::fillGeometry(HTofHit *hit) {
  // Fills in the LAB position parameters for the given
  //HTofHit.
  //
  // This is done by transforming the point (x,0,0) in the ROD
  //coordinate system to the LAB coordinate system. Where x is the reconstructed
  //x position inside the hit.

  HGeomVector rLocal,r;
  Float_t d,phi,theta;
  Float_t rad2deg = 180. / TMath::Pi();

  HModGeomPar *module=fTofGeometry->getModule(hit->getSector(), 
					      hit->getModule());
  HGeomTransform &trans = module->getLabTransform();

  HGeomVolume *rodVol=module->getRefVolume()->getComponent(hit->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();

  // Fill r with the hit coordinates in the ROD coordinate system
  // Since we do not have information about y,z coordinates of impact 
  // y=0, z=0 is used. Note that (0,0,0) corresponds to the rod center.
  r.setX(hit->getXpos());
  r.setY(0.);
  r.setZ(0.);
  
  rLocal=rodTrans.transFrom(r); //Transform to modue coordinate system
  r=trans.transFrom(rLocal); //Transform from Module to LAB system

  // Fill output
  d = r.length();
  theta = (d>0.) ? (rad2deg * TMath::ACos(r.getZ() / d)) : 0.;
  phi = rad2deg * TMath::ATan2( r.getY(), r.getX());
  if (phi < 0.) phi += 360.;

  hit->setXYZLab(r.getX(), r.getY(), r.getZ());
  hit->setDistance( d );
  hit->setTheta(theta);
  hit->setPhi(phi);
}

ClassImp(HTofHitF)














