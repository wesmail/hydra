#include "hmdcbeamtest.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hlocation.h"
#include "hmdcsizescells.h"
#include "hgeomvector.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"

//*-- Author : V.Pechenov
//*-- Modified : 12.09.99
//*-- Modified : 02.12.99 by V.Pechenov

///////////////////////////////////////////////////////////////
//  HMdcBeamTest
//
// class for analyzing of MDC track finder Dubna
// + event display
// 
//  For beam time 09.99 only !!!
///////////////////////////////////////////////////////////////

HMdcBeamTest::HMdcBeamTest() {
  fCalCat=0;
  fLookUpTb=0;
  fSizesCells=0;
  iter=0;   
  eventId=0;
  totClust=0;
  cutLeft=cutRight=0.;
}

HMdcBeamTest::HMdcBeamTest(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  fCalCat=0;
  fLookUpTb=0;
  fSizesCells=0;
  iter=0;   
  eventId=0;
  totClust=0;
  cutLeft=cutRight=0.;
}

void HMdcBeamTest::setParContainers() {
  fLookUpTb=(HMdcLookUpTb*)gHades->getRuntimeDb()->getContainer("MdcLookUpTb");
  if(!fLookUpTb) {
   printf("Error: MdcLookUpTb is absent\n");
   return;
  }
  fSizesCells=(HMdcSizesCells*)gHades->getRuntimeDb()->getContainer("MdcSizesCells");
  if (!fSizesCells) {
   printf("Error: MdcSizesCells is absent\n");
   return;
  }
  fGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if (!fGeomPar) {
    printf("Error: MdcGeomPar is absent\n");
    return;
  }
}

Bool_t HMdcBeamTest::init(void) {
  setParContainers();
  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) {
    return kFALSE;
  } 
//  else {
//    if (fCalCat->getClass()!=HMdcCal1Sim::Class()) {
//      Error("HMdcDigitizer::init()","Misconfigurated output category");
//      return kFALSE;
//    }
//  } 

  iter=(HIterator *)fCalCat->MakeIterator();
//  loc.set(4,0,0,0,0);
  
//Hists:
  setHists();
  
  HGeomVector vtmp;
  for(Int_t i=0; i<12; i++) {
   sysLayers[i]=new HGeomTransform();
   Int_t is=getTransfMat(i/6,i%6,sysLayers[i]);        // mod=0
   if(is==0) sysLayers[i]=0;
   vtmp=sysLayers[i]->getTransVector();
   vtmp/=10.;                                 // mm -> cm
   sysLayers[i]->setTransVector(vtmp);  
  }
    
  return kTRUE;
}


Int_t HMdcBeamTest::execute(void) {
  eventId++;
//--------------------------------
//    Bool_t doPlots=kFALSE;
//    Bool_t doPlots=kTRUE;
    Bool_t printClusters=kFALSE;
//    Bool_t printClusters=kTRUE;
//--------------------------------
 
 Int_t nClTot=0;
 
//--------Num. of hits in layers---(hists)-------------------
  iter->Reset();
  Int_t s,m,l,c;
  HMdcCal1* cal=0;
  while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if(s==2 && m==1) {
     if(cal->getNHits() == 2) {
       hist1[21]->Fill(cal->getTime1(),1.);
       hist1[22]->Fill(cal->getTime2(),1.);
       hist1[23]->Fill(cal->getTime1()-cal->getTime2(),1.);
       if( cutLeft < cutRight ) {
       if((cal->getTime1()-cal->getTime2()) < cutLeft ||
          (cal->getTime1()-cal->getTime2()) > cutRight) continue;
      }
     } else  continue; 
     if(l>=0 && l<6) hist1[l]->Fill((Float_t)c,1.);
    }
  }
//-----------------------------------------------------------
 
 
 for(Int_t sec=0; sec<6; sec++){
//  loc[0]=sec; 
  if(&((*fLookUpTb)[sec])==0) continue;
  //--------------------------------------
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  hist1[11]->Fill((Float_t)lookutsec.getNHitsTot()+0.00001,1.);
 //----------------------------------------------
  if(lookutsec.getNHitsTot() < 4) continue;
 //-----------------------------------------------
  coordSys=&(lookutsec.getParPlane());
  HMdc12Clusters& clusters=lookutsec.getClusters();  
  Int_t nclst=clusters.getNClusters();     // Num. of clusters 
  hist1[12]->Fill((Float_t)nclst,1.);
  totClust+=nclst;
 // --- Fill plots:--------------------------------------------------
//  if(doPlots) drawPlot(sec, 1);
  if(nclst>0) {
  for(Int_t nc=0; nc<nclst; nc++) {
   HMdcCluster& clst=clusters[nc];
   Float_t x=clst.getXmid();
   Float_t y=clst.getYmid();
   Float_t z=clst.getZmid();
   plotF[0]->Fill(x,y, 1.0);
   hist1[6]->Fill( atan(sqrt(x*x+y*y)/z)*180./acos(-1.), 1.);
   hist1[7]->Fill( acos(x/sqrt(x*x+y*y))*180./acos(-1.), 1.);
   hist1[31]->Fill(clst.getErrXmid(),1.);
   hist1[32]->Fill(clst.getErrYmid(),1.);
  }
  }
 //-------------------------------------------------------------------

  nClTot+=nclst;
  for(Int_t nc=0; nc<nclst; nc++) {
   HMdcCluster& clst=clusters[nc];
 
   //- - - - - - - - - 
   executeClust(nc,clst);
   //- - - - - - - - -   
  }

 } 
if(printClusters) {
   printf("*************Total %i clasters found:\n",nClTot);
  (*fLookUpTb).printClusters();
}

  return 0;
}

void HMdcBeamTest::printClusters(Int_t sec) {
  if(&((*fLookUpTb)[sec])==0) return; 
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  HMdc12Clusters& clusters=lookutsec.getClusters();
  clusters.print();  
} 

TH1F* HMdcBeamTest::getHist(Int_t n){
 if(n>=0 && n<200) return hist1[n];
 return 0;
}

TH2F* HMdcBeamTest::getPlotF(Int_t n){
 if(n>=0 && n<200) return plotF[n];
 return 0;
}

TH2C* HMdcBeamTest::getPlot(Int_t n){
 // --- Fill plots:--------------------------------------------------
  if(n==1 || n==2) {
   Int_t sec=2;
   if(&((*fLookUpTb)[sec])==0) return 0;
   HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
   HMdc12Clusters& clusters=lookutsec.getClusters();  
   Int_t nclst=clusters.getNClusters();     // Num. of clusters 
   Char_t name[15],title[20];
   sprintf(name,"Ev%3i_Sec%ip%c",eventId,sec+1,0);
   if( name[2] == ' ') name[2]='0';
   if( name[3] == ' ') name[3]='0';
   sprintf(title,"Event %i Sector %i%c",eventId,sec+1,0);
   if(n==1) {
    Float_t mult=(Float_t)totClust/(Float_t)eventId;
    Printf("=== Event %i: %i hits, %i clusters. <Multipliciti>=%itr./%iev.=%f",
    eventId,lookutsec.getNHitsTot(),nclst,totClust,eventId,mult);
//    if(plots[1]) delete plots[1];
    plots[1]=lookutsec.fillTH2C(name,title);
   }
   else {
    name[10]='c';
//    if(plots[2]) delete plots[2];
    if(!plots[2]) {
     plots[2]=new TH2C(name,title,
     lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
     lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
    }
    else {
     plots[2]->Reset();
     plots[2]->SetName(name);
     plots[2]->SetTitle(title);
    }
    plots[2]->SetFillColor(10);
    plots[2]->SetFillStyle(0);
    plots[2]->SetMarkerStyle(24);
    plots[2]->SetMarkerSize(1.4);
    plots[2]->SetMarkerColor(10);
    if(nclst==0) plots[2]->Fill(0.,0.,0);
    for(Int_t nc=0; nc<nclst; nc++) {
     HMdcCluster& clst=clusters[nc];
     plots[2]->Fill(clst.getXmid(),clst.getYmid(), 1.0);
    }
   }
  }
 //-------------------------------------------------------------------
 if(n>=0 && n<200) return plots[n];
 return 0;
}

void HMdcBeamTest::executeClust(Int_t nc, HMdcCluster& clst) {
			      
Int_t q1=clst.getQuality1();
   Int_t q2=clst.getQuality2();
   if(q1>q2) {Int_t qq=q1; q1=q2; q2=qq;}

// Fill hists:-------------------------
 
//     hist1[40]->Fill((Float_t)clst.getErrXmid(),1.);  // System ??? ->
//     hist1[41]->Fill((Float_t)clst.getErrYmid(),1.);

// Calculating crosses track with layers in MDC1&MDC2     
 /*    HGeomVector trP2(clst.getXmid(),clst.getYmid(),clst.getZmid());
     HGeomVector trP1(0.0,0.0,0.0);
     HGeomVector pointCros;
     for(Int_t i=0; i<12; i++) {
      calcCross( &trP1, &trP2, sysLayers[i], &pointCros);
      pointCros=sysLayers[i]->transTo(pointCros);
      
      HGeantMdc *fGnMdc=track.getGeantHit(loc[0],i/6,i%6,1);
      if(fGnMdc == 0) continue;
      Float_t x1,y1,x2,y2,tof;
      fGnMdc->getHit(x1,y1,tof);   
      Float_t dXi=(Float_t)x1-(Float_t)pointCros.getX();
      Float_t dYi=(Float_t)y1-(Float_t)pointCros.getY();
      
      hist1[101+i]->Fill(dXi,1.);
      hist1[121+i]->Fill(dYi,1.); 
     } */
     
/*
   for(loc[1]=0;loc[1]<2;loc[1]++) { 
    for(loc[2]=0;loc[2]<6;loc[2]++) {      // nl - Num. of layers in MDC1+MDC2
     Int_t nLayer=loc[1]*6+loc[2];
     Int_t nWires=clst.getNumHits(nLayer);   // num. fired wires in this clusters for loc
     for(Int_t nW=0; nW<nWires; nW++) {   
      loc[3]=clst.getHit(nLayer,nW);         // num. fired wire
      Int_t nTimes=clst.getTimes(nLayer,nW); // num. times for nWire         
      HMdcCal1Sim* cal=(HMdcCal1Sim*)fCalCat->getSlot(loc);
      if(!cal) {
       Printf("Error: Hit S%i M%i L%i C%i is absent",loc[0],loc[1],loc[2],loc[3]);
      } else {  
      
      HMdcSizesCellsCell& pSCCell=(*fSizesCells)[loc[0]][loc[1]][loc[2]][loc[3]];

//---------------------------------------------------------------      
       Float_t time1=cal->getTime1();
       Int_t nTr1=cal->getNTrack1();
       if(nTimes==2) {  
        Float_t time2=cal->getTime2();     
        Int_t nTr2=cal->getNTrack2();
       }      
//----------------------------------------------------------------
      }
     }     
    }
   }  */
}

void HMdcBeamTest::setHists(void) {
  for(Int_t i=0; i<200; i++) {hist1[i]=0; plots[i]=0; plotF[i]=0;}
  hist1[ 0]=new TH1F("h0","Sec.3 MDC2 Layer 1",200,0.,200.); 
  hist1[ 1]=new TH1F("h1","Sec.3 MDC2 Layer 2",200,0.,200.); 
  hist1[ 2]=new TH1F("h2","Sec.3 MDC2 Layer 3",200,0.,200.);
  hist1[ 3]=new TH1F("h3","Sec.3 MDC2 Layer 4",200,0.,200.); 
  hist1[ 4]=new TH1F("h4","Sec.3 MDC2 Layer 5",200,0.,200.); 
  hist1[ 5]=new TH1F("h5","Sec.3 MDC2 Layer 6",200,0.,200.);
  
  hist1[ 6]=new TH1F("h6","Theta",90,0.,90.);
  hist1[ 7]=new TH1F("h7","Phi",60,60.,120.);
  
  hist1[11]=new TH1F("h11","N.hits/Event",81,-0.5,80.5);
  hist1[12]=new TH1F("h12","N.clusters/Event",11,-0.5,10.5);
//  plotF[11]=new TH2F("pF11","N.hits | N.clusters",40,-0.,40.,10,0.,10.);
  
  hist1[21]=new TH1F("time1","time1",200,0.,2000.);
  hist1[22]=new TH1F("time2","time2",200,0.,2000.);
  hist1[23]=new TH1F("time1-2","time1-2",400,0.,400.);
  
  hist1[31]=new TH1F("h31","dX",100,0.0,2.0);
  hist1[32]=new TH1F("h32","dY",100,0.0,0.5);
  
  
  if(&((*fLookUpTb)[2])){
   HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[2];
   plotF[0] =new TH2F("All_clusters","All clusters",
   lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
   lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
  }
   

//  hist1[ 40]=new TH1F("h40","dX",200,0.,1.);
//  hist1[ 41]=new TH1F("h41","dY",200,0.,1.);
}

void HMdcBeamTest::writeHists(void) {
//  hist1[  0]->Divide(hist1[  1]); delete hist1[  1]; hist1[  1]=0;
//  hist1[ 10]->Divide(hist1[ 11]); delete hist1[ 11]; hist1[ 11]=0;
//  hist1[ 20]->Divide(hist1[ 21]); delete hist1[ 21]; hist1[ 21]=0;
//  hist1[ 50]->Divide(hist1[ 53]);
//  hist1[ 51]->Divide(hist1[ 53]);
//  hist1[ 52]->Divide(hist1[ 53]); delete hist1[ 53]; hist1[ 53]=0;
//Writing hists:
// TFile *fh=new TFile("ttest.root","RECREATE");
 for(Int_t i=0; i<200; i++) if(hist1[i]) hist1[i]->Write();
// fh->Close();
//Write plots:
// TFile *plot=new TFile("plots.root","RECREATE");
 for(Int_t i=0; i<200; i++) if(plots[i]) plots[i]->Write();
// plots[1]->Close();
}

Bool_t HMdcBeamTest::finalize(void) {
// writeHists();
 return kTRUE;
}

Int_t HMdcBeamTest::getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys) {
  HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod); 
  if(!pComVol)  return 0;
  HGeomVolume* pGeomVol=pComVol->getComponent(layer); 
  if(!pGeomVol)  return 0; 
  sys->setTransform(pGeomVol->getTransform());
  sys->transFrom(pComVol->getTransform());
  return 1;
}

void HMdcBeamTest::calcCross( HGeomVector* vc1, HGeomVector* vc2, 
            const HGeomTransform* trans, HGeomVector* vcNew){
// Calculeting a cross of line (vc1-vc2) with plane HGeomTransform
  Double_t x1,x2,y1,y2,z1,z2;
  x1=vc1->getX();   x2=vc2->getX();
  y1=vc1->getY();   y2=vc2->getY();
  z1=vc1->getZ();   z2=vc2->getZ();
  Double_t A,B,C,D;
  A=(trans->getTransVector()).getX();
  B=(trans->getTransVector()).getY();
  C=(trans->getTransVector()).getZ();
  D=-(A*A+B*B+C*C);
  Double_t point[3];
  point[0]=(x1/(x1-x2)*(B*(y1-y2)+C*(z1-z2))-B*y1-C*z1-D)/
           (A+(B*(y1-y2)+C*(z1-z2))/(x1-x2));
  point[1]=(point[0]-x1)*(y1-y2)/(x1-x2)+y1;
  point[2]=(-A*point[0]-B*point[1]-D)/C;
  vcNew->setVector(point);
}


ClassImp(HMdcBeamTest)
