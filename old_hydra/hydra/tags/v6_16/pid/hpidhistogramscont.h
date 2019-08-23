// @(#)$Id: hpidhistogramscont.h,v 1.4 2003-06-05 14:41:01 jaskula Exp $
//*-- Author : Marcin Jaskula 10/10/2002
// Modified  : Marcin Jaskula 02/06/2003
//           - Draw() method added

#ifndef HPIDHISTOGRAMSCONT_H
#define HPIDHISTOGRAMSCONT_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidHistogramsCont
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidparcont.h"

#include <TArrayI.h>
#include <TObjArray.h>

// -----------------------------------------------------------------------------

class HParamList;
class TH1;

// -----------------------------------------------------------------------------

class HPidHistogramsCont : public HPidParCont
{
private:

    Int_t           iNParticles;
    Int_t           iHistsPerPart;

    TArrayI         aParticles;
    TObjArray       aHistograms;

    Bool_t          bWarnings;  //!

    Bool_t          bOk;        //!

    // -------------------------------------------------------------------------

    void            setDefault(void);

    Int_t           getIndex(Int_t iPart, Int_t iOffset) const
                                { return iPart * iHistsPerPart + iOffset; }

    Bool_t          resize(Int_t iNPSet);

public:
                    HPidHistogramsCont(void);
                    HPidHistogramsCont(const char *name, const char *title,
                                    const char *context,
                                    Int_t iHistsPerPart = 1);
    virtual        ~HPidHistogramsCont(void);

    virtual void    clear(void);
    virtual void    detachHistograms(void);

    virtual void    putParams(HParamList *pl);
    virtual Bool_t  getParams(HParamList *pl);

    // -------------------------------------------------------------------------

    Int_t           getNParticles(void) const   { return iNParticles;   }
    Int_t           getHistsPerPart(void) const { return iHistsPerPart; }
    Int_t           getNHistograms(void) const
                                { return iNParticles * iHistsPerPart;   }

    // -------------------------------------------------------------------------

    Int_t           findParticleIndex(Int_t iPartId) const;

    Bool_t          addHistogram(Int_t iPartId, Int_t iOffset,
                                const TH1 *pHist);
    Bool_t          addHistogram(Int_t iPartId, const TH1 *pHist)
                                { return addHistogram(iPartId, 0, pHist); }

    void            removeHistogram(Int_t iPartId, Int_t iOffset = 0);
    const TH1*      getHistogram(Int_t iPartId, Int_t iOffset = 0) const;

    Stat_t          getValue(Int_t iPartId, Int_t iOffset,
                                Float_t fX, Float_t fY = 0.0f,
                                Float_t fZ = 0.0f) const;

    Stat_t          getValue(Int_t iPartId, Float_t fX,
                                Float_t fY = 0.0f, Float_t fZ = 0.0f) const
                                { return getValue(iPartId, 0, fX, fY, fZ); }

    // -------------------------------------------------------------------------

    void            setWarnings(Bool_t b = kTRUE) { bWarnings = b;      }
    Bool_t          getWarnings(void) const       { return bWarnings;   }

    // -------------------------------------------------------------------------

    virtual void    Print(Option_t *p = "")       { print();            }
    virtual void    print(void);

    // -------------------------------------------------------------------------

    virtual void Draw(const Option_t* option = "");

    // -------------------------------------------------------------------------

    ClassDef(HPidHistogramsCont, 1) // Container of histograms
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDHISTOGRAMSCONT_H */
