#include "hmdctargetmon.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TPaveLabel.h"
#include "TSystem.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcgeompar.h"
#include "hspecgeompar.h"
#include "heventheader.h"
#include "hmdcdetector.h"
#include "hmdcdef.h"
#include "hevent.h"
#include "hcategory.h"
#include "hgeomvolume.h"
#include "hmdchit.h"
#include "hiterator.h"
#include "hmdcseg.h"
#include "hrun.h"
#include "hldgrepfilesource.h"
#include "TLine.h"

ClassImp(HMdcTargetMon)

HMdcTargetMon::HMdcTargetMon(Bool_t isClOff, UInt_t pt) {
  isCoilOff=isClOff;
  setDefParam(pt);
}

HMdcTargetMon::HMdcTargetMon(Text_t *name,Text_t *title, Bool_t isClOff,
    UInt_t pt) : HReconstructor(name,title) {
  isCoilOff=isClOff;
  setDefParam(pt);
}

void HMdcTargetMon::setDefParam(UInt_t pt) {
  plotType        = pt;
  pHldGrepFSrc    = 0;
  timeStep        = 10.;
  mode            = kFALSE;
  useRtdbTarger   = kTRUE;
  label=0;
  combToMdc[0][0] = 0;
  combToMdc[0][1] = 1;
  combToMdc[1][0] = 0;
  combToMdc[1][1] = 2;
  combToMdc[2][0] = 0;
  combToMdc[2][1] = 3;
  combToMdc[3][0] = 1;
  combToMdc[3][1] = 2;
  combToMdc[4][0] = 1;
  combToMdc[4][1] = 3;
  combToMdc[5][0] = 2;
  combToMdc[5][1] = 3;

  iterMdcHit      = 0;
  firstEvent      = kTRUE;
  dateFEvent      = 0;
  timeFEvent      = 0;
  dateLEvent      = 0;
  timeLEvent      = 0;
  dPhiCut         = 5.5; //15.; //1.5;
  r0Cut           = 20.; //100.; //25.;

  nZmBins         = 1000; //900;
  zmin            = -500.;//-90; //-5000.;  //-1000.;
  zmax            = 600.;//100; //5000.;   //800.;
  histsMode       = 0;
  numEventsSave   = -1;
  eventCounter    = 0;
  
  fillSeg         = kFALSE;
  stat            = 0;
  sumOfFiles      = kTRUE;
  collectNRuns    = 0;
  runsCounter     = 0;
  for(Int_t i=0;i<6;i++) {
    dPhiH[i]      = 0;
    r0H[i]        = 0;
    canvs[i]      = 0;
    dPhiForNR[i]  = 0;
    r0ForNR[i]    = 0;
    canvsForNR[i] = 0;
    for(Int_t s=0;s<6;s++) {
      zVerH[i][s]            = 0;
      zVerVsEventH[i][s]     = 0;    
      zVerForNR[i][s]        = 0;
      zVerVsEventForNR[i][s] = 0;
      for(Int_t k=0;k<10;k++) {
        zVerDscH[i][s][k]     = 0;
        zVerDscForNR[i][s][k] = 0;
      }
   }
  }
  leftTargetPoint  = 0.; 
  rightTargetPoint = 0.;
  drawInOutTracks  = kFALSE;
  inTargetArr      = 0;
  allTargetArr     = 0;
  nRunsDraw        = 0;
  inOutTargCurrRun = 0;
  inOutTargetHist  = 0;
  canvasWidth      = 950;
  canvasHeight     = 700;
  pathHistOneRun   = ".";
  pathHistSumRun   = ".";
  formatEpsTo      = "-";
  deleteEpsFile    = kFALSE;
}

HMdcTargetMon::~HMdcTargetMon() {
  // destructor
  if(iterMdcHit) delete iterMdcHit;
  iterMdcHit=0;
  if(inTargetArr)  delete inTargetArr;
  inTargetArr  = 0;
  if(allTargetArr) delete allTargetArr;
  allTargetArr = 0;
}


void HMdcTargetMon::setMode(Bool_t m, Char_t* fileType) {
  mode = m;
  if(fileType==0) typePrintFile = "-";
  else {
    typePrintFile = fileType;
    typePrintFile.ToLower();
  }
}

void HMdcTargetMon::setConvertFormat(Char_t* p,Bool_t delEps) {
  if(p==0) {
    formatEpsTo="-";
    return;
  }
  deleteEpsFile = delEps;
  formatEpsTo   = p;
  if(formatEpsTo.Contains(".ps") || formatEpsTo.Contains(".eps")) {
    if(typePrintFile == "-") typePrintFile = ".eps";
  } else formatEpsTo="-";
}

TH1F* HMdcTargetMon::setTargetZRegion(Double_t lpoint,Double_t rpoint,
    Int_t nRuns) {
  // set target Z region for calulating 
  // n.tracks_in_target/n.tracks_out_target
  // nRuns - maximal number of runs in hist
  if(drawInOutTracks || nRuns<2 || lpoint>=rpoint) return 0;
  leftTargetPoint  = lpoint;
  rightTargetPoint = rpoint;
  drawInOutTracks  = kTRUE;
  nRunsDraw        = nRuns;
  inTargetArr      = new Double_t [nRunsDraw];
  allTargetArr     = new Double_t [nRunsDraw];
  for(Int_t i=0;i<nRuns;i++) inTargetArr[i]=allTargetArr[i]=0.;
  inOutTargCurrRun = 0;
  inOutTargetHist  = new TH1F("numTrInOutTarget",
      "N.TargetTracks/N.Tracks;Run",nRunsDraw,0.,nRunsDraw);
  inOutTargetHist->SetTitleSize(0.06,"XY");
  inOutTargetHist->GetXaxis()->SetTitleOffset(0.6);
//   inOutTargetHist->GetXaxis()->SetLabelSize(0.07);
//   inOutTargetHist->GetXaxis()->SetLabelOffset(-0.03);
//   inOutTargetHist->GetYaxis()->SetLabelSize(0.07);
//   inOutTargetHist->GetYaxis()->SetLabelOffset(0.0001);
  return inOutTargetHist;
}

void HMdcTargetMon::shiftArrays(void) {
  for(Int_t i=0;i<nRunsDraw-1;i++) {
    inTargetArr[i]  = inTargetArr[i+1];
    allTargetArr[i] = allTargetArr[i+1];
  }
  inTargetArr[nRunsDraw-1]  = 0.;
  allTargetArr[nRunsDraw-1] = 0.;
}

void HMdcTargetMon::fillInOutTargetHist(void) {
  if(!drawInOutTracks) return;
  if(inOutTargCurrRun < nRunsDraw-1) inOutTargCurrRun++;
  for(Int_t i=0;i<=inOutTargCurrRun;i++) fillInOutTarHistBin(i);
  if(inOutTargCurrRun == nRunsDraw-1) shiftArrays();
  setEntriesInOutTHist();
}

void HMdcTargetMon::setEntriesInOutTHist(void) {
  Int_t nRuns = inOutTargCurrRun;
  if(allTargetArr[inOutTargCurrRun]>0) nRuns++;
  inOutTargetHist->SetEntries(nRuns);
}

void HMdcTargetMon::fillInOutTarHistBin(Int_t bin) {
  if(allTargetArr[bin]>0) {
    Double_t cont = inTargetArr[bin]/allTargetArr[bin];
    Double_t err  = sqrt(inTargetArr[bin])/allTargetArr[bin];
    inOutTargetHist->SetBinContent(bin+1,cont);
    inOutTargetHist->SetBinError(bin+1,err);
  } else {
    inOutTargetHist->SetBinContent(bin+1,0.);
    inOutTargetHist->SetBinError(bin+1,0.);
  }
}

Bool_t HMdcTargetMon::init(void) {
  fMdcHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (fMdcHitCat) iterMdcHit=(HIterator *)fMdcHitCat->MakeIterator();
  else {
    Error("init","The category HMdcHit is absent");
    return kFALSE;
  }

  HMdcGeomPar* fMdcGeomPar=
      (HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if(!fMdcGeomPar) return kFALSE;

  HSpecGeomPar* fSpecGeomPar=
      (HSpecGeomPar*)gHades->getRuntimeDb()->getContainer("SpecGeomPar");
  if(!fSpecGeomPar) return kFALSE;
  
  HDataSource* pDataSource = gHades->getDataSource();
  pHldGrepFSrc =  dynamic_cast<HldGrepFileSource *>(pDataSource);
 
  if(fillSeg) {
    fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if (!fSegCat) {
      HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
      fSegCat = fMdcDet->buildCategory(catMdcSeg);
      if (!fSegCat) return kFALSE;
      gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
    }
  } else fSegCat=0;
  locSeg.set(2,0,0);
  
  if(!initHists()) return kFALSE;
  return kTRUE;
}

Bool_t HMdcTargetMon::initHists(void) {
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  Int_t* lMods=fMdcDet->getModules();
  Int_t nModsMax=(isCoilOff) ? 4:2;
  Int_t nSectors=0;
  for (Int_t n=0; n<24; n++) listMdc[n]=kFALSE;
  for (Int_t sec = 0; sec < 6; sec++) {
    mdcInSec[sec]=0;
    for(Int_t mod=0; mod<nModsMax; mod++) if(lMods[mod]>0) mdcInSec[sec]++;
    if(mdcInSec[sec]>=2) {
      for(Int_t mod=0; mod<nModsMax; mod++)
          if(lMods[mod]>0) listMdc[sec*4+mod]=kTRUE;
      nSectors++;
    } else mdcInSec[sec]=0;
    lMods += 4;
  }
  if(nSectors == 0) {
    Error("init","Numb. of MDC's is not enough for target finding!");
    return kFALSE;
  }
  

  for(Int_t comb=0; comb<6; comb++) {
    dPhiH[comb]=0;
    r0H[comb]=0;
    for(Int_t sec=0; sec<6; sec++) {
      zVerH[sec][comb]=0;
      zVerVsEventH[sec][comb]=0;
    }
  }
  Char_t name[100];
  Char_t title[100];
  for(Int_t comb=0; comb<6; comb++) nPads[comb]=0;
  gStyle->SetOptStat(10);
  for(Int_t sec=0; sec<6; sec++) {
    for(Int_t mod1=0; mod1<nModsMax-1; mod1++) {
      if(!listMdc[sec*4+mod1]) continue;
      for(Int_t mod2=mod1+1; mod2<nModsMax; mod2++) {
        if(!listMdc[sec*4+mod2]) continue;
        Int_t comb=mod1+mod2;
        if(mod1==0) comb--;
        nPads[comb]++;
        if(!dPhiH[comb]) {
          sprintf(name,"s%im%iv%i_dPhi",sec+1,mod1+1,mod2+1);
          sprintf(title,"S.%i M.%i-%i  #Delta#phi (deg.)",sec+1,mod1+1,mod2+1);
          dPhiH[comb]=new TH1F(name,title,200,-10.,10.);
          sprintf(name,"s%im%iv%i_Rm",sec+1,mod1+1,mod2+1);
          sprintf(title,"S.%i M.%i-%i  R_{min} (mm)",sec+1,mod1+1,mod2+1);
          r0H[comb]=new TH1F(name,title,500,-100.,100.);
        }
        if(!zVerH[sec][comb]) {
          sprintf(name,"s%im%iv%i_Zm",sec+1,mod1+1,mod2+1);
          sprintf(title,"S.%i M.%i-%i",sec+1,mod1+1,mod2+1);
          zVerH[sec][comb]=new TH1F(name,title,nZmBins,zmin,zmax);
          zVerH[sec][comb]->SetFillColor(3);
          if(histsMode==1) {
            zVerH[sec][comb]->GetXaxis()->SetTitle(
                "Downsc.Flag:  #color[4]{0}, #color[2]{1}             Zmin, (mm)");
          } else zVerH[sec][comb]->GetXaxis()->SetTitle("Zmin, (mm)");
          zVerH[sec][comb]->GetXaxis()->SetTitleOffset(0.9);
          if(histsMode==1) {
            sprintf(name,"s%im%iv%i_ZmDS0",sec+1,mod1+1,mod2+1);
            sprintf(title,"DownscalingFlag=0;Zmin, (mm)");
            zVerDscH[sec][comb][0]=new TH1F(name,title,nZmBins,zmin,zmax);
            zVerDscH[sec][comb][0]->SetFillColor(4);
            zVerDscH[sec][comb][0]->SetLineColor(4);
            sprintf(name,"s%im%iv%i_ZmDS1",sec+1,mod1+1,mod2+1);
            sprintf(title,"DownscalingFlag=1;Zmin, (mm)");
            zVerDscH[sec][comb][1]=new TH1F(name,title,nZmBins,zmin,zmax);
            zVerDscH[sec][comb][1]->SetLineColor(2);
          } else if(histsMode==2) {
            Int_t col[10]={2,4,5,6,7,8,9,10,34,36};
            for(Int_t n=0;n<8;n++) {
              sprintf(name,"s%im%iv%i_ZmTR%i",sec+1,mod1+1,mod2+1,n);
              sprintf(title,"Trigger. %i;Zmin, (mm)",n);
              zVerDscH[sec][comb][n]=new TH1F(name,title,nZmBins,zmin,zmax);
              zVerDscH[sec][comb][n]->SetLineColor(col[n]);
              zVerDscH[sec][comb][n]->SetFillColor(0);
            }
          }

          if(plotType==1) {
            sprintf(name,"s%im%iv%i_ZmVev",sec+1,mod1+1,mod2+1);
            sprintf(title,"S.%i M.%i-%i  Z_{min} vs Nev.;Event;Zmin",sec+1,mod1+1,mod2+1);
            zVerVsEventH[sec][comb]=
              new TH2S(name,title,1000,0,1000,500,zmin,zmax);
//new TH2S(name,title,1000,0,1000,256,0.,256.);
            zVerVsEventH[sec][comb]->SetBit(TH1::kCanRebin);
          } else if(plotType==2) {
            sprintf(name,"s%im%iv%i_YvX",sec+1,mod1+1,mod2+1);
            sprintf(title,"S.%i M.%i-%i  Y vs X;X (mm);Y (mm)",sec+1,mod1+1,mod2+1);
            zVerVsEventH[sec][comb]=
              new TH2S(name,title,40,-20.,20.,40,-20.,20.);
          } else if(plotType==3) {
            sprintf(name,"s%im%iv%i_ZmVtm",sec+1,mod1+1,mod2+1);
            sprintf(title,"S.%i M.%i-%i  Z_{min} vs ev.time;Time;Zmin",sec+1,mod1+1,mod2+1);
            zVerVsEventH[sec][comb]=
              new TH2S(name,title,500,0,500,500,zmin,zmax);
//new TH2S(name,title,1000,0,1000,256,0.,256.);
            zVerVsEventH[sec][comb]->SetBit(TH1::kCanRebin);
          } else {
            sprintf(name,"s%im%iv%i_ThVZm",sec+1,mod1+1,mod2+1);
            sprintf(title,"Theta vs Zmin;Zmin;Theta");
            zVerVsEventH[sec][comb]=new TH2S(name,title,100,-250.,150.,80,10,90);
          }
        }
      }
    }
  }
  initRunsSumHists();
  return kTRUE;
} 
 
void HMdcTargetMon::initRunsSumHists(void) {
  if(collectNRuns<2) return;  
  Char_t name[100]; 
  for(Int_t sec=0; sec<6; sec++) {
    for(Int_t mod1=0; mod1<3; mod1++) {
      for(Int_t mod2=mod1+1; mod2<4; mod2++) {
        Int_t comb=mod1+mod2;        
        if(mod1==0) comb--;

        if(dPhiH[comb]!=0 && dPhiForNR[comb]==0) {
          sprintf(name,"s%im%iv%i_dPhi_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
          dPhiForNR[comb]=new TH1F(*(dPhiH[comb]));
          dPhiForNR[comb]->SetName(name);
          sprintf(name,"s%im%iv%i_Rm_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
          r0ForNR[comb]=new TH1F(*(r0H[comb]));
          r0ForNR[comb]->SetName(name);
          
        }
        if(zVerH[sec][comb]!=0 && zVerForNR[sec][comb]==0) {
          sprintf(name,"s%im%iv%i_Zm_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
          zVerForNR[sec][comb]=new TH1F(*(zVerH[sec][comb]));
          zVerForNR[sec][comb]->SetName(name);
          if(histsMode==1) {
            sprintf(name,"s%im%iv%i_ZmDS0_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
            zVerDscForNR[sec][comb][0]=new TH1F(*(zVerDscH[sec][comb][0]));
            zVerDscForNR[sec][comb][0]->SetName(name);
            sprintf(name,"s%im%iv%i_ZmDS1_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
            zVerDscForNR[sec][comb][1]=new TH1F(*(zVerDscH[sec][comb][1]));
            zVerDscForNR[sec][comb][1]->SetName(name);
          } else if(histsMode==2) {
            for(Int_t n=0;n<8;n++) {
              sprintf(name,"s%im%iv%i_ZmTR%i_%iruns",sec+1,mod1+1,mod2+1,n,collectNRuns);
              zVerDscForNR[sec][comb][n]=new TH1F(*(zVerDscH[sec][comb][n]));
              zVerDscForNR[sec][comb][n]->SetName(name);
            }
          }

          if(plotType==1) {
            sprintf(name,"s%im%iv%i_ZmVev_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
            zVerVsEventForNR[sec][comb]=new TH2S(*(zVerVsEventH[sec][comb]));
            zVerVsEventForNR[sec][comb]->SetName(name);
            zVerVsEventForNR[sec][comb]->SetBit(TH1::kCanRebin);
          } else if(plotType==2) {
            sprintf(name,"s%im%iv%i_YvX_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
            zVerVsEventForNR[sec][comb]=new TH2S(*(zVerVsEventH[sec][comb]));
            zVerVsEventForNR[sec][comb]->SetName(name);
          } else if(plotType==3) {
            sprintf(name,"s%im%iv%i_ZmVtm_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
            zVerVsEventForNR[sec][comb]=new TH2S(*(zVerVsEventH[sec][comb]));
            zVerVsEventForNR[sec][comb]->SetName(name);
          } else {
            sprintf(name,"s%im%iv%i_ThVZm_%iruns",sec+1,mod1+1,mod2+1,collectNRuns);
            zVerVsEventForNR[sec][comb]=new TH2S(*(zVerVsEventH[sec][comb]));
            zVerVsEventForNR[sec][comb]->SetName(name);
          }
        }
      }
    }
  }
} 
 
Bool_t HMdcTargetMon::reinit(void) {
  evHeader=gHades->getCurrentEvent()->getHeader();
  if(!evHeader) {
    Error("reinit","Can't get pointer to event header.");
    return kFALSE;
  }

  HMdcGeomPar* fMdcGeomPar=
      (HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if(!fMdcGeomPar) return kFALSE;
  for(Int_t s=0;s<6;s++) {
    for(Int_t m=0;m<4;m++) {
      HModGeomPar* mGeomPar=fMdcGeomPar->getModule(s,m);
      if(mGeomPar) transf[s*4+m]=&(mGeomPar->getLabTransform());
      else transf[s*4+m]=0;
    }
  }

  HSpecGeomPar* fSpecGeomPar=
      (HSpecGeomPar*)gHades->getRuntimeDb()->getContainer("SpecGeomPar");
  if(!fSpecGeomPar) return kFALSE;
  if(useRtdbTarger) {
    Int_t nTarg=fSpecGeomPar->getNumTargets()-1;
    target=fSpecGeomPar->getTarget(0)->getTransform().getTransVector()
        +fSpecGeomPar->getTarget(nTarg)->getTransform().getTransVector();
    target /= 2.;
  }
  printf("*** Target: ");
  target.print();
  
  runId=evHeader->getEventRunNumber();
  if(sumOfFiles) {
    collectNRuns=0;       // collect n.runs for sumOfFiles=kFALSE only !!!
    extractFileInfo();
    if(firstEvent) {
      setCanvName();
      if(!mode) {
        timer.Reset();
        timer.Start();
      }
      firstEvent=kFALSE;
    }
  } else {
    if(firstEvent) {
      extractFileInfo();
      setCanvName();
      drawCanvases();
      if(collectNRuns>1) {
        firstRunName = fileNameR;
        lastRunName  = fileNameR;
        drawCanvasesRunsSum();
      }
    }
    stat=0;
    if(!mode) {
      timer.Reset();
      timer.Start();
    }
    firstEvent=kFALSE;
  }

  return kTRUE;
}

Bool_t HMdcTargetMon::finalize(void) {
  drawCanvases();
  saveCanvases();
  return kTRUE;
}

void HMdcTargetMon::saveCanvases(void) {
  if(stat<5) {
//    printf("Run %s: No tracks found!\n",fileNameR.Data());
    return;
  }
  if(typePrintFile.CompareTo("-")!=0) {
    typePrintFile.ToLower();
    for(Int_t comb=0; comb<6; comb++) {
      if(nPads[comb]==0) continue;
      TCanvas *canv = canvs[comb]; //(TCanvas*)gROOT->FindObject(cnComb[comb]);
      if(canv == 0) continue;
      if(typePrintFile.Contains(".pdf")) {
        canv->SaveAs(".ps");
        makePDF(cnComb[comb]);
      }
      saveCanvas(canv,pathHistOneRun);
    }
  }
}

void HMdcTargetMon::saveCanvasesForNR(void) {
  if(typePrintFile.CompareTo("-")!=0) {
    for(Int_t comb=0; comb<6; comb++) {
      if(nPads[comb]==0) continue;
      TCanvas *canv = canvsForNR[comb]; //(TCanvas*)gROOT->FindObject(cnComb[comb]);
      if(canv == 0) continue;
      if(typePrintFile.Contains(".pdf")) {
        canv->SaveAs(".ps");
        makePDF(cnCombForNR[comb]);
      }
      saveCanvas(canv,pathHistSumRun);
    }
  }
}

void HMdcTargetMon::saveCanvas(TCanvas* canv,TString& path) {
  if(canv == 0) return;
  TString fileName(path);
  fileName += "/";
  fileName += canv->GetName();
  if(formatEpsTo != "-" ) {
    TString fullFileName(fileName);
    if(formatEpsTo.Contains(".ps")) fullFileName += ".ps";
    else                            fullFileName += ".eps";
    canv->SaveAs(fullFileName.Data());
    Char_t command[500];
    sprintf(command,formatEpsTo,fileName.Data(),fileName.Data());
    Int_t flag = gSystem->Exec(command);
    if(deleteEpsFile && flag==0) {
      sprintf(command,"rm -f %s",fullFileName.Data());
      gSystem->Exec(command);
    }
  } else {
    if(typePrintFile.Length() == 0)     canv->SaveAs("");
    if(typePrintFile.Contains(".ps"))   canv->SaveAs((fileName+".ps").Data());
    if(typePrintFile.Contains(".eps"))  canv->SaveAs((fileName+".eps").Data());
    if(typePrintFile.Contains(".gif"))  canv->SaveAs((fileName+".gif").Data());
    if(typePrintFile.Contains(".svg"))  canv->SaveAs((fileName+".svg").Data());
    if(typePrintFile.Contains(".jpg"))  canv->SaveAs((fileName+".jpg").Data());
  }
  if(typePrintFile.Contains(".root")) canv->SaveAs((fileName+".root").Data());
}
    
Int_t HMdcTargetMon::execute(void) {
  UInt_t newRunId = (pHldGrepFSrc) ? (UInt_t) pHldGrepFSrc->getCurrentRunId() :
                                              evHeader->getEventRunNumber();
  if(runId != newRunId) {
      // New run but not first run! ------------------------------------------
    fillInOutTargetHist();
    if( !sumOfFiles ) {
      runsCounter++;
      drawCanvases();
      saveCanvases();  // save hists for each run
      if(numEventsSave > 0) eventCounter = 0;
      if(collectNRuns>1) {
        lastRunName = fileNameR;
        addHistsToRunsSum();     // add and draw hists
        if(runsCounter==collectNRuns) saveCanvasesForNR();
      }
      extractFileInfo();
      
      if(collectNRuns>1 && runsCounter==collectNRuns) {
        runsCounter  = 0;
        firstRunName = fileNameR;
        lastRunName  = fileNameR;
        resetHistsRunsSum();
        drawCanvasesRunsSum();
      }

       stat=0;
       setCanvName();
       resetHists();

       if(!mode) {
         timer.Reset();
         timer.Start();
       }
       firstEvent=kFALSE;
    }
  }
  
  UInt_t downscalingFlag=evHeader->getDownscalingFlag() & 1;
  
  Int_t tBit = (evHeader->getTBit()&255);
  Int_t hNum=-1;
  if(histsMode==2) {
    if(tBit&1) hNum=(tBit&32) ? 1:0;        // 0 - mult.4; 1 - mult.4+opp.sec.
    else if(tBit& 2) hNum=2;                // minimum bias
    else if(tBit& 4) hNum=(tBit&32) ? 4:3;  // 3 - mult.2; 4 - mult.2+opp.sec.
    else if(tBit& 8) hNum=5;                // M1 //small angle
    else if(tBit&16) hNum=7;                // M3 //pulser;
    else if(tBit&32) hNum=6;                // opp.sec.
  }
  
  
//Trigger Box setting for alignment run:
//   1         2        4       8       16           32
// 1         2        3       4        5           6             7       8
//M4    Start      Veto       M1      M3    opp. Sec.          1kHz   r.puls
// 2         C        D       C       0           2              0 0
//on        on      on     on      on         on           OFF   on
  
  
  if(evHeader->getId() != 1) return 0;
  timeLEvent= evHeader->getTime();
  UInt_t nevent=evHeader->getEventSeqNumber();
  
// static UInt_t oldEvn=0;
// static UInt_t mmm=0;
// if(oldEvn==0) {
//   oldEvn=nevent;
//   mmm=nevent;
// }
// if((tBit&128)) {
//   if(nevent-oldEvn > 200) {
//     printf("nevent=%i  nevent-oldEvn=%i\n",nevent,nevent-oldEvn);
//     mmm=nevent;
//   }
//   oldEvn=nevent;
// }
// nevent-=mmm;
// if((tBit&1)==0) return 0;
  for(Int_t n=0; n<24; n++) nhits[n]=0;
  HGeomVector vHit;
  HMdcHit* hit;
  iterMdcHit->Reset();
  while ( (hit=(HMdcHit*)iterMdcHit->Next()) != 0 ){
    //if(hit->getChi2()<0. || hit->getChi2()>30.) continue;
    //if(hit->getOffset()<-10.||hit->getOffset()>50.) continue;
    Int_t sec=hit->getSector();
    if(mdcInSec[sec]<2) continue;
    Int_t mod=hit->getModule();
    Int_t indx=sec*4+mod;
    if(!listMdc[indx]) continue;
    //   if(mod != modF && mod != modS) continue;

    vHit.setXYZ(hit->getX(),hit->getY(),0.);
    vHit=transf[indx]->transFrom(vHit);
    Int_t indx2=nhits[indx];
    x[indx][indx2]=vHit(0) - target(0);  // shift Z to target position
    y[indx][indx2]=vHit(1) - target(1);  //
    z[indx][indx2]=vHit(2);
    vHit -= target;
    Double_t phiN=TMath::ATan2(vHit(1),vHit(0))*180./TMath::Pi();
    if(phiN<0.) phiN+=360.;
//    Double_t thetaN=acos(vHit(2)/vHit.length())*180./TMath::Pi();
    phi[indx][indx2]=phiN;
    nhits[indx]++;
  }

//zVerVsEventH[0][5]->Fill(nevent+0.5,tBit);
  if(fSegCat) fSegCat->Clear(); 
    
  for(Int_t sec=0; sec<6; sec++) {
    if(mdcInSec[sec]<2) continue;
    
    Double_t secAng = -sec*60.*TMath::DegToRad();
    Double_t cosSec = TMath::Cos(secAng);
    Double_t sinSec = TMath::Sin(secAng);
    
    Int_t iS=sec*4;
    locSeg[0]=sec;
    for(Int_t mod1=0; mod1<3; mod1++) {
      Int_t iM1= iS+mod1;
      if(nhits[iM1]==0) continue;
      locSeg[1]=mod1>>1;   // po pervomu modulyu !!!
      for(Int_t mod2=mod1+1; mod2<4; mod2++) {
        Int_t iM2= iS+mod2;
        if(nhits[iM2]==0) continue;
        Int_t comb=mod1+mod2;
        if(mod1==0) comb--;
        for(Int_t hitM1=0; hitM1<nhits[iM1]; hitM1++) {
          for(Int_t hitM2=0; hitM2<nhits[iM2]; hitM2++) {
            //-------------------------------------------
            Double_t dX=x[iM2][hitM2]-x[iM1][hitM1];
            Double_t dY=y[iM2][hitM2]-y[iM1][hitM1];
            Double_t phiTr=TMath::ATan2(dY,dX);
            Double_t r0=y[iM1][hitM1]*cos(phiTr)-x[iM1][hitM1]*sin(phiTr);
            Double_t dZ=z[iM2][hitM2]-z[iM1][hitM1];
            Double_t thetaTr=acos(dZ/sqrt(dX*dX+dY*dY+dZ*dZ));
//if(fabs(thetaTr)<45.*TMath::DegToRad()) continue;
            if( fabs(thetaTr) < 0.01 ) continue;
            Double_t zm=z[iM1][hitM1] - cos(thetaTr)/sin(thetaTr) *
                (x[iM1][hitM1]*cos(phiTr)+y[iM1][hitM1]*sin(phiTr));
            //-------------------------------------------
            Double_t dPhi=phi[iM1][hitM1]-phi[iM2][hitM2];
            if(dPhi<-180.) dPhi+=360.;
            else if(dPhi>180.) dPhi-=360.;
            dPhiH[comb]->Fill(dPhi);

            //Double_t dTheta=theta[iM1][hitM1]-theta[iM2][hitM2];
//if(theta[iM1][hitM1]<40.||theta[iM2][hitM2]<40.) continue;
            if(fabs(dPhi)>=dPhiCut) continue;
            //-------------------------------------------
            r0H[comb]->Fill(r0);
            //-------------------------------------------
            if(fabs(r0)>=r0Cut) continue;
            
            stat++;
            
            if(fillSeg) {
              HMdcSeg* fSeg = (HMdcSeg*)fSegCat->getNewSlot(locSeg);
              if(!fSeg) Warning("execute","S.%i No HMdcSeg slot available",sec+1);
              else {
                fSeg=new(fSeg) HMdcSeg;
                fSeg->setPar(zm,r0,thetaTr,phiTr);
                fSeg->setSec(locSeg[0]);
                fSeg->setIOSeg(locSeg[1]);
                fSeg->setFlag((mod1<<2)+mod2);
                fSeg->setChi2(100.); //!!!!!!!!!!!!!!!!!
              }
            }
            
            
            zVerH[sec][comb]->Fill(zm);
            if(drawInOutTracks) {
              allTargetArr[inOutTargCurrRun] += 1.;
              if(zm>leftTargetPoint && zm<rightTargetPoint) 
                   inTargetArr[inOutTargCurrRun]  += 1.;
            }
            if(histsMode==1) zVerDscH[sec][comb][downscalingFlag]->Fill(zm);
            else if(histsMode==2 && hNum>=0 && hNum<8) 
                zVerDscH[sec][comb][hNum]->Fill(zm);
            if(plotType==1) {
              if(zm>zmin && zm<zmax) zVerVsEventH[sec][comb]->Fill(nevent+0.5,zm);
            } else if(plotType==2) {
              if(fabs(dZ)>1.e-20 && fabs(zm-target(2))<50.) {
                Double_t tan=dY/dZ;
                Double_t y0=tan*target(2)+y[iM1][hitM1]-tan*z[iM1][hitM1];
                tan=dX/dZ;
                Double_t x0=tan*target(2)+x[iM1][hitM1]-tan*z[iM1][hitM1];
                Double_t xl = x0*cosSec + y0*sinSec;
                Double_t yl = y0*cosSec - x0*sinSec;
                zVerVsEventH[sec][comb]->Fill(xl,yl);
              }
            } else if(plotType==3) {
              if(zm>zmin && zm<zmax) zVerVsEventH[sec][comb]->Fill(eventTime()+0.5,zm);
            } else zVerVsEventH[sec][comb]->Fill(zm,thetaTr*TMath::RadToDeg());
          }
        }
      }
    }
  }
  if(!mode) {
    if(timer.CpuTime()>=timeStep) {
      if(drawInOutTracks) {
        fillInOutTarHistBin(inOutTargCurrRun);
        setEntriesInOutTHist();
      }
      drawCanvases();
      timer.Reset();
    }
    timer.Start(kFALSE);
  }
  
  // Saving canvases for each "numEventsSave" events:
  if(numEventsSave > 0) {
    eventCounter++;
    if(eventCounter%numEventsSave == 0) {
      if(drawInOutTracks) {
        fillInOutTarHistBin(inOutTargCurrRun);
        setEntriesInOutTHist();
      }
      drawCanvases();
      saveCanvases();
    }
  }
  
  return 0;
}

void HMdcTargetMon::extractFileInfo(void) {
  dateFEvent  = evHeader->getDate();
  timeFEvent  = evHeader->getTime();
  TString fileNameOld(fileNameR);
  if(pHldGrepFSrc) {
    fileNameR = pHldGrepFSrc->getCurrentFileName();
    runId     = pHldGrepFSrc->getCurrentRunId();
//    runId     = evHeader->getEventRunNumber();
  } else {
    fileNameR = gHades->getRuntimeDb()->getCurrentFileName();
    runId     = gHades->getRuntimeDb()->getCurrentRun()->getRunId();
  }
  Int_t ind=fileNameR.Index("/");
  while(ind>=0) {
    fileNameR=fileNameR.Replace(0,ind+1,"");
    ind=fileNameR.Index("/");
  }
  ind=fileNameR.Index(".");
  if(ind>0) fileNameR=fileNameR.Remove(ind);
if(fileNameR.Length()>13) fileNameR=fileNameR.Remove(13);
  if(firstEvent || !sumOfFiles) fileName=fileNameR;
  else {
    fileName += ".";
//    fileName += fileNameR;
  }
}

void HMdcTargetMon::fillDateTime(void) {
  UChar_t* dt  = (UChar_t*)&dateFEvent;
  UChar_t* tm1 = (UChar_t*)&timeFEvent;
  UChar_t* tm2 = (UChar_t*)&timeLEvent;
  Char_t buf[100];
  sprintf(buf,"%02i-%02i-%4i:  %02i:%02i:%02i - %02i:%02i:%02i\n",
      dt[0],dt[1]+1,1900+dt[2],tm1[2],tm1[1],tm1[0],tm2[2],tm2[1],tm2[0]);
  dateTime=buf;
}

Int_t HMdcTargetMon::eventTime(void) {
  UChar_t* tm1=(UChar_t*)&timeFEvent;
  UChar_t* tm2=(UChar_t*)&timeLEvent;
  UInt_t t1=(((UInt_t)tm1[2])*60+((UInt_t)tm1[1]))*60+(UInt_t)tm1[0];
  UInt_t t2=(((UInt_t)tm2[2])*60+((UInt_t)tm2[1]))*60+(UInt_t)tm2[0];
  if(t1<=t2) return t2-t1;
  return (t2+86400)-t1;
}

void HMdcTargetMon::setCanvName(void) {
//   sprintf(cnCut,"cR%icuts",runId);
//   sprintf(ctCut,"Run %s    Cuts",fileNameR.Data());
  for(Int_t comb=0; comb<6; comb++)
    if(nPads[comb]>0) {
      sprintf(cnComb[comb],"target%s_%s_M%i%i",addCnName.Data(),
          fileNameR.Data(),combToMdc[comb][0]+1,combToMdc[comb][1]+1);
      sprintf(ctComb[comb],"Run %s  %s  MDC %i-%i",fileNameR.Data(),
          addCnName.Data(),combToMdc[comb][0]+1,combToMdc[comb][1]+1);
      if(canvs[comb]) {
        canvs[comb]->SetName(cnComb[comb]);
        canvs[comb]->SetTitle(ctComb[comb]);
        canvs[comb]->Update();
      }
   }
}

void HMdcTargetMon::setCanvNameForNR(void) {
  if(collectNRuns<2) return;
  for(Int_t comb=0; comb<6; comb++)
    if(nPads[comb]>0) {
      sprintf(cnCombForNR[comb],"targetSum%iruns%s_%s-%s_M%i%i",
          collectNRuns,addCnName.Data(),
          firstRunName.Data(),lastRunName.Data(),
          combToMdc[comb][0]+1,combToMdc[comb][1]+1);
      sprintf(ctCombForNR[comb],"Runs %s-%s  %s  MDC %i-%i",
          firstRunName.Data(),lastRunName.Data(),
          addCnName.Data(),combToMdc[comb][0]+1,combToMdc[comb][1]+1);
      if(canvsForNR[comb]) {
        canvsForNR[comb]->SetName(cnCombForNR[comb]);
        canvsForNR[comb]->SetTitle(ctCombForNR[comb]);
        canvsForNR[comb]->Update();
      }
   }
}

void HMdcTargetMon::setCanvasSize(Int_t ww, Int_t wh) {
  if(ww>50) canvasWidth  = ww;
  if(wh>50) canvasHeight = wh;
}

void HMdcTargetMon::drawCanvases(void) {
//  fillDateTime();
  Char_t title[200];
  if(label==0) label=new TPaveLabel(0.1,0.955,0.9,0.995," ");
//  sprintf(title,"Run:   #color[4]{%s}       Date:   #color[4]{%s}",
//      fileName.Data(),dateTime.Data());
  sprintf(title,"%s Run:   #color[4]{%s}",addCnName.Data(),fileName.Data());
  label->SetLabel(title);
  Int_t nPadsPerSector = drawInOutTracks ? 3 : 2;
  for(comb=0; comb<6; comb++) {
    if(nPads[comb]==0) continue;
    if(canvs[comb] == 0) {
      Int_t yln = canvasHeight;
      if(nPads[comb]>1) yln += (nPads[comb]-1)*100;
      if(yln>900) yln = 900;
      canvs[comb] = new TCanvas(cnComb[comb],ctComb[comb],canvasWidth,yln);
      currCanvas = canvs[comb];
      currCanvas->cd();
      currCanvas->SetFillColor(10);
      label->Draw();
      Double_t dY=0.95/nPads[comb];
      Int_t padsRow=0;
      for(Int_t sec=0; sec<6; sec++) {
        if(!zVerH[sec][comb]) continue;
        pad         = padsRow*nPadsPerSector;
        Double_t y1 = dY*(nPads[comb]-padsRow-1)+0.0001;
        Double_t y2 = dY*(nPads[comb]-padsRow)-0.0001;
        
        drawPad(0.0001,y1,0.6999,y2);
        sprintf(title,"sec.%i mod. %i-%i",sec+1,
            combToMdc[comb][0]+1,combToMdc[comb][1]+1);
        zVerH[sec][comb]->SetTitle(title);
        zVerH[sec][comb]->Draw();
        
        if(drawInOutTracks) {
          TLine* ln = new TLine();
          ln->SetLineColor(2);
          ln->SetLineWidth(4);
          ln->DrawLine(leftTargetPoint,0.,rightTargetPoint,0.);
        }
        
        fPads[comb][pad]->SetGridx();
        fPads[comb][pad]->SetGridy();
        
        if(histsMode==1) {
          zVerDscH[sec][comb][0]->Draw("same");
          zVerDscH[sec][comb][1]->Draw("same");
        } else if(histsMode==2)
            for(Int_t n=0;n<8;n++) zVerDscH[sec][comb][n]->Draw("same");
        
        Double_t y1b = drawInOutTracks ? (y1+y2)*0.4+0.0001 : y1;
        Double_t y2b = drawInOutTracks ? (y1+y2)*0.4-0.0001 : y2;
        pad++;
        drawPad(0.7001,y1b,0.9999,y2);
        if(plotType==2) {
          sprintf(title,"%s Z=%f",zVerVsEventH[sec][comb]->GetTitle(),target(2));
          zVerVsEventH[sec][comb]->SetTitle(title);
        }
	    gPad->SetGridx();
	    gPad->SetGridy();
        zVerVsEventH[sec][comb]->Draw("colz");
        
        if(drawInOutTracks) {
          pad++;
          drawPad(0.7001,y1,0.9999,y2b);
          inOutTargetHist->SetLineColor(2);
          inOutTargetHist->Draw();
        }
        padsRow++;
        
      }
    } else {
      canvs[comb]->cd();
      for(Int_t pad=0;pad<(nPadsPerSector*nPads[comb]); pad++)
        fPads[comb][pad]->Modified();
    }
    canvs[comb]->Modified();
    canvs[comb]->Update();
  }
}

void HMdcTargetMon::drawPad(Double_t x1,Double_t y1,Double_t x2,Double_t y2) {
  Char_t title[20];
  sprintf(title,"pad%i_%02i",comb,pad);
  currCanvas->cd();
  fPads[comb][pad] = new TPad(title,title,x1,y1,x2,y2);
  fPads[comb][pad]->Draw();
  fPads[comb][pad]->cd();
}

void HMdcTargetMon::drawCanvasesRunsSum(void) {
//  fillDateTime();
  setCanvNameForNR();
  Char_t title[200];
  if(label==0) label=new TPaveLabel(0.1,0.955,0.9,0.995," ");
//  sprintf(title,"Run:   #color[4]{%s}       Date:   #color[4]{%s}",
//      fileName.Data(),dateTime.Data());
  sprintf(title,"%s %i runs: #color[4]{%s-%s}",addCnName.Data(),runsCounter,
      firstRunName.Data(),lastRunName.Data());
  label->SetLabel(title);
  Int_t nPadsPerSector = drawInOutTracks ? 3 : 2;
  for(comb=0; comb<6; comb++) {
    if(nPads[comb]==0) continue;
    if(canvsForNR[comb] == 0) {
      Int_t yln = canvasHeight;
      if(nPads[comb]>1) yln += (nPads[comb]-1)*100;
      if(yln>900) yln = 900;
      canvsForNR[comb] = new TCanvas(cnCombForNR[comb],ctCombForNR[comb],
          canvasWidth,yln);
      currCanvas = canvsForNR[comb];
      currCanvas->cd();
      currCanvas->SetFillColor(10);
      label->Draw();
      Double_t dY=0.95/nPads[comb];
      Int_t padsRow=0;
      for(Int_t sec=0; sec<6; sec++) {
        if(!zVerForNR[sec][comb]) continue;
        pad         = padsRow*nPadsPerSector;
        Double_t y1 = dY*(nPads[comb]-padsRow-1)+0.0001;
        Double_t y2 = dY*(nPads[comb]-padsRow)-0.0001;
        
        drawPadS(0.0001,y1,0.6999,y2);
        sprintf(title,"sec.%i mod. %i-%i",sec+1,
            combToMdc[comb][0]+1,combToMdc[comb][1]+1);
        zVerForNR[sec][comb]->SetTitle(title);
        zVerForNR[sec][comb]->Draw();
        
        if(drawInOutTracks) {
          TLine* ln = new TLine();
          ln->SetLineColor(2);
          ln->SetLineWidth(4);
          ln->DrawLine(leftTargetPoint,0.,rightTargetPoint,0.);
        }
        
        fPadsForNR[comb][pad]->SetGridx();
        fPadsForNR[comb][pad]->SetGridy();
        
        if(histsMode==1) {
          zVerDscForNR[sec][comb][0]->Draw("same");
          zVerDscForNR[sec][comb][1]->Draw("same");
        } else if(histsMode==2)
            for(Int_t n=0;n<8;n++) zVerDscForNR[sec][comb][n]->Draw("same");
        
        Double_t y1b = drawInOutTracks ? (y1+y2)*0.4+0.0001 : y1;
        Double_t y2b = drawInOutTracks ? (y1+y2)*0.4-0.0001 : y2;
        pad++;
        drawPadS(0.7001,y1b,0.9999,y2);
        if(plotType==2) {
          sprintf(title,"%s Z=%f",zVerVsEventForNR[sec][comb]->GetTitle(),
              target(2));
          zVerVsEventForNR[sec][comb]->SetTitle(title);
        }
	    gPad->SetGridx();
	    gPad->SetGridy();
        zVerVsEventForNR[sec][comb]->Draw("colz");
        
        if(drawInOutTracks) {
          pad++;
          drawPadS(0.7001,y1,0.9999,y2b);
          inOutTargetHist->SetLineColor(2);
          inOutTargetHist->Draw();
        }
        padsRow++;
      }
    } else {
      canvsForNR[comb]->cd();
      for(Int_t pad=0;pad<(nPadsPerSector*nPads[comb]); pad++) 
          fPadsForNR[comb][pad]->Modified();
    }
    canvsForNR[comb]->Modified();
    canvsForNR[comb]->Update();
  }
}

void HMdcTargetMon::drawPadS(Double_t x1,Double_t y1,Double_t x2,Double_t y2) {
  Char_t title[20];
  sprintf(title,"pad%i_%02iS",comb,pad);
  currCanvas->cd();
  fPadsForNR[comb][pad] = new TPad(title,title,x1,y1,x2,y2);
  fPadsForNR[comb][pad]->Draw();
  fPadsForNR[comb][pad]->cd();
}

void HMdcTargetMon::setTargPos(Double_t xt, Double_t yt, Double_t zt) {
  useRtdbTarger=kFALSE;
  target.setXYZ(xt,yt,zt);
}

void HMdcTargetMon::setHistPar(Int_t nb, Double_t z1, Double_t z2) {
  nZmBins=nb;
  zmin=z1;
  zmax=z2;
}

void HMdcTargetMon::resetHists(void) {
  for(Int_t i=0;i<6;i++) {
    if(dPhiH[i]) dPhiH[i]->Reset();
    if(r0H[i]) r0H[i]->Reset();
    for(Int_t s=0;s<6;s++) {        
      if(zVerH[i][s]) zVerH[i][s]->Reset();
      if(zVerVsEventH[i][s]) zVerVsEventH[i][s]->Reset();
      for(Int_t k=0;k<10;k++) if(zVerDscH[i][s][k]) zVerDscH[i][s][k]->Reset();
    }
  }
  drawCanvases();
}

void HMdcTargetMon::resetHistsRunsSum(void) {
  for(Int_t i=0;i<6;i++) {
    if(dPhiForNR[i]) dPhiForNR[i]->Reset();
    if(r0ForNR[i]) r0ForNR[i]->Reset();
    for(Int_t s=0;s<6;s++) {        
      if(zVerForNR[i][s]) zVerForNR[i][s]->Reset();
      if(zVerVsEventForNR[i][s]) zVerVsEventForNR[i][s]->Reset();
      for(Int_t k=0;k<10;k++) if(zVerDscForNR[i][s][k]) zVerDscForNR[i][s][k]->Reset();
    }
  }
  drawCanvasesRunsSum();
}

void HMdcTargetMon::addHistsToRunsSum(void) {
  if(collectNRuns<2) return;
  for(Int_t i=0;i<6;i++) {
    if(dPhiH[i]) dPhiForNR[i]->Add(dPhiH[i]);
    if(r0H[i]) r0ForNR[i]->Add(r0H[i]);
    for(Int_t s=0;s<6;s++) {        
      if(zVerH[i][s]) zVerForNR[i][s]->Add(zVerH[i][s]);
      if(zVerVsEventH[i][s]) zVerVsEventForNR[i][s]->Add(zVerVsEventH[i][s]);
      for(Int_t k=0;k<10;k++) 
          if(zVerDscH[i][s][k]) zVerDscForNR[i][s][k]->Add(zVerDscH[i][s][k]);
    }
  }
  drawCanvasesRunsSum();
}

void HMdcTargetMon::makePDF(Char_t* psFile) {
  // Execute ps2pdf GhostScript command on closed PS file, delete PS file
  Char_t command[500];
  sprintf(command,"ps2pdf %s.ps",psFile);
  gSystem->Exec(command);
  sprintf(command,"rm %s.ps",psFile);
  gSystem->Exec(command);
}

