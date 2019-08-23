// $Id: hrichsenderrawaccu.cc,v 1.4 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:45:54
//
#include "hrichsenderrawaccu.h"
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


ClassImp(HRichSenderRawAccu)

HRichSenderRawAccu::HRichSenderRawAccu(const Text_t *name,const Text_t *title,const Text_t *host,Int_t port,Int_t evt) :
  HReconstructor(name,title)
{
    hostname = host;
    nPort = port; 
    nCount = evt;
    iLocalCount =0;
    fRawIter = NULL;
}

HRichSenderRawAccu::HRichSenderRawAccu()
{
 
}


HRichSenderRawAccu::~HRichSenderRawAccu(void) {
   if (sock) delete sock;
   if (mess) delete mess;
}



Bool_t HRichSenderRawAccu::init() {
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

    fRawIter = (HIterator*)fRawCat->MakeIterator();

    fRawAccu = pRichDet->buildCategory(catRichRaw);

    initNetwork();
    return kTRUE;
}
Int_t HRichSenderRawAccu::accumulate(){
   
    HRichRaw *pRichRaw;
    HRichRaw *pRichRawCopy;
    HLocation locat;
    fRawIter ->Reset();
    while((pRichRaw = (HRichRaw *)fRawIter->Next())){
	
	locat.set(3, pRichRaw->getSector(), pRichRaw->getRow(), pRichRaw->getCol());
	pRichRawCopy = (HRichRaw*)((HMatrixCategory*)fRawAccu)->getObject(locat);
	
	if (pRichRawCopy == NULL) {
	    pRichRawCopy = (HRichRaw*)((HMatrixCategory*)fRawAccu)->getSlot(locat);
	    if (pRichRawCopy != NULL) {
		pRichRawCopy = new(pRichRawCopy) HRichRaw;
		pRichRawCopy->setSector(locat[0]);
		pRichRawCopy->setCol(locat[2]);
		pRichRawCopy->setRow(locat[1]);
	    }
	}
	if (pRichRawCopy != NULL){
	    pRichRawCopy->addCharge(pRichRaw->getCharge());
	
	}

    }

    return iLocalCount++;
}

void HRichSenderRawAccu::initNetwork(){
    sock = new TSocket(hostname.Data(), nPort);
    Char_t str[32];
    sock->Recv(str, 32);
    mess = new TMessage(kMESS_OBJECT);
}
Bool_t HRichSenderRawAccu::finalize() {
    sock->Send("Finished"); 
    sock->Close();
    return kTRUE;
}
Int_t HRichSenderRawAccu::execute()
{
    if (accumulate()==nCount){
	iLocalCount =0;
	mess->Reset();             
	mess->WriteObject(fRawAccu);
	sock->Send(*mess);
	fRawAccu->Clear();
    }
    return 0;
}

