#include "hmdcrichcorr.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hrichhit.h"
#include "hmatchurich.h"
#include "richdef.h"
#include "triggerinfodef.h"
#include "hmdctrackddef.h"
#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "hmdcclus.h"
#include "hmdctimecut.h"
#include "hevent.h"
#include "hmdcgetcontainers.h"
#include "hmdccalibrater1.h"
#include "TH1.h"

ClassImp(HMdcRichCorr)


HMdcRichCorr::HMdcRichCorr(void) : HReconstructor() {
  nRingsCut=1;
  setup();
}

HMdcRichCorr::HMdcRichCorr(Text_t *name, Text_t *title, Int_t nRCut) :
    HReconstructor(name,title) {
  nRingsCut=nRCut;
  setup();
}

void HMdcRichCorr::setup(void) {
  radToDeg=180./acos(-1.);
  richFlag=0;
  clFinderType=1; // chamber clusters

  fcut=0;
  fLookUpTb=0;
  level4=10;
  level5=50;
  for(Int_t sec=0; sec<6; sec++) {
    for(Int_t mod=0; mod<4; mod++) nLayers[sec][mod]=6;
  }
  isInitialised=kFALSE;

  fClusCat=0;
  fCalib1=0;
  for(Int_t s=0;s<6;s++) {
    dThCuts[s][0][0]=dThCuts[s][1][0]=-5.;
    dThCuts[s][0][1]=dThCuts[s][1][1]=5.;
    dPhCuts[s][0][0]=dPhCuts[s][1][0]=-3.;
    dPhCuts[s][0][1]=dPhCuts[s][1][1]=3.;
    hDTh[s][0]=hDTh[s][1]=0;
    hDPh[s][0]=hDPh[s][1]=0;
  }
  hDThAll[0]=hDThAll[1]=0;
  hDPhAll[0]=hDPhAll[1]=0;
  hNRings=hNMRings=hNEvents=0;
  
  setHistFile(".","fhs_","NEW");
}

HMdcRichCorr::~HMdcRichCorr() {
  HMdcLookUpTb::deleteCont();
  if(fCalib1) delete fCalib1;  
}

void HMdcRichCorr::makeCalibrater1(Int_t vers=1,Int_t cut=1,Int_t domerge=0) {
  fCalib1=new HMdcCalibrater1("MdcCali1","MdcCali1",vers,cut,domerge);
}

Bool_t HMdcRichCorr::init(void) {
  nEvents=0;
  nRingEvents=0;
  nMatchedEvents=0;
  nMatchedEvents1R=0;
  if(fCalib1 && !fCalib1->init()) return kFALSE;
  fCal1Cat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if(!fCal1Cat) return kFALSE;
  iterCal1=(HIterator *)fCal1Cat->MakeIterator("native");

  fClusCat=HMdcGetContainers::getCatMdcClus(kTRUE);
  if(!fClusCat) return kFALSE;
  iterMdcClus=(HIterator *)fClusCat->MakeIterator("native");

  if(richFlag) fRichHitCat = gHades->getCurrentEvent()->getCategory(catRichHit);
  else fRichHitCat = gHades->getCurrentEvent()->getCategory(catMatchURich);
  if(!fRichHitCat) return kFALSE;
  if(fRichHitCat) iterRichHit=(HIterator *)fRichHitCat->MakeIterator("native");

  fLookUpTb=HMdcLookUpTb::getObject();
  fLookUpTb->setPar(160,kFALSE); //!!!!!!!!!!!!319! ???
  return kTRUE;
}

Bool_t HMdcRichCorr::reinit(void) {
  if(fCalib1) if(!fCalib1->reinit()) return kFALSE;
  return reinitMdcFinder();
}

Bool_t HMdcRichCorr::reinitMdcFinder(void) {
  if(!fLookUpTb->initContainer()) return kFALSE;
  printStatus();
  if( !isInitialised ) {
    for(Int_t sec=0; sec<6; sec++) if(&((*fLookUpTb)[sec]))
      (*fLookUpTb)[sec].setTypeClFinder(clFinderType);
    for(Int_t sec=0; sec<6; sec++) {
      HMdcLookUpTbSec* fLookUpTbSec=&((*fLookUpTb)[sec]);
      if(!fLookUpTbSec) continue;
      for(Int_t mod=0; mod<2; mod++) {
        HMdcLookUpTbMod* fLookUpTbMod=&((*fLookUpTbSec)[mod]);
        if( !fLookUpTbMod ) nLayers[sec][mod]=0;
      }
    }
    isInitialised=kTRUE;
  }
  makeHist();
  return kTRUE;
}

void HMdcRichCorr::makeHist(void) {
  Char_t buf[100];
  if(hDPhAll[0]==0) {
    hDThAll[0]=new TH1F("dThetaM1","dTheta M1",100,-20.,20.);
    hDPhAll[0]=new TH1F("dPhiM1","dPhi M1",100,-20.,20.);
    hDThAll[1]=new TH1F("dThetaM2","dTheta M2",100,-20.,20.);
    hDPhAll[1]=new TH1F("dPhiM2","dPhi M2",100,-20.,20.);
  } else {
    hDThAll[0]->Reset();
    hDPhAll[0]->Reset();
    hDThAll[1]->Reset();
    hDPhAll[1]->Reset();
  }
  for(Int_t sec=0; sec<6; sec++) for(Int_t mod=0; mod<2; mod++) {
    if(hDTh[sec][mod]==0) {
      sprintf(buf,"dThetaS%iM%i",sec+1,mod+1);
      hDTh[sec][mod]=new TH1F(buf,"",100,-20.,20.);
      sprintf(buf,"dPhiS%iM%i",sec+1,mod+1);
      hDPh[sec][mod]=new TH1F(buf,"",100,-20.,20.);
    } else {
      hDTh[sec][mod]->Reset();
      hDPh[sec][mod]->Reset();
    }
    sprintf(buf,"dTheta: S%i M%i; cut %.1f<dTheta<%.1f",
        sec+1,mod+1,dThCuts[sec][mod][0],dThCuts[sec][mod][1]);
    hDTh[sec][mod]->SetTitle(buf);
    sprintf(buf,"dPhi: S%i M%i; cut %.1f<dPhi<%.1f",
        sec+1,mod+1,dPhCuts[sec][mod][0],dPhCuts[sec][mod][1]);
    hDPh[sec][mod]->SetTitle(buf);
  }
  if(hNRings==0) {
    hNRings=new TH1F("hRings","<Num. of rings>",250,0.,500000.);
    hNMRings=new TH1F("hMRings","<Num. of matched rings>",250,0.,500000.);
    hNEvents=new TH1F("nEvents","Num of events",250,0.,500000.);
    hNRings->SetBit(TH1::kCanRebin);
    hNMRings->SetBit(TH1::kCanRebin);
    hNEvents->SetBit(TH1::kCanRebin);
  } else {
    hNRings->Reset();
    hNMRings->Reset();
    hNEvents->Reset();
    hNRings->SetBins(500,0.,500000.);
    hNMRings->SetBins(500,0.,500000.);
    hNEvents->SetBins(500,0.,500000.);
  }
}

void HMdcRichCorr::setNLayers(Int_t *lst) {
  Int_t* nLay=nLayers[0];
  for(Int_t i=0;i<6*4;i++) nLay[i]=lst[i];
}

void HMdcRichCorr::setDThetaCuts(Float_t dTh1, Float_t dTh2) {
  // set the same cuts for all mdc modules, (dTh1<dTh2 !)
  for(Int_t s=0;s<6;s++) {
    dThCuts[s][0][0]=dThCuts[s][1][0]=dTh1;
    dThCuts[s][0][1]=dThCuts[s][1][1]=dTh2;
  }
}

void HMdcRichCorr::setDPhiCuts(Float_t dPh1, Float_t dPh2) {
  // set the same cuts for all mdc modules, (dPh1<dPh2 !)
  for(Int_t s=0;s<6;s++) {
    dPhCuts[s][0][0]=dPhCuts[s][1][0]=dPh1;
    dPhCuts[s][0][1]=dPhCuts[s][1][1]=dPh2;
  }
}

void HMdcRichCorr::setDThetaCuts(Float_t* dTh) {
  // dTheta cuts seting
  Float_t* dThAr=dThCuts[0][0];
  for(Int_t i=0;i<6*2*2;i++) dThAr[i]=dTh[i];
}

void HMdcRichCorr::setDPhiCuts(Float_t* dPh) {
  // dPhi cuts seting
  Float_t* dPhAr=dPhCuts[0][0];
  for(Int_t i=0;i<6*2*2;i++) dPhAr[i]=dPh[i];
}

Int_t HMdcRichCorr::execute(void) {
  if(nEvents==0) {
    HMdcGetContainers::getFileNameWInd(fileName);
    HMdcGetContainers::getRunId(runId);
    HMdcGetContainers::getEventDate(eventDate);
    HMdcGetContainers::getEventTime(eventTime);
  }
  nEvents++;
  hNEvents->Fill(nEvents-0.5);
  iterRichHit->Reset();
  for(Int_t s=0;s<6;s++) nRichHits[s]=0;
  if(richFlag) fillListByRichHit();
  else fillListByMathURich();
  Int_t nRings=nRichHits[0]+nRichHits[1]+nRichHits[2]+nRichHits[3]+
      nRichHits[4]+nRichHits[5];
  if(nRings==0) return kSkipEvent;
  hNRings->Fill(nEvents-0.5,(Float_t)nRings);
  
  nRingEvents++;

  if(fCalib1) fCalib1->execute();
  findMdcClusters();

  iterMdcClus->Reset();
  HMdcClus* pMdcClus;
  Int_t nMatchedRings=0;
  while( (pMdcClus=(HMdcClus*)iterMdcClus->Next()) !=0) {
    if(pMdcClus->getIOSeg()!=0) continue;
    Int_t sec=pMdcClus->getSec();
    Float_t mdcTheta=pMdcClus->getTheta()*radToDeg;
    Float_t mdcPhi=pMdcClus->getPhi()*radToDeg+sec*60.;
    if(mdcPhi>360.) mdcPhi-=360.;
    Int_t mod=pMdcClus->getMod();
    if(mod<0) mod=1;
    for(Int_t r=0;r<nRichHits[sec];r++) {
      Float_t dPh=richPhi[sec][r]-mdcPhi;
      Float_t dTh=richTheta[sec][r]-mdcTheta;
      Bool_t dPhCond = dPh>=dPhCuts[sec][mod][0] && dPh<=dPhCuts[sec][mod][1];
      Bool_t dThCond = dTh>=dThCuts[sec][mod][0] && dTh<=dThCuts[sec][mod][1];
      if(dPhCond) hDTh[sec][mod]->Fill(dTh);
      if(dThCond) hDPh[sec][mod]->Fill(dPh);
      if(!dPhCond || !dThCond) continue;
      nMdcHitRing[sec][r]++;
      if(nMdcHitRing[sec][r]==1) nMatchedRings++;
    }
  }

  if(nMatchedRings) hNMRings->Fill(nEvents-0.5,(Float_t)nMatchedRings);
  if(nMatchedRings<nRingsCut) return kSkipEvent;
  nMatchedEvents++;
  if(nMatchedRings==1) nMatchedEvents1R++;

  return 0;
}

void HMdcRichCorr::fillListByRichHit(void) {
  HRichHit* pRichHit;
  while( (pRichHit=(HRichHit*)iterRichHit->Next()) !=0) {
    Int_t sec=pRichHit->getSector();
    Int_t& nHits=nRichHits[sec];
    if(nHits>=400) continue;
//     if(pRichHit->getRingPatMat()<=250 &&
//        pRichHit->getRingHouTra()<=28) continue;
    richTheta[sec][nHits]=pRichHit->getTheta();
    richPhi[sec][nHits]=pRichHit->getPhi();
    nMdcHitRing[sec][nHits]=0;
    nHits++;
  }
}

void HMdcRichCorr::fillListByMathURich(void) {
  HMatchURich*  pRichHit;
  while( (pRichHit=(HMatchURich*)iterRichHit->Next()) != 0) {
    Int_t sec=pRichHit->getSegmentId();  // It's sector.
    Int_t& nHits=nRichHits[sec];
    if(nHits>=400) continue;
    richTheta[sec][nHits]=pRichHit->getTheta();
    richPhi[sec][nHits]=pRichHit->getPhi();
    nMdcHitRing[sec][nHits]=0;
    nHits++;
  }
}


void HMdcRichCorr::findMdcClusters(void) {
  Int_t numHits[6][2];
  Int_t s,m,l,c;
  for(Int_t i=0; i<6; i++) numHits[i][0]=numHits[i][1]=0;
  iterCal1->Reset();

  // Fired wires collecting:
  (*fLookUpTb).clear();
  HMdcCal1* cal=0;
  while ((cal=(HMdcCal1 *)iterCal1->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if(nRichHits[s]==0) continue;
    if(m>1) continue;
    Int_t  nHits=cal->getNHits();
    if(nHits>0) nHits=1;
    else if( nHits<0 ) {
      nHits=-nHits;
      if(nHits==2) nHits=3;
    } else continue;
    if(fcut && fcut->cut(cal)) continue; // Cut. for real data
    (*fLookUpTb)[s].setCell(m,l,c,nHits);
    numHits[s][m]++;
  }

  // Cluster finder levels determination:
  Int_t minLevel[6][4];
  for(Int_t sec=0; sec<6; sec++) {
    if(nRichHits[sec]==0) continue;
    Int_t* minlv=minLevel[sec];
    minlv[2]=minlv[3]=0;
    for(Int_t mod=0; mod<2; mod++) {
      minlv[mod]=nLayers[sec][mod];
      if( minlv[mod]>0 ) {
        Int_t occup=(numHits[sec][mod]*6)/minlv[mod];
        if( occup <= level5 ) minlv[mod]--;
        if( occup <= level4 ) minlv[mod]--;
        if( occup < 3 ) minlv[mod]=3;
      }
    }
  }

  // Track finder:
  for(Int_t sec=0; sec<6; sec++) {
    nMdcHits[sec]=0;
    if(nRichHits[sec]==0 || &(*fLookUpTb)[sec]==0 ) continue;
    nMdcHits[sec]=(*fLookUpTb)[sec].findClusters(minLevel[sec]);
  }
}

Bool_t HMdcRichCorr::finalize(void) {
  for(Int_t sec=0; sec<6; sec++) for(Int_t mod=0; mod<2; mod++) {
    if(hDThAll[mod] && hDTh[sec][mod]) hDThAll[mod]->Add(hDTh[sec][mod]);
    if(hDPhAll[mod] && hDPh[sec][mod]) hDPhAll[mod]->Add(hDPh[sec][mod]);
  }
  TString fName=fileName;
  Int_t ind=fName.Index(".");
  if(ind>0) fName=fName.Remove(ind);
  if(histFileSuffix.Length()<1) histFileSuffix="fhs_";
  TString histFile=histFileDir;
  if(histFile.Length()>0) histFile+="/";
  histFile+=histFileSuffix+fName+".root";
  TFile* file=new TFile(histFile.Data(),histFileOption.Data());
  if(!file->IsOpen()) {
    printf("File %s already exists!\n",histFile.Data());
    char* ind="123456789";
    for(Int_t i=0;i<9;i++){
      delete file;
      histFile=histFileDir;
      if(histFile.Length()>0) histFile+="/";
      histFile+=histFileSuffix+fName+"_"+ind[i]+".root";
      file=new TFile(histFile.Data(),histFileOption.Data());
      if(file->IsOpen()) break;
    }
  }
  //  gROOT->SetBatch();
  if(hDThAll[0]) hDThAll[0]->Write();
  if(hDThAll[1]) hDThAll[1]->Write();
  if(hDPhAll[0]) hDPhAll[0]->Write();
  if(hDPhAll[1]) hDPhAll[1]->Write();
  for(Int_t sec=0; sec<6; sec++) for(Int_t mod=0; mod<2; mod++) {
    if(hDTh[sec][mod]) hDTh[sec][mod]->Write();
    if(hDPh[sec][mod]) hDPh[sec][mod]->Write();
  }
  hNEvents->Write();
  for(Int_t bin=0;bin<hNEvents->GetNbinsX();bin++) {
    Stat_t norm=hNEvents->GetBinContent(bin+1);
    if(norm==0.) break; 
    Stat_t nrn=hNRings->GetBinContent(bin+1);
    Stat_t nmrn=hNMRings->GetBinContent(bin+1);
    hNRings->SetBinContent(bin+1,nrn/norm);
    hNMRings->SetBinContent(bin+1,nmrn/norm);
    hNRings->SetBinError(bin+1,sqrt(nrn)/norm);
    hNMRings->SetBinError(bin+1,sqrt(nmrn)/norm);
  }
  hNRings->Write();
  hNMRings->Write();
  //  gROOT->SetBatch(kFALSE);
  file->Close();
  delete file;

  printf("\n\n********** Events filtering *********************************\n");
  printf("* FileName      : %s\n* RunId         : %s\n* Run start time: %s %s\n",
      fileName.Data(),runId.Data(),eventTime.Data(),eventDate.Data());
  printf("* Hist.file     : %s\n",histFile.Data());
  printf("*------------------------------------------------------------\n");
  printf("* Total number of real events processed  : %8i\n",nEvents);
  if(nEvents>0) {
    printf("* Number of events with rings            : %8i (%6.2f%c)\n",
        nRingEvents,Float_t(nRingEvents)/Float_t(nEvents)*100.,'%');
    printf("* Number of matched events               : %8i (%6.2f%c)\n",
        nMatchedEvents,Float_t(nMatchedEvents)/Float_t(nEvents)*100.,'%');
    printf("* Number of events with >=2 matched rings: %8i (%6.2f%c)\n",
        nMatchedEvents-nMatchedEvents1R,
        Float_t(nMatchedEvents-nMatchedEvents1R)/Float_t(nEvents)*100.,'%');
  }
  printf("*************************************************************\n\n");

  if(fCalib1) if(!fCalib1->finalize()) return kFALSE;
  return kTRUE;
}

void HMdcRichCorr::printStatus(void) const{
  // prints the parameters to the screen
  char* hType ="0 = comb.chamb.clust., 1 = single chamb.clust., 2 = 0+1";
  char* hLevS1="max. occupancy of MDC for cl.find. level 4 & 5 layers";
  const Int_t* l=nLayers[0];
  printf("\n\n-----------------------------------------------------------------\n");
   printf("HMdcRichCorr setup:\n");
  if(richFlag==0) printf(
    "Using rings param. from HMatchURich container for matching to MDC.\n");
  else printf("Using rings param. from HRichHit container for matching to MDC.\n");
  printf("Filtering of events with >=%i matched to mdc rings.\n",nRingsCut);
  printf("Matching cuts ( dTheta=thetaRich-thetaMdc  dPhi=phiRich-phiMdc ):\n");
  for(Int_t s=0;s<6;s++) for(Int_t m=0;m<2;m++) {
    printf("  %i sec. %i mdc   %6.2f<dTheta<%.2f   %6.2f<dPhi<%.2f\n",s+1,m+1,
        dThCuts[s][m][0],dThCuts[s][m][1],dPhCuts[s][m][0],dPhCuts[s][m][1]);
  }
  printf("-----------------------------------------------------------------\n");
  printf("HMdcTrackFinderSetup:\n");
  printf("ClustFinderType = %i :  %s\n",clFinderType,hType);
  printf("LevelsSegment1  = %i,%i  : %s\n",level4,level5,hLevS1);
  printf("NumLayersPerMod = {{%i,%i,,},\n",l[ 0],l[ 1]);
  printf("                   {%i,%i,,},\n",l[ 4],l[ 5]);
  printf("                   {%i,%i,,},\n",l[ 8],l[ 9]);
  printf("                   {%i,%i,,},\n",l[12],l[13]);
  printf("                   {%i,%i,,},\n",l[16],l[17]);
  printf("                   {%i,%i,,}}\n",l[20],l[21]);
  printf("-----------------------------------------------------------------\n");
}
