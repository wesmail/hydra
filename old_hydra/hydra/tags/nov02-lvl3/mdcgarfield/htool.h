#ifndef HTOOLS_H
#define HTOOLS_H
#include "TNamed.h"
#include "TObject.h"
#include "TH1.h"
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
 
ClassDef(HTool,1) // tool class
};
#endif  /*!HTOOL_H*/
