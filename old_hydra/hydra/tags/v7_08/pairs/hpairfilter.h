// $Id: hpairfilter.h,v 1.3 2005-04-27 17:55:37 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-04-22 19:38:11
//
// ---------------------------------------------------------------------------
#ifndef HPAIRFILTER_H
#define HPAIRFILTER_H
// ---------------------------------------------------------------------------
#include "hreconstructor.h"
// ---------------------------------------------------------------------------
class HCategory;
class HIterator;
class HPairCutPar;
// ---------------------------------------------------------------------------

class HPairFilter : public HReconstructor {
public:

	HPairFilter();
        HPairFilter(Text_t *name,Text_t *title);
	~HPairFilter();

        Bool_t init();
        Bool_t reinit();
        Bool_t finalize();
        Int_t execute();
        HCategory* getPairCat(){return pCatPair;};

        void setQAFileName(Char_t* c){cQAFileName=c;};
        TString& getQAFileName(void){return cQAFileName;};

        ClassDef(HPairFilter,0)

protected:
	HCategory* pCatPair;    //!
        HIterator* pIterPair;   //!
        HPairCutPar* pCuts;         //!
        Bool_t kIsReal;         //!
        TString cQAFileName; //!
        HPairQA* pQA;//!
};

#endif // HPAIRFILTER_H

