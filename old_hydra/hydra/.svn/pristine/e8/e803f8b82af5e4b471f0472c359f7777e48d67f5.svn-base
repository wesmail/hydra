#ifndef HMDCLISTGROUPCELLS_H
#define HMDCLISTGROUPCELLS_H

#include "hmdctbarray.h"
#include "TObject.h"

class HMdcList12GroupCells : public TObject {
  friend class HMdcList24GroupCells;
  private:
    enum {laySz=12};
    UChar_t arr[144];       // 144= 12bytes * 12layers
    Int_t firstCell[12];    // firstCell[layer] - first cell in layer
  public:
    HMdcList12GroupCells() {clear();}
    ~HMdcList12GroupCells(void) {;}
    inline HMdcList12GroupCells& operator = (const HMdcList12GroupCells& v);
    inline void clear(void);
    inline Int_t setTime(Int_t lay, Int_t cell, UChar_t time);
    Int_t add(HMdcList12GroupCells *lst);
    inline void delTime(Int_t lay, Int_t cell, UChar_t time);
    inline Int_t getNCells(Int_t lay, Int_t layEnd=-1) const;
    inline Int_t getNCells(void) const;    // Total num. of hits in 12 layers
    Int_t getMaxNCellsPerLay(void) const;  // maximal number of cells per layer
    inline Int_t getNDrTimes(void) const;
    inline Int_t getNDrTimes(Int_t lay) const;
    inline Int_t getNDrTimes(Int_t lay1,Int_t lay2) const;
    inline Int_t getNDrTimesMod(Int_t m) const {return getNDrTimes(m*6,m*6+5);}
    inline Int_t getNLayers(void) const; // Num. of lay. which have fired wires
    inline Int_t getNLayersMod(Int_t mod) const; // -/- in module
    inline Int_t getCell(Int_t lay, Int_t idx) const;
    inline Int_t getFirstCell(Int_t lay) const;
    inline Int_t getLastCell(Int_t lay) const;
    inline Int_t get4FirstCells(Int_t lay, Int_t* list) const; // for Hit&Seg
    inline Int_t next(Int_t lay, Int_t prCell) const;
    inline Bool_t getNext(Int_t lay, Int_t& cell) const;
    inline Bool_t getNextCell(Int_t& lay, Int_t& cell) const;
    inline Int_t previous(Int_t lay, Int_t prCell) const;
    inline UChar_t getTime(Int_t lay, Int_t cell) const;
    Int_t compare(HMdcList12GroupCells* lst, Int_t l1=-1, Int_t l2=-1,
        HMdcList12GroupCells* lstIdent=0) const;
    Bool_t isIncluded(HMdcList12GroupCells& lst, Int_t l1=0,Int_t l2=11) const;
    Int_t nIdentDrTimes(HMdcList12GroupCells* lst,Int_t l1=0, Int_t l2=11) const;
    void print(void) const;
    void print(Int_t lay) const;
  private:
    inline void setInBounds(Int_t& l1, Int_t& l2) const;

  ClassDef(HMdcList12GroupCells,1) //List of all cells in layer
};

inline HMdcList12GroupCells& HMdcList12GroupCells::operator =
    (const HMdcList12GroupCells& lst) {
  for(Int_t i=0; i<144; i++) arr[i]=lst.arr[i];
  for(Int_t i=0; i<12; i++) firstCell[i]=lst.firstCell[i];
  return *this;
}

inline void HMdcList12GroupCells::clear(void) {
  HMdcTBArray::clear(arr,arr+143);
  for(Int_t l=0; l<12; l++) firstCell[l]=0;
}

inline Int_t HMdcList12GroupCells::setTime(Int_t lay, Int_t cell, UChar_t time) {
  // return: 0 - ok.!;  n>0 - num.cells excl.from cl.
  if( cell<0 || (time&3)==0 || lay<0 || lay>=12) return 0;
  Int_t nCellsDel=0;
  UChar_t *iLayer=arr+lay*laySz;
  UChar_t *pEnd=iLayer+11;
  if(iLayer[0] == 0) firstCell[lay]=cell & 0xFFFFFFFC;  // =(cell/4)*4
  else if(cell < firstCell[lay]) {
    Int_t sh=(firstCell[lay]>>2)-(cell>>2);
    nCellsDel=HMdcTBArray::getNSet(pEnd-sh+1,pEnd);
    HMdcTBArray::shiftRight(iLayer,pEnd,pEnd-sh);
    firstCell[lay]=cell & 0xFFFFFFFC;
  }
  else if( cell-firstCell[lay] >= 48 ) return 1; // out of data array
  HMdcTBArray::set(iLayer, cell-firstCell[lay], time);
  return nCellsDel;
}

inline void HMdcList12GroupCells::delTime(Int_t lay, Int_t cell, UChar_t time) {
  Int_t pos=cell-firstCell[lay];
  if(pos < 0 || pos >=  48) return;
  UChar_t *iLayer=arr+lay*laySz;
  UChar_t *pEnd=iLayer+11;
  HMdcTBArray::unset(iLayer, pos, time);
  if(!HMdcTBArray::getNSet(iLayer, pEnd)) firstCell[lay]=0;
  else if( !iLayer[0] ) {
    Int_t sh=HMdcTBArray::first(iLayer,pEnd)>>2;
    HMdcTBArray::shiftLeft(iLayer,pEnd,iLayer+sh);
    firstCell[lay]+=sh*4;
  }
}

inline Int_t HMdcList12GroupCells::getNCells(Int_t lay, Int_t layEnd) const {
  if(layEnd==-1) layEnd=lay;
  if(lay<0 || lay>=12 || layEnd<0 || layEnd>=12 || lay>layEnd) return -1;
  return HMdcTBArray::getNSet(arr+lay*laySz,arr+layEnd*laySz+11);
}

inline Int_t HMdcList12GroupCells::getNCells(void) const {
  return HMdcTBArray::getNSet(arr,arr+143);
}

inline Int_t HMdcList12GroupCells::getNDrTimes(void) const {
  return HMdcBArray::getNSet(arr,arr+143);
}

inline Int_t HMdcList12GroupCells::getNDrTimes(Int_t lay) const {
  return getNDrTimes(lay,lay);
}

inline Int_t HMdcList12GroupCells::getNDrTimes(Int_t lay1,Int_t lay2) const {
  if(lay1<0 || lay2>=12) return -1;
  return HMdcBArray::getNSet(arr+lay1*laySz,arr+lay2*laySz+11);
}

inline Int_t HMdcList12GroupCells::getCell(Int_t lay, Int_t idx) const {
  // return: -1 - idx <0 or >num.cells
  Int_t pos=HMdcTBArray::position(arr+lay*laySz,arr+lay*laySz+11,idx);
  if(pos >= 0) pos+=firstCell[lay];
  return pos;
}

inline Int_t HMdcList12GroupCells::getFirstCell(Int_t lay) const {
  // return: -1 - num.cells=0
  Int_t pos=HMdcTBArray::first(arr+lay*laySz,arr+lay*laySz+11);
  if(pos >= 0) pos+=firstCell[lay];
  return pos;
}

inline Int_t HMdcList12GroupCells::get4FirstCells(Int_t lay, Int_t* list) const {
  // return: first cell (frCell) or -1 (num.cells=0)
  // fill list[0]-time of frCell, time frCell+1, ..., time frCell+3
  if(lay<0 || lay>=12) return -1;
  const UChar_t* addr=arr+lay*laySz;
  Int_t pos=HMdcTBArray::first(addr,addr+11);
  if(pos < 0) return -1;
  Int_t fcell=firstCell[lay]+pos;
  Int_t posLast=pos+4;
  for(Int_t ps=pos; ps<posLast; ps++) {
    *list=HMdcTBArray::get(addr,ps);
    list++;
  }
  return fcell;
}

inline Int_t HMdcList12GroupCells::getNLayers(void) const {
  Int_t nLayers=0;
  for(Int_t lay=0; lay<12; lay++) if(getFirstCell(lay)>=0) nLayers++;
  return nLayers;
}

inline Int_t HMdcList12GroupCells::getNLayersMod(Int_t mod) const {
  if(mod<0 || mod>1) return 0;
  Int_t nLayers=0;
  for(Int_t lay=mod*6; lay<(mod+1)*6; lay++) if(getFirstCell(lay)>=0) nLayers++;
  return nLayers;
}

inline Int_t HMdcList12GroupCells::getLastCell(Int_t lay) const {
  // return: -2 - num.cells=0
  Int_t pos=HMdcTBArray::last(arr+lay*laySz,arr+lay*laySz+11);
  if(pos >= 0) pos+=firstCell[lay];
  return pos;
}

inline Int_t HMdcList12GroupCells::next(Int_t lay, Int_t prCell) const {
  Int_t cell=
      HMdcTBArray::next(arr+lay*laySz,arr+lay*laySz+11,prCell-firstCell[lay]);
  if(cell>=0) cell+=firstCell[lay];
  return cell;
}

inline Bool_t HMdcList12GroupCells::getNext(Int_t lay, Int_t& cell) const {
  cell=next(lay,cell);
  if(cell>=0) return kTRUE;
  return kFALSE;
}

inline Bool_t HMdcList12GroupCells::getNextCell(Int_t& lay, Int_t& cell) const {
  if(lay<0) {
    lay=0;
    cell=-1;
  }
  for(;lay<12;lay++) if(getNext(lay,cell)) return kTRUE;
  return kFALSE;
}

inline Int_t HMdcList12GroupCells::previous(Int_t lay, Int_t prCell) const {
  Int_t cell=
    HMdcTBArray::previous(arr+lay*laySz,arr+lay*laySz+11,prCell-firstCell[lay]);
  if(cell>=0) cell+=firstCell[lay];
  return cell;
}

inline UChar_t HMdcList12GroupCells::getTime(Int_t lay, Int_t cell) const {
  Int_t pos=cell-firstCell[lay];
  if(pos<0 || pos>=48) return 0;
  return HMdcTBArray::get(arr+lay*laySz,pos);
}

inline void HMdcList12GroupCells::setInBounds(Int_t& l1, Int_t& l2) const {
  if(l1<0) {
    l1=0;
    l2=11;
  } else  {
    if(l2<0) l2=l1;
    else if(l2>=12) l2=11;
  }
}


class HMdcList24GroupCells : public TObject {
  private:
    enum {laySz=12};
    UChar_t arr[288];       // 288= 12bytes * 24layers 
    Int_t firstCell[24];    // firstCell[layer] - first cell in layer
  public:
    HMdcList24GroupCells() {clear();}
    ~HMdcList24GroupCells(void) {;}
    inline HMdcList24GroupCells& operator = (const HMdcList24GroupCells& v);
    inline void clear(Int_t seg=-1);
    inline void setSeg1(const HMdcList12GroupCells* seg);
    inline void setSeg2(const HMdcList12GroupCells* seg);
    inline void set(const HMdcList12GroupCells* seg1,
                    const HMdcList12GroupCells* seg2);
    inline Int_t setTime(Int_t lay, Int_t cell, UChar_t time);
    Int_t add(HMdcList24GroupCells *lst);
    inline void delTime(Int_t lay, Int_t cell, UChar_t time);
    inline Int_t getNCells(Int_t lay, Int_t layEnd=-1) const;
    inline Int_t getNCells(void) const;    // Total num. of hits in 24 layers
    Int_t getMaxNCellsPerLay(void) const;  // maximal number of cells per layer
    inline Int_t getNDrTimes(void) const;
    inline Int_t getNDrTimes(Int_t lay) const;
    inline Int_t getNDrTimes(Int_t lay1,Int_t lay2) const;
    inline Int_t getNDrTimesMod(Int_t m) const {return getNDrTimes(m*6,m*6+5);}
    inline Int_t getNLayers(void) const; // Num. of lay. which have fired wires
    inline Int_t getNLayersMod(Int_t mod) const; // -/- in module
    inline Int_t getCell(Int_t lay, Int_t idx) const;
    inline Int_t getFirstCell(Int_t lay) const;
    inline Int_t getLastCell(Int_t lay) const;
    inline Int_t get4FirstCells(Int_t lay, Int_t* list) const; // for Hit&Seg
    inline Int_t next(Int_t lay, Int_t prCell) const;
    inline Bool_t getNext(Int_t lay, Int_t& cell) const;
    inline Bool_t getNextCell(Int_t& lay, Int_t& cell) const;
    inline Int_t previous(Int_t lay, Int_t prCell) const;
    inline UChar_t getTime(Int_t lay, Int_t cell) const;
    Int_t compare(HMdcList24GroupCells* lst, Int_t l1=-1, Int_t l2=-1) const;
    Bool_t isIncluded(HMdcList24GroupCells& lst, Int_t l1=0,Int_t l2=11) const;
    Int_t nIdentDrTimes(HMdcList24GroupCells* lst,Int_t l1=0, Int_t l2=11) const;
    void print(void) const;
    void print(Int_t lay) const;
  private:
    inline void setInBounds(Int_t& l1, Int_t& l2) const;

  ClassDef(HMdcList24GroupCells,1) //List of all cells in layer
};

inline HMdcList24GroupCells& HMdcList24GroupCells::operator =
    (const HMdcList24GroupCells& lst) {
  for(Int_t i=0; i<288; i++) arr[i]=lst.arr[i];
  for(Int_t i=0; i<24; i++) firstCell[i]=lst.firstCell[i];
  return *this;
}

inline void HMdcList24GroupCells::setSeg1(const HMdcList12GroupCells* seg) {
  if(seg) {
    for(Int_t i=0; i<144; i++) arr[i]=seg->arr[i];
    for(Int_t i=0; i<12; i++) firstCell[i]=seg->firstCell[i];
  }
}

inline void HMdcList24GroupCells::setSeg2(const HMdcList12GroupCells* seg) {
  if(seg) {
    for(Int_t i=0; i<144; i++) arr[i+144]=seg->arr[i];
    for(Int_t i=0; i<12; i++) firstCell[i+12]=seg->firstCell[i];
  }
}

inline void HMdcList24GroupCells::set(const HMdcList12GroupCells* seg1,
                    const HMdcList12GroupCells* seg2) {
  if(seg1) setSeg1(seg1);
  else clear(0);
  if(seg2) setSeg2(seg2);
  else clear(1);
}
 
inline void HMdcList24GroupCells::clear(Int_t seg) {
  if(seg<0 || seg>1) {
    HMdcTBArray::clear(arr,arr+287);
    for(Int_t l=0; l<24; l++) firstCell[l]=0;
  } else {
    Int_t l0=seg*12;
    HMdcTBArray::clear(arr+l0*laySz,arr+(12+l0)*laySz-1);
    for(Int_t l=0; l<12; l++) firstCell[l+l0]=0;
  }
}

inline Int_t HMdcList24GroupCells::setTime(Int_t lay, Int_t cell, UChar_t time) {
  // return: 0 - ok.!;  n>0 - num.cells excl.from cl.
  if( cell<0 || (time&3)==0 || lay<0 || lay>=24) return 0;
  Int_t nCellsDel=0;
  UChar_t *iLayer=arr+lay*laySz;
  UChar_t *pEnd=iLayer+11;
  if(iLayer[0] == 0) firstCell[lay]=cell & 0xFFFFFFFC;  // =(cell/4)*4
  else if(cell < firstCell[lay]) {
    Int_t sh=(firstCell[lay]>>2)-(cell>>2);
    nCellsDel=HMdcTBArray::getNSet(pEnd-sh+1,pEnd);
    HMdcTBArray::shiftRight(iLayer,pEnd,pEnd-sh);
    firstCell[lay]=cell & 0xFFFFFFFC;
  }
  else if( cell-firstCell[lay] >= 48 ) return 1; // out of data array
  HMdcTBArray::set(iLayer, cell-firstCell[lay], time);
  return nCellsDel;
}

inline void HMdcList24GroupCells::delTime(Int_t lay, Int_t cell, UChar_t time) {
  Int_t pos=cell-firstCell[lay];
  if(pos < 0 || pos >=  48) return;
  UChar_t *iLayer=arr+lay*laySz;
  UChar_t *pEnd=iLayer+11;
  HMdcTBArray::unset(iLayer, pos, time);
  if(!HMdcTBArray::getNSet(iLayer, pEnd)) firstCell[lay]=0;
  else if( !iLayer[0] ) {
    Int_t sh=HMdcTBArray::first(iLayer,pEnd)>>2;
    HMdcTBArray::shiftLeft(iLayer,pEnd,iLayer+sh);
    firstCell[lay]+=sh*4;
  }
}

inline Int_t HMdcList24GroupCells::getNCells(Int_t lay, Int_t layEnd) const {
  if(layEnd==-1) layEnd=lay;
  if(lay<0 || lay>=24 || layEnd<0 || layEnd>=24 || lay>layEnd) return -1;
  return HMdcTBArray::getNSet(arr+lay*laySz,arr+layEnd*laySz+11);
}

inline Int_t HMdcList24GroupCells::getNCells(void) const {
  return HMdcTBArray::getNSet(arr,arr+287);
}

inline Int_t HMdcList24GroupCells::getNDrTimes(void) const {
  return HMdcBArray::getNSet(arr,arr+287);
}

inline Int_t HMdcList24GroupCells::getNDrTimes(Int_t lay) const {
  return getNDrTimes(lay,lay);
}

inline Int_t HMdcList24GroupCells::getNDrTimes(Int_t lay1,Int_t lay2) const {
  if(lay1<0 || lay2>=24) return -1;
  return HMdcBArray::getNSet(arr+lay1*laySz,arr+lay2*laySz+11);
}

inline Int_t HMdcList24GroupCells::getCell(Int_t lay, Int_t idx) const {
  // return: -1 - idx <0 or >num.cells
  Int_t pos=HMdcTBArray::position(arr+lay*laySz,arr+lay*laySz+11,idx);
  if(pos >= 0) pos+=firstCell[lay];
  return pos;
}

inline Int_t HMdcList24GroupCells::getFirstCell(Int_t lay) const {
  // return: -1 - num.cells=0
  Int_t pos=HMdcTBArray::first(arr+lay*laySz,arr+lay*laySz+11);
  if(pos >= 0) pos+=firstCell[lay];
  return pos;
}

inline Int_t HMdcList24GroupCells::get4FirstCells(Int_t lay, Int_t* list) const {
  // return: first cell (frCell) or -1 (num.cells=0)
  // fill list[0]-time of frCell, time frCell+1, ..., time frCell+3
  if(lay<0 || lay>=24) return -1;
  const UChar_t* addr=arr+lay*laySz;
  Int_t pos=HMdcTBArray::first(addr,addr+11);
  if(pos < 0) return -1;
  Int_t fcell=firstCell[lay]+pos;
  Int_t posLast=pos+4;
  for(Int_t ps=pos; ps<posLast; ps++) {
    *list=HMdcTBArray::get(addr,ps);
    list++;
  }
  return fcell;
}

inline Int_t HMdcList24GroupCells::getNLayers(void) const {
  Int_t nLayers=0;
  for(Int_t lay=0; lay<24; lay++) if(getFirstCell(lay)>=0) nLayers++;
  return nLayers;
}

inline Int_t HMdcList24GroupCells::getNLayersMod(Int_t mod) const {
  if(mod<0 || mod>3) return 0;
  Int_t nLayers=0;
  for(Int_t lay=mod*6; lay<(mod+1)*6; lay++) if(getFirstCell(lay)>=0) nLayers++;
  return nLayers;
}

inline Int_t HMdcList24GroupCells::getLastCell(Int_t lay) const {
  // return: -2 - num.cells=0
  Int_t pos=HMdcTBArray::last(arr+lay*laySz,arr+lay*laySz+11);
  if(pos >= 0) pos+=firstCell[lay];
  return pos;
}

inline Int_t HMdcList24GroupCells::next(Int_t lay, Int_t prCell) const {
  Int_t cell=
      HMdcTBArray::next(arr+lay*laySz,arr+lay*laySz+11,prCell-firstCell[lay]);
  if(cell>=0) cell+=firstCell[lay];
  return cell;
}

inline Bool_t HMdcList24GroupCells::getNext(Int_t lay, Int_t& cell) const {
  cell=next(lay,cell);
  if(cell>=0) return kTRUE;
  return kFALSE;
}

inline Bool_t HMdcList24GroupCells::getNextCell(Int_t& lay, Int_t& cell) const {
  if(lay<0) {
    lay=0;
    cell=-1;
  }
  for(;lay<24;lay++) if(getNext(lay,cell)) return kTRUE;
  return kFALSE;
}

inline Int_t HMdcList24GroupCells::previous(Int_t lay, Int_t prCell) const {
  Int_t cell=
    HMdcTBArray::previous(arr+lay*laySz,arr+lay*laySz+11,prCell-firstCell[lay]);
  if(cell>=0) cell+=firstCell[lay];
  return cell;
}

inline UChar_t HMdcList24GroupCells::getTime(Int_t lay, Int_t cell) const {
  Int_t pos=cell-firstCell[lay];
  if(pos<0 || pos>=48) return 0;
  return HMdcTBArray::get(arr+lay*laySz,pos);
}

inline void HMdcList24GroupCells::setInBounds(Int_t& l1, Int_t& l2) const {
  if(l1<0) {
    l1=0;
    l2=23;
  } else  {
    if(l2<0) l2=l1;
    else if(l2>23) l2=23;
  }
}


#endif
