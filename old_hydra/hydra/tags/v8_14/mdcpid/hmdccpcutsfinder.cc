using namespace std;
#include "hmdccpcutsfinder.h"
#include "hmdcfunc1.h"
#include <iomanip>
#include <iostream>
#include <TFile.h>
#include <TString.h>
#include <TH1F.h>
#include "hpidcpparam.h"
//*-- Author : J.Bielcik
//*-- Modified :15.5.2003
//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////
// HMdcCPCutsFinder                                       //  
// This class analyses ntuples filled with singles and    //
// doubles and fill                                       //  
// the HMdcRichClosePairCut container                     //  
// usage:                                                 //  
// HMdcCPCutsFinder *find=new HMdcCPCutsFinder(10,1,200)  //                                
// find->readSingles(singles)                             //  
// find->readDoubles(doubles)                             //  
// find->findCuts()                                       //  
// find->printContainer2File(filename)                    //  
////////////////////////////////////////////////////////////
HMdcCPCutsFinder::HMdcCPCutsFinder() {
  
  
  sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:clussize1:nwires1:level:level1:sector:rmcase");
  doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:clussize1:nwires1:level:level1:sector:rmcase");


  //  mdc_sing=new TNtuple("mdc_sing","mdc_sing","theta:phi:cls:nwires:level:cls1:nwires1:level1");
  //  mdc_doub=new TNtuple("mdc_doub","mdc_doub","theta:phi:cls:nwires:level:cls1:nwires1:level1");
  mdc_sing=new TNtuple("mdc_sing","mdc_sing","theta:phi:cls:nwires:level:module:sector");
  mdc_doub=new TNtuple("mdc_doub","mdc_doub","theta:phi:cls:nwires:level:module:sector");




  // sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
//   doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");


  mdc_cluster_max=200;
  mdc_wires_max=24;
  rich_charge_max=2500;

  mdc_cluster_step=10;
  mdc_wires_step=1;
  rich_charge_step=100;
  
  theta_bin_size=10;
  phi_bin_size=10;
 
  theta_min=10.0;
  theta_max=90.0;

  phi_min=0.0;
  phi_max=60.0;
  
  phi_nbin= ((Int_t) (phi_max-phi_min))/(2*phi_bin_size);
  // phi angles 0-10 an 50-60 are identical because of the symetry so I will put 0-60 only to 3 bins
  theta_nbin= ((Int_t) (theta_max-theta_min))/theta_bin_size;  
  lev_nbin=3;
}

HMdcCPCutsFinder::HMdcCPCutsFinder(Int_t m_cluster_step,Int_t m_wires_step,Int_t r_charge_step) {

  mdc_cluster_step=m_cluster_step;
  mdc_wires_step=m_wires_step;
  rich_charge_step=r_charge_step;
  
  //sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level:sector:rmcase");
  //doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level:sector:rmcase");
  
  //sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
  //doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
  
  sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:clussize1:nwires1:level:level1:sector:rmcase");
  doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:clussize1:nwires1:level:level1:sector:rmcase");

  mdc_sing=new TNtuple("mdc_sing","mdc_sing","theta:phi:cls:nwires:level:cls1:nwires1:level1");
  mdc_doub=new TNtuple("mdc_doub","mdc_doub","theta:phi:cls:nwires:level:cls1:nwires1:level1");
  
  mdc_cluster_max=200;
  mdc_wires_max=24; 
  rich_charge_max=2500;
 

 
  
  theta_bin_size=10;
  phi_bin_size=10;
 
  theta_min=10.0;
  theta_max=90.0;

  phi_min=0.0;
  phi_max=60.0;
  
  phi_nbin= ((Int_t) (phi_max-phi_min))/(2*phi_bin_size);
  // phi angles 0-10 an 50-60 are identical because of the symetry so I will put 0-60 only to 3 bins
  theta_nbin= ((Int_t) (theta_max-theta_min))/theta_bin_size; 
  lev_nbin=3; 
}
HMdcCPCutsFinder::~HMdcCPCutsFinder() { 
  delete sing;
  delete doub;
}

Bool_t  HMdcCPCutsFinder::readSingles(TNtuple * s) { 
  Float_t* values;
  Double_t read=12000.0;
  //read=s->GetEntries();
  printf("read singles=%f\n",read);
  for (Int_t i=0;i<read;i++){
    s->GetEntry(i);
    values=s->GetArgs();
    sing->Fill(values);
  }
  return kTRUE;
}

Bool_t  HMdcCPCutsFinder::readDoubles(TNtuple * d) { 
  Float_t* values;
  Double_t read=6000.0;
  //read=d->GetEntries();
  printf("read doubles=%f\n",read);
  for (Int_t i=0;i<read;i++){
    d->GetEntry(i);
    values=d->GetArgs();
    doub->Fill(values);
  }
  return kTRUE;
}

Bool_t  HMdcCPCutsFinder::readMdcSingles(TNtuple * s) { 
  Float_t* values;
  Double_t read=2000.0;
  read=s->GetEntries();
  printf("read mdc singles=%f\n",read);
  for (Int_t i=0;i<read;i++){
    s->GetEntry(i);
    values=s->GetArgs();
    mdc_sing->Fill(values);
  }
  return kTRUE;
}

Bool_t  HMdcCPCutsFinder::readMdcDoubles(TNtuple * d) { 
  Float_t* values;
  Double_t read=2000.0;
  read=d->GetEntries();
  printf("read mdc doubles=%f\n",read);
  for (Int_t i=0;i<read;i++){
    d->GetEntry(i);
    values=d->GetArgs();
    mdc_doub->Fill(values);
  }
  return kTRUE;
}

Bool_t  HMdcCPCutsFinder::findCuts() { 
  Float_t * values;
  Float_t lost_s,good_s,wrong_s; 
  Float_t S_B;
  Float_t quality,ef,back;
  
  Float_t ef_max;
  Int_t sing_inbin,doub_inbin;
  lost_s=0;
  good_s=0;
  wrong_s=0;

  
  Float_t allsingles=0;
  Float_t alldoubles=0; 
  
  allsingles=sing->GetEntries();
  alldoubles=doub->GetEntries();

  printf("number of singles in analysis:%Li\n",sing->GetEntries());
  printf("number of doubles in analysis:%Li\n",doub->GetEntries());
  //** for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
    //loop over levels
 Int_t lev_b=1;
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	//loop over phi
	ef_max=0.0;
	for(Int_t i=0;i<4;i++){
	  for(Int_t j=0;j<5;j++){
	    container[lev_b][theta_b][phi_b][i][j]= 0.0;
	  }
	}
	sing_inbin=0;
	doub_inbin=0;
	//calculate how much singles and doubles is in this bin

	for (Int_t i=0;i<sing->GetEntries();i++){
	//==for (Int_t i=0;i<10000;i++){
	  sing->GetEntry(i);
	  values=sing->GetArgs();
	  // printf("values[0]=%f values[1]=%f \n",values[0],values[1]);
	  if(inBin(values[0],values[1],values[10],theta_b,phi_b,lev_b)) sing_inbin++;
	}
	
	for (Int_t i=0;i<doub->GetEntries();i++){
	//==for (Int_t i=0;i<5000;i++){
	  doub->GetEntry(i);
	  values=doub->GetArgs();
	  if(inBin(values[0],values[1],values[10],theta_b,phi_b,lev_b)) doub_inbin++;
	}
	
	printf("BIN %i %i %i sin_inbin=%i  doub_inbin=%i \n",lev_b, theta_b,phi_b,sing_inbin,doub_inbin);
	
	
	for(Int_t mdc_cluster_cut=0;mdc_cluster_cut<mdc_cluster_max+1;mdc_cluster_cut=mdc_cluster_cut+mdc_cluster_step){
	  for(Int_t mdc_wires_cut=4;mdc_wires_cut<mdc_wires_max+1;mdc_wires_cut=mdc_wires_cut+mdc_wires_step){
	    //**  for(Int_t rich_charge_cut=0;rich_charge_cut<rich_charge_max+1;rich_charge_cut=rich_charge_cut+rich_charge_step){
	   Int_t rich_charge_cut=0;
	      //go over sing
	      
	      for (Int_t i=0;i<sing->GetEntries();i++){
		//==  for (Int_t i=0;i<10000;i++){
		sing->GetEntry(i);
		values=sing->GetArgs();
		if(inBin(values[0],values[1],values[10],theta_b,phi_b,lev_b)){
		  //**		  if((values[7]<mdc_cluster_cut)&&(values[8]<mdc_wires_cut)&&(values[6]<rich_charge_cut)) {
		  if((values[7]<mdc_cluster_cut)&&(values[8]<mdc_wires_cut)) {
		    good_s=good_s+1.0;
		    // printf("good_s=%f\n",good_s);
		  }else{
		    lost_s=lost_s+1.0;
		    //printf("lost_s=%f\n",lost_s);
		  }
		}
	      }
	      
	      //go over doubles
	      for (Int_t i=0;i<doub->GetEntries();i++){
		//==for (Int_t i=0;i<5000;i++){
		doub->GetEntry(i);
		values=doub->GetArgs();
		if(inBin(values[0],values[1],values[10],theta_b,phi_b,lev_b)){
		  //**		  if((values[7]<mdc_cluster_cut)&&(values[8]<mdc_wires_cut)&&(values[6]<rich_charge_cut)) {
		  if((values[7]<mdc_cluster_cut)&&(values[8]<mdc_wires_cut)) {
		    wrong_s=wrong_s+1.0;
		  }
		}
	      }
	      
	      
	      
	      quality=0.0;
	      ef=0.0;
	      back=0.0;
	      S_B=0.0;
	      
	      if(wrong_s>0.0) {
		S_B=(Float_t) good_s/wrong_s;
	      }else{
		S_B=(Float_t) good_s;
	      } 
	      
	      if((good_s+lost_s)>0.0&&(good_s+wrong_s)>0.0){
		ef=(Float_t) good_s/(good_s+lost_s);
		back=(Float_t) good_s/(good_s+wrong_s);
		quality=ef*back;
	      }
	       
	      if(ef>ef_max) ef_max=ef;
	      //  Float_t co=0.2;
	      //** if(ef>0.8) {
	      if(ef>0.5) {
		if(container[lev_b][theta_b][phi_b][0][1]<S_B){
		  container[lev_b][theta_b][phi_b][0][0]= ef ;
		  container[lev_b][theta_b][phi_b][0][1]= S_B;
		  container[lev_b][theta_b][phi_b][0][2]=mdc_cluster_cut;
		  container[lev_b][theta_b][phi_b][0][3]=mdc_wires_cut;
		  
		  container[lev_b][theta_b][phi_b][0][4]=rich_charge_cut;
		}
	      } 
	      
	      //**  if(ef>0.85) {
	      if(ef>0.6) {
		if(container[lev_b][theta_b][phi_b][1][1]<S_B){
		  container[lev_b][theta_b][phi_b][1][0]= ef ;
		  container[lev_b][theta_b][phi_b][1][1]= S_B;
		  container[lev_b][theta_b][phi_b][1][2]=mdc_cluster_cut;
		  container[lev_b][theta_b][phi_b][1][3]=mdc_wires_cut;
		  
		  container[lev_b][theta_b][phi_b][1][4]=rich_charge_cut;
		}
	      }
	      
	     //** if(ef>0.9) {
	      if(ef>0.8) {  
		if(container[lev_b][theta_b][phi_b][2][1]<S_B){
		  container[lev_b][theta_b][phi_b][2][0]= ef ;
		  container[lev_b][theta_b][phi_b][2][1]= S_B;
		  container[lev_b][theta_b][phi_b][2][2]=mdc_cluster_cut;
		  container[lev_b][theta_b][phi_b][2][3]=mdc_wires_cut;
		  
		  container[lev_b][theta_b][phi_b][2][4]=rich_charge_cut;
		}
	      }
	      
	      if(ef>0.95) {
		if(container[lev_b][theta_b][phi_b][3][1]<S_B){
		  container[lev_b][theta_b][phi_b][3][0]=ef ;
		  container[lev_b][theta_b][phi_b][3][1]=S_B;
		  container[lev_b][theta_b][phi_b][3][2]=mdc_cluster_cut;
		  container[lev_b][theta_b][phi_b][3][3]=mdc_wires_cut;
		  
		  container[lev_b][theta_b][phi_b][3][4]=rich_charge_cut;
		  
		}
	      }
	      
	      lost_s=0.0;
	      good_s=0.0;
	      wrong_s=0.0;
	      //* }
	  }
	}
	if(ef_max<0.8) printf("IN THIS BIN :%i %i %i  max eff was only=%f\n",lev_b,theta_b,phi_b,ef_max);
	
	for(Int_t a=0;a<4;a++)  printf("%2i %2i %2i efbin:%2i  ef:%4.2f  s/b:%6.2f  size:%4.0f wires:%4.0f  charge:%5.0f\n",lev_b,theta_b,phi_b,a,container[lev_b][theta_b][phi_b][a][0],container[lev_b][theta_b][phi_b][a][1],container[lev_b][theta_b][phi_b][a][2],container[lev_b][theta_b][phi_b][a][3],container[lev_b][theta_b][phi_b][a][4]);
	printf("------------------------------------------------------------------------------\n");
      }
    }
    //}
  return kTRUE;
}


void HMdcCPCutsFinder::findPidCuts(char *outputname){
  //HMdcCPCutsFinder::findPidCuts
  Char_t name_s[200];
  Char_t name_d[200];

  

  Float_t * values;  
  printf("creating histograms\n");
  for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
    //loop over levels
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	for(Int_t module=0;module<2;module++){
	  sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_s_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","cls_d_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	
	  hcls[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,0,300); 
	  hcls[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,0,300); 
	  
	  sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","cls_pd_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	
	  hcls_p[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,0,300); 
	  hcls_p[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,0,300); 


	  
	  sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_s_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","wires_d_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	  
	  hnwires[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,4,34); 
	  hnwires[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,4,34);

	  sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","wires_pd_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	  
	  hnwires_p[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,4,34); 
	  hnwires_p[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,4,34);

	  

	}
	sprintf(name_s,"%s%i%s%i%s%i","intcharge_s_",theta_b,"_",phi_b,"_",lev_b);
        sprintf(name_d,"%s%i%s%i%s%i","intcharge_d_",theta_b,"_",phi_b,"_",lev_b); 
	
	hcharge[theta_b][phi_b][lev_b][0]=new TH1F(name_s,name_s,15,0,1500); 
	hcharge[theta_b][phi_b][lev_b][1]=new TH1F(name_d,name_d,15,0,1500);
	
	sprintf(name_s,"%s%i%s%i%s%i","intcharge_ps_",theta_b,"_",phi_b,"_",lev_b);
        sprintf(name_d,"%s%i%s%i%s%i","intcharge_pd_",theta_b,"_",phi_b,"_",lev_b); 
	
	hcharge_p[theta_b][phi_b][lev_b][0]=new TH1F(name_s,name_s,15,0,1500); 
	hcharge_p[theta_b][phi_b][lev_b][1]=new TH1F(name_d,name_d,15,0,1500);


      }
    }
  }
  printf("filling singles to  histograms\n");
  for (Int_t i=0;i<sing->GetEntries();i++){
    sing->GetEntry(i);
    values=sing->GetArgs();
   
    // if(values[13]==0&&calculateMdcThetaBin(values[0])>0&&calculateMdcPhiBin(values[1])>0) hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[11])][0]->Fill(values[4]);
    if(calculateMdcThetaBin(values[0])>=0&&calculateMdcPhiBin(values[1])>=0) {
      //mdc module =0
      // printf("module=0 i=%i %f %f\n ",i,values[4],values[5]);
      //&&values[12]==0 means that is is Rich single -Mdc single and ==3 means Rich double -Mdc single
      if(calculateMdcLevBin(values[9])>=0&&values[12]==0){
	hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[9])][0][0]->Fill(values[4]);
	hnwires[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[9])][0][0]->Fill(values[5]);
      }
      //mdc module =1
      // printf("module=1\n");
      if(calculateMdcLevBin(values[10])>=0&&values[12]==0){
	hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[10])][1][0]->Fill(values[7]);
	hnwires[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[10])][1][0]->Fill(values[8]);
      }
      if(calculateMdcLevBin(values[9])>=0&&values[12]==0){
	hcharge[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[9])][0]->Fill(values[6]);
      }
      



    }

  }
 printf("filling doubles to  histograms\n");
  for (Int_t i=0;i<doub->GetEntries();i++){
    doub->GetEntry(i);
    values=doub->GetArgs();
    // if(values[13]==2&&calculateMdcThetaBin(values[0])>0&&calculateMdcPhiBin(values[1])>0) hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[11])][1]->Fill(values[4]);
    if(calculateMdcThetaBin(values[0])>=0&&calculateMdcPhiBin(values[1])>=0){
      //&&values[12]==2 means that is is Rich double -Mdc double and ==1 means Rich single -Mdc double
      //mdc module =0
      if(calculateMdcLevBin(values[9])>=0&&values[12]==2){
	hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[9])][0][1]->Fill(values[4]);
	hnwires[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[9])][0][1]->Fill(values[5]);
      }
      //mdc module =1
      if(calculateMdcLevBin(values[10])>=0&&values[12]==2){
	hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[10])][1][1]->Fill(values[7]);
	hnwires[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[10])][1][1]->Fill(values[8]);
      }
      if(calculateMdcLevBin(values[9])>=0&&values[12]==2){
	hcharge[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[9])][1]->Fill(values[6]);
      }
    }  
  }
  


 
  Float_t pds,pdd,ps,pd;
  TString  header=
    "###################################################################################################\n"
    "# Lepton Single/Double recognition parameters \n"
    "# Format: \n"
    "# levelbin thetabin phibin  cls pdfs pdfd ps pd\n"
    "# levelbin thetabin phibin  nwires pdfs pdfd ps pd\n"
    "# levelbin thetabin phibin  intcharge pdfs pdfd ps pd\n"
    "###################################################################################################\n"
    "[MdcClosePairCutsPar]\n";
  
  
  printf("Calculating pdf \n");
  FILE* myoutfile =fopen("c2c_cp_table.txt","w");
  Float_t bs,bd,sums,sumd;
  fprintf(myoutfile,"%s\n",header.Data());
    for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
  //for(Int_t lev_b=1;lev_b<2;lev_b++){
    //loop over levels
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	for(Int_t module=0;module<2;module++){
	  for(Int_t b=0;b<hcls[theta_b][phi_b][lev_b][module][0]->GetNbinsX();b++) {
	    
	    bs=hcls[theta_b][phi_b][lev_b][module][0]->GetBinContent(b);
	    bd=hcls[theta_b][phi_b][lev_b][module][1]->GetBinContent(b);

	    sumd=hcls[theta_b][phi_b][lev_b][module][1]->Integral();
	    sums=hcls[theta_b][phi_b][lev_b][module][0]->Integral();
	    
	    
	    // pds=hcls[theta_b][phi_b][lev_b][0]->GetBinContent(b)/hcls[theta_b][phi_b][lev_b][0]->Integral();
	    // 	  pdd=hcls[theta_b][phi_b][lev_b][1]->GetBinContent(b)/hcls[theta_b][phi_b][lev_b][1]->Integral();
	    // 	  ps=(pds*hcls[theta_b][phi_b][lev_b][0]->Integral())/(pds*hcls[theta_b][phi_b][lev_b][0]->Integral()+pdd*hcls[theta_b][phi_b][lev_b][1]->Integral());
	    // 	  pd=(pdd*hcls[theta_b][phi_b][lev_b][1]->Integral())/(pds*hcls[theta_b][phi_b][lev_b][0]->Integral()+pdd*hcls[theta_b][phi_b][lev_b][1]->Integral());
	    if(sums>0.0) {
	      pds=bs/sums;
	    } else {
	      pds=0.0;
	    }
	    
	    if(sumd>0.0) {
	      pdd=bd/sumd;
	    } else {
	      pdd=0.0;
	    }
	    
	    if((pds*sums+pdd*sumd)>0.0) {
	      ps=pds*sums/(pds*sums+pdd*sumd);
	      pd=pdd*sumd/(pds*sums+pdd*sumd);
	    } else {
	      ps=0.0;
	      pd=0.0;
	    }
	    
	    hcls_p[theta_b][phi_b][lev_b][module][0]->SetBinContent(b,pds);
	    hcls_p[theta_b][phi_b][lev_b][module][1]->SetBinContent(b,pdd);


	    
	    //if(phi_b==0)  printf("lev=%i the=%i phi=%i module=%i cls=%i bs=%f bd=%f pds=%f pdd=%f ps=%f pd=%f\n",lev_b,theta_b,phi_b,module,b*10,bs,bd,pds,pdd,ps,pd);
	    fprintf(myoutfile,"%2i %2i %2i %2i %4i %7.4f %7.4f %7.4f  %7.4f\n",lev_b,theta_b,phi_b,module,b*10,pds,pdd,ps,pd);
	  }
	  
	  
	  for(Int_t b=0;b<hnwires[theta_b][phi_b][lev_b][module][0]->GetNbinsX();b++) {
	    //  pds=hnwires[theta_b][phi_b][lev_b][0]->GetBinContent(b)/hnwires[theta_b][phi_b][lev_b][0]->Integral();
	    // 	  pdd=hnwires[theta_b][phi_b][lev_b][1]->GetBinContent(b)/hnwires[theta_b][phi_b][lev_b][1]->Integral();
	    // 	  ps=(pds*hnwires[theta_b][phi_b][lev_b][0]->Integral())/(pds*hnwires[theta_b][phi_b][lev_b][0]->Integral()+pdd*hnwires[theta_b][phi_b][lev_b][1]->Integral());
	    // 	  pd=(pdd*hnwires[theta_b][phi_b][lev_b][1]->Integral())/(pds*hnwires[theta_b][phi_b][lev_b][0]->Integral()+pdd*hnwires[theta_b][phi_b][lev_b][1]->Integral());
	    
	    bs=hnwires[theta_b][phi_b][lev_b][module][0]->GetBinContent(b);
	    bd=hnwires[theta_b][phi_b][lev_b][module][1]->GetBinContent(b);
	    sumd=hnwires[theta_b][phi_b][lev_b][module][1]->Integral();
	    sums=hnwires[theta_b][phi_b][lev_b][module][0]->Integral();
	    
	    
	    if(sums>0.0) {
	      pds=bs/sums;
	    } else {
	      pds=0.0;
	    }
	    
	    if(sumd>0.0) {
	    pdd=bd/sumd;
	    } else {
	      pdd=0.0;
	    }
	    
	    if((pds*sums+pdd*sumd)>0.0) {
	      ps=pds*sums/(pds*sums+pdd*sumd);
	      pd=pdd*sumd/(pds*sums+pdd*sumd);
	    } else {
	      ps=0.0;
	      pd=0.0;
	    }
	    
       
	    hnwires_p[theta_b][phi_b][lev_b][module][0]->SetBinContent(b,pds);
	    hnwires_p[theta_b][phi_b][lev_b][module][1]->SetBinContent(b,pdd);
	    
	    
	    
	    // if(phi_b==0)  printf("lev=%i the=%i phi=%i module=%i nwires=%i bs=%f bd=%f pds=%f pdd=%f ps=%f pd=%f\n",lev_b,theta_b,phi_b,module,b+4,bs,bd,pds,pdd,ps,pd);
	    fprintf(myoutfile,"%2i %2i %2i %2i %4i %7.4f %7.4f %7.4f  %7.4f\n",lev_b,theta_b,phi_b,module,b+4,pds,pdd,ps,pd);
	  }
	}

 	for(Int_t b=0;b<hcharge[theta_b][phi_b][lev_b][0]->GetNbinsX();b++) {
	  //histos for hcharge with module=0 and  module=1 should be the same
	  // pds=hcharge[theta_b][phi_b][lev_b][0]->GetBinContent(b)/hcharge[theta_b][phi_b][lev_b][0]->Integral();
// 	  pdd=hcharge[theta_b][phi_b][lev_b][1]->GetBinContent(b)/hcharge[theta_b][phi_b][lev_b][1]->Integral();
// 	  ps=(pds*hcharge[theta_b][phi_b][lev_b][0]->Integral())/(pds*hcharge[theta_b][phi_b][lev_b][0]->Integral()+pdd*hcharge[theta_b][phi_b][lev_b][1]->Integral());
// 	  pd=(pdd*hcharge[theta_b][phi_b][lev_b][1]->Integral())/(pds*hcharge[theta_b][phi_b][lev_b][0]->Integral()+pdd*hcharge[theta_b][phi_b][lev_b][1]->Integral());
 
	  bs=hcharge[theta_b][phi_b][lev_b][0]->GetBinContent(b);
	  bd=hcharge[theta_b][phi_b][lev_b][1]->GetBinContent(b);
	  sumd=hcharge[theta_b][phi_b][lev_b][1]->Integral();
	  sums=hcharge[theta_b][phi_b][lev_b][0]->Integral();


	  if(sums>0.0) {
	    pds=bs/sums;
	  } else {
	    pds=0.0;
	  }
	  
	  if(sumd>0.0) {
	    pdd=bd/sumd;
	  } else {
	    pdd=0.0;
	  }
    
	  if((pds*sums+pdd*sumd)>0.0) {
	    ps=pds*sums/(pds*sums+pdd*sumd);
	    pd=pdd*sumd/(pds*sums+pdd*sumd);
	  } else {
	    ps=0.0;
	    pd=0.0;
	  }

       
	  hcharge_p[theta_b][phi_b][lev_b][0]->SetBinContent(b,pds);
	  hcharge_p[theta_b][phi_b][lev_b][1]->SetBinContent(b,pdd);


	  if(phi_b==0)  printf("lev=%i the=%i phi=%i charge=%i bs=%f bd=%f pds=%f pdd=%f ps=%f pd=%f\n",lev_b,theta_b,phi_b,b*100,bs,bd,pds,pdd,ps,pd);
	  fprintf(myoutfile,"%2i %2i %2i %4i %7.4f %7.4f %7.4f  %7.4f\n",lev_b,theta_b,phi_b,b*100,pds,pdd,ps,pd);
     	}
	


      }
    }
  }
    fprintf(myoutfile,"##################\n");
    fclose(myoutfile); 


  printf("filling   histograms to file \n"); 
  TFile* outfile = new TFile(outputname,"RECREATE");
  outfile->cd();
  for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
    //loop over levels
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	for(Int_t module=0;module<2;module++){
	  hcls[theta_b][phi_b][lev_b][module][0]->Write();
	  hcls[theta_b][phi_b][lev_b][module][1]->Write();
	  hnwires[theta_b][phi_b][lev_b][module][0]->Write();
	  hnwires[theta_b][phi_b][lev_b][module][1]->Write();
	  hcls_p[theta_b][phi_b][lev_b][module][0]->Write();
	  hcls_p[theta_b][phi_b][lev_b][module][1]->Write();
	  hnwires_p[theta_b][phi_b][lev_b][module][0]->Write();
	  hnwires_p[theta_b][phi_b][lev_b][module][1]->Write();


	}
	hcharge[theta_b][phi_b][lev_b][0]->Write();
	hcharge[theta_b][phi_b][lev_b][1]->Write();
	hcharge_p[theta_b][phi_b][lev_b][0]->Write();
	hcharge_p[theta_b][phi_b][lev_b][1]->Write();
      }
    }
  }
  outfile->Close();




}

void HMdcCPCutsFinder::findPidCutsFromMdc(char *outputname){
  Char_t name_s[200];
  Char_t name_d[200];
  Int_t module=-1;
  printf("Producing histograms using only mdc information\n");
  printf("Output name=%s\n",outputname);

  Float_t * values;  
  printf("creating histograms \n");
  for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
    //loop over levels
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	for(Int_t module=0;module<2;module++){
	  sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_s_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","cls_d_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	
	  hcls[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,0,300); 
	  hcls[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,0,300); 
	  
	  sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","cls_pd_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	
	  hcls_p[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,0,300); 
	  hcls_p[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,0,300); 


	  
	  sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_s_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","wires_d_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	  
	  hnwires[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,4,34); 
	  hnwires[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,4,34);

	  sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",theta_b,"_",phi_b,"_",lev_b,"_",module);
	  sprintf(name_d,"%s%i%s%i%s%i%s%i","wires_pd_",theta_b,"_",phi_b,"_",lev_b,"_",module); 
	  
	  hnwires_p[theta_b][phi_b][lev_b][module][0]=new TH1F(name_s,name_s,30,4,34); 
	  hnwires_p[theta_b][phi_b][lev_b][module][1]=new TH1F(name_d,name_d,30,4,34);

	  

	}
      }
    }
  }
  printf("filling singles to  histograms from mdc \n");
  for (Int_t i=0;i<mdc_sing->GetEntries();i++){
    mdc_sing->GetEntry(i);
    values=mdc_sing->GetArgs();
   
    if(calculateMdcThetaBin(values[0])>=0&&calculateMdcPhiBin(values[1])>=0) {
     
      module=(Int_t) values[5];
      if(calculateMdcLevBin(values[4])>=0){ 
	//now fill histo for this particular module
	hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[4])][module][0]->Fill(values[2]);
	hnwires[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[4])][module][0]->Fill(values[3]);
      }
    }
  }

 printf("filling doubles to  histograms from mdc \n");
  for (Int_t i=0;i<mdc_doub->GetEntries();i++){
    mdc_doub->GetEntry(i);
    values=mdc_doub->GetArgs();
    
    if(calculateMdcThetaBin(values[0])>=0&&calculateMdcPhiBin(values[1])>=0){
      
      module=(Int_t) values[5];
      if(calculateMdcLevBin(values[4])>=0){
	//filling for particular module
	hcls[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[4])][module][1]->Fill(values[2]);
	hnwires[calculateMdcThetaBin(values[0])][calculateMdcPhiBin(values[1])][calculateMdcLevBin(values[4])][module][1]->Fill(values[3]);
      }
    }  
  }
  


 
  Float_t pds,pdd,ps,pd;
  TString  header=
    "###################################################################################################\n"
    "# Lepton Single/Double recognition parameters \n"
    "# Format: \n"
    "# levelbin thetabin phibin  cls pdfs pdfd ps pd\n"
    "# levelbin thetabin phibin  nwires pdfs pdfd ps pd\n"
    "# levelbin thetabin phibin  intcharge pdfs pdfd ps pd\n"
    "###################################################################################################\n"
    "[MdcClosePairCutsPar]\n";
  
  
  printf("Calculating pdf \n");
  FILE* myoutfile =fopen("c2c_cp_table.txt","w");
  Float_t bs,bd,sums,sumd;
  fprintf(myoutfile,"%s\n",header.Data());
    for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
  //for(Int_t lev_b=1;lev_b<2;lev_b++){
    //loop over levels
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	for(Int_t module=0;module<2;module++){
	  for(Int_t b=0;b<hcls[theta_b][phi_b][lev_b][module][0]->GetNbinsX();b++) {
	    
	    bs=hcls[theta_b][phi_b][lev_b][module][0]->GetBinContent(b);
	    bd=hcls[theta_b][phi_b][lev_b][module][1]->GetBinContent(b);

	    sumd=hcls[theta_b][phi_b][lev_b][module][1]->Integral();
	    sums=hcls[theta_b][phi_b][lev_b][module][0]->Integral();
	    
	    
	    // pds=hcls[theta_b][phi_b][lev_b][0]->GetBinContent(b)/hcls[theta_b][phi_b][lev_b][0]->Integral();
	    // 	  pdd=hcls[theta_b][phi_b][lev_b][1]->GetBinContent(b)/hcls[theta_b][phi_b][lev_b][1]->Integral();
	    // 	  ps=(pds*hcls[theta_b][phi_b][lev_b][0]->Integral())/(pds*hcls[theta_b][phi_b][lev_b][0]->Integral()+pdd*hcls[theta_b][phi_b][lev_b][1]->Integral());
	    // 	  pd=(pdd*hcls[theta_b][phi_b][lev_b][1]->Integral())/(pds*hcls[theta_b][phi_b][lev_b][0]->Integral()+pdd*hcls[theta_b][phi_b][lev_b][1]->Integral());
	    if(sums>0.0) {
	      pds=bs/sums;
	    } else {
	      pds=0.0;
	    }
	    
	    if(sumd>0.0) {
	      pdd=bd/sumd;
	    } else {
	      pdd=0.0;
	    }
	    
	    if((pds*sums+pdd*sumd)>0.0) {
	      ps=pds*sums/(pds*sums+pdd*sumd);
	      pd=pdd*sumd/(pds*sums+pdd*sumd);
	    } else {
	      ps=0.0;
	      pd=0.0;
	    }
	    
	    hcls_p[theta_b][phi_b][lev_b][module][0]->SetBinContent(b,pds);
	    hcls_p[theta_b][phi_b][lev_b][module][1]->SetBinContent(b,pdd);


	    
	    //if(phi_b==0)  printf("lev=%i the=%i phi=%i module=%i cls=%i bs=%f bd=%f pds=%f pdd=%f ps=%f pd=%f\n",lev_b,theta_b,phi_b,module,b*10,bs,bd,pds,pdd,ps,pd);
	    fprintf(myoutfile,"%2i %2i %2i %2i %4i %7.4f %7.4f %7.4f  %7.4f\n",lev_b,theta_b,phi_b,module,b*10,pds,pdd,ps,pd);
	  }
	  
	  
	  for(Int_t b=0;b<hnwires[theta_b][phi_b][lev_b][module][0]->GetNbinsX();b++) {
	    //  pds=hnwires[theta_b][phi_b][lev_b][0]->GetBinContent(b)/hnwires[theta_b][phi_b][lev_b][0]->Integral();
	    // 	  pdd=hnwires[theta_b][phi_b][lev_b][1]->GetBinContent(b)/hnwires[theta_b][phi_b][lev_b][1]->Integral();
	    // 	  ps=(pds*hnwires[theta_b][phi_b][lev_b][0]->Integral())/(pds*hnwires[theta_b][phi_b][lev_b][0]->Integral()+pdd*hnwires[theta_b][phi_b][lev_b][1]->Integral());
	    // 	  pd=(pdd*hnwires[theta_b][phi_b][lev_b][1]->Integral())/(pds*hnwires[theta_b][phi_b][lev_b][0]->Integral()+pdd*hnwires[theta_b][phi_b][lev_b][1]->Integral());
	    
	    bs=hnwires[theta_b][phi_b][lev_b][module][0]->GetBinContent(b);
	    bd=hnwires[theta_b][phi_b][lev_b][module][1]->GetBinContent(b);
	    sumd=hnwires[theta_b][phi_b][lev_b][module][1]->Integral();
	    sums=hnwires[theta_b][phi_b][lev_b][module][0]->Integral();
	    
	    
	    if(sums>0.0) {
	      pds=bs/sums;
	    } else {
	      pds=0.0;
	    }
	    
	    if(sumd>0.0) {
	    pdd=bd/sumd;
	    } else {
	      pdd=0.0;
	    }
	    
	    if((pds*sums+pdd*sumd)>0.0) {
	      ps=pds*sums/(pds*sums+pdd*sumd);
	      pd=pdd*sumd/(pds*sums+pdd*sumd);
	    } else {
	      ps=0.0;
	      pd=0.0;
	    }
	    
       
	    hnwires_p[theta_b][phi_b][lev_b][module][0]->SetBinContent(b,pds);
	    hnwires_p[theta_b][phi_b][lev_b][module][1]->SetBinContent(b,pdd);
	    
	    
	    
	    // if(phi_b==0)  printf("lev=%i the=%i phi=%i module=%i nwires=%i bs=%f bd=%f pds=%f pdd=%f ps=%f pd=%f\n",lev_b,theta_b,phi_b,module,b+4,bs,bd,pds,pdd,ps,pd);
	    fprintf(myoutfile,"%2i %2i %2i %2i %4i %7.4f %7.4f %7.4f  %7.4f\n",lev_b,theta_b,phi_b,module,b+4,pds,pdd,ps,pd);
	  }
	}
      }
    }
  }
    fprintf(myoutfile,"##################\n");
    fclose(myoutfile); 


  printf("filling   histograms to file \n"); 
  TFile* outfile = new TFile(outputname,"RECREATE");
  outfile->cd();
  for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
    //loop over levels
    for(Int_t theta_b=0;theta_b<theta_nbin;theta_b++){
      //loop over theta
      for(Int_t phi_b=0;phi_b<phi_nbin;phi_b++){
	for(Int_t module=0;module<2;module++){
	  hcls[theta_b][phi_b][lev_b][module][0]->Write();
	  hcls[theta_b][phi_b][lev_b][module][1]->Write();
	  hnwires[theta_b][phi_b][lev_b][module][0]->Write();
	  hnwires[theta_b][phi_b][lev_b][module][1]->Write();
	  hcls_p[theta_b][phi_b][lev_b][module][0]->Write();
	  hcls_p[theta_b][phi_b][lev_b][module][1]->Write();
	  hnwires_p[theta_b][phi_b][lev_b][module][0]->Write();
	  hnwires_p[theta_b][phi_b][lev_b][module][1]->Write();


	}
      }
    }
  }
  outfile->Close();

  
  //***********************************
  printf("check performance\n");
  
  
  HPidCPParam par(outputname);
  //in this method I calculate averall efficiency of cuts 
  //  Int_t the_b,phi_b,lev_b0,lev_b1;
  Int_t the_b,phi_b,lev_b0;
  Int_t dim=10;
  Float_t good_s[dim];
  Float_t lost_s[dim];
  Float_t wrong_s[dim];
  Float_t rejdoub_s[dim];
  Float_t prob[dim];
  prob[0]=0.0;
  prob[1]=0.1;
  prob[2]=0.2;
  prob[3]=0.3;
  prob[4]=0.4;
  prob[5]=0.5;
  prob[6]=0.6;
  prob[7]=0.7;
  prob[8]=0.8;
  prob[9]=0.9;
  //  Int_t cls_part_0=-1,cls_part_1=-1,nwires_part_0=-1,nwires_part_1=-1;
  Int_t cls_part_0=-1,nwires_part_0=-1;

  HMdcFunc1 fun;
 
  for (Int_t i=0;i<dim;i++){
    good_s[i]=0.0;
    lost_s[i]=0.0;
    wrong_s[i]=0.0;
    rejdoub_s[i]=0.0;
  }
  Float_t test_sing=12000.0;
  Float_t test_doub=4000.0;
  test_sing=mdc_sing->GetEntries();
  test_doub=mdc_doub->GetEntries();
  Float_t prob_tr=0.0;
  Float_t cannot_decide_sin=0.0;
  Float_t cannot_decide_doub=0.0;
  printf("test singles %f\n",test_sing);
  for (Int_t i=0;i<test_sing;i++){
    mdc_sing->GetEntry(i);
    values=mdc_sing->GetArgs();
    if(i%1000==0) printf("i=%i\n",i);
    the_b=fun.calculateBin(values[0],0);
    phi_b=fun.calculateBin(values[1],1);
    
    


    lev_b0=fun.calculateLevelBin((Int_t)values[4]);
    //lev_b1=fun.calculateLevelBin(values[7]);

    cls_part_0=(Int_t) values[2];
    //cls_part_1=(Int_t) values[5];
    nwires_part_0=(Int_t) values[3];
    //nwires_part_1=(Int_t) values[6];
    //charge_part=(Int_t) values[6];
    
    //prob_tr=par.getProbSingle(the_b,phi_b,lev_b0,lev_b1,cls_part_0,cls_part_1,nwires_part_0,nwires_part_1);
    prob_tr=par.getProbSingle(the_b,phi_b,lev_b0,cls_part_0,nwires_part_0,(Int_t)values[5]);
    

    //if(prob_tr>-1.0&&(lev_b0==0||lev_b1==0)){
    if(prob_tr>-1.0){
      for (Int_t ii=0;ii<dim;ii++){
	//  printf("sing t= %i p= %i l0=%i l1=%i cls0=%i nwires0=%i cls1=%i nwires1=%i charge=%i\n",the_b,phi_b,lev_b0,lev_b1,cls_part_0,nwires_part_0,cls_part_1,nwires_part_1,charge_part);
	
	// printf("probsin=%f test=%f\n",par.getProbSingle(the_b,phi_b,lev_b0,lev_b1,cls_part_0,cls_part_1,nwires_part_0,nwires_part_1,charge_part),prob[ii]);
	
	if (prob_tr>prob[ii]) {
	  good_s[ii]=good_s[ii]+1.0; 
	} else {
	  lost_s[ii]=lost_s[ii]+1.0;;  
	}
      }
    } else {
      cannot_decide_sin=cannot_decide_sin+1.0;
    }
  }
 
 
  printf("test doubles %f\n",test_doub);
  for (Int_t i=0;i<test_doub;i++){
    mdc_doub->GetEntry(i);
    values=mdc_doub->GetArgs();
    if(i%1000==0) printf("i=%i\n",i);
    the_b=fun.calculateBin(values[0],0);
    phi_b=fun.calculateBin(values[1],1);   // changed : bug?
    lev_b0=fun.calculateLevelBin((Int_t)values[4]);
    //lev_b1=fun.calculateLevelBin(values[7]);
    
    cls_part_0=(Int_t) values[2];
    //cls_part_1=(Int_t) values[5];
    nwires_part_0=(Int_t) values[3];
    //nwires_part_1=(Int_t) values[6];
    //charge_part=(Int_t) values[6];
    //prob_tr=par.getProbSingle(the_b,phi_b,lev_b0,lev_b1,cls_part_0,cls_part_1,nwires_part_0,nwires_part_1);
    
    prob_tr=par.getProbSingle(the_b,phi_b,lev_b0,cls_part_0,nwires_part_0,(Int_t)values[5]);


  //if(prob_tr>-1.0&&(lev_b0==0||lev_b1==0)){ 
    if(prob_tr>-1.0){ 
      for (Int_t ii=0;ii<dim;ii++){
	if (prob_tr>prob[ii]) {
	  wrong_s[ii]=wrong_s[ii]+1.0; 
	} else {
	  rejdoub_s[ii]=rejdoub_s[ii]+1.0;;  
	}
      }
    } else {
      cannot_decide_doub=cannot_decide_doub+1.0; 
    }
  }
  
  printf("======================================\n");
  printf("OVERALLPERFORMANCE\n");
  for (Int_t ii=0;ii<dim;ii++){ 
    printf("****************************\n");
    printf("PropSing=%f \n",prob[ii]);
    printf("FOUND singles=%f \n",good_s[ii]);
    printf("LOST singles=%f \n",lost_s[ii]); 
    printf("WRONG singles=%f \n",wrong_s[ii]);
    printf("REJ doubles=%f \n",rejdoub_s[ii]);
    printf("-------------------------------------\n");
    printf("EFF=%f\n",good_s[ii]/(test_sing-cannot_decide_sin));
    printf("S/B=%f\n",good_s[ii]/wrong_s[ii]);
    printf("REJ EFF=%f\n",rejdoub_s[ii]/(test_doub-cannot_decide_doub));
    printf("****************************\n");
  }
  printf("-------------------------------------\n");
  printf("can not decide sin=%f\n",cannot_decide_sin);
  printf("can not decide doub=%f\n",cannot_decide_doub);
}


Int_t HMdcCPCutsFinder::calculateMdcThetaBin(Float_t theta){
 
  Int_t theta_bin;
  
  //theta=TMath::Abs(theta);
  theta_bin=(Int_t) theta/theta_bin_size-1;
  if(theta_bin<theta_nbin){
    return   theta_bin;
  } else {
    return  -1;
  }
}

Int_t HMdcCPCutsFinder::calculateMdcPhiBin(Float_t mdc_phi){
  Int_t mdc_phi_bin;
  // mdc_phi=TMath::Abs(mdc_phi);
  if(((Int_t) mdc_phi%60)<30.0) {
    mdc_phi_bin=(Int_t) mdc_phi%60/phi_bin_size;
  }else{
    mdc_phi_bin=(Int_t) (60.0-(Int_t)mdc_phi%60)/phi_bin_size; 
  }
  
  if(mdc_phi_bin<phi_nbin){
    return   mdc_phi_bin;
  } else {
    return  -1;
  }
  
}

Int_t HMdcCPCutsFinder::calculateBin(Int_t mdc_angle,Int_t rich_angle,Int_t option) {
  // calculate the the bin of the angle (option=0 ... theta; =1 ... phi)
  // if theta, phi angles do not fall down to the same bin than return -1
  // in a case angles do not fall to the expecting range -2 is returned
  // in a case that rich bin and mdc bin are not identical -3 is returned

  Int_t mdc_theta_bin=-9;
  Int_t rich_theta_bin=-9;
  Int_t mdc_phi_bin=-9;
  Int_t rich_phi_bin=-9;

  switch (option) {
  case 0:
    // case theta
    if(mdc_angle>theta_min&&mdc_angle<theta_max) {
      mdc_theta_bin=((Int_t)(mdc_angle-theta_min))/theta_bin_size;
    } else {
      return -2;
    }
    //look at rich
    rich_theta_bin=rich_angle/theta_bin_size-1;
    
    if(rich_theta_bin==mdc_theta_bin) {
      return mdc_theta_bin;  
    }else{
      return -3;  
    }

  case 1:
    if(mdc_angle>0.0&&mdc_angle<360.0){
     mdc_phi_bin=(((Int_t) mdc_angle)%60)/phi_bin_size; 
    if(mdc_phi_bin>2) mdc_phi_bin=5-rich_phi_bin ;//e.g. 10-10 and 50-60 is bin 0 
    }else{
      return -2;
    }
    rich_phi_bin=(((Int_t) rich_angle)%60)/phi_bin_size; 
    if(rich_phi_bin>2) rich_phi_bin=5-rich_phi_bin ;//e.g. 10-10 and 50-60 is bin 0
    
    if(rich_phi_bin==mdc_phi_bin) {
      return mdc_phi_bin;  
    }else{
      return -3;  
    }
  
  default: 
    printf("Third argument should be 0-1 for theta-phi angle\n");
    return -1;
  }
}


void  HMdcCPCutsFinder::printContainer() { 
  for(Int_t h=0;h<3;h++){
    for(Int_t i=0;i<8;i++){
      for(Int_t j=0;j<3;j++){
	for(Int_t k=0;k<4;k++){
	  
	  printf("%2i %2i %2i efbin:%2i  ef:%4.2f s/b:%6.2f size:%4.0f wires:%4.0f charge:%5.0f\n",h,i,j,k,container[h][i][j][k][0],container[h][i][j][k][1],container[h][i][j][k][2],container[h][i][j][k][3],container[h][i][j][k][4]);
	  
	}
	printf("------------------------------------------------------------------------------\n");
      }
    }
  }
  
  
}

void  HMdcCPCutsFinder::calculateOverallRejection() { 
  //in this method I calculate averall efficiency of cuts 
  Int_t theta_b,phi_b,lev_b;
  Float_t good_s[4];
  Float_t lost_s[4];
  Float_t wrong_s[4];
  Float_t rejdoub_s[4];
  Float_t * values;
  
  for (Int_t i=0;i<4;i++){
    good_s[i]=0.0;
    lost_s[i]=0.0;
    wrong_s[i]=0.0;
    rejdoub_s[i]=0.0;
  }
  Int_t count_sin=0,count_doub=0;
  for (Int_t i=0;i<sing->GetEntries();i++){
  //==for (Int_t i=0;i<10000;i++){
    sing->GetEntry(i);
    values=sing->GetArgs();

    theta_b=calculateMdcThetaBin(values[0]);
    phi_b=calculateMdcPhiBin(values[1]);
    lev_b=calculateMdcLevBin(values[10]);
    //** delete next
    if(lev_b==1){
      count_sin++;
      for (Int_t e=0;e<4;e++){
	//for 4 various efficiencies I canculate s/b 
	//**   if((values[7]<container[lev_b][theta_b][phi_b][e][2])&&(values[8]<container[lev_b][theta_b][phi_b][e][3])&&(values[6]<container[lev_b][theta_b][phi_b][e][4]
	///**)){
	if((values[7]<container[lev_b][theta_b][phi_b][e][2])&&(values[8]<container[lev_b][theta_b][phi_b][e][3])){
	  good_s[e]=good_s[e]+1.0;	
	} else {
	  lost_s[e]=lost_s[e]+1.0;
	}
      }
    }
  }
  
  
for (Int_t i=0;i<doub->GetEntries();i++){
  //===for (Int_t i=0;i<5000;i++){
    doub->GetEntry(i);
    values=doub->GetArgs();
    
    
    theta_b=calculateMdcThetaBin(values[0]);
    phi_b=calculateMdcPhiBin(values[1]);
    lev_b=calculateMdcLevBin(values[10]);
    //**
    if(lev_b==1){
      count_doub++;
      for (Int_t e=0;e<4;e++){
	//for 4 various efficiencies I canculate s/b 
	//** if((values[7]<container[lev_b][theta_b][phi_b][e][2])&&(values[8]<container[lev_b][theta_b][phi_b][e][3])&&(values[6]<container[lev_b][theta_b][phi_b][e][4])){
	if((values[7]<container[lev_b][theta_b][phi_b][e][2])&&(values[8]<container[lev_b][theta_b][phi_b][e][3])){  
	  wrong_s[e]=wrong_s[e]+1.0;	
	} else {
	  rejdoub_s[e]=rejdoub_s[e]+1.0;
	}
      }
    }
}
  
  printf("======================================\n");
  printf("OVERALLPERFORMANCE\n");
  for (Int_t ii=0;ii<4;ii++){ 
    printf("****************************\n");
    printf("EFF BIN=%f \n",(80.0+5.0*ii));
    printf("FOUND singles=%f \n",good_s[ii]);
    printf("LOST singles=%f \n",lost_s[ii]); 
    printf("WRONG singles=%f \n",wrong_s[ii]);
    printf("REJ doubles=%f \n",rejdoub_s[ii]);
    printf("-------------------------------------\n");
    //** printf("EFF=%f\n",good_s[ii]/sing->GetEntries());
    printf("EFF=%f\n",good_s[ii]/count_sin);
    printf("S/B=%f\n",good_s[ii]/wrong_s[ii]);
    //** printf("REJ EFF=%f\n",rejdoub_s[ii]/doub->GetEntries());
    printf("REJ EFF=%f\n",rejdoub_s[ii]/count_doub);
    printf("****************************\n");
  }
}



void  HMdcCPCutsFinder::printContainer2File(const char* fname) { 
 
 //  // puts the ascii header to the string used in HMdcParAsciiFileIo
  TString  header=
    "###################################################################################################\n"
    "# Lepton Single/Double recognition parameters \n"
    "# Format: \n"
    "# levelbin thetabin phibin effbin eff signal/background mdcclustersize_cut mdcnwires_cut  richintcharge_cut\n"
    "###################################################################################################\n"
    "[MdcClosePairCutsPar]\n";
  
  FILE* myoutfile =fopen(fname,"w");
  fprintf(myoutfile,"%s\n",header.Data());
  for(Int_t h=0;h<3;h++){
    for(Int_t i=0;i<8;i++){
      for(Int_t j=0;j<3;j++){
	for(Int_t k=0;k<4;k++){
	  fprintf(myoutfile,"%2i %2i %2i %2i %4.2f %6.2f %4.0f %4.0f  %5.0f\n",h,i,j,k,container[h][i][j][k][0],container[h][i][j][k][1],container[h][i][j][k][2],container[h][i][j][k][3],container[h][i][j][k][4]);
	  
	}
      }
    }
  }
  fprintf(myoutfile,"##################\n");
  fclose(myoutfile); 
}

Bool_t  HMdcCPCutsFinder::inBin(Float_t mdc_theta,Float_t mdc_phi,Float_t level,Int_t theta_bin,Int_t phi_bin,Int_t lev_bin) {
  //checking if tracklet with level, mdc_theta and mdc_phi values belongs to bin: theta_bin,phi_bin,lev_bin
  Int_t mdc_theta_bin, mdc_phi_bin,mdc_lev_bin;
  Int_t test=0;
  // if(mdc_theta<20) test=1;
  if(test==1){
    cout<<"============"<<endl;
    // printf("mdc_theta=%f\n",mdc_theta);

  }
  
  mdc_theta=TMath::Abs(mdc_theta);
  mdc_phi=TMath::Abs(mdc_phi);

  if(test==1){
    printf("mdc_theta=%f\n",mdc_theta);
    printf("mdc_phi=%f\n",mdc_phi);
  }


  mdc_theta_bin=(Int_t) mdc_theta/theta_bin_size-1;
 
  if(test==1){
    printf("mdc_theta_bin=%i\n",mdc_theta_bin);
   
  }
  
  if(((Int_t) mdc_phi%60)<30) {
    mdc_phi_bin=((Int_t) mdc_phi%60)/phi_bin_size;
  }else{
    mdc_phi_bin=(((Int_t) (60.0-mdc_phi)%60))/phi_bin_size; 
  }
 
  if(test==1){
    printf("mdc_theta_bin=%i theta_bin=%i mdc_phi_bin=%i phi_bin=%i phi_bin_size=%i \n",mdc_theta_bin,theta_bin,mdc_phi_bin,phi_bin,phi_bin_size);
  }
  
  mdc_lev_bin=(Int_t )level-4;

  if((mdc_theta_bin==theta_bin)&&(mdc_phi_bin==phi_bin)&&(mdc_lev_bin==lev_bin)){
    // printf("kTRUE\n"); 
    return kTRUE;
  }
  //printf("kFALSE\n");
  return kFALSE;
}
ClassImp(HMdcCPCutsFinder)
