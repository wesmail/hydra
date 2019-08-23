#ifndef HSHOWERPARORAIO_H
#define HSHOWERPARORAIO_H

#include "hdetparoraio.h"
#include "TArrayI.h"
#include "TString.h"

class HOraConn;
class HShowerDetector;
class HShowerCalPar;
class HShowerAnalysisPar;
class HShowerUnpackParam;
class HShowerHitFPar;
class HShowerGeometry;
class HShowerDigiDetPar;

class HShowerParOraIo : public HDetParOraIo {
private:
  TArrayI* m_pShowerIds;      // id-numbers of all Shower modules
  TArrayI* m_pInitModules;    // array of modules initialized from Oracle
  Int_t m_nMaxModules;        // maximum number of modules in a sector     
  
  
  Int_t m_nRunStart;            // start time of actual run
  
public:
  HShowerParOraIo(HOraConn* p=0);
  ~HShowerParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HShowerCalPar*,      Int_t*);
  Bool_t read(HShowerUnpackParam*, Int_t*); 
  Bool_t read(HShowerHitFPar*,     Int_t*);
  Bool_t read(HShowerGeometry*,    Int_t*);
  Bool_t read(HShowerDigiDetPar*,  Int_t*);
  Int_t write(HParSet*);
  Bool_t write(HShowerDetector* p) {return kTRUE;}
  
  Int_t writePar(HShowerCalPar*);
  Int_t writePar(HShowerUnpackParam*);
  Int_t writePar(HShowerHitFPar*);
  Int_t writePar(HShowerDigiDetPar*);
  
  void printInfo(Text_t*);
  Bool_t startIo(void);
private:
  Int_t getParVersion(HParSet *pPar,Int_t *version);
  Int_t createVers(HParSet *pPar);
  Bool_t readModGeomNames(HShowerGeometry*,Int_t*);
  Bool_t readCompGeomNames(HShowerGeometry*,Int_t*);
  Bool_t readGeom(HShowerGeometry *pPar);
  Bool_t fillGeom(HShowerGeometry *pPar);
  void clearVersDate(Double_t*);
  Int_t readIds();
  void printIds();
  ClassDef(HShowerParOraIo,0) // SHOWER parameter I/O from Oracle
};

#endif  /* !HSHOWERPARORAIO_H */
