#ifndef HRECEVENT_H
#define HRECEVENT_H

#include "hevent.h"
#include <TClonesArray.h>
#include "heventheader.h"
#include <TObjArray.h>

class HTrack;
class HPartialEvent;

class HRecEvent : public HEvent {
protected:
  Int_t fRecLevel; //Reconstruction level for this event
  Int_t fNTracks; //Number of reconstructed tracks in this event
  HEventHeader fHeader; //Event header
  TObjArray *fPartialEvs; //- Partial events for this event
  TClonesArray *fTracks;//- Reconstructed tracks for this event
public:
  HRecEvent(void);
  ~HRecEvent(void);
  void makeBranch(TBranch *parent);
  void activateBranch(TTree *tree,Int_t splitLevel);
  HTrack *newTrack(void);
  void addTrack(HTrack &aTrack);
  HTrack *getTrack(UInt_t aId);
  void clearTracks(void);
  HEventHeader *getHeader(void) {return(&fHeader);}
  void setRecLevel(Int_t aRecLevel);
  Int_t getRecLevel(void);
  HPartialEvent *addPartialEvent(Cat_t eventCat,const Text_t *name,
				 const Text_t *title);
  HPartialEvent *getPartialEvent(Int_t idx);
  HCategory *getCategory(Cat_t aCat);
  Bool_t addCategory(Cat_t aCat,HCategory *cat,Option_t opt[]);
  Bool_t IsFolder(void) {return kTRUE;}
  void Browse(TBrowser *b);
  void Clear(void);
  void clearAll(Int_t level);
  ClassDef(HRecEvent,1) //An event under reconstruction
};

#endif /* !HRECEVENT_H */


