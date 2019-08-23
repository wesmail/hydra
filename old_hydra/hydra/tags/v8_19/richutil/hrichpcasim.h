// $Id: hrichpcasim.h,v 1.4 2008-09-18 13:05:36 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:13:31
//
#ifndef HRICHPCASIM_H
#define HRICHPCASIM_H

#include "hreconstructor.h"
#include "TFile.h"
#include "TPrincipal.h"
#include "TH3.h"
class HCategory;
class HIterator;
class HRichPCASim : public HReconstructor {
public:

    HRichPCASim();
    HRichPCASim(Text_t *name,Text_t *title);
    HRichPCASim(Text_t *name,Text_t *title, Char_t* filename);
    ~HRichPCASim();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getHitMatchCat(){return pHitMatchCat;};
    ClassDef(HRichPCASim,1) //ROOT extension

protected:
    Int_t nNbVariables; //! 
    HCategory* pHitMatchCat; //!
    HIterator* pIterMatchHit; //!
    TFile *pFile; //!
    TPrincipal *principal; //!
    TH3D *h3; //!
};

#endif // HRICHPCASIM_H
