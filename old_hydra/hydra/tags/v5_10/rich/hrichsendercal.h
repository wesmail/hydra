// File: hrichsendercal.h
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 00/05/10 12:34:22
//
#ifndef HRICHSENDERCAL_H
#define HRICHSENDERCAL_H

#include "hreconstructor.h"
#include "hlocation.h"
#include <TSocket.h>
#include <TMessage.h>

class HCategory;
class HIterator;

class HRichSenderCal : public HReconstructor {
public:

    HRichSenderCal();
    HRichSenderCal(Text_t *name,Text_t *title,Text_t *host,Int_t port);
    ~HRichSenderCal();

    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    HCategory* getCalCat(){return fCalCat;}
    void setCalCat(HCategory* pCalCat){fCalCat = pCalCat;}
    ClassDef(HRichSenderCal,1) //ROOT extension
	
private:
    Int_t nPort;
    Text_t *hostname;
    TSocket *sock;
    TMessage *mess;
    void initNetwork();
    HCategory *fCalCat; //Pointer to the Cal data category
};

#endif // HRICHSENDERCAL_H
