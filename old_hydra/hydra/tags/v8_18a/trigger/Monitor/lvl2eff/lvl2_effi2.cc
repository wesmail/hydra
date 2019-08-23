// EFFICIENCY OF LVL2 with respect to offline analysis
// to run it: (in a root session)
//  > .L lvl2_effi.cc+
//  > ana()
// this macro defines a lepton by
// 1. correlation HRichHit - HKickTrack
// 2. Quality cuts (minimum bias) on rings
// 3. Time-of-Flight cuts
// leptons properties are stored in a ntuple
// the analysis can be done separately for LVL1 or LVL2 events

#ifndef __CINT__
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TNtuple.h>
#include <TChain.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include "htofhit.h"
#include "htofhitf.h"
#include "htofcluster.h"
#include "hstarthit.h"
#include "htboxchan.h"
#include "hrichhit.h"
#include "hmatchurich.h"
#include "hrichhitIPU.h"
#include "hrichhitheader.h"
#include "hkicktrack.h"
#include "hshowerhittof.h"
#include "hlocation.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "htree.h"
#include "hrecevent.h"
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "haddef.h"
#include "hgeantdef.h"
// #include "calodef.h"
#include "hcategory.h"
#include "hades.h"
#include "hgeomvector.h"
#endif

// here i define the structures i need 
// rings, tracks, leptons

typedef struct {
  Int_t X;
  Int_t Y;
  Float_t th;
  Float_t ph;
  Int_t rqua;
  Int_t vqua; 
  Int_t pad; 
  Float_t ch;
  Int_t pm;
  Int_t ht;
  Float_t ce;
  Int_t ratio;
} RichHittype;

typedef struct {
  Float_t beta; 
  Float_t p; 
  Float_t th; 
  Float_t ph; 
  Float_t mth; 
  Float_t mph; 
  Float_t tof;
  Int_t charge;
  Int_t system;
  Int_t mdcNr;
  Int_t metaNr;
  Float_t pull;
  Float_t ch0;
  Float_t ch1;
  Float_t ch2;
} KickHittype;

typedef struct {		
  Int_t richNr; 
  Int_t kickNr; 
  Int_t mdcNr;  
  Int_t metaNr;
  Float_t pull;   
  Float_t ch;     
  Int_t pm;     	
  Int_t pad;    
  Float_t ce;     
  Int_t ht;     
  Float_t p;      
  Float_t tof;    
  Float_t beta;   
  Int_t system; 
  Float_t rth;    
  Float_t rph;
  Float_t tth;    
  Float_t tph;    
  Float_t mth;    
  Float_t mph;    
  Int_t charge;
  Int_t X;      
  Int_t Y;
  Int_t sec;  
  Int_t trmult;
  Int_t opangflag;
  Int_t unlike;
  Int_t ratio;
} LepHittype;


void process_filename_lst(TString &inputFile, TString &outputFile, TString prefix,TString outputpath="")
{
	TString file="", inputpath="";

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
	if(outputpath=="") outputpath+=".";// Fallback if no path is given
	if(!outputpath.EndsWith("/")) outputpath+="/";

	if( inputFile.EndsWith(".lst")){
		outputFile  = outputpath+prefix+".root";
	}else{
		outputFile  = outputpath+file+"_"+prefix+".root";
	}

	cout << "File output: " << outputFile << endl;
}

Int_t lvl2_effi2(TString filename,Int_t step,Bool_t verbose=false)
{
//  gROOT->Reset();
  TStopwatch timer;
  timer.Reset();
  timer.Start();

  TString outFile="";
  if(step==2){
     process_filename_lst(filename,outFile,"lvl2_downscale_step2");
  }else if( step==1){
     process_filename_lst(filename,outFile,"lvl2_downscale_step1");
  }else{
     process_filename_lst(filename,outFile,"lvl2_downscale");
  }

  TFile *hfile = new TFile((Text_t *)outFile.Data(),"RECREATE","correlations");

  // --------------------output file

  TNtuple *ntuple_lep = new TNtuple("ntuple_lep","lepton ntuple","dsflag:avcharge:patmat:pad:phi:theta:p:tof:beta:dth:dph:X:Y:sec:charge:system");
  TH1F *hmultrich=new TH1F("hmultrich","rich mult",65,-0.5,64.5);
  TH1F *hmultkick=new TH1F("hmultkick","kick mult",65,-0.5,64.5);
  TH1F *hmultkickrich=new TH1F("hmultkickrich","kick-rich mult",65,-0.5,64.5);
  TH1F *hele=new TH1F("hele","electron mult",65,-0.5,64.5);
  TH1F *hpos=new TH1F("hpos","positron mult",65,-0.5,64.5);
  TH1F *hele_sel=new TH1F("hele_sel","selected electron mult",65,-0.5,64.5);
  TH1F *hpos_sel=new TH1F("hpos_sel","selected positron mult",65,-0.5,64.5);
  TH1F *htrackperring= new TH1F("htrackperring", "Tracks correlated with rings",10,0,10);
  TH1F *htofino_incl  = new TH1F("htofino_incl","time tofino (all)",500,0,50);;
  TH1F *htofino_sho   = new TH1F("htofino_sho","time tofino (shower condition)",500,0,50);
  TH1F *hdownscFlag = new TH1F("hdownscFlag","hdownscFlag",2,0,2);
  TH1F *hdiffTheta = new TH1F("hdiffTheta","hdiffTheta",100,-25,25);
  TH1F *hdiffPhi = new TH1F("hdiffPhi","hdiffPhi",100,-25,25);
  TH2F *hdiffPhiTheta = new TH2F("hdiffPhiTheta","hdiffPhiTheta",100,-25,25,100,-25,25);

  // --------------------input files
  TString files[100];
  Int_t nbOfFiles;

	if(filename.EndsWith(".lst")){
      nbOfFiles=0;
      ifstream ff;
      ff.open(filename);
      for(Int_t i=0; i<100; i++){
         if(ff.fail() || ff.eof()) break;
         ff >> files[i];
         if( files[i].Length()<5){ i--; continue;}
         // File must be with full pathname
/*         if(!files[i].Contains("/")){// only filename or path?
            files[i]=file_out+(TString)"/"+(TString)files[i];
         }*/
         cout << files[i] << endl;
         nbOfFiles++;
      }
      ff.close();
	}else{
		if(!filename.EndsWith(".root")) filename+=".root";
		nbOfFiles=1;
		files[0]=filename;
   }

  Int_t TotalEntries = 0;
  Int_t TotalLeptons = 0;

  for (Int_t xx=0; xx<nbOfFiles; xx++) {
      cout << "File Nr " << xx << "    Name: " << files[xx] << endl;

      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(files[xx]);
      if (!f) f = new TFile(files[xx]);
      if (!f) {cout << "Could not open file!" << endl; continue;}
      if (f->IsZombie()){ cout << "Zombie file!" << endl; continue;}

      HTree *T = (HTree*)gDirectory->Get("T");
      if(!T) {cout << "No tree in file!" << endl; continue;}

      HEventHeader* fEventHead = (HEventHeader*)f->Get("EventHeader.");
      TBranch* evt = T->GetBranch("EventHeader");
      if(evt) evt->SetAddress(&fEventHead);
                
      f->cd("dirRich");
      HRichHit* rich = new HRichHit();
      HLinearCategory* catlHRichHit = (HLinearCategory*) gDirectory->Get("HRichHit.");
      TBranch* brlHRichHit = T->GetBranch("HRichHit");
      if(brlHRichHit) brlHRichHit->SetAddress(&catlHRichHit);
      TIterator *iter_rich = catlHRichHit->MakeIterator();

      f->cd("dirTracks");
      HKickTrack *kick = new HKickTrack();
      HMatrixCategory* catmHKickTrack = (HMatrixCategory*) gDirectory->Get("HKickTrack.");
      TBranch* brmHKickTrack = T->GetBranch("HKickTrack");
      if(brmHKickTrack) brmHKickTrack->SetAddress(&catmHKickTrack);
      TIterator *iter_kick = catmHKickTrack->MakeIterator();

      f->cd("dirShower"); 
      HShowerHitTof* ptlHShowerHitTof = new HShowerHitTof();
      HLinearCategory* catlHShowerHitTof = (HLinearCategory*) gDirectory->Get("HShowerHitTof.");
      TBranch* brlHShowerHitTof = T->GetBranch("HShowerHitTof");
      if(brlHShowerHitTof) brlHShowerHitTof->SetAddress(&catlHShowerHitTof);

      f->cd("dirTof"); 
      HTofHit* ptmHTofHit = new HTofHit();
      HMatrixCategory* catmHTofHit = (HMatrixCategory*) gDirectory->Get("HTofHit.");
      TBranch* brmHTofHit = T->GetBranch("HTofHit");
      if(brmHTofHit) brmHTofHit->SetAddress(&catmHTofHit);
      TIterator *iterHTofHit = catmHTofHit->MakeIterator();      

     // ******* hit structures *******
      Int_t sector;
      RichHittype     RichHits[6][60];
      KickHittype     KickHits[6][30];
      LepHittype      LepHits[30];
      Int_t nRichHits[6];
      Int_t nKickHits[6];
      Int_t nLepHits;

      // ******* preparations for event loop ******
      //Stat_t ientries = 10000;
      Stat_t ientries = T->GetEntries();  // how many events I want to analyze
      TotalEntries += (Int_t) ientries;
      cout << ientries << " entries in current file" << endl;

      // ******** EventLoop ************
      for (Int_t i=0; i<ientries;i++) {
      //printf(" Event number: %d\n",i);
      if(i%1000==0&&i<10000) printf(" Event number: %d\n",i);
      if(i%10000==0&&i>=10000) printf(" Event number: %d\n",i);

      // ******* set to 0 the counters *******
      nLepHits = 0;
      for ( sector = 0; sector < 6; sector++) {
      nRichHits[sector] = 0;
      nKickHits[sector] = 0;
      }

      evt->GetEntry(i);
      //here i decide which kind of event i want to analyze
      //DF=0-> LVL2, DF=1->LVL1

      hdownscFlag->Fill((fEventHead)->getDownscalingFlag());
      //if ( (fEventHead)->getDownscalingFlag()==0) continue; //analysis done for LVL1
      //if ( (fEventHead)->getDownscalingFlag()==1) continue; //analysis done for LVL2
      if ( step==0
	 		|| (step==1 && (fEventHead)->getDownscalingFlag()==0)
			|| (step==2 && (fEventHead)->getDownscalingFlag()!=0)
	){
         brlHRichHit->GetEntry(i);
         brmHKickTrack->GetEntry(i);
         brmHTofHit->GetEntry(i);
         brlHShowerHitTof->GetEntry(i);

         //------------------------- loop over RICH --------------------------------------
         // in the ring structure i put all the rings which fulfill the (minimum bias) quality cuts
         Int_t rich_count=0;
         iter_rich->Reset();
         while ((rich=(HRichHit*)iter_rich->Next())!= 0){
         sector = rich->getSector();
         Float_t avChrg = ((Float_t)rich->getRingAmplitude())/((Float_t)rich->getRingPadNr());

         if( (avChrg>4. && avChrg<40.) && (rich->getRingPatMat()>200) &&
               (rich->getRingPadNr()>5) ){ // selects the "minimum bias" rings

            RichHits[sector][nRichHits[sector]].X     = rich->getRingCenterX();
            RichHits[sector][nRichHits[sector]].Y     = rich->getRingCenterY();
            RichHits[sector][nRichHits[sector]].th    = rich->getTheta();
            RichHits[sector][nRichHits[sector]].ph    = rich->getPhi();
            RichHits[sector][nRichHits[sector]].ch    = avChrg;
            RichHits[sector][nRichHits[sector]].pm    = rich->getRingPatMat();
            RichHits[sector][nRichHits[sector]].ht    = rich->getRingHouTra();
            RichHits[sector][nRichHits[sector]].pad   = rich->getRingPadNr();
            RichHits[sector][nRichHits[sector]].ce    = rich->getCentroid();
            RichHits[sector][nRichHits[sector]].ratio = rich->getTestRati();

            rich_count++;
            nRichHits[sector]++;
         }
         } //------------------------- end of loop over RICH --------------------------------------   
         hmultrich->Fill(rich_count);


         //------------------------- loop over TRACKS --------------------------------------
         // in the track structure i put all the reconstructed tracks which fulfill a time of flight cut
         // and a quality cut
         // HKickTrack contains tracks reconstructed with the Kickplane method:
         // matching between a Mdc Segment and a hit in EITHER Tof or Shower

         Float_t phiShift[6]={0.F,60.F,120.F,180.F,240.F,-60.F};
         Int_t kick_count=0;
         iter_kick->Reset();
         while ((kick=(HKickTrack*)iter_kick->Next())!= 0){

         Float_t tof = 2100./(kick->getBeta()*299.792458);

         /*
         if ( (kick->getSystem()==0 &&   tof < 8.) ||
               (kick->getSystem()==1 && ( tof > 5. && tof < 8.11 ) ) ) { // time-of-flight condition
         */
         if (1) {

            sector = kick->getSector();

            if (kick->getPull()<5.) { // quality cut (minimum bias)
               KickHits[sector][nKickHits[sector]].beta   = kick->getBeta();
               KickHits[sector][nKickHits[sector]].p      = kick->getP();
               KickHits[sector][nKickHits[sector]].charge = kick->getCharge();
               KickHits[sector][nKickHits[sector]].th     = kick->getTheta()*TMath::RadToDeg();
               KickHits[sector][nKickHits[sector]].ph     = kick->getPhi()*TMath::RadToDeg()+ phiShift[kick->getSector()];
               KickHits[sector][nKickHits[sector]].system = kick->getSystem(); //0=shower 1=tof
               KickHits[sector][nKickHits[sector]].tof    = tof;
               KickHits[sector][nKickHits[sector]].mdcNr  = kick->getSegment();
               KickHits[sector][nKickHits[sector]].pull   = kick->getPull();
               KickHits[sector][nKickHits[sector]].metaNr = kick->getOuterHitId();

               Float_t sr,sphi,stheta,ttheta,tphi;
               sr=sphi=stheta=ttheta=tphi=0;

               if (KickHits[sector][nKickHits[sector]].system==0) { // TOFINO_SHOWER
            ptlHShowerHitTof  = 
            (HShowerHitTof*) catlHShowerHitTof->getObject(KickHits[sector][nKickHits[sector]].metaNr);
            ptlHShowerHitTof->getSphereCoord(&sr,&sphi,&stheta);
            KickHits[sector][nKickHits[sector]].mth   = stheta;
            KickHits[sector][nKickHits[sector]].mph   = sphi;
            KickHits[sector][nKickHits[sector]].ch0 = ptlHShowerHitTof->getSum(0);
            KickHits[sector][nKickHits[sector]].ch1 = ptlHShowerHitTof->getSum(1);
            KickHits[sector][nKickHits[sector]].ch2 = ptlHShowerHitTof->getSum(2);
               }   else if (KickHits[sector][nKickHits[sector]].system==1) { // TOF	    
            iterHTofHit->Reset(); 
            while ((ptmHTofHit = (HTofHit *)iterHTofHit->Next()) != 0)  {
            Int_t s =ptmHTofHit->getSector();
            Int_t m = ptmHTofHit->getModule();
            Int_t c = ptmHTofHit->getCell();	  
            Int_t metaNr1=KickHits[sector][nKickHits[sector]].metaNr;
            if ( ( (Int_t)(metaNr1/1000) == s ) &&
                  ( (Int_t)(metaNr1 - 1000*((Int_t)(metaNr1/1000)) )/100 == m ) &&
                  ( metaNr1 - 1000*((Int_t)(metaNr1/1000)) - 100* ((Int_t) (metaNr1 - 1000*((Int_t)(metaNr1/1000)) )/100) == c )  ) {

               ptmHTofHit->getTheta(ttheta);
               ptmHTofHit->getPhi(tphi);
               KickHits[sector][nKickHits[sector]].mth   = ttheta;
               KickHits[sector][nKickHits[sector]].mph   = tphi;
            }
            }
               }
               nKickHits[sector]++;
               kick_count++;
            }
         }
         } //------------------------- end of loop over TRACKS --------------------------------------
         hmultkick->Fill(kick_count);


         //start of histo filling
         Int_t FlagRichCorr=0;
         Int_t kickrich_count=0;
         Int_t ele=0;
         Int_t pos=0;
         Int_t ele_sel=0;
         Int_t pos_sel=0;

         // I have created sector-wise structure which contain rings and tracks
         // now I loop over sectors (I will search for matching only sector-wise!!!)
         for (Int_t sec = 0; sec < 6 ; sec++) { // sector loop

         for (Int_t actRich=0; actRich < nRichHits[sec]; actRich++) { // rich loop    
            Int_t flagCorrelated=0;

            for (Int_t actKick=0; actKick < nKickHits[sec]; actKick++) { // kick loop

               // I look at the Theta and Phi matching BEFORE cutting on them...
               Float_t diffTheta=RichHits[sec][actRich].th-KickHits[sec][actKick].th; 
               Float_t diffPhi=RichHits[sec][actRich].ph-KickHits[sec][actKick].ph;
               Float_t sinTheta=TMath::Sin ( KickHits[sec][actKick].th * TMath::DegToRad());

               hdiffTheta->Fill(diffTheta);
               hdiffPhi->Fill(diffPhi*sinTheta);
               hdiffPhiTheta->Fill(diffPhi*sinTheta,diffTheta);

               //kick-rich correlations
               // These sigma are defined AFTER you look at the distributions, subtract the backgroung
               // and fit the signal (with a gaussian)
               // The offset is defined when the correlation is not centered in 0 (mis-alignment)

               /*
               Float_t sigma_ph  = 4.6;
               Float_t sigma_th  = 4.1;
               Float_t offset_th = -0.13;
               */

               Float_t sigma_ph  = 1.03;
               Float_t sigma_th  = 2.97;
               Float_t offset_th = -1.63;

               /*
               Float_t sigma_ph  = 1.12;
               Float_t sigma_th  = 3.2;
               Float_t offset_th = -1.7;
               */

               /*
               Float_t sigma_ph  = 1.;
               Float_t sigma_th  = 1.;
               Float_t offset_th = 0.;
               */

               Float_t chi2 = TMath::Power( (( diffTheta - offset_th) / sigma_th) ,2) +
                        TMath::Power( (( diffPhi * sinTheta )   / sigma_ph) ,2);
               if( (chi2/2.) < 2) { // matching condition

            // now I have a lepton!!!
            LepHits[nLepHits].richNr    = actRich;
            LepHits[nLepHits].kickNr    = actKick;
            LepHits[nLepHits].mdcNr     = KickHits[sec][actKick].mdcNr;
            LepHits[nLepHits].metaNr    = KickHits[sec][actKick].metaNr;
            LepHits[nLepHits].pull      = KickHits[sec][actKick].pull;
            LepHits[nLepHits].ch        = RichHits[sec][actRich].ch;
            LepHits[nLepHits].pm        = RichHits[sec][actRich].pm;
            LepHits[nLepHits].pad       = RichHits[sec][actRich].pad;
            LepHits[nLepHits].ce        = RichHits[sec][actRich].ce;
            LepHits[nLepHits].ht        = RichHits[sec][actRich].ht;
            LepHits[nLepHits].ratio     = RichHits[sec][actRich].ratio;
            LepHits[nLepHits].p         = KickHits[sec][actKick].p;
            LepHits[nLepHits].tof       = KickHits[sec][actKick].tof;
            LepHits[nLepHits].beta      = KickHits[sec][actKick].beta;
            LepHits[nLepHits].system    = KickHits[sec][actKick].system;
            LepHits[nLepHits].rth       = RichHits[sec][actRich].th;
            LepHits[nLepHits].rph       = RichHits[sec][actRich].ph;
            LepHits[nLepHits].tth       = KickHits[sec][actKick].th;
            LepHits[nLepHits].mph       = KickHits[sec][actKick].mph;
            LepHits[nLepHits].mth       = KickHits[sec][actKick].mth;
            LepHits[nLepHits].tph       = KickHits[sec][actKick].ph;
            LepHits[nLepHits].charge    = KickHits[sec][actKick].charge;
            LepHits[nLepHits].X         = RichHits[sec][actRich].X;
            LepHits[nLepHits].Y         = RichHits[sec][actRich].Y;
            LepHits[nLepHits].sec       = sec;
            LepHits[nLepHits].opangflag = KickHits[sec][actKick].system;;
            LepHits[nLepHits].unlike    = 0;

            // here I fill the ntuple
            {
               Float_t arr[20];
               Int_t a=0;
               arr[a++]=(fEventHead)->getDownscalingFlag();
               arr[a++]=LepHits[nLepHits].ch;
               arr[a++]=LepHits[nLepHits].pm;
               arr[a++]=LepHits[nLepHits].pad;
               arr[a++]=LepHits[nLepHits].tph;
               arr[a++]=LepHits[nLepHits].tth;
               arr[a++]=LepHits[nLepHits].p;
               arr[a++]=LepHits[nLepHits].tof;
               arr[a++]=LepHits[nLepHits].beta;
               arr[a++]=LepHits[nLepHits].mth - LepHits[nLepHits].tth;
               arr[a++]=LepHits[nLepHits].mph - LepHits[nLepHits].tph;
               arr[a++]=LepHits[nLepHits].X;
               arr[a++]=LepHits[nLepHits].Y;
               arr[a++]=LepHits[nLepHits].sec;
               arr[a++]=LepHits[nLepHits].charge;
               arr[a++]=LepHits[nLepHits].opangflag;
               ntuple_lep ->Fill(arr);
            }

            nLepHits++;

            flagCorrelated++;
            kickrich_count++;
            if (LepHits[nLepHits].charge==1) pos++;
            if (LepHits[nLepHits].charge==-1) ele++;
            TotalLeptons++;
            // hcharge0_lep->Fill(KickHits[sec][actKick].ch0);
            // hcharge1_lep->Fill(KickHits[sec][actKick].ch1);
            // hcharge2_lep->Fill(KickHits[sec][actKick].ch2);
               } // correlated hits

            } // loop over kick
            htrackperring->Fill(flagCorrelated);

            // only here I know how many tracks match the same ring
            for (Int_t jj = nLepHits-1; jj >nLepHits-1-flagCorrelated; jj--)
               LepHits[jj].trmult    = flagCorrelated;

         } // loop over rich
         } // loop over sectors
         hmultkickrich->Fill(kickrich_count);
         hele->Fill(ele);
         hpos->Fill(pos);

      //-------------- end of lepton analysis  ------------------------------------------------------------


         }
      }   //loop over events

      f->Close();
//    } // multiplicity of file
  }  // file loop


  hfile->cd();
  ntuple_lep->Write();
  hmultrich->Write();
  hmultkick->Write();
  hmultkickrich->Write();
  hele->Write();
  hpos->Write();
  hele_sel->Write();
  hpos_sel->Write();
  htrackperring->Write();
  htofino_incl->Write();
  htofino_sho->Write();
  hdownscFlag->Write();
  hdiffTheta->Write();
  hdiffPhi->Write();
  hdiffPhiTheta->Write();

  hfile->Write();
  hfile->Save();
  hfile->Close();

  cout << "event processed " <<  TotalEntries << endl;
  cout << "leptons found " <<  TotalLeptons << endl;

  timer.Stop();
  printf("------------------------------------------------------\n");
  printf("Events processed\t: %i\n",TotalEntries);
  printf("Real time\t\t: %f\n",timer.RealTime());
  printf("Cpu time\t\t: %f\n",timer.CpuTime());
  if (TotalEntries) printf("Performance\t\t: %f cpu-s/ev\n",timer.CpuTime()/TotalEntries);
  if (TotalEntries) printf("Performance (real)\t: %f ev/s\n",TotalEntries/timer.RealTime());

  return (1);
}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
        TROOT app("lvl2_efficiency","compiled lvl2_efficiency macro");
        gROOT->SetBatch(kTRUE);

 Bool_t verbose=false;
   Int_t step=0;
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
				case '1':
					step=1;
					cout << "setting step "<<step<<endl;
					break;
				case '2':
					step=2;
					cout << "setting step "<<step<<endl;
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
			lvl2_effi2(argv[cur_arg],step,verbose);
			exit(0);
		case 2:
			lvl2_effi2(argv[cur_arg],step,verbose);
			exit(0);
		default:
	                cout << argv[0] << " [-v12] pathfilename" << endl;
	                cout <<  "-v  be verbose" << endl;
	                cout <<  "-1  run only step 1" << endl;
	                cout <<  "-2  run only step 2" << endl;
	                cout <<  " pathfilename is either a dst file or text file containing a list of files to be processed" << endl;
	                cout <<  " full path of the dst-files must be given Int_t the list file" << endl;
	                cout <<  " outputfile will be created in same folder as pathfilename.lst" << endl;
        	        exit(1);
        }
}
#endif
