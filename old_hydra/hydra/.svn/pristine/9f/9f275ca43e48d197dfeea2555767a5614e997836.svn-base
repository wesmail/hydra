#ifndef HGEOMINTERFACE_H
#define HGEOMINTERFACE_H

#include "TObjArray.h"
#include "TList.h"

class HGeomIo;
class HGeomSet;
class HGeomMedia;
class HGeomShapes;
class HGeomBuilder;
class HSpecGeomPar;
class HDetGeomPar;

enum EHGeoDetPart {
  kHGeomCave   =  0,
  kHGeomRich   =  1,
  kHGeomTarget =  2,
  kHGeomSect   =  3,
  kHGeomMdc    =  4,
  kHGeomCoils  =  5,
  kHGeomTof    =  6,
  kHGeomShower =  7,
  kHGeomFrames =  8,
  kHGeomStart  =  9,
  kHGeomFWall  = 10,
  kHGeomRpc    = 11,
  kHGeomUser   = 12 };


class HGeomInterface : public TObject { 
  HGeomIo*      fileInput;   // ASCII file I/O
  HGeomIo*      oraInput;    // Oracle input
  HGeomIo*      output;      // Oracle output
  Int_t         nSets;       // number of geometry sets (detector parts) 
  Int_t         nActualSets; // number of set in actual geometry
  Int_t         nFiles;      // number of geometry and hit files  
  TObjArray*    sets;        // array of geometry  sets
  HGeomMedia*   media;       // list of media
  HGeomShapes*  shapes;      // list of shapes
  TList*        masterNodes; // list of mother nodes used by several sets
  TString       setupFile;   // file with detector setups (subsets)
  HGeomBuilder* geoBuilder;  // actually used geometry builder
public:
  HGeomInterface();
  ~HGeomInterface();
  void setOracleInput(HGeomIo* p) {oraInput=p;}
  void setOutput(HGeomIo* p) {output=p;}
  void setGeomBuilder(HGeomBuilder* p) {geoBuilder=p;}
  HGeomIo* getFileInput() { return fileInput; }
  HGeomIo* getOraInput() { return oraInput; }
  HGeomIo* getOutput() { return output; }
  HGeomShapes* getShapes() { return shapes; }
  HGeomMedia* getMedia() { return media; }
  void addInputFile(const char*);
  Bool_t addAlignment(HSpecGeomPar*);
  Bool_t addAlignment(HDetGeomPar*);
  HGeomSet* findSet(const char*);
  Bool_t readSet(HGeomSet*);
  Bool_t writeSet(HGeomSet*);
  Bool_t writeSet(HGeomSet*,const char*);
  Bool_t createSet(HGeomSet*);
  void deleteSet(HGeomSet* pSet);
  Bool_t readMedia();
  Bool_t writeMedia();
  Bool_t writeMedia(const char*);
  Bool_t readAll();
  Bool_t writeAll();
  Bool_t createAll(Bool_t withCleanup=kFALSE);
  Bool_t createGeometry(Bool_t withCleanup=kFALSE);
  Bool_t readGeomConfig(const char*);
  void addSetupFile(const char* f) {setupFile=f;}
  Bool_t readSetupFile();
  void print();
private:
  HGeomIo* connectInput(const char*);
  Bool_t connectOutput(const char*);
  ClassDef(HGeomInterface,0) // Class to manage geometry for simulations
};

#endif /* !HGEOMINTERFACE_H */
