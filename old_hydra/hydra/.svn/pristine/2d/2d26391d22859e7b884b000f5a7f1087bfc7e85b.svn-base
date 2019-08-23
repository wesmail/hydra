//////////////////////////////////////////////////////////////
// reduction factor of the RICH IPU
// load the library  > .L redu.cc+
// call the function > reduction(...)
//////////////////////////////////////////////////////////////

#ifndef __CINT__
#include <iostream.h>
#include <fstream.h>
#include <unistd.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TCanvas.h>

//#include "/u/alberica/hydra/triggerutil/hrichhitIPU.h"
#include "hrichhitIPU.h"
#include "htree.h"
#include "hparticle.h"
#include "hkicktrack.h"
#include "hmatchushower.h"
#include "hmatchutof.h"
#include "hshowerhittof.h"
#include "hrichhit.h"
#include "hstarthit.h"
#include "htofhit.h"
#include "hmdcraw.h"
#include "hmdccal1.h"
#include "hmdccal2.h"
#include "hmdchit.h"
#include "hmdcseg.h"

#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hevent.h"
#include "haddef.h"
#include "hgeantdef.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hgeomvector.h"
#endif

#define RINGMIN 4
#define RINGMAX 11
#define VETOMIN 1
#define VETOMAX 6


typedef struct {
  Int_t X;
  Int_t Y;
  Int_t rqua;
  Int_t vqua;
  Float_t th;
  Float_t ph;
  Int_t pad;
  Int_t flag;
  Int_t flag2;
} RichHittype;



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
		outputFile  = outputpath+prefix;
	}else{
		outputFile  = outputpath+file+"_"+prefix;
	}

	cout << "File output: " << outputFile << ".root" << endl;
	cout << "File output: " << outputFile << ".txt" << endl;
}

Int_t reduction(TString filename,Int_t nr_events,Bool_t verbose)
{
	//  gROOT->Reset();
	TString outFile="";

	process_filename_lst(filename,outFile,"reduction");

	//--------------------- output file --------------------------------------------------------
	TFile *hfile = new TFile(outFile+".root","RECREATE");
	ofstream redufile(outFile+".txt");
	//------------------------------------------------------------------------------------------

	TH2F *hredu1 = new TH2F("hredu1","Reduction Factor",6,4,10,5,1,6);
	TH2F *hredu2 = new TH2F("hredu2","Reduction Factor",6,4,10,5,1,6);

	Int_t redu1[RINGMAX][VETOMAX]; // this is the array for the reduction factor
	Int_t redu2[RINGMAX][VETOMAX]; // this is the array for the reduction factor
	Int_t rings[RINGMAX][VETOMAX]; // this is the array for the number of rings

	for (Int_t rt=RINGMIN;rt<RINGMAX;rt++) {
		for (Int_t vt=VETOMIN;vt<VETOMAX;vt++) {
			redu1[rt][vt] = 0;
			redu2[rt][vt] = 0;
		}
	}

	Float_t red1;
	Float_t red2;

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
//         if(!files[i].Contains("/")){// only filename or path?
//			files[i]=file_out+(TString)"/"+(TString)files[i];
//			}
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
	Int_t DSEntries = 0;

	for (Int_t xx=0; xx<nbOfFiles; xx++) {
		cout << "File Nr " << xx << "    Name: " << files[xx] << endl;

		TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(files[xx]);
		if (!f) f = new TFile(files[xx]);
		if (!f) {cout << "Could not open file!" << endl; continue;}
		if (f->IsZombie()){ cout << "Zombie file!" << endl; continue;}

		HTree *T = (HTree*)gDirectory->Get("T");
		if(!T) {cout << "No tree in file!" << endl; continue;}

		// ----------------------------------------------------------------------------------------
		HEventHeader* fEventHead = (HEventHeader*)f->Get("EventHeader");
		TBranch* evt = T->GetBranch("EventHeader.");
		if(evt) evt->SetAddress(&fEventHead);

		f->cd("dirTrigger");
		HRichHitIPU* ptlHRichHitIPU = new HRichHitIPU();
		HLinearCategory* catlHRichHitIPU = (HLinearCategory*) gDirectory->Get("HRichHitIPU");
		TBranch* brlHRichHitIPU = T->GetBranch("HRichHitIPU.");
		if(brlHRichHitIPU) brlHRichHitIPU->SetAddress(&catlHRichHitIPU);
		TIterator *iterHRichHitIPU = catlHRichHitIPU->MakeIterator();

		f->cd();

		Int_t MU_nRichHits[6];
		Int_t sector;

		//----------------------------------------------------------
		//Stat_t ientries = 50;
		Stat_t ientries = T->GetEntries();
		TotalEntries += (Int_t) ientries;
		//---------- Eventloop -------------------------------------
		for (Int_t i=0; i<ientries;i++) {
			//  cout << "***************************evt " << i << endl;
			//  cout << "EventSeqNumber: " << fHead->getEventSeqNumber() << endl;
			if (i % 5000 == 0) cout << "Event " << i << endl;

			evt->GetEntry(i);

			//DF=0-> LVL2, DF=1->LVL1
			if ( (fEventHead)->getDownscalingFlag()==0) continue; //analysis done for LVL1
			//if ( (fEventHead)->getDownscalingFlag()==1) continue; //analysis done for LVL2
			DSEntries++;
			brlHRichHitIPU->GetEntry(i);

			for (Int_t actRingThr = RINGMIN;actRingThr<RINGMAX;actRingThr++) {
				for (Int_t actVetoThr = VETOMIN;actVetoThr<VETOMAX;actVetoThr++) {

					iterHRichHitIPU->Reset();

					rings[actRingThr][actVetoThr] = 0;

					for ( sector = 0; sector < 6; sector++) {
						MU_nRichHits[sector] = 0;
					}

					Int_t nRichHitsTotal = 0;
					Int_t xRichHit, yRichHit;

					//------------------------- loop over RICH IPU --------------------------------------
					Int_t RingsumPattern[6][96][96];
					for (Int_t s=0;s<6;s++)
						for (Int_t r=0;r<96;r++)
							for(Int_t c=0;c<96;c++)
								RingsumPattern[s][r][c]= 0;

					while ((ptlHRichHitIPU = (HRichHitIPU *)iterHRichHitIPU->Next()) != 0) {
						if( (ptlHRichHitIPU->getQuality()>actRingThr) &&(ptlHRichHitIPU->getVetoQuality()<actVetoThr)) {
							sector = ptlHRichHitIPU->getSector();
							yRichHit = ptlHRichHitIPU->getY();
							xRichHit = ptlHRichHitIPU->getX();
							RingsumPattern[sector][xRichHit][yRichHit]=ptlHRichHitIPU->getQuality();
						}
					}// end while
					iterHRichHitIPU->Reset();
					while ((ptlHRichHitIPU = (HRichHitIPU *)iterHRichHitIPU->Next()) != 0) {
						sector = ptlHRichHitIPU->getSector();
						yRichHit = ptlHRichHitIPU->getY();
						xRichHit = ptlHRichHitIPU->getX();
						if (RingsumPattern[sector][xRichHit][yRichHit] ){
							Int_t CurrentSum = ptlHRichHitIPU->getQuality();
							if ( CurrentSum  >= RingsumPattern[sector][xRichHit+1][yRichHit] &&
								CurrentSum  >= RingsumPattern[sector][xRichHit][yRichHit+1] &&
								CurrentSum  > RingsumPattern[sector][xRichHit-1][yRichHit] &&
								CurrentSum  > RingsumPattern[sector][xRichHit][yRichHit-1] ) {
								//     cout <<"***RICH " << i << "   " << sector << "   " << MU_RichHits[sector][MU_nRichHits[sector]].th << "   " <<  MU_RichHits[sector][MU_nRichHits[sector]].ph << "  " << MU_RichHits[sector][MU_nRichHits[sector]].rqua << "  " <<  MU_RichHits[sector][MU_nRichHits[sector]].vqua << endl;

								MU_nRichHits[sector]++;
								nRichHitsTotal++;

								rings[actRingThr][actVetoThr]++;
							}
						}
					}//------------------------- end of loop over RICH IPU --------------------------------------


					if (rings[actRingThr][actVetoThr]>=1)  // reduction factor with 1 ring detected
						redu1[actRingThr][actVetoThr]++;

					if (rings[actRingThr][actVetoThr]>=2)  // reduction factor with 2 rings detected
						redu2[actRingThr][actVetoThr]++;

				}  // actVetoThr loop
			}  // actRingThr loop


			catlHRichHitIPU->Clear();

		} // eventloop
		f->Close();
	} // file loop

	redufile << "****************************************************************" << endl;
	redufile << "*** event processed " <<  DSEntries << endl;
	for (Int_t rt=RINGMIN;rt<RINGMAX;rt++) {
		for (Int_t vt=VETOMIN;vt<VETOMAX;vt++) {
			redufile << "ring thr: " << rt << "  veto thr: " << vt << "  evts with >= 1 ring: " << redu1[rt][vt]
				<< "  evts with >= 2 rings: "<< redu2[rt][vt] << endl;

			red1 = ((Float_t) redu1[rt][vt]) / ((Float_t) DSEntries);
			red2 = ((Float_t) redu2[rt][vt]) / ((Float_t) DSEntries);

			hredu1->Fill(rt,vt,red1);
			hredu2->Fill(rt,vt,red2);

		}
	}
	redufile << "****************************************************************" << endl;
	redufile.close();

	hfile->cd();
	hredu1->Write();
	hredu2->Write();
	hfile->Close();

	return (1);
}

#ifndef __CINT
Int_t main(Int_t argc, Char_t** argv)
{
	TROOT app("reduction","compiled reduction macro");
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
			reduction(argv[cur_arg],0,verbose);
			exit(0);
		case 2:
			reduction(argv[cur_arg],atoi(argv[cur_arg+1]),verbose);
			exit(0);
		default:
			cout << argv[0] << " [-v] filename.root [number events]" << endl;
			cout <<  " pathfilename is either a dst file or text file containing a list of files to be processed" << endl;
         cout <<  " full path of the dst-files must be given Int_t the list file" << endl;
         cout <<  " outputfile will be created in same folder as pathfilename.lst" << endl;
	      cout <<  "-v  be verbose" << endl;
			exit(1);
	}
}
#endif
