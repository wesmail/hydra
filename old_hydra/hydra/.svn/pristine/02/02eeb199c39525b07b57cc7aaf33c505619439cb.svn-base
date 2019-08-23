//*-- Modified: Stefano Spataro 21/06/2005
//           - bIsAccepted included
//           - TofRec data members included 
//*-- Modified: Stefano Spataro 11/07/2005
//           - getBeta(nAlg),getMass2(nAlg) included, in order to have the correct beta/mass2 value according to tofrec flag
//*-- Modified: Stefano Spataro 05/08/2005 - Path length included (new classdef)
//*-- Modified: Stefano Spataro 22/08/2005 - Covariance matrix included (new classdef)
//*-- Modified: Anar Rustamov 08/11/2005 - Matching quality is added 
#ifndef HPIDTRACKDATA_H
#define HPIDTRACKDATA_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackData                                                        //
//                                                                      //
// A container of track data.                                           //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "piddef.h"
#include "hsymmat.h"

// -----------------------------------------------------------------------------

class HBaseTrack;
class HMetaMatch;

// -----------------------------------------------------------------------------

#define MAX_MDC_SEG 10


// -----------------------------------------------------------------------------
class HPidTrackData : public TObject
{
 protected:
  HSymMat6 cov[MAX_MOM_ALGS];     // covariance matrix of size 6
 public:
  
  HPidTrackData(void);
  
  Int_t       nBestMomAlg;
  Short_t     nRKTrackInd;        // Ind of track from runge-kutta algorithm
  Short_t     nKickTrackInd;      // Ind of the track from the Kick Plane: Index in the category
  Short_t     nKickTrack123Ind;   // Ind of the track from the 3-chamber-Kick Plane: Index in the category
  Short_t     nRefTrackInd;       // Ind of reference track 
  Short_t     nSplineTrackInd;    // Ind of spline track
  Float_t     fMetaMatchingQuality; //How good did the Meta hit match the track candidate
  Float_t     qIOMatching[MAX_MOM_ALGS];//Matching quality

  Bool_t      bIsClosePairCandidate;  //Did this have a rejected (chi-square cut) trajcetory in its neighbourhood?
  Float_t      fAngleWithClosePairCandidate;  //What is the angular distance with a possible close-pair candidate  (chi-square cut rejected trajcetory)?
  
  Short_t     aInnerMdcIds[MAX_MDC_SEG];      // Array of MDC segments in the neighbourhood
  Short_t     aInnerMdcCounts[MAX_MDC_SEG];   // How many times the segments were used
  UShort_t    unInnerMdcNumber;               // number of MDC segments in the set

  Float_t     fPull, fSplineChiSquare,fRKChiSquare;    //Quality parameters of KickPlane and Spline
    

  //These variables depend on the method of momentum reconstruction - thus we have an array
  //with one field per algorithm to store the data
  Short_t     nPolarity[MAX_MOM_ALGS];          // pos/neg paricle

  Float_t     fMomenta[MAX_MOM_ALGS];  //Stores the reconstructed momentum for each algorithm
  Float_t     fMomError[MAX_MOM_ALGS]; //Stores the error of the reconstructed momenta
  Float_t     fTrackR[MAX_MOM_ALGS];   //Stores the R (radius of closes approach for this track)
  Float_t     fTrackZ[MAX_MOM_ALGS];   //Stores the Z (z-coord of closes approach for this track)
  Float_t     fCorrectedEloss[MAX_MOM_ALGS]; //Energyloss as computed AFTER mom reconstruction; (from base track object)
  Float_t     fCorrectedBeta[MAX_MOM_ALGS];  //Velocity as computed AFTER length of track is known from mom reconstruction
  Float_t     fPathLength[MAX_MOM_ALGS];     // Path length of the track
  	      					    
  Float_t     fMassSquared[MAX_MOM_ALGS];  //Reconstructed Mass square values;

  Bool_t      bIsAccepted[MAX_MOM_ALGS];   // Stores the isXXXAccepted() values from HMetaMatch	 

  // These variables are used by the HPidTofRec start-time reconstructor
  Short_t     nTofRecFlag[MAX_MOM_ALGS];         // TofRec flag : -1: unchanged Tof (TofRec failed) / 0: unchanged tof (no HTofRec) / 1: lepton id / 2: pi- id 
  Float_t     fTofRecTof[MAX_MOM_ALGS];          // Reconstructed time-of-flight
  Float_t     fTofRecBeta[MAX_MOM_ALGS];         // Reconstructed beta
  Float_t     fTofRecMassSquared[MAX_MOM_ALGS];  // Reconstructed Mass square values 
  
  Float_t     getBeta(Int_t nAlg) { if (nTofRecFlag[nAlg]==0) return fCorrectedBeta[nAlg]; else return fTofRecBeta[nAlg];} // function to get the correct beta according to tofrec flag
  Float_t     getMass2(Int_t nAlg) { if (nTofRecFlag[nAlg]==0) return fMassSquared[nAlg]; else return fTofRecMassSquared[nAlg];} // function to get the correct mass2 according to tofrec flag
  Float_t     getPathLength(Int_t nAlg) { return fPathLength[nAlg];}

  HSymMat     &getCovariance(Int_t nAlg) { return cov[nAlg]; } 

  Float_t     getIOMatching(Int_t nAlg) {return qIOMatching[nAlg];}
   // -------------------------------------------------------------------------
  
  virtual void reset(Option_t * /*option*/ = "");

  // -------------------------------------------------------------------------
  
  void            print(void) const;
  void            Print(Option_t *o = "") const { print();                   }
  
  Bool_t          isBad(void) const         { return TestBit(kIsBad);        }
  void            setBad(Bool_t b = kTRUE)  { SetBit(kIsBad, b);             }
  
  Bool_t          isFiltered(void) const       { return TestBit(kIsFiltered);}
  void            setFiltered(Bool_t b = kTRUE){ SetBit(kIsFiltered, b);     }

  void            setIsCPCandidate(Bool_t b=kTRUE) {bIsClosePairCandidate=b;}
  
  // -------------------------------------------------------------------------
  HBaseTrack*     getBaseTrack(Int_t momAlgIndex) const;

  void                  fillFromBaseTrack(HBaseTrack* pBaseTrack, Int_t momalg);
  void                  fillFromMetaMatch(HMetaMatch* pMetaMatch);
  void                  fillFromTrackData(HPidTrackData* pSource);
  // -------------------------------------------------------------------------
  
  ClassDef(HPidTrackData, 7)  // Track data for PID
};
    
#endif //HPIDTRACKDATA_H
