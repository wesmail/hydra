//*-- AUTHOR : Ilse Koenig
//*-- Modified : 04/09/99

/////////////////////////////////////////////////////////////
// HMdcConnectedWires
/////////////////////////////////////////////////////////////

#include "hmdcconnectedwires.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hmdcmboreadout.h"
#include "TClass.h"
#include <iostream.h>
 
ClassImp(HMdcConnectedWires)
 
HMdcConnectedWires::HMdcConnectedWires() : HMdcLookupGeom() {
  // constructor sets the name of the container to "MdcConnectedWires"
  // creates the containers "MdcMboReadout" and "MdcLookupGeom" if not
  // existing and adds the pointers in the list of containers in the runtime
  // database
  fName="MdcConnectedWires";
  fTitle="Mdc lookup table from cal1 to raw for the connected wires";
  HRuntimeDb* rtdb=gHades->getRuntimeDb();
  readout=(HMdcMboReadout*)(rtdb->getContainer("MdcMboReadout"));
  if (!readout) {
    readout=new HMdcMboReadout();
    rtdb->addContainer(readout);
  }
  lookup=(HMdcLookupGeom*)(rtdb->getContainer("MdcLookupGeom"));
  if (!lookup) {
    lookup=new HMdcLookupGeom();
    rtdb->addContainer(lookup);
  }
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
