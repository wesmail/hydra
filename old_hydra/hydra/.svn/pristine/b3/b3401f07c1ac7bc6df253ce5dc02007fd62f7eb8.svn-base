//*-- AUTHOR : Alexander Belyaev
//
// Lagrange-Solmitz minimization procedure
// for HADES multi-channel kinematic refit
// (parametrization: 1/momentum - theta - phi, beam: 1/momentum - alpha - beta)
//
// Author: Alexander Belyaev
// Dubna, JINR, Laboratory of High Energy
// 15 November 2006 (without beam fitting)
// 12 May      2008 (with    beam fitting, when isBeamFitter == kTRUE)
//
////////////////////////////////////////////////////////////////////////

#ifndef HLAGRANGESOLMITZBEAMFIT_H
#define HLAGRANGESOLMITZBEAMFIT_H

#include <TObject.h>
// uses chLM.getChannelCode
#include "hchannellistmaker.h" // -> hHypBljvPidList -> BPidPhysicsConstants

class HLagrangeSolmitzBeamFit : public TObject 
 {protected:

  private://-----------------------------------------------

   void      constructio ();
   Double_t  sqr (Double_t x);
   
   void  fillGm1 ();
   Bool_t completePreparation ();
   void  get_F (Int_t IFIT, Double_t* F, Int_t n, Double_t* x, //Int_t* d,
			Double_t* m, Int_t neutra, Double_t* En);
   void  get_dF_dx  (Int_t IFIT, Double_t* dF_dx_4C, Double_t* dF_dx, Double_t* F, Int_t n, Double_t* x, //Int_t* d,
 			Double_t* m, Int_t neutra, Double_t En);   
   void  get_y (Double_t* y, Double_t* F);
   void  get_dy_dx (Int_t n, Double_t* dF_dx_4C, Double_t* dy_dx);//, Int_t* d);
   
   void  getErrorPropagation (Double_t* J, Double_t* cov, Double_t* COV);

   void  printMatrix (FILE* otp, Char_t* name, Int_t nLin, Int_t nCol, Double_t *P);
   Double_t scalarMul (Int_t dim, Double_t* A, Double_t* B);
   Double_t vectorModulo (Int_t dim, Double_t* vector);
   void  vectorPlusVector  (Int_t dim, Double_t* V1, Double_t* V2, Double_t* V);
   void  vectorMinusVector (Int_t dim, Double_t* V1, Double_t* V2, Double_t* V);
   void  matrixByVectorMul (Int_t n, Int_t dim, Double_t *M, Double_t *V, Double_t *A);
   
   void Gauss (Int_t n, Double_t *A, Double_t *b, Double_t *x, Int_t *rc);
   void Reverto (Int_t n, Double_t *J, Double_t *Jm1, Int_t *rc);
   void Transporto (Int_t nLinA, Int_t nColA, Double_t* A, Double_t* U);
   void matrixByMatrixMul
         (Int_t nLinA, Int_t nColA, Double_t *A, Int_t nLinB, Int_t nColB, Double_t *B, Double_t *U);
   void matrixMinusMatrix (Int_t nLin, Int_t nCol, Double_t *A, Double_t *B, Double_t *U);
   void matrixPlusMatrix  (Int_t nLin, Int_t nCol, Double_t *A, Double_t *B, Double_t *U);
   void matrixToMatrix    (Int_t nLin, Int_t nCol, Double_t *A, Double_t *U);

   void  sort       (Int_t n, Int_t* a);

     Int_t   beamIndex;
     Int_t   trgtIndex;
     Int_t   neutra ;
     // --------------------
     
     Double_t beamP,beamDp;
     Double_t beamA,beamDa;
     Double_t beamB,beamDb;
     
     Float_t eps2;
     Float_t eps3;
   
   Int_t   retCode;

   //common /CNTRL/ in fortran version
   Int_t   IFIT;
   Int_t   IT;
   Float_t CHI_2;
   Float_t prob;
   Float_t dE;
   Float_t dMom[3];

   Double_t pi,pi2;
   
   Bool_t isBeamTarget,isBeamFitted;
   Int_t  nPrt; // number of particles used in fitting
   Int_t  nPar; // number of parametra used in fitting = nPrt*3
   Bool_t isNeutra;
   
   HChannelListMaker chLM;
   
 Int_t    maxIterNmb;
 Double_t eps_L, eps_x;
 
 Float_t mb,mt,mMiss,beamPx,beamPy,beamPz,beamE;
  
 Double_t x          [3*(max_t + 1)*1];                //[3*(max_t + 1)][1];
 Double_t xM         [3*(max_t + 1)*1];                //[3*(max_t + 1)][1];
 Double_t Gm1        [(3*(max_t + 1))*(3*(max_t + 1))];//[3*(max_t + 1)][3*(max_t + 1)];
 Double_t Vm1        [(3*(max_t + 1))*(3*(max_t + 1))];//[3*(max_t + 1)][3*(max_t + 1)];
 Double_t y          [1*3];                            //[1][3];
 Double_t Um1        [3*3];                            //[3][3];

 Double_t m          [max_t + 1];
 Int_t    digit      [max_t+1];
 
 Double_t covTmp     [3*3*(max_t+1)];                  //[3][3][max_t+1]
 Double_t rCode;
// Char_t     stroke [80];
// Char_t     stroke0[80];
   
  public: // ----------------------------------------------------------
   HLagrangeSolmitzBeamFit();
   ~HLagrangeSolmitzBeamFit(void);

   void     setAddParametra (Int_t maxIN, Double_t e_L, Double_t e_x);
   Bool_t   setBeamTargetParametra 
       (Int_t beamID, 
        Double_t beamPx, // GeV/c
        Double_t beamPy, // GeV/c 
        Double_t beamPz, // GeV/c
        Int_t targetID);
   Bool_t   setBeamTargetParametra 
    (Int_t beamID, Double_t bP, Double_t bA, Double_t bB,
		Double_t bDp, Double_t bDa, Double_t bDb,
		Bool_t beamIsFitted,Int_t targetID);
   void     clearParticleSet ();
   Bool_t   addChargedParticle 
       (Int_t pID, 
        Double_t p,     // GeV/c
        Double_t theta, // Radian
        Double_t phi,   // Radian
        Double_t* cov   // corresponds
       );
   Bool_t   setNeutralParticle (Int_t pID);
   Bool_t   clearNeutralParticle ();
   Bool_t   fit ();
   Bool_t   isConvergence ();
   Int_t    getRetCode ();
   Int_t    getIT ();
   Float_t  get_dE ();
   Bool_t   get_dMom (Float_t* dM);
   Float_t  getChi2 ();
   Float_t  getProb ();
   Double_t getChannelCode ();
   Bool_t   getFittedBeam
       (Double_t* p,     // GeV/c
        Double_t* alpha, // Radians
        Double_t* beta,  // Radians
        Double_t* cov);  // corresponds
   Bool_t   getFittedChargedParticle 
       (Int_t tNo, 
        Double_t* p,     // GeV/c
        Double_t* theta, // Radians
        Double_t* phi,   // Radians
        Double_t* cov);  // corresponds
   Bool_t   getFittedNeutralParticle 
       (Double_t* p,     // GeV/c
        Double_t* theta, // Radians
        Double_t* phi,   // Radians
        Double_t* cov    // corresponds
       );
   
   ClassDef(HLagrangeSolmitzBeamFit,0) // another Kinematic Refit Approach
 };

#endif
