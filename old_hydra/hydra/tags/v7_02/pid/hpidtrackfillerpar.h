// @(#)$Id: hpidtrackfillerpar.h,v 1.7 2004-04-30 16:29:02 christ Exp $
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

// -----------------------------------------------------------------------------

class HPidTrackFillerPar : public HPidParCont
{
private:

    Float_t     fWindowKickRichTheta;
    Float_t     fWindowKickRichPhi;
    Float_t     fWindowMdcRichTheta;
    Float_t     fWindowMdcRichPhi;

    TArrayF     pLeftKickRichTheta;
    TArrayF     pRightKickRichTheta;

    TArrayF     pLeftKickRichPhi;
    TArrayF     pRightKickRichPhi;

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

 	TArrayF     &getLeftKickRichTheta(void) 
											{ return pLeftKickRichTheta;	}
    void        setLeftKickRichTheta(TArrayF pL) 
											{ 	pLeftKickRichTheta = pL;	}

 	TArrayF     &getRightKickRichTheta(void) 
											{ return pRightKickRichTheta;	}
    void        setRightKickRichTheta(TArrayF pR) 
											{	pRightKickRichTheta = pR;	}
 	TArrayF     &getLeftKickRichPhi(void) 
											{ return pLeftKickRichPhi;	}
    void        setLeftKickRichPhi(TArrayF pL) 
											{ 	pLeftKickRichPhi = pL;	}

 	TArrayF     &getRightKickRichPhi(void) 
											{ return pRightKickRichPhi;	}
    void        setRightKickRichPhi(TArrayF pR) 
											{	pRightKickRichPhi = pR;	}

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

    ClassDef(HPidTrackFillerPar, 5) // Container with params for HPidTrackFiller
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDTRACKFILLERPAR_H */
