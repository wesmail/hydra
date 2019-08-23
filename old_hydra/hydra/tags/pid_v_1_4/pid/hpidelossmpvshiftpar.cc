//*-- Author : Dusan Zovinec 13/03/2003

////////////////////////////////////////////////////////////////////////////////
//
// HPidElossMPVShiftPar
//
// HPidElossMPVShiftPar parameters for energy loss probability density
// function calculation. It allowes to identify peak of the appropriate
// probability density spectrum with the MPV parameter of the PDF.
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidelossmpvshiftpar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidElossMPVShiftPar)

// -----------------------------------------------------------------------------

HPidElossMPVShiftPar::HPidElossMPVShiftPar(const char *name, const char *title,
                            const char *context)
                            : HPidHistogramsCont(name, title, context, 2)
{
// Containers for HPidAlgMomVsEloss class
// For the algorithm descrition look into HPidAlgMomVsEloss class
//
// Init HPidHistogramsCont with 2 histograms:
// 0 - system 0 param 0 (mpv shift for eloss PDF)
// 1 - system 1 param 0 (mpv shift for eloss PDF)
}
