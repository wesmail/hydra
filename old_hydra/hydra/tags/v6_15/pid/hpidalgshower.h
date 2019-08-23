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
// -----------------------------------------------------------------------------

class HPidAlgShower : public HPidAlgorithm {
private:
    EnumPidAlgorithm_t m_algType;
    Int_t   m_nMult;
    HPidAlgShowerPar *pParams;
    TString *sDesc;
public:

                        HPidAlgShower(void);
                        HPidAlgShower(EnumPidAlgorithm_t algType,Int_t nMult=1);
                        HPidAlgShower(Float_t fWeight);
                        HPidAlgShower(EnumPidAlgorithm_t algType, Float_t fWeight,Int_t nMult=1);
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

    // -------------------------------------------------------------------------

    ClassDef(HPidAlgShower, 0)  // PreShower algorithms for PID
};

// -----------------------------------------------------------------------------

#endif //HPIDALGSHOWER_H
