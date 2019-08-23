//*-- Author : Dan Magestro
//*-- Created: 20/10/01
//*-- Last modified: 07/11/01

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
// HPhysicsMixedEvent
//
//  This class calculates event-wise variables or builds TObjArray's of
//  a 'mixed' event's particles (or subset of particles).  A mixed event
//  is constructed using an event-mixing algorithm which combines particles
//  from a sample of 10 consecutive events.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////
using namespace std;
#include "TRandom.h"
#include <time.h>
#include <iostream> 
#include <iomanip>
#include <math.h>
#include "TMath.h"
#include "TObjArray.h"
#include "phyanadef.h"
#include "hphysicsmixedevent.h"
#include "hparticle.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HPhysicsMixedEvent::HPhysicsMixedEvent(void) : HPhysicsEvent() {
// Default constructor
  evtCount = 0;
  for(Int_t i=0; i<10; i++) {
    event[i] = new TObjArray();
    event[i]->SetOwner(kTRUE);
  }
  sumArray = new TObjArray();
  mixedEvent = new TObjArray();
  iterMixed = (TIterator *)mixedEvent->MakeIterator();
}

HPhysicsMixedEvent::HPhysicsMixedEvent(HCategory *cat) : HPhysicsEvent(cat){
// Default constructor
  mixedEvent = new TObjArray();
  iterMixed = (TIterator *)mixedEvent->MakeIterator();
  evtCount = 0;
  for(Int_t i=0; i<10; i++) {
    event[i] = new TObjArray();
    event[i]->SetOwner(kTRUE);
  }
  sumArray = new TObjArray();
}

HPhysicsMixedEvent::HPhysicsMixedEvent(TClonesArray *array) : HPhysicsEvent(array){
  mixedEvent = new TObjArray();
  iterMixed = (TIterator *)mixedEvent->MakeIterator();
  evtCount = 0;
  for(Int_t i=0; i<10; i++) {
    event[i] = new TObjArray();
    event[i]->SetOwner(kTRUE);
  }
  sumArray = new TObjArray();
}

void HPhysicsMixedEvent::makeMixedEvent() {
// Build a TObjArray of HParticles sampled from 10 events.  Each time this
// function is called, the current event replaces the oldest event in the
// event[] array of events.
  mixedEvent->Clear();
  Int_t at = 0;
  if(evtCount == 10) evtCount = 0;

  // Fill appropriate element of TObjArray with current event
  event[evtCount]->Delete();
  HParticle *part = 0;
  iterPart->Reset();
  while ((part=(HParticle*) iterPart->Next())!=0) {
    HParticle *p = new HParticle(part);
    event[evtCount]->Add(p);

//      if(p->getPid()>0)
//        printf("###### pid = %3s  P = %7.2f  theta = %6.2f  phi = %6.2f\n",
//        HPhysicsConstants::pid(p->getPid()),p->P(),p->Theta()*r2d,p->Phi()*r2d);
//      else
//        printf("###### pid =  --  P = %7.2f  theta = %6.2f  phi = %6.2f\n",
//        p->P(),p->Theta()*r2d,p->Phi()*r2d);
  }

  // Form one large TObjArray from individual event TObjArrays
  sumArray->Clear();
  for(Int_t i=0; i<10; i++) {
    for(Int_t j=0; j < event[i]->GetEntriesFast(); j++) {
      sumArray->Add( ((HParticle*) event[i]->At(j)) );
    }
  }

  // Determine size of event by averaging sizes of the 10 events
  Int_t sumArraySize = sumArray->GetEntriesFast();
  Int_t mixedEventSize = (int) ((float)sumArraySize/10. + .5);

  // Fill mixed TObjArray with randomly selected particles from sumArray. Avoid
  // choosing the same particle more than once by removing it from the array.
  for(Int_t i=0; i<mixedEventSize; i++) {
    at = ran.Integer(sumArraySize);
    mixedEvent->Add( ((HParticle*)sumArray->At(at)) );
    sumArray->RemoveAt(at);
    sumArray->Compress();
    sumArraySize--;
  }
  evtCount++;
  return;
}

TObjArray* HPhysicsMixedEvent::getParticles(Int_t pid) {
// Returns an array of particles indicated by particle i.d.
  count++; if(count==50) count=0;
  HParticle  *part = 0;
  data[count]->Clear();

  iterMixed->Reset();
  while ((part=(HParticle*) iterMixed->Next())!=0)
     if(pid == part->getPid() || pid == -1) data[count]->Add(part);
  return data[count];
}

void HPhysicsMixedEvent::printEvent() {
// Print out particles (identified and not identified) in a mixed event
  HParticle *part = 0;
  Int_t count = 0;
  Float_t r2d=57.29578;
  iterMixed->Reset();
  while ((part=(HParticle*) iterMixed->Next())!=0) {
    if(part->getPid()>0)
       printf("MIX %3i) pid = %3s  P = %7.2f  theta = %6.2f  phi = %6.2f\n",
       count,HPhysicsConstants::pid(part->getPid()),part->P(),part->Theta()*r2d,part->Phi()*r2d);
    else
       printf("MIX %3i) pid =  --  P = %7.2f  theta = %6.2f  phi = %6.2f\n",
       count,part->P(),part->Theta()*r2d,part->Phi()*r2d);
    count++;
  }
  printf("\n");
  return;
}

TObjArray* HPhysicsMixedEvent::getParticles(const Char_t *name) {
// Returns an array of particles indicated by particle name or particle
// type (hadrons, noId, etc.).  It is also used internally by HPhysicsMixedEvent
// functions to 'decode' the input string.
  count++; if(count==50) count=0;
  HParticle  *part = 0;
  data[count]->Clear();
  HPhysicsConstants *table;
  Int_t pid;
  iterMixed->Reset();

  if(strcmp(name,"hadrons") == 0 || strcmp(name,"leptons") == 0
       || strcmp(name,"pions") == 0 || strcmp(name,"noId") == 0 ) {
    while ((part=(HParticle*) iterMixed->Next())!=0) {
      pid = part->getPid();
      if(strcmp(name,"hadrons") == 0)
        if( pid == table->pid("pi-") || pid == table->pid("pi+") ||
            pid == table->pid("K-")  || pid == table->pid("K+")  ||
            pid == table->pid("p")   || pid == table->pid("d") )
             data[count]->Add(part);
      if(strcmp(name,"pions") == 0)
        if( pid == table->pid("pi-") || pid == table->pid("pi+") )
             data[count]->Add(part);
      if(strcmp(name,"leptons") == 0)
        if( pid == table->pid("e-") || pid == table->pid("e+") )
             data[count]->Add(part);
      if(strcmp(name,"noId") == 0)
        if( pid == 0 )
             data[count]->Add(part);
    }
    return data[count];
  }
  else return getParticles(HPhysicsConstants::pid(name));
}

ClassImp(HPhysicsMixedEvent)
