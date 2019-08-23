//*-- $Id: hpidtrackdata.h,v 1.20 2007-08-07 06:59:11 kempter Exp $
//*-- Modified: Stefano Spataro 21/06/2005
//           - bIsAccepted included
//           - TofRec data members included 
//*-- Modified: Stefano Spataro 11/07/2005
//           - getBeta(nAlg),getMass2(nAlg) included, in order to have the correct beta/mass2 value according to tofrec flag
//*-- Modified: Stefano Spataro 05/08/2005 - Path length included (new classdef)
//*-- Modified: Stefano Spataro 22/08/2005 - Covariance matrix included (new classdef)
//*-- Modified: Anar Rustamov 08/11/2005 - Matching quality is added 
//*-- Modified: Tassilo Christ 05/02/2005 - Code documentation upgraded
#ifndef HPIDTRACKDATA_H
#define HPIDTRACKDATA_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HPidTrackData                                                        //
//                                                                      //
// A container of track data.                                           //
// Holding all indformation about results from different tracking       //
// algorithms. See documentation of individual variables                //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TObject.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "TError.h"
#include "piddef.h"
#include "hsymmat.h"

#include <math.h>
// -----------------------------------------------------------------------------

class HBaseTrack;
class HMetaMatch;

// -----------------------------------------------------------------------------


// -----------------------------------------------------------------------------
class HPidTrackData : public TObject
{
 protected:
  HSymMat6 cov[MAX_MOM_ALGS];     // covariance matrix array of size 6
 public:
  
  HPidTrackData(void);
  HPidTrackData(const HPidTrackData& source);
  //Indices of the track objects in the respective hydra categories from which this object was filled

  Int_t       nBestMomAlg;        // Which was the most sophisticated successful tracking method

  Short_t     nRKTrackInd;        // Index of track-object from runge-kutta algorithm: Index in the category
  Short_t     nKickTrackInd;      // Index of the track-object from the Kick Plane: Index in the category
  Short_t     nKickTrack123Ind;   // Index of the track-object from the 3-chamber-Kick Plane: Index in the category - not used 
  Short_t     nRefTrackInd;       // Index of reference track-object: Index in the category - not used
  Short_t     nSplineTrackInd;    // Ind of spline track-object: Index in the category

  Float_t     fMetaMatchingQuality;      //How good did the Meta hit match the track candidate?
  Float_t     fRKRichMatchingQuality;    //How good did the Rich hit match the RK track candidate?

  Float_t     dxRkMeta;           //XYZ coordinate difference of the Meta hit with shifted RK track-segment
  Float_t     dyRkMeta;  
  Float_t     dzRkMeta;  

  Float_t     dxMdcMeta;          //XYZ coordinate difference of the Meta hit with original track-segment
  Float_t     dyMdcMeta;  
  Float_t     dzMdcMeta;  


  Float_t     xMeta, yMeta, zMeta;           //xyz coordintes of hit in META
  Float_t     errXMeta, errYMeta, errZMeta;  //errors in xyz coordintes of hit in META

  Int_t       nCloseTracklets;                    //Number of tracklets closer than <x> degrees to this pidtrackacandidates

  Float_t     fPull, fSplineChiSquare,fRKChiSquare;    //Quality parameters of KickPlane, Spline and RK-tracking

  mutable Int_t iIndexClosestTracklet;  // After a call of getAngleWithClosestCandidate() this variable is used by getTrackletClusInf() to get the related cluster information objects
  TArrayI     aTrackletClusInf0;  //[nCloseTracklets]  // Array of indices to first cluster information object of nearby tracklets.
  TArrayI     aTrackletClusInf1;  //[nCloseTracklets]  // Array of indices to second cluster information object of nearby tracklets. 
  TArrayF     aTrackletDistances;//[nCloseTracklets]       // Array of Distances of inner MDC segments in the neighbourhood of the 
                                                          // current particle's segment
                                                          // Encodes distance, presence of ring and fit-results


  //These variables depend on the method of momentum reconstruction - there is one value per method - thus we have an array
  //with one field per algorithm to store the data

  Float_t getAngleWithClosestCandidate(Int_t selectLeptons, Int_t selectFitted) const;
  Float_t getAngleWithClosestHadronCandidate(void) const;
  Float_t getAngleWithClosestLeptonCandidate(void) const;
  
  //void    sortTracklets(void);

  Bool_t  closestCandidateIsFitted(Int_t selectLeptons) const;
  Bool_t  closestHadronCandidateIsFitted(void) const;
  Bool_t  closestLeptonCandidateIsFitted(void) const;
  
  Float_t     qIOMatching[MAX_MOM_ALGS]; //Matching quality of inner and outer segment

  Short_t     nPolarity[MAX_MOM_ALGS];          // pos/neg paricle?

  Float_t     fMomenta[MAX_MOM_ALGS];  //Stores the reconstructed momentum for each algorithm
  Float_t     fMomError[MAX_MOM_ALGS]; //Stores the error of the reconstructed momentum for each algorithm - not stored at present!!
  Float_t     fTrackR[MAX_MOM_ALGS];   //Stores R (radial distance at default z-coordinate for this track) for each algorithm
  Float_t     fTrackZ[MAX_MOM_ALGS];   //Stores Z (default z-coord for this track) for each algorithm

  Float_t     fRKPhi, fRKTheta; //Segment variables modified by RK tracking;

  Float_t     fCorrectedEloss[MAX_MOM_ALGS]; // Energyloss as computed AFTER mom reconstruction; (from base track object)
  Float_t     fCorrectedBeta[MAX_MOM_ALGS];  // Velocity as computed AFTER length of track is known from mom reconstruction

  Float_t     fPathLength[MAX_MOM_ALGS];     // Path length of the track
			    
  Float_t     fMassSquared[MAX_MOM_ALGS];    // Reconstructed Mass square values;

  Bool_t      bIsAccepted[MAX_MOM_ALGS];     // Stores whether the tracking method did or did not accept the track	 


  // These variables are used by the HPidTofRec start-time reconstructor
  Short_t     nTofRecFlag[MAX_MOM_ALGS];         // TofRec flag : -1: unchanged Tof (TofRec failed) 
                                                 //                0: unchanged tof (no HTofRec) 
                                                 //                1: lepton id / 2: pi- id 
  Float_t     fTofRecTof[MAX_MOM_ALGS];          // Reconstructed time-of-flight
  Float_t     fTofRecBeta[MAX_MOM_ALGS];         // Reconstructed beta
  Float_t     fTofRecMassSquared[MAX_MOM_ALGS];  // Reconstructed Mass square values 
  
  //Bool_t isSorted;

  //Function to get the correct beta according to tofrec flag. If the start time finder was running the reconstructed TOF is returned
  //otherwise the measured one is returned
  Float_t     getBeta(Int_t nAlg) const
    { 
      if (nTofRecFlag[nAlg]==0) return fCorrectedBeta[nAlg]; 
      else return fTofRecBeta[nAlg];
    } 

  //Function to get the correct mass2 according to tofrec flag. Same condition as in getBeta()
  Float_t     getMass2(Int_t nAlg) const
    { 
      if (nTofRecFlag[nAlg]==0) return fMassSquared[nAlg]; 
      else return fTofRecMassSquared[nAlg];} 


  //getters and setters will not be documented - use human reason
  Float_t     getPathLength(Int_t nAlg) const { return fPathLength[nAlg];}

  HSymMat     &getCovariance(Int_t nAlg) { return cov[nAlg]; } 

  Float_t     getIOMatching(Int_t nAlg) const {return qIOMatching[nAlg];}
   // -------------------------------------------------------------------------

  //Reset the object's data members
  virtual void reset(Option_t * /*option*/ = ""); 
  virtual void Clear(Option_t * /*option*/ = ""); 

  // -------------------------------------------------------------------------
  
  //report the object's data members
  void            print(void) const;
  void            Print(Option_t *o = "") const { print();                   }
  /*
  //Flag object as bad - not usable
  Bool_t          isBad(void) const         { return TestBit(kIsBad);        }
  void            setBad(Bool_t b = kTRUE)  { SetBit(kIsBad, b);             }
  
  //Flag object as filtered by some condition of the analysis
  Bool_t          isFiltered(void) const       { return TestBit(kIsFiltered);}
  void            setFiltered(Bool_t b = kTRUE){ SetBit(kIsFiltered, b);     }
  */

  
  // -------------------------------------------------------------------------
  //Find the base-track object which holds the information stored in the corresponding 
  //field of the arrays
  HBaseTrack*     getBaseTrack(Int_t momAlgIndex) const;

  //Essential function to copy all relevant information from base track to the data-arrays
  void                  fillFromBaseTrack(HBaseTrack* pBaseTrack, Int_t momalg);

  //copy information from a collection of indices (HMetaMatch) and all its indexed hits and tracks to this object
  void                  fillFromMetaMatch(HMetaMatch* pMetaMatch);

  //Append to the list of close by neighbours another segment;
  void                  appendCloseTracklet(Float_t distance, Int_t clusinf_index_0, Int_t clusinf_index_1);

  //copy information from an existing HPidTrackData object to another one. Used to create a self-describing
  // -------------------------------------------------------------------------

  //GETTERS:
  Int_t   getBestMomAlg() const {return nBestMomAlg;}       
  Short_t getRKTrackInd() const {return nRKTrackInd;}       
  Short_t getKickTrackInd() const {return nKickTrackInd;}     
  Short_t getKickTrack123Ind() const {return nKickTrack123Ind;}  
  Short_t getRefTrackInd() const {return nRefTrackInd;}      
  Short_t getSplineTrackInd() const {return nSplineTrackInd;}   

  Float_t getMetaMatchingQuality() const {return fMetaMatchingQuality;}     

  //UShort_t getInnerMdcNumber() const {return unInnerMdcNumber;}
  Float_t getPull() const {return fPull;}
  Float_t getSplineChiSquare() const {return fSplineChiSquare;}
  Float_t getRKChiSquare() const {return fRKChiSquare;}   

  Int_t   getNCloseTracklets(void) const {return nCloseTracklets;}     
  Float_t getTrackletDistance(Int_t index) const 
    {
      if(index>=getNCloseTracklets() || index<0)
	{
	  Error("HPidTrackData::getTrackletDistance(Int_t index)","index %d out of bounds",index);
	  return -1.0;
	}
      return aTrackletDistances[index];
    }  
  Int_t getTrackletClusInf(Int_t module, Int_t index = -1) const;
  Int_t getIndexClosestTracklet() const {return iIndexClosestTracklet;}

  Short_t getPolarity(Int_t index) const {return nPolarity[index];}         
  Float_t getMomenta(Int_t index) const {return fMomenta[index];} 
  Float_t getMomError(Int_t index) const {return fMomError[index];}
  Float_t getTrackR(Int_t index) const {return fTrackR[index];}
  Float_t getTrackZ(Int_t index) const {return fTrackZ[index];}
  Float_t getCorrectedEloss(Int_t index) const {return fCorrectedEloss[index];}
  Float_t getCorrectedBeta(Int_t index) const {return fCorrectedBeta[index];} 

  Bool_t  getIsAccepted(Int_t index) const {return bIsAccepted[index];}   
  Short_t getTofRecFlag(Int_t index) const {return nTofRecFlag[index];}         
  Float_t getTofRecTof(Int_t index) const {return fTofRecTof[index];}         
  Float_t getTofRecBeta(Int_t index) const {return fTofRecBeta[index];}        
  Float_t getTofRecMassSquared(Int_t index) const {return fTofRecMassSquared[index];}
  Float_t getMassSquared(Int_t index) const {return fMassSquared[index];}

  //Float_t getRK_R(void); const {return fRK_R;}
  //Float_t getRK_Z(void); const {return fRK_Z;}
  Float_t getRKTheta(void) const {return fRKTheta;}
  Float_t getRKPhi(void) const {return fRKPhi;}

  Float_t     getRKMetaMatchingQuality() const {

      return dxRkMeta != -1. && errXMeta != -1. ? sqrt(((dxRkMeta * dxRkMeta) / (errXMeta * errXMeta)) +
						       ((dyRkMeta * dyRkMeta) / (errYMeta * errYMeta)) +
						       ((dzRkMeta * dzRkMeta) / (errZMeta * errZMeta)) ) : -1. ;
  }
  Float_t     getRKRichMatchingQuality() const {return fRKRichMatchingQuality;}

  Float_t     getRkMetadx(void) const {return dxRkMeta;}    //XYZ coordinate difference of the Meta hit with shifted RK track-segment
  Float_t     getRkMetady(void) const {return dyRkMeta;}
  Float_t     getRkMetadz(void) const {return dzRkMeta;}

  Float_t     getMdcMetadx(void) const {return dxMdcMeta;}    //XYZ coordinate difference of the Meta hit with shifted RK track-segment
  Float_t     getMdcMetady(void) const {return dyMdcMeta;}
  Float_t     getMdcMetadz(void) const {return dzMdcMeta;}

  Float_t     getxMeta(void) const {return xMeta;}
  Float_t     getyMeta(void) const {return yMeta;}
  Float_t     getzMeta(void) const {return zMeta;}

  Float_t     geterrXMeta(void) const {return errXMeta;}
  Float_t     geterrYMeta(void) const {return errYMeta;}
  Float_t     geterrZMeta(void) const {return errZMeta;}

  //SETTERS:
  void setBestMomAlg(Int_t o_BestMomAlg)  {nBestMomAlg = o_BestMomAlg;}       
  void setRKTrackInd(Short_t o_RKTrackInd)  {nRKTrackInd = o_RKTrackInd;}       
  void setKickTrackInd(Short_t o_KickTrackInd){nKickTrackInd = o_KickTrackInd;}     
  void setKickTrack123Ind(Short_t o_KickTrack123Ind){nKickTrack123Ind = o_KickTrack123Ind;}  
  void setRefTrackInd(Short_t o_RefTrackInd){nRefTrackInd = o_RefTrackInd;}      
  void setSplineTrackInd(Short_t o_SplineTrackInd){nSplineTrackInd = o_SplineTrackInd;}   

  void setMetaMatchingQuality(Float_t o_MetaMatchingQuality)
    {fMetaMatchingQuality = o_MetaMatchingQuality;}     
  

  //void setInnerMdcNumber(UShort_t o_InnerMdcNumber){unInnerMdcNumber = o_InnerMdcNumber;}              

  void setPull(Float_t o_Pull){fPull = o_Pull;}
  void setSplineChiSquare(Float_t o_SplineChiSquare){fSplineChiSquare = o_SplineChiSquare;}
  void setRKChiSquare(Float_t o_RKChiSquare){fRKChiSquare = o_RKChiSquare;}

  //void setInnerMdcId(Int_t index, Short_t o_InnerMdcId) {aInnerMdcIds[index] = o_InnerMdcId;}     
  //void setInnerMdcCount(Int_t index, Short_t o_InnerMdcCount) {aInnerMdcCounts[index] = o_InnerMdcCount;}

  void setIOMatching(Int_t index,Float_t o_qIOMatching) {qIOMatching[index]=o_qIOMatching;}

  void setPolarity(Int_t index, Short_t o_Polarity) {nPolarity[index] = o_Polarity;}         
  void setMomenta(Int_t index, Float_t o_Momentum) {fMomenta[index] = o_Momentum;} 
  void setMomError(Int_t index, Float_t o_MomError) {fMomError[index] = o_MomError;}
  void setTrackR(Int_t index, Float_t o_TrackR) {fTrackR[index] = o_TrackR;}
  void setTrackZ(Int_t index, Float_t o_TrackZ) {fTrackZ[index] = o_TrackZ;}
  void setCorrectedEloss(Int_t index, Float_t o_CorrectedEloss) 
    {fCorrectedEloss[index] = o_CorrectedEloss;}
  void setCorrectedBeta(Int_t index, Float_t o_CorrectedBeta) {fCorrectedBeta[index] = o_CorrectedBeta;} 
  
  void setPathLength(Int_t index,Float_t o_PathLength) {fPathLength[index] = o_PathLength;}    
  void setMass2(Int_t index,Float_t o_Mass2) 
    {
      if(nTofRecFlag[index]==0)
	{
	  fMassSquared[index]=o_Mass2;
	}   
      else
	{
	  fTofRecMassSquared[index]=o_Mass2;
	}
    }
  void setIsAccepted(Int_t index, Bool_t o_IsAccepted) {bIsAccepted[index] = o_IsAccepted;}   
  void setTofRecFlag(Int_t index, Short_t o_TofRecFlag) {nTofRecFlag[index] = o_TofRecFlag;}         
  void setTofRecTof(Int_t index, Float_t o_TofRecTof) {fTofRecTof[index] = o_TofRecTof;}         
  void setBeta(Int_t index,Float_t o_Beta)
    {
      if(nTofRecFlag[index]==0)
	{
	  fCorrectedBeta[index]=o_Beta;
	}
      else
	{
	  fTofRecBeta[index]=o_Beta;
	}
    }

  
  //void setRK_R(Float_t o_fRK_R); const {fRK_R = o_fRK_R;}
  //void setRK_Z(Float_t o_fRK_Z); const {fRK_Z = o_fRK_Z;}
  void setRKTheta(Float_t o_fRKTheta) {fRKTheta = o_fRKTheta;}
  void setRKPhi(Float_t o_fRKPhi) {fRKPhi = o_fRKPhi;}

  void setRKRichMatchingQuality(Float_t qua) { fRKRichMatchingQuality = qua;}
  void setRkMetadx(Float_t o_dxRkMeta){dxRkMeta = o_dxRkMeta;}  
  void setRkMetady(Float_t o_dyRkMeta){dyRkMeta = o_dyRkMeta;}
  void setRkMetadz(Float_t o_dzRkMeta){dzRkMeta = o_dzRkMeta;}

  void setMdcMetadx(Float_t o_dxMdcMeta){dxMdcMeta = o_dxMdcMeta;}  
  void setMdcMetady(Float_t o_dyMdcMeta){dyMdcMeta = o_dyMdcMeta;}
  void setMdcMetadz(Float_t o_dzMdcMeta){dzMdcMeta = o_dzMdcMeta;}

  void     setxMeta(Float_t o_xMeta) {xMeta = o_xMeta;}
  void     setyMeta(Float_t o_yMeta) {yMeta = o_yMeta;}
  void     setzMeta(Float_t o_zMeta) {zMeta = o_zMeta;}

  void     seterrXMeta(Float_t o_errXMeta) {errXMeta = o_errXMeta;}
  void     seterrYMeta(Float_t o_errYMeta) {errXMeta = o_errYMeta;}
  void     seterrZMeta(Float_t o_errZMeta) {errZMeta = o_errZMeta;}



  
  ClassDef(HPidTrackData, 13)  // Track data for PID

};
    
#endif //HPIDTRACKDATA_H
