//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by I. Froehlich

#ifndef HHYP_QA_H
#define HHYP_QA_H

#include <TObject.h>
#include <TArrayI.h>
#include <TNtuple.h>
#include <TFile.h>
#include "hhypbasealgorithm.h"
#include <hcategory.h>
#include <hpidparticlesim.h>
#include <hpidgeanttrackset.h>
#include <hgeantkine.h>
#include <piddef.h>

// ***************************************************************************

class HHypQA:public HHypBaseAlgorithm
{

  // ***************************************************************************
private:

  HCategory      *m_pContCatPart;   //!Pointer to the hit data category HPidPArticle
  HIterator      *m_pContItPart;    //!Pointer to the hit data category HPidPArticle
  TNtuple        *qa;
  TNtuple        *trqa;

  Int_t           simuflag;  // if simulation
  HCategory      *simCat;
  HCategory      *CatPartSim;

  Bool_t          use_trqa;

protected:

  UInt_t          seqnr, date, time;  //Some time info for QA

public:

  HHypQA(char *name_i = NULL, Option_t par[] = NULL);

  virtual ~ HHypQA(void);

  Bool_t          execute();
  Bool_t          init();
  Bool_t          reinit();
  Bool_t          finalize();

  void            Dump(void);
  
  ClassDef(HHypQA, 0) // Projecting QA info to TNtuple

};

//============================================================================

#endif
