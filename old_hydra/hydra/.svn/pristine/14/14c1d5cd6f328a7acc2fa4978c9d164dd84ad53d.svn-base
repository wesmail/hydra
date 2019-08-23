//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboratio
*/


//-------------------------------------------------------------------------
//
// File name:       hdisplaydb.cc
// Subject:         Runtime data base for the histogram display.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            June 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HDisplayDB.
// This class forms the runtime data base for the histogram display panel. 
// The data base is destined to store the "external" histogrammes.
// "External" means not created inside HTOP but imported from the ROOT
// file as objects of the classes TH1F and TH2F.
// The data base has the same life time as the display panel.
//
//-------------------------------------------------------------------------

#include "hdisplaydb.h"
#include "hhistogram.h"
#include <TFile.h>
#include <TKey.h>
#include <TList.h>

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// This file contains the source code for the member functions of 
// the class HDisplayDB.
// This class forms the runtime data base for the histogram display panel. 
// The data base is destined to store the "external" histogrammes.
// "External" means not created inside HTOP but imported from the ROOT
// file as objects of the classes TH1F and TH2F.
// The data base has the same life time as the display panel.
///////////////////////////////////////////////////////////////////////

ClassImp(HDisplayDB)


//---------------------------------------------------------------------
//******  Bool_t HDisplayDB::add(HHistogram *p)
//
// This function adds a new histogram object to the data base.
//
//---------------------------------------------------------------------

Bool_t HDisplayDB::add(HHistogram *p) {
//
// This function adds a new histogram object to the data base.
//

  if(!p) return kFALSE;
  Char_t histname[200];

  Char_t nulstr[50] = "";
  if(!strcmp(nulstr,p->GetName())) return kFALSE;

  Int_t idx = find(p->GetName());
  if(idx != -1); // return kFALSE;

  if(getEntries() < 1000) {
    sprintf(histname,"%s%s%i",p->GetName(),"_",getEntries());
    if(p->getDim() == 1) p->getHist1()->SetName(histname);
    if(p->getDim() == 2) p->getHist2()->SetName(histname);
    p->SetName(histname);
    fContainer->Add(p);
  }

  return kTRUE;

}


//---------------------------------------------------------------------
//******  Int_t HDisplayDB::getEntries()
//
// This function returns the number of histogrammes found in the container
// of the data base.
//
//---------------------------------------------------------------------

Int_t HDisplayDB::getEntries() {
//
// This function returns the number of histogrammes found in the container
// of the data base.
//

  if(!fContainer) return -1;
  return (fContainer->GetLast() + 1);

}


//---------------------------------------------------------------------
//******  HHistogram* HDisplayDB::at(Int_t idx)
//
// This function returns the histogram which is stored in the container
// under index idx.
//
//---------------------------------------------------------------------

HHistogram* HDisplayDB::at(Int_t idx) {
//
// This function returns the histogram which is stored in the container
// under index idx.
//

  if(!fContainer) return 0;
  if(idx < 0) return 0;
  HHistogram *pRetVal = (HHistogram*) fContainer->At(idx);

  return pRetVal;

}



//---------------------------------------------------------------------
//******  Int_t HDisplayDB::find(const Char_t* name)
//
// This function returns the index of the histogram using its name as
// an input data.
//
//---------------------------------------------------------------------

Int_t HDisplayDB::find(const Char_t* name) {
//
// This function returns the index of the histogram using its name as
// an input data.
//

  if(!name) return -1;
  HHistogram *pHisto;

  for(Int_t i=0; i<getEntries(); i++) {
    pHisto = at(i);
    if(!strcmp(pHisto->GetName(),name)) return i;
  }

  return -1;

}


//---------------------------------------------------------------------
//******  void HDisplayDB::import(const Char_t* filename)
//
// This function imports objects of the classes TH1F and TH2F to the container
// of the data base from the file named filename.
//
//---------------------------------------------------------------------

void HDisplayDB::import(const Char_t* filename) {
//
// This function imports objects of the classes TH1F and TH2F to the container
// of the data base from the file named filename.
//

  if(!filename) return;
  TFile *pFile = new TFile(filename);

  if(!pFile) return;
  if(pFile->IsZombie()) return;

  HHistogram *pHisto;
  TKey *key;
  TIter it(pFile->GetListOfKeys());
  while((key = (TKey*) it())) {
    if(!strcmp(key->GetClassName(),"TH1F")) {
      pHisto = new HHistogram();
      pHisto->setDim(1);
      pHisto->setHist1((TH1F*) key->ReadObj());
      pHisto->SetName(pHisto->getHist1()->GetName());
      add(pHisto);
    }
    if(!strcmp(key->GetClassName(),"TH2F")) {
      pHisto = new HHistogram();
      pHisto->setDim(2);
      pHisto->setHist2((TH2F*) key->ReadObj());
      pHisto->SetName(pHisto->getHist2()->GetName());
      add(pHisto);
    }
  }

}
