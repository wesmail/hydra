//*-- Author: Ilse Koenig
//*-- Modified: Joern Wuestenfeld 07/14/2005
//*-- Version: $Id: htriggerparrootfileio.h,v 1.4 2005-07-15 09:00:25 wuestenf Exp $

#ifndef HTRIGGERPARROOTFILEIO_H
#define HTRIGGERPARROOTFILEIO_H

#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"

class HParRootFile;
class HParSet;
class HTriggerParMuLep;
class HTriggerParMuDilep;
class HTriggerParMomMap;
class HTriggerParShowerMap;
class HTriggerParRichMap;
class HRichIPUParPattern;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
 
class HTriggerParRootFileIo : public HDetParRootFileIo {
public:
  HTriggerParRootFileIo(HParRootFile* f);
  ~HTriggerParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HTriggerParMuLep*,Int_t*);
  Bool_t read(HTriggerParMuDilep*,Int_t*);
  Bool_t read(HTriggerParMomMap*,Int_t*);
  Bool_t read(HTriggerParShowerMap*,Int_t*);
  Bool_t read(HTriggerParRichMap*,Int_t*);
  Bool_t read(HRichIPUParPattern*,Int_t*);
  Bool_t read(HRichIPUParThresholds*,Int_t*);
  Bool_t read(HRichIPUParLocMax*,Int_t*);
 
  ClassDef(HTriggerParRootFileIo,0) // Class for TRIGGER parameter I/O from ROOT file
};

#endif  /*!HTRIGGERPARROOTFILEIO_H*/
