#ifndef HSPECPARORAIO_H
#define HSPECPARORAIO_H

#include "hdetparoraio.h"

class HSpecGeomPar;

class HSpecParOraIo : public HDetParOraIo {
private:
  Int_t geomContextId;           // id of context for geometry
  Int_t targetType;              // id of target version
  Int_t alignmentVersion;        // alignment version
  Double_t targetSetupDate[2];   // date range for target setup 
  Double_t geomVersDate[2];      // date range for geometry
public:
  HSpecParOraIo(HOraConn* p=0);
  ~HSpecParOraIo() {}
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Bool_t read(HSpecGeomPar*,Int_t*);
  Int_t getTargetVersion(HSpecGeomPar*,Int_t);
  Int_t readTargetNames(HSpecGeomPar*);
  Int_t getAlignmentVersion(HSpecGeomPar*,Int_t);
  Int_t readAlignment(TList*);
  Int_t writeAlignment(HSpecGeomPar*);
  Int_t getTargetType(const char*);
  ClassDef(HSpecParOraIo,0) // Parameter I/O from Oracle for spectrometer
};

#endif  /* !HSPECPARORAIO_H */
