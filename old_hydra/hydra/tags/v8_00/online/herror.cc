//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/

#include "herror.h"
#include <TGMsgBox.h>

//-------------------------------------------------------------------------
//
// File name:       herror.cc
// Subject:         Warning and error messages.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file cotains the source code for the member functions of 
// the class HError.
// This class contains two static functions which open on the screen
// panels with warning or error messages. In case of a warning 
// the function returns integer value which tells the user which
// of the buttons "OK" or "Cancel" was clicked.
//
//-------------------------------------------------------------------------

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
// This class contains two static functions which open on the screen
// panels with warning or error messages. In case of a warning 
// the function returns integer value which tells the user which
// of the buttons "OK" or "Cancel" was clicked.
////////////////////////////////////////////////////////////////////
ClassImp(HError) 

//---------------------------------------------------------------------
//******  HError::message(const char* errmsg)
//
// Error message.
//
//---------------------------------------------------------------------

void HError::message(const char* errmsg) {
//
// Error message.
//

  char msg[5000];
  Int_t retval;

  if(errmsg) {

    sprintf(msg,"%s",errmsg);
    new TGMsgBox(gClient->GetRoot(),gClient->GetRoot(),"Error message",
		 msg, kMBIconExclamation,kMBOk,&retval);
  }

}


//---------------------------------------------------------------------
//******  HError::warning(const char* errmsg)
//
// Warning message.
//
//---------------------------------------------------------------------

Int_t HError::warning(const char* wrnmsg) {
//
// Warning message.
//

  char msg[5000];
  Int_t retval;
  Int_t buttons;
  buttons = 0;
  buttons |= kMBOk;
  buttons |= kMBCancel;
  
  if(wrnmsg) {

    sprintf(msg,"%s",wrnmsg);
    new TGMsgBox(gClient->GetRoot(),gClient->GetRoot(),"Warning message",
		 msg, kMBIconExclamation,buttons,&retval);
  }

  if(retval == 4) return (retval>>2); // "OK" button clicked
  return (retval>>5);                 // "Cancel" button clicked

}






