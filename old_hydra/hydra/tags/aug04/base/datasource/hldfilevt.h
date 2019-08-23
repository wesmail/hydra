//

//////////////////////////////////////////////////////
//  HldFileEvent
//
//  Class for reading events from files
//
//////////////////////////////////////////////////////


#ifndef HLDFILEVT_H
#define HLDFILEVT_H

#include <fstream>
#include "hldevt.h"
#include "hldsubevt.h"
#include "hldunpack.h"

class HldFilEvt: public HldEvt {

private:
std::istream* file;


public:
 HldFilEvt() {                    // No input file
   file = 0;
   init();
 }
 HldFilEvt(const char* name) {    // read from file
   file = new std::ifstream(name);
   init();
 }
 ~HldFilEvt() {
   if (file) delete file;
   delete[] pData;
 }
 void init() {
   pHdr = (UInt4*) &hdr;
   pData = 0;
   lastSubEvtIdx = 0;
 }
 Bool_t setFile(const Text_t *name);

 Bool_t readSubEvt(size_t i);
 Bool_t read();
 Bool_t execute();
 Bool_t swap();
 public:
//ClassDef(HldFilEvt, 0)           // HADES LMD Event
};
#endif /* !HLDFILEVT_H */  

