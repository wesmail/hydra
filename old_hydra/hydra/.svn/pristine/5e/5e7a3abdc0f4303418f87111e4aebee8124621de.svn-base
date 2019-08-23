// @(#)$Id: hpidtrackfillerpar.h,v 1.9 2005-01-11 11:06:47 halo Exp $
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

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

class HParamList;

// -----------------------------------------------------------------------------

#define HPIDTRACKFILLERPAR_NAME    "PidTrackFillerPar"
#define HPIDTRACKFILLERPAR_TITLE   "Parameters for HPidTrackFiller"
#define HPIDTRACKFILLERPAR_CONTEXT "PidTrackFillerPar"
#define MAX_KICK_RICH_BIN 10
// -----------------------------------------------------------------------------

class HPidTrackFillerPar : public HPidHistogramsCont
{
private:

    

    // -------------------------------------------------------------------------

public:

                    HPidTrackFillerPar(
                            const char *name    = HPIDTRACKFILLERPAR_NAME,
                            const char *title   = HPIDTRACKFILLERPAR_TITLE,
                            const char *context = HPIDTRACKFILLERPAR_CONTEXT);
		    ~HPidTrackFillerPar(void){}

    // -------------------------------------------------------------------------
	    
    Float_t getDeltaPhiMin(Int_t sector, Float_t MomValue);
    Float_t getDeltaPhiMax(Int_t sector, Float_t MomValue);
    Float_t getDeltaThetaMin(Int_t sector, Float_t MomValue);
    Float_t getDeltaThetaMax(Int_t sector, Float_t MomValue);



    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFillerPar, 7) // Container with params for HPidTrackFiller
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDTRACKFILLERPAR_H */
