//-------------------------------------------------------------------------
//
// File name:       honlprocessevent.cc
// Subject:         Event processing inside HTOP
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HOnlProcessEvent.
// This class maintains processing of an event inside HTOP. That means
// 1) evaluation of detector sets
// 2) evaluation of local conditions
// 3) evaluation of global conditions
// 4) filling of the histogrammes
//
// Being derived from the HReconstructor class, HOnlProcessEvent can be easily
// added to the HYDRA task list.
// Init() function of the HOnlProcessEvent class performs all necessary 
// initialization for the whole HTOP.
//
//-------------------------------------------------------------------------

#include "honlprocessevent.h"
#include "honlinecontrol.h"

#include "hdetsetcont.h"
#include "hlocalcont.h"
#include "hglobalcont.h"
#include "hhistocont.h"

#include "hdetset.h"
#include "hlocalcondition.h"
#include "hglobalcondition.h"
#include "hhistogram.h"

#include "hmemorycheck.h"

#include "hconditionout.h"
#include <TList.h>
#include "hlocation.h"
#include <iostream.h>

ClassImp(HOnlProcessEvent)


//---------------------------------------------------------------------
//******  Bool_t HOnlProcessEvent::init()
//
// This function creates an object of the main control class HOnlineControl
// and makes full initialization of the HTOP.
//
//---------------------------------------------------------------------

Bool_t HOnlProcessEvent::init() {

  if(gOnline) {
    delete gOnline;
    gOnline = 0;
  }
  new HOnlineControl();
  if(!gOnline) return kFALSE;
  return (gOnline->read());

}


//---------------------------------------------------------------------
//******  Int_t HOnlProcessEvent::execute()
//
// This function is called inside HYDRA event loop and takes care of calling
// the functions which make evaluation of detector sets, local and global
// conditions and filling of the histogrammes.
//
//---------------------------------------------------------------------

Int_t HOnlProcessEvent::execute() {

  if(!gOnline) return -1;
  if(!gOnline->getDetSet() || !gOnline->getLocal() ||
     !gOnline->getGlobal() || !gOnline->getHisto()) return -1;
  //  if(gOnline->getMemCheck()->isAlarm()) return -1;

  HDetSetCont *pDSCont = gOnline->getDetSet();
  HDetSet *pSet = 0;

  //---------------------------
  // detector set evaluation
  //---------------------------

  for(Int_t i=0; i<pDSCont->getEntries(); i++) {
    pSet = pDSCont->at(i);

    if(pSet) pSet->evaluate();
  }

  //---------------------------
  // local condition evaluation
  //---------------------------

  HLocalCont *pLCont = gOnline->getLocal();
  HLocalCondition *pLC = 0;

  //  HConditionOut *pOut = 0;

  for(Int_t i=0; i < pLCont->getEntries(); i++) {

    pLC = pLCont->at(i);
    if(pLC) pLC->evaluate();

    //---------------------------------------------------

    /*    if(pLC) {

      pOut = pLC->getOutput();

      cout << "count1 = " << pOut->getCount(1) 
	   << "   count2 = " << pOut->getCount(2)
	   << "   countS = " << pOut->getCount(3)
	   << endl;
      
      TList *pList = pOut->getLocList();

      TIter iter(pList);
    
      HLocation *loc;

      while((loc = (HLocation*) iter())) {
      
	cout << "sec = " << (loc->getIndex(0))
	     << "   mod = " << (loc->getIndex(1))
	     << "   rod = " << (loc->getIndex(2))
	     << endl;
      }

      pList = pOut->getIdxList();

      TIter iter2(pList);

      while((loc = (HLocation*) iter2())) {
      
	cout << "idx1 = " << (loc->getIndex(0))
	     << "   idx2 = " << (loc->getIndex(1))
	     << "   idx3 = " << (loc->getIndex(2))
	     << endl;
      }
      }*/

    //---------------------------------------------------

  }

  //----------------------------
  // global condition evaluation
  //----------------------------

  HGlobalCont *pGCont = gOnline->getGlobal();
  HGlobalCondition *pGC = 0;

  for(Int_t i=0; i < pGCont->getEntries(); i++) {

    pGC = pGCont->at(i);
    if(pGC) pGC->evaluate();

  }

  //----------------------------
  // histogram filling
  //----------------------------

  HHistoCont *pHCont = gOnline->getHisto();
  HHistogram *pHisto = 0;

  for(Int_t i=0; i < pHCont->getEntries(); i++) {

    pHisto = pHCont->at(i);
    if(pHisto) pHisto->action();

  }

  return 0;

}









