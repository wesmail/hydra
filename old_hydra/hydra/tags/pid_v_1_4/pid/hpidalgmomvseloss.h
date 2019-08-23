//*-- Author : Dusan Zovinec 13/03/2003

#ifndef HPIDALGMOMVSELOSS_H
#define HPIDALGMOMVSELOSS_H

#include "hpidalgorithm.h"

// -----------------------------------------------------------------------------

class HPidAlgMomVsElossPar;
class HPidElossMPVShiftPar;

// -----------------------------------------------------------------------------

class HPidAlgMomVsEloss : public HPidAlgorithm
{
private:

    HPidAlgMomVsElossPar *pParams;
    HPidElossMPVShiftPar *pMPVShiftParam;
    Double_t dChebCo[3][13][8];

public:

                        HPidAlgMomVsEloss(void);
                        HPidAlgMomVsEloss(Float_t fWeight);
                       ~HPidAlgMomVsEloss(void) {}

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);
            Float_t     calculatePDF(Float_t fEloss, Float_t fMPVShiftParam, Float_t *fParams);
    virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);

    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgMomVsEloss, 0)  // Algorithm PID from the kick plain
};

// -----------------------------------------------------------------------------

#endif //HPIDALGMOMVSELOSS_H
