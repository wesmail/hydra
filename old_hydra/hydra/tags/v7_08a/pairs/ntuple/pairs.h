//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 16 23:21:22 2005 by ROOT version 4.00/06
// from TTree PairsFilter/PairsFilter
// found on file: sim.root
//////////////////////////////////////////////////////////

#ifndef pairs_h
#define pairs_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TH1F.h>
class pairs {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leave types
   Float_t         invmass;
   Float_t         opang;
   Float_t         rap;
   Float_t         pt;
   Float_t         charge;
   Float_t         isCutNb;
   Float_t         idxPart1;
   Float_t         idxPart2;
   Float_t         prob1;
   Float_t         prob2;
   Float_t         pid1;
   Float_t         pid2;
   Float_t         idxpidcand1;
   Float_t         sys1;
   Float_t         r1;
   Float_t         z1;
   Float_t         massexp1;
   Float_t         betaexp1;
   Float_t         momalgidx1;
   Float_t         chrg1;
   Float_t         mostprobpid1;
   Float_t         weightmostprobpid1;
   Float_t         theta1;
   Float_t         phi1;
   Float_t         sec1;
   Float_t         idxpidcand2;
   Float_t         sys2;
   Float_t         r2;
   Float_t         z2;
   Float_t         massexp2;
   Float_t         betaexp2;
   Float_t         momalgidx2;
   Float_t         chrg2;
   Float_t         mostprobpid2;
   Float_t         weightmostprobpid2;
   Float_t         theta2;
   Float_t         phi2;
   Float_t         sec2;
   Float_t         drmt1;
   Float_t         drmp1;
   Float_t         drmt2;
   Float_t         drmp2;
   Float_t         tof1;
   Float_t         tof2;
   Float_t         rpadnr1;
   Float_t         rcentroid1;
   Float_t         rt1;
   Float_t         rp1;
   Float_t         rpatmat1;
   Float_t         rhoutra1;
   Float_t         rampl1;
   Float_t         rlocmax41;
   Float_t         rpadnr2;
   Float_t         rcentroid2;
   Float_t         rt2;
   Float_t         rp2;
   Float_t         rpatmat2;
   Float_t         rhoutra2;
   Float_t         rampl2;
   Float_t         rlocmax42;
   Float_t         mom1;
   Float_t         mom2;
   Float_t         doubleHit;
   Float_t         Gpid1;
   Float_t         GparentId1;
   Float_t         GprocessId1;
   Float_t         Gmom1;
   Float_t         Gpid2;
   Float_t         GparentId2;
   Float_t         GprocessId2;
   Float_t         Gmom2;
   Float_t         Ginvmass;
   Float_t         Gopang;
   Float_t         Grap;
   Float_t         Gpt;
   Float_t         Gcharge;
   Float_t         GparentTrackNb1;
   Float_t         GparentTrackNb2;
   Float_t         GdecayId;
   Float_t         GCommonDet1;
   Float_t         GCommonDet2;
   Float_t         Gvx1;
   Float_t         Gvy1;
   Float_t         Gvz1;
   Float_t         Gvx2;
   Float_t         Gvy2;
   Float_t         Gvz2;
   Float_t         Gmed1;
   Float_t         Gmed2;
   Float_t         Ggeninfo1;
   Float_t         Ggenweight1;
   Float_t         Ggeninfo2;
   Float_t         Ggenweight2;
   Float_t         isGoodOpang;

   // List of branches
   TBranch        *b_invmass;   //!
   TBranch        *b_opang;   //!
   TBranch        *b_rap;   //!
   TBranch        *b_pt;   //!
   TBranch        *b_charge;   //!
   TBranch        *b_isCutNb;   //!
   TBranch        *b_idxPart1;   //!
   TBranch        *b_idxPart2;   //!
   TBranch        *b_prob1;   //!
   TBranch        *b_prob2;   //!
   TBranch        *b_pid1;   //!
   TBranch        *b_pid2;   //!
   TBranch        *b_idxpidcand1;   //!
   TBranch        *b_sys1;   //!
   TBranch        *b_r1;   //!
   TBranch        *b_z1;   //!
   TBranch        *b_massexp1;   //!
   TBranch        *b_betaexp1;   //!
   TBranch        *b_momalgidx1;   //!
   TBranch        *b_chrg1;   //!
   TBranch        *b_mostprobpid1;   //!
   TBranch        *b_weightmostprobpid1;   //!
   TBranch        *b_theta1;   //!
   TBranch        *b_phi1;   //!
   TBranch        *b_sec1;   //!
   TBranch        *b_idxpidcand2;   //!
   TBranch        *b_sys2;   //!
   TBranch        *b_r2;   //!
   TBranch        *b_z2;   //!
   TBranch        *b_massexp2;   //!
   TBranch        *b_betaexp2;   //!
   TBranch        *b_momalgidx2;   //!
   TBranch        *b_chrg2;   //!
   TBranch        *b_mostprobpid2;   //!
   TBranch        *b_weightmostprobpid2;   //!
   TBranch        *b_theta2;   //!
   TBranch        *b_phi2;   //!
   TBranch        *b_sec2;   //!
   TBranch        *b_drmt1;   //!
   TBranch        *b_drmp1;   //!
   TBranch        *b_drmt2;   //!
   TBranch        *b_drmp2;   //!
   TBranch        *b_tof1;   //!
   TBranch        *b_tof2;   //!
   TBranch        *b_rpadnr1;   //!
   TBranch        *b_rcentroid1;   //!
   TBranch        *b_rt1;   //!
   TBranch        *b_rp1;   //!
   TBranch        *b_rpatmat1;   //!
   TBranch        *b_rhoutra1;   //!
   TBranch        *b_rampl1;   //!
   TBranch        *b_rlocmax41;   //!
   TBranch        *b_rpadnr2;   //!
   TBranch        *b_rcentroid2;   //!
   TBranch        *b_rt2;   //!
   TBranch        *b_rp2;   //!
   TBranch        *b_rpatmat2;   //!
   TBranch        *b_rhoutra2;   //!
   TBranch        *b_rampl2;   //!
   TBranch        *b_rlocmax42;   //!
   TBranch        *b_mom1;   //!
   TBranch        *b_mom2;   //!
   TBranch        *b_doubleHit;   //!
   TBranch        *b_Gpid1;   //!
   TBranch        *b_GparentId1;   //!
   TBranch        *b_GprocessId1;   //!
   TBranch        *b_Gmom1;   //!
   TBranch        *b_Gpid2;   //!
   TBranch        *b_GparentId2;   //!
   TBranch        *b_GprocessId2;   //!
   TBranch        *b_Gmom2;   //!
   TBranch        *b_Ginvmass;   //!
   TBranch        *b_Gopang;   //!
   TBranch        *b_Grap;   //!
   TBranch        *b_Gpt;   //!
   TBranch        *b_Gcharge;   //!
   TBranch        *b_GparentTrackNb1;   //!
   TBranch        *b_GparentTrackNb2;   //!
   TBranch        *b_GdecayId;   //!
   TBranch        *b_GCommonDet1;   //!
   TBranch        *b_GCommonDet2;   //!
   TBranch        *b_Gvx1;   //!
   TBranch        *b_Gvy1;   //!
   TBranch        *b_Gvz1;   //!
   TBranch        *b_Gvx2;   //!
   TBranch        *b_Gvy2;   //!
   TBranch        *b_Gvz2;   //!
   TBranch        *b_Gmed1;   //!
   TBranch        *b_Gmed2;   //!
   TBranch        *b_Ggeninfo1;   //!
   TBranch        *b_Ggenweight1;   //!
   TBranch        *b_Ggeninfo2;   //!
   TBranch        *b_Ggenweight2;   //!
   TBranch        *b_isGoodOpang;   //!

    TObjArray* harr;//!
   pairs(TTree *tree=0);
   ~pairs();
    TObjArray* getHistograms(void){return harr;};
    TH1F* rebinVar(TH1F*,Float_t* ,Int_t ,Bool_t kNorm=kTRUE,Bool_t kErr=kTRUE);
    TH1F* getSignal(TH1F*, TH1F*);
    TH1F* getBackg(TH1F*, TH1F*, Int_t);
   Int_t  Cut(Int_t entry);
   Int_t  GetEntry(Int_t entry);
   Int_t  LoadTree(Int_t entry);
   void   Init(TTree *tree);
   void   Loop();
   Bool_t Notify();
   void   Show(Int_t entry = -1);
};

#endif // #ifdef pairs_cxx
