#ifndef HSTARTMONITOR_H
#define HSTARTMONITOR_H

#include "hreconstructor.h"
#include "TMapFile.h"
#include "TFile.h"
#include <TH1.h>
#include <TH2.h>
class HStartMonitor : public HReconstructor {
protected:
TMapFile *pMFile;
TFile *pFile;
 TH1F *histTdc[8]; //! 
 TH1F *histAdc[8]; //!
 TH1F *histMult1;  //!
 TH1F *histMult2;  //!
 TH2F *histM1M2;  //!
 TH2F *histj1j2;  //!
 Int_t fopt;      //!       
 TH1F* hdiff[4][4]; //!
UShort_t        adcstart[8];
UShort_t        tdcstart[8];

public:
Bool_t init(void);
HStartMonitor(Text_t* name , Text_t* title,Int_t fopt=0);
~HStartMonitor();
Bool_t finalize(void);
Int_t execute(void);
ClassDef(HStartMonitor,0) // Monitoring for start data
};

#endif /* !HSTARTMONITOR_H */
