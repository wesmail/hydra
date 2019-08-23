//*-- AUTHOR : J. Markert
////////////////////////////////////////////////////////////////////////////
// HMdcTrackFitPar
//
// Container class for Dubna track fitter parameters
//
////////////////////////////////////////////////////////////////////////////
#include "hmdctrackfitpar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include <iostream.h>
#include <stdlib.h>

ClassImp(HMdcTrackFitPar)

HMdcTrackFitPar::HMdcTrackFitPar(const char* name,const char* title,
                       const char* context)
    : HParCond(name,title,context)
{
    //
    strcpy(detName,"Mdc");
    driftVelocityMDC.Set(24);  // Drift Velocity MDC1
    maxIteration.Set(24);      // Maximal number of iterations
    step1.Set(24);             // step
    step2.Set(24);             // step
    step3.Set(24);             // step
    step4.Set(24);             // step
    step5.Set(24);             // step
    step6.Set(24);             // step
    weight1.Set(24);           // weight
    weight2.Set(24);           // weight
    weight3.Set(24);           // weight
    weight4.Set(24);           // weight
    weight5.Set(24);           // weight
    weight6.Set(24);           // weight
    stepNorm.Set(24);          // Initial value of step common for all parameters.
    tukeyConstant.Set(24);     // Tukey constant

    clear();
}
HMdcTrackFitPar::~HMdcTrackFitPar()
{
  // destructor
}
void HMdcTrackFitPar::clear()
{
    for(Int_t i=0;i<24;i++)
    {
	driftVelocityMDC[i]=0;
	maxIteration[i]=0;
	step1[i]=0;
	step2[i]=0;
	step3[i]=0;
	step4[i]=0;
	step5[i]=0;
	step6[i]=0;
	weight1[i]=0;
	weight2[i]=0;
	weight3[i]=0;
	weight4[i]=0;
	weight5[i]=0;
	weight6[i]=0;
	stepNorm[i]=0;
	tukeyConstant[i]=0;
    }
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HMdcTrackFitPar::printParam(void)
{
    // prints the parameters of HMdcTrackFitPar to the screen.
    printf ("############################################################################################\n");
    printf ("HMdcTrackFitPar:\n");
    printf (":driftVelocityMDC\n");
    for(Int_t s=0;s<6;s++){
	    printf ("%5.4f %5.4f %5.4f %5.4f\n",
                     driftVelocityMDC[s*4+0],driftVelocityMDC[s*4+1],
                     driftVelocityMDC[s*4+2],driftVelocityMDC[s*4+3]);
    }
    printf (":maxIteration\n");
    for(Int_t s=0;s<6;s++){
	    printf ("%i %i %i %i\n",
                     maxIteration[s*4+0],maxIteration[s*4+1],
                     maxIteration[s*4+2],maxIteration[s*4+3]);
    }
    printf (":step\n");
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    printf ("%i %i %f %f %f %f %f %f\n",s,m,
                     step1[s*4+m],step2[s*4+m],step3[s*4+m],
                     step4[s*4+m],step5[s*4+m],step6[s*4+m]);
	}
    }
    printf (":weight\n");
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    printf ("%i %i %f %f %f %f %f %f\n",s,m,
                     weight1[s*4+m],weight2[s*4+m],weight3[s*4+m],
                     weight4[s*4+m],weight5[s*4+m],weight6[s*4+m]);
	}
    }
    printf (":stepnorm\n");
    for(Int_t s=0;s<6;s++){
	    printf ("%f %f %f %f\n",
                     stepNorm[s*4+0],stepNorm[s*4+1],
                     stepNorm[s*4+2],stepNorm[s*4+3]);
    }
    printf (":tueckey\n");
    for(Int_t s=0;s<6;s++){
	    printf ("%f %f %f %f\n",
                     tukeyConstant[s*4+0],tukeyConstant[s*4+1],
                     tukeyConstant[s*4+2],tukeyConstant[s*4+3]);
    }
    printf ("############################################################################################\n");
}
Bool_t HMdcTrackFitPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
Int_t HMdcTrackFitPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}
void HMdcTrackFitPar::putParams(HParamList* l)
{
    // Puts all params of HMdcTrackFitPar to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;
    l->add("driftVelocityMDC",driftVelocityMDC);
    l->add("maxIteration",    maxIteration);
    l->add("step1",           step1);
    l->add("step2",           step2);
    l->add("step3",           step3);
    l->add("step4",           step4);
    l->add("step5",           step5);
    l->add("step6",           step6);
    l->add("weight1",         weight1);
    l->add("weight2",         weight2);
    l->add("weight3",         weight3);
    l->add("weight4",         weight4);
    l->add("weight5",         weight5);
    l->add("weight6",         weight6);
    l->add("stepNorm",        stepNorm);
    l->add("tukeyConstant",   tukeyConstant);
}
Bool_t HMdcTrackFitPar::getParams(HParamList* l)
{
    if (!l) return kFALSE;
    if(!( l->fill("driftVelocityMDC",&driftVelocityMDC))) return kFALSE;
    if(!( l->fill("maxIteration",    &maxIteration)))     return kFALSE;
    if(!( l->fill("step1",           &step1)))            return kFALSE;
    if(!( l->fill("step2",           &step2)))            return kFALSE;
    if(!( l->fill("step3",           &step3)))            return kFALSE;
    if(!( l->fill("step4",           &step4)))            return kFALSE;
    if(!( l->fill("step5",           &step5)))            return kFALSE;
    if(!( l->fill("step6",           &step6)))            return kFALSE;
    if(!( l->fill("weight1",         &weight1)))          return kFALSE;
    if(!( l->fill("weight2",         &weight2)))          return kFALSE;
    if(!( l->fill("weight3",         &weight3)))          return kFALSE;
    if(!( l->fill("weight4",         &weight4)))          return kFALSE;
    if(!( l->fill("weight5",         &weight5)))          return kFALSE;
    if(!( l->fill("weight6",         &weight6)))          return kFALSE;
    if(!( l->fill("stepNorm",        &stepNorm)))         return kFALSE;
    if(!( l->fill("tukeyConstant",   &tukeyConstant)))    return kFALSE;

    return kTRUE;
}
