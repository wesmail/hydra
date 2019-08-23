// File: hldunpack.h
//
// Author: Walter Karig <W.Karig@gsi.de>
// Last update: 97/12/02 14:12:11
//

#ifndef HLDUNPACK_H
#define HLDUNPACK_H

#include <TObject.h>
#include <iostream.h>

class HCategory;
class HldSubEvt;

class HldUnpack : public TObject {
public:
  HldUnpack(void);
  virtual ~HldUnpack(void);
  virtual int getSubEvtId(void) const = 0;
  typedef HldSubEvt ** HPP;
  HPP const getpSubEvt(void);
  virtual Int_t execute(void);
  virtual Bool_t init(void)=0;
  void setCategory(HCategory *aCat);
protected:  
  HldSubEvt* pSubEvt; //! Buffer where data are read from
  HCategory *pRawCat; //The category where data will be stored;
public: 
  ClassDef(HldUnpack, 1)          // base class for unpack
};

#endif /* !HLDUNPACK_H */  

