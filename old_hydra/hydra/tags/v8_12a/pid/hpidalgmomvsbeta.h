// @(#)$Id: hpidalgmomvsbeta.h,v 1.3 2005-01-11 11:06:40 halo Exp $
//*-- Author : Marcin Jaskula 06/09/2002

#ifndef HPIDALGMOMVSBETA_H
#define HPIDALGMOMVSBETA_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgMomVsBeta                                                           //
//                                                                            //
// Test algorithm for PID from information from the kick plain                //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgorithm.h"

// -----------------------------------------------------------------------------

class HPidAlgMomVsBetaPar;

// -----------------------------------------------------------------------------

class HPidAlgMomVsBeta : public HPidAlgorithm
{
private:

    HPidAlgMomVsBetaPar *pParams;

public:

                        HPidAlgMomVsBeta(void);
                        HPidAlgMomVsBeta(Float_t fWeight);
                       ~HPidAlgMomVsBeta(void) {}

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);
    virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);

    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    void                setMinBkg(Float_t f)        { fMinBkg = f;          }
    Float_t             getMinBkg(void)             { return fMinBkg;       }

    void                setMaxGaussDev(Float_t f)   { fMaxGaussDev = f;     }
    Float_t             getMaxGaussDev(void)        { return fMaxGaussDev;  }

    // -------------------------------------------------------------------------

    static const Float_t    kDefMinBkg;      // default fMinBkg value
   static const Float_t    kDefMaxGaussDev; // default fMaxGaussDev value

protected:
    Float_t                 fMinBkg;        // minimum value for the background
    Float_t                 fMaxGaussDev;   // max. deviation for gausses

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgMomVsBeta, 0)  // Algorithm PID from the kick plain
};

// -----------------------------------------------------------------------------

#endif //HPIDALGMOMVSBETA_H
