// @(#)$Id: hpidgeanttrackset.h,v 1.9 2006-11-24 16:00:42 christ Exp $
//*-- Author : Marcin Jaskula 01/12/2002
//  Modified : Marcin Jaskula 02/03/2003
//             get rid of TObject
//  Modified : Tassilo Christ 15.11.03 to adapt class for new tracking/pid interface

#ifndef HPidGeantTrackSet_h
#define HPidGeantTrackSet_h

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidGeantTrackSet                                                          //
//                                                                            //
// Geant tracks matched in HPidTrackCand. This container stores all           //
// information available from GEANT about particles which caused the hits     //
// that where associated with one another to form a particle candidate.       //
// This covers: True ID, particle number, creation mechanism, medium and      //
// parent, vertex coordinates and true momentum
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include <TObject.h>
#include "piddef.h"
#include <TArrayI.h>
#include <TArrayF.h>
#include <TLorentzVector.h>
#include <TVector3.h>

// -----------------------------------------------------------------------------


class HGeantKine;
class HGeantTof;
class HCategory;
class HPidTrackCand;
class HMetaMatch;

// -----------------------------------------------------------------------------

class HPidGeantTrackSet: public TObject
{
 public:
  
  //Possibly not needed any longer
  enum
    {
      //kNoTrack       =  0,    // no track
      //kNotFilled     = -1,    // this member was not filled
      //kNoRefObj      = -2,    // no availabe input object with tracks
      kOutOfBound    = -3,    // error value when data out of bounds requested
    };
  
  
  
  // -------------------------------------------------------------------------
  
  HPidGeantTrackSet(void)             { reset();  }
  HPidGeantTrackSet(const HPidGeantTrackSet& source);
  virtual    ~HPidGeantTrackSet(void) {}
  
  void        reset(void);
  
  // -------------------------------------------------------------------------
  Short_t     getRichTrack(UInt_t n) const;        //return the n-th track-Id in the array of track ids which contributed to the ring
  
  Short_t     getInnerMdcTrack(UInt_t n) const;    
  Short_t     getOuterMdcTrack(UInt_t n) const;
  Short_t     getShowerTrack(UInt_t n) const;
  Short_t     getTOFTrack(UInt_t n) const;
  Short_t     getRichIPUTrack(UInt_t n) const;
  Short_t     getMetaTrack(UInt_t n) const;
  
  
  Float_t     getRichWeight(UInt_t iIdx) const {return RICHWeights[iIdx];} 
  //return the weight at counter-position iIdx in the array of track-weights
  //which contributed to the ring
  Float_t     getInnerMdcWeight(UInt_t iIdx) const {return InnerMDCWeights[iIdx];} 
  Float_t     getOuterMdcWeight(UInt_t iIdx) const {return OuterMDCWeights[iIdx];} 
  Float_t     getShowerWeight(UInt_t iIdx) const   {return ShowerWeights[iIdx];} 
  Float_t     getTOFWeight(UInt_t iIdx) const      {return TOFWeights[iIdx];} 
  Float_t     getRichIPUWeight(UInt_t iIdx) const  {return RICHIPUWeights[iIdx];} 
  //Float_t     getMetaWeight(UInt_t iIdx);       
  
  
  // -------------------------------------------------------------------------
  //Get number of tracks contributing to the detector hits
  UInt_t      getNRichTracks(void) const {return nRICHTracks;}
  UInt_t      getNRichIPUTracks(void) const  {return nRICHIPUTracks;}
  UInt_t      getNInnerMdcTracks(void) const {return nInnerMdcTracks;}
  UInt_t      getNOuterMdcTracks(void) const  {return nOuterMdcTracks;}
  UInt_t      getNShowerTracks(void) const {return nShowerTracks;}
  UInt_t      getNTOFTracks(void) const {return nTOFTracks;}
  UInt_t      getNMetaTracks(void) const;
  UInt_t      getNCorrelatedTrackIds(void) const {return sNCorrTrackIds;}
  
  //get information from an input object and store the geant information from the hits that where correlated
  //in the HMetaMatch instance
  void         fillFromMetaMatch(HMetaMatch* pMetaMatch);
  
  // -------------------------------------------------------------------------
  //What was the strongest correlation in this set of hits? (Which is the largest set of detectors that saw the same particle)
  Short_t     getMostCommonCorrelation() const//OK
    {
      if (!isSorted) 
	{
	  ::Error("getMostCommonCorrelation","The Geant tracks are not sorted!");
	  return -1;
	}
      return correlationFlags[0];
    }

  //appends a track ID to the list of geant tracks that made up an HMetaMatch
  Int_t       addToContributingTracks(Int_t index, UInt_t detector); 
  
  //checks whether a track id is in this geant set
  Bool_t      isPartOfTrackSet(Int_t trackID);                 
  
  //finds the index (position) of the particle in the list of contributing tracks
  Int_t       getParticleIndex(Int_t trackID);
  
  
  //Adds the weight of the track to the list of weights in the detector in which the track was seen
  void        addTrackWeightToDetector(Float_t weight, Int_t trackID, UInt_t detector);
  
  
  
  //These functions determine whether or not the bit in "flag" for the "detector" is set or not.
  //Thus they tell us whether a correlation flag includes correlation in a specific subsystem
  //Many of them are obsolete
  
  Bool_t      wasSeenInDetector(const UInt_t flag, const UInt_t detector,Bool_t verbose=kFALSE) const;
  //Bool_t      wasSeenInInnerMdc(const UInt_t flag) const {return flag & kSeenInInnerMDC;}
  //Bool_t      wasSeenInOuterMdc(const UInt_t flag) const {return flag & kSeenInOuterMDC;}
  //Bool_t      wasSeenInMeta(const UInt_t flag) const {return flag & kSeenInMETA;}
  //Bool_t      wasSeenInTOF(const UInt_t flag) const {return flag & kSeenInTOF;}
  //Bool_t      wasSeenInShower(const UInt_t flag) const {return flag & kSeenInShower;}
  //Bool_t      wasSeenInRICH(const UInt_t flag) const {return flag & kSeenInRICH;}
  //Bool_t      wasSeenInRICHIPU(const UInt_t flag) const {return flag & kSeenInRICHIPU;}
  // -------------------------------------------------------------------------
  
  //Clean up dynamic memory - called when pCategory->Clear("C") is executed
  void            Clear(Option_t *opt = "");          //{ reset();  }

  virtual void    Print(Option_t* option = "") const      { print();     }
  
  void            print(void) const;
  
  // -------------------------------------------------------------------------
  
  //Find a certain GeantKineObject (the one for track number iTrack) 
  static HGeantKine*  getGeantKine(Int_t iTrack, HCategory *pCat = NULL);
  
  // -------------------------------------------------------------------------
  
  //Get information from the geant-info arrays by index
  UInt_t          getCorrelationFlag(Int_t i=0) const {return correlationFlags[i];}
  Int_t           getGeantTrackID(Int_t i=0) const {return correlatedTrackIds[i];}
  Int_t           getGeantPID(Int_t i=0) const {return GeantPIDs[i];}

  Int_t           getGeantParentID(Int_t i=0) const {return ParentIds[i];}
  Int_t           getGeantParentTrackID(Int_t i=0) const {return Parents[i];}


  Int_t           getGeantGrandParentID(Int_t i=0) const {return GrandParentIds[i];}
  Int_t           getGeantGrandParentTrackID(Int_t i=0) const {return GrandParents[i];}

  Float_t         getGenInfo(Int_t i=0) const {return GenInfo[i];}
  Float_t         getGenInfo1(Int_t i=0) const {return GenInfo1[i];}
  Float_t         getGenInfo2(Int_t i=0) const {return GenInfo2[i];}
  Float_t         getGenWeight(Int_t i=0) const {return GenWeight[i];}

  Float_t         getGeantVertexX(Int_t i=0) const {return VertexX[i];}
  Float_t         getGeantVertexY(Int_t i=0) const {return VertexY[i];}
  Float_t         getGeantVertexZ(Int_t i=0) const {return VertexZ[i];}

  Int_t           getGeantMediumID(Int_t i=0) const {return MediumIds[i];}
  Int_t           getGeantProcessID(Int_t i=0) const {return ProcessIds[i];}
  Float_t         getGeantMomX(Int_t i=0) const {return GeantMomX[i];}
  Float_t         getGeantMomY(Int_t i=0) const {return GeantMomY[i];}
  Float_t         getGeantMomZ(Int_t i=0) const {return GeantMomZ[i];}
  
  //Get momentum for the i-th particle that contributed
  Float_t         getTotalMomentum(Int_t i=0) const ;
  
  //get all weights for the i-th contributing particle
  /*
    Bool_t          getWeights(Float_t& weightRICHIPU, Float_t& weightRICH, Float_t& weightInnerMdc, Float_t& weightOuterMdc,
    Float_t& weightShower, Float_t& weightTOF,Int_t i=0);
    

  */

  Bool_t          getMomentumComponents(Float_t& momx, Float_t& momy,Float_t& momz, Int_t i=0);  
  
  
  //build the geant 3/4 momentum vector for the particle at position iPos
  TVector3* buildGeantMomentumVector(Int_t iPos=0) const ; 
  TLorentzVector* buildGeantLorentzVector(Int_t iPos=0) const;
  
  
 protected:    
   
   //These are helper functions to sort the arrays according to the degree of commonness of the found track ids!
   //void       sort(void);
   //void       sortverbose(void);

   void       sortsmart(void);
   void       bringCorrelatedDetectorsToFront();
   void       sortCorrelatedDetectors();
   void       sortSingleDetectors();
   Bool_t     isSingleDetector(UInt_t flag);
   
   //swap the content of array at positions i and j
   void       swap(Int_t i, Int_t j,TArrayF& array);
   void       swap(Int_t i, Int_t j,TArrayI& array);
   //void       swap(Int_t i, Int_t j,UInt_t* array);
   
   //call "swap" for all array
   void       swapVectors(Int_t i, Int_t j);


   void       diagnosis();                          // Check whether we have sorted all Geant particles such that the most
                                                    // common ones are at the beginning and the others follow in descending 
                                                    // degree of "commonness" 
   Int_t findFirstHitInTof(Int_t trackID,Int_t* count,Int_t storeFirstTrack=2);
 protected:
   
   Bool_t     isSorted;                             // indicates whether the contributing tracks have been sorted according to 
                                                    // their degree of "commonness"
   

   Short_t    sNCorrTrackIds;                       // How many different geant Ids where found in this set of matched hits ?

   // number of geant track ids in each detector
   UInt_t nRICHTracks; 
   UInt_t nRICHIPUTracks;
   UInt_t nInnerMdcTracks;
   UInt_t nOuterMdcTracks;
   UInt_t nShowerTracks;
   UInt_t nTOFTracks;

   TArrayI    correlatedTrackIds;//[sNCorrTrackIds] // A unique list of correlated Ids - size is fixed to avoid new delete calls!
   TArrayI    correlationFlags;//[sNCorrTrackIds]   // List of bit patterns storing which detectors saw the tracks in corrTrackIds 
   TArrayI    ProcessIds;//[sNCorrTrackIds]         // A list of the processes in which the correlated particles have been made

   TArrayI    ParentIds;//[sNCorrTrackIds    ]      // A list of the parent IDs of the correlated particles
   TArrayI    Parents;//[sNCorrTrackIds    ]        // A list of the parents track IDs of the correlated particles

   TArrayI    GrandParentIds;//[sNCorrTrackIds    ]      // A list of the grand parent IDs of the correlated particles
   TArrayI    GrandParents;//[sNCorrTrackIds    ]        // A list of the grand parents track IDs of the correlated particles

   TArrayF    GenInfo;//[sNCorrTrackIds    ]        
   TArrayF    GenInfo1;//[sNCorrTrackIds    ]
   TArrayF    GenInfo2;//[sNCorrTrackIds    ]
   TArrayF    GenWeight;//[sNCorrTrackIds    ]

   TArrayF    VertexX;//[sNCorrTrackIds]            // X,Y,Z Coordinate of the particles creation vertex
   TArrayF    VertexY;//[sNCorrTrackIds]
   TArrayF    VertexZ;//[sNCorrTrackIds]


   TArrayI    GeantPIDs;//[sNCorrTrackIds]          // A list of the Geant PIDs of the correlated particles
   TArrayI    MediumIds;//[sNCorrTrackIds]          // A list of the Medium IDs in which the correlated particles were created
   
   TArrayF    GeantMomX;//[sNCorrTrackIds]          // Arrays holding the three momentum-components of all involved particles
   TArrayF    GeantMomY;//[sNCorrTrackIds]
   TArrayF    GeantMomZ;//[sNCorrTrackIds]
   
   TArrayF    ShowerWeights;//[sNCorrTrackIds]      // Weights of all contributing particles to the hits that where reconstructed
   TArrayF    TOFWeights;//[sNCorrTrackIds]         // in the different subsystems
   TArrayF    RICHWeights;//[sNCorrTrackIds]
   TArrayF    RICHIPUWeights;//[sNCorrTrackIds]
   TArrayF    InnerMDCWeights;//[sNCorrTrackIds]
   TArrayF    OuterMDCWeights;//[sNCorrTrackIds]
   

   //If we are handling a hit in the META detector we add a flag for META and not for TOF/SHOWER in order to 
   //have one flag for the whole META system. We store the system additionally in the flags hasHitInShower, 
   //hasHitInTOF
   TArrayI    hasHitInShower;//[sNCorrTrackIds]     // Booleans indicating the systems (0,1) in which a contributing particle
   TArrayI    hasHitInTOF;//[sNCorrTrackIds]        // has been seen

 public:   
   Int_t      shrinkArrays(void);                   // Functions to dynamically increase/decrease the size of the array in order
   Int_t      enlargeArrays(void);                  // to adapt to the needs of each HPidGeantTrackSet. (Each instance can be
                                                    // composed of a different number of particles

   
   static Int_t nullparent;     

   // -------------------------------------------------------------------------
   
   ClassDef(HPidGeantTrackSet, 4)  // Geant tracks matched in HPidTrackCand
     };

#endif /* HPidGeantTrackSet_h */
