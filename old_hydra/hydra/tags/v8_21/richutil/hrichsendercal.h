// $Id: hrichsendercal.h,v 1.5 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 02/09/25 18:15:36
//
#ifndef HRICHSENDERCAL_H
#define HRICHSENDERCAL_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "TSocket.h"
#include "TMessage.h"
#include "TString.h"

class HCategory;
class HIterator;

class HRichSenderCal : public HReconstructor {
public:

    HRichSenderCal();
    HRichSenderCal(const Text_t *name,const Text_t *title,const Text_t *host,Int_t port);
    ~HRichSenderCal();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getCalCat(){return fCalCat;}
    void setCalCat(HCategory* pCalCat){fCalCat = pCalCat;}
    ClassDef(HRichSenderCal,1) //ROOT extension
	
private:
    Int_t nPort;
    TString hostname;//!
    TSocket *sock;//!
    TMessage *mess;//!
    void initNetwork();
    HCategory *fCalCat; //!
};

#endif // HRICHSENDERCAL_H
