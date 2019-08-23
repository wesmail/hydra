#if !defined HTOFROD_H
#define HTOFROD_H

#include "TLine.h"
#include "TAttFill.h"

class HTofRod : public TObject, public TAttFill {

 protected:
  
  Int_t sector;
  Int_t module;
  Int_t strip;

  TLine *line1;
  TLine *line2;
  TLine *line3;
  TLine *line4;

  Bool_t kActive;

 public:

  virtual Int_t DistancetoPrimitive(Int_t px, Int_t py);
  virtual void ExecuteEvent(Int_t event, Int_t px, Int_t py);  

 public:

  HTofRod();
  HTofRod(Int_t sec, Int_t mod, Int_t rod);
  virtual ~HTofRod() {}
  Int_t getSector() const {return sector;}
  Int_t getModule() const {return module;}
  Int_t getStrip() const {return strip;}
  void Paint(Option_t* option);
  Bool_t getStatus() {return kActive;}

 public:

  ClassDef(HTofRod,0)

};

#endif



