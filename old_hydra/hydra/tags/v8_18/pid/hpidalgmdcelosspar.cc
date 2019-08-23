
////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMdcElossPar
//
// HPidAlgMdcEloss parameters for PID
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgmdcelosspar.h"
#include "hparamlist.h"

// -----------------------------------------------------------------------------

ClassImp(HPidAlgMdcElossPar)

// -----------------------------------------------------------------------------

HPidAlgMdcElossPar::HPidAlgMdcElossPar(const Char_t *name, const Char_t *title,
                            const Char_t *context)
                            : HPidHistogramsCont(name, title, context, 1)
{

// Containers for HPidAlgMdceloss class
// For the algorithm description look into HPidAlgMdceloss class
//
// Init HPidHistogramsCont with 1 histograms per particle and sector. (The histogram is the same in each sector!)
// The histogram stores the measured energy loss in the drift chamber summed over inner and outer chambers

  
}
