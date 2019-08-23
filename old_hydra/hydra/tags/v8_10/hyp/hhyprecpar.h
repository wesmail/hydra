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
  
  HHypRecPar(const char *name = "HypRecPar",
	     const char *title = "Hyp parameters",
	     const char *context = "HypRecProductionCuts");
  ~HHypRecPar(void);
  
  Bool_t          getCut(char * cutID, Float_t & cutValue);//getting cut parameter cutID
  void            setCut(char * cutID, Float_t cutValue);  //setting cut parameter
  void            registerCut(char * cutID);               //called from algs in the init function

  Bool_t          init(HParIo *, Int_t * set = 0);
  Int_t           write(HParIo *);
  void            clear(void);
  void            putParams(HParamList *);
  Bool_t          getParams(HParamList *);
  void            print(void);
  
  ClassDef(HHypRecPar, 2)       // parameter container for HHypReconstructor
    
};

#endif
