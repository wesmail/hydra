//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

//-------------------------------------------------------------------------
//
// File name:       hmemorycheck.cc
// Subject:         Memory checks.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HMemoryCheck.
// This class serves to perform a check if the total amount of memory
// used by all histogrammes stays below the limit defined by the user.
//
//-------------------------------------------------------------------------

#include "hmemorycheck.h"
#include <TH1.h>
#include <TH2.h>
#include "honlinecontrol.h"
#include "hhistogram.h"
#include "hlocalcondition.h"
#include "hglobalcondition.h"
#include "hhistocont.h"
#include "hlocalcont.h"
#include "hglobalcont.h"

#include <TGMsgBox.h>

//////////////////////////////////////////////////////////////////////
// This class serves to perform a check if the total amount of memory
// used by all histogrammes stays below the limit defined by the user.
/////////////////////////////////////////////////////////////////////
ClassImp(HMemoryCheck)


//---------------------------------------------------------------------
//******  Int_t HMemoryCheck::calc(Int_t nDim, Int_t binsx, Int_t binsy)
//
// Calculates how many bytes in memory will occupy a histogram with
// nDim dimensions and number of bins binsx binsy
//
//---------------------------------------------------------------------

Int_t HMemoryCheck::calc(Int_t nDim, Int_t binsx, Int_t binsy) {
//
// Calculates how many bytes in memory will occupy a histogram with
// nDim dimensions and number of bins binsx binsy
//

  Int_t retval = 0;

  if(nDim == 1) retval = sizeof(TH1F) + (binsx+2) * sizeof(Float_t);
  if(nDim == 2) retval = sizeof(TH2F) + (binsx+2)*(binsy+2) * sizeof(Float_t);

  return retval;

}


//---------------------------------------------------------------------
//******  Int_t HMemoryCheck::calc1d(TH1F *h1d)
//
// Calculates how many bytes in memory occupies 1d histogram h1d.
//
//---------------------------------------------------------------------

Int_t HMemoryCheck::calc1d(TH1F *h1d) {
//
// Calculates how many bytes in memory occupies 1d histogram h1d.
//

  if(!h1d) return 0;
  Int_t binsx = h1d->GetXaxis()->GetNbins();

  Int_t retval = sizeof(TH1F) + (binsx+2) * sizeof(Float_t);

  return retval;

}


//---------------------------------------------------------------------
//******  Int_t HMemoryCheck::calc2d(TH2F *h2d)
//
// Calculates how many bytes in memory occupies 2d histogram h2d.
//
//---------------------------------------------------------------------

Int_t HMemoryCheck::calc2d(TH2F *h2d) {
//
// Calculates how many bytes in memory occupies 2d histogram h2d.
//

  if(!h2d) return 0;
  Int_t binsx = h2d->GetXaxis()->GetNbins();
  Int_t binsy = h2d->GetYaxis()->GetNbins();

  Int_t retval = sizeof(TH2F) + (binsx+2)*(binsy+2) * sizeof(Float_t);

  return retval;

}


//---------------------------------------------------------------------
//******  Bool_t HMemoryCheck::checkVirtual(Int_t nDim, Int_t binsx,
//                                                      Int_t binsy)
//
// Checks if adding of a new histogram with nDim dimensions and number
// of bins binsx, binsy will cause exceeding of the memory limit.
//
//---------------------------------------------------------------------

Bool_t HMemoryCheck::checkVirtual(Int_t nDim, Int_t binsx, Int_t binsy) {
//
// Checks if adding of a new histogram with nDim dimensions and number
// of bins binsx, binsy will cause exceeding of the memory limit.
//

  static Int_t count;
  check(&count);

  count += calc(nDim,binsx,binsy);
  if(count > fHiMemLimit) return kFALSE;

  return kTRUE;

}



//---------------------------------------------------------------------
//******  void HMemoryCheck::check(Int_t *c)
//
// Scans the container of histogrammes and checks if the total amount of 
// memory which they occupy is below fHiMemLimit.
//
//---------------------------------------------------------------------

void HMemoryCheck::check(Int_t *c) {
//
// Scans the container of histogrammes and checks if the total amount of 
// memory which they occupy is below fHiMemLimit.
//

  if(!gOnline) {
    kAlarm = kFALSE;
    return;
  }
  if(!gOnline->getHisto()) {
    kAlarm = kFALSE;
    return;
  }

  Int_t i;
  HHistogram *pHisto;
  //  HLocalCondition *pLocal = 0;
  //  HGlobalCondition *pGlobal = 0;
  Int_t count = 0;

  for(i=0;i<gOnline->getHisto()->getEntries();i++) {

    pHisto = gOnline->getHisto()->at(i);
    if(pHisto) {

      if(pHisto->getDim() == 1) count += calc1d(pHisto->getHist1());
      if(pHisto->getDim() == 2) count += calc2d(pHisto->getHist2());

      if(c) *c = count;

      if(count > fHiMemLimit) {
	kAlarm = kTRUE;
	alarm();
	return;
      }
    }
  }

  kAlarm = kFALSE;

}


//---------------------------------------------------------------------
//******  void HMemoryCheck::alarm()
//
// Alarm message.
//
//---------------------------------------------------------------------

void HMemoryCheck::alarm() {
//
// Alarm message.
//

  Int_t retval;

  char msg[200] = "Memory limit is exceeded.";

  new TGMsgBox(gClient->GetRoot(),gClient->GetRoot(),"Alarm message",
		 msg, kMBIconExclamation,kMBOk,&retval);

}







