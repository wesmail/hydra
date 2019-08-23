#include "hmdctargetmon.h"
#include "TROOT.h"
#include "TH1.h"
#include "TH2.h"
#include "TNtuple.h"
#include "TCanvas.h"
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
#include <TStyle.h>
#include <TPaveLabel.h>

ClassImp(HMdcTargetMon)

HMdcTargetMon::HMdcTargetMon(Bool_t isClOff, UInt_t pt) {
  isCoilOff=isClOff;
  setParam(pt);
}

HMdcTargetMon::HMdcTargetMon(Text_t *name,Text_t *title, Bool_t isClOff,
    UInt_t pt) :
    HReconstructor(name,title) {
  isCoilOff=isClOff;
  setParam(pt);
}

void HMdcTargetMon::setParam(UInt_t pt) {
  timeStep=10.;
  mode=kFALSE;
  label=0;
  combToMdc[0][0]=0;
  combToMdc[0][1]=1;
  combToMdc[1][0]=0;
  combToMdc[1][1]=2;
  combToMdc[2][0]=0;
  combToMdc[2][1]=3;
  combToMdc[3][0]=1;
  combToMdc[3][1]=2;
  combToMdc[4][0]=1;
  combToMdc[4][1]=3;
  combToMdc[5][0]=2;
  combToMdc[5][1]=3;

  iterMdcHit=0;
  firstEvent=0;
  dateFEvent=0;
  timeFEvent=0;
  dateLEvent=0;
  timeLEvent=0;
  dPhiCut=1.5;
  r0Cut=25.;

  plotType=pt;
  
  nZmBins=900;
  zmin=-1000.;
  zmax=800.;
}

HMdcTargetMon::~HMdcTargetMon() {
  // destructor
}


void HMdcTargetMon::setMode(Bool_t m, Char_t* fileType) {
  mode=m;
  if(fileType==0) typePrintFile="-";
  else typePrintFile=fileType;
}

Bool_t HMdcTargetMon::init(void) {
  fMdcHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (fMdcHitCat) iterMdcHit=(HIterator *)fMdcHitCat->MakeIterator();
  else {
    Error("init","The category HMdcHit is absent");
    return kFALSE;
  }
  firstEvent=kTRUE;

  HMdcGeomPar* fMdcGeomPar=
      (HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if(!fMdcGeomPar) return kFALSE;

  HSpecGeomPar* fSpecGeomPar=
      (HSpecGeomPar*)gHades->getRuntimeDb()->getContainer("SpecGeomPar");
  if(!fSpecGeomPar) return kFALSE;

  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  Int_t* lMods=fMdcDet->getModules();
  Int_t nModsMax=(isCoilOff) ? 4:2;
  nSectors=0;
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
  if(!nSectors) {
    Error("reinit","Numb. of MDC's is not enough for target finding!");
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
  char name[100];
  char title[100];
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
          zVerH[sec][comb]->GetXaxis()->SetTitle(
              "Downsc.Flag:  #color[4]{0}, #color[2]{1}             Zmin, (mm)");
          sprintf(name,"s%im%iv%i_ZmDS0",sec+1,mod1+1,mod2+1);
          sprintf(title,"DownscalingFlag=0");
          zVerDscH[sec][comb][0]=new TH1F(name,title,nZmBins,zmin,zmax);
          zVerDscH[sec][comb][0]->SetFillColor(4);
          zVerDscH[sec][comb][0]->SetLineColor(4);
          zVerDscH[sec][comb][0]->GetXaxis()->SetTitle("Zmin, (mm)");
          sprintf(name,"s%im%iv%i_ZmDS1",sec+1,mod1+1,mod2+1);
          sprintf(title,"DownscalingFlag=1");
          zVerDscH[sec][comb][1]=new TH1F(name,title,nZmBins,zmin,zmax);
          zVerDscH[sec][comb][1]->SetLineColor(2);
          zVerDscH[sec][comb][1]->GetXaxis()->SetTitle("Zmin, (mm)");
          if(plotType==1) {
            sprintf(name,"s%im%iv%i_ZmVev",sec+1,mod1+1,mod2+1);
            sprintf(title,"S.%i M.%i-%i  Z_{min} vs Nev.",sec+1,mod1+1,mod2+1);
            zVerVsEventH[sec][comb]=
              new TH2S(name,title,250,0,1000,250,-1000.,1000.);
            zVerVsEventH[sec][comb]->SetBit(TH1::kCanRebin);
            zVerVsEventH[sec][comb]->GetYaxis()->SetTitle("Zmin");
            zVerVsEventH[sec][comb]->GetXaxis()->SetTitle("Event");
          }  if(plotType==2) {
            sprintf(name,"s%im%iv%i_YvX",sec+1,mod1+1,mod2+1);
            sprintf(title,"S.%i M.%i-%i  Y vs X",sec+1,mod1+1,mod2+1);
            zVerVsEventH[sec][comb]=
              new TH2S(name,title,50,-50.,50.,50,-50.,50.);
            zVerVsEventH[sec][comb]->GetYaxis()->SetTitle("Y (mm)");
            zVerVsEventH[sec][comb]->GetXaxis()->SetTitle("X (mm)");
          } else {
           sprintf(name,"s%im%iv%i_ThVZm",sec+1,mod1+1,mod2+1);
            sprintf(title,"Theta vs Zmin");
            zVerVsEventH[sec][comb]=new TH2S(name,title,100,-150.,50.,80,10,90);
            zVerVsEventH[sec][comb]->GetXaxis()->SetTitle("Zmin");
            zVerVsEventH[sec][comb]->GetYaxis()->SetTitle("Theta");
          }
        }
      }
    }
  }
  return kTRUE;
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
  Int_t nTarg=fSpecGeomPar->getNumTargets()-1;
  target=fSpecGeomPar->getTarget(0)->getTransform().getTransVector()
      +fSpecGeomPar->getTarget(nTarg)->getTransform().getTransVector();
  target /= 2.;
  printf("*** Target: ");
  target.print();

  return kTRUE;
}

Bool_t HMdcTargetMon::finalize(void) {
  drawCanvCut();
  if(typePrintFile.CompareTo("-")!=0) {
    typePrintFile.ToLower();
    for(Int_t comb=0; comb<6; comb++) {
      if(nPads[comb]==0) continue;
      TCanvas *canv = (TCanvas*)gROOT->FindObject(cnComb[comb]);
      if(!canv) continue;
      if(typePrintFile.Length()==0) canv->SaveAs("");
      else {
        if(typePrintFile.Contains(".gif")) canv->SaveAs(".gif");
        if(typePrintFile.Contains(".ps")) canv->SaveAs(".ps");
        if(typePrintFile.Contains(".eps")) canv->SaveAs(".eps");
        if(typePrintFile.Contains(".svg")) canv->SaveAs(".svg");
        if(typePrintFile.Contains(".root")) canv->SaveAs(".root");
      }
    }
  }
  return kTRUE;
}

Int_t HMdcTargetMon::execute(void) {
  if(firstEvent) {
    extractFileInfo();
    setCanvName();
    timer.Reset();
    if(!mode) timer.Start();
  }
  UInt_t downscalingFlag=evHeader->getDownscalingFlag() & 1;
  if(evHeader->getId() != 1) return 0;

  timeLEvent= evHeader->getTime();
  UInt_t nevent=evHeader->getEventSeqNumber();

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
    x[indx][indx2]=vHit(0);//??? - target(0);  // shift Z to target position
    y[indx][indx2]=vHit(1);//??? - target(1);  //
    z[indx][indx2]=vHit(2);
    r2Hit[indx][indx2]=vHit(0)*vHit(0)+vHit(1)*vHit(1);
    vHit -= target;
    Double_t phiN=TMath::ATan2(vHit(1),vHit(0))*180./TMath::Pi();
    if(phiN<0.) phiN+=360.;
    Double_t thetaN=acos(vHit(2)/vHit.length())*180./TMath::Pi();
    phi[indx][indx2]=phiN;
    theta[indx][indx2]=thetaN;
    nhits[indx]++;
  }

  for(Int_t sec=0; sec<6; sec++) {
    if(mdcInSec[sec]<2) continue;
    Int_t iS=sec*4;
    for(Int_t mod1=0; mod1<4; mod1++) {
      Int_t iM1= iS+mod1;
      if(nhits[iM1]==0) continue;
      for(Int_t mod2=mod1+1; mod2<4; mod2++) {
        Int_t iM2= iS+mod2;
        if(nhits[iM2]==0) continue;
        Int_t comb=mod1+mod2;
        if(mod1==0) comb--;
        for(Int_t hitM1=0; hitM1<nhits[iM1]; hitM1++) {
          for(Int_t hitM2=0; hitM2<nhits[iM2]; hitM2++) {
            // r hit in first mdc should be < r hit in second mdc
            if(r2Hit[iM1][hitM1]>=r2Hit[iM2][hitM2]) continue;
            Double_t dPhi=phi[iM1][hitM1]-phi[iM2][hitM2];
            if(dPhi<-180.) dPhi+=360.;
            else if(dPhi>180.) dPhi-=360.;
            dPhiH[comb]->Fill(dPhi);

            //Double_t dTheta=theta[iM1][hitM1]-theta[iM2][hitM2];
            if(fabs(dPhi)>=dPhiCut) continue;
            //-------------------------------------------
            Double_t dX=x[iM2][hitM2]-x[iM1][hitM1];
            Double_t dY=y[iM2][hitM2]-y[iM1][hitM1];
            Double_t phiTr=TMath::ATan2(dY,dX);
            Double_t r0=y[iM1][hitM1]*cos(phiTr)-x[iM1][hitM1]*sin(phiTr);
            r0H[comb]->Fill(r0);
            //-------------------------------------------
            if(fabs(r0)>=r0Cut) continue;
            Double_t dZ=z[iM2][hitM2]-z[iM1][hitM1];
            Double_t thetaTr=acos(dZ/sqrt(dX*dX+dY*dY+dZ*dZ));
            if( fabs(thetaTr) < 0.01 ) continue;
            Double_t zm=z[iM1][hitM1] - cos(thetaTr)/sin(thetaTr) *
                (x[iM1][hitM1]*cos(phiTr)+y[iM1][hitM1]*sin(phiTr));
            // if(fabs(zm) >1000.) continue;
            zVerH[sec][comb]->Fill(zm);
            zVerDscH[sec][comb][downscalingFlag]->Fill(zm);
            if(plotType==1) {
              if(fabs(zm) >1000.) zVerVsEventH[sec][comb]->Fill(nevent+0.5,zm);
            } else if(plotType==2) {
              if(fabs(dZ)>1.e-20 && fabs(zm-target(2))<50.) {
                Double_t tan=dY/dZ;
                Double_t y0=tan*target(2)+y[iM1][hitM1]-tan*z[iM1][hitM1];
                tan=dX/dZ;
                Double_t x0=tan*target(2)+x[iM1][hitM1]-tan*z[iM1][hitM1];
                zVerVsEventH[sec][comb]->Fill(x0,y0);
              }
            } else zVerVsEventH[sec][comb]->Fill(zm,theta[iM2][hitM2]);
          }
        }
      }
    }
  }
  if(!mode) {
     if(timer.CpuTime()>=timeStep) {
       drawCanvCut();
       timer.Reset();
     }
     timer.Start(kFALSE);
   }
  return 0;
}

void HMdcTargetMon::extractFileInfo(void) {
  firstEvent=kFALSE;
  dateFEvent=evHeader->getDate();
  timeFEvent=evHeader->getTime();
  runId=evHeader->getEventRunNumber();
  fileName=gHades->getRuntimeDb()->getCurrentFileName();
  Int_t ind=fileName.Index("/");
  while(ind>=0) {
    fileName=fileName.Replace(0,ind+1,"");
    ind=fileName.Index("/");
  }
  ind=fileName.Index(".");
  if(ind>0) fileName=fileName.Remove(ind);
}

void HMdcTargetMon::fillDateTime(void) {
  UChar_t* dt;
  UChar_t* tm1;
  UChar_t* tm2;
  dt=(UChar_t*)&dateFEvent;
  tm1=(UChar_t*)&timeFEvent;
  tm2=(UChar_t*)&timeLEvent;
  char buf[100];
  sprintf(buf,"%02i-%02i-%4i:  %02i:%02i:%02i - %02i:%02i:%02i\n",
      dt[0],dt[1]+1,1900+dt[2],tm1[2],tm1[1],tm1[0],tm2[2],tm2[1],tm2[0]);
  dateTime=buf;
}

void HMdcTargetMon::setCanvName(void) {
  sprintf(cnCut,"cR%icuts",runId);
  sprintf(ctCut,"Run %s    Cuts",fileName.Data());
  for(Int_t comb=0; comb<6; comb++)
    if(nPads[comb]>0) {
      sprintf(cnComb[comb],"target_%s_M%i%i",fileName.Data(),
          combToMdc[comb][0]+1,combToMdc[comb][1]+1);
      sprintf(ctComb[comb],"Run %s    MDC %i-%i",fileName.Data(),
          combToMdc[comb][0]+1,combToMdc[comb][1]+1);
   }
}

void HMdcTargetMon::drawCanvCut(void) {
  fillDateTime();
  Char_t title[200];
  if(label==0) label=new TPaveLabel(0.1,0.955,0.9,0.995," ");
  sprintf(title,"Run:   #color[4]{%s}       Date:   #color[4]{%s}",
      fileName.Data(),dateTime.Data());
  label->SetLabel(title);
  for(Int_t comb=0; comb<6; comb++) {
    if(nPads[comb]==0) continue;
    TCanvas *canv = (TCanvas*)gROOT->FindObject(cnComb[comb]);
    if(!canv) {
      Int_t yln=(nPads[comb]>1) ? 900:700;
      canv = new TCanvas(cnComb[comb],ctComb[comb],600,yln);
      canv->cd();
      canv->SetFillColor(10);
      label->Draw();
      Double_t dY=0.95/nPads[comb];
      Int_t pad=0;
      for(Int_t sec=0; sec<6; sec++) {
        if(!zVerH[sec][comb]) continue;
        canv->cd();
        Int_t pad2=pad*2;
        sprintf(title,"pad%i_%02i",comb,pad2);
        fPads[comb][pad2]=new TPad(title,title,0.0001,
            dY*(nPads[comb]-pad-1)+0.0001,0.4999,dY*(nPads[comb]-pad)-0.0001);
        fPads[comb][pad2]->Draw();
        fPads[comb][pad2]->cd();
        sprintf(title,"sec.%i mod. %i-%i",sec+1,
            combToMdc[comb][0]+1,combToMdc[comb][1]+1);
        zVerH[sec][comb]->SetTitle(title);
        zVerH[sec][comb]->Draw();
        zVerDscH[sec][comb][0]->Draw("same");
        zVerDscH[sec][comb][1]->Draw("same");
        pad2++;
        canv->cd();
        sprintf(title,"pad%i_%02i",comb,pad2);
        fPads[comb][pad2]=new TPad(title,title,0.5001,
            dY*(nPads[comb]-pad-1)+0.0001,0.9999,dY*(nPads[comb]-pad)-0.0001);
        fPads[comb][pad2]->Draw();
        fPads[comb][pad2]->cd();
        if(plotType==2) {
          sprintf(title,"%s Z=%f",zVerVsEventH[sec][comb]->GetTitle(),target(2));
          zVerVsEventH[sec][comb]->SetTitle(title);
        }
        zVerVsEventH[sec][comb]->Draw("colz");
        pad++;
      }
    } else {
      canv->cd();
      for(Int_t pad=0;pad<(2*nPads[comb]); pad++) fPads[comb][pad]->Modified();
    }
    canv->Modified();
    canv->Update();
  }
}
