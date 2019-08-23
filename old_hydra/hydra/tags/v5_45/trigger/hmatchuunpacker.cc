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

   if (!fCatScalers) {
	fCatScalers = new HLinearCategory("HMUScalers");
	event->addCategory(catMatchUScalers,fCatScalers,"Trigger");
	//        event->getPartialEvent(catMatchULeptons)->
	//        setSubHeader(new HMatchULeptonsHeader ); 
      }
   
     iterLepton = fCatLeptons->MakeIterator();
     iterRich   = fCatRich->MakeIterator();


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

      fMatchUParam = new HMatchUUnpackParam();


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
  UInt_t i, scaler[20], sc[20], number_of_scalers;


  if (muData->getTriggerCodeFirstFormat() == 0 ){
    length = pData[0]; // in 16 bit  word 
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
   // if (muData->getNLeptons() == 0 ) return r;
   if ( muData->getNLeptons() > 100 ) {
     printf(" nleptons out of bound. %i \n", muData->getNLeptons());
     return (-1); 
  } 

   pDataL=pData+muDataSize+1;


   Int_t dataw=0;
   Int_t sector,RichNr,MetaNr,DetBit,eflag; 
   Float_t moml;
 
   sector = 0;
   RichNr = 0;
   MetaNr = 0;
   DetBit = 0;
   eflag  = 0;
   moml   = 0.0;



  if (fDebugMode)  printf("MU-Data decoding %i leptons \n", 
                                           muData->getNLeptons()); 
   

      for (Int_t nl=0; nl<2*(Int_t)muData->getNLeptons(); nl+=2){
	muDataSize+=2;

	dataw = ((pDataL[nl]<<16) | (pDataL[nl+1] & 0x0000FFFF));
	// dataw = 0x4f300003;
	sector = getLeptonSector(dataw);
	RichNr = getLeptonRichNr(dataw);
	MetaNr = getLeptonMetaNr(dataw);
	DetBit = getLeptonDetBit(dataw);
	eflag  = getLeptonFlag(dataw); 
	moml   = getLeptonMom(dataw); 


	
if (fDebugMode){ 
  dataw = 0x4f301013;
  cout << sector  <<"   "<< RichNr << "   " << MetaNr << "   " << DetBit 
       << eflag << "   " << moml << endl; 
  cout << "dataword: " << dataw  << endl;
}

//  	if (fDebugMode){ 
//  	cout << "dataword: " << dataw  << "  theta lep: " << thetal << endl;
//          printf(" MU-leptons: thetal:%f phil:%f 
//                                   eflag:%f moml:%f \n", 
//  			       (thetal/255.)*90,(phil/1535.)*360,eflag,moml); 
//  	}
     
//          Float_t angle =  (thetal/255.)*90; 
//  	Float_t phi =  (phil/1535.)*360; 
//  	Float_t mom =  moml; 
        
//  	if ( angle < 15 && fLog == 2 ) {
//           fprintf(fMU_logfile ," number_of_lepton found :%i \n", 
//           muData->getNLeptons());
//  	 fprintf(fMU_logfile ," ##################### lepton definition => 
//                    theta_l = %f , phil:%f moml:%f \n ", angle, phi ,mom );  
//  	 fprintf(fMU_logfile ," ########################################## \n");
//           fprintf(fMU_logfile,"pData:%x pDataL:%x indice:%i dw_1-2:0x%x-0x%x=0x%x\n ",
//                   *pData, *pDataL, nl, pDataL[nl],pDataL[nl+1],dataw); 
//           fprintf(fMU_logfile ," ########################################## \n");
//  	 pSubEvt->dumpIt(fMU_logfile );  
//  	}  	

  	muLept=(HMULeptons *)fCatLeptons->getNewSlot(fLoc);
  	if (muLept) {
  	  muLept=new (muLept) HMULeptons;

  	  muLept->setSector(sector);
	  muLept->setRichNr(RichNr);
	  muLept->setMetaNr(MetaNr);
	  muLept->setDetBit(DetBit);
  	  muLept->setEFlag(eflag);
  	  muLept->setMomentum(moml);
  	}

      }// ! leptons
     
      if (length==2){ // total length of MU=2 means that there is not even a rich 
	muData->setNDiLeptons( 0 );
      }
      else{
	pDataD = pDataL+ (muData->getNLeptons()*2);
	muData->setNDiLeptons( *pDataD );
      }
	Int_t nn1=0;
	Int_t nn2=0; 
      
      // decode dileptons numbers
        
   if (fDebugMode)  printf("MU-Data decoding %i Dileptons \n", 
                            muData->getNDiLeptons());  
 
   
    for (Int_t nl=1; nl<(1+(3*(Int_t)muData->getNDiLeptons())); nl+=3){
      muDataSize += 3;
      dataw = pDataD[nl] & 0x0000FFFF;
      nn1= dataw & 0xFF; 
      nn2= (dataw >> 8 ) & 0xFF; 
      dataw = ((pDataD[nl+1]<<16) | (pDataD[nl+2] & 0x0000FFFF));  
      muDiLept=(HMUDiLeptons *) fCatDiLeptons->getNewSlot(fLoc);
      if (muDiLept){
	muDiLept=new (muDiLept) HMUDiLeptons;
	//here settings 
	muDiLept->setN1( nn1 ); 
	muDiLept->setN2 ( nn2 );
	muDiLept->setMass( (Float_t) (dataw));    
      }
    } // ! dileptons 
 

    
   // -------------- scalers hack -------------------------------

   if(muDataSize < length) {
     number_of_scalers=0;
     ptr= pData + 2 + muDataSize;
     for(i=0; i<=16; i++) {
       if((muDataSize+(i*2))<length){
	 sc[i] = (*(ptr+2*i)<<16) | (*(ptr+2*i+1));      
	 number_of_scalers++;
       }
     } 
     
     muScalers=(HMUScalers *)fCatScalers->getNewSlot(fLoc);

     for( Int_t i = 0; i< 16; i++){
       scaler[i]=0;
     }

     for( Int_t i = 0; i< number_of_scalers; i++){
       scaler[i]=getScaler(sc[i]);
     }

     muScalers->setScaler0(scaler[0]);    
     muScalers->setScaler1(scaler[1]);
     muScalers->setScaler2(scaler[2]);
     muScalers->setScaler3(scaler[3]);
     muScalers->setScaler4(scaler[4]);
     muScalers->setScaler5(scaler[5]);
     muScalers->setScaler6(scaler[6]);
     muScalers->setScaler7(scaler[7]);
     muScalers->setScaler8(scaler[8]);
     muScalers->setScaler9(scaler[9]);
     muScalers->setScaler10(scaler[10]);
     muScalers->setScaler11(scaler[11]);
     muScalers->setScaler12(scaler[12]);
     muScalers->setScaler13(scaler[13]);
     muScalers->setScaler14(scaler[14]);
     muScalers->setScaler15(scaler[15]);

   }
   //


   //------------------------------------------------------------

    

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
  UInt_t fifo,colPat,hardsegId, segId,row;
  HMatchURich *muRich=0;
  Int_t Rich_Sector_Map[6]={0,5,1,3,2,4};

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
	hardsegId=getRichSegId(pData[i+1]);

	//        if(segId ==7) segId=1;
	segId = Rich_Sector_Map[hardsegId-1];
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
  if (fDebugMode) printf(" ***********begin MU-Tof %x %x %x %x \n", pData[0], pData[1],pData[2],pData[3]); 
 
  if (fDebugMode) printf(" decodeTof length:%i \n", length);

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
 if (fDebugMode)  printf(" values of lenght data word %x %x %i \n",pDat[0],pDat[1] ,length);  
 
  Int_t r=length+2;
  Int_t tofregister = (pDat[2] << 16) | (pDat[3] & 0x0000FFFF);
  HMatchUTofHeader *tofheader = (HMatchUTofHeader*)( 
    event->getPartialEvent(catMatchUTof)->getSubHeader());

  if (fDebugMode) printf(" decodeIpuTof length:%i \n", length); 

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
  Int_t bc=0;
  HMatchUShower *muShower=0;
  Int_t tempPat=rowPattern;
  Int_t nSector,nRowOffset,nSwap;    
  bc=nSector=nRowOffset=nSwap=0;
  HShowerSubIdPar *pSubIdPar=0;
  
  if (fShowerParam) 
    pSubIdPar= showerParamConfigure(fShowerParam, buildId);
  else
    Error("storeShowElements","no parameter container found");
  
  
  if (fDebugMode)  printf(" %x  row pattern \n", tempPat);
  
  while ( bc < 16 ) {
    if( (tempPat & 0x1) != 0 ){
      muShower=(HMatchUShower *)fCatShower->getNewSlot(fLoc);
      if (muShower) {
	muShower=new(muShower) HMatchUShower;
	muShower->setBuildID(buildId);
	muShower->setColumn(col);
	muShower->setRowPattern(rowPattern);
	//muShower->setRow(bc);  
	if(pSubIdPar){
	  nSector = pSubIdPar->getSector();
	  nRowOffset = 16 * pSubIdPar->getPartOfSector();
	  nSwap = (pSubIdPar->getSwap()) ? -1 : 1;

	  if (nSwap == -1) nRowOffset += 8;
	  muShower->setSector(nSector); 
	  muShower->setRowOffset(nRowOffset);
	}
	muShower->setRow(bc+nRowOffset);    
      }
   
  
      if (fDebugMode) {
	printf(" -I-storeShowerElements: shower object found  ** \n"); 
	printf(" -I-storeShowerElements: decoding ...  buildID:%i  column:%i   row:%i  bitpos:%i  rowoffset:%i \n", 
	       buildId, col,bc,bc,nRowOffset);
	if(pSubEvt)  pSubEvt->dumpIt(fMU_logfile);
      }
    }
    
    tempPat>>=1; bc++; 
  }
  return 0;
}


HShowerSubIdPar*  HMatchUUnpacker::showerParamConfigure(
				 HShowerUnpackParam* param, Int_t subEvtId){
  HShowerSubIdPar *pSubIdPar;
  if (fDebugMode)  cout << subEvtId << endl;
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
  if (fDebugMode) cout << " MU shower length " << length << endl;
  Int_t r=length+2;
  Int_t ipusize=0; 
  UInt_t* ptr= pData+2;
    if (fDebugMode) printf(" ********begin MU-Shower %x %x %x %x \n", pData[0], pData[1],pData[2],pData[3]); 
  
  if (fDebugMode)  printf(" decodeShower length:%i \n", length);
  if ( length > maxLength) {
                        Error("decodeShower","size inconsistency");
                        r=-1;
  } 

  while ( length > 0 && r>0 ) {
    

    ipusize=decodeIpuShower(ptr,length);
    //if (fDebugMode)  printf(" <MU Shower> ptr inc val:%x ipusize:%i \n",ptr,ipusize);
  
    if(ipusize==-1) {
      r=-1;
      return r; 
    }else{
      length-=ipusize; 
      ptr+=ipusize;
    }
  if (fDebugMode)  cout << " SHOWER length " << length << "  ipusize  " << ipusize << endl;
  }
  return r;
}

Int_t  HMatchUUnpacker::decodeIpuShower(UInt_t *pData,Int_t maxLength) {
  Int_t col=0,rowPat=0;
  Int_t showBuildId=0;
  // Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF)); 
 Int_t length =  (pData[1] & 0x0000FFFF); 
if (fShowerDebugMode)  printf(" values of lenght data word %x %x %i \n",pData[0],pData[1] ,length);  
 
  Int_t r=length; // 16-bit counted inclusive
  if (fDebugMode)  printf(" decodeIPuShower length:%i \n", length);
  if ( length > maxLength) {
    Error("decodeIpuShower","size inconsistency");
    r=-1; return r;
  } 

  Int_t showerheader = (pData[2] << 16) | (pData[3] & 0x0000FFFF);
  showBuildId = getShowBuildID( showerheader ); 
  
 if (fDebugMode)  cout << " showBuildId" << showBuildId << endl; 
  length-=4;
  Int_t c=4,dataw=0; 
  Int_t ic=0;
  while ( length > 2 ) {
    dataw = (pData[c] << 16) | (pData[c+1] & 0x0000FFFF); 
    rowPat = getShowRowPattern( dataw ); 
    col    = getShowColumn( dataw );    
    if (fDebugMode)   
      cout << "shower row col dataword  " << dataw << "  row pat "  << rowPat <<  "  col  " << col << endl;
    
    storeShowElements(showBuildId,rowPat,col); 
    ic++;
    c+=2;
    length-=2;
  }
  dataw = (pData[c] << 16) | (pData[c+1] & 0x0000FFFF); 
  
  if (length == 2 ){
    Int_t framec = getShowFrameCount( dataw );
    //   printf(" dataword %x \n", dataw); 
    if ( framec != ( r >> 1 ) - 2 ){ 
     if(fDebugMode) Error("-I- decodeIpuShower","trailer check failed ");     
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

Int_t HMatchUUnpacker::mapAngles(void) {

  HMULeptons *pMULept=0;
  HMatchURich *pMURich=0;
  Int_t col, row;
  Float_t theta, phi;
  Int_t sec;
  Int_t hitpersec[6];
  Int_t offset[6];

  iterRich->Reset();

 for (sec=0;sec<6;sec++)
   hitpersec[sec]=0;

  while (( pMURich = (HMatchURich *)iterRich->Next()) != 0) {
    col = pMURich->getColumn();
    row = pMURich->getRow();
    sec = pMURich->getSegmentId();
    theta = fMatchUParam->getTheta(row, col);
    phi = 30+fMatchUParam->getPhi(row, col);

    pMURich->setTheta(theta);
    pMURich->setPhi(phi); 
    hitpersec[sec]++;
  }

  offset[0]=0;
  if (fDebugMode)  cout << "Sec: 0   hits: " << hitpersec[0] << "  offset: " << offset[0] << endl; 
  for (sec=1;sec<6; sec++) {
    offset[sec]= offset[sec-1] + hitpersec[sec-1];
    if (fDebugMode)    cout << "Sec: " << sec << "  hits: " << hitpersec[sec] << "  offset: " << offset[sec] << endl; 
  }
  iterLepton->Reset();
  
  while (( pMULept = (HMULeptons *)iterLepton->Next()) != 0) {
    sec= pMULept->getSector();
    pMURich = (HMatchURich*) fCatRich->getObject(pMULept->getRichNr()+offset[sec]);
    if (fDebugMode)     cout << "getRich Nr " << pMULept->getRichNr()+offset[sec] << endl;
    pMULept->setTheta(pMURich->getTheta());
    pMULept->setPhi(pMURich->getPhi());
  }


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


    length-=subSize;

    mapAngles();


  }   //! psubEvt
  return kTRUE;
}


 



ClassImp(HMatchUUnpacker)