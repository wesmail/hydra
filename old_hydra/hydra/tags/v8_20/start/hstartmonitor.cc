//*-- AUTHOR : D.Bertini
//*-- Modified : 99/09/06 by D.Bertini
using namespace std;
#include "hstartmonitor.h"
#include "hldsubevt.h"
#include "hades.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include "hstartunpacker.h"
#include "hdebug.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream> 
#include <iomanip>

HStartMonitor::HStartMonitor(Text_t* name , Text_t* title, Int_t ifopt)
: HReconstructor(name,title) {
fopt = ifopt;
pMFile=0;
pFile=0;
}


HStartMonitor::~HStartMonitor(){
  if(fopt==1) pMFile->Close();
}

Bool_t HStartMonitor::init(void){
 printf(" HStartMonitor::init() called \n");
 //keep the start header 
  /*
 HRecEvent *event = 0;
 
 HPartialEvent *stpar=0;
 event=(HRecEvent *)gHades->getCurrentEvent();
 if (!event) return kFALSE;
 stpar=event->getPartialEvent(catStart);
 if (!stpar) return kFALSE; 
 header = (HStartHeader *)stpar->getSubHeader();
 if (!header) return kFALSE; 
 //init the ntuple
 */
  if( fopt == 1 ) {
   pMFile = TMapFile::Create("monstart.map","RECREATE", 100000000,
			   "Memory mapped file with Start-Raw-Data");
     }else{
    pFile = new TFile("monstart.root","RECREATE"); 
   }

 // sTree = new TTree("StartTree",
 //                        "Start info Tree");

histMult1 = new TH1F("Mult1","Start multiplicity",10,-.5,9.5);
histMult2 = new TH1F("Mult2","Veto  multiplicity",10,-0.5,9.5);
histM1M2  = new TH2F("m1m2","m1m2",5,-0.5,4.5,5,-0.5,4.5); 
histj1j2  = new TH2F("j1j2","j1j2",8,-0.5,7.5,8,-0.5,7.5); 
Text_t buf1[100];
Text_t buf2[100];
Text_t buf3[100];
// init histogramms


 for (Int_t i=0;i<4;i++){
   for(Int_t j=0;j<4;j++){  
   sprintf(buf3,"stvt%i-%i (Mult=1)",i+1,j+1);
   hdiff[i][j]= new TH1F(buf3,buf3,4000,-2000,2000); 
   }
 }


for(Int_t i=0;i<8;i++) {
  if( i<4 ){ 
  sprintf(buf1,"tdcStart%i",i+1);
  sprintf(buf2,"Start Time %i (Mult=1)",i+1);
  }else{ 
  sprintf(buf1,"tdcVeto%i",i+1);
  sprintf(buf2,"Veto Time %i (Mult=1)",i+1);
  }
  

  histTdc[i] = new TH1F(buf1,buf2,4096,0.5,4095.5);
  sprintf(buf1,"adc%i",i+1);
  sprintf(buf2,"Start Adc %i",i+1);
  histAdc[i] = new TH1F(buf1,buf2,4096,0.5,4095.5);
}
return kTRUE;
}


Bool_t HStartMonitor::finalize(void){

if( fopt == 1 ) {
     printf(" contents of a Map File \n");
     pMFile->ls();
     //   pMFile->Close();
     //delete pMFile;
 }else{ 
     pFile->Write();
     pFile->Close();   
 }
 return kTRUE; 
}

Int_t HStartMonitor::execute(void) {

 Int_t mult1 = 0;
 Int_t mult2 = 0;  
 Int_t j1=0; 
 Int_t j2=0;
 Int_t  k=0;  




//calculate the start-veto multiplicity 
  for(Int_t j=0;j<4;j++) {
    if(HStartUnpacker::tdcData[j]>10) {  j1=j; 
                          mult1++;
                        }
  }

  
  for(k=4;k<8;k++) {
    if(HStartUnpacker::tdcData[k]>10){  j2=k-4; 
                         mult2++;
                      }
  }




   
  if(fopt == 1) {  
                  pMFile->cd();
  }else{ 
                  pFile->cd(); 
  }


// diff veto-start
  if( (mult1 == 1) && (mult2 ==1) ){

      hdiff[j1][j2]->Fill((HStartUnpacker::tdcData[j1])-
                        (HStartUnpacker::tdcData[j2+4])); 

  
 }


                   
  histMult1->Fill(mult1);
  histMult2->Fill(mult2);
  histM1M2->Fill((Axis_t) mult1,(Axis_t) mult2); 
  histj1j2->Fill((Axis_t) j1,(Axis_t) j2); 
    
   

  
  // fill start hists  
  for(Int_t j=0;j<4;j++) {
      if(mult1 == 1) histTdc[j]->Fill(HStartUnpacker::tdcData[j]);
  }
  // fill veto hists 
  for(Int_t j=4;j<8;j++) {
      if(mult2 == 1) histTdc[j]->Fill(HStartUnpacker::tdcData[j]);
  }

if(fopt == 1) pMFile->Update();
 
 return kTRUE;
}

ClassImp(HStartMonitor)
