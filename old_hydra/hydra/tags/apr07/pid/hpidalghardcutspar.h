#ifndef HPIDALGHARDCUTSPAR_H
#define HPIDALGHARDCUTSPAR_H

#include "hparcond.h"
#include "TNamed.h"
#include "TArrayF.h"
#include "TCutG.h"

#define HARDCUTSPAR_MAX 100

class HPidAlgHardCutsPar:public HParCond
{
protected:
  
  TCutG           *HardCutValue[HARDCUTSPAR_MAX]; // any 2dim graphical cut
  TString         *HardCutID[HARDCUTSPAR_MAX];    // some unique string
  TArrayF         *x[HARDCUTSPAR_MAX];            // dummy for IO
  TArrayF         *y[HARDCUTSPAR_MAX];            // dummy for IO
  
  Int_t           numofparams;                    // number of cuts stored
  
 public:
  
  HPidAlgHardCutsPar(const char *name = "PidAlgHardCutsPar",
	     const char *title = "graphical cut parameters",
	     const char *context = "PidAlgHardCutsProductionCuts");
  ~HPidAlgHardCutsPar(void);
  
  TCutG *         getCut(char * cutID);                    //getting cut parameter cutID
  void            setCut(char * cutID, TCutG * cutValue);  //setting cut parameter
  void            registerCut(char * cutID);               //called from algs in the init function

  Bool_t          init(HParIo *, Int_t * set = 0);
  Int_t           write(HParIo *);
  void            clear(void);
  void            putParams(HParamList *);
  Bool_t          getParams(HParamList *);
  void            print(void);
  
  ClassDef(HPidAlgHardCutsPar, 1)       // parameter container for HPidAlgHardCuts (and all TCutsG)
    
};

#endif
