//*-- Author : Dusan Zovinec 13/03/2003

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMomVsElossPar
//
// HPidAlgMomVsEloss parameters for the energy loss probability density
// calculation.
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgmomvselosspar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgMomVsElossPar)

// -----------------------------------------------------------------------------

HPidAlgMomVsElossPar::HPidAlgMomVsElossPar(const char *name, const char *title,
                            const char *context)
                            : HPidHistogramsCont(name, title, context, 8)
{
// Containers for HPidAlgMomVsEloss class
// For the algorithm descrption look into HPidAlgMomVsEloss class
//
// Init HPidHistogramsCont with 8 histograms:
// 0 - system 0 param 0 (mpv used in eloss or pol0 param)
// 1 - system 0 param 1 (kappa used in eloss or pol1 param)
// 2 - system 0 param 2 (beta used in eloss or pol2 param)
// 3 - system 0 param 3 (xi used in eloss)
// 4 - system 1 param 0 (mpv used in eloss or pol0 param)
// 5 - system 1 param 1 (kappa used in eloss or pol1 param)
// 6 - system 1 param 2 (beta used in eloss or pol2 param)
// 7 - system 1 param 3 (xi used in eloss)
}
