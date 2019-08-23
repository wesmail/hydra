//*-- AUTHOR : Olga Pechenova
//*-- Modified : 2007 by Olga Pechenova
//
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HMdcLayerCorrPar
//
//  Container class keep parameters for distance correction (between 2 parts of layer)
//
//
//
//  Alternative method of getting parameters (without usage of runTimeDb):
//  setDefaultPar();
//
/////////////////////////////////////////////////////////////////////////


#include <TString.h>
#include "hmdclayercorrpar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"

#include <stdlib.h> 

ClassImp(HMdcLayerCorrPar)

HMdcLayerCorrPar::HMdcLayerCorrPar(const Char_t* name,const Char_t* title,
                const Char_t* context)
           : HParCond(name,title,context) {
  // constructor
  nMdc = 4;
  nLay = 6;
  numCorrLayers = 72;
  for(Int_t s=0;s<numCorrLayers;s++) {
    firstWire[s] = 777;
    shift[s] = 0.;
  }    
}

Bool_t HMdcLayerCorrPar::init(HParIo* inp,Int_t* set) {
 // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

void HMdcLayerCorrPar::clear(void) {
  for(Int_t s=0;s<numCorrLayers;s++) {
    firstWire[s] = 777;
    shift[s] = 0.;
  }    
  status=kFALSE;
  resetInputVersions();
  changed=kFALSE;
}

Int_t HMdcLayerCorrPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return 0;
}

void HMdcLayerCorrPar::putParams(HParamList* l) {
  // puts all parameters to the parameter list, which is used by the io
  if (!l) return;
  l->addBinary("firstWire",firstWire,numCorrLayers);
  l->addBinary("shift",shift,numCorrLayers);
}

Bool_t HMdcLayerCorrPar::getParams(HParamList* l) {
  // gets all parameters from the parameter list, which is used by the io
  if (!l) return kFALSE;
  if (!(l->fillBinary("firstWire",firstWire,numCorrLayers))) return kFALSE;
  if (!(l->fillBinary("shift",shift,numCorrLayers)))         return kFALSE;
  return kTRUE;
}


Bool_t HMdcLayerCorrPar::addLayerShift(Int_t s,Int_t m,Int_t l,
    Int_t fstWr,Float_t sh) {
  // add or change correction parameters
  if(fstWr>0 && isAddOk(s,m,l)) {
    Int_t ind = (s*(nMdc-2)+(m-2))*nLay+l;
    firstWire[ind] = fstWr;
    shift[ind]     = sh;
    return kTRUE;
  }
  return kFALSE;
}

void HMdcLayerCorrPar::setDefaultPar(void) {
  addLayerShift(0,2,1,96,0.25);
  addLayerShift(0,2,2,88,0.15);
  addLayerShift(0,2,3,82,0.1);
  addLayerShift(0,2,5,96,0.05);
  addLayerShift(0,3,0,92,0.3);
  addLayerShift(0,3,1,104,0.45);
  addLayerShift(0,3,2,82,0.6);
  addLayerShift(0,3,3,87,0.1);
  addLayerShift(0,3,4,93,0.2);
  addLayerShift(0,3,5,48,0.55);

  addLayerShift(1,2,0,96,0.15);
  addLayerShift(1,2,1,96,0.1);
  addLayerShift(1,2,3,83,0.2);
  addLayerShift(1,2,4,98,0.35);
  addLayerShift(1,3,0,93,0.18);
  addLayerShift(1,3,1,102,0.45);
  addLayerShift(1,3,2,77,0.22);
  addLayerShift(1,3,3,87,-0.1);
  addLayerShift(1,3,5,100,0.15);

  addLayerShift(2,2,2,92,0.15);
  addLayerShift(2,2,4,98,0.1);
  addLayerShift(2,3,2,64,0.23);
  addLayerShift(2,3,3,99,0.23);
  addLayerShift(2,3,4,84,-0.13);

  addLayerShift(3,2,1,97,0.5);
  addLayerShift(3,2,2,83,0.23);
  addLayerShift(3,2,4,97,0.1);
  addLayerShift(3,3,0,87,0.27);
  addLayerShift(3,3,1,99,0.45);
  addLayerShift(3,3,2,95,0.22);
  addLayerShift(3,3,3,101,0.53);
  addLayerShift(3,3,4,99,0.3);
  addLayerShift(3,3,5,87,0.4);

  addLayerShift(4,2,1,95,0.1);
  addLayerShift(4,2,2,87,0.21);
  addLayerShift(4,2,4,97,0.15);
  addLayerShift(4,3,0,91,0.65);
  addLayerShift(4,3,1,100,0.3);
  addLayerShift(4,3,2,99,0.4);
  addLayerShift(4,3,3,87,0.1);
  addLayerShift(4,3,4,100,0.1);

  addLayerShift(5,2,1,92,-0.05);
  addLayerShift(5,2,2,78,0.1);
  addLayerShift(5,2,3,91,0.15);
  addLayerShift(5,3,0,90,-0.1);
  addLayerShift(5,3,2,86,-0.2);
  addLayerShift(5,3,3,85,-0.13);
  addLayerShift(5,3,4,110,0.45);
  addLayerShift(5,3,5,95,0.23);

}

Bool_t HMdcLayerCorrPar::getLayerCorrPar(Int_t s,Int_t m,Int_t l,
    Int_t& fstWr,Float_t& sh) const {
  if(isAddOk(s,m,l)) {
    Int_t ind = (s*(nMdc-2)+(m-2))*nLay+l;
    fstWr = firstWire[ind];
    sh    = shift[ind];
  } else {
    fstWr = 777;
    sh    = 0.;
    return kFALSE;
  }
  return kTRUE;
}

