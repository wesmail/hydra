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
  
  HPidAlgHardCutsPar(const Char_t *name = "PidAlgHardCutsPar",
	     const Char_t *title = "graphical cut parameters",
	     const Char_t *context = "PidAlgHardCutsProductionCuts");
  ~HPidAlgHardCutsPar(void);
  
  TCutG *         getCut(const Char_t * cutID);                    //getting cut parameter cutID
  void            setCut(const Char_t * cutID, TCutG * cutValue);  //setting cut parameter
  void            registerCut(const Char_t * cutID);               //called from algs in the init function

  Bool_t          init(HParIo *, Int_t * set = 0);
  Int_t           write(HParIo *);
  void            clear(void);
  void            putParams(HParamList *);
  Bool_t          getParams(HParamList *);
  void            print(void);
  
  ClassDef(HPidAlgHardCutsPar, 1)       // parameter container for HPidAlgHardCuts (and all TCutsG)
    
};

#endif
