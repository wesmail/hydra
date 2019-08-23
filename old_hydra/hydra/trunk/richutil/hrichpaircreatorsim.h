// $Id: hrichpaircreatorsim.h,v 1.4 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:13:03
//
#ifndef HRICHPAIRCREATORSIM_H
#define HRICHPAIRCREATORSIM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hrichpaircreator.h"
class HCategory;
class HIterator;
class HDiHitMatchSim;
class HHitMatchSim;
class TObjArray;
class TFile;
class HTrackInfo;

class HRichPairCreatorSim : public HRichPairCreator {
public:

	HRichPairCreatorSim();
        HRichPairCreatorSim(const Text_t *name,const Text_t *title,const Char_t*);
	~HRichPairCreatorSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        Int_t fillHistosGEANTPairs(TObjArray*,const Char_t*,const Char_t*,const Char_t*);
        Int_t selectAndFillPairsSim(void);
    //Int_t fillOpangMDCSim(HDiHitMatch*,Int_t*,Int_t,Char_t*);
        void setOpangBasename(const Char_t* c){opangbasename=c;};
        const Char_t* getOpangBasename(void){return opangbasename;};
        HCategory* getHitMatchCat(){return pHitMatchCat;};
        HCategory* getHitDiMatchCat(){return pHitDiMatchCat;};
        ClassDef(HRichPairCreatorSim,1) //ROOT extension

protected:
	void initHistosSim();
        TObjArray* pHistArraySim;//!
        const Char_t* opangbasename;//!
};

#endif // HRICHPAIRCREATORSIM_H

