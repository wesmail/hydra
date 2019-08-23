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
  FILE *fp;
  Int_t xcell,ycell,count;
  Float_t th, ph; 
  Int_t i, file_status;

   
    fp = fopen("/u/alberica/xy_theta_phi_rich.txt","r");

    if(fp){
      Int_t count = 0;
     for(i=0; i<5000; i++) {
        file_status = fscanf(fp,"%i%i%i",&xcell,&ycell,&count);
        if (count==0) 
  	i++;
        else
  	file_status = fscanf(fp,"%f%f",&th,&ph);
	if((xcell>-1) && (xcell <100) && (ycell>-1) && (ycell<100)) {
  	theta[xcell][ycell] = th;
  	phi[xcell][ycell] = ph;
        }
      }
      fclose(fp);
    }
}  

HMatchUUnpackParam::~HMatchUUnpackParam()  {
  }













