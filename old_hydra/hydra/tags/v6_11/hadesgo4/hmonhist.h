#ifndef HMONHIST_H
#define HMONHIST_H
#include "TNamed.h"
#include "TString.h"
#include "TArrayI.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "iostream.h"


class HMonHistAddon: public TNamed {
protected:
    // hist properties
    Int_t fIsActive;
    Int_t fisResetable;
    Int_t refreshrate;
    Int_t nBinsX;
    Float_t Xlo;
    Float_t Xup;
    Int_t nBinsY;
    Float_t Ylo;
    Float_t Yup;
    Int_t arraySize1;
    Int_t arraySize2;
    TString XTitle;
    TString YTitle;
    TString ZTitle;
    TString Option;
    TString directory;
    TString HistType;
    TString format;
    Int_t stat;
    Int_t max;
    Int_t min;

    Int_t linecol;
    Int_t linestyle;
    Int_t fillcol;
    Int_t fillstyle;
    Int_t markercol;
    Int_t markerstyle;
    Float_t markersize;

    // canvas properties
    Int_t logX;
    Int_t logY;
    Int_t logZ;
    Int_t gridX;
    Int_t gridY;


public:
    HMonHistAddon (const char* name   ="",
		      const char* title  ="",
		      Int_t active=0,Int_t resetable=0,
		      Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0,
		      Int_t nbinsY=0,Float_t ylo=0,Float_t yup=0);
    HMonHistAddon (Text_t*);
    ~HMonHistAddon(void);
    void     setActive   (Int_t active)  {fIsActive=active;}
    Int_t    getActive   ()              {return fIsActive;}
    void     setResetable(Int_t reset)   {fisResetable=reset;}
    Int_t    getResetable()              {return fisResetable;}
    void     setRefreshRate(Int_t fresh) {refreshrate=fresh;}
    Int_t    getRefreshRate()            {return refreshrate;}
    void     setNBinsX(Int_t i)          {nBinsX=i;}
    Int_t    getNBinsX()                 {return nBinsX;}
    void     setXLo(Float_t i)           {Xlo=i;}
    Float_t  getXLo()                    {return Xlo;}
    void     setXUp(Float_t i)           {Xup=i;}
    Float_t  getXUp()                    {return Xup;}
    void     setNBinsY(Int_t i)          {nBinsY=i;}
    Int_t    getNBinsY()                 {return nBinsY;}
    void     setYLo(Float_t i)           {Ylo=i;}
    Float_t  getYLo()                    {return Ylo;}
    void     setYUp(Float_t i)           {Yup=i;}
    Float_t  getYUp()                    {return Yup;}
    void     setSize1(Int_t i)           {arraySize1=i;}
    Int_t    getSize1()                  {return arraySize1;}
    void     setSize2(Int_t i)           {arraySize2=i;}
    Int_t    getSize2()                  {return arraySize2;}
    void     setStat(Int_t i)            {stat=i;}
    Int_t    getStat()                   {return stat;}
    void     setMax(Int_t i)             {max=i;}
    Int_t    getMax()                    {return max;}
    void     setMin(Int_t i)             {min=i;}
    Int_t    getMin()                    {return min;}
    void     setLineCol(Int_t i)         {linecol=i;}
    Int_t    getLineCol()                {return linecol;}
    void     setLineStyle(Int_t i)       {linestyle=i;}
    Int_t    getLineStyle()              {return linestyle;}
    void     setFillCol(Int_t i)         {fillcol=i;}
    Int_t    getFillCol()                {return fillcol;}
    void     setFillStyle(Int_t i)       {fillstyle=i;}
    Int_t    getFillStyle()              {return fillstyle;}
    void     setMarkerCol(Int_t i)       {markercol=i;}
    Int_t    getMarkerCol()              {return markercol;}
    void     setMarkerStyle(Int_t i)     {markerstyle=i;}
    Int_t    getMarkerStyle()            {return markerstyle;}
    void     setMarkerSize(Float_t i)    {markersize=i;}
    Float_t  getMarkerSize()             {return markersize;}
    void     setLogX(Int_t i)            {logX=i;}
    Int_t    getLogX()                   {return logX;}
    void     setLogY(Int_t i)            {logY=i;}
    Int_t    getLogY()                   {return logY;}
    void     setLogZ(Int_t i)            {logZ=i;}
    Int_t    getLogZ()                   {return logZ;}
    void     setGridX(Int_t i)           {gridX=i;}
    Int_t    getGridX()                  {return gridX;}
    void     setGridY(Int_t i)           {gridY=i;}
    Int_t    getGridY()                  {return gridY;}
    void     setDirectory(TString dir)   {directory=dir;}
    TString  getDirectory()              {return directory;}
    void     setXTitle(TString title)    {XTitle=title;}
    TString  getXTitle()                 {return XTitle;}
    void     setYTitle(TString title)    {YTitle=title;}
    TString  getYTitle()                 {return YTitle;}
    void     setZTitle(TString title)    {ZTitle=title;}
    TString  getZTitle()                 {return ZTitle;}
    void     setOption(TString opt)      {Option=opt;}
    TString  getOption()                 {return Option;}
    void     setHistType(TString type)   {HistType=type;}
    TString  getHistType()               {return HistType;}
    void     setFormat(TString form)     {format=form;}
    TString  getFormat()                 {return format;}
    void     copySet(HMonHistAddon&);
    void     getSet(HMonHistAddon& add);
    void     read(const Text_t*);
    void     getText(Text_t* txt);
    void     setHist(TH1*);
    void     setHist(TH2*);
    void     setCanvas(TCanvas* c);
    virtual  void reset(Int_t level,Int_t count=0){};
    virtual  void add(HMonHistAddon*){};

    ClassDef(HMonHistAddon,1) // mon hist addidional features
};

class HMonHist: public HMonHistAddon {
protected:
    TH1* h;
public:
    HMonHist(const char* name   ="",
		const char* title  ="",
		Int_t active=0,Int_t resetable=0,
		Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0);
    HMonHist(HMonHistAddon&);
    ~HMonHist();
    TH1*    getP()           {return h;}
    void    reset(Int_t level,Int_t count=0);
    void    create();
    void    draw()           {h->Draw();}
    void    add(HMonHistAddon* h2){h->Add(((HMonHist*)h2)->getP());}
    ClassDef(HMonHist,1) // mon hist
};

class HMonHist2: public HMonHistAddon {
protected:
    TH2* h;
public:
    HMonHist2(const char* name   ="",
		 const char* title  ="",
		 Int_t active=0,Int_t resetable=0,
		 Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0,
		 Int_t nbinsY=0,Float_t ylo=0,Float_t yup=0);
    HMonHist2(HMonHistAddon&);
    ~HMonHist2();
    TH2*    getP()            {return h;}
    void    reset(Int_t level,Int_t count=0);
    void    create();
    void    draw()            {h->Draw();}
    void    add(HMonHistAddon* h2){h->Add(((HMonHist2*)h2)->getP());}
    ClassDef(HMonHist2,1) // mon hist
};
#endif  /*!HMONHIST_H*/
