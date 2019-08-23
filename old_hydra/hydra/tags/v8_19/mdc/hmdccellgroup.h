#ifndef HMDCCELLGROUP_H
#define HMDCCELLGROUP_H

#include "TObject.h"
#include <string.h>

class HMdcCellGroup : public TObject {
 protected:
  static UChar_t nCellsLookUp[256];         //!
  static UChar_t cellPosLookUp[4][256];     //!
  static Bool_t lookUpsInitialized;         //!
  UShort_t *pLayer; //!pointer to layer information.
  
  void initLookups(void);  
  HMdcCellGroup(UShort_t *layerBuffer) {
    if (!lookUpsInitialized) initLookups();
    pLayer=layerBuffer;
  }
 public:
  ~HMdcCellGroup(void) { }
  inline Int_t getNCells(Int_t layer);
  inline Int_t getCell(Int_t layer,Int_t idx);
  inline Int_t getSignId(Int_t layer,Int_t idx);
  inline void setSignId(Int_t layer,Int_t cell,
		       Int_t t1,Int_t t2=0,Int_t t3=0,Int_t t4=0);
  
  ClassDef(HMdcCellGroup,1) //A group of cells in layers
};

class HMdcCellGroup6 : public HMdcCellGroup {
 protected:
  UShort_t iLayer[6];
 public:
  HMdcCellGroup6(void) : HMdcCellGroup(iLayer) 
    { memset(iLayer,0,6*sizeof(UShort_t)); }
  ~HMdcCellGroup6(void) {}
  void clear(void) { memset(iLayer,0,6*sizeof(UShort_t)); }
  Int_t getSumWires()
  {
      Int_t sum=0;
      for(Int_t i=0;i<6;i++){sum+=getNCells(i);}
      return sum;
  }
  Int_t getNLayers()
  {
      Int_t sum=0;
      for(Int_t i=0;i<6;i++){if(getNCells(i)>0)sum++;}
      return sum;
  }
  ClassDef(HMdcCellGroup6,1) //Group of cells in 6 layers
};

class HMdcCellGroup12 : public HMdcCellGroup {
 protected:
  UShort_t iLayer[12];
 public:
  HMdcCellGroup12(void) : HMdcCellGroup(iLayer)
    { memset(iLayer,0,12*sizeof(UShort_t));}
  ~HMdcCellGroup12(void) {}
  void clear(void) { memset(iLayer,0,12*sizeof(UShort_t)); }
  Int_t getSumWires()
  {
      Int_t sum=0;
      for(Int_t i=0;i<12;i++){sum+=getNCells(i);}
      return sum;
  }
  Int_t getSumWires(UInt_t mod)
  {
      Int_t sum=0;
      if(mod==0)for(Int_t i=0;i<6;i++) {sum+=getNCells(i);}
      if(mod==1)for(Int_t i=6;i<12;i++){sum+=getNCells(i);}
      return sum;
  }
  Int_t getNLayers()
  {
      Int_t sum=0;
      for(Int_t i=0;i<12;i++){if(getNCells(i)>0)sum++;}
      return sum;
  }
  Int_t getNLayers(UInt_t mod)
  {
      Int_t sum=0;
      if(mod==0)for(Int_t i=0;i<6;i++){if(getNCells(i)>0)sum++;}
      if(mod==1)for(Int_t i=6;i<12;i++){if(getNCells(i)>0)sum++;}
      return sum;
  }
  ClassDef(HMdcCellGroup12,1) //group of cells in 12 layers.
};


//----------------------- Inlines ------------------------------
inline Int_t HMdcCellGroup::getNCells(Int_t layer) {
  return nCellsLookUp[pLayer[layer] & 0xFF];
}

inline Int_t HMdcCellGroup::getCell(Int_t layer,Int_t idx) {
  return ((pLayer[layer]>>8) & 0xFF)+cellPosLookUp[idx][pLayer[layer] & 0xFF];
}

inline Int_t HMdcCellGroup::getSignId(Int_t layer,Int_t idx) {
  //Returns 1 for time1 and 2 for time2 in cell basecell+idx; starting with 
  //idx=0
  if (idx < getNCells(layer) ) {
    Int_t shift=cellPosLookUp[idx][pLayer[layer] & 0xFF]*2;
    return (pLayer[layer] >> shift) & 0x3;
  }
  return 0;
}

inline void HMdcCellGroup::setSignId(Int_t layer,Int_t cell,
			      Int_t t1,Int_t t2,Int_t t3,Int_t t4) {
  //Sets the signal ids and base cell number for layer "layer"
  //Input:
  //  layer --> Number of layer to set
  //  cell  --> Number of the first cell fired in this layer
  //  t1 - t4 --> SignalId for cells: "cell"+0, "cell"+1 ... "cell"+4
  //              Possible values are:
  //                0 --> This cell has not fired
  //                1 --> First signal (time/distance) was used
  //                2 --> Second signal (time/distance) was used
  pLayer[layer]=((cell & 0xFF) << 8) | ((t4 & 0x3)<<6) | 
    ((t3 & 0x3)<<4) | ((t2 & 0x3)<<2) | ((t1 & 0x3));
}

#endif
