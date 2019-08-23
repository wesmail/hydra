// $Id: hpairfilter.h,v 1.7 2009-07-15 11:41:59 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-10 15:32:34
//
// ---------------------------------------------------------------------------
#ifndef HPAIRFILTER_H
#define HPAIRFILTER_H
// ---------------------------------------------------------------------------
#include <hreconstructor.h>
// ---------------------------------------------------------------------------
class HCategory;
class HIterator;
class HPairCutPar;
class HPairQA;
// ---------------------------------------------------------------------------

class HPairFilter : public HReconstructor {
public:

	HPairFilter();
        HPairFilter(const Text_t *name,const Text_t *title);
	~HPairFilter();

        Bool_t init();
        Bool_t reinit();
        Bool_t finalize();
        Int_t execute();
        HCategory* getPairCat(){return pCatPair;};

        void setQAFileName(const Char_t* c){cQAFileName=c;};
        TString& getQAFileName(void){return cQAFileName;};

        ClassDef(HPairFilter,1)

protected:
	HCategory* pCatPair;    //!
        HIterator* pIterPair;   //!
        HPairCutPar* pCuts;         //!
        Bool_t kIsReal;         //!
        TString cQAFileName; //!
        HPairQA* pQA;//!
};

#endif // HPAIRFILTER_H

