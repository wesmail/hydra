# ifndef HMDCMODULEGEOMETRY_H
# define HMDCMODULEGEOMETRY_H
using namespace std;
# include <fstream> 
# include "hparset.h"
# include "TArrayF.h"
# include "TArrayI.h"


class TArrayF;
class TArrayI;
class HParIo;


class HMdcModuleGeometry : public HParSet{       
private:
  Short_t fNSectors, fNModules; // number of sectors and modules
  Short_t fNLayers;            // number of layers
  TArrayF* fZPlane;            // plane coordinates
  TArrayF* fWireOrient;        // angle of wires
  TArrayF* fCosn;              // cos of the angle of each plane
  TArrayF* fSinn;              // sin of the angle of each plane
  TArrayF* fSgpln;             // error coordinate
  TArrayF* fPitch;             // pitch of each layer
  TArrayF* fCatDistance;       // distance between cathode planes (in cm)
  TArrayF* fFirstWirePos;      // position of the first wire in each layer
  TArrayF* fCentralWire;       // number of central wire in layer
  TArrayI* fMaxNumCells;       // maximum number of cells in each layer

  inline Int_t getIndex(Int_t nSect, Int_t nMod, Int_t nLay){return nSect*fNModules*fNLayers+nMod*fNLayers+nLay;}

public:
  HMdcModuleGeometry(const char* name="MdcModuleGeometry",
               const char* title="geometrical parameters for one MDC module",
               const char* context="");
  ~HMdcModuleGeometry(void);
  Bool_t init(HParIo* inp, Int_t* set);
  Int_t write(HParIo* output);

  Bool_t read(fstream* pFile, Int_t* set, Int_t inputNumber);
  Int_t writeFile(fstream* pFile);

  TArrayF* getZPlane(void){return fZPlane;}
  TArrayF* getWireOrient(void){return fWireOrient;}
  TArrayF* getCosn(void){return fCosn;}
  TArrayF* getSinn(void){return fSinn;}
  TArrayF* getSgpln(void){return fSgpln;}
  TArrayF* getPitch(void){return fPitch;}
  TArrayF* getFirstWirePos(void){return fFirstWirePos;}
  TArrayF* getCentralWire(void){return fCentralWire;}
  TArrayI* getMaxNumCells(void){return fMaxNumCells;}
  TArrayF* getCatDistance(void){return fCatDistance;}

  Short_t getNSectors(void){return fNSectors;}
  Short_t getNModules(void){return fNModules;}
  Short_t getNLayers(void){return fNLayers;}

  inline Float_t getZPlane(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getWireOrient(Int_t sect,Int_t mod,Int_t layer);
  inline Float_t getCosn(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getSinn(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getSgpln(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getPitch(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getFirstWirePos(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getCentralWire(Int_t sect, Int_t mod, Int_t layer);
  inline Int_t   getMaxNumCells(Int_t sect, Int_t mod, Int_t layer);
  inline Float_t getCatDistance(Int_t sect, Int_t mod, Int_t layer);

  Bool_t setZPlane(Int_t sector, Int_t module, Int_t layer, Float_t z);
  Bool_t setWireOrient(Int_t sector,Int_t module, Int_t layer, Float_t angle);
  Bool_t setCosn(Int_t sector, Int_t module, Int_t layer, Float_t cos);
  Bool_t setSinn(Int_t sector, Int_t module, Int_t layer, Float_t sin);
  Bool_t setSgpln(Int_t sector, Int_t module, Int_t layer, Float_t sigma);
  Bool_t setPitch(Int_t sector, Int_t module, Int_t layer, Float_t dist);
  Bool_t setFirstWirePos(Int_t sector, Int_t module, Int_t layer, Float_t pos);
  Bool_t setCentralWire(Int_t sector, Int_t module, Int_t layer, Float_t num);
  Bool_t setMaxNumCells(Int_t sector, Int_t module, Int_t layer, Int_t num);
  Bool_t setCatDistance(Int_t sector, Int_t module, Int_t layer, Float_t dist);

  void setNSectors(Short_t sect){fNSectors = sect;}
  void setNModules(Short_t mod){fNModules = mod;}
  void setNLayers(Short_t layers){fNLayers = layers;}

  ClassDef(HMdcModuleGeometry,1) // geometry of one mdc module
};     

inline Float_t HMdcModuleGeometry :: getZPlane(Int_t sect, Int_t mod, Int_t layer){
  return fZPlane->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry :: getWireOrient(Int_t sect, Int_t mod, Int_t layer){
  return fWireOrient->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry :: getCosn(Int_t sect, Int_t mod, Int_t layer){
  return fCosn->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry :: getSinn(Int_t sect, Int_t mod, Int_t layer){
  return fSinn->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry :: getSgpln(Int_t sect, Int_t mod, Int_t layer){
  return fSgpln->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry :: getPitch(Int_t sect, Int_t mod, Int_t layer){
  return fPitch->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry ::getFirstWirePos(Int_t sect,Int_t mod,Int_t layer){
  return fFirstWirePos->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry ::getCentralWire(Int_t sect,Int_t mod,Int_t layer){
  return fCentralWire->At(getIndex(sect,mod,layer));
}

inline Int_t  HMdcModuleGeometry ::getMaxNumCells(Int_t sect, Int_t mod, Int_t layer){
  return fMaxNumCells->At(getIndex(sect,mod,layer));
}

inline Float_t HMdcModuleGeometry ::getCatDistance(Int_t sect,Int_t mod,Int_t layer){
  return fCatDistance->At(getIndex(sect,mod,layer));
}

# endif           /* !HMDCMODULEGEOMETRY_H*/






