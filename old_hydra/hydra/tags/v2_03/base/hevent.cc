#include "hevent.h"

#include "hdebug.h"
#include <TTree.h>
#include <TBranch.h>
#include <TBranchClones.h>
#include <TBranchObject.h>
#include <TROOT.h>
#include <TDataMember.h>
#include <TRealData.h>
#include <TDataType.h>
#include <TClass.h>
#include "hades.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 22/04/98 by Manuel Sanchex
//*-- Copyright :GENP (Univ. Santiago de Compostela)

////////////////////////////////////
//HEvent Begin_Html
//<a href="Estructura_suceso.gif">(UML Diagram)</a> End_Html
//
//  ABC for the different kind of events (HPartialEvent,HRecEvent,
//HSimulatedEvent...)
//
//  An HEvent holds the data corresponding to an event, those data are arranged
// in categories (mdc raw data, mdc cal data, rich rings,...) which are objects
// instantianting one HCategory, to access one of these categories the user
// can call HEvent::getCategory(). Within the categories, each data object has
// a location (given by an object instantiating HLocation) which identifies 
// that object in the category (in some sense, the location is a generalized
// index).
//
//  So to acces one data object, firstly one needs to get the category and then
// the particular data object.
//
//  This class defines the most important functions any event must provide to
//access the data contained into it:
//
//  getCategory(Cat_t aCat):
//    returns the category identified by aCat
//
//  Clear(void)
//    clears the event. (Before loading a new event)
//
//  clearAll(Int_t level)
//    Clears the data in the event and the event structure 
//    (list of subevents...) depending on the value of "level".
//
//  makeBranch(TBranch *parent)
//    generates Root TTree branches for the data contained in the event and 
//    hangs them from "parent"
//
//  addCategory(Cat_t cat,HCategory *category,Option_t *opt)
//    This function adds the category "category" identified by "cat" to the 
//    event's structure.
////////////////////////////////


EXTERN TTree *gTree;

ClassImp(HEvent)

HEvent::HEvent(void) {
  fName=new TString("HEvent");
  fTitle=new TString("HEvent");
}

HEvent::HEvent(const Text_t *aName,const Text_t *aTitle) : 
  fName(0),fTitle(0) {
    fName=new TString(aName);
    fTitle=new TString(aTitle);
}

HEvent::~HEvent(void) {
  delete fName;
  delete fTitle;
}

void HEvent::Streamer(TBuffer &R__b)
{
   // Stream an object of class HEvent.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      fName->Streamer(R__b);
      fTitle->Streamer(R__b);
   } else {
      R__b.WriteVersion(HEvent::IsA());
      TObject::Streamer(R__b);
      fName->Streamer(R__b);
      fTitle->Streamer(R__b);
   }
}
