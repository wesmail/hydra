#ifndef HMATCHUMONITOR_H
#define HMATCHUMONITOR_H

#include <TObject.h>
#include <TArrayI.h>
#include "hreconstructor.h"
#include "hmatrixcatiter.h"


class HCategory;
class HIterator;

// ***************************************************************************

class HMatchUMonitor : public HReconstructor {

// ***************************************************************************
                                        
protected:
HCategory *fCatRich; //! pointer to RichIPU data
HCategory *fCatTof; //! pointer to TofIPU data
HCategory *fCatShower; //! pointer to ShowerIPU data

///// how to get the hit level of the same categories???

public:

TH2F *hnci=new TH2F("hnci","non-correlated algo hits",100,0.,100.,100,0.,100.);
TH2F *hnca=new TH2F("hnca","non-correlated IPU hits",100,0.,100.,100,0.,100.);
TH2F *hshcx=new TH2F("hshcx","X algo vs IPU",100,0.,100.,100,0.,100.);
TH2F *hshcy=new TH2F("hshcy","Y algo vs IPU",100,0.,100.,100,0.,100.);
TH1F *diffx=new TH1F("diffx","X algo - X IPU",100,0.,10.);
TH1F *diffy=new TH1F("diffy","Y algo - Y IPU",100,0.,10.);
         
HMatchUMonitor();
~HMatchUMonitor(); 

Int_t execute();  



ClassDef(HMatchUMonitor,1)
};

//============================================================================

#endif  // HMATCHUMONITOR_H

                
