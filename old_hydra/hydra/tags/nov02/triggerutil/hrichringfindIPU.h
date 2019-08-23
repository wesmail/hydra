#ifndef HRICHRINGFINDIPU_H
#define HRICHRINGFINDIPU_H

#include <TObject.h>
#include <TArrayI.h>
#include <TArrayS.h>
#include "richutildef.h"

class HRichAnalysisIPU;
class HRichHitIPU;
class HRichIPUParPattern;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HSpectrometer;
class HRuntimeDb;
// ****************************************************************************

class HRichRingFindIPU: public TObject {

// ****************************************************************************

 private:
  Bool_t pads[6][96][96]; //!
  Int_t Sector;//!
  
  Int_t lr, lc; //!
  Int_t label[96][96]; //!
  Int_t centers[96][96];//!
  Int_t ringsum[96][96];//!
  Int_t vetosum[96][96];//!
  Int_t nPads[96][96];//!
  Int_t labelPattern[64][2];//!  

 public:
  Int_t flag;//!
  Int_t label_size;//!

  HRichIPUParPattern *fRichIPUParPat; //!
  HRichIPUParThresholds *fRichIPUParThr; //!
  HRichIPUParLocMax *fRichIPUParLmax; //!

  Int_t getLabelPattern(Int_t aaa, Int_t bbb){return labelPattern[aaa][bbb];}
  void setLabelPattern(Int_t aaa, Int_t bbb, Int_t ccc){labelPattern[aaa][bbb]=ccc;}
  void makeLabelPattern(void);  

  void initParContainer(HSpectrometer *,HRuntimeDb *);

  Int_t iRingNr;//!
  HRichHitIPU *pRings;//!
  
  HRichRingFindIPU();//!
  virtual ~HRichRingFindIPU();//!
  
  Bool_t init(HRichAnalysisIPU*);//!
  
  HRichRingFindIPU(const HRichRingFindIPU& source);//!
  HRichRingFindIPU& operator=(const HRichRingFindIPU& source);//!
  
  Int_t Execute(void);//!
  
  void ZeroArray(void);//!
  inline void FiredPad(Int_t s, Int_t r, Int_t c) 
    { pads[s][r][c]=1; }
  inline void SetSector(Int_t s)
    { Sector = s; }
  
  void RingSearch();//!
  void MaxSearch();//!
  
  ClassDef(HRichRingFindIPU,1)
};

//============================================================================


#endif // HRICHRINGFINDIPU_H



























