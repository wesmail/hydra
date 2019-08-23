#if !defined HTOFSECTOR_H
#define HTOFSECTOR_H

#include "TLine.h"
#include "TAttFill.h"

class HTofSector : public TObject, public TAttFill {

 protected:
  Int_t number;

  TLine *line1;
  TLine *line2;
  TLine *line3;
  TLine *line4;

 public:
  HTofSector();
  HTofSector(Int_t sec);
  virtual ~HTofSector() {}
  Int_t getNumber() const {return number;}
  void Paint(Option_t* option);

 public:
  ClassDef(HTofSector,0)

};

#endif

