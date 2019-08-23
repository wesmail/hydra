//*-- Authors : Marcin Jaskula, Jerzy Pietraszko 11/10/2002
//            : code taken from HShowerHitFilnder and some old implemetation
//              of this class

#ifndef HShowerHitFinderIPU_H
#define HShowerHitFinderIPU_H

// -----------------------------------------------------------------------------

#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include "hratree.h"

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;
class HShowerCalIPU;
class HShowerHitIPU;

// -----------------------------------------------------------------------------

class HShowerHitFinderIPU : public HReconstructor
{
public:
            HShowerHitFinderIPU(void);
            HShowerHitFinderIPU(Text_t *name,Text_t *title,
                        Int_t iAlgFlag = 0, Int_t iThr1 = 0, Int_t iThr2 = 0);
            HShowerHitFinderIPU(Int_t iAlgFlag,
                        Int_t iThr1 = 0, Int_t iThr2 = 0);

           ~HShowerHitFinderIPU(void);

    // -------------------------------------------------------------------------

    Int_t   execute(void);
    Bool_t  init(void);
    Bool_t  finalize(void);
    Bool_t  initParameters(void);

    // -------------------------------------------------------------------------

    void    setAlgorithm(Int_t a)       { m_iAlgFlag = a;       }
    Int_t   getAlgorithm(void) const    { return m_iAlgFlag;    }
    void    setThreshold1(Int_t a)      { m_iThr1 = a;          }
    Int_t   getThreshold1(void) const   { return m_iThr1;       }
    void    setThreshold2(Int_t a)      { m_iThr2 = a;          }
    Int_t   getThreshold2(void) const   { return m_iThr2;       }
/*
Float_t calculateVar(HLocation &fLoc, int nRange, float avg);
HShowerHitFinderIPU &operator=(HShowerHitFinderIPU &c);

HCategory* getCalCat(){return m_pCalCat;}
HCategory* getHitCat(){return m_pHitCat;}
HCategory* getPIDCat(){return m_pPIDCat;}
HParSet* getHitFPar(){return m_pHitFPar;}
HParSet* getGeometry(){return m_pGeometry;}

void setCalCat(HCategory* pCalCat){m_pCalCat = pCalCat;}
void setHitCat(HCategory* pHitCat){m_pHitCat = pHitCat;}
void setPIDCat(HCategory* pPIDCat){m_pPIDCat = pPIDCat;}
void setHitFPar(HParSet* pPar){m_pHitFPar= pPar;}
void setGeometry(HParSet* pGeometry){m_pGeometry= pGeometry;}
void setCriterium(HShowerCriterium* pCrit);

Bool_t IsSortFlagSet(){return m_bIsSort;}
Bool_t IsFillPID(){return m_bIsFillPID;}
void setSortFlag(Bool_t bSort = kTRUE){m_bIsSort = bSort;}
void setFillPID(Bool_t bIsFillPID = kTRUE){m_bIsFillPID = bIsFillPID;}
*/

private:

    Int_t      m_iAlgFlag;  // flag for the algorithm
    Int_t      m_iThr1;     // thresholds
    Int_t      m_iThr2;

    HLocation  m_Loc;
    HRaTree   *m_pCellArr;  //random access table for searching local maxima

    HCategory *m_pCalCat;   //!Pointer to the cal data category
    HCategory *m_pHitCat;   //!Pointer to the hit data category

    HIterator *fIter;       //!Iterator for calibrated fired pads

    // -------------------------------------------------------------------------

    void    setDefault(void);
    Bool_t  lookForHit(HShowerCalIPU *pCal);
    void    fillSums(HShowerCalIPU *pCal, UShort_t nSum[3]);
    Bool_t  isLocalMax(HShowerCalIPU *pRef);

/*
void fillSums(HShowerHitIPU* hit, HLocation &fLoc);
virtual void calcCoord(HShowerHitIPU* hit, HLocation &fLoc);


void fillPID(HShowerHitIPU* hit, HShowerPID* pid);

HShowerHitHeader* getHitHeader(HLocation &fLoc);
void updateClusters(HLocation &fLoc);
void updateLocalMax(HLocation &fLoc);
void updateFiredCells(HLocation &fLoc);
*/

    ClassDef(HShowerHitFinderIPU, 0) //Searching Hits as PreSHOWER IPU
};

#endif
