// File: hrichparticleidentifier.h
// ****************************************************************************
//
// HRichParticleIdentifier
//
// Author: Leszek Kidon (ufkidon@psja1.if.uj.edu.pl)
// Modified: 2000/05/28 by Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
// ----------------------------------------------------------------------------
//
// ****************************************************************************
         


#ifndef HRICHPARTICLEIDENTIFIER_H 
#define HRICHPARTICLEIDENTIFIER_H 

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"

class HCategory;
class HIterator;
class HRichHit;
class HRichPID;
class HRichHitSelection;

class HRichParticleIdentifier: public HReconstructor {

public:
	HRichParticleIdentifier ();
        HRichParticleIdentifier(Text_t *name, Text_t *title);
	virtual ~HRichParticleIdentifier();

        Bool_t init(void);
        Bool_t finalize(void);
        Int_t execute(void);
        Bool_t initParameters(void);

        HCategory* getHitCat(){return m_pHitCat;}
        HCategory* getPIDCat(){return m_pPIDCat;}
        HParSet* getAnalysisPar(){return m_pAnalysisPar;}

        void setHitCat(HCategory* pHitCat){m_pHitCat = pHitCat;}
        void setPIDCat(HCategory* pPIDCat){m_pPIDCat = pPIDCat;}
        void setAnalysisPar(HParSet* pPar){m_pAnalysisPar= pPar;}
        void setHitSelection(HRichHitSelection* pCrit);

        ClassDef(HRichParticleIdentifier,1) //ROOT extension

private:
        void fillPID(HRichHit* hit, HRichPID* pid);

        HLocation m_zeroLoc; 

        HCategory *m_pHitCat; //!Pointer to the raw data category
        HCategory *m_pPIDCat; //!Pointer to the cal data category
        HParSet *m_pAnalysisPar; //Pointer to the analysis parameters container
 
        HRichHitSelection* m_pHitSelection;  //!definition of shower criterium

        HIterator *fIter;  //!Iterator for hits
};

#endif // HRICHPARTICLEIDENTIFIER_H  
