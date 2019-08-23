#ifndef HHISTARRAY_H
#define HHISTARRAY_H
#include "TNamed.h"
#include "TString.h"
#include "TArrayI.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "hmonhist.h"

class HHistArray : public HMonHistAddon{
protected:
    TObjArray* array;
public:
    HHistArray(const char* name   ="",
		  const char* title  ="",
		  Int_t active=0,Int_t resetable=0, Int_t size=1,Int_t size2=1,
		  Int_t nBinsX=0,Float_t Xlo=0,Float_t Xup=0);
    HHistArray(HMonHistAddon&);
    ~HHistArray(void);

    void     getDim(Int_t* i,Int_t* j)    {*i=arraySize1;*j=arraySize2;}
    Int_t    getSizeAll()    {return arraySize1*arraySize2;}
    void     fill   (Int_t,Int_t,Stat_t);
    void     setBinContent(Int_t,Int_t,Int_t,Stat_t);
    Stat_t   getBinContent(Int_t,Int_t,Int_t);
    void     setBinError  (Int_t,Int_t,Int_t,Stat_t);
    Stat_t   getBinError  (Int_t,Int_t,Int_t);
    void     reset(Int_t level,Int_t count=0);
    void     create();
    void     drawAll();
    void     add(HMonHistAddon* h2);
    TH1*     getP(Int_t s,Int_t m){return ((TH1*) (*array)[s*arraySize2+m]);}
    ClassDef(HHistArray,1) // Mdc mon hist array
};

class HHistArray2 : public HMonHistAddon{
protected:
    TObjArray* array;
public:
    HHistArray2(const char* name   ="",
		   const char* title  ="",
		   Int_t active=0,Int_t resetable=0, Int_t size=1,Int_t size2=1,
		   Int_t nBinsX=0,Float_t Xlo=0,Float_t Xup=0);
    HHistArray2(HMonHistAddon&);
    ~HHistArray2(void);

    void     getDim(Int_t* i,Int_t* j)    {*i=arraySize1;*j=arraySize2;}
    Int_t    getSizeAll()    {return arraySize1*arraySize2;}
    void     fill   (Int_t,Int_t,Stat_t,Stat_t);
    void     setBinContent(Int_t,Int_t,Int_t,Int_t,Stat_t);
    Stat_t   getBinContent(Int_t,Int_t,Int_t,Int_t);
    void     setBinError  (Int_t,Int_t,Int_t,Int_t,Stat_t);
    Stat_t   getBinError  (Int_t,Int_t,Int_t,Int_t);
    void     create();
    void     reset(Int_t level,Int_t count=0);
    void     add(HMonHistAddon* h2);
    TH2*     getP(Int_t s,Int_t m){return ((TH2*) (*array)[s*arraySize2+m]);}
    ClassDef(HHistArray2,1) // mon hist array
};
#endif  /*!HHISTARRAY_H*/
