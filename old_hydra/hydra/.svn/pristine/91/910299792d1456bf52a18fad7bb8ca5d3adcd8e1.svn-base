#include "hmdctof.h"
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
#include "heventheader.h"

//*-- Author : V.Pechenov
//*-- Modified: 12.09.99

///////////////////////////////////////////////////////////////
//  HMdcTof
//
// class for analyzing of MDC track finder Dubna
// + event display
// 
//  For beam time 09.99 only !!!
///////////////////////////////////////////////////////////////

HMdcTof::HMdcTof() {
  fCalCat=0;
  fTofHitCat=0;
  fLookUpTb=0;
  fSizesCells=0;
  iter=0;   
  eventId=0;
  totClust=0;
}

HMdcTof::HMdcTof(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  fCalCat=0;
  fLookUpTb=0;
  fTofHitCat=0;
  fSizesCells=0;
  iter=0;   
  eventId=0;
  totClust=0;

//-------------------------  
  FILE *fpt = fopen("tof_mdc.txt","r");
  Float_t A,B,uuu;
  Int_t Md,St;
  Int_t ncol;
 while(1) {
  ncol = fscanf(fpt,"%i%i%f%f%f",&Md,&St,&B,&A,&uuu);
  if(ncol<0) break;
  tmA[(Md-1)*8+St-1]=A;
  tmB[(Md-1)*8+St-1]=B;
 }
 if(fpt) fclose(fpt);
//-----------------------------
}

void HMdcTof::setParContainers() {
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

Bool_t HMdcTof::init(void) {
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
  
  fTofHitCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fTofHitCat) {
    return kFALSE;
  } 
  tofIter=(HIterator *)fTofHitCat->MakeIterator();
  
//Hists:
  setHists();
  
  for(Int_t i=0; i<12; i++) {
   sysLayers[i]=new HGeomTransform();
   Int_t is=getTransfMat(i/6,i%6,sysLayers[i]);        // mm!
   if(is==0) sysLayers[i]=0;
  }
  
  //==Efficiency=================
  nClust6=nClust5=nSingHit=nDublHit=0;
  for(Int_t i=0; i<6; i++) nClust5l[i]=nSingHitL[i]=nDublHitL[i]=0;
  //==============================
    
  return kTRUE;
}


Int_t HMdcTof::execute(void) {
/*  UInt_t id= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getId();
  UInt_t eSN=((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getEventSeqNumber();
  UInt_t eD= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getDate();
  UInt_t eT= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getTime();
  UInt_t eFN= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getEventFileNumber();
  UInt_t ePd= ((HEventHeader*) gHades->getCurrentEvent()->getHeader())->getEventPad();
  
//  if(id == 1) return 0;
Printf("----Id=%i  %i %i %i %i %i",id,eSN,eD,eT,eFN,ePd);
*/
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
  Int_t nHitsMdc[6];
  for(Int_t i=0; i<6; i++) nHitsMdc[i]=0;
  while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if(s==2 && m==1) {
//     if(cal->getNHits() == 2 ) {
      hist1[21]->Fill(cal->getTime1(),1.);
      hist1[22]->Fill(cal->getTime2(),1.);
      hist1[23]->Fill(cal->getTime1()-cal->getTime2(),1.);
      if(cutLeft<cutRight) {
       if((cal->getTime1()-cal->getTime2()) < cutLeft ||
          (cal->getTime1()-cal->getTime2()) > cutRight ) continue;
      }
//     } else  continue; 
     if(l>=0 && l<6) hist1[l]->Fill((Float_t)c,1.);
     nHitsMdc[s]++;
    }
  }
//-----------------------------------------------------------

//==================TOF================================
 nTof=0;
 Float_t xTofP[80];
 Float_t yTofP[80];
//=====================================================
 
 
 
 for(Int_t sec=0; sec<6; sec++){
//  loc[0]=sec; 
  if(&((*fLookUpTb)[sec])==0) continue;
  //--------------------------------------
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
 //----------------------------------------------
  if(lookutsec.getNHitsTot() < 4) continue;
 //-----------------------------------------------
  coordSys=&(lookutsec.getParPlane());
  HMdc12Clusters& clusters=lookutsec.getClusters();  
  Int_t nclst=clusters.getNClusters();     // Num. of clusters 
  totClust+=nclst;
 // --- Fill plots:--------------------------------------------------
//  if(doPlots) drawPlot(sec, 1);
  Float_t xMdc[500];
  Float_t yMdc[500];
  Int_t nclstT=0;
  if(nclst>0) {
  for(Int_t nc=0; nc<nclst; nc++) {
   HMdcCluster& clst=clusters[nc];
   Float_t x=clst.getX();
   Float_t y=clst.getY();
   Float_t z=clst.getZ();
   if(y>45.) {
    xMdc[nclstT]=x;
    yMdc[nclstT]=y;
    nclstT++;
    hist1[6]->Fill( atan(sqrt(x*x+y*y)/z)*180./acos(-1.), 1.);
    hist1[7]->Fill( acos(x/sqrt(x*x+y*y))*180./acos(-1.), 1.); 
   }
   plotF[0]->Fill(x,y, 1.0);
   //==Efficiency=================
//   if(nHitsMdc[2] <50) {
   if(nclst <=3) {
   Int_t numLayers=0;
   Int_t layer0nm=-1;
   Int_t indQ=1;
   for(Int_t l=6; l<12; l++) {
    Int_t nH=clst.getNumHits(l);
    if(nH>2) indQ=0;
    if(clst.getNumHits(l)>0) numLayers++;
    else layer0nm=l-6;
   }
   if(indQ==1 && numLayers>4) {
    if(numLayers==6) nClust6++;
    else {
     nClust5++;
     nClust5l[layer0nm]++;
    }
    for(Int_t l=6; l<12; l++) {
     if(clst.getNumHits(l) ==1) {
      nSingHit++;
      nSingHitL[l-6]++;
     }
     else if(clst.getNumHits(l) ==2) {
      nDublHit++;
      nDublHitL[l-6]++;
     }
    }
   }
   }
   //============================== 
  }
  }
  //==================TOF================================
  tofIter->Reset();
  HTofHit* tofHit=0;
  while ((tofHit=(HTofHit *)tofIter->Next())!=0) {
//   Int_t indTof=tofHit->getIndex();
//   if(indTof == -1) continue;
    if(tofHit->getSector() != 2) continue;
     Char_t cellT=tofHit->getCell();
     Char_t modT=tofHit->getModule();
     Float_t tof=tofHit->getTof();
     Float_t xPos=tofHit->getXpos();
                                           if (xPos==0.0) continue;
//Printf("TOF: m.%i c.%i x=%f",modT,cellT,xPos);
     xTofP[nTof]=xPos;
     Int_t iyPos=8*modT+cellT;
     yTofP[nTof]=iyPos;
 
     Float_t yPos=iyPos;

//&&&&new&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
/*     if(yPos<=31.5) {
     yPos=3*yPos+1.5            -0.8663;
    } else {
     yPos=96.+2*(yPos-32)+1.    -0.8663;
    }
    
    yPos=((200-yPos)*0.5)/2.35+41.7-0.5 ;
    yPos=yPos+0.47;
    
//    yPos=-(yPos-393.5)/4.608;
    xPos=(xPos-tmB[iyPos])/tmA[iyPos]; */
//&&&&old&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&& 
/*     xPos=-xPos*0.35+1.9;
     if(iyPos > 31) {
      yPos=96+2*(yPos-33)+1  -1.25+1.5;
      xPos=xPos-0.979;
     }
     else {
      yPos=3*(yPos-1)+1.5    +1.5;
      xPos=xPos+0.231;
     }
 
     yPos=((200-yPos)*0.5)/2.35+41.7-0.5; */
//=======TOF GEANT================
    if(yPos>31.5) {
     yPos=96+2*(yPos-33);
     yPos=-0.2057*yPos+82.2;
     xPos=-xPos*0.3339-0.4115+0.49;
    } else {
     yPos=3*(yPos-1)*1.05+1.5    +1.5;
     yPos=-0.214*yPos+83.77;
     xPos=-xPos*0.3437-0.791+0.8;
    }
  
 
     xTof[nTof]=xPos;
     yTof[nTof]=yPos;


     HGeomVector tof1(xPos,yPos,0.);
     HGeomVector tof2(xPos,yPos,200.);
     HGeomVector pointCrosTof;
 
     calcCross( &tof1, &tof2, coordSys, &pointCrosTof);
 
     plotF[1]->Fill(xPos,yPos,1.);
     hist1[8]->Fill( atan(sqrt(xPos*xPos+yPos*yPos)/pointCrosTof.getZ())*180./acos(-1.), 1.);
     hist1[9]->Fill( acos(xPos/sqrt(xPos*xPos+yPos*yPos))*180./acos(-1.), 1.);
 
 
     nTof++;
 
//    Printf("TOF: %i ind=%i tof=%f x=%f y=%f",
//    eventId,indTof,tof,xPos,yPos);
 }

   Int_t indMdc[100];
   for(Int_t i=0; i<nTof; i++) indMdc[i]=-1;
  //------------------------------------ 
   if(nclstT==1 && nTof==1) indMdc[0]=0;
   else if(nclstT>1 && nTof==1) {
    indMdc[0]=0;
    Float_t distMin=sqrt((xTof[0]-xMdc[0])*(xTof[0]-xMdc[0])/100+
                         (yTof[0]-yMdc[0])*(yTof[0]-yMdc[0]));
    for(Int_t nc=1; nc<nclstT; nc++) {
     Float_t distMin2=sqrt((xTof[0]-xMdc[nc])*(xTof[0]-xMdc[nc])/100+
                         (yTof[0]-yMdc[nc])*(yTof[0]-yMdc[nc]));
     if(distMin2<distMin) {
      indMdc[0]=nc;
      distMin=distMin2;
     }
    }
   }
   else if(nclstT==1 && nTof>1) {
    Int_t indTofS=0;
    Float_t distMin=sqrt((xTof[0]-xMdc[0])*(xTof[0]-xMdc[0])/100+
                         (yTof[0]-yMdc[0])*(yTof[0]-yMdc[0]));
    for(Int_t nc=1; nc<nTof; nc++) {
     Float_t distMin2=sqrt((xTof[nc]-xMdc[0])*(xTof[nc]-xMdc[0])/100+
                         (yTof[nc]-yMdc[0])*(yTof[nc]-yMdc[0]));
     if(distMin2<distMin) {
      indTofS=nc;
      distMin=distMin2;
     }
    }
    indMdc[indTofS]=0;
   }
   else if(nclstT>1 && nTof>1) {
    for(Int_t nt=0; nt<nTof; nt++) {
     Float_t distMin=sqrt((xTof[nt]-xMdc[0])*(xTof[nt]-xMdc[0])/100+
                         (yTof[nt]-yMdc[0])*(yTof[nt]-yMdc[0]));
     if(distMin<2.) indMdc[nt]=0;
     for(Int_t nc=1; nc<nclstT; nc++) {
      Float_t distMin2=sqrt((xTof[nt]-xMdc[nc])*(xTof[nt]-xMdc[nc])/100+
                         (yTof[nt]-yMdc[nc])*(yTof[nt]-yMdc[nc]));
      if(distMin2<distMin) {
       if(distMin<2.) indMdc[nt]=nc;
       distMin=distMin2;
      }
     }
    }
   }
//Printf("nTof=%i, nMdc=%i",nTof,nclstT);
   for(Int_t nt=0; nt<nTof; nt++) {
     if(indMdc[nt] >= 0) {
if(nTof==1 || nclstT==1) {
      plotF[100]->Fill(xTof[nt],xMdc[indMdc[nt]],1.);
      plotF[101]->Fill(yTof[nt],yMdc[indMdc[nt]],1.);
      hist1[100]->Fill(xTof[nt]-xMdc[indMdc[nt]],1.);
      hist1[101]->Fill(yTof[nt]-yMdc[indMdc[nt]],1.);
      Printf("MT: %f %f %f %f",xTofP[nt],yTofP[nt],xMdc[indMdc[nt]],yMdc[indMdc[nt]]);
}
     }
    }
//============================================================ 
 //-------------------------------------------------------------------

  nClTot+=nclst;
  for(Int_t nc=0; nc<nclst; nc++) {
   HMdcCluster& clst=clusters[nc];
 
   //- - - - - - - - - 
   executeClust(nc,clst);
   //- - - - - - - - -   
  }
//==Efficiency=================
/*
  Float_t erC5=sqrt((Float_t)nClust5);
  Float_t erC6=sqrt((Float_t)nClust6);
  Float_t erSH=sqrt((Float_t)nSingHit);
  Float_t erDH=sqrt((Float_t)nDublHit);
  
  Float_t effK=((Float_t)nClust5)/((Float_t)nClust6);
  Float_t dKdC5=1./((Float_t)nClust6);
  Float_t dKdC6=-((Float_t)nClust5)/(((Float_t)nClust6)*((Float_t)nClust6));
  
  Float_t effS=((Float_t)nSingHit)/((Float_t)nDublHit);
  Float_t dSdSH=1./((Float_t)nDublHit);
  Float_t dSdDH=-((Float_t)nSingHit)/(((Float_t)nDublHit)*((Float_t)nDublHit));
  
  Float_t effA=(effS+1.)*(effK+6.);
   Float_t dAdC5=(effS+1.)*dKdC5;
   Float_t dAdC6=(effS+1.)*dKdC6;
   Float_t dAdSH=dSdSH*(effK+6.);
   Float_t dAdDH=dSdDH*(effK+6.);
  Float_t effB=(12+6*effS)/effA;
   Float_t dBdSH=6*dSdSH/effA-(12+6*effS)*dAdSH/(effA*effA);
   Float_t dBdDH=6*dSdDH/effA-(12+6*effS)*dAdDH/(effA*effA);
   Float_t dBdC5=-(12+6*effS)/(effA*effA)*dAdC5;
   Float_t dBdC6=-(12+6*effS)/(effA*effA)*dAdC6;
  Float_t effC=6./effA;
   Float_t dCdSH=-6./(effA*effA)*dAdSH;
   Float_t dCdDH=-6./(effA*effA)*dAdDH;
   Float_t dCdC5=-6./(effA*effA)*dAdC5;
   Float_t dCdC6=-6./(effA*effA)*dAdC6;
  Float_t sqV=sqrt(effB*effB-4*effC);
  Float_t eff1=0.5*(effB+sqV);
   Float_t dEdSH=0.5*(dBdSH+0.5/sqV*(2.*effB*dBdSH-4.*dCdSH)) *erSH;
   Float_t dEdDH=0.5*(dBdDH+0.5/sqV*(2.*effB*dBdSH-4.*dCdDH)) *erDH;
   Float_t dEdC5=0.5*(dBdC5+0.5/sqV*(2.*effB*dBdC5-4.*dCdC5)) *erC5;
   Float_t dEdC6=0.5*(dBdC6+0.5/sqV*(2.*effB*dBdC6-4.*dCdC6)) *erC6;
  Float_t erEf=sqrt(dEdSH*dEdSH+dEdDH*dEdDH+dEdC5*dEdC5+dEdC6*dEdC6);
//  Float_t eff2=0.5*(effB-sqrt(effB*effB-4*effC));
  Float_t effSnDu=effS*eff1-2.*(1.-eff1);
  effSnDu=effSnDu/(1.+effSnDu);
  Printf("Eff.: eff=%f+/-%f, 6/5=%f 5cl.=%i 6cl.=%i",
  eff1,erEf,effSnDu,nClust5,nClust6);
  Float_t n6n5L[6], nSnDbL[6];
  for (Int_t i=0; i<6; i++) {
   effK=((Float_t)nClust5l[i])/((Float_t)nClust6);
   effS=((Float_t)nSingHitL[i])/((Float_t)nDublHitL[i]);
    erC5=sqrt((Float_t)nClust5l[i]);
    erC6=sqrt((Float_t)nClust6);
    erSH=sqrt((Float_t)nSingHitL[i]);
    erDH=sqrt((Float_t)nDublHitL[i]);
   effK=effK*6.;
    Float_t dKdC5=6./((Float_t)nClust6);
    Float_t dKdC6=-6.*((Float_t)nClust5l[i])/(((Float_t)nClust6)*((Float_t)nClust6));
   effA=(effS+1.)*(effK+6.);
    dAdC5=(effS+1.)*dKdC5;
    dAdC6=(effS+1.)*dKdC6;
    dAdSH=dSdSH*(effK+6.);
    dAdDH=dSdDH*(effK+6.);
   effB=(12+6*effS)/effA;
    dBdSH=6*dSdSH/effA-(12+6*effS)*dAdSH/(effA*effA);
    dBdDH=6*dSdDH/effA-(12+6*effS)*dAdDH/(effA*effA);
    dBdC5=-(12+6*effS)/(effA*effA)*dAdC5;
    dBdC6=-(12+6*effS)/(effA*effA)*dAdC6;
   effC=6./effA;
    dCdSH=-6./(effA*effA)*dAdSH;
    dCdDH=-6./(effA*effA)*dAdDH;
    dCdC5=-6./(effA*effA)*dAdC5;
    dCdC6=-6./(effA*effA)*dAdC6;
   sqV=sqrt(effB*effB-4*effC);
   eff1=0.5*(effB+sqV);
    dEdSH=0.5*(dBdSH+0.5/sqV*(2.*effB*dBdSH-4.*dCdSH)) *erSH;
    dEdDH=0.5*(dBdDH+0.5/sqV*(2.*effB*dBdSH-4.*dCdDH)) *erDH;
    dEdC5=0.5*(dBdC5+0.5/sqV*(2.*effB*dBdC5-4.*dCdC5)) *erC5;
    dEdC6=0.5*(dBdC6+0.5/sqV*(2.*effB*dBdC6-4.*dCdC6)) *erC6;
   erEf=sqrt(dEdSH*dEdSH+dEdDH*dEdDH+dEdC5*dEdC5+dEdC6*dEdC6);
//   eff2=0.5*(effB-sqrt(effB*effB-4*effC));
   effSnDu=effS*eff1-2.*(1.-eff1);
   effSnDu=effSnDu/(1.+effSnDu);
   Printf("%i) eff1=%f+/-%f, Sing.=%f",i,eff1,erEf,effSnDu);
  }
*/
//==============================
 } 
if(printClusters) {
   printf("*************Total %i clasters found:\n",nClTot);
  (*fLookUpTb).printClusters();
}

  return 0;
}

void HMdcTof::printClusters(Int_t sec) {
  if(&((*fLookUpTb)[sec])==0) return; 
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  HMdc12Clusters& clusters=lookutsec.getClusters();
  clusters.print();  
} 

TH1F* HMdcTof::getHist(Int_t n){
 if(n>=0 && n<200) return hist1[n];
 return 0;
}

TH2F* HMdcTof::getPlotF(Int_t n){
 if(n>=0 && n<200) return plotF[n];
 return 0;
}

TH2C* HMdcTof::getPlot(Int_t n){
 // --- Fill plots:--------------------------------------------------
  if(n==1 || n==2 || n==3) {
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
   else if(n==2) {
    name[10]='c';
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
     plots[2]->Fill(clst.getX(),clst.getY(), 1.0);
    }
   }
   else {
    name[10]='t';
    if(!plots[3]) {
     plots[3]=new TH2C(name,title,
     lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
     lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
    }
    else {
     plots[3]->Reset();
     plots[3]->SetName(name);
     plots[3]->SetTitle(title);
    }
    plots[3]->SetFillColor(10);
    plots[3]->SetFillStyle(0);
    plots[3]->SetMarkerStyle(25);
    plots[3]->SetMarkerSize(1.4);
    plots[3]->SetMarkerColor(10);
    if(nTof==0) plots[3]->Fill(0.,0.,0);
    for(Int_t nc=0; nc<nTof; nc++) {
     plots[3]->Fill(xTof[nc],yTof[nc], 1.0);
    }
   }
  }
 //-------------------------------------------------------------------
 if(n>=0 && n<200) return plots[n];
 return 0;
}

void HMdcTof::executeClust(Int_t nc, HMdcCluster& clst) {
			      
Int_t q1=clst.getQuality1();
   Int_t q2=clst.getQuality2();
   if(q1>q2) {Int_t qq=q1; q1=q2; q2=qq;}

// Fill hists:-------------------------
 
//     hist1[40]->Fill((Float_t)clst.getErrX(),1.);  // System ??? ->
//     hist1[41]->Fill((Float_t)clst.getErrY(),1.);

// Calculating crosses track with layers in MDC1&MDC2     
 /*    HGeomVector trP2(clst.getX(),clst.getY(),clst.getZ());
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

void HMdcTof::setHists(void) {
  for(Int_t i=0; i<200; i++) {hist1[i]=0; plots[i]=0; plotF[i]=0;}
  hist1[ 0]=new TH1F("h0","Sec.3 MDC2 Layer 1",200,0.,200.); 
  hist1[ 1]=new TH1F("h1","Sec.3 MDC2 Layer 2",200,0.,200.); 
  hist1[ 2]=new TH1F("h2","Sec.3 MDC2 Layer 3",200,0.,200.);
  hist1[ 3]=new TH1F("h3","Sec.3 MDC2 Layer 4",200,0.,200.); 
  hist1[ 4]=new TH1F("h4","Sec.3 MDC2 Layer 5",200,0.,200.); 
  hist1[ 5]=new TH1F("h5","Sec.3 MDC2 Layer 6",200,0.,200.);
  
  hist1[ 6]=new TH1F("h6","Theta (MDC)",90,0.,90.);
  hist1[ 7]=new TH1F("h7","Phi (MDC)",60,60.,120.);
  hist1[ 8]=new TH1F("h6","Theta (TOF)",90,0.,90.);
  hist1[ 9]=new TH1F("h7","Phi (TOF)",60,60.,120.);
  
  hist1[21]=new TH1F("time1","time1",200,0.,2000.);
  hist1[22]=new TH1F("time2","time2",200,0.,2000.);
  hist1[23]=new TH1F("time1-2","time1-2",400,0.,400.);
  
  
  plotF[100]=new TH2F("mdctof1","x(tof-mdc)",50,-50.,50.,50,-50.,50.);
  plotF[101]=new TH2F("mdctof2","y(tof-mdc)",70,20.,90.,70,20.,90.);
  hist1[100]=new TH1F("mdctof3","Xtof-Xmdc (mm)",80,-40.,40.);
  hist1[101]=new TH1F("mdctof4","Ytof-Ymdc (mm)",80,-4.,4.);
  
  if(&((*fLookUpTb)[2])){
   HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[2];
   plotF[0] =new TH2F("All_Clusters","All clusters Mdc",
   lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
   lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
   plotF[1] =new TH2F("All_Tof_Hits","All TOF hits",
   lookutsec.getNBinX(),lookutsec.getXlow(),lookutsec.getXup(),
   lookutsec.getNBinY(),lookutsec.getYlow(),lookutsec.getYup());
  }
   

//  hist1[ 40]=new TH1F("h40","dX",200,0.,1.);
//  hist1[ 41]=new TH1F("h41","dY",200,0.,1.);
}

void HMdcTof::writeHists(void) {
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

Bool_t HMdcTof::finalize(void) {
// writeHists();
 return kTRUE;
}

Int_t HMdcTof::getTransfMat(Int_t mod, Int_t layer, HGeomTransform* sys) {
  HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod); 
  if(!pComVol)  return 0;
  HGeomVolume* pGeomVol=pComVol->getComponent(layer); 
  if(!pGeomVol)  return 0; 
  sys->setTransform(pGeomVol->getTransform());
  sys->transFrom(pComVol->getTransform());
  return 1;
}

void HMdcTof::calcCross( HGeomVector* vc1, HGeomVector* vc2, 
            const HGeomTransform* trans, HGeomVector* vcNew){
// Calculeting a cross of line (vc1-vc2) with plane HGeomTransform
  Double_t x1,x2,y1,y2,z1,z2;
  x1=vc1->getX();   x2=vc2->getX();
  y1=vc1->getY();   y2=vc2->getY();
  z1=vc1->getZ();   z2=vc2->getZ();
  const HGeomRotation& mt=trans.getRotMatrix();
  const HGeomVector& tr=trans.getTransVector();
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


ClassImp(HMdcTof)
