# ifndef HMDCTARGETGEOMETRY_H
# define HMDCTARGETGEOMETRY_H
using namespace std;
# include <fstream> 
# include "TArrayF.h"
# include "hpario.h"
# include "hparset.h"


class HMdcTargetGeometry : public HParSet{       
private:
  Short_t fNSectors, fNModules;    // number of sectors and modules in the setup
  Double_t fXDim;                 // x dimension
  Double_t fYDim;                 // y dimension

  TArrayF* fZPos;                 // zposition of the target relative to each
                                 // mdc chamber in the MDC reference system 

  inline Int_t getIndex(Int_t nSect, Int_t nMod){return nSect*fNModules+nMod;}

public:
  HMdcTargetGeometry(const char* name="MdcTargetGeometry",
              const char* title="target geometry for mdc reconstruction",
              const char* context="");
  ~HMdcTargetGeometry(void);

  Bool_t init(HParIo* inp, Int_t* set);
  Int_t write(HParIo* output);

  Bool_t read(fstream *pFile, Int_t* set, Int_t inputNumber);
  Int_t writeFile(fstream *pFile);

  Short_t  getNSectors(void){return fNSectors;}
  Short_t  getNModules(void){return fNModules;}
  Double_t getXDim(void){return fXDim;}
  Double_t getYDim(void){return fYDim;}
  Double_t getZPos(Int_t sect, Int_t mod);

  void setNSectors(Short_t sectors){fNSectors = sectors;}
  void setNModules(Short_t modules){fNModules = modules;}
  void setXDim(Double_t dim){fXDim = dim;}
  void setYDim(Double_t dim){fYDim = dim;}
  Bool_t setZPos(Int_t sect, Int_t mod, Double_t z);
				     

ClassDef(HMdcTargetGeometry,1)
};





# endif        /* !HMDCTARGETGEOMETRY_H*/






