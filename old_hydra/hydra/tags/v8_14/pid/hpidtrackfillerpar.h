// @(#)$Id: hpidtrackfillerpar.h,v 1.15 2006-08-16 16:13:35 halo Exp $
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
// HPidTrackFillerPar - documentation in cc-file
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidhistogramscont.h"

// -----------------------------------------------------------------------------

class HParamList;

// -----------------------------------------------------------------------------

#define HPIDTRACKFILLERPAR_NAME    "PidTrackFillerPar"
#define HPIDTRACKFILLERPAR_TITLE   "Parameters for HPidTrackFiller"
#define HPIDTRACKFILLERPAR_CONTEXT "PidTrackFillerPar"
//#define MAX_KICK_RICH_BIN 10
// -----------------------------------------------------------------------------

class HPidTrackFillerPar : public HPidHistogramsCont
{
private:

  Float_t DeltaMdcMetaXMin[6][2]; //Windows for correlating META hits and MDC tracks
  Float_t DeltaMdcMetaYMin[6][2];
  Float_t DeltaMdcMetaXMax[6][2];
  Float_t DeltaMdcMetaYMax[6][2];


  Float_t DeltaRkMetaXMin[6][2]; //Windows for correlating META hits and Rk tracks
  Float_t DeltaRkMetaYMin[6][2];
  Float_t DeltaRkMetaXMax[6][2];
  Float_t DeltaRkMetaYMax[6][2];

    // -------------------------------------------------------------------------

public:

                    HPidTrackFillerPar(
                            const char *name    = HPIDTRACKFILLERPAR_NAME,
                            const char *title   = HPIDTRACKFILLERPAR_TITLE,
                            const char *context = HPIDTRACKFILLERPAR_CONTEXT);
		    ~HPidTrackFillerPar(void){}

    // -------------------------------------------------------------------------
    Float_t getInterpolatedValue(TH1* pH, Int_t sector, Float_t MomValue);
    Float_t getDeltaPhiMin(Int_t sector, Float_t MomValue);
    Float_t getDeltaPhiMax(Int_t sector, Float_t MomValue);
    Float_t getDeltaThetaMin(Int_t sector, Float_t MomValue);
    Float_t getDeltaThetaMax(Int_t sector, Float_t MomValue);
    Float_t getPolynomialCoeff(Int_t sector, Float_t VertexZCoord, Float_t* coefficients);

    Float_t getDeltaRkMetaXMin(Int_t sector,Int_t system) const {return DeltaRkMetaXMin[sector][system];}
    Float_t getDeltaRkMetaYMin(Int_t sector, Int_t system) const {return DeltaRkMetaYMin[sector][system];}
    Float_t getDeltaRkMetaXMax(Int_t sector, Int_t system) const {return DeltaRkMetaXMax[sector][system];}
    Float_t getDeltaRkMetaYMax(Int_t sector, Int_t system) const {return DeltaRkMetaYMax[sector][system];}

    Float_t getDeltaMdcMetaXMin(Int_t sector, Int_t system) const {return DeltaMdcMetaXMin[sector][system];}
    Float_t getDeltaMdcMetaYMin(Int_t sector, Int_t system) const {return DeltaMdcMetaYMin[sector][system];}
    Float_t getDeltaMdcMetaXMax(Int_t sector, Int_t system) const {return DeltaMdcMetaXMax[sector][system];}
    Float_t getDeltaMdcMetaYMax(Int_t sector, Int_t system) const {return DeltaMdcMetaYMax[sector][system];}



    void setDeltaRkMetaXMin(Int_t sector, Int_t system, Float_t val) {DeltaRkMetaXMin[sector][system] = val;}
    void setDeltaRkMetaYMin(Int_t sector, Int_t system, Float_t val) {DeltaRkMetaYMin[sector][system] = val;}
    void setDeltaRkMetaXMax(Int_t sector, Int_t system, Float_t val) {DeltaRkMetaXMax[sector][system] = val;}
    void setDeltaRkMetaYMax(Int_t sector, Int_t system, Float_t val) {DeltaRkMetaYMax[sector][system] = val;}

    void setDeltaMdcMetaXMin(Int_t sector, Int_t system, Float_t val) {DeltaMdcMetaXMin[sector][system] = val;}
    void setDeltaMdcMetaYMin(Int_t sector, Int_t system, Float_t val) {DeltaMdcMetaYMin[sector][system] = val;}
    void setDeltaMdcMetaXMax(Int_t sector, Int_t system, Float_t val) {DeltaMdcMetaXMax[sector][system] = val;}
    void setDeltaMdcMetaYMax(Int_t sector, Int_t system, Float_t val) {DeltaMdcMetaYMax[sector][system] = val;}




    // -------------------------------------------------------------------------

    ClassDef(HPidTrackFillerPar, 9) // Container with params for HPidTrackFiller
};

// -----------------------------------------------------------------------------

#endif  /* !HPIDTRACKFILLERPAR_H */
