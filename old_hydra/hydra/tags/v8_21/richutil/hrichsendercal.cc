// $Id: hrichsendercal.cc,v 1.4 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:45:37
//
#include "hrichsendercal.h"
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


ClassImp(HRichSenderCal)

HRichSenderCal::HRichSenderCal(const Text_t *name,const Text_t *title,const Text_t *host,Int_t port) :
  HReconstructor(name,title)
{
    hostname = host;
    nPort = port; 
}

HRichSenderCal::HRichSenderCal()
{
 
}


HRichSenderCal::~HRichSenderCal(void) {
   if (sock) delete sock;
   if (mess) delete mess;
}



Bool_t HRichSenderCal::init() {
    printf("initialization of rich raw data radio\n");

    HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()
                                                  ->getDetector("Rich");

    fCalCat=gHades->getCurrentEvent()->getCategory(catRichCal);
    if (!fCalCat) {
      fCalCat=pRichDet->buildCategory(catRichCal);

      if (!fCalCat) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichCal, fCalCat, "Rich");
    }
    initNetwork();
    return kTRUE;
}
void HRichSenderCal::initNetwork(){
    sock = new TSocket(hostname.Data(), nPort);
    Char_t str[32];
    sock->Recv(str, 32);
    mess = new TMessage(kMESS_OBJECT);
}
Bool_t HRichSenderCal::finalize() {
    sock->Send("Finished"); 
    sock->Close();
    return kTRUE;
}
Int_t HRichSenderCal::execute()
{
    mess->Reset();             
    mess->WriteObject(fCalCat);
    sock->Send(*mess);

    return 0;
}

