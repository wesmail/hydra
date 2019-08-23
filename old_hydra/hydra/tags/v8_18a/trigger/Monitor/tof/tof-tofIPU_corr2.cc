// TOF - TOF IPU correlation
// load the library  > .L /u/alberica/hydra542/macros/corr/tof-tofIPU_corr2.cc+
// call the function > cor_tof2()
// ---------------------------------------------------

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

#include "htofhit.h"
#include "hmatrixcategory.h"
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
  Float_t ph;
  Float_t th;
  Float_t time;
  Int_t mod;
  Int_t cell;
  Int_t det;
} Toftype;


Int_t ana_tof(TString inputFile,Int_t nEvents=0,Bool_t verbose=false)
{
//  Hades* myHades = new Hades;
//  gROOT->Reset();

  TH2F *non_corr_IPU[6];
  TH2F *non_corr_ana[6];
  TH1F *non_corr_IPU_time[6];
  TH1F *non_corr_ana_time[6];
  TH2F *corr_phi[6];
  TH2F *corr_theta[6];
  TH2F *corr_time[6];
  TH1F *diff_time[6];
  TH1F *diff_ang[6];
  TH1F *diff_theta[6];
  TH1F *diff_phi[6];
  TH2F *diff_thetaphi[6];
  TH2F *bad_evt[6];

  TH2F *corr_theta_phi_part[6];

  for (Int_t ii =0; ii<6; ii++){

    Char_t buffer[255];
    sprintf(buffer,"non_corr_IPU%i",ii);
    non_corr_IPU[ii]  =new TH2F(buffer,buffer,60,0.,60.,50,40.,90.);
    non_corr_IPU[ii]->SetOption("colz");

    sprintf(buffer,"non_corr_ana%i",ii);
    non_corr_ana[ii]=new TH2F(buffer,buffer,60,0.,60.,50,40.,90.);
    non_corr_ana[ii]->SetOption("colz");

    sprintf(buffer,"non_corr_IPU_time%i",ii);
    non_corr_IPU_time[ii]  =new TH1F(buffer,buffer,100,-10.,100.);

    sprintf(buffer,"non_corr_ana_time%i",ii);
    non_corr_ana_time[ii]=new TH1F(buffer,buffer,100,-10.,100.);

    sprintf(buffer,"corr_phi%i",ii);
    corr_phi[ii]=new TH2F(buffer,buffer,60,0.,60.,60,0.,60.);
    corr_phi[ii]->SetOption("colz");
    corr_phi[ii]->SetXTitle("Ana");
    corr_phi[ii]->SetYTitle("IPU");

    sprintf(buffer,"corr_theta%i",ii);
    corr_theta[ii]=new TH2F(buffer,buffer,50,40.,90.,50,40.,90.);
    corr_theta[ii]->SetOption("colz");
    corr_theta[ii]->SetXTitle("Ana");
    corr_theta[ii]->SetYTitle("IPU");
    
    sprintf(buffer,"corr_time%i",ii);
    corr_time[ii]=new TH2F(buffer,buffer,100,0.,100.,100,0.,100.);
    corr_time[ii]->SetOption("colz");
    corr_time[ii]->SetXTitle("Ana");
    corr_time[ii]->SetYTitle("IPU");
    
    sprintf(buffer,"diff_time%i",ii);
    diff_time[ii]=new TH1F(buffer,buffer,140,-70.,70.);

    sprintf(buffer,"diff_ang%i",ii);
    diff_ang[ii]=new TH1F(buffer,buffer,150,0,30);
    
    sprintf(buffer,"diff_phi%i",ii);
    diff_phi[ii]=new TH1F(buffer,buffer,100,-20.,20.);
    
    sprintf(buffer,"diff_theta%i",ii);
    diff_theta[ii]=new TH1F(buffer,buffer,100,-10.,10.);
    
    sprintf(buffer,"diff_thetaphi%i",ii); 
    diff_thetaphi[ii]=new TH2F(buffer,buffer,64,-0.5,63.5,40,-20,20);
    diff_thetaphi[ii]->SetOption("colz");

    sprintf(buffer,"bad_evt%i",ii);
    bad_evt[ii] =new TH2F(buffer,buffer,60,0,60,50,40,90);
    bad_evt[ii]->SetOption("colz");

    sprintf(buffer,"corr_theta_phi_part%i",ii);
    corr_theta_phi_part[ii]=new TH2F(buffer,buffer,60,0.,60.,50,40.,90.);
    corr_theta_phi_part[ii]->SetOption("colz");
 }

  TString outFile="";
  process_filename(inputFile,outFile,"_tofCORR");

  TFile *hfile = new TFile((Text_t *)outFile.Data(),"RECREATE","Test",2);

  TFile *f = new TFile(inputFile);
  if(!f) {cout << "File not opened! " <<inputFile << endl; exit(1);}

  HTree *T = (HTree*)gDirectory->Get("T");
  if(!T) {cout << "No tree in file!" << endl; exit(1);}

  f->cd("dirTof");
  HTofHit* ptmHTofHit = new HTofHit();
  HMatrixCategory* catmHTofHit = (HMatrixCategory*) gDirectory->Get("HTofHit");
  TBranch* brmHTofHit = T->GetBranch("HTofHit.");
  if(brmHTofHit) brmHTofHit->SetAddress(&catmHTofHit);
  TIterator *iterHTofHit = catmHTofHit->MakeIterator();

  f->cd("dirTrigger");
  HMatchUTof* ptlHMatchUTof = new HMatchUTof();
  HLinearCategory* catlHMatchUTof = (HLinearCategory*) gDirectory->Get("HMatchUTof");
  TBranch* brlHMatchUTof = T->GetBranch("HMatchUTof.");
  if(brlHMatchUTof) brlHMatchUTof->SetAddress(&catlHMatchUTof);
  TIterator *iterHMatchUTof = catlHMatchUTof->MakeIterator();


  //   f->cd("dirStart");
  //     HStartHit* ptlHStartHit = new HStartHit();
  //     HLinearCategory* catlHStartHit = (HLinearCategory*) gDirectory->Get("HStartHit");
  //     TBranch* brlHStartHit = T->GetBranch("HStartHit");
  //     if(brlHStartHit) brlHStartHit->SetAddress(&catlHStartHit);
  //     TIterator *iterHStartHit = catlHStartHit->MakeIterator();


  //--------------------


  Int_t ipuHitsSec[6];
  Int_t anaHitsSec[6];

  Toftype ipucoord[6][100];
  Toftype anacoord[6][100];
  // ******* preparations for event loop ******
  Stat_t ientries;  // how many events
  ientries = T->GetEntries();  // how many events
  if (nEvents>0 && nEvents<ientries)  ientries = nEvents;
  cout << ientries << " Events to process" << endl;

  Float_t phTof,thTof;
  Float_t myPhi, myTheta, myTime, startTime;
  Int_t mySect;

  Float_t diffPhi, diffTheta, diffSquared;
  Float_t diffList[9000][3];
  Float_t corrList[9000][8];
  Int_t nonCorrelatedIpu, nonCorrelatedAna;
  Int_t listLen, listPos, listPos2;
  Int_t actAna, actIpu;
  //Int_t ana_hits, mu_hits;
  Int_t ana_sum, mu_sum;

  ana_sum = mu_sum = 0;

  // ******** EventLoop ************

  for (Int_t i=0; i<ientries;i++) {
    if(verbose){
    	cout << "---------------------------evt" << i << endl;
    }else{
   	if((i%10000)==0) cout << "Event " << i << endl;
    }
    // T->GetEvent(i);

    for (Int_t j=0;j<6;j++) {
      ipuHitsSec[j] = 0;
      anaHitsSec[j] = 0;
    }

    //  ana_hits=0;
    //  mu_hits=0;

    for (Int_t j=0;j<6;j++) {
      for (Int_t k=0;k<100;k++) {

	ipucoord[j][k].ph = 0;
	ipucoord[j][k].th = 0;
	ipucoord[j][k].time = 0;
	ipucoord[j][k].mod = 0;
	ipucoord[j][k].cell = 0;
	ipucoord[j][k].det = 0;

	anacoord[j][k].ph = 0;
	anacoord[j][k].th = 0;
	anacoord[j][k].time = 0;
	anacoord[j][k].mod = 0;
	anacoord[j][k].cell = 0;
	anacoord[j][k].det = 0;
      }
    }

    //brlHStartHit->GetEntry(i);
    brmHTofHit->GetEntry(i);
    brlHMatchUTof->GetEntry(i);
   
    iterHTofHit->Reset();
    //iterHStartHit->Reset();
    iterHMatchUTof->Reset();
   
    //cout << "********************evt nb: " << i << endl;
    //cout << "EventSeqNumber: " << gHades->getCurrentEvent()->getHeader()->getEventSeqNumber() << endl;

    //while ((ptlHStartHit = (HStartHit *)iterHStartHit->Next()) != 0) {
    while ((ptmHTofHit = (HTofHit *)iterHTofHit->Next()) != 0) {

      //startTime = ptlHStartHit->getTime();
      startTime =0;
      ptmHTofHit->getPhi(phTof);
      ptmHTofHit->getTheta(thTof);
      mySect = ptmHTofHit->getSector();

      myTime = ptmHTofHit->getTof();
      if (mySect<5) 
	phTof -= ((mySect+1)*60);

      //cout << mySect << "  " << phTof << endl;
      if ((phTof>0.&&phTof<60.) &&(thTof>43.&&thTof<90.) ){	
	anacoord[mySect][anaHitsSec[mySect]].ph=phTof; 
	anacoord[mySect][anaHitsSec[mySect]].th=thTof; 
	anacoord[mySect][anaHitsSec[mySect]].time=myTime; 
	anacoord[mySect][anaHitsSec[mySect]].mod= ptmHTofHit->getModule();
	anacoord[mySect][anaHitsSec[mySect]].cell= ptmHTofHit->getCell();
	anacoord[mySect][anaHitsSec[mySect]].det = anacoord[mySect][anaHitsSec[mySect]].mod*8+anacoord[mySect][anaHitsSec[mySect]].cell;
   	if(verbose) cout << anacoord[mySect][anaHitsSec[mySect]].mod << "  " << anacoord[mySect][anaHitsSec[mySect]].cell
	     <<"  " << anacoord[mySect][anaHitsSec[mySect]].det << endl;
	////// try with and without start: in the IPU no start time subtraction !!!!!
	//	anacoord[ana_hits][3]=myTime;
	//anacoord[ana_hits][3]=myTime+startTime;


	
	if(verbose) cout << "AnaHit at phi: " << anacoord[mySect][anaHitsSec[mySect]].ph << "  theta: " << anacoord[mySect][anaHitsSec[mySect]].th << "  sect: " << mySect << "  start time: " << startTime << "  tof time " << anacoord[mySect][anaHitsSec[mySect]].time << endl;

	anaHitsSec[mySect]++;
	//ana_hits++;
      }
    }
    //} // start
    //     cout << " tot ana hits: " << ana_hits << endl;
    ana_sum += anaHitsSec[mySect];


    while ((ptlHMatchUTof = (HMatchUTof *)iterHMatchUTof->Next()) != 0) {

      myPhi = ptlHMatchUTof->getPhi();
      myTheta = ptlHMatchUTof->getTheta();
      mySect = ptlHMatchUTof->getSector();
      myTime = ptlHMatchUTof->getTime();

      if (mySect<5)
	myPhi -= ((mySect+1)*60);

      // if(myTheta>45&&myTheta<85){
      ipucoord[mySect][ipuHitsSec[mySect]].ph=myPhi;
      ipucoord[mySect][ipuHitsSec[mySect]].th=myTheta;
      ipucoord[mySect][ipuHitsSec[mySect]].time=myTime;

     if(verbose) cout << " IPUHit at phi: " << myPhi << "  theta: " << myTheta << "  sect: "
	   << mySect << "  time: " << myTime <<endl;

      ipuHitsSec[mySect]++;
      // mu_hits++;
      //}
    }
    //      cout << " tot ipu hits: " << mu_hits << endl;
    mu_sum += ipuHitsSec[mySect];

 
    //- ****** Correlate closest ipu/ana pairs *****
 
    // ipucoord[i][] contains hits from real hardware
    // anacoord[i][] contains hits from offline analysis
 
    listLen = 0;   // current number of array elements in difflist
    listPos = 0;
 
    //-** Create sorted list (diffList) of squared distances for all ipu-ana-hit combinations **
    //     Sorted by increasing squared distance

    for (Int_t sec=0; sec<6; sec++){ // loop over sector
      // Reset difflist (all array elements set to zero)
      for ( Int_t k=0; k<9000;k++) { //100 arbitrary value,1600 would be consistent
	for ( Int_t kkk=0; kkk<3; kkk++) {
	  diffList[k][kkk] = 0;
	}
      }

      for ( Int_t k=0; k<9000;k++) {
	for ( Int_t kkk=0; kkk<8; kkk++) {
	  corrList[k][kkk] = 0;
	}
      }

      for( actAna=0; actAna<anaHitsSec[sec];actAna++) {    // loop over  softhits
	for( actIpu=0; actIpu<ipuHitsSec[sec];actIpu++) {   // loop over  hardhits
       
	  diffPhi = ipucoord[sec][actIpu].ph - anacoord[sec][actAna].ph;
	  diffTheta = ipucoord[sec][actIpu].th - anacoord[sec][actAna].th;
	  diffSquared = diffPhi*diffPhi + diffTheta*diffTheta;   // squared distance ana-ipu
	  diff_ang[sec]->Fill(diffSquared);
	  //  	  cout << "Calc " << listPos << " " << diffSquared << " " << actAna
	  //  	       << " " << actIpu << endl; 

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
   
   
	  //  	  cout << "Fill " << listPos << " " << diffSquared << " "
	  //  	       << actAna << " " << actIpu << endl; 
	  diffList[listPos][0] = diffPhi*diffPhi + diffTheta*diffTheta;    // squared distance
	  diffList[listPos][1] = actAna;  //  corresponding ana hit (anacoord)   
	  diffList[listPos][2] = actIpu;  //  corresponding ipu hit (ipucoord)
	  listPos = 0;
	

	} // end of loop over hardhits
      } // end of loop over softhits 

      //- ** Now we can start to correlate ipu and ana rings based on diffList
 
      //-** group ipu-ana hit pairs with smallest distances **

      nonCorrelatedIpu = ipuHitsSec[sec];
      nonCorrelatedAna =  anaHitsSec[sec];

      listPos = 0; listPos2 = 0;
      while ( ( nonCorrelatedIpu > 0 ) && ( nonCorrelatedAna > 0 ) && ( listPos<listLen) ) {
	actAna = diffList[listPos][1];
	actIpu = diffList[listPos][2];
	if ( (ipucoord[sec][actIpu].ph >0) && (anacoord[sec][actAna].th>0) ) {
	  corrList[listPos2][0] = sec;
	  corrList[listPos2][1] = anacoord[sec][actAna].ph;
	  corrList[listPos2][2] = anacoord[sec][actAna].th;
	  corrList[listPos2][3] = ipucoord[sec][actIpu].ph;
	  corrList[listPos2][4] = ipucoord[sec][actIpu].th;
	  corrList[listPos2][5] = anacoord[sec][actAna].time;
	  corrList[listPos2][6] = ipucoord[sec][actIpu].time;
	  corrList[listPos2][7] = anacoord[sec][actAna].det;
	  listPos2++;
	  anacoord[sec][actAna].ph = 0;
	  anacoord[sec][actAna].th = 0;
	  anacoord[sec][actAna].time = 0;
	  ipucoord[sec][actIpu].ph = 0;
	  ipucoord[sec][actIpu].th = 0;
	  ipucoord[sec][actIpu].time = 0;
	  nonCorrelatedIpu--; nonCorrelatedAna--;
	}
	listPos++;
      }
      // cout << " non correlated: " << nonCorrelatedIpu << " "
      // << nonCorrelatedAna << endl;
      // }   // diffsquared

      //-** group all remaining ipu (or ana) hits with (0,0) **
      for( actAna=0; actAna<anaHitsSec[sec];actAna++) {
	// cout << "actAna: " << actAna << endl;
	if ( (anacoord[sec][actAna].ph != 0) && (anacoord[sec][actAna].th != 0) )  {
	  //  	cout << anacoord[actAna][0] << " "
	  //  	     << anacoord[actAna][1] << endl;  
	  corrList[listPos2][0] = sec;
	  corrList[listPos2][1] = anacoord[sec][actAna].ph;
	  corrList[listPos2][2] = anacoord[sec][actAna].th;
	  corrList[listPos2][3] = 0;
	  corrList[listPos2][4] = 40;
	  corrList[listPos2][5] = anacoord[sec][actAna].time;
	  corrList[listPos2][6] = 0;
	  non_corr_ana[sec]->Fill((Float_t)corrList[listPos2][1],(Float_t)corrList[listPos2][2],(Stat_t)1);
	  non_corr_ana_time[sec]->Fill((Float_t)corrList[listPos2][5],(Stat_t)1);
	  //if (corrList[listPos][1]==0 && corrList[listPos][2]==0 ){
	  //  	cout << "CorrAna " << corrList[listPos2][1] << " "
	  //  	     << corrList[listPos2][2] << " " << corrList[listPos2][3] << " " 
	  //  	     << corrList[listPos2][4] << " " << anacoord[actAna][0] << " " 
	  //  	     << anacoord[actAna][1] << endl;   
	  //}
	  listPos2++;
	  //   cout << "---------------------------evt" << i << endl;
	  //  	cout << "non corr ANA at phi: " << anacoord[sec][actAna].ph << "  theta: " << anacoord[sec][actAna].th << " sec: " << sec << endl;

	}
      }

      for( actIpu=0; actIpu<ipuHitsSec[sec];actIpu++) {
	if ( (ipucoord[sec][actIpu].ph != 0) && (ipucoord[sec][actIpu].th != 0)   ) {
	  corrList[listPos2][0] = sec;
	  corrList[listPos2][1] = 0;
	  corrList[listPos2][2] = 40;
	  corrList[listPos2][3] = ipucoord[sec][actIpu].ph;
	  corrList[listPos2][4] = ipucoord[sec][actIpu].th;
	  corrList[listPos2][5] = 0;
	  corrList[listPos2][6] = ipucoord[sec][actIpu].time;
	  non_corr_IPU[sec]->Fill((Float_t)corrList[listPos2][3],(Float_t)corrList[listPos2][4],(Stat_t)1);
	  non_corr_IPU_time[sec]->Fill((Float_t)corrList[listPos2][6],(Stat_t)1);
	  //  	cout << "CorrIPu " << corrList[listPos2][1] << " "
	  //  	     << corrList[listPos2][2] << " " << corrList[listPos2][3] << " " 
	  //  	     << corrList[listPos2][4] << " " << ipucoord[actIpu][0] << " " 
	  //  	     << ipucoord[actIpu][1] << endl;  
	  listPos2++;
	  //   cout << "---------------------------evt" << i << endl;
	  //  	cout << "non corr IPU at phi: " << ipucoord[sec][actIpu].ph << "  theta: " << ipucoord[sec][actIpu].th << " sec: " << sec << endl;
	}
      }
      //    cout << " ListPos2: " << listPos2 << endl;

      //-** fill histograms from list of correlated pairs (corrList) **
      for ( listPos = 0; listPos<listPos2; listPos++ ) {
	//        cout << "Corr " << corrList[listPos][1] << " "
	//  	   << corrList[listPos][2] << " " << corrList[listPos][3] << " "   
	//  	   << corrList[listPos][4] << endl;
	corr_phi[sec]->Fill((Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3],(Stat_t)1);
	corr_theta[sec]->Fill((Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4],(Stat_t)1);
	diff_phi[sec]->Fill((Float_t) corrList[listPos][1]-(Float_t) corrList[listPos][3],(Stat_t)1);
	diff_theta[sec]->Fill((Float_t) corrList[listPos][2]-(Float_t) corrList[listPos][4],(Stat_t)1);
	corr_time[sec]->Fill((Float_t)corrList[listPos][5],(Float_t) corrList[listPos][6],(Stat_t)1);
	diff_time[sec]->Fill((Float_t)corrList[listPos][5]-(Float_t) corrList[listPos][6],(Stat_t)1);
	diff_thetaphi[sec]->Fill((Float_t) corrList[listPos][7] , (Float_t) corrList[listPos][1]-(Float_t) corrList[listPos][3],(Stat_t)1);
	if (corrList[listPos][5]>40 && corrList[listPos][5]<60)
	  corr_theta_phi_part[sec]->Fill((Float_t) corrList[listPos][3],(Float_t) corrList[listPos][4],(Stat_t)1);


	if (abs( corrList[listPos][1]- corrList[listPos][3]) > 4) {
	  bad_evt[sec]->Fill(corrList[listPos][3], corrList[listPos][4] ,(Stat_t)1);
	  //if (sec == 1)
	  //cout <<sec << " : " << corrList[listPos][7] << " : " << corrList[listPos][2] << " : " << corrList[listPos][4] << endl;
	}


      }
    
 
      //- *********** End of Correlation ************

    } // end of loop over sector
 
   
    catlHMatchUTof->Clear();
    catmHTofHit->Clear(); 
    //catlHStartHit->Clear();
                                             
    // cout << "==================================" << endl;
  }  // **** End of EventLoop ****          

  cout << "Ana_sum: " << ana_sum << "  IPU_sum: " << mu_sum << endl;
 
  hfile->cd();
  for (Int_t ii =0; ii<6; ii++){  
    non_corr_IPU[ii]->Write();
    non_corr_ana[ii]->Write();
    non_corr_IPU_time[ii]->Write();
    non_corr_ana_time[ii]->Write();
    corr_phi[ii]->Write();
    corr_theta[ii]->Write();
    corr_time[ii]->Write();
    diff_time[ii]->Write();
    diff_ang[ii]->Write();
    diff_phi[ii]->Write();
    diff_theta[ii]->Write();
    diff_thetaphi[ii]->Write();
    bad_evt[ii]->Write();

    corr_theta_phi_part[ii]->Write();
  }
  hfile->Close();
              
   cout << outFile << endl;

  return (1);
}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("tof-tofIPU_corr","compiled tof tofIPU correlation macro");
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
		        ana_tof(argv[cur_arg],0,verbose);
		        exit(0);
		case 2:
		        ana_tof(argv[cur_arg],atoi(argv[cur_arg+1]),verbose);
		        exit(0);
		default:
	                cout << argv[0] << " [-v] pathfilename [number of events]" << endl;
	                cout <<  "-v  be verbose" << endl;
        	        exit(1);
        }
}
#endif

