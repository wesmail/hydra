#ifndef HSPECPARORAIO_H
#define HSPECPARORAIO_H

#include "hdetparoraio.h"
#include "TList.h"

class HSpecGeomPar;
class HGeomOraDetVersion;
class HMagnetPar;
class HScsPar;
class HScsChannel;
class HTrbLookup;

class HSpecParOraIo : public HDetParOraIo {
private:
  HGeomOraDetVersion* geomCaveVers;   // Geometry version and range for the cave
  HGeomOraDetVersion* geomSectorVers; // Geometry version and range for the sectors
  HGeomOraDetVersion* geomTargetVers; // Geometry version and range for the targets
  HParOraSet* alignTargetVers;        // Alignment version and range for the targets
public:
  HSpecParOraIo(HOraConn* p=0);
  ~HSpecParOraIo();
  Bool_t init(HParSet*,Int_t*);
  Int_t write(HParSet*);
private:
  Bool_t read(HSpecGeomPar*,Int_t*);
  Bool_t readTargetVolumes(HSpecGeomPar*,TList&);
  Bool_t read(HMagnetPar*);
  Bool_t read(HScsPar*);
  Int_t getVersion(HParSet*);
  Bool_t read(HTrbLookup*);
  Int_t readScsPeriod(Int_t,TString&,TString&,Int_t&);
  Bool_t readScsSummary(HScsPar*,Int_t,const char*,const char*,Int_t);
  Bool_t readScsRawF(HScsChannel*,Int_t,Int_t,char*,char*,Int_t);
  Bool_t readScsRawI(HScsChannel*,Int_t,Int_t,char*,char*,Int_t);
  Int_t writeAlignment(HSpecGeomPar*);
  Int_t createVers(HParSet*);
  Int_t writeTrbLookup(HTrbLookup*);
  Bool_t checkFrontEndAddresses(Int_t);
  void stringToDate(const char*,UInt_t&,UInt_t&);
  ClassDef(HSpecParOraIo,0) // Parameter I/O from Oracle for spectrometer
};

#endif  /* !HSPECPARORAIO_H */
