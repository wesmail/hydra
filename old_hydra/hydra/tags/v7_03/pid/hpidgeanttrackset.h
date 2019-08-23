// @(#)$Id: hpidgeanttrackset.h,v 1.5 2005-01-11 11:06:45 halo Exp $
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
// Geant tracks matched in HPidTrackCand                                      //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include <TObject.h>
#include "piddef.h"
#include <TArrayI.h>
#include <TArrayF.h>
#include <TLorentzVector.h>
#include <TVector3.h>
//#include "hpidcommontrack.h"
// -----------------------------------------------------------------------------


//#define DEBUGOUT(string) { if(debugoutput) cout << string << endl;} 

class HGeantKine;
class HCategory;
class HPidTrackCand;
class HMetaMatch;
//class HPidCommonTrack;


// -----------------------------------------------------------------------------

class HPidGeantTrackSet: public TObject
{
public:
  
   enum
    {
      kNoTrack       =  0,    // no track
      kNotFilled     = -1,    // this member was not filled
      kNoRefObj      = -2,    // no availabe input object with tracks
      kOutOfBound    = -3,    // error value when data out of bounds requested
    };



    // -------------------------------------------------------------------------

                HPidGeantTrackSet(void)             { reset();  }
		virtual    ~HPidGeantTrackSet(void)             { //might need to be implemented when arrays are used
		}

    void        reset(void);

    //void      Clear() --  might be required, if the deletion of dynamically allocated objects is required!
    // -------------------------------------------------------------------------

    Short_t     getRichTrack(UInt_t iIdx) ;     //return the track-Id at counter-position iIdx which contributed to the ring
    Short_t     getInnerMdcTrack(UInt_t iIdx);
    Short_t     getOuterMdcTrack(UInt_t iIdx);
    Short_t     getShowerTrack(UInt_t iIdx);
    Short_t     getTOFTrack(UInt_t iIdx);
    Short_t     getRichIPUTrack(UInt_t iIdx);
    Short_t     getMetaTrack(UInt_t iIdx);

    Float_t     getRichWeight(UInt_t iIdx) ;     //return the weight at counter-position iIdx which contributed to the ring
    Float_t     getInnerMdcWeight(UInt_t iIdx);
    Float_t     getOuterMdcWeight(UInt_t iIdx);
    Float_t     getShowerWeight(UInt_t iIdx);
    Float_t     getTOFWeight(UInt_t iIdx);
    Float_t     getRichIPUWeight(UInt_t iIdx);
    Float_t     getMetaWeight(UInt_t iIdx);


    // -------------------------------------------------------------------------

    UInt_t       getNRichTracks(void) const {return nRICHTracks;};
    UInt_t       getNRichIPUTracks(void) const  {return nRICHIPUTracks;};
    UInt_t       getNInnerMdcTracks(void) const {return nInnerMdcTracks;};
    UInt_t       getNOuterMdcTracks(void) const  {return nOuterMdcTracks;};
    UInt_t       getNShowerTracks(void) const {return nShowerTracks;};
    UInt_t       getNTOFTracks(void) const {return nTOFTracks;};
    UInt_t       getNMetaTracks(void) const;
    UInt_t       getNCorrelatedTrackIds(void) const {return sNCorrTrackIds;};

    void         fillFromMetaMatch(HMetaMatch* pMetaMatch);
    
    // -------------------------------------------------------------------------
    
    Short_t     getMostCommonCorrelation(){
      if (!isSorted) sort();
      return correlationFlags[0];
    }
    
    Int_t       addToContributingTracks(Int_t index, UInt_t detector); //appends a track ID to the list of geant tracks
    Bool_t      isPartOfTrackSet(Int_t trackID);                 //checks whether a track id is in this geant set
    Int_t       getParticleIndex(Int_t trackID);                       //finds the index (position) of the particle in the list
                                                                       //of contributing track ids
    
    void        addTrackWeightToDetector(Float_t weight, Int_t trackID, UInt_t detector); //Adds the weight of the track to the 
                                                                                          //list of weights in the detector

    Bool_t      wasSeenInDetector(const UInt_t flag, const UInt_t detector,Bool_t verbose=kFALSE) const;
    Bool_t      wasSeenInInnerMdc(const UInt_t flag) const {return flag & kSeenInInnerMDC;}
    Bool_t      wasSeenInOuterMdc(const UInt_t flag) const {return flag & kSeenInOuterMDC;}
    Bool_t      wasSeenInMeta(const UInt_t flag) const {return flag & kSeenInMETA;}
    Bool_t      wasSeenInTOF(const UInt_t flag) const {return flag & kSeenInTOF;}
    Bool_t      wasSeenInShower(const UInt_t flag) const {return flag & kSeenInShower;}
    Bool_t      wasSeenInRICH(const UInt_t flag) const {return flag & kSeenInRICH;}
    Bool_t      wasSeenInRICHIPU(const UInt_t flag) const {return flag & kSeenInRICHIPU;}
    // -------------------------------------------------------------------------
    
    void            Clear(Option_t *opt = "");          //{ reset();  }
    virtual void    Print(Option_t* option = "")       { print();     }
    
    void            print(void);
    
    // -------------------------------------------------------------------------
    
    static HGeantKine*  getGeantKine(Int_t iTrack, HCategory *pCat = NULL);
    
    // -------------------------------------------------------------------------
    UInt_t          getCorrelationFlag(Int_t i=0) {return correlationFlags[i];}
    Int_t           getGeantTrackID(Int_t i=0) {return correlatedTrackIds[i];}
    Int_t           getGeantPID(Int_t i=0) {return GeantPIDs[i];}
    Int_t           getGeantParentID(Int_t i=0) {return Parents[i];}
    Int_t           getGeantMediumID(Int_t i=0) {return MediumIds[i];}
    Int_t           getGeantProcessID(Int_t i=0) {return ProcessIds[i];}
    Float_t         getGeantMomX(Int_t i=0) {return GeantMomX[i];}
    Float_t         getGeantMomY(Int_t i=0) {return GeantMomY[i];}
    Float_t         getGeantMomZ(Int_t i=0) {return GeantMomZ[i];}
    
    Float_t         getTotalMomentum(Int_t i=0);

    Bool_t          getWeights(Float_t& weightRICHIPU, Float_t& weightRICH, Float_t& weightInnerMdc, Float_t& weightOuterMdc,
			       Float_t& weightShower, Float_t& weightTOF,Int_t i=0);

    Bool_t          getMomentumComponents(Float_t& momx, Float_t& momy,Float_t& momz, Int_t i=0);

    TVector3* buildGeantMomentumVector(Int_t iPos=0) const ; 
    TLorentzVector* buildGeantLorentzVector(Int_t iPos=0) const;


 protected:    

    //These are helper functions to sort the arrays according to the degree of commonness of the found track ids!
    void       sort(void);
    void       sortverbose(void);
    void       swap(Int_t i, Int_t j,TArrayF& array);
    void       swap(Int_t i, Int_t j,TArrayI& array);
    //void       swap(Int_t i, Int_t j,UInt_t* array);
    void       swapVectors(Int_t i, Int_t j);

    Bool_t     isSorted;                             // indicated whether the contributing tracks have been sorted according to 
    void       diagnosis();                          // their degree of "commonness"
    void       removeLonelyHits();

    Short_t    sNCorrTrackIds;                       // How many different geant Ids where found in this set of matched hits ?
    TArrayI    correlatedTrackIds;//[sNCorrTrackIds] // A unique list of correlated Ids - size is fixed to avoid new delete calls!
    TArrayI    correlationFlags;//[sNCorrTrackIds]   // List of bit patterns storing which detectors saw the tracks in corrTrackIds 
    TArrayI    ProcessIds;//[sNCorrTrackIds]         // A list of the processes in which the correlated particles have been made
    TArrayI    Parents;//[sNCorrTrackIds    ]        // A list of the parents of the correlated particles
    TArrayI    GeantPIDs;//[sNCorrTrackIds]          // A list of the Geant PIDs of the correlated particles
    TArrayI    MediumIds;//[sNCorrTrackIds]          // A list of the Medium IDs in which the correlated particles were created

    TArrayF    GeantMomX;//[sNCorrTrackIds]          // Arrays holding the three momentum-components of all involved particles
    TArrayF    GeantMomY;//[sNCorrTrackIds]
    TArrayF    GeantMomZ;//[sNCorrTrackIds]

    TArrayF    ShowerWeights;//[sNCorrTrackIds]
    TArrayF    TOFWeights;//[sNCorrTrackIds]
    TArrayF    RICHWeights;//[sNCorrTrackIds]
    TArrayF    RICHIPUWeights;//[sNCorrTrackIds]
    TArrayF    InnerMDCWeights;//[sNCorrTrackIds]
    TArrayF    OuterMDCWeights;//[sNCorrTrackIds]

    TArrayI    hasHitInShower;//[sNCorrTrackIds]
    TArrayI    hasHitInTOF;//[sNCorrTrackIds]

    Int_t      shrinkArrays(void);
    Int_t      enlargeArrays(void);

    // number of geant track ids in each detector
    UInt_t nRICHTracks; 
    UInt_t nRICHIPUTracks;
    UInt_t nInnerMdcTracks;
    UInt_t nOuterMdcTracks;
    UInt_t nShowerTracks;
    UInt_t nTOFTracks;

   

    // -------------------------------------------------------------------------

    ClassDef(HPidGeantTrackSet, 1)  // Geant tracks matched in HPidTrackCand
};

#endif /* HPidGeantTrackSet_h */
