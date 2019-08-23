// File: hrichsenderraw.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/05/10 12:38:14
//
#ifndef HRICHSENDERRAW_H
#define HRICHSENDERRAW_H

#include "hreconstructor.h"
#include "hlocation.h"
#include <TSocket.h>
#include <TMessage.h>

class HCategory;
class HIterator;

class HRichSenderRaw : public HReconstructor {
public:

    HRichSenderRaw();
    HRichSenderRaw(Text_t *name,Text_t *title,Text_t *host,Int_t port);
    ~HRichSenderRaw();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getRawCat(){return fRawCat;}
    void setRawCat(HCategory* pRawCat){fRawCat = pRawCat;}
    ClassDef(HRichSenderRaw,1) //ROOT extension
	
private:
    Int_t nPort;
    Text_t *hostname;
    TSocket *sock;
    TMessage *mess;
    void initNetwork();
    HCategory *fRawCat; //Pointer to the raw data category
};

#endif // HRICHSENDERRAW_H
