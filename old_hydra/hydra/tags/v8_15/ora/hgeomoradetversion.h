#ifndef HGEOMORADETVERSION_H
#define HGEOMORADETVERSION_H

#include "TNamed.h"

class HGeomOraDetVersion : public TNamed {
  Int_t detectorId;       // Id of detector
  Int_t geomVersion;      // geometry version
  Double_t versDate[2];   // Actual date range of version
public:
  HGeomOraDetVersion(const char* name=0,Int_t id=-1);
  ~HGeomOraDetVersion() {}
  Int_t getDetectorId() {return detectorId;}
  Int_t getGeomVersion() {return geomVersion;}
  Double_t getSince() {return versDate[0];}
  Double_t getUntil() {return versDate[1];}
  void setDetectorId(Int_t v) {detectorId=v;}
  void setGeomVersion(Int_t v) {geomVersion=v;}
  void setSince(Double_t v) {versDate[0]=v;}  
  void setUntil(Double_t v) {versDate[1]=v;}
  void fill(Int_t,Int_t,Double_t,Double_t);  
  void fill(Int_t,Double_t,Double_t);  
  void clearVersDate();
  ClassDef(HGeomOraDetVersion,0) // Class for geometry version management in Oracle
};

#endif  /* !HGEOMORADETVERSION_H */
