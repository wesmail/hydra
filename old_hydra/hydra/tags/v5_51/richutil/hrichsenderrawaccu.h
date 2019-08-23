// File: hrichsenderrawaccu.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/29 22:01:35
//
#ifndef HRICHSENDERAWACCU_H
#define HRICHSENERRAWACCU_H

#include "hreconstructor.h"
#include "hlocation.h"
#include <TSocket.h>
#include <TMessage.h>

class HCategory;
class HIterator;
class HIterator;

class HRichSenderRawAccu : public HReconstructor {
public:

    HRichSenderRawAccu();
    HRichSenderRawAccu(Text_t *name,Text_t *title,Text_t *host,Int_t port,Int_t evt);
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
    Text_t *hostname;//!
    TSocket *sock;//!
    TMessage *mess;//!
    void initNetwork();
    HCategory *fRawCat; //!
    HCategory *fRawAccu;//!
    HIterator *fRawIter;//!
};

#endif // RICHSENDERRAWACCU_H

