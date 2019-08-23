#include "hpidcpparam.h"
#include <iostream>
#include "TH1F.h"
#include "TFile.h"
#include "hpidmdcfunc.h"
using namespace std;
//Autor: Jaroslav Bielcik
//Modified:09.04.2003
//////////////////////////////////////////////////////////////
//HPidCPParam is special and temporary kind of container for//
//determining prob to be single or Double_t based on mdc info //
//////////////////////////////////////////////////////////////

HPidCPParam::HPidCPParam(const Char_t* name_of_paramfile) {
//container is initilized with files full of histograms with pdf

  setHist=kFALSE;
  name_of_parfile=name_of_paramfile;
}

HPidCPParam::~HPidCPParam() {
;
}


Float_t HPidCPParam::getProbSingle(Float_t theta,Float_t phi,Int_t level0,Int_t level1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1, Int_t charge){
  // theta and phi are in hades coor system 0-90 0-360 and level is just 4-6
  HPidMdcFunc f;
  Int_t thbin=-1,phibin=-1,level_bin0=-1,level_bin1=-1;
  thbin=f.calculateBin(theta,0);
  phibin=f.calculateBin(phi,1);
  level_bin0=f.calculateLevelBin(level0);
  level_bin1=f.calculateLevelBin(level1);
  if(thbin>=0&&phibin>=0&&level_bin0>=0&&level_bin1>=0){
    return getProbSingle(thbin,phibin,level_bin0,level_bin1,cls0,cls1,nwires0,nwires1,charge); 
  } else {
    printf("HPidCPParam::getProbSingle bins in argument out of range\n");
    return -1.0; 
  }
 }



Float_t HPidCPParam::getProbSingle(Int_t thbin,Int_t phibin,Int_t level_bin0,Int_t level_bin1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1, Int_t charge){
  //thbin 0-7 (10-20;...;80-90)
  //phibin 0-2 (0:[0-10 and 50-60 +(60*sector)];..;2:[20-30 and 30-40 +(60*sector)])
  //level bin 0-2 (4;5;6)

  if(thbin>=0&&thbin<=7&&phibin>=0&&phibin<=2&&level_bin0>=0&&level_bin0<=2&&level_bin1>=0&&level_bin1<=2) {

    Char_t name_s[200];
    TFile file_with_params(name_of_parfile.Data());
    file_with_params.cd();
    
    //constuct name from indexes => retrieve proper histo and than 
    
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",thbin,"_",phibin,"_",level_bin0,"_",0);
    
    TH1F * cls0ps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_pd_",thbin,"_",phibin,"_",level_bin0,"_",0);
    TH1F * cls0pd=(TH1F *)file_with_params.Get(name_s);
    
    
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * cls1ps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_pd_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * cls1pd=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",thbin,"_",phibin,"_",level_bin0,"_",0);
    TH1F * nwires0ps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_pd_",thbin,"_",phibin,"_",level_bin0,"_",0);
    TH1F * nwires0pd=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_pd_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * nwires1ps=(TH1F *)file_with_params.Get(name_s);
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * nwires1pd=(TH1F *)file_with_params.Get(name_s);
    
    
    sprintf(name_s,"%s%i%s%i%s%i","intcharge_ps_",thbin,"_",phibin,"_",level_bin1);
    TH1F * chargeps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i","intcharge_pd_",thbin,"_",phibin,"_",level_bin1);
    TH1F * chargepd=(TH1F *)file_with_params.Get(name_s);
    
    
    sprintf(name_s,"%s%i%s%i%s%i","intcharge_s_",thbin,"_",phibin,"_",level_bin1);
    TH1F * charges=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i","intcharge_d_",thbin,"_",phibin,"_",level_bin1);
    TH1F * charged=(TH1F *)file_with_params.Get(name_s);
    
    
    
    Float_t Ps_cls0=0.0,Ps_cls1=0.0,Ps_nwires0=0.0,Ps_nwires1=0.0,Ps_charge=0.0;
    Float_t Pd_cls0=0.0,Pd_cls1=0.0,Pd_nwires0=0.0,Pd_nwires1=0.0,Pd_charge=0.0;
    
    Float_t Ns=-1.0, Nd=-1.0; 
    Float_t PS=-1.0, PD=-1.0;

    Int_t c_bin= (Int_t) (cls0ps->GetXaxis()->GetXmax()-cls0ps->GetXaxis()->GetXmin())/cls0ps->GetNbinsX();
    
    Int_t w_bin= (Int_t) (nwires1ps->GetXaxis()->GetXmax()-nwires1ps->GetXaxis()->GetXmin())/nwires1ps->GetNbinsX();
    
    Int_t i_bin= (Int_t) (chargeps->GetXaxis()->GetXmax()-chargeps->GetXaxis()->GetXmin())/chargeps->GetNbinsX();
    
    // printf("bin %i %i %i \n", c_bin, w_bin,i_bin);
    
    
    Int_t cls_bin0=(Int_t ) cls0/c_bin+1;
    Int_t cls_bin1=(Int_t ) cls1/c_bin+1;
    
    Int_t nwires_bin0=(Int_t )  (nwires0/w_bin-nwires0ps->GetXaxis()->GetXmin());
    Int_t nwires_bin1=(Int_t )  (nwires1/w_bin-nwires1ps->GetXaxis()->GetXmin());
    
    Int_t intcharge_bin=(Int_t )  charge/i_bin;
    
    // printf("bin  cls_bin0=%i cls_bin1=%i nwires_bin0=%i nwires_bin1=%i intcharge_bin=%i \n", cls_bin0,cls_bin1,nwires_bin0,nwires_bin1,intcharge_bin);


    //calculate Ps
    Ps_cls0=cls0ps->GetBinContent(cls_bin0);
    Ps_cls1=cls1ps->GetBinContent(cls_bin1);
    
    Ps_nwires0=nwires0ps->GetBinContent(nwires_bin0);
    Ps_nwires1=nwires1ps->GetBinContent(nwires_bin1);
    
    Ps_charge=chargeps->GetBinContent(intcharge_bin);
    
    Pd_cls0=cls0pd->GetBinContent(cls_bin0);
    Pd_cls1=cls1pd->GetBinContent(cls_bin1);

    Pd_nwires0=nwires0pd->GetBinContent(nwires_bin0);
    Pd_nwires1=nwires1pd->GetBinContent(nwires_bin1);
    
    Pd_charge=chargepd->GetBinContent(intcharge_bin);
    
    Ns=charges->Integral();
    
    Nd=charged->Integral();
    
    Float_t div=(Ps_cls0*Ps_cls1*Ps_nwires0*Ps_nwires1*Ps_charge*Ns+Pd_cls0*Pd_cls1*Pd_nwires0*Pd_nwires1*Pd_charge*Nd);
    if(div>0){
      
      PS=Ps_cls0*Ps_cls1*Ps_nwires0*Ps_nwires1*Ps_charge*Ns/div;
      PD=Pd_cls0*Pd_cls1*Pd_nwires0*Pd_nwires1*Pd_charge*Nd/div;   
      // printf("PS=%f PD=%f\n",PS,PD);
    }
    file_with_params.Close();
    
    return PS;
  } else {
    //printf("HPidCPParam::getProbSingle bins in argument out of range\n");
    return -1.0;
    
  }
}

Float_t HPidCPParam::getProbSingle(Float_t theta,Float_t phi,Int_t level0,Int_t level1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1){
  // theta and phi are in hades coor system 0-90 0-360 and level is just 4-6
  HPidMdcFunc f;
  Int_t thbin=-1,phibin=-1,level_bin0=-1,level_bin1=-1;
  thbin=f.calculateBin(theta,0);
  phibin=f.calculateBin(phi,1);
  level_bin0=f.calculateLevelBin(level0);
  level_bin1=f.calculateLevelBin(level1);
  if(thbin>=0&&thbin<=7&&phibin>=0&&phibin<=2&&level_bin0>=0&&level_bin0<=2&&level_bin1>=0&&level_bin1<=2){
    return getProbSingle(thbin,phibin,level_bin0,level_bin1,cls0,cls1,nwires0,nwires1); 
  } else {
    
    return -1.0; 
  }
  
}

Float_t HPidCPParam::getProbSingle(Float_t theta,Float_t phi,Int_t level,Int_t cls,Int_t nwires,Int_t module){
  // theta and phi are in hades coor system 0-90 0-360 and level is just 4-6
  HPidMdcFunc f;
  Int_t thbin=-1,phibin=-1,level_bin=-1;
  thbin=f.calculateBin(theta,0);
  phibin=f.calculateBin(phi,1);
  level_bin=f.calculateLevelBin(level);
  
  if(thbin>=0&&thbin<=7&&phibin>=0&&phibin<=2&&level_bin>=0&&level_bin<=2&&module>=0&&module<=1){
    return getProbSingle(thbin,phibin,level_bin,cls,nwires,module); 
  } else {
    return -1.0; 
  }


}

Float_t HPidCPParam::getProbSingle(Int_t thbin,Int_t phibin,Int_t level_bin0,Int_t level_bin1,Int_t cls0,Int_t cls1,Int_t nwires0,Int_t nwires1){
  //thbin 0-7 (10-20;...;80-90)
  //phibin 0-2 (0:[0-10 and 50-60 +(60*sector)];..;2:[20-30 and 30-40 +(60*sector)])
  //level bin 0-2 (4;5;6)
 
  
  if(thbin>=0&&thbin<=7&&phibin>=0&&phibin<=2&&level_bin0>=0&&level_bin0<=2&&level_bin1>=0&&level_bin1<=2) {
    
    Char_t name_s[200];
    
    TFile file_with_params(name_of_parfile.Data());
    
    file_with_params.cd();
    
    //constuct name from indexes => retrieve proper histo and than 
    
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",thbin,"_",phibin,"_",level_bin0,"_",0);
    
    TH1F * cls0ps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_pd_",thbin,"_",phibin,"_",level_bin0,"_",0);
    TH1F * cls0pd=(TH1F *)file_with_params.Get(name_s);
    
    
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * cls1ps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_pd_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * cls1pd=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",thbin,"_",phibin,"_",level_bin0,"_",0);
    TH1F * nwires0ps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_pd_",thbin,"_",phibin,"_",level_bin0,"_",0);
    TH1F * nwires0pd=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * nwires1ps=(TH1F *)file_with_params.Get(name_s);
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_pd_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * nwires1pd=(TH1F *)file_with_params.Get(name_s);
    
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_s_",thbin,"_",phibin,"_",level_bin1,"_",0);
    TH1F * cls_s0=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_d_",thbin,"_",phibin,"_",level_bin1,"_",0);
    TH1F * cls_d0=(TH1F *)file_with_params.Get(name_s);

    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_s_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * cls_s1=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_d_",thbin,"_",phibin,"_",level_bin1,"_",1);
    TH1F * cls_d1=(TH1F *)file_with_params.Get(name_s);
    
    
    Float_t Ps_cls0=0.0,Ps_cls1=0.0,Ps_nwires0=0.0,Ps_nwires1=0.0;
    Float_t Pd_cls0=0.0,Pd_cls1=0.0,Pd_nwires0=0.0,Pd_nwires1=0.0;
    
    Float_t Ns1=-1.0, Nd1=-1.0,Ns0=-1.0, Nd0=-1.0;  
    Float_t PS=-1.0, PD=-1.0;

    Int_t c_bin= (Int_t) (cls0ps->GetXaxis()->GetXmax()-cls0ps->GetXaxis()->GetXmin())/cls0ps->GetNbinsX();
    
    Int_t w_bin= (Int_t) (nwires1ps->GetXaxis()->GetXmax()-nwires1ps->GetXaxis()->GetXmin())/nwires1ps->GetNbinsX();
    
    
    
   
    
    
    Int_t cls_bin0=(Int_t ) cls0/c_bin+1;
    Int_t cls_bin1=(Int_t ) cls1/c_bin+1;
    
    Int_t nwires_bin0=(Int_t )  (nwires0/w_bin-nwires0ps->GetXaxis()->GetXmin());
    Int_t nwires_bin1=(Int_t )  (nwires1/w_bin-nwires1ps->GetXaxis()->GetXmin());
    
   


    //calculate Ps
    Ps_cls0=cls0ps->GetBinContent(cls_bin0);
    Ps_cls1=cls1ps->GetBinContent(cls_bin1);
    
    Ps_nwires0=nwires0ps->GetBinContent(nwires_bin0);
    Ps_nwires1=nwires1ps->GetBinContent(nwires_bin1);
    
    // Ps_charge=chargeps->GetBinContent(intcharge_bin);
    
    Pd_cls0=cls0pd->GetBinContent(cls_bin0);
    Pd_cls1=cls1pd->GetBinContent(cls_bin1);

    Pd_nwires0=nwires0pd->GetBinContent(nwires_bin0);
    Pd_nwires1=nwires1pd->GetBinContent(nwires_bin1);
    
    
    
    Ns0=cls_s0->Integral();
    
    Nd0=cls_d0->Integral();

    Ns1=cls_s1->Integral();
    
    Nd1=cls_d1->Integral();


    
    //  printf("dCP:t=%i p=%i l0=%i c0=%i w0=%i l1=%i c1=%i w1=%i \n",thbin,phibin,level_bin0,cls0,nwires0,level_bin1,cls1,nwires1);
    // printf("Ps_c0=%f Ps_w0=%f Pd_c0=%f Pd_w0=%f Ns0=%f Nd0=%f \n",Ps_cls0,Ps_nwires0,Pd_cls0,Pd_nwires0,Ns0,Nd0);
    //printf("Ps_c1=%f Ps_w1=%f Pd_c1=%f Pd_w1=%f Ns1=%f Nd1=%f \n",Ps_cls1,Ps_nwires1,Pd_cls1,Pd_nwires1,Ns1,Nd1);
    
    Float_t div=(Ps_cls0*Ps_cls1*Ps_nwires0*Ps_nwires1*(Ns0+Ns1)+Pd_cls0*Pd_cls1*Pd_nwires0*Pd_nwires1*(Nd0+Nd1));
    if(div>0){
      
      PS=Ps_cls0*Ps_cls1*Ps_nwires0*Ps_nwires1*(Ns0+Ns1)/div;
      PD=Pd_cls0*Pd_cls1*Pd_nwires0*Pd_nwires1*(Nd0+Nd1)/div;   
      
    }
    file_with_params.Close();
    
    return PS;
  } else {
    // printf("HPidCPParam::getProbSingle bins in argument out of range\n");
    return -1.0;
    
  }
}

Float_t HPidCPParam::getProbSingle(Int_t thbin,Int_t phibin,Int_t level_bin,Int_t cls,Int_t nwires,Int_t module){
  //module 0-1 using info only from one chamber (for example if other doesnot exist)
  //thbin 0-7 (10-20;...;80-90)
  //phibin 0-2 (0:[0-10 and 50-60 +(60*sector)];..;2:[20-30 and 30-40 +(60*sector)])
  //level bin 0-2 (4;5;6)
  
  if(thbin>=0&&thbin<=7&&phibin>=0&&phibin<=2&&level_bin>=0&&level_bin<=2&&module>=0&&module<=1) {
    
    Char_t name_s[200];
    
    TFile file_with_params(name_of_parfile.Data());
    file_with_params.cd();
    
    //constuct name from indexes => retrieve proper histo and than 
    
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_ps_",thbin,"_",phibin,"_",level_bin,"_",module);
    TH1F * clsps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_pd_",thbin,"_",phibin,"_",level_bin,"_",module);
    TH1F * clspd=(TH1F *)file_with_params.Get(name_s);
    
       
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_ps_",thbin,"_",phibin,"_",level_bin,"_",module);
    TH1F * nwiresps=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","wires_pd_",thbin,"_",phibin,"_",level_bin,"_",module);
    TH1F * nwirespd=(TH1F *)file_with_params.Get(name_s);
    
        
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_s_",thbin,"_",phibin,"_",level_bin,"_",module);
    TH1F * cls_s=(TH1F *)file_with_params.Get(name_s);
    
    sprintf(name_s,"%s%i%s%i%s%i%s%i","cls_d_",thbin,"_",phibin,"_",level_bin,"_",module);
    TH1F * cls_d=(TH1F *)file_with_params.Get(name_s);
    
    Float_t Ps_cls=0.0,Ps_nwires=0.0;
    Float_t Pd_cls=0.0,Pd_nwires=0.0;
    
    Float_t Ns=-1.0, Nd=-1.0; 
    Float_t PS=-1.0, PD=-1.0;

    Int_t c_bin= (Int_t) (clsps->GetXaxis()->GetXmax()-clsps->GetXaxis()->GetXmin())/clsps->GetNbinsX();
    
    Int_t w_bin= (Int_t) (nwiresps->GetXaxis()->GetXmax()-nwiresps->GetXaxis()->GetXmin())/nwiresps->GetNbinsX();
   
    
    Int_t cls_bin=(Int_t ) cls/c_bin+1;
   
    
    Int_t nwires_bin=(Int_t )  (nwires/w_bin-nwiresps->GetXaxis()->GetXmin());
   
    
    

    //calculate Ps
    Ps_cls=clsps->GetBinContent(cls_bin);
    Ps_nwires=nwiresps->GetBinContent(nwires_bin);
    Pd_cls=clspd->GetBinContent(cls_bin);
    Pd_nwires=nwirespd->GetBinContent(nwires_bin);
    
    Ns=cls_s->Integral();
    Nd=cls_d->Integral();
    //printf("CP:thbin=%i phibin=%i level_bin=%i cls=%i nwires=%i module=%i \n",thbin,phibin,level_bin,cls,nwires,module);
    //printf("Ps_cls=%f Ps_nwires=%f Pd_cls=%f Pd_nwires=%f Ns=%f Nd=%f \n",Ps_cls,Ps_nwires,Pd_cls,Pd_nwires,Ns,Nd);



    Float_t div=(Ps_cls*Ps_nwires*Ns+Pd_cls*Pd_nwires*Nd);
    if(div>0){
      
      PS=Ps_cls*Ps_nwires*Ns/div;
      PD=Pd_cls*Pd_nwires*Nd/div;   
      
    } 
    file_with_params.Close();
    
    //for -1 case make default:
    if(PS==-1){
      if(level_bin==0){
	if(module==0){
	  if(cls<=50&&nwires<=7) return 1.1;
	  return -0.1;
	} else if (module==1){
	  if(cls<=40&&nwires<=7) return 1.1;
	  return -0.1;
	}
      }

      if(level_bin==1){
	if(module==0){
	  if(cls<=70&&nwires<=7) return 1.1;
	  return -0.1;
	} else if (module==1){
	  if(cls<=60&&nwires<=7) return 1.1;
	  return -0.1;
	}
      }

      if(level_bin==2){
	if(module==0){
	  if(cls<=110&&nwires<=7) return 1.1;
	  return -0.1;
	} else if (module==1){
	  if(cls<=60&&nwires<=7) return 1.1;
	  return -0.1;
	}
      }
    }

    return PS;
  } else {
    // printf("HPidCPParam::getProbSingle bins in argument out of range\n");
    return -1.0;
    
  }
}


ClassImp(HPidCPParam)
