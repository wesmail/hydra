// File: hrichunpacker.h
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/09/02 14:17:57
//
#ifndef HRICHUNPACKER_H
#define HRICHUNPACKER_H
#include <TROOT.h>
#include <TVector.h>
#include <TFile.h>
#include "hldunpack.h"
#include "hcategory.h"
#include "haddef.h"
#include "hades.h"
#include "hevent.h"
#include "haddef.h"

class HRichUnpacker: public HldUnpack {
public:
    HRichUnpacker(void);
    ~HRichUnpacker(void);
    int getSubEvtId(void) const;
    Bool_t init(void);
    int execute(void); 
    
private:
    TVector * uipTable;   
    TVector * uip2xyPosition;   
    TVector * uip2xPosition;  
    TVector * uip2yPosition;
    TVector * xyPosition2uip;  
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
    ClassDef(HRichUnpacker,1)        // unpack RICH data
};
#endif /* !HRICHUNPACKER_H */






