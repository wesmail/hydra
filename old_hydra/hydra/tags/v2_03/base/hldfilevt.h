//

//////////////////////////////////////////////////////
//  HldFileEvent
//
//  Class for reading events from files
//
//////////////////////////////////////////////////////


#ifndef HLDFILEVT_H
#define HLDFILEVT_H

#include <fstream.h>
#include "hldevt.h"
#include "hldsubevt.h"
#include "hldunpack.h"

class HldFilEvt: public HldEvt {

private:
istream* file;


public:
HldFilEvt() {                    // data from stdin
    file = &cin;
    init();
}
HldFilEvt(const char* name) {    // read from file
    file = new ifstream(name);
    init();
}
~HldFilEvt() {
    if (file != &cin)
      delete file;
      delete[] pData;
}
void init() {
    pHdr = (UInt4*) &hdr;
    pData = 0;
    lastSubEvtIdx = 0;
}

Bool_t readSubEvt(size_t i);
Bool_t read();
Bool_t execute();
public:
//ClassDef(HldFilEvt, 0)           // HADES LMD Event
};
#endif /* !HLDFILEVT_H */  

