//*-- AUTHOR : Anar
//*-- HypCopy : B. Spruck
//*-- Modified : 26/10/2005 by B. Spruck

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


void HHypKineRefitUtils::calcFConserv(Double_t *input,Float_t *errInput, Float_t *F,Float_t *errF)
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
  Ener[2]=sqrt(data[2].length()*data[2].length()+mPion*mPion);
  Ener[3]=sqrt(data[3].length()*data[3].length()+mPion*mPion);  
  
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
  

     //DER[i]=(TMath::Power((missing4(output)-135*135),2)-
     //     TMath::Power((missing4(out[i])-135*135),2))/(output[i]-out[i][i]);



//bu hadronic uchun

     DER11[i]=((missing4(out[i])-135.*135.)-
          ((missing4(out1[i]))-135.*135.))/(out[i][i]-out1[i][i])/2.;
    

//bu elektromagnetik uchun

// DER11[i]=((missing4(out[i]))-
//        ((missing4(out1[i]))))/(out[i][i]-out1[i][i])/2.;
   


                    
   }
 

}




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

void HHypKineRefitUtils::CalcDer(Float_t *DER,Double_t *Output)
{
   
   for(Int_t i=0; i<12; i++)
      {
	 DER[i]=0.;
	 output[i]=Output[i];
      }

   Etot[0]=sqrt(output[0]*output[0]+mProton*mProton);
   Etot[1]=sqrt(output[3]*output[3]+mProton*mProton);
   Etot[2]=sqrt(output[6]*output[6]+mPion*mPion);
   Etot[3]=sqrt(output[9]*output[9]+mPion*mPion);
   
   Float_t der[12];

   DER[0]=2*Etot[0]*derE(0)-2*AllE*derE(0)+2*derE(0,1)+2*derE(0,2)+2*derE(0,3);
   DER[3]=2*Etot[1]*derE(1)-2*AllE*derE(1)+2*derE(1,0)+2*derE(1,2)+2*derE(1,3);
   DER[6]=2*Etot[2]*derE(2)-2*AllE*derE(2)+2*derE(2,0)+2*derE(2,1)+2*derE(2,3);
   DER[9]=2*Etot[3]*derE(3)-2*AllE*derE(3)+2*derE(3,0)+2*derE(3,1)+2*derE(3,2);
   
   
   der[0]=2*output[0]-2*pInput*cos(output[1])+2*derP(0,1)+2*derP(0,2)+2*derP(0,3);
   der[1]=-2*pInput*output[0]*(-sin(output[1]))+2*derTet(0,1)+2*derTet(0,2)+2*derTet(0,3);
   der[2]=2*derPhi(0,1)+2*derPhi(0,2)+2*derPhi(0,3);

   der[3]=2*output[3]-2*pInput*cos(output[4])+2*derP(1,0)+2*derP(1,2)+2*derP(1,3);
   der[4]=-2*pInput*output[3]*(-sin(output[4]))+2*derTet(1,0)+2*derTet(1,2)+2*derTet(1,3);
   der[5]=2*derPhi(1,0)+2*derPhi(1,2)+2*derPhi(1,3);
          
   der[6]=2*output[6]-2*pInput*cos(output[7])+2*derP(2,0)+2*derP(2,1)+2*derP(2,3);
   der[7]=-2*pInput*output[6]*(-sin(output[7]))+2*derTet(2,0)+2*derTet(2,1)+2*derTet(2,3);
   der[8]=2*derPhi(2,0)+2*derPhi(2,1)+2*derPhi(2,3);

   der[9]=2*output[9]-2*pInput*cos(output[10])+2*derP(3,0)+2*derP(3,1)+2*derP(3,2);
   der[10]=-2*pInput*output[9]*(-sin(output[10]))+2*derTet(3,0)+2*derTet(3,1)+2*derTet(3,2);
   der[11]=2*derPhi(3,0)+2*derPhi(3,1)+2*derPhi(3,2);

   for(Int_t i=0; i<12; i++)
      {
	 DER[i]-=der[i];
	 DER[i]/=(2*sqrt(missing4(output)));
      }
   
}

Float_t HHypKineRefitUtils::derP(Int_t i, Int_t j)
{
   Float_t der=output[3*j]*(sin(output[3*j+1])*cos(output[3*j+2])*sin(output[3*i+1])*cos(output[3*i+2])+sin(output[3*j+1])*sin(output[3*j+2])*sin(output[3*i+1])*sin(output[3*i+2])+cos(output[3*j+1])*cos(output[3*i+1]));

   return der;
}

Float_t HHypKineRefitUtils::derTet(Int_t i, Int_t j)
{
 Float_t der=output[3*j]*(sin(output[3*j+1])*cos(output[3*j+2])*cos(output[3*i+1])*cos(output[3*i+2])+sin(output[3*j+1])*sin(output[3*j+2])*cos(output[3*i+1])*sin(output[3*i+2])-cos(output[3*j+1])*sin(output[3*i+1]));
													
return der;												
   
}

Float_t HHypKineRefitUtils::derPhi(Int_t i, Int_t j)
{
   Float_t der=output[3*j]*(-sin(output[3*j+1])*cos(output[3*j+2])*sin(output[3*i+1])*sin(output[3*i+2])+sin(output[3*j+1])*sin(output[3*j+2])*sin(output[3*i+1])*cos(output[3*i+2]));

   return der;
}

Float_t HHypKineRefitUtils::derE(Int_t i, Int_t j)
{
   
   Float_t der=Etot[j]*output[3*i]/sqrt(Etot[i]);
   return der;
}

Float_t HHypKineRefitUtils::derE(Int_t i)
{
   
   Float_t der=output[3*i]/sqrt(Etot[i]);
   return der;
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
