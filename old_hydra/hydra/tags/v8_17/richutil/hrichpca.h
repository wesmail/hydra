// $Id: hrichpca.h,v 1.3 2008-05-09 16:16:33 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:13:17
//
#ifndef HRICHPCA_H
#define HRICHPCA_H

#include "hreconstructor.h"
#include <TFile.h>
#include <TPrincipal.h>
#include <TH3.h>
#include <TH2.h>
class HCategory;
class HIterator;
class HRichPCA : public HReconstructor {
public:

    HRichPCA();
    HRichPCA(Text_t *name,Text_t *title);
    HRichPCA(Text_t *name,Text_t *title, Char_t* filename);
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

