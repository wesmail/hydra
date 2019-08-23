// @(#)$Id: hpidreconstructorpar.cc,v 1.1 2002-10-30 10:55:33 halo Exp $
//*-- Author : Marcin Jaskula 10/10/2002

////////////////////////////////////////////////////////////////////////////////
//
// HPidReconstructorPar
//
// HPidReconstructor parameters with particles intensities
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidreconstructorpar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidReconstructorPar)

// -----------------------------------------------------------------------------

HPidReconstructorPar::HPidReconstructorPar(const char *name, const char *title,
                            const char *context)
                            : HPidHistogramsCont(name, title, context, 2)
{
// Init HPidHistogramsCont with 2 histograms (systems) per particle
}
