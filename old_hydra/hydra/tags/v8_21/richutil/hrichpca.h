// $Id: hrichpca.h,v 1.5 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:13:17
//
#ifndef HRICHPCA_H
#define HRICHPCA_H

#include "hreconstructor.h"
#include "TFile.h"
#include "TPrincipal.h"
#include "TH3.h"
#include "TH2.h"
class HCategory;
class HIterator;
class HRichPCA : public HReconstructor {
public:

    HRichPCA();
    HRichPCA(const Text_t *name,const Text_t *title);
    HRichPCA(const Text_t *name,const Text_t *title,const Char_t* filename);
    ~HRichPCA();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getHitMatchCat(){return pHitMatchCat;};
    ClassDef(HRichPCA,1) //ROOT extension

protected:
    TH3D *h3;//!
    TH3D *h3t;//!
    TH2D *h2; //!
    Int_t nNbVariables; //! 
    HCategory* pHitMatchCat; //!
    HIterator* pIterMatchHit; //!
    TFile *pFile; //!
    TPrincipal *principal; //!
};

#endif // HRICHPCA_H

