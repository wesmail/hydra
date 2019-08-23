//*-- AUTHOR : D.Magestro
//*-- Created : 01/09/2001
//*-- Last modified: 07/11/01

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HPhysicsEvent
//
//  This class calculates event-wise variables or builds TObjArray's of
//  an event's particles (or subset of particles).  All of the class's
//  functions support Geant particle i.d.'s, particle names (such as "e+")
//  and the particle types: "hadrons", "leptons", "pions", and "noId".
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
#include "TIterator.h"
#include "TObjArray.h"
#include "phyanadef.h"
#include "hphysicsevent.h"
#include "hparticle.h"
#include "hades.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hphysicsconstants.h"
#include "hrecevent.h"
#include "hlocation.h"

HPhysicsEvent::HPhysicsEvent(void) {
// Default constructor
  fPartCat = 0;
  fPartArray = 0;
  iterPart=0;
  for(Int_t i=0;i<50;i++) data[i] = new TObjArray();
}

HPhysicsEvent::HPhysicsEvent(HCategory *cat) {
// Takes category as input
  fPartCat = cat;
  fPartArray = 0;
  iterPart = (HIterator *)fPartCat->MakeIterator("native");
  for(Int_t i=0;i<50;i++) data[i] = new TObjArray();
}

HPhysicsEvent::HPhysicsEvent(TClonesArray *array) {
// Takes TClonesArray as input
  fPartCat = 0;
  fPartArray = array;
  iterPart = (TIterator *)fPartArray->MakeIterator();
  for(Int_t i=0;i<50;i++) data[i] = new TObjArray();
}

Float_t HPhysicsEvent::sumPt(TObjArray *array) {
// Sum transverse momentum for array of particles
  HParticle  *part = 0;
  Double_t sum = 0;
  TIterator *iter = array->MakeIterator();

  while ((part=(HParticle*) iter->Next())!=0)  sum += part->Pt();
  return sum;
}

Float_t HPhysicsEvent::sumEt(TObjArray *array) {
// Sum transverse energy for array of particles
  HParticle  *part = 0;
  Double_t sum = 0;
  TIterator *iter = array->MakeIterator();

  while ((part=(HParticle*) iter->Next())!=0)
     sum += part->E()*sin(part->Theta());
  return sum;
}

Float_t HPhysicsEvent::sumMt(TObjArray *array) {
// Sum transverse mass for array of particles
  HParticle  *part = 0;
  Double_t sum = 0;
  TIterator *iter = array->MakeIterator();

  while ((part=(HParticle*) iter->Next())!=0) sum += part->Mt();
  return sum;
}

Int_t HPhysicsEvent::mult(TObjArray *array) {
// Multiplicity of array of particles
  return array->GetEntriesFast();
}

void HPhysicsEvent::printEvent() {
// Print out particles (identified and not identified) in an event
  HParticle *part = 0;
  Int_t count = 0;
  Float_t r2d=57.29578;
  iterPart->Reset();
  while ((part=(HParticle*) iterPart->Next())!=0) {
    if(part->getPid()>0)
       printf(" %3i) pid = %3s  P = %7.2f  theta = %6.2f  phi = %6.2f\n",
       count,HPhysicsConstants::pid(part->getPid()),part->P(),part->Theta()*r2d,part->Phi()*r2d);
    else
       printf(" %3i) pid =  --  P = %7.2f  theta = %6.2f  phi = %6.2f\n",
       count,part->P(),part->Theta()*r2d,part->Phi()*r2d);
    count++;
  }
  printf("\n");
  return;
}

TObjArray* HPhysicsEvent::getParticles(Int_t pid) {
// Returns an array of particles indicated by particle i.d.
  count++; if(count==50) count=0;
  HParticle  *part = 0;
  data[count]->Clear();

  iterPart->Reset();
  while ((part=(HParticle*) iterPart->Next())!=0)
    if(pid == part->getPid() || pid == -1) data[count]->Add(part);
  return data[count];
}

TObjArray* HPhysicsEvent::getParticles(Char_t *name) {
// Returns an array of particles indicated by particle name or particle
// type (hadrons, noId, etc.).  It is also used internally by HPhysicsEvent
// functions to 'decode' the input string.
  count++; if(count==50) count=0;
  HParticle  *part = 0;
  data[count]->Clear();
  HPhysicsConstants *table;
  Int_t pid;
  iterPart->Reset();

  if(strcmp(name,"hadrons") == 0 || strcmp(name,"leptons") == 0
       || strcmp(name,"pions") == 0 || strcmp(name,"noId") == 0 ) {
    while ((part=(HParticle*) iterPart->Next())!=0) {
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

ClassImp(HPhysicsEvent)
