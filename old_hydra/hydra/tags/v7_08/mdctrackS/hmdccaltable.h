#ifndef HMDCCALTABLE_H
#define HMDCCALTABLE_H

//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HMdcCalLinkList                                                      //
//                                                                      //
// A doubly linked list of HMdcCal3. 
//                                                                      //
//////////////////////////////////////////////////////////////////////////

#include "TCollection.h"
#include "TObject.h"
#include "hmdccal3.h"


class HMdcCalLink;
class HMdcCalLinkListIter;


class HMdcCell{

 private: 
  HMdcCalLink** fHits;         // ! pointer to hits 
  Int_t fNHits;                // number of hits in this cell

 public:
  HMdcCell(void) : fNHits(0) {fHits = new HMdcCalLink*[10];}
  HMdcCell(Int_t maxHits) : fNHits(0){fHits = new HMdcCalLink*[maxHits];}
  ~HMdcCell(void){delete [] fHits;}
  HMdcCalLink* getHit(Int_t idx);
  void setHit(HMdcCalLink* link, Int_t idx);

  void removeHit(HMdcCalLink* link);
  void removeHits(void);
  Int_t getNHits(void){return fNHits;}

};




class HMdcCalLinkList : public TObject {

friend  class HMdcCalLinkListIter;

private:
   HMdcCell* fCells;         //! cells storing pointers to hits 
   HMdcCalLink  *fFirst;     //! pointer to first entry in linked list
   HMdcCalLink  *fLast;      //! pointer to last entry in linked list
   Int_t fMaxHitsPerCell;    // maximum number of hits per cell
   Int_t fCapacity;          // maximum number of cells


   HMdcCalLink* newLink(HMdcCal3* hit, HMdcCalLink* prev=0);
   void  add(HMdcCal3 *hit);

public:
   HMdcCalLinkList():fMaxHitsPerCell(10), fCapacity(208) { fFirst = fLast = 0; fCells = new HMdcCell[fCapacity];}

   HMdcCalLinkList(Int_t maxHits, Int_t cap) :fMaxHitsPerCell(maxHits),fCapacity(cap) { fFirst = fLast = 0; fCells = new HMdcCell[fCapacity];}

   ~HMdcCalLinkList();
   void  clear(Option_t* option = "");
   void  deleteList();
   HMdcCalLinkListIter *MakeIterator(Bool_t dir = kIterForward) const;

   void  addHits(HMdcCal3 *hit1, HMdcCal3* hit2);
   void swap(HMdcCalLink* link1, HMdcCalLink* link2);
   void remove(HMdcCal3 *hit);
   void remove(HMdcCalLink* lnk);
   void  deleteLink(HMdcCalLink *lnk);
 
   HMdcCalLink *getFirst() const { return fFirst; }
   HMdcCalLink *getLast() const { return fLast; }


   Int_t getCapacity(void) {return fCapacity;}
   HMdcCell* getCell(Int_t idx){
     if(idx<fCapacity) return (fCells + idx);
     return NULL;
   }

   ClassDef(HMdcCalLinkList,0)  //Doubly linked list
};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
// HMdcCalLink                                                          //
//                                                                      //
// Wrapper around an HMdcCal3 so it can be stored in a                //
//   HMdcCalLinkList.                                                   //
//                                                                      //
//////////////////////////////////////////////////////////////////////////
class HMdcCalLink {

friend  class HMdcCalLinkList;

private:
 HMdcCalLink *fNext;        //! pointer to next element in list
 HMdcCalLink *fPrev;        //! pointer to previous element in list 
 HMdcCalLink *fPartner;     //! pointer to hit partner
 HMdcCal3  *fData;        //! pointer to hit

protected:
   HMdcCalLink() { fNext = fPrev = this; fData = 0; fPartner = 0;}

public:
   HMdcCalLink(HMdcCal3 *hit) : fNext(0), fPrev(0), fPartner(0), fData(hit) {hit->setLink(this);}
   HMdcCalLink(HMdcCal3 *hit, HMdcCalLink *lnk);
   virtual ~HMdcCalLink() {;}

   HMdcCal3*  getData() const { return fData; }
   void         setData(HMdcCal3 *hit) { fData = hit; }
   HMdcCalLink  *getNext() { return fNext; }
   HMdcCalLink  *getPrevious() { return fPrev; }
   HMdcCalLink *getPartner(){ return fPartner;}
   void setNext(HMdcCalLink * link){fNext = link;}
   void setPrevious(HMdcCalLink* link){fPrev = link;}
   void setPartner(HMdcCalLink* link){fPartner = link;}

   ClassDef(HMdcCalLink,0)  //Linked list


};


//////////////////////////////////////////////////////////////////////////
//                                                                      //
//                                                                      //
//                                                                      //
// Iterator of linked list.                                             //
//                                                                      //
//////////////////////////////////////////////////////////////////////////

class HMdcCalLinkListIter {

private:
   const  HMdcCalLinkList       *fList;         //! list being iterated
   HMdcCalLink          *fCurCursor;    //! current position in list
   HMdcCalLink          *fCursor;       //! next position in list
   Bool_t             fDirection;    //! iteration direction
   Bool_t             fStarted;      //! iteration started


 public:

   HMdcCalLinkListIter() : fList(0), fCursor(0), fStarted(kFALSE) { }

   //public:
   HMdcCalLinkListIter(const HMdcCalLinkList *l, Bool_t dir = kIterForward);
   //   HMdcCalLinkListIter(const HMdcCalLinkListIter &iter);
   virtual ~HMdcCalLinkListIter() { }

   HMdcCal3*  Next();
   void Reset() { fStarted = kFALSE; }

   ClassDef(HMdcCalLinkListIter,0)  //Linked list iterator
};

#endif
