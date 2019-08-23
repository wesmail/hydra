// @(#)$Id:
//*-- Author : Marcin Jaskula 14/10/2002

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShowerPar
//
// HPidAlgShower parameters for distributions calculations
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgshowerpar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgShowerPar)

// -----------------------------------------------------------------------------

HPidAlgShowerPar::HPidAlgShowerPar(const char *name, const char *title,
                            const char *context)
			: HPidHistogramsCont(name, title, context, PIDALGSHOWERPAR_HIST_NUM)
{
// Containers for HPidAlgShower class
// For the algorithm descrition look into HPidAlgShower class
//
}
