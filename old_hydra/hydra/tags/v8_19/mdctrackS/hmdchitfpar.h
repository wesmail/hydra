#ifndef HMDCHITFPAR_H
#define HMDCHITFPAR_H

# include <fstream>
# include "TArrayF.h"
# include "TArrayI.h"
# include "hpario.h"
# include "hparset.h"


class HMdcHitFPar : public HParSet{

private:

  Short_t fNSectors, fNModules;      // nbr. of sectors and modules
  TArrayI* fMinNumHits;             // minimum number of hits in a segment
  TArrayI* fMaxNumCommonHits;       // maximum number of common hits
  TArrayF* fMaxChi;                 // maximum chi square allowed
  TArrayF* fRoad;                   // road to search hits
  TArrayF* fMaxSlopeX;              // maximum slope in x
  TArrayF* fMinSlopeX;              // minimum slope in x
  TArrayF* fMaxSlopeY;              // maximum slope in y
  TArrayF* fMinSlopeY;              // minimum slope in y
  TArrayF* fDistZAxis;              // maximum distance to z axis
  TArrayF* fZHit;                   // z coordinate at which results of fit are given
  
  inline Int_t getIndex(Int_t nSect, Int_t nMod){return nSect*fNModules+nMod;}

public:

  HMdcHitFPar(const Char_t* name="MdcHitFPar",
              const Char_t* title="parameters for Mdc hit finder HMdcHitF",
              const Char_t* context="");
  ~HMdcHitFPar(void);
  Bool_t init(HParIo* input, Int_t* set);
  Int_t write(HParIo* output);

  Bool_t read(fstream* pFile, Int_t* set, Int_t inputNumber);
  Int_t writeFile(fstream *pFile);

  Short_t getNSectors(void){return fNSectors;}
  Short_t getNModules(void){return fNModules;}

  inline Int_t getMinNumHits(Int_t sect, Int_t mod);
  inline Int_t getMaxNumCommonHits(Int_t sect, Int_t mod);
  inline Float_t getMaxChi(Int_t sect, Int_t mod);
  inline Float_t getRoad(Int_t sect, Int_t mod);
  inline Float_t getMaxSlopeX(Int_t sect, Int_t mod);
  inline Float_t getMinSlopeX(Int_t sect, Int_t mod);
  inline Float_t getMaxSlopeY(Int_t sect, Int_t mod);
  inline Float_t getMinSlopeY(Int_t sect, Int_t mod);
  inline Float_t getDistZAxis(Int_t sect, Int_t mod);
  inline Float_t getZHit(Int_t sect, Int_t mod);


  void setNSectors(Short_t sectors){fNSectors = sectors;}
  void setNModules(Short_t modules){fNModules = modules;}

  Bool_t setMinNumHits(Int_t nSect, Int_t nMod, Int_t num);
  Bool_t setMaxNumCommonHits(Int_t nSect, Int_t nMod, Int_t num);
  Bool_t setMaxChi(Int_t nSect, Int_t nMod, Float_t chi);
  Bool_t setRoad(Int_t nSect, Int_t nMod, Float_t dist);
  Bool_t setMaxSlopeX(Int_t nSect, Int_t nMod, Float_t slope);
  Bool_t setMinSlopeX(Int_t nSect, Int_t nMod, Float_t slope);
  Bool_t setMaxSlopeY(Int_t nSect, Int_t nMod, Float_t slope);
  Bool_t setMinSlopeY(Int_t nSect, Int_t nMod, Float_t slope);
  Bool_t setDistZAxis(Int_t nSect, Int_t nMod, Float_t distance);
  Bool_t setZHit(Int_t nSect, Int_t nMod, Float_t z);

ClassDef(HMdcHitFPar,1)
};

  inline Int_t HMdcHitFPar :: getMinNumHits(Int_t sect, Int_t mod){
    return fMinNumHits->At(getIndex(sect,mod));
  }

  inline Int_t HMdcHitFPar ::getMaxNumCommonHits(Int_t sect, Int_t mod){
    return fMaxNumCommonHits->At(getIndex(sect, mod));
  }

  inline Float_t HMdcHitFPar :: getMaxChi(Int_t sect, Int_t mod){
    return fMaxChi->At(getIndex(sect, mod));
  }

  inline Float_t HMdcHitFPar :: getRoad(Int_t sect, Int_t mod){
    return fRoad->At(getIndex(sect,mod));
  }

  inline Float_t HMdcHitFPar :: getMaxSlopeX(Int_t sect, Int_t mod){
    return fMaxSlopeX->At(getIndex(sect,mod));
  }

  inline Float_t HMdcHitFPar :: getMinSlopeX(Int_t sect, Int_t mod){
    return fMinSlopeX->At(getIndex(sect,mod));
  }

  inline Float_t HMdcHitFPar :: getMaxSlopeY(Int_t sect, Int_t mod){
    return fMaxSlopeY->At(getIndex(sect,mod));
  }

  inline Float_t HMdcHitFPar :: getMinSlopeY(Int_t sect, Int_t mod){
    return fMinSlopeY->At(getIndex(sect,mod));
  }

  inline Float_t HMdcHitFPar :: getDistZAxis(Int_t sect, Int_t mod){
    return fDistZAxis->At(getIndex(sect,mod));
  }

  inline Float_t HMdcHitFPar :: getZHit(Int_t sect, Int_t mod){
    return fZHit->At(getIndex(sect,mod));
  }

#endif
