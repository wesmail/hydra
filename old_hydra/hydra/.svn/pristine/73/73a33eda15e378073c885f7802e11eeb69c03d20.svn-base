#include "hrtsavgolpar.h"
#include "hdetpario.h"
#include "hpario.h"
#include "hparamlist.h"
#include "hkicktask.h"
#include "hruntimedb.h"


/************************************************************/
/*                      HRSAVGOLPAR                         */
/************************************************************/

HRtSavGolPar::HRtSavGolPar(const char *name, const char *title,
    const char *context) : HParCond(name,title,context) {
  strcpy(detName,"Mdc");
  HKickTask::setKickParIo(HRuntimeDb::instance());
}

Bool_t HRtSavGolPar::init(HParIo *inp, Int_t *set) {
  HDetParIo *input = inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRtSavGolPar::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (io) return (out->write(this));
  return -1;
}

void HRtSavGolPar::putParams(HParamList* l) {
  if (!l) return;
  l->add("fNLeft",fNLeft);
  l->add("fNRight",fNRight);
  l->add("fM",fM);
} 

Bool_t HRtSavGolPar::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("fNLeft",&fNLeft)) return kFALSE;
  if (!l->fill("fNRight",&fNRight)) return kFALSE;
  if (!l->fill("fM",&fM)) return kFALSE;
  return kTRUE;
}

void HRtSavGolPar::readFrom(const HRtSavGolPar &o) {
  fNLeft = o.fNLeft;
  fNRight = o.fNRight;
  fM = o.fM;
}
  
ClassImp(HRtSavGolPar)  
