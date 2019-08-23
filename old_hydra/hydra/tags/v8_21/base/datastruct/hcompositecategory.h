#pragma interface
#ifndef HCOMPOSITECATEGORY_H
#define HCOMPOSITECATEGORY_H

#include <hcategory.h>


class HCompositeCategory : public HCategory {
protected:

public:
  HCompositeCategory(void);
  ~HCompositeCategory(void);
  virtual void setLenghts(Int_t loc, Int_t val){;}
  virtual void setOffsets(Int_t loc, Int_t val){;}
  virtual Int_t* getOffsets() { return 0;}
  virtual Int_t* getLenghts() { return 0;} 
  virtual TArrayI getMaxSizes(void){return 0;}
  virtual Int_t  getMaxSize(void){return -1;}
  virtual void  setPattern(Int_t* off, Int_t* len){;}
  virtual Int_t init(){ return -1;}   

  ClassDef(HCompositeCategory,1) //Generic comp. data category
};

#endif /* !HCOMPOSITECATEGORY */
