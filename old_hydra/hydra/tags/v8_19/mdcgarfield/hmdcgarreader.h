#ifndef HMDCGARREADER_H
#define HMDCGARREADER_H
#include "TNamed.h"
#include "TObject.h"
#include "TH1.h"
#include "TH2.h"
#include "TGraph.h"
#include "TGraphErrors.h"
#include "TArray.h"

class TFile;
class TDirectory;
class TH1D;
class TString;

class HMdcGarReader : public TNamed {
protected:

    void    initVariables ();
public:
    HMdcGarReader(const Char_t* name="",const Char_t* title="");
    ~HMdcGarReader  ();
static TGraph* readXT(TString,Int_t);
static TGraph* readXTdiffusion(TString,Int_t);
//static void  readE();
static TGraph* readMagboltzVdrift(TString,Int_t);
static TGraph* readMagboltzGasPrint(TString,TString,Int_t);
static TArrayD* readMatrix(TString,Int_t&,Int_t&);
static TH1F*    readMatrix1DToHist(TString,Float_t,Float_t);
static TH2F*    readMatrix2DToHist(TString,Float_t,Float_t,Float_t,Float_t);
static TH1F*         readHist1D(TString,Float_t scale=1.,Float_t scalex=1.);
static TH1F*         readHist1D(TString,TString,Float_t scale=1.,Float_t scalex=1.);
static TGraph*       readHist1DToGraph(TString,Float_t scale=1.,Float_t scalex=1.);
static TGraphErrors* readHist1DToGraph(TString,TString,Float_t scale=1.,Float_t scalex=1.);

//static void  readIsoChrones();
ClassDef(HMdcGarReader,1) // Class for reading several GARFIELD formats
};
#endif  /*!HMDCGARREADER_H*/
