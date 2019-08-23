//*-- Author : Dan Magestro
//*-- Created: 01/09/01
//*-- Last modified: 07/11/01
#ifndef HPHYSICSEVENT_H
#define HPHYSICSEVENT_H

#include "TObject.h"
#include "iostream.h"
#include "hlocation.h"
#include "hphysicsconstants.h"

class HIterator;
class HCategory;
class TObjArray;
class TClonesArray;

class HPhysicsEvent : public TObject {

protected:
  HCategory* fPartCat;          //! Pointer to Particle data category
  TClonesArray* fPartArray;     //! Pointer to Particle data TClonesArray
  TIterator* iterPart;          //! Iterator over Particle category

  TObjArray *data[50];         //! Utility TObjArray array of pointers
  Int_t count;                 //! Counter for utility TObjArray's

public:
  HPhysicsEvent(void);
  HPhysicsEvent(HCategory *cat);
  HPhysicsEvent(TClonesArray *array);
  ~HPhysicsEvent(void) {;}

  Int_t  execute(void){return 0;}

  Float_t sumPt(TObjArray *data);
  Float_t sumPt(Int_t pid=-1) {return sumPt(getParticles(pid));};
  Float_t sumPt(Char_t *pid)  {return sumPt(getParticles(pid));};

  Float_t sumEt(TObjArray *data);
  Float_t sumEt(Int_t pid=-1) {return sumEt(getParticles(pid));};
  Float_t sumEt(Char_t *pid)  {return sumEt(getParticles(pid));};

  Float_t sumMt(TObjArray *data);
  Float_t sumMt(Int_t pid=-1) {return sumMt(getParticles(pid));};
  Float_t sumMt(Char_t *pid)  {return sumMt(getParticles(pid));};

  Int_t   mult(TObjArray *data);
  Int_t   mult(Int_t pid=-1)  {return mult(getParticles(pid));};
  Int_t   mult(Char_t *pid)   {return mult(getParticles(pid));};

  void printEvent();

  TObjArray *getParticles(Int_t pid=-1);
  TObjArray *getParticles(Char_t *pid);

public:
  ClassDef(HPhysicsEvent,1) // Calculates event-wise variables or returns subsets of HParticles
};

#endif
