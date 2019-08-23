// @(#)$Id: hpidreconstructorpar.cc,v 1.3 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 10/10/2002

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//
// HPidReconstructorPar
//
// HPidReconstructor parameters with particles intensities
// There are two histograms per species and sector: The relints for TOF and
// the relints for SHOWER
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
