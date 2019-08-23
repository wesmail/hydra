//*-- Author : Jacek Otwinowski 28/02/05
//
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgStandCutsPar
//
// Parameters for hard-cuts lepton analysis used by HPidAlgStandCuts algorithm
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgstandcutspar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgStandCutsPar)

// -----------------------------------------------------------------------------

HPidAlgStandCutsPar::HPidAlgStandCutsPar(const char *name, const char *title,
                            const char *context)
                            : HPidHistogramsCont(name, title, context, 8)
{
// Containers for HPidAlgStandCuts class
// For the algorithm descrition look into HPidAlgStandCuts class
//
// Init HPidHistogramsCont with 7 histograms:
// 0 - param 0 (Rich ring parameters)
// 1 - param 1 (Pre-Shower F(p) parametrisation)
// 2 - param 2 (Pre-Shower Sum0(p) parametrisation)
// 3 - param 3 system 0 (Momentum vs. Beta - lower limits)
// 4 - param 4 system 0 (Momentum vs. Beta - upper limits)
// 5 - param 5 system 1 (Momentum vs. Beta - lower limits)
// 6 - param 6 system 1 (Momentum vs. Beta - upper limits)
// 7 - param 7 Pre-Shower Sum2+Sum1-Sum0 vs momentum parametrisation
}
