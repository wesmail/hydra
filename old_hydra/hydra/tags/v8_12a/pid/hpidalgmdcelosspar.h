// @(#)$Id: hpidalgmdcelosspar.h,v 1.1 2006-08-16 16:12:53 halo Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGMDCELOSSPAR_H
#define HPIDALGMDCELOSSPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMdcElossPar
// This container holds the PDF values required to compute the probability that
// a track is a lepton/hadron/fake based upon energy loss information in MDC
//
////////////////////////////////////////////////////////////////////////////////
using namespace std;

#include "hpidhistogramscont.h"
#include "hpidtrackcand.h"
#include "piddef.h"
#include <iostream>


#define PIDALGMDCELOSSPAR_NAME    "PidAlgMdcElossPar"
#define PIDALGMDCELOSSPAR_TITLE   "Parameters for HPidAlgMdcEloss"
#define PIDALGMDCELOSSPAR_CONTEXT "HPidAlgMdcElossContext"

// -----------------------------------------------------------------------------
#include <TH1.h>

class HPidAlgMdcElossPar : public HPidHistogramsCont
{

 public:
  
  HPidAlgMdcElossPar(
			   const char *name    = PIDALGMDCELOSSPAR_NAME,
			   const char *title   = PIDALGMDCELOSSPAR_TITLE,
			   const char *context = PIDALGMDCELOSSPAR_CONTEXT);
  virtual    ~HPidAlgMdcElossPar(void) {}
  
  // -------------------------------------------------------------------------
 public:
  // -------------------------------------------------------------------------
  
  ClassDef(HPidAlgMdcElossPar, 1) // Params for HPidReconstructor
    };
    
    // -----------------------------------------------------------------------------
    
#endif  /* !HPIDALGMDCELOSSPAR_H */
    
