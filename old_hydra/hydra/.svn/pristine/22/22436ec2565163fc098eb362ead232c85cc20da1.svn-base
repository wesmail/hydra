// $Id: hpairfilter.h,v 1.5 2006-08-16 15:42:18 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-06-21 14:31:13
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

