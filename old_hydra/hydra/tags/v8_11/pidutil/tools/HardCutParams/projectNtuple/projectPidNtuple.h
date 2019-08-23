#ifndef HPidHistograms_h
#define HPidHistograms_h

#include "hydraIncludes.h"
#include "hsudummyrec.h"
#include "TNtuple.h"

//---------------------------------------------------------------
class HPidHistograms: public HSUDummyRec 
{
public: 
	HPidHistograms(const char *pFileName);
	HPidHistograms() {};
    ~HPidHistograms(void){};

    virtual Bool_t init(void);
    virtual Int_t  execute(void);
    virtual Bool_t finalize(void);

private:
	// pointers to the input categories and their iterators  
	HCategory *pPidTrackCand;
	HIterator *pitPidTrackCand;

	TNtuple *pTrackCandidate; // lept track properties

};

#endif

