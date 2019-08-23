//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $Id: htriggerparrichmap.h,v 1.4 2008-05-09 16:18:42 halo Exp $

#ifndef HTRIGGERPARRICHMAP_H
#define HTRIGGERPARRICHMAP_H

#include <TObject.h>
#include <TObjArray.h>
#include "hlocation.h"
#include "hparset.h"


class HTriggerParRichMap : public HParSet {
private:
  Int_t row;                    // row number
  Int_t col;                    // column number

  Float_t rich_theta[96][96];   // theta map
  Float_t rich_phi[96][96];     // phi map

  void printPause(void);
public:
  HTriggerParRichMap(const Char_t* name="TriggerParRichMap",
                    const Char_t* title="Parameters for MU Rich Mapping ",
                    const Char_t* context="TriggerProduction");
  ~HTriggerParRichMap(void);

  Int_t getRowSize() {return 96;}
  Int_t getColSize() {return 96;}  
  Float_t getRichTheta(Int_t r, Int_t c){return rich_theta[r][c];}
  Float_t getRichPhi(Int_t r, Int_t c){return rich_phi[r][c];}
  void setRichTheta(Int_t r, Int_t c, Float_t sth){rich_theta[r][c]=sth;}
  void setRichPhi(Int_t r, Int_t c, Float_t sph){rich_phi[r][c]=sph;}
  
  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t*, Int_t, Int_t); 
  void setDefaults(void);
  ClassDef(HTriggerParRichMap,1)  // Container for the MU Rich Mapping parameters
};

#endif /* !HTRIGGERPARRICHMAP_H */











