//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Feb 13 15:43:36 2005 by ROOT version 4.02/00
// from TTree ntuple_pp_pip_pim_pi0_miss/Demo ntuple
// found on file: /d/c2/hadeshyp/output/hyp_ntup_jan04_120205.root
//////////////////////////////////////////////////////////

#ifndef project_exp_h
#define project_exp_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

class project_exp {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Float_t         pp_miss;
   Float_t         miss4;
   Float_t         pippim_invmass;
   Float_t         fProbAlg;

   // List of branches
   TBranch        *b_pp_miss;   //!
   TBranch        *b_miss4;   //!
   TBranch        *b_pippim_invmass;   //!
   TBranch        *b_fProbAlg;   //!

   project_exp(TTree *tree=0);
   virtual ~project_exp();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef project_exp_cxx
project_exp::project_exp(TTree *tree)
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/d/c2/hadeshyp/output/hyp_ntup_jan04_120205.root");
      if (!f) {
         f = new TFile("/d/c2/hadeshyp/output/hyp_ntup_jan04_120205.root");
      }
      tree = (TTree*)gDirectory->Get("ntuple_pp_pip_pim_pi0_miss");

   }
   Init(tree);
}

project_exp::~project_exp()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t project_exp::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t project_exp::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->IsA() != TChain::Class()) return centry;
   TChain *chain = (TChain*)fChain;
   if (chain->GetTreeNumber() != fCurrent) {
      fCurrent = chain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void project_exp::Init(TTree *tree)
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

   fChain->SetBranchAddress("pp_miss",&pp_miss);
   fChain->SetBranchAddress("miss4",&miss4);
   fChain->SetBranchAddress("pippim_invmass",&pippim_invmass);
   fChain->SetBranchAddress("fProbAlg",&fProbAlg);
   Notify();
}

Bool_t project_exp::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. Typically here the branch pointers
   // will be retrieved. It is normaly not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed.

   // Get branch pointers
   b_pp_miss = fChain->GetBranch("pp_miss");
   b_miss4 = fChain->GetBranch("miss4");
   b_pippim_invmass = fChain->GetBranch("pippim_invmass");
   b_fProbAlg = fChain->GetBranch("fProbAlg");

   return kTRUE;
}

void project_exp::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t project_exp::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef project_exp_cxx
