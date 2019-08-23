//*-- AUTHOR : Alexander Belyaev
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// Lagrange-Solmitz minimization procedure
// for HADES multi-channel kinematic refit
// (parametrization: momentum - theta - phi, beam isn't fitted)
//
// Author: Alexander Belyaev
// Dubna, JINR, Laboratory of High Energy
// 15 November 2006
////////////////////////////////////////////////////////////////////////

#include "hlagrangesolmitzfit.h"  
#include <TMath.h>

ClassImp(HLagrangeSolmitzFit) 

void HLagrangeSolmitzFit::constructio () 
{Int_t i,j; 
 pi   = TMath::Pi();
 pi2  = pi*2;
  
 maxIterNmb = 25;
 eps_L = 1.0e-2; //1.0e-4;
 eps_x = 1.0e-2; //1.0e-4;
 
 for (i = 0; i < max_t; i++)
   for (j = 0; j < max_t; j++) Gm1[max_t*i + j] = 0;
 
 isBeamTarget = kFALSE;
 nPrt         = 0;
 isNeutra     = kFALSE;
 retCode      = -1;
// sprintf (stroke0,"%s","");
// sprintf (stroke ,"%s","");
 
}

//------------------------------------------------------------------- 
HLagrangeSolmitzFit::HLagrangeSolmitzFit () 
 {constructio(); 
 }

HLagrangeSolmitzFit::~HLagrangeSolmitzFit() 
 {
 }

//--------------------------------------------------------------------------------
void HLagrangeSolmitzFit::setAddParametra (Int_t maxIN, Double_t e_L, Double_t e_x)
{maxIterNmb = maxIN;
 eps_x      = e_x;
 eps_L      = e_L;
}
 
Bool_t HLagrangeSolmitzFit::setBeamTargetParametra 
    (Int_t beamID, Double_t bPx, Double_t bPy, Double_t bPz, Int_t targetID)
{
// Char_t beamName[20],trgtName[20];
 if (HBPidPhysicsConstants::pid(beamID) != NULL)
  {beamIndex = beamID;
   if (HBPidPhysicsConstants::pid(targetID) != NULL)
    {trgtIndex = targetID;
//     sprintf (trgtName,"%s",HBPidPhysicsConstants::pid(trgtIndex));
//     if (strcoll(trgtName,"p") == 0) 
//      {sprintf (beamName,"%s",HBPidPhysicsConstants::pid(beamIndex));
       beamPx = bPx;
       beamPy = bPy;
       beamPz = bPz;
//       sprintf (stroke0,"%s%s%s%s%s",stroke0,beamName," ",trgtName,"  -> ");
//       sprintf (stroke ,"%s",stroke0);
       isBeamTarget = kTRUE;
       return         kTRUE;
//      }
//      else return kFALSE;
    }
    else return kFALSE;
  }
  else
   {printf ("there is no beam particle with pid = %i\n",beamIndex);
    return kFALSE;
   }
}

void HLagrangeSolmitzFit::clearParticleSet ()
{Int_t i,j;
 nPrt = 0; nPar = 0;
 for (i = 0; i < max_t; i++)
   for (j = 0; j < max_t; j++) Gm1[max_t*i + j] = 0;
 isNeutra = kFALSE;
 neutra   = 0;
 retCode  = -1;
// sprintf (stroke,"%s",stroke0);
}

Bool_t HLagrangeSolmitzFit::addChargedParticle 
    (Int_t pID, Double_t p, Double_t theta, Double_t phi, Double_t* cov)
{Int_t i,j,n;
 Float_t mass;
 Double_t k2;
// Char_t prtName[20];
 if (nPrt < max_t)
  if (p > 0.0)
   if (0 <= theta)
     {mass = HBPidPhysicsConstants::mass(pID)/1000.0;
      if (mass > 0)
       {
//        sprintf (prtName,"%s",HBPidPhysicsConstants::pid(pID));
//        strcat (stroke,prtName);
//        for (j = 1; j <= int(nameLength - strlen(prtName)); j++) strcat (stroke," ");
        i = nPrt;
        j = nPar;
        n = nPrt*9;
        xM[j] = 1.0/p; 
        k2 = sqr(xM[j]); 
        covTmp[n  ] = sqr(-k2)*cov[0]; 
        covTmp[n+1] =     -k2 *cov[1]; 
        covTmp[n+2] =     -k2 *cov[2];
        j++;
        xM[j] = theta;
        covTmp[n+3] =     -k2 *cov[3]; 
        covTmp[n+4] =          cov[4]; 
        covTmp[n+5] =          cov[5];
        j++;
        xM[j] = phi; 
        covTmp[n+6] =     -k2 *cov[6]; 
        covTmp[n+7] =          cov[7]; 
        covTmp[n+8] =          cov[8];
        j++;
        d[i] = 1;
        m[i] = mass;
        digit[i+1] = pID;
        i++;
        nPrt = i; nPar = j;
        return kTRUE;
       }
       else return kFALSE;
     }
    else return kFALSE;
   else return kFALSE;
  else return kFALSE;
}

void HLagrangeSolmitzFit::fillGm1 ()
{Int_t i,j,k,l,m,n;
 for (i = 0; i < nPar; i++)
   for (j = 0; j < nPar; j++) Gm1[nPar*i + j] = 0.0;
 j = 0; 
 i = 0;
 m = 0;
 for (l = 1; l <= nPrt; l++)
  {k = i*9;
   n = m*nPar+j;
   Gm1[n  ] = covTmp[k+0]; 
   Gm1[n+1] = covTmp[k+1]; 
   Gm1[n+2] = covTmp[k+2];
   j++;
   m++;
   n = m*nPar+j;
   Gm1[n-1] = covTmp[k+3]; 
   Gm1[n  ] = covTmp[k+4]; 
   Gm1[n+1] = covTmp[k+5];
   j++;
   m++;
   n = m*nPar+j;
   Gm1[n-2] = covTmp[k+6]; 
   Gm1[n-1] = covTmp[k+7]; 
   Gm1[n  ] = covTmp[k+8];
   j++;
   m++;
   i++;
  }   
}

Bool_t HLagrangeSolmitzFit::clearNeutralParticle ()
{isNeutra = kFALSE;
 neutra   = 0;
 return kTRUE;
}

Bool_t HLagrangeSolmitzFit::setNeutralParticle (Int_t pID)
{Double_t neutraMass;
 neutraMass = HBPidPhysicsConstants::mass(pID)/1000.0;
 if (neutraMass >= 0) 
  {neutra = pID;
   isNeutra = kTRUE;
   return kTRUE;
  }
  else return kFALSE;
}

#define maxL 4
Bool_t HLagrangeSolmitzFit::fit () //(Int_t maxIterNmb, Double_t eps_L, Double_t eps_x)
{Double_t dx         [3*(max_t + 1)*1];                //[3*(max_t + 1)][1];
 Double_t dx_T       [1*3*(max_t + 1)];                //[1][3*(max_t + 1)];
 Double_t dGm1       [(3*(max_t + 1))*(3*(max_t + 1))];//[3*(max_t + 1)][3*(max_t + 1)];
 Double_t dF_dx_4C   [3*(max_t + 1)*maxL];             //[3*(max_t + 1)][maxL]; //for 4C  fit
 Double_t dF_dx      [3*(max_t + 1)*maxL];             //[3*(max_t + 1)][maxL]; //for any fit
 Double_t dF_dx_old  [3*(max_t + 1)*maxL];             //[3*(max_t + 1)][maxL];
 Double_t ddF_dx     [3*(max_t + 1)*maxL];             //[3*(max_t + 1)][maxL];
 Double_t Psi        [3*(max_t + 1)*1];                //[3*(max_t + 1)][1];
 Double_t Psi_T      [1*3*(max_t + 1)];                //[1][3*(max_t + 1)];
 Double_t Psi_T_Gm1  [1*3*(max_t + 1)];                //[1][3*(max_t + 1)];
 Double_t E          [3*(max_t + 1)*maxL];             //[3*(max_t + 1)][maxL];
 Double_t E_Hm1      [3*(max_t + 1)*maxL];             //[3*(max_t + 1)][maxL];
 Double_t E_T        [maxL*3*(max_t + 1)];             //[maxL][3*(max_t + 1)];
 Double_t dy_dx      [3*(max_t + 1)*3];                //[3*(max_t + 1)][3];
 Double_t dy_dx_T    [3*3*(max_t + 1)];                //[3][3*(max_t + 1)];
 Double_t dy_dx_T_Vm1[3*3*(max_t + 1)];                //[3][3*(max_t + 1)];
 Double_t H          [maxL*maxL];                      //[maxL][maxL];
 Double_t Hm1        [maxL*maxL];                      //[maxL][maxL];
 Double_t lambda     [1*maxL];                         //[1][maxL];
 Double_t lambda_T   [maxL*1];                         //[maxL][1];
 Double_t lambda_T_H [maxL*1];                         //[maxL][1];
 Double_t F          [1*maxL];                         //[1][maxL];
 Double_t F_T        [maxL*1];                         //[maxL][1];
 Double_t F_Hm1      [1*maxL];                         //[1][maxL];
 Double_t dF         [1*maxL];                         //[1][maxL];
 Double_t b          [1*maxL];                         //[1][maxL];
 Double_t b_T        [maxL*1];                         //[maxL][1];
     
 Int_t    i,j,rc;
 Bool_t   convergence,loopPossible,twice,errorsOutOf;
 Double_t chi2[1*1];
 Double_t dM_L[1*1];
 Double_t dM_x[1*1];
 Double_t En; //energy of missing particle
// Char_t     prtName[20];
 if (isBeamTarget && (nPrt > 0))
  if ( (maxIterNmb > 1) &&
       (0.0 < eps_L) && (eps_L < 1.0) && (0.0 < eps_x) && (eps_x < 1.0)    )
   {retCode = 0;
    if (isNeutra) 
     {IFIT = 1; // IFIT = nC;
//      sprintf (prtName,"%s",HBPidPhysicsConstants::pid(neutra));
//      strcat (stroke,prtName);
//      for (j = 1; j <= int(nameLength - strlen(prtName)); j++) strcat (stroke," ");
     }
     else        
     {IFIT = 4;
//      for (j = 1; j <= int(nameLength); j++) strcat (stroke," ");
     }
    fillGm1 ();
    matrixToMatrix (nPar,1,xM,x);
    for (i = 0; i < IFIT; i++) lambda[i] = 0;
    get_F     (IFIT,F,nPrt,x,d,m,neutra,&En);
    get_dF_dx (IFIT,dF_dx_4C,dF_dx,F,nPrt,x,d,m,neutra,En);
    IT = 0;                                
    twice = false;                         
    convergence  = false; //until false    
    loopPossible = true;  //until false    
    while (loopPossible)
     {IT++;
      if (IT >= maxIterNmb) 
       {loopPossible = false; 
        retCode = 1;
       }
       else
        {matrixToMatrix (nPar,IFIT,dF_dx,dF_dx_old);
         matrixByMatrixMul (nPar,nPar,Gm1,nPar,IFIT,dF_dx,E);
         Transporto        (nPar,IFIT,E,E_T);
         matrixByMatrixMul (IFIT,nPar,E_T,nPar,IFIT,dF_dx,H);
         Reverto (IFIT,H,Hm1,&rc);
         if (rc != 0) 
          {loopPossible = false; 
           retCode = 2;
          }
          else
           {matrixMinusMatrix (nPar,1,xM,x,dx);
            Transporto (nPar,1,dx,dx_T);
            matrixByMatrixMul (1,nPar,dx_T,nPar,IFIT,dF_dx,dF);
            matrixPlusMatrix  (1,IFIT,F,dF,b);
            Transporto (1,IFIT,b,b_T);
            matrixByMatrixMul (IFIT,IFIT,Hm1,IFIT,1,b_T,lambda);
            matrixByMatrixMul (nPar,IFIT,E,IFIT,1,lambda,dx);
            matrixMinusMatrix (nPar,1,xM,dx,x);
            get_F (IFIT,F,nPrt,x,d,m,neutra,&En);
            get_dF_dx (IFIT,dF_dx_4C,dF_dx,F,nPrt,x,d,m,neutra,En);
            Transporto (1,IFIT,F,F_T);
            matrixByMatrixMul (1,IFIT,F,IFIT,IFIT,Hm1,F_Hm1);
            matrixByMatrixMul (1,IFIT,F_Hm1,IFIT,1,F_T,dM_L);
            if (dM_L[0] >= eps_L) twice = false;
             else
              {matrixMinusMatrix (nPar,IFIT,dF_dx,dF_dx_old,ddF_dx);
               matrixByMatrixMul (nPar,IFIT,ddF_dx,IFIT,1,lambda,Psi);
               Transporto (nPar,1,Psi,Psi_T);
               matrixByMatrixMul (1,nPar,Psi_T,nPar,nPar,Gm1,Psi_T_Gm1);
               matrixByMatrixMul (1,nPar,Psi_T_Gm1,nPar,1,Psi,dM_x);
               if (dM_x[0] >= eps_x) twice = false;
                else
                 if (!twice) twice = true;
                  else {loopPossible = false; convergence = true;} //OK!
              }
           }
        }
     }
    if (convergence)
     {//calculating fitted variables error matrix:
      matrixByMatrixMul (nPar,nPar,Gm1,nPar,IFIT,dF_dx,E);
      Transporto (nPar,IFIT,E,E_T);
      matrixByMatrixMul (nPar,IFIT,E,IFIT,IFIT,Hm1,E_Hm1);
      matrixByMatrixMul (nPar,IFIT,E_Hm1,IFIT,nPar,E_T,dGm1);
      matrixMinusMatrix (nPar,nPar,Gm1,dGm1,Vm1); //fitted variables error matrix
      errorsOutOf = false;
      i = 0;
      while ((i < nPar) && !errorsOutOf)
       {j = i*nPar + i;  
        if ((dGm1[j] <= 0) || (Gm1[j] <= dGm1[j])) errorsOutOf = true;
        i++;
       }
      if (!errorsOutOf)
       {//calculating the CHI_2:
        Transporto (IFIT,1,lambda,lambda_T);
        matrixByMatrixMul (1,IFIT,lambda_T,IFIT,IFIT,H,lambda_T_H);
        matrixByMatrixMul (1,IFIT,lambda_T_H,IFIT,1,lambda,chi2);
        CHI_2 = float(chi2[0]);
        //calculating the confidence level:
        prob = TMath::Prob(CHI_2,IFIT);
        if (IFIT == 1)
         {get_y (y,F);
          get_dy_dx (nPrt,dF_dx_4C,dy_dx,d);
          Transporto (nPar,3,dy_dx,dy_dx_T);
          matrixByMatrixMul (3,nPar,dy_dx_T,nPar,nPar,Vm1,dy_dx_T_Vm1);
          matrixByMatrixMul (3,nPar,dy_dx_T_Vm1,nPar,3,dy_dx,Um1); //calculated variables error matrix
         }
        //habemus fit: q2, x, Vm1, y, Um1
        dE = F[0]; //[1 -1];
        dMom[0] = 0; dMom[1] = 0; dMom[2] = 0;
        if (IFIT != 1) 
         {dMom[0] = F[1]; //[2 -1]; 
          dMom[1] = F[2]; //[3 -1]; 
          dMom[2] = F[3]; //[4 -1];
         }
        sort (nPrt,digit);
        
        rCode = chLM.getChannelCode (nPrt,digit,neutra);
//        rCode = 0;
//        for (i = 1; i <= nPrt; i++) rCode = rCode*codeMul + digit[i];
//        rCode = rCode*codeMul;
//        if (IFIT == 1) rCode = rCode + neutra;
        
       }
       else retCode = 3;
     }
    return kTRUE; // procedure call is OK, but retCode may be != 0
   }
  else return kFALSE;
 else return kFALSE;
}

Bool_t HLagrangeSolmitzFit::isConvergence ()
{if (retCode == 0)        return kTRUE;
  else if (retCode ==  1) return kFALSE;
  else if (retCode ==  2) return kFALSE;
  else if (retCode ==  3) return kFALSE;
  else if (retCode == -1) return kFALSE; // fit has not been called
  else                    return kFALSE;
}

Int_t HLagrangeSolmitzFit::getRetCode ()
{return retCode;
}

Int_t HLagrangeSolmitzFit::getIT ()
{if (retCode == 0) return IT;
  else             return -1;
}

Float_t HLagrangeSolmitzFit::get_dE ()
{if (retCode == 0) return dE;
  else             return -1.0;
}

Bool_t HLagrangeSolmitzFit::get_dMom (Float_t* dM)
{if (retCode == 0) 
  {dM[0] = dMom[0];
   dM[1] = dMom[1];
   dM[2] = dMom[2];
   return kTRUE;
  }
  else return kFALSE;
}

Float_t HLagrangeSolmitzFit::getChi2 ()
{if (retCode == 0) return CHI_2;
  else             return -1.0;
}

Float_t HLagrangeSolmitzFit::getProb ()
{if (retCode == 0) return prob;
  else             return -1.0;
}

Double_t HLagrangeSolmitzFit::getChannelCode ()
{if (retCode == 0) return rCode;
  else             return -1.0;
}

void HLagrangeSolmitzFit::get_y (Double_t* y, Double_t* F)
{y[0] = -F[1]; //[2-1];
 y[1] = -F[2]; //[3-1];
 y[2] = -F[3]; //[4-1];
}

Bool_t HLagrangeSolmitzFit::getFittedChargedParticle 
    (Int_t tNo, Double_t* p, Double_t* theta, Double_t* phi, Double_t* cov)
{Int_t j,n;
 Double_t p2;
 if (retCode == 0)
  {j = 3*tNo;
   *p = 1.0/x[j]; p2 = sqr(*p); 
   n = j*nPar + j;
   cov[0] = sqr(-p2)*Vm1[n  ]; cov[1] = -p2*Vm1[n+1]; cov[2] = -p2*Vm1[n+2];
   j++;
   *theta = x[j]; 
   n = j*nPar + j;
   cov[3] =     -p2 *Vm1[n-1]; cov[4] =     Vm1[n  ]; cov[5] =     Vm1[n+1];
   j++;
   *phi   = x[j]; 
   n = j*nPar + j;
   cov[6] =     -p2 *Vm1[n-2]; cov[7] =     Vm1[n-1]; cov[8] =     Vm1[n  ];
   return kTRUE;
  }
  else return kFALSE;
}

Bool_t HLagrangeSolmitzFit::getFittedNeutralParticle 
    (Double_t* p, Double_t* theta, Double_t* phi, Double_t* cov)
{Double_t J[9];
 Double_t cosT,sinT,pXY,cosP,sinP;
 if (retCode == 0)
  {*p = vectorModulo (3,y);
   cosT = y[2]/(*p); 
   *theta = acos(cosT);
   pXY  = vectorModulo (2,y);
   sinT = pXY/(*p);
   if (pXY > 1.0e-7)
    {cosP = y[0]/pXY;   sinP = y[1]/pXY;
     *phi  = asin(sinP);
     if       (y[0] < 0) *phi = pi  - (*phi);
      else if (y[1] < 0) *phi = pi2 + (*phi);
     J[0] =  y[0]/(*p);      J[1] = y[1]/(*p);      J[2] =  cosT;
     J[3] =  cosP*cosT/(*p); J[4] = sinP*cosT/(*p); J[5] = -sinT/(*p);
     J[6] = -sinP/pXY;       J[7] = cosP/pXY;       J[8] =  0;
     getErrorPropagation (J, Um1, cov);
    }
    else
    {*phi = 0.0;
     cov[0] = Um1[0]; cov[1] = 0.0;            cov[2] = 0.0;
     cov[3] = 0.0;    cov[4] = cov[0]/sqr(*p); cov[5] = 0.0;
     cov[6] = 0.0;    cov[7] = 0.0;            cov[8] = 0.0; // it's conventionality
    }
   return kTRUE;
  }
  else return kFALSE;
}

void HLagrangeSolmitzFit::get_dy_dx (Int_t nPrt, Double_t* dF_dx_4C, Double_t* dy_dx, Int_t* d)
{Int_t i,j;
 for (i = 0; i <= nPrt; i++)
  {j = 3*i;
//parametrisation independent calculations:
   dy_dx[j*3 + 0] = -dF_dx_4C[j*4 + 1]; //[j*4 + 2-1]; 
   dy_dx[j*3 + 1] = -dF_dx_4C[j*4 + 2]; //[j*4 + 3-1]; 
   dy_dx[j*3 + 2] = -dF_dx_4C[j*4 + 3]; //[j*4 + 4-1];
   j++;
   dy_dx[j*3 + 0] = -dF_dx_4C[j*4 + 1]; //[j*4 + 2-1]; 
   dy_dx[j*3 + 1] = -dF_dx_4C[j*4 + 2]; //[j*4 + 3-1]; 
   dy_dx[j*3 + 2] = -dF_dx_4C[j*4 + 3]; //[j*4 + 4-1];
   j++;
   dy_dx[j*3 + 0] = -dF_dx_4C[j*4 + 1]; //[j*4 + 2-1]; 
   dy_dx[j*3 + 1] = -dF_dx_4C[j*4 + 2]; //[j*4 + 3-1]; 
   dy_dx[j*3 + 2] = -dF_dx_4C[j*4 + 3]; //[j*4 + 4-1];
  }
}

void HLagrangeSolmitzFit::get_dF_dx 
 (Int_t IFIT, Double_t* dF_dx_4C, Double_t* dF_dx, Double_t* F, Int_t nPrt, Double_t* x, Int_t* d, 
  Double_t* m, Int_t neutra, Double_t En)
{Int_t i,j;
 Double_t sinT,cosT,sinP,cosP,x2;
    for (i = 0; i < nPrt; i++) // i - number of fitted particles
     {sinP = sin(x[i*3 + 2]);
      cosP = cos(x[i*3 + 2]);
      sinT = sin(x[i*3 + 1]);
      cosT = cos(x[i*3 + 1]);
      x2   = sqr(x[i*3    ]);
      // j - number of line of matrix
      j = i*3 + 0; // 1st parameter (d.../d(1/p))
      dF_dx_4C[j*4 + 0] = -d[i]/(x2*sqrt(1.0 + sqr(m[i]*x[i*3]))); // [j*4 + 1 - 1]
      dF_dx_4C[j*4 + 1] = -d[i]*cosP*sinT/x2;                      // [j*4 + 2 - 1]
      dF_dx_4C[j*4 + 2] = -d[i]*sinP*sinT/x2;                      // [j*4 + 3 - 1]
      dF_dx_4C[j*4 + 3] = -d[i]*     cosT/x2;                      // [j*4 + 4 - 1]
      j++;         // 2nd parameter (d.../dtheta)
      dF_dx_4C[j*4 + 0] =  0;                                   // [j*4 + 1 - 1]
      dF_dx_4C[j*4 + 1] =  d[i]*cosP*cosT/x[3*i];               // [j*4 + 2 - 1]
      dF_dx_4C[j*4 + 2] =  d[i]*sinP*cosT/x[3*i];               // [j*4 + 3 - 1]
      dF_dx_4C[j*4 + 3] = -d[i]*     sinT/x[3*i];               // [j*4 + 4 - 1]
      j++;         // 3rd parameter (d.../dphi)
      dF_dx_4C[j*4 + 0] =  0;                                   // [j*4 + 1 - 1]
      dF_dx_4C[j*4 + 1] = -d[i]*sinP*sinT/x[3*i];               // [j*4 + 2 - 1]
      dF_dx_4C[j*4 + 2] =  d[i]*cosP*sinT/x[3*i];               // [j*4 + 3 - 1]
      dF_dx_4C[j*4 + 3] =  0;                                   // [j*4 + 4 - 1]
     }
 // the rest of procedure should not be changed:    
 if (IFIT == 1) 
  for (i = 0; i < nPrt; i++)
   {j = i*3 + 0; 
    dF_dx[j + 0] = dF_dx_4C[j*4 + 0] +        // 1 - 1
             (F[1]*dF_dx_4C[j*4 + 1] +        // 2 - 1
              F[2]*dF_dx_4C[j*4 + 2] +        // 3 - 1
              F[3]*dF_dx_4C[j*4 + 3])/En;     // 4 - 1
    j++;         
    dF_dx[j + 0] = dF_dx_4C[j*4 + 0] +        // 1 - 1
             (F[1]*dF_dx_4C[j*4 + 1] +        // 2 - 1
              F[2]*dF_dx_4C[j*4 + 2] +        // 3 - 1
              F[3]*dF_dx_4C[j*4 + 3])/En;     // 4 - 1
    j++;         
    dF_dx[j + 0] = dF_dx_4C[j*4 + 0] +        // 1 - 1
             (F[1]*dF_dx_4C[j*4 + 1] +        // 2 - 1
              F[2]*dF_dx_4C[j*4 + 2] +        // 3 - 1
              F[3]*dF_dx_4C[j*4 + 3])/En;     // 4 - 1
   }
  else 
   for (i = 0; i < nPrt; i++)
    {j = i*3 + 0; 
     dF_dx[j*4 + 0] = dF_dx_4C[j*4 + 0]; // 1 - 1
     dF_dx[j*4 + 1] = dF_dx_4C[j*4 + 1]; // 2 - 1
     dF_dx[j*4 + 2] = dF_dx_4C[j*4 + 2]; // 3 - 1
     dF_dx[j*4 + 3] = dF_dx_4C[j*4 + 3]; // 4 - 1
     j++;         
     dF_dx[j*4 + 0] = dF_dx_4C[j*4 + 0]; // 1 - 1
     dF_dx[j*4 + 1] = dF_dx_4C[j*4 + 1]; // 2 - 1
     dF_dx[j*4 + 2] = dF_dx_4C[j*4 + 2]; // 3 - 1
     dF_dx[j*4 + 3] = dF_dx_4C[j*4 + 3]; // 4 - 1
     j++;         
     dF_dx[j*4 + 0] = dF_dx_4C[j*4 + 0]; // 1 - 1
     dF_dx[j*4 + 1] = dF_dx_4C[j*4 + 1]; // 2 - 1
     dF_dx[j*4 + 2] = dF_dx_4C[j*4 + 2]; // 3 - 1
     dF_dx[j*4 + 3] = dF_dx_4C[j*4 + 3]; // 4 - 1
    }
}

void HLagrangeSolmitzFit::get_F (Int_t IFIT, Double_t* F, Int_t nPrt, Double_t* x, Int_t* d, 
      Double_t* m, Int_t neutra, Double_t* En)
{Int_t i;
 Double_t sinT,cosT;
 
 Double_t p,p2;
 Float_t  m2,m1;
 F[0] = 0; // 1 - 1
 F[1] = 0; // 2 - 1
 F[2] = 0; // 3 - 1
 F[3] = 0; // 4 - 1
 for (i = 0; i < nPrt; i++)
  {F[0] = F[0] + d[i]*sqrt(sqr(1.0/x[3*i]) + sqr(m[i])); // 1 - 1
   sinT = sin(x[3*i + 1]);
   cosT = cos(x[3*i + 1]);
   F[1] = F[1] + d[i]*cos(x[3*i + 2])*sinT/x[3*i + 0];   // 2 - 1
   F[2] = F[2] + d[i]*sin(x[3*i + 2])*sinT/x[3*i + 0];   // 3 - 1
   F[3] = F[3] + d[i]*                cosT/x[3*i + 0];   // 4 - 1
  }
  p  = sqrt(sqr(beamPx) + sqr(beamPy) + sqr(beamPz));
  p2 = sqr(p);
  m1 = HBPidPhysicsConstants::mass(beamIndex);
  m1 = m1/1000.0;
  m2 = sqr(m1);
  F[0] = F[0] - sqrt(p2 + m2); // 1 - 1
  F[1] = F[1] - beamPx;        // 2 - 1
  F[2] = F[2] - beamPy;        // 3 - 1
  F[3] = F[3] - beamPz;        // 4 - 1
 // the rest of procedure should not be changed:
 m1 = HBPidPhysicsConstants::mass(trgtIndex)/1000.0;
 F[0] = F[0] - m1;                     // 1 - 1
 if (IFIT == 1)  
  {m1 = HBPidPhysicsConstants::mass(neutra)/1000.0;
   *En = sqrt(sqr(F[1]) +              // 2 - 1
              sqr(F[2]) +              // 3 - 1
              sqr(F[3]) + sqr(m1));    // 4 - 1
   F[0] = F[0] + *En;                  // 1 - 1
  }
}

void HLagrangeSolmitzFit::getErrorPropagation (Double_t* J, Double_t* cov, Double_t* COV)
{Double_t J_T[3*3],J_cov[3*3];
 Transporto        (3,3,J,J_T);
 matrixByMatrixMul (3,3,J    ,3,3,cov,J_cov);
 matrixByMatrixMul (3,3,J_cov,3,3,J_T,COV);
}

// void HLagrangeSolmitzFit::printMatrix (FILE* otp, Char_t* name, Int_t nLin, Int_t nCol, Double_t *P)
//  {Int_t i,j,n;
//   fprintf (otp,"%s (%i,%i)\n",name,nLin,nCol);
//   for (i = 0; i < nLin; i++)
//    {for (j = 0; j < nCol; j++)
//      {n = i*nCol + j;
//       if (P[n] == 0) fprintf (otp,"  0       ");
//        else if ( (fabs(P[n]) >= 100.0) || (fabs(P[n]) < 1.0e-6) ) fprintf (otp,"%10.2e",P[n]);
//              else fprintf (otp,"%10.6f",P[n]);
//      }
//     fprintf (otp,"\n");
//    }
//  }

Double_t HLagrangeSolmitzFit::scalarMul (Int_t dim, Double_t* A, Double_t* B)
 {Int_t i;
  Double_t scalar;
  scalar = 0;
  for (i = 0; i < dim; i++) scalar = scalar + A[i]*B[i];
  return scalar;
 } 
 
Double_t HLagrangeSolmitzFit::vectorModulo (Int_t dim, Double_t* vector)
 {Double_t vm;
  Int_t i;
  vm = 0;
  for (i = 0; i < dim; i++) vm = vm + vector[i]*vector[i];
  vm = sqrt(vm);  
  return vm;  
 } 

void HLagrangeSolmitzFit::vectorPlusVector (Int_t dim, Double_t *V1, Double_t *V2, Double_t *V)
{Int_t i;
 for (i = 0; i < dim; i++) V[i] = V1[i] + V2[i];
}
 
void HLagrangeSolmitzFit::vectorMinusVector (Int_t dim, Double_t *V1, Double_t *V2, Double_t *V)
{Int_t i;
 for (i = 0; i < dim; i++) V[i] = V1[i] - V2[i];
}
 
void HLagrangeSolmitzFit::matrixByVectorMul (Int_t n, Int_t dim, Double_t *M, Double_t *V, Double_t *A)
{Int_t i,j;
 Double_t t;
 for (i = 0; i < n; i++)
 {t = 0;
  for (j = 0; j < n; j++) t = t + M[i*dim + j]*V[j];
  A[i] = t;  
 }
}

void HLagrangeSolmitzFit::Gauss 
(Int_t n, Double_t *A, Double_t *b, Double_t *x, Int_t *rc)
 {Bool_t singular,enough;
  Int_t i,j,k,nMax;
  Double_t t;
  const Double_t accuracy = 1.0e-15;
  if (n <= 0) *rc = 6;
  else
  {singular = false;
   if (n > 1)
    {i = 0;
     while ( (i < n-1) && !singular)
      {nMax = i; t = 0;
       for (j = i; j < n; j++)
         if (fabs(A[j*n + i]) > t) {nMax = j; t = fabs(A[j*n + i]); }
       if (nMax != i)
        {for (k = i; k < n; k++)
          {t = A[i*n + k]; A[i*n + k] = A[nMax*n + k]; A[nMax*n + k] = t;}
         t = b[i]; b[i] = b[nMax]; b[nMax] = t;
        }
       if (fabs(A[i*n + i]) < accuracy) singular = true; 
       else
       {j = i + 1;
        while (j < n)
         {if (A[j*n + i] != 0)
           {t = A[j*n + i]/A[i*n + i];
            for (k = i; k < n; k++) A[j*n + k] = A[j*n + k] - A[i*n + k]*t;  // +1
            b[j] = b[j] - b[i]*t;
           }
          j++;
         }
       }
       i++;
      }
    }
   if (singular) *rc = 3;
    
   else
   {*rc = 0; enough = false; i = n - 1;
    while (!enough)
     {t = 0;
      if (i != n - 1)
        for (j = i+1; j < n; j++) t = t + A[i*n + j]*x[j];
      if (fabs(A[i*n + i]) < accuracy) {*rc = 3; enough = true;}
      else
       {x[i] = (b[i] - t)/A[i*n + i];
        if (i > 0) i--;
        else enough = true;
       }
     }
   }
  }
 } 
 
#define maxDim 36
void HLagrangeSolmitzFit::Reverto (Int_t n, Double_t *J, Double_t *Jm1, Int_t *rc)
{Int_t i,j,k,l;
 Double_t unum[maxDim],Jcur[maxDim*maxDim];
 Double_t x[maxDim];
 if (n > maxDim)
  {printf ("Reverto: dimension %i > maxDim = %i\n",n,maxDim); abort ();}
 i = 0; *rc = 0;
 while ( (i < n) && (*rc == 0) )
  {for (j = 0; j < n; j++) unum[j] = 0;
   unum[i] = 1.0;
   for (k = 0; k < n; k++)
    for (l = 0; l < n; l++)
      Jcur[k*n + l] = J[k*n + l]; 
   Gauss (n,Jcur,unum,x,rc);
   for (j = 0; j < n; j++) Jm1[j*n + i] = x[j];
   i++;
  }
}
    
void HLagrangeSolmitzFit::Transporto (Int_t nLinA, Int_t nColA, Double_t* A, Double_t* U)
 {Int_t    i,j;
  for (i = 0; i < nLinA; i++)
    for (j = 0; j < nColA; j++)
      U[j*nLinA + i] = A[i*nColA + j];
 }
 
void HLagrangeSolmitzFit::matrixByMatrixMul
(Int_t nLinA, Int_t nColA, Double_t *A, Int_t nLinB, Int_t nColB, Double_t *B, Double_t *U)
 {Int_t i,j,k;
  Double_t t;
  if (nColA != nLinB) 
    {printf ("matrixByMatrixMul: dimension violation: nColA %i != nLinB %i\n",nColA,nLinB); abort();}
  for (j = 0; j < nColB; j++)
    for (i = 0; i < nLinA; i++)
     {t = 0;
      for (k = 0; k < nColA; k++) t = t + A[i*nColA  + k]*B[k*nColB  + j];
      U[i*nColB  + j] = t;
     }
 }
 
void HLagrangeSolmitzFit::matrixMinusMatrix
(Int_t nLin, Int_t nCol, Double_t *A, Double_t *B, Double_t *U)
 {Int_t i,j;
  for (i = 0; i < nLin; i++)
    for (j = 0; j < nCol; j++)
     U[i*nCol + j] = A[i*nCol + j] - B[i*nCol + j];
 }
 
void HLagrangeSolmitzFit::matrixPlusMatrix
(Int_t nLin, Int_t nCol, Double_t *A, Double_t *B, Double_t *U)
 {Int_t i,j;
  for (i = 0; i < nLin; i++)
    for (j = 0; j < nCol; j++)
     U[i*nCol + j] = A[i*nCol + j] + B[i*nCol + j];
 }
 
void HLagrangeSolmitzFit::matrixToMatrix
(Int_t nLin, Int_t nCol, Double_t *A, Double_t *U)
 {Int_t i,j;
  for (i = 0; i < nLin; i++)
    for (j = 0; j < nCol; j++)
     U[i*nCol + j] = A[i*nCol + j];
 }


void HLagrangeSolmitzFit::sort (Int_t n, Int_t* a)
{Int_t i,j,t;
 for (i = n-1;i >= 1; i--)
   for (j = 1; j <= i; j++)
     if (a[j] > a[j+1]) {t = a[j]; a[j] = a[j+1]; a[j+1] = t;}
}

Double_t HLagrangeSolmitzFit::sqr (Double_t x)
{return x*x;
}

/*
Ctrl/Mouse - select text block
Ctrl/9     - shift  text block left
Ctrl/0     - shift  text block right
Ctrl/7     - comment selected lines ON
Ctrl/8     - comment selected lines OFF
*/
