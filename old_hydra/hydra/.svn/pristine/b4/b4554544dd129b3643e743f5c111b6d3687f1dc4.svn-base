// @(#)$Id:
//*-- Author : Tomasz  Wójcik 24/04/2003

#ifndef HPIDALGSHOWER_H
#define HPIDALGSHOWER_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgShower                                                              //
//                                                                            //
// PreShower algorithms for PID                                               //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgorithm.h"

// -----------------------------------------------------------------------------

class HPidAlgShower;
class HPidAlgShowerPar;
class HKickTrack;
class HShowerHitTof;
// -----------------------------------------------------------------------------

class HPidAlgShower : public HPidAlgorithm {
private:
    HPidAlgShowerPar *m_pParams;
    HKickTrack *m_pKickTrack;
    HShowerHitTof *m_pShowerHitTof;
    Int_t m_nId;
    Int_t m_nOffset;
    Float_t m_fFactor;
    Float_t m_fCharge;
    Float_t m_fMom;
    Float_t m_fTheta;
    Float_t m_aSum[3];
    TString *m_sDesc;
public:

    HPidAlgShower(void);
    HPidAlgShower(Float_t fWeight);
    ~HPidAlgShower(void) {}

    // -------------------------------------------------------------------------

    virtual Bool_t      init(void);
    virtual Bool_t      reinit(void);
    virtual Bool_t      finalize(void);

    virtual Bool_t      calculatePDF(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);
    virtual Bool_t      calculateCL(HPidTrackCand *pTrack,
                                    Float_t afReturn[], Short_t &nCatIndex);

    virtual void        print(void) const;
    void clearMembers();
    // -------------------------------------------------------------------------

    ClassDef(HPidAlgShower, 0)  // PreShower algorithms for PID
};

// -----------------------------------------------------------------------------

#endif //HPIDALGSHOWER_H
