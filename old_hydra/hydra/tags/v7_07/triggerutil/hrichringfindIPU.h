//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/13/2005
//*-- Version: $Id: hrichringfindIPU.h,v 1.5 2005-07-15 09:02:50 wuestenf Exp $

#ifndef HRICHRINGFINDIPU_H
#define HRICHRINGFINDIPU_H

#include "TObject.h"

class HRichAnalysisIPU;
class HRichIPUParPattern;
class HRichIPUParThresholds;
class HRichIPUParLocMax;
class HSpectrometer;
class HRuntimeDb;
class HTriggerSetup;
class TObjArray;
// ****************************************************************************

class HRichRingFindIPU : public TObject
{

// ****************************************************************************
protected:
  HRichIPUParPattern *fRichIPUParPat;     //!
  HRichIPUParThresholds *fRichIPUParThr;  //!
  HRichIPUParLocMax *fRichIPUParLmax;     //!
  HTriggerSetup *fRichSetup;              //! Task setup parameters

  Bool_t pads[6][96][96];  //!
  Int_t Sector;            //!
  Int_t flag;              //!
  Int_t label_size;        //!

  Int_t iRingNr;           //!
  TObjArray *pRings;       //!

  Int_t lr, lc;              //!
  Int_t label[96][96];       //!
  Int_t centers[96][96];     //!
  Int_t ringsum[96][96];     //!
  Int_t vetosum[96][96];     //!
  Int_t nPads[96][96];       //!
  Int_t labelPattern[64][2]; //!
  Int_t version;             //!

  Bool_t initParContainer(HSpectrometer *,HRuntimeDb *);
  virtual void fillHit(Int_t row, Int_t column, Int_t quality, Int_t vetoQuality, Int_t nPads);

public:
  HRichRingFindIPU(void);
  virtual ~HRichRingFindIPU(void);

  Bool_t init(HRichAnalysisIPU*);

  HRichRingFindIPU(const HRichRingFindIPU& source);
  HRichRingFindIPU& operator=(const HRichRingFindIPU& source);

  Int_t Execute(void);

  Int_t getLabelPattern(Int_t aaa, Int_t bbb){return labelPattern[aaa][bbb];};
  void setLabelPattern(Int_t aaa, Int_t bbb, Int_t ccc){labelPattern[aaa][bbb]=ccc;};
  void makeLabelPattern(void);  

  const char* checkParamContext(void);


  void ZeroArray(void);
  inline void FiredPad(Int_t s, Int_t r, Int_t c)
    { pads[s][r][c]=1; };
  inline void SetSector(Int_t s)
    { Sector = s; };

  TObjArray *getRings(void){return pRings;};

  void RingSearch(void);
  void MaxSearch(void);

  ClassDef(HRichRingFindIPU,1)  // RICH ring finder offline algorithm for IPU emulation
};

//============================================================================


#endif // HRICHRINGFINDIPU_H
