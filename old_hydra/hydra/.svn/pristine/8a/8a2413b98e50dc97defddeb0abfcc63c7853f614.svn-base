//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#ifndef HRICHIPUPARPATTERN_H
#define HRICHIPUPARPATTERN_H

#include "hparset.h"

class HParIO;
class TString;

class HRichIPUParPattern : public HParSet
{
private:
  Int_t NGroups;
  Int_t NPadsinGroup;
  Int_t NPadDimensions;

  Int_t pattern[29][5][2];  

public:
  HRichIPUParPattern(const Char_t* name="RichIPUParPattern",
                    const Char_t* title="Parameters for Rich IPU Pattern ",
                    const Char_t* context="RICHIPUEmulation");
  ~HRichIPUParPattern(void);

  Int_t getNGroups(void){return NGroups;}
  Int_t getNPadsinGroup(void){return NPadsinGroup;}
  Int_t getNPadDimensions(void){return NPadDimensions;}

  void setNGroups(Int_t gr) {NGroups=gr;}
  void setNPadsinGroup(Int_t pg) {NPadsinGroup=pg;}
  void setNPadDimensions(Int_t re) {NPadDimensions=re;}

  Int_t getPattern(Int_t gr, Int_t pg, Int_t re){return pattern[gr][pg][re];}
  void setPattern(Int_t gr, Int_t pg, Int_t re, Int_t pat){pattern[gr][pg][re]=pat;}

  Bool_t init(HParIo*,Int_t*);
  Int_t write(HParIo*);
  void clear();
  void printParam();
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(Char_t*, Int_t, Int_t, Int_t);
  Bool_t writesize(Char_t *buf);
  void setDefaults(void);

  ClassDef(HRichIPUParPattern,1)  // Container for the RICH IPU Pattern parameters
};

#endif /* !HRICHIPUPARPATTERN_H */
