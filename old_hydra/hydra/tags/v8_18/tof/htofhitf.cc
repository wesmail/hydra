#include "htofhitf.h"
#include "hades.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofhitsim.h"
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
#include "hspecgeompar.h"
#include "hstarthit.h"
#include "hstartdef.h"
#include <TMath.h>

//*-- Author :
//*-- Modified : 14/11/2004 by P. Tlusty -
//*   tof hit lab position now related to [0,0,0]
//*-- Modified : 23/10/2002 by M. Sanchez
//*-- Modified : 09/05/2002 by D. Zovinec
//*-- Modified : 14/03/2002 by D. Zovinec
//*-- Modified : 21/09/2001 by D. Zovinec
//*-- Modified : 24/04/2001 by M. Sanchez
//*-- Modified : 08/03/2001 by M. Sanchez
//*-- Modified : 27/03/2000 by I. Koenig
//*-- Modified : 17/03/2000 by R. Holzmann
//*-- Modified : 30/11/99 by M. Sanchez
//*-- Modified : 02/11/99 by D. Vasiliev
//*-- Modified : 15/06/98 by M. Sanchez


//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
// HTofHitF Tof hit finder
//
// adapted from /u/halo/packages/abase/new by Manuel Sanchez (17/06/98)
//
// HTofHitF is the reconstructor class that iterates over RAW data
// and finds the TOF hits.
// The HIT is only created and filled if both left and right TDC time
// is available.
//
// Left (right) TDC time is used to find xpos and tof of hit (see HTofHit).
// Then HIT coordinates in the LAB, phi, theta and distance from the
// target are calculated by using HTofGeomPar.
//
// Left (right) ADC amplitude is used to find xposAdc and energy
// deposited in the rod . Additional information amplL and amplR
// is calculated (see HTofHit).
// The flagAdc have been added to distinguish levels of ADC info
// available:  no ADC         - flagAdc = 0
//             left ADC only  - flagAdc = -1
//             right ADC only - flagAdc = 1
//             both ADC's     - flagAdc = 2
// Important!!!: Note that if only one ADC is available (case flagAdc = -1 or 1)
// the xposAdc cannot be calculated. Also amplL (amplR) cannot be found
// when left (right) ADC has not provided the signal.
//
// Empirical dependence of tof on position along the rod included
// (see talk of P.Tlusty in Coll.Meeting XI at Smolenice, TOF detector performance.)
//
// Time walk correction is included in the tof reconstruction.
//
/////////////////////////////////////////////////////////////////////////

void HTofHitF::initParContainer(HSpectrometer *spec, HRuntimeDb *rtdb) {
  // Adds to the runtime database "rtdb" the containers needed by the Hit Finder
  //spec is used to get information of the spectrometer setup.
  fCalPar=(HTofCalPar *)rtdb->getContainer("TofCalPar");
  fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");
  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
}

HTofHitF::HTofHitF(void) {
  fCalPar = NULL;
  fCalParSim = NULL;
  fRawCat = fHitCat = NULL;
  fRawCatTmp = fHitCatTmp = NULL;
  fStartHitCat = NULL;
  fTofSimulation = kFALSE;
  fLoc.set(3,0,0,0);
  iter = NULL;
  iterTmp = NULL;
}

HTofHitF::HTofHitF(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fCalPar = NULL;
  fCalParSim = NULL;
  fRawCat = fHitCat = NULL;
  fRawCatTmp = fHitCatTmp = NULL;
  fStartHitCat = NULL;
  fTofSimulation = kFALSE;
  fLoc.set(3,0,0,0);
  iter = NULL;
  iterTmp = NULL;
}

HTofHitF::~HTofHitF(void) {
  if(iter) delete iter;
}

HTofHit *HTofHitF::makeHit(TObject *address) {
  return new(address) HTofHit;
}

void HTofHitF::fillHit(HTofHit *hit, HTofRaw *raw) {
}

Bool_t HTofHitF::init(void) {
  // Initializes data levels and container parameters for operation of the
  //hit finder
  Bool_t r=kTRUE; // return value
  HSpectrometer *spec = gHades->getSetup();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  HEvent *ev = gHades->getCurrentEvent(); // Event structure
  printf("initialization of Tof hit finder\n");

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
      if (!fStartHitCat) Warning("init","Start hit level not defined; setting start time to 0");
    } else {
      Warning("init","Start detector not found; setting start time to 0");
      fStartHitCat = 0;
    }
  } else {
    Error("init","Setup, RuntimeDb or event structure not found");
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
    //---------------------------------------------
    // read catTofRaw and create hits in catTofHit
    Bool_t sim=kFALSE;
    if      ( fTofSimulation) sim=kTRUE;
    else if (!fTofSimulation) sim=kFALSE;
    fillHitCat(sim,kFALSE);
    //---------------------------------------------

    //---------------------------------------------
    // In embedding mode one has to
    // read catTofRawTmp and create hits in catTofHitTmp
    // in addition. Embedded Hits will be merged on hitlevel
    // later
    if(fTofSimulation&&gHades->getEmbeddingMode()>0){
	fillHitCat(kTRUE,kTRUE);
        mergeHitCats();
    }
    //---------------------------------------------

#if DEBUG_LEVEL>2
    gDebuger->leaveFunc("HTofHitF::execute");
#endif
    return 0;
}

void HTofHitF::fillGeometry(HTofHit *hit) {
  // Fills in the LAB position parameters for the given HTofHit.
  //
  // This is done by transforming the point (x,0,0) in the ROD coordinate
  // system to the LAB coordinate system. Where x is the reconstructed
  // x position inside the hit.

  HGeomVector rLocal,r;
  Float_t d,phi,theta;
  Float_t rad2deg = 180./TMath::Pi();

  HModGeomPar *module=fTofGeometry->getModule(hit->getSector(),hit->getModule());
  HGeomTransform &trans = module->getLabTransform();
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(hit->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();

  // Fill r with the hit coordinates in the ROD coordinate system
  // Since we do not have information about y,z coordinates of impact
  // y=0, z=0 is used. Note that (0,0,0) corresponds to the rod center.
  r.setX(hit->getXpos());
  r.setY(0.);
  r.setZ(0.);

  rLocal=rodTrans.transFrom(r);  // transform to module coordinate system
  r=trans.transFrom(rLocal);     // transform from module to LAB system


  HGeomVolume *tv=fSpecGeometry->getTarget(0);
  if (tv) r -= tv->getTransform().getTransVector();   // correct for target position

  // Fill output
  d = r.length();
  theta = (d>0.) ? (rad2deg * TMath::ACos(r.getZ() / d)) : 0.;
  phi = rad2deg * TMath::ATan2( r.getY(), r.getX());
  if (phi < 0.) phi += 360.;

  if (tv) r += tv->getTransform().getTransVector();   // correct for target position

  hit->setXYZLab(r.getX(), r.getY(), r.getZ());
  hit->setDistance( d );
  hit->setTheta(theta);
  hit->setPhi(phi);
}

void HTofHitF::fillHitCat(Bool_t sim,Bool_t embed)
{

    Float_t atofCorr = 0.000000276; // empirical correction of atof dependence on axpos

    HTofRaw *raw=NULL;
    HTofHit *hit=NULL;
    HStartHit *sH=NULL;

    Float_t atof;
    Float_t axpos;
    Float_t startTime=0.0;
    Float_t subCl;      // raw left charge subtracted by left ADC pedestal
    Float_t subCr;      // raw right charge subtracted by right ADC pedestal
    Float_t slopeAdcL;   // left ADC slope
    Float_t slopeAdcR;  // right ADC slope
    Float_t xposAdc;
    Float_t depE;
    Float_t leftA;
    Float_t rightA;
    Float_t twalk;
    Float_t twoffT;
    Int_t flagadc;

    //--------------------------------------------------
    // start time extraction. Only for real data!
    if(!sim)
    {
	if (fStartHitCat) {
	    if((sH=(HStartHit *)fStartHitCat->getObject(0))!=NULL){
		startTime =sH->getTime();
	    }
	    else{
		startTime = 0.0;
	    }
	} else {
	    startTime = 0.0;
	}
    } else {
	startTime=0.0;
    }
    //--------------------------------------------------

    HIterator*  iterLocal;
    HCategory*  catHitLocal;
    HTofCalPar* calparLocal;

    //--------------------------------------------------
    // switch data pointer depending if running for
    // sim or real data!
    if(sim&&embed)  iterLocal=iterTmp;
    else            iterLocal=iter;

    if(sim&&embed)  catHitLocal=fHitCatTmp;
    else            catHitLocal=fHitCat;

    if(sim&&embed)  calparLocal=fCalParSim;
    else            calparLocal=fCalPar;
    //--------------------------------------------------


    iterLocal->Reset();
    while ( (raw=(HTofRaw *)iterLocal->Next())!=NULL) {
	fLoc[0]=raw->getSector();
	fLoc[1]=raw->getModule();
	fLoc[2]=raw->getCell();

	// Hit level is only filled if both left and right time are set
	if(raw->getLeftTime() && raw->getRightTime()){
	    hit = (HTofHit *)catHitLocal->getSlot(fLoc);
	    if (hit) {
		hit=makeHit(hit);
		HTofCalParCell& cell=(*calparLocal)[fLoc[0]][fLoc[1]][fLoc[2]];

		// The TDC's data is elaborated here.
		atof = (raw->getLeftTime() * cell.getLeftK() +
			raw->getRightTime()*cell.getRightK())/2.0 - cell.getTimK();

		//Substract start time. Zero if no start hit level is filled
		atof -= startTime;

		axpos = cell.getVGroup()*(raw->getRightTime() * cell.getRightK() -
					  raw->getLeftTime()*cell.getLeftK())/2.0 +cell.getPosK();

		// Empirical correction of time of flight
		if(!sim){
		    atof = atof + (axpos*axpos*atofCorr);
		}
		// The ADC's data is elaborated here.
		xposAdc=0.0;
		depE=0.0;
		leftA=0.0;
		rightA=0.0;
		flagadc=0;
		twoffT=cell.getTimeWalkC3();
		twalk=twoffT;

		// If at least one ADC signal then preliminary calculations.
		if(raw->getLeftCharge() || raw->getRightCharge()) {
		    subCl = (raw->getLeftCharge() - cell.getPedestalL());
		    subCr = (raw->getRightCharge() - cell.getPedestalR());
		    slopeAdcL = (cell.getEdepK())*(TMath::Exp((cell.getGainAsym())/(cell.getAttLen())));
		    slopeAdcR = (cell.getEdepK())*(TMath::Exp(-(cell.getGainAsym())/(cell.getAttLen())));

		    // Individual cases.
		    if(raw->getLeftCharge()){
			flagadc=-1;
			depE=(subCl*cell.getEdepK())*(TMath::Exp((cell.getGainAsym()-axpos)/(cell.getAttLen())));
			leftA=subCl*slopeAdcL;
			if(leftA>0.0) twalk=twalk+(cell.getTimeWalkC1()/(TMath::Sqrt(leftA)))-(twoffT/2.0); // time walk correction (left ADC)
		    }
		    if(raw->getRightCharge()){
			flagadc=1;
			depE=(subCr*cell.getEdepK())*(TMath::Exp((axpos-cell.getGainAsym())/(cell.getAttLen())));
			rightA=subCr*slopeAdcR;
			if(rightA>0.0) twalk=twalk+(cell.getTimeWalkC2()/(TMath::Sqrt(rightA)))-(twoffT/2.0); // time walk correction (right ADC)
		    }
		    if(raw->getLeftCharge() && raw->getRightCharge()) {
			flagadc=2;
			xposAdc=(cell.getAttLen()/2.0)*(TMath::Log(subCl/subCr)) + cell.getGainAsym();
			depE=(cell.getEdepK())*(TMath::Sqrt(subCl*subCr));
		    }
		}

		// Time walk correction.
		if(!sim){
		    atof = atof + twalk;
		}
		hit->setSector((Char_t) fLoc[0]);
		hit->setModule((Char_t) fLoc[1]);
		hit->setCell((Char_t) fLoc[2]);
		hit->setTof(atof);
		hit->setXpos(axpos);
		hit->setXposAdc(xposAdc);
		hit->setEdep(depE);
		hit->setLeftAmp(leftA);
		hit->setRightAmp(rightA);
		hit->setAdcFlag(flagadc);
		fillHit(hit,raw);

		fillGeometry(hit);
	    }
	}
    }
}
void HTofHitF::mergeHitCats()
{
    //--------------------------------------------------
    HTofHitSim* hit;
    HTofHitSim* hittmp;

    TIterator* hititer=fHitCatTmp->MakeIterator("native");
    hititer->Reset();

    while ( (hittmp=(HTofHitSim *)hititer->Next())!=NULL)
    {
	fLoc[0]=hittmp->getSector();
	fLoc[1]=hittmp->getModule();
	fLoc[2]=hittmp->getCell();
	hit= (HTofHitSim*) fHitCat->getObject(fLoc);

	if(hit)
	{   // real hit is existing already
	    // merging should be done here. At the moment
	    // sim hits will be always transported
	    new (hit) HTofHitSim(*hittmp);
	}
	else
	{   // cell was not fired by real hit before
	    hit= (HTofHitSim*) fHitCat->getSlot(fLoc);
	    if(hit)
	    {
		new (hit)HTofHitSim(*hittmp);
	    }
	    else
	    {
		Error("mergeHitCats()","Could not retrieve slot in catTofHit!");
	    }
	}

    }

    delete hititer;
}

ClassImp(HTofHitF)














