// $Id: hpaircutpar.h,v 1.11 2008-05-09 16:14:32 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-24 11:07:22
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
    HPairCutPar(const Char_t* name="PairCutParameters",
		const Char_t* title="Pair Analysis Parameters",
		const Char_t* context="PairDefault");
    virtual ~HPairCutPar();
    //  
    Bool_t  init(HParIo*, Int_t*);
    void finalizeInit();
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
    // pair distance
    Bool_t checkPairDist(HPair*);
    Bool_t checkPairVertex(HPair*);
    // get init flag
    Bool_t getInitFlag(){return kIsInit;};
protected:

    Bool_t compareParticleIndicesWithList(HPair*, Int_t*,Int_t );

    // cut values
    Float_t fPairOpeningAngle;
    Float_t fPairDist;
    Float_t fZRichExit;
    Float_t fZRichBulb;
    Float_t fZTargetShift;
    Float_t fRFlanch;
    Float_t fXBeamCenter;
    Float_t fYBeamCenter;
    Float_t fBeamRadius;

    // flags to activate cuts
    Int_t nPairOpeningAngleFlag;
    Int_t nDoubleDetectorHitFlag;
    Int_t nPairDistFlag;
    Int_t nPairVertexFlag;

    // flags for active recursive cuts
    Int_t nPairOpeningAngleRecFlag;
    Int_t nDoubleDetectorHitRecFlag;

    // bit fields
    Int_t nBitFieldFlags;//!
    Int_t nBitFieldPassedCuts;//!
    Int_t nBitFieldRecursiveFlags;//!
    
    // order of cuts for bit fields
    static const UInt_t NOCUT ;//!
    static const UInt_t DIRECTCUT ;//!
    static const UInt_t OPANG;//!
    static const UInt_t DOUBLEHIT;//!
    static const UInt_t DIST;//!
    static const UInt_t VERTEX;//!
    static const UInt_t OPANGDOUBLEHIT ;//!

    // init flag
    Bool_t kIsInit;//!

    TString* pActiveCutNames;//!

    TObjArray* pArrCutPairs;//!

    ClassDef(HPairCutPar,3)
};


#endif // HPAIRCUTPAR_H


