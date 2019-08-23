//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/12/2005
//*-- Version: $Id: hmatchutofheader.h,v 1.5 2005-07-15 09:00:24 wuestenf Exp $

#ifndef HMATCHUTOFHEADER_H
#define HMATCHUTOFHEADER_H
#include <TObject.h>

class HMatchUTofHeader : public TObject {
 private:
  Int_t fTriggerTag;   // Trigger number of event
  Int_t fID;           // Trigger id (type) of event
  Int_t fTriggerCode;  // 
  Int_t fVeto;         //
 public:
  HMatchUTofHeader(){};
  ~HMatchUTofHeader(){};
  void setTriggers(Int_t i, Int_t j){
    fTriggerTag  = i;
    fTriggerCode = j;
  };

  void setID( Int_t i){ fID=i;};
  void setVeto( Int_t i){fVeto=i;};
  Int_t getTriggerTag(){ return fTriggerTag;};
  Int_t getTriggerCode(){return fTriggerCode;};
  Int_t getVeto(){return fVeto;};
  Int_t getID(){return fID;};
                    
  ClassDef(HMatchUTofHeader,0) // Tof header of matching unit subevent
};

#endif
