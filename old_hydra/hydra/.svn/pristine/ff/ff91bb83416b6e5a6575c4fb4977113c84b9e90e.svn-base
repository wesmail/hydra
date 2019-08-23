#ifndef HMDCTRACKGCORRPAR_H
#define HMDCTRACKGCORRPAR_H
#include "TString.h"
#include "TObject.h"
#include "TNamed.h"
#include "hparcond.h"
#include "hpario.h"
#include "hmdcseg.h"
#include "hmdcsegsim.h"
//CORRECTION PARAMETERS FOR MOMENTUM RECONSTRUCTION
class HMdcTrackGField;
class HMdcTrackGCorrections;
class HMdcTrackGSpline;
class HMdcTrackGFieldPar;
class HGeomVector;

class HMdcTrackGCorrPar:public HParCond {
protected:
    Int_t doCorrections;
//    HMdcTrackGField* field; //!
    HMdcTrackGCorrections* corr;
    HMdcTrackGSpline* spline;  //!
    HMdcTrackGFieldPar *fieldmap; //!
public:
   
   HMdcTrackGCorrPar(const char* name="MdcTrackGCorrPar",
		     const char* title="Correction parameters for Spline fitting",
		     const char* context="MdcTrackGCorrParProduction");
    ~HMdcTrackGCorrPar();
    Bool_t  init(HParIo*, Int_t*);
    Int_t   write(HParIo*);
    void    putParams(HParamList*);
    Bool_t  getParams(HParamList*);//{return kTRUE;}
    void    clear();
    void    setCorrectionFlag(Int_t flag){ doCorrections=flag;}
    Int_t   getCorrectionFlag(){ return doCorrections;}
    void    initCorrections(TString, TString,TString,TString);
    Double_t getphiI(Int_t i);
    Double_t calcMomentum(HGeomVector*,Bool_t cond=kTRUE,Double_t targetz=0.);
    Double_t calcMomentum(HMdcSeg *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   
 Double_t calcMomentum123(HMdcSeg *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
 void getDistance(Double_t *);
    Double_t getMetaDistance(Double_t,Double_t,Double_t);
//    void    calcField(Double_t *a,Double_t *b,Int_t i);

 //  HMdcTrackGField* getField(){return field;}
   HMdcTrackGCorrections* getCorr(){return corr;}
   HMdcTrackGSpline* getSPline() {return spline;}
    ClassDef(HMdcTrackGCorrPar,1)
};
#endif
