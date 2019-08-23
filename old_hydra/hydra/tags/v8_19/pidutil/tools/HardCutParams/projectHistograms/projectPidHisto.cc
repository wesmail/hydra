#include "TStyle.h"
#include "TCanvas.h"

#include "projectPidHisto.h"

projectPidHisto::projectPidHisto(TTree *tree,TFile *outFile)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("outPidNtuple.root");
      if (!f) {
         f = new TFile("outPidNtuple.root");
      }
      tree = (TTree*)gDirectory->Get("Ntuple");

   }
   Init(tree,outFile);
}

projectPidHisto::~projectPidHisto()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t projectPidHisto::GetEntry(Int_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}

Int_t projectPidHisto::LoadTree(Int_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Int_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void projectPidHisto::Init(TTree *tree, TFile *outFile)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses of the tree
   // will be set. It is normaly not necessary to make changes to the
   // generated code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running with PROOF.

   // Set branch addresses
   if (tree == 0) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("pmat",&pmat);
   fChain->SetBranchAddress("acharge",&acharge);
   fChain->SetBranchAddress("npad",&npad);
   fChain->SetBranchAddress("rcent",&rcent);
   fChain->SetBranchAddress("beta_k",&beta_k);
   fChain->SetBranchAddress("mom_k",&mom_k);
   fChain->SetBranchAddress("pull",&pull);
   fChain->SetBranchAddress("pol_k",&pol_k);
   fChain->SetBranchAddress("mass_k",&mass_k);
   fChain->SetBranchAddress("beta_s",&beta_s);
   fChain->SetBranchAddress("mom_s",&mom_s);
   fChain->SetBranchAddress("chi2_s",&chi2_s);
   fChain->SetBranchAddress("pol_s",&pol_s);
   fChain->SetBranchAddress("mass_s",&mass_s);
   fChain->SetBranchAddress("beta_r",&beta_r);
   fChain->SetBranchAddress("mom_r",&mom_r);
   fChain->SetBranchAddress("chi2_r",&chi2_r);
   fChain->SetBranchAddress("pol_r",&pol_r);
   fChain->SetBranchAddress("mass_r",&mass_r);
   fChain->SetBranchAddress("maxF",&maxF);
   fChain->SetBranchAddress("F10",&F10);
   fChain->SetBranchAddress("F20",&F20);
   fChain->SetBranchAddress("sum0",&sum0);
   fChain->SetBranchAddress("sum1",&sum1);
   fChain->SetBranchAddress("sum2",&sum2);
   fChain->SetBranchAddress("tof",&tof);
   fChain->SetBranchAddress("theta",&theta);
   fChain->SetBranchAddress("phi",&phi);
   fChain->SetBranchAddress("sec",&sec);
   fChain->SetBranchAddress("sys",&sys);
   fChain->SetBranchAddress("chi2_inmdc",&chi2_inmdc);
   fChain->SetBranchAddress("chi2_outmdc",&chi2_outmdc);
   fChain->SetBranchAddress("showerId",&showerId);
   fChain->SetBranchAddress("tofId",&tofId);
   fChain->SetBranchAddress("richId",&richId);
   fChain->SetBranchAddress("kickId",&kickId);
   fChain->SetBranchAddress("splineId",&splineId);
   fChain->SetBranchAddress("rungeId",&rungeId);
   fChain->SetBranchAddress("isRichKick",&isRichKick);
   fChain->SetBranchAddress("isRichSpline",&isRichSpline);
   fChain->SetBranchAddress("isRichRungeKutta",&isRichRungeKutta);
   Notify();

   // output file
   if(outFile==0) return;
   outFile->cd();

   // histograms
   Char_t name[256];
   
   for(Int_t iAlg = 0; iAlg < MAX_ALG; iAlg++) // tracking algorithms
   {
     for(Int_t iSec = 0; iSec < 6; iSec++) // sectors
     {
       for(Int_t iSys = 0; iSys < 2; iSys++) // systems
       {
         for(Int_t iPol = 0; iPol < 2; iPol++) // polarization
         {
		   sprintf(name,"pMomVsBeta_alg%d_sec%d_sys%d_pol%d",iAlg,iSec,iSys,iPol);
           pMomVsBeta[iAlg][iSec][iSys][iPol] = new TH2F(name,name,140,0,1.4,200,0,2000);
	     } 
	   }

	   sprintf(name,"pRichPM_alg%d_sec%d",iAlg,iSec);
       pRichPM[iAlg][iSec] = new TH1F(name,name,200,0,1000);

	   sprintf(name,"pRichRC_alg%d_sec%d",iAlg,iSec);
       pRichRC[iAlg][iSec] = new TH1F(name,name,200,-1,5);

	   sprintf(name,"pRichNP_alg%d_sec%d",iAlg,iSec);
       pRichNP[iAlg][iSec] = new TH1F(name,name,50,0,50);

	   sprintf(name,"pRichAC_alg%d_sec%d",iAlg,iSec);
       pRichAC[iAlg][iSec] = new TH1F(name,name,100,0,100);

	   sprintf(name,"pShowF10VsMom_alg%d_sec%d",iAlg,iSec);
       pShowF10VsMom[iAlg][iSec] = new TH2F(name,name,200,0,2000,500,0,50);

	   sprintf(name,"pShowF20VsMom_alg%d_sec%d",iAlg,iSec);
       pShowF20VsMom[iAlg][iSec] = new TH2F(name,name,200,0,2000,500,0,50);

	   sprintf(name,"pShowMaxFVsMom_alg%d_sec%d",iAlg,iSec);
       pShowMaxFVsMom[iAlg][iSec] = new TH2F(name,name,200,0,2000,500,0,50);

	   sprintf(name,"pShowSum0VsMom_alg%d_sec%d",iAlg,iSec);
       pShowSum0VsMom[iAlg][iSec] = new TH2F(name,name,200,0,2000,1000,0,100);

	   sprintf(name,"pShowSumDiffVsMom_alg%d_sec%d",iAlg,iSec);
       pShowSumDiffVsMom[iAlg][iSec] = new TH2F(name,name,200,0,2000,1000,-200,300);
     }
   } 
}

Bool_t projectPidHisto::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.

   // Get branch pointers
   b_pmat = fChain->GetBranch("pmat");
   b_acharge = fChain->GetBranch("acharge");
   b_npad = fChain->GetBranch("npad");
   b_rcent = fChain->GetBranch("rcent");
   b_beta_k = fChain->GetBranch("beta_k");
   b_mom_k = fChain->GetBranch("mom_k");
   b_pull = fChain->GetBranch("pull");
   b_pol_k = fChain->GetBranch("pol_k");
   b_mass_k = fChain->GetBranch("mass_k");
   b_beta_s = fChain->GetBranch("beta_s");
   b_mom_s = fChain->GetBranch("mom_s");
   b_chi2_s = fChain->GetBranch("chi2_s");
   b_pol_s = fChain->GetBranch("pol_s");
   b_mass_s = fChain->GetBranch("mass_s");
   b_beta_r = fChain->GetBranch("beta_r");
   b_mom_r = fChain->GetBranch("mom_r");
   b_chi2_r = fChain->GetBranch("chi2_r");
   b_pol_r = fChain->GetBranch("pol_r");
   b_mass_r = fChain->GetBranch("mass_r");
   b_maxF = fChain->GetBranch("maxF");
   b_F10 = fChain->GetBranch("F10");
   b_F20 = fChain->GetBranch("F20");
   b_sum0 = fChain->GetBranch("sum0");
   b_sum1 = fChain->GetBranch("sum1");
   b_sum2 = fChain->GetBranch("sum2");
   b_tof = fChain->GetBranch("tof");
   b_theta = fChain->GetBranch("theta");
   b_phi = fChain->GetBranch("phi");
   b_sec = fChain->GetBranch("sec");
   b_sys = fChain->GetBranch("sys");
   b_chi2_inmdc = fChain->GetBranch("chi2_inmdc");
   b_chi2_outmdc = fChain->GetBranch("chi2_outmdc");
   b_showerId = fChain->GetBranch("showerId");
   b_tofId = fChain->GetBranch("tofId");
   b_richId = fChain->GetBranch("richId");
   b_kickId = fChain->GetBranch("kickId");
   b_splineId = fChain->GetBranch("splineId");
   b_rungeId = fChain->GetBranch("rungeId");
   b_isRichKick = fChain->GetBranch("isRichKick");
   b_isRichSpline = fChain->GetBranch("isRichSpline");
   b_isRichRungeKutta = fChain->GetBranch("isRichRungeKutta");

   return kTRUE;
}

void projectPidHisto::Show(Int_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}

Int_t projectPidHisto::getIndex(Int_t pol)
{
	if(pol==1) return 0;
	else if(pol == -1) return 1; 
    else return 10;
 		
return 10;
}

void projectPidHisto::fillHistograms(Int_t alg)
{
Int_t pol=10;

    switch(alg)
	{
		case 0:

		pol = getIndex((int)pol_k);

        pMomVsBeta[alg][(int)sec][(int)sys][pol]->Fill(beta_k,mom_k); // sector/system/charge
        pRichPM[alg][(int)sec]->Fill(pmat); 		  // Ring Pattern Matrix
        pRichRC[alg][(int)sec]->Fill(rcent); 		  // Ring Ring Centroid
        pRichNP[alg][(int)sec]->Fill(npad); 		  // Ring Number of Fired Pads
        pRichAC[alg][(int)sec]->Fill(acharge); 		  // Ring Average Charge

		// only for leptons making shower
		if(sys == 0 && beta_k > 0.8)
		{
			if(sum0>0)
			{
			pShowF10VsMom[alg][(int)sec]->Fill(mom_k,F10);  // Multiplication Factor
			pShowF20VsMom[alg][(int)sec]->Fill(mom_k,F20);  // Multiplication Factor
			pShowMaxFVsMom[alg][(int)sec]->Fill(mom_k,maxF);  // Multiplication Factor
			pShowSum0VsMom[alg][(int)sec]->Fill(mom_k,sum0);  // Sum of Charge
			}

			pShowSumDiffVsMom[alg][(int)sec]->Fill(mom_k,sum1+sum2-sum0);  // Sum Difference of Charge
		}

	    break;	

	    case 1:
		pol = getIndex((int)pol_s);

        pMomVsBeta[alg][(int)sec][(int)sys][pol]->Fill(beta_s,mom_s); // sector/system/charge
        pRichPM[alg][(int)sec]->Fill(pmat); 		  // Rich Pattern Matrix
        pRichRC[alg][(int)sec]->Fill(rcent); 		  // Rich Ring Centroid
        pRichNP[alg][(int)sec]->Fill(npad); 		  // Rich Number of Fired Pads
        pRichAC[alg][(int)sec]->Fill(acharge); 		  // Rich Average Charge

		// only for leptons making shower
		if(sys == 0 && beta_s > 0.8)
		{
			if(sum0>0)
			{
			pShowF10VsMom[alg][(int)sec]->Fill(mom_s,F10);  // Multiplication Factor
			pShowF20VsMom[alg][(int)sec]->Fill(mom_s,F20);  // Multiplication Factor
			pShowMaxFVsMom[alg][(int)sec]->Fill(mom_s,maxF);  // Multiplication Factor
			pShowSum0VsMom[alg][(int)sec]->Fill(mom_s,sum0);  // Sum of Charge
			}
			pShowSumDiffVsMom[alg][(int)sec]->Fill(mom_s,sum1+sum2-sum0);  // Sum Difference of Charge
		}

	    break;	

        case 2:
		pol = getIndex((int)pol_r);

        pMomVsBeta[alg][(int)sec][(int)sys][pol]->Fill(beta_r,mom_r); // sector/system/charge
        pRichPM[alg][(int)sec]->Fill(pmat); 		  // Ring Pattern Matrix
        pRichRC[alg][(int)sec]->Fill(rcent); 		  // Ring Ring Centroid
        pRichNP[alg][(int)sec]->Fill(npad); 		  // Ring Number of Fired Pads
        pRichAC[alg][(int)sec]->Fill(acharge); 		  // Ring Average Charge

		// only for leptons making shower
		if(sys == 0 && beta_r > 0.8)
		{
			if(sum0>0)
			{
			pShowF10VsMom[alg][(int)sec]->Fill(mom_r,F10);    // Multiplication Factor
			pShowF20VsMom[alg][(int)sec]->Fill(mom_r,F20);    // Multiplication Factor
			pShowMaxFVsMom[alg][(int)sec]->Fill(mom_r,maxF);  // Multiplication Factor
			pShowSum0VsMom[alg][(int)sec]->Fill(mom_r,sum0);  // Sum of Charge
			}
			pShowSumDiffVsMom[alg][(int)sec]->Fill(mom_r,sum1+sum2-sum0);  // Sum Difference of Charge
		}
	    break;	

		default:
		break;
	}
}
Int_t projectPidHisto::Cut(Int_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}


void projectPidHisto::Loop(Int_t events, Int_t startevt)
{
//   In a ROOT session, you can do:
//      Root > .L fillTrackCandidateHist.C
//      Root > fillTrackCandidateHist t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   Int_t nentries = Int_t(fChain->GetEntriesFast());
   if(events > 0) nentries = events;

   Int_t nbytes = 0, nb = 0;

   for (Int_t jentry=startevt; jentry<nentries;jentry++) {
      Int_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

	  if(jentry%10000 == 0) cout << " event: "  << jentry << endl; 
	

      if(richId < 0) continue; // only electron candidates 

      if(kickId >= 0 && isRichKick)
	  {
	 	fillHistograms(0); 
	  }
	  if(splineId>=0 && isRichSpline)
	  {
	 	fillHistograms(1); 
	  }
	  if(chi2_s>0 && rungeId>=0 && isRichRungeKutta)
	  {
	 	fillHistograms(2); 
	  } 
   }
}
