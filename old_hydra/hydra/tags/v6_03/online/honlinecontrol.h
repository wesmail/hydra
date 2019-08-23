#if !defined HONLINECONTROL_H
#define HONLINECONTROL_H

//-------------------------------------------------------------------------
//
// File name:       honlinecontrol.h
// Subject:         Main control class of HTOP
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it" 
// Date:            February 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the declaration of the main control class 
// HOnlineControl. This class maintains containers of detector sets, 
// histogrammes, local and global conditions, and contours. It also
// contains pointers to the objects of classes HConsistency and HMemoryCheck
// which are responsible of performing control that all run-time defined
// objects are self consistent and do not exceed memory limit defined by
// a user for a particular analysis session. 
// Only one object of this class can be created during the session. This
// object can be always addressed by means of a global pointer gOnline.
//
//-------------------------------------------------------------------------

#include <TObject.h>

class HDetSetCont;
class HLocalCont;
class HGlobalCont;
class HHistoCont;
class HContourCont;
class HConsistency;
class HMemoryCheck;

class HOnlineControl : public TObject {

 protected:

  HDetSetCont       *fDetSet;      // container of detector sets
  HLocalCont        *fLocal;       // container of local conditions
  HGlobalCont       *fGlobal;      // container of global conditions
  HHistoCont        *fHisto;       // container of histogrammes
  HContourCont      *fContour;     // container of contours
  HConsistency      *fConsistency; //!
  HMemoryCheck      *fMemCheck;
  char               savefile[200]; //!

 public:

  HOnlineControl();
  virtual ~HOnlineControl();
  Bool_t init();
  Bool_t read();
  void write();
  HDetSetCont* getDetSet() const { return fDetSet; }
  HLocalCont* getLocal() const { return fLocal; }
  HGlobalCont* getGlobal() const { return fGlobal; }
  HHistoCont* getHisto() const { return fHisto; }
  HContourCont* getContour() const { return fContour; }
  HConsistency* getConsistency() const { return fConsistency; }
  HMemoryCheck* getMemCheck() const { return fMemCheck; }
  void setSaveFile(const char* name) { if(name) sprintf(savefile,"%s",name); }

 public:

  ClassDef(HOnlineControl,1) //Main control class of HTOP (HADES TOF Online Project)

};

R__EXTERN HOnlineControl *gOnline;

#endif


