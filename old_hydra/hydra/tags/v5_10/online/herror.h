#if !defined HERROR_H
#define HERROR_H

//-------------------------------------------------------------------------
//
// File name:       herror.h
// Subject:         Warning and error messages.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file cotains the declaration of the class HError.
// This class contains two static functions which open on the screen
// panels with warning or error messages. In case of a warning 
// the function returns integer value which tells the user which
// of the buttons "OK" or "Cancel" was clicked.
//
//-------------------------------------------------------------------------

#include <TObject.h>

class HError : public TObject {

 public:

  HError() : TObject() {}
  virtual ~HError() {}
  static void message(const char* errmsg);
  static Int_t warning(const char* wrnmsg);

 public:

  ClassDef(HError,0)

};

#endif
