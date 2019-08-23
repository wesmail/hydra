// @(#)$Id:
//*-- Author : Tomasz  Wojcik 14/02/2005

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgShowerPar
//
// HPidAlgShowerPar parameters for PID
// Container stores PDFs for charge multiplication factors and
// PDFs for sum of charges from Pre-chamber (3x3 pads)
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgshowerpar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgShowerPar)

// -----------------------------------------------------------------------------

HPidAlgShowerPar::HPidAlgShowerPar(const Char_t *name, const Char_t *title,
                            const Char_t *context)
			: HPidHistogramsCont(name, title, context, PIDALGSHOWERPAR_HIST_NUM)
{
// Containers for HPidAlgShower class
// For the algorithm descrition look into HPidAlgShower class
//
}
