#ifndef HTOOLS_H
#define HTOOLS_H
#include "TNamed.h"
#include "TObject.h"
#include "TObjArray.h"
#include "TH1.h"
#include "TH2.h"
#include "TF1.h"
#include "TString.h"
#include "TDirectory.h"
#include "TCutG.h"
#include "TClass.h"
#include "TCanvas.h"
#include "TGraph.h"
#include "TGraphErrors.h"

class HTool : public TNamed {
protected:
    void    initVariables ();
public:
    HTool(const char* name="",const char* title="");
    ~HTool  ();
//------------Files------------------------------
static Bool_t      open (TFile**,TString,TString);
static Bool_t      close(TFile**);
static Bool_t      openAscii (FILE**,TString,TString);
static Bool_t      closeAscii(FILE**);
static TObjArray*  glob(TString pattern);
static Bool_t      writeNtupleToAscii(TString Input ="",TString ntuple ="",TString Output ="",TString separator=",",TString selection  ="", TString condition ="",Long64_t entries =-1, Long64_t startEntry=0,Int_t ColWidth =0);
static void        backTrace(Int_t level);
//------------Dirs-------------------------------
static TDirectory* Mkdir(TDirectory *, const Char_t *, Int_t, Int_t p=1);
static TDirectory* changeToDir(TString);
static Bool_t      checkDir(TString,TFile*);
//------------misc-------------------------------
static TList*      getListOfAllDataMembers(TClass* cl);
static void        scanOracle(TString,TString);
static Double_t    roundDigits(Double_t d, Int_t ndigits=-1);
static Float_t     roundDigits(Float_t f , Int_t ndigits=-1);
static Int_t       exec(TString command, TString& output);

static void        sort(Int_t n, Char_t*   arr,Int_t* index=0,Bool_t down=kTRUE,Bool_t overwrite=kTRUE);
static void        sort(Int_t n, Short_t*  arr,Int_t* index=0,Bool_t down=kTRUE,Bool_t overwrite=kTRUE);
static void        sort(Int_t n, Int_t*    arr,Int_t* index=0,Bool_t down=kTRUE,Bool_t overwrite=kTRUE);
static void        sort(Int_t n, Float_t*  arr,Int_t* index=0,Bool_t down=kTRUE,Bool_t overwrite=kTRUE);
static void        sort(Int_t n, Double_t* arr,Int_t* index=0,Bool_t down=kTRUE,Bool_t overwrite=kTRUE);
static void        sortParallel(Int_t n,Int_t nArrays,Char_t*   listarr,Int_t leading=0,Int_t* index=0,Bool_t down=kTRUE);
static void        sortParallel(Int_t n,Int_t nArrays,Short_t*  listarr,Int_t leading=0,Int_t* index=0,Bool_t down=kTRUE);
static void        sortParallel(Int_t n,Int_t nArrays,Int_t*    listarr,Int_t leading=0,Int_t* index=0,Bool_t down=kTRUE);
static void        sortParallel(Int_t n,Int_t nArrays,Float_t*  listarr,Int_t leading=0,Int_t* index=0,Bool_t down=kTRUE);
static void        sortParallel(Int_t n,Int_t nArrays,Double_t* listarr,Int_t leading=0,Int_t* index=0,Bool_t down=kTRUE);
static Double_t    kurtosis(Int_t n, Short_t*  data);
static Double_t    kurtosis(Int_t n, Int_t*    data);
static Double_t    kurtosis(Int_t n, Float_t*  data);
static Double_t    kurtosis(Int_t n, Double_t* data);
static Double_t    skewness(Int_t n, Short_t*  data);
static Double_t    skewness(Int_t n, Int_t*    data);
static Double_t    skewness(Int_t n, Float_t*  data);
static Double_t    skewness(Int_t n, Double_t* data);
static Double_t    weightedMean(Int_t n, Short_t*  data,Short_t* dataerr);
static Double_t    weightedMean(Int_t n, Int_t*    data,Int_t* dataerr);
static Double_t    weightedMean(Int_t n, Float_t*  data,Float_t* dataerr);
static Double_t    weightedMean(Int_t n, Double_t* data,Double_t* dataerr);
static Double_t    weightedSigma(Int_t n, Short_t* dataerr);
static Double_t    weightedSigma(Int_t n, Int_t*    dataerr);
static Double_t    weightedSigma(Int_t n, Float_t*  dataerr);
static Double_t    weightedSigma(Int_t n, Double_t* dataerr);
static Double_t    weightedMeanAndSigma(Int_t n, Short_t*  data,Short_t*  dataerr, Double_t* mean, Double_t* sigma);
static Double_t    weightedMeanAndSigma(Int_t n, Int_t*    data,Int_t*    dataerr, Double_t* mean, Double_t* sigma);
static Double_t    weightedMeanAndSigma(Int_t n, Float_t*  data,Float_t*  dataerr, Double_t* mean, Double_t* sigma);
static Double_t    weightedMeanAndSigma(Int_t n, Double_t* data,Double_t* dataerr, Double_t* mean, Double_t* sigma);

//------------objects----------------------------
static void        deleteObject(TObject* h)   {if(h){delete h;h=0;}}
static void        writeObject (TObject* h)   {if(h){h->Write();}}
//------------hists------------------------------
static void        roundHist(TH2*h=0,Int_t ndigit=-1,Int_t ndigiterr=-1);
static TH1*        getHist(TFile* inp,TString name);
static TObjArray*  slices(TH2* h=0,TF1* f=0,TString axis="y",Int_t firstbin=1,Int_t lastbin=0,Int_t cut=0,TString opt="QNR",TString suffix="",Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TObjArray*  projections(TH2* h2,TString axis="y",Int_t firstbin=0,Int_t lastbin=-1,Int_t nsteps=-99,TString opt="e",TString suffix="",Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TObjArray*  fitArray(TObjArray* array=0,TF1* fit=0,TString name="",Float_t min=0,Float_t max=0,Int_t opt=0,Float_t x1=0,Float_t x2=0,Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TH1D*       projectY(TH1* h=0,Int_t xbin1=1,Int_t xbin2=0,TString suff="",Int_t ybin=0,Float_t ymin=-99,Float_t ymax=-99,Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TGraphErrors* fitScatter(TH2* h=0,TF1* f=0,TString opt="QNR",Bool_t silent=kFALSE,Float_t xmin=0,Float_t xmax=0,Float_t ymin=0,Float_t ymax=0,Float_t window=0,Bool_t clean=kFALSE);
static void        cleanHist(TH1* h,Double_t threshold=3,Double_t val=0);
static Int_t       removeEnds(TH1* h,Int_t first=1,Int_t last=-1);
//static TH1*        removeEnds(const TH1* h,Int_t first=1,Int_t last=-1,TString newname="");
static Int_t       findFilledRange(TH1* h,Int_t& first,Int_t& last);
static void        resetHist(TH1* h,Float_t val=-99,Float_t valerr=-99);
static TH1*        copyHist(TH1* h,TString name="",Int_t val=-99,Int_t valerr=-99);
static TH1*        copyHistRange(TH1* h,TString name="",Int_t val=-99,Int_t valerr=-99,Int_t start=0,Int_t end=-99);
static TF1*        cleanHistBelowLine(TH2* h,TF1* f=0,TString axis="y",Int_t firstbin=1,Int_t lastbin=0,Int_t cut=0,TString opt="QNR",TString suffix="",
                               TString optline="pol1",Float_t windowfunc=1000.);
static TF1*        cleanHistArroundLine(TH2* h,TF1* f=0,TString axis="y",Int_t firstbin=1,Int_t lastbin=0,Int_t cut=0,TString opt="QNR",TString suffix="",
                               TString optline="pol1",Float_t windowfunc=1000.,Float_t windowfunc2=1000.);
static Bool_t      cleanHistCutG(TH2* =0,TCutG* =0);
static void        setHistErrors(TH1* h,TH1* h2);
static Double_t    getMaxHistArray(TH1** h,Int_t size,Int_t& index);
static Double_t    getMinHistArray(TH1** h,Int_t size,Int_t& index);
static TH2*        reBin(TH2* h2=0,Int_t groupX=2,Int_t gropY=2,TString newname="");
static TH2*        exchangeXY(TH2* h2=0,TString newname="");
static Bool_t      flipAxis(TH2* h2=0,TString opt="x");
static TGraph*     histToGraph(TH1* h=0,TString newname="",Bool_t exchange=kFALSE,Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TGraphErrors* histToGraphErrors(TH1* h=0,TString newname="",Bool_t exchange=kFALSE,Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TGraph*       hist2DToGraph(TH2* h=0,Float_t xmin=0,Float_t xmax=0,Float_t ymin=0,Float_t ymax=0);
static TGraphErrors* hist2DToGraphErrors(TH2* h=0,Float_t xmin=0,Float_t xmax=0,Float_t ymin=0,Float_t ymax=0);
static Bool_t      shiftHistByBin(TH1* h=0,Int_t shiftbin=0);
static Bool_t      shiftHist(TH1* h=0,Float_t shift=0);
static Int_t       normalize_max(TH2* h2=0,TString axis="y");
//------------Graphs----------------------------
static Bool_t      exchangeXYGraph(TGraph* g=0);
static Bool_t      scaleGraph(TGraph* g=0,Double_t xscale=1.,Double_t yscale=1.);
static Bool_t      shiftGraph(TGraph* g=0,Double_t xshift=0.,Double_t yshift=0.);
//------------strings---------------------------
static TString*    parseString(TString options,Int_t& size,TString separator="",Bool_t tolower=kTRUE);
static Bool_t      findString(TString* classes,Int_t size,TString name);
//------------compare tree----------------------
static Bool_t      readHistsDescription(TString file,TObjArray* myhists,TString* classes=0,Int_t sizeclass=0);
static Bool_t      makeHists(TString infile="",Int_t evs=0,TString histdescriptionfile="",TString listofClasses="");
static Bool_t      drawHistComp(TString nameinp1="",TString nameinp2="",TString name="",TCanvas* comp=0,Int_t padn=1);
static Bool_t      compHistFiles(TString nameinp1="",TString nameinp2="",TString name="");
static Bool_t      printHistInfo(TH1* h1=0);
static Bool_t      printCompHistInfo(TH1* h1=0,TH1* h2=0,Int_t detail=0);
static Bool_t      writeHistInfo(TH1* h1=0,FILE* anaout=0);
ClassDef(HTool,1) // tool class
};
#endif  /*!HTOOL_H*/
