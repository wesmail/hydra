#ifndef HHODOPARORAIO_H
#define HHODOPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HHodoDetector;
class HHodoCalPar;
class HHodoRefHitPar;

class HHodoParOraIo : public HDetParOraIo {
private:
  Int_t numModules;             // number of modules found in Oracle
  Int_t runStart;               // start time of actual run
  TArrayI* hodoIds;             // array of modules in the actual setup
  TArrayI* initModules;         // array of modules initialized from Oracle
public:
  HHodoParOraIo(HOraConn* p=0);
  ~HHodoParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Int_t startIo(HParSet*);
  Int_t readIds();
  Int_t getVersion(HParSet*,Int_t*,Int_t*);
  Int_t getVersion(HParSet*);
  Bool_t read(HHodoCalPar*,Int_t*);
  Bool_t read(HHodoRefHitPar*,Int_t*);
  Int_t createVers(HParSet*);
  Int_t writePar(HHodoCalPar*);
  Int_t writePar(HHodoRefHitPar*);
  ClassDef(HHodoParOraIo,0) // HODO detector parameter I/O from Oracle
};

#endif  /* !HHODOPARORAIO_H */
