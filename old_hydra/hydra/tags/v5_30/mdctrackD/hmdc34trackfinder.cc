#include "hmdc34trackfinder.h"
#include "hmdctrackddef.h"
//#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hmdcgetcontainers.h"
//#include "TStopwatch.h"

#include "TROOT.h"

//*-- Author : V.Pechenov
//*-- Modified : 14.04.99

////////////////////////////////////////////////////////////////
// HMdc34TrackFinder
//
//  HMdc34TrackFinder(Text_t *name,Text_t *title, Bool_t per=kFALSE)
//  per(persistency)=kFALSE - category HMdcClus will not write
//  to output file, =kTRUE - will write
//
////////////////////////////////////////////////////////////////

HMdc34TrackFinder::HMdc34TrackFinder(Bool_t per) {
  // per(persistency)=kFALSE - category HMdcClus will not writing to output
  // file, =kTRUE - will
  perMdcClus=per;
  setParContainers();
}

HMdc34TrackFinder::HMdc34TrackFinder(Text_t *name,Text_t *title, Bool_t per) : 
                   HReconstructor(name,title) {
  perMdcClus=per;
  setParContainers();
}

void HMdc34TrackFinder::setParContainers() {
  fCalCat=0;
  iter=0;    
  level4=10;
  level5=50;
  fSegCat=0;
  locSeg.set(2,0,0);
  fClFinder=HMdc34ClFinder::getObject();
  fcut=HMdcTimeCut::getExObject();
  if( !fcut ) Printf("Cut for drift time not seted!");
  
/*for(Int_t ns=0; ns<6; ns++) { //-----?????? 
  sprintf(nameCanv[ns],"Sec%i_Seg2",ns+1);//-----??????
  Int_t n1=(ns%3)*410+1;
  Int_t n2=(ns/3)*530+1;
  canv[ns]=new TCanvas(nameCanv[ns],nameCanv[ns],n1,n2,400,500); //-----??????
}*/   //-----??????

}

Bool_t HMdc34TrackFinder::init(void) {
  fCalCat=HMdcGetContainers::getCatMdcCal1();
  if( !fCalCat ) return kFALSE;
  isGeant=HMdcGetContainers::isGeant();
  iter=(HIterator *)fCalCat->MakeIterator("native");
  
  fClusCat=HMdcGetContainers::getCatMdcClus(kTRUE);
  if( !fClusCat ) return kFALSE;
  fClusCat->setPersistency(perMdcClus);  // perenesti v HMdcTrackFinder !!!
  fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if( !fmdc ) return kFALSE;
  modules=fmdc->getModules(); 
  
  fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);                      
  if( !fSegCat ) return kFALSE;
  iterSeg=(HIterator *)fSegCat->MakeIterator();
  if( !fClFinder || !(fClFinder->initCat()) ) return kFALSE;
  return kTRUE;
}


Int_t HMdc34TrackFinder::execute(void) {

//TStopwatch timer;
//timer.Start();
  Int_t s,m,l,c;  
  (*fClFinder).clear();  
  iter->Reset();  
  HMdcCal1* cal=0;
  while ((cal=(HMdcCal1 *)iter->Next())!=0) {
    cal->getAddress(s,m,l,c);
    if( m < 2 || fmdc->getModule(s,m) == 0 ) continue;
    if(isGeant) {
      // === Geant data ====================================
//Int_t tr1=cal->getNTrack1();   //----???---
//Int_t tr2=(cal->getNHits()==2) ? cal->getNTrack2() : -100;
//if((*fClFinder)[s].getNTrack()!=tr1 && (*fClFinder)[s].getNTrack()!=tr2) continue;
//      if(((HMdcCal1Sim *)cal)->getStatus()==-1) continue;
      (*fClFinder)[s][m][l].setCell(c,cal->getNHits()); // nHits=1 =>1, nHits=2 =>3
    } else {
      // === Real data ===================================== 
      if(fcut && fcut->cut(cal)) continue; //  Cut.  Beam-time !!!
      //(*fClFinder)[s][m][l].setCell(c,cal->getNHits());

      (*fClFinder)[s][m][l].setCell(c,1);  // !!!!!!
    }
  }
   
//timer.Stop();
//printf(" Cpu time for setCell=%f\n",timer.CpuTime());
//timer.Start();
  
  HMdcSeg* seg=0;
  Int_t numcl=0;
  for(Int_t sec=0; sec<6; sec++) {
    HMdc34ClFinderSec& fSec=(*fClFinder)[sec];
    if( !(&fSec) ) continue;
    const Int_t *modInSec=modules+sec*4;
    Int_t minBin[4]={0,0,0,0};  // In f. for 4 modules ?
    Int_t nHitsTot=0;
    for(Int_t mod=2; mod<4; mod++) {
      if( modInSec[mod] ) {
        minBin[mod]=6;
        Int_t nHits=fSec[mod].getNCells();
        nHitsTot+=nHits;
        if( nHits <= level5 ) minBin[mod]--;
        if( nHits <= level4 ) minBin[mod]--;
      }
    }
//canv[sec]->Clear(); canv[sec]->Modified(); canv[sec]->Update();  //---???---
    if( !nHitsTot ) continue;
    iterSeg->Reset();
    locSeg[0]=sec;
    locSeg[1]=0;
    iterSeg->gotoLocation(locSeg);
//Int_t nClust=0;
    while ((seg=(HMdcSeg *)iterSeg->Next())!=0) {
//nClust++;
//  Int_t ncl=fSec.findClustersSeg2(seg,minBin);
//  numcl+=ncl;
//  Printf("S.%i: %i cluslters.",sec,ncl);
      numcl+=fSec.findClustersSeg2(seg,minBin);

//---???---:
//if(ncl>0) continue;
//Printf("S%i cl.%i minBins=%i,%i",sec,nClust,minBin[2],minBin[3]);
/*  Char_t buf[20];
  sprintf(buf,"Pl_Sec%i_Seg2",sec+1);
//  TH2C* pl=fSec.getPlotSeg2()->getPlotV2(buf, buf);
  TH2C* pl=fSec.getPlotSeg2()->getPlot(buf, buf);
  TCanvas* can=(TCanvas*)gROOT->FindObject(nameCanv[sec]);
  if(!canv) continue;
  can->cd(); can->cd(1);
  pl->Draw("COLZ");
  can->Modified(); can->Update(); 
break; */
    }
  }
  fClusCat->filter(*((HFilter*)&filter));    
//timer.Stop();  
//printf(" Cpu time for track finder mdc3&4 =%g\n",timer.CpuTime());

  
//printf("*************Total %i clasters found:\n",numcl);
//  (*fClFinder).printClusters();

  return 0;
} 

ClassImp(HMdc34TrackFinder)
