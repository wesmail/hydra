//*-- Author : Walter Karig 
//*-- Modified : 05/06/98 by Manuel Sanchez
//*-- Modified : 97/11/13 15:24:59 by Walter Karig
//
#include "htofunpacker.h"
#include "htofraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "iostream.h"

/////////////////////////////////
//HTofUnpacker
//
//  This is the HUnpacker used to read HTof data from LMD files
//
//////////////////////////////////


FILE *fp;
Int_t lookup[256][22];

HTofUnpacker::HTofUnpacker(void) {
  pRawCat=NULL;
  pTofData = NULL;
  for(Int_t i=0; i<256; i++) {
    for(Int_t j=0; j<22; j++) lookup[i][j] = -1;
  }
}


HTofUnpacker::~HTofUnpacker(void) {
}


Bool_t HTofUnpacker::init(void) {
  pRawCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!pRawCat) {
      return kFALSE;
    } else {
      gHades->getCurrentEvent()->addCategory(catTofRaw,pRawCat,"Tof");
    }
  }
  fp = fopen("tofunp.txt","r");
  if(!fp) return kFALSE;
  Int_t ncol;
  Int_t idx, crate, slot;
  Char_t comm[100];
  while(1) {
    ncol = fscanf(fp,"%i %i %i %s",&idx,&crate,&slot,comm);
    if(ncol<0) break;
    if(crate>=0 && crate<256 && slot>0 && slot<22) {

      //      cout<<"idx : "<<idx<<"  crate : "<<crate<<"  slot :"<<slot<<
      //	"   "<<comm<<endl;

      lookup[crate][slot] = idx;
    }
  }
  return kTRUE;
}

int HTofUnpacker::execute() {

  int i = fill();
  return i;
}


int HTofUnpacker::fill() {

  //  cout << "In HTofUnpacker::fill()" << endl;

  if(pTofData) {
    delete [] pTofData;
    pTofData = NULL;
  }

  //  if(pSubEvt) cout << "pSubEvt exists" << endl;
  //  else cout << "pSubEvt doesn't exist" << endl;

  Int_t i,j;
  UInt_t bit5s; // first 5 bits if counting from the left
  Int_t moduleType = 0; // 0 - nothing, 1 = TDC or 2 = ADC
  UInt_t slot = 22;
  UInt_t crate = 256;
  UInt_t moduleCh = 32; 
  UInt_t data;
  Int_t idx = -1;
  UInt_t channels = 0;
  Int_t temp;
  Int_t flag; // 1 - TDC, 2 - ADC

  if(pSubEvt) {

    HLocation loc;

    loc.set(3,0,0,0);

    Int_t length = pSubEvt->getEnd() - pSubEvt->getData();

    //    printf(" length = %i \n",length);  

    pTofData = new UInt_t[length];
    UInt_t *pointer = pSubEvt->getData();
    for(i=0;i<length;i++) pTofData[i] = pointer[i];

    i = 0;
    while(i<length) {

      if(i==0) { 
	if(((UInt_t)(byte(1,pTofData[i]))) != 193) break; // not 0xC1(not TOF)
	i++;
	continue;
      }

      if(pTofData[i] == 0xc1fffff0) break; // end of TOF subevent

      bit5s = (UInt_t) byte(1,pTofData[i]) >> 3;

      if(bit5s > 21) {

	if(((UInt_t)(byte(1,pTofData[i]))) == 209) {
	  moduleType = 1; // 0xD1 - TDC
	  i++;
	  continue;
	}
	if(((UInt_t)(byte(1,pTofData[i]))) == 210) {
	  moduleType = 2; // 0xD2 - ADC
	  i++;
	  continue;
	}
      }

      slot = bit5s;
      crate = (UInt_t) byte(2,pTofData[i]); 

      //      cout << "crate : " << crate << "   slot : " << slot << endl;

      if(crate>=0 && crate<256 && slot>0 && slot<22) idx = lookup[crate][slot];
      

      channels = (UInt_t) byte(3,pTofData[i++]);

      //      cout << "idx : " << idx << "   channels : " << channels << endl;

      for(j=i; j < (Int_t)(i+channels); j++) {
	moduleCh = (moduleCh | (pTofData[j] & 0x001f0000)) >> 16;
        data = (pTofData[j] & 0xfff ) % 0xfff;

	if(idx>=0 && idx<48 && moduleCh >= 0 && moduleCh < 32) {

	  //	  cout << "crate : " << crate << "   slot : " << slot << 
	  //	    "  moduleCh :" << moduleCh << "   data : " << data << endl;

	  if(idx < 24) { // TDC

	    loc[0] = idx/4;
	    temp = idx%4;
	    flag = 1;
	  }

	  if(idx >= 24) { // ADC

	    loc[0] = (idx-24)/4;
	    temp = (idx-24)%4;
	    flag = 2;
	  }

	  if(temp==0 || temp==2) loc[1] = moduleCh/8;     // modules 1-4
	  if(temp==1 || temp==3) loc[1] = moduleCh/8 + 4; // modules 5-8

	  loc[2] = moduleCh % 8;
	  

	  //--------------------------------------------------------
	  //that cabling was changed

	  /*	  if(temp==0) { // modules 1-2
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 0;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 1;
	      loc[2] = moduleCh%8;
	    }
	  }
	  if(temp==1) { // modules 3-4
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 2;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 3;
	      loc[2] = moduleCh%8;
	    }
	  }
	  if(temp==2) { // modules 5-6
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 4;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 5;
	      loc[2] = moduleCh%8;
	    }
	  }
	  if(temp==3) { // modules 7-8
	    if(moduleCh/8 == 0 || moduleCh/8 == 2) {
	      loc[1] = 6;
	      loc[2] = moduleCh%8;
	    }
	    if(moduleCh/8 == 1 || moduleCh/8 == 3) {
	      loc[1] = 7;
	      loc[2] = moduleCh%8;
	    }
	  }*/
	  //--------------------------------------------------

	  HTofRaw *pCell = (HTofRaw*) pRawCat->getObject(loc);

	  if (pCell == NULL) { 
	    pCell = (HTofRaw*) pRawCat->getSlot(loc);
	    if (pCell != NULL) {
	      pCell = new(pCell) HTofRaw;   // needed to get a streamer
	                                    // for HTree::Fill() 
	      pCell->setRightTime(0.0);
	      pCell->setLeftTime(0.0);
	      pCell->setRightCharge(0.0);
	      pCell->setLeftCharge(0.0);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t)loc[1]);
	      pCell->setCell((Char_t)loc[2]);
	    } else return kFALSE;
	  }

	  if(temp==0 || temp==1) { // right sides
	    if(flag==1) { // TDC
	      pCell->setRightTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);

	    }	      
	    if(flag == 2) { // ADC
	      pCell->setRightCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }

	  if(temp==2 || temp==3) { // left sides
	    if(flag == 1) { // TDC
	      pCell->setLeftTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setLeftCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }


	  //--------------------------------------------------------
	  //that cabling was changed

	  /*if(moduleCh/16 == 0) { // left sides

	    if(flag == 1) { // TDC
	      pCell->setLeftTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setLeftCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }

	  if(moduleCh/16 == 1) { // right sides

	    if(flag == 1) { // TDC
	      pCell->setRightTime((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	    if(flag == 2) { // ADC
	      pCell->setRightCharge((Float_t) data);
	      pCell->setSector((Char_t) loc[0]);
	      pCell->setModule((Char_t) loc[1]);
	      pCell->setCell((Char_t) loc[2]);
	    }
	  }*/
	  //--------------------------------------------------


	} // idx>=0 && idx<48 && moduleCh >= 0 && moduleCh < 32

      } // j=i; j < (Int_t)(i+channels); j++

      i += channels + 1;

      if(pTofData[i] == 0xd1fffff0 || pTofData[i]  == 0xd2fffff0) i++;

    }
      
  }

  return kTRUE;

}


ClassImp(HTofUnpacker) 








