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
#include "hspectrometer.h"
#include "hmdcdef.h"
#include "hevent.h"
#include "hcategory.h"
#include "hgeomvolume.h"
#include "hmdchit.h"
#include "hiterator.h"
#include "TStyle.h"


ClassImp(HMdcTargetMon)

HMdcTargetMon::HMdcTargetMon(Bool_t isClOff, Int_t stp, Text_t *fileName) {
  isCoilOff=isClOff;
  step=stp;
  setParam(fileName);
}

HMdcTargetMon::HMdcTargetMon(Text_t *name,Text_t *title, Bool_t isClOff,
    Int_t stp, Text_t *fileName) : HReconstructor(name,title) {
  isCoilOff=isClOff;
  step=stp;
  setParam(fileName);
}

void HMdcTargetMon::setParam(Text_t *fileName) {
  if(!fileName || strlen(fileName)==0) htFileName=0;
  else {
    htFileName=new Text_t [strlen(fileName)+1];
    for(UInt_t n=0; n<strlen(fileName)+1; n++) htFileName[n]=fileName[n];
  }

  tuple=0;
  if(htFileName) {
    htFile = new TFile(htFileName,"RECREATE",htFileName);
    htFile->cd();
    tuple=0; //??????????????????????????????????????
  } else htFile=0;

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
  
//  for(Int_t i=0; i<1000; i++) hist[i]=0;
  iterMdcHit=0;
  firstEvent=0;
  dateFEvent=0;
  timeFEvent=0;
  dateLEvent=0;
  timeLEvent=0;
  dPhiCut=1.0;
  r0Cut=20.;
}

HMdcTargetMon::~HMdcTargetMon() {
  // destructor

  if(htFile) {
    htFile->cd();
//    for(Int_t i=0; i<1000; i++) if(hist[i] && hist[i]->GetSumOfWeights() != 0)
//        hist[i]->Write();

    if(tuple) {
      tuple->Write();
      delete tuple;
    }
    htFile->Write();
    htFile->Close();
    delete htFile;
  }
  if(htFileName) delete [] htFileName;
//  HMdcGetContainers::deleteCont();
}


Bool_t HMdcTargetMon::init(void) {
//  fGetContainers=HMdcGetContainers::getObject();
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
          zVerH[sec][comb]=new TH1F(name,title,500,-1000.,1000.);
          zVerH[sec][comb]->SetFillColor(3);
          zVerH[sec][comb]->GetXaxis()->SetTitle("Zmin, (mm)");
          sprintf(name,"s%im%iv%i_ZmVev",sec+1,mod1+1,mod2+1);
          sprintf(title,"S.%i M.%i-%i  Z_{min} vs. Nev.",
              sec+1,mod1+1,mod2+1);
          zVerVsEventH[sec][comb]=
              new TH2S(name,title,250,0,step*250,250,-1000.,1000.);
          zVerVsEventH[sec][comb]->SetBit(TH1::kCanRebin);
          zVerVsEventH[sec][comb]->GetYaxis()->SetTitle("Zmin");
          zVerVsEventH[sec][comb]->GetXaxis()->SetTitle("Event");
        }
      }
    }
  }
  return kTRUE;
}

Bool_t HMdcTargetMon::reinit(void) {  
//  if(fSizesCells && !fSizesCells->initContainer()) return kFALSE;
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
//  dateFEvent=evHeader->getDate();
//  timeFEvent=evHeader->getTime();
  return kTRUE;
}

Int_t HMdcTargetMon::execute(void) {
  if(evHeader->getId() != 1) return 0;
  if(firstEvent) {
    extractFileInfo();
    setCanvName();
    drawCanvCut();
  }
  timeLEvent= evHeader->getTime();
  UInt_t nevent=evHeader->getEventSeqNumber();

//  fillDateTime();
//  printf("%s runId=%i  %s\n",fileName.Data(),runId,dateTime.Data());

  for(Int_t n=0; n<24; n++) nhits[n]=0;
  HGeomVector vHit;
  HMdcHit* hit;
  iterMdcHit->Reset();
  while ( (hit=(HMdcHit*)iterMdcHit->Next()) != 0 ){
    Int_t sec=hit->getSector();
    if(mdcInSec[sec]<2) continue;
    Int_t mod=hit->getModule();
    Int_t indx=sec*4+mod;
    if(!listMdc[indx]) continue;
//   if(mod != modF && mod != modS) continue;

    vHit.setXYZ(hit->getX(),hit->getY(),0.);
    vHit=transf[indx]->transFrom(vHit);
    Int_t indx2=nhits[indx];
    x[indx][indx2]=vHit(0);
    y[indx][indx2]=vHit(1);
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
            if(fabs(dPhi)<dPhiCut) {
              //-------------------------------------------
              Double_t dX=x[iM2][hitM2]-x[iM1][hitM1];
              Double_t dY=y[iM2][hitM2]-y[iM1][hitM1];
              Double_t phiTr=TMath::ATan2(dY,dX);
              Double_t r0=y[iM1][hitM1]*cos(phiTr)-x[iM1][hitM1]*sin(phiTr);
              r0H[comb]->Fill(r0);
              //-------------------------------------------
              if(fabs(r0)<r0Cut) {
                Double_t dZ=z[iM2][hitM2]-z[iM1][hitM1];
                Double_t thetaTr=acos(dZ/sqrt(dX*dX+dY*dY+dZ*dZ));
                if( fabs(thetaTr) < 0.01 ) continue;
                Double_t zm=z[iM1][hitM1] - cos(thetaTr)/sin(thetaTr) *
                    (x[iM1][hitM1]*cos(phiTr)+y[iM1][hitM1]*sin(phiTr));
                if(fabs(zm) >1000.) continue; //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
                zVerH[sec][comb]->Fill(zm);
                zVerVsEventH[sec][comb]->Fill(nevent+0.5,zm);
              }
            }
          }
        }
      }
    }
  }
  Int_t n=nevent/500;
  if(nevent-n*500 == 0) drawCanvCut();
  return 0;
}

void HMdcTargetMon::extractFileInfo(void) {
  firstEvent=kFALSE;
  dateFEvent=evHeader->getDate();
  timeFEvent=evHeader->getTime();
  runId=evHeader->getEventRunNumber();
  fileName=gHades->getRuntimeDb()->getCurrentFileName();
  Int_t ind=fileName.Index("/xx");
  if(ind>=0) fileName=fileName.Replace(0,ind+1,"");
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
//   TCanvas *canv = (TCanvas*)gROOT->FindObject(cnCut);
//   if(!canv) {
//     Int_t lenght=900/nPads[comb];
//     if(lenght>400) lenght=300;
//     lenght *= nSectors
//     canv = new TCanvas(cnCut,ctCut,lenght,800);
//     canv->Divide(nSectors,2);
//   }
  fillDateTime();
  for(Int_t comb=0; comb<6; comb++) {
    if(nPads[comb]==0) continue;
    TCanvas *canv = (TCanvas*)gROOT->FindObject(cnComb[comb]);
    if(!canv) {
      Int_t yln=(nPads[comb]>1) ? 800:400;
      canv = new TCanvas(cnComb[comb],ctComb[comb],800,yln);
      canv->Divide(2,nPads[comb]);
      canv->Draw();
    }
    canv->cd();
    Int_t pad=1;
    char title[200];
    for(Int_t sec=0; sec<6; sec++) {
      if(!zVerH[sec][comb]) continue;
      canv->cd(pad++);      
      sprintf(title,"s%i m%i-%i:   %s \n   %s",
         sec+1,combToMdc[comb][0]+1,combToMdc[comb][1]+1,
         fileName.Data(),dateTime.Data());
      zVerH[sec][comb]->SetTitle(title);
      zVerH[sec][comb]->Draw();
      canv->cd(pad++);
      zVerVsEventH[sec][comb]->Draw();
    }
    canv->Modified();
    canv->Update();
  }
}
