#include "hgeantkine.h"
#include <hcategory.h>
#include <hlinkeddataobject.h>
#include <hgeantmdc.h>

//*-- Author : Romain Holzmann, GSI
//*-- Modified : 14/12/99 by Romain Holzmann
//*-- Modified : 06/10/03 by Romain Holzmann
//*-- Copyright : GSI, Darmstadt

////////////////////////////////////////////////////////////////////////////
//  HGeantKine
// 
//  HGeant KINE event header
//
////////////////////////////////////////////////////////////////////////////

ClassImp(HGeantKine)

HGeantKine::HGeantKine(void) {
  trackNumber = 0;
  parentTrack = 0;
  particleID = 0;
  mediumNumber = 0;
  creationMechanism = 0;
  xVertex = yVertex = zVertex = 0.;
  xMom = yMom = zMom = 0.;
  generatorInfo = 0.;
  generatorWeight = 1.;
  firstRichHit = firstMdcHit = firstTofHit = firstShowerHit = -1;
  pRich = pMdc = pTof = pShow = NULL;
}

HGeantKine::HGeantKine(HGeantKine &aKine) {
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
  generatorWeight = aKine.generatorWeight;
  firstRichHit = aKine.firstRichHit;
  firstMdcHit = aKine.firstMdcHit;
  firstTofHit = aKine.firstTofHit;
  firstShowerHit = aKine.firstShowerHit;
}

HGeantKine::~HGeantKine(void) {
}

void HGeantKine::setParticle(Int_t aTrack, Int_t aID) {
  trackNumber = aTrack;
  particleID = aID;
}

void HGeantKine::setCreator(Int_t aPar, Int_t aMed, Int_t aMech) {
  parentTrack = aPar;
  mediumNumber = aMed;
  creationMechanism = aMech;
}

void HGeantKine::setVertex(Float_t ax, Float_t ay, Float_t az) {
  xVertex = ax;
  yVertex = ay;
  zVertex = az;
}

void HGeantKine::setMomentum(Float_t apx, Float_t apy, Float_t apz) {
  xMom = apx;
  yMom = apy;
  zMom = apz;
}

void HGeantKine::setGenerator(Float_t aInfo, Float_t aWeight) {
  generatorInfo = aInfo;
  generatorWeight = aWeight;
}

void HGeantKine::getParticle(Int_t &aTrack, Int_t &aID) {
  aTrack = trackNumber;
  aID = particleID;
}

void HGeantKine::getCreator(Int_t &aPar, Int_t &aMed, Int_t &aMech) {
  aPar = parentTrack;
  aMed = mediumNumber;
  aMech = creationMechanism;
}

void HGeantKine::getVertex(Float_t &ax, Float_t &ay, Float_t &az) {
  ax = xVertex;
  ay = yVertex;
  az = zVertex;
}

void HGeantKine::getMomentum(Float_t &apx, Float_t &apy, Float_t &apz) {
  apx = xMom;
  apy = yMom;
  apz = zMom;
}

void HGeantKine::getGenerator(Float_t &aInfo, Float_t &aWeight) {
  aInfo = generatorInfo;
  aWeight = generatorWeight;
}

///////////////////////////////////////////////////////////////////////////////

Int_t HGeantKine::setHitIndex(HCategory* pCat,Short_t& firstHit, Int_t index) {
  // set next hit index in generic linked list
  if(pCat == NULL) return 1;
  if(pCat->getObject(index) == NULL) return 1;

  if(firstHit == -1) {              // first hit in linked list
     firstHit = (Short_t)index; 
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
  // sort RICH photon hits in linked list for present track
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
  // sort MDC hits in linked list for actual track by increasing time-of-flight
  // (this is a simple bubble sort, should be replaced by something faster!)
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
  // sort TOF hits in linked list for present track
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

void HGeantKine::sortShowerHits(void) {
  // sort SHOWER hits in linked list for present track
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
  // return next hit in category made by present track
   if(index == -1 || pCat == NULL) return NULL;
   HLinkedDataObject* p = (HLinkedDataObject*)(pCat->getObject(index));
   index = p->getNextHitIndex();
   return p;
}

Int_t HGeantKine::getNRichHits(void) {
  // return number of RICH photon hits made by present track
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
  // return number of MDC hits made by present track
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
// return number of MDC hits made by present track
// for MDC module (range: [0,3])

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
  // return number of TOF hits made by present track
  //
   Int_t n = 0;
   if(firstTofHit > -1) {
      resetTofIter();
      while(nextTofHit() != NULL) n++;
      resetTofIter();
   }
   return n;
}

Int_t HGeantKine::getNShowerHits(void) {
  // return number of SHOWER hits made by present track
  //
   Int_t n = 0;
   if(firstShowerHit > -1) {
      resetShowerIter();
      while(nextShowerHit() != NULL) n++;
      resetShowerIter();
   }
   return n;
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
      R__b >> generatorWeight;
      if (R__v == 1) {
         firstRichHit = -1;
         firstMdcHit = -1;
         firstTofHit = -1;
         firstShowerHit = -1;
      } else if (R__v == 2) {
         R__b >> firstRichHit;
         R__b >> firstMdcHit;
         R__b >> firstTofHit;
         R__b >> firstShowerHit;
      } else {
         Error("Streamer","Object version not known"); 
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
      R__b << generatorWeight;
      R__b << firstRichHit;
      R__b << firstMdcHit;
      R__b << firstTofHit;
      R__b << firstShowerHit;
   }
} 


