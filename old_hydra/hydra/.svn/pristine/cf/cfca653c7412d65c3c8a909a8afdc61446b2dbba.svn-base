#include "hmdcanroot.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcclussim.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "TH2.h"
#include "TProfile.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include "heventheader.h"
#include "hmdcgetcontainers.h"
#include "../start/hstartunpacker.h"
#include "iostream.h"
#include "TMath.h"
#include "TLine.h"
#include "TStyle.h"
#include "TMarker.h"
#include "hkickplane.h"
//#include "hmdclookuptb.h"
#include "hmdccal1sim.h"
#include "hcategory.h"
#include "TCanvas.h"
#include "TROOT.h"
#include "TTree.h"
#include "hmdcsimeventinf.h"

//*-- Author : V.Pechenov
//*-- Modified : 12.09.99
//*-- Modified : 02.12.99 by V.Pechenov

///////////////////////////////////////////////////////////////
//  HMdcAnRoot
//
// class for analyzing of MDC track finder Dubna
// + event display
//
//  For beam time 09.99 only !!!
///////////////////////////////////////////////////////////////


ClassImp(HMdcAnRoot)


HMdcAnRoot::HMdcAnRoot() {
  fClusCat=0;
  iter=0;
}

HMdcAnRoot::HMdcAnRoot(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  fClusCat=0;
  iter=0;
}

HMdcAnRoot::~HMdcAnRoot() {
  // destructor
}


Bool_t HMdcAnRoot::init(void) {
  
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if(!parCont) return kFALSE; 
  fKick=parCont->getKickPlane(); 

  fClusCat=HMdcGetContainers::getCatMdcClus(kTRUE);
  if (!fClusCat)  return kFALSE;
  isGeant=HMdcGetContainers::isGeant();
  if(isGeant) inform=new HMdcSimEventInf();
  else inform=0;

  iter=(HIterator *)fClusCat->MakeIterator();
  loc.set(2,0,0);
  for(Int_t i=0; i<100; i++) plot[i]=0;
  for(Int_t i=0; i<100; i++) hst[i]=0;
  hitTuple = new TNtuple("hit", "hits", 
      "mom:theta1:phi1:theta2:phi2:xp:yp:zp:xk:yk:zk");
  hst[0] = new TH1F("Theta","Seg.1 Theta",180,0.,180.);
  hst[9] = new TH1F("MomGnKi","Pgeant - Pcluster",500,-1000.0,1000.0);
  hst[10] = new TH1F("Mom_T","Momentum",300,0.0,3000.0);
  hst[11] = new TH1F("Mom_P","Momentum (Poz.)",300,0.0,3000.0);
  hst[12] = new TH1F("Mom_N","Momentum (Neg.)",300,0.0,3000.0);
  plot[0]=new TH2F("dPhdTh1","dTheta vs. dPhi",240,-120.,120.,240,-120., 120.);
//  plot[0]=new TH2F("dPhdTh1","dTheta vs. dPhi",240,-360.,360.,240,-120., 120.);
  plot[1]=new TH2F("dPhdTh2","dPhi vs. Theta",100,0.,100., 240,-120.,120.);
  plot[2]=new TH2F("dPhdTh3","dPhi vs. Phi",100,60.,120., 240,-120.,120.);
  plot[3]=new TH2F("dPhdTh4","dTheta vs. Theta",100,0.,100., 240,-120.,120.);
  plot[4]=new TH2F("dPhdTh5","dTheta vs. Phi",120,60.,120., 240,-120.,120.);
  plot[  9]=new TH2F("p9","Pclusters vs. Pgeant",300,0.0,3000.0,300,0.,3000.);
  plot[ 10]=new TH2F("p10","dTheta vs. momentum",300,0.0,3000.0,300,-90.,+90.);
  plot[ 11]=new TH2F("p11","dPhi vs. momentum",300,0.0,3000.0,300,-90.,+90.); 
  plot[ 20]=new TH2F("p20","Y vs. X",450,-900.,900, 500,300., 1900.);
  
//  plot[0]=new TH2F("dis-tb","dis-tb",160,-0.8,0.8,120,0., 120.);
//  prFile[0]=new TProfile("Prdis-tb","Prdis-tb",160,-0.8,0.8,0., 120.);
 /* plot[0]=new TH2F("dis-tb","dis-tb",160,-0.8,0.8,230,-30., 200.);
  prFile[0]=new TProfile("time-dist","Xtime-Ydist",230,-30., 200., 0.0,0.8);
  prFile[1]=new TProfile("dist-time","dist-time",160,0.0,0.8, -30., 200.);
  
  
  hst[0] = new TH1F("Time1(all)","Time1 (all)",200,-1000.,1000.);*/
  return kTRUE;
}

Int_t HMdcAnRoot::execute(void) {
  HGeomVector pKick;
  for(Int_t sec=0; sec<6; sec++) {  
    iter->Reset();
    HMdcClus *clus=0;
    loc.set(2,sec,1);
    iter->gotoLocation(loc);
     while ((clus=(HMdcClus *)iter->Next())!=0) {
   //    clus->print();
   //    ((HMdcPrPlane *)clus)->print();
       Float_t xt=clus->getXTarg();
       Float_t yt=clus->getYTarg();
       Float_t zt=clus->getZTarg();
       Float_t xp=clus->getX();
       Float_t yp=clus->getY();
       Float_t zp=clus->getZ();
       Float_t th=(acos(zt/sqrt(xt*xt+yt*yt+zt*zt)));
       Float_t ph=(acos(xt/sqrt(xt*xt+yt*yt)));
       Float_t thCl=clus->getTheta();
       Float_t phCl=clus->getPhi();
//       Double_t dTh=th-thCl;
       th=th*180./TMath::Pi();
       ph=ph*180./TMath::Pi();
       thCl=thCl*180./TMath::Pi();
       phCl=phCl*180./TMath::Pi();
       Float_t dPhi= ph-phCl;
       if(dPhi>180.) dPhi=dPhi-360;
//       else if(dPhi<-180.) dPhi=-360.-dPhi;
       Int_t pol=(th-thCl >= 0.) ? +1:-1;
       Double_t err=0.;
       
       pKick.setXYZ(xt,yt,zt);
       Double_t pt=fKick->getPt(pKick,err,pol);
//       Double_t dTh=TMath::ATan2(zt,yt)-TMath::ATan2((zt-clus->getZ()),
//                                                     (yt-clus->getY()));
       
       Double_t dTh=TMath::ATan2(yt,zt)-TMath::ATan2((clus->getY()-yt),
                                                     (clus->getZ()-zt));
       Float_t mom=TMath::Abs(pt/(2.*sin(dTh/2.))); //???????
       hitTuple->Fill(mom,th,ph,thCl,phCl,clus->getX(),clus->getY(),
                       clus->getZ(),xt,yt,zt);   
       // rezol.------------------------------
       if(inform) {
         Int_t nTracks=((HMdcClusSim*)clus)->getNTracks();
         if(nTracks==1) {
           Int_t trn=((HMdcClusSim*)clus)->getTrack(0);
           HMdcSimTrackInf& track=(*inform)[trn];
           if(&track) {
             //-----------------------
             HGeantKine* fKine=track.getKine();
             if(!fKine) continue;
             Float_t apx,apy,apz;
             fKine->getMomentum(apx,apy,apz);
             Float_t momGnt=sqrt(apx*apx+apy*apy+apz*apz);
             hst[9]->Fill(momGnt-mom);
             plot[9]->Fill(momGnt,mom);
//Printf("momGnt=%g, mom=%g",momGnt,mom);
/*             HGeantMdc *fGnMdc1;
             HGeantMdc *fGnMdc2;
             for(Int_t layer=5; layer>=0; layer--) {
              l1=layer;
              fGnMdc1=track.getGeantHit(loc[0],0,layer,nTm);
              if(fGnMdc1 != 0) break;
             }
             for(Int_t layer=0; layer<6; layer++) {
              l2=layer;
              fGnMdc2=track.getGeantHit(loc[0],1,layer,nTm);
              if(fGnMdc2 != 0) break;
              if(fGnMdc1==0 || fGnMdc2==0) continue; //?????
              Float_t x1,y1,x2,y2,tof;
              fGnMdc1->getHit(x1,y1,tof);
              fGnMdc2->getHit(x2,y2,tof);
              HGeomVector vc1(x1*10.,y1*10.,0.); //cm!
              HGeomVector vc2(x2*10.,y2*10.,0.); //cm!
              HGeomVector cross;
              clus->getIntersection(v1,v2-v1,cross);
             } */
             //----------------------
           }
         }
       }
if(TMath::Abs(ph-phCl)>15.) continue;  //!!!!!!!!!!!!
//if(TMath::Abs(th-thCl)>75.) continue;
if(TMath::Abs(th-thCl)>40.) continue;
       hst[0]->Fill(clus->getTheta()*180/TMath::Pi());
       plot[0]->Fill(dPhi,th-thCl);
       plot[1]->Fill(th,dPhi);
       plot[2]->Fill(ph,dPhi);
       plot[3]->Fill(th,dPhi);
       plot[4]->Fill(ph,dPhi);
       plot[20]->Fill(clus->getX(),clus->getY());
       hst[10]->Fill(mom);
       (pol>0) ? hst[11]->Fill(mom) : hst[12]->Fill(mom);
       plot[10]->Fill(mom,th-thCl);
       plot[11]->Fill(mom,dPhi);
     }
  }
  return 0;
}

void HMdcAnRoot::drawProjPlots(Int_t sec) {
/* if( ! &((*fLookUpTb)[sec]) ) return;
 if( ! &((*this)[sec]) ) return;
 TCanvas* canv=(*this)[sec].getCanvasProj();
 if(canv) {
  canv->cd(0);
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  Char_t name[15],title[20];
  sprintf(name,"S%iPlProj",sec+1);
  sprintf(title,"Event %i Sector %i",eventId,sec+1);
  TH2C* plt=lookutsec.fillTH2C(name,title);
  plt->SetXTitle("x, (mm)");
  plt->SetYTitle("y, (mm)");
  plt->SetMinimum(0.5);
  plt->SetMaximum( (maxClusters[sec]<=6) ? 6.3 : 12.5 );
  plt->Draw("COLZ");
  HMdc12Clusters& clusters=lookutsec.getClusters();
  Int_t nclst=clusters.getNClusters();     // Num. of clusters
  TMarker marker;
  marker.SetMarkerStyle(24);
  marker.SetMarkerSize(1.4);
  for(Int_t nc=0; nc<nclst; nc++) {
   marker.SetMarkerColor(qual[sec][nc]+2);
   marker.DrawMarker(clusters[nc].getX(),clusters[nc].getY());
  }
  canv->Modified(); 								
  canv->Update();
 } */
}

void HMdcAnRoot::drawHitsPlots(Int_t sec) {
/* if( ! &((*fLookUpTb)[sec]) ) return;
 if( ! &((*this)[sec]) ) return;
 TCanvas* canv=(*this)[sec].getCanvasHits();
 if(canv) {
  canv->cd(1);
  (*this)[sec].getPlotAllHits()->Draw();
  canv->cd(2);
  (*this)[sec].getPlotSelHits()->Draw();
  canv->Modified(); 								
  canv->Update();
 } */
}

Bool_t HMdcAnRoot::finalize(void) {
// writeHists();
  for(Int_t i=0; i<100; i++) if(plot[i]) plot[i]->Write();
  for(Int_t i=0; i<100; i++) if(hst[i]) hst[i]->Write();
  hitTuple->Write();
//  hst[0]->Draw();
//  plot[0]->Draw();
  return kTRUE;
}
 
