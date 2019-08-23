// $Id: hrichdumphitsim.h,v 1.2 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 18:09:49
//
#ifndef HRICHDUMPHITSIM_H
#define HRICHDUMPHITSIM_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hrichdumphit.h"
class HCategory;
class HIterator;

class HRichDumpHitSim : public HRichDumpHit {
public:

	HRichDumpHitSim();
        HRichDumpHitSim(Text_t *name,Text_t *title);
	~HRichDumpHitSim();

        Bool_t init();
        Bool_t finalize();
        Int_t execute();
        
    
        ClassDef(HRichDumpHitSim,1) //ROOT extension

private:

};

#endif // HRICHDUMPHITSIM_H
