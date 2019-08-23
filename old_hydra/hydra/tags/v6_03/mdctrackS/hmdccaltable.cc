# include "hdebug.h"
#include "hmdccaltable.h"

ClassImp(HMdcCalLinkList)

HMdcCalLinkList::~HMdcCalLinkList(){
   // Remove all objects from the list. Does not delete the objects.
  clear();
}


void HMdcCalLinkList::add(HMdcCal3 *hit){

#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::add");
#endif

   // Add HMdcCal3 at the end of the list.

   if (!hit) return;

   if (!fFirst) {
      fFirst = newLink(hit);
      fLast  = fFirst;
   } else
      fLast = newLink(hit, fLast);

   Int_t cell = hit->getCell();
   //Int_t hitNum = hit->getHitNumber();
   //if(cell< fCapacity) fCells[cell].setHit(fLast,hitNum);
   if(cell< fCapacity) fCells[cell].setHit(fLast,fCells[cell].getNHits());

#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::add");
#endif
}


void HMdcCalLinkList::addHits(HMdcCal3 *hit1, HMdcCal3* hit2){

#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::addHits");
#endif

   // Add HMdcCal3s at the end of the list.

  if (!hit1 || !hit2) return;

  HMdcCalLink* temp;

  add(hit1);                       // add first hit to list
  temp = fLast;                    // stores pointer to first hit in temp
  fLast = newLink(hit2,fLast);     // add second hit to list
  fLast->setPartner(temp);
  temp->setPartner(fLast);
        
  Int_t cell = hit2->getCell();
  //Int_t hitnum = hit2->getHitNumber();
  //if(cell < fCapacity) fCells[cell].setHit(fLast,hitnum);

  if(cell < fCapacity) 
    fCells[cell].setHit(fLast,fCells[cell].getNHits());

#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::addHits");
#endif
}


void HMdcCalLinkList::swap(HMdcCalLink* link1, HMdcCalLink* link2){

  // swap link1 and link2 in list 

  HMdcCal3* temp , *temp2;
  HMdcCalLink *partner1, *partner2;

  temp = link1->getData();
  temp2 = link2->getData();

  //Int_t cell1 = temp->getCell();
  //Int_t hit1  = temp->getHitNumber();
  //Int_t cell2 = temp2->getCell();
  //Int_t hit2  = temp2->getHitNumber();
  
  //fCells[cell1].setHit(link2,hit1);
  //fCells[cell2].setHit(link1,hit2);
  

  partner1 = link1->getPartner();
  partner2 = link2->getPartner();

  link1->setData(temp2);
  link2->setData(temp);

  link1->getData()->setLink(link1);
  link2->getData()->setLink(link2);

  // if link1 and link2 are not partners, set the partners
  // if they are partners, then ok

  if(partner1!=link2){                   
    link1->setPartner(partner2);
    link2->setPartner(partner1);
    partner2->setPartner(link1);
    partner1->setPartner(link2); 
 }
  

}






void HMdcCalLinkList::clear(Option_t* option){
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::clear");
#endif

   // Remove all objects from the list. Does not delete the objects.

  //  Bool_t nodel = !strcmp(option, "nodelete") ? kTRUE : kFALSE;

  while (fFirst) {
    HMdcCalLink *tlk = fFirst;
    fFirst = fFirst->getNext();
    tlk->getData()->setLink(NULL);
    //tlk->setPartner(NULL);

    // delete only heap objects marked OK to clear
    //if (!nodel && tlk->getData() && tlk->getData()->IsOnHeap()) {
    //if (tlk->getData()->TestBit(kCanDelete))
    //TCollection::GarbageCollect(tlk->getData());
    //}
    delete tlk;
  }
  fFirst = fLast = 0;

  if(fCells) delete [] fCells;

#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::clear");
#endif

}



void HMdcCalLinkList::deleteList(){
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::deleteList");
#endif
   // Remove all objects from the list AND delete all heap based objects.

  
  while (fFirst) {
    HMdcCalLink *tlk = fFirst;
    fFirst = fFirst->getNext();
    tlk->getData()->setLink(NULL);

    // delete only heap objects
    //if (tlk->getData() && tlk->getData()->IsOnHeap()) {
    //  TCollection::GarbageCollect(tlk->getData());
    //}
    deleteLink(tlk);
  }
  fFirst = fLast = 0;

  //!!
  // clear fCells

  for(Int_t i=0; i<fCapacity; i++) fCells[i].removeHits();


#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::deleteList");
#endif
}



void HMdcCalLinkList::deleteLink(HMdcCalLink *lnk){

#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::deleteLink");
#endif
   // Delete a HMdcCalLink object.

 Int_t cell = lnk->getData()->getCell();
 //Int_t hitNum = lnk->getData()->getHitNumber();
 //fCells[cell].removeHit(hitNum);
 fCells[cell].removeHit(lnk);

 lnk->getData()->setLink(NULL);
 lnk->fNext = lnk->fPrev = lnk->fPartner = 0;
 lnk->fData = 0;
 delete lnk;

#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::deleteLink");
#endif
}


inline HMdcCalLinkListIter *HMdcCalLinkList::MakeIterator(Bool_t dir) const{
   // Return a list iterator.

   return new HMdcCalLinkListIter(this, dir);

}


HMdcCalLink *HMdcCalLinkList::newLink(HMdcCal3 *hit, HMdcCalLink *prev){
   // Return a new HMdcCalLink.
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::newLink");
#endif
   if (prev)
      return new HMdcCalLink(hit, prev);
   else
      return new HMdcCalLink(hit);
#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::newLink");
#endif
}


void HMdcCalLinkList::remove(HMdcCal3 *hit){
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::remove(HMdcCal3*)");
#endif

   // Remove hit and its partner from the list.

  if (!hit) return;

  HMdcCalLink *lnk = hit->getLink();
  if(!lnk) return;

  HMdcCalLink *partner = lnk->getPartner();

  remove(lnk);
  if (partner) remove(partner);


#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::remove(HMdcCal3*)");
#endif

}


void HMdcCalLinkList::remove(HMdcCalLink *lnk){
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLinkList::removeLink");
#endif

   // Remove link from list

  if (!lnk) return;

  if (lnk == fFirst) {
    fFirst = lnk->getNext();
    if (lnk == fLast) fLast = fFirst;
    else              fFirst->fPrev = 0;
    deleteLink(lnk);
  } else if (lnk == fLast) {
    fLast = lnk->getPrevious();
    fLast->fNext = 0;
    deleteLink(lnk);
  } else {
    lnk->getPrevious()->fNext = lnk->getNext();
    lnk->getNext()->fPrev = lnk->getPrevious();
    deleteLink(lnk);
  }


#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLinkList::remove(HMdcCal3*)");
#endif

}








//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HMdcCalLink                                                          //
//                                                                      //
// Wrapper around a TObject so it can be stored in a HMdcCalLinkList.   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////


HMdcCalLink::HMdcCalLink(HMdcCal3 *hit, HMdcCalLink *prev)
          : fNext(prev->fNext), fPrev(prev), fPartner(0), fData(hit){
#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcCalLink::HMdcCalLink(HMdcCal3*, HMdcCalLink*)");
#endif
   // Create a new HMdcCalLink.

   fPrev->fNext = this;
   if (fNext) fNext->fPrev = this;
   fData->setLink(this);

#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcCalLink::HMdcCalLink(HMdcCal3*, HMdcCalLink*)");
#endif
}

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HMdcCalLinkListIter                                                            //
//                                                                      //
// Iterator of linked list.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

ClassImp(HMdcCalLinkListIter)

HMdcCalLinkListIter::HMdcCalLinkListIter(const HMdcCalLinkList *l, Bool_t dir)
        : fList(l), fCurCursor(0), fCursor(0), fDirection(dir), fStarted(kFALSE)
{
   // Create a new list iterator. By default the iteration direction
   // is kIterForward. To go backward use kIterBackward.
}


HMdcCal3* HMdcCalLinkListIter::Next(){

   // Return next object in the list. Returns 0 when no more objects in list.

   if (!fList) return 0;

   if (fDirection == kIterForward) {
      if (!fStarted) {
         fCursor = fList->fFirst;
         fStarted = kTRUE;
      }
      fCurCursor = fCursor;
      if (fCursor) fCursor = fCursor->getNext();
   } else {
      if (!fStarted) {
         fCursor = fList->fLast;
         fStarted = kTRUE;
      }
      fCurCursor = fCursor;
      if (fCursor) fCursor = fCursor->getPrevious();
   }

   if (fCurCursor) return fCurCursor->getData();
   return 0;
}




  HMdcCalLink* HMdcCell :: getHit(Int_t idx){

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcCell::getHit");
#endif

    if (idx < fNHits) return fHits[idx];
    return NULL;
}


  void HMdcCell :: setHit(HMdcCalLink* link, Int_t idx){
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcCell::setHit");
#endif
    
    if (fHits[idx] == NULL) fNHits++;
    fHits[idx] = link;

  }



  void HMdcCell :: removeHit(HMdcCalLink* link){
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcCell::removeHit");
#endif
    if(!link) return;

    //Int_t hitnum = link->getData()->getHitNumber();
    for(Int_t i=0; i<fNHits; i++){
      if(fHits[i]==link){
	for(Int_t j=i; j<fNHits-1; j++) fHits[j] = fHits[j+1];
	fHits[fNHits-1] = NULL;
	fNHits--;
	return;
      }
    }

  }


  void HMdcCell :: removeHits(void){
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcCell::removeHits");
#endif

    for(Int_t i=0; i<fNHits; i++) fHits[i] = NULL;
    fNHits = 0;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcCell::removeHits");
#endif
  }






