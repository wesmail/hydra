// @(#)$Id: hpidalgmomvsbeta.h,v 1.1 2002-10-30 10:55:33 halo Exp $
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

    ClassDef(HPidAlgMomVsBeta, 0)  // Algorithm PID from the kick plain
};

// -----------------------------------------------------------------------------

#endif //HPIDALGMOMVSBETA_H
