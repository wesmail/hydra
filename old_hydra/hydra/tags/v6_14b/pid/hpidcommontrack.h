// @(#)$Id: hpidcommontrack.h,v 1.3 2003-10-23 18:54:25 jaskula Exp $
//*-- Author : Marcin Jaskula 25/02/2003
//  Modified : Marcin Jaskula 29/05/2003
//             getLorentzVector added
//  Modified : Marcin Jaskula 22/06/2003
//             static method getLorentzVector from HGeantKine added

#ifndef HPidCommonTrack_h
#define HPidCommonTrack_h

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidCommonTrack                                                            //
//                                                                            //
// Common track used in HPidGeantTrackSet                                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include <TVector3.h>
#include <TLorentzVector.h>

// -----------------------------------------------------------------------------

class HGeantKine;
class HCategory;
class HPidGeantTrackSet;

// -----------------------------------------------------------------------------

class HPidCommonTrack
{
public:

    enum enumCommonTrack
    {
        kTrackNotSet    = -1, // the object doesn't contain valid data
        kTrackFull      =  0, // seen in all detectors: RICH, MDC and META
        kTrackKickPlane =  1, // seen in MDC and META (from HKickTrack)
        kTracklet       =  2  // seen before the MF (RICH and MDC)
    };

    // -------------------------------------------------------------------------

                HPidCommonTrack(void)       { setDefault();         }
    virtual    ~HPidCommonTrack(void)       { }

    // -------------------------------------------------------------------------

    void        set(const HPidCommonTrack *pSet);

    Char_t      getDetectors(void) const    { return cDetectors;    }
    void        setDetectors(Char_t c)      { cDetectors = c;       }

    UInt_t      getProcess(void) const      { return iProcess;      }
    void        setProcess(UInt_t ui)       { iProcess = ui;        }

    UInt_t      getMask(void) const         { return iMask;         }
    void        setMask(UInt_t ui)          { iMask = ui;           }

    Short_t     getTrackId(void) const      { return nTrackId;      }
    void        setTrackId(Short_t n)       { nTrackId = n;         }

    Short_t     getGeantId(void) const      { return nGeantId;      }
    void        setGeantId(Short_t n)       { nGeantId = n;         }

    void        setGeantParams(Short_t nTrack, Short_t nPid)
                                        { nTrackId = nTrack; nGeantId = nPid;  }

    const TVector3*     getMomentum(void) const         { return &vMomentum; }
    void                setMomentum(const TVector3 &v)  { vMomentum = v;     }
    void                setMomentum(const TVector3 *p)  { vMomentum = *p;    }
    void                setMomentum(Double_t dX, Double_t dY, Double_t dZ)
                                        { vMomentum.SetXYZ(dX, dY, dZ);      }

    TLorentzVector      getLorentzVector(void) const;

    // -------------------------------------------------------------------------

    Bool_t              fillKine(UInt_t iMask = 0);

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option = "") const     { print();  }
    void                print(void) const;

    void                Clear(Option_t *opt = "")       { setDefault();     }

    // -------------------------------------------------------------------------

    static HGeantKine*  getGeantKine(Int_t iTrack, HCategory *pCat = NULL);

    static TLorentzVector getLorentzVector(HGeantKine *pKine);
    static TLorentzVector getLorentzVector(Short_t nTrack,
                            HCategory *pCat = NULL);

protected:

    Char_t      cDetectors; // where seen (see enumHPidCommonTrack)
    UInt_t      iProcess;   // bit flags defining the processes and parents
    UInt_t      iMask;      // which bits in the iProcess flag are valid

    Short_t     nTrackId;   // track id in HGeantKine branch
    Short_t     nGeantId;   // pid of the track

    TVector3    vMomentum;  // 3 momentum of the particle (form HGenatKine)

    // -------------------------------------------------------------------------

    void        setDefault(void);

    // -------------------------------------------------------------------------

    ClassDef(HPidCommonTrack, 1)  // Parameters of the "common" track
};

// -----------------------------------------------------------------------------

#endif /* HPidCommonTrack_h */
