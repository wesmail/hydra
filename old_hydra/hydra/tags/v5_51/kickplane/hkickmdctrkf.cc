#include "hkickmdctrkf.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmdcgeompar.h"
#include "hmdcdetector.h"
#include "hkickcandidateevaluator.h"
#include "hspecgeompar.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hruntimedb.h"
#include "hmdchit.h"
#include "hmdcseg.h"
#include "hkickmdctrk.h"
#include "hgeomvector.h"
#include "hades.h"
#include "hmdcdef.h"
#include "hkickplane.h"
#include "hkickimpact.h"
#include "hkickcandidate.h"
#include "hmdctrk.h"
#include "kickdef.h"
#include "hmatrixcategory.h"
#include "hgeomvolume.h"

//#include "hkickimpactsim.h"

#include <iostream.h>

//*-- Author : M. Sanchez (Nov 2000)

////////////////////////////////////////////////////////////////
// HKickMdcTrkF
//
//  This class takes a segment in the inner lever arm and a hit in MDC3
// and creates a HKickMdcTrk object out of them. Thaht is, matches the
// segments and hits corresponding to the same track and fits the momentum
///////////////////////////////////////////////////////////////


HKickMdcTrkF::HKickMdcTrkF(Text_t name[],Text_t title[]) : HKickTask (name, title) {
  fLaLocation.set(2,0,0);
  fImpacts=new TClonesArray("HKickImpact",20);
  fConservativeMode = kTRUE;
  fDebugMode = kFALSE;
}

HKickMdcTrkF::~HKickMdcTrkF(void) {
}

void HKickMdcTrkF::assignUniqueCombinations(void) {
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

Bool_t HKickMdcTrkF::isUnique(Int_t i,Int_t j) {
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

void HKickMdcTrkF::evaluateCandidates(void) {
  //Evaluates each of the candidates filling control variables which are
  //are used for the matching of tracks.
  Int_t dx=fCandidates.getDimX();

  Int_t i,j;
  Int_t n = fOuterHitArray.GetEntriesFast();
  HKickImpact *imp=0;
  HMdcHit *hit=0;

  for (i = 0; i < dx; i++) {
    imp=(HKickImpact *)fImpacts->UncheckedAt(i);
    for (j=0; j<n ; j++) {
      HKickCandidate &candidate=fCandidates(i,j);
      hit = (HMdcHit *)fOuterHitArray.UncheckedAt(j);
      evaluate(imp, hit, candidate);
      fillControl(candidate,imp);
    }
  }
}

void HKickMdcTrkF::fillControl(HKickCandidate &c, HKickImpact *imp) {

}

// void HKickMdcTrkF::fillData(HKickCandidate &candidate, HKickImpact *imp) {
//     //Fills output data in the output category
//   HMdcTrk *c=0;
//   HMdcSeg *seg=0;
//   TObject *slot=0;

//   slot = fMdcOutput->getNewSlot(fSectorLoc);
//   if (slot) {
//     c = new (slot) HMdcTrk;
//     seg = imp->getOriginalSegment();
//     c->setZ(seg->getZ(),seg->getErrZ());
//     c->setR(seg->getR(),seg->getErrR());
//     c->setTheta(seg->getTheta(),seg->getErrTheta());
//     c->setPhi(seg->getPhi(),seg->getErrPhi());
//     c->setP(candidate.getMomentum(),candidate.getMomentumError());
//     c->setCharge(candidate.getPolarity());
//     c->setFlag(candidate.isAssigned()?1:0);
//   }
// }

void HKickMdcTrkF::fillData(HKickCandidate &candidate, HKickImpact *imp) {
    //Fills output data in the output category
  HKickMdcTrk *c=0;
  TObject *slot=0;
  HGeomVector r0;
  HGeomVector rOut;
  HGeomVector &kpos=imp->position();
  HGeomVector &kdir=imp->direction();

  //FIXME: este no tiene porque ser el vertice
  r0.setX(kpos.getX() - (kdir.getX()/kdir.getZ()) * kpos.getZ());
  r0.setY(kpos.getY() - (kdir.getY()/kdir.getZ()) * kpos.getZ());
  r0.setZ(0.);

  candidate.getPosition1(rOut);

  slot = fMdcOutput->getNewSlot(fSectorLoc);
  if (slot) {
    c = new (slot) HKickMdcTrk;

    c->setZeroPos(r0);
    c->setKickPos(kpos);
    c->setOuterPos(rOut);
    c->setMomentum(candidate.getMomentum());
    c->setCharge(int(candidate.getPolarity()));
    c->setPull(candidate.getXPull());
    c->setDist(candidate.getDist());
    c->setKickDist(candidate.getKickDist());
    c->setSector(candidate.getSector());
    //c->setFlag( ((HKickImpactSim*)imp)->getTrack() ); //FIXME: Move to sim class
  }
}

void HKickMdcTrkF::assignRemainingCombinations(void) {
  // Adds combinations not found with assignUniqueCombinations to the 
  //output.
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

Int_t HKickMdcTrkF::execute(void) {

  for (Int_t sector=0; sector<6; sector++) { //FIXME: loop on active sectors
    fLaLocation[0]=sector;
    fSectorLoc[0] = sector;
    // read hits from inner chambers
    fLaLocation[1]=0;
    readHits(fLaLocation);

    // read hits from outer chambers
    fLaLocation[1]=2;
    readOuterHits(fLaLocation);

    // Resize candidate matrix to hold all possible combinations
    fCandidates.resize(fNImpacts,fOuterHitArray.GetEntriesFast());
    evaluateCandidates();

    // Assign unique combinations (high signal/background)
    fMaxXPull = .2;
    fMaxD1[0] = 100.;
    fMaxD1[1] = 20.;
    fMaxKickDist[0] = 100.;
    fMaxKickDist[1] = 500.;
    assignUniqueCombinations();
//     fMaxXPull = 1.;
//     fMaxD1 = 50.;
//     fMaxKickDist = 82.;
//     assignUniqueCombinations();

    // Assign all remaining combinations passing the cuts
    if (fConservativeMode) {
      assignRemainingCombinations();
    }

  }
  return 0;
}

Bool_t HKickMdcTrkF::init(void) {
  fSectorLoc.set(1,0);
  if (gHades) {
    HEvent *event=gHades->getCurrentEvent();
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *spec=gHades->getSetup();
    if (event && rtdb) {
      fMdc = dynamic_cast<HMdcDetector *>(spec->getDetector("Mdc"));

      setKickParIo(rtdb);

      if (!initMdc(event, rtdb, spec)) return kFALSE;


      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");


    } else return kFALSE; //! (event && rtdb)
  } else return kFALSE; //! (gHades)

  if (fDebugMode) {
    fControl = new TNtuple("kickmatch","kickmatch","xK:yK:zK:xC:yC:zC:dK:d:pull:xO:yO:zO");
  }

  return kTRUE;
}

Bool_t HKickMdcTrkF::initMdc(HEvent *event,HRuntimeDb *rtdb,
			     HSpectrometer *spec) {
  //Called by init() to initialize variables related to the MDC
  if (fMdc) {
    fMdcInput=event->getCategory(catMdcSeg);
    if (!fMdcInput) return kFALSE;
    else fMdcInputIter=(HIterator *)fMdcInput->MakeIterator();
    
    fMdcHitInput = event->getCategory(catMdcHit);
    if (!fMdcHitInput) return kFALSE;
    else fMdcHitInputIter=(HIterator *)fMdcHitInput->MakeIterator();

    // commented out for Nov'00 beamtime...
//     fMdcOutput=event->getCategory(catMdcTrack);
//     if (!fMdcOutput) {
//       fMdcOutput = fMdc->buildCategory(catMdcTrack);
//       if (!fMdcOutput) return kFALSE; 
//       event->addCategory(catMdcTrack,fMdcOutput,"Mdc");
//     }
    fMdcOutput=event->getCategory(catKickMdcTrk);
    if (!fMdcOutput) {
      Int_t sizes[2] = {6, 100};
      fMdcOutput = new HMatrixCategory("HKickMdcTrk",2,sizes,1.);
      if (!fMdcOutput) return kFALSE; 
      event->addCategory(catKickMdcTrk,fMdcOutput,"Tracks");
    }
    
    fMdcGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");

    fKickPlane = (HKickPlane *)rtdb->getContainer("KickPlaneMDC3");
    
  } else return kFALSE;
  return kTRUE;
}


Bool_t HKickMdcTrkF::finalize(void) {
  if (fDebugMode) {
    gHades->getOutputFile()->cd();
    fControl->Write();
  }
  return kTRUE;
}


void HKickMdcTrkF::readHits(HLocation &loc) {
  //Loops on the input category reading objects and adding objects to
  //the list of candidates fImpacts.
  HMdcSeg *hit=0;
  HGeomVector r;
  HKickImpact *imp=0;
  Int_t mdcSector=0;
  fNImpacts=0;
  fImpacts->Clear();
  fMdcInputIter->Reset();
  fMdcInputIter->gotoLocation(loc);
  while ( (hit=(HMdcSeg *)fMdcInputIter->Next()) != 0 ) {
    mdcSector=hit->getSec();
    imp=new( (*fImpacts)[fNImpacts++] ) HKickImpact;
    transform(hit,r,imp->direction());
    fKickPlane->calcIntersection(r,imp->direction(),imp->position());
    imp->setSector(mdcSector);
    imp->setOriginalSegment(hit);
  }
}

void HKickMdcTrkF::evaluate(HKickImpact *imp, HMdcHit *hit,
			    HKickCandidate &out) {
  //Evaluates one particular combination
  Float_t p;
  Double_t errPt;
  HGeomVector r,alpha,cross;
  transform(hit, r, alpha);
  HGeomVector &iR     = imp->position();
  HGeomVector &iAlpha = imp->direction();
  HGeomVector dir    = r - iR; dir /= dir.length();
  Double_t g1         = TMath::ATan2( iAlpha.getZ(), iAlpha.getY() );
  Double_t g2         = TMath::ATan2( dir.getZ(), dir.getY() );
  Double_t f1         = TMath::ATan2( iAlpha.getY(), iAlpha.getX() );
  Double_t f2         = TMath::ATan2( dir.getY(), dir.getX() );
  HGeomVector n       = iAlpha.vectorProduct(alpha); n /= n.length();
  Double_t dist       = TMath::Abs( (iR - r).scalarProduct(n) );
  Double_t sinGhi     = TMath::Sin( (g2 - g1) / 2. );
  Double_t sinPhi     = TMath::Sin( (f2 - f1) / 2. );
  Int_t pol           = (sinGhi>0)?1:-1;
  Double_t pt         = fKickPlane->getPt(iR, errPt, pol);
  Double_t ptf        = fKickPlane->getPtPhi(iR, errPt, pol);

  p = pt / ( 2. * sinGhi);

  // Fill output
  out.setMomentum(p);
  out.setDenominator(2. * sinGhi);
  out.setDist(dist);
  out.setXPull(sinPhi -
	       ptf / (2. * p));
	       //cout << sinPhi << " -- " << (ptf / (2*p)) << endl;
	       //cout << f1 << " ** " << f2 << "**";
	//cout << TMath::ATan2(alpha.getY(),alpha.getX()) << endl;
  // Distance from cross point to kickplane.
  fVertexFitter.reset();
  fVertexFitter.addLine(iR,iAlpha);
  fVertexFitter.addLine(r,alpha);
  fVertexFitter.getVertex(cross);
  out.setKickDist( fKickPlane->distanceTo(cross) );
  out.setPosition1(r.getX(),r.getY(),r.getZ());
  out.setPolarity(pol);
  out.setSector(hit->getSector());

  if (fDebugMode) {
    fControl->Fill(iR.getX(),iR.getY(),iR.getZ(),cross.getX(),cross.getY(),cross.getZ(),
          fKickPlane->distanceTo(cross), dist, out.getXPull(),
  	  r.getX(),r.getY(),r.getZ());
  }
}

void HKickMdcTrkF::transform(HMdcHit *hit, HGeomVector &r,
			     HGeomVector &alpha) {
  //Transforms between the representation of a straight line in HMdcHit
  //to a representation based in a position vector and a director vector in
  //the sector reference system.
  HGeomVector rLocal,alphaLocal;
  HModGeomPar *mod = fMdcGeometry->getModule(hit->getSector(),hit->getModule());

  if (mod) { 
    HGeomTransform &transMod = mod->getLabTransform();
    HGeomTransform &transSec = fSpecGeometry->getSector(hit->getSector())->getTransform();
    HGeomTransform trans(transMod);
    trans.transTo(transSec);

    rLocal.setX(hit->getX());
    rLocal.setY(hit->getY());
    rLocal.setZ(0.);
    alphaLocal.setX(hit->getXDir());
    alphaLocal.setY(hit->getYDir());
    alphaLocal.setZ( TMath::Sqrt(1. - hit->getXDir()*hit->getXDir() -
				 hit->getYDir()*hit->getYDir()) );

    r = trans.transFrom(rLocal);
    alpha = trans.getRotMatrix() * alphaLocal;
  } else Warning("transform","Module %i unknown",hit->getModule());
}

void HKickMdcTrkF::transform(HMdcSeg *hit,
			     HGeomVector &r,HGeomVector &alpha) {
  //Transforms between the representation of a straight line in HMdcSeg
  //to a representation based in a position vector and a director vector in
  //the sector reference system.  
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

void HKickMdcTrkF::readOuterHits(HLocation &levArmLoc) {
  // Reads hits in MDC3
  HMdcHit *hit=0;
  Int_t i=0;

  fOuterHitArray.Clear();
  fMdcHitInputIter->Reset();
  fMdcHitInputIter->gotoLocation(levArmLoc);
  while ( (hit=(HMdcHit *)fMdcHitInputIter->Next()) != 0) {
    //fMdcHitInputIter->getLocation().Dump();
    //cout << hit->IsA()->GetName() << endl;
    fOuterHitArray.AddAtAndExpand(hit,i);
    i++;
  }
}

ClassImp(HKickMdcTrkF)
