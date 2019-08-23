//*-- AUTHORs:   Alexander Belyaev, Ingo Froelich
//*-- Modified :

#ifndef HHypMultiChannelFiller_H
#define HHypMultiChannelFiller_H

#include "hreconstructor.h"
#include "hlocation.h"
#include "hhyplist.h"
#include "TArrayI.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"

#include "hcategory.h"
#include "hdebug.h"
#include "hades.h"
#include "hypinfodef.h"
//#include "hpidphysicsconstants.h"
 
//=============================
#include "hchannellistmaker.h" // -> BPidPhysicsConstants -> hBPidPhysicsConstants
#include <time.h> 
//=============================

#include <cmath>
#include <iostream>

//=====================
class HCategory;
class HGeomTransform;
class HIterator; 

//=====================

class           HHypMultiChannelFiller:public HReconstructor
{
  enum tContent   {pressed,notPressed};
//  enum            {nameLngt = 6};
 enum {chNmbMax    =  200};
//  enum            {maxT = 6};
  
public:
  HHypMultiChannelFiller();
  HHypMultiChannelFiller(const Text_t * name,const Text_t * title);
  ~HHypMultiChannelFiller();

  Bool_t          SetExitList(Int_t e_list);
  
   void     setBeamPID (Int_t pid);
   void     setTargetPID (Int_t pid);
   void     setBeam (TLorentzVector* beamVector);

  Bool_t    init (void);       
  Bool_t  reinit (void);       
  Int_t   execute(void);    
  Bool_t  finalize(void);   

  void            setContCatList(HCategory * pContCat)
  {
    m_pContCatList = pContCat;
  };
  void            setContCatComb(HCategory * pContCat)
  {
    m_pContCatComb = pContCat;
  };
  void            setNPlus(Int_t np)
  {
    nPlus = np;
  };
  void            setNMinus(Int_t nm)
  {
    nMinus = nm;
  };

private:
    
//===================================    

//   HEventHeader*     evHdr;
//   Int_t eventRunNumber;
//   Int_t eventSeqNumber;
  Bool_t printFlag;
    
  HChannelListMaker chLM;
  
  HIterator*       iterTrack;

//--------------------------------------------------------       
   struct tTrack
    {Int_t       chargeSign;
     Bool_t      isRICH;
     Int_t       trID;
     Int_t       nh;
     Int_t       pid[max_h+1]; // max_h see in hHypBljvPidList
    };
     Int_t       nt;
     tTrack      T[max_t+1]; // max_t see in hHypBljvPidList
    
   struct tChain  
    {tChain* next;
     Int_t   pidCharged[max_t+1];
     Int_t   tidCharged[max_t+1];
     Int_t   pidMissing;
    };
   tChain* pChainBegin;
   tChain* pChainEnd;

//++++++++++++++++++++++++ 
  Int_t counterMinus;
  Int_t counterPlus;
  Int_t counter;
  Int_t Charge[10];
//+++++++++++++++++++++++++  
   
  //channelListMaker produces:
  Int_t     pNmb;        //number of positive particles in use
  Int_t     posPID[max_particles];
  Int_t     nNmb;        //number of negative particles in use
  Int_t     negPID[max_particles];
  Int_t     tNmb;        //number of total    particles in use
  Int_t     totPID[max_particles];
  Int_t     chNmb;       // number of channels produced
  Double_t  chCodeList[chNmbMax];
  TString   chLineList[chNmbMax];
  
   FILE*    otp; //for accompanying text 

   TString  inDir;
   TString  inFile;
   TString  inFileName;
   
   time_t   tempus;
   
   TLorentzVector* beam;         //! Pointer to the total CM energy for missing mass calculations
   Int_t    beamIndex;
   Int_t    trgtIndex;
   Float_t  Ek;
   Float_t  EsLim;  // GeV,   limit system energy   in Lab
   Float_t  Es;     // GeV,   mean  system energy   in Lab 
                    // = sqrt(sqr(mBeam) + sqr(pBeam)) + mTarget, where mBeam - mean mom beam value
   Float_t  beamMomIdeal;
   Float_t  beamDp;

   Int_t   evFirstNo;  // printing 1st events No
   Int_t   evLastNo;  // printing last events No
   
   Float_t pi;
   Float_t pi2;
   Float_t pi_2;
   Float_t gradDivRad;
   Float_t thetaMin;
   Float_t minMom; // GeV/c
   Float_t maxVelo; //1.2; // // maximum for calculated speed of light
   Float_t maxMeasuredMomentum;
   Float_t theLimErr;
   Float_t phiLimErr;
   
   Int_t   trckInputNo; //number of input tracks
   Int_t   overflowTrNo;
   Int_t   trNo;    // full number of all accepted tracks in all accepted events
   
   Int_t   evInputNo;
   Int_t   badRecsNmb; //number of empty or overflowed events
   Int_t   delPhiTrNo;
   Int_t   delThetaTrNo;
   Int_t   phiOutOfAcceptanceTrNo;
   Int_t   thetaOutOfAcceptanceTrNo;
   Int_t   noPolarityTrNo;
   Int_t   wrongVeloTrNo;
   Int_t   wrongMomTrNo;
   
   Int_t   evNo;    // full number of all accepteded events
   
   Int_t   noCombinations;
   
   Int_t   proFit;

   Float_t  dm;   // GeV, so (neutron.mass - proton.mass) ~ 0.001294
   Float_t  mBetweenMuAndE;
   
   Int_t   electron,positron,piMinus,piPlus,muMinus,muPlus,neutrino;
   Int_t   pi0,proton;
 
   Int_t   chargeIn,leptonIn,baryonIn; //,maximumTracks;
   Int_t   electronIn,muonIn;
   Float_t ECMS;
     
   tContent tableContentType;

//===================================
    
  Int_t exitList;
  Int_t           nPlus, nMinus, // Ingo: these variables now not used. Are its need?
                  numberOfParticles;     // Total Number of Particles

  Int_t           numberOfCombinations, 
                  numberOfNegCombinations,  // Ingo: what is the purpose of it?
                  numberOfPosCombinations;  // Ingo: what is the purpose of it?
  Int_t         **pid_real_array;       // only real particles
  Int_t         **tid_real_array;       // only real track id's // <---- Ingo!
  Int_t          *missing_array;        // missing particles

  TArrayI         particleArrayPlus, particleArrayMinus;
  HCategory      *m_pContCatList;       //!Pointer to the hit data category HHypList
  HCategory      *m_pContCatComb;       //!Pointer to the hit data category HHypComb
  HCategory      *m_pContCatPart;       //!Pointer to the hit data category HPidPArticle
  HIterator      *m_pContItPart;        //!Pointer to the hit data category HPidPArticle

  void            create_pid_table(void);       // create permutations -> pid_full_array

  void            fill_pid_table(HHypComb * hypcomb); // copy pid_real_array to hypcomb
//   void            fill_pid_idx  (HHypComb * hypcomb); // fill pid idx into hypcomb // <--- Ingo!
  Bool_t          fill_pid_fprob(HHypComb * hypcomb, Int_t * numpidtracks);     // fill pid fprob into hypcomb

  //Alexander: add helper function as needed
//================================================== 
   Bool_t   existSuchChannel         (Double_t chC);
   void     printComb                (tChain* pChain, Int_t prong, Int_t* trackNo,Int_t* partIndex, Int_t neutra);
   void     getCombination           (Int_t prong, Int_t* trackNo,Int_t* partIndex, Int_t neutra );
//    Int_t    getChNo                  (Double_t chCode); // not used
   void     sortParticleCombinations (Int_t prong, Int_t* trackNo, Int_t chargeOut);
   void     sortTrackSubSets         (Int_t nt, Int_t prong, Int_t* trackNo, Bool_t* overflow);
   void     multiChannelCombinations ();
//-----------------------------------------------        
   void     getECMS    ();
   void     getSysPar  ();
   void     prologue   (void);
   void     printParticlePropertyTable (FILE* otp); //, Int_t nParticles);
   Bool_t   intro      (void);
   
   void     printMatrix (FILE* otp,const Char_t* name, Int_t nLin, Int_t nCol, Double_t *P);
   void     print_cov (Double_t* cov);
   void     storeHypothesis      (Int_t tNo, Int_t pNmb);
   void     storeSignHypotheses  (Int_t Num, Int_t* PID, Int_t tNo,
                                  Bool_t isRICH, Float_t massEstim, Int_t lepton);
   void     getPidTrackCandEvent (Bool_t* overflow);
   Double_t sqr    (Double_t x);
   void     sort   (Int_t n, Int_t* a);
   void     report ();

   Bool_t   IngoInit     (); // <--- Ingo: your sample is used here
   Int_t    IngoExecute  (); // <--- Ingo: your sample is used here
   void     IngoFinalize (); // <--- Ingo: your sample is used here
//===========================================

  ClassDef(HHypMultiChannelFiller, 0)   // Fills the HypMultiChannel with all possible particle combinations
};

#endif
