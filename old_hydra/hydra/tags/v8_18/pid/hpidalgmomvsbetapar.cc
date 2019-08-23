// @(#)$Id: hpidalgmomvsbetapar.cc,v 1.3 2008-05-09 16:15:05 halo Exp $
//*-- Author : Marcin Jaskula 14/10/2002

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMomVsBetaPar
//
// HPidAlgMomVsBeta parameters for distributions calculations
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgmomvsbetapar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgMomVsBetaPar)

// -----------------------------------------------------------------------------

HPidAlgMomVsBetaPar::HPidAlgMomVsBetaPar(const Char_t *name, const Char_t *title,
                            const Char_t *context)
                            : HPidHistogramsCont(name, title, context, 6)
{
// Containers for HPidAlgMomVsBeta class
// For the algorithm descrition look into HPidAlgMomVsBeta class
//
// Init HPidHistogramsCont with 6 histograms:
// 0 - system 0 param 0 (mean for gauss or pol0 param)
// 1 - system 0 param 1 (sigma for gauss or pol1 param)
// 2 - system 0 param 2 (not used for gauss or pol2 param)
// 3 - system 1 param 0 (mean for gauss or pol0 param)
// 4 - system 1 param 1 (sigma for gauss or pol1 param)
// 5 - system 1 param 2 (not used for gauss or pol2 param)
}
