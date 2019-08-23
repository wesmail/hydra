// hpidTest.h, by Tassilo Christ 22.06.03
// Reconstructor class intended to evaluate the performance of the pid software
// under different operating condidions


#ifndef  HPIDTEST_H
#define  HPIDTEST_H
#include "hreconstructor.h"
#include "haddef.h"
#include "piddef.h"

class HIterator;
class HCategory;
class HPidParticleSimExtHit;
class HPidTrackCand;
class HGeantKine;
class HPidCommonTrack;
class TNtuple;
class TTree;
class HRichHitSim;

//Attention! Keep the order
#define cp_nwiresmdc0     0
#define cp_nwiresmdc1     1
#define cp_clussizemdc0   2
#define cp_clussizemdc1   3  
#define cp_mdcchisquare   4
#define cp_clusfinderlvl0 5
#define cp_clusfinderlvl1 6
#define cp_kicktype       7
#define cp_momentum       8
#define cp_pmq            9 
#define cp_avcharge       10 
#define cp_padnr          11 
#define cp_centroid       12 
#define cp_locmax         13 
#define cp_ringtype       14
#define cp_theta          15 
#define cp_phi            16 
#define cp_sector         17 
#define cp_corrtype       18
#define cp_recID          19
#define cp_trueID         20
#define cp_seenInRICH     21
#define cp_kickcharge     22

class HPidTest : public HReconstructor 
{
 public:
  

  // default settings: 
  // make only PID histograms (no CP ID) 
  // don't use rings correlated to segments - just use KTs  
  HPidTest(Char_t* OutputFile = "pidPerformance.root");
  ~HPidTest();

  
  Bool_t init();
  Bool_t finalize();
  Int_t execute();
  Bool_t clearCPPattern();
  Bool_t fillCPPattern(HPidParticleSimExtHit* pParticle);
  Bool_t bookKickObservables(HPidTrackCand* pCandidate);
  Bool_t bookRingObservables(HPidTrackCand* pCandidate);
  Float_t getRingType(HRichHitSim* pRichHitSim);
    //Int_t getTruePID(const HPidCommonTrack* commonTrack);
 private:

  Int_t evcount;

  //In this array we store a vector of observables that we intend to use later for CP ID.
  Float_t PatternInstance[23];

  //An Ntuple to store the properties of the recognized particles and their true ids
  TNtuple* recognizedParticles;

  //A tree to store the observables deduced from recognized electrons for Close Pair ID
  TTree* CloseTrackObservables;

  //Destination file
  TString* OutFileName;

  //Categories and Iterators
  HCategory* pGeantKineCategory;
  HCategory* pParticleCategory;
  HCategory* pMdcSegCategory;
  HCategory* pMdcHitCategory;
  HCategory* pMdcClusInfCategory;

  HIterator* pIteratorGeantKine;
  HIterator* pIteratorParticle;
  HIterator* pIteratorMdcSeg;
  HIterator* pIteratorMdcHit;
  HIterator* pIteratorMdcClusInf;

  //Pointers to the candidate, the ring and the KickTrack

  HPidParticleSimExtHit* theParticle;
  HGeantKine*  theKine;

 public:
  ClassDef(HPidTest,0) 
};

#endif // HPIDTEST_H
