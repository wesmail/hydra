// $Id: hpairqa.h,v 1.6 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-04-20 18:02:10
//
// ---------------------------------------------------------------------------
#ifndef HPAIRQA_H
#define HPAIRQA_H
// ---------------------------------------------------------------------------
#include <TNamed.h>
// ---------------------------------------------------------------------------
class TFile;
class TNtuple;
class TObjArray;
class TString;
class TTree;
// ---------------------------------------------------------------------------
class HCategory;
class HIterator;
// ---------------------------------------------------------------------------
class HPair;
class HPairExt;
class HPairData;
class HPairGeantData;
class HPairSim;
class HPairSimExt;
// ---------------------------------------------------------------------------

class HPairQA : public TNamed {


public:

    HPairQA();
    HPairQA(const Text_t *name, const Text_t *title);
    ~HPairQA();
    

    Bool_t fillData(HIterator*);
    Bool_t fillData(TObjArray*);
    Bool_t fillData(HPair*);
    Bool_t fillData(HPairExt*);
    Bool_t fillData(HPairSim*);
    Bool_t fillData(HPairSimExt*);
    void setOutputFileName(TString &s){cOutputFileName = s;};
    void setNTupleFillingActive(Int_t n){nFlagNTupleFilling = n;};
    void setTTreeFillingActive(Int_t n){nFlagTTreeFilling = n;};
    void setHistogramFillingActive(Int_t n){nFlagHistogramFilling = n;};
    void setCutQAFillingActive(Int_t n){nFlagCutQAFilling = n;};
    void dumpToStdout(void){;}
    Bool_t finalize();

ClassDef(HPairQA,0)
// ---------------------------------------------------------------------------

private:
    Int_t nNbCutItems;
    Int_t nNbNtupleItems;
    TString cOutputFileName;
    Int_t nFlagNTupleFilling;
    Int_t nFlagTTreeFilling;
    Int_t nFlagHistogramFilling;
    Int_t nFlagCutQAFilling;
    HCategory* pCatPair;
    HIterator* pIterPair;
    TObjArray* pHistArray;
    TNtuple* pNTupleOut;
    TTree* pTTreeOut;
    TFile *pFile;
    Bool_t kIsInit;
    Bool_t kIsReal;
    void reset(void);
    Bool_t init();
    Bool_t createNTuple();
    Bool_t createTTree();
    Bool_t fillHistograms(const HPairData&); 
    Bool_t fillNtuple(const HPairData&); 
    Bool_t fillNtuple(const HPairGeantData&); 
    void fillTH1F(const char*,const char*,Float_t,Float_t weight=1.);
    void fillTH2F(const char*,const char*,Float_t,Float_t,Float_t weight1=1.);

};
#endif // HPAIRQA_H
