#include <stdlib.h>
#include "hmatchuunpacker.h"
#include "hcategory.h"
#include "hades.h"
#include "hrecevent.h"
#include "heventheader.h"
#include "hmatchurich.h"
#include "hmatchushower.h"
#include "triggerinfodef.h"
#include "hlinearcategory.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hmatchutofheader.h"
#include "hruntimedb.h"
#include "hpartialevent.h"
#include "hshowergeometry.h"
#include "hshowerpad.h"
#include "hgeomvector.h"
#include "hgeomvector2.h"
#include "hgeomvolume.h"
#include "htriggerparshowermap.h"
#include "htriggerparrichmap.h"

#define DEG_TO_RAD(x) (x*TMath::Pi()/180)

const Int_t HMatchUUnpacker::kSubEvtId = 512; //id: 0x00000200

HMatchUUnpacker::HMatchUUnpacker(Int_t unites, Bool_t compression, Int_t flog) {
  fCatScalers= NULL;
  fCatRich=NULL;
  fCatShower=NULL;
  fCatTof=NULL;
  fCatLeptons=NULL;
  fCatDiLeptons=NULL;       

  fUnit=unites;
  fLog = flog;
  fMU_logfile=NULL;
  richc = 0; 
  data=NULL;
  event=NULL;
  iterLepton = NULL;
  isCompressed = compression;
  muData = NULL;
  muScalers = NULL;

  fShowerParam=NULL;
  fTrigParSho=NULL; 
  fTrigParRich=NULL; 
  
  fDebugMode=kFALSE;  
  fRichDebugMode=kFALSE;
  fShowerDebugMode=kFALSE;
  
  sec=0;
  sec_old=0;
  
  for (Int_t z=0; z<6; z++) {
    hitpersec[z]=0;
    nShower[z]=0;
    nTof[z]=0;
    for (Int_t y =0; y<100;y++) {
      mapNrTof[z][y]=0;
      mapNrShower[z][y]=0;
    }
  }

   evt=0;
   tofNr=tofLep=0;
   showerNr=showerLep=0;

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

    if (iterLepton) delete iterLepton;
    iterLepton = NULL;

    if(muData) delete muData; 
    muData = NULL;

    if (data) delete[] data;
    data=NULL;
    event=NULL;

}




Bool_t HMatchUUnpacker::init(void) {

  if (gHades) {
    event= (HRecEvent*) gHades->getCurrentEvent();       

    if (event) {
      if(event->getPartialEvent(catMatchU)) muData =
            (HMUData*)(event->getPartialEvent(catMatchU)->getSubHeader()); 

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
        fCatTof->setCatHeader(new HMatchUTofHeader); 
      }
   
      fCatLeptons = event->getCategory(catMatchULeptons);
    
      if (!fCatLeptons) {
	fCatLeptons = new HLinearCategory("HMULeptons");
	event->addCategory(catMatchULeptons,fCatLeptons,"Trigger");
      }
      if (!fCatDiLeptons) {
	fCatDiLeptons = new HLinearCategory("HMUDiLeptons");
	event->addCategory(catMatchUDiLeptons,fCatDiLeptons,"Trigger");
      }

      if (!fCatScalers) {
	fCatScalers = new HLinearCategory("HMUScalers");
	event->addCategory(catMatchUScalers,fCatScalers,"Trigger");
      }
 
      if (!muData) {
        muData = new HMUData();
        event->getPartialEvent(catMatchU)->setSubHeader(muData);
      }

      iterLepton = fCatLeptons->MakeIterator();


      HRuntimeDb* rtdb=gHades->getRuntimeDb();

      if(rtdb){  


	//-------------------------------------

	fTrigParSho=(HTriggerParShowerMap *)rtdb->getContainer("TriggerParShowerMap");
	if (fTrigParSho) {
	  cout << "Trigger Parameters Shower Mapping initialized" << endl;
	} else {
	  cout << "******  Trigger Parameter Shower Mapping Initialization failed!! *********" << endl;
	}

	//-------------------------------------
 	//-------------------------------------

 	fTrigParRich=(HTriggerParRichMap *)rtdb->getContainer("TriggerParRichMap");
 	if (fTrigParRich) {
 	  cout << "Trigger Parameters Rich Mapping initialized" << endl;
 	} else {
 	  cout << "******  Trigger Parameter Rich Mapping Initialization failed!! *********" << endl;
 	}

	//-------------------------------------
        fShowerParam = (HShowerUnpackParam*)rtdb->getContainer("ShowerUnpackParam");
        if (!fShowerParam) {
	  Warning("init","ShowerUnpackParam container not found in rtdb "); 
          Warning("init","call default initialisation");
	  fShowerParam = new HShowerUnpackParam;
	  fShowerParam->defaultInit();
        }
      } else {
	Error("init","no runtimeDb found !");
        return kFALSE; //!rtdb
      }

	//-------------------------------------

    } else return kFALSE; //! event
  } else return kFALSE; //! gHades

  return kTRUE;
}


Int_t  HMatchUUnpacker::decodeMUData(UInt_t *pData,Int_t maxLength) {

  UInt_t *pDataL=NULL,*pDataD=NULL;
  Int_t length=0;
  Int_t r=0;
  UInt_t *ptr =NULL;
  UInt_t scaler[14], sc[14]; 
  Int_t number_of_scalers=0;
  triggerCode=-1;

  for (Int_t z =0; z<14; z++) {
    scaler[z]=0;
    sc[z]=0;
  }


  if (muData->getTriggerCodeFirstFormat() == 0 ){
    length = pData[0]; // in 16 bit  word 
    muData->setMULength(length);
    r= length+1;
    ptr = pData+1;
  }
  else {
    triggerCode = *pData & 0xf;
    length= pData[1] ; // in 16 bit  word
    muData->setMULength(length); 
    ptr= pData +2; 
    r= length+2;
  }

  
  muData->clear();  
  muData->setTriggerCode(triggerCode);

  if (fDebugMode)  
    printf ("MU-Data length :%i %i \n", triggerCode,length ); 
  
  if (length == 1 ) return r; 
  
  HMULeptons *muLept=NULL;
  HMUDiLeptons *muDiLept=NULL;  
  
  
  if ( length > maxLength) {
    Error("decodeMUData","size inconsistency");
    r=-1; return r; 
  } 
  
  
  Int_t muDataSize = 0 ;

  Int_t scaler_flag =0;
  Int_t scalers_length = 20;

  if ( (gHades->isCalibration()) && (length >2)){  
    // for calibration events (in previous times triggerCode != 1) the length of MU data would be 2
    // if it is > than 2, it means that there are scalers
    // in this case we make the decode of mudata with a reduced length (2)
    // the scaler length is added at the end where the scalers are decoded
    // note: scalers can be present only in calibration events!!!
    length -= scalers_length; 
    scaler_flag =1;
  }
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
   Float_t *pMass=NULL;
 
   sector = 0;
   RichNr = 0;
   MetaNr = 0;
   DetBit = 0;
   eflag  = 0;
   moml   = 0.0;



   if (fDebugMode)  
   printf("MU-Data decoding %i leptons \n", muData->getNLeptons()); 
   

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
      
      if ( (length==2) || (length ==3) ){ // total length of MU=2 means that there is not even a rich 
	muData->setNDiLeptons( 0 );
      }
      else{
	pDataD = pDataL+ (muData->getNLeptons()*2);
	muData->setNDiLeptons( *pDataD );
      }
	Int_t nn1=0;
	Int_t nn2=0; 
      
      // decode dileptons numbers
        
	if (fDebugMode)  
	printf("MU-Data decoding %i Dileptons \n", muData->getNDiLeptons());  
 
   
    for (Int_t nl=1; nl<(1+(3*(Int_t)muData->getNDiLeptons())); nl+=3){
      muDataSize += 3;
      dataw = pDataD[nl] & 0x0000FFFF;
      nn1= dataw & 0xFF; 
      nn2= (dataw >> 8 ) & 0xFF; 
      dataw = ((pDataD[nl+1]<<16) | (pDataD[nl+2] & 0x0000FFFF));  
      muDiLept=(HMUDiLeptons *) fCatDiLeptons->getNewSlot(fLoc);
      if (muDiLept){
	muDiLept=new (muDiLept) HMUDiLeptons;
	muDiLept->setN1( nn1 ); 
	muDiLept->setN2 ( nn2 );

	pMass = (Float_t *) &dataw;
	muDiLept->setMass(*pMass);

	if (fDebugMode)  cout << "inv mass " << muDiLept->getMass() << "    Dataw " << *pMass << endl;
      }
    } // ! dileptons 
 
    // -------------- scalers hack -------------------------------
    if (scaler_flag ==1) length += scalers_length; // here the scaler_length is added again and the scalers are decoded

    if(muDataSize < length) { // there is an additional length reserved for scalers and marked by the scaler_flag
      number_of_scalers=0;
      ptr= pData + 2 + muDataSize;

      //the first 5 scalers are composed of 2 datawords 
      for(int i=0; i<10; i+=2) {
	if((muDataSize+i)<length){
	  sc[number_of_scalers] = (*(ptr+i)<<16) | (*(ptr+i+1));
	  number_of_scalers++;
	}
      }

      // the next 8 scalers are single datawords
      for (int i=10;i<18;i++){
	if((muDataSize+i)<length){
	  sc[number_of_scalers]=*(ptr+i);
	  number_of_scalers++;
	}
      }

      // the last scaler is again composed of 2 datawords
      for(int i=18; i<20; i+=2) {
	if((muDataSize+i)<length){
	  sc[number_of_scalers] = (*(ptr+i)<<16) | (*(ptr+i+1));      
	  number_of_scalers++;
	}
      }

      
      muScalers=(HMUScalers *)fCatScalers->getNewSlot(fLoc);
     
      if (muScalers) {
	muScalers =new (muScalers) HMUScalers();
	
	
	for( Int_t i = 0; i< 14; i++){
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
  
	Float_t *scaF=NULL;
	scaF = (Float_t *) &scaler[13];
	muScalers->setScaler13(*scaF);
      }
    } 

    return r;
}

Int_t HMatchUUnpacker::decodeRich(UInt_t *pData,Int_t maxLength) {
  Int_t length= pData[0]*2;
  Int_t r= length+1,ipusize; 
  UInt_t* ptr= pData+1; 

  UInt_t MU_sec_pattern_Rich =0;
  RichSecPatternMask = 0x0;

  sec_count = 0;

  if ( length == 6 ) richc++;    
  if (fDebugMode) printf(" decodeRich length:%i pdata0:%x \
                              pData1:%x  maxLength:%i\n", length
			 , pData[0],pData[1], maxLength);

  if ( length > maxLength) {
    Error("decodeRich","size inconsistency");
    r=-1;       
  }


  ipusize=0;
  while ( length > 0 && r>0  ) {
    ipusize=decodeIpuRich(ptr,length);
    sec_count++;
   
    if(ipusize==-1) r=-1;
    else{
      length-=ipusize; 
      ptr+=ipusize;
    }
    
  }

  MU_sec_pattern_Rich = muData->getSecHitPatternRich();


  if(  ( muData->getMULength() > 5) && (MU_sec_pattern_Rich != RichSecPatternMask)  &&  (gHades->isReal())
       && ( (muData->getNHitsShower() + muData->getNHitsTof()) > 1 ) ) {
    cout << " ERROR: Rich Hit Pattern Wrong!!! " << MU_sec_pattern_Rich << "  " << RichSecPatternMask 
 	 << " TRIGGER CODE: " << triggerCode << endl;
  }
  
  
  return r;
}


Int_t HMatchUUnpacker::decodeIpuRich(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Rich subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  // trigger code (pData[0]) is skipped so

  HMatchURich *muRich=NULL;
  Int_t length=getRichSize(pData[1])*2;  //One 32bit word goes to 2.
  UInt_t sec_header = getRichSecHeader(pData[0]);
  if (length == 0 ) return 0;
  Int_t r=length,i=0,bc;
  UInt_t fifo,colPat,hardsegId, segId,row,col, segId_old, row_old, col_old;

  // --- here are the rich IPU configuration - hardcoded ---
  //UInt_t Rich_Sector_Map[6]={0,5,1,3,2,4}; // before 309oct02
  //UInt_t Rich_Sector_Map[6]={1,5,0,3,2,4}; //311oct02 configuration
  UInt_t Rich_Sector_Map[7]={0,1,2,3,4,5,9}; // sep03 configuration

  Float_t theta, phi;
  Int_t FullCount, ReducedCount;

  segId_old = row_old = col_old = 0; //these are used for internal check
  FullCount = ReducedCount = 0;
  bc = fifo = colPat = hardsegId = segId = row = col =0;
  theta = phi = 0.;
  if (fDebugMode)  printf(" decodeIpuRich length:%i \n", length);

  if ( Rich_Sector_Map[sec_header-1]!=sec_count ) {
    cout << "HMUUnpack: Mismatch in RichIPU!!! Sector_Count " << sec_count << " != Sector_Header " 
	 << Rich_Sector_Map[sec_header-1] << " (mapped from " << sec_header << ")" << endl;
  }

  if (length>maxLength) {
    Error("decodeRich","SubEvent size too big");
    if (fDebugMode)  printf(" decodeIpuRich pData[1]:%i \
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
	if (colPat==0) {
	  cout << "HMUUnpack: Problem in RichIPU colpat=0 (not possible)!!! " << endl;
	}
	if ((hardsegId < 1) || (hardsegId >6) ) {
	  cout << "HMUUnpack: Mismatch in RichIPU!!! HardsegId = " << hardsegId << endl; 
	  hardsegId = 7;
	}
	segId = Rich_Sector_Map[hardsegId-1];
	
	row=getRichRow(pData[i+1]);
	
	bc=0;
	tempPat=colPat;
	if (fRichDebugMode) printf("-I-decodeIpuRich: colpattern: x%0x \n", tempPat);
	
	while ( bc < 8 ) {
	  //	  while (bc<8 && ((tempPat & 0x1) == 0)) {tempPat>>=1; bc++;}
	  if ( (tempPat & 0x1) != 0 )  { 
	    if ( bc < 8 ) {
	      col = 95 - fifo * 8 - bc;

	if (fRichDebugMode) printf("-I-decodeIpuRich: col-row: %i %i \n", col,row);
		      
 	phi   = fTrigParRich->getRichPhi(row-23,col);
 	theta = fTrigParRich->getRichTheta(row-23,col);  
	      
	if ( (theta<999)&&(phi<999) ) {
	  if (fRichDebugMode) printf(" -I-decodeIpuRich: decoding ...  row:%i  column:%i fifo:%i bitpos:%i \
segId:%i theta:%f phi:%f \n  ",   row-23, 95 - fifo * 8 - bc,fifo,bc,segId,theta,phi);
	  
	  
	  muRich=(HMatchURich *)fCatRich->getNewSlot(fLoc);
	  if (muRich) {
	    muRich=new (muRich) HMatchURich;
	    muRich->setFifo(fifo);
	    muRich->setColumnPattern(colPat);	      
	    muRich->setSegmentId(segId);
	    muRich->setRowNb(row);
	    muRich->setColumn(col);
	    muRich->setRow(row-23);
	    muRich->setY(col);
	    muRich->setX(row-23);

	    RichSecPatternMask |= (1<<segId);

	    if (segId<5) phi += 60*(segId+1);  	    
	    muRich->setTheta(theta);
	    muRich->setPhi(phi); 
	    
	    hitpersec[segId]++;

	    // if there is a mismatch in the RichIPU sector information, it is corrected and reset
	    // a warning message appeare
	    if ( ( segId != sec_count ) || ( segId != Rich_Sector_Map[sec_header-1] ) ) {
	      cout << "HMUUnpack: Mismatch in RichIPU!!! Sector_Count " << sec_count << " Sector_Header " 
		   << Rich_Sector_Map[sec_header-1] << " (mapped from " << sec_header    
		   << ")  SegId " << segId << "  Hardseg " << hardsegId << endl;
	      muRich->setSegmentId(sec_count);
	    }

	    // internal check on the hit ordering: X(row) has priority and must increase, Y(col) must decrease 
	    if (row < row_old) {
	      cout << "HMUUnpack: Mismatch in RichIPU!!! in sec " << sec_count << "  X " << row 
		   << "  after X_old " << row_old << endl; 
	    } else if (row == row_old) {
	      if ( col > col_old ) { 
		cout << "HMUUnpack: Mismatch in RichIPU!!! in sec " << sec_count << "  in X " << row << "  Y " << col 
		     << "  after Y_old " << col_old << endl; 
	      }
	    }

	    if (segId != segId_old) {
	      FullCount=ReducedCount=0;
	    } 

	    row_old = row;
	    col_old = col;
	    segId_old = segId;

	    if(theta>0.1) { // valid range of mapped rings
	      mapNrRich[segId][ReducedCount]=FullCount;
	      ReducedCount++;
	    } 
	    FullCount++;



	    if (fRichDebugMode) {
	      printf(" -I-decodeIpuRich: rich object found  ** \n"); 
	      printf(" -I-decodeIpuRich: decoding ...  row:%i  column:%i fifo:%i bitpos:%i \
segId:%i theta:%f phi:%f \n  ",   row-23, 95 - fifo * 8 - bc,fifo,bc,segId,theta,phi);
	      if(pSubEvt)  pSubEvt->dumpIt(fMU_logfile);
	    }
	    
	  }
	}
	else {
	  if (fRichDebugMode) printf(" -I-decodeIpuRich: !!!!!!! DISCARDED  row:%i  column:%i fifo:%i bitpos:%i \
segId:%i theta:%f phi:%f \n  ",   row-23, 95 - fifo * 8 - bc,fifo,bc,segId,theta,phi);     
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
  Int_t r= length+1,ipusize=0; 
  UInt_t* ptr= pData+1; 

  UInt_t MU_sec_pattern_Tof =0;
  TofSecPatternMask = 0x0;

  if (fDebugMode) printf(" ***********begin MU-Tof %x %x %x %x \n", pData[0], pData[1],pData[2],pData[3]); 
 
  if (fDebugMode) printf(" decodeTof length:%i \n", length);

  if ( length > maxLength) {
                        Error("decodeTof","size inconsistency");
                        r=-1;
  } 
  while ( length > 0 && r>0 ) {
   ipusize=decodeIpuTof(ptr,length);
   
   // cout << "ipu size: " << ipusize << endl;


   if(ipusize==-1) r=-1;
   else{
     length-=ipusize; 
     ptr+=ipusize;
    }
  }

  MU_sec_pattern_Tof = muData->getSecHitPatternTof();

  if ( (muData->getSecHitPatternRich()>0 )&&  ( MU_sec_pattern_Tof != TofSecPatternMask ) ) {
    cout << "*************************** event " << evt << "****************" << endl;
    cout << " ERROR: Tof Hit Pattern Wrong!!! " << MU_sec_pattern_Tof << "  " << TofSecPatternMask << endl;
  }

  return r;
}

Int_t HMatchUUnpacker::decodeIpuTof(UInt_t *pDat,Int_t maxLength) {
  // return the number of bytes in Tof subevent.
  Int_t length =  ((pDat[0]<<16) | (pDat[1] & 0x0000FFFF))*2; 
  if (fDebugMode)  printf(" values of lenght data word %x %x %i \n",pDat[0],pDat[1] ,length);  
 
  Int_t r=length+2;
  Int_t tofregister = (pDat[2] << 16) | (pDat[3] & 0x0000FFFF);
  HMatchUTofHeader *tofheader = (HMatchUTofHeader*)(fCatTof->getCatHeader());


  if (fDebugMode) printf(" decodeIpuTof length:%i \n", length); 

  if (length > maxLength) {
    Error("decodeIpuTof","size inconsistency");                  
    return -1;
  } 

  if (!tofheader) {
    Error(" decodeIpuTof","header missing"); 
    r=-1;
  } else {
    HMatchUTof *mtof=NULL;
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

Int_t HMatchUUnpacker::fillTofData( HMatchUTof* mtof, UInt_t dataword){

  ULong_t theta = dataword & 0xff00;
  theta = theta >> 8; 
  Int_t sec = (dataword & 0xf);  
  ULong_t dtheta = (dataword >>15 ) & 0x1;
  ULong_t phi = (dataword >> 16) & 0xff;
  Int_t dphi = (dataword >> 31) & 0x1; 
  Float_t time = ((float) ((dataword >> 24) & 0xff))/2.;
  if (fDebugMode) cout << " ingo time " <<  ((dataword >> 24) & 0xff) << " cal time " << time << endl;
  Int_t PID = (dataword >> 4) & 0x3;  
  Float_t atheta =  theta/255. * 90.; 
  Float_t aphi =  phi/255. * 60.;  

  if (sec < 5) {
    //phi = phi + (sec+1)*60;
    aphi = aphi + (sec+1)*60;
    dphi = dphi + (sec+1)*60;
  }

  mtof->setTheta(atheta, dtheta); 
  mtof->setPhi(aphi,dphi);
  mtof->setiTheta(theta); 
  mtof->setiPhi(phi);
  mtof->setTime(time);
  mtof->setPID(PID);
  mtof->setSec(sec);

  TofSecPatternMask |= (1<<sec);

  nTof[sec]++;

  if (sec != sec_old) {
    tofLep = 0;
  }
  if (PID ==1) {
    mapNrTof[sec][tofLep]=tofNr;
    tofLep++;
  }
  tofNr++; 
  sec_old = sec;

  return sec;
} 


Int_t HMatchUUnpacker::storeShowElements(Int_t buildId, Int_t rowPattern, 
                                                        Int_t col){
  Float_t Sh_phi=0.;
  Int_t bc=0;
  HMatchUShower *muShower= NULL;
  Int_t tempPat=rowPattern;
  Int_t sec,nRowOffset,nSwap;    
  bc=sec=nRowOffset=nSwap=0;
  HShowerSubIdPar *pSubIdPar=NULL;

  fLoc.set(0);
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

	if(pSubIdPar){
	  sec = pSubIdPar->getSector();
	  nRowOffset = 16 * pSubIdPar->getPartOfSector();
	  nSwap = (pSubIdPar->getSwap()) ? -1 : 1;

	  if (nSwap == -1) nRowOffset += 8;
	  muShower->setSector(sec); 
	  muShower->setRowOffset(nRowOffset);
	}
	muShower->setRow(bc+nRowOffset);    
	
	ShowerSecPatternMask |= (1<<sec);


	if (fShowerDebugMode) cout << " here starts the shower mapping" << endl;
	muShower->setTheta(fTrigParSho->getShowerTheta(col,bc+nRowOffset));


	Sh_phi = fTrigParSho->getShowerPhi(col,bc+nRowOffset);
	if (sec < 5)
	  Sh_phi += ((sec + 1) *60);
	muShower->setPhi(Sh_phi);
	//muShower->setPhi(fTrigParSho->getShowerPhi(bc+nRowOffset,col));
 	if (fShowerDebugMode) 
	  cout << "shower sec: " << sec << " row: " << bc+nRowOffset << "  col :" << col 
	       << " phi " << Sh_phi << endl;
 
		if (fShowerDebugMode) 
cout << "row " <<  bc+nRowOffset << "  col " << col << "  theta " << muShower->getTheta() << "  phi " << muShower->getPhi() << endl;
	
	if (fShowerDebugMode) cout << "end of the shower mapping" << endl;

	nShower[sec]++;
	if (fShowerDebugMode) cout << "sector " << sec << " nShower[" << sec << "]=" << nShower[sec] << endl;
	if (sec != sec_old) {
	  showerLep = 0;
	}
	mapNrShower[sec][showerLep]=showerNr;
	if (fShowerDebugMode) cout << "mapNrShower[ " << sec << "][" << showerLep << "]=" << showerNr << endl;
	showerLep++;
	showerNr++;
	sec_old = sec;


      }
    }
    
    tempPat>>=1; bc++; 
  }
  return 0;
}


HShowerSubIdPar*  HMatchUUnpacker::showerParamConfigure(
				 HShowerUnpackParam* param, Int_t subEvtId){
  HShowerSubIdPar *pSubIdPar=NULL;
  if (fDebugMode)  cout << subEvtId << endl;
  pSubIdPar = param->getSubIdPar(subEvtId);
  if (pSubIdPar) 
    return   pSubIdPar;
  else{
    if (fDebugMode) printf("showerParamConfigure: \
                            not corrected initialization of \
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
  UInt_t MU_sec_pattern_Shower =0;
  ShowerSecPatternMask = 0x0;


    if (fDebugMode) printf(" ********begin MU-Shower %x %x %x %x \n", pData[0], pData[1],pData[2],pData[3]); 
  
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
  if (fDebugMode)  cout << " SHOWER length " << length << "  ipusize  " << ipusize << endl;
  }

  MU_sec_pattern_Shower = muData->getSecHitPatternShower();

  if ( (muData->getSecHitPatternRich()>0 )&&  ( MU_sec_pattern_Shower != ShowerSecPatternMask ) ) {
    cout << "*************************** event " << evt << "****************" << endl;
    cout << " ERROR: Tof Hit Pattern Wrong!!! " << MU_sec_pattern_Shower << "  " << ShowerSecPatternMask << endl;
  }

  return r;
}

Int_t  HMatchUUnpacker::decodeIpuShower(UInt_t *pData,Int_t maxLength) {
  Int_t col=0,rowPat=0;
  Int_t showBuildId=0;
  // Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF)); 
 Int_t length =  (pData[1] & 0x0000FFFF); 
if (fDebugMode)  printf(" values of lenght data word %x %x %i \n",pData[0],pData[1] ,length);  
 
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
 
  if (length >0) { // there is at leat 1 hit or a trailer !!!
    while ( length > 2 ) { // it is necessarily a hit
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
    
    if(fDebugMode)  cout << "shower lenght now is: " << length << endl;
    if (length == 2 ){  // there is still a trailer! 
      if (pData[c] == 0x7000){
	c+=2;
	length-=2;
	Int_t framec = getShowFrameCount( dataw );
	//   printf(" dataword %x \n", dataw); 
	if ( framec != ( r >> 1 ) - 2 ){ 
	  if(fDebugMode) Error("-I- decodeIpuShower","trailer check failed ");     
	}   
      } else {
	rowPat = getShowRowPattern( dataw ); 
	col    = getShowColumn( dataw );    
	if (fDebugMode)   
	  cout << "shower row col dataword  " << dataw << "  row pat "  << rowPat <<  "  col  " << col << endl;
	
	storeShowElements(showBuildId,rowPat,col); 
	ic++;
	c+=2;
	length-=2;
      }
    } //decode trailers 
    
    else {
      if (fDebugMode)  cout << " ERROR Shower lenght " << length << "  (should be 2 ) " << endl;
    }
  } 
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

   HMULeptons    *pMULept=NULL;
   HMatchURich   *pMURich=NULL;
   HMatchUShower *pMUShower=NULL;
   HMatchUTof    *pMUTof=NULL;

  Int_t offset[6];
  offset[0]=0;
  if (fDebugMode)  cout << "Map Angle Rich Sec: 0  hits: " << hitpersec[0] << "  offset: " << offset[0] << endl; 
  for (sec=1;sec<6; sec++) {
    offset[sec]= offset[sec-1] + hitpersec[sec-1];
    if (fDebugMode)    cout << "Map Angle Rich Sec: " << sec << "  hits: " << hitpersec[sec] << "  offset: " << offset[sec] << endl; 
  }



  iterLepton->Reset();
 

  while (( pMULept = (HMULeptons *)iterLepton->Next()) != 0) {
    sec= pMULept->getSector();
      pMURich = (HMatchURich*) fCatRich->getObject(mapNrRich[sec][pMULept->getRichNr()]+offset[sec]);
      if (fDebugMode) cout << "Rich Nr " << pMULept->getRichNr()+offset[sec] << " Offset: " << offset[sec] << endl;
      if (fDebugMode) cout << "Rich theta " << pMURich->getTheta() << " Rich phi " << pMURich->getPhi() << endl;
      pMULept->setThetaRich(pMURich->getTheta());
      pMULept->setPhiRich(pMURich->getPhi());



      // mapping meta angles 
      if (fDebugMode) cout << "sec " << sec<< " meta nr " << pMULept->getMetaNr() << "  det bit " << pMULept->getDetBit() << " sh hits " << nShower[sec] << "  tof hits " << nTof[sec] << endl;     
      if ( ( ( pMULept->getDetBit()==1 ) && ( pMULept->getMetaNr() <= nShower[sec] ) ) ||
	   ( ( pMULept->getDetBit()==0 ) && ( pMULept->getMetaNr() <= nTof[sec]    ) ) ) {
	

	if (pMULept->getDetBit()==1) {
	  pMUShower = (HMatchUShower*) 
	    fCatShower->getObject(mapNrShower[sec][pMULept->getMetaNr()]);
	  if (fDebugMode) cout << "sho tot Nr " << mapNrShower[sec][pMULept->getMetaNr()]  << " detbit: " << pMULept->getDetBit() << endl;
	  	  
	  pMULept->setThetaMeta(pMUShower->getTheta());
	  pMULept->setPhiMeta(pMUShower->getPhi());
	  if (fDebugMode) cout << "MetaS theta " <<  pMUShower->getTheta() << " MetaS phi " << pMUShower->getPhi() << endl;
	}
	
	else if (pMULept->getDetBit()==0) {
	  pMUTof = (HMatchUTof*) 
	    fCatTof->getObject(mapNrTof[sec][pMULept->getMetaNr()]);

	  pMULept->setThetaMeta(pMUTof->getTheta());
	  pMULept->setPhiMeta(pMUTof->getPhi());
	  if (fDebugMode) cout << "MetaT theta " <<  pMUTof->getTheta() << " MetaT phi " << pMUTof->getPhi() << endl;
	}
	
      }
      else {
	cout << "Bug in Meta Number!!!!!!" << endl;
	continue;
      }
      
  }
  
  if (fDebugMode) cout << "end of the mapping" << endl;
  return 0;
}
//-----------------------------------------------------------------------------------------------------------

Int_t HMatchUUnpacker::execute(void) {
  evt++;
  if (fDebugMode) cout << "*************************** event " << evt << "****************" << endl;

  tofNr=tofLep=0;
  showerNr=showerLep=0;

  for (sec=0;sec<6;sec++){
    hitpersec[sec]=0;
    nShower[sec]=0;
    nTof[sec]=0;
    for (int i=0; i>100; i++){
      mapNrTof[sec][i]=0;
      mapNrShower[sec][i]=0;
      mapNrRich[sec][i]=0;
    }
  }



  if (pSubEvt) {
    Int_t length=pSubEvt->getDataLen(); 
    if (fDebugMode) printf(" subevt found  with lenght: %i \n",length);      

    UInt_t * pData=NULL;
    if( isCompressed )
      pData = expansion ( pSubEvt->getData(), length);     
    else  
      pData=pSubEvt->getData();

   

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


