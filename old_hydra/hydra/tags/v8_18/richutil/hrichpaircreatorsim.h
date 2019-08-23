// $Id: hrichpaircreatorsim.h,v 1.3 2008-05-09 16:16:33 halo Exp $
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
        HRichPairCreatorSim(Text_t *name,Text_t *title,Char_t*);
	~HRichPairCreatorSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        Int_t fillHistosGEANTPairs(TObjArray*,Char_t*,Char_t*,Char_t*);
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

