// File: hrichsenderraw.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/04/26 20:26:54
//

#include "hrichsenderraw.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"


ClassImp(HRichSenderRaw)

HRichSenderRaw::HRichSenderRaw(Text_t *name,Text_t *title,Text_t *host,Int_t port) :
  HReconstructor(name,title)
{
    hostname = host;
    nPort = port; 
}

HRichSenderRaw::HRichSenderRaw()
{
 
}


HRichSenderRaw::~HRichSenderRaw(void) {
   if (sock) delete sock;
   if (mess) delete mess;
}



Bool_t HRichSenderRaw::init() {
    printf("initialization of rich raw data radio\n");

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fRawCat=gHades->getCurrentEvent()->getCategory(catRichRaw);
    if (!fRawCat) {
      fRawCat=pRichDet->buildCategory(catRichRaw);

      if (!fRawCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichRaw, fRawCat, "Rich");
    }
    initNetwork();
    return kTRUE;
}
void HRichSenderRaw::initNetwork(){
    sock = new TSocket(hostname, nPort);
    Char_t str[32];
    sock->Recv(str, 32);
    mess = new TMessage(kMESS_OBJECT);
}
Bool_t HRichSenderRaw::finalize() {
    sock->Send("Finished"); 
    sock->Close();
    return kTRUE;
}
Int_t HRichSenderRaw::execute()
{
    cout<<"in sender raw exec"<<endl;
    mess->Reset();
    //fRawCat->Dump();
    mess->WriteObject(fRawCat);
    sock->Send(*mess);

    return 0;
}

