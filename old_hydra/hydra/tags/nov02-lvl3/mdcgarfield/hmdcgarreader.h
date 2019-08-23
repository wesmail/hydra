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
//static void  readE();
static TGraph* readMagboltz(TString,Int_t);
//static void  readIsoChrones();
ClassDef(HMdcGarReader,1) // Class for producing the MDC Cal2 parameters
};
#endif  /*!HMDCGARREADER_H*/
