// File: hrichlocalmaxheader.cc
//
// Author: Laura Fabbietti <L.Fabbietti@physik.tu-muenchen.de>
// Last update by Laura Fabbietti: 02/02/20 21:33:54
//
using namespace std;
#include "hrichlocalmaxheader.h" 
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
ClassImp(HRichLocalMaxHeader)

HRichLocalMaxHeader::HRichLocalMaxHeader(){
    iEventFlag=0;
    fMeanThetaPad1=0;
    fMeanThetaPad2=0;
    fMeanThetaLoc1=0;
    fMeanThetaLoc2=0;
   
}
void HRichLocalMaxHeader::setSecMult(Float_t sec[6]){
    multSec0 =sec[0]; 
    multSec1=sec[1]; 
    multSec2=sec[2]; 
    multSec3=sec[3]; 
    multSec4=sec[4]; 
    multSec5=sec[5]; 
    //  cout<<" "<<multSec0<<"  "<<multSec1<<"  "<<multSec2<<" "<<multSec3<<" "<<multSec4<<" "<<multSec5<<endl;
}
void HRichLocalMaxHeader::getSecMult(Float_t mult[6]){
    mult[0] = multSec0;
    mult[1] = multSec1;
    mult[2] = multSec2;
    mult[3] = multSec3;
    mult[4] = multSec4;
    mult[5] = multSec5;
}
void HRichLocalMaxHeader::setMeanChargeSec(Float_t mC[6]){
    meanChargeSec0=mC[0];
    meanChargeSec1=mC[1];
    meanChargeSec2=mC[2];
    meanChargeSec3=mC[3];
    meanChargeSec4=mC[4];
    meanChargeSec5=mC[5];
}
void HRichLocalMaxHeader::getMeanChargeSec(Float_t mC[6]){
    mC[0]=meanChargeSec0;
    mC[1]=meanChargeSec1;
    mC[2]=meanChargeSec2;
    mC[3]=meanChargeSec3;
    mC[4]=meanChargeSec4;
    mC[5]=meanChargeSec5;
}
