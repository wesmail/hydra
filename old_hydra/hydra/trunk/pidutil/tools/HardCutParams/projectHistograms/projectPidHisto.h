//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Mar 21 08:20:27 2005 by ROOT version 4.00/03
// from TTree pTrackCandidate/Lepton track candidate properties
// found on file: ../../../full_test.root
//////////////////////////////////////////////////////////

#ifndef projectPidHisto_h
#define projectPidHisto_h

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include <iostream>

#define MAX_ALG 3 // number of tracking algorithms 0 - KICK, 1-Spline, 2-Runge Kutta

using namespace std;

class projectPidHisto {
public :

   // histograms
   TH2F *pMomVsBeta[MAX_ALG][6][2][2]; // sector/system/charge
   TH1F *pRichPM[MAX_ALG][6]; 		  // Ring Pattern Matrix alg/sector
   TH1F *pRichRC[MAX_ALG][6]; 		  // Ring Ring Centroid
   TH1F *pRichNP[MAX_ALG][6]; 		  // Ring Number of Fired Pads
   TH1F *pRichAC[MAX_ALG][6]; 		  // Ring Average Charge

   TH2F *pShowF10VsMom[MAX_ALG][6];  // Multiplication Factor
   TH2F *pShowF20VsMom[MAX_ALG][6];  // Multiplication Factor
   TH2F *pShowMaxFVsMom[MAX_ALG][6]; // Max Multiplication Factor
   TH2F *pShowSum0VsMom[MAX_ALG][6]; // Sum of Charge vs momentum
   TH2F *pShowSumDiffVsMom[MAX_ALG][6]; // Sum Difference sum1+sum2-sm0 of Charge vs momentum

   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Float_t         pmat;
   Float_t         acharge;
   Float_t         npad;
   Float_t         rcent;
   Float_t         beta_k;
   Float_t         mom_k;
   Float_t         pull;
   Float_t         pol_k;
   Float_t         mass_k;
   Float_t         beta_s;
   Float_t         mom_s;
   Float_t         chi2_s;
   Float_t         pol_s;
   Float_t         mass_s;
   Float_t         beta_r;
   Float_t         mom_r;
   Float_t         chi2_r;
   Float_t         pol_r;
   Float_t         mass_r;
   Float_t         maxF;
   Float_t         F10;
   Float_t         F20;
   Float_t         sum0;
   Float_t         sum1;
   Float_t         sum2;
   Float_t         tof;
   Float_t         theta;
   Float_t         phi;
   Float_t         sec;
   Float_t         sys;
   Float_t         chi2_inmdc;
   Float_t         chi2_outmdc;
   Float_t         showerId;
   Float_t         tofId;
   Float_t         richId;
   Float_t         kickId;
   Float_t         splineId;
   Float_t         rungeId;
   Float_t         isRichKick;
   Float_t         isRichSpline;
   Float_t         isRichRungeKutta;

   // List of branches
   TBranch        *b_pmat;   //!
   TBranch        *b_acharge;   //!
   TBranch        *b_npad;   //!
   TBranch        *b_rcent;   //!
   TBranch        *b_beta_k;   //!
   TBranch        *b_mom_k;   //!
   TBranch        *b_pull;   //!
   TBranch        *b_pol_k;   //!
   TBranch        *b_mass_k;   //!
   TBranch        *b_beta_s;   //!
   TBranch        *b_mom_s;   //!
   TBranch        *b_chi2_s;   //!
   TBranch        *b_pol_s;   //!
   TBranch        *b_mass_s;   //!
   TBranch        *b_beta_r;   //!
   TBranch        *b_mom_r;   //!
   TBranch        *b_chi2_r;   //!
   TBranch        *b_pol_r;   //!
   TBranch        *b_mass_r;   //!
   TBranch        *b_maxF;   //!
   TBranch        *b_F10;   //!
   TBranch        *b_F20;   //!
   TBranch        *b_sum0;   //!
   TBranch        *b_sum1;   //!
   TBranch        *b_sum2;   //!
   TBranch        *b_tof;   //!
   TBranch        *b_theta;   //!
   TBranch        *b_phi;   //!
   TBranch        *b_sec;   //!
   TBranch        *b_sys;   //!
   TBranch        *b_chi2_inmdc;   //!
   TBranch        *b_chi2_outmdc;   //!
   TBranch        *b_showerId;   //!
   TBranch        *b_tofId;   //!
   TBranch        *b_richId;   //!
   TBranch        *b_kickId;   //!
   TBranch        *b_splineId;   //!
   TBranch        *b_rungeId;   //!
   TBranch        *b_isRichKick;   //!
   TBranch        *b_isRichSpline;   //!
   TBranch        *b_isRichRungeKutta;   //!

   projectPidHisto(TTree *tree=0, TFile *outFile=0);
   ~projectPidHisto();
   Int_t  Cut(Int_t entry);
   Int_t  GetEntry(Int_t entry);
   Int_t  LoadTree(Int_t entry);
   void   Init(TTree *tree, TFile *outFile);
   void   Loop(Int_t events=0, Int_t startevt=0);
   Bool_t Notify();
   void   Show(Int_t entry = -1);
   void   fillHistograms(Int_t alg);
   Int_t  getIndex(Int_t pol);
};
#endif
