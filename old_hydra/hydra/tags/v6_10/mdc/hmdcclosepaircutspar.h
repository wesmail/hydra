#ifndef HMDCCLOSEPAIRCUTSPAR_H
#define HMDCCLOSEPAIRCUTSPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"
class HMdcDetector;
class HMdcClosePairCutsParCut : public TObject {
 protected:
  Float_t efficiency;      // single recognition efficiency
  Float_t sig2back;        // signal to background ratio
  Float_t mdc_size_cut;    // mdc cluster size cut
  Float_t mdc_wires_cut;   // mdc cluster contributing wires  cut
  Float_t mdc_nmerged_cut; // mdc cluster number of merged clusters 
  Float_t rich_charge_cut; // rich integrated charge in ring cut
  
 public:
  HMdcClosePairCutsParCut() : efficiency(0.F), sig2back(0.F),mdc_size_cut(0.F), mdc_wires_cut(0.F),mdc_nmerged_cut(0.F),rich_charge_cut(0.F) {;}
  ~HMdcClosePairCutsParCut() {;}
  Float_t getEfficiency() {return efficiency;}
  Float_t getSig2Back() {return sig2back;}
  Float_t getMdcClsSizeCut() {return mdc_size_cut ;}
  Float_t getMdcContWiresCut() {return mdc_wires_cut;}
  Float_t getRichIntChargeCut(){return rich_charge_cut;}
  Float_t getMdcMergedClsCut(){return mdc_nmerged_cut;} 

  void fill(Float_t e,Float_t s ,Float_t ms,Float_t mw, Float_t mc, Float_t rch ) {
    efficiency=e;    
    sig2back=s;        
    mdc_size_cut=ms;    
    mdc_wires_cut=mw;
    mdc_nmerged_cut=mc;    
    rich_charge_cut=rch;
  }
  void fill(HMdcClosePairCutsParCut& r) {
    efficiency=r.getEfficiency();    
    sig2back=r.getSig2Back();        
    mdc_size_cut=r.getMdcClsSizeCut();    
    mdc_wires_cut=r.getMdcContWiresCut();
    mdc_nmerged_cut=r.getMdcMergedClsCut();    
    rich_charge_cut=r.getRichIntChargeCut();
    }
  void setEfficiency(Float_t e) {efficiency=e;}
  void setSig2Back(Float_t s) {sig2back=s;}
  void setMdcClsSizeCut(Float_t ms) {mdc_size_cut=ms;}
  void setMdcContWiresCut(Float_t mw) {mdc_wires_cut=mw;}
  void setMdcMergedClsCut(Float_t mc) {mdc_nmerged_cut=mc;}
  void setRichIntChargeCut(Float_t mch) {rich_charge_cut=mch;}

  void clear() {
    efficiency=0.0;    
    sig2back=0.0;        
    mdc_size_cut=0.0;    
    mdc_wires_cut=0.0;
    mdc_nmerged_cut=0.0;    
    rich_charge_cut=0.0;
  }
  
  void print(){
    printf("%4.2f %6.2f %4.0f %4.0f %4.0f %5.0f\n",efficiency,sig2back,mdc_size_cut,mdc_wires_cut,mdc_nmerged_cut,rich_charge_cut);
    
  }
    ClassDef(HMdcClosePairCutsParCut,1) // Cut level of  ClosePairCuts  parameters 
};


class HMdcClosePairCutsParEf : public TObject {
protected:
    TObjArray* array; // array of pointers of type HMdcClosePairCutsParCut
public:
    HMdcClosePairCutsParEf(Int_t n=4);
    ~HMdcClosePairCutsParEf();
    HMdcClosePairCutsParCut& operator[](Int_t i) {
      return *static_cast<HMdcClosePairCutsParCut*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcClosePairCutsParEf,1) // Efficiency bin level of  ClosePairCuts  parameters 
};

class HMdcClosePairCutsParPhi : public TObject {
protected:
    TObjArray* array;   // array of pointers of type HMdcClosePairCutsParEf
public:
    HMdcClosePairCutsParPhi(Int_t m=3, Int_t n = 4);
    ~HMdcClosePairCutsParPhi();
    HMdcClosePairCutsParEf& operator[](Int_t i) {
      return *static_cast<HMdcClosePairCutsParEf*>((*array)[i]);
    }
    Int_t getSize();
    ClassDef(HMdcClosePairCutsParPhi,1) //  Phi bin level of  ClosePairCuts  parameters 
};



class HMdcClosePairCutsPar : public HParSet {
 protected:
  TObjArray* array;     //  array of pointers of HMdcClosePairCutsParPhi 
  HMdcDetector *fMdc;
  TString comment;
 public:
  HMdcClosePairCutsPar(const char* name="MdcClosePairCutsPar",const char* title="close pair rejection  parameters for Mdc-Rich",const char* context="",Int_t l=8,Int_t m=3,Int_t n=4);
  ~HMdcClosePairCutsPar();
  HMdcClosePairCutsParPhi& operator[](Int_t i) {
    return *static_cast<HMdcClosePairCutsParPhi*>((*array)[i]);
  }
  
  void setContainerComment(TString mycomment){comment=mycomment;}
  TString getContainerComment(){return comment;}
  void printContainerComment(){printf(comment.Data());}
  Int_t getSize();  
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void putAsciiHeader(TString&);
  Bool_t writeline(char *, Int_t, Int_t, Int_t);
  void readline(const char*, Int_t*);
  void clear();
  void printParam(void);
   
  ClassDef(HMdcClosePairCutsPar,1) // Container of ClosePairCutsPar   parameters
    };
    
    
#endif  /*!HMDCCLOSEPAIRCUTSPAR_H*/
    
  

