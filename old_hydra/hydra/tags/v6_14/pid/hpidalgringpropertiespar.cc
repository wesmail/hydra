// @(#)$Id: hpidalgringpropertiespar.cc,v 1.3 2003-11-06 12:11:20 christ Exp $
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
                            : HPidHistogramsCont(name, title, context, 32)
{
// Containers for HPidAlgRingProperties class
// For the algorithm description look into HPidAlgRingProperties class
//
// Init HPidHistogramsCont with 32 histograms. 
// One for each theta bin (10 degree bins) for leptons which holds the joint PDF of single and double rings!
// and one for each theta bin (10 degree bins) for hadrons which holds the PDF of rings which were accidentially
// recognized but not of leptonic origin and were correlated with a hadronic 
// KT 

// One histogram for the confidence levels in the eigencoordinates for both
// types of rings in each theta bin

//Order of arrangement in the histo-container:

//         PID:             leptonic                              hadronic
// Offset:
//
//   0               pdf_centroid_10_to20_degrees            pdf_centroid_10_to20_degrees         
//
//------------>      all other angles
//
//   8               pdf_nocentroid_10_to20_degrees   	     pdf_nocentroid_10_to20_degrees  
//
//------------>      all other angles
//
//   16              cl_centroid_10_to20_degrees     	     cl_centroid_10_to20_degrees     
//
//------------>      all other angles
//
//   24              cl_nocentroid_10_to20_degrees   	     cl_nocentroid_10_to20_degrees   
//
//------------>      all other angles
//
}
