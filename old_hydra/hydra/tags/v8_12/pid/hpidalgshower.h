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
class HPidTrackData;
class HPidHitData;
// -----------------------------------------------------------------------------

class HPidAlgShower : public HPidAlgorithm {
private:
    HPidAlgShowerPar *m_pParams;    //Pointer to parameters container with PDFs
    HKickTrack *m_pKickTrack;       //Pointer to kick-plane object
    HShowerHitTof *m_pShowerHitTof; //Pointer to HShowerHitTof object
    HPidTrackData *m_pTrackData;    //Pointer to HPidTrackData object
    HPidHitData *m_pHitData;        //Pointer to HPidHitData object
    Int_t m_nId;                    //Particle ID
    Int_t m_nOffset;                //Offset for given PDF histogram
    Int_t m_nSector;                //Sector number
    Int_t m_nMomAlgInd;             //Momentum algorithm ID (see piddef.h)
    Float_t m_fFactor;              //Charge multiplication factor (max value)
    Float_t m_fCharge;              //Particle polarity
    Float_t m_fMom;                 //Track Momentum
    Float_t m_fTheta;               //Track Theta
    Float_t m_aSum[3];              //Shower Sum values
    TString *m_sDesc;               //Algorithm description
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
