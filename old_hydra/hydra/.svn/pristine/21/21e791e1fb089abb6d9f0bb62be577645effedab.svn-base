#define pairs_cxx
#include "pairs.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include <iostream>

using namespace std;
pairs::pairs(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("sim.root");
      if (!f) {
         f = new TFile("sim.root");
      }
      tree = (TTree*)gDirectory->Get("PairsFilter");

   }
   Init(tree);
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
   fChain->SetBranchAddress("isGoodOpang",&isGoodOpang);
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
   b_isGoodOpang = fChain->GetBranch("isGoodOpang");

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
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
void pairs::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L pairs.C
//      Root > pairs t
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

   Int_t nbytes = 0, nb = 0;
   harr = new TObjArray(5);

   TH1F* hmass_unlike = new TH1F("mass_unlike","mass_unlike",100,0,1000);
   harr->Add(hmass_unlike);

   TH1F* hmass_likep = new TH1F("mass_likep","mass_likep",100,0,1000);
   harr->Add(hmass_likep);

   TH1F* hmass_likem = new TH1F("mass_likem","mass_likem",100,0,1000);
   harr->Add(hmass_likem);

   TH1F* hmass_unlike_true = new TH1F("mass_unlike_true","mass_unlike_true",
				      100,0,1000);
   harr->Add(hmass_unlike_true);


   // L O O P /////////////////////////////////////////////////////////////

   for (Int_t jentry=0; jentry<nentries;jentry++) {
      Int_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;

      if (charge==0 && isGoodOpang==1 && doubleHit==0 && GCommonDet1>=76 && GCommonDet2 >= 76 )  hmass_unlike->Fill(invmass);
      if (charge==2 && isGoodOpang==1  && doubleHit==0)	 hmass_likep->Fill(invmass);
      if (charge==-2 && isGoodOpang==1  && doubleHit==0)  hmass_likem->Fill(invmass);

      if (charge==0 && isGoodOpang==1 && GdecayId>=0 && doubleHit==0 && GCommonDet1>=76 && GCommonDet2 >= 76)
	  hmass_unlike_true->Fill(invmass);

      
   } // end of L O O P ////////////////////////////////////////////////////

   // calculate background and signal

   Float_t xbins[] = {0.,10.,20.,30.,40.,
		      50.,60.,70.,80.,90.,
		      100.,120.,140.,160.,
		      180.0,210.,250.,
		      300.,350.,420.,500.,580.,
		      680.,780.,880.,1000.};

   Int_t nbins = sizeof(xbins)/sizeof(Float_t);

   TH1F* hmass_back         = getBackg(hmass_likep,hmass_likem,0);
   TH1F* hmass_sig          = getSignal(hmass_unlike, hmass_back);
   harr->Add(hmass_back);
   harr->Add(hmass_sig);
   TH1F* hmass_sig_rebin = rebinVar(hmass_sig,xbins,nbins-1);
   harr->Add(hmass_sig_rebin);
   TH1F* hmass_unlike_true_rebin = rebinVar(hmass_unlike_true,xbins,nbins-1);
   harr->Add(hmass_unlike_true_rebin);
   TH1F* hmass_unlike_true_rebin_c = (TH1F*) hmass_unlike_true_rebin
       ->Clone("hmass_unlike_true_rebin_c");
   hmass_unlike_true_rebin_c->Divide(hmass_sig_rebin);
   harr->Add(hmass_unlike_true_rebin_c);

   TH1F* hmass_back_rebin = rebinVar(hmass_back,xbins,nbins-1);
   harr->Add(hmass_back_rebin);
   
   Float_t LVL1      = 120210266;
   Float_t LVL1_KICK = 110588298;

   Float_t NORM = LVL1;
 
   hmass_sig_rebin->Scale(1./NORM);
   hmass_unlike_true_rebin->Scale(1./NORM);

   hmass_back_rebin->Scale(1./NORM);
}


TH1F* pairs::rebinVar(TH1F* h,Float_t* xbins,Int_t nbins,
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

TH1F* pairs::getBackg(TH1F *pp, TH1F* nn, Int_t typ)
{
    // typ=0  geom mean
    // typ=1  arith mean
    char name[256];
    sprintf(name,"%s_CB",pp->GetName());

    TH1F *bground = new TH1F(name,"CB",pp->GetNbinsX(),0,
			     pp->GetXaxis()->GetXmax());

    bground->Sumw2();
    Double_t massPP, massNN;
    Double_t errPP, errNN;
    Int_t maxBins = pp->GetNbinsX();

    for(int i = 1; i < maxBins+1; i++)
    {// skip underflow bin
	massPP  =  pp->GetBinContent(i);
	massNN  =  nn->GetBinContent(i);

	errPP   = pp->GetBinError(i);
	errNN   = nn->GetBinError(i);



	if(typ == 0 )
	{
	    bground->SetBinContent(i,2.*TMath::Sqrt(massPP*massNN));

	    if(massNN>0. && massPP>0. && errPP*errPP*massNN*massPP>=0.)
		bground->SetBinError(i,TMath::Sqrt(  errPP*errPP*massNN/
						     massPP                  +
						     errNN*errNN*massPP/
						     massNN));
	}

	if(typ == 1)
	{
	    bground->SetBinContent(i,massPP+massNN);
	    Float_t radix = errPP*errPP + errNN*errNN;
	    if (radix>=0.) bground->SetBinError(i,TMath::Sqrt(radix));
	}
    }
    
    return bground;
}

TH1F* pairs::getSignal(TH1F *pn, TH1F* backg)
{
    char name[256];
    sprintf(name,"%s_sig",pn->GetName());
    TH1F  *hsig = new TH1F(name,"signal",pn->GetNbinsX(),0,
			   pn->GetXaxis()->GetXmax());
    hsig->Sumw2();
    hsig->Add(pn,backg,1.,-1.);
    return hsig;
}


