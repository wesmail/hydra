//-------------------------------------------------------------------------
//
// File name:       hbrowserdb.cc
// Subject:         Run time data base for the browser and manager of online
//                  objects.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HBrowserDB.
// This class forms the run time data base for the manager of the online
// objects. The main destination of that data base is storing of the user
// defined parameters. It also keeps trace of all existing online objects
// and helps the panel to display them correctly in the list box of
// the manager.
// The data base has the same life time as the browser panel.
//
//-------------------------------------------------------------------------

#include "hbrowserdb.h"
#include "honlinecontrol.h"
#include "hdetsetcont.h"
#include "hdetset.h"
#include "hlocalcont.h"
#include "hlocalcondition.h"
#include "hglobalcont.h"
#include "hglobalcondition.h"
#include "hhistocont.h"
#include "hhistogram.h"
#include "hparameter.h"

ClassImp(HBrowserDB)

//---------------------------------------------------------------------
//******  HBrowserDB::HBrowserDB()
//
// Constructor of the class HBrowserDB.
//
//---------------------------------------------------------------------

HBrowserDB::HBrowserDB() : TObject() {

  clear();
  fContainer = new TObjArray(200);
  if(fContainer) putDefault();
  if(gOnline) putUserPar();

}


//---------------------------------------------------------------------
//******  HBrowserDB::~HBrowserDB()
//
// Destructor of the class HBrowserDB.
//
//---------------------------------------------------------------------

HBrowserDB::~HBrowserDB() {

  if(fContainer) {

    HUserParameter *pDel;
    for(Int_t i=0; i<getEntries(); i++) {

      pDel = (HUserParameter*) fContainer->RemoveAt(i);
      if(pDel) {
	delete pDel;
	pDel = 0;
      }
    }

    delete fContainer;
  }

}


//---------------------------------------------------------------------
//******  Bool_t HBrowserDB::add(HUserParameter *p)
//
// This function adds a new user defined parameter to the data base.
//
//---------------------------------------------------------------------

Bool_t HBrowserDB::add(HUserParameter *p) {

  if(!p) return kFALSE;

  char nulstr[50] = "";
  if(!strcmp(nulstr,p->GetName())) return kFALSE;

  Int_t idx = find(p->GetName());
  if(idx != -1) return kFALSE;
  if(p->getIndex() == 0) return kFALSE;

  HUserParameter *pNewCopy = new HUserParameter();
  if(!pNewCopy) return kFALSE;

  p->copy(pNewCopy);

  fContainer->Add(pNewCopy);

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Int_t HBrowserDB::getEntries()
//
// This function returns the number of parameters found in the local
// container of the data base.
//
//---------------------------------------------------------------------

Int_t HBrowserDB::getEntries() {

  if(!fContainer) return -1;
  return (fContainer->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HUserParameter HBrowserDB::at(Int_t idx)
//
// This function returns the user parameter which is stored in the container
// under index idx.
//
//---------------------------------------------------------------------

HUserParameter* HBrowserDB::at(Int_t idx) {

  if(!fContainer) return 0;
  if(idx < 0) return 0;
  HUserParameter *pRetVal = (HUserParameter*) fContainer->At(idx);

  return pRetVal;

}


//---------------------------------------------------------------------
//******  Int_t HBrowserDB::find(const char* name)
//
// This function returns the index of the user parameter using its name as
// an input data.
//
//---------------------------------------------------------------------

Int_t HBrowserDB::find(const char* name) {

  if(!name) return -1;
  HUserParameter *pPar;

  for(Int_t i=0; i<getEntries(); i++) {
    pPar = at(i);
    if(!strcmp(pPar->GetName(),name)) return i;
  }

  return -1;

}


//---------------------------------------------------------------------
//******  void HBrowserDB::putDefault()
//
// This function puts to the data base default parameters.
//
//---------------------------------------------------------------------

void HBrowserDB::putDefault() {

  HUserParameter *pParam;
  const char* tofrawn[] = { "cell_R_TO", "timeL_TO", "timeR_TO", 
			    "amplL_TO", "amplR_TO" };
  const char* tofhitn[] = { "cell_H_TO", "t-o-f_TO", "x-pos_TO" };
  const char* mdccal1n[] = { "time1_MD", "time2_MD" };
  const char* mdccal2n[] = { "dist1_MD", "dist2_MD", "errdist1_MD", 
			     "errdist2_MD" };
  const char* mdchitn[] = { "X_MD", "Y_MD", "Z_MD" };
  const char* richrawn[] = { "charge_R_RI", "row_R_RI", "col_R_RI" };        
  const char* richcaln[] = { "charge_C_RI", "row_C_RI", "col_C_RI" };        
  const char* richhitn[] = { "Xloc_RI", "Yloc_RI", "Xlab_RI", "Ylab_RI",
			     "Zlab_RI", "Theta_RI", "Phi_RI" }; 
  const char* shhitn[] = { "Xloc_SH", "Yloc_SH", "Xlab_SH", "Ylab_SH",
			   "Zlab_SH" }; 
  Int_t i;

  for(i=0;i<5;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(96); 
    pParam->setIndex(i+1);
    pParam->SetName(tofrawn[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<3;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(97); 
    pParam->setIndex(i+1);
    pParam->SetName(tofhitn[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<2;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(2); 
    pParam->setIndex(i+1);
    pParam->SetName(mdccal1n[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<4;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(4); 
    pParam->setIndex(i+1);
    pParam->SetName(mdccal2n[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<3;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(1); 
    pParam->setIndex(i+1);
    pParam->SetName(mdchitn[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<3;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(32);
    pParam->setIndex(i+1);
    pParam->SetName(richrawn[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<3;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(34);
    pParam->setIndex(i+1);
    pParam->SetName(richcaln[i]);
    fContainer->Add(pParam);
  }
  for(i=0;i<7;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(33); 
    pParam->setIndex(i+1);
    pParam->SetName(richhitn[i]);
    fContainer->Add(pParam);
  }
  pParam = new HUserParameter();
  pParam->setCategory(64);
  pParam->setIndex(1);
  pParam->SetName("charge_R_SH");
  fContainer->Add(pParam);
  pParam = new HUserParameter();
  pParam->setCategory(66);
  pParam->setIndex(1);
  pParam->SetName("charge_C_SH");
  fContainer->Add(pParam);
  for(i=0;i<5;i++) {
    pParam = new HUserParameter();
    pParam->setCategory(65); 
    pParam->setIndex(i+1);
    pParam->SetName(shhitn[i]);
    fContainer->Add(pParam);
  }

}


//---------------------------------------------------------------------
//******  void HBrowserDB::putUserPar()
//
// The user defined parameters are saved only if they are used by 
// the histogrammes. This function scans all histogrammes and if finds a user
// defined parameter puts it to the local container of the data base.
//
//---------------------------------------------------------------------

void HBrowserDB::putUserPar() {

  if(!gOnline) return;
  HHistogram *pHisto = 0;
  Int_t i;

  for(i=0; i < gOnline->getHisto()->getEntries(); i++) {

    pHisto = gOnline->getHisto()->at(i);
    if(pHisto) {
      add(pHisto->getParameter(1));
      add(pHisto->getParameter(2));
    }
  }

}

      
//---------------------------------------------------------------------
//******  void HBrowserDB::clear()
//
// The data base helps the GUI panel to display the different online objects
// in the same list box. This is done by keeping indexes of the existing 
// objects in a special array. 
// This function takes care of clearing that array.
//
//---------------------------------------------------------------------

void HBrowserDB::clear() {

  for(Int_t i=0; i<501; i++) aridx[i] = -1;
  cursor = 0;
  tabpos = 0;

}


//---------------------------------------------------------------------
//******  void HBrowserDB::changed(Int_t tab)
//
// This function is called from a GUI panel whenever a tab is clicked.
// That means switching inside browser from objects of one type to another
// (for example from detector sets to histogrammes). The function puts
// to the internal array the indexes of that type of the objects which was
// chosen by the tab.
//
//---------------------------------------------------------------------

void HBrowserDB::changed(Int_t tab) {

  if(!gOnline) return;
  HDetSet *pSet;
  HLocalCondition *pLocal;
  HGlobalCondition *pGlobal;
  HHistogram *pHisto;
  Int_t i = 0;
  clear();
  switch(tab) {

  case 1:

    for(i=0; i < gOnline->getDetSet()->getEntries(); i++) {

      pSet = gOnline->getDetSet()->at(i);
      if(pSet) {
	if(pSet->getDetName() == TOF) aridx[++cursor] = i;
      }
    }
    break;

  case 2:

    for(i=0; i < gOnline->getDetSet()->getEntries(); i++) {

      pSet = gOnline->getDetSet()->at(i);
      if(pSet) {
	if(pSet->getDetName() == MDC) aridx[++cursor] = i;
      }
    }
    break;

  case 3:

    for(i=0; i < gOnline->getDetSet()->getEntries(); i++) {

      pSet = gOnline->getDetSet()->at(i);
      if(pSet) {
	if(pSet->getDetName() == RICH) aridx[++cursor] = i;
      }
    }
    break;

  case 4:

    for(i=0; i < gOnline->getDetSet()->getEntries(); i++) {

      pSet = gOnline->getDetSet()->at(i);
      if(pSet) {
	if(pSet->getDetName() == SHOWER) aridx[++cursor] = i;
      }
    }
    break;

  case 5:

    for(i=0; i < gOnline->getDetSet()->getEntries(); i++) {

      pSet = gOnline->getDetSet()->at(i);
      if(pSet) {
	if(pSet->getDetName() == START) aridx[++cursor] = i;
      }
    }
    break;

  case 6:

    for(i=0; i < gOnline->getDetSet()->getEntries(); i++) {

      pSet = gOnline->getDetSet()->at(i);
      if(pSet) {
	if(pSet->getDetName() == TOFINO) aridx[++cursor] = i;
      }
    }
    break;

  case 7:

    for(i=0; i < gOnline->getHisto()->getEntries(); i++) {

      pHisto = gOnline->getHisto()->at(i);
      if(pHisto) aridx[++cursor] = i;
    }
    break;

  case 8:

    for(i=0; i < gOnline->getLocal()->getEntries(); i++) {

      pLocal = gOnline->getLocal()->at(i);
      if(pLocal) aridx[++cursor] = i;
    }
    break;

  case 9:

    for(i=0; i < gOnline->getGlobal()->getEntries(); i++) {

      pGlobal = gOnline->getGlobal()->at(i);
      if(pGlobal) aridx[++cursor] = i;
    }
    break;

  default:
    break;
  }

  tabpos = tab;

}

	









