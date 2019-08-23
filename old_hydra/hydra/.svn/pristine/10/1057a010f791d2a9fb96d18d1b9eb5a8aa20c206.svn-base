#ifndef HMDCGARREADER_H
#define HMDCGARREADER_H
#include "TNamed.h"
#include "TObject.h"
#include "TH1.h"
#include "TGraph.h"

class TFile;
class TDirectory;
class TH1D;
class TString;

class HMdcGarReader : public TNamed {
protected:

    void    initVariables ();
public:
    HMdcGarReader(const char* name="",const char* title="");
    ~HMdcGarReader  ();
static TGraph* readXT(TString,Int_t);
static TGraph* readXTdiffusion(TString,Int_t);
//static void  readE();
static TGraph* readMagboltzVdrift(TString,Int_t);
static TGraph* readMagboltzGasPrint(TString,TString,Int_t);
//static void  readIsoChrones();
ClassDef(HMdcGarReader,1) // Class for reading several GARFIELD formats
};
#endif  /*!HMDCGARREADER_H*/
