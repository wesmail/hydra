//*-- AUTHOR : Tiago Perez
//*-- Modified : 20/04/2005 by I. Froehlich

#ifndef HHYPBASEALGORITHM_H
#define HHYPBASEALGORITHM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hhyplist.h"
#include "TArrayI.h"
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hypinfodef.h"
#include "hhyprecpar.h"
#include <hphysicsconstants.h>

#include "TObject.h"
#include "TArrayI.h"
#include "TString.h"

#include "hhypchannel.h"

class HHypBaseAlgorithm
{

protected:

  Int_t           sourceIdx;    // The index number of the source hyplist object 
  Int_t           exitIdx;      // The index number of the hyplist object created in base_execute
  Int_t           exitList;     // The physical exit list (see HHypChannel)
  Int_t           initList;     // My physical init list

  TString         algoName;     //! Name of the algorithm (for printout, debugging...)
  HHypList       *mylist;       // Pointer to the exitList (which THIS algorith may modify) 
  HHypRecPar     *pParams;      // pointer to container with params

  TLorentzVector *beam;         //! Pointer to the total CM energy for missing mass calculations
  Int_t beam_pid, target_pid;   //! Beam and Target PID (needed for Kine Refit)
  HHypChannel    *channel;      //! Pointer to the official channel list

  TString         options;            //! Option list
  TFile          *histofile;    //! Store optional information in this file

public:

  HHypBaseAlgorithm(const Char_t *name_i = NULL,const  Option_t par[] = NULL);
  
  virtual ~ HHypBaseAlgorithm(void);

  HHypList       *GetList(void);

  void            SetBeam(TLorentzVector * mybeam)       {beam = mybeam;};
  void            SetTargetPID(Int_t target){ target_pid=target;};
  void            SetBeamPID(Int_t projectile){ beam_pid=projectile;};
  void            SetHypChannel(HHypChannel * mychannel) {channel = mychannel;};
  void            SetParams(HHypRecPar * Params)         {pParams = Params;};

  Bool_t          SetSourceChannelAlgIdx(Int_t i_id);
  Bool_t          SetExitList(Int_t e_list);
  Bool_t          SetInitList(Int_t i_list);
  Bool_t          SetExitChannelAlgIdx(Int_t e_id);
  Int_t           GetExitChannelAlgIdx();
  const Char_t   *GetName();
  TString        *GetOpt(TString st);  //return the option for st 
  Int_t           GetSourceChannelAlgIdx();
  Bool_t          skip;

  void            SetHFile(TFile * file)                 {histofile = file;};
  TFile*          GetHFile()                             {return histofile;};

  virtual Bool_t  execute();
  virtual Bool_t  init();
  virtual Bool_t  reinit();
  virtual Bool_t  finalize();

  Bool_t          base_execute();
  Bool_t          base_init();
  Bool_t          base_reinit();
  Bool_t          base_finalize();

  ClassDef(HHypBaseAlgorithm, 0)        // Base class for all hyp algorithms
};

#endif // HHYPBASEALGORITHM_H
