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
  // logging  mechanism
  if(fLog > 0 ){
    fMU_logfile = fopen("MU-Bad-Angle.log","w");
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
    muData =  event->getHeader()->getMUData(); 
       
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
   
     fCatLeptons = event->getCategory(catMatchULeptons);
    

     if (!fCatLeptons) {
	fCatLeptons = new HLinearCategory("HMULeptons");
	event->addCategory(catMatchULeptons,fCatLeptons,"Trigger");
	//        event->getPartialEvent(catMatchULeptons)->
	//        setSubHeader(new HMatchULeptonsHeader ); 
      }
     if (!fCatDiLeptons) {
	fCatDiLeptons = new HLinearCategory("HMUDiLeptons");
	event->addCategory(catMatchUDiLeptons,fCatDiLeptons,"Trigger");
	//        event->getPartialEvent(catMatchULeptons)->
	//        setSubHeader(new HMatchULeptonsHeader ); 
      }
   
      HRuntimeDb* rtdb=gHades->getRuntimeDb();
      if(rtdb){  

      fShowerParam = (HShowerUnpackParam*)rtdb->
                                            getContainer("ShowerUnpackParam");
       
      // printf (" fShowerParam:%x \n",fShowerParam);
      //  fShowerParam->Dump(); 
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
  UInt_t *pDataL=0,*pDataD=0;
  Int_t length;
  Int_t r;
  UInt_t *ptr =0;
  Int_t triggerCode=-1;

  if (muData->getTriggerCodeFirstFormat() == 0 ){
    length= pData[0]; // in 16 bit  word 
    r= length+1;
    ptr = pData+1;
  }
  else {
    triggerCode = *pData;
    length= pData[1]; // in 16 bit  word  
    ptr= pData +2; 
    r= length+2;
  }

  if (fDebugMode)  printf ("MU-Data length :%i \n", length ); 
  
  if (length == 1 ) return r; 
  HMULeptons *muLept=0;
  HMUDiLeptons *muDiLept=0;  
  
  if (fDebugMode)  printf(" decodeMUData length, maxLength:%i %i
               \n", length,maxLength);
  if ( length > maxLength) {
    Error("decodeMUData","size inconsistency");
    r=-1; return r; 
  } 

  muData->clear();  
  muData->setTriggerCode(triggerCode);
  Int_t muDataSize = 0 ;

   muDataSize =  muData->decode(ptr,length); 

   if ( muDataSize < 0 ) { 
     Error("decodeMUData","data word decoding problem ");
     r =-1; return r;
   }

   // no leptons found
   if (muData->getNLeptons() == 0 ) return r;
   if ( muData->getNLeptons() > 1000 ) {
     printf(" nleptons out of bound. %i \n", muData->getNLeptons());
     return (-1); 
  } 

   pDataL=pData+muDataSize+1;

   Int_t dataw=0;
   Float_t thetal,phil,eflag,moml; 
   thetal = 0.0;
   phil   = 0.0;
   eflag  = 0.0;
   moml   = 0.0;

  if (fDebugMode)  printf("MU-Data decoding %i leptons \n", 
                                           muData->getNLeptons()); 
   

      for (Int_t nl=0; nl<(Int_t)muData->getNLeptons(); nl+=2){
	dataw = ((pDataL[nl]<<16) | (pDataL[nl+1] & 0x0000FFFF));
	thetal = getLeptonTheta(dataw);
	phil   = getLeptonPhi(dataw);
	eflag  = getLeptonFlag(dataw); 
	moml   = getLeptonMom(dataw);  
	if (fDebugMode){ 
	cout << "dataword: " << dataw  << "  theta lep: " << thetal << endl;
        printf(" MU-leptons: thetal:%f phil:%f 
                                 eflag:%f moml:%f \n", 
			       (thetal/255.)*90,(phil/1535.)*360,eflag,moml); 
	}
     
        Float_t angle =  (thetal/255.)*90; 
	Float_t phi =  (phil/1535.)*360; 
	Float_t mom =  moml; 
        
	if ( angle < 15 && fLog == 2 ) {
         fprintf(fMU_logfile ," number_of_lepton found :%i \n", 
         muData->getNLeptons());
	 fprintf(fMU_logfile ," ##################### lepton definition => 
                  theta_l = %f , phil:%f moml:%f \n ", angle, phi ,mom );  
	 fprintf(fMU_logfile ," ########################################## \n");
         fprintf(fMU_logfile,"pData:%x pDataL:%x indice:%i dw_1-2:0x%x-0x%x=0x%x\n ",
                 *pData, *pDataL, nl, pDataL[nl],pDataL[nl+1],dataw); 
         fprintf(fMU_logfile ," ########################################## \n");
	 pSubEvt->dumpIt(fMU_logfile );  
	}  	

	muLept=(HMULeptons *)fCatLeptons->getNewSlot(fLoc);
	if (muLept) {
	  muLept=new (muLept) HMULeptons;
	  muLept->setTheta( (thetal/255.)*90 );
	  muLept->setPhi( (phil/1535.)*360 );
	  muLept->setEFlag(eflag);
	  muLept->setMomentum(moml);
	}
	
      }// ! leptons
      
   pDataD = pDataL+ (muData->getNLeptons()*2);
   //printf(" ndileptons %i - %i  \n ", *pDataD , muData->getNLeptons());
   muData->setNDiLeptons( *pDataD );
   Int_t nn1=0;
   Int_t nn2=0; 
   dataw = ((pDataD[0]<<16) | (pDataD[1] & 0x0000FFFF));
      // decode dileptons numbers
        
   if (fDebugMode)  printf("MU-Data decoding %i Dileptons \n", 
                            muData->getNDiLeptons());  
   
    for (Int_t nl=0; nl<(Int_t)muData->getNDiLeptons(); nl+=2){
	  dataw = ((pDataD[nl]<<16) | (pDataD[nl+1] & 0x0000FFFF));
	  nn1= dataw & 0xFF; 
	  nn2= (dataw >> 8 ) & 0xFF;    
	  dataw = ((pDataD[nl+2]<<16) | (pDataD[nl+3] & 0x0000FFFF));  
	  muDiLept=(HMUDiLeptons *) fCatDiLeptons->getNewSlot(fLoc);
	  if (muDiLept){
	    muDiLept=new (muDiLept) HMUDiLeptons;
            //here settings 
            muDiLept->setN1( nn1 ); 
            muDiLept->setN2 ( nn2 );
            muDiLept->setMass( (Float_t) (dataw));   
	}
      } // ! diletptons 
 
  return r;
}

Int_t HMatchUUnpacker::decodeRich(UInt_t *pData,Int_t maxLength) {
  Int_t length= pData[0]*2;
  Int_t r= length+1,ipusize; 
  UInt_t* ptr= pData+1; 

  if ( length == 6 ) richc++;    
  if (fDebugMode) printf(" decodeRich length:%i pdata0:%x  
                              pData1:%x  maxLength:%i\n", length
                          , pData[0],pData[1], maxLength);

  if ( length > maxLength) {
                        Error("decodeRich","size inconsistency");
                        r=-1;
       
  }

  int i=0; 
  ipusize=0;
  while ( length > 0 && r>0  ) {
    ipusize=decodeIpuRich(ptr,length);
    
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
  if (length == 0 ) return 0;
  Int_t r=length,i=0,bc;
  UInt_t fifo,colPat,segId,row;
  HMatchURich *muRich=0;
  
  if (fDebugMode)  printf(" decodeIpuRich length:%i \n", length);
  if (length>maxLength) {
    Error("decodeRich","SubEvent size too big");
    if (fDebugMode)  printf(" decodeIpuRich pData[1]:%i 
                              length:%i maxL:%i \n",pData[1], 
                              length, maxLength);
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
        if(segId ==7) segId=1;
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
	      muRich->setRow(row-23);
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
  if (length == 0 ) return 0;
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
     ptr+=ipusize;
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
    HMatchUTof *mtof=0;
    Int_t headerSize=4;
    UInt_t dataWord=0;
    Int_t i=0,cursor=0;
    tofheader->setTriggers(getTofTriggerTag(tofregister),
                           getTofTriggerCode(tofregister));
    tofheader->setID(getTofId(tofregister));  
    tofheader->setVeto(getTofVeto(tofregister));  

    for (cursor=headerSize, i=0; i < (length - 2)/2; cursor+=2, i++) {
      mtof = (HMatchUTof*)fCatTof->getNewSlot(fLoc);
      if (mtof) {
	mtof = new (mtof) HMatchUTof;
	dataWord = (pDat[cursor] << 16) | (pDat[cursor+1] & 0xFFFFFFFF);
	fillTofData(mtof,dataWord);
      } else Warning("decodeTof","No memory available for TOF subsubevent");
    } 
  }
  return r;
}

Bool_t HMatchUUnpacker::fillTofData( HMatchUTof* mtof, UInt_t dataword){
  ULong_t theta = dataword & 0xff00;
  theta = theta >> 8; 
  Int_t sec = (dataword & 0xf);  
  ULong_t dtheta = (dataword >>15 ) & 0x1;
  ULong_t phi = (dataword >> 16) & 0xff;
  Int_t dphi = (dataword >> 31) & 0x1; 
  Int_t time = (dataword >> 24) & 0xff; 
  Int_t PID = (dataword >> 4) & 0x3;  
  Float_t atheta =  theta/256. * 90.; 
  Float_t aphi =  phi/256. * 60.;  

  if (2 == 1){
    // conversion to RAdians (we don't want that)
                  atheta =  DEG_TO_RAD(atheta);
		  aphi   =    DEG_TO_RAD(aphi);
  } 
  mtof->setTheta(atheta, dtheta); 
  mtof->setPhi(aphi,dphi);
  mtof->setTime(time);
  mtof->setPID(PID);
  mtof->setSec(sec);

  return kTRUE;
} 


Int_t HMatchUUnpacker::storeShowElements(Int_t buildId, Int_t rowPattern, 
                                                        Int_t col){
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
  
  // while ( bc < 16 ) {
  //   if( (tempPat & 0x1) != 0 ){
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
  //  }
  //  }
  //  tempPat>>=1; bc++; 
  //     }! while
  return 0;
}


HShowerSubIdPar*  HMatchUUnpacker::showerParamConfigure(
				 HShowerUnpackParam* param, Int_t subEvtId){
  HShowerSubIdPar *pSubIdPar;
  pSubIdPar = param->getSubIdPar(subEvtId);
  if (pSubIdPar) 
    return   pSubIdPar;
  else{
    if (fDebugMode) printf("showerParamConfigure:
                            not corrected initialization of 
                            MUShowerUnpacker id = %d\n",subEvtId );
    return NULL;
  } 
}

Int_t HMatchUUnpacker::decodeShower(UInt_t *pData,Int_t maxLength) {
  // 32-bit exclusive
  Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF))*2; 
  Int_t r=length+2;
  Int_t ipusize=0; 
  UInt_t* ptr= pData+2;
  
  if (fDebugMode)  printf(" decodeShower length:%i \n", length);
  if ( length > maxLength) {
                        Error("decodeShower","size inconsistency");
                        r=-1;
  } 

  while ( length > 0 && r>0 ) {
    ipusize=decodeIpuShower(ptr,length);
    if(ipusize==-1) {
      r=-1;
      return r;
    }else{
      length-=ipusize; 
      ptr+=ipusize;
    }
  }
  return r;
}

Int_t  HMatchUUnpacker::decodeIpuShower(UInt_t *pData,Int_t maxLength) {
  Int_t col=0,rowPat=0;
  Int_t showBuildId=0;
  Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF)); 
  Int_t r=length; // 16-bit counted inclusive
  if (fDebugMode)  printf(" decodeIPuShower length:%i \n", length);
  if ( length > maxLength) {
    Error("decodeIpuShower","size inconsistency");
    r=-1; return r;
  } 

  Int_t showerheader = (pData[2] << 16) | (pData[3] & 0x0000FFFF);
  showBuildId = getShowBuildID( showerheader ); 
  
  length-=4;
  Int_t c=4,dataw=0; 
  Int_t ic=0;
  while ( length > 2 ) {
    dataw = (pData[c] << 16) | (pData[c+1] & 0x0000FFFF); 
    rowPat = getShowRowPattern ( dataw ); 
    col    = getShowColumn( dataw );    
    
    storeShowElements(showBuildId,rowPat,col); 
    ic++;
    c+=2;
    length-=2;
  }
  dataw = (pData[c] << 16) | (pData[c+1] & 0x0000FFFF); 
  
  if (length == 2 ){
    Int_t framec = getShowFrameCount( dataw );
    //  printf(" dataword %x \n", dataw); 
    if ( framec != ( r >> 1 ) - 2 ){ 
      Error("-I- decodeIpuShower","trailer check failed ");     
    }     
  } //decode trailers 
  
  return r;
}


Int_t HMatchUUnpacker::decodeShowerOld(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Shower subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  //Each 16bit word goes to a 32bit word.

  //   Int_t length=pData[0]; 
  //   Int_t r=length;
  //   UInt_t *pDataInit= pData;
  //   UInt_t *ptr= pData; 
  //   Int_t col,rowPat;
  //   Int_t showBuildId=0;
  
  //   if (fDebugMode) printf(" decodeShower  lenght : %x %i maxL:%i \n",
  //                            length, length,maxLength);
  //   if ( (length > maxLength) && pData[1] != 0x1111) {
  //     Warning("decodeShower","Inconsistency detected");
  //     r=-1;
  //   } else {
  
  //     for (;ptr < pDataInit+length;ptr++ ){   
  //       // test for the # trailers (debug mode) 
  
  //        switch ( *ptr ){
  //         case 0x2: 
  // 	  if (fDebugMode) {printf(" status flag %x \n", *ptr);   
  //                            printf(" status %x \n", *(++ptr));
  //           }else{
  //           }   
  //              break;
  //         case 0x3: 
  // 	 if (fDebugMode){ printf(" buildID flag %x \n", *ptr);
  //                           printf(" decode buildID  %i \n",
  //                                  getShowBuildID(++ptr ));
  //          }else{
  // 	   showBuildId = getShowBuildId(++ptr);   
  //          }
  //              break;
  //         case 0x4: 
  // 	 if (fDebugMode) {printf(" Row flag %x \n", *ptr);    
  // 	                  printf(" Row PAttern %x \n",*(++ptr));
  // 	 }else{
  //            rowPat=getShowRowPattern(++ptr); 
  //          }
  //              break;
  //         case 0x5: 
  //          if (fDebugMode) {printf(" Col flag %x \n", *ptr);   
  //                           printf(" Col %x \n", *(++ptr));
  // 	 }else{
  //            col = getShowColumn(++ptr);
  //            storeShowElements(showBuildId,rowPat,col);
  // 	 }    
  //          break;
	
	    
  // 	   //shows frame trailer 
  //         case 0x6: 
  //                  if (fDebugMode){ 
  //                           printf(" f counter flag %x \n", *ptr);   
  //                           printf(" f counter %x \n", *(++ptr));
  //                  } 
  //              break;
  //         case 0x7: 
  //                   if (fDebugMode){
  // 	                  printf(" f marker flag %x \n", *ptr);   
  // 	                  printf(" f marker %x \n", *(++ptr));
  // 		  }
  //              break;
  // 	}
  
  //      }//!switch
  //     }
  return 0;
}


Int_t HMatchUUnpacker::execute(void) {
  if (pSubEvt) { 
    Int_t length=pSubEvt->getDataLen(); 
    if (fDebugMode) printf(" subevt found  with lenght: %i \n",length);  
    UInt_t *pData=pSubEvt->getData();
    Int_t subSize;
    if ( (subSize=decodeMUData(pData,length)) == -1) {
      if (fDebugMode) printf("MU-Unpack: can not decode MU block\n");  
      return kTRUE;
    }
    length-=subSize;
    pData+=subSize;  
    if ( (subSize=decodeRich(pData,length)) == -1) {
    if (fDebugMode) printf("MU-Unpack: can not decode RICH block\n");  
    if ( fLog == 1 )  pSubEvt->dumpIt(fMU_logfile);
    return kTRUE;
    }
    length-=subSize;
     pData+=subSize;  
     // skip old event format for shower
     if ( *(pData+1) == 0x1111) {
       if (fDebugMode)  printf(" old event format of shower, skip it.\n"); 
       subSize = 6;
     }
     else {
       if (fDebugMode)  printf(" subsize:%i \n",subSize); 
       if ( (subSize=decodeShower(pData,length)) == -1) {
	 if (fDebugMode) printf("MU-Unpack: can not decode SHOWER block\n");  
	 return kTRUE;
       }
     }
    length-=subSize;
    pData+=subSize;
    if ( (subSize=decodeTof(pData,length)) == -1){
      if (fDebugMode) printf("MU-Unpack: can not decode TOF block\n");   
     return kTRUE;
    }
    
  }   //! psubEvt
  return kTRUE;
}


 



ClassImp(HMatchUUnpacker)
