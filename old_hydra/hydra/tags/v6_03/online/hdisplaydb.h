#if !defined HDISPLAYDB_H
#define HDISPLAYDB_H

//-------------------------------------------------------------------------
//
// File name:       hdisplaydb.h
// Subject:         Runtime data base for the histogram display panel.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            June 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HDisplayDB.
// This class forms the runtime data base for the histogram display panel. 
// The data base is destined to store the "external" histogrammes.
// "External" means not created inside HTOP but imported from the ROOT
// file as objects of the classes TH1F and TH2F.
// The data base has the same life time as the display panel.
//
//-------------------------------------------------------------------------

#include <TObjArray.h>

class HHistogram;

class HDisplayDB : public TObject {

 public:

  TObjArray *fContainer; //! container of histogrammes (HHistogram objects)

 public:

  HDisplayDB() { fContainer = new TObjArray(1000); }
  virtual ~HDisplayDB() { fContainer->Delete(); delete fContainer; }
  Bool_t add(HHistogram *p);
  HHistogram *at(Int_t idx);
  Int_t getEntries();
  Int_t find(const char* name);
  void import(const char* filename);
  
 public:

  ClassDef(HDisplayDB,0) //Runtime data base for the histogram display

};

#endif
