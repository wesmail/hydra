#pragma implementation

#include <iostream.h>
#include "hades.h"
#include "hspectrometer.h"
/*
#include "hparhadasciifileio.h"
*/
#include "hlocation.h"
#include "hspectrometer.h"
#include "hmatchuunpackparam.h"

ClassImp(HMatchUUnpackParam)

HMatchUUnpackParam::HMatchUUnpackParam() {

// default initialization
  for (Int_t xx=0;xx<96;xx++) {
    for (Int_t yy=0;yy<96;yy++) {
      theta[xx][yy]=2000;
      phi[xx][yy]=2000;
    }
  }
  

  FILE *fp;
  Int_t xcell,ycell,count;
  Float_t th, ph; 
  Int_t i, file_status;

   
    fp = fopen("/u/alberica/xy_theta_phi_rich.txt","r");

    if(fp){
      Int_t count = 0;

     
     for(i=0; ( (i<5000) && (!feof(fp)) ); i++) {
       file_status = fscanf(fp,"%i%i%i",&xcell,&ycell,&count);
       if(i==4990) {
	 cout << "error: " << "x:" << xcell << " y:" << ycell << " count:" << count << endl;
       }

       theta[xcell][ycell] = 1000;
       phi[xcell][ycell] = 1000;

       if (count != 0) {
	 file_status = fscanf(fp,"%f%f",&th,&ph);
	 if((xcell>-1) && (xcell <100) && (ycell>-1) && (ycell<100)) {
	   theta[xcell][ycell] = th;
	   phi[xcell][ycell] = ph;
	 }
       }
     }

     fclose(fp);
    }

//-------------------------------------------------------------------
// default initialization
for (Int_t xx=0;xx<32;xx++) {
  for (Int_t yy=0;yy<32;yy++) {
    theta_sh[xx][yy]=2000;
    phi_sh[xx][yy]=2000;
  }
}

FILE *fpSh;
Int_t ccellSh,rcellSh;
Float_t thetaSh, phiSh;           

Int_t j, fileSh_status;

fpSh = fopen("/u/alberica/xy_theta_phi_shower.txt","r");
if(fpSh){
  for(j=0; j<1025; j++) {
    fileSh_status = fscanf(fpSh,"%i%i%f%f",&ccellSh,&rcellSh,&phiSh,&thetaSh);
    
    if (phiSh>300.0) {
      phiSh-=330.0;
    }
    else if(phiSh<40) {
      phiSh+=30; 
    }
    
    theta_sh[ccellSh][rcellSh] = 1000;
    phi_sh[ccellSh][rcellSh] = 1000;
    
    if((ccellSh>-1) && (ccellSh <32) && (rcellSh>-1) && (rcellSh<32)) {
      theta_sh[ccellSh][rcellSh] = thetaSh;
      phi_sh[ccellSh][rcellSh] = phiSh;
    }
  }
  
  if(fpSh) fclose(fpSh);
  
}
}  

HMatchUUnpackParam::~HMatchUUnpackParam()  {
}














