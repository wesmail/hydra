//*-- AUTHOR : Ilse Koenig
//*-- Modified : 17/01/2002

/////////////////////////////////////////////////////////////
// HMdcConnectedWires
/////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcconnectedwires.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hmdcmboreadout.h"
#include "TClass.h"
#include <iostream> 
#include <iomanip>
 
ClassImp(HMdcConnectedWires)
 
HMdcConnectedWires::HMdcConnectedWires(const char* name,const char* title,
                                       const char* context)
                   : HMdcLookupGeom(name,title,context) {
  // constructor creates the containers "MdcMboReadout" and "MdcLookupGeom"
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  readout=(HMdcMboReadout*)(rtdb->getContainer("MdcMboReadout"));
  lookup=(HMdcLookupGeom*)(rtdb->getContainer("MdcLookupGeom"));
}


Bool_t HMdcConnectedWires::init(HParIo*,Int_t*) {
  if (readout->hasChanged() || lookup->hasChanged()) {
    HMdcLookupGeom& lMdc=*lookup;
    HMdcMboReadout& rMdc=*readout;
    for(Int_t s=0;s<getSize();s++) {
      HMdcLookupGSec& lSec=lMdc[s];
      HMdcMboReadoutSec& rSec=rMdc[s];
      HMdcLookupGSec& pSec=(*this)[s];
      for(Int_t m=0;m<pSec.getSize();m++) {
        HMdcLookupGMod& lMod=lSec[m];
        HMdcMboReadoutMod& rMod=rSec[m];
        HMdcLookupGMod& pMod=pSec[m];
        for(Int_t l=0;l<pMod.getSize();l++) {
          HMdcLookupMoth& lMoth=lMod[l];
          HMdcMboReadoutMoth& rMoth=rMod[l];
          HMdcLookupMoth& pMoth=pMod[l];
          if (rMoth.getRoc()>0 && pMoth.getSize()) {
            for(Int_t c=0;c<pMoth.getSize();c++) {
              pMoth[c].setNLayer(lMoth[c].getNLayer());
              pMoth[c].setNCell(lMoth[c].getNCell());
            }
          }
        }
      }
    }
  }
  versions[1]++;
  changed=kTRUE;
  return kTRUE;
}
