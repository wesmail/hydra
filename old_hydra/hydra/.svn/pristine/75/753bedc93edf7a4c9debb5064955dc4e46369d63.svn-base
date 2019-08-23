#include "hmdctest.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hrundb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hlocation.h"
#include "hgeomvector.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include "heventheader.h"
#include "hmdcgetcontainers.h"
#include "../start/hstartunpacker.h"
#include "TMath.h"
#include "TLine.h"
#include "TStyle.h"
#include "TMarker.h"

//*-- Author : V.Pechenov
//*-- Modified : 12.09.99
//*-- Modified : 02.12.99 by V.Pechenov

///////////////////////////////////////////////////////////////
//  HMdcTest
//
// class for analyzing of MDC track finder Dubna
// + event display
//
//  For beam time 09.99 only !!!
///////////////////////////////////////////////////////////////


ClassImp(HMdcTest)


HMdcTest::HMdcTest() {

  fCalCat=0;
  fLookUpTb=0;
  iter=0;
  eventId=0;
}

HMdcTest::HMdcTest(Text_t *name,Text_t *title) : HReconstructor(name,title) {
  fCalCat=0;
  fLookUpTb=0;
  iter=0;
  eventId=0;
}

HMdcTest::~HMdcTest() {
  // destructor
}


Bool_t HMdcTest::init(void) {
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return kFALSE;
  fcut=HMdcTimeCut::getExObject();
  fLookUpTb=HMdcLookUpTb::getExObject();
  fLayerGeomPar=parCont->getMdcLayerGeomPar();
  fGeomPar=parCont->getMdcGeomPar();
  if( !fLookUpTb || !fLayerGeomPar || !fGeomPar ) return kFALSE;


  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat)  return kFALSE;
  isGeant=(fCalCat->getClass()!=HMdcCal1Sim::Class()) ? kFALSE : kTRUE;

  iter=(HIterator *)fCalCat->MakeIterator();
  loc.set(4,0,0,0,0);

  for(Short_t mod=0; mod<4; mod++) {
   HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(mod);
   for(Int_t layer=0; layer<6; layer++) {
    sysLayers[mod][layer]=0;
    if(!pComVol) continue;
    HGeomVolume* pGeomVol=pComVol->getComponent(layer);
    if(!pGeomVol) continue;

    sysLayers[mod][layer]=new HGeomTransform(pGeomVol->getTransform());
    sysLayers[mod][layer]->transFrom(pComVol->getTransform());  // mm!
   }
  }
  
  loc.set(4,0,0,0,0); 
//  plot[0]=new TH2F("dis-tb","dis-tb",160,-0.8,0.8,120,0., 120.);
//  prFile[0]=new TProfile("Prdis-tb","Prdis-tb",160,-0.8,0.8,0., 120.);
  plot[0]=new TH2F("dis-tb","dis-tb",160,-0.8,0.8,230,-30., 200.);
  prFile[0]=new TProfile("time-dist","Xtime-Ydist",230,-30., 200., 0.0,0.8);
  prFile[1]=new TProfile("dist-time","dist-time",160,0.0,0.8, -30., 200.);
  
  
  hst[0] = new TH1F("Time1(all)","Time1 (all)",200,-1000.,1000.);
  hst[1] = new TH1F("Time2(all)","Time2 (all)",200,-1000.,1000.);
  hst[2] = new TH1F("Time2-Time1(all)","Time2-Time1 (all)",200,-1000.,1000.);
  
  hst[3] = new TH1F("Time1(cl.)","Tim1 (cl.)",200,-1000.,1000.);
  hst[4] = new TH1F("Time2(cl.)","Tim2 (cl.)",200,-1000.,1000.);
  hst[5] = new TH1F("Time2-Time1(cl.)","Time2-Time1 (cl.)",200,-1000.,1000.);
  
  hst[6] = new TH1F("Time1(sel.cl.)","Time1 (sel.cl.)",200,-1000.,1000.);
  hst[7] = new TH1F("Time2(sel.cl.)","Time2 (sel.cl.)",200,-1000.,1000.);
  hst[8] = new TH1F("Time2-Time1(sel.cl.)","Time2-Time1 (sel.cl.)",200,-1000.,1000.);
  
  hst[20] = new TH1F("L1YDH","L1 Y for dubl.hit",2000,0.,200.);
  hst[21] = new TH1F("L2YDH","L2 Y for dubl.hit",2000,0.,200.);
  hst[22] = new TH1F("L3YDH","L3 Y for dubl.hit",2000,0.,200.);
  hst[23] = new TH1F("L4YDH","L4 Y for dubl.hit",2000,0.,200.);
  hst[24] = new TH1F("L5YDH","L5 Y for dubl.hit",2000,0.,200.);
  hst[25] = new TH1F("L6YDH","L6 Y for dubl.hit",2000,0.,200.);

  return kTRUE;
}

Int_t HMdcTest::execute(void) {
  eventId++;
  
  
 Int_t nClastCell[6][2][6][250];
 for(Int_t s=0; s<6; s++) {
   for(Int_t m=0; m<2; m++) {
     for(Int_t l=0; l<6; l++) {
       for(Int_t c=0; c<250; c++) nClastCell[s][m][l][c]=0;
     }
   }
 }

//--------Num. clusters have cell ...------------------------
  iter->Reset();

  // Geant data ===========  
  if(isGeant) {
   HMdcCal1Sim* cal=0;
   while ((cal=(HMdcCal1Sim *)iter->Next())!=0) {
    if(cal->getStatus()==-1) continue;
    Int_t s,m,l,c;
    cal->getAddress(s,m,l,c);
    nClastCell[s][m][l][c]++;
   }
  }
  // Real data ===========  
  else {
   HMdcCal1* cal=0;
   while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    if(fcut && fcut->cut(cal)) continue;     //For beam time data !!!!!!!!!!!!!!!!!
    Int_t s,m,l,c;
    cal->getAddress(s,m,l,c);
    nClastCell[s][m][l][c]++;
    hst[0]->Fill(cal->getTime1());
    if(cal->getNHits()==2) {
      hst[1]->Fill(cal->getTime2());
      hst[2]->Fill(cal->getTime2()-cal->getTime1());
    }
   }
  }
//-----------------------------------------------------------


 for(Int_t sec=0; sec<6; sec++){
  if( ! &((*fLookUpTb)[sec]) ) continue;
  maxClusters[sec]=0;
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  coordSys=&(lookutsec.getParPlane());
 //===Selecting of clusters (tracks)===========================
  HMdc12Clusters& clusters=lookutsec.getClusters();
  Int_t nclst=clusters.getNClusters();     // Num. of clusters
  Short_t nLayers[2][nclst];
  if(nclst==0) continue;
  for(Int_t nc=0; nc<nclst; nc++) {
   HMdcCluster& clst=clusters[nc];
   nLayers[0][nc]=nLayers[1][nc]=0;
   for(Short_t l=0; l<12; l++) {
    Short_t mod=l/6;
    Short_t nCells=clst.getNumHits(l);
    if(nCells>0) {
     nLayers[mod][nc]++;
     for(Int_t n=0; n<nCells; n++) {
      Short_t nCell=clst.getHit(l,n);
      nClastCell[sec][mod][l%6][nCell]++;
      
      loc[0]=sec; loc[1]=mod; loc[2]=l%6; loc[3]=nCell;
      HMdcCal1* cal=(HMdcCal1*)fCalCat->getSlot(loc); 
      if(cal) {
        hst[3]->Fill(cal->getTime1());
        if(cal->getNHits()==2) {
          hst[4]->Fill(cal->getTime2());
          hst[5]->Fill(cal->getTime2()-cal->getTime1());
        }
      }
     }
    }
   }
  }

  for(Int_t nc=0; nc<nclst; nc++) {
   qual[sec][nc]=1;
   HMdcCluster& clst=clusters[nc];
   Short_t maxClust=clst.getMaximum();
   if(maxClust>maxClusters[sec]) maxClusters[sec]=maxClust;
   if(maxClust > 6) {
    if(maxClust >10) qual[sec][nc]=0;                   // Selecting clusters ...
    if(nLayers[0][nc] < 5 || nLayers[1][nc] < 5) qual[sec][nc]=0;
   }
   else if(maxClust == 6 || (nLayers[0][nc] < 5 && nLayers[1][nc] < 5)) qual[sec][nc]=0;
   if(!qual[sec][nc]) continue;
   Int_t nCellsZ[2];
   nCellsZ[0]=nCellsZ[1]=-1;
   Short_t cellF=0;
   Short_t cellS=0;
   
   for(Short_t mod=1; mod<2; mod++) {     //!!!!!!!!!!!!!!!!!!!!!!!
     
    if(nLayers[mod][nc]==0) continue;
    for(Short_t layer=0; layer<6; layer++) {
     Short_t nCells=clst.getNumHits(mod*6+layer);
     //----- Num. fired cells in plane >2
     if(nCells>2) qual[sec][nc]=0;
     //----- Num. fired cells in plane >0 & <=2
     else if(nCells>0){
      for(Int_t n=0; n<nCells; n++) {
       Short_t cell=clst.getHit(mod*6+layer,n);
       if(nClastCell[sec][mod][layer][cell] > 2) qual[sec][nc]=0; 
       else {
        // For dubl.hits |n.cell1-n.cell2| must be = 1
        if(n==0) cellF=cellS=cell;
        else {
         cellS=cell;
         if(TMath::Abs(cellF-cellS) != 1) qual[sec][nc]=0;
         else {
          if(cellF>cellS) {
           Short_t tmp=cellF;
           cellF=cellS;
           cellS=tmp;
          }
         }
        }
       }
      }
      // Nearests cells can not be fired
      if(qual[sec][nc]) {
       if(cellF>0 && nClastCell[sec][mod][layer][cellF-1] >0) qual[sec][nc]=0;
       if(nClastCell[sec][mod][layer][cellS+1] >0) qual[sec][nc]=0;
      }
     }
     //----- N. fired cells in plane = 0
     else {
      HGeomVector trP2(clst.getX(),clst.getY(),clst.getZ());
      HGeomVector trP1(0.0,0.0,0.0);
      HGeomVector pointCros;
      calcCross( &trP1, &trP2, mod, layer, &pointCros);
      pointCros=sysLayers[mod][layer]->transTo(pointCros);
      Float_t xcoor=(Float_t)pointCros.getX();
      Float_t ycoor=(Float_t)pointCros.getY();

      HMdcLayerGeomParLay& fLayer = (*fLayerGeomPar)[sec][mod][layer];
      Float_t pitch = fLayer.getPitch();
      Float_t wOrient = fLayer.getWireOrient()*(TMath::Pi()/180.);
      Float_t centWire = fLayer.getCentralWireNr()-1.;

      Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);
      Float_t tmp=(centWire*pitch + y_wire+0.5*pitch)/pitch;
      Int_t nZrCell=(Int_t)tmp;
      if(tmp < 0.0 || nZrCell>=fLayer.getNumWires())  qual[sec][nc]=0;
      else {
       if(nZrCell<0 || nZrCell>=245) qual[sec][nc]=0;
       if(nZrCell>0 && nClastCell[sec][mod][layer][nZrCell-1]>0) qual[sec][nc]=0;
       if(nClastCell[sec][mod][layer][nZrCell] >0) qual[sec][nc]=0;
       if(nClastCell[sec][mod][layer][nZrCell+1] >0) qual[sec][nc]=0;
       nCellsZ[mod]=nZrCell;
      }
     }
     if(!qual[sec][nc]) break;
    }
   }

  }
  for(Int_t nc=0; nc<nclst; nc++) {
//   (*this)[sec].getPlotAllHits()->Fill(x,y);
//   if(qual[sec][nc]) (*this)[sec].getPlotSelHits()->Fill(x,y);
//___________________________________________________________
  if(qual[sec][nc]) {
   HMdcCluster& clst=clusters[nc];
   Float_t x=clst.getX();
   Float_t y=clst.getY();
   Float_t z=clst.getZ();
   HGeomVector pointCros;
    
    Short_t mod=1;
    
    for(Int_t layer=0; layer<6; layer++) {
      HGeomVector trP2(x,y,z);
      HGeomVector trP1(0.0,0.0,0.0);
      calcCross( &trP1, &trP2, mod, layer, &pointCros);
      pointCros=sysLayers[mod][layer]->transTo(pointCros);
      Float_t xcoor=(Float_t)pointCros.getX();
      Float_t ycoor=(Float_t)pointCros.getY();

      HMdcLayerGeomParLay& fLayer = (*fLayerGeomPar)[sec][mod][layer];
      Float_t pitch = fLayer.getPitch()/10.;
      Float_t halfPitch = 0.5*pitch; 
      Float_t wOrient = fLayer.getWireOrient()*(TMath::Pi()/180.);
      Float_t centWire = fLayer.getCentralWireNr()-1.;
      Float_t halfCatDist = 0.5*fLayer.getCatDist()/10.;
      // Transformation from MDC's coordinate system to layer's coord. system:
      Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);
      
      
         
      Short_t nCells=clst.getNumHits(mod*6+layer);
      
      if( nCells == 2 )  {
        Float_t nWr=(y_wire+centWire*pitch)/pitch;
        hst[20+layer]->Fill(nWr);
      }
      
      
      for(Int_t ncI=0; ncI<nCells; ncI++) {
        Short_t nCell=clst.getHit(mod*6+layer,ncI);

        loc[0]=sec; loc[1]=mod; loc[2]=layer; loc[3]=nCell;
        HMdcCal1* cal=(HMdcCal1*)fCalCat->getSlot(loc); 
        if(cal) {
          hst[6]->Fill(cal->getTime1());
          if(cal->getNHits()==2) {
            hst[7]->Fill(cal->getTime2());
            hst[8]->Fill(cal->getTime2()-cal->getTime1());
          }
        }
        
        
        

        if( layer != 2 && layer != 3 ) continue;
            
//        if(nCell<20 || nCell>40) continue;

         HGeomVector trP1T=sysLayers[mod][layer]->transTo(trP1);
         HGeomVector trP2T=sysLayers[mod][layer]->transTo(trP2);
         Float_t DX=trP2T.getX()-trP1T.getX();
         Float_t DY=trP2T.getY()-trP1T.getY();
         Float_t DZ=trP2T.getZ()-trP1T.getZ();
         Float_t theta=atan(sqrt(DX*DX+DY*DY)/DZ);
         Float_t phi=asin(DY/sqrt(DX*DX+DY*DY));
 

         Float_t ctanalpha=tan(theta)*sin(phi-wOrient);
       //  Float_t dY=halfCatDist*ctanalpha;
       //  dY<0.0 ? dY=-dY : dY;
       //  Float_t tmp=(centWire*pitch + y_wire+dY+halfPitch)/pitch;
       //  if(tmp < 0.0) return kFALSE;
       //  Int_t nCmax=(Int_t)tmp;
       //  tmp=(centWire*pitch + y_wire-dY+halfPitch)/pitch;
       //  Int_t nCmin;
       //  tmp < 0.0 ? nCmin=0 : nCmin=(Int_t)tmp;
       //  if(nCmin >= nWires) return kFALSE;
       //  nCmax>=nWires ? nCmax=nWires-1 :nCmax;
       //printf("nTr=%i, %i-%i",trkNum,nCmin,nCmax);

          Float_t yDist=y_wire-(nCell*pitch-centWire*pitch);
          Float_t sinAlpha=sqrt(1./(1.+ctanalpha*ctanalpha));
          Float_t cosAlpha=sqrt(1.-sinAlpha*sinAlpha);
          Float_t per = yDist*sinAlpha;
       //   per<0.0 ? per=-per : per;
//          if(per*sinAlpha > halfPitch) {
//           Float_t z1=(yDist+halfPitch)/ctanalpha;
//           Float_t z2=(yDist-halfPitch)/ctanalpha;
//           (z1*=z1)<(z2*=z2) ? per=sqrt(halfPitch*halfPitch+z1) : per=sqrt(halfPitch*halfPitch+z2);
//          } else if(per*cosAlpha > halfCatDist) {
//           Float_t y1=halfCatDist*ctanalpha+yDist;
//           Float_t y2=halfCatDist*ctanalpha-yDist;
//           (y1*=y1)<(y2*=y2) ? per=sqrt(y1+halfCatDist*halfCatDist) : per=sqrt(y2+halfCatDist*halfCatDist);
//          }    


//          HMdcCal1* cal=0;
          loc[0]=sec;
          loc[1]=mod;
          loc[2]=layer;
          loc[3]=nCell;
          cal=(HMdcCal1*)fCalCat->getSlot(loc);
          Float_t nTB=cal->getTime1();
       //Printf("Ntm=%i t1=%f t2=%f",cal->getNHits(),cal->getTime1(),cal->getTime2());
       //   per=fabs(per);
          plot[0]->Fill(per,nTB);
          per=fabs(per);
          prFile[0]->Fill(nTB,per);
          prFile[1]->Fill(per,nTB);
       // Printf("per=%e time=%e",per,nTB);




   //      Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);
   //      Float_t tmp=(centWire*pitch + y_wire+0.5*pitch)/pitch;
   //      Int_t nZrCell=(Int_t)tmp;
   //      
        
      }
    }
   }
//_______________________________________________________________   
  }
 }
 return 0;
}

void HMdcTest::drawProjPlots(Int_t sec) {
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

void HMdcTest::drawHitsPlots(Int_t sec) {
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


void HMdcTest::printClusters(Int_t sec) {
  if(&((*fLookUpTb)[sec])==0) return;
  HMdcLookUpTbSec& lookutsec = (*fLookUpTb)[sec];
  HMdc12Clusters& clusters=lookutsec.getClusters();
  clusters.print();
}


Bool_t HMdcTest::finalize(void) {
// writeHists();
 return kTRUE;
}

Int_t HMdcTest::calcCross( HGeomVector *point1, HGeomVector *point2,
                                 Short_t mod, Short_t layer, HGeomVector *cross){
// Calculeting a cross of line (point1-point2) with layer
  if(!sysLayers[mod][layer]) return 0;
  Double_t x1=point1->getX();   Double_t x2=point2->getX();
  Double_t y1=point1->getY();   Double_t y2=point2->getY();
  Double_t z1=point1->getZ();   Double_t z2=point2->getZ();
  const HGeomRotation& mt=sysLayers[mod][layer]->getRotMatrix();
  const HGeomVector& tr=sysLayers[mod][layer]->getTransVector();
  Double_t A=(mt(3)*mt(7)-mt(6)*mt(4));
  Double_t B=(mt(6)*mt(1)-mt(0)*mt(7));
  Double_t C=(mt(0)*mt(4)-mt(3)*mt(1));
  Double_t D=-(A*tr(0)+B*tr(1)+C*tr(2));
  Double_t point[3];
  point[0]=(x1*(B*(y1-y2)+C*(z1-z2))-(B*y1+C*z1+D)*(x1-x2))/  
           (A*(x1-x2)+B*(y1-y2)+C*(z1-z2));
  point[1]=(point[0]-x1)*(y1-y2)/(x1-x2)+y1; //??? cm. notes
  point[2]=(-A*point[0]-B*point[1]-D)/C;
  cross->setVector(point);
  return 1;
}
  
