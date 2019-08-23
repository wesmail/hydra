#include "hrecevent.h"
#include "hades.h"
#include <TBrowser.h>
#include <TBranchClones.h>
#include "htree.h"
#include "htrack.h"
#include "hpartialevent.h"
#include "hdebug.h"

//*-- Author : Manuel Sanchez 
//*-- Modified : 02/11/98 by Manuel Sanchez
//*-- Modified : 24/03/98 by Manuel Sanchez
//*-- Copyright : GENP (Univ. Santiago de Compostela)

////////////////////////////////////
//HRecEvent
// 
// A HRecEvent is an event under reconstruction, and ,in particular, a 
//reconstructed event. These events can be in different reconstruction levels,
//the reconstruction level of a HRecEvent is controlled by setRecLevel() and
//getRecLevel(); the reconstruction levels are identified by global constants
//(rlRaw, rlHit ...)
//
// As for the data, a HRecEvent holds reconstructed tracks, an event header
//and a list of HPartialEvent objects.
//
////////////////////////////////

ClassImp(HRecEvent)

HRecEvent::HRecEvent(void) : HEvent("Event","Event under Reconstruction") {
  //Allocates a new HRecEvent
  fRecLevel=rlUndefined;
  fTracks=new TClonesArray("HTrack");
  fPartialEvs=new TObjArray(10);
  fNTracks=0;
}

HRecEvent::~HRecEvent(void) {
  //Cleans the HRecEvent for its deallocation
  if (fPartialEvs) {fPartialEvs->Delete(); delete fPartialEvs;}
  if (fTracks) {fTracks->Clear(); delete fTracks;}
}

void HRecEvent::activateBranch(TTree *tree,Int_t splitLevel) {
  // see HEvent::activateBranch
   HPartialEvent **par;
#if DEBUG_LEVEL > 2
   gDebuger->enterFunc("HRecEvent::activateBranch");
#endif   
   if (tree==NULL) return;

   if (splitLevel==0) {
   } else if (splitLevel==1) {
     Int_t i;
     TObjArray &vector=*fPartialEvs;
     for(i=0;i<=fPartialEvs->GetLast();i++) {
       par=(HPartialEvent **)&vector[i];
       if ((*par)!=NULL) {
#if DEBUG_LEVEL>2
	 gDebuger->message("Activating %s",(*par)->GetName());
	 gDebuger->message("fPartialEvs=%p",fPartialEvs);
#endif
	 tree->SetBranchAddress((*par)->GetName(),par);
	 tree->SetBranchStatus((*par)->GetName(),1);
       }
     }
   } else if (splitLevel==2) {
     Int_t i;
     TObjArray &vector=*fPartialEvs;
     for(i=0;i<=fPartialEvs->GetLast();i++) {
       par=(HPartialEvent **)&(vector[i]);
       if ((*par)!=NULL) {
	 tree->SetBranchAddress((*par)->GetName(),par);
	 tree->SetBranchStatus((*par)->GetName(),1);
	 (*par)->activateBranch(tree,splitLevel);
       }
     }
   }
#if DEBUG_LEVEL>2
   gDebuger->leaveFunc("HRecEvent::activateBranch");
#endif
}
void HRecEvent::makeBranch(TBranch *parent) {
  //see HEvent::makeBranch 
   HPartialEvent **par;
   TBranch *b=NULL;
   Int_t sl=2;
   
#if DEBUG_LEVEL > 2
   gDebuger->enterFunc("HRecEvent::makeBranch");
#endif
   if (parent==NULL) return;

   sl=gHades->getSplitLevel();
   if (sl==0) {
   } else if (sl==1) {
     Int_t i;
     TObjArray &vector=*fPartialEvs;
     for(i=0;i<=fPartialEvs->GetLast();i++) {
       par=(HPartialEvent **)&vector[i];
       if ((*par)!=NULL) {
	 b=gHades->getTree()->addBranch( (*par)->GetName(),
					 (*par)->ClassName(),par,32000,0);
       }
     }
   } else if (sl==2) {
     Int_t i;
     TObjArray &vector=*fPartialEvs;
     for(i=0;i<=fPartialEvs->GetLast();i++) {
       par=(HPartialEvent **)&(vector[i]);
       if ((*par)!=NULL) {
	 b=gHades->getTree()->addBranch( (*par)->GetName(),
					 (*par)->ClassName(),par,4000,1);
	 (*par)->makeBranch(b);
       }
     }
   }
#if DEBUG_LEVEL>2
   gDebuger->leaveFunc("HRecEvent::activateBranch");
#endif
}

void HRecEvent::Clear(void) {
  //Clears the data in the event (i.e. clears the internal buffers...)
     TIter next(fPartialEvs);
     HPartialEvent *ev;
     while ( (ev=(HPartialEvent *)next())!=NULL) {
              ev->Clear();
     }
   if (fTracks) fTracks->Clear();
   fNTracks=0;
}

void HRecEvent::clearAll(Int_t level) {
  //Clears the data in the event and the event structure (list of subevents...)
  if (level==0) {
    if (fPartialEvs) fPartialEvs->Delete();
  } else {
    TIter next(fPartialEvs);
    HPartialEvent *ev;
    while ((ev=(HPartialEvent *)next())!=NULL) {
      ev->clearAll(level);
    }
  }
  if (fTracks) fTracks->Clear();
  fNTracks=0;
}

HTrack *HRecEvent::newTrack(void) {
  //Returns an pointer to a new HTrack object
  TClonesArray &tracks=*fTracks;
  new(tracks[fNTracks++]) HTrack;
  return ((HTrack *)tracks[fNTracks-1]);
}

void HRecEvent::addTrack(HTrack &aTrack) {
  // Adds the track aTrack to the list of reconstructed tracks
  TClonesArray &tracks=*fTracks;
  new (tracks[fNTracks++]) HTrack(aTrack);

}

HTrack *HRecEvent::getTrack(UInt_t aId) {
  // Returns the track identified by aId (the position in the track list) 
  HTrack *track;
  track=(HTrack *)fTracks->At(aId);
  return(track);
}

void HRecEvent::clearTracks(void) {
  // Clears the track list
  fNTracks=0;
  if (fTracks) fTracks->Clear();
}

Int_t HRecEvent::getRecLevel(void) {
  // Returns the reconstruction level for this event
 return(fRecLevel);
}

void HRecEvent::setRecLevel(Int_t aRecLevel) {
  // Sets the reconstruction level for the event
  fRecLevel=aRecLevel;
}

HPartialEvent *HRecEvent::getPartialEvent(Int_t idx) {
  // Returns a pointer to the partial event with number idx.
  if ((idx>>kBitCategorySize)>fPartialEvs->GetLast()) return NULL;
  return ((HPartialEvent *)fPartialEvs->At(idx>>kBitCategorySize));
}


HCategory *HRecEvent::getCategory(Cat_t aCat) {
  //Returns the category identified by aCat in the correct Partial event
  HPartialEvent *ev;
  HCategory *cat;
  ev=getPartialEvent(aCat);
  if (!ev) return NULL;
  cat=ev->getCategory(aCat);
  return cat;
  //  return getPartialEvent(aCat)->getCategory(aCat);
}

Bool_t HRecEvent::addCategory(Cat_t aCat,HCategory *cat,Option_t opt[]) {
  // Adds a new category to the event. The partial event it belongs to is 
  //determined by aCat; if this partial event doesn't exist, one is created
  //with the name given in opt.
  HPartialEvent *event;
  event=getPartialEvent(aCat);
  if (!event) {
    Int_t aCatBase = aCat - aCat%kCategorySize;    // extract base category
    event=addPartialEvent(aCatBase,opt,opt);
    if (!event) return kFALSE;
  }
  return event->addCategory(aCat,cat,opt);
}

HPartialEvent *HRecEvent::addPartialEvent(Cat_t eventCat,
					  const Text_t *name,
					  const Text_t *title) {
  // Adds a new HPartialEvent to the list of HPartialEvent objects in the 
  //HRecEvent
  //
  //Input:
  // eventCat  ---> Base category for the event (i.e. for Mdc it is catMdc)
  // name      ---> Name of the new partial event (used to build Root trees)
  // title     ---> Title of the new partial event
   HPartialEvent *event;
   event=new HPartialEvent(name,title,eventCat);
   if (event) fPartialEvs->AddAtAndExpand(event,eventCat>>kBitCategorySize);
   return event;
}

void HRecEvent::Browse(TBrowser *b) {
  // Event browser.
   TIter next(fPartialEvs);
   HPartialEvent *ev;
   
   if (fTracks) b->Add(fTracks,"Tracks");
   while ( (ev=(HPartialEvent *)next())!=NULL) {
      b->Add(ev);
   }
}

// void HRecEvent::Streamer(TBuffer &R__b)
// {
//    // Stream an object of class HRecEvent.
//    if (R__b.IsReading()) {
//       Version_t R__v = R__b.ReadVersion(); if (R__v) { }
//       HEvent::Streamer(R__b);
//       R__b >> fRecLevel;
//       R__b >> fNTracks;
//       fHeader.Streamer(R__b);
//       //R__b >> fPartialEvs;
//       fTracks->Streamer(R__b);
//    } else {
//       R__b.WriteVersion(HRecEvent::IsA());
//       HEvent::Streamer(R__b);
//       R__b << fRecLevel;
//       R__b << fNTracks;
//       fHeader.Streamer(R__b);
//       //R__b << fPartialEvs;
//       fTracks->Streamer(R__b);
//    }
// }
