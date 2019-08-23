//*-- Author : Dan Magestro
//*-- Created: 20/10/01
//*-- Last modified: 07/11/01
#ifndef HPHYSICSMIXEDEVENT_H
#define HPHYSICSMIXEDEVENT_H
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hlocation.h"
#include "hphysicsconstants.h"
#include "hphysicsevent.h"
#include <TRandom.h>

class TObjArray;

class HPhysicsMixedEvent : public HPhysicsEvent {

private:
  TRandom ran;              //! Random number generator
  TObjArray *mixedEvent;    //! Mixed event
  TIterator *iterMixed;     //! Mixed event iterator

  TObjArray *event[10];     //! Array of events
  TObjArray *sumArray;      //! Array of particles from 10 events
  Int_t evtCount;           //!

public:
  HPhysicsMixedEvent(void);
  HPhysicsMixedEvent(HCategory *cat);
  HPhysicsMixedEvent(TClonesArray *array);
  ~HPhysicsMixedEvent(void) {;}

  Float_t sumPt(Int_t pid=-1) {return HPhysicsEvent::sumPt(getParticles(pid));};
  Float_t sumPt(Char_t *pid)  {return HPhysicsEvent::sumPt(getParticles(pid));};

  Float_t sumEt(Int_t pid=-1) {return HPhysicsEvent::sumEt(getParticles(pid));};
  Float_t sumEt(Char_t *pid)  {return HPhysicsEvent::sumEt(getParticles(pid));};

  Float_t sumMt(Int_t pid=-1) {return HPhysicsEvent::sumMt(getParticles(pid));};
  Float_t sumMt(Char_t *pid)  {return HPhysicsEvent::sumMt(getParticles(pid));};

  Int_t mult(Int_t pid=-1)  {return HPhysicsEvent::mult(getParticles(pid));};
  Int_t mult(Char_t *pid)   {return HPhysicsEvent::mult(getParticles(pid));};

  void makeMixedEvent();
  void printEvent();

  TObjArray *getParticles(Int_t pid=-1);
  TObjArray *getParticles(Char_t *pid);

public:
  ClassDef(HPhysicsMixedEvent,1) // Event-mixing class based on HPhysicsEvent
};

#endif
