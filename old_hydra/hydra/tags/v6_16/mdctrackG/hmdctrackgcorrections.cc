using namespace std;
#include<iostream>
#include<fstream>
#include"hmdctrackgcorrections.h"
#include<math.h>
ClassImp(HMdcTrackGCorrections)
   
HMdcTrackGCorrections::HMdcTrackGCorrections(const char* name, const char* title):
      TNamed(name,title)
{
   for(Int_t i=0; i<34; i++)
      tetaI[i]=18+2*i;
   for(Int_t i=0; i<16; i++)
      phiI[i]=60+2*i;
}

HMdcTrackGCorrections::~HMdcTrackGCorrections()
{
   ;
}
void HMdcTrackGCorrections::clear()
{
}
void HMdcTrackGCorrections::init(TString infile1, TString infile2)
{
   Double_t pp;
   ifstream input1, input2;
   
   input1.open(infile1.Data(),ios::in);
   input2.open(infile2.Data(),ios::in);
   
   for(Int_t i=0; i<60; i++)
      {
	 input1>>precI[i];
	 input2>>pp;
      }
   for (Int_t k=0; k<34; k++)
      for (Int_t i=0; i<16; i++)
	 for(Int_t j=0; j<60; j++)
	    {
	       input1>>corrE[k][i][j];
	       input2>>corrP[k][i][j];
      }  
   input1.close();
   input2.close();
   
}
Double_t HMdcTrackGCorrections::getphiI(Int_t i)
{
return phiI[i];
}



