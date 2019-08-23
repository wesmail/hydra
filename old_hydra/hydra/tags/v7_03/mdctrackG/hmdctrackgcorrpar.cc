//*--AUTHOR A.RUSTAMOV
using namespace std;
#include "hmdctrackgcorrpar.h"
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

ClassImp(HMdcTrackGCorrPar);


HMdcTrackGCorrPar::HMdcTrackGCorrPar(const char* name, const char* title, const char* context)
    :HParCond(name,title,context)
{
    fieldmap = new HMdcTrackGFieldPar("FieldmapParameters","FieldmapParameters");
    corr  = new HMdcTrackGCorrections("Corrections","Corrections");
    spline= new HMdcTrackGSpline("SplineMomentum","SplineMomentum");
    corr1=new HMdcTrackGCorrections("Corrections1","Corrections1");
}

HMdcTrackGCorrPar::~HMdcTrackGCorrPar()
{
    if(fieldmap)  delete fieldmap;
    if(corr)   delete corr;
    if(corr1) delete corr1;
    if(spline) delete spline;
}

Bool_t HMdcTrackGCorrPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  Bool_t check=kFALSE;
  if (input)   check=(input->init(this,set));
  //spline->setDataPointer(fieldmap->getPointer(),corr1);
  return check;
}
Int_t HMdcTrackGCorrPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcTrackGCorrPar::putParams(HParamList* l)
{
    // Puts all params of HMdcDigitPar to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;
    l->add("doCorrections", doCorrections);
//    l->addBinary("field",   field);
    l->addBinary("corr",    corr);
    l->addBinary("corr1",    corr1);
//     l->addBinary("spline",    spline);
}
Bool_t HMdcTrackGCorrPar::getParams(HParamList* l)
{
    if (!l) return kFALSE;
    if(!( l->fill("doCorrections",     &doCorrections)))      return kFALSE;
//    if(!( l->fillBinary("field",field))) return kFALSE;
    if(!( l->fillBinary("corr",corr))) return kFALSE;
    if(!( l->fillBinary("corr1",corr1))) return kFALSE;
    return kTRUE;
}
void HMdcTrackGCorrPar::clear()
{
///    field->clear();
    corr ->clear();
}
//void HMdcTrackGCorrPar::initFieldMap(TString input)
//{
//  field->init(input);
//}

void HMdcTrackGCorrPar:: initCorrections(TString inputE,TString inputP,TString inputE3,TString inputP3)
{
    corr->init(inputE,inputP,inputE3,inputP3);
}

void HMdcTrackGCorrPar:: initCorrections1(TString inputE,TString inputP,TString inputE3,TString inputP3)
{
    corr1->init(inputE,inputP,inputE3,inputP3);
}
    

Double_t HMdcTrackGCorrPar:: calcMomentum(HGeomVector* vector,Bool_t cond,Double_t target)
{
    return spline->calcMomentum(vector,cond,target);
}
Double_t HMdcTrackGCorrPar:: calcMomentum(HMdcSeg *seg[2] ,Bool_t cond,Double_t target)
{
  return spline->calcMomentum(seg,cond,target);
}



Double_t HMdcTrackGCorrPar:: calcMomentum123(HMdcSeg *seg[2] ,Bool_t cond,Double_t target)
{
  return spline->calcMomentum123(seg,cond,target);
}
void HMdcTrackGCorrPar::getDistance(Double_t * dist)
{
   spline->getDistance(dist);
}

Double_t HMdcTrackGCorrPar::getMetaDistance(Double_t x,Double_t y,Double_t z)
{
  return ( spline->getMetaDistance(x,y,z));
}

void HMdcTrackGCorrPar::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcTrackGCorrPar.
   
      UInt_t R__s, R__c;
         if (R__b.IsReading()) {
	    Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
	    HParCond::Streamer(R__b);
	    R__b >> doCorrections;
	    R__b >> corr;
	    
	    if(R__v==2)
	    {
	    R__b >> corr1;
	    }
	     else if(R__v<=1)
		 {
		  corr1=0;
		 }
	    R__b.CheckByteCount(R__s, R__c, HMdcTrackGCorrPar::IsA());
	 } else {
	 R__c = R__b.WriteVersion(HMdcTrackGCorrPar::IsA(), kTRUE);
	 HParCond::Streamer(R__b);
	 R__b << doCorrections;
	 R__b << corr;
	 R__b << corr1;
	 R__b.SetByteCount(R__c, kTRUE);
	 }
 }
										       