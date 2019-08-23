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
class HGeantKine;
class HPidCommonTrack;
class TNtuple;

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
  Int_t getTruePID(const HPidCommonTrack* commonTrack){
#warning "HPidTest :getTruePID() :function declared, but not implemented!"
      return 0;
  }
 private:

  Int_t evcount;
  

  //An Ntuple to store the properties of the recognized particles and their true ids
  TNtuple* recognizedParticles;

  //Destination file
  TString* OutFileName;

  //Categories and Iterators
  HCategory* pGeantKineCategory;
  HCategory* pParticleCategory;
  

  HIterator* pIteratorGeantKine;
  HIterator* pIteratorParticle;


  //Pointers to the candidate, the ring and the KickTrack

  HPidParticleSimExtHit* theParticle;
  HGeantKine*  theKine;

 public:
  ClassDef(HPidTest,0) 
};

#endif // HPIDTEST_H
