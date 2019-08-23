#ifndef HHYPRECPAR_H
#define HHYPRECPAR_H

#include "hparcond.h"
#include "hypinfodef.h"
#include "TNamed.h"
#include "TArrayF.h"

class HHypRecPar:public HParCond
{
protected:
  
  TArrayF         hypCutValue;                    // any cut parameter
  TString         *hypCutID[MAX_PARAM_VALUES];    // some unique string
  
  Int_t           numofparams;                    // number of params stored
  
 public:
  
  HHypRecPar(const Char_t *name = "HypRecPar",
	     const Char_t *title = "Hyp parameters",
	     const Char_t *context = "HypRecProductionCuts");
  ~HHypRecPar(void);
  
  Bool_t          getCut(const Char_t * cutID, Float_t & cutValue);//getting cut parameter cutID
  void            setCut(const Char_t * cutID, Float_t cutValue);  //setting cut parameter
  void            registerCut(const Char_t * cutID);               //called from algs in the init function

  Bool_t          init(HParIo *, Int_t * set = 0);
  Int_t           write(HParIo *);
  void            clear(void);
  void            putParams(HParamList *);
  Bool_t          getParams(HParamList *);
  void            print(void);
  
  ClassDef(HHypRecPar, 2)       // parameter container for HHypReconstructor
    
};

#endif
