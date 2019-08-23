
using namespace std;
#include "hmdctrackgfieldpar.h"
#include "hmdctrackgcorrections.h"
#include "hgeomvector.h"
#include "hmdctrackgspline.h"
#include"hmdctrackgfield.h"
#include"hpario.h"
#include"hdetpario.h"
#include"hparamlist.h"
#include"hades.h"
#include"hmdcdetector.h"
#include"hspectrometer.h"
#include"hparrootfileio.h"
#include"hruntimedb.h"
#include<iostream>
#include"stdlib.h"

ClassImp(HMdcTrackGFieldPar);


HMdcTrackGFieldPar::HMdcTrackGFieldPar(const char* name, const char* title, const char* context)
    :HParCond(name,title,context)
{
    field = new HMdcTrackGField("Fieldmap","Fieldmap");
}

HMdcTrackGFieldPar::~HMdcTrackGFieldPar()
{
    if(field)  delete field;
}

Bool_t HMdcTrackGFieldPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  
  if (input) return input->init(this,set);
  
  return kFALSE;
}
Int_t HMdcTrackGFieldPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcTrackGFieldPar::putParams(HParamList* l)
{
    // Puts all params of HMdcDigitPar to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;
    l->addBinary("field",   field);
}
Bool_t HMdcTrackGFieldPar::getParams(HParamList* l)
{
    if (!l) return kFALSE;
    if(!( l->fillBinary("field",field))) return kFALSE;
    return kTRUE;
}
void HMdcTrackGFieldPar::clear()
{
    field->clear();
}
void HMdcTrackGFieldPar::initFieldMap(TString input)
{
field->init(input);
}
void HMdcTrackGFieldPar::calcField(Double_t *a,Double_t *b,Int_t i){ field->calcField(a,b,i);}

