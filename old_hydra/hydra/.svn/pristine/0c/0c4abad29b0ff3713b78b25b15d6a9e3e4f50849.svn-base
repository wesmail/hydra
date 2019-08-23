// @(#)$Id: hpidhistogramscont.h,v 1.10 2008-05-09 16:15:06 halo Exp $
//*-- Author : Marcin Jaskula 10/10/2002
// Modified  : Marcin Jaskula 02/06/2003
//           - Draw() method added

#ifndef HPIDHISTOGRAMSCONT_H
#define HPIDHISTOGRAMSCONT_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidHistogramsCont: Base class for all histograms holding PDFs, CLs and
// Relints. There are three numbers govering the arrangement of histograms
// in the TObjArray aHistograms: 
// 
// The number of particles
// The number of histograms per particle
// The number of sectors (constant - 6)
//
// The order is such:
// 1st histogram for species 1 and sector 0
// 2nd histogram for species 1 and sector 0
// 3rd...
// 4th...
// ...
// 1st histogram for species 1 and sector 1
// 2nd histogram for species 1 and sector 1
// 3rd...
// 4th...
// ...
// 1st histogram for species 2 and sector 0
// 2nd histogram for species 2 and sector 0
// 3rd...
// 4th...
// ...

//
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

    Int_t           getIndex(Int_t iPart, Int_t iOffset, Int_t iSector ) const
      { 
	return 6 * iHistsPerPart * iPart + iHistsPerPart * iSector + iOffset; 
      }
    

    Bool_t          resize(Int_t iNPSet);

public:
                    HPidHistogramsCont(void);
                    HPidHistogramsCont(const Char_t *name, const Char_t *title,
                                    const Char_t *context,
                                    Int_t iHistsPerPart = 1);
    virtual        ~HPidHistogramsCont(void);

    virtual void    clear(void);
    virtual void    detachHistograms(void);

    virtual void    putParams(HParamList *pl);
    virtual Bool_t  getParams(HParamList *pl);

    // -------------------------------------------------------------------------
    TObjArray & getArray(){return aHistograms;}
    Int_t           getNParticles(void) const   { return iNParticles;   }
    Int_t           getHistsPerPart(void) const { return iHistsPerPart; }
    Int_t           getNHistograms(void) const
                                { return iNParticles * iHistsPerPart;   }

    // -------------------------------------------------------------------------

    Int_t           findParticleIndex(Int_t iPartId) const;

    Bool_t          addHistogram(Int_t iPartId, Int_t iOffset, Int_t iSector,
                            const TH1 *pHist);

    Bool_t          addHistogram(Int_t iPartId, const TH1 *pHist)
                                { return addHistogram(iPartId, 0, 0,  pHist); }

    void            removeHistogram(Int_t iPartId, Int_t iOffset = 0, Int_t iSector = 0 );
    TH1*            getHistogram(Int_t iPartId, Int_t iOffset, Int_t iSector) const;
    //TH1*            getHistogram(Int_t iPartId, Int_t iOffset = 0) const;

    Stat_t          getValue(Int_t iPartId, Int_t iOffset, Int_t iSector,
                                Float_t fX, Float_t fY = 0.0f,
                                Float_t fZ = 0.0f) const;

    Stat_t          getValue(Int_t iPartId, Float_t fX,
                                Float_t fY = 0.0f, Float_t fZ = 0.0f) const
                                { return getValue(iPartId, 0, 0, fX, fY, fZ); }

    // -------------------------------------------------------------------------

    void            setWarnings(Bool_t b = kTRUE) { bWarnings = b;      }
    Bool_t          getWarnings(void) const       { return bWarnings;   }

    // -------------------------------------------------------------------------

    virtual void    Print(Option_t *p = "")       { print();            }
    virtual void    print(void);

    // -------------------------------------------------------------------------

    virtual void Draw(const Option_t* option = "");

    // -------------------------------------------------------------------------
    virtual Bool_t checkContext(Int_t nMomAlgInd);
    virtual void setContext(Int_t nMomAlgInd);
    // -------------------------------------------------------------------------

    ClassDef(HPidHistogramsCont, 1) // Container of histograms
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDHISTOGRAMSCONT_H */
