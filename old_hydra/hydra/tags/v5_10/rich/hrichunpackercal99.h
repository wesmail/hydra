// File: HRichUnpackerCal99.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/12/17 17:12:02
//
#ifndef HRichUnpackerCal99_H
#define HRichUnpackerCal99_H
#include <TROOT.h>
#include <TVector.h>
#include <TFile.h>
#include "hldunpack.h"
#include "hcategory.h"
#include "haddef.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"
#include "hparset.h"
class HCategory;
class HIterator;
class HRichUnpackerCal99: public HldUnpack {
public:
    HRichUnpackerCal99(int);
    ~HRichUnpackerCal99(void);
    int getSubEvtId(void) const;
    Bool_t init(void);
    int execute(void); 
    void initCalPar(void);
    void diagnose(int,int,int,int);
    Bool_t finalize();
    unsigned int multMax;
    unsigned int multMin;
    int fEventNr;
    HParSet* getCalPar(){return fCalPar;}
private:
    int richId;
    TVector * uipTable;   
    TVector * uip2xyPosition;   
    TVector * uip2xPosition;  
    TVector * uip2yPosition;
    TVector * xyPosition2uip;  
    HCategory *pCalCat; 
    HParSet *fCalPar;   
    HParSet *fGeometry; 
    int charge;
    struct Upi{
	int sector;
	int controler;
	int port;
	int modul;
	int channel;
    } upi;
    int getUipIdent(int controler,int port, int modul, int channel){
	return controler * 10000 + port * 1000 + modul * 100 + channel;
    }
public:
    ClassDef(HRichUnpackerCal99,1)        // unpack RICH data
};
#endif /* !HRichUnpackerCal99_H */
















