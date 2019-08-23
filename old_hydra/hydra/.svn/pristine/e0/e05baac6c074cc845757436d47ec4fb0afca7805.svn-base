//*-- AUTHOR : I. Froehlich
//*-- Modified : 21/04/2005 by B. Spruck
//*-- Modified : 11/05/2005 by I. Froehlich

#ifndef HHYPCHANNEL_H
#define HHYPCHANNEL_H

#define HHYP_MAX_HYPCHANNELS 	50
#define HHYP_SUBCHANNELS	256

#include <TObject.h>
#include <TArrayI.h>

class             HCategory;
class             HIterator;
class             HSpectrometer;
class             HRuntimeDb;

class HHypChannel
{

private:
  Bool_t          Add(const char *, Int_t, Int_t);
  Int_t           dynamic_pointer;                              //! what is the next free channel number
  Int_t           add_pointer;                                  //! Pointer to the next element
  const char     *hypchannel_names[HHYP_MAX_HYPCHANNELS];       //! names 
  int             hypchannel_numbers[HHYP_MAX_HYPCHANNELS];     //! numbers

public:

  HHypChannel();
  virtual ~ HHypChannel();

  Int_t           Get(const char *);
  const char     *Get(Int_t);

  ClassDef(HHypChannel, 0)      // Elementary reaction naming scheme
};

//============================================================================

#endif
