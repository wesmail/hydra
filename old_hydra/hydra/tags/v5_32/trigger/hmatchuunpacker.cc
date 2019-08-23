#include "hmatchuunpacker.h"
#include "hcategory.h"
#include "hades.h"
#include "hrecevent.h"
#include "hmatchurich.h"
#include "hmatchushower.h"
#include "triggerinfodef.h"
#include "hlinearcategory.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hmatchutofheader.h"
#include "hruntimedb.h"
#include "hpartialevent.h"

#define DEG_TO_RAD(x) (x*TMath::Pi()/180)

const Int_t HMatchUUnpacker::kSubEvtId = 512;

HMatchUUnpacker::HMatchUUnpacker(Int_t unites,Int_t flog) {
  fCatRich=0;
  fCatShower=0;
  fUnit=unites;
  fLog = flog;
  richc = 0; 
  if(fLog == 1){
    fMU_logfile = fopen("MU-IpuRich.log","w");
    if (fMU_logfile == NULL){
      printf("Cannot open MU- log file\n");
      exit(-1);
    }
  }else  fMU_logfile = stderr; 
}

HMatchUUnpacker::~HMatchUUnpacker(void) {
  fCatRich=0;
  fCatShower=0;

}


Bool_t HMatchUUnpacker::init(void) {
  if (gHades) {
    event= (HRecEvent*) gHades->getCurrentEvent();
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
        event->getPartialEvent(catMatchUTof)->
               setSubHeader(new HMatchUTofHeader ); 
      }
   
      HRuntimeDb* rtdb=gHades->getRuntimeDb();
      if(rtdb){  

      fShowerParam = (HShowerUnpackParam*)rtdb->
                                            getContainer("ShowerUnpackParam");
      if (!fShowerParam) {
	Warning("init","ShowerUnpackParam container not found in rtdb "); 
        Warning("init","call default initialisation");
	fShowerParam = new HShowerUnpackParam;
	fShowerParam->defaultInit();
      }
      }else {
	Error("init","no runtimeDb found !");
        return kFALSE; //!rtdb
      }

    } else return kFALSE; //! event
  } else return kFALSE; //! gHades

  return kTRUE;
}


Int_t  HMatchUUnpacker::decodeMUData(UInt_t *pData,Int_t maxLength) {
  Int_t length= pData[0];
  Int_t r= length+1;
  if (fDebugMode)  printf(" decodeMUData length:%i \n", length);
  if ( length > maxLength) {
    Error("decodeMUData","size inconsistency");
    r=-1;
  } 
  
  return r;
}

Int_t HMatchUUnpacker::decodeRich(UInt_t *pData,Int_t maxLength) {

  Int_t length= pData[0]*2;
  Int_t r= length+1,ipusize; 
  UInt_t* ptr= pData+1; 

  if ( length == 6 ) richc++;    
    if (fDebugMode)  printf(" decodeRich length:%i pdata0:%x  
                              maxLength:%i\n", length
                          , pData[0], maxLength);
  //   printf(" decodeRich length:%i pdata0:%x  
  //                           maxLength:%i\n", length
  //                         , pData[0], maxLength); 
  if ( length > maxLength) {
                        Error("decodeRich","size inconsistency");
                        r=-1;
       
  }
 int i=0; 
  ipusize=0;
  while ( length > 0 && r>0 ) {
    //   printf("ipu:%i length:%i\n" , ++i, length);
   ipusize=decodeIpuRich(ptr,length);
   //   printf("ipuSize:%i  \n", ipusize);
  
   if(ipusize==-1) r=-1;
   else{
     length-=ipusize; 
     ptr+=ipusize;
   }

  }
  return r;
}


Int_t HMatchUUnpacker::decodeIpuRich(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Rich subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  // trigger code (pData[0]) is skipped so
  

  Int_t length=getRichSize(pData[1])*2;  //One 32bit word goes to 2.
  Int_t r=length,i=0,bc;
  UInt_t fifo,colPat,segId,row;
  HMatchURich *muRich=0;

  if (fDebugMode)  printf(" decodeIpuRich length:%i \n", length);
  if (length>maxLength) {
    Error("decodeRich","SubEvent size too big");
    //  printf(" decodeIpuRich pData[1]:%i 
    //              length:%i maxL:%i \n",pData[1], length, maxLength);
    r=-1;
  } else {
    for (i=2;i<length;i+=2) { //i=2 --> first data word
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
	if (fRichDebugMode) printf("-I-decodeIpuRich: colpattern: x%0x \n", tempPat);

	while ( bc < 8 ) {
	  //	  while (bc<8 && ((tempPat & 0x1) == 0)) {tempPat>>=1; bc++;}
	  if ( (tempPat & 0x1) != 0 )  { 
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
	      if (fRichDebugMode) {
		printf(" -I-decodeIpuRich: rich object found  ** \n"); 
		printf(" -I-decodeIpuRich: decoding ...  
                                            row:%i  
                                            column:%i   
                                            fifo:%i  
                                            bitpos:%i  
                                            segId:%i \n  ", 
		       row-24, 
		       95 - fifo * 8 - bc,fifo,bc,segId);
              if(pSubEvt)  pSubEvt->dumpIt(fMU_logfile);
	      } 
	      
	    }
	  }
	  }
	  tempPat>>=1; bc++;
	}
	
	} else {
	  Error("decodeRich","Corrupted data word detected");
	}
    }
  }
  return r;
}

Int_t HMatchUUnpacker::decodeTof(UInt_t *pData,Int_t maxLength) {

  Int_t length= pData[0]*2;
  Int_t r= length+1,ipusize; 
  UInt_t* ptr= pData+1; 

  if (fDebugMode)  printf(" decodeTof length:%i \n", length);

  if ( length > maxLength) {
                        Error("decodeTof","size inconsistency");
                        r=-1;
  } 
  while ( length > 0 && r>0 ) {
   ipusize=decodeIpuTof(ptr,length);
   if(ipusize==-1) r=-1;
   else{
     length-=ipusize; 
     ptr+=length;
    }
  }
  return r;

}

Int_t HMatchUUnpacker::decodeIpuTof(UInt_t *pDat,Int_t maxLength) {
  // return the number of bytes in Tof subevent.

  Int_t length =  ((pDat[0]<<16) | (pDat[1] & 0x0000FFFF))*2; 
  Int_t r=length+2;
  Int_t tofregister = (pDat[2] << 16) | (pDat[3] & 0x0000FFFF);
  HMatchUTofHeader *tofheader = (HMatchUTofHeader*)( 
    event->getPartialEvent(catMatchUTof)->getSubHeader());

  if (fDebugMode)  printf(" decodeIpuTof length:%i \n", length); 

  if ( length > maxLength) {
                        Error("decodeIpuTof","size inconsistency");
                        
                       return ( -1 );
  } 

 
  if (!tofheader) {
    Error(" decodeIpuTof","header missing"); 
    r=-1;
  } else {

    TObject *slot;
    HMatchUTof *mtof=0;
    Int_t headerSize=4;
    
    UInt_t dataWord=0;
    Int_t i=0,cursor=0;
   
    tofheader->setTriggers(getTofTriggerTag(tofregister),getTofTriggerCode(tofregister));
    tofheader->setID(getTofId(tofregister));  
    tofheader->setVeto(getTofVeto(tofregister));  
     
    for (cursor=headerSize, i=0; i < length - 2; cursor+=2, i++) {
      slot = fCatTof->getNewSlot(fLoc);
      if (slot) {
	mtof = new(slot) HMatchUTof;
	dataWord = (pDat[cursor] << 16) | (pDat[cursor+1] & 0xFFFFFFFF);
	fillTofData(mtof,dataWord);
      } else Warning("decodeTof","No memory available for TOF subsubevent");
    } 
  }
  return r;
}

Bool_t HMatchUUnpacker::fillTofData( HMatchUTof* mtof, UInt_t dataword){
  Int_t theta = dataword & 0x7fff; 
  Int_t dtheta = (dataword >>15 ) & 0x1;
  Int_t phi = (dataword >> 16) & 0x7fff;
  Int_t dphi = (dataword >> 31) & 0x1; 
   
  Float_t atheta =  theta/32768. * 90.; 
  Float_t aphi =  phi/32768. * 360.;  
 
  if (fUnit == 1){
    // conversion to RAdians
                  atheta =  DEG_TO_RAD(atheta);
		  aphi   =    DEG_TO_RAD(aphi);
  } 
                   
  mtof->setTheta(atheta, dtheta); 
  mtof->setPhi(aphi,dphi);

  return kTRUE;
} 


Int_t HMatchUUnpacker::storeShowElements(Int_t buildId, Int_t rowPattern, Int_t col){
  Int_t bc;
  HMatchUShower *muShower=0;
  Int_t tempPat=rowPattern;
  Int_t nSector,nRowOffset,nSwap;    
  bc=nSector=nRowOffset=nSwap=0;
  HShowerSubIdPar *pSubIdPar=0;

  if (fShowerParam) 
    pSubIdPar= showerParamConfigure(fShowerParam, buildId);
  else
     Error("storeShowElements","no parameter container found");

   
  if (fShowerDebugMode)  printf(" %x  row pattern \n", tempPat);
  
  while ( bc < 16 ) {
    if( (tempPat & 0x1) != 0 ){
      muShower=(HMatchUShower *)fCatShower->getNewSlot(fLoc);
      if (muShower) {
	muShower=new(muShower) HMatchUShower;
	muShower->setBuildID(buildId);
	muShower->setColumn(col);
	muShower->setRowPattern(rowPattern);
	muShower->setRow(bc);  
         if(pSubIdPar){
	   nSector = pSubIdPar->getSector();
	   nRowOffset = 16 * pSubIdPar->getPartOfSector();
	   nSwap = (pSubIdPar->getSwap()) ? -1 : 1;
	   if (nSwap == -1) nRowOffset += 8;
	   muShower->setSector(nSector); 
	   muShower->setRowOffset(nRowOffset);
	 }

	 if (fShowerDebugMode) {
	   printf(" -I-storeShowerElements: shower object found  ** \n"); 
	   printf(" -I-storeShowerElements: decoding ...  
                                            buildID:%i  
                                            column:%i   
                                            row:%i  
                                            bitpos:%i  
                                            rowoffset:%i \n", 
		  buildId, col,bc,bc,nRowOffset);
	   if(pSubEvt)  pSubEvt->dumpIt(fMU_logfile);
	 }
      }
    }
    tempPat>>=1; bc++; 
  }//! while
  return 0;
}


HShowerSubIdPar*  HMatchUUnpacker::showerParamConfigure(
                                   HShowerUnpackParam* param, Int_t subEvtId){
  HShowerSubIdPar *pSubIdPar;
  pSubIdPar = param->getSubIdPar(subEvtId);
  if (pSubIdPar) 
    return   pSubIdPar;
  else{
    //  printf("showerParamConfigure:
    //         not corrected initialization of MUShowerUnpacker id = %d\n",subEvtId );
   return NULL;
  } 
}



Int_t HMatchUUnpacker::decodeShower(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Shower subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  //Each 16bit word goes to a 32bit word.

  Int_t length=pData[0]; 
  Int_t r=length;
  UInt_t *pDataInit= pData;
  UInt_t *ptr= pData; 
  Int_t col,rowPat;
  Int_t showBuildId=0;

  if (fDebugMode) printf(" decodeShower  lenght : %x %i maxL:%i \n",
                           length, length,maxLength);
  if ( (length > maxLength) && pData[1] != 0x1111) {
    Warning("decodeShower","Inconsistency detected");
    r=-1;
  } else {
    
    for (;ptr < pDataInit+length;ptr++ ){   
      // test for the # trailers (debug mode) 
      
       switch ( *ptr ){
        case 0x2: 
	  if (fDebugMode) {printf(" status flag %x \n", *ptr);   
                           printf(" status %x \n", *(++ptr));
          }else{
          }   
             break;
        case 0x3: 
	 if (fDebugMode){ printf(" buildID flag %x \n", *ptr);
                          printf(" decode buildID  %i \n",
                                 getShowBuildID(++ptr ));
         }else{
	   showBuildId = getShowBuildId(++ptr);   
         }
             break;
        case 0x4: 
	 if (fDebugMode) {printf(" Row flag %x \n", *ptr);    
	                  printf(" Row PAttern %x \n",*(++ptr));
	 }else{
           rowPat=getShowRowPattern(++ptr); 
         }
             break;
        case 0x5: 
         if (fDebugMode) {printf(" Col flag %x \n", *ptr);   
                          printf(" Col %x \n", *(++ptr));
	 }else{
           col = getShowColumn(++ptr);
           storeShowElements(showBuildId,rowPat,col);
	 }    
         break;
	
	    
	   //shows frame trailer 
        case 0x6: 
                 if (fDebugMode){ 
                          printf(" f counter flag %x \n", *ptr);   
                          printf(" f counter %x \n", *(++ptr));
                 } 
             break;
        case 0x7: 
                  if (fDebugMode){
	                  printf(" f marker flag %x \n", *ptr);   
	                  printf(" f marker %x \n", *(++ptr));
		  }
             break;
	}
       
     }//!switch
    }
    
 
 return r;
}


Int_t HMatchUUnpacker::execute(void) {
    if (pSubEvt) { 
    Int_t length=pSubEvt->getDataLen(); 
    if (fDebugMode) printf(" subevt found  with lenght: %i \n",length);  
    UInt_t *pData=pSubEvt->getData();
    Int_t subSize;
    if ( (subSize=decodeMUData(pData,length)) == -1) return kFALSE;
    length-=subSize;
    pData+=subSize;  
    if ( (subSize=decodeRich(pData,length)) == -1) {
    if ( fLog == 1 )  pSubEvt->dumpIt(fMU_logfile);
      return kTRUE;
    }
    length-=subSize;
    pData+=subSize;  
    if (fDebugMode)  printf(" subsize:%i \n",subSize); 
    if ( (subSize=decodeShower(pData,length)) == -1) return kFALSE;
    length-=subSize;
    pData+=subSize;
    if ( (subSize=decodeTof(pData,length)) == -1) return kTRUE;
    }   //! psubEvt
  return kTRUE;
}


 



ClassImp(HMatchUUnpacker)
