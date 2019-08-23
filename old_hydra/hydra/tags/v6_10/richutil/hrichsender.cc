// File: hrichsender.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/05/05 11:30:58
//

#include "hrichsender.h"
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


ClassImp(HRichSender)

HRichSender::HRichSender(Text_t *name,Text_t *title,Text_t *host,Int_t port) :
  HReconstructor(name,title)
{
    hostname = host;
    nPort = port; 
}

HRichSender::HRichSender()
{
 
}


HRichSender::~HRichSender(void) {
   if (sock) delete sock;
   if (mess) delete mess;
}



Bool_t HRichSender::init() {
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
void HRichSender::initNetwork(){
    sock = new TSocket(hostname, nPort);
    char str[32];
    sock->Recv(str, 32);
    mess = new TMessage(kMESS_OBJECT);
}
Bool_t HRichSender::finalize() {
    sock->Send("Finished"); 
    sock->Close();
    return kTRUE;
}
Int_t HRichSender::execute()
{
    mess->Reset();             
    mess->WriteObject(fRawCat);
    sock->Send(*mess);

    return 0;
}

