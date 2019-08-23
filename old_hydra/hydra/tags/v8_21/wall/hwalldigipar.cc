//_HADES_CLASS_DESCRIPTION
//created by I.Koenig 21.11.2006
/////////////////////////////////////////////////////////////////////////
//
//  HWallDigiPar
//  Container for the WALL digitisation parameters
//
/////////////////////////////////////////////////////////////////////////

#include "TString.h"
#include "hwalldigipar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"

ClassImp(HWallDigiPar)

HWallDigiPar::HWallDigiPar(const Char_t* name,const Char_t* title,
                           const Char_t* context)
             : HParCond(name,title,context) {
  // constructor
  clear();
}

Bool_t HWallDigiPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HWallDigiPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return -1;
}

void HWallDigiPar::clear() {
  // clears the container
  for(Int_t i=0;i<getSize();i++) {
    TDC_slope[i]     = 0.F;
    TDC_offset[i]    = 0.F;
    ADC_slope[i]     = 0.F;
    ADC_offset[i]    = 0.F;
    CFD_threshold[i] = 0.F;
    ADC_threshold[i] = 0.F;
  }
  status=kFALSE;
  resetInputVersions();
}

void HWallDigiPar::putParams(HParamList* l) {
  // puts all parameters to the parameter list, which is used by the io
  if (!l) return;
  l->add("TDC_slope",TDC_slope,WALL_MAXCELLS);
  l->add("TDC_offset",TDC_offset,WALL_MAXCELLS);
  l->add("ADC_slope",ADC_slope,WALL_MAXCELLS);
  l->add("ADC_offset",TDC_offset,WALL_MAXCELLS);
  l->add("CFD_threshold",CFD_threshold,WALL_MAXCELLS);
  l->add("ADC_threshold",ADC_threshold,WALL_MAXCELLS);
}

Bool_t HWallDigiPar::getParams(HParamList* l) {
 // gets all parameters from the parameter list, which is used by the io
  if (!l) return kFALSE;
  if (!(l->fill("TDC_slope",TDC_slope,WALL_MAXCELLS)))         return kFALSE;
  if (!(l->fill("TDC_offset",TDC_offset,WALL_MAXCELLS)))       return kFALSE;
  if (!(l->fill("ADC_slope",ADC_slope,WALL_MAXCELLS)))         return kFALSE;
  if (!(l->fill("ADC_offset",TDC_offset,WALL_MAXCELLS)))       return kFALSE;
  if (!(l->fill("CFD_threshold",CFD_threshold,WALL_MAXCELLS))) return kFALSE;
  if (!(l->fill("ADC_threshold",ADC_threshold,WALL_MAXCELLS))) return kFALSE;
  return kTRUE;
}
