#include "hmdctftest.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
//#include "hdetector.h"
#include "hevent.h"
#include "hlocation.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include "hmdcgetcontainers.h"



//*-- Author : V.Pechenov
//*-- Modified: 14.04.99

HMdcTFTest::HMdcTFTest() {
  fCalCat=0;
  setParContainers();
  iter=0;   
}

HMdcTFTest::HMdcTFTest(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  fCalCat=0;
  setParContainers();
  iter=0;    
}

void HMdcTFTest::setParContainers() {
  fLookUpTb  =HMdcLookUpTb::getExObject();
  fSizesCells=HMdcSizesCells::getExObject();
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  fGeomPar=(parCont) ? parCont->getMdcGeomPar() : 0;
}

Bool_t HMdcTFTest::init(void) {
  if( !fLookUpTb || !fSizesCells || !fGeomPar ) return kFALSE;
  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) return kFALSE;
  else {
    if (fCalCat->getClass()!=HMdcCal1Sim::Class()) {
      Error("HMdcDigitizer::init()","Misconfigurated output category");
      return kFALSE;
    }
  } 

  iter=(HIterator *)fCalCat->MakeIterator();
  loc.set(4,0,0,0,0);
  
//Hists:
  setHists();
  
  for(Int_t i=0; i<24; i++) {
   sysLayers[i]=new HGeomTransform();
   Int_t is=getTransfMat(i/6,i%6,sysLayers[i]);        // mm!
   if(is==0) sysLayers[i]=0;
  }
    
  return kTRUE;
}


Int_t HMdcTFTest::execute(void) {
//--------------------------------
    Bool_t doPlots=kFALSE;
//    Bool_t doPlots=kTRUE;
    Bool_t printClusters=kFALSE;
//    Bool_t printClusters=kTRUE;
//--------------------------------

 inform.collectInf();   // Collecting inform.
 inform.getEventVertex(xEvVr,yEvVr,zEvVr); 
 eventId=inform.getEventId();

 hist1[50]->Fill(xEvVr,1.);
 hist1[51]->Fill(yEvVr,1.);
 hist1[52]->Fill(zEvVr,1.); 
 
//if(eventId == 186) return 0;


//-------------------------------------------------------------
// Int_t listTrU[200];
 Int_t nTracksMdcU;
 nTracksMdcU=inform.getNTracks();
//Printf("nTr=%i",nTracksMdcU);
 hist1[80]->Fill(nTracksMdcU,1.);
 for(Int_t nTr=0; nTr<nTracksMdcU; nTr++) {
  Int_t nTrL=inform.getTrackNum(nTr);
  HMdcSimTrackInf& trk=inform[nTrL];
  if(&trk) {
   Short_t nHits=trk.getNGeantHits();
   hist1[81]->Fill(nHits,1.);
//   Short_t nPiaces=trk.getNPiaces();
   Int_t nSec=trk.getNSec();
   hist1[82]->Fill(nSec,1.);
   
//  Printf("n.t.%i Tr.N.%i nHits=%i nPiaces=%i nSec=%i",
//  nTr,nTrL,nHits,nPiaces,nSec);
   for(Int_t nS=0; nS<6; nS++) {
    for(Int_t nM=0; nM<4; nM++) {
     Short_t nTm=trk.getNTimes(nS, nM);
//     if(nTm>0) Printf("s.%i m.%i nT.=%i",nS,nM,nTm);
     if(nTm>0) hist1[83]->Fill(nTm,1.);
    }
   }
  }
  else Printf("Track N%i is absent",nTr);
 }
//-------------------------------------------------------------
 
 
//!!!   //? -ubran otbor trekov & cm-mm!!!
 HGeomVector trB,trE;
 for(Int_t nTr=0; nTr<nTracksMdcU; nTr++) {
   Int_t nTrL=inform.getTrackNum(nTr);
   HMdcSimTrackInf& trk=inform[nTrL];
   if(&trk) {
//trk.printTrack();
//?     if(trk.getNSec() != 1) continue;
//?     if(trk.getNPiaces() != 4) continue;
     HGeantKine* fKine=trk.getKine();
     Float_t mom=-1000.;
     if(!fKine) continue;
     Float_t apx,apy,apz;
     fKine->getMomentum(apx,apy,apz);
     mom=sqrt(apx*apx+apy*apy+apz*apz);
     Int_t par,med,mech;
     fKine->getCreator(par,med,mech);
//     if(par) continue;
     Int_t nnTr,pID;
     fKine->getParticle(nnTr,pID);
//Printf("par=%4i, med=%2i, mech=%3i",par,med,mech);
     for(Int_t nS=0; nS<6; nS++) {
       Short_t nTm=trk.getNTimes(nS, 0);
       if(nTm==0) continue;
//??       if(trk.getQuality(nS,0)!=1 || trk.getQuality(nS,1)!=1 ||
//??          trk.getQuality(nS,2)!=1 || trk.getQuality(nS,3)!=1) break;
       Double_t th[4]={0.,0.,0.,0.};
       Double_t ph[4]={0.,0.,0.,0.};
       Double_t thSg[2]={0.,0.};
       Double_t phSg[2]={0.,0.};
       Double_t thPr=0;
       Double_t phPr=0;
       Double_t dThLLmax=0;
       Double_t dPhLLmax=0;
       Double_t dThMMmax=0;
       Double_t dPhMMmax=0;
       Int_t ntseg[2]={0,0};
       Int_t ntmod[4]={0,0,0,0};
       for(Int_t nM=0; nM<4; nM++) {
         for(Int_t nL=0; nL<6; nL++) {
//?           if(trk.getNTimes(nS,nM,nL) != 1) continue;
           HGeantMdc* hit=trk.getGeantHit(nS,nM,nL,1);
           if(!hit) continue;
           //- - - - - - - - - - 
           Float_t x,y,tof,ptof,theta,phi;
           Float_t vec[3];
           hit->getHit(x,y,tof,ptof);
           vec[0]=x*10.; vec[1]=y*10.; vec[2]=0.; //cm->mm
//?            vec[0]=x; vec[1]=y; vec[2]=0.; //cm->mm
//Printf("s%i m%i l%i x%f y%f t%f p%f",nS,nM,nL,x,y,theta,phi);
           hit->getIncidence(theta,phi);
           trB.setVector(vec);
           vec[2]=cos(theta*TMath::Pi()/180.);
           Float_t lengthXY=sqrt(1.-vec[2]*vec[2]);
           vec[0]+=lengthXY*cos(phi*TMath::Pi()/180.);
           vec[1]+=lengthXY*sin(phi*TMath::Pi()/180.);
           trE.setVector(vec);
           trB=sysLayers[nM*6+nL]->transFrom(trB);
           trE=sysLayers[nM*6+nL]->transFrom(trE);
//if(nS==0) plots[60]->Fill(trB.getX(),trB.getZ());
//if(nS==0) plots[61]->Fill(trB.getY(),trB.getZ());
//if(nS==0) plots[62]->Fill(trB.getX(),trB.getY());
//trE.print();
//trB.print();
           trE-=trB;
//trE.print();
           ntseg[nM/2]++;
           ntmod[nM]++;
           Double_t thN=acos(trE.getZ()/trE.length())*180./TMath::Pi();
           Double_t phN=TMath::ATan2(trE.getY(),trE.getX())*180./TMath::Pi();
           th[nM]+=thN;
           ph[nM]+=phN;
           if(ntmod[nM] > 1) {
             if(nL > 0) {
               hist1[141]->Fill(thN-thPr);
               hist1[142]->Fill(phN-phPr);
               if(dThLLmax<TMath::Abs(thN-thPr)) dThLLmax=TMath::Abs(thN-thPr);
               if(dPhLLmax<TMath::Abs(phN-phPr)) dPhLLmax=TMath::Abs(phN-phPr);
             } else if(nM>0) {
               hist1[151]->Fill(thN-thPr);
               hist1[152]->Fill(phN-phPr);
               if(dThMMmax<TMath::Abs(thN-thPr)) dThMMmax=TMath::Abs(thN-thPr);
               if(dPhMMmax<TMath::Abs(phN-phPr)) dPhMMmax=TMath::Abs(phN-phPr);
             }
           }
           thPr=thN;
           phPr=phN;
           //- - - - - - - - - - 
         }
       }
       thSg[0]=th[0]+th[1];
       thSg[1]=th[2]+th[3];
       phSg[0]=ph[0]+ph[1];
       phSg[1]=ph[2]+ph[3];
       for(Int_t nmd=0; nmd<4; nmd++) {
         if(ntmod[nmd] > 0) {
           th[nmd]/=ntmod[nmd];
           ph[nmd]/=ntmod[nmd];
         }
         else ph[nmd]=th[nmd]=1000.;
       }
       for(Int_t nsg=0; nsg<2; nsg++) {
         if(ntseg[nsg] > 0) {
           thSg[nsg]/=ntseg[nsg];
           phSg[nsg]/=ntseg[nsg];
         }
         else phSg[nsg]=thSg[nsg]=0.;
       }
//Printf("Th:%g %g Phi:%g %g",th[0],th[1],ph[0],ph[1]);
       if(ntmod[1]<=0 || ntmod[2]<=0) continue;
       hist1[140]->Fill(pID+0.5);
       plots[50]->Fill(mom,th[1]-th[2]);
       plots[51]->Fill(mom,ph[1]-ph[2]);
       plots[52]->Fill(ph[1]-ph[2],th[1]-th[2]);
       plots[53]->Fill(th[1],ph[1]-ph[2]);
       plots[54]->Fill(ph[1],ph[1]-ph[2]);
       plots[55]->Fill(th[1],th[1]-th[2]);
       plots[56]->Fill(ph[1],th[1]-th[2]);
       hist1[143]->Fill(th[1]-th[2]);
       hist1[144]->Fill(ph[1]-ph[2]);
       if(pID == 8) hist1[145]->Fill(ph[1]-ph[2]);
       else if(pID == 9) hist1[146]->Fill(ph[1]-ph[2]);
       else if(pID == 14) hist1[147]->Fill(ph[1]-ph[2]);
       if(dThLLmax<0.1 && dPhLLmax<0.1 && dThMMmax<1.0 && dPhMMmax<1.0){
         plots[60]->Fill(mom,th[1]-th[2]);
         plots[61]->Fill(mom,ph[1]-ph[2]); 
         plots[62]->Fill(ph[1]-ph[2],th[1]-th[2]);
         hist1[153]->Fill(th[1]-th[2]);
         hist1[154]->Fill(ph[1]-ph[2]);
         if(pID == 8) hist1[155]->Fill(ph[1]-ph[2]);
         else if(pID == 9) hist1[156]->Fill(ph[1]-ph[2]);
         else if(pID == 14) hist1[157]->Fill(ph[1]-ph[2]);
       }
     }
   }
 }


 for(Int_t sec=0; sec<6; sec++){
  loc[0]=sec; 
  			      
 dens=(Float_t)(inform.getNTotHitsSec(loc[0]))/12.;

 
  //--------------------------------------
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  coordSys=&(lookutsec.getParPlane());
 // --- Fill plots:--------------------------------------------------
  TH2C *plotGeant=0;
  TH2C *plotCluster=0;
  if(doPlots) {
   Char_t name[15],title[20];
   sprintf(name,"Ev%3i_Sec%ip%c",eventId,sec,0);
   if( name[2] == ' ') name[2]='0';
   if( name[3] == ' ') name[3]='0';
   sprintf(title,"Event %i Sector %i%c",eventId,sec,0);
   TH2C *plot=lookutsec.fillTH2C(name,title);
   plot->SetOption("COLZ");
   plot->Write();
   //- - - - - - - - - - - - - - - - 
   name[10]='g';
   plotGeant=new TH2C(name,title,
   lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
   lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
   plotGeant->SetFillColor(10);
   plotGeant->SetFillStyle(0);
   plotGeant->SetMarkerStyle(2);
   plotGeant->SetMarkerSize(1.4);
   plotGeant->SetMarkerColor(10);
   name[10]='c';
   plotCluster=new TH2C(name,title,
   lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
   lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
   plotCluster->SetFillColor(10);
   plotCluster->SetFillStyle(0);
   plotCluster->SetMarkerStyle(24);
   plotCluster->SetMarkerSize(1.4);
   plotCluster->SetMarkerColor(10);
  }
 //-------------------------------------------------------------------
  Int_t listTr[200];
  Int_t nTracksMdc12=inform.getListTracksSeg(sec, 0, listTr);
  Int_t listFoundTr[200];
  Int_t listQualTr[200];
  for(Int_t i=0; i<nTracksMdc12; i++) {listFoundTr[i]=listQualTr[i]=0;} 
  Int_t nGoodTr=0;
  Int_t nVeryGoodTr=0;
  Int_t nTrSameCros=0;
  for(Int_t n=0; n<nTracksMdc12; n++) {
   listQualTr[n]=calcQualtTr( sec, listTr[n]);
   if(listQualTr[n] == 3) nTrSameCros++;
   else if(listQualTr[n]>=1) {
    nGoodTr++;
    if(listQualTr[n] == 1) nVeryGoodTr++;
   }
  }
  
  if(doPlots) {
   for(Int_t n=0; n<nTracksMdc12; n++) {
    if(listQualTr[n] == 1 || listQualTr[n] == 2) {
     HGeomVector hitP;
     calcCrossProj(listTr[n], coordSys, &hitP,1);
     plotGeant->Fill(hitP.getX(),hitP.getY(),1.);
    }
   }
  }

  HMdc12Clusters& clusters=lookutsec.getClusters();  
  Int_t nclst=clusters.getNClusters();     // Num. of clusters 
  if(printClusters) Printf("\nSec.%i Num.Clusters=%i",sec,nclst);
  for(Int_t nc=0; nc<nclst; nc++) {
   HMdcCluster& clst=clusters[nc];
   
   if(doPlots) plotCluster->Fill(clst.getX(),clst.getY(), 1.0);
 
   //- - - - - - - - - Finding tracks in cluster:
   Int_t nTrList[50];
   Int_t nHitsTr[50];
   Int_t nHitsInCl;
   Int_t nTracksClust=analClust(clst, nTrList, nHitsTr, nHitsInCl);
  
   for(Int_t i=0; i<nTracksClust; i++) {
    for(Int_t n=0; n<nTracksMdc12; n++) {
     if(nTrList[i] == listTr[n]) listFoundTr[n]++;
    }
   }
   //- - - - - - - - - 
   if(printClusters) printClust(nc, clst,nTracksClust,nTrList, nHitsTr); 
   //- - - - - - - - - 
   executeClust(nc,clst,nTracksClust,nTrList,  nHitsInCl, nHitsTr);
   //- - - - - - - - -   
   
   
   
   //-------------------
  }
  Int_t nTrF1t[4],nTrNotF[4], nTrFm1t[4],nTotal[4];
  for(Int_t i=0; i<4; i++) nTrF1t[i]=nTrNotF[i]=nTrFm1t[i]=nTotal[i]=0;
  for(Int_t n=0; n<nTracksMdc12; n++) {
   nTotal[listQualTr[n]]++;
   if(listFoundTr[n] == 1) nTrF1t[listQualTr[n]]++;
   else if(listFoundTr[n] > 1)  nTrFm1t[listQualTr[n]]++;
   else nTrNotF[listQualTr[n]]++;
  }
  Float_t effic[4];
  for(Int_t i=0; i<4; i++) {
   if(nTotal[i]==0) effic[i]=0.0;
   else effic[i]=(nTrF1t[i]+nTrFm1t[i])/(Float_t)nTotal[i];
  }

  
if(nTotal[1]>0)  hist1[18]->Fill(dens,(Float_t)nTrNotF[1]/(Float_t)nTotal[1]);
  hist1[19]->Fill(dens,1.);
  
  if(printClusters) {
   Printf("\n-----------------------------------------------------");
   Printf("Quality tracks:    0    1	2    3");
   Printf("Total         :%5i%5i%5i%5i",nTotal[0],nTotal[1],nTotal[2],nTotal[3]);
   Printf("Found  1 times:%5i%5i%5i%5i",nTrF1t[0],nTrF1t[1],nTrF1t[2],nTrF1t[3]);
   printf("Found >1 times:%5i%5i%5i%5i |",nTrFm1t[0],nTrFm1t[1],nTrFm1t[2],nTrFm1t[3]);
   for(Int_t n=0; n<nTracksMdc12; n++) {
    if(listFoundTr[n] > 1) {
     HMdcSimTrackInf& trk=inform[listTr[n]];
     printf(" %i(%i)",listTr[n],trk.getNSimHits(loc[0],0)+trk.getNSimHits(loc[0],1));
    }
   }
   printf("\n");
   printf("Not found	 :%5i%5i%5i%5i |",nTrNotF[0],nTrNotF[1],nTrNotF[2],nTrNotF[3]);
   for(Int_t n=0; n<nTracksMdc12; n++) {
    if(listFoundTr[n] == 0) {
     HMdcSimTrackInf& trk=inform[listTr[n]];
     printf(" %i(%i)",listTr[n],trk.getNSimHits(loc[0],0)+trk.getNSimHits(loc[0],1));
    }
   }
   printf("\n");
   Printf("Efficiency	 : %.2f %.2f %.2f %.2f",effic[0],effic[1],effic[2],effic[3]);
   Printf("\n\n");
  }
 } 

//printf("*************Total %i clasters found:\n",numcl);
//  (*fLookUpTb).printClusters();

  return 0;
}

void HMdcTFTest::executeClust(Int_t nc, HMdcCluster& clst,
                   Int_t nTracksClust, Int_t* nTrList, Int_t nHitsInCl, Int_t* nHitsTr) {
 
 if(nTracksClust>0) {
  if(nTracksClust>1) hist1[10]->Fill(dens,1.);
  hist1[11]->Fill(dens,1.);
 } else hist1[14]->Fill(dens,1.);
 hist1[15]->Fill(dens,1.);

   
   Int_t q1=clst.getQuality1();
   Int_t q2=clst.getQuality2();
   if(q1>q2) {Int_t qq=q1; q1=q2; q2=qq;}
   
   if(q1+q2<=4) hist1[24]->Fill(dens,1.);
   hist1[25]->Fill(dens,1.);

// Fill hists:-------------------------
   if(nTracksClust==1) {                       // Only one track in cluster
    Int_t nTr=nTrList[0];
    if(calcQualtTr(loc[0], nTr) == 1) {  // Track can be recaved
    
    HMdcSimTrackInf& trk=inform[nTr];
    HGeantKine* fKine=trk.getKine();
    Float_t mom=-1000.;
    Float_t apx,apy,apz;
    if(fKine) {
     fKine->getMomentum(apx,apy,apz);
     mom=sqrt(apx*apx+apy*apy+apz*apz);
     hist1[55]->Fill(mom,1.);
    }
    
     HMdcSimTrackInf& track=inform[nTr]; 
        
     Int_t nDigHits=track.getNSimHits(loc[0],0)+track.getNSimHits(loc[0],1);
     
     
     hist1[20]->Fill(dens,(Float_t)nHitsTr[0]/(Float_t)nHitsInCl);
     hist1[21]->Fill(dens,1.);
     hist1[22]->Fill(dens,(Float_t)nHitsTr[0]/(Float_t)nDigHits);
     hist1[23]->Fill(dens,1.);
     
     
     for(Int_t l=0; l<6; l++) {
      hist1[0]->Fill((Float_t)track.getNSimHits(loc[0],0,l)+0.5,1.);
      hist1[0]->Fill((Float_t)track.getNSimHits(loc[0],1,l)+0.5,1.);
      hist1[0]->Fill((Float_t)track.getNSimHits(loc[0],2,l)+0.5,1.);
      hist1[0]->Fill((Float_t)track.getNSimHits(loc[0],3,l)+0.5,1.); 
     }
     hist1[1]->Fill((Float_t)track.getNSimHits(loc[0],0)+0.5,1.);
     hist1[1]->Fill((Float_t)track.getNSimHits(loc[0],1)+0.5,1.);
     hist1[1]->Fill((Float_t)track.getNSimHits(loc[0],2)+0.5,1.);
     hist1[1]->Fill((Float_t)track.getNSimHits(loc[0],3)+0.5,1.);
     
     hist1[2]->Fill((Float_t)track.getNSimHits(loc[0],0)+
                    (Float_t)track.getNSimHits(loc[0],1)+0.5,1.);
    
     HGeomVector clVect(clst.getX(),clst.getY(),clst.getZ());
     clVect=coordSys->transTo(clVect);
     HGeomVector hit;
     calcCrossProj(nTr, coordSys, &hit,1);
     hit=coordSys->transTo(hit);
     Float_t dX=(Float_t)hit.getX()-(Float_t)clVect.getX();
     Float_t dY=(Float_t)hit.getY()-(Float_t)clVect.getY();
     hist1[100]->Fill(dX,1.);
     hist1[120]->Fill(dY,1.);
   if(zEvVr > -0.1) hist1[56]->Fill(dY,1.);
   else  hist1[57]->Fill(dY,1.);
     hist1[40]->Fill((Float_t)clst.getErrX(),1.);  // System ??? ->
     hist1[41]->Fill((Float_t)clst.getErrY(),1.);
     hist1[42]->Fill(dX/(Float_t)clst.getErrX(),1.);  // System ??? ->
     hist1[43]->Fill(dY/(Float_t)clst.getErrY(),1.);

// Calculating crosses track with layers in MDC1&MDC2     
     HGeomVector trP2(clst.getX(),clst.getY(),clst.getZ());
     HGeomVector trP1(0.0,0.0,0.0);
     HGeomVector pointCros;
     for(Int_t i=0; i<12; i++) {
      calcCross( &trP1, &trP2, sysLayers[i], &pointCros);
      pointCros=sysLayers[i]->transTo(pointCros);
      
      HGeantMdc *fGnMdc=track.getGeantHit(loc[0],i/6,i%6,1);
      if(fGnMdc == 0) continue;
      Float_t x1,y1,tof,ptof;
      fGnMdc->getHit(x1,y1,tof,ptof);
      Float_t dXi=(Float_t)x1*10.-(Float_t)pointCros.getX(); // geant: cm->mm
      Float_t dYi=(Float_t)y1*10.-(Float_t)pointCros.getY(); // geant: cm->mm
//?      Float_t dXi=(Float_t)x1-(Float_t)pointCros.getX(); // geant: mm
//?      Float_t dYi=(Float_t)y1-(Float_t)pointCros.getY(); // geant: mm
      
      hist1[101+i]->Fill(dXi,1.);
      hist1[121+i]->Fill(dYi,1.);
      if(i==0) {
       if(zEvVr > -0.1) hist1[58]->Fill(dYi,1.);
       else hist1[59]->Fill(dYi,1.);
      }
      if(i==11) {
       if(zEvVr > -0.1) hist1[60]->Fill(dYi,1.);
       else hist1[61]->Fill(dYi,1.);
      }
     }
     
    }
   }


/*   HGeomVector clV(clst.getX(),clst.getY(),clst.getZ());
   clV=coordSys->transTo(clV);
   if(nTracksClust==1) {
    Int_t nTr=nTrList[0];
    HMdcSimTrackInf& track=inform[nTr];
    track.getNTimes(loc[0],0);

    if(track.getNTimes(loc[0],0) == 1 && track.getNTimes(loc[0],1) == 1) {
     HGeomVector hit;
     calcCrossProj(nTr, coordSys, &hit,1);
     hit=coordSys->transTo(hit);
     Float_t dX=(Float_t)hit.getX()-(Float_t)clV.getX();
     Float_t dY=(Float_t)hit.getY()-(Float_t)clV.getY();
     hist1[30]->Fill(dX,1.);
     hist1[31]->Fill(dY,1.);
     hist1[42]->Fill((Float_t)clst.getErrXmid(),1.);  // System ??? ->
     hist1[43]->Fill((Float_t)clst.getErrYmid(),1.);
     if(q1==1 && q2==1) {
      hist1[32]->Fill(dX,1.);
      hist1[33]->Fill(dY,1.);
     } else if(q1==1 && q2==2) {
      hist1[34]->Fill(dX,1.);
      hist1[35]->Fill(dY,1.);
     } else if(q1==1 && q2==3) {
      hist1[36]->Fill(dX,1.);
      hist1[37]->Fill(dY,1.);
     } else if(q1==2 && q2==2) {
      hist1[38]->Fill(dX,1.);
      hist1[39]->Fill(dY,1.);
     } else {
      hist1[40]->Fill(dX,1.);
      hist1[41]->Fill(dY,1.);
     }
    }
   } */
   for(loc[1]=0;loc[1]<2;loc[1]++) { 
    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
     Int_t nLayer=loc[1]*6+loc[2];
     Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
     for(Int_t nW=0; nW<nWires; nW++) {   
      loc[3]=clst.getHit(nLayer,nW);         // num. fired wire
//      Int_t nTimes=clst.getTimes(nLayer,nW); // num. times for nWire         
      HMdcCal1Sim* cal=(HMdcCal1Sim*)fCalCat->getSlot(loc);
      if(!cal) {
       Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
      } else {  
      
//      HMdcSizesCellsCell& pSCCell=(*fSizesCells)[loc[0]][loc[1]][loc[2]][loc[3]];

//---------------------------------------------------------------      
/*       Float_t time1=cal->getTime1();
       Int_t nTr1=cal->getNTrack1();
       if(nTimes==2) {  
        Float_t time2=cal->getTime2();     
        Int_t nTr2=cal->getNTrack2();
       }  */    
//----------------------------------------------------------------
      }
     }     
    }
   }  
}

void HMdcTFTest::setHists(void) {
  for(Int_t i=0; i<200; i++) {hist1[i]=0; plots[i]=0;}
   hist1[  0]=new TH1F("g0","Num.hits for track in layer  ",5,0.,5.);
   hist1[  1]=new TH1F("g1","Num.hits for track in module ",20,0.,20.);
   hist1[  2]=new TH1F("g2","Num.hits for track in modules 1&2",30,0.,30.); 
  
  hist1[ 10]=new TH1F("h010","<Num. of clusters with >1 tracks/Total num. of clusters>",15,0.,45.);
  hist1[ 11]=new TH1F("h011","Num.of tracks in cluster",15,0.,45.);
  hist1[ 14]=new TH1F("h014","<Num. of fake clusters/Total num. of clusters>",15,0.,45.);
  hist1[ 15]=new TH1F("h015","Num. fake tracks",15,0.,45.);
  hist1[ 18]=new TH1F("h018","<Num. of lost tracks/Total num. of tracks>",15,0.,45.);
  hist1[ 19]=new TH1F("h019","h018",15,0.,45.); 
  
  hist1[ 20]=new TH1F("h020","<Num. of hits from track in cluster/Total num. of hits in cluster>",15,0.,45.);
  hist1[ 21]=new TH1F("h021","dh021",15,0.,45.);
  hist1[ 22]=new TH1F("h022","<Num. of hits from track in cluster/Total num. of hits from track>",15,0.,45.);
  hist1[ 23]=new TH1F("h023","dh023",15,0.,45.);
  hist1[ 24]=new TH1F("h024","<Num. of clusters with good qual./Total num. of clusters>",15,0.,45.);
  hist1[ 25]=new TH1F("h025","dh025",15,0.,45.);
  
  
  hist1[ 40]=new TH1F("h040","dX",50,0.,10.);
  hist1[ 41]=new TH1F("h041","dY",50,0.,5.);
  hist1[ 42]=new TH1F("h042","(Xgeant-Xcluster)/dX",50,0.,2.);
  hist1[ 43]=new TH1F("h043","(Ygeant-Ycluster)/dY",50,0.,4.);
  
  hist1[ 50]=new TH1F("h050","X-ver.",100,-50.,50.);
  hist1[ 51]=new TH1F("h051","Y-ver.",120,-60.,60.);
  hist1[ 52]=new TH1F("h052","Z-ver.",300,-150.,150.);
  
  hist1[ 55]=new TH1F("h055","Momentum",300,0.0,3000.0);
  hist1[ 56]=new TH1F("h056","Zv>0 Ygeant-Ycluster (proj. plane)",100,-50.,50.);
  hist1[ 57]=new TH1F("h057","Zv<0 Ygeant-Ycluster (proj. plane)",100,-50.,50.);
  hist1[ 58]=new TH1F("h058","Zv>0 Ygeant-Ycluster (MDC1,layer 1)",100,-50.,50.);
  hist1[ 59]=new TH1F("h059","Zv<0 Ygeant-Ycluster (MDC1,layer 1)",100,-50.,50.);
  hist1[ 60]=new TH1F("h060","Zv>0 Ygeant-Ycluster (MDC2,layer 6)",100,-50.,50.);
  hist1[ 61]=new TH1F("h061","Zv<0 Ygeant-Ycluster (MDC2,layer 6)",100,-50.,50.);
  
  
  hist1[ 80]=new TH1F("h080","N. of tracks/event",500,0.5,500.5);
  hist1[ 81]=new TH1F("h081","N. of GEANT Hits/track",500,0.5,500.5);
  hist1[ 82]=new TH1F("h082","N. sectors/track",6,0.5,6.5);
  hist1[ 83]=new TH1F("h083","N. times track go through one module",50,0.5,50.5);
  
  hist1[ 100]=new TH1F("h100","Xgeant-Xcluster (proj. plane)",100,-100.,100.);
  hist1[ 101]=new TH1F("h101","Xgeant-Xcluster (MDC1,layer 1)",100,-100.,100.);
  hist1[ 102]=new TH1F("h102","Xgeant-Xcluster (MDC1,layer 2)",100,-100.,100.);
  hist1[ 103]=new TH1F("h103","Xgeant-Xcluster (MDC1,layer 3)",100,-100.,100.);
  hist1[ 104]=new TH1F("h104","Xgeant-Xcluster (MDC1,layer 4)",100,-100.,100.);
  hist1[ 105]=new TH1F("h105","Xgeant-Xcluster (MDC1,layer 5)",100,-100.,100.);
  hist1[ 106]=new TH1F("h106","Xgeant-Xcluster (MDC1,layer 6)",100,-100.,100.);
  hist1[ 107]=new TH1F("h107","Xgeant-Xcluster (MDC2,layer 1)",100,-100.,100.);
  hist1[ 108]=new TH1F("h108","Xgeant-Xcluster (MDC2,layer 2)",100,-100.,100.);
  hist1[ 109]=new TH1F("h109","Xgeant-Xcluster (MDC2,layer 3)",100,-100.,100.);
  hist1[ 110]=new TH1F("h110","Xgeant-Xcluster (MDC2,layer 4)",100,-100.,100.);
  hist1[ 111]=new TH1F("h111","Xgeant-Xcluster (MDC2,layer 5)",100,-100.,100.);
  hist1[ 112]=new TH1F("h112","Xgeant-Xcluster (MDC2,layer 6)",100,-100.,100.);
  hist1[ 120]=new TH1F("h120","Ygeant-Ycluster (proj. plane)",100,-50.,50.);
  hist1[ 121]=new TH1F("h121","Ygeant-Ycluster (MDC1,layer 1)",100,-50.,50.);
  hist1[ 122]=new TH1F("h122","Ygeant-Ycluster (MDC1,layer 2)",100,-50.,50.);
  hist1[ 123]=new TH1F("h123","Ygeant-Ycluster (MDC1,layer 3)",100,-50.,50.);
  hist1[ 124]=new TH1F("h124","Ygeant-Ycluster (MDC1,layer 4)",100,-50.,50.);
  hist1[ 125]=new TH1F("h125","Ygeant-Ycluster (MDC1,layer 5)",100,-50.,50.);
  hist1[ 126]=new TH1F("h126","Ygeant-Ycluster (MDC1,layer 6)",100,-50.,50.);
  hist1[ 127]=new TH1F("h127","Ygeant-Ycluster (MDC2,layer 1)",100,-50.,50.);
  hist1[ 128]=new TH1F("h128","Ygeant-Ycluster (MDC2,layer 2)",100,-50.,50.);
  hist1[ 129]=new TH1F("h129","Ygeant-Ycluster (MDC2,layer 3)",100,-50.,50.);
  hist1[ 130]=new TH1F("h130","Ygeant-Ycluster (MDC2,layer 4)",100,-50.,50.);
  hist1[ 131]=new TH1F("h131","Ygeant-Ycluster (MDC2,layer 5)",100,-50.,50.);
  hist1[ 132]=new TH1F("h132","Ygeant-Ycluster (MDC2,layer 6)",100,-50.,50.);
  
  hist1[ 140]=new TH1F("h140","Part.ID",60,0.,60.);
  hist1[ 141]=new TH1F("h141","dTheta(layer by layer)",800,-2.,2.);
  hist1[ 142]=new TH1F("h142","dPhi(layer by layer)",800,-2.,2.);
  hist1[ 151]=new TH1F("h151","dTheta(mod by mod)",800,-2.,2.);
  hist1[ 152]=new TH1F("h152","dPhi(mod by mod)",800,-2.,2.);
  hist1[ 143]=new TH1F("h143","dTheta",360,-90.,90.);
  hist1[ 144]=new TH1F("h144","dPhi",720,-90.,90.);
  hist1[ 145]=new TH1F("h145","dPhi ID=8,",720,-90.,90.);
  hist1[ 146]=new TH1F("h146","dPhi ID=9,",720,-90.,90.);
  hist1[ 147]=new TH1F("h147","dPhi ID=14,",720,-90.,90.);
  
  
  plots[ 50]=new TH2F("p50","dTheta vs. momentum",300,0.0,3000.0,300,-90.,+90.);
  plots[ 51]=new TH2F("p51","dPhi vs. momentum",300,0.0,3000.0,300,-90.,+90.);
  plots[ 52]=new TH2F("p52","* dTheta vs. dPhi",240,-120.,120.,240,-120., 120.);
  plots[ 53]=new TH2F("p53","dPhi vs. Theta",100,0.,100., 240,-120.,120.);
  plots[ 54]=new TH2F("p54","dPhi vs. Phi",100,60.,120., 240,-120.,120.);
  plots[ 55]=new TH2F("p55","dTheta vs. Theta",100,0.,100., 240,-120.,120.);
  plots[ 56]=new TH2F("p56","dTheta vs. Phi",120,60.,120., 240,-120.,120.);
  
  hist1[ 153]=new TH1F("h153","* dTheta",360,-90.,90.);
  hist1[ 154]=new TH1F("h154","* dPhi",720,-90.,90.);
  hist1[ 155]=new TH1F("h155","* dPhi ID=8,",720,-90.,90.);
  hist1[ 156]=new TH1F("h156","* dPhi ID=9,",720,-90.,90.);
  hist1[ 157]=new TH1F("h157","* dPhi ID=14,",720,-90.,90.);
  
  plots[  60]=new TH2F("p60","* dTheta vs. momentum",300,0.0,3000.0,300,-90.,+90.);
  plots[  61]=new TH2F("p61","* dPhi vs. momentum",300,0.0,3000.0,300,-90.,+90.);
  plots[  62]=new TH2F("p62","* dTheta vs. dPhi",240,-120.,120.,240,-120., 120.);
  
  
//  plots[  60]=new TH2F("p60","x vs. z",400,-500.0,500.0,400,200.,1900.);
//  plots[  61]=new TH2F("p61","y vs. z",400,200.,1700.0,400,200.,1900.);
//  plots[  62]=new TH2F("p61","y vs. x",400,-500.0,500.0,400,200.,1700.);
}

void HMdcTFTest::writeHists(void) {
  hist1[ 10]->Divide(hist1[ 11]); delete hist1[ 11]; hist1[ 11]=0;
  hist1[ 14]->Divide(hist1[ 15]); delete hist1[ 15]; hist1[ 15]=0;
  hist1[ 18]->Divide(hist1[ 19]); delete hist1[ 19]; hist1[ 19]=0;
  hist1[ 20]->Divide(hist1[ 21]); delete hist1[ 21]; hist1[ 21]=0;
  hist1[ 22]->Divide(hist1[ 23]); delete hist1[ 23]; hist1[ 23]=0;
  hist1[ 24]->Divide(hist1[ 25]); delete hist1[ 25]; hist1[ 25]=0; 
  
//Writing hists:
// TFile *fh=new TFile("ttest.root","RECREATE");
  for(Int_t i=0; i<200; i++) if(hist1[i]) hist1[i]->Write();
// fh->Close();
//Write plots:
// TFile *plot=new TFile("plots.root","RECREATE");
  for(Int_t i=0; i<200; i++) if(plots[i]) plots[i]->Write();
// plot->Close();
}

Bool_t HMdcTFTest::finalize(void) {
 writeHists();
 return kTRUE;
}

void HMdcTFTest::printClust(Int_t nc, HMdcCluster& clst, 
      Int_t nTracksClust, Int_t* nTrList, Int_t* nHitsTr ) {
   Printf("\nCluster %i; Q:%i-%i; X=%f+/-%f Y=%f+/-%f Z=%f+/-%f",
            nc,clst.getQuality1(),clst.getQuality2(),
   clst.getX(),clst.getErrX(),
   clst.getY(),clst.getErrY(),
   clst.getZ(),clst.getErrZ());
   Printf("Cluster's window,  LeftUp:   X=%f Y=%f Z=%f",
   clst.getXleft(),clst.getYup(),clst.getZup());
   Printf("                   RightDown X=%f Y=%f Z=%f",
   clst.getXright(),clst.getYdown(),clst.getZdown());
//------------------------------------------
   Printf("%i tracks in clusters nTr[nHits(nHitsTot)]nT_1,2,3,4 x,y,z(in Mdc1, Layer0):",
                                                         nTracksClust);
 for(Int_t i=0; i<nTracksClust; i++) {
  Int_t nTrI=nTrList[i];
  HMdcSimTrackInf& track=inform[nTrI];
  Int_t nDigHits=track.getNSimHits(loc[0],0)+track.getNSimHits(loc[0],1);
  Int_t qMdc1=track.getQuality(loc[0],0);
  Int_t qMdc2=track.getQuality(loc[0],1);
  printf(" %3i[%i(%i)]%i,%i,%i,%i", nTrI,nHitsTr[i],nDigHits,qMdc1,qMdc2,
  track.getQuality(loc[0],2),track.getQuality(loc[0],3));
  Int_t qSeg;
  qMdc1 < qMdc2 ? qSeg=qMdc1 : qSeg=qMdc2;
  if(qSeg > 0) {

   if(track.getQuality(loc[0],0) == 1 &&
     track.getQuality(loc[0],1) == 1) {

    HGeomVector hit;
    calcCrossProj(nTrI, coordSys, &hit,1);

    printf("   x=%f  y=%f z=%f\n", hit.getX(),hit.getY(),hit.getZ());

    for(Int_t i=2; i<=qSeg; i++) {
     calcCrossProj(nTrI, coordSys, &hit,i);
     Printf("                      x=%f  y=%f z=%f", hit.getX(),hit.getY(),hit.getZ());
    }
   }
  }
 }
//-------------------------------------------
   for(loc[1]=0;loc[1]<2;loc[1]++) { 
    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
     Int_t nLayer=loc[1]*6+loc[2];
     Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
      printf("%2i) M%i L%i C(nT)[nTr]:", nLayer,loc[1],loc[2]);
     for(Int_t nW=0; nW<nWires; nW++) {   
      loc[3]=clst.getHit(nLayer,nW);         // num. fired wire
      Int_t nTimes=clst.getTimes(nLayer,nW); // num. times for nWire
      printf(" %i(%i)", loc[3],nTimes);         
      HMdcCal1Sim* cal=(HMdcCal1Sim*)fCalCat->getSlot(loc);
      if(!cal) {
       Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
      } else {  

//---------------------------------------------------------------
       Int_t nTr1=cal->getNTrack1();
       printf("[%i",nTr1);
       if(nTimes==2) {     
        Int_t nTr2=cal->getNTrack2();
        printf(";%i",nTr2);
       }
       printf("]");       
//----------------------------------------------------------------
      }
     }
     printf("\n");     
    }
   } 
}

Int_t HMdcTFTest::analClust(HMdcCluster& clst, Int_t* nTrL,  Int_t* nTrLT, Int_t& nHitsInCl) {
 iter->Reset();
  Int_t nTrCu[12][50][2];
  Int_t nTrLa[12];
  Int_t nTotTrL[50];    
  Int_t nTotTr;
  Int_t nTr;
  nTotTr=0;
  nHitsInCl=0;
  for(Int_t i=0; i<12; i++) {
   nTrLa[i]=0;
   for(Int_t j=0; j<50; j++) nTrCu[i][j][1]=0;
  }
  
  for(loc[1]=0;loc[1]<2;loc[1]++) { 
  for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
   Int_t nLayer=loc[1]*6+loc[2];
   Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
   nHitsInCl=nHitsInCl+nWires;
   for(Int_t nW=0; nW<nWires; nW++) {   
    loc[3]=clst.getHit(nLayer,nW);         // num. fired wire      
    HMdcCal1Sim* cal=(HMdcCal1Sim*)fCalCat->getSlot(loc);
    if(!cal) {
     Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
    } else {  
     for(Int_t nt=0; nt<clst.getTimes(nLayer,nW); nt++) {
      if(nt==0) nTr=cal->getNTrack1();
      else nTr=cal->getNTrack2();
      Int_t ind=nTotTr;
      for(Int_t n=0; n<nTotTr; n++) if(nTotTrL[n]==nTr) {ind=n; break;} 
      if(ind == nTotTr) {nTotTrL[nTotTr]=nTr; nTotTr++;}
      ind=nTrLa[nLayer];
      for(Int_t n=0; n<nTrLa[nLayer]; n++) if(nTrCu[nLayer][n][0]==nTr) {ind=n;break;}
      if(ind==nTrLa[nLayer]) {
       nTrCu[nLayer][ind][0]=nTr;
       nTrCu[nLayer][ind][1]++;
       nTrLa[nLayer]++;
      } else nTrCu[nLayer][ind][1]++;
     }
     
    }
   }
  }
 }
 Int_t nTrCl=0;
 for(Int_t n=0; n<nTotTr; n++) {
  Int_t sum1=0;
  Int_t sum2=0;
  Int_t sumT=0;
  for(Int_t l=0; l<12; l++) {
   for(Int_t i=0; i<nTrLa[l]; i++) {
    if(nTotTrL[n]==nTrCu[l][i][0]) {
     if(l<6) sum1++;
     else sum2++;
     sumT+=nTrCu[l][i][1];
    }
   }
  }
  if(sum1>3 && sum2>3) {
   nTrL[nTrCl]=nTotTrL[n];
   nTrLT[nTrCl]=sumT;
   nTrCl++;
  }
 }
  return nTrCl;
}

void HMdcTFTest::calcCrossProj(Int_t nTr,const HGeomTransform* trans, 
             HGeomVector *cross, Int_t nTm){
// For tracks wich cross mdc1&2 one time only !!!!
  HGeantMdc *fGnMdc1;
  HGeantMdc *fGnMdc2;
  Int_t l1,l2;

  HMdcSimTrackInf& track=inform[nTr];
  for(Int_t layer=5; layer>=0; layer--) {
   l1=layer;
   fGnMdc1=track.getGeantHit(loc[0],0,layer,nTm);
   if(fGnMdc1 != 0) break;
  }
  for(Int_t layer=0; layer<6; layer++) {
   l2=layer;
   fGnMdc2=track.getGeantHit(loc[0],1,layer,nTm);
   if(fGnMdc2 != 0) break;
  }
  if(fGnMdc1==0 || fGnMdc2==0) return; //?????
  Float_t x1,y1,x2,y2,tof,ptof;
  fGnMdc1->getHit(x1,y1,tof,ptof);
  fGnMdc2->getHit(x2,y2,tof,ptof);
  HGeomVector vc1(x1*10.,y1*10.,0.); //cm!
  HGeomVector vc2(x2*10.,y2*10.,0.); //cm!
//?  HGeomVector vc1(x1,y1,0.); //mm!
//?  HGeomVector vc2(x2,y2,0.); //mm!
 
  HGeomTransform trans1,trans2;
  Int_t is=getTransfMat(0,l1,&trans1);        // mm!
  if(is==0) return;
  is=getTransfMat(1, l2, &trans2);             // mm!
  if(is==0) return;
  vc1=trans1.transFrom(vc1);
  vc2=trans2.transFrom(vc2);
  calcCross(&vc1, &vc2, trans, cross);
  return;
}

Int_t HMdcTFTest::getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys) {
  HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod); 
  if(!pComVol)  return 0;
  HGeomVolume* pGeomVol=pComVol->getComponent(layer); 
  if(!pGeomVol)  return 0; 
  sys->setTransform(pGeomVol->getTransform());
  sys->transFrom(pComVol->getTransform());
  return 1;
}

void HMdcTFTest::calcCross( HGeomVector* vc1, HGeomVector* vc2, 
            const HGeomTransform* trans, HGeomVector* vcNew){
// Calculeting a cross of line (vc1-vc2) with plane HGeomTransform
  Double_t x1,x2,y1,y2,z1,z2;
  x1=vc1->getX();   x2=vc2->getX();
  y1=vc1->getY();   y2=vc2->getY();
  z1=vc1->getZ();   z2=vc2->getZ();
  const HGeomRotation& mt=trans->getRotMatrix();
  const HGeomVector& tr=trans->getTransVector();
  Double_t A=(mt(3)*mt(7)-mt(6)*mt(4));
  Double_t B=(mt(6)*mt(1)-mt(0)*mt(7));
  Double_t C=(mt(0)*mt(4)-mt(3)*mt(1));
  Double_t D=-(A*tr(0)+B*tr(1)+C*tr(2));
  Double_t point[3];
  point[0]=(x1*(B*(y1-y2)+C*(z1-z2))-(B*y1+C*z1+D)*(x1-x2))/
           (A*(x1-x2)+B*(y1-y2)+C*(z1-z2));
  point[1]=(y1*C*(z1-z2)-(A*point[0]+C*z1+D)*(y1-y2))/
           (B*(y1-y2)+C*(z1-z2));
  point[2]=(-A*point[0]-B*point[1]-D)/C;
  
  vcNew->setVector(point);
}

Int_t HMdcTFTest::calcQualtTr(Int_t sec, Int_t nTr) {
 HMdcSimTrackInf& trk=inform[nTr];
 if(trk.getQuality(sec, 0) == 0 || trk.getQuality(sec, 1) == 0) return 0;
 HGeantKine* fKine=trk.getKine();
 Float_t xTrVr,yTrVr,zTrVr;
 xTrVr=yTrVr=zTrVr=-1.0e+30;
 if(fKine) fKine->getVertex(xTrVr,yTrVr,zTrVr);
 else Printf("Event %i: The track %i absent in kine-cont.!!!",eventId,nTr);
 if( fabs(xEvVr - xTrVr) >0.1 ) return 0;
 if( fabs(yEvVr - yTrVr) >0.1 ) return 0;
 if( fabs(zEvVr - zTrVr) >0.2 ) return 0;
 if(trk.getQuality(sec,0) > 1 || trk.getQuality(sec,1) > 1) return 3;
 else if(trk.getQuality(sec,2) == 1 && trk.getQuality(sec,3) == 1) return 1;
 return 2;   
}

ClassImp(HMdcTFTest)
