using namespace std;
#include <iostream> 
#include <iomanip>
#include <unistd.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>
#include "hstarthit.h"
#include "htboxchan.h"
#include "hlocation.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "htree.h"
#include "hrecevent.h"
#include "hevent.h"
#include "hdetector.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "hcategory.h"
#include "hades.h"
#include "hscalersreader.h"
#include "hiterator.h"
#include "hspectrometer.h"

HScalersReader::HScalersReader(void){
  fTBoxCat=NULL;
  iter_tbox = 0;

}

HScalersReader::HScalersReader(Text_t *name,Text_t *title) : HReconstructor (name,title) {
  fTBoxCat=NULL;
  iter_tbox = 0;
  counter_execute=0;
}

HScalersReader::~HScalersReader(void) {
  if(iter_tbox) delete iter_tbox;
  if(ofile){
    writeHist(ofile);
    ofile->Delete();
  }
  printf("\nNumber of scaler events %d \n",counter_execute);

  if (fNameRootScalersReader) delete fNameRootScalersReader;
}

Bool_t HScalersReader::init(void) {

  fTBoxCat=gHades->getCurrentEvent()->getCategory(catTBoxChan);
  if (!fTBoxCat) {
    fTBoxCat=gHades->getSetup()->getDetector("TBox")->buildCategory(catTBoxChan);
    if (!fTBoxCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTBoxChan,fTBoxCat,"TBox");
  }
  iter_tbox=(HIterator*)fTBoxCat->MakeIterator();

  if(fNameRootScalersReader)  ofile = new TFile(fNameRootScalersReader,"RECREATE");
  if (ofile) createHist(ofile);
  return kTRUE;
}


Int_t HScalersReader::execute(void) {

  HTBoxChan* tbox=NULL;

  //  Get tbox data and fill histograms.
  Int_t chan,scaler1=0,scaler2=0,scaler3=0,scaler4=0;
  Int_t mycount=0;
  iter_tbox->Reset();
  while ( (tbox=(HTBoxChan *)iter_tbox->Next())!=NULL) {
    tbox->get(chan,scaler1,scaler2,scaler3,scaler4);

    //fill scaler content over the time
    if(counter_execute<2001){
      histInput[chan]->Fill(counter_execute,scaler1);
      histReduce[chan]->Fill(counter_execute,scaler2);
      histStartScaler[chan]->Fill(counter_execute,scaler3);
      histVetoScaler[chan]->Fill(counter_execute,scaler4);
    }
     histInputScalerPattern->Fill(chan+1,scaler1);
     histReduceScalerPattern->Fill(chan+1,scaler2);

    // start detector profile histogram
    if(chan<4){
      histStartScalerPattern->Fill(3-chan+1,scaler3);
      histStartScalerPattern->Fill(4+chan+1,scaler4);
    }
    // veto detector profile histogram
    if(chan==4)histVetoScalerPattern->Fill(4,scaler3);
    if(chan==5)histVetoScalerPattern->Fill(3,scaler3);
    if(chan==6)histVetoScalerPattern->Fill(2,scaler3);
    if(chan==7)histVetoScalerPattern->Fill(1,scaler3);
    if(chan==4)histVetoScalerPattern->Fill(5,scaler4);
    if(chan==5)histVetoScalerPattern->Fill(6,scaler4);
    if(chan==6)histVetoScalerPattern->Fill(7,scaler4);
    if(chan==7)histVetoScalerPattern->Fill(8,scaler4);

    mycount++;
  }           //end of iter

  if(mycount!=0) {
    counter_execute++;
  }

  return 0;
}

void HScalersReader::setOutputRoot(Char_t *c){
    if (fNameRootScalersReader) delete fNameRootScalersReader;
    fNameRootScalersReader = new Char_t[strlen(c)+1];
    strcpy(fNameRootScalersReader, c);
}

void HScalersReader::createHist(TFile* ofile){
  ofile->cd();
  Text_t buffer[255];
  Int_t det=0;
  for(det=0;det<8;det++) {
    sprintf(buffer,"hScalerInput_%i",det+1);
    histInput[det] = new TH1F(buffer,"Input",2000,0,2000);
    sprintf(buffer,"hScalerReduce_%i",det+1);
    histReduce[det] = new TH1F(buffer,"Reduce",2000,0,2000);
    sprintf(buffer,"hScalerStart_%i",det+1);
    histStartScaler[det] = new TH1F(buffer,"Start",2000,0,2000);
    sprintf(buffer,"hScalerVeto_%i",det+1);
    histVetoScaler[det] = new TH1F(buffer,"Veto",2000,0,2000);
  }

  histStartScalerPattern = new TH1F("hPatternStartScaler","PatternStart",8,0.5,8.5);
  histVetoScalerPattern = new TH1F("hPatternVetoScaler","PatternVeto",8,0.5,8.5);
  histInputScalerPattern = new TH1F("hPatternInputScaler","Triggerbox In",8,0.5,8.5);
  histReduceScalerPattern = new TH1F("hPatternReduceScaler","Triggerbox Out",8,0.5,8.5);

}

void HScalersReader::writeHist(TFile* ofile){
  // Write histograms to a rootfile.
  ofile->cd();

  histStartScalerPattern->Write();
  histVetoScalerPattern->Write();
  histInputScalerPattern->Write();
  histReduceScalerPattern->Write();

  for(Int_t i = 0; i<8; i++){
    histReduce[i]->Write();
    histInput[i]->Write();
    histStartScaler[i]->Write();
    histVetoScaler[i]->Write();
  }
  ofile->Save();
  ofile->Close();
}




ClassImp(HScalersReader)
