//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon May 16 23:21:22 2005 by ROOT version 4.00/06
// from TTree PairsFilter/PairsFilter
// found on file: sim.root
//////////////////////////////////////////////////////////

#ifndef pairsE_h
#define pairsE_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TObjArray.h>
#include <TH1F.h>
#include <TH2F.h>
#include <TH3F.h>


//////////////////////////////////////////////////////////////
// CUT DEFINITIONS - PAIR FRAMEWORK - 25052005
//////////////////////////////////////////////////////////////
// isCutNb==0 : isGoodOpang==1 && isNotDoubleHit==1 && all tracks removed
// isCutNb!=0 : all removed pairs, by direct cuts and track removing
// isCutNb==1 : isGoodOpang!=1 || isNotDoubleHit!=1, direct cut pairs
// isCutNb!=1 : isGoodOpang==1 && isNotDoubleHit==1              
// isCutNb==2 : pairs removed by track removing due to opening angle cut
// isCutNb!=2 : all pairs except for isCutNb==2
// isCutNb==3 : pairs removed by track removing due to nodoublehit cut
// isCutNb!=3 : all pairs except for isCutNb==3
// isCutNb==10: pairs removed by track removing due to opang and nodouble cut
// isCutNb!=10: all pairs except for isCutNb==10 
// 
// HISTOGRAMS

// 0.)  nocuts
// 1.)  isGoodOpang==1
// 2.)  isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 (+track removing) 
// 3.)  isNotDoubleHit==1
// 4.)  isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 (+track removing)
// 5.)  isGoodOpang==1 && isNotDoubleHit==1 
// 6.)  isGoodOpang==1 && isNotDoubleHit==1 && iscpcandidate1==0 && iscpcandidate2==0 
// 7.)  isCutNb==0 (all direct + all track removing)
// 8.)  isCutNb==0 && mom1>120 && mom2>120
// 9.)  isCutNb==0 && iscpcandidate1==0 && iscpcandidate2==0 
// 10.) isCutNb==0 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120 
// 11.) isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1
// 12.) isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1
// 13.) isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1 && iscpcandidate1==0 && iscpcandidate2==0
// 14.) isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0
// 15.) isGoodOpang==1 && isCutNb!=2 && isCutNb!=10 && isNotDoubleHit==1 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120
// 16.) isNotDoubleHit==1 && isCutNb!=3 && isCutNb!=10 && isGoodOpang==1 && iscpcandidate1==0 && iscpcandidate2==0 && mom1>120 && mom2>120

#define MAXCUT   36 // 2nd half is with efficiency correction   
#define MAXPOL   3  // pair polarity [0] = -2, [1] = 0, [2] = 2

class pairsE {
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
    Float_t         qspline1;
    Float_t         qspline2;
    Float_t         innerchisquare1;
    Float_t         innerchisquare2;
    Float_t         outerchisquare1;
    Float_t         outerchisquare2;
    Float_t         distancetovertex1;
    Float_t         distancetovertex2;
    Float_t         iscpcandidate1;
    Float_t         iscpcandidate2;
    Float_t         opangcpcandidate1;
    Float_t         opangcpcandidate2;
    Float_t         isGoodOpang;
    Float_t         isNotDoubleHit;

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
    TBranch        *b_qspline1;   //!
    TBranch        *b_qspline2;   //!
    TBranch        *b_innerchisquare1;   //!
    TBranch        *b_innerchisquare2;   //!
    TBranch        *b_outerchisquare1;   //!
    TBranch        *b_outerchisquare2;   //!
    TBranch        *b_distancetovertex1;   //!
    TBranch        *b_distancetovertex2;   //!
    TBranch        *b_iscpcandidate1;   //!
    TBranch        *b_iscpcandidate2;   //!
    TBranch        *b_opangcpcandidate1;   //!
    TBranch        *b_opangcpcandidate2;   //!
    TBranch        *b_isGoodOpang;   //!
    TBranch        *b_isNotDoubleHit;   //!

   TObjArray* harr;//!
   pairsE(TTree *tree=0);
   ~pairsE();
   TObjArray* getHistograms(void){return harr;};
   TH1F* rebinVar(TH1F*,Float_t* ,Int_t ,Bool_t kNorm=kTRUE,Bool_t kErr=kTRUE);
   TH1F* getSignal(TH1F*, TH1F*);
   TH1F* getBackg(TH1F*, TH1F*, Int_t);
   TH1F *getNorm(TH1F*, Float_t);
   Float_t getRecPhi(Int_t, Float_t);
   void fillHistograms(Int_t cut, Float_t weight);

   Int_t  Cut(Int_t entry);
   Int_t  GetEntry(Int_t entry);
   Int_t  LoadTree(Int_t entry);
   void   Init(TTree *tree);
   void   Loop(Int_t n=0);
   Bool_t Notify();
   void   Show(Int_t entry = -1);
    Float_t getEfficiencyFactor();
    // EFFICIENCY MATRICES
    TFile* pEleEffFile;
    TFile* pPosiEffFile;
    TH3F* p3DEffEle;
    TH3F* p3DEffPosi;

   //list of histograms
   TH1F* hmass_cut_pol[MAXCUT][MAXPOL]; //!
   TH1F* hmass_cut_pol_norm[MAXCUT][MAXPOL];

   TH1F* hmass_back0_cut[MAXCUT]; 
   TH1F* hmass_sig0_cut[MAXCUT];
   TH1F* hmass_back1_cut[MAXCUT]; 
   TH1F* hmass_sig1_cut[MAXCUT];

   TH1F* hmass_back0_cut_norm[MAXCUT]; 
   TH1F* hmass_sig0_cut_norm[MAXCUT];
   TH1F* hmass_back1_cut_norm[MAXCUT]; 
   TH1F* hmass_sig1_cut_norm[MAXCUT];


   TH1F* hoangle_cut_pol[MAXCUT][MAXPOL];
   TH1F* hoangle_cut_pol_norm[MAXCUT][MAXPOL];

   TH1F* hoangle_back0_cut[MAXCUT]; 
   TH1F* hoangle_sig0_cut[MAXCUT];
   TH1F* hoangle_back1_cut[MAXCUT]; 
   TH1F* hoangle_sig1_cut[MAXCUT];

   TH1F* hoangle_back0_cut_norm[MAXCUT]; 
   TH1F* hoangle_sig0_cut_norm[MAXCUT];
   TH1F* hoangle_back1_cut_norm[MAXCUT]; 
   TH1F* hoangle_sig1_cut_norm[MAXCUT];

   TH1F* hrap_cut_pol[MAXCUT][MAXPOL];
   TH1F* hrap_cut_pol_norm[MAXCUT][MAXPOL];

   TH1F* hrap_back0_cut[MAXCUT]; 
   TH1F* hrap_sig0_cut[MAXCUT];
   TH1F* hrap_back1_cut[MAXCUT]; 
   TH1F* hrap_sig1_cut[MAXCUT];

   TH1F* hrap_back0_cut_norm[MAXCUT]; 
   TH1F* hrap_sig0_cut_norm[MAXCUT];
   TH1F* hrap_back1_cut_norm[MAXCUT]; 
   TH1F* hrap_sig1_cut_norm[MAXCUT];

   TH1F* hpt_cut_pol[MAXCUT][MAXPOL];
   TH1F* hpt_cut_pol_norm[MAXCUT][MAXPOL];

   TH1F* hpt_back0_cut[MAXCUT]; 
   TH1F* hpt_sig0_cut[MAXCUT];
   TH1F* hpt_back1_cut[MAXCUT]; 
   TH1F* hpt_sig1_cut[MAXCUT];

   TH1F* hpt_back0_cut_norm[MAXCUT]; 
   TH1F* hpt_sig0_cut_norm[MAXCUT];
   TH1F* hpt_back1_cut_norm[MAXCUT]; 
   TH1F* hpt_sig1_cut_norm[MAXCUT];

   TH1F* hpolar_cut_pol[MAXCUT][MAXPOL];
   TH1F* hpolar_cut_pol_norm[MAXCUT][MAXPOL];
 
   TH1F* hpolar_back0_cut[MAXCUT];
   TH1F* hpolar_sig0_cut[MAXCUT];
   TH1F* hpolar_back1_cut[MAXCUT];
   TH1F* hpolar_sig1_cut[MAXCUT];

   TH1F* hpolar_back0_cut_norm[MAXCUT];
   TH1F* hpolar_sig0_cut_norm[MAXCUT];
   TH1F* hpolar_back1_cut_norm[MAXCUT];
   TH1F* hpolar_sig1_cut_norm[MAXCUT];

   Float_t polar;
   Float_t polarweight;

};

#endif // #ifdef pairs_cxx
