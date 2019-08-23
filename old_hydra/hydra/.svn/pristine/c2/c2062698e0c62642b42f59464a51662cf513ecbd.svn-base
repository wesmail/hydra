//*-- Authors: A.Ivashkin && A.Sadovsky (04.11.2004)
#ifndef HRUNGEKUTTA_H
#define HRUNGEKUTTA_H
#include "TString.h"
#include "TObject.h"
#include "TNamed.h"
#include "hgeomvector.h"
#include "hgeomrotation.h"
#include "hgeomtransform.h"
#include "hmdctrackgfield.h"

class HRungeKutta{
 private:

   Double_t FPOL; //factor of the magnetic field strength

   Double_t p0Guess;
   Double_t m0Guess;
   Double_t kdEdxAirCorr;
   HMdcTrackGField* externalFieldMap; //only used to access field map from Anar's dedicated class for that.
   Float_t  trackDirectionAtVertex[3];
   Float_t  providedVertex[3];

   //--------------------------------- RK-tracking -------------------------------------
   Double_t pi;
   Double_t zseg[2], roseg[2],tetaseg[2],phiseg[2];
   Float_t  step0;
   Float_t  pfit, chi2;
   Float_t  pver, nRKsteps; //,dE_dx;
   Float_t  length;
   Float_t  ddp, ddh, ddv, d1h, d1v;
   Float_t  dpar[5];
   Float_t  sig[8];
   Double_t dir1[3],dir2[3];
   Double_t u[6];
   Double_t hit[4][3],fit[4][3]; // hits and fited points at mdc's
   Double_t fitdp[4][3],fitdh[4][3],fitdv[4][3],fit1h[4][3],fit1v[4][3];
   Double_t dydpar[5][8], ydata[8], fitpar[5];
   Double_t zout,rout,thetaout,phiout;
   Int_t    sector;
   Double_t x0id[4],y0id[4],z0id[4]; //ideal center of mdc's
   Double_t alphaid[4], betaid[4], gammaid[4]; // ideal euler angles for mdc's
   Double_t x0[4][6],y0[4][6],z0[4][6]; //real center of mdc's
   Double_t alpha[4][6], beta[4][6], gamma[4][6]; // real euler angles for mdc's
   Double_t xyzmodule[4][3];
   Double_t dxtemp[4], dytemp[4], dztemp[4], dalphatemp[4], dbetatemp[4], dgammatemp[4];
   //-----------------------------------------------------------------------------------
  
 public:
   HRungeKutta();
  ~HRungeKutta();
   void init(HMdcTrackGField* fieldMapFromHydra); //field map is provided from Hydra
   void setFieldFactor(Float_t fpol){ FPOL = fpol; }
   void clear();

   Float_t getPfit(){return pfit;}    //momentum after RK-fit
   Int_t ndf; //number of degrees of freedom
   Float_t getChi2(){return chi2/Float_t(ndf);} //normalized chi^2 after RK-fit (so far for the case of 4-MDCs only: ndf = 3 = 8measurements-5varParams)

   Float_t uFitedAtMDC1[6];  //to store fited trajectory point and direction at MDC1
   Float_t uFitedAtMDC4[6];  //to store fited trajectory point and direction at MDC4
   Float_t uTracedToMETA[6]; //to store fited trajectory point and direction at META

   Float_t getXfithit(Int_t iMdc){ return (fit[iMdc-1][0]-hit[iMdc-1][0]); }  // returns (Xfit-Xhit) in Segment Coord System for MDC={1,2,3,4}
   Float_t getYfithit(Int_t iMdc){ return (fit[iMdc-1][1]-hit[iMdc-1][1]); }  // returns (Yfit-Yhit) in Segment Coord System for MDC={1,2,3,4}
   Float_t getXhit(   Int_t iMdc){ return (hit[iMdc-1][0]); }                 // returns Xhit in Segment Coord System for MDC={1,2,3,4}
   Float_t getYhit(   Int_t iMdc){ return (hit[iMdc-1][1]); }                 // returns Yhit in Segment Coord System for MDC={1,2,3,4}
            
   Float_t getXtackMDC1FitPos(){  return uFitedAtMDC1[0]; } // x
   Float_t getYtackMDC1FitPos(){  return uFitedAtMDC1[1]; } // y on MDC1 after RungeKutta fit
   Float_t getZtackMDC1FitPos(){  return uFitedAtMDC1[2]; } // z
   Float_t getDXtackMDC1FitPos(){ return uFitedAtMDC1[3]; } // dx
   Float_t getDYtackMDC1FitPos(){ return uFitedAtMDC1[4]; } // dy on MDC1 after RungeKutta fit
   Float_t getDZtackMDC1FitPos(){ return uFitedAtMDC1[5]; } // dz
   Float_t getXtackMDC4FitPos(){  return uFitedAtMDC4[0]; } // x
   Float_t getYtackMDC4FitPos(){  return uFitedAtMDC4[1]; } // y on MDC4 after RungeKutta fit
   Float_t getZtackMDC4FitPos(){  return uFitedAtMDC4[2]; } // z

   Float_t getXtackOnMETA(){      return uTracedToMETA[0];} // x
   Float_t getYtackOnMETA(){      return uTracedToMETA[1];} // y on META after RungeKutta fit and tracking up to a corresponding rod surface of META detector
   Float_t getZtackOnMETA(){      return uTracedToMETA[2];} // z

   Float_t getMDC1_MDC4length(){  return length;          } // RK-track length MDC1-MDC4
   Float_t getMDC12_theta(){      return thetaout;        } // theta obtained from MDC12 segment after RK-fit
   Float_t getMDC12_phi(){        return phiout;          } // phi   obtained from MDC12 segment after RK-fit
   
   Float_t getXdirAtVertex(){ return trackDirectionAtVertex[0]; } // RK-Track direction
   Float_t getYdirAtVertex(){ return trackDirectionAtVertex[1]; } // at the vertex point [ the vertex is provided by user using one of momRkgNewDinamicSeg12() calls]
   Float_t getZdirAtVertex(){ return trackDirectionAtVertex[2]; } // (can be used instead of MdcSeg1 direction)

   void  momRkgNewDinamic3hit(        Double_t*  x, Double_t*  y, Int_t sec);
   void  momRkgNewDinamic3hit(        Double_t*  x, Double_t*  y, Int_t sec, Double_t pGuess);
   void  momRkgNewDinamic3hit(        Double_t*  x, Double_t*  y, Int_t sec, Double_t pGuess, Double_t pAprox);
   void  momRkgNewDinamic3hitOriginal(Double_t*  x, Double_t*  y, Int_t sec);
   void  transformNew3hit();
   void  parSetNew3hit(Float_t, Float_t);
   void  gentrkNew3hit(Float_t, Double_t*, Double_t* p[3], Int_t);
   void  derive3hit(Double_t* p[3], Int_t);
   bool  linSys3hit();

   void  momRkgNewDinamic(        Double_t*  x, Double_t*  y, Int_t sec);
   void  momRkgNewDinamic(        Double_t*  x, Double_t*  y, Int_t sec, Double_t pGuess);
   void  momRkgNewDinamic(        Double_t*  x, Double_t*  y, Int_t sec, Double_t pGuess, Double_t pAprox);
   void  momRkgNewDinamicOriginal(Double_t*  x, Double_t*  y, Int_t sec);

   void  momRkgNewDinamicSeg12(   Double_t* u1, Double_t* u2, Int_t sec); //user provides MDC12 segment 6-vector {x,y,z,dx,dy,dz} , MDC34 segment 6-vector {x,y,z,dx,dy,dz} and sector
   void  momRkgNewDinamicSeg12(   Double_t* u1, Double_t* u2, Int_t sec, Double_t pGuess);  //user provides MDC12 segment 6-vector {x,y,z,dx,dy,dz} , MDC34 segment 6-vector {x,y,z,dx,dy,dz} and sector and initial guess for momentum
   void  momRkgNewDinamicSeg12(   Double_t* u1, Double_t* u2, Int_t sec, Double_t pGuess, Double_t pAprox);
   void  momRkgNewDinamicOriginalFromSegments(Double_t* u1, Double_t* u2, Int_t sec); //user provides MDC12 segment 6-vector {x,y,z,dx,dy,dz} , MDC34 segment 6-vector {x,y,z,dx,dy,dz} and sector
   void  traceFromMDC1_to_Vertex(Float_t* xyzVertex); //xyzVertex={xV,yV,zV} is used after Runge-Kutta fiting to propagate trajectory from point on MDC1 backwards till z=zVertex, which is provided by user

   //-not-for-hydra-//void  parSet(Float_t, Float_t);
   //-not-for-hydra-//void  momRkg(    Double_t* , Double_t* , Double_t* , Double_t*, Int_t);
   //-not-for-hydra-//void  momRKp1234(Double_t* , Double_t* , Double_t* , Double_t*, Int_t);
   //-not-for-hydra-//void  momRkgSasha(Double_t* p1, Double_t* p2, Double_t* p3, Double_t* p4, Int_t sector);
   //-not-for-hydra-//void  gentrk(Float_t, Double_t*, Double_t* p[4], Int_t);
   //-not-for-hydra-//void  transform();
   //-not-for-hydra-//void  mdcIntersect(Double_t* , Int_t, Double_t*);

   void  momRkgNew(Double_t* x, Double_t* y, Int_t* sec);
   void  gentrkBackToVertexZ(Float_t p, Float_t* U, Float_t* xyzVertex);  //propogates track backvards from MDC1 till Z=zVertex plane (it is valuable if called after RK-fit)
   void  traceMDC4_META(Float_t* pointOnMETA, Float_t* normVectorOfMETA); //called by user (do it after RK-fit) to propogate track till plane{point,normvector} of META detector
   Double_t calcAirElossCorrection(Float_t p);
   Bool_t   findIntersectPointVector6WithPlane(Double_t* u, Float_t *planeR0, Float_t* planeNorm, Double_t* pointIntersect);

   void  transformNew();
   void  mdcIntersectNew(Double_t* , Int_t, Double_t*);
   void  initMom();
   Float_t distance(Double_t*, Double_t*);
   void  parSetNew(Float_t, Float_t);
   void  cosines(Float_t, Float_t, Double_t*);   
   void  gentrkNew(Float_t, Double_t*, Double_t* p[4], Int_t);
   void  rkgtrk(Float_t, Float_t,   Double_t*);
   void  rkeqfw(Double_t*, Float_t, Double_t*, Float_t*);
   void  derive(Double_t* p[4], Int_t);
   bool  linSys();
   void  outputdata();
   void  geomcorrection(Double_t*, Int_t);
   void  setdEdxAirCorrection(Bool_t kdEdxAirCorrFlag);
   void  mdcgeom();

   //-local-geometry-service-routines-----------------------------------------------------
 private:
   Bool_t       mdcInstalled[4][6]; //remembers which MDCs have known geometry
   HGeomTransform hMDCgTRANS[4][6]; //[module][sector]
   //-not-for-hydra-//FILE *protocolFile;

 public:           
   void           applyMDCgeom();
   //-not-for-hydra-//Bool_t         readMDCpositionsFromASCII(Char_t* mdcGeomASCIIfileName);
   Bool_t         readMDCpositionsFromProgram(Int_t s, Int_t m, HGeomTransform gtrMod2Sec);
   void           debugMDCpositions();
   HGeomVector    findEulerAnglesASD(HGeomRotation rot);
   
   void           writeGeomSnapshoftToHydraLikeFormSector(Int_t s);

   //-not-for-hydra-//HGeomVector    findEulerAngles(HGeomRotation rot);
   //-not-for-hydra-//HGeomTransform getMdcGeomTransform(Int_t m, Int_t s);
   //-not-for-hydra-//void           protocolFileOpen();
   //-not-for-hydra-//void           protocolChi2geomNow(Int_t s, Double_t CHIQ);
   //-not-for-hydra-//Double_t       protocolReadMDCpositionsFromOneLine(Int_t s);
   //-not-for-hydra-//void           printShiftsAndEulers(Int_t m, Int_t s);

 //ClassDef(HRungeKutta,0)

};


#endif
