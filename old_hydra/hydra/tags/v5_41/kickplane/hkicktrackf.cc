//*-- Author : M.Sanchez (24.05.2000)
//*-- Modified : M.Sanchez(24.05.2000)
//*-- Copyright : GENP
#include "hkicktrackf.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hkicktrackf.h"
#include "hgeomvector.h"
#include "hgeomtransform.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include "hdetgeompar.h"
#include "htofgeompar.h"
#include "hmdcgeompar.h"
#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hmdcdef.h"
#include "hmatrixcategory.h"
#include "hiterator.h"
#include "hspecgeompar.h"
#include "hkicktrackfpar.h"
#include "hkicktrack.h"
#include "hkickparasciifileio.h"
#include "hparasciifileio.h"
#include "kickdef.h"
#include "showertofinodef.h"
#include "hshowergeometry.h"
#include "tofdef.h"
#include "showerdef.h"
#include "htofinogeompar.h"
#include <TClonesArray.h>
#include <TRandom.h>
#include <TMath.h>
#include <stdio.h>


///////////////////////////////////////////////
// HKickTrackF
//
//  This class is intended to math Mdc segments before and after de
//  magnet associating the matching pairs a momentum The momentum
//  calculation and matching is done using
//a kick plane technique to parametrize the deflection of tracks
//in the HADES inhomogeneous magnet.
//
// FIXME: This class needs to be broken in pieces.
////////////////////////////////////////////////

const Int_t HKickTrackF::kInactiveSector=0;//Sector is not active
const Int_t HKickTrackF::kLowResSector=1; //Sector has not outer chambers

HKickTrackF::HKickTrackF(void) 
  : HKickTask("kick.f","Kick track finder"),fOuterHitArray(1000) {
  //Default constructor.
  fImpacts=new TClonesArray("HKickImpact",20);
  fInput=0;
  fNImpacts=0;
  fHasTof=fHasShower=kFALSE;
  fNTofHits=fNShowerHits=0;
  fEvaluator=0;
  fConservativeMode=kTRUE;
  fSectors.Set(6); //FIXME: Nb of sectors should not be hardcoded.
}

HKickTrackF::HKickTrackF(Text_t name[],Text_t title[]) 
  : HKickTask(name,title),fOuterHitArray(1000) {
  //Default constructor.
  fImpacts=new TClonesArray("HKickImpact",20);
  fInput=0;
  fNImpacts=0;
  fHasTof=fHasShower=kFALSE;
  fNTofHits=fNShowerHits=0;
  fEvaluator=0;
  fConservativeMode=kTRUE;
  fSectors.Set(6); //FIXME: Nb of sectors should not be hardcoded.
}

HKickTrackF::~HKickTrackF(void) {
  //Guess it?
  if (fEvaluator) delete fEvaluator;
  delete fImpacts;
}

Int_t HKickTrackF::execute(void) {
  //Launchs reconstruction
  return execute_mult();
}

Int_t HKickTrackF::execute_mult(void) {
  //This is the brain of this class. It directs the reconstruction for each event
  Int_t r=0;
  Float_t pStep;
  HLocation levArmLoc;

  fSectorLoc.set(1,0);
  levArmLoc.set(2,0,0);

  for (fSectorLoc[0]=0; fSectorLoc[0] < fSectors.GetSize(); fSectorLoc[0]++) {
    if ( fSectors[fSectorLoc[0]] != kInactiveSector ) { 
      fOuterHitArray.Clear();
      levArmLoc[1] = 0;
      levArmLoc[0] = fSectorLoc[0];
      readHits(levArmLoc);

      if (fSectors[fSectorLoc[0]] == kLowResSector) {
  	readMetaHits(fSectorLoc); 
	fCandidates.resize(fNImpacts, fNTotalHits);
	evaluateMetaCandidates(); //Fill fCandidates matrix
      } else {
	Error("execute",
	      "Internal error, please report bug code 1 to M. Sanchez");
      }

      pStep = (fKickPar->getMaxXPull2()-fKickPar->getMaxXPull1()) / 
	fKickPar->getXPullStep();
      pStep = (fKickPar->getPMin2() - fKickPar->getPMin()) / pStep;
    
      for (fMaxXPull = fKickPar->getMaxXPull1(), fMinMomentum=fKickPar->getPMin(); 
	   fMaxXPull < fKickPar->getMaxXPull2();
	   fMaxXPull += fKickPar->getXPullStep(), fMinMomentum += pStep) {
	assignUniqueCombinations();
      }
      if (fConservativeMode) {
	assignRemainingCombinations();
      }

      fImpacts->Clear();
      fNImpacts=0;
    }
  }
  return r; 
}

void HKickTrackF::fillControl(HKickCandidate &candidate, HKickImpact *imp) {
  //Fills control variables, histograms....
  
}


void HKickTrackF::evaluateMetaCandidates(void) {
  // Loops on all identified candidates evaluating each of them. 
  // That is, calculating its associated momentum and other useful variables 
  // for the algorithm
  Int_t dx=fCandidates.getDimX();

  HGeomVector tofR;
  Int_t i,j,k;
  HKickImpact *imp=0;
  HTofHit *tofHit=0;
  HShowerHit *shoHit=0;
  // cout << "ll" << endl;
  for (i=0;i<dx;i++) {
    imp=(HKickImpact *)fImpacts->UncheckedAt(i);
    for (j=0; j<fNTofHits; j++) {
      HKickCandidate &candidate=fCandidates(i,j);
      tofHit = (HTofHit *)fOuterHitArray.UncheckedAt(j);
      fEvaluator->evaluate(imp, tofHit, candidate);
      fillControl(candidate,imp);
      // candidate.Dump();
    }
    for (j=fNTofHits,k=0; j<fNTofHits+fNShowerHits; j++,k++) {
      HKickCandidate &candidate = fCandidates(i,j);
      shoHit = (HShowerHit *)fOuterHitArray.UncheckedAt(j);
      fEvaluator->evaluate(imp, shoHit, candidate); 
      fillControl(candidate,imp);
    }
  }
}

Bool_t HKickTrackF::passesCuts(HKickCandidate &c) {
  //Checks if a given candidate passes all cuts to be considered a good candidate
  Float_t aPhi=TMath::Abs(c.getXPull());
  return ( !c.isAssigned() && 
	   ( aPhi < fMaxXPull ) &&
	   (c.getMomentum() > fMinMomentum) );
}

void HKickTrackF::assignRemainingCombinations(void) {
  int i,j;
  int impacts = fCandidates.getDimX();
  int dy = fCandidates.getDimY();

  for (i=0;i<impacts;i++) { //FIXME: optimize (list of remaining candidates)
    for (j = 0; j < dy; j++) {
      if ( passesCuts(fCandidates(i,j)) ) {
	fillData(fCandidates(i,j),(HKickImpact *)fImpacts->At(i));
      }
    }
  } 
}

void HKickTrackF::assignUniqueCombinations(void) {
  //Match those candidates which pass all cuts and verifying that
  //neither the META hit or the MDC hit are used in other valid candidates. 
  //In case such a conflict is detected, this method tries to find a solution.
  Int_t impacts=fCandidates.getDimX();
  Int_t dy=fCandidates.getDimY();
  Int_t maxMatchs=0,matchsFound;
  Int_t i,j;

  maxMatchs=(impacts < dy)?impacts:dy;
  if (maxMatchs > 0) {
    do {
      matchsFound=0;
      for (i=0;i<impacts;i++) {
	for (j=0;j<dy;j++) {
	  if ( passesCuts(fCandidates(i,j)) ) {
	    if ( isUnique(i,j) ) { 
	      fillData(fCandidates(i,j),(HKickImpact *)fImpacts->At(i));
	      matchsFound++;
	      //maxMatchs--;
	    }
	  }
	}
      }
      if ( matchsFound >= maxMatchs ) matchsFound=0;    
    } while (matchsFound>0);
  }
}

HKickTrack *HKickTrackF::makeTrack(void) {
  TObject *slot = 0;
  HKickTrack *track = 0;

  slot = fCandidateOut->getNewSlot(fSectorLoc);
  if (slot) {
    track = new(slot) HKickTrack;
  } 

  return track;
}

HKickTrack * HKickTrackF::fillData(HKickCandidate &candidate,
				   HKickImpact *imp) {
  //Fills output data in the output category
  HKickTrack *c=0;
  HMdcSeg *seg=0;

  c = makeTrack();
  if (c) {
    seg = imp->getOriginalSegment();
    c->setSector(seg->getSec());
    c->setSegment(seg->getFlag());
    c->setZ(seg->getZ(),seg->getErrZ());
    c->setR(seg->getR(),seg->getErrR());
    c->setTheta(seg->getTheta(),seg->getErrTheta());
    c->setPhi(seg->getPhi(),seg->getErrPhi());
    c->setP(candidate.getMomentum(),candidate.getMomentumError());
    c->setCharge(char(candidate.getPolarity()));
    c->setMass(candidate.getMass(),100.); //FIXME: Do proper error calculation
    c->setQuality(candidate.isAssigned()?1:0);
    c->setPTof(candidate.getPTof());
    c->setErrPTof(candidate.getPTofErr());
    c->setPID(candidate.getPID());
    c->setSystem(candidate.getSystem());
    c->setTof(candidate.getTof());
    c->setBeta(candidate.getBeta());
    c->pull = candidate.getXPull();
    if (candidate.getSystem() == 0) { //Specific stuff for Shower
      HShowerHit *oHit = 0;
      oHit = dynamic_cast<HShowerHit *>(candidate.getOuterHit());
      if (!oHit) {
	Error("fillData","Internal error. Please report bug!");
      } else {
	c->setOuterHitId(fShowerHits->getIndex(oHit)); //FIXME: Optimize
	c->setShowerSum10(oHit->getSum(1) / oHit->getSum(0));
	c->setShowerSum20(oHit->getSum(2) / oHit->getSum(0));
      }
    } else {                          //Specific stuff for TOF
      HTofHit *oHit = 0;
      oHit = dynamic_cast<HTofHit *>(candidate.getOuterHit());
      if (!oHit) {
	Error("fillData","Internal error. Please report bug!");
      } else {
	c->setOuterHitId(oHit->getSector()*1000 + oHit->getModule()*100 + 
			 oHit->getCell());
      }     
    }
  }
  return c;
}



Bool_t HKickTrackF::isUnique(Int_t i,Int_t j) {
  //Checks if a candidate is unique. The mechanism for solving conflicts is implemented
  //here
  Int_t k,l;
  
  for (k=0;k<fCandidates.getDimY(); k++) 
    if (k!=j && passesCuts(fCandidates(i,k)) ) return kFALSE;
  for (k=0;k<fCandidates.getDimX(); k++) 
    if (k!=i && passesCuts(fCandidates(k,j)) ) {
      for (l=0; 
	   l<fCandidates.getDimY() && (!passesCuts(fCandidates(k,l)) || l==j);
	   l++) ;
      if (l==fCandidates.getDimY()) 
	return kFALSE;
    }
  for (k=0;k<fCandidates.getDimX();k++) 
    fCandidates(k,j).markAssigned();
  return kTRUE;
}


HKickCandidateEvaluator *HKickTrackF::makeEvaluator(void) {
  //Creates a candidate evaluator. Factory method
  return new HKickCandidateEvaluator(fKickPlane);
}

Bool_t HKickTrackF::init(void) {
  //Initializes 
  if (gHades) {
    HEvent *event=gHades->getCurrentEvent();
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *spec=gHades->getSetup();
    if (event && rtdb) {
      HDetector *tof=spec->getDetector("Tof");
      HDetector *shower = spec->getDetector("Shower");
      HDetector *mdc = spec->getDetector("Mdc");
      HDetector *tofino = spec->getDetector("Tofino");

      setKickParIo(rtdb);

      if (mdc) { //Has the user set up a MDC?
	
	fInput=event->getCategory(catMdcSeg);
	if (!fInput) {
	  Error("init","No MDC segment category defined");
	  return kFALSE;
	}
	else fIter=(HIterator *)fInput->MakeIterator();

	for (int i=0;i<fSectors.GetSize();i++) { //Sector configuration
	  if (mdc->isSectorActive(i)) {
	    fSectors[i] = kLowResSector; //By default
	  } else {
	    fSectors[i] = kInactiveSector;
	  }
	}
	fMdcGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
	if (!fMdcGeometry) {
	  fMdcGeometry=(HMdcGeomPar *)spec->createDetGeomPar("Mdc");
	}

	fKickPlane = HKickPlane::getMeta();

	if (!fEvaluator) fEvaluator=makeEvaluator();
	fEvaluator->setMdcGeometry(fMdcGeometry);
	fKickPar=(HKickTrackFPar *)rtdb->getContainer("KickTrackFPar");
	if (!fKickPar) {
	  fKickPar = new HKickTrackFPar;
	  rtdb->addContainer(fKickPar);
	}
      } else {
	Error("init","No MDC detector defined");
	return kFALSE;
      }

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
      if (!fSpecGeometry) {
	fSpecGeometry = (HSpecGeomPar *)spec->createSpecGeomPar();
      }
      fEvaluator->setSpecGeometry(fSpecGeometry);
      if (tof) { //Has the user set up a TOF?
	fTofHits=event->getCategory(catTofHit);
	if (!fTofHits) {
	  fTofHits=tof->buildCategory(catTofHit);
	  if (!fTofHits) {
	    Error("init","No TOF input");
	    return kFALSE;
	  }
	}
	fTofIter=(HIterator *)fTofHits->MakeIterator();
	fTofGeometry=(HTofGeomPar *)rtdb->getContainer("TofGeomPar");
	if (!fTofGeometry) {
	  fTofGeometry=(HTofGeomPar *)spec->createDetGeomPar("Tof");
	}
	fEvaluator->setTofGeometry(fTofGeometry);
	fHasTof=kTRUE; //TOF found!
      }
            

      if (shower) { //Has the user set up a Shower?

	if (tofino) { //Require Tofino only if there is Shower.
	  fTofinoGeometry = (HTofinoGeomPar *)rtdb->getContainer("TofinoGeomPar");
	  if (!fTofinoGeometry) {
	    fTofinoGeometry = (HTofinoGeomPar *)spec->createDetGeomPar("Tofino");
	  }
	  fEvaluator->setTofinoGeometry(fTofinoGeometry);
	} else {
	  Warning("init","The kickplane requires the Tofino detector to be defined");
	  return kFALSE;
	}

	fShowerGeometry = (HShowerGeometry *)rtdb->getContainer("ShowerGeometry");
	if (!fShowerGeometry) {
	  fShowerGeometry = new HShowerGeometry;
	  if (!fShowerGeometry) {
	    Error ("init","Unable to create Shower Geometry container");
	    return kFALSE;
	  }
	  rtdb->addContainer(fShowerGeometry);
	}
	fEvaluator->setShowerGeometry(fShowerGeometry);
	fShowerHits=event->getCategory(catShowerHitTof);
	if (!fShowerHits) {
	  //fShowerHits = shower->buildCategory(catShowerHitTof);
	  if (!fShowerHits) {
	    Error("init","No Shower input available");
	    return kFALSE;
	  }
	}
	fShowerIter = (HIterator *)fShowerHits->MakeIterator();
	fHasShower=kTRUE; //Shower found!
      }

      //Setup output
      fCandidateOut=event->getCategory(catTrack);
      if (!fCandidateOut) {
	Int_t ssize[2]={6,100};
	fCandidateOut = new HMatrixCategory("HKickTrack",2,ssize,0.5);
	if (fCandidateOut) event->addCategory(catKickTrack,
					      fCandidateOut,
					      "Tracks");
	else {
	  Error("init","Unable to setup output");
	  return kFALSE;
	}
      }
      //!Setup output
    } else {
      Error ("init","! (event && rtdb)");
      return kFALSE; //! (event && rtdb)
    }
  } else {
    Error ("init","! (gHades)");
    return kFALSE; //! (gHades)		
  }

  return kTRUE;
}

Bool_t HKickTrackF::reinit(void) {
  fEvaluator->setTofXResolution(fKickPar->getTofXResolution());
  return kTRUE;
}

Bool_t HKickTrackF::finalize(void) {
  //Finalization code
  return kTRUE;
}

void HKickTrackF::clearImpactList(void) {
  fImpacts->Clear();
}

void HKickTrackF::readMetaHits(HLocation &loc) {
  //Reads hits from MDC and TOF detectors
  HTofHit *tofHit=0;
  HShowerHit *shoHit=0;
  Int_t i=0;
  if (hasTof()) {
    fTofIter->Reset(); //FIXME: Remove ??
    fTofIter->gotoLocation(loc);
    i=0;
    while ( (tofHit=(HTofHit *)fTofIter->Next()) != 0) {
      if (!hasShower() || tofHit->getModule() < 8) { //Outer TOF
	if (tofHit->getTof() > fKickPar->getMinTof() && 
	    tofHit->getTof() < fKickPar->getMaxTof() ) {
	  fOuterHitArray.AddAtAndExpand(tofHit,i);
	  i++;
	}
      }
    }
    fNTofHits=i;
  } else fNTofHits=0;
  if (hasShower()) {
    fShowerIter->Reset(); //FIXME: remove ??
    while ( (shoHit=(HShowerHit *)fShowerIter->Next()) != 0) {
      if (shoHit->getSector()==fSectorLoc[0] && shoHit->getModule()==0) {
	fOuterHitArray.AddAtAndExpand(shoHit,i);
	i++;
      }
    }
    fNShowerHits=i - fNTofHits;
  } else fNShowerHits=0;  
  fNTotalHits=fNTofHits + fNShowerHits;
}

void HKickTrackF::readHits(HLocation &loc) {
  //Loops on the input category reading objects and adding objects to
  //the list of candidates fImpacts.
  HMdcSeg *hit=0;
  HGeomVector r;
  HKickImpact *imp=0;
  Int_t i=0;
  Int_t mdcSector=0;
  fIter->Reset();
  fIter->gotoLocation(loc);
  while ( (hit=(HMdcSeg *)fIter->Next()) != 0 ) {
    hit->setFlag(i);
    i++;
    mdcSector=hit->getSec();
    imp=new( (*fImpacts)[fNImpacts++] ) HKickImpact;
    transform(hit,r,imp->direction());
    fKickPlane->calcIntersection(r,imp->direction(),imp->position());
    imp->setSector(mdcSector);
    imp->setOriginalSegment(hit);
  }
}

void HKickTrackF::transform(HMdcSeg *hit,
			    HGeomVector &r,HGeomVector &alpha) {
  //Calculates position and direction vector for a HMdcHit
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;

  theta=hit->getTheta();
  phi=hit->getPhi();
  r.setZ(hit->getZ());
  r.setX(hit->getR()*TMath::Cos(phi+pi2));
  r.setY(hit->getR()*TMath::Sin(phi+pi2));
  alpha.setZ(TMath::Cos(theta));
  alpha.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alpha.setX(TMath::Sin(theta)*TMath::Cos(phi));
}

//------------------------- HKickCandidateMatrix --------------------
HKickCandidateMatrix::~HKickCandidateMatrix(void) {
  if (fData) { delete[] fData; fData=0; }
}

void HKickCandidateMatrix::resize(Int_t dx,Int_t dy) {
  //Resizes the candidate matrix to dx x dy
  if (fData) { delete[] fData; fData=0; }
  fData=new HKickCandidate[dx*dy];
  fDimX=dx; fDimY=dy;
  fSize=dx*dy;
}

static HKickCandidate gNullCandidate;

HKickCandidate &HKickCandidateMatrix::operator()(Int_t i,Int_t j) {
  //Gives back the candidate in position i,j of the matrix
  Int_t linAddr=i*fDimY+j;
  if (fData) {
    return fData[ (linAddr<fSize) ? linAddr : 0];
  } // else Warning("operator()","No data set");
  return gNullCandidate;
}

ostream &HKickCandidateMatrix::print(ostream &out) {
  //Prints the candidate matrix to the stream "out" for inspection.
  for (int i=0; i<fDimX; i++) {
    for (int j=0; j<fDimY; j++) {
      out << fData[i*fDimY+j].getMomentum() << "/";
      out << fData[i*fDimY+j].getXPull() << "\t";
    }
    out << endl;
  }
  return out;
}

ClassImp(HKickTrackF)

