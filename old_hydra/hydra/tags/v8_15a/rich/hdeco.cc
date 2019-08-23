#include "hevent.h"
#include "hdeco.h"
#include "hades.h"
#include <iostream> 
#include "hrecevent.h"
#include "hpartialevent.h"
#include "heventheader.h"
#include <TString.h>
ClassImp(HDeco)
  HDeco::HDeco(){
}
HDeco::~HDeco(){
}
HDeco::HDeco(Text_t *name,Text_t *title, Option_t *sel): HReconstructor(name, title) {
  sele = sel;
  sele.ToLower();
}

Int_t HDeco::execute(){
  TString trig="trigger";

  HRecEvent *event=0;
  event=(HRecEvent *)gHades->getCurrentEvent();
  Int_t flag =event->getHeader()->getDownscalingFlag();
  // cout<< " flag  "<<flag<<endl;
  //cout<<" choice "<<sele<<endl;
  if(sele.CompareTo(trig)==0){
  if(flag==1) {
    // cout<<" rejecting downsc"<<endl;
    return kSkipEvent;

  }
  }
  else if(flag==0) return kSkipEvent;
 
  return 0;
} 
Bool_t  HDeco::init(){
  cout<<" selecting seocnd level trigger data"<<endl;
  return kTRUE;
}
