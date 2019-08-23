#include "hkickpionimass.h"
#include "hkicktrack.h"
#include "hgeomvector.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hades.h"
#include "hruntimedb.h"
#include "kickdef.h"
#include "hevent.h"
#include <math.h>

HKickPionIMass::HKickPionIMass(const Text_t name[],const Text_t title[]) : HReconstructor (name,title) {
  fPiMinus = new TClonesArray("HKickIMassParticle",100);
  fPiPlus = new TClonesArray("HKickIMassParticle",100);
  fProton = new TClonesArray("HKickIMassParticle",100);
}

HKickPionIMass::~HKickPionIMass(void) {
  delete fPiMinus;
  delete fPiPlus;
  delete fProton;
}

void HKickPionIMass::readMetaTracks(void) {
  HRtMetaTrack *track = 0;
  Int_t nprot=0,npiplus=0,npimin=0;
  HKickIMassParticle *particle = 0;

  fInputIter->Reset();
  while ( (track = (HRtMetaTrack *)fInputIter->Next()) != 0) {
    if (track->getSector()!=2) continue;
    if (isProton(track)) {
      particle = new((*fProton)[nprot]) HKickIMassParticle; 
      convert(track,particle);
      nprot++;
    } else if (isPion(track)) {
      if (track->getPolarity()>0) {
	particle = new((*fPiPlus)[npiplus]) HKickIMassParticle;
	convert(track,particle);
	npiplus++;
      } else {
	particle = new((*fPiMinus)[npimin]) HKickIMassParticle;
	convert(track,particle);
	npimin++;
      }
    }
  }
}

void HKickPionIMass::readKickTracks(void) {
  HKickTrack *track = 0;
  Int_t nprot=0,npiplus=0,npimin=0;
  HKickIMassParticle *particle = 0;

  fInputIter->Reset();
  while ( (track = (HKickTrack *)fInputIter->Next()) != 0) {
    if (track->getSector()==0 || track->getSector()==2) continue;
    //if (track->getSector()!=2) continue;
    if (isProton(track)) {
      particle = new((*fProton)[nprot]) HKickIMassParticle; 
      convert(track,particle);
      nprot++;
    } else if (isPion(track)) {
      if (track->getCharge()>0) {
	particle = new((*fPiPlus)[npiplus]) HKickIMassParticle;
	convert(track,particle);
	npiplus++;
      } else {
	particle = new((*fPiMinus)[npimin]) HKickIMassParticle;
	convert(track,particle);
	npimin++;
      }
    }
  }
}

Bool_t HKickPionIMass::isProton(Float_t pz,Float_t beta,Int_t sys) {
  if (sys==0) {
    if (fCutProtonSho->IsInside(beta,pz)) return kTRUE;
  } else {
    if (fCutProtonTof->IsInside(beta,pz)) return kTRUE;
  }
  return kFALSE;
}

Bool_t HKickPionIMass::isPion(Float_t pz,Float_t beta,Int_t sys) {
  if (sys==0) {
    if (fCutPiPlusSho->IsInside(beta,pz) || fCutPiMinusSho->IsInside(beta,pz)) return kTRUE;
  } else {
    if (fCutPiPlusTof->IsInside(beta,pz) || fCutPiMinusTof->IsInside(beta,pz)) return kTRUE;
  }
  return kFALSE;
}

Bool_t HKickPionIMass::isProton(HKickTrack *track) {
  Float_t pz = track->getP()*track->getCharge();
  Float_t beta = track->getBeta();

  return isProton(pz,beta,track->getSystem());
}

Bool_t HKickPionIMass::isPion(HKickTrack *track) {
  return isPion(track->getP()*track->getCharge(),track->getBeta(),track->getSystem());
}

Bool_t HKickPionIMass::isProton(HRtMetaTrack *track) {
  Float_t pz = track->getPMdc3()*track->getPolarity();
  Float_t beta = track->getBeta();

  return isProton(pz,beta,int(track->getSystem()));
}

Bool_t HKickPionIMass::isPion(HRtMetaTrack *track) {
  Float_t pz = track->getPMdc3()*track->getPolarity();
  Float_t beta = track->getBeta();
  Int_t sys = int(track->getSystem());
  return isPion(pz,beta,sys);
}

void HKickPionIMass::convert(HKickTrack *track,HKickIMassParticle *part) {
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;
  HGeomVector r,alpha,rLoc,alphaLoc;
  HGeomTransform &trans = fGeomPar->getSector(track->getSector())->getTransform();

  theta=track->getTheta();
  phi=track->getPhi();
  rLoc.setZ(track->getZ());
  rLoc.setX(track->getR()*TMath::Cos(phi+pi2));
  rLoc.setY(track->getR()*TMath::Sin(phi+pi2));
  alphaLoc.setZ(TMath::Cos(theta));
  alphaLoc.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alphaLoc.setX(TMath::Sin(theta)*TMath::Cos(phi));

  r=trans.transFrom(rLoc);
  alpha = trans.getRotMatrix()*alphaLoc;

  TLorentzVector &p = part->P();
  Float_t momentum = track->getP();
  if (isProton(track)) {
    momentum += 3.35 + exp(5.15147 - 0.004549*momentum);
  } else if (isPion(track)) {
    momentum += 3.908 + exp(3.903 - 0.0175*momentum);
  }
  Float_t E = momentum / track->getBeta();
  p.SetPxPyPzE(alpha.getX()*momentum, alpha.getY()*momentum, 
      alpha.getZ()*momentum, E);
  part->setSystem(track->getSystem());
}

void HKickPionIMass::convert(HRtMetaTrack *track,HKickIMassParticle *part) {
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;
  HGeomVector r,alpha,rLoc,alphaLoc;
  HGeomTransform &trans = fGeomPar->getSector(track->getSector())->getTransform();

  theta=track->getTheta();
  phi=track->getPhi();
  rLoc.setZ(track->getZ());
  rLoc.setX(track->getR()*TMath::Cos(phi+pi2));
  rLoc.setY(track->getR()*TMath::Sin(phi+pi2));
  alphaLoc.setZ(TMath::Cos(theta));
  alphaLoc.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alphaLoc.setX(TMath::Sin(theta)*TMath::Cos(phi));

  r=trans.transFrom(rLoc);
  alpha = trans.getRotMatrix()*alphaLoc;

  TLorentzVector &p = part->P();
  Float_t momentum = track->getPMdc3();
  if (isProton(track)) {
    momentum += 3.35 + exp(5.15147 - 0.004549*momentum);
  } else if (isPion(track)) {
    momentum += 3.908 + exp(3.903 - 0.0175*momentum);
  }
  Float_t E = momentum / track->getBeta();
  p.SetPxPyPzE(alpha.getX()*momentum, alpha.getY()*momentum, 
      alpha.getZ()*momentum, E);
  part->setSystem(int(track->getSystem()));
}

void HKickPionIMass::clearArrays(void) {
  fPiMinus->Clear();
  fPiPlus->Clear();
  fProton->Clear();
}

Int_t HKickPionIMass::execute(void) {
  HKickIMassParticle *part = 0, *part2 = 0;
  clearArrays();
  if (fMode == kLowRes) readKickTracks();
  else if (fMode == kHiRes) readMetaTracks();
  else return -1;
  TIterator *iterPP = fPiPlus->MakeIterator();
  TIterator *iterPM = fPiMinus->MakeIterator();
  TIterator *iterP = fProton->MakeIterator();

  iterPP->Reset();
  while ( (part = (HKickIMassParticle*)iterPP->Next()) != 0) {
    iterPM->Reset();
    while ( (part2 = (HKickIMassParticle *)iterPM->Next()) != 0) {
      fillPair(0, part, part2);
    }

    iterP->Reset();
    while ( (part2 = (HKickIMassParticle *)iterP->Next()) != 0) {
      fillPair(1, part, part2);
    }
  }

  iterPM->Reset();
  while ( (part = (HKickIMassParticle *)iterPM->Next()) != 0) {
    iterP->Reset();
    while ( (part2 = (HKickIMassParticle *)iterP->Next()) != 0) {
      fillPair(2, part, part2);
    }
  }

  delete iterPP;
  delete iterPM;
  delete iterP;
  return 0;
}

void HKickPionIMass::fillPair(Int_t type, HKickIMassParticle *p1, HKickIMassParticle *p2) {
  TLorentzVector &pp1 = p1->P();
  TLorentzVector &pp2 = p2->P();
  Float_t M = (pp1+pp2).M();
  Float_t sys = p1->getSystem() + p2->getSystem();
  fOutput->Fill(sys,type,M);
}

Bool_t HKickPionIMass::init(void) {
  HRuntimeDb *rtdb = Hades::instance()->getRuntimeDb();
  fGeomPar = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

  HEvent *ev = Hades::instance()->getCurrentEvent();
  if (ev) {
    if (fMode == kLowRes)
      fInput = ev->getCategory(catKickTrack);
    else if (fMode == kHiRes)
      fInput = ev->getCategory(catKickMdcTrk);
    else return kFALSE;
    if (!fInput) return kFALSE;
    fInputIter = (HIterator *)fInput->MakeIterator();
  }
  if (gHades->getOutputFile()) {
    gHades->getOutputFile()->cd();
    fOutput = new TNtuple("imass","imass","sys:type:imass");
  }

  TFile *ff = new TFile("pid_cuts.root");
  fCutPiPlusSho = (TCutG *)ff->Get("cut_piplus_shower");
  fCutPiMinusSho = (TCutG *)ff->Get("cut_piminus_shower");
  fCutProtonSho = (TCutG *)ff->Get("cut_proton_shower");
  fCutPiPlusTof = (TCutG *)ff->Get("cut_piplus_tof");
  fCutPiMinusTof = (TCutG *)ff->Get("cut_piminus_tof");
  fCutProtonTof = (TCutG *)ff->Get("cut_proton_tof");
  delete ff;

  return kTRUE;
}

Bool_t HKickPionIMass::reinit(void) {
  return kTRUE;
}

Bool_t HKickPionIMass::finalize(void) {
  if (gHades->getOutputFile()) {
    gHades->getOutputFile()->cd();
    fOutput->Write();
  }
  return kTRUE;
}

ClassImp(HKickPionIMass)
