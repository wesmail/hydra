# ifndef HMDCLEVERARMGEOMETRY_H
# define HMDCLEVERARMGEOMETRY_H
using namespace std;

# include <fstream> 
# include "TArrayF.h"
# include "hparset.h"

class HParIo;


class HMdcLeverArmGeometry: public HParSet{
private:
  Short_t fNSectors;                   // number of sectos in setup
  Short_t fNLeverArms;                 // number of lever arms in setup
  TArrayF* fCosn;                      // cos of second module related to first
  TArrayF* fSinn;                      // sin of second module related to first
  TArrayF* fTraslation;                // traslation matrix

  TArrayF* fCosS;                     // cos of second module related to sector
  TArrayF* fSinS;                     // sin of second module related to sector
  TArrayF* fTraslationS;              // traslation vector to sector
  inline Int_t getIndex(Int_t nSect, Int_t nLever){return nSect*fNLeverArms + nLever;}

public:
  HMdcLeverArmGeometry(const Char_t* name="MdcLeverArmGeometry",
         const Char_t* title=
            "geometry container for the mdc reconstruction in one lever arm",
         const Char_t* context="");
  ~HMdcLeverArmGeometry(void);

  Bool_t init(HParIo* inp, Int_t* set);
  Int_t write(HParIo* output);

  Bool_t read(fstream *pFile, Int_t* set,Int_t inputNumber);
  Int_t writeFile(fstream* pFile);

  Short_t getNSectors(void){return fNSectors;}
  Short_t getNLeverArms(void){return fNLeverArms;}
  inline Float_t getTrasElement(Int_t sect, Int_t lever,Int_t index);
  inline Float_t getCosn(Int_t sect, Int_t lever);
  inline Float_t getSinn(Int_t sect, Int_t lever);
  inline Float_t getTrasElementS(Int_t sect, Int_t lever,Int_t index);
  inline Float_t getCosS(Int_t sect, Int_t lever);
  inline Float_t getSinS(Int_t sect, Int_t lever);
  
  Bool_t setTrasElement(Int_t sect, Int_t lever,Int_t index, Float_t element);
  Bool_t setCosn(Int_t sect, Int_t lever, Float_t cos);
  Bool_t setSinn(Int_t sect, Int_t lever, Float_t sin);
  Bool_t setTrasElementS(Int_t sect, Int_t lever,Int_t index, Float_t element);
  Bool_t setCosS(Int_t sect, Int_t lever, Float_t cos);
  Bool_t setSinS(Int_t sect, Int_t lever, Float_t sin);
ClassDef(HMdcLeverArmGeometry,1)
};

inline Float_t HMdcLeverArmGeometry :: getCosn(Int_t sect, Int_t lever){
    return fCosn->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getSinn(Int_t sect, Int_t lever){
    return fSinn->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getTrasElement(Int_t sect, Int_t lever, Int_t index){
    return fTraslation->At(getIndex(sect,lever)*3+index);
}

inline Float_t HMdcLeverArmGeometry :: getCosS(Int_t sect, Int_t lever){
    return fCosS->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getSinS(Int_t sect, Int_t lever){
    return fSinS->At(getIndex(sect,lever));
}

inline Float_t HMdcLeverArmGeometry :: getTrasElementS(Int_t sect, Int_t lever, Int_t index){
    return fTraslationS->At(getIndex(sect,lever)*3+index);
}

# endif           /* !HMDCLEVERARMGEOMETRY_H */
