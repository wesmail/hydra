/////////////////////////////////////////////////////////////////////////
// MU (HMULeptons) - MU Emu 1-1correlation
// based on RICH (HMatchURich) and Meta (HMatchUTof, HMtchUShower) IPUs
// lepton trigger emulated
// now also momentum determination included
//
// load the library  > .L mu-muEmu_momcorr.cc+
// call the function > mu-muEmu_corr()
/////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include <iostream.h>
#include <unistd.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

#include "hmatchurich.h"
#include "hmatchushower.h"
#include "hmatchutof.h"
#include "hmuleptons.h"
#include "hmuEmuleptons.h"

#include "hlinearcategory.h"
#include "htree.h"
#include "hrecevent.h"
#include "heventheader.h"
#include "hevent.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hgeomvector.h"
#include "hades.h"
#endif

void process_filename(TString &inputFile, TString &outputFile, TString suffix, TString outputpath="")
{
  TString file="", inputpath="";

  if (!inputFile.EndsWith(".root") && !inputFile.EndsWith(".hld")){
    inputFile+=".root";
  }
  cout << "File input: " << inputFile << endl;

  // now, Do bad thing with string
  Char_t buffer[1000], *c;
  strncpy(buffer,(const Char_t *)inputFile,999);
  c=strrchr(buffer,'/');
  if(c){
    *c=0;
    file=c+1;
    inputpath=buffer;
  }else{
    file=inputFile;
    inputpath="";
  }

  strncpy(buffer,(const Char_t *)file,999);
  c=strrchr(buffer,'.');

  if(c){
    *c=0;
    file=buffer;
  }

  if(outputpath=="") outputpath=inputpath;
  if(!outputpath.EndsWith("/")) outputpath+="/";

  outputFile  = outputpath+file+suffix+".root";

  cout << "File output: " << outputFile << endl;
}

typedef struct {
  Float_t rth;
  Float_t rph;
  Float_t mth;
  Float_t mph;
  Float_t p;
  Float_t DP;
  Float_t DT;
  Int_t eleflag;
  Int_t MF;
  Int_t sec;
} MULeptontype;

typedef struct {
  Float_t m_a;
  Float_t opangle_a;
  Float_t m_u;
  Float_t opangle_u;
  Float_t m_l;
  Float_t opangle_l;
} MUEmuDiLeptontype;

Int_t ana_emu_newcut(TString inputFile,Int_t nEvents=0,Bool_t verbose=false)
{
//  gROOT->Reset();

  TString outFile="";
  process_filename(inputFile,outFile,"_muCORRnew");

  TFile *hfile = new TFile((Text_t *)outFile.Data(),"RECREATE","Test",2);

  TFile *f = new TFile(inputFile);
if(!f) {cout << "File not opened! " <<inputFile << endl; exit(1);}

  HTree *T = (HTree*)gDirectory->Get("T");
  if(!T) {cout << "No tree in file!" << endl; exit(1);}

  HEventHeader* fEventHead = (HEventHeader*)f->Get("EventHeader");
  TBranch* evt = T->GetBranch("EventHeader.");
  if(evt) evt->SetAddress(&fEventHead);

  f->cd("dirTrigger");
  HMatchURich* ptlHMatchURich = new HMatchURich();
  HLinearCategory* catlHMatchURich = (HLinearCategory*) gDirectory->Get("HMatchURich");
  TBranch* brlHMatchURich = T->GetBranch("HMatchURich.");
  if(brlHMatchURich) brlHMatchURich->SetAddress(&catlHMatchURich);
  TIterator *iterHMatchURich = catlHMatchURich->MakeIterator();

  HMatchUTof* ptlHMatchUTof = new HMatchUTof();
  HLinearCategory* catlHMatchUTof = (HLinearCategory*) gDirectory->Get("HMatchUTof");
  TBranch* brlHMatchUTof = T->GetBranch("HMatchUTof.");
  if(brlHMatchUTof) brlHMatchUTof->SetAddress(&catlHMatchUTof);
  TIterator *iterHMatchUTof = catlHMatchUTof->MakeIterator();

  HMatchUShower* ptlHMatchUShower = new HMatchUShower();
  HLinearCategory* catlHMatchUShower = (HLinearCategory*) gDirectory->Get("HMatchUShower");
  TBranch* brlHMatchUShower = T->GetBranch("HMatchUShower.");
  if(brlHMatchUShower) brlHMatchUShower->SetAddress(&catlHMatchUShower);
  TIterator *iterHMatchUShower = catlHMatchUShower->MakeIterator();

  HMULeptons* ptlHMULeptons = new HMULeptons();
  HLinearCategory* catlHMULeptons = (HLinearCategory*) gDirectory->Get("HMULeptons");
  TBranch* brlHMULeptons = T->GetBranch("HMULeptons.");
  if(brlHMULeptons) brlHMULeptons->SetAddress(&catlHMULeptons);
  TIterator *iterHMULeptons = catlHMULeptons->MakeIterator();

  HMUEMULeptons* ptlHMUEMULeptons = new HMUEMULeptons();
  HLinearCategory* catlHMUEMULeptons = (HLinearCategory*) gDirectory->Get("HMUEMULeptons");
  TBranch* brlHMUEMULeptons = T->GetBranch("HMUEMULeptons.");
  if(brlHMUEMULeptons) brlHMUEMULeptons->SetAddress(&catlHMUEMULeptons);
  TIterator *iterHMUEMULeptons = catlHMUEMULeptons->MakeIterator();

  f->cd();


  Int_t rRichIPU, cRichIPU, sRichIPU;
  Int_t nLeptons, leptperevt, emuleptperevt, dileptperevt_u, dileptperevt_l, dileptperevt_a;
  Float_t diffTheta, diffPhi;
  Float_t diffPhi_Rich, diffTheta_Rich, diffPhi_Meta, diffTheta_Meta, diffSquared;


  Int_t EPS_MU, EPS_EMU;

  Float_t diffList[30000][3];
  Float_t corrList[30000][7];
  Int_t nonCorrelatedIpu, nonCorrelatedAna;
  Int_t listLen, listPos, listPos2;

  TH2F *hnci=new TH2F("hnci","Non-correlated MU hits",180.,-90.,90,120.,-60.,60);
  hnci->SetOption("colz");
  hnci->GetXaxis()->SetTitle("Delta Theta - MU");
  hnci->GetYaxis()->SetTitle("Delta Phi - MU");
  TH2F *hnci_2=new TH2F("hnci_2","Non-correlated MU hits",60.,0.,60,120.,-60.,60);
  hnci_2->SetOption("colz");
  hnci_2->GetXaxis()->SetTitle("Phi Rich - MU");
  hnci_2->GetYaxis()->SetTitle("Delta Phi - MU");
  TH2F *hnca=new TH2F("hnca","Non-correlated EMU hits",180.,-90.,90,120.,-60.,60);
  hnca->SetOption("colz");
  hnca->GetXaxis()->SetTitle("Delta Theta - EMU");
  hnca->GetYaxis()->SetTitle("Delta Phi - EMU");
  TH2F *hnca_2=new TH2F("hnca_2","Non-correlated ana hits",60.,0.,60,120.,-60.,60);
  hnca_2->SetOption("colz");
  hnca_2->GetXaxis()->SetTitle("Phi Rich - EMU");
  hnca_2->GetYaxis()->SetTitle("Delta Phi - EMU");
  TH2F *hmuhits=new TH2F("hmuhits","hmuhits",60.,0.,60,120.,-60.,60);
  hmuhits->SetOption("colz");
  TH2F *hshcx=new TH2F("Delta_Phi","Delta Phi EMU vs MU",120.,-60.,60.,120.,-60.,60.);
  hshcx->SetOption("colz");
  hshcx->GetXaxis()->SetTitle("EMU");
  hshcx->GetYaxis()->SetTitle("MU");
  TH2F *hshcy=new TH2F("Delta_Theta","Delta Theta EMU vs MU",180.,-90.,90.,180.,-90.,90.);
  hshcy->SetOption("colz");
  hshcy->GetXaxis()->SetTitle("EMU");
  hshcy->GetYaxis()->SetTitle("MU");
  TH2F *hshcp=new TH2F("Momentum","p EMU vs MU",256.,0.,256.,256.,0.,256.);
  hshcp->SetOption("colz");
  hshcp->GetXaxis()->SetTitle("EMU");
  hshcp->GetYaxis()->SetTitle("MU");
  TH1F *diff=new TH1F("diff","Position difference",100,0,100);

  MULeptontype   MU_EmuLeptonHits[200];
  MULeptontype      MU_LeptonHits[200];

  Int_t MU_nLeptons;
  Int_t MU_nEmuLeptons;

  Int_t ipuword[6][96][12];
  Int_t numberOfIpuWords[6];

  // ******* preparations for event loop ******
  Stat_t ientries;  // how many events
  ientries = T->GetEntries();  // how many events
  if (nEvents>0 && nEvents<ientries)  ientries = nEvents;
  cout << ientries << " Events to process" << endl;

  //---------- Eventloop ----------------
  for (Int_t i=0; i<ientries; i++) {
    evt->GetEntry(i);
    if (i % 10000 == 0)
      cout << "Event " << i << " seq num " <<(fEventHead)->getEventSeqNumber() << endl;

    if ((fEventHead)->getId()==1)
      {
	brlHMatchURich->GetEntry(i);
	brlHMatchUTof->GetEntry(i);
	brlHMatchUShower->GetEntry(i);
	brlHMULeptons->GetEntry(i);
	brlHMUEMULeptons->GetEntry(i);

	iterHMatchURich->Reset();
	iterHMatchUTof->Reset();
	iterHMatchUShower->Reset();
	iterHMULeptons->Reset();
	iterHMUEMULeptons->Reset();

	nLeptons = leptperevt = emuleptperevt = 0;
	dileptperevt_a = dileptperevt_u = dileptperevt_l = 0;
	
	EPS_MU = EPS_EMU = 0;

	MU_nLeptons=0;
	MU_nEmuLeptons=0;

	Int_t TotalNumberOfRichIpuWords = 0;
	Int_t TotalNumberOfTofHits = 0;
	Int_t TotalNumberOfShowerIpuWords = 0;
	Int_t ipuwordSho[6][32][2];
      
	for (Int_t ii =0; ii<6; ii++) {
	  numberOfIpuWords[ii]=0;
	  for (Int_t iic = 0; iic<32; iic++)
	    {
	      for (Int_t iim = 0; iim<2; iim++)
		ipuwordSho[ii][iic][iim] = 0;
	    }
	  for (Int_t iic = 0; iic<96; iic++)
	    {
	      for (Int_t iim = 0; iim<12; iim++)
		ipuword[ii][iic][iim] = 0;
	    }
	}
	
	//// RICH
	while ( (ptlHMatchURich = (HMatchURich *)iterHMatchURich->Next() ) != 0  )
	  {
	    rRichIPU = ptlHMatchURich->getRow();
	    cRichIPU = ptlHMatchURich->getColumn();
	    sRichIPU = ptlHMatchURich->getSegmentId();
	    
	    ipuword[sRichIPU][rRichIPU ][(Int_t)cRichIPU/8]++;
	    if ( ipuword[sRichIPU][rRichIPU][(Int_t)cRichIPU/8] == 1 )
	      {
		numberOfIpuWords[sRichIPU]++;
		TotalNumberOfRichIpuWords++;
	      }
	  
	    if(verbose) cout << "RICH: sec " << sRichIPU << " theta " << ptlHMatchURich->getTheta() << " phi "<< ptlHMatchURich->getPhi() << " row " << rRichIPU << " col " << cRichIPU <<  endl;
	  } // End while
	if (TotalNumberOfRichIpuWords>=6) 
	  continue;

	while ((ptlHMatchUTof = (HMatchUTof *)iterHMatchUTof->Next()) != 0) {
	  if(verbose) cout << "Tof: sec " << ptlHMatchUTof->getSector() << " theta " << ptlHMatchUTof->getTheta() \
	       << " phi " << ptlHMatchUTof->getPhi() << " time " << ptlHMatchUTof->getTime()
	       << " PID " << ptlHMatchUTof->getPID() << endl;
	  TotalNumberOfTofHits++;
	}
 Int_t rShoIPU, cShoIPU, sShoIPU;
	while ((ptlHMatchUShower = (HMatchUShower *)iterHMatchUShower->Next()) != 0) {
	  if(verbose) cout << "Shower: sec " << ptlHMatchUShower->getSector() << " theta " << ptlHMatchUShower->getTheta() \
	       << " phi " << ptlHMatchUShower->getPhi() << endl;

	  rShoIPU = ptlHMatchUShower->getRow();
	  cShoIPU = ptlHMatchUShower->getColumn();
	  sShoIPU = ptlHMatchUShower->getSector();

	  ipuwordSho[sShoIPU][rShoIPU][(Int_t)cShoIPU/16]++;
	  if (ipuwordSho[sShoIPU][rShoIPU][(Int_t)cShoIPU/16]==1)
	    {
	      TotalNumberOfShowerIpuWords++;
	    }

	}

      if (TotalNumberOfRichIpuWords * (TotalNumberOfShowerIpuWords+TotalNumberOfTofHits)>=50) continue;

//      cout << endl;

      Int_t MaxWords = 8;
      Int_t toomanyringsinevt=0;
      Int_t toomanyringsinsec[6];



      for (Int_t ii =0; ii<6; ii++) {
	toomanyringsinsec[ii] =0;
	if ( numberOfIpuWords[ii]>= MaxWords ) {
	  //cout << " numberOfIpuWords: " <<  numberOfIpuWords[ii] << endl;
	  toomanyringsinsec[ii] =1;
	  //cout << "toomanyringsinsec[" << ii << "]= " << toomanyringsinsec[ii] << endl;
	}

	if (toomanyringsinsec[ii]){
	  toomanyringsinevt =1;
	  //cout << "toomanyringsinevt " << toomanyringsinevt << endl;
	}
      }

      if (toomanyringsinevt){
	cout << "skip the evt (toomanyringsinevt)" << endl;
	continue;
	// if the rich has exceeded the max number of data words, the matching unit triggers the event positively,
	// without even trying a match
	// therefore we skip the event without trying a correlation with the emulation
      }

      Float_t phi = 0.; 
      //// LEPTONS
      while (( ptlHMULeptons  = (HMULeptons *)iterHMULeptons->Next()) != 0) {
	Int_t sector = ptlHMULeptons->getSector();

	MU_LeptonHits[MU_nLeptons].sec = sector;

	//// rich in LEPTON
	MU_LeptonHits[MU_nLeptons].rth = ptlHMULeptons->getThetaRich();
	MU_LeptonHits[MU_nLeptons].rph = ptlHMULeptons->getPhiRich();
	if (sector<5)
	  MU_LeptonHits[MU_nLeptons].rph -= 60*(sector+1);

	//// meta in LEPTON
	MU_LeptonHits[MU_nLeptons].mth = ptlHMULeptons->getThetaMeta();
	MU_LeptonHits[MU_nLeptons].mph = ptlHMULeptons->getPhiMeta(); 
	if (sector<5)
	  MU_LeptonHits[MU_nLeptons].mph -= 60*(sector+1);

	MU_LeptonHits[MU_nLeptons].p = ptlHMULeptons->getMom();
	MU_LeptonHits[MU_nLeptons].eleflag = ptlHMULeptons->getFlag();
	MU_LeptonHits[MU_nLeptons].DP = MU_LeptonHits[MU_nLeptons].rph-MU_LeptonHits[MU_nLeptons].mph;
	MU_LeptonHits[MU_nLeptons].DT = MU_LeptonHits[MU_nLeptons].rth-MU_LeptonHits[MU_nLeptons].mth;
	MU_LeptonHits[MU_nLeptons].MF = 0;

	diffTheta = MU_LeptonHits[MU_nLeptons].DT;
	diffPhi = MU_LeptonHits[MU_nLeptons].DP;


	phi = MU_LeptonHits[MU_nLeptons].rph;
	if (sector < 5) phi -= 60*(sector+1);
	hmuhits->Fill(phi,MU_LeptonHits[MU_nLeptons].DP);

	if(verbose){
		 cout << i <<  " seq num " <<(fEventHead)->getEventSeqNumber() <<
		 	"   ****************Found Lepton in MU." << endl;
		cout << "Lept:  sec " << ptlHMULeptons->getSector()
	     		<< "  detector bit  " << ptlHMULeptons->getDetBit() << endl;
		cout << "RICH: phi " << MU_LeptonHits[MU_nLeptons].rph
	     		<< "  theta  " << ptlHMULeptons->getThetaRich() << " nr " << ptlHMULeptons->getRichNr() << endl;
		cout << "META: phi  " <<  MU_LeptonHits[MU_nLeptons].mph
	     		<< "  theta  " <<   ptlHMULeptons->getThetaMeta() << " nr " << ptlHMULeptons->getMetaNr()
	     		<< "  momentum  " << MU_LeptonHits[MU_nLeptons].p << endl;
		cout << "***********************PHI DIFFERENCE " << diffPhi << endl;
	}

	//    if (diffTheta>0&&MU_LeptonHits[MU_nLeptons].eleflag==1) cout << "***************************evt " << i
	// 								<<"WRONG ELE FLAG!!!" << endl;
	//    if (diffTheta<0&&MU_LeptonHits[MU_nLeptons].eleflag==0) cout << "***************************evt " << i
	// 								<< "WRONG ELE FLAG!!!" << endl;
	//    cout << "---------------------------------------" << endl;

	MU_nLeptons++;
	leptperevt++;
      }
//      cout << endl;
//      cout << endl;

      // --------------------- EMU LEPTONS ------------------------------------
      while ((ptlHMUEMULeptons = (HMUEMULeptons *)iterHMUEMULeptons->Next()) != 0) {  // get allemu leptons
	if (MU_nEmuLeptons<7) {
	  MU_EmuLeptonHits[MU_nEmuLeptons].sec  = ptlHMUEMULeptons->getSector();
	  MU_EmuLeptonHits[MU_nEmuLeptons].p    = ptlHMUEMULeptons->getMom();
	  MU_EmuLeptonHits[MU_nEmuLeptons].rth  = ptlHMUEMULeptons->getThetaRich();
	  MU_EmuLeptonHits[MU_nEmuLeptons].rph  = ptlHMUEMULeptons->getPhiRich();
	  MU_EmuLeptonHits[MU_nEmuLeptons].mth  = ptlHMUEMULeptons->getThetaMeta();
	  MU_EmuLeptonHits[MU_nEmuLeptons].mph  = ptlHMUEMULeptons->getPhiMeta();
	  MU_EmuLeptonHits[MU_nEmuLeptons].eleflag = ptlHMUEMULeptons->getFlag();
	  MU_EmuLeptonHits[MU_nEmuLeptons].DP = MU_EmuLeptonHits[MU_nEmuLeptons].rph-MU_EmuLeptonHits[MU_nEmuLeptons].mph;
	  MU_EmuLeptonHits[MU_nEmuLeptons].DT = MU_EmuLeptonHits[MU_nEmuLeptons].rth-MU_EmuLeptonHits[MU_nEmuLeptons].mth;
	  MU_EmuLeptonHits[MU_nEmuLeptons].MF = 0;


	if(verbose){
	  cout << i << " seq num " <<(fEventHead)->getEventSeqNumber() <<"   ****************Found Lepton in MU- EMULATION." << endl;
	  cout << "Lept:  sec "<< ptlHMUEMULeptons->getSector() << endl;
	  cout << "RICH: phi " << ptlHMUEMULeptons->getPhiRich()
	       << "  theta  "  << ptlHMUEMULeptons->getThetaRich() << endl;
	  cout << "META: phi " << ptlHMUEMULeptons->getPhiMeta()
	       << "  theta  "  << ptlHMUEMULeptons->getThetaMeta()
	       << "  momentum  " << MU_EmuLeptonHits[MU_nEmuLeptons].p << endl;

	  cout << "***********************PHI DIFFERENCE " << MU_EmuLeptonHits[MU_nEmuLeptons].DP << endl;
	  cout << "---------------------------------------" << endl;
	}
	  MU_nEmuLeptons++;
	  emuleptperevt++;
	}
      }

//      cout << endl;



      //- ************************** Correlate closest ipu/ana pairs *****************************

      // MU_LeptonHits[i][] contains hits from real hardware
      // MU_EmuLeptonHits[i][] contains hits from offline analysis

      listLen = 0;   // current number of array elements in difflist
      listPos = 0;

      Int_t actAna, actIpu;    
      //-** Create sorted list (diffList) of squared distances for all ipu-ana-hit combinations **
      //     Sorted by increasing squared distance

      // Reset difflist (all array elements set to zero)
      for ( Int_t k=0; k<30000;k++) { //100 arbitrary value,1600 would be consistent
	for(Int_t kkk=0; kkk<3; kkk++)
	  { diffList[k][kkk] = 0;
	  }
	for(Int_t kkk=0; kkk<7; kkk++)
	  { corrList[k][kkk] = 0;
	  }
      }

      if (leptperevt>30) continue; // i do not any correlation for events with cutoff

      for( actAna=0; actAna<emuleptperevt;actAna++) {    // loop over  softhits
	for( actIpu=0; actIpu<leptperevt;actIpu++) {   // loop over  hardhits

	  //       if ( (MU_LeptonHits[actIpu].sec == MU_EmuLeptonHits[actAna].sec ) &&
	  //  	  (MU_LeptonHits[actIpu].sec>1) ) {

	  diffPhi_Rich   = MU_LeptonHits[actIpu].rph - MU_EmuLeptonHits[actAna].rph;
	  diffTheta_Rich = MU_LeptonHits[actIpu].rth - MU_EmuLeptonHits[actAna].rth;
	  diffPhi_Meta   = MU_LeptonHits[actIpu].mph - MU_EmuLeptonHits[actAna].mph;
	  diffTheta_Meta = MU_LeptonHits[actIpu].mth - MU_EmuLeptonHits[actAna].mth;
	  diffSquared    = diffPhi_Rich*diffPhi_Rich + diffTheta_Rich*diffTheta_Rich +
	    diffPhi_Meta*diffPhi_Meta + diffTheta_Meta*diffTheta_Meta;   // squared distance ana-ipu
	  diff->Fill(diffSquared);
	  //      cout << "Calc " << listPos << " " << diffSquared << " " << actAna
	  // 	  << " " << actIpu << endl; 
	  // Now determine in which position (listPos) the new value 
	  // must be placed in the SORTED list
	  if ( listLen == 0 ) {
	    listPos = 0;  // special case: first entry goes to position zero in difflist
	  } else {
	    // loop over existing entries in difflist until new value
	    // diffSquared is bigger
	    while ( (diffSquared > diffList[listPos][0]) && ( listPos < listLen) ) {
	      listPos++;
	    }
	    // now we found the position where the new array element 
	    // has to be placed in difflist
	    // and we copy all following element to the next position 
	    // in order to get a free array element for the new entry
	    for(Int_t listPos2=listLen; listPos2>listPos;listPos2--) {
	      diffList[listPos2][0] = diffList[listPos2-1][0];
	      diffList[listPos2][1] = diffList[listPos2-1][1];
	      diffList[listPos2][2] = diffList[listPos2-1][2];
	    }
	  }                                                 

	  // Now we can fill the new array elements at position listPos

	  listLen++;       // increase number of array elements by one

	  // diffList contains:


	  //      cout << "Fill " << listPos << " " << diffSquared << " "
	  // 	  << actAna << " " << actIpu << endl; 
	  diffList[listPos][0] = diffSquared;    // squared distance
	  diffList[listPos][1] = actAna;  //  corresponding ana ring (MU_EmuLeptonHits)
	  diffList[listPos][2] = actIpu;  //  corresponding ipu ring (MU_LeptonHits)
	  listPos = 0;

	  //} // end of sector condition
	} // end of loop over hardhits
      } // end of loop over softhits 


	//- ** Now we can start to correlate ipu and ana rings based on diffList

	//-** group ipu-ana hit pairs with smallest distances **
      nonCorrelatedIpu = leptperevt;
      nonCorrelatedAna = emuleptperevt;
      //  cout << " non correlated: " << nonCorrelatedIpu << " " << nonCorrelatedAna << endl;

      listPos = 0; listPos2 = 0;
      //  while ( ( nonCorrelatedIpu > 0 ) && ( nonCorrelatedAna > 0 ) && ( listPos<listLen) ) {
      actAna = diffList[listPos][1];
      actIpu = diffList[listPos][2];
      //     cout << "act ana " << actAna << MU_EmuLeptonHits[actAna].rth << "  " << MU_EmuLeptonHits[actAna].rph << endl;
      //     cout << "act ipu " << actIpu << MU_LeptonHits[actIpu].rth << "  " << MU_LeptonHits[actIpu].rph << endl;
      //     cout << "matching flag: " << MU_LeptonHits[actIpu].MF << "  " << MU_EmuLeptonHits[actAna].MF << endl;

      if ( (MU_LeptonHits[actIpu].MF==0) && (MU_EmuLeptonHits[actAna].MF==0) ) {
	//      cout << "matching flag: " << MU_LeptonHits[actIpu].MF << "  " << MU_EmuLeptonHits[actAna].MF << endl;
	corrList[listPos2][1] = MU_EmuLeptonHits[actAna].DP;
	corrList[listPos2][2] = MU_EmuLeptonHits[actAna].DT;
	corrList[listPos2][3] = MU_LeptonHits[actIpu].DP;
	corrList[listPos2][4] = MU_LeptonHits[actIpu].DT;
	corrList[listPos2][5] = MU_EmuLeptonHits[actAna].p;
	corrList[listPos2][6] = MU_LeptonHits[actIpu].p;

	if (  fabs(MU_EmuLeptonHits[actAna].DT-MU_LeptonHits[actIpu].DT) > 0.1 ||
	      fabs(MU_EmuLeptonHits[actAna].DP-MU_LeptonHits[actIpu].DP) > 0.1 ) {
	  cout << "*********************evt " << i << " seq num " <<(fEventHead)->getEventSeqNumber() << endl;
	  cout << "  BAD CORRELATION: Delta phi Emu " << corrList[listPos2][1]
	       << "  Delta theta Emu "  << corrList[listPos2][2] << "  Delta phi MU "
	       << corrList[listPos2][3] << "  Delta theta MU "  << corrList[listPos2][4] << endl;
	}

	listPos2++;
	MU_EmuLeptonHits[actAna].MF++;
	MU_LeptonHits[actIpu].MF++;
	nonCorrelatedIpu--; nonCorrelatedAna--;	  
	//cout << " NCI: " << nonCorrelatedIpu << "  NCA " << nonCorrelatedAna << endl;
      }
      listPos++;
      //}

      //cout << " non correlated: " << nonCorrelatedIpu << " " << nonCorrelatedAna << endl;

      //-** group all remaining ipu (or ana) hits with (0,0) **
      /*
	for( actAna=0; actAna<leptperevt; actAna++ ) {
	if ( MU_EmuLeptonHits[actAna].MF==0 ) {

	Float_t phi_Rich = MU_EmuLeptonHits[actAna].rph;
	if (MU_EmuLeptonHits[actAna].sec < 5)
	phi_Rich -= 60* (MU_EmuLeptonHits[actAna].sec+1);

	corrList[listPos2][1] = MU_EmuLeptonHits[actAna].DP;
	corrList[listPos2][2] = MU_EmuLeptonHits[actAna].DT;
	corrList[listPos2][3] = 0;
	corrList[listPos2][4] = 0;
	hnca->Fill((Float_t)corrList[listPos2][2],(Float_t)corrList[listPos2][1],(Stat_t)1);
	hnca_2->Fill((Float_t) phi_Rich,corrList[listPos2][1],(Stat_t)1);

	//if ( fabs ( fabs(corrList[listPos2][1]) - (8+0.25* fabs(phi_Rich-30) )  ) >0.5    ) {
	cout << "*********************evt " << i <<  " seq num " <<(fEventHead)->getEventSeqNumber() <<endl;
	cout << "  NON CorrAna: Delta phi " << corrList[listPos2][1]
	<< "  Delta theta " <<corrList[listPos2][2] << " " << corrList[listPos2][3] << " " 
	<< corrList[listPos2][4] << " phi rich " << MU_EmuLeptonHits[actAna].rph << endl;
	cout << endl;
	//}
	listPos2++;
	}
	}



	for( actIpu=0; actIpu<leptperevt;actIpu++) {
	if ( MU_LeptonHits[actIpu].MF==0 ) {

	Float_t phi_Rich = MU_LeptonHits[actIpu].rph;
	if (MU_LeptonHits[actIpu].sec < 5)
	phi_Rich -= 60* (MU_LeptonHits[actIpu].sec+1);

	corrList[listPos2][1] = 0;
	corrList[listPos2][2] = 0;
	corrList[listPos2][3] = MU_LeptonHits[actIpu].DP;
	corrList[listPos2][4] = MU_LeptonHits[actIpu].DT;
	hnci->Fill((Float_t)corrList[listPos2][4],(Float_t)corrList[listPos2][3],(Stat_t)1);
	hnci_2->Fill(phi_Rich,(Float_t)corrList[listPos2][3],(Stat_t)1);

	if ( fabs ( fabs(corrList[listPos2][3]) - (8+0.25* fabs(phi_Rich-30) )  ) >0.5    ) {
	cout << "*********************evt " << i <<  " seq num " <<(fEventHead)->getEventSeqNumber() <<endl;
	cout << "  NON CorrIPU: Delta phi " << corrList[listPos2][3]
	<< "  Delta theta "  << corrList[listPos2][4] << " " << corrList[listPos2][1] << " " 
	<< corrList[listPos2][2] << endl;
	cout << endl;
	}
	listPos2++;
	}
	}
      */

      //    cout << " ListPos2: " << listPos2 << endl;
      //-** fill histograms from list of correlated pairs (corrList) **
      for ( listPos = 0; listPos<listPos2; listPos++ ) {
	//     cout << "Corr " << corrList[listPos][1] << " "
	// 	 << corrList[listPos][2] << " " << corrList[listPos][3] << " "   
	// 	 << corrList[listPos][4] << endl;
	hshcx->Fill((Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3],(Stat_t)1);
	hshcy->Fill((Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4],(Stat_t)1);
	hshcp->Fill((Float_t) corrList[listPos][5],(Float_t) corrList[listPos][6],(Stat_t)1);
	if (corrList[listPos][5]!=corrList[listPos][6]) {
	  cout << "***************** UNCORRELATED MOMENTUM in evt " << i << endl;
	  cout << corrList[listPos][5] << " " << corrList[listPos][6] << endl;
	}
	if ((corrList[listPos][1]!=corrList[listPos][3]) || (corrList[listPos][2]!=corrList[listPos][4])) {
	  cout << "***************** UNCORRELATED ANGLE in evt " << i << endl;
	  cout << corrList[listPos][1] << " " << corrList[listPos][3] << "----"
	       << corrList[listPos][1] << " " << corrList[listPos][3] << endl;
	}

      }

      //- ************************* End of Correlation *****************************************



      catlHMatchURich->Clear();
      catlHMatchUTof->Clear();
      catlHMatchUShower->Clear();
      catlHMULeptons->Clear();
      catlHMUEMULeptons->Clear();

    } // evtid
  } // eventloop 


  hfile->cd();

  hnci->Write();
  hnci_2->Write();
  hnca->Write();
  hnca_2->Write();
  hmuhits->Write();
  hshcx->Write();
  hshcy->Write();
  hshcp->Write();
  diff->Write();

  hfile->Close();

  cout << outFile << endl;

  return (1);


}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("mu-muIPU_corr_new","compiled  mu muIPU new correlation macro");
        gROOT->SetBatch(kTRUE);

 Bool_t verbose=false;
 Int_t cur_arg=1;
	while(cur_arg<argc){
		if(argv[cur_arg][0]!='-') break;
		for(Int_t i=1; 1; i++){
			if(argv[cur_arg][i]==0) break;
			switch(argv[cur_arg][i]){
				case 'v':
					verbose=true;
					cout << "verbosity is ON"<<endl;
					break;
				default:
					cout << "unknown argument \""<<argv[cur_arg][i]<<"\""<<endl;
		        	        exit(1);
			}
		}
		cur_arg++;
	}
	switch( argc-cur_arg)
        {
		case 1:
			ana_emu_newcut(argv[cur_arg],0,verbose);
			exit(0);
		case 2:
			ana_emu_newcut(argv[cur_arg],atoi(argv[cur_arg+1]),verbose);
			exit(0);
		default:
	                cout << argv[0] << " [-v] pathfilename [number of events]" << endl;
	                cout <<  "-v  be verbose" << endl;
        	        exit(1);
        }
}
#endif
