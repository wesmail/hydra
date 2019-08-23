#ifndef HMDCLISTCELLS_H
#define HMDCLISTCELLS_H

#include "hmdctbarray.h"
#include "TObject.h"
//#include "TMath.h"

class HMdcLayListCells : public TObject {
  friend class HMdcLayListCellsAndTimes;
  friend class HMdcModListCellsAndTimes;
  friend class HMdcSecListCellsAndTimes;
  friend class HMdcEvntListCellsAndTimes;
  protected:
    enum { arrSize=56,    // array size 
           maxCells=224}; // max.number of cells in layer (= arrSize*4)
    UChar_t arr[arrSize];
    UChar_t* arrEnd;
    Float_t* drTimes;
  public:
    HMdcLayListCells() {init();}
    ~HMdcLayListCells(void) {}
    void copyListOfCells(HMdcLayListCells& lst);
    inline Bool_t boundsOk(Int_t cell) const;
    inline void clear(void);
    inline Bool_t setTime(Int_t cell,UChar_t time);
    inline void delTime(Int_t cell,UChar_t time);
    inline Int_t getNCells(void) const;
    inline Int_t getCell(Int_t idx) const;
    inline Int_t getFirstCell(void) const;
    inline Int_t getLastCell(void) const;
    inline Int_t next(Int_t prCell) const;
    inline Bool_t nextCell(Int_t& cell) const;
    inline Int_t previous(Int_t prCell) const;
    inline UChar_t getTime(Int_t cell) const;
    inline Bool_t isCell(Int_t cell) const;
    void print(void) const;
    
    inline Bool_t addTime(Int_t cell, Float_t timeValue);
    inline Bool_t nextTime(Int_t& cell, Float_t& timeValue) const;
    inline Float_t getTimeValue(Int_t cell) const;

  protected:
    inline void init(void);
    void setDrTimesArr(Float_t* tm) {drTimes=tm;}

  ClassDef(HMdcLayListCells,0) //List of all cells in layer
};

inline void HMdcLayListCells::copyListOfCells(HMdcLayListCells& lst) {
  memcpy(arr,lst.arr,arrSize);
}
  
inline Bool_t HMdcLayListCells::boundsOk(Int_t cell) const {
  return (cell>=0 && cell < maxCells) ? kTRUE : kFALSE;
}

inline void HMdcLayListCells::init(void) {
  arrEnd=arr+arrSize-1;
  drTimes=0;
  clear();
}

inline void HMdcLayListCells::clear(void) {
  HMdcTBArray::clear(arr,arrEnd);
}

inline Bool_t HMdcLayListCells::setTime(Int_t cell,UChar_t time) {
  if(!boundsOk(cell)) return kFALSE;
  HMdcTBArray::set(arr,cell,time);
  return kTRUE;
}

inline void HMdcLayListCells::delTime(Int_t cell,UChar_t time) {
  if(boundsOk(cell)) HMdcTBArray::unset(arr,cell,time);
}

inline Int_t HMdcLayListCells::getNCells(void) const {
  return HMdcTBArray::getNSet(arr,arrEnd);
}

inline Int_t HMdcLayListCells::getCell(Int_t idx) const {
  return HMdcTBArray::position(arr,arrEnd,idx);
}

inline Int_t HMdcLayListCells::getFirstCell(void) const {
  return HMdcTBArray::first(arr,arrEnd);
}

inline Int_t HMdcLayListCells::getLastCell(void) const {
  return HMdcTBArray::last(arr,arrEnd);
}

inline Int_t HMdcLayListCells::next(Int_t prCell) const {
  return HMdcTBArray::next(arr,arrEnd,prCell);
}

inline Bool_t HMdcLayListCells::nextCell(Int_t& cell) const {
  cell=HMdcTBArray::next(arr,arrEnd,cell);
  return (cell<0) ? kFALSE:kTRUE;
}

inline Int_t HMdcLayListCells::previous(Int_t prCell) const {
  return HMdcTBArray::previous(arr,arrEnd,prCell);
}


inline UChar_t HMdcLayListCells::getTime(Int_t cell) const {
  if(boundsOk(cell)) return HMdcTBArray::get(arr,cell);
  return 0;
}

inline Bool_t HMdcLayListCells::isCell(Int_t cell) const {
  return (getTime(cell)) ? kTRUE:kFALSE;
}

inline Bool_t HMdcLayListCells::addTime(Int_t cell, Float_t timeValue) {
  if(drTimes && boundsOk(cell)) {
    HMdcTBArray::set(arr,cell,1);  // time1 only !!!
    drTimes[cell]=timeValue;
    return kTRUE;
  } else return kFALSE;
}

inline Bool_t HMdcLayListCells::nextTime(Int_t& cell, Float_t& timeValue) 
    const {
  if(!nextCell(cell) || drTimes==0) return kFALSE;
  timeValue=drTimes[cell];
  return kTRUE;
}

inline Float_t HMdcLayListCells::getTimeValue(Int_t cell) const {
  return (drTimes && getTime(cell)) ? drTimes[cell]:-9999.0;
}

//-------------------------------------------------------------------

class HMdcLayListCellsAndTimes : public HMdcLayListCells {
  friend class HMdcModListCellsAndTimes;
  protected:
    Float_t times[maxCells];   // drift times list (for MdcCal1::time1 only !!!)
  public:
    HMdcLayListCellsAndTimes() {drTimes=times;}
    ~HMdcLayListCellsAndTimes(void) {}

  ClassDef(HMdcLayListCellsAndTimes,0) //List of all cells in layer
};

// - - - - - - - - - - - - - - - - - - - - - - - - - -

class HMdcModListCells : public TObject {
  friend class HMdcSecListCellsAndTimes;
  protected:
    HMdcLayListCells list[6];
    Float_t* drTimes6l;
  public:
    HMdcModListCells(void) {clear(); drTimes6l=0;}
    ~HMdcModListCells(void) {}
    HMdcLayListCells& operator[](Int_t l) {
      if(nLayOk(l)) return list[l];
      HMdcLayListCells* null=0;
      return *null;
    }
    inline void clear(void);
    inline Bool_t setTime(Int_t lay, Int_t cell, UChar_t time);
    inline void delTime(Int_t lay, Int_t cell, UChar_t time);
    inline Int_t getNCells(void) const;
    inline Bool_t nextCell(Int_t& lay, Int_t& cell) const;
    inline UChar_t getTime(Int_t lay, Int_t cell) const;
    inline Bool_t isCell(Int_t lay, Int_t cell) const;
    inline Int_t getNLayers(void) const;
    void print(void) const;
    
    inline Bool_t addTime(Int_t l, Int_t c, Float_t tm);
    inline Bool_t nextTime(Int_t& l, Int_t& c, Float_t& tm) const;
    inline Float_t getTimeValue(Int_t l, Int_t c) const;
  protected:
    inline Bool_t nLayOk(Int_t lay) const;

  ClassDef(HMdcModListCells,0) //List of all cells in layer
};

inline void HMdcModListCells::clear(void) {
  for(Int_t l=0;l<6;l++) list[l].clear();
}

inline Bool_t HMdcModListCells::nLayOk(Int_t lay) const {
  return (lay>=0&&lay<6) ? kTRUE:kFALSE;
}

inline Bool_t HMdcModListCells::setTime(Int_t lay, Int_t cell, UChar_t time) {
  if(nLayOk(lay)) return list[lay].setTime(cell,time);
  return kFALSE;
}

inline Bool_t HMdcModListCells::nextCell(Int_t& lay, Int_t& cell) const {
  if(lay<0) {
    lay=0;
    cell=-1;
  }
  for(;lay<6;lay++) if(list[lay].nextCell(cell)) return kTRUE;
  lay=-1;
  return kFALSE;
}

inline void HMdcModListCells::delTime(Int_t lay, Int_t cell, UChar_t time) {
  if(nLayOk(lay)) list[lay].delTime(cell,time);
}

inline Int_t HMdcModListCells::getNCells(void) const {
  Int_t nCells=0;
  for(Int_t l=0;l<6;l++) nCells+=list[l].getNCells();
  return nCells;
}

inline UChar_t HMdcModListCells::getTime(Int_t lay, Int_t cell) const {
  if(nLayOk(lay)) return list[lay].getTime(cell);
  return 0;
}

inline Int_t HMdcModListCells::getNLayers(void) const {
  Int_t nLay=0;
  for(Int_t l=0;l<6;l++) if(list[l].getFirstCell()>=0) nLay++;
  return nLay;
}

inline Bool_t HMdcModListCells::isCell(Int_t lay, Int_t cell) const {
  if(nLayOk(lay)) return list[lay].isCell(cell);
  return kFALSE;
}


inline Bool_t HMdcModListCells::addTime(Int_t l, Int_t c, Float_t tm) {
  if(nLayOk(l)) return list[l].addTime(c,tm);
  return kFALSE;
}

inline Bool_t HMdcModListCells::nextTime(Int_t& l, Int_t& c, Float_t& tm) 
    const {
  if(!nextCell(l,c) && drTimes6l) return kFALSE;
  tm=getTimeValue(l,c);
  return kTRUE;
}

inline Float_t HMdcModListCells::getTimeValue(Int_t l,Int_t c) 
    const {
  if(nLayOk(l)) return list[l].getTimeValue(c);
  return -9999.0;
}

//-------------------------------------------------------------------

class HMdcModListCellsAndTimes : public HMdcModListCells {
  protected:
    Float_t times[6][HMdcLayListCells::maxCells]; // drift times list (time1!!!)
  public:
    HMdcModListCellsAndTimes(void) {setDrTimesArr();}
    ~HMdcModListCellsAndTimes(void) {}
  protected:
    void setDrTimesArr(void);
  
  ClassDef(HMdcModListCellsAndTimes,0) //List of all cells in layer
};

// - - - - - - - - - - - - - - - - - - - - - - - - - -

class HMdcSecListCells : public TObject {
  protected:
    HMdcModListCells list[4];
    Float_t* drTimes4m;
  public:
    HMdcSecListCells(void) {clear(); drTimes4m=0;}
    ~HMdcSecListCells(void) {}
    HMdcModListCells& operator[](Int_t m) {
      if(nModOk(m)) return list[m];
      HMdcModListCells* null=0;
      return *null;
    }
    inline void clear(void);
    inline Bool_t setTime(Int_t m, Int_t l, Int_t c, UChar_t t);
    inline void delTime(Int_t m, Int_t l, Int_t c, UChar_t t);
    inline Int_t getNCells(void) const;
    inline Bool_t nextCell(Int_t& m, Int_t& l, Int_t& c) const;
    inline UChar_t getTime(Int_t m, Int_t l, Int_t c) const;
    inline Int_t getNLayers(void) const;
    void print(void) const;

    inline Bool_t addTime(Int_t m, Int_t l, Int_t c, Float_t tm);
    inline Bool_t nextTime(Int_t& m, Int_t& l, Int_t& c, Float_t& tm) const;
    inline Float_t getTimeValue(Int_t m, Int_t l, Int_t c) const;
  protected:
    inline Bool_t nModOk(Int_t m) const;

  ClassDef(HMdcSecListCells,0) //List of all cells in layer
};

inline void HMdcSecListCells::clear(void) {
  for(Int_t m=0;m<4;m++) list[m].clear();
}

inline Bool_t HMdcSecListCells::nModOk(Int_t m) const {
  return (m>=0&&m<4) ? kTRUE:kFALSE;
}

inline Bool_t HMdcSecListCells::setTime(Int_t m, Int_t l, Int_t c, UChar_t t) {
  if(nModOk(m)) return list[m].setTime(l,c,t);
  return kFALSE;
}

inline Bool_t HMdcSecListCells::nextCell(Int_t& m, Int_t& l, Int_t& c) const {
  if(m<0) {
    m=l=0;
    c=-1;
  }
  for(;m<4;m++) if(list[m].nextCell(l,c)) return kTRUE;
  m=-1;
  return kFALSE;
}

inline void HMdcSecListCells::delTime(Int_t m, Int_t l, Int_t c, UChar_t t) {
  if(nModOk(m)) list[m].delTime(l,c,t);
}

inline Int_t HMdcSecListCells::getNCells(void) const {
  Int_t nCells=0;
  for(Int_t m=0;m<4;m++) nCells+=list[m].getNCells();
  return nCells;
}

inline UChar_t HMdcSecListCells::getTime(Int_t m, Int_t l, Int_t c) const {
  if(nModOk(m)) return list[m].getTime(l,c);
  return 0;
}

inline Int_t HMdcSecListCells::getNLayers(void) const {
  Int_t nLay=0;
  for(Int_t m=0;m<4;m++) nLay+=list[m].getNLayers();
  return nLay;
}

inline Bool_t HMdcSecListCells::addTime(Int_t m, Int_t l, Int_t c, Float_t tm) {
  if(nModOk(m)) return list[m].addTime(l,c,tm);
  return kFALSE;
}

inline Bool_t HMdcSecListCells::nextTime(Int_t& m, Int_t& l, Int_t& c, 
    Float_t& tm) const {
  if(!nextCell(m,l,c) && drTimes4m) return kFALSE;
  tm=getTimeValue(m,l,c);
  return kTRUE;
}

inline Float_t HMdcSecListCells::getTimeValue(Int_t m, Int_t l, Int_t c)
    const {
  if(nModOk(m)) return list[m].getTimeValue(l,c);
  return -9999.0;
}

//-------------------------------------------------------------------

class HMdcSecListCellsAndTimes : public HMdcSecListCells {
  protected:
    Float_t times[4][6][HMdcLayListCells::maxCells]; // drift times list (time1!!!)
  public:
    HMdcSecListCellsAndTimes() {setDrTimesArr();}
    ~HMdcSecListCellsAndTimes(void) {}
  protected:
    void setDrTimesArr(void);

  ClassDef(HMdcSecListCellsAndTimes,0) //List of all cells in layer
};

// - - - - - - - - - - - - - - - - - - - - - - - - - -

class HMdcEvntListCells : public TObject {
  protected:
    HMdcSecListCells list[6];
    Float_t* drTimes6s;
  public:
    HMdcEvntListCells(void) {clear(); drTimes6s=0;}
    ~HMdcEvntListCells(void) {}
    HMdcSecListCells& operator[](Int_t s) {
      if(nSecOk(s)) return list[s];
      HMdcSecListCells* null=0;
      return *null;
    }
    inline void clear(void);
    inline Bool_t setTime(Int_t s, Int_t m, Int_t l, Int_t c, UChar_t t);
    inline void delTime(Int_t s, Int_t m, Int_t l, Int_t c, UChar_t t);
    inline Int_t getNCells(void) const;
    inline Bool_t nextCell(Int_t& s, Int_t& m, Int_t& l, Int_t& c) const;
    inline Bool_t nextCell(Int_t& s,Int_t& m,Int_t& l,Int_t& c,UChar_t& tm) const;
    inline UChar_t getTime(Int_t s, Int_t m, Int_t l, Int_t c) const;
    void print(void) const;

    inline Bool_t addTime(Int_t s, Int_t m, Int_t l, Int_t c, Float_t tm);
    inline Bool_t nextTime(Int_t& s, Int_t& m, Int_t& l, Int_t& c, Float_t& tm) const;
    inline Float_t getTimeValue(Int_t s, Int_t m, Int_t l, Int_t c) const;
  protected:
    inline Bool_t nSecOk(Int_t s) const;

  ClassDef(HMdcEvntListCells,0) //List of all cells in layer
};

inline void HMdcEvntListCells::clear(void) {
  for(Int_t s=0;s<6;s++) list[s].clear();
}

inline Bool_t HMdcEvntListCells::nSecOk(Int_t s) const {
  return (s>=0&&s<6) ? kTRUE:kFALSE;
}

inline Bool_t HMdcEvntListCells::setTime(Int_t s, Int_t m, Int_t l, Int_t c, 
    UChar_t t) {
  if(nSecOk(s)) return list[s].setTime(m,l,c,t);
  return kFALSE;
}

inline Bool_t HMdcEvntListCells::nextCell(Int_t& s,Int_t& m,Int_t& l,Int_t& c) 
    const {
  if(s<0) {
    s=m=l=0;
    c=-1;
  }
  for(;s<6;s++) if(list[s].nextCell(m,l,c)) return kTRUE;
  s=-1;
  return kFALSE;
}

inline Bool_t HMdcEvntListCells::nextCell(Int_t& s,Int_t& m,Int_t& l,Int_t& c,
    UChar_t& tm) const {
  if(nextCell(s,m,l,c)) {
    tm=getTime(s,m,l,c);
    if(tm) return kTRUE;
  }
  return kFALSE;
}

inline void HMdcEvntListCells::delTime(Int_t s, Int_t m, Int_t l, Int_t c, 
    UChar_t t) {
  if(nSecOk(s)) list[s].delTime(m,l,c,t);
}

inline Int_t HMdcEvntListCells::getNCells(void) const {
  Int_t nCells=0;
  for(Int_t s=0;s<6;s++) nCells+=list[s].getNCells();
  return nCells;
}

inline UChar_t HMdcEvntListCells::getTime(Int_t s, Int_t m, Int_t l, Int_t c) 
    const {
  if(nSecOk(s)) return list[s].getTime(m,l,c);
  return 0;
}

inline Bool_t HMdcEvntListCells::addTime(Int_t s, Int_t m, Int_t l, Int_t c, 
    Float_t tm) {
  if(nSecOk(s)) return list[s].addTime(m,l,c,tm);
  return kFALSE;
}

inline Bool_t HMdcEvntListCells::nextTime(Int_t& s, Int_t& m, Int_t& l, Int_t& c, 
    Float_t& tm) const {
  if(!nextCell(s,m,l,c) && drTimes6s) return kFALSE;
  tm=getTimeValue(s,m,l,c);
  return kTRUE;
}

inline Float_t HMdcEvntListCells::getTimeValue(Int_t s, Int_t m, Int_t l, 
    Int_t c) const {
  if(nSecOk(s)) return list[s].getTimeValue(m,l,c);
  return -9999.0;
}

//-------------------------------------------------------------------

class HMdcEvntListCellsAndTimes : public HMdcEvntListCells {
  protected:
    Float_t times[6][4][6][HMdcLayListCells::maxCells]; // drift times list (time1!!!)
  public:
    HMdcEvntListCellsAndTimes() {setDrTimesArr();}
    ~HMdcEvntListCellsAndTimes(void) {}
  protected:
    void setDrTimesArr(void);

  ClassDef(HMdcEvntListCellsAndTimes,0) //List of all cells in layer
};

#endif
