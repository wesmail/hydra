// $Id: hpaircutpar.h,v 1.9 2006-04-12 16:37:28 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-04-11 17:27:54
//
// ---------------------------------------------------------------------------
#ifndef HPAIRCUTPAR_H
#define HPAIRCUTPAR_H
// ---------------------------------------------------------------------------
#include <hparcond.h>
// ---------------------------------------------------------------------------
class HIterator;
class HPair;
class HParamList;
// ---------------------------------------------------------------------------
class HPairCutPar: public HParCond {

public:
    HPairCutPar(const char* name="PairCutParameters",
		const char* title="Pair Analysis Parameters",
		const char* context="PairDefault");
    virtual ~HPairCutPar();
    //  
    Bool_t  init(HParIo*, Int_t*);
    Int_t   write(HParIo*);
    void    putParams(HParamList*);
    Bool_t  getParams(HParamList*);
    void    clear();
    void    printParam(void);
    Bool_t  check(HPair*);
    Int_t   getBitFieldPassedFlags(){return nBitFieldPassedCuts;};
    Int_t   getBitFieldFlags(){return nBitFieldFlags;};
    Int_t   getBitFieldRecursiveFlags(){return nBitFieldRecursiveFlags;};
    TString* getActiveCutList(){return pActiveCutNames;};
    Int_t   getNbActiveCuts();
    Bool_t  checkRecursive(HIterator*);
    Bool_t  checkRecursive(TObjArray*);
    // pair property cuts
    Bool_t checkOpeningAngle(HPair*);
    // common detector hit
    Bool_t checkCommonDetectorHit(HPair*);

protected:

    Bool_t compareParticleIndicesWithList(HPair*, Int_t*,Int_t );

    // cut values
    Float_t fPairOpeningAngle;

 
    // flags to activate cuts
    Int_t nPairOpeningAngleFlag;
    Int_t nDoubleDetectorHitFlag;

    // flags for active recursive cuts
    Int_t nPairOpeningAngleRecFlag;
    Int_t nDoubleDetectorHitRecFlag;

    // bit fields
    Int_t nBitFieldFlags;
    Int_t nBitFieldPassedCuts;
    Int_t nBitFieldRecursiveFlags;
    
    // order of cuts for bit fields
    //    enum nCutNb {OPANG,NOSAMEMETA};//!

    static const UInt_t NOCUT ;//!
    static const UInt_t DIRECTCUT ;//!
    static const UInt_t OPANG;//!
    static const UInt_t DOUBLEHIT;//!
    static const UInt_t OPANGDOUBLEHIT ;//!

    TString* pActiveCutNames;//!

    TObjArray* pArrCutPairs;//!
    ClassDef(HPairCutPar,1)
};


#endif // HPAIRCUTPAR_H


