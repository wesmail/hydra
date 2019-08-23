#include "hcategorysplititer.h"

HCategorySplitIter::HCategorySplitIter(void) {

}

HCategorySplitIter::HCategorySplitIter(HCategorySplit *cat,
						   Bool_t dir) {
  fCat=cat;
  Reset();
}

HCategorySplitIter::~HCategorySplitIter(void) {
}

TCollection *HCategorySplitIter::GetCollection(void) const {
  return fCat->fTableArray;
}

TObject *HCategorySplitIter::Next(void) {
  TObject *r=NULL;
  if (!fFinnished) {
    r=fCurrentClones->UncheckedAt(fCloneCursor);
    fCloneCursor++;
    if (!(fCloneCursor<fCloneEnd)) {
      fCurrentClones=NULL;
      //printf("clones acabado\n");
      do {
	fCursor++;
	if (!(fCursor<fCursorEnd)) {fFinnished=kTRUE; return r;}
	while ( (fCurrentClones=(TClonesArray *)
		  (fCat->fTableArray->UncheckedAt(fCursor)))==NULL) { 
	  fCursor++;
	  if (!(fCursor<fCursorEnd)) {fFinnished=kTRUE; return r;}
	}
      } while ((fCloneEnd=fCurrentClones->GetEntriesFast())==0);
      fCloneCursor=fCloneStart;
    }      
  }
  //if (r) printf("Not null\n"); else printf("Null\n");
  return r;
}

void HCategorySplitIter::Reset(void) {
  fCursor=-1;
  fCursorEnd=fCat->fTableArray->GetEntriesFast();
  fCurrentClones=NULL;
  fFinnished=kFALSE;
  do {
    fCursor++;
    if (!(fCursor<fCursorEnd)) {fFinnished=kTRUE; return;}
    while ( (fCurrentClones=(TClonesArray *)
	      (fCat->fTableArray->UncheckedAt(fCursor)))==NULL) { 
      fCursor++;
      if (!fCursor<fCursorEnd) {fFinnished=kTRUE; return;}
    }
  } while ((fCloneEnd=fCurrentClones->GetEntriesFast())==0);
  fCursorStart=fCursor;
  fCloneCursor=fCloneStart=0;
}

Bool_t HCategorySplitIter::gotoLocation(HLocation &loc) {
  Int_t i,prod=1;

  if (loc.getNIndex()==0) {
    Reset();
  } else {
    fCurrentClones=NULL;
    fFinnished=kFALSE;
    fCursor=loc[0];
    for(i=1;i<loc.getNIndex();i++) 
      fCursor=fCursor*fCat->fSizesArray.fArray[i]+loc[i];
    for(;i<fCat->fSizesArray.fN;i++) prod*=fCat->fSizesArray.fArray[i];
    fCursor*=prod;
    fCursorEnd=prod+fCursor;
    fCursor--;
    
    do {
      fCursor++;
      if (!(fCursor<fCursorEnd)) {fFinnished=kTRUE; return kFALSE;}
      while ( (fCurrentClones=(TClonesArray *)
	       (fCat->fTableArray->UncheckedAt(fCursor)))==NULL) { 
	fCursor++;
	if (!fCursor<fCursorEnd) {fFinnished=kTRUE; return kFALSE;}
      }
    } while ((fCloneEnd=fCurrentClones->GetEntriesFast())==0);
    fCursorStart=fCursor;
    fCloneCursor=fCloneStart=0;
  }
  return kTRUE;
}

HLocation &HCategorySplitIter::getLocation(void) {
  return fLoc;
}


ClassImp(HCategorySplitIter)
