using namespace std;
//*-- Author : Pavel Tlusty	 
//*-- Last Update : Stefano Spataro 2-nov-2004
//
#include "htofhitf.h"
#include "hades.h"
#include "htofraw.h"
#include "htofhit.h"
#include "htofcalpar.h"
#include "htofinoraw.h"
#include "htofinocal.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "tofdef.h"
#include "hstartdef.h"
#include "tofinodef.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hstartcal.h"
#include "hstartraw.h"
#include "hstarthit.h"
#include "hstartdef.h"
#include "htboxchan.h"
#include "TMath.h"
#include "htofoffliner.h"
#include "TH1.h"
#include "TH2.h"

HTofOfflineR::HTofOfflineR(void){
  fHitTofCat=NULL;
  fRawTofCat=NULL;
  fCalTofinoCat=NULL;
  fRawTofinoCat=NULL;
  fRawStartCat=NULL;
  fCalStartCat=NULL;
  fHitStartCat=NULL;
  iter_htof = 0;
  iter_tof = 0;
  iter_ctofino=0;
  iter_tofino=0;
  iter_start = 0;
  iter_cstart=0;
  iter_hstart=0;

}

HTofOfflineR::HTofOfflineR(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fHitTofCat=NULL;
  fRawTofCat=NULL;
  fCalTofinoCat=NULL;
  fRawTofinoCat=NULL;
  fRawStartCat=NULL;
  fCalStartCat=NULL;
  fHitStartCat=NULL;
  iter_htof = 0;
  iter_tof = 0;
  iter_ctofino=0;
  iter_tofino=0;
  iter_start = 0;
  iter_cstart=0;
  iter_hstart=0;

  counter_execute=0;
  empty=0;
  full=0;
}

HTofOfflineR::~HTofOfflineR(void) {
  if(iter_htof) delete iter_htof;
  if(iter_tof)  delete  iter_tof;
  if(iter_ctofino) delete  iter_ctofino;
  if(iter_tofino) delete  iter_tofino;
  if(iter_start)  delete  iter_start;
  if(iter_cstart) delete iter_cstart;
  if(iter_hstart) delete iter_hstart;

  if(ofile)   writeHist(ofile);
  printf("\nfull empty hemisphere %d %d \n",full,empty);
  printf("\nNumber of scaler events %d \n",counter_execute);

  if (fNameAsciiTofOfflineR) delete fNameAsciiTofOfflineR;
  if (fNameRootTofOfflineR)  delete fNameRootTofOfflineR;
}

Bool_t HTofOfflineR::init(void) {

  fTBoxCat=gHades->getCurrentEvent()->getCategory(catTBoxChan);
  if (!fTBoxCat) {
    fTBoxCat=gHades->getSetup()->getDetector("TBox")->buildCategory(catTBoxChan);
    if (!fTBoxCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catTBoxChan,fTBoxCat,"TBox");
  }
  iter_tbox=(HIterator*)fTBoxCat->MakeIterator();

  fRawTofCat=gHades->getCurrentEvent()->getCategory(catTofRaw);
  if (!fRawTofCat) {
    fRawTofCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofRaw);
    if (!fRawTofCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofRaw,fRawTofCat,"Tof");
  }
  iter_tof=(HIterator*)fRawTofCat->MakeIterator();

  fHitTofCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fHitTofCat) {
    fHitTofCat=gHades->getSetup()->getDetector("Tof")->buildCategory(catTofHit);
    if (!fHitTofCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofHit,fHitTofCat,"Tof");
  }
  iter_htof=(HIterator*)fHitTofCat->MakeIterator();

  fRawTofinoCat=gHades->getCurrentEvent()->getCategory(catTofinoRaw);
  if (!fRawTofinoCat) {
    fRawTofinoCat=gHades->getSetup()->getDetector("Tofino")->buildCategory(catTofinoRaw);
    if (!fRawTofinoCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofinoRaw,fRawTofinoCat,"Tofino");
  }
  iter_tofino=(HIterator*)fRawTofinoCat->MakeIterator();

  fCalTofinoCat=gHades->getCurrentEvent()->getCategory(catTofinoCal);
  if (!fCalTofinoCat) {
    fCalTofinoCat=gHades->getSetup()->getDetector("Tofino")->buildCategory(catTofinoCal);
    if (!fCalTofinoCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofinoCal,fCalTofinoCat,"Tofino");
  }
  iter_ctofino=(HIterator*)fCalTofinoCat->MakeIterator();


  fRawStartCat=gHades->getCurrentEvent()->getCategory(catStartRaw);
  if (!fRawStartCat) {
    fRawStartCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartRaw);
    if (!fRawStartCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartRaw,fRawStartCat,"Start");
  }
  iter_start=(HIterator*)fRawStartCat->MakeIterator();

  fCalStartCat=gHades->getCurrentEvent()->getCategory(catStartCal);
  if (!fCalStartCat) {
    fCalStartCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartCal);
    if (!fCalStartCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catStartCal,fCalStartCat,"Start");
  }
  iter_cstart=(HIterator*)fCalStartCat->MakeIterator();

  fHitStartCat=gHades->getCurrentEvent()->getCategory(catStartHit);
  if (!fHitStartCat) Warning("init","Start Hit level not defining: setting start time to 0");

  //  if (!fHitStartCat) {
  //    fHitStartCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartHit);
  //    if (!fHitStartCat) return kFALSE;
  //    else gHades->getCurrentEvent()->addCategory(catStartHit,fHitStartCat,"Start");
  //  }

    if (fHitStartCat) iter_hstart=(HIterator*)fHitStartCat->MakeIterator();

  if(fNameRootTofOfflineR)  ofile = new TFile(fNameRootTofOfflineR,"RECREATE");
  if (ofile) createHist(ofile);
  return kTRUE;
}


Int_t HTofOfflineR::execute(void) {



  Int_t SeqNumber; SeqNumber=gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
  Int_t TrigBit=gHades->getCurrentEvent()->getHeader()->getTBit();
  TrigBit=Int_t(log(Float_t(TrigBit))/log(2.)+1);
  //printf("t1 %d t2 %d\n",gHades->getCurrentEvent()->getHeader()->getTBit(),TrigBit);
  Int_t DownFlag=gHades->getCurrentEvent()->getHeader()->getDownscalingFlag();
  Int_t EventId;
  EventId = gHades->getCurrentEvent()->getHeader()->getId();
  //  printf("SeqNumber %d TrigBit %d DownFlag %d EventId %d \n",SeqNumber,TrigBit,DownFlag,EventId);


  HTofHit* tofhit=NULL;
  HTBoxChan* tbox=NULL;
  HTofRaw* tofraw=NULL;
  HTofinoRaw* tofinoraw=NULL;
  HStartRaw* startraw=NULL;
  HStartCal* startcal=NULL;
  HStartHit* starthit=NULL;
  Int_t multStart = 0, multVeto = 0;
  Int_t multTof = 0,multTofL = 0,multTofR = 0, multTofTrigger = 0;
  Int_t multTofino = 0;
  Int_t multAnalog = 0;

  Int_t start_module, start_strip, cstart_module, cstart_strip= -1,hstart_strip;
  Float_t cstart_time = 0, hstart_time=0;
  // Float_t veto_time=-2000; // not used variable
  Bool_t hstart_flag=kFALSE;
  Int_t  startdet = -1, start_time;
  // Int_t vetodet = -1; // not used variable
  Float_t  tof_timeL, tof_timeR, htof_xpos, htof_tof, distTof, corrT;
  Float_t  tof_chargeL, tof_chargeR;
  Float_t  htof_theta,htof_phi,toflab_x,toflab_y,toflab_z,htof_ypos;
  Float_t  tof_leftA, tof_rightA, tof_leftA_posCorr, tof_rightA_posCorr, tof_A_posCorr, AttLen;
  Float_t  htof_Edep,htof_xposAdc;
  Int_t  htof_Adcflag;
  Int_t  tof_sector, tof_module, tof_cell, htof_sector, htof_module, htof_cell;
  Float_t time=0;
  Int_t  itof,itof2,htof_rod;

  Int_t  tofino_sector, tofino_cell;
  Float_t tofino_time,tofino_charge;
  Int_t itofino;

  //  Get tbox data and fill histograms.

  Int_t chan,scaler1=0,scaler2=0,scaler3=0,scaler4=0;
  Int_t mycount=0;
  // Int_t count_start=0; // not used variable
  iter_tbox->Reset();
  while ( (tbox=(HTBoxChan *)iter_tbox->Next())!=NULL) {
   tbox->get(chan,scaler1,scaler2,scaler3,scaler4);

   //   printf("TBox data: %d %d %d %d %d\n",chan,scaler1,scaler2,scaler3,scaler4);

   //fill scaler content over the time 
   if(counter_execute<2001){
   histInput[chan]->Fill(counter_execute,scaler1);
   histReduce[chan]->Fill(counter_execute,scaler2);
   histStartScaler[chan]->Fill(counter_execute,scaler3);
   histVetoScaler[chan]->Fill(counter_execute,scaler4);
   }
   histInputScalerPattern->Fill(chan+1,scaler1);
   histReduceScalerPattern->Fill(chan+1,scaler2);
   histStartScalerPattern->Fill(chan+1,scaler3);
   histVetoScalerPattern->Fill(chan+1,scaler4);

   mycount++;   
  }           //end of iter

  if(mycount!=0) {
    counter_execute++;
    //printf("counter %d \n",counter_execute);
  }


  //  if(TrigBit!=1) return 1;


  //  Get start raw data and fill histograms.

  //multStart=0;
  multVeto=0;
  iter_start->Reset();
  while ( (startraw=(HStartRaw *)iter_start->Next())!=NULL) {
      start_time=startraw->getTime();
      start_module=startraw->getModule();
      start_strip=startraw->getStrip();
      if(start_module == 0){
	multStart++;
        histStartT[start_strip]->Fill(start_time);
        histPatternStart->Fill(start_strip+1);
	}
      if(start_module == 1){
        multVeto++;
        histVetoT[start_strip]->Fill(start_time);
        histPatternVeto->Fill(start_strip+1);
      }
      if(start_module == 5){
        start_time=startraw->getTime();
        histHaloT[start_strip]->Fill(start_time);
        histPatternHalo->Fill(start_strip+1);
	//printf("start_strip %d start_time %d %d %d %d %d\n",start_strip,start_time,startraw->getTime(),startraw->getTimeL(),startraw->getTimeR(),startraw->getTimeM()); 
      }
      //}
  }
  histMultStart->Fill(multStart);
  histMultVeto->Fill(multVeto);
  //if (multVeto==1)histVetoT[start_strip]->Fill(veto_time);
  //if (multVeto==1)vetodet = start_strip;


  //  Get start calibrated data and fill histograms.

  multStart=0;
  //multVeto=0;
  iter_cstart->Reset();
  while ( (startcal=(HStartCal *)iter_cstart->Next())!=NULL) {
    cstart_module=startcal->getModule();
    if(cstart_module==0){
       cstart_strip=startcal->getStrip();
       cstart_time=startcal->getTime();
       //cstart_time=startcal->getTof();
       //histStartC[cstart_strip]->Fill(cstart_time);
       multStart++;
       startdet = cstart_strip;
    }
    if(cstart_module==1){
      //       multVeto++;
    }
  }
  if(multStart==1) histStartC[cstart_strip]->Fill(cstart_time);
  histMultStart->Fill(multStart);
  //histMultVeto->Fill(multVeto);

  //  Get start hit data.
  
  Int_t multStartHit=0;
  if (fHitStartCat) {
  iter_hstart->Reset();
  while ( (starthit=(HStartHit *)iter_hstart->Next())!=NULL) {
    multStartHit++;
    hstart_strip=starthit->getStrip();
    hstart_time=starthit->getTime();
    //printf("time = %f\n ", hstart_time);
    hstart_flag=starthit->getFlag();
    //if(hstart_flag){
     if(multStart==1)  histStartH[hstart_strip]->Fill(hstart_time);
     //}
  }
  }

  histStartStat->Fill(0.);
  if(multStart==1) histStartStat->Fill(1.);
  if(multStartHit>0)  histStartStat->Fill(2.);

  //Get tofino raw data and fill histograms.
  multTofino=0;
  iter_tofino->Reset();
  while ( (tofinoraw=(HTofinoRaw *)iter_tofino->Next())!=NULL) {
    tofino_sector=tofinoraw->getSector();
    tofino_cell=tofinoraw->getCell();
    tofino_time=tofinoraw->getTime();
    tofino_charge=tofinoraw->getCharge();
    itofino=(tofino_sector*4+tofino_cell+1);
    if((tofino_time >10) && (tofino_time < 3900)){
      histPatternTofino->Fill(itofino);
      multTofino++;
    }
    //if (multStart==1) {
      histTofinoTime[tofino_sector][tofino_cell]->Fill(tofino_time);
      histTofinoTDC->Fill(itofino,tofino_time);
      histTofinoADC->Fill(itofino,tofino_charge);
      if((tofino_time >10) && (tofino_time < 3900))histTofinoADCok->Fill(itofino,tofino_charge);
      //}
  }
  histMultTofino->Fill(multTofino);

  //  Get tof raw data and fill histograms.

  multTof=0;
  multTofL=0;
  multTofR=0;

  iter_tof->Reset();
  while ( (tofraw=(HTofRaw *)iter_tof->Next())!=NULL) {
    tof_timeL=tofraw->getLeftTime();
    tof_timeR=tofraw->getRightTime();
    tof_chargeL=tofraw->getLeftCharge();
    tof_chargeR=tofraw->getRightCharge();
    tof_sector=tofraw->getSector();
    tof_module=tofraw->getModule();
    tof_cell=tofraw->getCell();

    itof2= (tof_sector*64+tof_module*8+tof_cell+1);

    if((tof_timeL >10) && (tof_timeL < 3900) && (tof_timeR > 10)  && (tof_timeR < 3900)){
       histPatternTof->Fill(itof2);
       multTof++;
    }
    if((tof_timeL >10) && (tof_timeL < 3900)){
      histPatternTofl->Fill(itof2);
      histADCleftOK->Fill(itof2,tof_chargeL);
      multTofL++;
      multTofTrigger++;
    }
    if((tof_timeR > 10)  && (tof_timeR < 3900)){
      histPatternTofr->Fill(itof2);
      histADCrightOK->Fill(itof2,tof_chargeR);
      multTofR++;
      multTofTrigger++;
    }

    itof=itof2-1;
    //if(itof2==154) multAnalog=Int_t(tof_chargeR);

    //    if (multStart==1) {
      histLeftT[tof_sector][tof_module][tof_cell]->Fill(tof_timeL);
      histTDCleft->Fill(itof2,tof_timeL);
      histADCleft->Fill(itof2,tof_chargeL);
      histRightT[tof_sector][tof_module][tof_cell]->Fill(tof_timeR);
      histTDCright->Fill(itof2,tof_timeR);
      //if(itof2!=154) histADCright->Fill(itof2,tof_chargeR);
      histADCright->Fill(itof2,tof_chargeR);
      //    }
  }
  histMultTof->Fill(multTof);
  histMultTofL->Fill(multTofL);
  //histMultTofR->Fill(multTofR);
  histMultTofTofino->Fill(multTof+multTofino);
  histMultTofTofinoTrigger->Fill(multTofTrigger/2+multTofino);

  histMultvsTrigBit->Fill(TrigBit,multTof+multTofino);
  histMultvsDownFlag->Fill(DownFlag,multTof+multTofino);
  if(TrigBit==1||TrigBit==5)histMultvsDownFlagM4->Fill(DownFlag,multTof+multTofino);
  histMultAnalog->Fill(multAnalog);
  histMult2->Fill(multTof+multTofino,multAnalog);

  //  Get tof hit data and fill histograms.

  iter_htof->Reset();
  while ( (tofhit=(HTofHit *)iter_htof->Next())!=NULL) {
//    if(multStart==1) {
      htof_xpos = tofhit->getXpos();
      htof_tof = tofhit->getTof();
      htof_sector=tofhit->getSector();
      htof_module=tofhit->getModule();
      htof_cell=tofhit->getCell();
      tof_leftA=tofhit->getLeftAmp();
      tof_rightA=tofhit->getRightAmp();
      htof_Edep=tofhit->getEdep();
      htof_xposAdc=tofhit->getXposAdc();
      htof_Adcflag=tofhit->getAdcFlag();
      tofhit->getDistance(distTof);
      tofhit->getTheta(htof_theta);
      tofhit->getPhi(htof_phi);
      tofhit->getXYZLab(toflab_x,toflab_y,toflab_z);

      corrT=((distTof-2100)/300);
      time = (htof_tof - corrT);
      //            printf("cstart = %f \n", cstart_time);

      //      if(startdet>3) time=time+1;

      itof2= (htof_sector*64+htof_module*8+htof_cell+1);
      htof_rod=htof_module*8+htof_cell;
      if(htof_rod<32) htof_ypos=htof_rod*30+15;
      if(htof_rod>=32) htof_ypos=(htof_rod-32)*20+970;
      htof_ypos=800.-htof_ypos;
      AttLen=2400;
      if(htof_module>3) AttLen=2110;
      tof_leftA_posCorr=tof_leftA/(TMath::Exp(htof_xpos/AttLen));
      tof_rightA_posCorr=tof_rightA/(TMath::Exp(-htof_xpos/AttLen));

      histPOSsum->Fill(htof_xpos);
      histTDCsum->Fill(itof2,time);
      histTDCdiff->Fill(itof2,htof_xpos);
      if(htof_Adcflag==2)histADCsum->Fill(itof2,htof_Edep);
      if(htof_Adcflag==2)histADCpos->Fill(itof2,htof_xposAdc);
      if ((htof_Adcflag== -1) || (htof_Adcflag== 2)){
        histADCl_cal->Fill(itof2,tof_leftA_posCorr);
        if(time<9.0) histADCl_cal_mip->Fill(itof2,tof_leftA_posCorr);
      }
      if ((htof_Adcflag== 1) || (htof_Adcflag== 2)){
        histADCr_cal->Fill(itof2,tof_rightA_posCorr);
        if(time<9.0) histADCr_cal_mip->Fill(itof2,tof_rightA_posCorr);
      }
      if(htof_Adcflag==2){
        tof_A_posCorr = TMath::Sqrt(tof_leftA_posCorr*tof_rightA_posCorr);
        histADC_cal->Fill(itof2,tof_A_posCorr);
        if(time<9.0) histADC_cal_mip->Fill(itof2,tof_A_posCorr);
      }
      histDiff[htof_sector][htof_module][htof_cell]->Fill(htof_xpos);
      histSum[htof_sector][htof_module][htof_cell]->Fill(time);
      histEdepvstof[htof_sector][htof_module][htof_cell]->Fill(time,htof_Edep);
      histEdepvstofsum->Fill(time,htof_Edep);
      histTOFsum->Fill(time);
      histTOFsumnocor->Fill(htof_tof);
      histTOFsumS[startdet]->Fill(time);
      histTOFtheta->Fill(htof_theta);
      histTOFth[htof_sector]->Fill(htof_theta);
      histTOFphi->Fill(htof_phi);
      histTOFpos[htof_sector]->Fill(htof_xpos,htof_ypos);
      if(histTOFyvsx->GetEntries()<100000) histTOFyvsx->Fill(toflab_x,toflab_y);

//    } //startmul==1
  }  //end of tof iter


  if((multTof+multTofino)==2){  //for el. scattering

  //if((multTof+multTofino)==4){
    Int_t count=0,count2,micount=0;
    Int_t sector[20],sector2[20],tofid[20],tempsec;
    Float_t tof[20];
    iter_htof->Reset();
    while ( (tofhit=(HTofHit *)iter_htof->Next())!=NULL) {
      sector[count]=tofhit->getSector();
      sector2[count]=tofhit->getSector();
      tof[count]=tofhit->getTof();
      tofid[count]=tofhit->getSector()*64+tofhit->getModule()*8+tofhit->getCell();
      //printf("mycount %d %d\n",micount,tofid[count]);
      micount++;

      if(count>0) {
	//histtofdif->Fill(tof[count]-tof[count-1]);    
	histtofdif->Fill(tofid[count]-tofid[count-1]);    
      }

      count2=count;
      if(count!=0) {
       if((tofid[count]-tofid[count-1])==1)tofid[count-1]=tofid[count];
       else count2++;
       //printf("%d %d %d %d \n",count,count2,tofid[count-1],tofid[count]);   
      }
      else {
       count2++;
       //printf("%d %d %d \n",count,count2,tofid[count]);   
      }
      count=count2;      
    }
    //printf("final %d %d \n",micount,count);   

    //    if(count==2)histtofdif->Fill(tof[0]-tof[1]);    

    iter_tofino->Reset();
    while ( (tofinoraw=(HTofinoRaw *)iter_tofino->Next())!=NULL) {
     tofino_time=tofinoraw->getTime();
     if((tofino_time >10) && (tofino_time < 3900)){
      sector[count]=tofinoraw->getSector()+6;
      sector2[count]=tofinoraw->getSector();
      count++;
     } 
   }
  
    Int_t esec[6]={0,0,0,0,0,0};
    if(count==4){
      for(Int_t i=0;i<count;i++) esec[sector2[i]]++;
      //printf("%d %d %d %d %d %d\n",esec[0],esec[1],esec[2],esec[3],esec[4],esec[5]);
      if((esec[0]+esec[1]+esec[2])==0||
         (esec[1]+esec[2]+esec[3])==0||
         (esec[2]+esec[3]+esec[4])==0||
         (esec[3]+esec[4]+esec[5])==0||
         (esec[4]+esec[5]+esec[0])==0||
         (esec[5]+esec[0]+esec[1])==0)  empty++;
      else  full++;
    }

    histMultTofR->Fill(count);
    histtrigger->Fill(sector[0],sector[1]);

    if(sector2[1]<sector2[0]){
      tempsec=sector2[1];
      sector2[1]=sector2[0];
      sector2[0]=tempsec;
    }
    histtrigger2->Fill(sector2[0],sector2[1]);
  }    
  return 1;
}

void HTofOfflineR::setOutputAscii(Char_t *c){
    if (fNameAsciiTofOfflineR) delete fNameAsciiTofOfflineR;
    fNameAsciiTofOfflineR = new Char_t[strlen(c)+1];
    strcpy(fNameAsciiTofOfflineR, c);
}

void HTofOfflineR::setOutputRoot(Char_t *c){
    if (fNameRootTofOfflineR) delete fNameRootTofOfflineR;
    fNameRootTofOfflineR = new Char_t[strlen(c)+1];
    strcpy(fNameRootTofOfflineR, c);
}

void HTofOfflineR::createHist(TFile* ofile){
  ofile->cd();
  Text_t buffer[255];
  Int_t sec=0,mod=0,det=0;

  for (sec=0;sec<6;sec++)  {
    for (mod=0;mod<8;mod++) {
      for(det=0;det<8;det++) {
        sprintf(buffer,"TOF_%i_%i_%i_%s",sec+1,mod+1,det+1,"left");
        histLeftT[sec][mod][det]=new TH1F(buffer,"TOF raw data (left)",4096, 0, 4096);
        sprintf(buffer,"TOF_%i_%i_%i_%s",sec+1,mod+1,det+1,"right");
        histRightT[sec][mod][det]=new TH1F(buffer,"TOF raw data (right)",4096, 0, 4096);
        sprintf(buffer,"TOF_%i_%i_%i_%s",sec+1,mod+1,det+1,"diff");
	//        histDiff[sec][mod][det]=new TH1F(buffer,"TOF calib data (diff)", 500,-5000,5000);
	histDiff[sec][mod][det]=new TH1F(buffer,"TOF calib data (diff)", 500,-5000,5000);
        sprintf(buffer,"TOF_%i_%i_%i_%s",sec+1,mod+1,det+1,"sum");
        histSum[sec][mod][det]=new TH1F(buffer,"TOF calib data (sum)",600,-10,50);
        sprintf(buffer,"TOF_%i_%i_%i_%s",sec+1,mod+1,det+1,"Edepvstof");
        histEdepvstof[sec][mod][det]=new TH2F(buffer,"TOF Edep vs tof",500,0,50,300,0.,15.);
      }
    }
  }

  for(det=0;det<16;det++) {
    sprintf(buffer,"START_%i_%s",det+1,"time");
    histStartT[det]=new TH1F(buffer,"START raw data",4096, 0, 4096);
    sprintf(buffer,"START_%i_%s",det+1,"cal");
    histStartC[det]=new TH1F(buffer,"START cal data",1000, -31.25, 31.25);
    sprintf(buffer,"START_%i_%s",det+1,"hit");
    histStartH[det]=new TH1F(buffer,"START hit data",1000, -31.25, 31.25);
    sprintf(buffer,"VETO_%i_%s",det+1,"time");
    histVetoT[det]=new TH1F(buffer,"VETO raw data",4096, 0, 4096);
    sprintf(buffer,"HALO_%i_%s",det+1,"time");
    histHaloT[det]=new TH1F(buffer,"HALO raw data",4096, 0, 4096);
    sprintf(buffer,"TOFsumS_%i",det+1);
    histTOFsumS[det] = new TH1F(buffer,"TOFsumS",1500,0,150);
  }

  for(det=0;det<8;det++) {
    sprintf(buffer,"Scaler_Input_%i",det+1);
    histInput[det] = new TH1F(buffer,"Input",2000,0,2000);
    sprintf(buffer,"Scaler_Reduce_%i",det+1);
    histReduce[det] = new TH1F(buffer,"Reduce",2000,0,2000);
    sprintf(buffer,"Scaler_Start_%i",det+1);
    histStartScaler[det] = new TH1F(buffer,"Start",2000,0,2000);
    sprintf(buffer,"Scaler_Veto_%i",det+1);
    histVetoScaler[det] = new TH1F(buffer,"Veto",2000,0,2000);
  }

  for(sec=0;sec<6;sec++) {
    for(det=0;det<4;det++){
       sprintf(buffer,"TOFINO_%i_%i_%s",sec+1,det+1,"time");
       histTofinoTime[sec][det]=new TH1F(buffer,"TOFINO raw data",4096,0,4096);
    }
  }

    for(sec=0;sec<6;sec++) {
      sprintf(buffer,"TOFpos_%i",sec+1);
      histTOFpos[sec]=new TH2F(buffer,"tof pos y vs x",300,-1500,1500,200,-1000,1000);
      sprintf(buffer,"TOFtheta_%i",sec+1);
      histTOFth[sec]=new TH1F(buffer,"tof theta",100,0,100);
    }

  histStartScalerPattern = new TH1F("PatternStartScaler","PatternStart",8,0.5,8.5);
  histVetoScalerPattern = new TH1F("PatternVetoScaler","PatternVeto",8,0.5,8.5);
  histInputScalerPattern = new TH1F("PatternInputScaler","Triggerbox In",8,0.5,8.5);
  histReduceScalerPattern = new TH1F("PatternReduceScaler","Triggerbox Out",8,0.5,8.5);

  histMultvsDownFlag = new TH2F("MultvsDownFlag","MultvsDownFlag",2,0,2,65,-0.5,64.5);
  histMultvsDownFlagM4 = new TH2F("MultvsDownFlagM4","MultvsDownFlagM4",2,0,2,65,-0.5,64.5);
  histMultvsTrigBit = new TH2F("MultvsTrigBit","MultvsTrigBit",8,1,9,65,-0.5,64.5);

  histMultStart = new TH1F("MultStart","Start multiplicity",9,-0.5,8.5);
  histMultVeto = new TH1F("MultVeto","Veto multiplicity",9,-0.5,8.5);
  histPatternStart = new TH1F("PatternStart","PatternStart",16,0.5,16.5);
  histPatternVeto = new TH1F("PatternVeto","PatternVeto",16,0.5,16.5);
  histPatternHalo = new TH1F("PatternHalo","PatternVeto",16,0.5,16.5);
  histStartStat = new TH1F("StartStat","Start Statistics",9,-0.5,8.5);

  histMultTofL = new TH1F("MultTofL","Tof multiplicity left",65,-0.5,64.5);
  histMultTofR = new TH1F("MultTofR","Tof multiplicity right",65,-0.5,64.5);
  histMultTof = new TH1F("MultTof","Tof multiplicity",65,-0.5,64.5);
  histMultTofTofino = new TH1F("MultTofTofino","MultTofTofino",89,-0.5,88.5);
  histMultTofTofinoTrigger = new TH1F("MultTofTofinoTrigger","MultTofTofinoTrigger",89,-0.5,88.5);
  histMultAnalog = new TH1F("MultTofAnalog","MultTofAnalog",1024,0,4096);
  histMult2 = new TH2F("Mult2","Mult2",89,-0.5,88.5,1024,0,4096);
  histPatternTof = new TH1F("PatternTof","PatternTof",384,0.5,384.5);
  histPatternTofl = new TH1F("PatternTofl","PatternTofl",384,0.5,384.5);
  histPatternTofr = new TH1F("PatternTofr","PatternTofr",384,0.5,384.5);

  histTDCleft = new TH2F("TDCleft","TDCleft",384,0.5,384.5,1024,0,4096);
  histTDCright = new TH2F("TDCright","TDCright",384,0.5,384.5,1024,0,4096);
  histADCleft = new TH2F("ADCleft","ADCleft",384,0.5,384.5,1024,0,4096);
  histADCright = new TH2F("ADCright","ADCright",384,0.5,384.5,1024,0,4096);
  histADCleftOK = new TH2F("ADCleftOK","ADCleftOK",384,0.5,384.5,1024,0,4096);
  histADCrightOK = new TH2F("ADCrightOK","ADCrightOK",384,0.5,384.5,1024,0,4096);
  histADCl_cal = new TH2F("ADCl_cal","ADCl_cal",384,0.5,384.5,300,0,15);
  histADCl_cal_mip = new TH2F("ADCl_cal_mip","ADCl_cal_mip",384,0.5,384.5,300,0,15);
  histADCr_cal = new TH2F("ADCr_cal","ADCr_cal",384,0.5,384.5,300,0,15);
  histADCr_cal_mip = new TH2F("ADCr_cal_mip","ADCr_cal_mip",384,0.5,384.5,300,0,15);
  histADC_cal = new TH2F("ADC_cal","ADC_cal",384,0.5,384.5,300,0,15);
  histADC_cal_mip = new TH2F("ADC_cal_mip","ADC_cal_mip",384,0.5,384.5,300,0,15);
  histTDCsum = new TH2F("TDCsum","TDCsum",384,0.5,384.5,750,-50,100);
  histTDCdiff = new TH2F("TDCdiff","TDCdiff",384,0.5,384.5,500,-5000,5000);
  histADCsum = new TH2F("ADCsum","ADCsum",384,0.5,384.5,300,0,15);
  histADCpos = new TH2F("ADCpos","ADCpos",384,0.5,384.5,500,-5000,5000);
  histTOFsum = new TH1F("TOFsum","TOFsum",1500,0,150);
  histTOFsumnocor = new TH1F("TOFsumnocor","TOFsumnocor",1500,0,150);
  histPOSsum = new TH1F("POSsum","POSsum",500,-5000,5000);
  histEdepvstofsum=new TH2F("Edepvstofsum","TOF Edep vs tof",500,0,50,300,0.,15.);
  histTOFtheta = new TH1F("TOFtheta","tof theta",100,0,100);
  histTOFphi = new TH1F("TOFphi","tof phi",360,0,360);
  histTOFyvsx= new TH2F("TOFyvsx","TOF y vs x",600,-3000,3000,600,-3000,3000);

  histMultTofino = new TH1F("MultTofino","MultTofino", 25, -0.5, 24.5);
  histPatternTofino = new TH1F("PatternTofino","PatternTofino", 24, 0.5, 24.5);
  histTofinoTDC = new TH2F("TofinoTDC","TofinoTDC",24,0.5,24.5,1024,0,4096);
  histTofinoADC = new TH2F("TofinoADC","TofinoADC",24,0.5,24.5,1024,0,4096);
  histTofinoADCok = new TH2F("TofinoADCok","TofinoADCok",24,0.5,24.5,1024,0,4096);

  histtrigger = new TH2F("trigger","trigger",12,-0.5,11.5,12,-0.5,11.5);
  histtrigger2 = new TH2F("trigger2","trigger2",6,-0.5,5.5,6,-0.5,5.5);
  histtofdif = new TH1F("tofdif","tofdif",400,-100,100);

}

void HTofOfflineR::writeHist(TFile* ofile){
  // Write histograms to a rootfile.
  ofile->cd();

  for (Int_t sec=0;sec<6;sec++)  {
    for (Int_t mod=0;mod<8;mod++) {
      for(Int_t det=0;det<8;det++) {
        histLeftT[sec][mod][det]->Write();
        histRightT[sec][mod][det]->Write();
        histDiff[sec][mod][det]->Write();
        histSum[sec][mod][det]->Write();
        histEdepvstof[sec][mod][det]->Write();
      }
    }
  }

  for (Int_t sec=0;sec<6;sec++)  {
    for(Int_t det=0;det<4;det++) {
      histTofinoTime[sec][det]->Write();
    }
  }

  for(Int_t det=0;det<16;det++) {
    histStartT[det]->Write();
    histStartC[det]->Write();
    histStartH[det]->Write();
    histVetoT[det]->Write();
    histHaloT[det]->Write();
    histTOFsumS[det]->Write();
  }
  for(Int_t det=0;det<8;det++) {
    histInput[det]->Write();
    histReduce[det]->Write();
    histStartScaler[det]->Write();
    histVetoScaler[det]->Write();
  }

  for (Int_t sec=0;sec<6;sec++)  {
    histTOFpos[sec]->Write();
    histTOFth[sec]->Write();
      }

  histStartScalerPattern->Write();
  histVetoScalerPattern->Write();
  histInputScalerPattern->Write();
  histReduceScalerPattern->Write();

  histMultvsTrigBit->Write();
  histMultvsDownFlag->Write();
  histMultvsDownFlagM4->Write();

  histMultTofino->Write();
  histPatternTofino->Write();
  histTofinoTDC->Write();
  histTofinoADC->Write();
  histTofinoADCok->Write();
  histMultStart->Write();
  histMultVeto->Write();
  histPatternStart->Write();
  histPatternVeto->Write();
  histPatternHalo->Write();
  histStartStat->Write();  
  histMultTofL->Write();
  histMultTofR->Write();
  histMultTof->Write();
  histMultTofTofino->Write();
  histMultTofTofinoTrigger->Write();
  histMultAnalog->Write();
  histMult2->Write();
  histPatternTof->Write();
  histPatternTofl->Write();
  histPatternTofr->Write();
  histTDCleft->Write();
  histTDCright->Write();
  histADCleft->Write();
  histADCright->Write();
  histADCleftOK->Write();
  histADCrightOK->Write();
  histADCl_cal->Write();
  histADCl_cal_mip->Write();
  histADCr_cal->Write();
  histADCr_cal_mip->Write();
  histADC_cal->Write();
  histADC_cal_mip->Write();
  histTDCsum->Write();
  histTDCdiff->Write();
  histADCsum->Write();
  histADCpos->Write();
  histTOFsum->Write();
  histTOFsumnocor->Write();
  histPOSsum->Write();
  histEdepvstofsum->Write();
  histTOFtheta->Write();
  histTOFphi->Write();
  histTOFyvsx->Write();

  histtrigger->Write();
  histtrigger2->Write();
  histtofdif->Write();

  ofile->Save();
  ofile->Close();
  delete ofile;
}




ClassImp(HTofOfflineR)
