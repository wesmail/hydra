using namespace std;
#include "hmdccpcutsfinder.h"
#include <iostream> 
#include <iomanip>
#include <TFile.h>
#include <TString.h>
//*-- Author : J.Bielcik
//*-- Modified :20.2.2002
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
  
  
  
  sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
  doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");


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
  
 
  
  sing=new TNtuple("sing","sing","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");
  doub=new TNtuple("doub","doub","m_the:m_phi:r_the:r_phi:clussize:nwires:charge:mergecls:sig1:sig2:alpha:level");


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
  printf("read singles=%f\n",s->GetEntries());
  for (Int_t i=0;i<s->GetEntries();i++){
    s->GetEntry(i);
    values=s->GetArgs();
    sing->Fill(values);
  }
  return kTRUE;
}

Bool_t  HMdcCPCutsFinder::readDoubles(TNtuple * d) { 
  Float_t* values;
  printf("read doubles=%f\n",d->GetEntries());
  for (Int_t i=0;i<d->GetEntries();i++){
    d->GetEntry(i);
    values=d->GetArgs();
    doub->Fill(values);
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

  printf("number of singles in analysis:%f\n",sing->GetEntries());
  printf("number of doubles in analysis:%f\n",doub->GetEntries());
  for(Int_t lev_b=0;lev_b<lev_nbin;lev_b++){
    //loop over levels
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
	  sing->GetEntry(i);
	  values=sing->GetArgs();
	  // printf("values[0]=%f values[1]=%f \n",values[0],values[1]);
	  if(inBin(values[0],values[1],values[11],theta_b,phi_b,lev_b)) sing_inbin++;
	}
	
	for (Int_t i=0;i<doub->GetEntries();i++){
	  doub->GetEntry(i);
	  values=doub->GetArgs();
	  if(inBin(values[0],values[1],values[11],theta_b,phi_b,lev_b)) doub_inbin++;
	}
	
	printf("BIN %i %i %i sin_inbin=%i  doub_inbin=%i \n",lev_b, theta_b,phi_b,sing_inbin,doub_inbin);
	
	
	for(Int_t mdc_cluster_cut=0;mdc_cluster_cut<mdc_cluster_max+1;mdc_cluster_cut=mdc_cluster_cut+mdc_cluster_step){
	  for(Int_t mdc_wires_cut=4;mdc_wires_cut<mdc_wires_max+1;mdc_wires_cut=mdc_wires_cut+mdc_wires_step){
	    for(Int_t rich_charge_cut=0;rich_charge_cut<rich_charge_max+1;rich_charge_cut=rich_charge_cut+rich_charge_step){
	      //go over sing
	      
	      for (Int_t i=0;i<sing->GetEntries();i++){
		sing->GetEntry(i);
		values=sing->GetArgs();
		if(inBin(values[0],values[1],values[11],theta_b,phi_b,lev_b)){
		  if((values[4]<mdc_cluster_cut)&&(values[5]<mdc_wires_cut)&&(values[6]<rich_charge_cut)) {
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
		doub->GetEntry(i);
		values=doub->GetArgs();
		if(inBin(values[0],values[1],values[11],theta_b,phi_b,lev_b)){
		  if((values[4]<mdc_cluster_cut)&&(values[5]<mdc_wires_cut)&&(values[6]<rich_charge_cut)) {
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
	      
	      if(ef>0.8) {
		if(container[lev_b][theta_b][phi_b][0][1]<S_B){
		  container[lev_b][theta_b][phi_b][0][0]= ef ;
		  container[lev_b][theta_b][phi_b][0][1]= S_B;
		  container[lev_b][theta_b][phi_b][0][2]=mdc_cluster_cut;
		  container[lev_b][theta_b][phi_b][0][3]=mdc_wires_cut;
		  
		  container[lev_b][theta_b][phi_b][0][4]=rich_charge_cut;
		}
	      } 
	      
	      if(ef>0.85) {
		if(container[lev_b][theta_b][phi_b][1][1]<S_B){
		  container[lev_b][theta_b][phi_b][1][0]= ef ;
		  container[lev_b][theta_b][phi_b][1][1]= S_B;
		  container[lev_b][theta_b][phi_b][1][2]=mdc_cluster_cut;
		  container[lev_b][theta_b][phi_b][1][3]=mdc_wires_cut;
		  
		  container[lev_b][theta_b][phi_b][1][4]=rich_charge_cut;
		}
	      }
	      
	      if(ef>0.9) {
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
	    }
	  }
	}
	if(ef_max<0.8) printf("IN THIS BIN :%i %i %i  max eff was only=%f\n",lev_b,theta_b,phi_b,ef_max);
	
	for(Int_t a=0;a<4;a++)  printf("%2i %2i %2i efbin:%2i  ef:%4.2f  s/b:%6.2f  size:%4.0f wires:%4.0f  charge:%5.0f\n",lev_b,theta_b,phi_b,a,container[lev_b][theta_b][phi_b][a][0],container[lev_b][theta_b][phi_b][a][1],container[lev_b][theta_b][phi_b][a][2],container[lev_b][theta_b][phi_b][a][3],container[lev_b][theta_b][phi_b][a][4]);
	printf("------------------------------------------------------------------------------\n");
      }
    }
  }
  return kTRUE;
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
    if(mdc_angle>phi_min&&mdc_angle<phi_max){
      if(mdc_angle<30.0) {
	mdc_phi_bin=mdc_angle/phi_bin_size;
      }else{
	mdc_phi_bin=((Int_t)(60.0-mdc_angle))/phi_bin_size; 
      }
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
  
  for (Int_t i=0;i<sing->GetEntries();i++){
    sing->GetEntry(i);
    values=sing->GetArgs();

    theta_b=calculateMdcThetaBin(values[0]);
    phi_b=calculateMdcPhiBin(values[1]);
    lev_b=calculateMdcLevBin(values[11]);
    
    for (Int_t e=0;e<4;e++){
      //for 4 various efficiencies I canculate s/b 
      if((values[4]<container[lev_b][theta_b][phi_b][e][2])&&(values[5]<container[lev_b][theta_b][phi_b][e][3])&&(values[6]<container[lev_b][theta_b][phi_b][e][4]
)){
	good_s[e]=good_s[e]+1.0;	
      } else {
	lost_s[e]=lost_s[e]+1.0;
      }
    }
  }
  
  
  for (Int_t i=0;i<doub->GetEntries();i++){
    doub->GetEntry(i);
    values=doub->GetArgs();
    
    
    theta_b=calculateMdcThetaBin(values[0]);
    phi_b=calculateMdcPhiBin(values[1]);
    lev_b=calculateMdcLevBin(values[11]);

    for (Int_t e=0;e<4;e++){
      //for 4 various efficiencies I canculate s/b 
      if((values[4]<container[lev_b][theta_b][phi_b][e][2])&&(values[5]<container[lev_b][theta_b][phi_b][e][3])&&(values[6]<container[lev_b][theta_b][phi_b][e][4])){
	wrong_s[e]=wrong_s[e]+1.0;	
      } else {
	rejdoub_s[e]=rejdoub_s[e]+1.0;
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
    printf("EFF=%f\n",good_s[ii]/sing->GetEntries());
    printf("S/B=%f\n",good_s[ii]/wrong_s[ii]);
    printf("REJ EFF=%f\n",rejdoub_s[ii]/doub->GetEntries());
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
