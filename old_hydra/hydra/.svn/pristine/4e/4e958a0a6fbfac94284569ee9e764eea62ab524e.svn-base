//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by B. Spruck
//*-- Modified : 11/05/2005 by I. Froehlich

#ifndef HHYPCHANNEL_H
#define HHYPCHANNEL_H

#define HHYP_MAX_HYPCHANNELS 	75
#define HHYP_SUBCHANNELS	256

#include <TObject.h>
#include <TArrayI.h>

class HHypChannel
{

private:
  Bool_t          Add(const Char_t *, Int_t, Int_t);
  Int_t           dynamic_pointer;                              //! what is the next free channel number
  Int_t           add_pointer;                                  //! Pointer to the next element
  const Char_t     *hypchannel_names[HHYP_MAX_HYPCHANNELS];       //! names 
  Int_t             hypchannel_numbers[HHYP_MAX_HYPCHANNELS];     //! numbers

public:

  HHypChannel();
  virtual ~ HHypChannel();

  Int_t           Get(const Char_t *);
  const Char_t     *Get(Int_t);

  ClassDef(HHypChannel, 0)      // Elementary reaction naming scheme
};

//============================================================================

#endif
