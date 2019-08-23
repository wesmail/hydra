#ifndef HTRIGGERPARORAIO_H
#define HTRIGGERPARORAIO_H

#include "hdetparoraio.h"

class HTriggerParRichMap;
class HTriggerParShowerMap;
class HTriggerParMomMap;
class HRichIPUParThresholds;
class HRichIPUParPattern;

class HTriggerParOraIo : public HDetParOraIo {
private:
  Int_t runStart;    // start time of current run
public:
  HTriggerParOraIo(HOraConn* p=0);
  ~HTriggerParOraIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Int_t startIo(HParSet*);
  Int_t getVersion(HParSet*,Int_t&);
  Bool_t read(HTriggerParRichMap*);
  Bool_t read(HTriggerParShowerMap*);
  Bool_t read(HTriggerParMomMap*);
  Int_t readArray(HTriggerParMomMap*,Int_t);
  Bool_t read(HRichIPUParThresholds*);
  Bool_t read(HRichIPUParPattern*);
  Int_t readArray(HRichIPUParPattern*,Int_t);
  Int_t createVersion(HParSet*);
  Int_t writePar(HTriggerParRichMap*);
  Int_t writePar(HTriggerParShowerMap*);
  Int_t writePar(HTriggerParMomMap*);
  Int_t writeArray(HTriggerParMomMap*,Int_t);
  Int_t writePar(HRichIPUParThresholds*);
  Int_t writePar(HRichIPUParPattern*);
  Int_t writeArray(HRichIPUParPattern*,Int_t);
  ClassDef(HTriggerParOraIo,0) // Parameter I/O from Oracle for TRIGGER
};

#endif  /* !HTRIGGERPARORAIO_H */
