#ifndef __HCLONESARRAY__
#define __HCLONESARRAY__


#include <TClonesArray.h>
#include "hbarray.h"
#include <TMath.h>



class HClonesArray : public TClonesArray {
private:
  HBArray *barray; //!linked bit field  
public:
   HClonesArray();
   HClonesArray(const Char_t * classname, Int_t s=1000, Bool_t b=kFALSE); 
   ~HClonesArray(){
   }
   void setBitArray(HBArray* arr) { barray = arr;}  
   void Compress();
   // void Streamer (TBuffer &b); 

ClassDef ( HClonesArray, 1 )
};

#endif 
 




