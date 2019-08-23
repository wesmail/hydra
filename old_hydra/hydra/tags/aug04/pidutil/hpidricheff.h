// hpidPDFRichEff.cc 3.05.2004  by Tassilo Christ
// mailto tassilo.christ@ph.tum.de


//helper class to check rich efficiency


#ifndef  HPIDRICHEFF_H
#define  HPIDRICHEFF_H
#include "hreconstructor.h"
#include "haddef.h"
#include "piddef.h"

class TH1F;
class TTree;
class HCategory;
class HIterator;
class HPidTrackCand;
class HRichHit;
class HRichHitSim;
class HKickTrack;
class HRichAnalysisPar;
class HRichGeometryPar;
class HGeantKine;
class HMdcSeg;
class HMdcHit;
class TFile;

class HPidRichEff : public HReconstructor 
{
 public:
  

  HPidRichEff(Char_t* OutputFile = "richeff.root",Int_t which_particle_ID=-99, Bool_t getGeantMom=kFALSE);
  ~HPidRichEff();

  
  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  
  Bool_t checkForRing();
  Bool_t checkForKickTrack();
  Bool_t save();

 private:

  Int_t evcount;
  Bool_t takeRealMom;
  Int_t desiredParticleId;
  
  //Histograms to hold the momentum of the tracks with and without ring

  TFile* OutputFile;

  TH1F* MomDistHadRing; 
  TH1F* MomDistLepRing; 
  TH1F* MomDistHadNoRing; 
  TH1F* MomDistLepNoRing; 
  TH1F* MomDistHadTotal; 
  TH1F* MomDistLepTotal; 
  TH1F* MomDistHadRatio; 
  TH1F* MomDistLepRatio; 

  //Destination file
  TString* OutFileName;

  //Categories and Iterators
  HCategory* pInputCategory;
    HIterator* pIteratorInput;


  //Pointers to the candidate, the ring and the KickTrack
  HPidTrackCand* theCandidate;

  HRichHit*    theRing;
  HKickTrack*  theKickTrack;

 public:
  ClassDef(HPidRichEff,0) 
};

#endif // HPIDRICHEFF_H
