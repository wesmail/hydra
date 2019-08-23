//*-- Author  : A.Ivashkin <ivashkin@inr.ru>
//*-- Modified: A.Sadovsky <sadovski@fz-rossendorf.de> (04.11.2004 Hydra implementation)
using namespace std;
#include <iostream>
#include <fstream>
#include "hrungekutta.h"
#include <math.h>
#include "TMatrixD.h"

#define  PI 3.14159265358979323846

//ClassImp(HRungeKutta)	
		
HRungeKutta::HRungeKutta(){
   //magnetic field definition (if user forget to call setFieldFactor(Float_t fpol), we use arbitary FieldFactor==FPOL to be sure that at least something works)
   FPOL = 0.7215; //Nov02_FPOL=0.7215 //Sep03_FPOL=0.9235

   //Energy loss correction in Air is off by default
   kdEdxAirCorr = kFALSE;

   step0=25.;  //25.; //20.;
   ddp=0.01;ddh=0.001; ddv=0.001; d1h=0.01; d1v=0.01;
   dpar[0]=ddp, dpar[1]=ddh, dpar[2]=ddv, dpar[3]=d1h, dpar[4]=d1v;
   //-MDC1            //-MDC2            //-MDC3            //-MDC4
   sig[0]=0.140*2.00; sig[2]=0.140*2.00; sig[4]=0.140*2.00; sig[6]=0.140*2.00; //sigma X
   sig[1]=0.140*1.00; sig[3]=0.140*1.00; sig[5]=0.140*1.00; sig[7]=0.140*1.00; //sigma Y


   ndf = 0; //must be reset to the actual value in any of momentum calculation function (ndf=3 for MDC1234, ndf=1 for MDC123)
   //
   // If no geometry provided by mistake - ideal one is always known
   //

   //// ideal geometry of mdc's /////////////////////////////
   //// in a human readable form ////////////////////////////
   //x0id[0]=0.;      y0id[0]= 392.354;    z0id[0]= 403.529;
   //x0id[1]=0.;      y0id[1]= 585.842;    z0id[1]= 425.723;
   //x0id[2]=0.;      y0id[2]=1040.677;    z0id[2]= 901.253;
   //x0id[3]=0.;      y0id[3]=1267.451;    z0id[3]=1097.645;
   //alphaid[0]=90.0; betaid[0]=44.195530; gammaid[0]=-90.0;
   //alphaid[1]=90.0; betaid[1]=53.994635; gammaid[1]=-90.0;
   //alphaid[2]=90.0; betaid[2]=49.106580; gammaid[2]=-90.0;
   //alphaid[3]=90.0; betaid[3]=49.106580; gammaid[3]=-90.0;
   //////////////////////////////////////////////////////////

   // Hector's aligned geometry of mdc sector==0///////////////
   // in a human readable form ////////////////////////////////
   x0id[0]=0.585;      y0id[0]= 391.101;   z0id[0]= 374.594;
   x0id[1]=1.642;      y0id[1]= 584.959;   z0id[1]= 419.938;
   x0id[2]=4.839;      y0id[2]=1049.568;   z0id[2]= 900.305;
   x0id[3]=3.414;      y0id[3]=1275.861;   z0id[3]=1097.255;
   alphaid[0]=89.8953; betaid[0]=44.86500; gammaid[0]=-89.8773;
   alphaid[1]=90.1462; betaid[1]=53.93300; gammaid[1]=-90.0203;
   alphaid[2]=90.3607; betaid[2]=48.96640; gammaid[2]=-90.1027;
   alphaid[3]=90.0000; betaid[3]=49.10658; gammaid[3]=-90.0000;
   ////////////////////////////////////////////////////////////

   //// Hector's aligned geometry of mdc sector==3as0///////////////
   //// in a human readable form ////////////////////////////////
   //x0id[0]=2.511;      y0id[0]= 392.631;   z0id[0]= 375.613;
   //x0id[1]=5.004;      y0id[1]= 587.212;   z0id[1]= 418.761;
   //x0id[2]=7.609;      y0id[2]=1057.710;   z0id[2]= 917.345;
   //x0id[3]=8.723;      y0id[3]=1284.213;   z0id[3]=1114.056;
   //alphaid[0]=89.9846; betaid[0]=44.3778; gammaid[0]=-89.8634;
   //alphaid[1]=89.8891; betaid[1]=54.0949; gammaid[1]=-89.8797;
   //alphaid[2]=89.7182; betaid[2]=49.027 ; gammaid[2]=-90.1484;
   //alphaid[3]=90.0000; betaid[3]=49.1066; gammaid[3]=-90.0000;
   //////////////////////////////////////////////////////////////

   trackDirectionAtVertex[0] = 0.0;
   trackDirectionAtVertex[1] = 0.0;
   trackDirectionAtVertex[2] = 0.0;

   providedVertex[0] =      0.0; //
   providedVertex[1] =      0.0; // Preset user-provided vertex to minus infinity
   providedVertex[2] =-100000.0; //
};

HRungeKutta::~HRungeKutta(){
   //
};

void HRungeKutta::clear(){
   //
};

void HRungeKutta::init(HMdcTrackGField* fieldMapFromHydra){
   //////////////////////////////////////////////////////////////////////////////////////////////
   // Alternative initialisation, in case we would like to use fieldMap from Hydra             //
   // This is used while running in Hydra, then field map is red from a common parameter place //
   // So we do not need reread it locally - we just access fieldmap functions instead          //
   // locally defined ones inside Runge-Kutta                                                  //
   //////////////////////////////////////////////////////////////////////////////////////////////
   externalFieldMap = fieldMapFromHydra;
}

//---------------------------------------------------------------------------------------------//
//----------------group-of-functions-for-MDC123-Runge-Kutta-fit&propagation--------------------//
//---------------------------------------------------------------------------------------------//
void HRungeKutta::momRkgNewDinamic3hit(Double_t* x, Double_t* y, Int_t sec){
  p0Guess = 999999999.0;
  m0Guess = 999999999.0;
  momRkgNewDinamic3hitOriginal(x, y, sec);
}

void HRungeKutta::momRkgNewDinamic3hit(Double_t* x, Double_t* y, Int_t sec, Double_t pGuess){
  p0Guess = pGuess;
  m0Guess = 999999999.0;
  momRkgNewDinamic3hitOriginal(x, y, sec);
}

void HRungeKutta::momRkgNewDinamic3hit(Double_t* x, Double_t* y, Int_t sec, Double_t pGuess, Double_t mAprox){
  p0Guess = pGuess;
  m0Guess = mAprox;
  momRkgNewDinamic3hitOriginal(x, y, sec);
}

void HRungeKutta::momRkgNewDinamic3hitOriginal(Double_t* x, Double_t* y, Int_t sec){  
   //////////////////////////////////////////////////////////////////
   // This function reads hit on MDC (MDC local coordinate system) //
   // {i.e. pairs of coordinates (x[i],y[i]); i=0,1,2  z==0 }      //
   // translates them into sector system and does momentum         //
   // calculation using Runge Kutta method with 3MDCs              //
   //////////////////////////////////////////////////////////////////
   // Assumed that chambers has been read once and installed       //
   // in apropriative positions. For that call at least once       //
   // geom. init. before: mdcgeom() or hydraLikeGeom2RKlikeGeom()  //
   //                      fixed          ascii-file               //
   //                     geometry         geometry                //
   //////////////////////////////////////////////////////////////////
   ndf = 1; //{x,y}*3mdc=6 - {p,x,y,dx,dy}=5 == 1
   sector=sec; //sec[0];
   for(Int_t i=0;i<3;i++){ 
      xyzmodule[i][0]=x[i]; 
      xyzmodule[i][1]=y[i];
      xyzmodule[i][2]=0.;
   }	

   transformNew3hit(); //makes rot-matrix , transl. vector, and transforms point (hit) into SectorCoordSys
   
   initMom(); //preparations for momentum calculation, need mdc1-mdc2 mdc3-mdc4 tracklet directions on input

   if(pfit==0.) pfit=0.0001;	
   Double_t *ptr[3]; //*ptr[4];
   Float_t chi2old=0;
   for(Int_t i=0;i<3;i++) fit[0][i]=hit[0][i];
   Float_t dist=distance(&hit[0][0],&hit[1][0]);
   //cout << "dist=" << dist  << endl;
   Float_t uh=(hit[1][0]-hit[0][0])/dist; //napravliaiuschi cos po x vnachale is hit[][]-massiva
   Float_t uv=(hit[1][1]-hit[0][1])/dist; //napravliaiuschi cos po y vnachale is hit[][]-massiva
   //cout << "cos=" << uh << "  " << uv << endl;

   Int_t iter;
   for(iter=0;iter<11;iter++){
      chi2=0.;
      parSetNew3hit(uh,uv);

      for(Int_t npc=0;npc<3;npc++){ //-loop-over-3-MDCs--
         for(Int_t i=0;i<2;i++) chi2+=(hit[npc][i]-fit[npc][i])*(hit[npc][i]-fit[npc][i])/(sig[npc*2+i]*sig[npc*2+i]);
      }//---------------------------//-loop-over-3-MDCs--

      if(chi2==0.) chi2=0.001;
      //OK//if((TMath::Abs(chi2-chi2old)/chi2)<0.005) break;
      if((TMath::Abs(chi2-chi2old)/chi2)<0.009) break;

      chi2old=chi2;

      for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fitdp[i];			
      derive3hit(ptr,0);
      for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fitdh[i];			
      derive3hit(ptr,1);	
      for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fitdv[i];			
      derive3hit(ptr,2);
      for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fit1h[i];			
      derive3hit(ptr,3);
      for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fit1v[i];			
      derive3hit(ptr,4);

      ydata[0]=fit[0][0]-hit[0][0]; //x MDC1
      ydata[1]=fit[0][1]-hit[0][1]; //y MDC1
      ydata[2]=fit[1][0]-hit[1][0]; //x MDC2
      ydata[3]=fit[1][1]-hit[1][1]; //y MDC2
      ydata[4]=fit[2][0]-hit[2][0]; //x MDC3
      ydata[5]=fit[2][1]-hit[2][1]; //y	MDC3
      //ydata[6]=fit[3][0]-hit[3][0]; //x MDC4 cancel
      //ydata[7]=fit[3][1]-hit[3][1]; //y MDC4 cancel

      if(!linSys3hit()){ break;}
      //cout << "system has no solution" << endl; break;}						  
      pfit=pfit+fitpar[0];
      uh=uh+fitpar[1];
      uv=uv+fitpar[2];
      fit[0][0]=fit[0][0]+fitpar[3];
      fit[0][1]=fit[0][1]+fitpar[4];
      fit[0][2]=fit[0][2]-fitpar[4]*0.972306; //tangens pervoi kamery (MDCI) ostorojno - on tut zabit jelezno
      if(pfit==0.) pfit=10.;
      //cout << "pfit=  " << pfit << endl;		
   }
   //cout << "iter= " << iter << endl;

   outputdata();
}

void HRungeKutta::transformNew3hit(){
  ///////////////////////////////////////////////////////////////////////
  // Version for 3MDCs                                                 //
  // Transforms MDC-hit --> MDC-segment needed before momentum reconst.//
  ///////////////////////////////////////////////////////////////////////
  // This function access MDC parameters in human readable format      //
  // (x0,y0,z0,alpha,beta,gamma) and converts them first into          //
  // [degrees (0,90,180,270,360)]                                      //
  // rotation matrix and translation vector and in the end             //
  // it acceses point (hit) in local coord. system of MDC and          //
  // does the transformation of the point (hit) to Lab system          //
  ///////////////////////////////////////////////////////////////////////
  Double_t prim,segu,terc;
  Double_t rotlab[3][3];

  for(Int_t imdc=0;imdc<3;imdc++){ //-loop-over-MDCs--inCaseOf3MDCs---
     prim=alpha[imdc][sector]*PI/180.;
     segu=beta[ imdc][sector]*PI/180.;
     terc=gamma[imdc][sector]*PI/180.;

     rotlab[0][0]=( cos(prim)*cos(segu)*cos(terc))-(sin(prim)*sin(terc));
     rotlab[0][1]=(-cos(prim)*cos(segu)*sin(terc))-(sin(prim)*cos(terc));
     rotlab[0][2]=( cos(prim)*sin(segu));
     rotlab[1][0]=( sin(prim)*cos(segu)*cos(terc))+(cos(prim)*sin(terc));
     rotlab[1][1]=(-sin(prim)*cos(segu)*sin(terc))+(cos(prim)*cos(terc));
     rotlab[1][2]=( sin(prim)*sin(segu));
     rotlab[2][0]=(-sin(segu)*cos(terc));
     rotlab[2][1]=( sin(segu)*sin(terc));
     rotlab[2][2]=( cos(segu)          );

     for(Int_t ixyz=0;ixyz<3;ixyz++){//-loop-over-coordinate-123--
        hit[imdc][ixyz]=0.;
        for(Int_t index=0;index<3;index++){
           hit[imdc][ixyz]+=rotlab[ixyz][index]*xyzmodule[imdc][index];
        }//-end-for-index
     }//-loop-over-coordinate-123--

     hit[imdc][0]+=x0[imdc][sector];
     hit[imdc][1]+=y0[imdc][sector];
     hit[imdc][2]+=z0[imdc][sector];
  }//-loop-over-MDCs---inCaseOf3MDCs---

  //reshit' problemu otsutstvia napravliaiuschei vo vneshnem segmente
  //vneshnii segment neizvesten - ne mogemotsenit'' nachal'nyi impul's iz prostogo kickplane
  for(Int_t i=0;i<3;i++){
     dir1[i]=hit[1][i]-hit[0][i];
     dir2[i]=0.0; //hit[3][i]-hit[2][i];
  }
}

void HRungeKutta::parSetNew3hit(Float_t uh,Float_t uv){//-version-for-3MDCs--
   // uv - dobavka k vertikal'noi koordinate
   // uh - dobavka k horizontal'noi koordinate
   //
   Double_t *ptr[3];
   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fit[i];		
   gentrkNew3hit(pfit,u,ptr,1);

   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fitdp[i];	
   Float_t ptmp=pfit+ddp;	
   gentrkNew3hit(ptmp,u,ptr,2);

   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh+ddh,uv,&u[3]);
   for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fitdh[i];
   gentrkNew3hit(pfit,u,ptr,3);

   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv+ddv,&u[3]);
   for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fitdv[i];
   gentrkNew3hit(pfit,u,ptr,4);

   u[0]=fit[0][0]+d1h;
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fit1h[i];
   gentrkNew3hit(pfit,u,ptr,5);

   u[0]=fit[0][0];
   u[1]=fit[0][1]+d1v;
   u[2]=fit[0][2]-d1v*0.972306;
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<3;i++) ptr[i]=(Double_t*)&fit1v[i];
   gentrkNew3hit(pfit,u,ptr,6);

   //cout << "fit=" << fit[0][2] << "  " << fit[1][0] << endl;
   //cout << "cosnew=" << u[3] << "  " << u[4] << endl;	
}		

void HRungeKutta::gentrkNew3hit(Float_t p, Double_t* u, Double_t *hitxyz[3], Int_t kind ){
   //
   // here p == momentum
   // u - is 6-vector {pos,dir}={x,y,z,dx,dy,dz}
   //
   Float_t dstep=step0*2.00;
   Float_t hstep=step0*0.50;
   Float_t qstep=step0*0.25;	
   Int_t   jstep;
   Float_t  step;
   Double_t uold[3];
   for(Int_t i=0;i<3;i++) hitxyz[0][i]=u[i];


   if(kind==1){
      for(Int_t i=0;i<3;i++){uold[i]=u[i];length=0;}

      // before any propogation is started we remember initial momentum pver = p
      // and initial track point position (on MDC1 in this version) and on MDC4 also (see the end of loop)
      // this is used to build track trajectory length from vertex till META detector
      pver = p;  //asd//
      uFitedAtMDC1[0] = u[0]; // x
      uFitedAtMDC1[1] = u[1]; // y  of track on MDC1 (after RK ray tracking)
      uFitedAtMDC1[2] = u[2]; // z
      uFitedAtMDC1[3] = u[3]; // dx
      uFitedAtMDC1[4] = u[4]; // dy of track on MDC1 (after RK ray tracking)
      uFitedAtMDC1[5] = u[5]; // dz
   }

   jstep=0;	
   for(Int_t npc=1;npc<3;npc++) {
     step=step0;
     do{	
        jstep+=1;
        //cout << "u_fist  =" << u[0] << "  " << u[1] << "  " << u[2] << "  "<< kind << endl;

        if(kdEdxAirCorr){ //-momentum-correction-in-case-dE/dx-is-taken-into-account--
           Double_t dp = step*calcAirElossCorrection(p);
           if(p>30*dp){ //asd
              p=p-dp; //here we make dE/dx correction
           }
        }//-end-if-----------momentum-correction-in-case-dE/dx-is-taken-into-account--

        rkgtrk(step,p,u);
        if(kind==1){length+=step;} //here we calculate track length

        //cout << "u_second=" << u[0] << "  " << u[1] << "  " << u[2] << endl;
        mdcIntersectNew(u, npc+1, &hitxyz[npc][0]);
        //cout << "fitnpc=" << hitxyz[npc][0] << "  " << hitxyz[npc][1] << "  " << hitxyz[npc][2]<< endl;
        if(distance(u,&hitxyz[npc][0])<=dstep*2.) step=hstep;
        if(distance(u,&hitxyz[npc][0])<=step0   ) step=qstep;

        if(kind==1){ 
           //length+=distance(u,uold);
           for(Int_t i=0;i<3;i++){uold[i]=u[i];}
        }//dlia odnogo iz variantov vychisliaem dlinu treka i zapominaem starye znachenia tochki - takim obrazom poluchaem dlinu na kajdom shage
     }while( u[2]<hitxyz[npc][2]&&jstep<500 );

     if(npc==2){ //if/then we are in the MDC3 plane
       if(kind==1){
         // Here we store the MDC3 track position after fit (for subsequent propagation outside of RK-tracking)
         uFitedAtMDC4[0] = u[0]; // x
         uFitedAtMDC4[1] = u[1]; // y  of track on MDC4 (after RK ray tracking)
         uFitedAtMDC4[2] = u[2]; // z
         uFitedAtMDC4[3] = u[3]; // dx
         uFitedAtMDC4[4] = u[4]; // dy of track on MDC4 (after RK ray tracking)
         uFitedAtMDC4[5] = u[5]; // dz
       }
     }
   }//endfor--loop-over-npc--
  
   nRKsteps = jstep;
   //cout << "pver=" << pver << " p=" << p << " pver-p=" << pver-p << endl;
	
   //if(kind==1){ //only for one of variations RK (from 6 possible ones)
   //   //cout << "jstep=" << jstep << endl;
   //   //cout << "hit=" << hit[1][0] << "  " << hit[1][1] << "  " << hit[1][2]<< endl;
   //   //cout << "fitnew=" << hitxyz[1][0] << "  " << hitxyz[1][1] << "  " << hitxyz[1][2]<< endl;
   //   //cout << "hit=" << hit[3][0] << "  " << hit[3][1] << "  " << hit[3][2]<< endl;
   //   //cout << "fitnew=" << hitxyz[3][0] << "  " << hitxyz[3][1] << "  " << hitxyz[3][2]<< endl;
   //}
}

void HRungeKutta::derive3hit(Double_t* hitpar[3], Int_t kind){ //3MDC version
   dydpar[kind][0]=(hitpar[0][0]-fit[0][0])/dpar[kind];
   dydpar[kind][1]=(hitpar[0][1]-fit[0][1])/dpar[kind];
   dydpar[kind][2]=(hitpar[1][0]-fit[1][0])/dpar[kind];
   dydpar[kind][3]=(hitpar[1][1]-fit[1][1])/dpar[kind];
   dydpar[kind][4]=(hitpar[2][0]-fit[2][0])/dpar[kind];
   dydpar[kind][5]=(hitpar[2][1]-fit[2][1])/dpar[kind];	
   //dydpar[kind][6]=(hitpar[3][0]-fit[3][0])/dpar[kind]; //MDC4 cancelled
   //dydpar[kind][7]=(hitpar[3][1]-fit[3][1])/dpar[kind]; //MDC4 cencelled
  	 
   if(kind==0){
     for(Int_t i=0;i<8;i++){
        //cout << "dydpar=" << dydpar[0][i] << endl;
     }
   }
}		

bool HRungeKutta::linSys3hit(){ //-version-for-3MDCs--
   Double_t data[5][5], elements[5];
   bool noerr;
   Double_t *ptr;

   for(Int_t i=0;i<5;i++){	  
    for(Int_t j=0;j<5;j++){data[j][i]=0.;
     for(Int_t k=0;k<6;k++){ //now not till 8 but  till 6 becouse we have (x,y) for 3 chambers only
        data[j][i]+=dydpar[j][k]*dydpar[i][k]/(sig[k]); 
     } 	 
    }	  	
   }  

   //for(Int_t i=0;i<5;i++){
   //for(Int_t j=0;j<5;j++){
   //cout << "data=" << data[j][i] << endl;			
   //}
   //}		
		
   for(Int_t i=0;i<5;i++){elements[i]=0.;
      for(Int_t k=0;k<6;k++){ //now not till 8 but  till 6 becouse we have (x,y) for 3 chambers only
         elements[i]-= ydata[k]*dydpar[i][k]/sig[k];
      }
      //cout << "elements=" << elements[i] << endl;
   }
		
   ptr=(Double_t*)data;   
   TMatrixD m(5,5,ptr,NULL);
   if(m.Determinant()<0.0001 && m.Determinant()>-0.0001){
     //cout<<"Determinant is equal to zero"<<endl;
     noerr=false;
     return noerr;
     exit(1);
   }
   ptr=(Double_t*)elements;
   TMatrixD b(5,1,ptr,NULL);
   TMatrixD m1(5,1);
   //m.Print();
   m.Invert(0);
   //m.Print();
   m1.Mult(m,b);
   //m1.Print();
  
   //ptr=(Double_t*) &m1;
   for(Int_t i=0;i<5;i++){ fitpar[i]=m1(i,0); } //cout << "fitpar=" << fitpar[i] << endl;}

   noerr=true;
   return noerr;
}


//---------------------------------------------------------------------------------------------//
//----------------group-of-functions-for-MDC1234-Runge-Kutta-fit&propagation-------------------//
//---------------------------------------------------------------------------------------------//

void HRungeKutta::momRkgNewDinamic(Double_t* x, Double_t* y, Int_t sec){
  p0Guess = 999999999.0;
  m0Guess = 999999999.0;
  momRkgNewDinamicOriginal(x, y, sec);
}

void HRungeKutta::momRkgNewDinamic(Double_t* x, Double_t* y, Int_t sec, Double_t pGuess){
  p0Guess = pGuess;
  m0Guess = 999999999.0;
  momRkgNewDinamicOriginal(x, y, sec);
}

void HRungeKutta::momRkgNewDinamic(Double_t* x, Double_t* y, Int_t sec, Double_t pGuess, Double_t mAprox){
  p0Guess = pGuess;
  m0Guess = mAprox;
  momRkgNewDinamicOriginal(x, y, sec);
}

void HRungeKutta::momRkgNewDinamicOriginal(Double_t* x, Double_t* y, Int_t sec){  
   //////////////////////////////////////////////////////////////////
   // This function reads hit on MDC (MDC local coordinate system) //
   // {i.e. pairs of coordinates (x[i],y[i]); i=0,1,2,3, z==0 }    //
   // translates them into sector system and does momentum         //
   // calculation using Runge Kutta method                         //
   //////////////////////////////////////////////////////////////////
   // Assumed that chambers has been read once and installed       //
   // in apropriative positions. For that call at least once       //
   // geom. init. before: mdcgeom() or hydraLikeGeom2RKlikeGeom()  //
   //                      fixed          ascii-file               //
   //                     geometry         geometry                //
   //////////////////////////////////////////////////////////////////
   ndf = 3; //{x,y}*4mdc=8 - {p,x,y,dx,dy}=5 == 3
   sector=sec; //sec[0];
   for(Int_t i=0;i<4;i++){ 
      xyzmodule[i][0]=x[i]; 
      xyzmodule[i][1]=y[i];
      xyzmodule[i][2]=0.;
   }	

   transformNew(); //makes rot-matrix , transl. vector, and transforms point (hit) into SectorCoordSys
   
   initMom(); //preparations for momentum calculation, need mdc1-mdc2 mdc3-mdc4 tracklet directions on input

   if(pfit==0.) pfit=0.0001;	
   Double_t *ptr[4];
   Float_t chi2old=0;
   for(Int_t i=0;i<3;i++) fit[0][i]=hit[0][i];
   Float_t dist=distance(&hit[0][0],&hit[1][0]);
   //cout << "dist=" << dist  << endl;
   Float_t uh=(hit[1][0]-hit[0][0])/dist; //napravliaiuschi cos po x vnachale is hit[][]-massiva
   Float_t uv=(hit[1][1]-hit[0][1])/dist; //napravliaiuschi cos po y vnachale is hit[][]-massiva
   //cout << "cos=" << uh << "  " << uv << endl;

   Int_t iter;
   for(iter=0;iter<11;iter++){
      chi2=0.;
      parSetNew(uh,uv);
      for(Int_t npc=0;npc<4;npc++){
         for(Int_t i=0;i<2;i++) chi2+=(hit[npc][i]-fit[npc][i])*(hit[npc][i]-fit[npc][i])/(sig[(npc*2)+i]*sig[(npc*2)+i]);
      }
	
      if(chi2==0.) chi2=0.001;
      if((TMath::Abs(chi2-chi2old)/chi2)<0.005) break;		

      chi2old=chi2;
		
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdp[i];			
      derive(ptr,0);
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdh[i];			
      derive(ptr,1);	
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdv[i];			
      derive(ptr,2);
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1h[i];			
      derive(ptr,3);
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1v[i];			
      derive(ptr,4);

      ydata[0]=fit[0][0]-hit[0][0];
      ydata[1]=fit[0][1]-hit[0][1];
      ydata[2]=fit[1][0]-hit[1][0];
      ydata[3]=fit[1][1]-hit[1][1];
      ydata[4]=fit[2][0]-hit[2][0];
      ydata[5]=fit[2][1]-hit[2][1];	
      ydata[6]=fit[3][0]-hit[3][0];
      ydata[7]=fit[3][1]-hit[3][1];	
	
      if(!linSys()){ break;}
      //cout << "system has no solution" << endl; break;}						  
      pfit=pfit+fitpar[0];
      uh=uh+fitpar[1];
      uv=uv+fitpar[2];
      fit[0][0]=fit[0][0]+fitpar[3];
      fit[0][1]=fit[0][1]+fitpar[4];
      fit[0][2]=fit[0][2]-fitpar[4]*0.972306; //tangens pervoi kamery (MDCI) ostorojno - on tut zabit jelezno
      if(pfit==0.) pfit=10.;
      //cout << "pfit=  " << pfit << endl;		

   }
   //cout << "iter= " << iter << endl;

   outputdata();
}


void HRungeKutta::momRkgNewDinamicSeg12(Double_t* u1, Double_t* u2, Int_t sec){
  p0Guess = 999999999.0;
  m0Guess = 999999999.0;
  momRkgNewDinamicOriginalFromSegments(u1, u2, sec);
}

void HRungeKutta::momRkgNewDinamicSeg12(Double_t* u1, Double_t* u2, Int_t sec, Double_t pGuess){
  p0Guess = pGuess;
  m0Guess = 999999999.0;
  momRkgNewDinamicOriginalFromSegments(u1, u2, sec);
}

void HRungeKutta::momRkgNewDinamicSeg12(Double_t* u1, Double_t* u2, Int_t sec, Double_t pGuess, Double_t mAprox){
  p0Guess = pGuess;
  m0Guess = mAprox;
  momRkgNewDinamicOriginalFromSegments(u1, u2, sec);
}

void HRungeKutta::momRkgNewDinamicOriginalFromSegments(Double_t* u1, Double_t* u2, Int_t sec){
   ///////////////////////////////////////////////////(Double_t*  x, Double_t*  y, Int_t sec)//
   ////////////////////////////////////////////////////////////////////////////////
   // This function reads two tracklet position direction vectors u1 and u2      //
   // in MDC-Sector-Coodrinate-System                                            //
   // u1={x1,y1,z1,dx1,dy1,dz1} == {pos, dir}                                    //
   // u2={x2,y2,z2,dx2,dy2,dz2} == {pos, dir}                                    //
   // and projects them into corresponding MDC chambers which are also           //
   // treated in a sector coordinate system.                                     //
   // As the result it finds 4 points on MDC1,MDC2,MDC3 and MDC4,                //
   // which are then send for Runge-Kutta track propogation                      //
   //                                                                            //
   // NB: this momentum reconstructed edition is very usefull for Hydra analysis //
   //     where you normally get HMdcSeg (tracklet with position and direction)  //
   ////////////////////////////////////////////////////////////////////////////////
   // Assumed that chambers has been read once and installed                     //
   // in apropriative positions. For that call at least once                     //
   // geom. init. before: mdcgeom() or hydraLikeGeom2RKlikeGeom()                //
   //                      fixed          ascii-file                             //
   //                     geometry         geometry                              //
   ////////////////////////////////////////////////////////////////////////////////
   ndf = 3; //{x,y}*4mdc=8 - {p,x,y,dx,dy}=5 == 3
   sector=sec;

   mdcIntersectNew(u1, 1, &hit[0][0]);
   mdcIntersectNew(u1, 2, &hit[1][0]);
   mdcIntersectNew(u2, 3, &hit[2][0]);
   mdcIntersectNew(u2, 4, &hit[3][0]);
   for(Int_t i=0;i<3;i++){
      dir1[i]=hit[1][i]-hit[0][i];
      dir2[i]=hit[3][i]-hit[2][i];
   }

   initMom(); //preparations for momentum calculation, need mdc1-mdc2 mdc3-mdc4 tracklet directions on input

   if(pfit==0.) pfit=0.0001;	
   Double_t *ptr[4];
   Float_t chi2old=0;
   for(Int_t i=0;i<3;i++) fit[0][i]=hit[0][i];
   Float_t dist=distance(&hit[0][0],&hit[1][0]);
   //cout << "dist=" << dist  << endl;
   Float_t uh=(hit[1][0]-hit[0][0])/dist; //napravliaiuschi cos po x vnachale is hit[][]-massiva
   Float_t uv=(hit[1][1]-hit[0][1])/dist; //napravliaiuschi cos po y vnachale is hit[][]-massiva
   //cout << "cos=" << uh << "  " << uv << endl;

   Int_t iter;
   for(iter=0;iter<11;iter++){
      chi2=0.;
      parSetNew(uh,uv);
      for(Int_t npc=0;npc<4;npc++){
         for(Int_t i=0;i<2;i++) chi2+=(hit[npc][i]-fit[npc][i])*(hit[npc][i]-fit[npc][i])/(sig[(npc*2)+i]*sig[(npc*2)+i]);
      }
	
      if(chi2==0.) chi2=0.001;
      if((TMath::Abs(chi2-chi2old)/chi2)<0.005) break;		

      chi2old=chi2;
		
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdp[i];			
      derive(ptr,0);
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdh[i];			
      derive(ptr,1);	
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdv[i];			
      derive(ptr,2);
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1h[i];			
      derive(ptr,3);
      for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1v[i];			
      derive(ptr,4);

      ydata[0]=fit[0][0]-hit[0][0];
      ydata[1]=fit[0][1]-hit[0][1];
      ydata[2]=fit[1][0]-hit[1][0];
      ydata[3]=fit[1][1]-hit[1][1];
      ydata[4]=fit[2][0]-hit[2][0];
      ydata[5]=fit[2][1]-hit[2][1];	
      ydata[6]=fit[3][0]-hit[3][0];
      ydata[7]=fit[3][1]-hit[3][1];	
	
      if(!linSys()){ break;}
      //cout << "system has no solution" << endl; break;}						  
      pfit=pfit+fitpar[0];
      uh=uh+fitpar[1];
      uv=uv+fitpar[2];
      fit[0][0]=fit[0][0]+fitpar[3];
      fit[0][1]=fit[0][1]+fitpar[4];
      fit[0][2]=fit[0][2]-fitpar[4]*0.972306; //tangens pervoi kamery (MDCI) ostorojno - on tut zabit jelezno
      if(pfit==0.) pfit=10.;	
      //cout << "pfit=  " << pfit << endl;		

   }
   //cout << "iter= " << iter << endl;
   outputdata();
}

void HRungeKutta::traceFromMDC1_to_Vertex(Float_t* xyzVertex){
   providedVertex[0] = xyzVertex[0];
   providedVertex[1] = xyzVertex[1];
   providedVertex[2] = xyzVertex[2];
   //
   // here we can trace MDC1-Vertex
   //
   if(providedVertex[2]>-10000.){ 
      gentrkBackToVertexZ(pfit, uFitedAtMDC1, providedVertex); 
      providedVertex[0] =      0.0; // Preset it back to minus infinity
      providedVertex[1] =      0.0; // so that it will not propagate track to vertex
      providedVertex[2] =-100000.0; // next time without special request from user
   }
}


void HRungeKutta::momRkgNew(Double_t* x, Double_t* y, Int_t* sec){  
        ndf = 3; //{x,y}*4mdc=8 - {p,x,y,dx,dy}=5 == 3
        sector=sec[0];
        for(Int_t i=0;i<4;i++){ 
           xyzmodule[i][0]=x[i]; 
	   xyzmodule[i][1]=y[i];
           xyzmodule[i][2]=0.;
        }
	mdcgeom();
        transformNew();
	initMom();

        if(pfit==0.) pfit=0.0001;	
	Double_t *ptr[4];
	Float_t chi2old=0;
	for(Int_t i=0;i<3;i++) fit[0][i]=hit[0][i];
	Float_t dist=distance(&hit[0][0],&hit[1][0]);
        //cout << "dist=" << dist  << endl;
	Float_t uh=(hit[1][0]-hit[0][0])/dist; //napravliaiuschi cos po x vnachale
	Float_t uv=(hit[1][1]-hit[0][1])/dist; //napraviiauschii cos po y vnachale
        //cout << "cos=" << uh << "  " << uv << endl;

	Int_t iter;
	for(iter=0;iter<11;iter++){
	   chi2=0.;
	   parSetNew(uh,uv);
	   for(Int_t npc=0;npc<4;npc++){
	      for(Int_t i=0;i<2;i++) chi2+=(hit[npc][i]-fit[npc][i])*(hit[npc][i]-fit[npc][i])/(sig[(npc*2)+i]*sig[(npc*2)+i]);
	   }

	   if(chi2==0.) chi2=0.001;
	   if((TMath::Abs(chi2-chi2old)/chi2)<0.005) break;

	   chi2old=chi2;
	   	
	   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdp[i];			
	   derive(ptr,0);
	   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdh[i];			
	   derive(ptr,1);	
	   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdv[i];			
	   derive(ptr,2);
           for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1h[i];			
	   derive(ptr,3);
	   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1v[i];			
	   derive(ptr,4);

	   ydata[0]=fit[0][0]-hit[0][0];
	   ydata[1]=fit[0][1]-hit[0][1];
	   ydata[2]=fit[1][0]-hit[1][0];
	   ydata[3]=fit[1][1]-hit[1][1];
	   ydata[4]=fit[2][0]-hit[2][0];
	   ydata[5]=fit[2][1]-hit[2][1];	
	   ydata[6]=fit[3][0]-hit[3][0];
	   ydata[7]=fit[3][1]-hit[3][1];	

	   if(!linSys()){ break;}
           //cout << "system has no solution" << endl; break;}						  

	   pfit=pfit+fitpar[0];
	   uh=uh+fitpar[1];
	   uv=uv+fitpar[2];
	   fit[0][0]=fit[0][0]+fitpar[3];
	   fit[0][1]=fit[0][1]+fitpar[4];
	   fit[0][2]=fit[0][2]-fitpar[4]*0.972306;
	   if(pfit==0.) pfit=10.;	
           //cout << "pfit=  " << pfit << endl;		
	
	}
        //cout << "iter= " << iter << endl;
	outputdata();
}

void HRungeKutta::transformNew(){
  ///////////////////////////////////////////////////////////////////////
  // Transforms MDC-hit --> MDC-segment needed before momentum reconst.//
  ///////////////////////////////////////////////////////////////////////
  // This function access MDC parameters in human readable format      //
  // (x0,y0,z0,alpha,beta,gamma) and converts them first into          //
  // [degrees (0,90,180,270,360)]                                      //
  // rotation matrix and translation vector and in the end             //
  // it acceses point (hit) in local coord. system of MDC and          //
  // does the transformation of the point (hit) to Lab system          //
  ///////////////////////////////////////////////////////////////////////
  Double_t prim,segu,terc;
  Double_t rotlab[3][3];

  for(Int_t imdc=0;imdc<4;imdc++){ //-loop-over-MDCs--
     prim=alpha[imdc][sector]*PI/180.;
     segu=beta[ imdc][sector]*PI/180.;
     terc=gamma[imdc][sector]*PI/180.;

     rotlab[0][0]=( cos(prim)*cos(segu)*cos(terc))-(sin(prim)*sin(terc));
     rotlab[0][1]=(-cos(prim)*cos(segu)*sin(terc))-(sin(prim)*cos(terc));
     rotlab[0][2]=( cos(prim)*sin(segu));
     rotlab[1][0]=( sin(prim)*cos(segu)*cos(terc))+(cos(prim)*sin(terc));
     rotlab[1][1]=(-sin(prim)*cos(segu)*sin(terc))+(cos(prim)*cos(terc));
     rotlab[1][2]=( sin(prim)*sin(segu));
     rotlab[2][0]=(-sin(segu)*cos(terc));
     rotlab[2][1]=( sin(segu)*sin(terc));
     rotlab[2][2]=( cos(segu)          );

     for(Int_t ixyz=0;ixyz<3;ixyz++){//-loop-over-coordinate-123--
        hit[imdc][ixyz]=0.;
        for(Int_t index=0;index<3;index++){
           hit[imdc][ixyz]+=rotlab[ixyz][index]*xyzmodule[imdc][index];
        }//-end-for-index
     }//-loop-over-coordinate-123--

     hit[imdc][0]+=x0[imdc][sector];
     hit[imdc][1]+=y0[imdc][sector];
     hit[imdc][2]+=z0[imdc][sector];
  }//-loop-over-MDCs--

  for(Int_t i=0;i<3;i++){
     dir1[i]=hit[1][i]-hit[0][i];
     dir2[i]=hit[3][i]-hit[2][i];
  }
}

void HRungeKutta::mdcIntersectNew(Double_t* point, Int_t mdcN, Double_t xyz[3]){
   Double_t  pline[3],   dirline[3];
   Double_t pplane[3], normplane[3], dif[3];
   Double_t prod, tpar;

   for(Int_t i=0;i<3;i++){
      pline[  i]=point[i];
      dirline[i]=point[i+3];
   }
   pplane[0]=x0[mdcN-1][sector];
   pplane[1]=y0[mdcN-1][sector];
   pplane[2]=z0[mdcN-1][sector];

   normplane[0]=cos(alpha[mdcN-1][sector]/180.*PI) * sin(beta[mdcN-1][sector]/180.*PI);
   normplane[1]=sin(alpha[mdcN-1][sector]/180.*PI) * sin(beta[mdcN-1][sector]/180.*PI);
   normplane[2]=cos(beta[ mdcN-1][sector]/180.*PI);

   prod=0.;
   tpar=0.;
   for(Int_t i=0;i<3;i++){
      prod+=dirline[i]*normplane[i];
      dif[i]=pplane[i]-pline[i];
      tpar+=dif[i]*normplane[i];
   }

   tpar=tpar/prod;

   for(Int_t i=0;i<3;i++){
      xyz[i]=pline[i]+dirline[i]*tpar;
   }
   //+//printf("=1=xyz=={%f,%f,%f}\n",xyz[0],xyz[1],xyz[2]);
}


void HRungeKutta::initMom(){
   //
   // Here we set initial approximation momentum value
   //
   if(p0Guess==999999999.0){
      //
      // User did not supply a momentum guess, we start from simpleminded kickplane
      //
      Float_t temp=sqrt((dir1[1]*dir1[1]+dir1[2]*dir1[2])*(dir2[1]*dir2[1]+dir2[2]*dir2[2]));
      if(temp==0.) temp=1.;	
      Float_t cosxi=(dir1[1]*dir2[1]+dir1[2]*dir2[2])/temp;
      if(cosxi> 1.0) cosxi=1;
      if(cosxi<-1.0) cosxi=-1;
      Float_t xi=acos(cosxi);
      //cout << "xi=  " << xi*180./PI << endl;
      Float_t vprod=(dir1[1]*dir2[2]-dir1[2]*dir2[1]);
      temp=sin(xi/2.);
      if(temp==0.) temp=0.001;

      Float_t temp1=TMath::Abs(vprod);
      if(temp1==0.) temp1=0.001;
      pfit=75./(2*temp)*vprod/temp1;
   }else{
      //
      // User supplied momentum guess, so we use it as 0-approximation
      //
      pfit=p0Guess;
   }
   //cout << "pfit=  " << pfit << endl;
}
		
Float_t HRungeKutta::distance(Double_t p1[3],Double_t p2[3]){ 
   Float_t dist=sqrt( (p1[0]-p2[0])*(p1[0]-p2[0])+
                      (p1[1]-p2[1])*(p1[1]-p2[1])+
                      (p1[2]-p2[2])*(p1[2]-p2[2])  );
   return dist;
}		

void HRungeKutta::parSetNew(Float_t uh,Float_t uv){	
   // uv - vertical additive variation
   // uh - horisontal additive variation
   //
   Double_t *ptr[4];
   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit[i];		
   gentrkNew(pfit,u,ptr,1);

   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdp[i];	
   Float_t ptmp=pfit+ddp;	
   gentrkNew(ptmp,u,ptr,2);

   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh+ddh,uv,&u[3]);
   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdh[i];
   gentrkNew(pfit,u,ptr,3);

   u[0]=fit[0][0];
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv+ddv,&u[3]);
   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fitdv[i];
   gentrkNew(pfit,u,ptr,4);

   u[0]=fit[0][0]+d1h;
   u[1]=fit[0][1];
   u[2]=fit[0][2];
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1h[i];
   gentrkNew(pfit,u,ptr,5);

   u[0]=fit[0][0];
   u[1]=fit[0][1]+d1v;
   u[2]=fit[0][2]-d1v*0.972306;
   cosines(uh,uv,&u[3]);
   for(Int_t i=0;i<4;i++) ptr[i]=(Double_t*)&fit1v[i];
   gentrkNew(pfit,u,ptr,6);

   //cout << "fit=" << fit[0][2] << "  " << fit[1][0] << endl;
   //cout << "cosnew=" << u[3] << "  " << u[4] << endl;	
}		

		
void HRungeKutta::cosines(Float_t uh, Float_t uv, Double_t* dir){
   Float_t prod=sqrt(uh*uh+uv*uv);
   if(prod==0.) prod=0.0001;
   if(prod<1.){ 
     dir[0]=uh; dir[1]=uv; dir[2]=sqrt(1.-prod*prod);
   }else{
     dir[0]=uh/prod; dir[1]=uv/prod; dir[2]=0.;
   }	
}

void HRungeKutta::setdEdxAirCorrection(Bool_t kdEdxAirCorrFlag){
   kdEdxAirCorr = kdEdxAirCorrFlag;
   if(kdEdxAirCorr){
      printf("RungeKutta: Energy loss in the air is TAKEN into account!\n");
   }else{
      printf("RungeKutta: Energy loss in the air is NEGLECTED!\n");
   }
}

Double_t HRungeKutta::calcAirElossCorrection(Float_t p){
   ///////////////////////////////////////////////////////////////////////////////////////////
   // Here we can correct on energy loss in medium ??
   // on the length of "step" we know how much momentum is dropping due to energy loss
   // the problem here, that momentum is still unknown, despite we can use apriory
   // given information provided by another tracking (for Ex.: spline-track).
   //
   // Idea: 1st) we have already i-th iteration step momentum approximation
   //       of the particle, and it is p.
   //       2nd) we do correction for any particles as if it is a proton(TEMPORARY as a TEST)
   //
   // Observation: we can take either beta from TOF or to calculate it as beta=(p/Mpart)/sqrt(1-(p/Mpart)*(p/Mpart))
   //              What is better??? We start from beta=(p/Mpart)/sqrt(1-(p/Mpart)*(p/Mpart)) 
   //                                scince no external information is needed in this case
   ///////////////////////////////////////////////////////////////////////////////////////////
   Double_t Mpart = 938.27; //139.6 //938.27;  //[MeV] mass of traced particle

   if(m0Guess == 999999999.0){ Mpart=0.51099; }else{ Mpart=m0Guess; }

   Double_t rhoAir= 1.293E-3;       //[gr/cm^3] air dencity at ground conditions
   Double_t Me    = 0.51099906;     //[MeV] mass of electron  in units of 
   Int_t    z     = 1;              //[electrons] charge of traced particle (for proton z=1, for duteron z=2)
   Double_t I     = 8.57e-05;       //[MeV] or 85.7[eV] mean exitation potential
   Double_t Z     = 6.55833;        //[electrons] atomic number of absorbed material (CONFIRM if it is OK??)
   Double_t Na    = 6.022136736E23; //[1/mol]-Avogadro's number
   Double_t re    = 2.81794092E-13; //[cm] classical electron radius
   Double_t K     = 4*PI*Na*re*re*Me; //coefficient == 0.307075 [MeV] (good way to check Na,re,Me constants)
   Double_t A     = 14.3; //atomic mass of medium (air) (possibly wrong, CHECK it!!!)
   Double_t beta  = (p/Mpart)/sqrt(1.0+(p/Mpart)*(p/Mpart));
   Double_t Tmax  = 2.0*Me*beta*beta/(1.-beta*beta)/(1.0 + 2.*(Me/Mpart)*sqrt( 1.+beta*beta/(1-beta*beta) ) + (Me/Mpart)*(Me/Mpart));

   //+//Double_t TmaxAP= 2.0*Me*beta*beta/(1.-beta*beta);
   //+//cout << "Tmax="<<Tmax<<" TmaxAprox="<<TmaxAP<<endl;
   //+//Double_t dE_dxA= z*z*Z*K*(0.5*log(2.0*Me*Tmax*beta*beta/((1.-beta*beta)*I*I)) - beta*beta)/(A*beta*beta);
   //+//cout << "0.5log(...) - beta^2 == " << 0.5*log(2.0*Me*Tmax*beta*beta/(1-beta*beta)/(I*I))<<" - " << beta*beta <<" = " << 0.5*log(2.0*Me*Tmax*beta*beta/(1-beta*beta)/I/I) - beta*beta << endl;

   Double_t dE_dx = z*z*Z*K/A/beta/beta*(0.5*log(2.0*Me*Tmax*beta*beta/(1.-beta*beta)/I/I) - beta*beta);
   Double_t dp_corr_over_dSTEP = (sqrt(p*p+Mpart*Mpart)/p)*dE_dx*rhoAir;

   ///////////////////////////////////////////////////////
   // dp = step*(sqrt(p*p+Mpart*Mpart)/p)*dE_dx*rhoAir; //
   ///////////////////////////////////////////////////////

   //+//cout << "beta="<< beta << " p=" << p << " dp=" << dp << " dp/p=" << dp/p << " dE/dx=" << dE_dx << ","<<dE_dxA <<endl;

   return dp_corr_over_dSTEP;
}

void HRungeKutta::gentrkNew(Float_t p, Double_t* u, Double_t *hitxyz[4], Int_t kind ){
   //
   // here p == momentum
   // u - is 6-vector {pos,dir}={x,y,z,dx,dy,dz}
   //
   Float_t dstep=step0*2.00;
   Float_t hstep=step0*0.50;
   Float_t qstep=step0*0.25;	
   Int_t   jstep;
   Float_t  step;
   Double_t uold[3];
   for(Int_t i=0;i<3;i++) hitxyz[0][i]=u[i];


   if(kind==1){
      for(Int_t i=0;i<3;i++){uold[i]=u[i];length=0;}

      // before any propogation is started we remember initial momentum pver = p
      // and initial track point position (on MDC1 in this version) and on MDC4 also (see the end of loop)
      // this is used to build track trajectory length from vertex till META detector
      pver = p;  //asd//
      uFitedAtMDC1[0] = u[0]; // x
      uFitedAtMDC1[1] = u[1]; // y  of track on MDC1 (after RK ray tracking)
      uFitedAtMDC1[2] = u[2]; // z
      uFitedAtMDC1[3] = u[3]; // dx
      uFitedAtMDC1[4] = u[4]; // dy of track on MDC1 (after RK ray tracking)
      uFitedAtMDC1[5] = u[5]; // dz
   }

   jstep=0;	
   for(Int_t npc=1;npc<4;npc++) {
     step=step0;
     do{	
        jstep+=1;
        //cout << "u_fist  =" << u[0] << "  " << u[1] << "  " << u[2] << "  "<< kind << endl;

        if(kdEdxAirCorr){ //-momentum-correction-in-case-dE/dx-is-taken-into-account--
           Double_t dp = step*calcAirElossCorrection(p);
           if(p>30*dp){ //asd
              p=p-dp; //here we make dE/dx correction
           }
        }//-end-if-----------momentum-correction-in-case-dE/dx-is-taken-into-account--

        rkgtrk(step,p,u);
        if(kind==1){length+=step;} //here we calculate track length

        //cout << "u_second=" << u[0] << "  " << u[1] << "  " << u[2] << endl;
        mdcIntersectNew(u, npc+1, &hitxyz[npc][0]);
        //cout << "fitnpc=" << hitxyz[npc][0] << "  " << hitxyz[npc][1] << "  " << hitxyz[npc][2]<< endl;
        if(distance(u,&hitxyz[npc][0])<=dstep*2.) step=hstep;
        if(distance(u,&hitxyz[npc][0])<=step0   ) step=qstep;

        if(kind==1){ 
           //length+=distance(u,uold);
           for(Int_t i=0;i<3;i++){uold[i]=u[i];}
        }//dlia odnogo iz variantov vychisliaem dlinu treka i zapominaem starye znachenia tochki - takim obrazom poluchaem dlinu na kajdom shage
     }while( u[2]<hitxyz[npc][2]&&jstep<500 );

     if(npc==3){ //if/then we are in the MDC4 plane
       if(kind==1){
         // Here we store the MDC4 track position after fit (for subsequent propagation outside of RK-tracking)
         uFitedAtMDC4[0] = u[0]; // x
         uFitedAtMDC4[1] = u[1]; // y  of track on MDC4 (after RK ray tracking)
         uFitedAtMDC4[2] = u[2]; // z
         uFitedAtMDC4[3] = u[3]; // dx
         uFitedAtMDC4[4] = u[4]; // dy of track on MDC4 (after RK ray tracking)
         uFitedAtMDC4[5] = u[5]; // dz
       }
     }
   }//endfor--loop-over-npc--
  
   nRKsteps = jstep;
   //cout << "pver=" << pver << " p=" << p << " pver-p=" << pver-p << endl;
	
   //if(kind==1){ //only for one of variations RK (from 6 possible ones)
   //   //cout << "jstep=" << jstep << endl;
   //   //cout << "hit=" << hit[1][0] << "  " << hit[1][1] << "  " << hit[1][2]<< endl;
   //   //cout << "fitnew=" << hitxyz[1][0] << "  " << hitxyz[1][1] << "  " << hitxyz[1][2]<< endl;
   //   //cout << "hit=" << hit[3][0] << "  " << hit[3][1] << "  " << hit[3][2]<< endl;
   //   //cout << "fitnew=" << hitxyz[3][0] << "  " << hitxyz[3][1] << "  " << hitxyz[3][2]<< endl;
   //}
}

void HRungeKutta::traceMDC4_META(Float_t* pointOnMETA, Float_t* normVectorOfMETA){
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // This function: should be called only after RK-minimisation is done                                      //
   //                propagates track in magnetic field in forward direction from MDC4 position till track    //
   //                intersection with META plane (is defined by META-hit and normal vector to rod's plane    //
   // here p == pfit - momentum after RK-fitting                                                              //
   //                                                                                                         //
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////
   Float_t dstep = step0*2.00;
   Float_t  step = step0;
   Int_t   jstep = 0;
   Double_t dist = 0;
   Double_t u[6];
   u[0] = uFitedAtMDC4[0]; //
   u[1] = uFitedAtMDC4[1]; // Local copy of tracking particle position
   u[2] = uFitedAtMDC4[2]; //
   u[3] = uFitedAtMDC4[3]; //
   u[4] = uFitedAtMDC4[4]; // Local copy of tracking particle direction 
   u[5] = uFitedAtMDC4[5]; // 

   Double_t xyzIntersect[3];

   Bool_t propagate = kTRUE;
   
   while(propagate && jstep<500){
     jstep++;
     if( findIntersectPointVector6WithPlane(u, pointOnMETA, normVectorOfMETA, xyzIntersect) ){
       //in case we have an intersection we know Z=xyzIntersec[2] -- till which to propagate
       dist = distance(u,xyzIntersect);
     }else{
       //in case they are parallel and we do not have an intersection -
       //we do tracking anyway - hoping that on the next step they are going to be non-parallel
       //even if they are parallel - after 500 steps this tracking will stop itself
     }
     if(u[2]>=xyzIntersect[2]){ //we passed through the META plane - stop tracking
       propagate = kFALSE;
     }
     if(dist<dstep){step=dist*1.000001;} //last step is just a bit more then necessary to cross the final plane
   
     rkgtrk(step,pfit,u); // we use pfit as value for momentum to propagate
     //+//printf("MDC4-META(%i): {xyz}={%5.2f,%5.2f,%5.2f} zMETA=%5.2f, dist=%5.2f\n", jstep,u[0],u[1],u[2],xyzIntersect[2],dist);

     //
     // Output of u[] - to the user
     //
     uTracedToMETA[0] = u[0];
     uTracedToMETA[1] = u[1];
     uTracedToMETA[2] = u[2];
     uTracedToMETA[3] = u[3];
     uTracedToMETA[4] = u[4];
     uTracedToMETA[5] = u[5];
   }
}


void HRungeKutta::gentrkBackToVertexZ(Float_t p, Float_t* U, Float_t* xyzVertex){
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////
   // This function: should be called only after RK-minimisation is done                                      //
   //                propagates track in magnetic field in backward direction from MDC1 position to Z=zVertex //
   // here p == momentum                                                                                      //
   // u  - is 6-vector {pos,dir}={x,y,z,dx,dy,dz}                                                             //
   // z0 - is z coordinate then we stop track propagation                                                     //
   //                                                                                                         //
   /////////////////////////////////////////////////////////////////////////////////////////////////////////////
   Float_t dstep = step0*2.00;
   Float_t  step = step0;
   Int_t   jstep = 0;

   Double_t dist = 0;
   Float_t  nPl[3]={0.0, 0.0, 1.0}; //normal vector to Z=zVertex plane
   Double_t xyzIntersect[3];
   Double_t u[6];
   u[0] =  U[0]; //
   u[1] =  U[1]; // Local copy of tracking particle position/direction 
   u[2] =  U[2]; //
   u[3] = -U[3]; //
   u[4] = -U[4]; // Local copy of tracking particle position/direction 
   u[5] = -U[5]; // (reversed to go back from MDC1 to target)

   while(u[2]>=xyzVertex[2] && jstep<500){
     jstep++;
   
     if( findIntersectPointVector6WithPlane(u, xyzVertex, nPl, xyzIntersect) ){
       //in case we have an intersection
       dist = distance(u,xyzIntersect);
     }else{
       //in case they are parallel and we do not have an intersection -
       //we do tracking anyway - hoping that on the next step they are going to be non-parallel
       //even if they are parallel - after 500 steps this tracking will stop itself
     }
     if(dist<dstep){step=dist*1.000001;} //last step is just a bit more then necessary to cross the final plane
   
     rkgtrk(step,p,u);
     //+//printf("MDC1-Vert(%i): {xyz}={%5.2f,%5.2f,%5.2f} zVert=%5.2f\n", jstep,u[0],u[1],u[2],xyzVertex[2]);
     
   }//-end-while--
   u[3] = -u[3]; //
   u[4] = -u[4]; // Reverse track direction back to usual "from Traget to MDC1"
   u[5] = -u[5]; //

   trackDirectionAtVertex[0] = u[3];  //
   trackDirectionAtVertex[1] = u[4];  // in the end we save track directions into corresponding array
   trackDirectionAtVertex[2] = u[5];  //
   Float_t ul, Ul;

   Ul = sqrt(U[3]*U[3] + U[2]*U[2] + U[1]*U[1]);
   ul = sqrt(u[3]*u[3] + u[2]*u[2] + u[1]*u[1]);

   //cout << "U[3]=" << U[3] << "  u[3]=" << u[3] << endl;
   //cout << "U[4]=" << U[4] << "  u[4]=" << u[4] << endl;
   //cout << "U[5]=" << U[5] << "  u[5]=" << u[5] << endl;
   //printf("gentrkBackToVertexZ: X u[x]/ul=%6.3f, U[x]/Ul=%6.3f  diff = %6.3f\n",u[3]/ul, U[3]/Ul, u[3]/ul-U[3]/Ul);
   //printf("gentrkBackToVertexZ: Y u[y]/ul=%6.3f, U[y]/Ul=%6.3f  diff = %6.3f\n",u[4]/ul, U[4]/Ul, u[4]/ul-U[4]/Ul);
   //printf("gentrkBackToVertexZ: Z u[z]/ul=%6.3f, U[z]/Ul=%6.3f  diff = %6.3f\n",u[5]/ul, U[5]/Ul, u[5]/ul-U[5]/Ul);
}


Bool_t HRungeKutta::findIntersectPointVector6WithPlane(Double_t* u, Float_t *planeR0, Float_t* planeNorm, Double_t* pointIntersect){
   /////////////////////////////////////////////////////////////////////////////////////////////
   // u={Xp,Yp,Zp,dXp,dYp,dZp} - "moving point" current position and movment direction vector //
   // planeR0[3] some point on the plane to which intersection is supposed to happen          //
   // planeNorm[3] perpendicular vector to the plane                                          //
   // pointIntersect[3] - output - the intersection point of "moving point" with plane        //
   /////////////////////////////////////////////////////////////////////////////////////////////

   Double_t denom = (planeNorm[0]*u[3] + planeNorm[1]*u[4] + planeNorm[2]*u[5]);
   if(denom!=0.0){
      Double_t t = ( (planeR0[0]-u[0])*planeNorm[0] + 
                     (planeR0[1]-u[1])*planeNorm[1] + 
                     (planeR0[2]-u[2])*planeNorm[2]  ) / denom;
      
      pointIntersect[0] = u[0] + u[3]*t;
      pointIntersect[1] = u[1] + u[4]*t;
      pointIntersect[2] = u[2] + u[5]*t;
      return kTRUE;
   }else{
      //No intersection is found : (plane || track)
      return kFALSE;
   }
}



void HRungeKutta::rkgtrk(Float_t step, Float_t p, Double_t* u){
   //One step of track tracing from the position and in the direction of u[6]
   Float_t	k1[3],k2[3],k3[3],k4[3];
   Double_t u1[6],u2[6];
   Float_t hstep=step*0.5;
   Float_t qstep=step*step*0.125;
   Double_t b[3];
   Double_t ucm[3];
   for(Int_t i=0;i<3;i++) ucm[i]=u[i]/10.;		
   externalFieldMap->calcField(ucm, b, FPOL);
   //calcField(ucm, b, FPOL);
   ////cout << "zfield=" << ucm[2] << "field=" << b[0] << "  "<< b[1] << "  "<< b[2] << endl;
   rkeqfw(b,p,u,k1);

   for(Int_t i=0;i<3;i++){
       u1[  i]=u[  i]+hstep*u[3+i]+qstep*k1[i];
       u1[3+i]=u[3+i]+hstep*k1[ i];
   }

   for(Int_t i=0;i<3;i++) ucm[i]=u1[i]/10.;		
   externalFieldMap->calcField(ucm, b, FPOL);
   //calcField(ucm, b, FPOL);
   rkeqfw(b,p,u1,k2);
   u1[3] = u[3] + hstep*k2[0];
   u1[4] = u[4] + hstep*k2[1];
   u1[5] = u[5] + hstep*k2[2];

   rkeqfw(b,p,u1,k3);	  
   for(Int_t i=0;i<3;i++){
     u2[  i]=u[  i]+step*u[3+i]+2.*hstep*hstep*k3[i];
     u2[3+i]=u[3+i]+step*k3[ i];
   }

   for(Int_t i=0;i<3;i++) ucm[i]=u2[i]/10.;		
   externalFieldMap->calcField(ucm, b, FPOL);
   //calcField(ucm, b, FPOL);

   rkeqfw(b,p,u2,k4);
   for(Int_t i=0;i<3;i++){
       u[  i]=u[  i]+step*u[3+i]+step*step*(k1[i]+k2[i]+k3[i])/6.;
       u[3+i]=u[3+i]+step*(k1[i]+2.*k2[i]+2.*k3[i]+k4[i])/6.;
   }
}

void HRungeKutta::rkeqfw(Double_t b[3], Float_t p, Double_t u[6], Float_t duds[3]){
   Double_t  con=0.2997925; //con=0.2997925E-4;
   enum{X,Y,Z,DX,DY,DZ};
   duds[X]=con*(u[DY]*b[Z]-u[DZ]*b[Y])/p;
   duds[Y]=con*(u[DZ]*b[X]-u[DX]*b[Z])/p;
   duds[Z]=con*(u[DX]*b[Y]-u[DY]*b[X])/p;	
}	
				
void HRungeKutta::derive(Double_t* hitpar[4], Int_t kind){
   dydpar[kind][0]=(hitpar[0][0]-fit[0][0])/dpar[kind];
   dydpar[kind][1]=(hitpar[0][1]-fit[0][1])/dpar[kind];
   dydpar[kind][2]=(hitpar[1][0]-fit[1][0])/dpar[kind];
   dydpar[kind][3]=(hitpar[1][1]-fit[1][1])/dpar[kind];
   dydpar[kind][4]=(hitpar[2][0]-fit[2][0])/dpar[kind];
   dydpar[kind][5]=(hitpar[2][1]-fit[2][1])/dpar[kind];	
   dydpar[kind][6]=(hitpar[3][0]-fit[3][0])/dpar[kind];
   dydpar[kind][7]=(hitpar[3][1]-fit[3][1])/dpar[kind];	
  	 
   if(kind==0){
     for(Int_t i=0;i<8;i++){
        //cout << "dydpar=" << dydpar[0][i] << endl;
     }
   }
}		


bool HRungeKutta::linSys(){
   Double_t data[5][5], elements[5];
   bool noerr;
   Double_t *ptr;

   for(Int_t i=0;i<5;i++){	  
    for(Int_t j=0;j<5;j++){data[j][i]=0.;
     for(Int_t k=0;k<8;k++){
        data[j][i]+=dydpar[j][k]*dydpar[i][k]/(sig[k]); 
     } 	 
    }	  	
   }  

   //for(Int_t i=0;i<5;i++){
   //for(Int_t j=0;j<5;j++){
   //cout << "data=" << data[j][i] << endl;			
   //}
   //}		
		
   for(Int_t i=0;i<5;i++){elements[i]=0.;
      for(Int_t k=0;k<8;k++){
         elements[i]-= ydata[k]*dydpar[i][k]/sig[k];
      }
      //cout << "elements=" << elements[i] << endl;
   }
		
   ptr=(Double_t*)data;   
   TMatrixD m(5,5,ptr,NULL);
   if(m.Determinant()<0.0001 && m.Determinant()>-0.0001){
     //cout<<"Determinant is equal to zero"<<endl;
     noerr=false;
     return noerr;
     exit(1);
   }
   ptr=(Double_t*)elements;
   TMatrixD b(5,1,ptr,NULL);
   TMatrixD m1(5,1);
   //m.Print();
   m.Invert(0);
   //m.Print();
   m1.Mult(m,b);
   //m1.Print();
  
   //ptr=(Double_t*) &m1;
   for(Int_t i=0;i<5;i++){ fitpar[i]=m1(i,0); } //cout << "fitpar=" << fitpar[i] << endl;}

   noerr=true;
   return noerr;
}

void HRungeKutta::outputdata(){
   Double_t x0,y0,z0,cosx,cosy,cosz;
   x0=fit[0][0]; 
   y0=fit[0][1];
   z0=fit[0][2];   
   cosx=fit[1][0]-fit[0][0];
   cosy=fit[1][1]-fit[0][1];
   cosz=fit[1][2]-fit[0][2];

   thetaout=acos(cosz/sqrt(cosx*cosx+cosy*cosy+cosz*cosz));
   phiout=atan(cosy/cosx);
   if(phiout<0.) phiout=PI+phiout;
   rout=y0*cos(phiout)-x0*sin(phiout);
   zout=-(x0*cos(phiout)+y0*sin(phiout))*cos(phiout)/sin(phiout);
}

void HRungeKutta::geomcorrection(Double_t* corr, Int_t mdcnum){
  for(Int_t i=0;i<4;i++){
     dxtemp[    i]=0.; dytemp[   i]=0.; dztemp[    i]=0.;
     dalphatemp[i]=0.; dbetatemp[i]=0.; dgammatemp[i]=0.;
  }
  if(mdcnum>0 && mdcnum<5){
    dxtemp[    mdcnum-1]=corr[0]; dytemp[   mdcnum-1]=corr[1]; dztemp[    mdcnum-1]=corr[2];
    dalphatemp[mdcnum-1]=corr[3]; dbetatemp[mdcnum-1]=corr[4]; dgammatemp[mdcnum-1]=corr[5];
  }
  if(mdcnum==12){
    dxtemp[    0]=corr[0]; dytemp[   0]=corr[1]; dztemp[    0]=corr[2];
    dalphatemp[0]=corr[3]; dbetatemp[0]=corr[4]; dgammatemp[0]=corr[5];
    dxtemp[    1]=corr[0]; dytemp[   1]=corr[1]; dztemp[    1]=corr[2];
    dalphatemp[1]=corr[3]; dbetatemp[1]=corr[4]; dgammatemp[1]=corr[5];  
  }
  if(mdcnum==34){
    dxtemp[    2]=corr[0]; dytemp[   2]=corr[1]; dztemp[    2]=corr[2];
    dalphatemp[2]=corr[3]; dbetatemp[2]=corr[4]; dgammatemp[2]=corr[5];
    dxtemp[    3]=corr[0]; dytemp[   3]=corr[1]; dztemp[    3]=corr[2];
    dalphatemp[3]=corr[3]; dbetatemp[3]=corr[4]; dgammatemp[3]=corr[5];  
  }

  if(mdcnum==1234){
    dxtemp[    0]=corr[0]; dytemp[   0]=corr[1]; dztemp[    0]=corr[2];
    dalphatemp[0]=corr[3]; dbetatemp[0]=corr[4]; dgammatemp[0]=corr[5];
    dxtemp[    1]=corr[0]; dytemp[   1]=corr[1]; dztemp[    1]=corr[2];
    dalphatemp[1]=corr[3]; dbetatemp[1]=corr[4]; dgammatemp[1]=corr[5];  
    dxtemp[    2]=corr[0]; dytemp[   2]=corr[1]; dztemp[    2]=corr[2];
    dalphatemp[2]=corr[3]; dbetatemp[2]=corr[4]; dgammatemp[2]=corr[5];
    dxtemp[    3]=corr[0]; dytemp[   3]=corr[1]; dztemp[    3]=corr[2];
    dalphatemp[3]=corr[3]; dbetatemp[3]=corr[4]; dgammatemp[3]=corr[5];  
  }
}

void HRungeKutta::mdcgeom(){
  /////////////////////////////////////////////////
  // This routine defines the coordinates of all //
  // MDC chambers in a human readable format     //
  // (x0,y0,z0,alpha,beta,gamma)                 //
  // translation shifts  and rotation Euler's    //
  /////////////////////////////////////////////////

  Double_t    dx0[4][6],   dy0[4][6],    dz0[4][6];
  Double_t dalpha[4][6], dbeta[4][6], dgamma[4][6];

  //***********************************************
  // Sector 0
  dx0[   0][0]=0.; dy0[  0][0]=0.; dz0[   0][0]=0.;
  dx0[   1][0]=0.; dy0[  1][0]=0.; dz0[   1][0]=0.;
  dx0[   2][0]=0.; dy0[  2][0]=0.; dz0[   2][0]=0.;
  dx0[   3][0]=0.; dy0[  3][0]=0.; dz0[   3][0]=0.;
  dalpha[0][0]=0.; dbeta[0][0]=0.; dgamma[0][0]=0.;
  dalpha[1][0]=0.; dbeta[1][0]=0.; dgamma[1][0]=0.;
  dalpha[2][0]=0.; dbeta[2][0]=0.; dgamma[2][0]=0.;
  dalpha[3][0]=0.; dbeta[3][0]=0.; dgamma[3][0]=0.;

  // Sector 1
  dx0[   0][1]=0.; dy0[  0][1]=0.; dz0[   0][1]=0.;
  dx0[   1][1]=0.; dy0[  1][1]=0.; dz0[   1][1]=0.;
  dx0[   2][1]=0.; dy0[  2][1]=0.; dz0[   2][1]=0.;
  dx0[   3][1]=0.; dy0[  3][1]=0.; dz0[   3][1]=0.;
  dalpha[0][1]=0.; dbeta[0][1]=0.; dgamma[0][1]=0.;
  dalpha[1][1]=0.; dbeta[1][1]=0.; dgamma[1][1]=0.;
  dalpha[2][1]=0.; dbeta[2][1]=0.; dgamma[2][1]=0.;
  dalpha[3][1]=0.; dbeta[3][1]=0.; dgamma[3][1]=0.;

  // Sector 2
  dx0[   0][2]=0.; dy0[  0][2]=0.; dz0[   0][2]=0.;
  dx0[   1][2]=0.; dy0[  1][2]=0.; dz0[   1][2]=0.;
  dx0[   2][2]=0.; dy0[  2][2]=0.; dz0[   2][2]=0.;
  dx0[   3][2]=0.; dy0[  3][2]=0.; dz0[   3][2]=0.;
  dalpha[0][2]=0.; dbeta[0][2]=0.; dgamma[0][2]=0.;
  dalpha[1][2]=0.; dbeta[1][2]=0.; dgamma[1][2]=0.;
  dalpha[2][2]=0.; dbeta[2][2]=0.; dgamma[2][2]=0.;
  dalpha[3][2]=0.; dbeta[3][2]=0.; dgamma[3][2]=0.;

  // Sector 3
  dx0[   0][3]=0.; dy0[  0][3]=0.; dz0[   0][3]=0.;
  dx0[   1][3]=0.; dy0[  1][3]=0.; dz0[   1][3]=0.;
  dx0[   2][3]=0.; dy0[  2][3]=0.; dz0[   2][3]=0.;
  dx0[   3][3]=0.; dy0[  3][3]=0.; dz0[   3][3]=0.;
  dalpha[0][3]=0.; dbeta[0][3]=0.; dgamma[0][3]=0.;
  dalpha[1][3]=0.; dbeta[1][3]=0.; dgamma[1][3]=0.;
  dalpha[2][3]=0.; dbeta[2][3]=0.; dgamma[2][3]=0.;
  dalpha[3][3]=0.; dbeta[3][3]=0.; dgamma[3][3]=0.;

  // Sector 4
  dx0[   0][4]=0.; dy0[  0][4]=0.; dz0[   0][4]=0.;
  dx0[   1][4]=0.; dy0[  1][4]=0.; dz0[   1][4]=0.;
  dx0[   2][4]=0.; dy0[  2][4]=0.; dz0[   2][4]=0.;
  dx0[   3][4]=0.; dy0[  3][4]=0.; dz0[   3][4]=0.;
  dalpha[0][4]=0.; dbeta[0][4]=0.; dgamma[0][4]=0.;
  dalpha[1][4]=0.; dbeta[1][4]=0.; dgamma[1][4]=0.;
  dalpha[2][4]=0.; dbeta[2][4]=0.; dgamma[2][4]=0.;
  dalpha[3][4]=0.; dbeta[3][4]=0.; dgamma[3][4]=0.;

  // Sector 5
  dx0[   0][5]=0.; dy0[  0][5]=0.; dz0[   0][5]=0.;
  dx0[   1][5]=0.; dy0[  1][5]=0.; dz0[   1][5]=0.;
  dx0[   2][5]=0.; dy0[  2][5]=0.; dz0[   2][5]=0.;
  dx0[   3][5]=0.; dy0[  3][5]=0.; dz0[   3][5]=0.;
  dalpha[0][5]=0.; dbeta[0][5]=0.; dgamma[0][5]=0.;
  dalpha[1][5]=0.; dbeta[1][5]=0.; dgamma[1][5]=0.;
  dalpha[2][5]=0.; dbeta[2][5]=0.; dgamma[2][5]=0.;
  dalpha[3][5]=0.; dbeta[3][5]=0.; dgamma[3][5]=0.;
  //***********************************************


  for(Int_t imdc=0;imdc<4;imdc++){
    for(Int_t isect=0;isect<6;isect++){
      x0[   imdc][isect] =  x0id[   imdc] + dx0[   imdc][isect];
      y0[   imdc][isect] =  y0id[   imdc] + dy0[   imdc][isect];
      z0[   imdc][isect] =  z0id[   imdc] + dz0[   imdc][isect];
      alpha[imdc][isect] =  alphaid[imdc] + dalpha[imdc][isect];
      beta[ imdc][isect] =  betaid[ imdc] + dbeta[ imdc][isect];
      gamma[imdc][isect] =  gammaid[imdc] + dgamma[imdc][isect];      
      // for alignment
      x0[   imdc][isect] += dxtemp[    imdc];
      y0[   imdc][isect] += dytemp[    imdc];
      z0[   imdc][isect] += dztemp[    imdc];
      alpha[imdc][isect] += dalphatemp[imdc];
      beta[ imdc][isect] += dbetatemp[ imdc];
      gamma[imdc][isect] += dgammatemp[imdc];
    }//end-for--isect--
  }//end-for--imdc--

}

Bool_t HRungeKutta::readMDCpositionsFromProgram(Int_t s, Int_t m, HGeomTransform gtrMod2Sec){
  hMDCgTRANS[  m][s] = gtrMod2Sec;
  mdcInstalled[m][s] = kTRUE;
  return kTRUE;
}


void HRungeKutta::debugMDCpositions(){
  printf("\n **************** debugging of MDC positions ***************\n");
  for(Int_t s=0; s<6; s++){ //-sector-loop--
     //if(s==0 || s==3){
     for(Int_t m=0; m<4; m++){
        const Double_t rad2deg = 57.29577951308;
        HGeomVector aEuler = findEulerAnglesASD(hMDCgTRANS[m][s].getRotMatrix());
        printf( "===================A==Euler angles=S%i======================\n",s);
        cout << "aEuler["<<m<<"]=" << aEuler.getX()*rad2deg<<","<<aEuler.getY()*rad2deg<<","<<aEuler.getZ()*rad2deg<<endl;
        printf( "======================------------=========================%i\n", mdcInstalled[m][s]);
        HGeomVector transV = hMDCgTRANS[m][s].getTransVector();
        printf( "===TranslationV={%f,%f,%f}\n",transV.getX(), transV.getY(), transV.getZ());
     }
     //}
  } //end-for-sector-loop--  
}


void HRungeKutta::applyMDCgeom(){ //instead of mdcgeom()
   /////////////////////////////////////////////////////
   // This function to be used instead of "mdcgeom()" //
   // works with ASCII defined geometry               //
   // or with user-defined absolute geometry          //
   /////////////////////////////////////////////////////
   for(Int_t m=0; m<4; m++){
   for(Int_t s=0; s<6; s++){
      //cout << "--mdcInstalled["<<m<<"]["<<s<<"]="<<mdcInstalled[m][s]<<endl;
      if( mdcInstalled[m][s] ){
        //cout << "[" << m << "][" <<s<< "]" << endl;
        //Eulers
        //+//printf("--[s=%i][m=%i]--\n",s,m);
        //+//hMDCgTRANS[m][s].print();
        HGeomVector aEuler = findEulerAnglesASD(hMDCgTRANS[m][s].getRotMatrix());
        //Translation shifts
        //warning - eulers has to be in degrees not in Radians...
        Double_t Dpi = 3.14159265358979323846;
        alpha[m][s] = aEuler.getX()*180./Dpi;
        beta[ m][s] = aEuler.getY()*180./Dpi;
        gamma[m][s] = aEuler.getZ()*180./Dpi;
        HGeomVector aTransl = hMDCgTRANS[m][s].getTransVector();
        x0[m][s]    = aTransl.getX();
        y0[m][s]    = aTransl.getY();
        z0[m][s]    = aTransl.getZ();
      }else{
        //for these MDCs whose geometry was not included in ASCII file with geometry
        //we do not change parameters => ideal geometry is used...
        x0[   m][s] =  x0id[   m];
        y0[   m][s] =  y0id[   m];
        z0[   m][s] =  z0id[   m];
        alpha[m][s] =  alphaid[m];
        beta[ m][s] =  betaid[ m];
        gamma[m][s] =  gammaid[m];
      }
   }  
   }  
}


HGeomVector HRungeKutta::findEulerAnglesASD(HGeomRotation rot){
  ////////////////////////////////////////////////////////////////////
  // From the relative rotation, get the euler angles (radians)     //
  //                                                                //
  // From an Euler rotation (see Dahlinger paper for the convention)// 
  // the euler angles are:                                          //
  // euler[0] = atan2(rot(5)/sin(euler[1]),rot(2)/sin(euler[1]))    //
  // euler[1] = acos (rot(8))    with possible change of sign       //
  // euler[2] = atan2(rot(7)/sin(euler[1]),-rot(6)/sin(euler[1]))   //
  //                                                                //
  // (c) taken from Hector Alvares Pol (GENP)                       //
  // modified by A.Sadovski 20.05.2004                              //
  // -------------------------------------------------------------- //
  // Problem: Euler transformations has error of 0.000002           //
  //          This could cause problems while aligning              //
  // -------------------------------------------------------------- //
  // See also /misc/halo/anal/v6_15/alignment/hmdcmetaaligner.cc    //
  ////////////////////////////////////////////////////////////////////
  const Double_t Dpi     = 3.14159265358979323846;
  Double_t euler[3];
  HGeomVector eul;

  //Checking if rot(8) is in the acos() domain
  if(rot(8)< 0.9999999 && rot(8)>-0.9999999) euler[1] = acos(rot(8));
  else euler[1]=0;
  Double_t sinaux;
  if(euler[1] == 0.){
    //euler[0] and euler[2] are equivalent. Write all in euler[0]
    euler[0]= (Dpi/2.)+acos(rot(0));
    euler[2]=-(Dpi/2.);
  } 
  else{  //IS AN EULER MATRIX
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
  }
  
  //if(MA_DEBUG>0){
  //  cout<<endl<<"Euler angles: "<<euler[0] << ", "<< euler[1]<<", "<<euler[2]<< endl;
  //}
  
  HGeomRotation test;  
  test.setEulerAngles(euler[0]*180./Dpi,
		      euler[1]*180./Dpi,
		      euler[2]*180./Dpi); 
  
  //if(MA_DEBUG>0){
  //  cout << endl <<"Rotation from Euler angles (first attemp): " << endl;
  //  test.print();
  //}
  
  //Now solving the problem when euler[1]<0
  Double_t eps = 0.00001; //check precision
  
  if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
      (fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ) {
    //if(MA_DEBUG>0){cout<<endl<<"Bad election in first euler angle! "<<"Trying again. "<<endl;}

    euler[1] = - euler[1]; 
    sinaux = sin(euler[1]);
    euler[0] = atan2(rot(5)/sinaux,rot(2)/sinaux);
    euler[2] = atan2(rot(7)/sinaux,-rot(6)/sinaux);
    
    test.setEulerAngles(euler[0]*180./Dpi,
			euler[1]*180./Dpi,
			euler[2]*180./Dpi);  
    
    //if(MA_DEBUG>0){
    //  cout << "Rotation from Euler angles (second attemp): " << endl;
    //  test.print();
    //}
    
    //testing if euler angles are rigth now
    if( (fabs(test(0)-rot(0))>eps) || (fabs(test(1)-rot(1))>eps) || 
	(fabs(test(3)-rot(3))>eps) || (fabs(test(4)-rot(4))>eps) ){
      cout << endl << "FATAL ERROR: Bad election in euler angles! "<< endl;
      cout << "Original rot matrix: "<< endl;
      rot.print();
      cout << "From obtained euler angles: " << endl;
      test.print();
      //What to do??
    }
  }
  eul.setX(euler[0]); 
  eul.setY(euler[1]); 
  eul.setZ(euler[2]); 
  return eul;
}

//---------------------------------------------------------------------------------------------------------------//
// auxiliary functions to read/write geometry of MDCs from/to ASCII file. If the user wants to force alignment   //
//---------------------------------------------------------------------------------------------------------------//

void HRungeKutta::writeGeomSnapshoftToHydraLikeFormSector(Int_t s){
  ///////////////////////////////////////////////////////////////////////////////////////
  // Service routine, for debugging or alignment.                                      //
  // This will copy MDC geometry of sector=s actually used inside Runge Kutta tracking //
  // into ASCII file rkMdcSectorGeom[%s].txt for the user private purposes.            //
  // WARNING: transformation is valid with precision only 0.000010                     //
  //          to store parameters we use better precision 0.000001 so be aware of this.//
  // USE THIS FUNCTION ON YOUR RISK due to this small error                            //
  ///////////////////////////////////////////////////////////////////////////////////////
  const Char_t* name[4][6] = {"DR1M1", "DR1M2", "DR1M3", "DR1M4", "DR1M5", "DR1M6",
                              "DR2M1", "DR2M2", "DR2M3", "DR2M4", "DR2M5", "DR2M6",
                              "DR3M1", "DR3M2", "DR3M3", "DR3M4", "DR3M5", "DR3M6",
                              "DR4M1", "DR4M2", "DR4M3", "DR4M4", "DR4M5", "DR4M6" };
  Char_t  outASCIIfile[56];
  sprintf(outASCIIfile, "rkMdcGeomSector%i.txt",s);

  FILE *outGeomFile = fopen( outASCIIfile , "w");
  if(!outGeomFile){ 
    printf("HRungeKutta: output geometry file [%s] can not be opened ...\n",outASCIIfile);
    return;
  }

  printf("HRungeKutta: writing current MDC geometry for sector=%i into [%s]\n", s, outASCIIfile);
  for(Int_t m=0; m<4; m++){
     //HGeomRotation hRot = hMDCgTRANS[m][s].getRotMatrix();
     //HGeomVector   hTra = hMDCgTRANS[m][s].getTransVector();

     //
     // Here we have to do a transformation from SectorCoordSystem (which is used in Runge Kutta internally)
     // into a LabCoordinateSystem (which is used to Store MDC geometry parameters in ASCII parameter file)
     //
     HGeomTransform mdcGeomTrans;

     const Double_t rad2deg = 57.29577951308;
     HGeomVector aEuler = findEulerAnglesASD(hMDCgTRANS[m][s].getRotMatrix());
     HGeomRotation rB;
     rB.setEulerAngles(aEuler.getX()*rad2deg+60.0*s, aEuler.getY()*rad2deg, aEuler.getZ()*rad2deg);
     mdcGeomTrans.setRotMatrix(rB);
     HGeomVector bEuler = findEulerAnglesASD(mdcGeomTrans.getRotMatrix());

     HGeomVector transV = hMDCgTRANS[m][s].getTransVector();
     Double_t r = sqrt(transV.getX()*transV.getX() + transV.getY()*transV.getY());
     Double_t a = acos(transV.getX()/r);
     if(transV.getY()<0){a = -a;} //taking into account that ARCCOS is defined on (0-:-180)
     a = a + s*60.0/rad2deg; //rotating the translation vector in x-y plane
     HGeomVector vRotated(r*cos(a),r*sin(a),transV.getZ());
     mdcGeomTrans.setTransVector(vRotated);

     HGeomRotation hRot = mdcGeomTrans.getRotMatrix();
     HGeomVector   hTra = mdcGeomTrans.getTransVector();


     fprintf(outGeomFile,"// ------------------------------------------------------\n");
     fprintf(outGeomFile,"%s  %s\n", name[m][s], name[m][0]);
     fprintf(outGeomFile,"%9.6f%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f%10.6f\n",hRot.getElement(0,0), hRot.getElement(0,1), hRot.getElement(0,2), hRot.getElement(1,0), hRot.getElement(1,1), hRot.getElement(1,2), hRot.getElement(2,0), hRot.getElement(2,1), hRot.getElement(2,2) );
     fprintf(outGeomFile,"%9.3f%10.3f%10.3f\n", hTra.getX(), hTra.getY(), hTra.getZ() );
  }
}

