// @(#)$Id: hpidtrackfillerpar.h,v 1.8 2004-07-08 13:45:52 christ Exp $
//*-- Author : Marcin Jaskula 09/10/2002
// Modified  : Marcin Jaskula 31/10/2002
//           - Print() and print() moved to HPidParCont
//*-- Modified: Marcin Jaskula and Jacek Otwinowski 05/02/2003
//           - iTofinoMult removed
//*-- Modified: Marcin Jaskula 14/03/2003
//           - meta overlap added

#include "TArrayF.h"
#ifndef HPIDTRACKFILLERPAR_H
#define HPIDTRACKFILLERPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidTrackFillerPar
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidparcont.h"

// -----------------------------------------------------------------------------

class HParamList;

// -----------------------------------------------------------------------------

#define HPIDTRACKFILLERPAR_NAME    "PidTrackFillerPar"
#define HPIDTRACKFILLERPAR_TITLE   "Parameters for HPidTrackFiller"
#define HPIDTRACKFILLERPAR_CONTEXT "PidTrackFillerPar"
#define MAX_KICK_RICH_BIN 10
// -----------------------------------------------------------------------------

class HPidTrackFillerPar : public HPidParCont
{
private:

    Float_t     fWindowKickRichTheta;
    Float_t     fWindowKickRichPhi;
    Float_t     fWindowMdcRichTheta;
    Float_t     fWindowMdcRichPhi;

    TArrayF     pLeftKickRichTheta[6];
    TArrayF     pRightKickRichTheta[6];

    TArrayF     pLeftKickRichPhi[6];
    TArrayF     pRightKickRichPhi[6];

    TArrayF     pLeftMdcRichTheta;
    TArrayF     pRightMdcRichTheta;

    TArrayF     pLeftMdcRichPhi;
    TArrayF     pRightMdcRichPhi;

    Float_t     fOverlapTofMax;
    Float_t     fOverlapShwMin;
    Float_t     fOverlapDPhi;
    Float_t     fOverlapDThetaMinNeg;
    Float_t     fOverlapDThetaMaxNeg;
    Float_t     fOverlapDThetaMinPos;
    Float_t     fOverlapDThetaMaxPos;

    // -------------------------------------------------------------------------

public:

                    HPidTrackFillerPar(
                            const char *name    = HPIDTRACKFILLERPAR_NAME,
                            const char *title   = HPIDTRACKFILLERPAR_TITLE,
                            const char *context = HPIDTRACKFILLERPAR_CONTEXT);
    virtual        ~HPidTrackFillerPar(void);

    virtual void    clear(void);

    virtual void    putParams(HParamList *pl);
    virtual Bool_t  getParams(HParamList *pl);

    // -------------------------------------------------------------------------

    Float_t     getWindowKickRichTheta(void) const
                                            { return fWindowKickRichTheta;    }
    void        setWindowKickRichTheta(Float_t f) { fWindowKickRichTheta = f; }

    Float_t     getWindowKickRichPhi(void) const
                                            { return fWindowKickRichPhi;      }
    void        setWindowKickRichPhi(Float_t f) { fWindowKickRichPhi = f;     }

    Float_t     getWindowMdcRichTheta(void) const
                                            { return fWindowMdcRichTheta;     }
    void        setWindowMdcRichTheta(Float_t f) { fWindowMdcRichTheta = f;   }
    Float_t     getWindowMdcRichPhi(void) const
                                            { return fWindowMdcRichPhi;       }
    void        setWindowMdcRichPhi(Float_t f) { fWindowMdcRichPhi = f;       }

 	TArrayF     &getLeftKickRichTheta(Int_t nSector) 
											{ return pLeftKickRichTheta[nSector];	}
    void        setLeftKickRichTheta(TArrayF pL,Int_t nSector) 
											{pLeftKickRichTheta[nSector]=pL;}

 	TArrayF     &getRightKickRichTheta(Int_t nSector) 
											{ return pRightKickRichTheta[nSector];	}
    void        setRightKickRichTheta(TArrayF pR,Int_t nSector) 
											{	pRightKickRichTheta[nSector] = pR;	}
 	TArrayF     &getLeftKickRichPhi(Int_t nSector) 
											{ return pLeftKickRichPhi[nSector];	}
    void        setLeftKickRichPhi(TArrayF pL,Int_t nSector) 
											{ 	pLeftKickRichPhi[nSector] = pL;	}

 	TArrayF     &getRightKickRichPhi(Int_t nSector) 
											{ return pRightKickRichPhi[nSector];	}
    void        setRightKickRichPhi(TArrayF pR,Int_t nSector) 
											{	pRightKickRichPhi[nSector] = pR;	}

	TArrayF     &getLeftMdcRichTheta(void) 
											{ return pLeftMdcRichTheta;	}
    void        setLeftMdcRichTheta(TArrayF pL) 
											{ 	pLeftMdcRichTheta = pL;	}

 	TArrayF     &getRightMdcRichTheta(void) 
											{ return pRightMdcRichTheta;	}
    void        setRightMdcRichTheta(TArrayF pR) 
											{	pRightMdcRichTheta = pR;	}
 	TArrayF     &getLeftMdcRichPhi(void) 
											{ return pLeftMdcRichPhi;	}
    void        setLeftMdcRichPhi(TArrayF pL) 
											{ 	pLeftMdcRichPhi = pL;	}

 	TArrayF     &getRightMdcRichPhi(void) 
											{ return pRightMdcRichPhi;	}
    void        setRightMdcRichPhi(TArrayF pR) 
											{	pRightMdcRichPhi = pR;	}

    Float_t     getOverlapTofMax(void) const    { return fOverlapTofMax;      }
    void        setOverlapTofMax(Float_t f)     { fOverlapTofMax = f;         }
    Float_t     getOverlapShwMin(void) const    { return fOverlapShwMin;      }
    void        setOverlapShwMin(Float_t f)     { fOverlapShwMin = f;         }
    Float_t     getOverlapDPhi(void) const      { return fOverlapDPhi;        }
    void        setOverlapDPhi(Float_t f)       { fOverlapDPhi = f;           }
    Float_t     getOverlapDThetaMinNeg(void) const
                                                { return fOverlapDThetaMinNeg;}
    void        setOverlapDThetaMinNeg(Float_t f)
                                                { fOverlapDThetaMinNeg = f;   }
    Float_t     getOverlapDThetaMaxNeg(void) const
                                                { return fOverlapDThetaMaxNeg;}
    void        setOverlapDThetaMaxNeg(Float_t f)
                                                { fOverlapDThetaMaxNeg = f;   }
    Float_t     getOverlapDThetaMinPos(void) const
                                                { return fOverlapDThetaMinPos;}
    void        setOverlapDThetaMinPos(Float_t f)
                                                { fOverlapDThetaMinPos = f;   }
    Float_t     getOverlapDThetaMaxPos(void) const
                                                { return fOverlapDThetaMaxPos;}
    void        setOverlapDThetaMaxPos(Float_t f)
                                                { fOverlapDThetaMaxPos = f;   }

    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFillerPar, 6) // Container with params for HPidTrackFiller
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDTRACKFILLERPAR_H */
