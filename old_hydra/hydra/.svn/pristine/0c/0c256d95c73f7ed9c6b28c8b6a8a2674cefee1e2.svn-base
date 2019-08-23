//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hparameter.cc
// Subject:         Extraction of parameter values.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            January 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of
// the classes HDefaultParameter and HUserParameter.
// These classes extract parameter values from the data categories.
//
//-------------------------------------------------------------------------

#include "hparameter.h"
#include "hexpstring.h"
#include "hades.h"
#include "hcategory.h"
#include "hevent.h"
#include "hdataobject.h"
#include "hlocation.h"

#include "hindextransform.h"

#include "htofraw.h"
#include "htofhit.h"

#include "hrichraw.h"
#include "hrichcal.h"
#include "hrichhit.h"

#include "hshowerraw.h"
#include "hshowercal.h"
#include "hshowerhit.h"

#include "hmdccal1.h"
#include "hmdccal2.h"
#include "hmdchit.h"

#include "htofinoraw.h"

//#include "htofpos.h"
//#include <TMath.h>

#include <TRandom.h>

///////////////////////////////////////////////////////////////////////
// The classes HDefaultParameter and HUserParameter extract parameter values 
// from the data categories.
///////////////////////////////////////////////////////////////////////
ClassImp(HDefaultParameter)
ClassImp(HUserParameter)


HDataObject *dataobj;

//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getValue(Int_t acat,Int_t idx,
//                                            HLocation &aloc)
//
// This function returns the value of the parameter.
//
// acat - category (tofraw, showercal etc.)
// idx  - index of the parameter (example: tofraw - cell(idx=1), tleft(idx=2),
//        tright(idx=3) etc.)
// aloc - location in the data category
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getValue(Int_t acat, Int_t idx, HLocation &aloc) {
//
// This function returns the value of the parameter.
//
// acat - category (tofraw, showercal etc.)
// idx  - index of the parameter (example: tofraw - cell(idx=1), tleft(idx=2),
//        tright(idx=3) etc.)
// aloc - location in the data category
//

  if(!gHades) return 999999.;

  HCategory *categ;
  categ = gHades->getCurrentEvent()->getCategory(acat);
  if(!categ) return 999999.;

  dataobj = categ->getObject(aloc);
  if(!dataobj) return 999999.;

  if(!strcmp(dataobj->ClassName(),"HTofRaw")) return getTofRaw(idx);
  if(!strcmp(dataobj->ClassName(),"HTofHit")) return getTofHit(idx);
  if(!strcmp(dataobj->ClassName(),"HRichRaw")) return getRichRaw(idx);
  if(!strcmp(dataobj->ClassName(),"HRichCal")) return getRichCal(idx);
  if(!strcmp(dataobj->ClassName(),"HRichHit")) return getRichHit(idx);
  if(!strcmp(dataobj->ClassName(),"HShowerRaw")) return getShowerRaw(idx);
  if(!strcmp(dataobj->ClassName(),"HShowerCal")) return getShowerCal(idx);
  if(!strcmp(dataobj->ClassName(),"HShowerHit")) return getShowerHit(idx);
  if(!strcmp(dataobj->ClassName(),"HMdcCal1")) return getMdcCal1(idx);
  if(!strcmp(dataobj->ClassName(),"HMdcCal2")) return getMdcCal2(idx);
  if(!strcmp(dataobj->ClassName(),"HMdcHit")) return getMdcHit(idx);
  if(!strcmp(dataobj->ClassName(),"HTofinoRaw")) return getTofinoRaw(idx);

  return 999999.;

}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getTofRaw(Int_t idx)
//
// This function returns the value of the parameter in category tofraw.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getTofRaw(Int_t idx) {
//
// This function returns the value of the parameter in the category tofraw.
//

  if(idx==0 || idx==1) { // return cell number in the range 0-383
    Int_t maxind[3] = { 6, 8, 8 };
    Int_t hit[3];
    hit[0] = (Int_t) (((HTofRaw*) dataobj)->getSector());
    hit[1] = (Int_t) (((HTofRaw*) dataobj)->getModule());
    hit[2] = (Int_t) (((HTofRaw*) dataobj)->getCell());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(3,maxind,hit));
    return cell;
  }
  if(idx==TOF_RAW_T1) return ((HTofRaw*) dataobj) -> getLeftTime();
  if(idx==TOF_RAW_T2) return ((HTofRaw*) dataobj) -> getRightTime();
  if(idx==TOF_RAW_A1) return ((HTofRaw*) dataobj) -> getLeftCharge();
  if(idx==TOF_RAW_A2) return ((HTofRaw*) dataobj) -> getRightCharge();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getTofHit(Int_t idx)
//
// This function returns the value of the parameter in category tofhit.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getTofHit(Int_t idx) {
//
// This function returns the value of the parameter in the category tofhit.
//

  Int_t sec, mod, rod;
  Float_t xpos;
  Float_t dist, x, y, z;
  Float_t theta = 0.;
  Float_t phi = 0.;
  //  Float_t rad2deg = 180./3.1415926;

  if(idx==0 || idx==1) { // return cell number in the range 0-383
    Int_t maxind[3] = { 6, 8, 8 };
    Int_t hit[3];
    hit[0] = (Int_t) (((HTofHit*) dataobj)->getSector());
    hit[1] = (Int_t) (((HTofHit*) dataobj)->getModule());
    hit[2] = (Int_t) (((HTofHit*) dataobj)->getCell());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(3,maxind,hit));
    return cell;
  }
  if(idx==TOF_HIT_TOF) return ((HTofHit*) dataobj) -> getTof();
  if(idx==TOF_HIT_X)   return ((HTofHit*) dataobj) -> getXpos();
  sec = (Int_t)  ((HTofHit*) dataobj)->getSector();
  mod = (Int_t)  ((HTofHit*) dataobj)->getModule();
  rod = (Int_t)  ((HTofHit*) dataobj)->getCell();
  xpos = (Float_t) ((HTofHit*) dataobj)->getXpos();

  //  if(!gTofPos) return 999999.;
  //  x = (Float_t) gTofPos->getTofLabX(sec,mod,rod,xpos);
  //  y = (Float_t) gTofPos->getTofLabY(sec,mod,rod,xpos);
  //  z = (Float_t) gTofPos->getTofLabZ(sec,mod,rod,xpos);

  //  dist = (Float_t) TMath::Sqrt(x*x + y*y + z*z);

  //  if(dist>0.0) theta = rad2deg * TMath::ACos(z/dist);
  //  if(x != 0.0) phi = rad2deg * TMath::ATan2(y,x);

  ((HTofHit*) dataobj)->getXYZLab(x,y,z);
  ((HTofHit*) dataobj)->getDistance(dist);
  ((HTofHit*) dataobj)->getTheta(theta);
  ((HTofHit*) dataobj)->getPhi(phi);

  if(idx==TOF_HIT_XLAB) return x;
  if(idx==TOF_HIT_YLAB) return y;
  if(idx==TOF_HIT_ZLAB) return z;
  if(idx==TOF_HIT_DIST) return dist;
  if(idx==TOF_HIT_THETA) return theta;
  if(idx==TOF_HIT_PHI) return phi;

  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getRichRaw(Int_t idx)
//
// This function returns the value of the parameter in category richraw.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getRichRaw(Int_t idx) {
//
// This function returns the value of the parameter in the category richraw.
//

  if(idx==0) { // return cell number in the range 0-55295
    Int_t maxind[3] = { 6, 96, 96 };
    Int_t hit[3];
    hit[0] = (Int_t) (((HRichRaw*) dataobj)->getSector());
    hit[1] = (Int_t) (((HRichRaw*) dataobj)->getRow());
    hit[2] = (Int_t) (((HRichRaw*) dataobj)->getCol());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(3,maxind,hit));
    return cell;
  }
  if(idx==RICH_RAW_CHARGE) return ((HRichRaw*) dataobj) -> getCharge();
  if(idx==RICH_RAW_ROW) return ((Float_t) (((HRichRaw*) dataobj) -> getRow()));
  if(idx==RICH_RAW_COL) return ((Float_t) (((HRichRaw*) dataobj) -> getCol()));
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getRichCal(Int_t idx)
//
// This function returns the value of the parameter in category richcal.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getRichCal(Int_t idx) {
//
// This function returns the value of the parameter in the category richcal.
//

  if(idx==0) { // return cell number in the range 0-55295
    Int_t maxind[3] = { 6, 96, 96 };
    Int_t hit[3];
    hit[0] = (Int_t) (((HRichCal*) dataobj)->getSector());
    hit[1] = (Int_t) (((HRichCal*) dataobj)->getRow());
    hit[2] = (Int_t) (((HRichCal*) dataobj)->getCol());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(3,maxind,hit));
    return cell;
  }
  if(idx==RICH_CAL_CHARGE) return ((HRichCal*) dataobj) -> getCharge();
  if(idx==RICH_CAL_ROW) return ((Float_t) (((HRichCal*) dataobj) -> getRow()));
  if(idx==RICH_CAL_COL) return ((Float_t) (((HRichCal*) dataobj) -> getCol()));
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getRichHit(Int_t idx)
//
// This function returns the value of the parameter in category richhit.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getRichHit(Int_t idx) {
//
// This function returns the value of the parameter in the category richhit.
//

  Float_t retval;
  Float_t temp, temp2;

  retval = 999999.;
  if(idx==RICH_HIT_LOCALX) retval = (Float_t) (((HRichHit*) dataobj)
			    ->iRingX);
  if(idx==RICH_HIT_LOCALY) retval = (Float_t) (((HRichHit*) dataobj)
			    ->iRingY);


  /*  if(idx==RICH_HIT_LOCALX) ((HRichHit*) dataobj)
			     -> getLocalXY(&retval, &temp);
  if(idx==RICH_HIT_LOCALY) ((HRichHit*) dataobj)
			     -> getLocalXY(&temp, &retval);*/
  if(idx==RICH_HIT_LABX)   ((HRichHit*) dataobj) 
			     -> getLabXYZ(&retval, &temp, &temp2);
  if(idx==RICH_HIT_LABY)   ((HRichHit*) dataobj) 
			     -> getLabXYZ(&temp, &retval, &temp2);
  if(idx==RICH_HIT_LABZ)   ((HRichHit*) dataobj) 
			     -> getLabXYZ(&temp, &temp2, &retval);
  if(idx==RICH_HIT_THETA) retval = ((HRichHit*) dataobj)
			    -> getTheta();
  if(idx==RICH_HIT_PHI) retval = ((HRichHit*) dataobj)
			  -> getPhi();
  return retval;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getShowerRaw(Int_t idx)
//
// This function returns the value of the parameter in category showerraw.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getShowerRaw(Int_t idx) {
//
// This function returns the value of the parameter in the category showerraw.
//

  if(idx==0) { // return cell number in the range 0-18431
    Int_t maxind[4] = { 6, 3, 32, 32 };
    Int_t hit[4];
    hit[0] = (Int_t) (((HShowerRaw*) dataobj)->getSector());
    hit[1] = (Int_t) (((HShowerRaw*) dataobj)->getModule());
    hit[2] = (Int_t) (((HShowerRaw*) dataobj)->getRow());
    hit[3] = (Int_t) (((HShowerRaw*) dataobj)->getCol());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(4,maxind,hit));
    return cell;
  }
  if(idx==SHOWER_RAW_CHARGE) return ((HShowerRaw*) dataobj) -> getCharge();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getShowerCal(Int_t idx)
//
// This function returns the value of the parameter in category showercal.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getShowerCal(Int_t idx) {
//
// This function returns the value of the parameter in the category showercal.
//

  if(idx==0) { // return cell number in the range 0-18431
    Int_t maxind[4] = { 6, 3, 32, 32 };
    Int_t hit[4];
    hit[0] = (Int_t) (((HShowerCal*) dataobj)->getSector());
    hit[1] = (Int_t) (((HShowerCal*) dataobj)->getModule());
    hit[2] = (Int_t) (((HShowerCal*) dataobj)->getRow());
    hit[3] = (Int_t) (((HShowerCal*) dataobj)->getCol());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(4,maxind,hit));
    return cell;
  }
  if(idx==SHOWER_CAL_CHARGE) return ((HShowerCal*) dataobj) -> getCharge();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getShowerHit(Int_t idx)
//
// This function returns the value of the parameter in category showerhit.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getShowerHit(Int_t idx) {
//
// This function returns the value of the parameter in the category showerhit.
//

  Float_t retval;
  Float_t temp, temp2;

  retval = 999999.;

  if(idx==0) { // return cell number in the range 0-18431
    Int_t maxind[4] = { 6, 3, 32, 32 };
    Int_t hit[4];
    hit[0] = (Int_t) (((HShowerHit*) dataobj)->getSector());
    hit[1] = (Int_t) (((HShowerHit*) dataobj)->getModule());
    hit[2] = (Int_t) (((HShowerHit*) dataobj)->getRow());
    hit[3] = (Int_t) (((HShowerHit*) dataobj)->getCol());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(4,maxind,hit));
    return cell;
  }

  if(idx==SHOWER_HIT_LOCALX) ((HShowerHit*) dataobj)
			       -> getXY(&retval, &temp);
  if(idx==SHOWER_HIT_LOCALY) ((HShowerHit*) dataobj)
			       -> getXY(&temp, &retval);
  if(idx==SHOWER_HIT_LABX)   ((HShowerHit*) dataobj) 
			       -> getLabXYZ(&retval, &temp, &temp2);
  if(idx==SHOWER_HIT_LABY)   ((HShowerHit*) dataobj) 
			       -> getLabXYZ(&temp, &retval, &temp2);
  if(idx==SHOWER_HIT_LABZ)   ((HShowerHit*) dataobj) 
			       -> getLabXYZ(&temp, &temp2, &retval);
  return retval;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getMdcCal1(Int_t idx)
//
// This function returns the value of the parameter in category mdccal1.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getMdcCal1(Int_t idx) {
//
// This function returns the value of the parameter in the category mdccal1.
//

  if(idx==0) { // return cell number in the range 0-28799
    Int_t maxind[4] = { 6, 4, 6, 200 };
    Int_t hit[4];
    hit[0] = (Int_t) (((HMdcCal1*) dataobj)->getSector());
    hit[1] = (Int_t) (((HMdcCal1*) dataobj)->getModule());
    hit[2] = (Int_t) (((HMdcCal1*) dataobj)->getLayer());
    hit[3] = (Int_t) (((HMdcCal1*) dataobj)->getCell());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(4,maxind,hit));
    return cell;
  }
  if(idx==MDC_CAL1_T1) return ((HMdcCal1*) dataobj) -> getTime1();
  if(idx==MDC_CAL1_T2) return ((HMdcCal1*) dataobj) -> getTime2();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getMdcCal2(Int_t idx)
//
// This function returns the value of the parameter in category mdccal2.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getMdcCal2(Int_t idx) {
//
// This function returns the value of the parameter in the category mdccal2.
//

  if(idx==0) { // return cell number in the range 0-28799
    Int_t maxind[4] = { 6, 4, 6, 200 };
    Int_t hit[4];
    hit[0] = (Int_t) (((HMdcCal2*) dataobj)->getSector());
    hit[1] = (Int_t) (((HMdcCal2*) dataobj)->getModule());
    hit[2] = (Int_t) (((HMdcCal2*) dataobj)->getLayer());
    hit[3] = (Int_t) (((HMdcCal2*) dataobj)->getCell());
    Float_t cell = (Float_t) (HIndexTransform::mult2lin(4,maxind,hit));
    return cell;
  }
  if(idx==MDC_CAL2_DIST1)    return ((HMdcCal2*) dataobj) -> getDist1();
  if(idx==MDC_CAL2_DIST2)    return ((HMdcCal2*) dataobj) -> getDist2();
  if(idx==MDC_CAL2_ERRDIST1) return ((HMdcCal2*) dataobj) -> getErrDist1();
  if(idx==MDC_CAL2_ERRDIST2) return ((HMdcCal2*) dataobj) -> getErrDist2();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getMdcHit(Int_t idx)
//
// This function returns the value of the parameter in category mdchit.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getMdcHit(Int_t idx) {
//
// This function returns the value of the parameter in the category mdchit.
//

  if(idx==MDC_HIT_X)    return ((HMdcHit*) dataobj) -> getX();
  if(idx==MDC_HIT_Y)    return ((HMdcHit*) dataobj) -> getY();
  if(idx==MDC_HIT_THETA)    return ((HMdcHit*) dataobj) -> getTheta();
  if(idx==MDC_HIT_PHI) return ((HMdcHit*) dataobj) -> getPhi();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getTofinoRaw(Int_t idx)
//
// This function returns the value of the parameter in category tofinoraw.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getTofinoRaw(Int_t idx) {
//
// This function returns the value of the parameter in the category tofinoraw.
//

  //  if(idx==TOFINO_RAW_TDC) return ((HTofinoRaw*) dataobj) -> getTdcT();
  //  if(idx==TOFINO_RAW_ADC) return ((HTofinoRaw*) dataobj) -> getAdcT();
  return 999999.;
}


//---------------------------------------------------------------------
//******  Int_t HDefaultParameter::getMaxDefIdx(Int_t acat)
//
// This function returns the number of default parameters in the category
// acat.
//
//---------------------------------------------------------------------

Int_t HDefaultParameter::getMaxDefIdx(Int_t acat) {
//
// This function returns the number of default parameters in the category
// acat.
//

  Int_t retval = 0;
  switch(acat) {

  case 96: { retval = 5; break; } // catTofRaw
  case 97: { retval = 9; break; } // catTofHit
  case 2:  { retval = 2; break; } // catMdcCal1
  case 4:  { retval = 4; break; } // catMdcCal2
  case 1:  { retval = 4; break; } // catMdcHit
  case 32: { retval = 3; break; } // catRichRaw
  case 34: { retval = 3; break; } // catRichCal
  case 33: { retval = 7; break; } // catRichHit
  case 64: { retval = 1; break; } // catShowerRaw
  case 66: { retval = 1; break; } // catShowerCal
  case 65: { retval = 5; break; } // catShowerHit
  default: { retval = 0; break; }
  }

  return retval;
}


//---------------------------------------------------------------------
//******  Float_t HDefaultParameter::getParameterValue(HLocation &aloc)
//
// This function returns the parameter value in the location aloc.
//
//---------------------------------------------------------------------

Float_t HDefaultParameter::getParameterValue(HLocation &aloc) {
//
// This function returns the parameter value in the location aloc.
//

  Float_t retval = getValue(fCategory,fIndex,aloc);
  return retval;

}


//---------------------------------------------------------------------
//******  Bool_t HDefaultParameter::isEqual(HDefaultParameter *apar)
//
// This function checks if two parameters are identical.
//
//---------------------------------------------------------------------

Bool_t HDefaultParameter::isEqual(HDefaultParameter *apar) {
//
// This function checks if two parameters are identical.
//

  if(!apar) return kFALSE;
  if(apar->getCategory() != fCategory) return kFALSE;
  if(apar->getIndex() != fIndex) return kFALSE;

  return kTRUE;

}



//---------------------------------------------------------------------
//******  Float_t HUserParameter::getParameterValue(HLocation &aloc)
//
// This function returns the value of the user defined parameter.
//
//---------------------------------------------------------------------

Float_t HUserParameter::getParameterValue(HLocation &aloc) {
//
// This function returns the value of the user defined parameter.
//

  Float_t retval = 999999.;
  if(!fExpression) return (getValue(fCategory,fIndex,aloc));

  Double_t *argu = new Double_t[getMaxDefIdx(fCategory)];
  if(!argu) return retval;

  for(Int_t i=0; i<fParNumber; i++) {

    argu[fParIdx[i]-1] = (Double_t) getValue(fCategory,fParIdx[i],aloc);

  }

  retval = (Float_t) fExpression->EvalPar(0,argu);
  delete [] argu;

  return retval;

}


//---------------------------------------------------------------------
//******  void HUserParameter::addIndex(Int_t idx)
//
// The user defined parameter contains the expression made of default
// parameters.
// This function adds the index of the default parameter used in 
// the expression to the list of indexes of default parameters.
//
//---------------------------------------------------------------------

void HUserParameter::addIndex(Int_t idx) {
//
// The user defined parameter contains the expression made of default
// parameters.
// This function adds the index of the default parameter used in 
// the expression to the list of indexes of default parameters.
//

  if(idx <= 0 || idx > getMaxDefIdx(fCategory)) return;
  if(fParNumber >= 10) return;
  fParIdx[fParNumber++] = idx;

}


//---------------------------------------------------------------------
//******  void HUserParameter::copy(TObject *hup)
//
// This function copies this object to hup.
//
//---------------------------------------------------------------------

void HUserParameter::copy(TObject *hup) {
//
// This function copies this object to hup.
//

  if(!hup) return;
  TNamed::Copy(*hup);
  if(fExpression) 
    ((HUserParameter*)hup)->fExpression = (TFormula*) fExpression->Clone();
  for(Int_t i=0; i<10; i++) 
    (*((HUserParameter*)hup)).fParIdx[i] = fParIdx[i];
  ((HUserParameter*)hup)->fParNumber = fParNumber;
  ((HUserParameter*)hup)->fCategory = fCategory;
  ((HUserParameter*)hup)->fIndex = fIndex;

}


//---------------------------------------------------------------------
//******  void HUserParameter::setExpression(HExpString *pES)
//
// This function sets the expression to be evaluated.
//
//---------------------------------------------------------------------

void HUserParameter::setExpression(HExpString *pES) {
//
// This function sets the expression to be evaluated.
//

  if(!pES) return;
  pES->checkParam();
  if(!pES->isValid()) return;

  if(fExpression) {
    delete fExpression;
    fExpression = 0;
  }

  char nullstr[50] = "";

  if(!strcmp(nullstr,GetName())) 
    fExpression = new TFormula("formula",(*pES).exp);
  else fExpression = new TFormula(GetName(),(*pES).exp);

  for(Int_t i=0; i < (pES->parcur + 1); i++) {

    if(!fParNumber) addIndex((*pES).paridx[i]+1);

    for(Int_t j=0; j < fParNumber; j++) {
      if(fParIdx[j] == (*pES).paridx[i] + 1) continue;
      else addIndex((*pES).paridx[i]+1);
    }
  }

}

  
//---------------------------------------------------------------------
//******  void HUserParameter::setName()
//
// This function sets the name of the parameter.
// Not used, since now the name is also user defined.
//
//---------------------------------------------------------------------

void HUserParameter::setName() {
//
// This function sets the name of the parameter.
// Not used, since now the name is also user defined.
//

  Int_t value;
  char parname[50];

  value = (Int_t) (gRandom->Rndm() * 1000);
  switch(fCategory) {

  case 96: { sprintf(parname,"%s%i","tofraw_",value); break;  }
  case 97: { sprintf(parname,"%s%i","tofhit_",value); break;  }
  case 2:  { sprintf(parname,"%s%i","mdccal1_",value); break; }
  case 4:  { sprintf(parname,"%s%i","mdccal2_",value); break; }
  case 1:  { sprintf(parname,"%s%i","mdchit_",value); break;  }
  case 32: { sprintf(parname,"%s%i","richraw_",value); break; }
  case 34: { sprintf(parname,"%s%i","richcal_",value); break; }
  case 33: { sprintf(parname,"%s%i","richhit_",value); break; }
  case 64: { sprintf(parname,"%s%i","showraw_",value); break; }
  case 66: { sprintf(parname,"%s%i","showcal_",value); break; }
  case 65: { sprintf(parname,"%s%i","showhit_",value); break; }
  default: { sprintf(parname,"%i",value); break; }

  }

  SetName(parname);

  if(fExpression) fExpression->SetName(parname);

}









