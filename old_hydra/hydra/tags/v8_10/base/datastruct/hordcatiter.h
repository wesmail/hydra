#ifndef HORDCATITER_H
#define HORDCATITER_H

#include "hiterator.h"
#include "hordcategory.h"

class HOrdCatIter : public HIterator {
private:
  HOrdCatIter(void);
protected:
  HOrdCategory* ordCat; // corresponding category
  HLocation* fCurrentLoc; //Current location in the category
  HOrdClonesArray *fArray; //array being iterated
  TObject *fCurrent; //Current object
  Int_t  fCurrentOff,fCurrentLen; 
  Bool_t fDir;
  Int_t cursor; 
  
public:
  HOrdCatIter(HOrdCategory *cat,Bool_t dir=kIterForward);
  HOrdCatIter(HLocation *loc,HOrdCategory *cat,Bool_t dir=kIterForward);
  virtual ~HOrdCatIter(void);
  TCollection *GetCollection(void) const;
  TObject *Next(void);
  void Reset(void);
  Bool_t gotoLocation(HLocation &loc); // ??
  HLocation &getLocation(void);


  ClassDef(HOrdCatIter,1) //Iterator for a HOrdCategory
};

#endif /* !HORDCATITER_H */
