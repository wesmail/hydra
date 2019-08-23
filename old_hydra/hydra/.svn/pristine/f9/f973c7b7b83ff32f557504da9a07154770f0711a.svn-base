#if !defined HPARAMETER_H
#define HPARAMETER_H

//-------------------------------------------------------------------------
//
// File name:       hparameter.h
// Subject:         Extraction of parameter values.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the classes HDefaultParameter and
// HUserParameter.
// These classes extract parameter values from the data categories.
//
//-------------------------------------------------------------------------

#include "TFormula.h"

enum TofRawPar      { TOF_RAW_CELL = 1, TOF_RAW_T1, 
		      TOF_RAW_T2, TOF_RAW_A1, TOF_RAW_A2 };
enum TofHitPar      { TOF_HIT_CELL = 1, TOF_HIT_TOF, TOF_HIT_X, 
		      TOF_HIT_XLAB, TOF_HIT_YLAB, TOF_HIT_ZLAB, TOF_HIT_DIST,
                      TOF_HIT_THETA, TOF_HIT_PHI };
enum RichRawPar     { RICH_RAW_CHARGE = 1, RICH_RAW_ROW, RICH_RAW_COL };
enum RichCalPar     { RICH_CAL_CHARGE = 1, RICH_CAL_ROW, RICH_CAL_COL };
enum RichHitPar     { RICH_HIT_LOCALX = 1, 
		      RICH_HIT_LOCALY, 
		      RICH_HIT_LABX, 
		      RICH_HIT_LABY, 
		      RICH_HIT_LABZ,
                      RICH_HIT_THETA,
                      RICH_HIT_PHI };
enum ShowerRawPar   { SHOWER_RAW_CHARGE = 1 };
enum ShowerCalPar   { SHOWER_CAL_CHARGE = 1 };
enum ShowerHitPar   { SHOWER_HIT_LOCALX = 1,
		      SHOWER_HIT_LOCALY,
		      SHOWER_HIT_LABX,
		      SHOWER_HIT_LABY,
		      SHOWER_HIT_LABZ };
enum MdcCal1Par     { MDC_CAL1_T1 = 1, MDC_CAL1_T2 };
enum MdcCal2Par     { MDC_CAL2_DIST1 = 1,
		      MDC_CAL2_DIST2,
		      MDC_CAL2_ERRDIST1,
		      MDC_CAL2_ERRDIST2 };
enum MdcHitPar      { MDC_HIT_X = 1, MDC_HIT_Y, MDC_HIT_THETA, MDC_HIT_PHI };
enum TofinoRawPar   { TOFINO_RAW_TDC = 1, TOFINO_RAW_ADC };

class HLocation;
class HExpString;

class HDefaultParameter : public TNamed {

 protected:

  Float_t getTofRaw(Int_t idx);
  Float_t getTofHit(Int_t idx);
  Float_t getRichRaw(Int_t idx);
  Float_t getRichCal(Int_t idx);
  Float_t getRichHit(Int_t idx);
  Float_t getShowerRaw(Int_t idx);
  Float_t getShowerCal(Int_t idx);
  Float_t getShowerHit(Int_t idx);
  Float_t getMdcCal1(Int_t idx);
  Float_t getMdcCal2(Int_t idx);
  Float_t getMdcHit(Int_t idx);
  Float_t getTofinoRaw(Int_t idx);

 protected:

  Int_t   fCategory;
  Int_t   fIndex;

 public:

  HDefaultParameter() : TNamed() { fCategory = -1; fIndex = 0; }
  virtual ~HDefaultParameter() {}
  void setCategory(Int_t acat) { fCategory = acat; }
  Int_t getCategory() const { return fCategory; }
  void setIndex(Int_t aindex) { fIndex = aindex; }
  Int_t getIndex() const { return fIndex; }
  Float_t getValue(Int_t acat, Int_t idx, HLocation &aloc);
  virtual Float_t getParameterValue(HLocation &aloc);
  static Int_t getMaxDefIdx(Int_t acat);
  Bool_t isEqual(HDefaultParameter *apar);

 public:

  ClassDef(HDefaultParameter,1) //Default parameter

};


class HUserParameter : public HDefaultParameter {

 protected:
  
  TFormula  *fExpression;
  Int_t      fParIdx[10]; // indexes of default parameters of this category
  Int_t      fParNumber;

 public:

  HUserParameter() : HDefaultParameter() { fExpression = 0; fParNumber = 0; }
  virtual ~HUserParameter() { if(fExpression) delete fExpression; }
  void setExpression(HExpString *pES);
  const Char_t* getExpression() { 
    if(fExpression) return fExpression->GetTitle();
    return 0;
  }
  void setName();
  Int_t getCategory() const { return fCategory; }
  void addIndex(Int_t idx);
  void clear() { fParNumber = 0; }
  virtual Float_t getParameterValue(HLocation &aloc);
  void copy(TObject *hup);

 public:

  ClassDef(HUserParameter,1) //User defined parameter

};

#endif








