#define pairsE_cxx
#include "pairsE.h"
#include <TH2.h>
#include <TError.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;
pairsE::pairsE(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
       Error("ctor","pointer to tree is zero");
//       TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("sim.root");
//       if (!f) {
//          f = new TFile("sim.root");
//       }
//       tree = (TTree*)gDirectory->Get("PairsFilter");

   }
   Init(tree);
}

pairsE::~pairsE()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t pairsE::GetEntry(Int_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Int_t pairsE::LoadTree(Int_t entry)
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

void pairsE::Init(TTree *tree)
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

   fChain->SetBranchAddress("invmass",&invmass);
   fChain->SetBranchAddress("opang",&opang);
   fChain->SetBranchAddress("rap",&rap);
   fChain->SetBranchAddress("pt",&pt);
   fChain->SetBranchAddress("charge",&charge);
   fChain->SetBranchAddress("isCutNb",&isCutNb);
   fChain->SetBranchAddress("idxPart1",&idxPart1);
   fChain->SetBranchAddress("idxPart2",&idxPart2);
   fChain->SetBranchAddress("prob1",&prob1);
   fChain->SetBranchAddress("prob2",&prob2);
   fChain->SetBranchAddress("pid1",&pid1);
   fChain->SetBranchAddress("pid2",&pid2);
   fChain->SetBranchAddress("idxpidcand1",&idxpidcand1);
   fChain->SetBranchAddress("sys1",&sys1);
   fChain->SetBranchAddress("r1",&r1);
   fChain->SetBranchAddress("z1",&z1);
   fChain->SetBranchAddress("massexp1",&massexp1);
   fChain->SetBranchAddress("betaexp1",&betaexp1);
   fChain->SetBranchAddress("momalgidx1",&momalgidx1);
   fChain->SetBranchAddress("chrg1",&chrg1);
   fChain->SetBranchAddress("mostprobpid1",&mostprobpid1);
   fChain->SetBranchAddress("weightmostprobpid1",&weightmostprobpid1);
   fChain->SetBranchAddress("theta1",&theta1);
   fChain->SetBranchAddress("phi1",&phi1);
   fChain->SetBranchAddress("sec1",&sec1);
   fChain->SetBranchAddress("idxpidcand2",&idxpidcand2);
   fChain->SetBranchAddress("sys2",&sys2);
   fChain->SetBranchAddress("r2",&r2);
   fChain->SetBranchAddress("z2",&z2);
   fChain->SetBranchAddress("massexp2",&massexp2);
   fChain->SetBranchAddress("betaexp2",&betaexp2);
   fChain->SetBranchAddress("momalgidx2",&momalgidx2);
   fChain->SetBranchAddress("chrg2",&chrg2);
   fChain->SetBranchAddress("mostprobpid2",&mostprobpid2);
   fChain->SetBranchAddress("weightmostprobpid2",&weightmostprobpid2);
   fChain->SetBranchAddress("theta2",&theta2);
   fChain->SetBranchAddress("phi2",&phi2);
   fChain->SetBranchAddress("sec2",&sec2);
   fChain->SetBranchAddress("drmt1",&drmt1);
   fChain->SetBranchAddress("drmp1",&drmp1);
   fChain->SetBranchAddress("drmt2",&drmt2);
   fChain->SetBranchAddress("drmp2",&drmp2);
   fChain->SetBranchAddress("tof1",&tof1);
   fChain->SetBranchAddress("tof2",&tof2);
   fChain->SetBranchAddress("rpadnr1",&rpadnr1);
   fChain->SetBranchAddress("rcentroid1",&rcentroid1);
   fChain->SetBranchAddress("rt1",&rt1);
   fChain->SetBranchAddress("rp1",&rp1);
   fChain->SetBranchAddress("rpatmat1",&rpatmat1);
   fChain->SetBranchAddress("rhoutra1",&rhoutra1);
   fChain->SetBranchAddress("rampl1",&rampl1);
   fChain->SetBranchAddress("rlocmax41",&rlocmax41);
   fChain->SetBranchAddress("rpadnr2",&rpadnr2);
   fChain->SetBranchAddress("rcentroid2",&rcentroid2);
   fChain->SetBranchAddress("rt2",&rt2);
   fChain->SetBranchAddress("rp2",&rp2);
   fChain->SetBranchAddress("rpatmat2",&rpatmat2);
   fChain->SetBranchAddress("rhoutra2",&rhoutra2);
   fChain->SetBranchAddress("rampl2",&rampl2);
   fChain->SetBranchAddress("rlocmax42",&rlocmax42);
   fChain->SetBranchAddress("mom1",&mom1);
   fChain->SetBranchAddress("mom2",&mom2);
   fChain->SetBranchAddress("doubleHit",&doubleHit);
   fChain->SetBranchAddress("qspline1",&qspline1);
   fChain->SetBranchAddress("qspline2",&qspline2);
   fChain->SetBranchAddress("innerchisquare1",&innerchisquare1);
   fChain->SetBranchAddress("innerchisquare2",&innerchisquare2);
   fChain->SetBranchAddress("outerchisquare1",&outerchisquare1);
   fChain->SetBranchAddress("outerchisquare2",&outerchisquare2);
   fChain->SetBranchAddress("distancetovertex1",&distancetovertex1);
   fChain->SetBranchAddress("distancetovertex2",&distancetovertex2);
   fChain->SetBranchAddress("iscpcandidate1",&iscpcandidate1);
   fChain->SetBranchAddress("iscpcandidate2",&iscpcandidate2);
   fChain->SetBranchAddress("opangcpcandidate1",&opangcpcandidate1);
   fChain->SetBranchAddress("opangcpcandidate2",&opangcpcandidate2);
   // C U T S
   fChain->SetBranchAddress("isGoodOpang",&isGoodOpang);
   fChain->SetBranchAddress("isNotDoubleHit",&isNotDoubleHit);


   //histogram booking
   harr = new TObjArray();

   // CHANGE BINNING HERE
   Float_t xbins[] = {0.,10.,20.,30.,40., 50.,60.,70.,80.,90.,
		             100.,120.,140.,160.,
		             180.0,210.,250.,
		             300.,350.,420.,500.,580.,
		             680.,780.,880.,1000.};

   Int_t nbins = sizeof(xbins)/sizeof(Float_t);

   ///////////////////////////////////////////////////////////////////////
   char name[256];

   for(Int_t icut=0; icut<MAXCUT; icut++)
   {

	   

       for(Int_t ipol=0; ipol<MAXPOL; ipol++)
       {
	   sprintf(name,"hmass_cut%d_pol%d",icut,ipol);
	   hmass_cut_pol[icut][ipol] = new TH1F(name,name,nbins-1,xbins);
	   harr->Add(hmass_cut_pol[icut][ipol]);
	   
	   sprintf(name,"hoangle_cut%d_pol%d",icut,ipol);
	   hoangle_cut_pol[icut][ipol] = new TH1F(name,name,180,0,180);
	   harr->Add(hoangle_cut_pol[icut][ipol]);

	   sprintf(name,"hrap_cut%d_pol%d",icut,ipol);
	   hrap_cut_pol[icut][ipol] = new TH1F(name,name,60,0,3);
	   harr->Add(hrap_cut_pol[icut][ipol]);

	   sprintf(name,"hpt_cut%d_pol%d",icut,ipol);
	   hpt_cut_pol[icut][ipol] = new TH1F(name,name,nbins-1,xbins);
	   harr->Add(hpt_cut_pol[icut][ipol]);
       }

   }

   // Init efficiency matrices

   pLowRangeEffFile = new TFile("/u/eberl/EFFICIENCY/matricesEffSingle.LowRange.root","READ");
   if (pLowRangeEffFile) 
   {
       pLowRangeEffFile->cd();
       p3DeffEleLow = (TH3F*) pLowRangeEffFile->Get("effi3DEle"); 
       p3DeffPosiLow = (TH3F*) pLowRangeEffFile->Get("effi3DPosi"); 
   }
   else Error("Init","pointer to eff matrix file is NULL");

   pHighRangeEffFile = new TFile("/u/eberl/EFFICIENCY/matricesEffSingle.HighRange.root","READ");
   if (pHighRangeEffFile) 
   {
       pHighRangeEffFile->cd();
       p3DeffEleHigh = (TH3F*) pHighRangeEffFile->Get("effi3DEle"); 
       p3DeffPosiHigh = (TH3F*) pHighRangeEffFile->Get("effi3DPosi"); 
   }
   else Error("Init","pointer to eff matrix file is NULL");
   
   Notify();
}

Bool_t pairsE::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.
    cout<<"start of notify function"<<endl;

   // Get branch pointers
    b_invmass = fChain->GetBranch("invmass");
    b_opang = fChain->GetBranch("opang");
    b_rap = fChain->GetBranch("rap");
    b_pt = fChain->GetBranch("pt");
    b_charge = fChain->GetBranch("charge");
    b_isCutNb = fChain->GetBranch("isCutNb");
    b_idxPart1 = fChain->GetBranch("idxPart1");
    b_idxPart2 = fChain->GetBranch("idxPart2");
    b_prob1 = fChain->GetBranch("prob1");
    b_prob2 = fChain->GetBranch("prob2");
    b_pid1 = fChain->GetBranch("pid1");
    b_pid2 = fChain->GetBranch("pid2");
    b_idxpidcand1 = fChain->GetBranch("idxpidcand1");
    b_sys1 = fChain->GetBranch("sys1");
    b_r1 = fChain->GetBranch("r1");
    b_z1 = fChain->GetBranch("z1");
    b_massexp1 = fChain->GetBranch("massexp1");
    b_betaexp1 = fChain->GetBranch("betaexp1");
    b_momalgidx1 = fChain->GetBranch("momalgidx1");
    b_chrg1 = fChain->GetBranch("chrg1");
    b_mostprobpid1 = fChain->GetBranch("mostprobpid1");
    b_weightmostprobpid1 = fChain->GetBranch("weightmostprobpid1");
    b_theta1 = fChain->GetBranch("theta1");
    b_phi1 = fChain->GetBranch("phi1");
    b_sec1 = fChain->GetBranch("sec1");
    b_idxpidcand2 = fChain->GetBranch("idxpidcand2");
    b_sys2 = fChain->GetBranch("sys2");
    b_r2 = fChain->GetBranch("r2");
    b_z2 = fChain->GetBranch("z2");
    b_massexp2 = fChain->GetBranch("massexp2");
    b_betaexp2 = fChain->GetBranch("betaexp2");
    b_momalgidx2 = fChain->GetBranch("momalgidx2");
    b_chrg2 = fChain->GetBranch("chrg2");
    b_mostprobpid2 = fChain->GetBranch("mostprobpid2");
    b_weightmostprobpid2 = fChain->GetBranch("weightmostprobpid2");
    b_theta2 = fChain->GetBranch("theta2");
    b_phi2 = fChain->GetBranch("phi2");
    b_sec2 = fChain->GetBranch("sec2");
    b_drmt1 = fChain->GetBranch("drmt1");
    b_drmp1 = fChain->GetBranch("drmp1");
    b_drmt2 = fChain->GetBranch("drmt2");
    b_drmp2 = fChain->GetBranch("drmp2");
    b_tof1 = fChain->GetBranch("tof1");
    b_tof2 = fChain->GetBranch("tof2");
    b_rpadnr1 = fChain->GetBranch("rpadnr1");
    b_rcentroid1 = fChain->GetBranch("rcentroid1");
    b_rt1 = fChain->GetBranch("rt1");
    b_rp1 = fChain->GetBranch("rp1");
    b_rpatmat1 = fChain->GetBranch("rpatmat1");
    b_rhoutra1 = fChain->GetBranch("rhoutra1");
    b_rampl1 = fChain->GetBranch("rampl1");
    b_rlocmax41 = fChain->GetBranch("rlocmax41");
    b_rpadnr2 = fChain->GetBranch("rpadnr2");
    b_rcentroid2 = fChain->GetBranch("rcentroid2");
    b_rt2 = fChain->GetBranch("rt2");
    b_rp2 = fChain->GetBranch("rp2");
    b_rpatmat2 = fChain->GetBranch("rpatmat2");
    b_rhoutra2 = fChain->GetBranch("rhoutra2");
    b_rampl2 = fChain->GetBranch("rampl2");
    b_rlocmax42 = fChain->GetBranch("rlocmax42");
    b_mom1 = fChain->GetBranch("mom1");
    b_mom2 = fChain->GetBranch("mom2");
    b_doubleHit = fChain->GetBranch("doubleHit");
    b_qspline1 = fChain->GetBranch("qspline1");
    b_qspline2 = fChain->GetBranch("qspline2");
    b_innerchisquare1 = fChain->GetBranch("innerchisquare1");
    b_innerchisquare2 = fChain->GetBranch("innerchisquare2");
    b_outerchisquare1 = fChain->GetBranch("outerchisquare1");
    b_outerchisquare2 = fChain->GetBranch("outerchisquare2");
    b_distancetovertex1 = fChain->GetBranch("distancetovertex1");
    b_distancetovertex2 = fChain->GetBranch("distancetovertex2");
    b_iscpcandidate1 = fChain->GetBranch("iscpcandidate1");
    b_iscpcandidate2 = fChain->GetBranch("iscpcandidate2");
    b_opangcpcandidate1 = fChain->GetBranch("opangcpcandidate1");
    b_opangcpcandidate2 = fChain->GetBranch("opangcpcandidate2");
    b_isGoodOpang = fChain->GetBranch("isGoodOpang");
    b_isNotDoubleHit = fChain->GetBranch("isNotDoubleHit");
    cout<<"end of notify function"<<endl;
    
    return kTRUE;
}

void pairsE::Show(Int_t entry)
{
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t pairsE::Cut(Int_t entry)
{
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}

void pairsE::Loop()
{
    if (fChain == 0) return;
    
    Int_t nentries = Int_t(fChain->GetEntriesFast());
    
    Int_t nbytes = 0, nb = 0;
    
    
    // L O O P /////////////////////////////////////////////////////////////

    
    for(Int_t jentry=0; jentry<nentries; jentry++) 
    {
	Int_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);   nbytes += nb;
	// if (Cut(ientry) < 0) continue;
	
	Float_t weight = 1.;
	//////////// C U T S /////////////////////////////////////////////
	// fill histograms for different cuts

    if (pid1!=2 &&  pid1!=3) continue;
    if (pid2!=2 &&  pid2!=3) continue;

	fillHistograms(0,weight);
	if(isGoodOpang==1)
	    fillHistograms(1,weight);
	if(isGoodOpang==1 && isCutNb!=2  && isCutNb!=10)
	    fillHistograms(2,weight);
	if(isNotDoubleHit==1)
	    fillHistograms(3,weight);
	if(isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10) 
	    fillHistograms(4,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1)
	    fillHistograms(5,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1 && 
	   iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(6,weight);
	if (isCutNb==0)
	    fillHistograms(7,weight);
	// do not tell me about problems below 120 MeV/c (J. Stroth)
	if (isCutNb==0 && mom1>120 && mom2>120)  
	    fillHistograms(8,weight);
	if (isCutNb==0 && iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(9,weight);
	if (isCutNb==0 && iscpcandidate1==0 && iscpcandidate2==0 && 
	    mom1>120 && mom2>120)
	    fillHistograms(10,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1)
	    fillHistograms(11,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1)
	    fillHistograms(12,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && 
	    isNotDoubleHit==1 && iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(13,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && 
	    isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(14,weight);
    if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
        isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120)
        fillHistograms(15,weight);
    if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
        isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120)
        fillHistograms(16,weight);


    // reconstruction efficiency factor

	Float_t fEffCorr = getEfficiencyFactor();
	weight= weight * fEffCorr;

	fillHistograms(15,weight);
	if(isGoodOpang==1)
	    fillHistograms(16,weight);
	if(isGoodOpang==1 && isCutNb!=2  && isCutNb!=10)
	    fillHistograms(17,weight);
	if(isNotDoubleHit==1)
	    fillHistograms(18,weight);
	if(isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10) 
	    fillHistograms(19,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1)
	    fillHistograms(20,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1 && 
	   iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(21,weight);
	if (isCutNb==0)
	    fillHistograms(22,weight);
	// do not tell me about problems below 120 MeV/c (J. Stroth)
	if (isCutNb==0 && mom1>120 && mom2>120)  
	    fillHistograms(23,weight);
	if (isCutNb==0 && iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(24,weight);
	if (isCutNb==0 && iscpcandidate1==0 && iscpcandidate2==0 && 
	    mom1>120 && mom2>120)
	    fillHistograms(25,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1)
	    fillHistograms(26,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1)
	    fillHistograms(27,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && 
	    isNotDoubleHit==1 && iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(28,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && 
	    isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0)
	    fillHistograms(29,weight);
    if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
        isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120)
        fillHistograms(30,weight);
    if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
        isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120)
        fillHistograms(31,weight);
	
	//////////// C U T S /////////////////////////////////////////////

	
    } // end of L O O P ////////////////////////////////////////////////////
    
    // calculate background and signal
    // Float_t evt = 110582360.;    // SIMULATION LVL1 EQUIVALENT
     Float_t evt = 71875519 * 9.;   // EXP LVL1_M4 EQUIVALENT
     evt *= 0.92;                   // EXP LVL2 EFFICIENCY
      
    for(Int_t icut=0; icut<MAXCUT; icut++)
    {
	// GEOMETRIC CB
	hmass_back0_cut[icut] = getBackg(hmass_cut_pol[icut][0],
					 hmass_cut_pol[icut][2],0);
	hmass_sig0_cut[icut]  = getSignal(hmass_cut_pol[icut][1],
					  hmass_back0_cut[icut]);
	
	hmass_back0_cut_norm[icut] = getNorm(hmass_back0_cut[icut],evt);
	hmass_sig0_cut_norm[icut]  = getNorm(hmass_sig0_cut[icut],evt);
	
	harr->Add(hmass_back0_cut[icut]);
	harr->Add(hmass_sig0_cut[icut]);
	harr->Add(hmass_back0_cut_norm[icut]);
	harr->Add(hmass_sig0_cut_norm[icut]);
	//
	hoangle_back0_cut[icut] = getBackg(hoangle_cut_pol[icut][0],
					 hoangle_cut_pol[icut][2],0);
	hoangle_sig0_cut[icut]  = getSignal(hoangle_cut_pol[icut][1],
					  hoangle_back0_cut[icut]);
	
	hoangle_back0_cut_norm[icut] = getNorm(hoangle_back0_cut[icut],evt);
	hoangle_sig0_cut_norm[icut]  = getNorm(hoangle_sig0_cut[icut],evt);
	
	harr->Add(hoangle_back0_cut[icut]);
	harr->Add(hoangle_sig0_cut[icut]);
	harr->Add(hoangle_back0_cut_norm[icut]);
	harr->Add(hoangle_sig0_cut_norm[icut]);

	//
	hrap_back0_cut[icut] = getBackg(hrap_cut_pol[icut][0],
					 hrap_cut_pol[icut][2],0);
	hrap_sig0_cut[icut]  = getSignal(hrap_cut_pol[icut][1],
					  hrap_back0_cut[icut]);
	
	hrap_back0_cut_norm[icut] = getNorm(hrap_back0_cut[icut],evt);
	hrap_sig0_cut_norm[icut]  = getNorm(hrap_sig0_cut[icut],evt);
	
	harr->Add(hrap_back0_cut[icut]);
	harr->Add(hrap_sig0_cut[icut]);
	harr->Add(hrap_back0_cut_norm[icut]);
	harr->Add(hrap_sig0_cut_norm[icut]);
	//
	hpt_back0_cut[icut] = getBackg(hpt_cut_pol[icut][0],
					 hpt_cut_pol[icut][2],0);
	hpt_sig0_cut[icut]  = getSignal(hpt_cut_pol[icut][1],
					  hpt_back0_cut[icut]);
	
	hpt_back0_cut_norm[icut] = getNorm(hpt_back0_cut[icut],evt);
	hpt_sig0_cut_norm[icut]  = getNorm(hpt_sig0_cut[icut],evt);
	
	harr->Add(hpt_back0_cut[icut]);
	harr->Add(hpt_sig0_cut[icut]);
	harr->Add(hpt_back0_cut_norm[icut]);
	harr->Add(hpt_sig0_cut_norm[icut]);

	///////////////////////////////////////////////////////////////	
	// ARTHMETIC CB
	hmass_back1_cut[icut] = getBackg(hmass_cut_pol[icut][0],
					 hmass_cut_pol[icut][2],1);
	hmass_sig1_cut[icut]  = getSignal(hmass_cut_pol[icut][1],
					  hmass_back1_cut[icut]);
	
	hmass_back1_cut_norm[icut] = getNorm(hmass_back1_cut[icut],evt);
	hmass_sig1_cut_norm[icut]  = getNorm(hmass_sig1_cut[icut],evt);
	
	harr->Add(hmass_back1_cut[icut]);
	harr->Add(hmass_sig1_cut[icut]);
	harr->Add(hmass_back1_cut_norm[icut]);
	harr->Add(hmass_sig1_cut_norm[icut]);
	//
	hoangle_back1_cut[icut] = getBackg(hoangle_cut_pol[icut][0],
					 hoangle_cut_pol[icut][2],1);
	hoangle_sig1_cut[icut]  = getSignal(hoangle_cut_pol[icut][1],
					  hoangle_back1_cut[icut]);
	
	hoangle_back1_cut_norm[icut] = getNorm(hoangle_back1_cut[icut],evt);
	hoangle_sig1_cut_norm[icut]  = getNorm(hoangle_sig1_cut[icut],evt);
	
	harr->Add(hoangle_back1_cut[icut]);
	harr->Add(hoangle_sig1_cut[icut]);
	harr->Add(hoangle_back1_cut_norm[icut]);
	harr->Add(hoangle_sig1_cut_norm[icut]);

	//
	hrap_back1_cut[icut] = getBackg(hrap_cut_pol[icut][0],
					 hrap_cut_pol[icut][2],1);
	hrap_sig1_cut[icut]  = getSignal(hrap_cut_pol[icut][1],
					  hrap_back1_cut[icut]);
	
	hrap_back1_cut_norm[icut] = getNorm(hrap_back1_cut[icut],evt);
	hrap_sig1_cut_norm[icut]  = getNorm(hrap_sig1_cut[icut],evt);
	
	harr->Add(hrap_back1_cut[icut]);
	harr->Add(hrap_sig1_cut[icut]);
	harr->Add(hrap_back1_cut_norm[icut]);
	harr->Add(hrap_sig1_cut_norm[icut]);
	//
	hpt_back1_cut[icut] = getBackg(hpt_cut_pol[icut][0],
					 hpt_cut_pol[icut][2],1);
	hpt_sig1_cut[icut]  = getSignal(hpt_cut_pol[icut][1],
					  hpt_back1_cut[icut]);
	
	hpt_back1_cut_norm[icut] = getNorm(hpt_back1_cut[icut],evt);
	hpt_sig1_cut_norm[icut]  = getNorm(hpt_sig1_cut[icut],evt);
	
	harr->Add(hpt_back1_cut[icut]);
	harr->Add(hpt_sig1_cut[icut]);
	harr->Add(hpt_back1_cut_norm[icut]);
	harr->Add(hpt_sig1_cut_norm[icut]);


	///////////////////////////////////////////////////////////////	
	
	for(Int_t ipol=0; ipol<MAXPOL; ipol++)
	{
	    hmass_cut_pol_norm[icut][ipol] = 
		getNorm(hmass_cut_pol[icut][ipol],evt);
	    hoangle_cut_pol_norm[icut][ipol] = 
		getNorm(hoangle_cut_pol[icut][ipol],evt);
	    hrap_cut_pol_norm[icut][ipol] = 
		getNorm(hrap_cut_pol[icut][ipol],evt);
	    hpt_cut_pol_norm[icut][ipol] = 
		getNorm(hpt_cut_pol[icut][ipol],evt);
	    
	    harr->Add(hmass_cut_pol_norm[icut][ipol]);
	    harr->Add(hoangle_cut_pol_norm[icut][ipol]);
	    harr->Add(hrap_cut_pol_norm[icut][ipol]);
	    harr->Add(hpt_cut_pol_norm[icut][ipol]);
	}
	
    }
    
    
}

void pairsE::fillHistograms(Int_t cut, Float_t weight) 
{
    Int_t pol=-1; 
    
    if(charge == -2) pol = 0;
    else if(charge == 0) pol = 1;
    else if(charge == 2) pol = 2;

    // RECONSTRUCTED 
    hmass_cut_pol[cut][pol]->Fill(invmass,weight);
    hoangle_cut_pol[cut][pol]->Fill(opang,weight);
    hrap_cut_pol[cut][pol]->Fill(rap,weight);
    hpt_cut_pol[cut][pol]->Fill(pt,weight);

}



TH1F * pairsE::getBackg(TH1F *pp, TH1F* ee,Int_t typ)
{
    TString name(pp->GetName());
    name.Remove(name.Length()-4, name.Length() );
    name.Append("back_");
    name+=typ;

    TH1F *bground = (TH1F*) pp->Clone(name.Data());
    bground->Reset();
    bground->Sumw2();
    
    Double_t massPP, massNN;
    Int_t Bins = pp->GetNbinsX();
    
    for(int i = 1; i <= Bins; i++)
    {
	massPP  =  pp->GetBinContent(i);
	massNN  =  ee->GetBinContent(i);
	
	if(typ == 0)
	{
	    bground->SetBinContent(i,2*TMath::Sqrt(massPP*massNN));
	    if(massPP*massNN)
		bground->SetBinError(i,TMath::Sqrt((pp->GetBinError(i))*
						   pp->GetBinError(i)  *
						   massNN/massPP       +
						   ee->GetBinError(i)  *
						   ee->GetBinError(i)  *
						   massPP/massNN));
	}
	if(typ == 1)
	{
            bground->SetBinContent(i,massPP+massNN);
            bground->SetBinError(i,TMath::Sqrt(pp->GetBinError(i)*
					       pp->GetBinError(i)+
					       ee->GetBinError(i)*
					       ee->GetBinError(i)));
	}
}
    
    return bground;
}


TH1F* pairsE::getNorm(TH1F *ep,Float_t evt)
{
    // evt is for event number normalization !!!
    TString name(ep->GetName());
    name.Append("_norm");
    TH1F *invMass = (TH1F*)ep->Clone(name.Data());
    invMass->Sumw2();

    // scale yields
    for (Int_t j=1;j<invMass->GetNbinsX()+1;j++)
    {
	Int_t bin = invMass->GetBin(j);
	invMass->SetBinContent(bin,invMass->GetBinContent(bin)/
			       (evt*invMass->GetBinWidth(bin)));
    }
    
    // scale errors
    for (Int_t j=1;j<invMass->GetNbinsX()+1;j++)
    {
	Int_t bin = invMass->GetBin(j);
	invMass->SetBinError(bin,invMass->GetBinError(bin)/
			     (evt*invMass->GetBinWidth(bin)));
    }

    return invMass;
}

TH1F * pairsE::getSignal(TH1F *ep, TH1F* bg)
{
    TString name(bg->GetName());
    name.Append("_sig");

    TH1F *invMass = (TH1F*) ep->Clone(name.Data());
    invMass->Reset();
    invMass->Sumw2();
    invMass->Add(ep,bg,1.,-1.);
    return invMass;
}

TH1F* pairsE::rebinVar(TH1F* h,Float_t* xbins,Int_t nbins,
		       Bool_t kNorm ,Bool_t kErr)
{
    
    // ASSUMPTION: input histo is __NOT__ normalized to bin width !
    // rebin fixed bin histogram to variable binning
    // default: divide by bin width
    // default: calculate new errors after rebinning
    
    TString name(h->GetName());
    name.Append("_rebinned");

    //create new histogram with new binning
    TH1F * hn = new TH1F(name.Data(),"rebinned_histo",nbins,xbins);
    hn->Sumw2();
    h->Sumw2();

    cout<<"Array input size: "<<h->GetNbinsX()<<endl;
    cout<<"Array output size: "<<hn->GetNbinsX()<<endl;

    //init array to store errors
    Float_t *errn = new Float_t[hn->GetNbinsX()+1];
    for (Int_t k=0;k<hn->GetNbinsX()+1;k++) errn[k]=0.;
    cout<<"Error array created with size: "<<hn->GetNbinsX()+1<<endl;


    // fill rebinned histogram, skip underflow bin
    for (Int_t j=1;j<h->GetNbinsX()+1;j++)
    {
        Int_t i = h->GetBin(j);
        //find center of bin of old histo
        Float_t cc = h->GetBinCenter(i);
        //stop if old bins are beyond new bins
        if (cc > hn->GetBinCenter(hn->GetBin(hn->GetNbinsX()))+
            hn->GetBinWidth(hn->GetBin(hn->GetNbinsX()))/2. ) break;


        //fill old content in new bin number bin
        Int_t bin = hn->Fill(cc,h->GetBinContent(i));

        //check bounds, sum errors quadratic, save in new bin
        if (kErr && bin>=0 && bin<=hn->GetNbinsX())
            errn[bin]+=h->GetBinError(i)*h->GetBinError(i);

    }

    // Normalization to bin width
    if (kNorm)
    {
        for (Int_t j=1;j<hn->GetNbinsX()+1;j++)
        {
            Int_t bin = hn->GetBin(j);
            hn->SetBinContent( bin, hn->GetBinContent(bin)/ 
			       hn->GetBinWidth(bin)
			       );
        }

    }

    // Set errors, rescale if normalized
    if (kErr)
    {
        for (Int_t j=0;j<hn->GetNbinsX()+1;j++)
        {
            Int_t bin = hn->GetBin(j);
            hn->SetBinError(bin,TMath::Sqrt(errn[j]));
            if (kNorm) hn->SetBinError(bin,hn->GetBinError(bin)/
				       hn->GetBinWidth(bin));
        }
    }

    delete [] errn;
    return hn;
}

Float_t pairsE::getRecPhi(Int_t sec, Float_t phi)
{
  switch(sec)
  { 
    case 5:
    return phi;
    break;

    case 0:
    case 1:
    case 2:
    case 3:
    case 4:

    return (phi-(sec+1)*60.);
    break;

    default:
    break;
  }

return phi;
}



Float_t pairsE::getEfficiencyFactor()
{
    Float_t fEff1 = 1.;
    Float_t fEff2 = 1.;
    Float_t d2r = TMath::DegToRad();

	// phi needs to be recalculated to be in 0-60 deg range
	Float_t phi1_tmp = getRecPhi(sec1,phi1);
	Float_t phi2_tmp = getRecPhi(sec2,phi2);

    if (chrg1==1) // positron
    {
	if (invmass<400. && pt<500. )
	{
	    fEff1 = p3DeffPosiLow->
		GetBinContent(p3DeffPosiLow->FindBin(d2r*phi1_tmp,d2r*theta1,mom1));
	}
	else
	{
	    fEff1 = p3DeffPosiHigh->
		GetBinContent(p3DeffPosiHigh->FindBin(d2r*phi1_tmp,d2r*theta1,mom1));
	}
    }
    else if (chrg1==-1) // electron
    {
	if (invmass<400. && pt<500. )
	{
	    fEff1 = p3DeffEleLow->
		GetBinContent(p3DeffEleLow->FindBin(d2r*phi1_tmp,d2r*theta1,mom1));
	}
	else
	{
	    fEff1 = p3DeffEleHigh->
		GetBinContent(p3DeffEleHigh->FindBin(d2r*phi1_tmp,d2r*theta1,mom1));
	}
    }

    // second leg of the pair
    if (chrg2==1) // positron
    {
	if (invmass<400. && pt<500. )
	{
	    fEff2 = p3DeffPosiLow->
		GetBinContent(p3DeffPosiLow->FindBin(d2r*phi2_tmp,d2r*theta2,mom2));
	}
	else
	{
	    fEff2 = p3DeffPosiHigh->
		GetBinContent(p3DeffPosiHigh->FindBin(d2r*phi2_tmp,d2r*theta2,mom2));
	}
    }
    else if (chrg2==-1) // electron
    {
	if (invmass<400. && pt<500. )
	{
	    fEff2 = p3DeffEleLow->GetBinContent(p3DeffEleLow->FindBin(d2r*phi2_tmp,d2r*theta2,mom2));
	}
	else
	{
	    fEff2 = p3DeffEleHigh->GetBinContent(p3DeffEleHigh->FindBin(d2r*phi2_tmp,d2r*theta2,mom2));
	}
    }

    if (fEff1*fEff2) return 1./(fEff1*fEff2);
    else return 1.;
}









