// File: hldunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 97/12/02 14:12:11
//
using namespace std;
#ifndef HLDUNPACK_H
#define HLDUNPACK_H

#include <TObject.h>
#include <iostream> 
#include <iomanip>

class HCategory;
class HldSubEvt;

class HldUnpack : public TObject {
public:
  HldUnpack(void);
  virtual ~HldUnpack(void);
  virtual Int_t getSubEvtId(void) const = 0;
  typedef HldSubEvt ** HPP;
  HPP const getpSubEvt(void);
  virtual Int_t execute(void);
  virtual Bool_t init(void)=0;
  virtual Bool_t reinit(void) { return kTRUE; }
  virtual Bool_t finalize(void) {return kTRUE;}
  void setCategory(HCategory *aCat);
protected:  
  HldSubEvt* pSubEvt; //! pointer to subevent where data are read from
  HCategory *pRawCat; //! pointer to category where data will be stored;
public: 
  ClassDef(HldUnpack, 1)  // base class for the raw data unpackers
};

#endif /* !HLDUNPACK_H */  

