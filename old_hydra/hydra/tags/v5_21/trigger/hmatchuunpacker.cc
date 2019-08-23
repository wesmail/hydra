#include "hmatchuunpacker.h"
#include "hcategory.h"
#include "hades.h"
#include "hevent.h"
#include "hmatchurich.h"
#include "hmatchushower.h"
#include "triggerinfodef.h"
#include "hlinearcategory.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hmatchutof.h"


const Int_t HMatchUUnpacker::kSubEvtId = 512;

HMatchUUnpacker::HMatchUUnpacker(void) {
  fCatRich=0;
  fCatShower=0;
}

HMatchUUnpacker::~HMatchUUnpacker(void) {
  fCatRich=0;
  fCatShower=0;
}


Bool_t HMatchUUnpacker::init(void) {
  if (gHades) {
    HEvent *event=gHades->getCurrentEvent();
    if (event) {
      fCatRich=event->getCategory(catMatchURich);
      if (!fCatRich) {
	fCatRich=new HLinearCategory("HMatchURich");
	event->addCategory(catMatchURich,fCatRich,"Trigger");
      }
      fCatShower=event->getCategory(catMatchUShower);
      if (!fCatShower) {
	fCatShower=new HLinearCategory("HMatchUShower");
	event->addCategory(catMatchUShower,fCatShower,"Trigger");
      }
      fCatTof = event->getCategory(catMatchUTof);
      if (!fCatTof) {
	fCatTof = new HLinearCategory("HMatchUTof");
	event->addCategory(catMatchUTof,fCatTof,"Trigger");
      }
    } else return kFALSE; //! event
  } else return kFALSE; //! gHades

  return kTRUE;
}

Int_t HMatchUUnpacker::decodeRich(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Rich subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  // trigger code (pData[0]) is skipped so
  //Int_t length=getRichSize(pData[2])*2;  //One 32bit word goes to 2.

  Int_t length=getRichSize(pData[2])*2;  //One 32bit word goes to 2.
  Int_t r=length,i=0,bc;
  UInt_t fifo,colPat,segId,row;
  HMatchURich *muRich=0;

  if (length>maxLength) {
    Warning("decodeRich","SubEvent size too big");
    r=-1;
  } else {
    for (i=3;i<length;i+=2) { //i=3 --> first data word
      // Internal checks does not works proprely !!!
      //  if ( checkRichMSW(pData[i]) && checkRichLSW(pData[i+1]) ) {
        if(kTRUE){
	UInt_t tempPat=0;
	fifo=getRichFifoNb(pData[i])-1;
	colPat=getRichColumPattern(pData[i]);
	segId=getRichSegId(pData[i+1]);
	row=getRichRow(pData[i+1]);

	bc=0;
	tempPat=colPat;
	if (fDebugMode) printf(" %i  col pattern \n", tempPat);
	while ( bc < 8 ) {
	  //	  while (bc<8 && ((tempPat & 0x1) == 0)) {tempPat>>=1; bc++;}
	  if ( (tempPat & 0x1) != 0 )  { 
	    if (fDebugMode) printf(" %i \n",bc);        
	  if ( bc < 8 ) {
	    muRich=(HMatchURich *)fCatRich->getNewSlot(fLoc);
	    if (muRich) {
	      muRich=new (muRich) HMatchURich;
	      muRich->setFifo(fifo);
	      muRich->setColumnPattern(colPat);

	      muRich->setSegmentId(segId);
	      muRich->setRowNb(row);
	      muRich->setColumn(95 - fifo * 8 - bc);
	      muRich->setRow(row-24);
	      if (fDebugMode) {
		printf(" ** rich object found  ** \n"); 
		printf(" decoding row:%i column:%i fifo:%i bc:%i \n", row-24, 
		       95 - fifo * 8 - bc,fifo,bc);
	      } 
	    
	    }
	  }
	}
       tempPat>>=1; bc++;
       }

      } else {
	Warning("decodeRich","Corrupted data word detected");
      }
    }
  }
  return r;
}

Int_t HMatchUUnpacker::decodeTof(UInt_t *pData,Int_t maxLength) {
  // return the number of bytes in Tof subevent.
  Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF)); 
  Int_t triggerTag = (pData[2] << 16) | (pData[3] & 0x0000FFFF);
  TObject *slot;
  HMatchUTof *mtof=0;
  Int_t headerSize=4;
  UInt_t dataWord=0;
  Int_t i=0,cursor=0;

  if ( (length > maxLength) ) {
    Warning("decodeTof","Inconsistent sizes: length %x, maxLength=%x",length,maxLength);
  } else {
    slot = fCatTof->getNewSlot(fLoc);
    if (slot) {
      mtof = new(slot) HMatchUTof;
      mtof->setTriggerTag(triggerTag);
      for (cursor=headerSize, i=0; i < length; cursor+=2, i++) {
	dataWord = (pData[cursor] << 16) | (pData[cursor+1] & 0xFFFFFFFF);
	mtof->setDataWord(i,dataWord);
      }
    } else Warning("decodeTof","No memory available for TOF subsubevent");
  }
  return length;
}

Int_t HMatchUUnpacker::decodeShower(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Shower subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  Int_t length=pData[0]; //Each 16bit word goes to a 32bit word.
  Int_t r=length;
  Int_t cursor=1;
  Int_t ipc,col,rowPat;
  Int_t bc=0;
  HMatchUShower *muShower=0;
  if (fDebugMode) printf(" lenght : %x %i :%i \n",length, length,maxLength);
  if ( (length > maxLength) && pData[cursor] != 0x1111) {
    Warning("decodeShower","Inconsistency detected");
    r=-1;
  } else {
    cursor++;
    while (cursor<length) {
      ipc=pData[cursor];
      cursor++;
      cursor++;  //Bypass trigger tag
      while ( cursor < length && !isShowTrailer(pData[cursor]) ) {
	col=getShowColumn(pData[cursor]); cursor++;
	rowPat=getShowRowPattern(pData[cursor]); 
	//Add to category
        if (fDebugMode) printf(" row pattern initial : %x \n",pData[cursor]); 
        cursor++; 
       
       bc=0;
       Int_t tempPat=rowPat;
       if (fDebugMode)  printf(" %i  col pattern \n", tempPat);
	while ( bc < 15 ) {
	  if( (tempPat & 0x1) != 0 ){
        muShower=(HMatchUShower *)fCatShower->getNewSlot(fLoc);
	if (muShower) {
	  muShower=new(muShower) HMatchUShower;
	  muShower->setIPC(ipc);
	  muShower->setColumn(col);
	  muShower->setRowPattern(rowPat);
          muShower->setRow(bc);  
	  if (fDebugMode) {
	    printf("** shower obj created **\n"); 
	    printf(" ipc:%i col:%i row:%i rowPattern:%x \n",ipc,col,bc,
		   rowPat);
	  }    
	  
           }
	  }
	tempPat>>=1; bc++; 
	}
       }
      // need to know the trailer specific pattern
      while (cursor<length && isShowTrailer(pData[cursor]))
	{
     Warning("decodeShower","bypass trailer %x",pData[cursor]);
     cursor++; 
    }
     //Bypass trailers.
    }
  }
  return r;
}

Int_t HMatchUUnpacker::execute(void) {
    if (pSubEvt) { 
    Int_t length=pSubEvt->getDataLen(); 
    if (fDebugMode) printf(" subevt found  with lenght: %i \n",length);  
    UInt_t *pData=pSubEvt->getData();
    Int_t subSize;
    if ( (subSize=decodeRich(pData,length)) == -1) return kFALSE;
    length-=subSize;
    pData+=subSize+1;  
    if (fDebugMode)  printf(" subsize:%i \n",subSize); 
    if ( (subSize=decodeShower(pData,length)) == -1) return kFALSE;
    length-=subSize;
    pData+=subSize;
    if ( (subSize=decodeTof(pData,length)) == -1) return kFALSE;
    }   //! psubEvt
  return kTRUE;
}


 



ClassImp(HMatchUUnpacker)
