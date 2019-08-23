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

class HTool : public TNamed {
protected:
    void    initVariables ();
public:
    HTool(const char* name="",const char* title="");
    ~HTool  ();
static Bool_t      open (TFile**,TString,TString);
static Bool_t      close(TFile**);
static Bool_t      openAscii (FILE**,TString,TString);
static Bool_t      closeAscii(FILE**);

static TDirectory* Mkdir(TDirectory *, const Char_t *, Int_t, Int_t p=1);
static TDirectory* changeToDir(TString);
static Bool_t      checkDir(TString,TFile*);
static void        scanOracle(TString,TString);
static void        deleteObject(TObject* h)   {if(h){delete h;h=0;}}
static void        writeObject (TObject* h)   {if(h){h->Write();}}
static TObjArray*  slices(TH2F* h=0,TF1* f=0,TString axis="x",Int_t firstbin=1,Int_t lastbin=0,Int_t cut=0,TString opt="QNR",Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static TObjArray*  projections(TH2F* h2,TString axis="x",Int_t firstbin=0,Int_t lastbin=-1,Int_t nsteps=-99,TString opt="e",Int_t markerstyle=8,Int_t markercolor=2,Float_t markersize=0.7);
static void        cleanHist(TH1* h,Int_t threshold=3,Int_t val=0);

ClassDef(HTool,1) // tool class
};
#endif  /*!HTOOL_H*/
