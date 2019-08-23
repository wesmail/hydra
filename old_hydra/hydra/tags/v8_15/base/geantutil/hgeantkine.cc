#include "hgeantkine.h"
#include "hlinearcategory.h"
#include "hlinkeddataobject.h"
#include "hgeantmdc.h"
#include "hgeanttof.h"
#include "hgeantrich.h"
#include "hgeantshower.h"
#include "hgeantrpc.h"
#include "TRandom.h"

//*-- Author : Romain Holzmann, GSI
//*-- Modified : 23/03/05 by Romain Holzmann
//*-- Modified : 05/04/04 by Romain Holzmann
//*-- Modified : 03/11/03 by Romain Holzmann
//*-- Modified : 09/10/03 by Peter Zumbruch
//*-- Modified : 06/10/03 by Romain Holzmann
//*-- Modified : 14/12/99 by Romain Holzmann
//*-- Copyright : GSI, Darmstadt
//
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////////
//
//  HGeantKine
// 
//  GEANT KINE data on primary and secondary particles in the simulated event
//
//***************************************************************************
//
// Inline functions:
//
//   void setWeight(Float_t aWeight)          Set weight of track
//   Int_t getTrack() const                   Get GEANT track number
//   Int_t getID() const                      Get GEANT particle id
//   Int_t getParentTrack() const             Get parent track number
//   void setParentTrack(Int_t track)         Set parent track number
//   Float_t getTotalMomentum() const         Get particle momentum (Ptot)
//   Float_t getTransverseMomentum() const    Get transverse momentum (Pt)
//   void getMomentum(HGeomVector& v)         Get momentum vector
//   void setActive(Bool_t flag = kTRUE)      Set track active/inactive
//   void setSuppressed(Bool_t flag = kTRUE)  Set track suppressed/unsuppressed
//   void setUserVal(Float_t val)             Set user specific values (for eaxpmple used for track embedding)
//   Bool_t isActive() const                  Test if track is active
//   Bool_t isPrimary() const                 Test if track is a primary
//   Bool_t isSuppressed() const              Test if track is suppressed
//   Float_t getUserVal()                     Get the specific user flag
//   Int_t setRichHitIndex(Int_t index)       Set index of RICH photon hit
//   Int_t setMdcHitIndex(Int_t index)        Set index of MDC hit
//   Int_t setTofHitIndex(Int_t index)        Set index of TOF hit
//   Int_t setRpcHitIndex(Int_t index)        Set index of RPC hit
//   Int_t setShowerHitIndex(Int_t index)     Set index of SHOWER hit
//   Int_t getFirstRichHit()                  Get index of first RICH photon hit of this track
//   Int_t getFirstMdcHit()                   Get index of first MDC hit of this track
//   Int_t getFirstTofHit()                   Get index of first TOF hit of this track
//   Int_t getFirstRpcHit()                   Get index of first TOF hit of this track
//   Int_t getFirstShowerHit()                Get index of first RPC hit of this track
//   HLinkedDataObject* nextRichHit()         Get pointer to next RICH photon hit in linked list
//   HLinkedDataObject* nextMdcHit()          Get pointer to next MDC photon hit in linked list
//   HLinkedDataObject* nextTofHit()          Get pointer to next TOF photon hit in linked list
//   HLinkedDataObject* nextShowerHit()       Get pointer to next SHOWER hit in linked list
//   HLinkedDataObject* nextRpcHit()          Get pointer to next RPC hit in linked list
//   void setRichCategory(HCategory* p)       Set pointer to RICH photon hit category
//   void setMdcCategory(HCategory* p)        Set pointer to MDC hit category
//   void setTofCategory(HCategory* p)        Set pointer to TOF hit category
//   void setRpcCategory(HCategory* p)        Set pointer to RPC hit category
//   void setShowerCategory(HCategory* p)     Set pointer to SHOWER hit category
//   void resetRichIter()                     Reset iterator on RICH photon hits
//   void resetMdcIter()                      Reset iterator on MDC hits
//   void resetTofIter()                      Reset iterator on TOF hits
//   void resetRpcIter()                      Reset iterator on RPC hits
//   void resetShowerIter()                   Reset iterator on SHOWER hits
//
/////////////////////////////////////////////////////////////////////////////

ClassImp(HGeantKine)

HGeantKine::HGeantKine(void) {
  // Default constructor.
  trackNumber = 0;
  parentTrack = 0;
  particleID = 0;
  mediumNumber = 0;
  creationMechanism = 0;
  xVertex = yVertex = zVertex = 0.;
  xMom = yMom = zMom = 0.;
  generatorInfo = 0.;
  generatorInfo1 = 0.;
  generatorInfo2 = 0.;
  generatorWeight = 1.;
  firstRichHit = firstMdcHit = firstTofHit = firstShowerHit = firstRpcHit = -1;
  pRich = pMdc = pTof = pShow = pRpc = NULL;
  richIndex = mdcIndex = showIndex = tofIndex = rpcIndex = 0;
  active = kFALSE;
  suppressed = kFALSE;
  userVal = -1;
}

HGeantKine::HGeantKine(HGeantKine &aKine) {
  // Copy constructor.
  trackNumber = aKine.trackNumber;
  parentTrack = aKine.parentTrack;
  particleID = aKine.particleID;
  mediumNumber = aKine.mediumNumber;
  creationMechanism = aKine.creationMechanism;
  xVertex = aKine.xVertex;
  yVertex = aKine.yVertex;
  zVertex = aKine.zVertex;
  xMom = aKine.xMom;
  yMom = aKine.yMom;
  zMom = aKine.zMom;
  generatorInfo = aKine.generatorInfo;
  generatorInfo1 = aKine.generatorInfo1;
  generatorInfo2 = aKine.generatorInfo2;
  generatorWeight = aKine.generatorWeight;
  firstRichHit = aKine.firstRichHit;
  firstMdcHit = aKine.firstMdcHit;
  firstTofHit = aKine.firstTofHit;
  firstRpcHit = aKine.firstRpcHit;
  firstShowerHit = aKine.firstShowerHit;
  active = aKine.active;
  suppressed = aKine.suppressed;
  userVal = aKine.userVal;
}

HGeantKine::~HGeantKine(void) {
  // Destructor.
}

void HGeantKine::setParticle(Int_t aTrack, Int_t aID) {
  // Set GEANT track and particle id.
  trackNumber = aTrack;
  particleID = aID;
}

void HGeantKine::setCreator(Int_t aPar, Int_t aMed, Int_t aMech) {
  // Set information on particle creation: parent track number, midium number, and creation mechanism.
  parentTrack = aPar;
  mediumNumber = aMed;
  creationMechanism = aMech;
}

void HGeantKine::setVertex(Float_t ax, Float_t ay, Float_t az) {
  // Set creation vertex (x,y,z in mm) in lab frame.
  xVertex = ax;
  yVertex = ay;
  zVertex = az;
}

void HGeantKine::setMomentum(Float_t apx, Float_t apy, Float_t apz) {
  // Set particle momentum components at creation (in MeV/c).
  xMom = apx;
  yMom = apy;
  zMom = apz;
}

void HGeantKine::setGenerator(Float_t aInfo, Float_t aWeight) {
  // Set information on generator and weight.
  generatorInfo = aInfo;
  generatorWeight = aWeight;
}

void HGeantKine::setGenerator(Float_t aInfo, Float_t aInfo1, Float_t aInfo2) {
  // Set full generator information.
  generatorInfo = aInfo;
  generatorInfo1 = aInfo1;
  generatorInfo2 = aInfo2;
}

void HGeantKine::getParticle(Int_t &aTrack, Int_t &aID) {
  // Retrieve GEANT track number and id.
  aTrack = trackNumber;
  aID = particleID;
}

void HGeantKine::getCreator(Int_t &aPar, Int_t &aMed, Int_t &aMech) {
  // Retrieve parenet track number, medium and mechanism of creation.
  aPar = parentTrack;
  aMed = mediumNumber;
  aMech = creationMechanism;
}

void HGeantKine::getVertex(Float_t &ax, Float_t &ay, Float_t &az) {
  // Retrieve creation vertex (in mm) in lab frame.
  ax = xVertex;
  ay = yVertex;
  az = zVertex;
}

void HGeantKine::getMomentum(Float_t &apx, Float_t &apy, Float_t &apz) {
  // Retrieve momentum components at creation time.
  apx = xMom;
  apy = yMom;
  apz = zMom;
}

void HGeantKine::getGenerator(Float_t &aInfo, Float_t &aWeight) {
  // Retrieve generator infomation and track weight. 
  aInfo = generatorInfo;
  aWeight = generatorWeight;
}

void HGeantKine::getGenerator(Float_t &aInfo, Float_t &aInfo1, Float_t &aInfo2) {
  // Retrieve full generator information.
  aInfo = generatorInfo;
  aInfo1 = generatorInfo1;
  aInfo2 = generatorInfo2;
}

void HGeantKine::setNewTrackNumber(Int_t track) {
  // Set track number to new value and update indexes in all hit categories. 
  // Used in HGeant to re-index after having suppressed non-active tracks;
  // see fillkine.cc.
  Int_t next = 0;
  HLinkedDataObject* p = NULL;

  trackNumber = track;

  if ( (next = (Int_t)firstMdcHit) != -1) {  // update Mdc hits
    resetMdcIter();
    do {
       p = (HLinkedDataObject*)(pMdc->getObject(next));
       ( (HGeantMdc*)p )->setTrack(track);
    } while( (next = p->getNextHitIndex()) != -1);
  }

  if ( (next = (Int_t)firstRichHit) != -1) { // update Rich photon hits
    resetRichIter();
    do {
       p = (HLinkedDataObject*)(pRich->getObject(next));
       ( (HGeantRichPhoton*)p )->setTrack(track);
    } while( (next = p->getNextHitIndex()) != -1);
  }

  if ( (next = (Int_t)firstTofHit) != -1) {  // update Tof hits
    resetTofIter();
    do {
       p = (HLinkedDataObject*)(pTof->getObject(next));
       ( (HGeantTof*)p )->setTrack(track);
    } while( (next = p->getNextHitIndex()) != -1);
  }

  if ( (next = (Int_t)firstRpcHit) != -1) {  // update Rpc hits
    resetRpcIter();
    do {
       p = (HLinkedDataObject*)(pRpc->getObject(next));
       ( (HGeantRpc*)p )->setTrack(track);
    } while( (next = p->getNextHitIndex()) != -1);
  }

  if ( (next = (Int_t)firstShowerHit) != -1) { // update Shower hits
    resetShowerIter();
    do {
       p = (HLinkedDataObject*)(pShow->getObject(next));
       ( (HGeantShower*)p )->setTrack(track);
    } while( (next = p->getNextHitIndex()) != -1);
  }

}

///////////////////////////////////////////////////////////////////////////////

Int_t HGeantKine::setHitIndex(HCategory* pCat,Short_t& firstHit, Int_t index) {
  // Set next hit index in generic linked list.
  //
  if(pCat == NULL) return 1;
  if(pCat->getObject(index) == NULL) return 1;

  if(firstHit == -1) {              // first hit in linked list
     firstHit = (Short_t)index; 
     active = kTRUE;
  } else {
     HLinkedDataObject* p = NULL;
     Int_t next = (Int_t)firstHit;
     do {                               // find end of list
        p = (HLinkedDataObject*)(pCat->getObject(next));
        next = p->getNextHitIndex();
     } while(next != -1);
     p->setNextHitIndex(index);         // append hit to end of list
  }
  return 0;
}

void HGeantKine::sortRichHits(void) {
  // Sort RICH photon hits in linked list for present track.
  //
  if(pRich == NULL) return;
  Int_t nHits = getNRichHits();
  if(nHits < 2) return;  // nothing to do

  HLinkedDataObject* p = NULL;
  for(Int_t i=0; i<nHits-1;i++) {  //  iterate through list nHits times
     resetRichIter();
     Int_t next = (Int_t)firstRichHit;
     do {
        p = (HLinkedDataObject*)(pRich->getObject(next));
	//
	// do here the swapping of hits
	//
        next = p->getNextHitIndex();
     } while(next != -1);
  }
  return;
}

void HGeantKine::sortMdcHits(void) {
  //
  // Sort MDC hits in linked list for actual track by increasing time-of-flight
  // (this is a simple bubble sort, should be replaced by something faster!).
  //
  // As sortVariable is non-persistent, the sort cannot be done a posteriori,
  // i.e. when the tree is read back. It has to be done before writing!
  //
  if(pMdc == NULL) return;
  Int_t nHits = getNMdcHits();
  if(nHits < 2) return;  // nothing to do

  HLinkedDataObject *pPrevPrev, *pPrev, *pHere;
  Float_t tofHere = 1000000.0;
  Float_t tofPrev = 1000000.0;
  for(Int_t i=0; i<nHits-1;i++) {  //  iterate through list nHits-1 times
     resetMdcIter();
     Int_t prev = -2;
     Int_t here = -1;
     Int_t next = (Int_t)firstMdcHit;
     pPrevPrev = pPrev = NULL;
     do {
        pHere = (HLinkedDataObject*)(pMdc->getObject(next));
        if(pHere == NULL) {
           printf("\nNULL in HGeantKine::sortMdcHits() at index %d ! \n",next);
           return;
        }
        prev = here;
        here = next;
        next = pHere->getNextHitIndex();
        tofHere = pHere->sortVariable;  // as sortVariable is non-persistent
	                                // the sort cannot be done a posteriori

        if(pPrev != NULL) {
           if(tofHere < tofPrev) { // swap hits if present hit is earlier
              if(pPrevPrev == NULL) firstMdcHit = (Short_t)here;
              else pPrevPrev->setNextHitIndex(here);
              pPrev->setNextHitIndex(next);
              pHere->setNextHitIndex(prev);
              pPrevPrev = pHere;
              here = prev;
              continue;
           }
        }

        pPrevPrev = pPrev;
        pPrev = pHere;
        tofPrev = tofHere;
     } while(next != -1);
  }
  return;
}

void HGeantKine::sortTofHits(void) {
  // Sort TOF hits in linked list for present track.
  //
  if(pTof == NULL) return;
  Int_t nHits = getNTofHits();
  if(nHits < 2) return;  // nothing to do

  HLinkedDataObject *pPrevPrev, *pPrev, *pHere;
  Float_t tofHere = 1000000.0;
  Float_t tofPrev = 1000000.0;
  for(Int_t i=0; i<nHits-1;i++) {  //  iterate through list nHits-1 times
     resetTofIter();
     Int_t prev = -2;
     Int_t here = -1;
     Int_t next = (Int_t)firstTofHit;
     pPrevPrev = pPrev = NULL;
     do {
        pHere = (HLinkedDataObject*)(pTof->getObject(next));
        if(pHere == NULL) {
           printf("\nNULL in HGeantKine::sortTofHits() at index %d ! \n",next);
           return;
        }
        prev = here;
        here = next;
        next = pHere->getNextHitIndex();
        tofHere = pHere->sortVariable;  // as sortVariable is non-persistent
	                                // the sort cannot be done a posteriori

        if(pPrev != NULL) {
           if(tofHere < tofPrev) { // swap hits if present hit is earlier
              if(pPrevPrev == NULL) firstTofHit = (Short_t)here;
              else pPrevPrev->setNextHitIndex(here);
              pPrev->setNextHitIndex(next);
              pHere->setNextHitIndex(prev);
              pPrevPrev = pHere;
              here = prev;
              continue;
           }
        }

        pPrevPrev = pPrev;
        pPrev = pHere;
        tofPrev = tofHere;
     } while(next != -1);
  }
  return;
}


void HGeantKine::sortRpcHits(void) {
  // Sort RPC hits in linked list for present track FIXME!! Is it ok??
  //
  if(pRpc == NULL) return;
  Int_t nHits = getNRpcHits();
  if(nHits < 2) return;  // nothing to do

  HLinkedDataObject *pPrevPrev, *pPrev, *pHere;
  Float_t tofHere = 1000000.0;
  Float_t tofPrev = 1000000.0;
  for(Int_t i=0; i<nHits-1;i++) {  //  iterate through list nHits-1 times
     resetRpcIter();
     Int_t prev = -2;
     Int_t here = -1;
     Int_t next = (Int_t)firstRpcHit;
     pPrevPrev = pPrev = NULL;
     do {
        pHere = (HLinkedDataObject*)(pRpc->getObject(next));
        if(pHere == NULL) {
           printf("\nNULL in HGeantKine::sortRpcHits() at index %d ! \n",next);
           return;
        }
        prev = here;
        here = next;
        next = pHere->getNextHitIndex();
        tofHere = pHere->sortVariable;  // as sortVariable is non-persistent
	                                // the sort cannot be done a posteriori

        if(pPrev != NULL) {
           if(tofHere < tofPrev) { // swap hits if present hit is earlier
              if(pPrevPrev == NULL) firstRpcHit = (Short_t)here;
              else pPrevPrev->setNextHitIndex(here);
              pPrev->setNextHitIndex(next);
              pHere->setNextHitIndex(prev);
              pPrevPrev = pHere;
              here = prev;
              continue;
           }
        }

        pPrevPrev = pPrev;
        pPrev = pHere;
        tofPrev = tofHere;
     } while(next != -1);
  }
  return;
}






void HGeantKine::sortShowerHits(void) {
  // Sort SHOWER hits in linked list for present track.
  //
  if(pShow == NULL) return;
  Int_t nHits = getNShowerHits();
  if(nHits < 2) return;  // nothing to do

  HLinkedDataObject* p = NULL;
  for(Int_t i=0; i<nHits-1;i++) {  //  iterate through list nHits times
     resetShowerIter();
     Int_t next = (Int_t)firstShowerHit;
     do {
        p = (HLinkedDataObject*)(pShow->getObject(next));
	//
	// do here the swapping of hits
	//
        next = p->getNextHitIndex();
     } while(next != -1);
  }
  return;
}

HLinkedDataObject* HGeantKine::nextHit(HCategory* pCat,Int_t& index) {
  // Return next hit in category made by present track.
   if(index == -1 || pCat == NULL) return NULL;
   HLinkedDataObject* p = (HLinkedDataObject*)(pCat->getObject(index));
   index = p->getNextHitIndex();
   return p;
}

Int_t HGeantKine::getNRichHits(void) {
  // Return number of RICH photon hits made by present track.
  //
   Int_t n = 0;
   if(firstRichHit > -1) {
      resetRichIter();
      while(nextRichHit() != NULL) n++;
      resetRichIter();
   }
   return n;
}

Int_t HGeantKine::getNMdcHits(void) {
  // Return number of MDC hits made by present track.
  //
   Int_t n = 0;
   if(firstMdcHit > -1) {
      resetMdcIter();
      while(nextMdcHit() != NULL) n++;
      resetMdcIter();
   }
   return n;
}

Int_t HGeantKine::getNMdcHits(Int_t module) {
// Return number of MDC hits made by present track
// for MDC module (range: [0,3]).

  Int_t n = 0;
  if (module >= 0 && module <4)
     {
     if(firstMdcHit > -1)
        {
	HGeantMdc* hit = NULL;
	resetMdcIter();
	while((hit = (HGeantMdc*) nextMdcHit()) != NULL)
	   {
	   if (hit->getModule() == module)
	      {
	      n++;
	      } 
	   }
	resetMdcIter();
	return n;
	}
     }
     else
     {
	return -1;
     }
  return n;
}

Int_t HGeantKine::getNTofHits(void) {
  // Return number of TOF hits made by present track.
  //
   Int_t n = 0;
   if(firstTofHit > -1) {
      resetTofIter();
      while(nextTofHit() != NULL) n++;
      resetTofIter();
   }
   return n;
}

Int_t HGeantKine::getNRpcHits(void) {
  // Return number of RPC hits made by present track.
  //
   Int_t n = 0;
   if(firstRpcHit > -1) {
      resetRpcIter();
      while(nextRpcHit() != NULL) n++;
      resetRpcIter();
   }
   return n;
}

Int_t HGeantKine::getNShowerHits(void) {
  // Return number of SHOWER hits made by present track.
  //
   Int_t n = 0;
   if(firstShowerHit > -1) {
      resetShowerIter();
      while(nextShowerHit() != NULL) n++;
      resetShowerIter();
   }
   return n;
}

Int_t HGeantKine::getSystem(void) 
{
  // Return system crossed by the present track
  //  -1: no contact:
  //   0: TOFINO
  //   1: TOF
  //   2: TOFINO and TOF

   Int_t n = -1;
   Int_t n0 = 0;
   Int_t n1 = 0;
   if(firstTofHit > -1) {
      HGeantTof* hit = NULL;
      resetTofIter();
      while((hit = (HGeantTof*) nextTofHit()) != NULL)
	{
	  if (hit->getModule()<22) // Tofino starts from module 22 on.
	    {
	      n1++; // TOF hits
	    }
	  else
	    {
	      n0++; // TOFINO hits
	    }
	}

      resetTofIter();
      if ( n0 > 0 && n1 <= 0) n = 0; // hits in TOFINO, but no hits in TOF --> TOFINO
      if ( n1 > 0 && n0 <= 0) n = 1; // hits in TOF, but no hits in TOFINO --> TOF
      if ( n0 > 0 && n1 >  0) n = 2; // hits in TOF and hits in TOFINO --> BOTH 
   }
   return n;
}

UInt_t HGeantKine::getMdcSectorBitArray(void) 
{
  // Returns bit array containing sectors crossed by current track
  //   bit 0: sector 0
  //   bit 1: sector 1
  //   bit 2: sector 2
  //   bit 3: sector 3
  //   bit 4: sector 4
  //   bit 5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5
  //  in hex:  0x2a
  //  decimal: 34

  UInt_t n=0;
  if(firstMdcHit > -1)
    {
      HGeantMdc* hit = NULL;
      resetMdcIter();
      while((hit = (HGeantMdc*) nextMdcHit()) != NULL)
	{
	  n|=(0x1<<hit->getSector());
	}
      resetMdcIter();
      return n&0x3f;
      
    }
  return n&0x3f;
}

UInt_t HGeantKine::getMdcSectorDecimalArray(void) 
{
  // Returns decimal array containing sectors crossed by current track
  //   digit 10^0: sector 0
  //   digit 10^1: sector 1
  //   digit 10^2: sector 2
  //   digit 10^3: sector 3
  //   digit 10^4: sector 4
  //   digit 10^5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5

  UInt_t bit = getMdcSectorBitArray() && 0x3f;
  UInt_t n=0;
  
  for (int p=0; p<6; p++)
    {
      if ((bit>>p)&0x1)
      {
	n+=(UInt_t) pow(10.,p);
      }
    }
  return n;
}

UInt_t HGeantKine::getShowerSectorBitArray(void) 
{
  // Returns bit array containing sectors crossed by current track
  //   bit 0: sector 0
  //   bit 1: sector 1
  //   bit 2: sector 2
  //   bit 3: sector 3
  //   bit 4: sector 4
  //   bit 5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5
  //  in hex: 0x2a
  // decimal: 34

  UInt_t n=0;
  if(firstShowerHit > -1)
    {
      HGeantShower* hit = NULL;
      resetShowerIter();
      while((hit = (HGeantShower*) nextShowerHit()) != NULL)
	{
	  n|=(0x1<<hit->getSector());
	}
      resetShowerIter();
      return n&0x3f;
      
    }
  return n&0x3f;
}

UInt_t HGeantKine::getShowerSectorDecimalArray(void) 
{
  // Returns decimal array containing sectors crossed by current track
  //   digit 10^0: sector 0
  //   digit 10^1: sector 1
  //   digit 10^2: sector 2
  //   digit 10^3: sector 3
  //   digit 10^4: sector 4
  //   digit 10^5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5

  UInt_t bit = getShowerSectorBitArray() && 0x3f;
  UInt_t n=0;
  
  for (int p=0; p<6; p++)
    {
      if ((bit>>p)&0x1)
      {
	n+=(UInt_t) pow(10.,p);
      }
    }
  return n;
}

UInt_t HGeantKine::getTofSectorBitArray(void) 
{
  // Returns bit array containing sectors crossed by current track
  //   bit 0: sector 0
  //   bit 1: sector 1
  //   bit 2: sector 2
  //   bit 3: sector 3
  //   bit 4: sector 4
  //   bit 5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5
  //  in hex: 0x2a
  // decimal: 34

  UInt_t n=0;
  if(firstTofHit > -1)
    {
      HGeantTof* hit = NULL;
      resetTofIter();
      while((hit = (HGeantTof*) nextTofHit()) != NULL)
	{
	  n|=(0x1<<hit->getSector());
	}
      resetTofIter();
      return n&0x3f;
      
    }
  return n&0x3f;
}

UInt_t HGeantKine::getTofSectorDecimalArray(void) 
{
  // Returns decimal array containing sectors crossed by current track
  //   digit 10^0: sector 0
  //   digit 10^1: sector 1
  //   digit 10^2: sector 2
  //   digit 10^3: sector 3
  //   digit 10^4: sector 4
  //   digit 10^5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5

  UInt_t bit = getTofSectorBitArray() && 0x3f;
  UInt_t n=0;
  
  for (int p=0; p<6; p++)
    {
      if ((bit>>p)&0x1)
      {
	n+=(UInt_t) pow(10.,p);
      }
    }
  return n;
}

UInt_t HGeantKine::getRpcSectorBitArray(void) 
{
  // Returns bit array containing sectors crossed by current track
  //   bit 0: sector 0
  //   bit 1: sector 1
  //   bit 2: sector 2
  //   bit 3: sector 3
  //   bit 4: sector 4
  //   bit 5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5
  //  in hex: 0x2a
  // decimal: 34

  UInt_t n=0;
  if(firstRpcHit > -1)
    {
      HGeantRpc* hit = NULL;
      resetRpcIter();
      while((hit = (HGeantRpc*) nextRpcHit()) != NULL)
	{
	  n|=(0x1<<hit->getSector());
	}
      resetRpcIter();
      return n&0x3f;
      
    }
  return n&0x3f;
}

UInt_t HGeantKine::getRpcSectorDecimalArray(void) 
{
  // Returns decimal array containing sectors crossed by current track
  //   digit 10^0: sector 0
  //   digit 10^1: sector 1
  //   digit 10^2: sector 2
  //   digit 10^3: sector 3
  //   digit 10^4: sector 4
  //   digit 10^5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5

  UInt_t bit = getRpcSectorBitArray() && 0x3f;
  UInt_t n=0;
  
  for (int p=0; p<6; p++)
    {
      if ((bit>>p)&0x1)
      {
	n+=(UInt_t) pow(10.,p);
      }
    }
  return n;
}

UInt_t HGeantKine::getRichSectorBitArray(void) 
{
  // Returns bit array containing sectors crossed by current track
  //   bit 0: sector 0
  //   bit 1: sector 1
  //   bit 2: sector 2
  //   bit 3: sector 3
  //   bit 4: sector 4
  //   bit 5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5
  //  in hex: 0x2a
  // decimal: 34

  UInt_t n=0;
  if(firstRichHit > -1)
    {
      HGeantRichPhoton* hit = NULL;
      resetRichIter();
      while((hit = (HGeantRichPhoton*) nextRichHit()) != NULL)
	{
	  n|=(0x1<<hit->getSector());
	}
      resetRichIter();
      return n&0x3f;
    }
  return n&0x3f;
}

UInt_t HGeantKine::getRichSectorDecimalArray(void) 
{
  // Returns decimal array containing sectors crossed by current track
  //   digit 10^0: sector 0
  //   digit 10^1: sector 1
  //   digit 10^2: sector 2
  //   digit 10^3: sector 3
  //   digit 10^4: sector 4
  //   digit 10^5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5

  UInt_t bit = getRichSectorBitArray() && 0x3f;
  UInt_t n=0;
  
  for (int p=0; p<6; p++)
    {
      if ((bit>>p)&0x1)
      {
	n+=(UInt_t) pow(10.,p);
      }
    }
  return n;
}

UInt_t HGeantKine::getSectorBitArray(void)
{
  // Returns bit array containing sectors crossed by current track
  // as an or of all detectors (RICH, TOF, SHOWER, MDC)
  //   bit 0: sector 0
  //   bit 1: sector 1
  //   bit 2: sector 2
  //   bit 3: sector 3
  //   bit 4: sector 4
  //   bit 5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5
  //  in hex:  0x2a
  //  decimal: 34

  UInt_t n=0;
  n|=getRichSectorBitArray();
  n|=getTofSectorBitArray();
  n|=getRpcSectorBitArray();
  n|=getMdcSectorBitArray();
  n|=getShowerSectorBitArray();
  return n&0x3f;
}

UInt_t HGeantKine::getSectorDecimalArray(void)
{
  // Returns decimal array containing sectors crossed by current track
  // as an or of all detectors (RICH, TOF, SHOWER, MDC)
  //   digit 10^0: sector 0
  //   digit 10^1: sector 1
  //   digit 10^2: sector 2
  //   digit 10^3: sector 3
  //   digit 10^4: sector 4
  //   digit 10^5: sector 5
  //
  // i.e. 101010 -> Track crossed sector 1,3 and 5.

  UInt_t bit = getSectorBitArray() && 0x3f;
  UInt_t n=0;
  
  for (int p=0; p<6; p++)
    {
      if ((bit>>p)&0x1)
      {
	n+=(UInt_t) pow(10.,p);
      }
    }
  return n;
}

Int_t HGeantKine::getSector(void)
{
  // Returns sector if only one sector is crossed by current track
  // otherwise it returns a negative value
  // (-7 is returned if there is no hit)
  // where its absolute value gives the number of sectors crossed.
  // See getSectorBitArray(), getMdcBitArray(), getTofBitArray(), getRpcBitArray(),
  // getRichBitArray(), getShowerBitArray() for more details.

  UInt_t n=getSectorBitArray();
  if (n==0) return -7;
  Int_t sector=-1;
  Int_t ctr=0;
  for (int p=0; p<6; p++)
    {
      if ((n>>p)&0x1) 
	{
	  if (ctr==0) 
	    {
	      sector = p;
	      ctr++;
	    }
	  else ctr++;
	}
    }
    return (ctr>1)?-ctr:sector;
}

void HGeantKine::Streamer(TBuffer &R__b)
{
   // Stream an object of class HGeantKine.

   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(); if (R__v) { }
      TObject::Streamer(R__b);
      R__b >> trackNumber;
      R__b >> parentTrack;
      R__b >> particleID;
      R__b >> mediumNumber;
      R__b >> creationMechanism;
      R__b >> xVertex;
      R__b >> yVertex;
      R__b >> zVertex;
      R__b >> xMom;
      R__b >> yMom;
      R__b >> zMom;
      R__b >> generatorInfo;
      if (R__v > 4) {
         R__b >> generatorInfo1;
         R__b >> generatorInfo2;
      } else {
         generatorInfo1 = 0.;
         generatorInfo2 = 0.;
      }
      R__b >> generatorWeight;

      if (R__v > 1) {
         R__b >> firstRichHit;
         R__b >> firstMdcHit;
         R__b >> firstTofHit;
	 if (R__v > 5) { // rpc added in vers 6
	     R__b >> firstRpcHit;
	 } else {
           firstRpcHit = -1;
	 }
         R__b >> firstShowerHit;
      } else {
         firstRichHit = -1;
         firstMdcHit = -1;
         firstTofHit = -1;
         firstRpcHit = -1;
	 firstShowerHit = -1;
      }

      if (R__v > 2) {
	 R__b >> active;
      } else {
 	 if (parentTrack==0) active = kTRUE;       // primary track
	 else active = kFALSE;

	 if(R__v > 5)
	 {   // rpc added in vers 6
	     if ( firstRichHit > -1 ||
		 firstMdcHit > -1  ||
		 firstTofHit > -1  ||
		 firstRpcHit > -1  ||
		 firstShowerHit>-1 ) active = kTRUE;  // track made a hit
	 } else {
	     if ( firstRichHit > -1 ||
		 firstMdcHit > -1  ||
		 firstTofHit > -1  ||
		 firstShowerHit>-1 ) active = kTRUE;  // track made a hit
	 }
      }

      if (R__v > 3) {
         R__b >> suppressed;
      } else {
	 suppressed = kFALSE;
      }
      if (R__v > 6) {
         R__b >> userVal;
      } else {
	 userVal = 0;
      }

   } else {
      R__b.WriteVersion(HGeantKine::IsA());
      TObject::Streamer(R__b);
      R__b << trackNumber;
      R__b << parentTrack;
      R__b << particleID;
      R__b << mediumNumber;
      R__b << creationMechanism;
      R__b << xVertex;
      R__b << yVertex;
      R__b << zVertex;
      R__b << xMom;
      R__b << yMom;
      R__b << zMom;
      R__b << generatorInfo;
      R__b << generatorInfo1;
      R__b << generatorInfo2;
      R__b << generatorWeight;
      R__b << firstRichHit;
      R__b << firstMdcHit;
      R__b << firstTofHit;
      R__b << firstRpcHit;
      R__b << firstShowerHit;
      R__b << active;
      R__b << suppressed;
      R__b << userVal;
   }
} 


Bool_t HGeantKine::setChainActive(Int_t track, Bool_t flag, HLinearCategory* cat)
{
  // Set track and all its ancestors up to primary track active. 
  //
  HLinearCategory* catKine;
  if (track < 1) return kFALSE;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return kFALSE;
  HGeantKine* pKine = (HGeantKine*)catKine->getObject(track-1);
  if (pKine == NULL) return kFALSE;

  do {   // iterate over all ancestors until primary is found
    pKine->setActive(flag);
    track = pKine->getTrack();
  } while ((pKine=getParent(track,catKine)) != NULL);
  
  return kTRUE;
}

Bool_t HGeantKine::setAllDescendentsActive(Int_t trackIn, Bool_t flag, HLinearCategory* cat)
{
  // Set track and all of its descendents active.
  //
  HLinearCategory* catKine;
  if (trackIn < 1) return kFALSE;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return kFALSE;
  HGeantKine* pKine = (HGeantKine*)catKine->getObject(trackIn-1);

  Int_t nEntries=catKine->getEntries();
  for (Int_t i=0; i<nEntries; i++) {
    pKine = (HGeantKine*)catKine->getObject(i);
    if (pKine->hasAncestor(trackIn,catKine)) pKine->setActive(flag);
  }
  
  return kTRUE;
}

Bool_t HGeantKine::setAllDescendentsSuppressed(Int_t trackIn, Bool_t flag, HLinearCategory* cat)
{
  // Set track and all of its descendents suppressed. 
  //
  HLinearCategory* catKine;
  if (trackIn < 1) return kFALSE;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return kFALSE;
  HGeantKine* pKine = (HGeantKine*)catKine->getObject(trackIn-1);
  if (pKine == NULL) return kFALSE;

  Int_t nEntries=catKine->getEntries();
  for (Int_t i=0; i<nEntries; i++) {
    pKine = (HGeantKine*)catKine->getObject(i);
    if (pKine->hasAncestor(trackIn,catKine)) {
      pKine->setSuppressed(flag);
      if (flag==kTRUE) pKine->setActive(kFALSE);
    }
  }
  
  return kTRUE;
}

Bool_t HGeantKine::suppressTracks(Int_t id, Float_t acceptedFraction, HLinearCategory* cat) {
  // Suppress randomly a certain fraction of tracks of particle kind id, as well as all of its
  // descendants.   This can be used to correct (lower!) the yield of a certain particle species
  // AFTER the event generator has been run.
  //
  if (id<0 || acceptedFraction < 0. || acceptedFraction > 1.) return kFALSE;
  HLinearCategory* catKine;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  Int_t track=1;
  HGeantKine* pKine;
  pKine = (HGeantKine*)catKine->getObject(track-1);
  if (pKine == NULL) return kFALSE;
  while (pKine && pKine->isPrimary()) { // iterate over primaries and throw dice to
    if ((pKine->getID()) == id) {   // decide if they are to be suppressed or not
      if (gRandom->Rndm() > acceptedFraction)
        setAllDescendentsSuppressed(track, kTRUE, catKine);  // propagate the verdict to all descendents
    }
    track++;
    pKine = (HGeantKine*)catKine->getObject(track-1);
  }
  return kTRUE;
}

HGeantKine* HGeantKine::getParent(Int_t track, HLinearCategory* cat)
{ 
  // Return pointer to parent object, return NULL if none.
  //
  if (track < 1) return NULL;
  HLinearCategory* catKine;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return NULL;
  HGeantKine* pKine = (HGeantKine*)catKine->getObject(track-1);
  if (pKine == NULL) return NULL;
  Int_t parent = pKine->getParentTrack();
  if (parent == 0) return NULL;
  else return (HGeantKine*)catKine->getObject(parent-1);
}


HGeantKine* HGeantKine::getGrandParent(Int_t track, HLinearCategory* cat)
{ 
  // Return pointer to grandparent object, return NULL if none.
  //
  if (track < 1) return NULL;
  HLinearCategory* catKine;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return NULL;
  HGeantKine* pKine = getParent(track,catKine);
  if (pKine == NULL) return NULL;
  Int_t grandparent = pKine->getParentTrack();
  if(grandparent == 0) return NULL;
  else return (HGeantKine*)catKine->getObject(grandparent-1);
}

HGeantKine* HGeantKine::getPrimary(Int_t trackIn, HLinearCategory* cat)
{
  // Return pointer to primary ancestor of track.
  //
  if (trackIn < 1) return NULL;
  HLinearCategory* catKine;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return NULL;
  HGeantKine* pKine = (HGeantKine*)catKine->getObject(trackIn-1);
  if (pKine == NULL) return NULL;
  HGeantKine* pSave;
  Int_t track=0;

  do {   // iterate over all ancestors until primary is found
    pSave = pKine;
    track = pKine->getTrack();
  } while ((pKine=pKine->getParent(track,catKine)) != NULL);
  
  return pSave;
}

Bool_t HGeantKine::hasAncestor(Int_t trackIn, HLinearCategory* cat)
{
  // Check if current track has trackIn as ancestor (or is trackIn).
  //
  if (trackIn < 1) return kFALSE;
  HLinearCategory* catKine;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return kFALSE;
  HGeantKine* pKine = this;
  Int_t track = -1;

  do {   // iterate over all ancestors until track is found
    track = pKine->getTrack();
    if (track == trackIn) return kTRUE;
  } while ((pKine=getParent(track,catKine)) != NULL);
  
  return kFALSE;
}

HGeantKine* HGeantKine::getCommonAncestor(Int_t track1, Int_t track2, HLinearCategory* cat)
{
  // Return pointer to nearest ancestor of 2 tracks.
  // This ancestor is not necessarily a primary. 
  //
  if (track1 < 1 || track2 < 1) return NULL;
  HLinearCategory* catKine;
  if (cat != NULL) catKine = cat;
  else catKine = (HLinearCategory*)gHades->getCurrentEvent()
                                         ->getCategory(catGeantKine);
  if (catKine == NULL) return NULL;
  HGeantKine* pKine1 = (HGeantKine*)catKine->getObject(track1-1);
  if (pKine1 == NULL) return NULL;
  HGeantKine* pKine2 = (HGeantKine*)catKine->getObject(track2-1);
  if (pKine2 == NULL) return NULL;

  Int_t track=0;
  if (track1 < track2) {  // do the search along the shortest branch

    do {   // iterate until common ancestor or primary is found
      track = pKine1->getTrack();
      if (pKine2->hasAncestor(track,catKine)) return pKine1;
    } while ((pKine1=pKine1->getParent(track,catKine)) != NULL);
    return NULL;

  } else {

    do {   // iterate until common ancestor or primary is found
      track = pKine2->getTrack();
      if (pKine1->hasAncestor(track,catKine)) return pKine2;
    } while ((pKine2=pKine2->getParent(track,catKine)) != NULL);
    return NULL;
  }
}

