// Lagrange-Solmitz minimization procedure
// for HADES multi-channel kinematic refit
// (parametrization: momentum - theta - phi, beam isn't fitted)

// Author: Alexander Belyaev
// Dubna, JINR, Laboratory of High Energy
// 15 November 2006

#ifndef HLAGRANGESOLMITZFIT_H
#define HLAGRANGESOLMITZFIT_H

#include <TObject.h>

#define nameLength 6

#define nulla 0
#define minima 1
#define media 2
#define maxima 3
#define paranoja 4

#define max_t 8

class HLagrangeSolmitzFit : public TObject 
 {protected:

  private://-----------------------------------------------        

   void      constructio ();
   Double_t  sqr (Double_t x);
   
   void  fillGm1 (
//       FILE* otp
       );
   void  get_F (Int_t IFIT, Double_t* F, Int_t n, Double_t* x, Int_t* d, Double_t* m, 
               Int_t neutra, Double_t* En
//       ,FILE* otp
       );
   void  get_dF_dx  (Int_t IFIT, Double_t* dF_dx_4C, Double_t* dF_dx, Double_t* F, Int_t n, 
          Double_t* x, Int_t* d, Double_t* m, Int_t neutra, Double_t En);   
   void  get_y (Double_t* y, Double_t* F);
   void  get_dy_dx (Int_t n, Double_t* dF_dx_4C, Double_t* dy_dx, Int_t* d);
   
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

//   time_t tempus;
   
     Int_t   beamIndex;
     Int_t   trgtIndex;
     Int_t   neutra ;
     // --------------------
     
     Double_t beamPx;
     Double_t beamPy;
     Double_t beamPz;
     
//     Float_t beamP;
//     Float_t beamPhi;
//     Float_t beamTheta;
     
     Float_t q2lim4Cfit; // 100.0 in Samogon
     Float_t q2lim1Cfit; //  50.0 in Samogon
     Float_t eps2;
     Float_t eps3;
     Int_t   maxIterNmb;   //  50
   
   Int_t   retCode;

   //common /CNTRL/ in fortran version
   Int_t   IFIT;
   Int_t   IT;
   Float_t CHI_2;
   Float_t prob;
   Float_t dE;
   Float_t dMom[3];

   Double_t pi,pi2;
   
   Bool_t isBeamTarget;
   Int_t  nPrt; // number of particles used in fitting
   Int_t  nPar; // number of parametra used in fitting = nPrt*3
   Bool_t isNeutra;
   
 Double_t x          [3*(max_t + 1)*1];                //[3*(max_t + 1)][1];
 Double_t xM         [3*(max_t + 1)*1];                //[3*(max_t + 1)][1];
 Double_t Gm1        [(3*(max_t + 1))*(3*(max_t + 1))];//[3*(max_t + 1)][3*(max_t + 1)];
 Double_t Vm1        [(3*(max_t + 1))*(3*(max_t + 1))];//[3*(max_t + 1)][3*(max_t + 1)];
 Double_t y          [1*3];                            //[1][3];
 Double_t Um1        [3*3];                            //[3][3];
 Int_t    d          [max_t + 1];
 Double_t m          [max_t + 1];
 Int_t    digit      [max_t+1];
 
 Double_t covTmp     [3*3*(max_t+1)];                  //[3][3][max_t+1]
 Double_t rCode;
 Char_t     stroke [80];
 Char_t     stroke0[80];
   
  public:
   HLagrangeSolmitzFit();
//   HLagrangeSolmitzFit(Text_t *name,Text_t *title);
   ~HLagrangeSolmitzFit(void);

   Bool_t init  (void);
   Bool_t reinit(void);
   
   Bool_t   setBeamTargetParametra 
       (Int_t beamID, 
        Double_t beamPx, // GeV/c
        Double_t beamPy, // GeV/c 
        Double_t beamPz, // GeV/c
        Int_t targetID
//        ,FILE*
       );
   void     clearParticleSet (
//       FILE* otp
       );
   Bool_t   addChargedParticle 
       (Int_t pID, 
        Double_t p,     // GeV/c
        Double_t theta, // Radian
        Double_t phi,   // Radian
        Double_t* cov   // corresponds
//        ,FILE* otp
       );
   Bool_t   setNeutralParticle (Int_t pID
//       ,FILE* otp
       );
   Bool_t   clearNeutralParticle ();
   Bool_t   fit (
//       FILE* otp
       );
   Bool_t   isConvergence (
//       FILE* otp
       );
   Int_t    getRetCode ();
   Int_t    getIT ();
   Float_t  get_dE ();
   Bool_t   get_dMom (Float_t* dM);
   Float_t  getChi2 ();
   Float_t  getProb ();
   Double_t getChannelCode ();
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
   
   ClassDef(HLagrangeSolmitzFit,0) //Digitizes MDC data.
 };

#endif
