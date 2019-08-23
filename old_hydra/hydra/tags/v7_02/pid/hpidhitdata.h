// @(#)$Id: hpidhitdata.h,v 1.7 2004-04-30 15:08:08 christ Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added

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

	virtual void    Print(const Option_t* option) const     { print();     }
    void            print(void) const;

    // -------------------------------------------------------------------------

    enum
    {
        kRICH   = 0x01,
        kMDC    = 0x02,
        kTOF    = 0x04,
        kSHOWER = 0x08,
        kAll    = kRICH | kMDC | kTOF | kSHOWER
    };

    void            clearFlags(void)                { ResetBit(kAll << 14); }
    UInt_t          getFlags(void)
                            { return ((TestBits(kAll << 14) >> 14) & kAll); }

    void            setFlagRICH(Bool_t b = kTRUE)   { SetBit(kRICH << 14);  }
    Bool_t          getFlagRICH(void)
                            { return TestBits(kRICH << 14);     }

    void            setFlagMDC(Bool_t b = kTRUE)    { SetBit(kMDC << 14);   }
    Bool_t          getFlagMDC(void)
                            { return TestBits(kMDC << 14);      }

    void            setFlagTOF(Bool_t b = kTRUE)    { SetBit(kTOF << 14);   }
    Bool_t          getFlagTOF(void)
                            { return TestBits(kTOF << 14);      }

    void            setFlagSHOWER(Bool_t b = kTRUE) { SetBit(kSHOWER << 14);}
    Bool_t          getFlagSHOWER(void)
                            { return TestBits(kSHOWER << 14);   }

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
    Float_t iMdcChiSquare;
    // -------------------------------------------------------------------------

    // TOF/TOFino energy lost
    Float_t fMetaEloss;

    // Shower observables:
    Float_t sum1;
    Float_t sum2;
    Float_t sum0;
    Float_t tof;

    // -------------------------------------------------------------------------

    ClassDef(HPidHitData, 3)  // Store data for HPidParticle from Hits
};

#endif /* HPidHitData_h */
