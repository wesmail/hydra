//*************************************************************************
//
//  HGeantKine
//
//  GEANT KINE event data (GEANT data on primary and secondary tracks)
//
//  last modified on 23/03/2005 by R.Holzmann  (GSI)
//*************************************************************************
#ifndef HGEANTKINE_H
#define HGEANTKINE_H

#include <TMath.h>
#include <TObject.h>
#include "hades.h"
#include "hdataobject.h"
#include "hrecevent.h"
#include "hgeomvector.h"

class HLinkedDataObject;
class HCategory;
class HLinearCategory;

class HGeantKine : public TObject
{
private:
  Int_t richIndex;          //! internal index variables
  Int_t mdcIndex;           //! used to set up linked chains
  Int_t tofIndex;           //! of hit objects
  Int_t rpcIndex;           //! of hit objects
  Int_t showIndex;          //! 
  HCategory* pRich;         //! internal pointers to category
  HCategory* pMdc;          //! used to set up linked chains
  HCategory* pTof;          //! of hit objects
  HCategory* pRpc;          //! of hit objects
  HCategory* pShow;         //!

protected:
  Int_t trackNumber;        // GEANT track number
  Int_t parentTrack;        // GEANT track number of parent particle
  Int_t particleID;         // GEANT particle ID number
  Int_t mediumNumber;       // GEANT medium of creation number
  Int_t creationMechanism;  // GEANT creation mechanism number
  Float_t xVertex;          // x of track vertex  (in mm)
  Float_t yVertex;          // y
  Float_t zVertex;          // z
  Float_t xMom;             // x component of particle momentum (in MeV/c)
  Float_t yMom;             // y
  Float_t zMom;             // z
  Float_t generatorInfo;    // event generator info
  Float_t generatorInfo1;   // additional event generator info
  Float_t generatorInfo2;   // additional event generator info
  Float_t generatorWeight;  // associated weight
  Short_t firstRichHit;     // index of first hit in RICH category
  Short_t firstMdcHit;      // index of first hit in MDC category
  Short_t firstTofHit;      // index of first hit in TOF category
  Short_t firstRpcHit;      // index of first hit in RPC category
  Short_t firstShowerHit;   // index of first hit in SHOWER category
  Bool_t active;            // active flag (set if track participates in a hit)
  Bool_t suppressed;        // flag used to filter out particle hits before digitization 

public:
  HGeantKine(void);
  HGeantKine(HGeantKine &aKine);
  ~HGeantKine(void);
  void setParticle(Int_t aTrack, Int_t aID);
  void setCreator(Int_t aPar, Int_t aMed, Int_t aMech);
  void setVertex(Float_t ax, Float_t ay, Float_t az);
  void setMomentum(Float_t apx, Float_t apy, Float_t apz);
  void setGenerator(Float_t aInfo, Float_t aWeight);
  void setGenerator(Float_t aInfo, Float_t aInfo1, Float_t aInfo2);
  void setWeight(Float_t aWeight) {generatorWeight = aWeight;}
  void getParticle(Int_t &aTrack, Int_t &aID);
  void getCreator(Int_t &aPar, Int_t &aMed, Int_t &aMech);
  void getVertex(Float_t &ax, Float_t &ay, Float_t &az);
  void getMomentum(Float_t &apx, Float_t &apy, Float_t &apz);
  inline Int_t getTrack(void) const { return trackNumber; }
  inline Int_t getID(void) const { return particleID; }
  inline Int_t getParentTrack(void) const { return parentTrack; }
  inline void setParentTrack(Int_t track) { parentTrack = track; }
  void setNewTrackNumber(Int_t track);
  Float_t getTotalMomentum(void) const {
    return sqrt(xMom*xMom + yMom*yMom + zMom*zMom);
  }
  Float_t getTransverseMomentum(void) const {
    return sqrt(xMom*xMom + yMom*yMom);
  }

// Enabling the following usefull functions creates a dependency with PID code
// (-> move HPidPhysicsConstants stuff e.g. to base/util/HPhysicsConstants) 
  /*
  Float_t getM(void) const {return HPidPhysicsConstants::mass(particleID);}
  Float_t getE(void) const (return sqrt(getM()*getM() + xMom*xMom + yMom*yMom + zMom*zMom);}
  Float_t getEkin(void) const {return getE()-getM();}
  Float_t getRapidity(void) const{
    return 0.5*TMath::Log((getE()+zMom)/(getE()-zMom));
  }
  */

  void getMomentum(HGeomVector &v) { v.setXYZ(xMom, yMom, zMom); }
  void getGenerator(Float_t &aInfo, Float_t &aWeight);
  void getGenerator(Float_t &aInfo, Float_t &aInfo1, Float_t &aInfo2);
  inline void setActive(Bool_t flag=kTRUE) {active=flag;}
  inline void setSuppressed(Bool_t flag=kTRUE) {suppressed=flag;}
  inline Bool_t isActive(void) const {return active;}
  inline Bool_t isPrimary(void) const {return (parentTrack==0);}
  inline Bool_t isSuppressed(void) const {return suppressed;}
  Bool_t hasAncestor(Int_t track, HLinearCategory* cat=NULL);

  static Bool_t setChainActive(Int_t track, Bool_t flag=kTRUE, HLinearCategory* cat=NULL);
  static Bool_t setAllDescendentsActive(Int_t track, Bool_t flag=kTRUE, HLinearCategory* cat=NULL);
  static Bool_t setAllDescendentsSuppressed(Int_t track, Bool_t flag=kTRUE, HLinearCategory* cat=NULL);
  static HGeantKine* getParent(Int_t track, HLinearCategory* cat=NULL);
  static HGeantKine* getGrandParent(Int_t track, HLinearCategory* cat=NULL);
  static HGeantKine* getPrimary(Int_t track, HLinearCategory* cat=NULL);
  static HGeantKine* getCommonAncestor(Int_t track1, Int_t track2, HLinearCategory* cat=NULL);
  static Bool_t suppressTracks(Int_t id, Float_t acceptedFraction, HLinearCategory* cat=NULL);

  inline Int_t setRichHitIndex(Int_t index);
  inline Int_t setMdcHitIndex(Int_t index);
  inline Int_t setTofHitIndex(Int_t index);
  inline Int_t setRpcHitIndex(Int_t index);
  inline Int_t setShowerHitIndex(Int_t index);

  Int_t getNRichHits(void);  // return number of hits in list
  Int_t getFirstRichHit() {return firstRichHit;}
  Int_t getNMdcHits(void);
  Int_t getFirstMdcHit() {return firstMdcHit;}
  Int_t getNMdcHits(Int_t module);
  Int_t getNTofHits(void);
  Int_t getFirstTofHit() {return firstTofHit;}
  Int_t getNRpcHits(void);
  Int_t getFirstRpcHit() {return firstRpcHit;}
  Int_t getNShowerHits(void);
  Int_t getFirstShowerHit() {return firstShowerHit;}

  void sortRichHits(void);  // sort hits in list
  void sortMdcHits(void);
  void sortTofHits(void);
  void sortRpcHits(void);
  void sortShowerHits(void);

  Int_t getSystem(void); 
  Int_t getSector(void); 
  UInt_t getMdcSectorBitArray(void);
  UInt_t getMdcSectorDecimalArray(void); 
  UInt_t getShowerSectorBitArray(void); 
  UInt_t getShowerSectorDecimalArray(void); 
  UInt_t getTofSectorBitArray(void); 
  UInt_t getTofSectorDecimalArray(void); 
  UInt_t getRpcSectorBitArray(void); 
  UInt_t getRpcSectorDecimalArray(void); 
  UInt_t getRichSectorBitArray(void); 
  UInt_t getRichSectorDecimalArray(void);
  UInt_t getSectorBitArray(void);
  UInt_t getSectorDecimalArray(void);

  inline HLinkedDataObject* nextRichHit();
  inline HLinkedDataObject* nextMdcHit();
  inline HLinkedDataObject* nextTofHit();
  inline HLinkedDataObject* nextRpcHit();
  inline HLinkedDataObject* nextShowerHit();

  // The following 4 are needed if the categories are created outside the HYDRA event
  // and are hence not accessible via HRecEvent::getCategory(cat), e.g. in a macro.
  inline void setRichCategory(HCategory* p) {pRich = p;} 
  inline void setMdcCategory(HCategory* p) {pMdc = p;}   
  inline void setTofCategory(HCategory* p) {pTof = p;}
  inline void setRpcCategory(HCategory* p) {pRpc = p;}
  inline void setShowerCategory(HCategory* p) {pShow = p;}

  inline void resetRichIter(void);
  inline void resetMdcIter(void);
  inline void resetTofIter(void);
  inline void resetRpcIter(void);
  inline void resetShowerIter(void);

private:
  Int_t setHitIndex(HCategory* p, Short_t& first, Int_t index);
  HLinkedDataObject* nextHit(HCategory* p, Int_t& next);
   
  ClassDef(HGeantKine,6) // GEANT KINE data class
};

//------------------inlines----------------------------------
//----------------------hit index----------------------------
  inline Int_t HGeantKine::setRichHitIndex(Int_t index) {
  // set next RICH photon hit index in linked list
    resetRichIter();
    return setHitIndex(pRich,firstRichHit,index);
  }

  inline Int_t HGeantKine::setMdcHitIndex(Int_t index) {
  // set next MDC hit index in linked list
    resetMdcIter();
    return setHitIndex(pMdc,firstMdcHit,index);
  }

  inline Int_t HGeantKine::setTofHitIndex(Int_t index) {
  // set next TOF hit index in linked list
    resetTofIter();
    return setHitIndex(pTof,firstTofHit,index);
  }

  inline Int_t HGeantKine::setRpcHitIndex(Int_t index) {
  // set next RPC hit index in linked list
    resetRpcIter();
    return setHitIndex(pRpc,firstRpcHit,index);
  }

  inline Int_t HGeantKine::setShowerHitIndex(Int_t index) {
  // set next SHOWER hit index in linked list
    resetShowerIter();
    return setHitIndex(pShow, firstShowerHit,index);
  }

//------------------------linked data objects--------------------
  inline HLinkedDataObject* HGeantKine::nextRichHit() {
  // return next RICH photon hit made by present track
     return nextHit(pRich,richIndex);
  }

  inline HLinkedDataObject* HGeantKine::nextMdcHit() {
  // return next MDC hit made by present track
     return nextHit(pMdc,mdcIndex);
  }

  inline HLinkedDataObject* HGeantKine::nextTofHit() {
  // return next TOF hit made by present track
     return nextHit(pTof,tofIndex);
  }

  inline HLinkedDataObject* HGeantKine::nextRpcHit() {
  // return next RPC hit made by present track
     return nextHit(pRpc,rpcIndex);
  }

  inline HLinkedDataObject* HGeantKine::nextShowerHit() {
  // return next SHOWER hit made by present track
     return nextHit(pShow,showIndex);
  }

//----------------------iterators--------------------------------
  inline void HGeantKine::resetRichIter(void) {  // reset list iterator
     richIndex = (Int_t)firstRichHit;
     if(!pRich) pRich = ((HRecEvent*)gHades->getCurrentEvent())
                      ->getCategory(catRichGeantRaw);
  }
  inline void HGeantKine::resetMdcIter(void) {
     mdcIndex = (Int_t)firstMdcHit;
     if(!pMdc) pMdc = ((HRecEvent*)gHades->getCurrentEvent())
                    ->getCategory(catMdcGeantRaw);
  }
  inline void HGeantKine::resetTofIter(void) {
     tofIndex = (Int_t)firstTofHit;
     if(!pTof) pTof = ((HRecEvent*)gHades->getCurrentEvent())
                    ->getCategory(catTofGeantRaw);
  }
 
  inline void HGeantKine::resetRpcIter(void) {
     rpcIndex = (Int_t)firstRpcHit;
     if(!pRpc) pRpc = ((HRecEvent*)gHades->getCurrentEvent())
                    ->getCategory(catRpcGeantRaw);
  }

  inline void HGeantKine::resetShowerIter(void) {
     showIndex = (Int_t)firstShowerHit;
     if(!pShow) pShow = ((HRecEvent*)gHades->getCurrentEvent())
                      ->getCategory(catShowerGeantRaw);
  }

#endif  /*! HGEANTKINE_H */









