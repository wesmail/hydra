#if !defined HBROWSERDB_H
#define HBROWSERDB_H

//-------------------------------------------------------------------------
//
// File name:       hbrowserdb.h
// Subject:         Run time data base for the browser and manager of online
//                  objects.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HBrowserDB.
// This class forms the run time data base for the manager of the online
// objects. The main destination of that data base is storing of the user
// defined parameters. It also keeps trace of all existing online objects
// and helps the panel to display them correctly in the list box of
// the manager.
// The data base has the same life time as the browser panel.
//
//-------------------------------------------------------------------------


#include <TObjArray.h>

class HUserParameter;

class HBrowserDB : public TObject {

 public:

  TObjArray *fContainer; //! container of user parameters
  Int_t aridx[501];
  Int_t tabpos;
  Int_t cursor;

 public:

  HBrowserDB();
  virtual ~HBrowserDB();
  Bool_t add(HUserParameter *p);
  HUserParameter *at(Int_t idx);
  Int_t getEntries();
  Int_t find(const char* name);
  void clear();
  void changed(Int_t tab);
  void putDefault();
  void putUserPar();

 public:
  
  ClassDef(HBrowserDB,0)

};

#endif
