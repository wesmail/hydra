// $Id: hpairfilter.h,v 1.6 2006-11-24 16:14:03 eberl Exp $
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
        HPairFilter(Text_t *name,Text_t *title);
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

