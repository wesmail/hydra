// @(#)$Id: hpidalgringpropertiespar.cc,v 1.1 2003-06-22 14:39:16 christ Exp $
//*-- Author : Marcin Jaskula 14/10/2002

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgRingPropertiesPar
//
// HPidAlgRingProperties parameters for distributions calculations
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgringpropertiespar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgRingPropertiesPar)

// -----------------------------------------------------------------------------

HPidAlgRingPropertiesPar::HPidAlgRingPropertiesPar(const char *name, const char *title,
                            const char *context)
                            : HPidHistogramsCont(name, title, context, 4)
{
// Containers for HPidAlgRingProperties class
// For the algorithm description look into HPidAlgRingProperties class
//
// Init HPidHistogramsCont with 5 histograms. 
// One for leptons which holds the joint PDF of single and double rings!
// and one for hadrons which holds the PDF of rings which were accidentially
// recognized but not of leptonic origin and were correlated with a hadronic 
// KT 

// One histogram for the confidence levels in the eigencoordinates for both
// types of rings

//Order of arrangement in the histo-container:

//         PID:        leptonic                  hadronic
// Offset:
//   0               pdf_centroid            pdf_centroid         
//   1               pdf_nocentroid   	     pdf_nocentroid  
//   2               cl_centroid     	     cl_centroid     
//   3               cl_nocentroid   	     cl_nocentroid   
}
