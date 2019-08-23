#if !defined HINDEXTRANSFORM_H
#define HINDEXTRANSFORM_H

//-------------------------------------------------------------------------
//
// File name:       hindextransform.h
// Subject:         Index transformations between multi and one dimensional
//                  arrays.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the class HIndexTransform.
// This class contains a set of static functions which allow to perform
// index transformations between multi and one dimensional arrays.
//
//-------------------------------------------------------------------------

#include <TObject.h>

class HIndexTransform : public TObject {

 public:

  HIndexTransform() : TObject() {}
  virtual ~HIndexTransform() {}
  static Int_t  mult2lin(Int_t n, Int_t* maxind, Int_t* idx);
  static Bool_t lin2mult(Int_t n, Int_t* maxind, Int_t idx, Int_t* retar);
  static void  print(Int_t n, Int_t* maxind, Int_t* idx);
  static void print(Int_t n, Int_t* maxind, Int_t idx);

 public:
  
  ClassDef(HIndexTransform,0) //Index transformations between multi and one dimensional arrays

};

#endif





