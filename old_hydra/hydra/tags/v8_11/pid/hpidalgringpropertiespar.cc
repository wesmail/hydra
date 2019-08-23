// @(#)$Id: hpidalgringpropertiespar.cc,v 1.9 2006-08-16 16:13:34 halo Exp $
//*-- Author : Marcin Jaskula 14/10/2002

//_HADES_CLASS_DESCRIPTION
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
                            : HPidHistogramsCont(name, title, context, 5)
{

// Containers for HPidAlgRingProperties class
// For the algorithm description look into HPidAlgRingProperties class
//
// Init HPidHistogramsCont with 5 histograms per particle and sector. (4 per sector + 1 normalization histogram per sector)
// We have:
// - one TH2F for each theta bin (10-30,30-50,50-90 degrees) for leptons which holds the joint PDF of single 
//   and double rings, 
// - one for each theta bin for hadrons which holds the PDF of rings which were accidentially
//   recognized but were correlated with a hadronic track
// - and the same set of histos for fakes
// The last histogram holds the normalization factors for the species as a function of momentum 




//Order of arrangement in the histo-container: (arrangement repeats itself for each particle species and sector)
//
// Offset:
//
//   0   :     pdf_with_centroid_0_to_45_degrees         
//
//   1   :     pdf_with_centroid_45_to_90_degrees         
//
//   2   :     pdf_no_centroid_0_to_45_degrees         
//
//   3   :     pdf_no_centroid_45_to_90_degrees         
//
//   4   :     normalization factors for this sector as a function of theta and momentum

  
}
