#ifndef HSEQARRI_H
#define HSEQARRI_H

#include "TObject.h"

class HSeqArrBlock : public TObject {
  friend class HSeqArr;
  friend class HSeqArrIter;
  protected:
    enum {blockSize=0x40000,    // size of one block of array in bytes
          nBits=18,             // index>>nBits - is number of array block
          bitMask=0x3FFFF};     // index&BITSMASK - is element index in block
    Char_t arr[blockSize];
    UInt_t blockNumber;         // array block number
    HSeqArrBlock *nextBlock;    // pointer to the next array block

    HSeqArrBlock(void);
    HSeqArrBlock(HSeqArrBlock* p);
    virtual ~HSeqArrBlock(void);
    Bool_t expand(void);
    static UInt_t blockIndex(UInt_t n) {return n>>nBits;}
    static UInt_t indexInBlock(UInt_t i) {return i&bitMask;}
  private:
  ClassDef(HSeqArrBlock,0);
};


enum ESeqArrType {kNoType,
                  kChar,kUChar,kShort,kUShort,kInt,kUInt,kLong64,kULong64,
                  kFloat,kDouble,
                  kBool};

class HSeqArr : public TObject {
  friend class HSeqArrIter; //???????????????????????????
  public:
  protected:
    ESeqArrType type;               // array type
    HSeqArrBlock* firstBlock; // pinter to the first block of array
    HSeqArrBlock* lastBlock;  // pinter to the last block of array
    UInt_t wordLength;        // length of word in array
    
    UInt_t size;              // total size of array in bytes
    UInt_t nBlocks;           // number of blocks in array
    UInt_t blockLength;       // size of HSeqArrBlock object in bytes
    
    HSeqArrBlock* block;      // pointer to the current HSeqArrBlock object
    UInt_t index;             // current index of element in full array (0-size)
    Char_t* cElem;            // pointer to the current element
    Bool_t isFrst;            // ???

  protected:
    void init(ESeqArrType tp);
    Bool_t addBlock(void);
    Bool_t setCurrBlock(UInt_t n);
    Bool_t err(ESeqArrType tp);
    Bool_t tpOk(ESeqArrType tp) {return (type==tp) ? kTRUE:err(tp);}
    
    Bool_t next(ESeqArrType tp);
    Bool_t nextAExp(ESeqArrType tp);
    
//     Char_t&    getChar(void) const    {return *(Char_t*   )cElem;}   
//     UChar_t&   getUChar(void) const   {return *(UChar_t*  )cElem;}
//     Short_t&   getShort (void) const  {return *(Short_t*  )cElem;}
//     UShort_t&  getUShort(void) const  {return *(UShort_t* )cElem;} 
//     Int_t&     getInt(void) const     {return *(Int_t*    )cElem;}    
//     UInt_t&    getUInt(void) const    {return *(UInt_t*   )cElem;}   
//     Long64_t&  getLong64(void) const  {return *(Long64_t* )cElem;} 
//     ULong64_t& getULong64(void) const {return *(ULong64_t*)cElem;}
//     Float_t&   getFloat(void) const   {return *(Float_t*  )cElem;}  
//     Double_t&  getDouble(void) const  {return *(Double_t* )cElem;} 
//     Bool_t&    getBool(void) const    {return *(Bool_t*   )cElem;}
    
    Bool_t set(Char_t    &e) {*(Char_t*   )cElem=e; return kTRUE;}   
    Bool_t set(UChar_t   &e) {*(UChar_t*  )cElem=e; return kTRUE;}
    Bool_t set(Short_t   &e) {*(Short_t*  )cElem=e; return kTRUE;}
    Bool_t set(UShort_t  &e) {*(UShort_t* )cElem=e; return kTRUE;} 
    Bool_t set(Int_t     &e) {*(Int_t*    )cElem=e; return kTRUE;}    
    Bool_t set(UInt_t    &e) {*(UInt_t*   )cElem=e; return kTRUE;}   
    Bool_t set(Long64_t  &e) {*(Long64_t* )cElem=e; return kTRUE;} 
    Bool_t set(ULong64_t &e) {*(ULong64_t*)cElem=e; return kTRUE;}
    Bool_t set(Float_t   &e) {*(Float_t*  )cElem=e; return kTRUE;}  
    Bool_t set(Double_t  &e) {*(Double_t* )cElem=e; return kTRUE;} 
    Bool_t set(Bool_t    &e) {*(Bool_t*   )cElem=e; return kTRUE;}
    
    Bool_t get(Char_t    &e) const {e=*(Char_t*   )cElem; return kTRUE;}   
    Bool_t get(UChar_t   &e) const {e=*(UChar_t*  )cElem; return kTRUE;}
    Bool_t get(Short_t   &e) const {e=*(Short_t*  )cElem; return kTRUE;}
    Bool_t get(UShort_t  &e) const {e=*(UShort_t* )cElem; return kTRUE;} 
    Bool_t get(Int_t     &e) const {e=*(Int_t*    )cElem; return kTRUE;}    
    Bool_t get(UInt_t    &e) const {e=*(UInt_t*   )cElem; return kTRUE;}   
    Bool_t get(Long64_t  &e) const {e=*(Long64_t* )cElem; return kTRUE;} 
    Bool_t get(ULong64_t &e) const {e=*(ULong64_t*)cElem; return kTRUE;}
    Bool_t get(Float_t   &e) const {e=*(Float_t*  )cElem; return kTRUE;}  
    Bool_t get(Double_t  &e) const {e=*(Double_t* )cElem; return kTRUE;} 
    Bool_t get(Bool_t    &e) const {e=*(Bool_t*   )cElem; return kTRUE;}
    
  public:
    HSeqArr(void) {init(kNoType);}
    HSeqArr(ESeqArrType tp) {init(tp);}
    HSeqArr(Char_t) {init(kChar);}
    ~HSeqArr(void);
    
    Bool_t setArrType(ESeqArrType tp);
    UInt_t getArraySize(void) const {return size/wordLength;}
    UInt_t getMemSize(void) const;
    HSeqArrIter* makeIterator(void);
    Bool_t isBoundOk(UInt_t i) const {return (i<size) ? kTRUE:kFALSE;}
    
    void rewind(void);
    
    Bool_t addElement(void);         // adding new element and setting cursor
    Bool_t setCursor(UInt_t indArr);
    
    void setElement(Char_t    e) {if(tpOk(kChar   )) set(e);}
    void setElement(UChar_t   e) {if(tpOk(kUChar  )) set(e);}
    void setElement(Short_t   e) {if(tpOk(kShort  )) set(e);}
    void setElement(UShort_t  e) {if(tpOk(kUShort )) set(e);}
    void setElement(Int_t     e) {if(tpOk(kInt    )) set(e);}
    void setElement(UInt_t    e) {if(tpOk(kUInt   )) set(e);}
    void setElement(Long64_t  e) {if(tpOk(kLong64 )) set(e);}
    void setElement(ULong64_t e) {if(tpOk(kULong64)) set(e);}
    void setElement(Float_t   e) {if(tpOk(kFloat  )) set(e);}   
    void setElement(Double_t  e) {if(tpOk(kDouble )) set(e);}
    void setElement(Bool_t    e) {if(tpOk(kBool   )) set(e);}
    
    void getElement(Char_t&    e) {if(tpOk(kChar   )) get(e);}
    void getElement(UChar_t&   e) {if(tpOk(kUChar  )) get(e);}
    void getElement(Short_t&   e) {if(tpOk(kShort  )) get(e);}
    void getElement(UShort_t&  e) {if(tpOk(kUShort )) get(e);}
    void getElement(Int_t&     e) {if(tpOk(kInt    )) get(e);}
    void getElement(UInt_t&    e) {if(tpOk(kUInt   )) get(e);}
    void getElement(Long64_t&  e) {if(tpOk(kLong64 )) get(e);}
    void getElement(ULong64_t& e) {if(tpOk(kULong64)) get(e);}
    void getElement(Float_t&   e) {if(tpOk(kFloat  )) get(e);}   
    void getElement(Double_t&  e) {if(tpOk(kDouble )) get(e);}
    void getElement(Bool_t&    e) {if(tpOk(kBool   )) get(e);}
    
    Bool_t addNext(Char_t    e) {return nextAExp(kUChar  ) ? set(e):kFALSE;}
    Bool_t addNext(UChar_t   e) {return nextAExp(kUChar  ) ? set(e):kFALSE;}
    Bool_t addNext(Short_t   e) {return nextAExp(kShort  ) ? set(e):kFALSE;}
    Bool_t addNext(UShort_t  e) {return nextAExp(kUShort ) ? set(e):kFALSE;}
    Bool_t addNext(Int_t     e) {return nextAExp(kInt    ) ? set(e):kFALSE;}
    Bool_t addNext(UInt_t    e) {return nextAExp(kUInt   ) ? set(e):kFALSE;}
    Bool_t addNext(Long64_t  e) {return nextAExp(kLong64 ) ? set(e):kFALSE;}
    Bool_t addNext(ULong64_t e) {return nextAExp(kULong64) ? set(e):kFALSE;}
    Bool_t addNext(Float_t   e) {return nextAExp(kFloat  ) ? set(e):kFALSE;}   
    Bool_t addNext(Double_t  e) {return nextAExp(kDouble ) ? set(e):kFALSE;}
    Bool_t addNext(Bool_t    e) {return nextAExp(kBool   ) ? set(e):kFALSE;}
    
    Bool_t getNext(Char_t&    e) {return next(kUChar  ) ? get(e):kFALSE;}
    Bool_t getNext(UChar_t&   e) {return next(kUChar  ) ? get(e):kFALSE;}
    Bool_t getNext(Short_t&   e) {return next(kShort  ) ? get(e):kFALSE;}
    Bool_t getNext(UShort_t&  e) {return next(kUShort ) ? get(e):kFALSE;}
    Bool_t getNext(Int_t&     e) {return next(kInt    ) ? get(e):kFALSE;}
    Bool_t getNext(UInt_t&    e) {return next(kUInt   ) ? get(e):kFALSE;}
    Bool_t getNext(Long64_t&  e) {return next(kLong64 ) ? get(e):kFALSE;}
    Bool_t getNext(ULong64_t& e) {return next(kULong64) ? get(e):kFALSE;}
    Bool_t getNext(Float_t&   e) {return next(kFloat  ) ? get(e):kFALSE;}   
    Bool_t getNext(Double_t&  e) {return next(kDouble ) ? get(e):kFALSE;}
    Bool_t getNext(Bool_t&    e) {return next(kBool   ) ? get(e):kFALSE;}

  ClassDef(HSeqArr,0);
};

class HSeqArrIter : public TObject {
  friend class HSeqArr;
  protected:
    HSeqArr* array;
    HSeqArrBlock* block;      // pointer to the current HSeqArrBlock object
    UInt_t index;             // current index of element in full array (0-size)
    Char_t* cElem;            // pointer to the current element
    Bool_t isFrst;            // ???
  private:
    HSeqArrIter(void) {}
  protected:
    HSeqArrIter(HSeqArr* arr);
    Bool_t next(ESeqArrType tp);
    Bool_t nextAExp(ESeqArrType tp);
 
  public:
    ~HSeqArrIter(void) {}
    void reset(void);

    Bool_t next(void);
    Bool_t nextAndExpand(void);
    Bool_t setCursor(UInt_t indArr=0);
    
    void set(Char_t    e) {*(Char_t*   )cElem=e;} // Set element in cursor position  
    void set(UChar_t   e) {*(UChar_t*  )cElem=e;} // ...
    void set(Short_t   e) {*(Short_t*  )cElem=e;} // ...
    void set(UShort_t  e) {*(UShort_t* )cElem=e;} // ...
    void set(Int_t     e) {*(Int_t*    )cElem=e;} // ...
    void set(UInt_t    e) {*(UInt_t*   )cElem=e;} // ...
    void set(Long64_t  e) {*(Long64_t* )cElem=e;} // ...
    void set(ULong64_t e) {*(ULong64_t*)cElem=e;} // ...
    void set(Float_t   e) {*(Float_t*  )cElem=e;} // ...
    void set(Double_t  e) {*(Double_t* )cElem=e;} // ...
    void set(Bool_t    e) {*(Bool_t*   )cElem=e;} // ...
    
    void get(Char_t    &e) const {e=*(Char_t*   )cElem;} // Get element from cursor position
    void get(UChar_t   &e) const {e=*(UChar_t*  )cElem;} // ...
    void get(Short_t   &e) const {e=*(Short_t*  )cElem;} // ...
    void get(UShort_t  &e) const {e=*(UShort_t* )cElem;} // ...
    void get(Int_t     &e) const {e=*(Int_t*    )cElem;} // ...
    void get(UInt_t    &e) const {e=*(UInt_t*   )cElem;} // ...
    void get(Long64_t  &e) const {e=*(Long64_t* )cElem;} // ...
    void get(ULong64_t &e) const {e=*(ULong64_t*)cElem;} // ...
    void get(Float_t   &e) const {e=*(Float_t*  )cElem;} // ...
    void get(Double_t  &e) const {e=*(Double_t* )cElem;} // ...
    void get(Bool_t    &e) const {e=*(Bool_t*   )cElem;} // ...
    
    
    Bool_t setNext(Char_t    e) {if(!nextAExp(kUChar  )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(UChar_t   e) {if(!nextAExp(kUChar  )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(Short_t   e) {if(!nextAExp(kShort  )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(UShort_t  e) {if(!nextAExp(kUShort )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(Int_t     e) {if(!nextAExp(kInt    )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(UInt_t    e) {if(!nextAExp(kUInt   )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(Long64_t  e) {if(!nextAExp(kLong64 )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(ULong64_t e) {if(!nextAExp(kULong64)) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(Float_t   e) {if(!nextAExp(kFloat  )) return kFALSE; set(e); return kTRUE;}   
    Bool_t setNext(Double_t  e) {if(!nextAExp(kDouble )) return kFALSE; set(e); return kTRUE;}
    Bool_t setNext(Bool_t    e) {if(!nextAExp(kBool   )) return kFALSE; set(e); return kTRUE;}
    
    Bool_t getNext(Char_t&    e) {if(!next(kUChar  )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(UChar_t&   e) {if(!next(kUChar  )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(Short_t&   e) {if(!next(kShort  )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(UShort_t&  e) {if(!next(kUShort )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(Int_t&     e) {if(!next(kInt    )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(UInt_t&    e) {if(!next(kUInt   )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(Long64_t&  e) {if(!next(kLong64 )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(ULong64_t& e) {if(!next(kULong64)) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(Float_t&   e) {if(!next(kFloat  )) return kFALSE; get(e); return kTRUE;}   
    Bool_t getNext(Double_t&  e) {if(!next(kDouble )) return kFALSE; get(e); return kTRUE;}
    Bool_t getNext(Bool_t&    e) {if(!next(kBool   )) return kFALSE; get(e); return kTRUE;}
        
  ClassDef(HSeqArrIter,0);  
};

#endif  /*!HSEQARRI_H*/
