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

// -----------------------------------------------------------------------------

class HBaseTrack;
class HMetaMatch;

// -----------------------------------------------------------------------------

#define MAX_MDC_SEG 10
#define MAX_MOM_ALGS 10


// -----------------------------------------------------------------------------
class HPidTrackData : public TObject
{
 public:
  
  HPidTrackData(void);
  
  Int_t       nBestMomAlg;
  Short_t     nRKTrackInd;        // Ind of track from runge-kutta algorithm
  Short_t     nKickTrackInd;      // Ind of the track from the 3-chamber-Kick Plane: Index in the category
  Short_t     nKickTrack123Ind;      // Ind of the track from the Kick Plane: Index in the category
  Short_t     nRefTrackInd;       // Ind of reference track 
  Short_t     nSplineTrackInd;    // Ind of spline track
  Float_t     fMetaMatchingQuality; //How good did the Meta hit match the track candidate
  
  Short_t     aInnerMdcIds[MAX_MDC_SEG];      // Array of MDC segments in the neighbourhood
  Short_t     aInnerMdcCounts[MAX_MDC_SEG];   // How many times the segments were used
  UShort_t    unInnerMdcNumber;               // number of MDC segments in the set

  Float_t     fPull, fSplineChiSquare;    //Quality parameters of KickPlane and Spline
    

  //These variables depend on the method of momentum reconstruction - thus we have an array
  //with one field per algorithm to store the data
  Short_t     nPolarity[MAX_MOM_ALGS];          // pos/neg paricle

  Float_t     fMomenta[MAX_MOM_ALGS];  //Stores the reconstructed momentum for each algorithm
  Float_t     fMomError[MAX_MOM_ALGS]; //Stores the error of the reconstructed momenta
  Float_t     fTrackR[MAX_MOM_ALGS];   //Stores the R (radius of closes approach for this track)
  Float_t     fTrackZ[MAX_MOM_ALGS];   //Stores the Z (z-coord of closes approach for this track)
  Float_t     fCorrectedEloss[MAX_MOM_ALGS]; //Energyloss as computed AFTER mom reconstruction; (from base track object)
  Float_t     fCorrectedBeta[MAX_MOM_ALGS];  //Velocity as computed AFTER length of track is known from mom reconstruction
  	      					    
  Float_t     fMassSquared[MAX_MOM_ALGS];  //Reconstructed Mass square values;

  // -------------------------------------------------------------------------
  
  virtual void reset(Option_t * /*option*/ = "");

  // -------------------------------------------------------------------------
  
  void            print(void) const;
  void            Print(Option_t *o = "") const { print();                   }
  
  Bool_t          isBad(void) const         { return TestBit(kIsBad);        }
  void            setBad(Bool_t b = kTRUE)  { SetBit(kIsBad, b);             }
  
  Bool_t          isFiltered(void) const       { return TestBit(kIsFiltered);}
  void            setFiltered(Bool_t b = kTRUE){ SetBit(kIsFiltered, b);     }
  
  // -------------------------------------------------------------------------
  HBaseTrack*     getBaseTrack(Int_t momAlgIndex) const;

  void                  fillFromBaseTrack(HBaseTrack* pBaseTrack, Int_t momalg);
  void                  fillFromMetaMatch(HMetaMatch* pMetaMatch);
  void                  fillFromTrackData(HPidTrackData* pSource);
  // -------------------------------------------------------------------------
  
  ClassDef(HPidTrackData, 1)  // Track data for PID
};
    
#endif //HPIDTRACKDATA_H
