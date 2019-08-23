#ifndef HCONDPARORAIO_H
#define HCONDPARORAIO_H

#include "hdetparoraio.h"

class HOraConn;
class HParSet;
class HParCond;

class HCondParOraIo : public HDetParOraIo {
public:
  HCondParOraIo(HOraConn* p=0);
  ~HCondParOraIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
  Bool_t readFromLoadingTable(HParCond*,Int_t);
private:
  Bool_t readLoadBlob(HParamBinObj*,Int_t);
  ClassDef(HCondParOraIo,0) // I/O from Oracle for parameter containers derived from HParCond
};

#endif  /* !HCONDPARORAIO_H */

