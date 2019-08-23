#ifndef HPIDALGLEPTONCAND_H
#define HPIDALGLEPTONCAND_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidAlgLeptonCand                                                          //
//                                                                            //
// Algoritm 1 for rich-kickplane correlated                                   //
// Using Rich-KickPlane information                                           //
////////////////////////////////////////////////////////////////////////////////

#include "hpidalgorithm.h"

// -----------------------------------------------------------------------------

//class HPidAlgClosePairRejPar;

// -----------------------------------------------------------------------------

class HPidAlgLeptonCand : public HPidAlgorithm
{
private:

  
public:

                        HPidAlgLeptonCand(void);
                        HPidAlgLeptonCand(Float_t fWeight);
                       ~HPidAlgLeptonCand(void) {}

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

    ClassDef(HPidAlgLeptonCand, 0)  // Algorithm PID from mdc-rich hits
};

// -----------------------------------------------------------------------------

#endif //HPIDLEPTONCAND_H
