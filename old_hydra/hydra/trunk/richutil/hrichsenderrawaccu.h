// $Id: hrichsenderrawaccu.h,v 1.5 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:16:10
//
#ifndef HRICHSENDERAWACCU_H
#define HRICHSENERRAWACCU_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TString.h"

class HCategory;
class HIterator;
class HIterator;

class HRichSenderRawAccu : public HReconstructor {
public:

    HRichSenderRawAccu();
    HRichSenderRawAccu(const Text_t *name,const Text_t *title,const Text_t *host,Int_t port,Int_t evt);
    ~HRichSenderRawAccu();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getRawCat(){return fRawCat;}
    Int_t accumulate();
    void setRawCat(HCategory* pRawCat){fRawCat = pRawCat;}
    ClassDef(HRichSenderRawAccu,1) //ROOT extension
	
private:
    Int_t nCount;
    Int_t iLocalCount;
    Int_t nPort;
    TString hostname;//!
    TSocket *sock;//!
    TMessage *mess;//!
    void initNetwork();
    HCategory *fRawCat; //!
    HCategory *fRawAccu;//!
    HIterator *fRawIter;//!
};

#endif // RICHSENDERRAWACCU_H

