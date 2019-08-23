#ifndef HRICHPARORAIO_H
#define HRICHPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "TString.h"

class HOraConn;
class HRichDetector;
class HParSet;
class HRichCalPar;
class HRichGeometryPar;
class HRichMappingPar;

class HRichParOraIo : public HDetParOraIo {
private: 
  TArrayI* richIds;        // id-numbers of all Rich sectors
  TArrayI* initModules;    // array of modules initialized from Oracle
public:
  HRichParOraIo(HOraConn* p=0);
  ~HRichParOraIo();
  Bool_t init(HParSet*,Int_t*); 
    Bool_t read(HRichCalPar*,Int_t*); 
    Bool_t read(HRichGeometryPar*,Int_t*); 
    Bool_t read(HRichMappingPar*,Int_t*); 
    Int_t write(HParSet*); 
    Bool_t write(HRichDetector* p) {return kTRUE;} 
    Int_t writePar(HRichCalPar*); 
    Int_t writePar(HRichGeometryPar*); 
    Int_t writePar(HRichMappingPar*); 
  Int_t startIo(void); 
  void printIds(); 
  void printInfo(Text_t*); 
private:
  Int_t readIds();   
  Int_t createCalparVers(HRichCalPar*); 
  Int_t getVersion(HParSet* pPar,Int_t* set,Int_t* versions); 
  ClassDef(HRichParOraIo,0) // RICH parameter I/O from Oracle
};

#endif  /* !HRICHPARORAIO_H */
