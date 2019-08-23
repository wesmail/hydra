///////////////////////////////////////////////////////////////////////////
//  HGeantKine
//
//  GEANT KINE event data
//
//  last modified on 1/06/2000 by R.Holzmann  (GSI)
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTKINE_H
#define HGEANTKINE_H

#include <hades.h>
#include <hdataobject.h>
#include <hrecevent.h>

class HLinkedDataObject;
class HCategory;

class HGeantKine : public HDataObject
{
private:
  Int_t richIndex;          //! internal index variable
  Int_t mdcIndex;           //! 
  Int_t tofIndex;           //! 
  Int_t showIndex;          //! 
  HCategory* pRich;         //! internal pointer to category
  HCategory* pMdc;          //!
  HCategory* pTof;          //!
  HCategory* pShow;         //!

protected:
  Int_t trackNumber;        // GEANT track number
  Int_t parentTrack;        // track number of parent particle
  Int_t particleID;         // GEANT particle ID number
  Int_t mediumNumber;       // GEANT medium of creation number
  Int_t creationMechanism;  // GEANT creation mechanism number
  Float_t xVertex;          // x of track vertex  (in cm)
  Float_t yVertex;          // y
  Float_t zVertex;          // z
  Float_t xMom;             // x component of particle momentum (in MeV/c)
  Float_t yMom;             // y
  Float_t zMom;             // z
  Float_t generatorInfo;    // event generator info
  Float_t generatorWeight;  // associated weight 
  Short_t firstRichHit;     // index of first hit in RICH category
  Short_t firstMdcHit;      // index of first hit in MDC category
  Short_t firstTofHit;      // index of first hit in TOF category
  Short_t firstShowerHit;   // index of first hit in SHOWER category

public:
  HGeantKine(void);
  HGeantKine(HGeantKine &aKine);
  ~HGeantKine(void);
  void setParticle(Int_t aTrack, Int_t aID);
  void setCreator(Int_t aPar, Int_t aMed, Int_t aMech);
  void setVertex(Float_t ax, Float_t ay, Float_t az);
  void setMomentum(Float_t apx, Float_t apy, Float_t apz);
  void setGenerator(Float_t aInfo, Float_t aWeight);
  void getParticle(Int_t &aTrack, Int_t &aID);
  void getCreator(Int_t &aPar, Int_t &aMed, Int_t &aMech);
  void getVertex(Float_t &ax, Float_t &ay, Float_t &az);
  void getMomentum(Float_t &apx, Float_t &apy, Float_t &apz);
  Float_t getTotalMomentum(void) const {
    return sqrt(xMom*xMom + yMom*yMom + zMom*zMom);
  }
  void getGenerator(Float_t &aInfo, Float_t &aWeight);

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

  inline Int_t HGeantKine::setShowerHitIndex(Int_t index) {
  // set next SHOWER hit index in linked list
    resetShowerIter();
    return setHitIndex(pShow, firstShowerHit,index);
  }

  Int_t getNRichHits(void);  // return number of hits in list
  Int_t getNMdcHits(void);
  Int_t getNTofHits(void);
  Int_t getNShowerHits(void);

  void sortRichHits(void);  // sort hits in list
  void sortMdcHits(void);
  void sortTofHits(void);
  void sortShowerHits(void);

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

  inline HLinkedDataObject* HGeantKine::nextShowerHit() {
  // return next SHOWER hit made by present track
     return nextHit(pShow,showIndex);
  }

  // The following 4 are needed if the categories are created outside the event
  // and are therefore not accessible via HRecEvent::getCategory(cat), e.g. in
  // a macro.
  inline void setRichCategory(HCategory* p) {pRich = p;} 
  inline void setMdcCategory(HCategory* p) {pMdc = p;}   
  inline void setTofCategory(HCategory* p) {pTof = p;}
  inline void setShowerCategory(HCategory* p) {pShow = p;}

  inline void resetRichIter(void) {  // reset list iterator
     richIndex = (Int_t)firstRichHit;
     if(!pRich) pRich = ((HRecEvent*)gHades->getCurrentEvent())
                      ->getCategory(catRichGeantRaw);
  }
  inline void resetMdcIter(void) {
     mdcIndex = (Int_t)firstMdcHit;
     if(!pMdc) pMdc = ((HRecEvent*)gHades->getCurrentEvent())
                    ->getCategory(catMdcGeantRaw);
  }
  inline void resetTofIter(void) {
     tofIndex = (Int_t)firstTofHit;
     if(!pTof) pTof = ((HRecEvent*)gHades->getCurrentEvent())
                    ->getCategory(catTofGeantRaw);
  }
  inline void resetShowerIter(void) {
     showIndex = (Int_t)firstShowerHit;
     if(!pShow) pShow = ((HRecEvent*)gHades->getCurrentEvent())
                      ->getCategory(catShowerGeantRaw);
  }

private:
  Int_t setHitIndex(HCategory* p, Short_t& first, Int_t index);
  HLinkedDataObject* nextHit(HCategory* p, Int_t& next);
   
  ClassDef(HGeantKine,2) // Geant KINE event data class
};

#endif  /*! HGEANTKINE_H */









