//*-- AUTHOR : Olga Pechenova
//*-- Modified : 2006 by Olga Pechenova
//
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HEnergyLossCorrPar
//
//  Container class keep parameters for energy loss correction
//
//
//  Float_t getDeltaMom(Int_t pId, Float_t mom, Float_t theta); 
//     Function getDeltaMom returns the correction value (delP) for momentum.
//     Parameters: - pId is particle ID. If parameter pId is not correct
//                 function getDeltaMom returns 0.
//                 - mom is Runge-Kutta reconstructed momentum (MeV).
//                 - theta is theta angle in the interval [15;87] (degrees).
//                 If parameter theta is out of this interval    
//                 function getDeltaMom returns 0.
//
//  Float_t getCorrMom(Int_t pId, Float_t mom, Float_t theta) {
//          return mom+getDeltaMom(pId,mom,theta);}
//     Function getCorrMom returns the corrected momentum.
//     Parameters have the same meanings as for function getDeltaMom.
//
//
//  Alternative method of getting parameters (without usage of runTimeDb):
//
//  Bool_t  setDefaultPar(const Char_t *rn="jan04");
//     Function setDefaultPar sets default values to parameters
//     for corresponding beam team.
//
//  Example:
//        HEnergyLossCorrPar enLossCorr;
//        enLossCorr.setDefaultPar("jan04"); // or "nov02", or "aug04"
//        Float_t delP;
//        ..............................
//        beginning of your loop;
//          ..............................................
//          delP = enLossCorr.getDeltaMom(14, mom, theta);
//          ..............................................
//        end of your loop;
/////////////////////////////////////////////////////////////////////////


#include <TString.h>
#include "henergylosscorrpar.h"
#include "hpidphysicsconstants.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"

#include <stdlib.h> 

ClassImp(HEnergyLossCorrPar)

HEnergyLossCorrPar::HEnergyLossCorrPar(const Char_t* name,const Char_t* title,
                const Char_t* context)
           : HParCond(name,title,context) {
  // constructor
  momInt1=720.;
  momInt2=2000.;
  nThetaReg=7;
  nParamsProt=10;
  thetaStep=10.;
  thetaMiddle0=23.;
  for(Int_t i=0;i<nThetaReg*nParamsProt;i++) parMomCorrProt[i]=0.;
// for electrons
  nParamsElect=5;
  for(Int_t i=0;i<nParamsElect;i++) parMomCorrElect[i]=0.;
}

Bool_t HEnergyLossCorrPar::init(HParIo* inp,Int_t* set) {
 // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

void HEnergyLossCorrPar::clear(void) {
  for(Int_t i=0;i<nThetaReg*nParamsProt;i++) parMomCorrProt[i]=0.;
  for(Int_t i=0;i<nParamsElect;i++) parMomCorrElect[i]=0.;
  status=kFALSE;
  resetInputVersions();
  changed=kFALSE;
}

Int_t HEnergyLossCorrPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return 0;
}


void HEnergyLossCorrPar::putParams(HParamList* l) {
  // puts all parameters to the parameter list, which is used by the io
  if (!l) return;
  l->add("momInt1",        momInt1);
  l->add("momInt2",        momInt2);
  l->add("nThetaReg",      nThetaReg);
  l->add("nParamsProt",    nParamsProt);
  l->addBinary("parMomCorrProt", parMomCorrProt,nThetaReg*nParamsProt);
  l->add("thetaMiddle0",   thetaMiddle0);
  l->add("thetaStep",      thetaStep);

  l->add("nParamsElect",    nParamsElect);
  l->addBinary("parMomCorrElect", parMomCorrElect,nParamsElect);
}


Bool_t HEnergyLossCorrPar::getParams(HParamList* l) {
  // gets all parameters from the parameter list, which is used by the io
  if (!l) return kFALSE;
  if (!(l->fill("momInt1",            &momInt1)))          return kFALSE;
  if (!(l->fill("momInt2",            &momInt2)))          return kFALSE;
  if (!(l->fill("nThetaReg",          &nThetaReg)))        return kFALSE;
  if (!(l->fill("nParamsProt",        &nParamsProt)))      return kFALSE;
  if (!(l->fillBinary("parMomCorrProt",parMomCorrProt,nThetaReg*nParamsProt)))   return kFALSE;
  if (!(l->fill("thetaMiddle0",       &thetaMiddle0)))     return kFALSE;
  if (!(l->fill("thetaStep",          &thetaStep)))        return kFALSE;

  if (!(l->fill("nParamsElect",       &nParamsElect)))     return kFALSE;
  if (!(l->fillBinary("parMomCorrElect",parMomCorrElect,nParamsElect)))   return kFALSE;
  return kTRUE;
}  

Bool_t HEnergyLossCorrPar::setDefaultPar(const Char_t *rn) {
  TString run=rn;
  if(run=="nov02") {
    momInt1=600.;
    momInt2=2000.;
    nThetaReg=7;
    thetaStep=10.;
    thetaMiddle0=23.;
    nParamsProt=10;
    // 1st theta interval
    parMomCorrProt[0]=0.0506414;parMomCorrProt[1]=-68.332;parMomCorrProt[2]=30017.5;
    parMomCorrProt[3]=87.1936;parMomCorrProt[4]=-0.00405782;parMomCorrProt[5]=-0.000841998;parMomCorrProt[6]=4.52039;
    parMomCorrProt[7]=5.37248;parMomCorrProt[8]=-0.00121608;parMomCorrProt[9]=2.4008;
    // 2nd theta interval
    parMomCorrProt[10]=0.056843;parMomCorrProt[11]=-73.7507;parMomCorrProt[12]=30890.6;
    parMomCorrProt[13]=85.2889;parMomCorrProt[14]=-0.00406022;parMomCorrProt[15]=-0.000823641;parMomCorrProt[16]=4.41504;
    parMomCorrProt[17]=3.97163;parMomCorrProt[18]=-0.00105256;parMomCorrProt[19]=2.3175;
    // 3rd theta interval
    parMomCorrProt[20]=0.0692018;parMomCorrProt[21]=-85.0326;parMomCorrProt[22]=33230.6;
    parMomCorrProt[23]=86.0512;parMomCorrProt[24]=-0.0040888;parMomCorrProt[25]=-0.000838878;parMomCorrProt[26]=4.39438;
    parMomCorrProt[27]=4.30892;parMomCorrProt[28]=-0.00110695;parMomCorrProt[29]=2.29046;
    // 4th theta interval
    parMomCorrProt[30]=0.05393;parMomCorrProt[31]=-72.6496;parMomCorrProt[32]=31345.2;
    parMomCorrProt[33]=92.3181;parMomCorrProt[34]=-0.00414646;parMomCorrProt[35]=-0.000836083;parMomCorrProt[36]=4.41321;
    parMomCorrProt[37]=4.73116;parMomCorrProt[38]=-0.00115162;parMomCorrProt[39]=2.30843;
    // 5th theta interval
    parMomCorrProt[40]=0.052317;parMomCorrProt[41]=-70.9782;parMomCorrProt[42]=31023.3;
    parMomCorrProt[43]=94.4471;parMomCorrProt[44]=-0.00416224;parMomCorrProt[45]=-0.000882047;parMomCorrProt[46]=4.56496;
    parMomCorrProt[47]=5.04969;parMomCorrProt[48]=-0.00117343;parMomCorrProt[49]=2.36712;
    // 6th theta interval
    parMomCorrProt[50]=0.0524236;parMomCorrProt[51]=-71.4746;parMomCorrProt[52]=31505.2;
    parMomCorrProt[53]=97.8558;parMomCorrProt[54]=-0.00416946;parMomCorrProt[55]=-0.000919853;parMomCorrProt[56]=4.75295;
    parMomCorrProt[57]=5.53359;parMomCorrProt[58]=-0.00120439;parMomCorrProt[59]=2.46908;
    // 7th theta interval
    parMomCorrProt[60]=0.0532758;parMomCorrProt[61]=-72.6315;parMomCorrProt[62]=32132.7;
    parMomCorrProt[63]=100.503;parMomCorrProt[64]=-0.00416881;parMomCorrProt[65]=-0.000962655;parMomCorrProt[66]=4.95385;
    parMomCorrProt[67]=5.78055;parMomCorrProt[68]=-0.00120932;parMomCorrProt[69]=2.57004;
		 
    //for electrons
    nParamsElect=5;
    parMomCorrElect[0]=2.29512;
    parMomCorrElect[1]=0.042546;
    parMomCorrElect[2]=-0.00175886;
    parMomCorrElect[3]=2.47346/100000;
    parMomCorrElect[4]=-1.0764/10000000;

    return kTRUE;
  } else if(run=="jan04") {
    momInt1=720.;
    momInt2=2000.;
    nThetaReg=7;
    thetaStep=10.;
    thetaMiddle0=23.;
    nParamsProt=10;

    // 1st theta interval
    parMomCorrProt[0]=0.0404892;parMomCorrProt[1]=-63.2212;parMomCorrProt[2]=31512.3;
    parMomCorrProt[3]=88.7387;parMomCorrProt[4]=-0.00383467;parMomCorrProt[5]=-0.000831376;parMomCorrProt[6]=4.83807;
    parMomCorrProt[7]=8.32566;parMomCorrProt[8]=-0.00134054;parMomCorrProt[9]=2.69398;
    // 2nd theta interval
    parMomCorrProt[10]=0.0392893;parMomCorrProt[11]=-61.712;parMomCorrProt[12]=31187;
    parMomCorrProt[13]=67.067;parMomCorrProt[14]=-0.00344743;parMomCorrProt[15]=-0.000770298;parMomCorrProt[16]=4.73321;
    parMomCorrProt[17]=4.08464;parMomCorrProt[18]=-0.00100119;parMomCorrProt[19]=2.70498;
    // 3rd theta interval
    parMomCorrProt[20]=0.0402474;parMomCorrProt[21]=-63.1836;parMomCorrProt[22]=31819.4;
    parMomCorrProt[23]=95.3604;parMomCorrProt[24]=-0.00391875;parMomCorrProt[25]=-0.000930825;parMomCorrProt[26]=5.09812;
    parMomCorrProt[27]=4.57863;parMomCorrProt[28]=-0.00103624;parMomCorrProt[29]=2.68722;
    // 4th theta interval
    parMomCorrProt[30]=0.0421779;parMomCorrProt[31]=-64.9599;parMomCorrProt[32]=31849.2;
    parMomCorrProt[33]=85.6957;parMomCorrProt[34]=-0.00379927;parMomCorrProt[35]=-0.000812833;parMomCorrProt[36]=4.78868;
    parMomCorrProt[37]=9.17729;parMomCorrProt[38]=-0.00138972;parMomCorrProt[39]=2.64987;
    // 5th theta interval
    parMomCorrProt[40]=0.0417347;parMomCorrProt[41]=-64.2374;parMomCorrProt[42]=31548.9;
    parMomCorrProt[43]=97.1928;parMomCorrProt[44]=-0.00399476;parMomCorrProt[45]=-0.000827159;parMomCorrProt[46]=4.80468;
    parMomCorrProt[47]=6.59782;parMomCorrProt[48]=-0.00125117;parMomCorrProt[49]=2.63429;
    // 6th theta interval
    parMomCorrProt[50]=0.0404892;parMomCorrProt[51]=-63.2212;parMomCorrProt[52]=31512.3;
    parMomCorrProt[53]=88.7387;parMomCorrProt[54]=-0.00383467;parMomCorrProt[55]=-0.000831377;parMomCorrProt[56]=4.83807;
    parMomCorrProt[57]=8.32566;parMomCorrProt[58]=-0.00134054;parMomCorrProt[59]=2.69398;
    // 7th theta interval
    parMomCorrProt[60]=0.0396831;parMomCorrProt[61]=-62.4906;parMomCorrProt[62]=31610.8;
    parMomCorrProt[63]=86.3941;parMomCorrProt[64]=-0.00374661;parMomCorrProt[65]=-0.000816029;parMomCorrProt[66]=4.88749;
    parMomCorrProt[67]=4.51922;parMomCorrProt[68]=-0.00104096;parMomCorrProt[69]=2.73135;

    //for electrons
    nParamsElect=5;
    parMomCorrElect[0]=2.21734; 
    parMomCorrElect[1]=0.0794622; 
    parMomCorrElect[2]=-0.00258538;
    parMomCorrElect[3]=3.03896/100000; 
    parMomCorrElect[4]=-1.11898/10000000;
    
    return kTRUE;
  } else if(run=="aug04") {
    momInt1=800.;
    momInt2=2000.;
    nThetaReg=7;
    thetaStep=10.;
    thetaMiddle0=23.;
    nParamsProt=10;
    // 1st theta interval
    parMomCorrProt[0]=0.0319386;parMomCorrProt[1]=-52.3932;parMomCorrProt[2]=28573.9;
    parMomCorrProt[3]=65.0824;parMomCorrProt[4]=-0.00340829;parMomCorrProt[5]=-0.000751249;parMomCorrProt[6]=4.9188;
    parMomCorrProt[7]=7.01293;parMomCorrProt[8]=-0.00120289;parMomCorrProt[9]=2.87892;
    // 2nd theta interval
    parMomCorrProt[10]=0.0354272;parMomCorrProt[11]=-57.1894;parMomCorrProt[12]=30552.1;
    parMomCorrProt[13]=73.0873;parMomCorrProt[14]=-0.00349743;parMomCorrProt[15]=-0.000814299;parMomCorrProt[16]=5.21396;
    parMomCorrProt[17]=5.66944;parMomCorrProt[18]=-0.00108139;parMomCorrProt[19]=2.99888;
    // 3rd theta interval
    parMomCorrProt[20]=0.0369929;parMomCorrProt[21]=-59.2908;parMomCorrProt[22]=31560.5;
    parMomCorrProt[23]=74.1808;parMomCorrProt[24]=-0.00343821;parMomCorrProt[25]=-0.000746043;parMomCorrProt[26]=5.2487;
    parMomCorrProt[27]=5.80041;parMomCorrProt[28]=-0.00111498;parMomCorrProt[29]=3.16789;
    // 4th theta interval
    parMomCorrProt[30]=0.0381148;parMomCorrProt[31]=-62.3502;parMomCorrProt[32]=34058.2;
    parMomCorrProt[33]=74.2051;parMomCorrProt[34]=-0.00327892;parMomCorrProt[35]=-0.000714812;parMomCorrProt[36]=5.57025;
    parMomCorrProt[37]=7.58451;parMomCorrProt[38]=-0.00115899;parMomCorrProt[39]=3.49298;
    // 5th theta interval
    parMomCorrProt[40]=0.0405264;parMomCorrProt[41]=-66.253;parMomCorrProt[42]=36385.7;
    parMomCorrProt[43]=80.9644;parMomCorrProt[44]=-0.00328172;parMomCorrProt[45]=-0.00073345;parMomCorrProt[46]=6.02206;
    parMomCorrProt[47]=5.72358;parMomCorrProt[48]=-0.000985652;parMomCorrProt[49]=3.77823;
    // 6th theta interval
    parMomCorrProt[50]=0.0442906;parMomCorrProt[51]=-72.6287;parMomCorrProt[52]=39923.8;
    parMomCorrProt[53]=112.382;parMomCorrProt[54]=-0.00364194;parMomCorrProt[55]=-0.00117441;parMomCorrProt[56]=7.26952;
    parMomCorrProt[57]=10.0128;parMomCorrProt[58]=-0.00119632;parMomCorrProt[59]=4.12544;
    // 7th theta interval
    parMomCorrProt[60]=0.0419634;parMomCorrProt[61]=-70.9195;parMomCorrProt[62]=40627.6;
    parMomCorrProt[63]=122.213;parMomCorrProt[64]=-0.00366415;parMomCorrProt[65]=-0.00126151;parMomCorrProt[66]=7.75827;
    parMomCorrProt[67]=9.35643;parMomCorrProt[68]=-0.00113441;parMomCorrProt[69]=4.36657;
		
    //for electrons
    nParamsElect=5;
    parMomCorrElect[0]=3.21963;
    parMomCorrElect[1]=-0.0257486;
    parMomCorrElect[2]=0.000407469;
    parMomCorrElect[3]=9.67088e-06;
    parMomCorrElect[4]=-9.93232e-08;

    return kTRUE;
  }
  return kFALSE;
}


Bool_t  HEnergyLossCorrPar::setParamProt(Int_t nTh, Int_t nParPr, Float_t thM0, Float_t thSt, 
        Float_t* parPr, Float_t mInt1, Float_t mInt2) {
  if(nTh<=0 || nParPr<=0 || nTh*nParPr>nThetaReg*nParamsProt) return kFALSE;
  momInt1=mInt1;
  momInt2=mInt2;
  nThetaReg=nTh;
  nParamsProt=nParPr;
  for(Int_t i=0; i<nThetaReg*nParamsProt; i++) parMomCorrProt[i]=parPr[i];
  thetaMiddle0=thM0;
  thetaStep=thSt;
  return kTRUE;
}

Bool_t  HEnergyLossCorrPar::setParamElect(Int_t nParEl, Float_t* parEl) {
  if(nParEl<=0 || nParEl>nParamsElect) return kFALSE;
  nParamsElect=nParEl;
  for(Int_t i=0; i<nParamsElect; i++) parMomCorrElect[i]=parEl[i];
  return kTRUE;
}

Float_t HEnergyLossCorrPar::getDeltaMom(Int_t pId, Float_t mom, Float_t theta) {

  Float_t delP=0.;
  if(theta<15.||theta>87.) return 0.;

  if(pId==2||pId==3) { 
    if(mom<50.) return 0.;
    // 4th order polynomial
    delP = parMomCorrElect[0]+theta*(parMomCorrElect[1]+theta*(parMomCorrElect[2]+theta*(parMomCorrElect[3]+parMomCorrElect[4]*theta)));
  }
  else {

    Float_t pMom, K[nParamsProt];
    Float_t mPr=HPidPhysicsConstants::mass(14);
    Float_t mPart = HPidPhysicsConstants::mass(pId);
    Int_t   chg   = HPidPhysicsConstants::charge(pId);
    if(mPart<=0.||(chg!=1&&chg!=-1)) return 0.;

    if(pId==14) pMom=mom;
    else pMom = sqrt(pow(mPr/mPart*(sqrt(mom*mom+mPart*mPart)-mPart)+mPr,2)-pow(mPr,2)); 

    if(pMom<170.) return 0.;
    
    Int_t i=int((theta-thetaMiddle0)/thetaStep+1);
    if(i==nThetaReg) i=nThetaReg-1;

    Int_t j;

    if(pMom<momInt1) j=0; else if(pMom<momInt2) j=3; else j=7;
    if(i==0) {
    // first theta interval
      Float_t thVal = (thetaMiddle0-theta)/thetaStep;
      K[j]   = parMomCorrProt[j] - (parMomCorrProt[nParamsProt+j]-parMomCorrProt[j])*thVal;
      K[j+1] = parMomCorrProt[j+1] - (parMomCorrProt[nParamsProt+j+1]-parMomCorrProt[j+1])*thVal;
      K[j+2] = parMomCorrProt[j+2] - (parMomCorrProt[nParamsProt+j+2]-parMomCorrProt[j+2])*thVal;
      if(pMom>momInt1 && pMom<momInt2) 
        K[j+3] = parMomCorrProt[j+3] - (parMomCorrProt[nParamsProt+j+3]-parMomCorrProt[j+3])*thVal;

//      K[j]=parMomCorrProt[j]; K[j+1]=parMomCorrProt[j+1]; K[j+2]=parMomCorrProt[j+2];
//      if(pMom>momInt1&&pMom<momInt2) K[j+3]=parMomCorrProt[j+3];
    }//i=0
    else {
      Int_t bound1 = (i-1)*nParamsProt;
      Int_t bound2 = i*nParamsProt;
      Float_t thInsideInt = (theta-(thetaMiddle0+thetaStep*(i-1)))/thetaStep;
      if(i==nThetaReg-1) {
      // last theta interval
        K[j]=parMomCorrProt[bound2+j] + (parMomCorrProt[bound2+j]-parMomCorrProt[bound1+j])*thInsideInt;
        K[j+1]=parMomCorrProt[bound2+j+1] + (parMomCorrProt[bound2+j+1]-parMomCorrProt[bound1+j+1])*thInsideInt;
        K[j+2]=parMomCorrProt[bound2+j+2] + (parMomCorrProt[bound2+j+2]-parMomCorrProt[bound1+j+2])*thInsideInt;
        if(pMom>momInt1&&pMom<momInt2) 
          K[j+3]=parMomCorrProt[bound2+j+3] + (parMomCorrProt[bound2+j+3]-parMomCorrProt[bound1+j+3])*thInsideInt;
      }
      else {
      // theta intervals excluding first and last
        K[j]=parMomCorrProt[bound1+j] + (parMomCorrProt[bound2+j]-parMomCorrProt[bound1+j])*thInsideInt;
        K[j+1]=parMomCorrProt[bound1+j+1] + (parMomCorrProt[bound2+j+1]-parMomCorrProt[bound1+j+1])*thInsideInt;
        K[j+2]=parMomCorrProt[bound1+j+2] + (parMomCorrProt[bound2+j+2]-parMomCorrProt[bound1+j+2])*thInsideInt;
        if(pMom>momInt1&&pMom<momInt2) 
          K[j+3]=parMomCorrProt[bound1+j+3] + (parMomCorrProt[bound2+j+3]-parMomCorrProt[bound1+j+3])*thInsideInt;
      } //else
    } //else

    if(pMom<momInt1) return K[j]*pMom+K[j+1]+K[j+2]/pMom;
    if(pMom<momInt2) return K[j]*exp(K[j+1]*pMom)+K[j+2]*pMom+K[j+3];
    return K[j]*exp(K[j+1]*pMom)+K[j+2];
  } // else not electrons

  return delP;
}
