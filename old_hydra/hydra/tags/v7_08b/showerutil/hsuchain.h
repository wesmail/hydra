// @(#)$Id: hsuchain.h,v 1.1 2003-07-11 10:41:41 jaskula Exp $
//*-- Author : Marcin Jaskula 03/06/2003

#ifndef HSUChain_H
#define HSUChain_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HSUChain                                                                   //
//                                                                            //
// Chain with access to the HCategory objects                                 //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include <TChain.h>
#include <TObjArray.h>

// -----------------------------------------------------------------------------

class HCategory;

// -----------------------------------------------------------------------------

class HSUChain : public TChain
{
public:
                        HSUChain(const char *pName = "T");
                       ~HSUChain(void);


    virtual Int_t   Add(const char *name, Int_t nentries = 0)
                                    { return TChain::Add(name, nentries); }

    virtual Int_t   GetEntry(Int_t entry = 0, Int_t getall = 0);

    virtual Bool_t  Notify(void);

    HCategory*      getCategory(const char *pName);

    // -------------------------------------------------------------------------

protected:


    TObjArray       arr;

    // -------------------------------------------------------------------------

    ClassDef(HSUChain, 0)         // Chain with access to the HCategory objects
};

// -----------------------------------------------------------------------------

#endif //HSUChain_H

