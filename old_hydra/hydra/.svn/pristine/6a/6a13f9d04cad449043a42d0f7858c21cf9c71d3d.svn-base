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
#include "tofdef.h"
#include "showerdef.h"
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

//TClonesArray *HKickTrackF::createImpactList(void) {
//  return new TClonesArray("HKickImpact",20);
//}

const Int_t HKickTrackF::kInactiveSector=0;//Sector is not active
const Int_t HKickTrackF::kLowResSector=1; //Sector has not outer chambers
const Int_t HKickTrackF::kMidResSector=2; //Only one outer chamber is available

HKickTrackF::HKickTrackF(Text_t name[],Text_t title[]) 
  : HReconstructor(name,title),fOuterHitArray(1000) {
  //Default constructor.
  fImpacts=new TClonesArray("HKickImpact",20);
  fInput=0;
  fNImpacts=0;
  fHasTof=fHasShower=kFALSE;
  fNTofHits=fNShowerHits=0;
  fEvaluator=0;
  fConservativeMode=kFALSE;
}

HKickTrackF::~HKickTrackF(void) {
  //Guess it?

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
      levArmLoc[1] = 0;
      levArmLoc[0] = fSectorLoc[0];
      readHits(levArmLoc);

      if (fSectors[fSectorLoc[0]] == kLowResSector) {
  	readMetaHits(fSectorLoc); 
	fCandidates.resize(fNImpacts, fNTotalHits);
	evaluateMetaCandidates(); //Fill fCandidates matrix
      } else if ( fSectors[fSectorLoc[0]] == kMidResSector) {
	levArmLoc[1] = 0;
	readOuterHits(levArmLoc);
	fCandidates.resize(fNImpacts, fNTotalHits);
	evaluateOuterCandidates(); //Fill fCandidates matrix
      } else {
	Error("execute","Internal error, please report this bug. (code 1)");
      }
      
      pStep = (fKickPar->getMaxXPull2()-fKickPar->getMaxXPull1()) / 
	fKickPar->getXPullStep();
      pStep = (fKickPar->getPMin2() - fKickPar->getPMin()) / pStep;
    
      for (fMaxXPull[0] = fKickPar->getMaxXPull1(), fMinMomentum=fKickPar->getPMin(); 
	   fMaxXPull[0] < fKickPar->getMaxXPull2();
	   fMaxXPull[0] += fKickPar->getXPullStep(), fMinMomentum += pStep) {
	fMaxXPull[1]=fMaxXPull[0];
	assignUniqueCombinations();
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
  //Loops on all identified candidates evaluating each of them. That is, calculating
  //its associated momentum and other useful variables for the algorithm
  Int_t dx=fCandidates.getDimX();

  HGeomVector tofR;
  Int_t i,j,k;
  HKickImpact *imp=0;
  HTofHit *tofHit=0;
  HShowerHit *shoHit=0;

  for (i=0;i<dx;i++) {
    imp=(HKickImpact *)fImpacts->UncheckedAt(i);
    for (j=0; j<fNTofHits; j++) {
      HKickCandidate &candidate=fCandidates(i,j);
      tofHit = (HTofHit *)fOuterHitArray.UncheckedAt(j);
      fEvaluator->evaluate(imp, tofHit, candidate);
      fillControl(candidate,imp);
    }
    for (j=fNTofHits,k=0; j<fNTofHits+fNShowerHits; j++,k++) {
      HKickCandidate &candidate = fCandidates(i,j);
      shoHit = (HShowerHit *)fOuterHitArray.UncheckedAt(j);
      fEvaluator->evaluate(imp, shoHit, candidate); 
      fillControl(candidate,imp);
    }
  }
}

void HKickTrackF::evaluateOuterCandidates(void) {
  //The same as evaluateMetaCandidates but with Mdc3 
  Int_t dx=fCandidates.getDimX();

  HGeomVector tofR;
  Int_t i,j;
  HKickImpact *imp=0;
  HMdcSeg *seg=0;

  for (i=0;i<dx;i++) {
    imp=(HKickImpact *)fImpacts->UncheckedAt(i);
    for (j=0; j<fNTotalHits; j++) {
      HKickCandidate &candidate=fCandidates(i,j);
      seg = (HMdcSeg *)fOuterHitArray.UncheckedAt(j);
      fEvaluator->evaluate(imp, seg, candidate);
      fillControl(candidate,imp);
    }
  }
}

Bool_t HKickTrackF::passesCuts(HKickCandidate &c) {
  //Checks if a given candidate passes all cuts to be considered a good candidate
  Float_t aPhi=TMath::Abs(c.getXPull());
  Int_t lowHigh = (c.getGhi1() < fGhiChange)?0:1;
  return ( !c.isAssigned() && 
	   (aPhi < fMaxXPull[lowHigh]) &&
	   (c.getMomentum() > fMinMomentum) );
}

void HKickTrackF::assignUniqueCombinations(void) {
  //Match those candidates which pass all cuts and verifying that
  //neither the META hit or the MDC hit are used in other valid candidates. In case
  //such a conflict is detected, this method tries to find a solution.
  Int_t impacts=fCandidates.getDimX();
  Int_t dy=fCandidates.getDimY();
  Int_t maxMatchs=0,matchsFound;
  Int_t i,j;

  //if (interactive) {
  // cout << "************************************\n";
  //for (i=0; i<fCandidates.getDimX(); i++) {
  //  for (j=0; j<fCandidates.getDimY(); j++) {
  //if (passesCuts(fCandidates(i,j))) {
  //  cout << fCandidates(i,j).getMomentum() << "/";
  //  cout << fCandidates(i,j).getDeltaPhi() << "\t";
  //} else cout << "0/0 \t\t";
  //  }
  //  cout << endl;
  //}
  //}

  maxMatchs=(impacts < dy)?impacts:dy;
  if (maxMatchs > 0) {
    do {
      matchsFound=0;
      for (i=0;i<impacts;i++) {
	for (j=0;j<dy;j++) {
	  if ( passesCuts(fCandidates(i,j)) ) {
	    if ( fConservativeMode || isUnique(i,j) ) { 
	      //Marks assigned candidates
	      fCandidates(i,j).markAssigned();
	      fillData(fCandidates(i,j),(HKickImpact *)fImpacts->At(i));
	      //if (interactive) cout << "(" << i << "," << j << ") ";
	      matchsFound++;
	      //maxMatchs--;
	    }
	  }
	}
      }
      if ( matchsFound >= maxMatchs ) matchsFound=0;    
    } while (matchsFound>0);
  }
  //if (interactive) cout << endl;  
}

void HKickTrackF::fillData(HKickCandidate &candidate,HKickImpact *imp) {
  //Fills output data in the output category
  HKickTrack *c=0;
  HMdcSeg *seg=0;
  TObject *slot=0;
  slot = fCandidateOut->getNewSlot(fSectorLoc);
  if (slot) {
    c = new (slot) HKickTrack;
    seg = imp->getOriginalSegment();
    c->setZ(seg->getZ(),seg->getErrZ());
    c->setR(seg->getR(),seg->getErrR());
    c->setTheta(seg->getTheta(),seg->getErrTheta());
    c->setPhi(seg->getPhi(),seg->getErrPhi());
    c->setP(candidate.getMomentum(),candidate.getMomentumError());
    c->setCharge(candidate.getPolarity());
    c->setMass(candidate.getMass(),10.); //FIXME: Do proper error calculation
  }
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


Int_t HKickTrackF::execute_1(void) { 
  //OBSOLETE. Read Class Description.
//   Int_t r=0;
//   readHits();
//   if (fNImpacts == 0) r=1;
//   else {
//     //for the moment only multiplicity one events allowed.
//     HTofHit *tofHit=0;
//     Double_t stheta=0;
//     Double_t p=0;
//     HGeomVector tofR,tofAlpha;
//     HKickImpact *imp=(HKickImpact *)fImpacts->At(0);
//     fTofIter->Reset();
//     tofHit=(HTofHit *)fTofIter->Next(); // Multiplitity one.
//     if (tofHit) {
//       Double_t errPt;
//       transform(tofHit,tofR);
//       tofAlpha=tofR-imp->position();
//       tofAlpha/=tofAlpha.length();
//       stheta=imp->direction().scalarProduct(tofAlpha);
//       stheta=TMath::Sqrt(1.-stheta*stheta);
//       p=fKickPlane->getPt(imp->position(),errPt)/stheta;
//       fData->Fill(imp->getMomentum(),p,fKickPlane->getPt(imp->position(),errPt),
// 		  stheta,imp->position().getX(),imp->position().getY(),
// 		  imp->position().getZ());
//     } else r=1;
//   }
//   fImpacts->Clear();
//   fNImpacts=0;

  return 0;
}

HKickCandidateEvaluator *HKickTrackF::makeEvaluator(void) {
  //Creates a candidate evaluator. Factory method
  return new HKickCandidateEvaluator(fKickPlane);
}

void HKickTrackF::setKickParIo(HRuntimeDb *rtdb) {
  //Adds kickplane's Parameter Io to the runtime database.
  HParIo *io=0;
  io=rtdb->getFirstInput();
  if (io != 0) {
    if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
      HKickParAsciiFileIo* p=
	new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
    }
  }
  io=rtdb->getSecondInput();
  if (io != 0) {
    if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
      HKickParAsciiFileIo* p=
	new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
    }
  }
  io=rtdb->getOutput();
  if (io != 0) {
    if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
      HKickParAsciiFileIo* p=
	new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
    }
  }
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

      setKickParIo(rtdb);

      if (mdc) { //Has the user set up a MDC?
	
	fInput=event->getCategory(catMdcSeg);
	if (!fInput) return kFALSE;
	else fIter=(HIterator *)fInput->MakeIterator();
	fSectors.Set(mdc->getMaxSectors()); //Get nb of sectors
	for (int i=0;i<fSectors.GetSize();i++) { //Sector configuration
	  if (mdc->isSectorActive(i)) {
	    if (mdc->getModule(i,2) > 0 || mdc->getModule(i,3) > 0) {
	      fSectors[i] = kLowResSector;
	    } else {
	      fSectors[i] = kLowResSector;
	    }
	  } else {
	    fSectors[i] = kInactiveSector;
	  }
	}
	fMdcGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
	if (!fMdcGeometry) {
	  fMdcGeometry=(HMdcGeomPar *)spec->createDetGeomPar("Mdc");
	}
	fKickPlane=(HKickPlane *)rtdb->getContainer("KickPlane");
	if (!fKickPlane) {
	  fKickPlane = new HKickPlane;
	  rtdb->addContainer(fKickPlane);
	}
	if (!fEvaluator) fEvaluator=makeEvaluator();
	fEvaluator->setMdcGeometry(fMdcGeometry);
	fKickPar=(HKickTrackFPar *)rtdb->getContainer("KickTrackFPar");
	if (!fKickPar) {
	  fKickPar = new HKickTrackFPar;
	  rtdb->addContainer(fKickPar);
	}
      } else return kFALSE;

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
      if (!fSpecGeometry) {
	fSpecGeometry = (HSpecGeomPar *)spec->createSpecGeomPar();
      }
      fEvaluator->setSpecGeometry(fSpecGeometry);
      if (tof) { //Has the user set up a TOF?
	fTofHits=event->getCategory(catTofHit);
	if (!fTofHits) {
	  fTofHits=tof->buildCategory(catTofHit);
	  if (!fTofHits) return kFALSE;
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
	fShowerHits=event->getCategory(catShowerHit);
	if (!fShowerHits) {
	  fShowerHits = shower->buildCategory(catShowerHit);
	  if (!fShowerHits) return kFALSE;
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
	else return kFALSE;
      }
      //!Setup output
    } else return kFALSE; //! (event && rtdb)
  } else return kFALSE; //! (gHades)
		
  return kTRUE;
}

Bool_t HKickTrackF::finalize(void) {
  //Finalization code
  return kTRUE;
}

void HKickTrackF::clearImpactList(void) {
  fImpacts->Clear();
}

void HKickTrackF::readOuterHits(HLocation &levArmLoc) {
  HMdcSeg *seg=0;
  Int_t i=0;

  fIter->Reset();
  fIter->gotoLocation(levArmLoc);
  while ( (seg=(HMdcSeg *)fIter->Next()) != 0) {
    fOuterHitArray.AddAtAndExpand(seg,i);
    i++;
  }
  fNTotalHits=i;
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
	fOuterHitArray.AddAtAndExpand(tofHit,i);
	i++;
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
  Int_t mdcSector=0;
  fIter->Reset();
  fIter->gotoLocation(loc);
  while ( (hit=(HMdcSeg *)fIter->Next()) != 0 ) {
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
  /*  Float_t x,y,errX,errY,dx,dy,errDx,errDy;
  HGeomVector rLocal,alphaLocal;
  Int_t sec,mod;
  hit->getSecMod(sec,mod);
  HModGeomPar *module=fMdcGeometry->getModule(0,mod);
  HGeomTransform &modTrans=module->getLabTransform();

  hit->getX(x,errX);
  hit->getY(y,errY);
  hit->getXDir(dx,errDx);
  hit->getYDir(dy,errDy);
  alphaLocal.setX(dx);
  alphaLocal.setY(dy);
  alphaLocal.setZ(TMath::Sqrt(1-dx*dx-dy*dy));
  rLocal.setX(x);
  rLocal.setY(y);
  rLocal.setZ(0.0);
  r=modTrans.transFrom(rLocal);
  alpha=modTrans.getRotMatrix()*alphaLocal;*/
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
      out << fData[i*fDimY+j].getDeltaPhi() << "\t";
    }
    out << endl;
  }
  return out;
}

ClassImp(HKickTrackF)

