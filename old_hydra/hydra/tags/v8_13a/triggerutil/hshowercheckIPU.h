// @(#)$Id: hshowercheckIPU.h,v 1.2 2003-11-21 15:13:19 halo Exp $
//*-- Author : Jerzy Pietraszko & Marcin Jaskula 02/11/2002

#ifndef HSHOWERCHECKIPU_H
#define HSHOWERCHECKIPU_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HShowerCheckIPU                                                            //
//                                                                            //
// Compare the results of the software IPU emulation with the data from MU    //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"
#include <TString.h>

// -----------------------------------------------------------------------------

class HIterator;
class TH1;
class TFile;
class HQAMaker;

// -----------------------------------------------------------------------------

// structure for storing positions of the hits
class HShowerHitIPUAddres
{
public:
                 HShowerHitIPUAddres(Char_t s = 0, Char_t x = 0, Char_t y = 0)
                                    : cS(s), cX(x), cY(y), bMask(kFALSE) {}

    // -------------------------------------------------------------------------

    void         set(Char_t s = 0, Char_t x = 0, Char_t y = 0)
                                    { cS = s; cX = x; cY = y; bMask = kFALSE;  }

    // -------------------------------------------------------------------------

    void         print(void) const          { printf("%s\n", toString());      }
    inline const char*  toString(void) const;

    void         mask(void)                 { bMask = kTRUE;                   }
    Bool_t       isMasked(void)             { return bMask;                    }
    void         clearMask(void)            { bMask = kFALSE;                  }

    // -------------------------------------------------------------------------

    Char_t  cS;     // sector
    Char_t  cX;     // column
    Char_t  cY;     // row
    Bool_t  bMask;  // mask
};

// -----------------------------------------------------------------------------

inline const char*  HShowerHitIPUAddres::toString(void) const
{
static char s[100];

    sprintf(s, "%s S: %d  X: %2d  Y: %2d", (bMask) ? "M" : " ", cS, cX, cY);

    return s;
}

// -----------------------------------------------------------------------------

inline Bool_t operator==(const HShowerHitIPUAddres &l,
                    const HShowerHitIPUAddres &r)
{
    return ((l.cS == r.cS) && (l.cX == r.cX) && (l.cY == r.cY)
                && (l.bMask == r.bMask));
}

// -----------------------------------------------------------------------------

class HShowerCheckIPU : public HReconstructor
{
public:
                        HShowerCheckIPU(Int_t iRatio = 100,
                                    const char *pOut = "");
                        HShowerCheckIPU(Text_t name[], Text_t title[],
                                    Int_t iRatio = 100, const char *pOut = "");

                       ~HShowerCheckIPU(void);

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Int_t       execute(void);
    virtual Bool_t      finalize(void);

    // -------------------------------------------------------------------------

    void                setInfoRate(Int_t iRate)        { iInfoRatio = iRate;  }
    Int_t               getInfoRate(void) const         { return iInfoRatio;   }

    void                setVerbose(Int_t i)             { iVerbose = i;        }
    Int_t               getVerbose(void) const          { return iVerbose;     }

    void                setOutputFile(const Char_t *pName)
                                                  { sOutputFile = pName;       }
    const char*         getOutputFile(void) const
                                                  { return sOutputFile.Data(); }

    void                setHistEvents(Int_t i)          { iHistEvents = i;     }
    Int_t               getHistEvents(void) const       { return iHistEvents;  }

    // -------------------------------------------------------------------------

    Int_t               getEvents(void) const           { return iEvents;      }
    Int_t               getErrors(void) const           { return iErrors;      }

    // -------------------------------------------------------------------------

    inline const TH1*   getHist(UInt_t i) const;
    const TH1*          getHistCorrX(void) const
                                            { return getHist(kHistCorrX);      }
    const TH1*          getHistCorrY(void) const
                                            { return getHist(kHistCorrY);      }
    const TH1*          getHistNoCorrAna(void) const
                                            { return getHist(kHistNoCorrAna);  }
    const TH1*          getHistNoCorrIPU(void) const
                                            { return getHist(kHistNoCorrIPU);  }
    const TH1*          getHistDoubleAna(void) const
                                            { return getHist(kHistDoubleAna);  }
    const TH1*          getHistDoubleIPU(void) const
                                            { return getHist(kHistDoubleIPU);  }

    // -------------------------------------------------------------------------

    Bool_t              writeHistograms(void);

    void                dumpEvent(void) const;
    void                writeInfo(void) const;

    // -------------------------------------------------------------------------

    void                setQA(HQAMaker *p)     { pQA = p;                 }
    HQAMaker*           getQA(void) const      { return pQA;              }

    Bool_t              putHistograms(TList *pList);

protected:

    enum {
        kHistCorrX = 0,
        kHistCorrY,
        kHistCorr,
        kHistNoCorrAna,
        kHistNoCorrIPU,
        kHistNoCorrXY,
        kHistDoubleAna,
        kHistDoubleIPU,
        kHistIPUInEvent,
        kHistAnaInEvent,
        kHistErrorInEvent,
        kHistMax,

        kMaxAna = 6000,
        kMaxIPU = 6000
    };

    // -------------------------------------------------------------------------

    HIterator          *pitAna;
    HIterator          *pitIPU;

    // -------------------------------------------------------------------------

    HShowerHitIPUAddres asAna[kMaxAna];
    HShowerHitIPUAddres asIPU[kMaxIPU];

    Int_t               iAnaNum;
    Int_t               iIPUNum;

    // -------------------------------------------------------------------------

    TH1                *ahOut[kHistMax];
    TString             sOutputFile;

    // -------------------------------------------------------------------------

    Int_t               iInfoRatio;
    Int_t               iVerbose;

    Int_t               iEvents;
    Int_t               iErrors;

    Bool_t              bInitOk;

    Int_t               iHistEvents;

    // -------------------------------------------------------------------------

    HQAMaker            *pQA;

    // -------------------------------------------------------------------------

    void                setDefault(Int_t iRatio, const char *pOut);
    void                checkDoubles(void);
    Int_t               checkDoubles(HShowerHitIPUAddres *pArr,
                                        Int_t iMax, Int_t iType);
    void                matchHits(void);

    // -------------------------------------------------------------------------

    ClassDef(HShowerCheckIPU, 0)  // Check shower IPU with the software data
};

// -----------------------------------------------------------------------------

inline const TH1* HShowerCheckIPU::getHist(UInt_t i) const
{
// Return histogram by its name

    if(i >= kHistMax)
    {
        Error("getHist", "Out of bounds: %d / %d\n", i, kHistMax);
        return NULL;
    }

    return ahOut[i];
}

// -----------------------------------------------------------------------------

#undef DEFAULT_OPTIONS

// -----------------------------------------------------------------------------

#endif //HSHOWERCHECKIPU_H
