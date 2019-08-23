//*-- AUTHOR : C.Muentz
//*-- Modified :

///////////////////////////////////////////////////////////////////////////////
//
// HMdcSelfTracking
//
//
///////////////////////////////////////////////////////////////////////////////
#include "hmdcselftracking.h"

#include <stdlib.h>
#include <fstream.h>
#include <iomanip.h>
#include <iostream.h>
#include <stdio.h>

#include <TClass.h>
#include <TFile.h>
#include <TH1.h> // added! CM
#include <TH2.h> // added! CM
#include <TProfile.h> // added! CM
#include "hades.h"
#include "hmdcdetector.h"
#include "hmdccal1.h"
#include "hrecevent.h"
#include "htree.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmatrixcategory.h"
#include "hpartialevent.h"
#include "htaskset.h"
#include "hiterator.h"
#include "hcategory.h"
#include "hspectrometer.h"
#include "hmdcdef.h"
#include "hevent.h"

ClassImp(HMdcSelfTracking)


HMdcSelfTracking::HMdcSelfTracking(void)
{
    //setDefault();
    fout   = NULL;
    calCat = NULL;
    iter   = NULL;
    // some local declarations:

// *** cuts ***
 tdifmin = 20;
 tdifmax= 150;
 tdifcut = 10;
 tmin=0;
 tmax=300;
// *** end cuts ***

 cellmin=104;
 cellmax=108;


il1=3;
il2=4;

cnt0=0;
cnt1=0;
cnt2=0;

}

HMdcSelfTracking::HMdcSelfTracking(Text_t* name,Text_t* title) : HReconstructor(name,title)
{
    //setDefault();
    fout   = NULL;
    calCat = NULL;
    iter   = NULL;
    // some local declarations:
   
// *** cuts ***
 tdifmin = 20;
 tdifmax= 150;
 tdifcut = 10;
 tmin=0;
 tmax=300;
// *** end cuts ***

 cellmin=104;
 cellmax=108;

il1=3;
il2=4;

cnt0=0;
cnt1=0;
cnt2=0;

}

HMdcSelfTracking::~HMdcSelfTracking(void)
{
    // Destructor.
    if (iter)       delete iter;
    if (fNameRoot)  delete fNameRoot;
    //if(fout) delete fout;
    iter = NULL;
}


void HMdcSelfTracking::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcSelfTracking where all
    //created histograms were written.
    
    if (fNameRoot) delete fNameRoot;
    fNameRoot = new Char_t[strlen(c)+1];
    strcpy(fNameRoot, c);
}

Bool_t HMdcSelfTracking::init(void)
{
    //  Inits HMdcSelfTracking
    calCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
    if (!calCat) {
        calCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal1);
        if (!calCat) return kFALSE;
        else gHades->getCurrentEvent()->addCategory(catMdcCal1,calCat,"Mdc");
    }
    // creates an iterator which loops over all fired cells
    iter=(HIterator *)calCat->MakeIterator("native");
    if (fNameRoot && !fout)
        fout = new TFile(fNameRoot,"RECREATE");
 
    createHist();
    resetCounters();
    fActive=kTRUE;

    return kTRUE;
}

void HMdcSelfTracking::createHist()
{
// allocate histograms

char htitle[20], ptitle[20];
for (Int_t i=1;i<7;i++){
  sprintf(htitle, "%s%i","Time_cell",i);
  sprintf(ptitle, "%s%i%s","pTime_cell[",i,"]");
  pTime_cell[i] = new TH2S(ptitle,htitle,300,0,600,200,0,200);
  sprintf(htitle, "%s%i","Time0_cell",i);
  sprintf(ptitle, "%s%i%s","pTime0_cell[",i,"]");
  pTime0_cell[i] = new TH2S(ptitle,htitle,350,-100,600,200,0,200);
  if (i>0 && i<3) {
    sprintf(htitle, "%s%i","Timesum_cell",i);
    sprintf(ptitle, "%s%i%s","pTimesum_cell[",i,"]");
    pTimesum_cell[i] = new TH2S(ptitle,htitle,200,0,200,100,0,200);
    sprintf(htitle, "%s%i","Timesumdif",i);
    sprintf(ptitle, "%s%i%s","pTimesumdif[",i,"]");
    pTimesumdif[i] = new TH2S(ptitle,htitle,75,0,150,50,-100,100);
    for (Int_t j=90; j<116; j++){
       sprintf(htitle, "%s%i%s%i","Fish",i,"_",j);
       sprintf(ptitle, "%s%i%s%i%s","pFish[",i,"][",j,"]");
       pFish[i][j] = new TH2S(ptitle,htitle,75,0,150,50,-100,100);
       sprintf(htitle, "%s%i%s%i","Tsum_ev",i,"_",j);
       sprintf(ptitle, "%s%i%s%i%s","pTsum_ev[",i,"][",j,"]");
       pTsum_ev[i][j] = new TH2S(ptitle,htitle,300,0,600000,80,50,130);
       sprintf(htitle, "%s%i%s%i","Tsum_ev_prof",i,"_",j);
       sprintf(ptitle, "%s%i%s%i%s","pTsum_ev_prof[",i,"][",j,"]");
       pTsum_ev_prof[i][j] = new TProfile(ptitle,htitle,150,0,600000,50,130);
    }
  }
  sprintf(htitle, "%s%i","Layer",i);
  sprintf(ptitle, "%s%i%s","pLayer[",i,"]");
  hlayer[i] = new TH1D(ptitle,htitle,200,0,200);
}
ptdif[0] = new TH1D("ptdif[0]","tdif0",250,-50,200);
ptdif[1] = new TH1D("ptdif[1]","tdif1",250,-50,200);
layer0[1] = new TH1D("player01","layer01",200,0,200);
layer0[2] = new TH1D("player02","layer02",200,0,200);
info = new TH1D("pinfo","info",20,0,10);
control = new TH1D("pcontrol","control",20,0,10);
eff = new TH1F("peff","eff",160,0,160);



}
/*
void HMdcSelfTracking::deleteHist()
{
// Created Histograms are deleted
}
*/

void HMdcSelfTracking::fillHist()
{

     // analyze hits per event:
     // layer 3: +0deg, PC=98.5, layer 4: -0deg, PC 98
     if (nhit[il1] > 0) {
       for (Int_t ihit=1; ihit<nhit[il1]+1; ihit++) { // loop over hits in layer3
         flag[0]=0;
         for (Int_t ii=1; ii<nhit[il1]+1; ii++){ // check adjacent cells
            if (abs(c[il1][ihit]-c[il1][ii])==1) flag[0]++;
         }        
         if (flag[0]<1) { // hit cells  w/o neighbours
           cnt0++;
           counter[0][c[il1][ihit]]++;
           flag[1]=0; flag[2]=0;
           for (Int_t ii=1; ii<nhit[il2]+1; ii++){ // loop over hits in layer4
             if ((c[il1][ihit]-c[il2][ii])==1) { // left
               flag[1]++;
               lhit[1]=ii;
	     } 
	     if ((c[il1][ihit]-c[il2][ii])==0) { //right
               flag[2]++;
               lhit[2]=ii;
	     }
           } // layer4 hit loop
 
           if ((flag[1] + flag[2]) >0) { // hit(s) in layer4, incl. double hits
             cnt1++;
             counter[1][c[il1][ihit]]++;
           }
           if ((flag[1] + flag[2])==2) { // double hit in layer4
             cnt2++; 
             counter[2][c[il1][ihit]]++;
           } 

           if ((flag[1] + flag[2])==1) { // single hit in layer4

             id=1; // fill left
             if (flag[2]==1) id=2; //fill right

	     Float_t t3=t1[3][ihit];
	     Float_t t4=t1[4][lhit[id]];
	     if (c[il1][ihit]>(cellmin-1) && c[il1][ihit]<(cellmax+1)) 
	       pTimesumdif[id] -> Fill(t3+t4,t3-t4,1.);  // empty
	     if (c[il1][ihit]>89 && c[il1][ihit]<116) {
	       pFish[id][c[il1][ihit]] -> Fill(t3+t4,t3-t4,1.);  // empty
	       pTsum_ev[id][c[il1][ihit]] ->  Fill(i,t3+t4,1.);  // empty
	       pTsum_ev_prof[id][c[il1][ihit]] -> Fill(i,t3+t4);  // empty
	     }  
	     if ((t3-t4)*(t3-t4)<tdifcut*tdifcut) {
	       pTimesum_cell[id] -> Fill(c[il1][ihit],t3+t4,1.);
	       layer0[id]->Fill((Float_t)c[il1][ihit],1.);
	     }
	    
	   } // end case single hit in layer4
 
         } // end case layer3 sinlge hit cell  
       } // end layer3 hit loop
     } // end case layer3


     // end event-wise analysis
  

}

void HMdcSelfTracking::writeHist()
{
    // All created histograms are written to a rootfile.
    fout->cd();

    info -> Write();
    control -> Write();
    ptdif[0] -> Write(); ptdif[1] -> Write();
    layer0[1] -> Write(); layer0[2] -> Write();
    eff -> Write();
    for (Int_t i=1; i<7; i++){
        hlayer[i] -> Write();
        pTime_cell[i] -> Write();
        pTime0_cell[i] -> Write();
        if (i>0 && i<3){
            pTimesum_cell[i] -> Write();
            pTimesumdif[i] -> Write();
            for (Int_t j=90; j<116; j++){
                pFish[i][j] -> Write();
                pTsum_ev[i][j] -> Write();
                pTsum_ev_prof[i][j] -> Write();
            }
        }
    }
}


Bool_t HMdcSelfTracking::finalize()
{

    fillControlHists();
    if (fout)  writeHist ();

    fout->Save();
    fout->Close();
    delete fout;

    return kTRUE;
}

void HMdcSelfTracking::resetCounters()
{
 // reset counter
for (Int_t ii=0; ii<3; ii++){ for (Int_t jj=0;jj<161;jj++){ counter[ii][jj]=0; } }
for (Int_t ii=0; ii<11; ii++){ ctrl[ii]=0; }

for (Int_t ii=0; ii<8; ii++){ nhit[ii]=0;} 
}

void HMdcSelfTracking::fillControlHists()
{
  info->Fill(1,cnt0);info->Fill(2,cnt1);info->Fill(3,cnt2);
     control->Fill(1,ctrl[0]);control->Fill(2,ctrl[1]);
     control->Fill(3,ctrl[2]);control->Fill(4,ctrl[3]);

     printf(" layer3 hits: %i, corresponding layer4 hits: %i (double hits: %i)\n", cnt0, cnt1, cnt2);

     for (Int_t i=1; i<161; i++){
         if (counter[0][i]>0 && counter[1][i]>0)
             eff->Fill(i, (counter[1][i]/counter[0][i])*100 );
     }

}

Int_t HMdcSelfTracking::execute()
{
     iter->Reset();
     // reset nhit & pc counters
     for (Int_t ii=0; ii<8; ii++){ nhit[ii]=0;}

     // hit loop, fill local variables:
     HMdcCal1 *objMdcC;

     while ( (objMdcC=(HMdcCal1 *)iter->Next())!=0 ) {
         sector = objMdcC->getSector() + 1;
         module = objMdcC->getModule() + 1;
         layer = objMdcC->getLayer() + 1;
         cell = objMdcC->getCell() + 1;
         tt1 = objMdcC->getTime1();
         tt2 = objMdcC->getTime2();
         ptdif[0] -> Fill(tt2-tt1,1.);
	 pTime0_cell[layer] -> Fill(tt1,(Float_t) cell,1.);
        
         // CUTS:
         ctrl[0]++;
	 if (tt1>tmin && tt1<tmax){   // time1 (time window due to "-99")
           ctrl[1]++;
	   if (tt2>tmin && tt2<tmax){  // time2
             ctrl[2]++;
	     if ((tt2-tt1)>tdifmin && (tt2-tt1)<tdifmax){  // time-above-thr. 
               nhit[layer]++;
               ctrl[3]++;
               c[layer][nhit[layer]] = cell;
               t1[layer][nhit[layer]]   = tt1;
               t2[layer][nhit[layer]]   = tt2;
	       pTime_cell[layer] -> Fill(tt1,(Float_t) cell,1.);
               ptdif[1] -> Fill(tt2-tt1,1.);
               hlayer[layer] -> Fill((Float_t) cell, 1.);
	       } // time-dif
	    } // time2
         } // time1
         
     } // end hit loop     end of while Iterator
     fillHist();
     return 0;
} // end event loop

