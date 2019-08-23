//*-- AUTHOR : J. Markert
////////////////////////////////////////////////////////////////////////////
// HMdcDigitPar
//
// Container class for Mdc digitization parameters
//
////////////////////////////////////////////////////////////////////////////
#include "hmdcdigitpar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include <iostream.h>
#include <stdlib.h>

ClassImp(HMdcDigitPar)

HMdcDigitPar::HMdcDigitPar(const char* name,const char* title,
                       const char* context)
    : HParCond(name,title,context)
{
    //
    strcpy(detName,"Mdc");
    layEff.Set(144);
    clear();
}
HMdcDigitPar::~HMdcDigitPar()
{
  // destructor
}
void HMdcDigitPar::clear()
{
    for(Int_t i=0;i<144;i++)
    {
      layEff[i]=-99;
    }
    signalspeed=-99;
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HMdcDigitPar::printParam(void)
{
    // prints the parameters of HMdcDigitPar to the screen.
    printf ("############################################################################################\n");
    printf ("HMdcDigitPar:\n");
    printf ("LayerEfficiency:\n");
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    printf ("s %i m %i %5.4f %5.4f %5.4f %5.4f %5.4f %5.4f\n",
		    s,m,
		    layEff[(s*24)+(m*6)+0],layEff[(s*24)+(m*6)+1],layEff[(s*24)+(m*6)+2],
		    layEff[(s*24)+(m*6)+3],layEff[(s*24)+(m*6)+4],layEff[(s*24)+(m*6)+5]);
	}
    }
    printf ("SignalSpeed: %6.4f ns/mm\n",signalspeed);
    printf ("############################################################################################\n");
}
Bool_t HMdcDigitPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
Int_t HMdcDigitPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}
void HMdcDigitPar::putParams(HParamList* l)
{
    // Puts all params of HMdcDigitPar to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;
    l->add("layEff",     layEff);
    l->add("signalspeed",signalspeed);

}
Bool_t HMdcDigitPar::getParams(HParamList* l)
{
    if (!l) return kFALSE;
    if(!( l->fill("layEff",     &layEff)))      return kFALSE;
    if(!( l->fill("signalspeed",&signalspeed))) return kFALSE;

    return kTRUE;
}
