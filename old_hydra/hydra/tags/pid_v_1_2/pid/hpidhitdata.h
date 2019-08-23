// @(#)$Id: hpidhitdata.h,v 1.2 2003-03-14 15:21:17 jaskula Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added

#ifndef HPidHitData_h
#define HPidHitData_h

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all intersting data from hit objects matched in          //
// HPidTrackCand. Dedicated to store data for extensions of HPidParticle.     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include <TObject.h>

// -----------------------------------------------------------------------------

class HPidTrackCand;

// -----------------------------------------------------------------------------

class HPidHitData : public TObject
{
public:

                    HPidHitData(void)         { reset();  }
    virtual        ~HPidHitData(void)         { }

    void            reset(void);
    virtual void    Clear(Option_t*)          { reset();  }

    void            fill(HPidTrackCand *pTrack);

    // -------------------------------------------------------------------------

    virtual void    Print(Option_t*) const    { print();  }
    void            print(void) const;

    // -------------------------------------------------------------------------

    // all data are public

    // RICH stuff
    Int_t   nRingPadNr;
    Float_t fRingCentroid;
    Float_t fRichTheta;
    Float_t fRichPhi;
    Int_t   nRingPatMat;
    Int_t   nRingHouTra;
    Int_t   nRingAmplitude;
    Int_t   nRingLocalMax4;
    Float_t fRingFitVar;
    Float_t fRingFitRad;
    Float_t fRingFitTheta;
    Float_t fRingFitPhi;

    // -------------------------------------------------------------------------

    // MDC stuff
    Int_t   iMdcClsSize0;
    Int_t   iMdcNWires0;
    Int_t   iMdcLevelCls0;
    Int_t   iMdcClsSize1;
    Int_t   iMdcNWires1;
    Int_t   iMdcLevelCls1;

    // -------------------------------------------------------------------------

    // TOF/TOFino energy lost
    Float_t fMetaEloss;

    // -------------------------------------------------------------------------

    ClassDef(HPidHitData, 2)  // Store data for HPidParticle from Hits
};

#endif /* HPidHitData_h */
