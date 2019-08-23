#define pairs_cxx
#include "pairs.h"
#include <TH2.h>
#include <TError.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <TLorentzVector.h>
#include <iostream>

using namespace std;
pairs::pairs(TTree *tree, TString opt)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
	if (tree == 0) {
		Error("ctor","pointer to tree is zero");
	}
	opt.ToUpper();
#ifdef SIMULATION
	evtGen = bUnknown;
	if (opt.Contains("PLUTO")) {
		cout << "event generator pluto" << endl;
		evtGen = bPluto;
	} else if (opt.Contains("ELEMENTARY")) {
		cout << "event generator pluto with elementary reactions" << endl;
		evtGen = bElementary;
	} else if (opt.Contains("URQMD")) {
		cout << "event generator UrQMD" << endl;
		evtGen = bUrQMD;
	} else {
		cout << "unknown event generator" << endl;
	}
#endif // SIMULATION
	Init(tree);
	// set a default value for the event number normalization
	// can be overwritten by calling the function SetEvents(Double_t);
	evt = 1.;
}

pairs::~pairs()
{
	if (!fChain) return;
	delete fChain->GetCurrentFile();
}

Int_t pairs::GetEntry(Int_t entry)
{
// Read contents of entry.
	if (!fChain) return 0;
	return fChain->GetEntry(entry);
}

Int_t pairs::LoadTree(Int_t entry)
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

void pairs::Init(TTree *tree)
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
	fChain->SetBranchAddress("closestlepisfitted1",&closestlepisfitted1);
	fChain->SetBranchAddress("closestlepisfitted2",&closestlepisfitted2);
	fChain->SetBranchAddress("closesthadisfitted1",&closesthadisfitted1);
	fChain->SetBranchAddress("closesthadisfitted2",&closesthadisfitted2);
	fChain->SetBranchAddress("opangclosestlep1",&opangclosestlep1);
	fChain->SetBranchAddress("opangclosestlep2",&opangclosestlep2);
	fChain->SetBranchAddress("opangclosesthad1",&opangclosesthad1);
	fChain->SetBranchAddress("opangclosesthad2",&opangclosesthad2);
	fChain->SetBranchAddress("IOm_chi2_1",&IOm_chi2_1);
	fChain->SetBranchAddress("IOm_chi2_2",&IOm_chi2_2);
	fChain->SetBranchAddress("pairvertx",&pairvertx);
	fChain->SetBranchAddress("pairverty",&pairverty);
	fChain->SetBranchAddress("pairvertz",&pairvertz);
	fChain->SetBranchAddress("pairdistx",&pairdistx);
	fChain->SetBranchAddress("pairdisty",&pairdisty);
	fChain->SetBranchAddress("pairdistz",&pairdistz);
	fChain->SetBranchAddress("pairdist",&pairdist);
	fChain->SetBranchAddress("evtNr",&evtNr);
	fChain->SetBranchAddress("DSflag",&DSflag);
	fChain->SetBranchAddress("trigDec",&trigDec);
	fChain->SetBranchAddress("evtVertX",&evtVertX);
	fChain->SetBranchAddress("evtVertY",&evtVertY);
	fChain->SetBranchAddress("evtVertZ",&evtVertZ);
	fChain->SetBranchAddress("run",&run);
#ifdef SIMULATION
	fChain->SetBranchAddress("Gpid1",&Gpid1);
	fChain->SetBranchAddress("GparentId1",&GparentId1);
	fChain->SetBranchAddress("GprocessId1",&GprocessId1);
	fChain->SetBranchAddress("Gmom1",&Gmom1);
	fChain->SetBranchAddress("Gpid2",&Gpid2);
	fChain->SetBranchAddress("GparentId2",&GparentId2);
	fChain->SetBranchAddress("GprocessId2",&GprocessId2);
	fChain->SetBranchAddress("Gmom2",&Gmom2);
	fChain->SetBranchAddress("Ginvmass",&Ginvmass);
	fChain->SetBranchAddress("Gopang",&Gopang);
	fChain->SetBranchAddress("Grap",&Grap);
	fChain->SetBranchAddress("Gpt",&Gpt);
	fChain->SetBranchAddress("Gcharge",&Gcharge);
	fChain->SetBranchAddress("GparentTrackNb1",&GparentTrackNb1);
	fChain->SetBranchAddress("GparentTrackNb2",&GparentTrackNb2);
	fChain->SetBranchAddress("GdecayId",&GdecayId);
	fChain->SetBranchAddress("GCommonDet1",&GCommonDet1);
	fChain->SetBranchAddress("GCommonDet2",&GCommonDet2);
	fChain->SetBranchAddress("Gvx1",&Gvx1);
	fChain->SetBranchAddress("Gvy1",&Gvy1);
	fChain->SetBranchAddress("Gvz1",&Gvz1);
	fChain->SetBranchAddress("Gvx2",&Gvx2);
	fChain->SetBranchAddress("Gvy2",&Gvy2);
	fChain->SetBranchAddress("Gvz2",&Gvz2);
	fChain->SetBranchAddress("Gmed1",&Gmed1);
	fChain->SetBranchAddress("Gmed2",&Gmed2);
	fChain->SetBranchAddress("Ggeninfo1",&Ggeninfo1);
	fChain->SetBranchAddress("Ggenweight1",&Ggenweight1);
	fChain->SetBranchAddress("Ggeninfo2",&Ggeninfo2);
	fChain->SetBranchAddress("Ggenweight2",&Ggenweight2);
	fChain->SetBranchAddress("GgrandparentTrackNb1",&GgrandparentTrackNb1);
	fChain->SetBranchAddress("GgrandparentTrackNb2",&GgrandparentTrackNb2);
	fChain->SetBranchAddress("GgrandparentId1",&GgrandparentId1);
	fChain->SetBranchAddress("GgrandparentId2",&GgrandparentId2);

	if( fChain->FindBranch("Ggeninfo1_1") != NULL) // P L U T O
	{
	fChain->SetBranchAddress("Ggeninfo1_1",&Ggeninfo1_1);
	fChain->SetBranchAddress("Ggeninfo1_2",&Ggeninfo1_2);
	fChain->SetBranchAddress("Ggeninfo2_1",&Ggeninfo2_1);
	fChain->SetBranchAddress("Ggeninfo2_2",&Ggeninfo2_2);
	}
#endif // SIMULATION

	// C U T S
	fChain->SetBranchAddress("isGoodOpang",&isGoodOpang);
	fChain->SetBranchAddress("isNotDoubleHit",&isNotDoubleHit);


	//histogram booking
	harr = new TObjArray();

	// CHANGE BINNING HERE
	// 2005-08-01 Binning used for QM05 talk

	// mass binning

#ifdef SIMULATION

	// Pluto nov02 binning
#if 1
	Float_t xbins[] = {0.,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,
			   125.,150.,200.,250.,300.,350.,400.,450.,500.,
			   550.,600.,650.,700.,760.,820.,900.,1000.};
#endif	
	// nov02 gen4 urqmd binning
#if 0
	Float_t xbins[] = {0.,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,
			   120.,140.,160.,180.,210.,250.,300.,350.,420.,
			   500.,580.,680.,780.,880.,1000.};
#endif
	
#else
	
	// nov02 exp final binning
	Float_t xbins[] = {0.,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,
			   125.,150.,200.,250.,300.,350.,400.,450.,500.,
			   550.,600.,650.,700.,760.,820.,900.,1000.};
#endif

#if 0
	Float_t xbins[] = {0.,30.,90.,120.,160.,200.,300.,400.,600.,800.,1000.};
#endif

	Int_t nbins = sizeof(xbins)/sizeof(Float_t);

	// pt binning
#ifdef SIMULATION

	// Pluto nov02 binning
#if 1
	Float_t xbins1[] = {0.,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,
			   125.,150.,200.,250.,300.,350.,400.,450.,500.,
			   550.,600.,650.,700.,760.,820.,900.,1000.};
#endif	

	// nov02 gen4 urqmd binning
#if 0
	Float_t xbins1[] = {0.,10.,20.,30.,40.,50.,60.,70.,80.,90.,100.,
		120.,140.,160.,180.,210.,250.,300.,350.,420.,
		500.,580.,680.,780.,880.,1000.};
#endif

#else
	// nov02 exp final binning
	
	Float_t xbins1[] = {0.,40.,80.,120.,180.0,260.,360.,460.,560.,
			    660., 760., 860., 960.,1100.};
#endif
	Int_t nbins1 = sizeof(xbins1)/sizeof(Float_t);

	///////////////////////////////////////////////////////////////////////
	char name[256];

	for(Int_t icut=0; icut<MAXCUT; icut++)
	{
#ifdef SIMULATION
		sprintf(name,"hmass_cut%d_truecb",icut);
		hmass_cut_truecb[icut] = new TH1F(name,name,nbins-1,xbins);
		hmass_cut_truecb[icut]->Sumw2();

		sprintf(name,"hoangle_cut%d_truecb",icut);
		hoangle_cut_truecb[icut] = new TH1F(name,name,180,0,180);
		hoangle_cut_truecb[icut]->Sumw2();

		sprintf(name,"hrap_cut%d_truecb",icut);
		hrap_cut_truecb[icut] = new TH1F(name,name,60,0,3);
		hrap_cut_truecb[icut]->Sumw2();

		sprintf(name,"hpt_cut%d_truecb",icut);
		hpt_cut_truecb[icut] = new TH1F(name,name,nbins1-1,xbins1);
		hpt_cut_truecb[icut]->Sumw2();

		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			sprintf(name,"hpolar_cut%d_minv%d_truecb",icut,iminv);
			hpolar_cut_truecb[icut][iminv] = new TH1F(name,name,45,0,180);
			hpolar_cut_truecb[icut][iminv]->Sumw2();
		}
#endif // SIMULATION

		for(Int_t ipol=0; ipol<MAXPOL; ipol++)
		{
		sprintf(name,"hmass_cut%d_pol%d",icut,ipol);
		hmass_cut_pol[icut][ipol] = new TH1F(name,name,nbins-1,xbins);
		hmass_cut_pol[icut][ipol]->Sumw2();

		sprintf(name,"hoangle_cut%d_pol%d",icut,ipol);
		hoangle_cut_pol[icut][ipol] = new TH1F(name,name,180,0,180);
		hoangle_cut_pol[icut][ipol]->Sumw2();

		sprintf(name,"hrap_cut%d_pol%d",icut,ipol);
		hrap_cut_pol[icut][ipol] = new TH1F(name,name,60,0.,3.);
		hrap_cut_pol[icut][ipol]->Sumw2();

		sprintf(name,"hpt_cut%d_pol%d",icut,ipol);
		hpt_cut_pol[icut][ipol] = new TH1F(name,name,nbins1-1,xbins1);
		hpt_cut_pol[icut][ipol]->Sumw2();

		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			sprintf(name,"hpolar_cut%d_minv%d_pol%d",icut,iminv,ipol);
			hpolar_cut_pol[icut][iminv][ipol] = new TH1F(name,name,45,0,180);
			hpolar_cut_pol[icut][iminv][ipol]->Sumw2();
		}
		}

#ifdef SIMULATION
		for(Int_t ipair=0; ipair<MAXPAIR; ipair++)
		{
		sprintf(name,"hmass_cut%d_true%d",icut,ipair);
		hmass_cut_true[icut][ipair] = new TH1F(name,name,nbins-1,xbins);
		hmass_cut_true[icut][ipair]->Sumw2();

		sprintf(name,"hoangle_cut%d_true%d",icut,ipair);
		hoangle_cut_true[icut][ipair] = new TH1F(name,name,180,0,180);
		hoangle_cut_true[icut][ipair]->Sumw2();

		sprintf(name,"hrap_cut%d_true%d",icut,ipair);
		hrap_cut_true[icut][ipair] = new TH1F(name,name,60,0,3);
		hrap_cut_true[icut][ipair]->Sumw2();

		sprintf(name,"hpt_cut%d_true%d",icut,ipair);
		hpt_cut_true[icut][ipair] = new TH1F(name,name,nbins1-1,xbins1);
		hpt_cut_true[icut][ipair]->Sumw2();

		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			sprintf(name,"hpolar_cut%d_minv%d_true%d",icut,iminv,ipair);
			hpolar_cut_true[icut][iminv][ipair] = new TH1F(name,name,45,0,180);
			hpolar_cut_true[icut][iminv][ipair]->Sumw2();
		}
		}
#endif // SIMULATION
	}
	// Init efficiency matrices

	pEleEffFile = new TFile("./matricesEffSingle.PairCode.Ele.root","READ");
	if (pEleEffFile) {
		pEleEffFile->cd();
		p3DEffEle = (TH3F*) pEleEffFile->Get("effi3DEleAllCut");
	} else {
		Error("Init","pointer to eff matrix file is NULL");
		p3DEffEle = NULL;
	}

	pPosiEffFile = new TFile("./matricesEffSingle.PairCode.Posi.root","READ");
	if (pPosiEffFile) {
		pPosiEffFile->cd();
		p3DEffPosi = (TH3F*) pPosiEffFile->Get("effi3DPosiAllCut");
	} else {
		Error("Init","pointer to eff matrix file is NULL");
		p3DEffPosi = NULL;
	}

	// calculate background and signal
	Int_t maxcut = MAXCUT;
	if ((p3DEffPosi == NULL) || (p3DEffEle == NULL)) {
		// at least one of the matrices missing, to not save
		// efficiency-corrected histograms
		maxcut = EFF_OFFSET;
	}

	for(Int_t icut=0; icut<maxcut; icut++)
	{
#ifdef SIMULATION
		harr->Add(hmass_cut_truecb[icut]);
		harr->Add(hoangle_cut_truecb[icut]);
		harr->Add(hrap_cut_truecb[icut]);
		harr->Add(hpt_cut_truecb[icut]);

		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			harr->Add(hpolar_cut_truecb[icut][iminv]);
		}
#endif // SIMULATION

		for(Int_t ipol=0; ipol<MAXPOL; ipol++)
		{
		harr->Add(hmass_cut_pol[icut][ipol]);
		harr->Add(hoangle_cut_pol[icut][ipol]);
		harr->Add(hrap_cut_pol[icut][ipol]);
		harr->Add(hpt_cut_pol[icut][ipol]);

		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			harr->Add(hpolar_cut_pol[icut][iminv][ipol]);
		}
		}

#ifdef SIMULATION
		for(Int_t ipair=0; ipair<MAXPAIR; ipair++)
		{
		harr->Add(hmass_cut_true[icut][ipair]);
		harr->Add(hoangle_cut_true[icut][ipair]);
		harr->Add(hrap_cut_true[icut][ipair]);
		harr->Add(hpt_cut_true[icut][ipair]);

		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			harr->Add(hpolar_cut_true[icut][iminv][ipair]);
		}
		}
#endif // SIMULATION
	}

	Notify();
}

Bool_t pairs::Notify()
{
	// The Notify() function is called when a new file is opened. This
	// can be either for a new TTree in a TChain or when when a new TTree
	// is started when using PROOF. Typically here the branch pointers
	// will be retrieved. It is normaly not necessary to make changes
	// to the generated code, but the routine can be extended by the
	// user if needed.
    //	cout<<"start of notify function"<<endl;

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
	b_closestlepisfitted1 = fChain->GetBranch("closestlepisfitted1");
	b_closestlepisfitted2 = fChain->GetBranch("closestlepisfitted2");
	b_closesthadisfitted1 = fChain->GetBranch("closesthadisfitted1");
	b_closesthadisfitted2 = fChain->GetBranch("closesthadisfitted2");
	b_opangclosestlep1 = fChain->GetBranch("opangclosestlep1");
	b_opangclosestlep2 = fChain->GetBranch("opangclosestlep2");
	b_opangclosesthad1 = fChain->GetBranch("opangclosesthad1");
	b_opangclosesthad2 = fChain->GetBranch("opangclosesthad2");
	b_IOm_chi2_1 = fChain->GetBranch("IOm_chi2_1");
	b_IOm_chi2_2 = fChain->GetBranch("IOm_chi2_2");
	b_pairvertx = fChain->GetBranch("pairvertx");
	b_pairverty = fChain->GetBranch("pairverty");
	b_pairvertz = fChain->GetBranch("pairvertz");
	b_pairdistx = fChain->GetBranch("pairdistx");
	b_pairdisty = fChain->GetBranch("pairdisty");
	b_pairdistz = fChain->GetBranch("pairdistz");
	b_pairdist = fChain->GetBranch("pairdist");
	b_evtNr = fChain->GetBranch("evtNr");
	b_DSflag = fChain->GetBranch("DSflag");
	b_trigDec = fChain->GetBranch("trigDec");
	b_evtVertX = fChain->GetBranch("evtVertX");
	b_evtVertY = fChain->GetBranch("evtVertY");
	b_evtVertZ = fChain->GetBranch("evtVertZ");
	b_run = fChain->GetBranch("run");
#ifdef SIMULATION
	b_Gpid1 = fChain->GetBranch("Gpid1");
	b_GparentId1 = fChain->GetBranch("GparentId1");
	b_GprocessId1 = fChain->GetBranch("GprocessId1");
	b_Gmom1 = fChain->GetBranch("Gmom1");
	b_Gpid2 = fChain->GetBranch("Gpid2");
	b_GparentId2 = fChain->GetBranch("GparentId2");
	b_GprocessId2 = fChain->GetBranch("GprocessId2");
	b_Gmom2 = fChain->GetBranch("Gmom2");
	b_Ginvmass = fChain->GetBranch("Ginvmass");
	b_Gopang = fChain->GetBranch("Gopang");
	b_Grap = fChain->GetBranch("Grap");
	b_Gpt = fChain->GetBranch("Gpt");
	b_Gcharge = fChain->GetBranch("Gcharge");
	b_GparentTrackNb1 = fChain->GetBranch("GparentTrackNb1");
	b_GparentTrackNb2 = fChain->GetBranch("GparentTrackNb2");
	b_GdecayId = fChain->GetBranch("GdecayId");
	b_GCommonDet1 = fChain->GetBranch("GCommonDet1");
	b_GCommonDet2 = fChain->GetBranch("GCommonDet2");
	b_Gvx1 = fChain->GetBranch("Gvx1");
	b_Gvy1 = fChain->GetBranch("Gvy1");
	b_Gvz1 = fChain->GetBranch("Gvz1");
	b_Gvx2 = fChain->GetBranch("Gvx2");
	b_Gvy2 = fChain->GetBranch("Gvy2");
	b_Gvz2 = fChain->GetBranch("Gvz2");
	b_Gmed1 = fChain->GetBranch("Gmed1");
	b_Gmed2 = fChain->GetBranch("Gmed2");
	b_Ggeninfo1 = fChain->GetBranch("Ggeninfo1");
	b_Ggenweight1 = fChain->GetBranch("Ggenweight1");
	b_Ggeninfo2 = fChain->GetBranch("Ggeninfo2");
	b_Ggenweight2 = fChain->GetBranch("Ggenweight2");
	b_GgrandparentTrackNb1 = fChain->GetBranch("GgrandparentTrackNb1");
	b_GgrandparentTrackNb2 = fChain->GetBranch("GgrandparentTrackNb2");
	b_GgrandparentId1 = fChain->GetBranch("GgrandparentId1");
	b_GgrandparentId2 = fChain->GetBranch("GgrandparentId2");

	if( fChain->FindBranch("Ggeninfo1_1") != NULL) // P L U T O
	{
	b_Ggeninfo1_1 = fChain->GetBranch("Ggeninfo1_1");
	b_Ggeninfo1_2 = fChain->GetBranch("Ggeninfo1_2");
	b_Ggeninfo2_1 = fChain->GetBranch("Ggeninfo2_1");
	b_Ggeninfo2_2 = fChain->GetBranch("Ggeninfo2_2");
	}
#endif // SIMULATION
	b_isGoodOpang = fChain->GetBranch("isGoodOpang");
	b_isNotDoubleHit = fChain->GetBranch("isNotDoubleHit");

	//	cout<<"end of notify function"<<endl;

	return kTRUE;
}

void pairs::Show(Int_t entry)
{
	// Print contents of entry.
	// If entry is not specified, print current entry
	if (!fChain) return;
	fChain->Show(entry);
}

Int_t pairs::Cut(Int_t entry)
{
	// This function may be called from Loop.
	// returns 1 if entry is accepted.
	// returns -1 otherwise.
	return 1;
}

void pairs::Loop(Int_t nevt)
{
	if (fChain == 0) return;

	Int_t nentries = Int_t(fChain->GetEntriesFast());

	if (nevt>0 && nevt<nentries) nentries = nevt;

	Int_t nbytes = 0, nb = 0;


	// L O O P /////////////////////////////////////////////////////////////

	TLorentzVector *vLep1 = new TLorentzVector(0.,0.,0.,0.511);
	TLorentzVector *vLep2 = new TLorentzVector(0.,0.,0.,0.511);

	const Double_t d2r = TMath::DegToRad();
	const Double_t r2d = TMath::RadToDeg();

	for(Int_t jentry=0; jentry<nentries; jentry++)
	{
	Int_t ientry = LoadTree(jentry);
	if (ientry < 0) break;
	nb = fChain->GetEntry(jentry);	nbytes += nb;
	if (invmass<=150.) {// low mass cut
		minvbin=0;
	} else if (invmass>150. && invmass<=600.) {// medium mass cut
		minvbin=1;
	} else if (invmass>600.) {// high mass cut
		minvbin=2;
	}

	// if (Cut(ientry) < 0) continue;

	Float_t weight = 1.;
	// compute polarization angle in dilepton frame

	Double_t pt1 = mom1*TMath::Sin(d2r*theta1);
	Double_t eta1 = -TMath::Log(TMath::Tan(0.5*d2r*theta1));
	vLep1->SetPtEtaPhiM(pt1,eta1,phi1,0.511);
	Double_t pt2 = mom2*TMath::Sin(d2r*theta2);
	Double_t eta2 = -TMath::Log(TMath::Tan(0.5*d2r*theta2));
	vLep2->SetPtEtaPhiM(pt2,eta2,phi2,0.511);
	Double_t px = vLep1->Px() + vLep2->Px();
	Double_t py = vLep1->Py() + vLep2->Py();
	Double_t pz = vLep1->Pz() + vLep2->Pz();
	Double_t Etot = vLep1->E() + vLep2->E();
	Double_t betaDilx = px/Etot;
	Double_t betaDily = py/Etot;
	Double_t betaDilz = pz/Etot;
	Double_t betaDil = (vLep1->P() + vLep2->P())/Etot;

	vLep1->Boost(-betaDilx,-betaDily,-betaDilz); // go to dilepton cm
	vLep2->Boost(-betaDilx,-betaDily,-betaDilz);

	Double_t cosp = (vLep1->Px()*betaDilx +
					vLep1->Py()*betaDily +
					vLep1->Pz()*betaDilz)/(vLep1->P()*betaDil);

	polar = r2d*TMath::ACos(cosp); // emission angle of 1st lepton in degrees
	polarweight = 1.;
#if 0
	polarweight = 1./sqrt(1.00001-cosp*cosp); // go from dNdTheta to dNdOmega
#endif
#ifdef SIMULATION
	// calculate weight for downscaling of enhanced BR
	weight = calcWeight();
#endif // SIMULATION

	//////////// C U T S /////////////////////////////////////////////
	// fill histograms for different cuts

	if (pid1!=2 && pid1!=3) continue;
	if (pid2!=2 && pid2!=3) continue;

	fillHistograms(0,weight);
	if(isGoodOpang==1)
		fillHistograms(1,weight);
	if(isGoodOpang==1 && isCutNb!=2 && isCutNb!=10)
		fillHistograms(2,weight);
	if(isNotDoubleHit==1)
		fillHistograms(3,weight);
	if(isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10)
		fillHistograms(4,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1)
		fillHistograms(5,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1 &&
		closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(6,weight);
	if (isCutNb==0)
		fillHistograms(7,weight);
	// do not tell me about problems below 120 MeV/c (J. Stroth)
	if (isCutNb==0 && mom1>120 && mom2>120)
		fillHistograms(8,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(9,weight);

	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass < 150.)
	    fillHistograms(10,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass > 150. && invmass < 550.)
	    fillHistograms(11,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass > 550.)
	    fillHistograms(12,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass > 150.)
	    fillHistograms(13,weight);

#if 0
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		mom1>120 && mom2>120)
		fillHistograms(10,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1)
		fillHistograms(11,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1)
		fillHistograms(12,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 &&
		isNotDoubleHit==1 && closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(13,weight);
#endif

	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
		isGoodOpang==1 && closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(14,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
		isGoodOpang==1 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		mom1>120 && mom2>120)
		fillHistograms(15,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
		isGoodOpang==1 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		mom1>120 && mom2>120)
		fillHistograms(16,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		pt>100.)
		fillHistograms(17,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		pt>100. && invmass>140.)
		fillHistograms(18,weight);

	// reconstruction efficiency correction
	Float_t fEffCorr = getEfficiencyFactor();
	weight *= fEffCorr;

	fillHistograms(EFF_OFFSET,weight);
	if(isGoodOpang==1)
		fillHistograms(EFF_OFFSET+1,weight);
	if(isGoodOpang==1 && isCutNb!=2 && isCutNb!=10)
		fillHistograms(EFF_OFFSET+2,weight);
	if(isNotDoubleHit==1)
		fillHistograms(EFF_OFFSET+3,weight);
	if(isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10)
		fillHistograms(EFF_OFFSET+4,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1)
		fillHistograms(EFF_OFFSET+5,weight);
	if(isGoodOpang==1 && isNotDoubleHit==1 &&
		closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(EFF_OFFSET+6,weight);
	if (isCutNb==0)
		fillHistograms(EFF_OFFSET+7,weight);
	// do not tell me about problems below 120 MeV/c (J. Stroth)
	if (isCutNb==0 && mom1>120 && mom2>120)
		fillHistograms(EFF_OFFSET+8,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(EFF_OFFSET+9,weight);

	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass < 150.)
	    fillHistograms(EFF_OFFSET+10,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass > 150. && invmass < 550.)
	    fillHistograms(EFF_OFFSET+11,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass > 550.)
	    fillHistograms(EFF_OFFSET+12,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
	    invmass > 150.)
	    fillHistograms(EFF_OFFSET+13,weight);
#if 0
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		mom1>120 && mom2>120)
		fillHistograms(EFF_OFFSET+10,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1)
		fillHistograms(EFF_OFFSET+11,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1)
		fillHistograms(EFF_OFFSET+12,weight);
	if (isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 &&
		isNotDoubleHit==1 && closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(EFF_OFFSET+13,weight);
#endif
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
		isGoodOpang==1 && closestlepisfitted1==1 && closestlepisfitted2==1)
		fillHistograms(EFF_OFFSET+14,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
		isGoodOpang==1 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		mom1>120 && mom2>120)
		fillHistograms(EFF_OFFSET+15,weight);
	if (isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 &&
		isGoodOpang==1 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		mom1>120 && mom2>120)
		fillHistograms(EFF_OFFSET+16,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		pt>100.)
		fillHistograms(EFF_OFFSET+17,weight);
	if (isCutNb==0 && closestlepisfitted1==1 && closestlepisfitted2==1 &&
		pt>100. && invmass>140.)
		fillHistograms(EFF_OFFSET+18,weight);
	//////////// C U T S /////////////////////////////////////////////

	} // end of L O O P ////////////////////////////////////////////////////

	// calculate background and signal
	Int_t maxcut = MAXCUT;
	if ((p3DEffPosi == NULL) || (p3DEffEle == NULL)) {
		// at least one of the matrices missing, to not save
		// efficiency-corrected histograms
		maxcut = EFF_OFFSET;
	}

	for(Int_t icut=0; icut<maxcut; icut++)
	{
	// GEOMETRIC CB
	hmass_back0_cut[icut] = getBackg(hmass_cut_pol[icut][0],
					hmass_cut_pol[icut][2],0);
	hmass_sig0_cut[icut] = getSignal(hmass_cut_pol[icut][1],
					hmass_back0_cut[icut]);

	hmass_back0_cut_norm[icut] = getNorm(hmass_back0_cut[icut],evt);
	hmass_sig0_cut_norm[icut] = getNorm(hmass_sig0_cut[icut],evt);

	harr->Add(hmass_back0_cut[icut]);
	harr->Add(hmass_sig0_cut[icut]);
	harr->Add(hmass_back0_cut_norm[icut]);
	harr->Add(hmass_sig0_cut_norm[icut]);

	//
	hoangle_back0_cut[icut] = getBackg(hoangle_cut_pol[icut][0],
					hoangle_cut_pol[icut][2],0);
	hoangle_sig0_cut[icut] = getSignal(hoangle_cut_pol[icut][1],
					hoangle_back0_cut[icut]);

	hoangle_back0_cut_norm[icut] = getNorm(hoangle_back0_cut[icut],evt);
	hoangle_sig0_cut_norm[icut] = getNorm(hoangle_sig0_cut[icut],evt);

	harr->Add(hoangle_back0_cut[icut]);
	harr->Add(hoangle_sig0_cut[icut]);
	harr->Add(hoangle_back0_cut_norm[icut]);
	harr->Add(hoangle_sig0_cut_norm[icut]);

	//
	hrap_back0_cut[icut] = getBackg(hrap_cut_pol[icut][0],
					hrap_cut_pol[icut][2],0);
	hrap_sig0_cut[icut] = getSignal(hrap_cut_pol[icut][1],
					hrap_back0_cut[icut]);

	hrap_back0_cut_norm[icut] = getNorm(hrap_back0_cut[icut],evt);
	hrap_sig0_cut_norm[icut] = getNorm(hrap_sig0_cut[icut],evt);

	harr->Add(hrap_back0_cut[icut]);
	harr->Add(hrap_sig0_cut[icut]);
	harr->Add(hrap_back0_cut_norm[icut]);
	harr->Add(hrap_sig0_cut_norm[icut]);

	//
	hpt_back0_cut[icut] = getBackg(hpt_cut_pol[icut][0],
					hpt_cut_pol[icut][2],0);
	hpt_sig0_cut[icut] = getSignal(hpt_cut_pol[icut][1],
					hpt_back0_cut[icut]);

	hpt_back0_cut_norm[icut] = getNorm(hpt_back0_cut[icut],evt);
	hpt_sig0_cut_norm[icut] = getNorm(hpt_sig0_cut[icut],evt);

	harr->Add(hpt_back0_cut[icut]);
	harr->Add(hpt_sig0_cut[icut]);
	harr->Add(hpt_back0_cut_norm[icut]);
	harr->Add(hpt_sig0_cut_norm[icut]);

	//
	for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
	{
		hpolar_back0_cut[icut][iminv] =
			getBackg(hpolar_cut_pol[icut][iminv][0],
			hpolar_cut_pol[icut][iminv][2],0);
		hpolar_sig0_cut[icut][iminv] =
			getSignal(hpolar_cut_pol[icut][iminv][1],
			hpolar_back0_cut[icut][iminv]);

		hpolar_back0_cut_norm[icut][iminv] =
			getNorm(hpolar_back0_cut[icut][iminv],evt);
		hpolar_sig0_cut_norm[icut][iminv] =
			getNorm(hpolar_sig0_cut[icut][iminv],evt);

		harr->Add(hpolar_back0_cut[icut][iminv]);
		harr->Add(hpolar_sig0_cut[icut][iminv]);
		harr->Add(hpolar_back0_cut_norm[icut][iminv]);
		harr->Add(hpolar_sig0_cut_norm[icut][iminv]);
	}

	///////////////////////////////////////////////////////////////
	// ARTHMETIC CB
	hmass_back1_cut[icut] = getBackg(hmass_cut_pol[icut][0],
					hmass_cut_pol[icut][2],1);
	hmass_sig1_cut[icut] = getSignal(hmass_cut_pol[icut][1],
					hmass_back1_cut[icut]);

	hmass_back1_cut_norm[icut] = getNorm(hmass_back1_cut[icut],evt);
	hmass_sig1_cut_norm[icut] = getNorm(hmass_sig1_cut[icut],evt);

	harr->Add(hmass_back1_cut[icut]);
	harr->Add(hmass_sig1_cut[icut]);
	harr->Add(hmass_back1_cut_norm[icut]);
	harr->Add(hmass_sig1_cut_norm[icut]);
	//
	hoangle_back1_cut[icut] = getBackg(hoangle_cut_pol[icut][0],
					hoangle_cut_pol[icut][2],1);
	hoangle_sig1_cut[icut] = getSignal(hoangle_cut_pol[icut][1],
					hoangle_back1_cut[icut]);

	hoangle_back1_cut_norm[icut] = getNorm(hoangle_back1_cut[icut],evt);
	hoangle_sig1_cut_norm[icut] = getNorm(hoangle_sig1_cut[icut],evt);

	harr->Add(hoangle_back1_cut[icut]);
	harr->Add(hoangle_sig1_cut[icut]);
	harr->Add(hoangle_back1_cut_norm[icut]);
	harr->Add(hoangle_sig1_cut_norm[icut]);

	//
	hrap_back1_cut[icut] = getBackg(hrap_cut_pol[icut][0],
					hrap_cut_pol[icut][2],1);
	hrap_sig1_cut[icut] = getSignal(hrap_cut_pol[icut][1],
					hrap_back1_cut[icut]);

	hrap_back1_cut_norm[icut] = getNorm(hrap_back1_cut[icut],evt);
	hrap_sig1_cut_norm[icut] = getNorm(hrap_sig1_cut[icut],evt);

	harr->Add(hrap_back1_cut[icut]);
	harr->Add(hrap_sig1_cut[icut]);
	harr->Add(hrap_back1_cut_norm[icut]);
	harr->Add(hrap_sig1_cut_norm[icut]);
	//
	hpt_back1_cut[icut] = getBackg(hpt_cut_pol[icut][0],
					hpt_cut_pol[icut][2],1);
	hpt_sig1_cut[icut] = getSignal(hpt_cut_pol[icut][1],
					hpt_back1_cut[icut]);

	hpt_back1_cut_norm[icut] = getNorm(hpt_back1_cut[icut],evt);
	hpt_sig1_cut_norm[icut] = getNorm(hpt_sig1_cut[icut],evt);

	harr->Add(hpt_back1_cut[icut]);
	harr->Add(hpt_sig1_cut[icut]);
	harr->Add(hpt_back1_cut_norm[icut]);
	harr->Add(hpt_sig1_cut_norm[icut]);

	for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
	{
		hpolar_back1_cut[icut][iminv] =
			getBackg(hpolar_cut_pol[icut][iminv][0],
			hpolar_cut_pol[icut][iminv][2],1);
		hpolar_sig1_cut[icut][iminv] =
			getSignal(hpolar_cut_pol[icut][iminv][1],
			hpolar_back1_cut[icut][iminv]);

		hpolar_back1_cut_norm[icut][iminv] =
			getNorm(hpolar_back1_cut[icut][iminv],evt);
		hpolar_sig1_cut_norm[icut][iminv] =
			getNorm(hpolar_sig1_cut[icut][iminv],evt);

		harr->Add(hpolar_back1_cut[icut][iminv]);
		harr->Add(hpolar_sig1_cut[icut][iminv]);
		harr->Add(hpolar_back1_cut_norm[icut][iminv]);
		harr->Add(hpolar_sig1_cut_norm[icut][iminv]);
	}

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
		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			hpolar_cut_pol_norm[icut][iminv][ipol] =
				getNorm(hpolar_cut_pol[icut][iminv][ipol],evt);
		}

		harr->Add(hmass_cut_pol_norm[icut][ipol]);
		harr->Add(hoangle_cut_pol_norm[icut][ipol]);
		harr->Add(hrap_cut_pol_norm[icut][ipol]);
		harr->Add(hpt_cut_pol_norm[icut][ipol]);
		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			harr->Add(hpolar_cut_pol_norm[icut][iminv][ipol]);
		}
	}
#ifdef SIMULATION
	hmass_cut_truecb_norm[icut] =
		getNorm(hmass_cut_truecb[icut],evt);
	hoangle_cut_truecb_norm[icut] =
		getNorm(hoangle_cut_truecb[icut],evt);
	hrap_cut_truecb_norm[icut] =
		getNorm(hrap_cut_truecb[icut],evt);
	hpt_cut_truecb_norm[icut] =
		getNorm(hpt_cut_truecb[icut],evt);
	for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
	{
		hpolar_cut_truecb_norm[icut][iminv] =
			getNorm(hpolar_cut_truecb[icut][iminv],evt);
	}

	harr->Add(hmass_cut_truecb_norm[icut]);
	harr->Add(hoangle_cut_truecb_norm[icut]);
	harr->Add(hrap_cut_truecb_norm[icut]);
	harr->Add(hpt_cut_truecb_norm[icut]);
	for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
	{
		harr->Add(hpolar_cut_truecb_norm[icut][iminv]);
	}

	for(Int_t ipair=0; ipair<MAXPAIR; ipair++)
	{
		hmass_cut_true_norm[icut][ipair] =
			getNorm(hmass_cut_true[icut][ipair],evt);
		hoangle_cut_true_norm[icut][ipair] =
			getNorm(hoangle_cut_true[icut][ipair],evt);
		hrap_cut_true_norm[icut][ipair] =
			getNorm(hrap_cut_true[icut][ipair],evt);
		hpt_cut_true_norm[icut][ipair] =
			getNorm(hpt_cut_true[icut][ipair],evt);
		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			hpolar_cut_true_norm[icut][iminv][ipair] =
				getNorm(hpolar_cut_true[icut][iminv][ipair],evt);
		}

		harr->Add(hmass_cut_true_norm[icut][ipair]);
		harr->Add(hoangle_cut_true_norm[icut][ipair]);
		harr->Add(hrap_cut_true_norm[icut][ipair]);
		harr->Add(hpt_cut_true_norm[icut][ipair]);
		for(Int_t iminv=0; iminv<MAXMINVBINS; iminv++)
		{
			harr->Add(hpolar_cut_true_norm[icut][iminv][ipair]);
		}
	}
#endif // SIMULATION
	}
}

void pairs::fillHistograms(Int_t cut, Float_t weight)
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
	hpolar_cut_pol[cut][minvbin][pol]->Fill(polar,weight*polarweight);
#ifdef SIMULATION
	if (pol == 1) {
	if(evtGen==bPluto)
	{
		if(GCommonDet1 >=76 && GCommonDet2 >=76 &&
		(Ggeninfo2_1 != Ggeninfo2_2) && Ggeninfo1_1>0 && Ggeninfo1_2>0 )
		{
		hmass_cut_truecb[cut]->Fill(invmass,weight);
		hoangle_cut_truecb[cut]->Fill(opang,weight);
		hrap_cut_truecb[cut]->Fill(rap,weight);
		hpt_cut_truecb[cut]->Fill(pt,weight);
		hpolar_cut_truecb[cut][minvbin]->Fill(polar,weight*polarweight);
		}

	if ( GparentTrackNb1==0 && GparentTrackNb2==0 && // primary particles
		Ggeninfo2_1==Ggeninfo2_2 && // parent track
		Ggeninfo1_1==Ggeninfo1_2 && // parent id
		Ggeninfo1_1>0			&&
		GCommonDet1>=76		&& // common hit seen in all detectors
		GCommonDet2 >= 76)
	{
		hmass_cut_true[cut][0]->Fill(invmass,weight);
		hoangle_cut_true[cut][0]->Fill(opang,weight);
		hrap_cut_true[cut][0]->Fill(rap,weight);
		hpt_cut_true[cut][0]->Fill(pt,weight);
		hpolar_cut_true[cut][minvbin][0]->Fill(polar,weight*polarweight);

		switch((int)Ggeninfo1_1)
		{
		case 7051: // pi0 dalitz
			hmass_cut_true[cut][1]->Fill(invmass,weight);
			hoangle_cut_true[cut][1]->Fill(opang,weight);
			hrap_cut_true[cut][1]->Fill(rap,weight);
			hpt_cut_true[cut][1]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][1]->Fill(polar,weight*polarweight);
			break;

		case 7001: // conversion
			hmass_cut_true[cut][2]->Fill(invmass,weight);
			hoangle_cut_true[cut][2]->Fill(opang,weight);
			hrap_cut_true[cut][2]->Fill(rap,weight);
			hpt_cut_true[cut][2]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][2]->Fill(polar,weight*polarweight);

		break;

		case 17051: // eta dalitz
			hmass_cut_true[cut][3]->Fill(invmass,weight);
			hoangle_cut_true[cut][3]->Fill(opang,weight);
			hrap_cut_true[cut][3]->Fill(rap,weight);
			hpt_cut_true[cut][3]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][3]->Fill(polar,weight*polarweight);
			break;

		case 34051: // delta dalitz
			hmass_cut_true[cut][4]->Fill(invmass,weight);
			hoangle_cut_true[cut][4]->Fill(opang,weight);
			hrap_cut_true[cut][4]->Fill(rap,weight);
			hpt_cut_true[cut][4]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][4]->Fill(polar,weight*polarweight);
			break;

		case 41: // rho direct
			hmass_cut_true[cut][5]->Fill(invmass,weight);
			hoangle_cut_true[cut][5]->Fill(opang,weight);
			hrap_cut_true[cut][5]->Fill(rap,weight);
			hpt_cut_true[cut][5]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][5]->Fill(polar,weight*polarweight);
			break;

		case 52051: // omega dalitz
			hmass_cut_true[cut][6]->Fill(invmass,weight);
			hoangle_cut_true[cut][6]->Fill(opang,weight);
			hrap_cut_true[cut][6]->Fill(rap,weight);
			hpt_cut_true[cut][6]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][6]->Fill(polar,weight*polarweight);
			break;

		case 52: // omega direct
			hmass_cut_true[cut][7]->Fill(invmass,weight);
			hoangle_cut_true[cut][7]->Fill(opang,weight);
			hrap_cut_true[cut][7]->Fill(rap,weight);
			hpt_cut_true[cut][7]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][7]->Fill(polar,weight*polarweight);
			break;

		default:
		break;
		}
	}
	}
	else if(evtGen==bUrQMD) // U R Q M D
	{

		//cout << " GCommonDet1 "<< GCommonDet1 << endl;
		//cout << " GCommonDet2 "<< GCommonDet2 << endl;

		if(GCommonDet1 >=76 && GCommonDet2 >=76 &&
			(GparentTrackNb1 != GparentTrackNb2) )
		{
		hmass_cut_truecb[cut]->Fill(invmass,weight);
		hoangle_cut_truecb[cut]->Fill(opang,weight);
		hrap_cut_truecb[cut]->Fill(rap,weight);
		hpt_cut_truecb[cut]->Fill(pt,weight);
		hpolar_cut_truecb[cut][minvbin]->Fill(polar,weight*polarweight);
		}

		if(GdecayId>=0 && GCommonDet1 >=76 && GCommonDet2 >=76)
		{
		hmass_cut_true[cut][0]->Fill(invmass,weight);
		hoangle_cut_true[cut][0]->Fill(opang,weight);
		hrap_cut_true[cut][0]->Fill(rap,weight);
		hpt_cut_true[cut][0]->Fill(pt,weight);
		hpolar_cut_true[cut][minvbin][0]->Fill(polar,weight*polarweight);

		switch((int)GdecayId)
		{
		case 0: // pi0 dalitz
			hmass_cut_true[cut][1]->Fill(invmass,weight);
			hoangle_cut_true[cut][1]->Fill(opang,weight);
			hrap_cut_true[cut][1]->Fill(rap,weight);
			hpt_cut_true[cut][1]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][1]->Fill(polar,weight*polarweight);
			break;

		case 1: // conversion
			hmass_cut_true[cut][2]->Fill(invmass,weight);
			hoangle_cut_true[cut][2]->Fill(opang,weight);
			hrap_cut_true[cut][2]->Fill(rap,weight);
			hpt_cut_true[cut][2]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][2]->Fill(polar,weight*polarweight);
			break;

		case 2: // eta dalitz
			hmass_cut_true[cut][3]->Fill(invmass,weight);
			hoangle_cut_true[cut][3]->Fill(opang,weight);
			hrap_cut_true[cut][3]->Fill(rap,weight);
			hpt_cut_true[cut][3]->Fill(pt,weight);
			hpolar_cut_true[cut][minvbin][3]->Fill(polar,weight*polarweight);
			break;

		default:
			break;
		}
		}
	}
	}
#endif // SIMULATION
}

TH1F *pairs::getBackg(TH1F *pp, TH1F* ee,Int_t typ)
{
	TString name(pp->GetName());
	name.Remove(name.Length()-4, name.Length() );
	name.Append("back_");
	name+=typ;

	TH1F *bground = (TH1F*) pp->Clone(name.Data());
	bground->Reset();
#if 0
	bground->Sumw2();
#endif

	Double_t massPP, massNN;
	Int_t Bins = pp->GetNbinsX();

	for(int i = 1; i <= Bins; i++)
	{
	massPP = pp->GetBinContent(i);
	massNN = ee->GetBinContent(i);

	if(typ == 0)
	{
		bground->SetBinContent(i,2*TMath::Sqrt(massPP*massNN));
		if(massPP*massNN)
		bground->SetBinError(i,TMath::Sqrt((pp->GetBinError(i))*
							pp->GetBinError(i) *
							massNN/massPP		+
							ee->GetBinError(i) *
							ee->GetBinError(i) *
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

TH1F *pairs::getNorm(TH1F *ep,Float_t evt)
{
	// evt is for event number normalization !!!
	TString name(ep->GetName());
	name.Append("_norm");
	TH1F *invMass = (TH1F*)ep->Clone(name.Data());
#if 0
	invMass->Sumw2();
#endif

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

TH1F *pairs::getSignal(TH1F *ep, TH1F* bg)
{
	TString name(bg->GetName());
	name.Append("_sig");

	TH1F *invMass = (TH1F*) ep->Clone(name.Data());
	invMass->Reset();
#if 0
	invMass->Sumw2();
#endif
	invMass->Add(ep,bg,1.,-1.);
	return invMass;
}

TH1F *pairs::rebinVar(TH1F* h,Float_t* xbins,Int_t nbins,
				Bool_t kNorm ,Bool_t kErr)
{

	// ASSUMPTION: input histo is __NOT__ normalized to bin width !
	// rebin fixed bin histogram to variable binning
	// default: divide by bin width
	// default: calculate new errors after rebinning

	TString name(h->GetName());
	name.Append("_rebinned");

	//create new histogram with new binning
	TH1F *hn = new TH1F(name.Data(),"rebinned_histo",nbins,xbins);
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

Float_t pairs::getRecPhi(Float_t sec, Float_t phi)
{
	if (phi>180.) {
		return phi -360.;
	}
	return phi;
}

Float_t pairs::getEfficiencyFactor()
{
	if(mom1>2000 || mom2>2000) return 0.;
	Float_t fEff1 = 1.;
	Float_t fEff2 = 1.;
	const Float_t d2r = TMath::DegToRad();

	// phi needs to be recalculated to be in 0-60 deg range
	phi1 = getRecPhi(sec1,phi1);
	phi2 = getRecPhi(sec2,phi2);

	// first leg of the pair
	if (chrg1==1) // positron
	{
	if (p3DEffPosi) fEff1 = p3DEffPosi->
				GetBinContent(p3DEffPosi->FindBin(d2r*phi1,d2r*theta1,mom1));
	else fEff1 = 1.;
	}
	else if (chrg1==-1) // electron
	{
	if (p3DEffEle) fEff1 = p3DEffEle->
		GetBinContent(p3DEffEle->FindBin(d2r*phi1,d2r*theta1,mom1));
	else fEff1=1.;
	}

	// second leg of the pair
	if (chrg2==1) // positron
	{

	if (p3DEffPosi)
		fEff2 = p3DEffPosi->
			GetBinContent(p3DEffPosi->FindBin(d2r*phi2,d2r*theta2,mom2));
	else
		fEff2=1.;
	}
	else if (chrg2==-1) // electron
	{
	if (p3DEffEle)
		fEff2 = p3DEffEle->
			GetBinContent(p3DEffEle->FindBin(d2r*phi2,d2r*theta2,mom2));
	else
		fEff2=1.;
	}
	if (fEff1*fEff2>0)
		return 1./(fEff1*fEff2);
	else
	{
		cout<<" Efficiency is "<<fEff1*fEff2<<endl;
		return 0.;
	}
}

#ifdef SIMULATION
Float_t pairs::calcWeight()
{
	Float_t weight = 1.0;

	if(evtGen==bPluto) // P L U T O
	{
	const Int_t PLUTO_PI0_GAMMA = 7001;
	const Int_t PLUTO_ETA_GAMMA = 1701;
	const Int_t PLUTO_OMEGA_GAMMA = 5201;

	// DALITZ/DIRECT PAIR FROM THE SAME MESON
	if( Ggeninfo2_1 == Ggeninfo2_2 && Ggeninfo1_1==Ggeninfo1_2
		&& Ggeninfo1_1 >0
		&& Ggeninfo1_1 != PLUTO_PI0_GAMMA
		&& Ggeninfo1_1 != PLUTO_ETA_GAMMA
		&& Ggeninfo1_1 != PLUTO_OMEGA_GAMMA)
	{
		weight = Ggenweight1;
	}

	// DIRECT PAIR FROM CONVERSION
	else if( Ggeninfo2_1 == Ggeninfo2_2 && Ggeninfo1_1==Ggeninfo1_2
			&& Ggeninfo1_1 >0
			&& (Ggeninfo1_1 == PLUTO_PI0_GAMMA
			|| Ggeninfo1_1 == PLUTO_ETA_GAMMA
			|| Ggeninfo1_1 == PLUTO_OMEGA_GAMMA)
			&& Ggeninfo1 == Ggeninfo2) // 1 grand parent == 2 grand parent
	{
		weight = Ggenweight1;
	}
	// MIXED PAIR FROM THE SAME MESON and 1 SUBSEQUENT CONVERSION
	else if(Ggeninfo2_1 != Ggeninfo2_2 && Ggeninfo1_1!=Ggeninfo1_2
			&& Ggeninfo1_1 >0 && Ggeninfo1_2 >0
			&& ( (Ggeninfo1_1 == PLUTO_PI0_GAMMA
				||Ggeninfo1_1 == PLUTO_ETA_GAMMA
				||Ggeninfo1_1 == PLUTO_OMEGA_GAMMA)
			&&	(Ggeninfo1_2 != PLUTO_PI0_GAMMA
				||Ggeninfo1_2 != PLUTO_ETA_GAMMA
				||Ggeninfo1_2 != PLUTO_ETA_GAMMA)
				&& Ggeninfo1==Ggeninfo2_2 ) // 1 grandparent track == 2 parent track
			||
				( (Ggeninfo1_2 == PLUTO_PI0_GAMMA
				||Ggeninfo1_2 == PLUTO_ETA_GAMMA
				||Ggeninfo1_2 == PLUTO_OMEGA_GAMMA)
			&&	(Ggeninfo1_1 != PLUTO_PI0_GAMMA
				||Ggeninfo1_1 != PLUTO_ETA_GAMMA
				||Ggeninfo1_1 != PLUTO_ETA_GAMMA)
				&& Ggeninfo2==Ggeninfo2_1 ) // 2 grandparent track == 1 parent track
			)
	{
		weight = Ggenweight1;
	}
	// MIXED PAIR FROM THE SAME 2 SUBSEQUENT CONVERSION
	else if(Ggeninfo2_1 != Ggeninfo2_2 && Ggeninfo1_1==Ggeninfo1_2
			&& Ggeninfo1_1 >0
			&& ( (Ggeninfo1_1 == PLUTO_PI0_GAMMA
				||Ggeninfo1_1 == PLUTO_ETA_GAMMA
				||Ggeninfo1_1 == PLUTO_OMEGA_GAMMA)
				&& Ggeninfo1==Ggeninfo2 ) // 1 grandparent track == 2 grandparent track
			)
	{
		weight = Ggenweight1;
	}
	else
	{
		weight = Ggenweight1*Ggenweight2;
	}
	}
	else if(evtGen==bUrQMD) // U R Q M D
	{
	const Float_t ENHANCEMENT_ETA = 10.;
	const Int_t GAMMA = 1;
	const Int_t ETA = 17;

	// ETA DALITZ PAIR
	if(GparentTrackNb1 == GparentTrackNb2 && GparentId1==ETA)
		weight = 1./ENHANCEMENT_ETA;

	// ETA MIXED PAIR
	else if(GparentTrackNb1 != GparentTrackNb2 &&
		(GparentId1 == ETA || GparentId2==ETA))
		weight = 1./ENHANCEMENT_ETA;

	// MIXED PAIR WITH 2 ETA DALITZ
	else if(GparentTrackNb1 != GparentTrackNb2 &&
		(GparentId1 == ETA && GparentId2==ETA))
		weight = 1./(ENHANCEMENT_ETA*ENHANCEMENT_ETA);

	// MIXED PAIR WITH 2 ETA DALITZ AND 1 SUBSEQUENT CONVERSION
	else if ( ((GgrandparentId1==GparentId2 && GgrandparentId1==ETA) ||
			(GgrandparentId2==GparentId1 && GgrandparentId2==ETA)) &&
		(GgrandparentTrackNb1!=GparentTrackNb2 ||
			GgrandparentTrackNb2!=GparentTrackNb1) )
		weight = 1./(ENHANCEMENT_ETA*ENHANCEMENT_ETA);

	// MIXED PAIR WITH 2 ETA DALITZ AND 2 SUBSEQUENT CONVERSION
	else if ( (GgrandparentId1==GgrandparentId2 &&
			GgrandparentId1==ETA)								&&
		(GgrandparentTrackNb1!=GgrandparentTrackNb2) )
		weight = 1./(ENHANCEMENT_ETA*ENHANCEMENT_ETA);

	// CONVERSION PAIR AFTER ETA DALITZ
	else if (GparentTrackNb1 == GparentTrackNb2 && GparentId1==GAMMA &&
		GgrandparentId1==GgrandparentId2 && GgrandparentId1==ETA &&
		GgrandparentTrackNb1==GgrandparentTrackNb2)
		weight = 1./ENHANCEMENT_ETA;

	// MIXED PAIR ETA DALITZ AND CONVERSION AFTER ETA DALITZ
	else if ((GgrandparentTrackNb1==GparentTrackNb2 ||
		GgrandparentTrackNb2==GparentTrackNb1)	&&
		((GgrandparentId1==GparentId2 && GgrandparentId1==ETA) ||
		(GgrandparentId2==GparentId1 && GgrandparentId2==ETA)))
		weight = 1./ENHANCEMENT_ETA;

	// NO ENHANCED PARTICLE INVOLVED
	else
		weight = Ggenweight1;
	}
	return weight;
}
#endif // SIMULATION

