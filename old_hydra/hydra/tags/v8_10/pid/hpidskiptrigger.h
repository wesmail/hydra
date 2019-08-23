//*-- Author : Jacek Otwinowski 11/08/03

#ifndef HPIDSKIPTRIGGER_H
#define HPIDSKIPTRIGGER_H

////////////////////////////////////////////////////////////////////////////////
// Author: Jacek Otwinowski                                                   //
// HPidSkipTrigger                                               	      //
//                                                                            //
// Filter the  Events according to user defined condition                     //
// This can be first- or second-level events or a condition on multiplicity
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"
#include "hcategory.h"
#include "hiterator.h"
// -----------------------------------------------------------------------------

class HPidSkipTrigger : public HReconstructor
{
public:
    HPidSkipTrigger(Text_t name[], Text_t title[], Option_t option[]="");
    HPidSkipTrigger(Text_t option[]="");
    ~HPidSkipTrigger(void) {;};

    // -------------------------------------------------------------------------

    //all tasks must have this - documentation on cc-file
    virtual Int_t       execute(void);
    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    // -------------------------------------------------------------------------

protected:

    // -------------------------------------------------------------------------

    Int_t nTot; // event counters
    Int_t nSkip;
    Int_t nAccept;

    // -------------------------------------------------------------------------
	
    void  setDefault(void);
    void  setOptions(Option_t* );

    enum skiptrigger {none,lvl1,lvl2};

    //flags confguring which event type to skip
    Bool_t bSkip2ndLvlEvents;
    Bool_t bSkip1stLvlEvents;
    Bool_t bSkipLowMult;
    Bool_t bSkipTrigM4;
    // -------------------------------------------------------------------------

    ClassDef(HPidSkipTrigger, 0)     // Filler of HPidDilepton
};

// -----------------------------------------------------------------------------

#endif //HPidSkipTrigger
