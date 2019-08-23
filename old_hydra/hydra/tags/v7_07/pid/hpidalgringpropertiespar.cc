// @(#)$Id: hpidalgringpropertiespar.cc,v 1.7 2005-02-22 15:57:43 christ Exp $
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
                            : HPidHistogramsCont(name, title, context, 7)
{

// Containers for HPidAlgRingProperties class
// For the algorithm description look into HPidAlgRingProperties class
//
// Init HPidHistogramsCont with 7 histograms per particle and sector. (6 per sector + 1 normalization histogram per sector)
// We have:
// - one TH2F for each theta bin (10-30,30-50,50-90 degrees) for leptons which holds the joint PDF of single 
//   and double rings, 
// - one for each theta bin for hadrons which holds the PDF of rings which were accidentially
//   recognized but not of leptonic origin and were correlated with a hadronic track
// - and the same set of histos for fakes
// The last histogram holds the normalization factors for the species as a function of momentum 




//Order of arrangement in the histo-container: (arrangement repeats itself for each particle species and sector)
//
// Offset:
//
//   0   :     pdf_with_centroid_10_to_90_degrees            pdf_with_centroid_10_to_30_degrees         
//
//   1   :     pdf_with_centroid_30_to_90_degrees            pdf_with_centroid_30_to_50_degrees         
//
//   2   :     pdf_with_centroid_50_to_90_degrees            pdf_with_centroid_50_to_90_degrees         
//
//   3   :     pdf_no_centroid_10_to_90_degrees              pdf_no_centroid_10_to_30_degrees         
//
//   4   :     pdf_no_centroid_30_to_90_degrees              pdf_no_centroid_30_to_50_degrees         
//
//   5   :     pdf_no_centroid_50_to_90_degrees              pdf_no_centroid_50_to_90_degrees         
//
//   6   :     normalization factors for this sector as a function of theta and momentum

  
}
