#ifndef HRPCTHRESPAR_H
#define HRPCTHRESPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

using namespace std;
#include <fstream>

class HRpcThresChan : public TObject {
protected:
  Float_t  TofTh;     // tof threshold
  Float_t  TotTh;     // tot threshold
public:
  HRpcThresChan()      { clear(); }
  ~HRpcThresChan()     {;}
  Float_t getTofTh()   { return TofTh; }
  Float_t getTotTh()   { return TotTh; }

  void fill(Float_t f,Float_t t) {
    TofTh = f;
    TotTh = t;
  }
  void fill(HRpcThresChan& r) {
    TofTh = r.getTofTh();
    TotTh = r.getTotTh();
  }
  void setTofTh(Float_t f)      { TofTh = f; }
  void setTotTh(Float_t t)      { TotTh = t; }
  void clear() {
    TofTh = -999;
    TotTh = -999;
  }
  ClassDef(HRpcThresChan,1) // Channel level
};


class HRpcThresBoard: public TObject {
friend class HRpcThresPar;
protected:
  TObjArray* array;     // pointer array containing HRpcThresChan objects
public:
  HRpcThresBoard();
  ~HRpcThresBoard();
  
  HRpcThresChan& operator[](Int_t i) {
    return *static_cast<HRpcThresChan*>((*array)[i]);
  }
  HRpcThresChan* getChannel(Int_t chan) {
    if (chan>=0 && chan<=128) return &((*this)[chan]);
    else return 0;
  }
  Int_t getSize()  { return 128; }
  void clear();
  ClassDef(HRpcThresBoard,1) // Board level
};


class HRpcThresPar : public HParSet {
protected:
  TObjArray* array;  // array of pointers of type HRpcThresBoard
  Int_t offset;
  Int_t nBoards;
 public:
  HRpcThresPar(const Char_t* name="RpcThresPar",
	       const Char_t* title="Thresholds for RPC",
	       const Char_t* context="RpcThresProduction",
	       Int_t a_offset=800, Int_t a_nBoards=100);
  ~HRpcThresPar();
  HRpcThresBoard* getBoard(Int_t subeventId) {
    return (HRpcThresBoard*)(array->At(subeventId-offset));
  }
  HRpcThresBoard& operator[](Int_t i) {
    return *static_cast<HRpcThresBoard*>((*array)[i]);
  }
  Int_t getSize()   { return array->GetEntries(); }
  Int_t getOffset() { return offset; }
  Int_t getBoards() { return nBoards; }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void fill(Int_t,Int_t,Float_t,Float_t);
  void readline(const Char_t*, Int_t*);
  void putAsciiHeader(TString&);
  void write(fstream&);
  ClassDef(HRpcThresPar,1) // Container for thresholds
};

#endif  /* !HRPCTHRESPAR_H */
