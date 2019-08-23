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
