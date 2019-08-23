//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck
//*-- Modified : 26/01/2006 by T. Perez
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////
//
// HHypKineRefitUtils
//
// Some auxiliar functions for the Kinematic refit. 
// NEED TO BE CHANGED:
// 1. Some of the functions do already exist in ROOT, why not to use them.
//    -> All kinematic variables can be prduced from TLorentzVector easier.
// 2. Now there is a series of different functions for each channel.
//    -> Rewrite the functions to have only 1 wich accepts as parameter
//       The 4 particles + missing particle.
// 3. init(2200.) What is that!!!
//    -> EVEN THE BEAM ENERGY IS HARDCODED HERE! in the hyp frameworok 
//       there is a pointer TLorentzVector *beam, containig the beam params. 
////////////////////////////////////////////////////////////////////////

#include "hhypKineRefitUtils.h"

using namespace std;

HHypKineRefitUtils::HHypKineRefitUtils()
{
  init(2200.);
}

HHypKineRefitUtils::HHypKineRefitUtils(Float_t eB)
{
  init(eB);
}

Float_t HHypKineRefitUtils::calcFromKin(Float_t theta)
{
  Float_t KinP=pInput/(cos(theta)+GammaF*GammaF*sin(theta)*tan(theta));
  return KinP;
}


// void HHypKineRefitUtils::calcFConserv(Double_t *input,Float_t *errInput, Float_t *F,Float_t *errF)
// {
//   HGeomVector data[4];
//   Float_t Ener[4];

//   for(Int_t i=0; i<4; i++)
//     {
//       data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
//     }  
  
//   F[0]=data[0].getX()+data[1].getX()+data[2].getX()+data[3].getX()-AllX;
//   F[1]=data[0].getY()+data[1].getY()+data[2].getY()+data[3].getY()-AllY;
//   F[2]=data[0].getZ()+data[1].getZ()+data[2].getZ()+data[3].getZ()-AllZ;
//   F[3]=0;
//   Ener[0]=sqrt(data[0].length()*data[0].length()+mProton*mProton);
//   Ener[1]=sqrt(data[1].length()*data[1].length()+mProton*mProton);
//   Ener[2]=sqrt(data[2].length()*data[2].length()+mPion*mPion);
//   Ener[3]=sqrt(data[3].length()*data[3].length()+mPion*mPion);  
  
//   for(Int_t i=0; i<4; i++)
//     {
//       F[3]+=Ener[i];
//       errF[i]=0;
//     }
//   F[3]-=AllE;
  
//   for(Int_t i=0; i<4; i++)
//     {
//       errF[0]+=TMath::Power(sin(input[3*i+1])*cos(input[3*i+2])*errInput[3*i],2)+
// 	TMath::Power(input[3*i]*cos(input[3*i+1])*cos(input[3*i+2])*errInput[3*i+1],2)+
// 	TMath::Power(input[3*i]*sin(input[3*i+1])*sin(input[3*i+2])*errInput[3*i+2],2);
      
//       errF[1]+=TMath::Power(sin(input[3*i+1])*sin(input[3*i+2])*errInput[3*i],2)+
// 	TMath::Power(input[3*i]*cos(input[3*i+1])*sin(input[3*i+2])*errInput[3*i+1],2)+
// 	TMath::Power(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2])*errInput[3*i+2],2);
      
//       errF[2]+=TMath::Power(cos(input[3*i+1])*errInput[3*i],2)+
// 	TMath::Power(input[3*i]*sin(input[3*i+1])*errInput[3*i+1],2);
      
//       errF[3]+=(TMath::Power(input[3*i]/Ener[i]*errInput[3*i],2));
      
//     }


// }

void HHypKineRefitUtils::calcFConservElec(Double_t *input,Float_t *errInput, Float_t *F,Float_t *errF)
{
  HGeomVector data[4];
  Float_t Ener[4];

  for(Int_t i=0; i<4; i++)
    {
      data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
    }  
  
  F[0]=data[0].getX()+data[1].getX()+data[2].getX()+data[3].getX()-AllX;
  F[1]=data[0].getY()+data[1].getY()+data[2].getY()+data[3].getY()-AllY;
  F[2]=data[0].getZ()+data[1].getZ()+data[2].getZ()+data[3].getZ()-AllZ;
  F[3]=0;
  Ener[0]=sqrt(data[0].length()*data[0].length()+mProton*mProton);
  Ener[1]=sqrt(data[1].length()*data[1].length()+mProton*mProton);
  Ener[2]=sqrt(data[2].length()*data[2].length()+mElec*mElec);
  Ener[3]=sqrt(data[3].length()*data[3].length()+mElec*mElec);  
  
  for(Int_t i=0; i<4; i++)
    {
      F[3]+=Ener[i];
      errF[i]=0;
    }
  F[3]-=AllE;
  
  for(Int_t i=0; i<4; i++)
    {
      errF[0]+=TMath::Power(sin(input[3*i+1])*cos(input[3*i+2])*errInput[3*i],2)+
	TMath::Power(input[3*i]*cos(input[3*i+1])*cos(input[3*i+2])*errInput[3*i+1],2)+
	TMath::Power(input[3*i]*sin(input[3*i+1])*sin(input[3*i+2])*errInput[3*i+2],2);
      
      errF[1]+=TMath::Power(sin(input[3*i+1])*sin(input[3*i+2])*errInput[3*i],2)+
	TMath::Power(input[3*i]*cos(input[3*i+1])*sin(input[3*i+2])*errInput[3*i+1],2)+
	TMath::Power(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2])*errInput[3*i+2],2);
      
      errF[2]+=TMath::Power(cos(input[3*i+1])*errInput[3*i],2)+
	TMath::Power(input[3*i]*sin(input[3*i+1])*errInput[3*i+1],2);
      
      errF[3]+=(TMath::Power(input[3*i]/Ener[i]*errInput[3*i],2));
      
    }
}

void HHypKineRefitUtils::init(Float_t eB)
{
  eBeam=eB;
  mProton=938.27200;
  mPion=139.57018;
  mElec=0.511;
  ELab=eB+mProton;
  pInput=sqrt(ELab*ELab-mProton*mProton);
  ECM=sqrt(2*mProton*mProton+2*ELab*mProton);
  GammaF=(ELab+mProton)/ECM;
  BetaF=pInput/(ELab+mProton);
  AllE=ELab+mProton;
  AllX=0;
  AllY=0;
  AllZ=pInput;
}

Float_t HHypKineRefitUtils::scalar(HGeomVector &data1, HGeomVector &data2)
{
   return (data1.getX()*data2.getX()+data1.getY()*data2.getY()+data1.getZ()*data2.getZ());
}

Float_t HHypKineRefitUtils::missing2(HGeomVector &data3,HGeomVector &data4)
{
  Float_t Etotal=eBeam+mProton;
  Float_t P1=sqrt(Etotal*Etotal-mProton*mProton);
  Float_t P2=0.;
  Float_t P1z=P1;
  HGeomVector dataaa1;
  dataaa1.setXYZ(0,0,P1z);
  Float_t E1=sqrt(P1*P1+mProton*mProton);
  Float_t E2=mProton;
  Float_t P3=sqrt(data3.getX()*data3.getX()+data3.getY()*data3.getY()+data3.getZ()*data3.getZ());
  Float_t P4=sqrt(data4.getX()*data4.getX()+data4.getY()*data4.getY()+data4.getZ()*data4.getZ());
  Float_t E3=sqrt(P3*P3+mProton*mProton);
  Float_t E4=sqrt(P4*P4+mProton*mProton);
  
  Float_t totalESq=(E1+E2-E3-E4)*(E1+E2-E3-E4);
  Float_t totalPSq=P1*P1+P2*P2+P4*P4+P3*P3-2*scalar(dataaa1,data3)-2*scalar(dataaa1,data4)+2*scalar(data3,data4);
  return (totalESq-totalPSq);
}

Float_t HHypKineRefitUtils::missing2Elec(HGeomVector &data3,HGeomVector &data4)
{
  Float_t Etotal=eBeam+mProton;
  Float_t P1=sqrt(Etotal*Etotal-mProton*mProton);
  Float_t P2=0.;
  Float_t P1z=P1;
  HGeomVector dataaa1;
  dataaa1.setXYZ(0,0,P1z);
  Float_t E1=sqrt(P1*P1+mProton*mProton);
  Float_t E2=mProton;
  Float_t P3=sqrt(data3.getX()*data3.getX()+data3.getY()*data3.getY()+data3.getZ()*data3.getZ());
  Float_t P4=sqrt(data4.getX()*data4.getX()+data4.getY()*data4.getY()+data4.getZ()*data4.getZ());
  Float_t E3=sqrt(P3*P3+mProton*mProton);
  Float_t E4=sqrt(P4*P4+mProton*mProton);
  
  Float_t totalESq=(E1+E2-E3-E4)*(E1+E2-E3-E4);
  Float_t totalPSq=P1*P1+P2*P2+P4*P4+P3*P3-2*scalar(dataaa1,data3)-2*scalar(dataaa1,data4)+2*scalar(data3,data4);
  return (totalESq-totalPSq);
}

Float_t HHypKineRefitUtils::missing4(HGeomVector &data3,HGeomVector &data4,HGeomVector &data5,HGeomVector &data6)
{
   Float_t Etotal=eBeam+mProton;
   Float_t P1=sqrt(Etotal*Etotal-mProton*mProton);
   Float_t P2=0.; 
   Float_t P1z=P1;
   Float_t E1=sqrt(P1*P1+mProton*mProton);
   Float_t E2=mProton;
   
   HGeomVector dataa1;
   dataa1.setXYZ(0.,0.,P1z);
   
   Float_t P3=sqrt(data3.getX()*data3.getX()+data3.getY()*data3.getY()+data3.getZ()*data3.getZ());
   Float_t P4=sqrt(data4.getX()*data4.getX()+data4.getY()*data4.getY()+data4.getZ()*data4.getZ());
   Float_t P5=sqrt(data5.getX()*data5.getX()+data5.getY()*data5.getY()+data5.getZ()*data5.getZ());
   Float_t P6=sqrt(data6.getX()*data6.getX()+data6.getY()*data6.getY()+data6.getZ()*data6.getZ());
   
   //P5-piminus
   //P6-piplus
   Float_t E3=sqrt(P3*P3+mProton*mProton);
   Float_t E4=sqrt(P4*P4+mProton*mProton); //oslun ikinci proton
   Float_t E5=sqrt(P5*P5+mPion*mPion); //olsin piplyus
   Float_t E6=sqrt(P6*P6+mPion*mPion);
   Float_t totalESq=(E1+E2-E3-E4-E5-E6)*(E1+E2-E3-E4-E5-E6);
   Float_t totalPSq=P1*P1+P2*P2+P3*P3+P4*P4+P5*P5+P6*P6-
     2*scalar(dataa1,data3)-2*scalar(dataa1,data4)-
     2*scalar(dataa1,data5)-2*scalar(dataa1,data6)+
     2*scalar(data3,data4)+2*scalar(data5,data6)+
     2*scalar(data3,data5)+2*scalar(data4,data5)+
     2*scalar(data3,data6)+2*scalar(data4,data6);
   return (totalESq-totalPSq);  
}

Float_t HHypKineRefitUtils::missing4Elec(HGeomVector &data3,HGeomVector &data4,HGeomVector &data5,HGeomVector &data6)
{
   Float_t Etotal=eBeam+mProton;
   Float_t P1=sqrt(Etotal*Etotal-mProton*mProton);
   Float_t P2=0.; 
   Float_t P1z=P1;
   Float_t E1=sqrt(P1*P1+mProton*mProton);
   Float_t E2=mProton;
   
   HGeomVector dataa1;
   dataa1.setXYZ(0.,0.,P1z);
   
   Float_t P3=sqrt(data3.getX()*data3.getX()+data3.getY()*data3.getY()+data3.getZ()*data3.getZ());
   Float_t P4=sqrt(data4.getX()*data4.getX()+data4.getY()*data4.getY()+data4.getZ()*data4.getZ());
   Float_t P5=sqrt(data5.getX()*data5.getX()+data5.getY()*data5.getY()+data5.getZ()*data5.getZ());
   Float_t P6=sqrt(data6.getX()*data6.getX()+data6.getY()*data6.getY()+data6.getZ()*data6.getZ());
   
   //P5-piminus
   //P6-piplus
   Float_t E3=sqrt(P3*P3+mProton*mProton);
   Float_t E4=sqrt(P4*P4+mProton*mProton); //oslun ikinci proton
   Float_t E5=sqrt(P5*P5+mElec*mElec); //olsin piplyus
   Float_t E6=sqrt(P6*P6+mElec*mElec);
   Float_t totalESq=(E1+E2-E3-E4-E5-E6)*(E1+E2-E3-E4-E5-E6);
   Float_t totalPSq=P1*P1+P2*P2+P3*P3+P4*P4+P5*P5+P6*P6-
     2*scalar(dataa1,data3)-2*scalar(dataa1,data4)-
     2*scalar(dataa1,data5)-2*scalar(dataa1,data6)+
     2*scalar(data3,data4)+2*scalar(data5,data6)+
     2*scalar(data3,data5)+2*scalar(data4,data5)+
     2*scalar(data3,data6)+2*scalar(data4,data6);
   return (totalESq-totalPSq);  
}

Float_t HHypKineRefitUtils::missing4(Float_t *input)
{
  //bucaglar derecede:

   HGeomVector data[4];
   for(Int_t i=0; i<4; i++)
      {
	 data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
      }
   
   return (missing4(data[0],data[1],data[2],data[3]));
}

Float_t HHypKineRefitUtils::missing4Elec(Float_t *input)
{
  //bucaglar derecede:

   HGeomVector data[4];
   for(Int_t i=0; i<4; i++)
      {
	 data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
      }
   
   return (missing4Elec(data[0],data[1],data[2],data[3]));
}

Float_t HHypKineRefitUtils::missing4(Double_t *input)
{
   HGeomVector data[4];
   for(Int_t i=0; i<4; i++)
      {
	 data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
      }
   
   return (missing4(data[0],data[1],data[2],data[3]));
}

Float_t HHypKineRefitUtils::missing4Elec(Double_t *input)
{
  //bucaglar derecede:

  HGeomVector data[4];
  // for(Int_t i=0; i<4; i++)
  //  {
  //    input[3*i+1]*=acos(-1.)/180.;
  //    input[3*i+2]*=acos(-1.)/180.; 
  //  }
    for(Int_t i=0; i<4; i++)
      {
	data[i].setXYZ(input[3*i]*sin(input[3*i+1])*cos(input[3*i+2]),input[3*i]*sin(input[3*i+1])*sin(input[3*i+2]),input[3*i]*cos(input[3*i+1]));
      }
 
    return (missing4Elec(data[0],data[1],data[2],data[3]));
}

void HHypKineRefitUtils::CalcDerNum(Float_t *DER11,Double_t *Output)
{
 Double_t out[12][12];
 Double_t out1[12][12];
 for(Int_t i=0; i<12; i++)
      {
	//DER11[i]=0.;
	 output[i]=Output[i];
        
      }
 for(Int_t j=0; j<12; j++)
   for(Int_t i=0; i<12; i++)  
     {
       out[j][i]=output[i];
       out1[j][i]=output[i];
     }
 for(Int_t i=0; i<12; i++)
   {
     out[i][i]+=0.0001;
     out1[i][i]-=0.0001; //output[i]/1000000.;
if(i==3 || i==6 || i==9 || i==0)
  {
    out[i][i]+=0.1;
    out1[i][i]-=0.1;
       }
//bu hadronic uchun
     DER11[i]=((missing4(out[i])-135.*135.)-
          ((missing4(out1[i]))-135.*135.))/(out[i][i]-out1[i][i])/2.;
//bu elektromagnetik uchun
// DER11[i]=((missing4(out[i]))-
//        ((missing4(out1[i]))))/(out[i][i]-out1[i][i])/2.;
   }
}

void HHypKineRefitUtils::CalcDerNumPipPimGamma(Float_t *DER11,Double_t *Output)
{
 Double_t out[12][12];
 Double_t out1[12][12];
 for(Int_t i=0; i<12; i++)
      {	 
	output[i]=Output[i];  
      }
 for(Int_t j=0; j<12; j++)
   for(Int_t i=0; i<12; i++)  
     {
       out[j][i]=output[i];
       out1[j][i]=output[i];
     }
 for(Int_t i=0; i<12; i++)
   {
     out[i][i]+=0.0001;
     out1[i][i]-=0.0001; //output[i]/1000000.;
     if(i==3 || i==6 || i==9 || i==0)
       {
	 out[i][i]+=0.1;
	 out1[i][i]-=0.1;
       }     
     // CHANNEL eta -> pi+ pim gamma. i.e. missing gamma= 0
     DER11[i]=((missing4(out[i]))- 
	       ((missing4(out1[i]))))/(out[i][i]-out1[i][i])/2.;    
   }
} // END CalcDerNumPipPimGamma

void HHypKineRefitUtils::CalcDerNumElec(Float_t *DER11,Double_t *Output)
{
 Double_t out[12][12];
 Double_t out1[12][12];
 for(Int_t i=0; i<12; i++)
      {
	//DER11[i]=0.;
	 output[i]=Output[i];
        
      }
 for(Int_t j=0; j<12; j++)
   for(Int_t i=0; i<12; i++)  
     {
       out[j][i]=output[i];
       out1[j][i]=output[i];
     }
 for(Int_t i=0; i<12; i++)
   {
     out[i][i]+=0.0001;
     out1[i][i]-=0.0001; //output[i]/1000000.;
     if(i==3 || i==6 || i==9 || i==0)
       {
	 out[i][i]+=0.1;
	 out1[i][i]-=0.1;
       }
     //DER[i]=(TMath::Power((missing4(output)-135*135),2)-
     //     TMath::Power((missing4(out[i])-135*135),2))/(output[i]-out[i][i]);
     //bu hadronic uchun     
     //     DER11[i]=((missing4(out[i])-135.*135.)-
     //          ((missing4(out1[i]))-135.*135.))/(out[i][i]-out1[i][i])/2.;
     //bu elektromagnetik uchun
     DER11[i]=((missing4Elec(out[i]))-
	       ((missing4Elec(out1[i]))))/(out[i][i]-out1[i][i])/2.;     
   }
}

Float_t HHypKineRefitUtils::secondIter(Float_t *input,Double_t *foutput,Float_t *errInput,Double_t *out,Float_t *derOut)
{// Only for missing Pi0 !!!
   Float_t der[12];
   CalcDerNum(der,foutput);
   Float_t fT=(missing4(output)-135.*135.);
   Float_t sum=0.;
   Float_t sum1=0.;
   Float_t lambda;
   for(Int_t i=0; i<12; i++)
      {
	 sum+=der[i]*(input[i]-output[i]);
	 sum1+=der[i]*der[i]*errInput[i]*errInput[i];
	 derOut[i]=errInput[i]*errInput[i]-TMath::Power(errInput[i],4)*der[i]*der[i]/(der[i]*der[i]*errInput[i]*errInput[i]);
      }
   sum+=fT;
   lambda=sum/sum1;

   for(Int_t i=0; i<12; i++)
      {
	 out[i]=input[i]-der[i]*lambda*errInput[i]*errInput[i];
      }
   return 0;
}

Float_t HHypKineRefitUtils::secondIterPipPimGamma(Float_t *input,Double_t *foutput,Float_t *errInput,Double_t *out,Float_t *derOut)
{// Only for missing Gamma !!!
   Float_t der[12];
   CalcDerNumPipPimGamma(der,foutput);
   Float_t fT=(missing4(output));
   Float_t sum=0.;
   Float_t sum1=0.;
   Float_t lambda;
   for(Int_t i=0; i<12; i++)
      {
	 sum+=der[i]*(input[i]-output[i]);
	 sum1+=der[i]*der[i]*errInput[i]*errInput[i];
	 derOut[i]=errInput[i]*errInput[i]-TMath::Power(errInput[i],4)*der[i]*der[i]/(der[i]*der[i]*errInput[i]*errInput[i]);
      }
   sum+=fT;
   lambda=sum/sum1;

   for(Int_t i=0; i<12; i++)
      {
	 out[i]=input[i]-der[i]*lambda*errInput[i]*errInput[i];
      }
   return 0;
}

Float_t HHypKineRefitUtils::secondIterElec(Float_t *input,Double_t *foutput,Float_t *errInput,Double_t *out,Float_t *derOut)
{// Only for missing Gamma !!!
   Float_t der[12];
   CalcDerNumElec(der,foutput);
   Float_t fT=(missing4Elec(output));
   Float_t sum=0.;
   Float_t sum1=0.;
   Float_t lambda;
   for(Int_t i=0; i<12; i++)
      {
	 sum+=der[i]*(input[i]-output[i]);
	 sum1+=der[i]*der[i]*errInput[i]*errInput[i];
	 derOut[i]=errInput[i]*errInput[i]-TMath::Power(errInput[i],4)*der[i]*der[i]/(der[i]*der[i]*errInput[i]*errInput[i]);
      }
   sum+=fT;
   lambda=sum/sum1;

   for(Int_t i=0; i<12; i++)
      {
	 out[i]=input[i]-der[i]*lambda*errInput[i]*errInput[i];
      }
   return 0;
}


ClassImp(HHypKineRefitUtils);
