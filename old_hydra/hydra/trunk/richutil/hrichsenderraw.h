// $Id: hrichsenderraw.h,v 1.5 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:15:53
//
#ifndef HRICHSENDERRAW_H
#define HRICHSENDERRAW_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TString.h"

class HCategory;
class HIterator;

class HRichSenderRaw : public HReconstructor {
public:

    HRichSenderRaw();
    HRichSenderRaw(const Text_t *name,const Text_t *title,const Text_t *host,Int_t port);
    ~HRichSenderRaw();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getRawCat(){return fRawCat;}
    void setRawCat(HCategory* pRawCat){fRawCat = pRawCat;}
    ClassDef(HRichSenderRaw,1) //ROOT extension
	
private:
    Int_t nPort;
    TString hostname;//!
    TSocket *sock;//!
    TMessage *mess;//!
    void initNetwork();
    HCategory *fRawCat; //!
};

#endif // HRICHSENDERRAW_H
