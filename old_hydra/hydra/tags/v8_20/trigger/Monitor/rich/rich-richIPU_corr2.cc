///////////////////////////////////////////////////////////////////////////
// RICH Algo (HRichHitIPU) - RICH IPU (HMatchURich) correlation sector-wise
// 1:1 correlation
// load the library  > .L richAlgo-richIPU_corr.cc+
// call the function > cor_algo2()
///////////////////////////////////////////////////////////////////////////

#ifndef __CINT__
#include <iostream.h>
#include <unistd.h>
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TCanvas.h"
//#include "/u/alberica/hydra/trigger/hmatchurich.h"
//#include "/u/alberica/hydra/triggerutil/hrichhitIPU.h"
#include "hmatchurich.h"
#include "hrichhitIPU.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "htree.h"
#include "hrecevent.h"
#include "hevent.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "hcategory.h"
#include "hdetector.h"
#include "haddef.h"
#include "hrichraw.h"
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
   Int_t X;
   Int_t Y;
   Int_t MF;
 } RichIPUtype;

 typedef struct {
   Int_t X;
   Int_t Y;
   Int_t RQ;
   Int_t VQ;
   Int_t MF;
 } RichAlgotype;

Int_t ana_rich(TString inputFile,Int_t nEvents=0,Bool_t verbose=false)
{
//  gROOT->Reset();

  TH2F *hallipu[6];
  TH2F *hallana[6];
  TH2F *hnci[6];
  TH2F *hnca[6];
  TH2F *hncii[6];
  TH2F *hncaa[6];
  TH2F *hshcx[6];
  TH2F *hshcy[6];
  TH2F *hcorr[6];
  TH1F *diff[6];
  TH1F *hNIHits[6];
  TH1F *hNAHits[6];
  TH1F *hNIHitscut[6];
  TH1F *hNAHitscut[6];
  TH1F *CRQuality[6];
  TH1F *NCRQuality[6];
  TH1F *CRVetoQuality[6];
  TH1F *NCRVetoQuality[6];
  TH2F *RingVeto;
  TH2F *NCRingVeto;
  TH1F *hdiscrepancy[6];
  TH1F *htipuhits[6];
  TH1F *htanahits[6];

  TH2F *hiaVax[6];
  TH2F *hiaVay[6];


  for (Int_t ii =0; ii<6; ii++){
    Char_t buffer[255];
    Char_t title[255];
    sprintf(buffer,"hallipu%i",ii);
    sprintf(title,"IPU hits in sector %i",ii);
    hallipu[ii]  =new TH2F(buffer,title,100,0.,100.,100,0.,100.); // Plot of all IPU hits per sector
    hallipu[ii]->SetOption("colz");
    hallipu[ii]->SetXTitle("x");
    hallipu[ii]->SetYTitle("y");
    sprintf(buffer,"hallana%i",ii);
    sprintf(title,"Ana hits in sector %i",ii);
    hallana[ii] =new TH2F(buffer,title,100,0.,100.,100,0.,100.); // Plot of all Ana hits per sector
    hallana[ii]->SetOption("colz");
    hallana[ii]->SetXTitle("x");
    hallana[ii]->SetYTitle("y");

    sprintf(buffer,"hnci%i",ii);
    sprintf(title,"Not correlated IPU hits in sector %i",ii);
    hnci[ii]  =new TH2F(buffer,title,100,0.,100.,100,0.,100.);  // Rings found by IPU & ! by analisys
    hnci[ii]->SetOption("colz");
    hnci[ii]->SetXTitle("x");
    hnci[ii]->SetYTitle("y");
    sprintf(buffer,"hnca%i",ii);
    sprintf(title,"Not correlated Ana hits in sector %i",ii);
    hnca[ii] =new TH2F(buffer,title,100,0.,100.,100,0.,100.);  // Rings found by Ana & ! by analisys
    hnca[ii]->SetOption("colz");
    hnca[ii]->SetXTitle("x");
    hnca[ii]->SetYTitle("y");

    sprintf(buffer,"hncii%i",ii);
    sprintf(title,"Not correlated IPU hits in sector %i with diff>2",ii);
    hncii[ii]  =new TH2F(buffer,title,100,0.,100.,100,0.,100.);  // Rings found by IPU & ! by analisys with diff>2
    hncii[ii]->SetOption("colz");                                // NeighborFlag = 0
    hncii[ii]->SetXTitle("x");
    hncii[ii]->SetYTitle("y");
    sprintf(buffer,"hncaa%i",ii);
    sprintf(title,"Not correlated Ana hits in sector %i with diff>2",ii);
    hncaa[ii] =new TH2F(buffer,title,100,0.,100.,100,0.,100.);  // Rings found by Ana & ! by analisys with diff>2
    hncaa[ii]->SetOption("colz");
    hncaa[ii]->SetXTitle("x");
    hncaa[ii]->SetYTitle("y");

    sprintf(buffer,"hshcx%i",ii);
    sprintf(title,"X-correlation in sector %i",ii);
    hshcx[ii] =new TH2F(buffer,title,100,0.,100.,100,0.,100.);
    hshcx[ii]->SetOption("colz");
    hshcx[ii]->SetXTitle("Ana");
    hshcx[ii]->SetYTitle("IPU");
    sprintf(buffer,"hshcy%i",ii);
    sprintf(title,"Y-correlation in sector %i",ii);
    hshcy[ii] =new TH2F(buffer,title,100,0.,100.,100,0.,100.);
    hshcy[ii]->SetOption("colz");
    hshcy[ii]->SetXTitle("Ana");
    hshcy[ii]->SetYTitle("IPU");

    sprintf(buffer,"hcorr%i",ii);
    sprintf(title,"Correlated IPU hits in sector %i",ii);
    hcorr[ii] =new TH2F(buffer,title,100,0.,100.,100,0.,100.);  // Plot of correlated IPU hits per sector
    hcorr[ii]->SetOption("colz");
    hcorr[ii]->SetXTitle("Ana");
    hcorr[ii]->SetYTitle("IPU");
    // (x_ipu-x_ana)+(y_ipu-y_ana) per event and sect for every comb. ana-ipu hit
    sprintf(buffer,"diff%i",ii);
    sprintf(title,"(x_ipu-x_ana)+(y_ipu-y_ana) in sector %i",ii);
    diff[ii]  =new TH1F(buffer,title,400,0,400);
    diff[ii]->SetXTitle("Event");
    diff[ii]->SetYTitle("diff");

    sprintf(buffer,"hNIHits%i",ii);
    sprintf(title,"IPU hits/ev in sector %i",ii);
    hNIHits[ii] = new TH1F(buffer,title,100,0,100);  // IPU Hits/ev per sector
    hNIHits[ii]->SetXTitle("Event");
    hNIHits[ii]->SetYTitle("IPU hits");
    sprintf(buffer,"hNAHits%i",ii);
    sprintf(title,"Ana hits/ev in sector %i",ii);
    hNAHits[ii] = new TH1F(buffer,title,100,0,100);  // Ana Hits/ev per sector
    hNAHits[ii]->SetXTitle("Event");
    hNAHits[ii]->SetYTitle("Ana hits");

    sprintf(buffer,"hNIHitscut%i",ii);
    sprintf(title,"IPU hits/ev in sector %i with Ana hits/ev = 1",ii);
    hNIHitscut[ii] = new TH1F(buffer,title,100,0,100);  // IPU Hits/ev per sector if Ana hits = 1
    hNIHitscut[ii]->SetXTitle("Event");
    hNIHitscut[ii]->SetYTitle("IPU hits");
    sprintf(buffer,"hNAHitscut%i",ii);
    sprintf(title,"Ana hits/ev in sector %i with IPU hits/ev = 1",ii);
    hNAHitscut[ii] = new TH1F(buffer,title,100,0,100);  // Ana Hits/ev per sector if IPU hits = 1
    hNAHitscut[ii]->SetXTitle("Event");
    hNAHitscut[ii]->SetYTitle("IPU hits");

    sprintf(buffer,"CRQuality%i",ii);
    sprintf(title,"Correlated rings - Ring quality %d",ii);
    CRQuality[ii] = new TH1F(buffer,title,20,0,20); // Correlated rings quality
    CRQuality[ii]->SetXTitle("Event");
    CRQuality[ii]->SetYTitle("Ring quality (pads)");
    sprintf(buffer,"NCRQuality%i",ii);
    sprintf(title,"Not correlated rings - Ring quality %d",ii);
    NCRQuality[ii] = new TH1F(buffer,title,20,0,20); // Not Correlated rings quality
    NCRQuality[ii]->SetXTitle("Event");
    NCRQuality[ii]->SetYTitle("Ring quality (pads)");
    sprintf(buffer,"CRVetoQuality%i",ii);
    sprintf(title,"Correlated rings - Veto quality %d",ii);
    CRVetoQuality[ii] = new TH1F(buffer,title,20,0,20); // Correlated rings - Veto quality
    CRVetoQuality[ii]->SetXTitle("Event");
    CRVetoQuality[ii]->SetYTitle("Veto quality (pads)");
    sprintf(buffer,"NCRVetoQuality%i",ii);
    sprintf(title,"Not correlated rings - Veto quality %d",ii);
    NCRVetoQuality[ii] = new TH1F(buffer,title,6,0,6); // Not Correlated rings - Veto quality
    NCRVetoQuality[ii]->SetXTitle("Event");
    NCRVetoQuality[ii]->SetYTitle("Veto quality (pads)");

    sprintf(buffer,"hdiscrepancy%i",ii);
    hdiscrepancy[ii] = new TH1F(buffer,buffer,2550,0,255000);
    sprintf(buffer,"htipuhits%i",ii);
    htipuhits[ii] = new TH1F(buffer,buffer,2550,0,255000);
    sprintf(buffer,"htanahits%i",ii);
    htanahits[ii] = new TH1F(buffer,buffer,2550,0,255000);

    // To select events that are not perfectly correlated
    sprintf(buffer,"hipuanaVanaX%i",ii);
    sprintf(title,"X_IPU/X_Ana vs X_Ana - Sector %i",ii);
    hiaVax[ii]  =new TH2F(buffer,title,100,0.,100.,100,0.,2.);
    hiaVax[ii]->SetOption("colz");
    hiaVax[ii]->SetXTitle("Ana");
    hiaVax[ii]->SetYTitle("IPU/Ana");
    sprintf(buffer,"hipuanaVanaY%i",ii);
    sprintf(title,"Y_IPU/Y_Ana vs Y_Ana - Sector %i",ii);
    hiaVay[ii]  =new TH2F(buffer,title,100,0.,100.,100,0.,2.);
    hiaVay[ii]->SetOption("colz");
    hiaVay[ii]->SetXTitle("Ana");
    hiaVay[ii]->SetYTitle("IPU/Ana");
  }
  RingVeto =new TH2F("RingVeto","RingVeto",20,0.,20.,6.,0.,6.);
  RingVeto->SetOption("colz");
  NCRingVeto =new TH2F("NCRingVeto","NCRingVeto",20,0.,20.,6.,0.,6.);
  NCRingVeto->SetOption("colz");

  Int_t Tot_allfiles_mdcmatchMETA , Tot_allfiles_mdcmatchRich, Tot_allfiles_mdcmatchIPU;
  Tot_allfiles_mdcmatchMETA = Tot_allfiles_mdcmatchRich= Tot_allfiles_mdcmatchIPU=0;

  TString outFile="";
  process_filename(inputFile,outFile,"_richCORR");

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
  if (catlHMatchURich==NULL)
  	{
	cout << "HMatchURich category not found!!\nExiting!!!!" << endl;
	exit(EXIT_FAILURE);
	}
  TBranch* brlHMatchURich = T->GetBranch("HMatchURich.");
  if(brlHMatchURich) brlHMatchURich->SetAddress(&catlHMatchURich);
  TIterator *iterHMatchURich = catlHMatchURich->MakeIterator();

  HRichHitIPU* ptlHRichHitIPU = new HRichHitIPU();
  HLinearCategory* catlHRichHitIPU = (HLinearCategory*) gDirectory->Get("HRichHitIPU");
  if (catlHRichHitIPU==NULL)
  	{
	cout << "HRichHitIPU category not found!!\nExiting!!!!" << endl;
	exit(EXIT_FAILURE);
	}
  TBranch* brlHRichHitIPU = T->GetBranch("HRichHitIPU.");
  if(brlHRichHitIPU) brlHRichHitIPU->SetAddress(&catlHRichHitIPU);
  TIterator *iterHRichHitIPU = catlHRichHitIPU->MakeIterator();


  Int_t maxWordIPU[6];
  Int_t maxWordANA[6];
  Int_t ipuword[6][96][12];
  Int_t numberOfIpuWords[6];
  Int_t discrepancy[6];
  Int_t ipuHitsSec[6];  // Number of hits from the IPU per sector
  Int_t anaHitsSec[6];  // Number of hits from the ana per sector

  Int_t skipEventFlag;

  RichIPUtype  ipucoord[6][1000];  // Contains all IPU rings coordinates (HMatchURich)
  RichAlgotype anacoord[6][1000];  // Contains all ana rings coordinates (HRichHitIPU) and quality for veto e ring

  Int_t myX, myY, mySect;

  Int_t diffX, diffY, diffSquared;
  Int_t diffList[15000][3];
  Int_t diffMatrix[400][400];
  Int_t SqrDiff;
  Int_t corrList[15000][5];
  Int_t nonCorrelatedIpu, nonCorrelatedAna;
  Int_t listLen, listPos, listPos2;
  Int_t actAna, actIpu;
  Int_t ana_sum, mu_sum;  // Total number of hits (for all events and sectors)

  Int_t n16chMis = 0;

  ana_sum = 0;
  mu_sum = 0;
// ******* preparations for event loop ******
  Stat_t ientries;  // how many events
  ientries = T->GetEntries();  // how many events
  if (nEvents>0 && nEvents<ientries)  ientries = nEvents;
  cout << ientries << " Events to process" << endl;
// ******** EventLoop ************

  for (Int_t i=0; i<ientries;i++) {
    T->GetEvent(i);
    evt->GetEntry(i);
    if ( (i % 10000) == 0) cout << " ----" << i<< endl;
    //cout << "IPUEntries: " << mu_hits << endl;

    if ((fEventHead)->getId()==1){

    for (Int_t j=0;j<6;j++) {
      numberOfIpuWords[j] = 0;
      for(Int_t ii=0;ii<96;ii++)
	for(Int_t jj=0;jj<12;jj++)
	  ipuword[j][ii][jj]=0;
    }
    
    for (Int_t j=0;j<6;j++) {
      discrepancy[j]=0;
      ipuHitsSec[j] = 0;
      anaHitsSec[j] = 0;
      maxWordIPU[j] = 0;
      maxWordANA[j] = 9999;
    }
    
    iterHRichHitIPU->Reset();
    iterHMatchURich->Reset();    

    skipEventFlag = 0;       
   
    while ((ptlHMatchURich = (HMatchURich *)iterHMatchURich->Next()) != 0) {  // get all IPU rings   
      
      myX = ptlHMatchURich->getRow();  // -3 for new FPGA design (Jan04) !!!!!!
      myY = ptlHMatchURich->getColumn();
      mySect = ptlHMatchURich->getSegmentId();
      
      ipucoord[mySect][ipuHitsSec[mySect]].X=myX; 
      ipucoord[mySect][ipuHitsSec[mySect]].Y=myY;  
      ipucoord[mySect][ipuHitsSec[mySect]].MF=0;
      hallipu[mySect]->Fill(myX,myY);
      
      ipuword[mySect][myX][(Int_t)myY/8]++;
      if (ipuword[mySect][myX][(Int_t)myY/8]==1)
	numberOfIpuWords[mySect]++;
      
      if (numberOfIpuWords[mySect]==8) skipEventFlag++;

      if(verbose) cout << "EVT " << i << " IPURing at x=" <<ipucoord[mySect][ipuHitsSec[mySect]].X  << " y="
	   << ipucoord[mySect][ipuHitsSec[mySect]].Y << " sect="  << mySect  <<endl;

       if (maxWordIPU[mySect] < 100*myX + myY)
	 maxWordIPU[mySect]=100*myX + myY;
       ipuHitsSec[mySect]++;
    }
    

//     if (skipEventFlag) {
//       cout << "Skip Event " << i << endl;
//       continue;
//     }


    Int_t RingsumPattern[6][96][96];  // Contains the quality of the ring for each sector and each ring center
    for (Int_t s=0;s<6;s++)
      for (Int_t r=0;r<96;r++)
        for(Int_t c=0;c<96;c++)
          RingsumPattern[s][r][c]= 0;



    
    Int_t version=2; // 1=old, 2=new
    
    iterHRichHitIPU->Reset();
    while ((ptlHRichHitIPU = (HRichHitIPU *)iterHRichHitIPU->Next()) != 0) {
      if ((version==1) || 
	  ( (version==2) && ( (ptlHRichHitIPU->getQuality()>8) &&(ptlHRichHitIPU->getVetoQuality()<4 )))) {
        mySect = ptlHRichHitIPU->getSector();
        myY = ptlHRichHitIPU->getY();
        myX = ptlHRichHitIPU->getX();
      if ((myX == 0) || (myY == 0)) continue;
        RingsumPattern[mySect][myX][myY]=ptlHRichHitIPU->getQuality();
      }

    }
    iterHRichHitIPU->Reset();
      while ((ptlHRichHitIPU = (HRichHitIPU *)iterHRichHitIPU->Next()) != 0 ) {
      mySect = ptlHRichHitIPU->getSector();
      myY = ptlHRichHitIPU->getY();
      myX = ptlHRichHitIPU->getX();
      if ((myX == 0) || (myY == 0)) continue;
      //cout << "myX/Y: " << myY << " " << myX << endl;
      if ( (ptlHRichHitIPU->getQuality()>8) && (ptlHRichHitIPU->getVetoQuality()<4) ) {
        Int_t CurrentSum = ptlHRichHitIPU->getQuality();
        if ( CurrentSum  >= RingsumPattern[mySect][myX+1][myY] &&
             CurrentSum  >= RingsumPattern[mySect][myX][myY+1] &&
             CurrentSum  > RingsumPattern[mySect][myX-1][myY] &&
             CurrentSum  > RingsumPattern[mySect][myX][myY-1] ) {  // Local maximun search

	    anacoord[mySect][anaHitsSec[mySect]].X = myX;
	    anacoord[mySect][anaHitsSec[mySect]].Y = myY;
	    anacoord[mySect][anaHitsSec[mySect]].RQ = ptlHRichHitIPU->getQuality();
	    anacoord[mySect][anaHitsSec[mySect]].VQ = ptlHRichHitIPU->getVetoQuality();
	    anacoord[mySect][anaHitsSec[mySect]].MF = 0;
	    hallana[mySect]->Fill(myX,myY);

	    if(verbose) cout << "EVT " << i << " AnaRing at x=" << anacoord[mySect][anaHitsSec[mySect]].X << " y="
		 << anacoord[mySect][anaHitsSec[mySect]].Y << " sect="  << mySect << "  ring quality "
		 <<anacoord[mySect][anaHitsSec[mySect]].RQ << "  veto quality "
		 <<anacoord[mySect][anaHitsSec[mySect]].VQ <<"  dataw " << ptlHRichHitIPU->getDataWord() << endl;

	    anaHitsSec[mySect]++;
	  }
        }
      }

    
      //---
    
    for (Int_t ii =0; ii<6; ii++){  
      hNAHits[ii]->Fill(anaHitsSec[ii]);
      if (ipuHitsSec[ii] == 1)
	hNAHitscut[ii]->Fill(anaHitsSec[ii]);
    } 
    

    for (mySect=0;mySect<6;mySect++){
      ana_sum += anaHitsSec[mySect];
      mu_sum += ipuHitsSec[mySect];
    }
    
    for (Int_t ii =0; ii<6; ii++){  
      hNIHits[ii]->Fill(ipuHitsSec[ii]);
      if (anaHitsSec[ii] == 1)
	hNIHitscut[ii]->Fill(ipuHitsSec[ii]);
  }
    
    
    //- ****** Correlate closest ipu/ana pairs *****
    
    for (Int_t sec=0; sec<6; sec++){ // loop over sector
      //if (ipuHitsSec[sec] ==1 ) { // restrict to multiplicity 1 in IPU
      if ( (anaHitsSec[sec]>7) || (ipuHitsSec[sec]>7))  continue;  // If there is more than 7 hits per sector skip!

      listLen = 0;   // current number of array elements in difflist
      listPos = 0;
      
      //-** Create sorted list (diffList) of squared distances for all ipu-ana-hit combinations **
      //     Sorted by increasing squared distance
      
      // Reset difflist (all array elements set to zero)
      for ( Int_t k=0; k<1000;k++) { //1000 arbitrary value,1600 would be consistent
	diffList[k][0] = 0;
      }
      
      for( actAna=0; actAna<anaHitsSec[sec];actAna++) {    // loop over  softrings
	for( actIpu=0; actIpu<ipuHitsSec[sec];actIpu++) {   // loop over  hardrings

	   
	  diffX = ipucoord[sec][actIpu].X - anacoord[sec][actAna].X;
	  diffY = ipucoord[sec][actIpu].Y - anacoord[sec][actAna].Y;
	  diffSquared = diffX*diffX + diffY*diffY;   // squared distance ana-ipu
	  diff[sec]->Fill(diffSquared);
// 	  cout << "Calc " << listPos << " " << diffSquared << " " << actAna << " " << actIpu << endl;
 
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
//  	    cout << "Fill " << listPos << " " << diffSquared << " " << actAna << " " << actIpu << endl;
	    SqrDiff = diffX*diffX + diffY*diffY;
	    diffMatrix[actAna][actIpu] = SqrDiff;
	    diffList[listPos][0] = SqrDiff;    // squared distance
	    diffList[listPos][1] = actAna;  //  corresponding ana ring (anacoord)   
	    diffList[listPos][2] = actIpu;  //  corresponding ipu ring (ipucoord)
	    listPos = 0;
	   

	  
	} // end of loop over hardrings
      } // end of loop over softrings 
      

      //- ** Now we can start to correlate ipu and ana rings based on diffList
      //-** group ipu-ana hit pairs with smallest distances **
    
      nonCorrelatedIpu = ipuHitsSec[sec];
      nonCorrelatedAna = anaHitsSec[sec];
      //cout  << "sec: " << sec << "  NCIStart: " << nonCorrelatedIpu << "  NCAStart: " <<  nonCorrelatedAna << endl;
      listPos = 0; listPos2 = 0;
      while ( ( nonCorrelatedIpu > 0 ) && ( nonCorrelatedAna > 0 ) && ( listPos<listLen) ) {
	actAna = diffList[listPos][1];
	//cout << " Set actAna to " << actAna << " at listPos " << listPos << endl;
	actIpu = diffList[listPos][2];
	if ( (ipucoord[sec][actIpu].MF==0) && (anacoord[sec][actAna].MF==0) ) {
	  RingVeto->Fill(anacoord[sec][actAna].RQ,anacoord[sec][actAna].VQ);
	  CRQuality[sec]->Fill(anacoord[sec][actAna].RQ);
	  CRVetoQuality[sec]->Fill(anacoord[sec][actAna].VQ);
	  //cout << "now ring quality at sec " << sec << " and pos " << actAna << " is "
	  //<< anacoord[sec][actAna].RQ << endl;
	  //cout << "Corr of AnaNr " << actAna << " and IPUNr " << actIpu << endl;
	  corrList[listPos2][0] = sec;
	  corrList[listPos2][1] = anacoord[sec][actAna].X;
	  corrList[listPos2][2] = anacoord[sec][actAna].Y;
	  corrList[listPos2][3] = ipucoord[sec][actIpu].X;
	  corrList[listPos2][4] = ipucoord[sec][actIpu].Y;
	  listPos2++;
	  hcorr[sec]->Fill(ipucoord[sec][actIpu].X, ipucoord[sec][actIpu].Y);
	  anacoord[sec][actAna].MF++;
	  anacoord[sec][actAna].MF++;
	  ipucoord[sec][actIpu].MF++;
	  ipucoord[sec][actIpu].MF++;
	  nonCorrelatedIpu--; nonCorrelatedAna--;	  
	}
	listPos++;
      }

      
      //-** group all remaining ana hits with (0,0) **
      for( actAna=0; actAna<anaHitsSec[sec];actAna++) {
	if ( (anacoord[sec][actAna].MF == 0) )   { // if it has not been correlated yet

	  Int_t NeighborFlag = 0;
	  for( actIpu=0; actIpu<ipuHitsSec[sec];actIpu++) {
	    if ( diffMatrix[actAna][actIpu] <= 2 ) NeighborFlag = 1;
	  }

	  corrList[listPos2][0] = sec;
	  corrList[listPos2][1] = anacoord[sec][actAna].X;
	  corrList[listPos2][2] = anacoord[sec][actAna].Y;
	  corrList[listPos2][3] = 0;
	  corrList[listPos2][4] = 0;
	  hnca[sec]->Fill((Float_t)corrList[listPos2][1],(Float_t)corrList[listPos2][2],(Stat_t)1);
	  if ( !NeighborFlag)
	    hncaa[sec]->Fill((Float_t)corrList[listPos2][1],(Float_t)corrList[listPos2][2],(Stat_t)1);
	  NCRQuality[sec]->Fill(anacoord[sec][actAna].RQ);
	  NCRVetoQuality[sec]->Fill(anacoord[sec][actAna].VQ);
	  NCRingVeto->Fill(anacoord[sec][actAna].RQ,anacoord[sec][actAna].VQ);

// 	  if (corrList[listPos][3]==0 && corrList[listPos][4]==0 ){
//   	  cout << "evt: " << i << "  NONCorrAna " << corrList[listPos2][1] << " "
//  	       << corrList[listPos2][2] << " " << corrList[listPos2][3] << " " 
//  	       << corrList[listPos2][4] << "  sec " << sec << " X " << anacoord[sec][actAna].X << " Y " 
//  	       << anacoord[sec][actAna].Y << " ringR " << anacoord[sec][actAna].RQ << " vetoR " 
// 	       << anacoord[sec][actAna].VQ << endl;
// 	  }

	  discrepancy[sec]++;
	  listPos2++;
	} 
      }

      //-** group all remaining ipu hits with (0,0) **
      for( actIpu=0; actIpu<ipuHitsSec[sec];actIpu++) {
	if ( (ipucoord[sec][actIpu].MF == 0) ) { // if it has not been correlated yet


	  Int_t NeighborFlag = 0;
	  for( actAna=0; actAna<anaHitsSec[sec];actAna++) {
	    if ( diffMatrix[actAna][actIpu] <= 2 ) NeighborFlag = 1;
	  }

	  corrList[listPos2][0] = sec;
	  corrList[listPos2][1] = 0;
	  corrList[listPos2][2] = 0;
	  corrList[listPos2][3] = ipucoord[sec][actIpu].X;
	  corrList[listPos2][4] = ipucoord[sec][actIpu].Y;
	  hnci[sec]->Fill((Float_t)corrList[listPos2][3],(Float_t)corrList[listPos2][4],(Stat_t)1);
	  if ( !NeighborFlag)
	    hncii[sec]->Fill((Float_t)corrList[listPos2][3],(Float_t)corrList[listPos2][4],(Stat_t)1);

// 	  if ( corrList[listPos2][1]==0 && corrList[listPos2][2] ==0) {
//  	    cout << "evt: " << i << "  NONCorrIPu " << corrList[listPos2][1] << " "
//  		 << corrList[listPos2][2] << " " << corrList[listPos2][3] << " " 
//  		 << corrList[listPos2][4] << "  sec " << sec << " X " << ipucoord[sec][actIpu].X << " Y "
//  		 << ipucoord[sec][actIpu].Y << endl;  
// 	    }

	  discrepancy[sec]++;
	  listPos2++;
	}
      }

      //-** fill histograms from list of correlated pairs (corrList) **
      for ( listPos = 0; listPos<listPos2; listPos++ ) {
// 	cout << "Corr " << corrList[listPos][1] << " "
// 	  	   << corrList[listPos][2] << " " << corrList[listPos][3] << " "   
// 	  	   << corrList[listPos][4] << endl; 
	sec =  corrList[listPos][0];
	hshcx[sec]->Fill((Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3],(Stat_t)1);
	hshcy[sec]->Fill((Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4],(Stat_t)1);
	if ((corrList[listPos][2] == corrList[listPos][4]) && (abs(corrList[listPos][1]-corrList[listPos][3])==16 )) 
	  {
	    cout << "16 CHANNEL MISMATCH at X" << corrList[listPos2][4] << "  Y "  << corrList[listPos][3] << endl;
	    n16chMis++;
	  }
	if (!(corrList[listPos][1] == 0 || corrList[listPos][3]==0 ) )
	  {
	    hiaVax[sec]->Fill((Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3]/corrList[listPos][1],(Stat_t)1);
	  }
	if (!(corrList[listPos][2] == 0 || corrList[listPos][4]==0 ) )
	  {
	    hiaVay[sec]->Fill((Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4]/corrList[listPos][2],(Stat_t)1);
	  }
      }
      
      
      //- *********** End of Correlation ************
      
      //}
 
    } // end of loop over sector
    
    catlHMatchURich->Clear();
    catlHRichHitIPU->Clear(); 
    
    for (Int_t ii =0; ii<6; ii++){
      hdiscrepancy[ii]->Fill(i,discrepancy[ii]);
      htipuhits[ii]->Fill(i,ipuHitsSec[ii]);
      htanahits[ii]->Fill(i,anaHitsSec[ii]);
    }
    
    } // real events
  } // **** End of EventLoop ****          

  cout << "Ana_sum: " << ana_sum << endl;
  cout << "MU_sum: " << mu_sum << endl;
  cout << "Number of 16 channels mismatch: " << n16chMis << endl;


      f->Close();

  hfile->cd();
  for (Int_t ii =0; ii<6; ii++){
    hshcx[ii]->Write();
    hshcy[ii]->Write();
    hallipu[ii]->Write();
    hallana[ii]->Write();
    hnca[ii]->Write();
    hnci[ii]->Write();
    hncaa[ii]->Write();
    hncii[ii]->Write();
    hcorr[ii]->Write();
    diff[ii]->Write();
    hNIHits[ii]->Write();
    hNAHits[ii]->Write();
    hNIHitscut[ii]->Write();
    hNAHitscut[ii]->Write();
    CRQuality[ii]->Write();
    NCRQuality[ii]->Write();
    CRVetoQuality[ii]->Write();
    NCRVetoQuality[ii]->Write();
    hdiscrepancy[ii]->Write();
    htipuhits[ii]->Write();
    htanahits[ii]->Write();
    hiaVax[ii]->Write();
    hiaVay[ii]->Write();
  }
  RingVeto->Write();
  NCRingVeto->Write();
  
  hfile->Close();

  cout << "event processed " <<  ientries << endl;
  cout << outFile << endl;
 return (1);
}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("rich-richIPU_corr","compiled rich richIPU correlation macro");
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
        		ana_rich(argv[cur_arg],0,verbose);
        		exit(0);
		case 2:
        		ana_rich(argv[cur_arg],atoi(argv[cur_arg+1]),verbose);
        		exit(0);
		default:
                	cout << argv[0] << " [-v] pathfilename> [number of events]" << endl;
                	cout << " -v   be verbose" << endl;
                	exit(1);
        }
}
#endif
