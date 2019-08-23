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
  HMdcLeverArmGeometry(const char* name="MdcLeverArmGeometry",
         const char* title=
            "geometry container for the mdc reconstruction in one lever arm",
         const char* context="");
  ~HMdcLeverArmGeometry(void);

  Bool_t init(HParIo* inp, Int_t* set);
  Int_t write(HParIo* output);

  Bool_t read(fstream *pFile, Int_t* set,Int_t inputNumber);
  Int_t writeFile(fstream* pFile);

  Short_t getNSectors(void){return fNSectors;}
  Short_t getNLeverArms(void){return fNLeverArms;}
  Float_t getTrasElement(Int_t sect, Int_t lever,Int_t index);
  Float_t getCosn(Int_t sect, Int_t lever);
  Float_t getSinn(Int_t sect, Int_t lever);
  Float_t getTrasElementS(Int_t sect, Int_t lever,Int_t index);
  Float_t getCosS(Int_t sect, Int_t lever);
  Float_t getSinS(Int_t sect, Int_t lever);
  
  Bool_t setTrasElement(Int_t sect, Int_t lever,Int_t index, Float_t element);
  Bool_t setCosn(Int_t sect, Int_t lever, Float_t cos);
  Bool_t setSinn(Int_t sect, Int_t lever, Float_t sin);
  Bool_t setTrasElementS(Int_t sect, Int_t lever,Int_t index, Float_t element);
  Bool_t setCosS(Int_t sect, Int_t lever, Float_t cos);
  Bool_t setSinS(Int_t sect, Int_t lever, Float_t sin);



ClassDef(HMdcLeverArmGeometry,1)
};

# endif           /* !HMDCLEVERARMGEOMETRY_H */
