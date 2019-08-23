#if !defined HCONFIGURE_H
#define HCONFIGURE_H

//-------------------------------------------------------------------------
//
// File name:       hconfigure.h
// Subject:         Configuration of the analysis run.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            April 2000
//
// Remarks:         under construction
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HConfigure.
// This class keeps trace of the configuration of the analysis run
//-------------------------------------------------------------------------


#include <TObject.h>

class HConfigure : public TObject {

 public:

  Int_t fConfig[27];
  Text_t input[200];
  Text_t output[200];

 public:

  HConfigure() : TObject() {}
  virtual ~HConfigure() {}
  Bool_t configure();

 public:

  ClassDef(HConfigure,0) //Configuration of the analysis run

};

#endif






