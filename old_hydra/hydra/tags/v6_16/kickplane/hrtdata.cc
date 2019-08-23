#include "hrtdata.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hrtfunctional.h"
#include "hkicktask.h"
#include "hruntimedb.h"


/************************************************************/
/*                      HRTDATA                             */
/************************************************************/

HRtData::HRtData(const char *name, const char *title, const char *context) 
        : HParSet(name, title, context) {
  strcpy(detName,"Mdc");
  HKickTask::setKickParIo(HRuntimeDb::instance());
  fGrid[0] = fGrid[1] = 0;
}

HRtData::~HRtData(void) {
  for (int i=0;i<2;i++)
    if (fGrid[i]) {
      delete fGrid[i];
      fGrid[i]=0;
    }
}

Bool_t HRtData::init(HParIo *inp, Int_t *set) {
  HDetParIo *input = inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRtData::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (out) return (out->write(this));
  return -1;
}

void HRtData::stealData(HRtData &o) {
  delete fGrid[0];
  delete fGrid[1];
  fGrid[0] = o.fGrid[0];
  fGrid[1] = o.fGrid[1];
  o.clear();
}
  
ClassImp(HRtData)  

