//*-- Author : Ilse Koenig
//*-- Modified : 11/07/99 by Ilse Koenig

///////////////////////////////////////////////////////////////////////////////
// HMdcCal1EvtDisplay
// 
// class for the event display of the MDC cal1 level
///////////////////////////////////////////////////////////////////////////////

#include "hmdccal1evtdisplay.h"
#include "hmdcevtdisplay.h"
#include "hmdcnodes.h"
#include "hmdccal1.h"
#include "hiterator.h"
#include "hcategory.h"

ClassImp(HMdcCal1EvtDisplay)

HMdcCal1EvtDisplay::HMdcCal1EvtDisplay(HCategory* c,HMdcNodes* n) {
  fName="MdcCal1EvtDisplay";
  fTitle="Mdc Cal1 event display";
  cat=c;
  nodes=n;
  isActive=kTRUE;
  tabtLim1=0.;
  tabtLim2=50000.;
}


Bool_t HMdcCal1EvtDisplay::reinit(void) {
  iter=(HIterator *)cat->MakeIterator("native");
  return kTRUE;
}


Int_t HMdcCal1EvtDisplay::execute() {
  clear();
  Int_t s,m,l,c;
  iter->Reset();
  HMdcCal1* cal=0;
  TNode* node=0;
  while ((cal=(HMdcCal1*)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    Float_t dt=cal->getTime1()-cal->getTime2();
    node=nodes->getNode(s,m,l,c);
    if (node && dt>tabtLim1 && dt<tabtLim2) node->SetVisibility(1);
  }
  return 1;
}     


void HMdcCal1EvtDisplay::clear(void) {
  nodes->clear();
}
