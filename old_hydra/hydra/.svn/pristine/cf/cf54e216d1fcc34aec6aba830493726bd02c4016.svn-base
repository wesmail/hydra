#ifndef HSHOWERROOTREADER_H
#define HSHOWERROOTREADER_H

#include "hgeantreader.h"
#include "heventheader.h"
#include "hlocation.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hindextable.h"
#include  <TTree.h> 
#include  "hshowergeanthit.h" 


class HShowerGRootReader:public HGeantReader {
private:
   HCategory *fShowGeantHit;          // pointer to geant hit data
   HLocation loc;                     // location  for new object 
   Int_t fEventId;                    // event number
   TTree *tr;                       // pointer to root tree 
  HShowerGeantHit *hit;              
   
public:
   HShowerGRootReader(void);
   ~HShowerGRootReader(void);
   Bool_t execute(void);
   Bool_t init(void);

ClassDef(HShowerGRootReader,0) //Geant-Root  reader for Shower data
};

#endif

