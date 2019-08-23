// $Id: hrichsender.h,v 1.4 2008-09-18 13:05:36 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:15:22
//
#ifndef HRICHSENDER_H
#define HRICHSENDER_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TSocket.h"
#include "TMessage.h"

class HCategory;
class HIterator;

class HRichSender : public HReconstructor {
public:

    HRichSender();
    HRichSender(Text_t *name,Text_t *title,Text_t *host,Int_t port);
    ~HRichSender();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getRawCat(){return fRawCat;}
    void setRawCat(HCategory* pRawCat){fRawCat = pRawCat;}
    ClassDef(HRichSender,1) //ROOT extension
	
private:
    Int_t nPort;
    Text_t *hostname;//!
    TSocket *sock;//!
    TMessage *mess;//!
    void initNetwork();
    HCategory *fRawCat; //!
};

#endif // HRICHSENDER_H


