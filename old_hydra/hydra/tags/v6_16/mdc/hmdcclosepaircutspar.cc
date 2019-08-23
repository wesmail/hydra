//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 18/06/2002 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HMdcClosePairCutsPar                                                                                                //
// Container storing the cuts to make close pair rejection                                                             //
// If the all values of the cuts are larger than those from mdc-rich tracklet                                          // 
// than traclet is single. Cuts on mdc cluster size, mdc contributing wires                                            // 
//  and integrated charge in ring are stored.                                                                          //
// Because first two mdc variables are depending on theta,phi angle ,                                                  //
// there are different cuts for several theta,phi regions. There are 8 bins in theta space:                            //
// mdc cls finder level bins:                                                                                          //
// 0:  4                                                                                                               //
// 1:  5                                                                                                               //
// 2:  6                                                                                                               //
// theta bins:                                                                                                         //
// 0:  10-20 degrees                                                                                                   // 
// 1:  20-30 degrees                                                                                                   // 
// 2:  30-40 degrees                                                                                                   //
// 3:  40-50 degrees                                                                                                   //
// 4:  50-60 degrees                                                                                                   //
// 5:  60-70 degrees                                                                                                   //
// 6:  70-80 degrees                                                                                                   // 
// 7:  80-90 degrees                                                                                                   //
// There are three bins in phi space                                                                                   // 
// phi bins:                                                                                                           // 
// 0:   0-10 & 50-60                                                                                                   // 
// 1:  10-20 & 40-50                                                                                                   //
// 2:  20-30 & 30-40                                                                                                   //
// The rejection power S/B ratio depends on minimal single lepton efficiency therefore there are                       //
// 4 bins of efficiency                                                                                                //
// efficiency bins:                                                                                                    //
// 0:  80-85                                                                                                           //
// 1:  85-90                                                                                                           //
// 2:  90-95                                                                                                           // 
// 3:  95-100                                                                                                          //
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using namespace std;
#include <stdlib.h>
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream> 
#include <iomanip>
#include "hmdcclosepaircutspar.h"
#include "hmessagemgr.h"

ClassImp(HMdcClosePairCutsParCut)
ClassImp(HMdcClosePairCutsParEf)
ClassImp(HMdcClosePairCutsParPhi)
ClassImp(HMdcClosePairCutsParTheta)
ClassImp(HMdcClosePairCutsPar)

HMdcClosePairCutsParEf::HMdcClosePairCutsParEf(Int_t n) {
  // constructor creates a TClonesArray of type
  // HMdcClosePairCutsParCut
  array = new TObjArray(n);
  for (Int_t i = 0; i< n; ++i) array->AddAt(new HMdcClosePairCutsParCut(),i);
}

HMdcClosePairCutsParEf::~HMdcClosePairCutsParEf() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcClosePairCutsParEf::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcClosePairCutsParPhi::HMdcClosePairCutsParPhi(Int_t m, Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcClosePairCutsParEf
  array = new TObjArray(m);
  for (Int_t i = 0; i < m; i++) {
    array->AddAt(new HMdcClosePairCutsParEf(n),i);
  }
}
 
HMdcClosePairCutsParPhi::~HMdcClosePairCutsParPhi() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcClosePairCutsParPhi::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcClosePairCutsParTheta::HMdcClosePairCutsParTheta(Int_t l, Int_t m, Int_t n) {
  // constructor creates an array of pointers of type
  // HMdcClosePairCutsParPhi
  array = new TObjArray(l);
  for (Int_t i = 0; i < l; i++) {
    array->AddAt(new HMdcClosePairCutsParPhi(m,n),i);
  }
  
}
 
HMdcClosePairCutsParTheta::~HMdcClosePairCutsParTheta() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcClosePairCutsParTheta::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcClosePairCutsPar::HMdcClosePairCutsPar(const char* name,const char* title,
                       const char* context,Int_t h,Int_t l,Int_t m,Int_t n) : HParSet(name,title,context) {
  //creating array of the  HMdcClosePairCutsParTheta 
  //container has a tree structure
 
  //fName="MdcClosePairCutsPar";
  //fTitle="close pair rejection  parameters for Mdc-Rich";
  comment="no comment";
  strcpy(detName,"Mdc");
  if (gHades) {
      fMdc =(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
  } else {
    fMdc = 0;
  }
   
  array=new  TObjArray(l);
  for (Int_t i=0; i<h; ++i) array->AddAt(new HMdcClosePairCutsParTheta(l,m,n),i);
  theta_bin_size=10;
  phi_bin_size=10;
  theta_nbin=l;
  phi_nbin=m;
 
}

HMdcClosePairCutsPar::~HMdcClosePairCutsPar() {
  // destructor
  array->Delete();
  delete array;
}

Int_t HMdcClosePairCutsPar::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcClosePairCutsPar::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  Int_t myset[1]={1};
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,myset));
  return kFALSE;
}

Int_t HMdcClosePairCutsPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcClosePairCutsPar::clear() {
  for(Int_t n=0;n<getSize();n++) {
    HMdcClosePairCutsParTheta& the=(*this)[n];
    for(Int_t m=0;m<the.getSize();m++) {
      HMdcClosePairCutsParPhi& phi=the[m];
      for(Int_t a=0;a<phi.getSize();a++) {
	HMdcClosePairCutsParEf& ef=phi[a];
	for(Int_t b=0;b<ef.getSize();b++) {
	  HMdcClosePairCutsParCut& cut=ef[b];
	  cut.clear();
	}
      }
    }
  }

  status=kFALSE;
  resetInputVersions();
}




void HMdcClosePairCutsPar::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Lepton Single/Double recognition parameters \n"
    "# Format: \n"
    "# levelbin thetabin phibin effbin eff signal/background mdcclustersize_cut mdcnwires_cut  richintcharge_cut\n";
}

Bool_t HMdcClosePairCutsPar::writeline(char *buf, Int_t l, Int_t t, Int_t p, Int_t ef) {
  // writes one line to the buffer used by ascii file I/O
  
  HMdcClosePairCutsParCut &cut=(*this)[l][t][p][ef];
  sprintf(buf,"%2i %2i %2i %2i %4.2f %6.2f %4.0f %4.0f %5.0f\n",l,t, p,ef,cut.getEfficiency(),cut.getSig2Back(),
          cut.getMdcClsSizeCut(), cut.getMdcContWiresCut(),cut.getRichIntChargeCut());
  return kTRUE;
  
}

void HMdcClosePairCutsPar::readline (const char* buf,Int_t *) {
  // writes one line to the buffer used by ascii file I/O
  Int_t  l,t,p,ef;
  Float_t efficiency;      // single recognition efficiency
  Float_t sig2back;        // signal to background ratio
  Float_t mdc_size_cut ;    // mdc cluster size cut
  Float_t mdc_wires_cut;   // mdc cluster contributing wires  cut
  Float_t rich_charge_cut ;  // rich integrated charge in ring 
  
  sscanf(buf,"%i%i%i%i%f%f%f%f%f\n",&l,&t, &p,&ef,&efficiency, &sig2back,&mdc_size_cut,&mdc_wires_cut,&rich_charge_cut);
  if(l<=getSize()){
    HMdcClosePairCutsParTheta &the=(*this)[l];
    if(t<=the.getSize()){
      HMdcClosePairCutsParPhi &phi=the[t];
      if (p<=phi.getSize()){
	HMdcClosePairCutsParEf &efb=phi[p];
	if(ef<=efb.getSize()){
	  HMdcClosePairCutsParCut &cut=efb[ef];
	  
	  cut.fill(efficiency,sig2back,mdc_size_cut,mdc_wires_cut,rich_charge_cut);
	}else {
	  Error("HMdcClosePairCutsPar::readline()","Eff array out of bounds");
	}      
      }else {
	Error("HMdcClosePairCutsPar::readline()","Phi array out of bounds");
      }
    }else {
      Error("HMdcClosePairCutsPar::readline()","Theta array out of bounds");
    }
  }else {
    Error("HMdcClosePairCutsPar::readline()","Level array out of bounds");
  }
  
}

void HMdcClosePairCutsPar::printParam(void) {
  SEPERATOR_msg("#",60);
  INFO_msg(10,DET_MDC,"HMdcClosePairCutsPar:");
  INFO_msg(10,DET_MDC,"Format:");
  INFO_msg(10,DET_MDC,"levelbin thetabin phibin effbin  efficiency s/b mdcclustersizecut mdccontwirescut richintchargecut:");
  //printf ("HMdcClosePairCutsPar:\n");
  //printf ("Format:\n");
  //printf ("thetabin phibin effbin  efficiency s/b mdcclustersizecut mdccontwirescut richintchargecut:\n");
  for(Int_t lev=0;lev<getSize();lev++) { 
    HMdcClosePairCutsParTheta& the=(*this)[lev];
    for(Int_t th=0;th<the.getSize();th++) {
      HMdcClosePairCutsParPhi& phi=the[th];
      for(Int_t p=0;p<phi.getSize();p++) {
	HMdcClosePairCutsParEf& ef=phi[p];
	for(Int_t e=0;e<ef.getSize();e++) {
	  HMdcClosePairCutsParCut& cut=ef[e];
	  gHades->getMsg()->info(10,DET_MDC,GetName(),
				 "%2i %2i %2i %2i %4.2f %6.2f %4.0f %4.0f %5.0f\n",lev,th,p,e,cut.getEfficiency(),cut.getSig2Back(),cut.getMdcClsSizeCut(),cut.getMdcContWiresCut(),cut.getRichIntChargeCut());    
	  //printf("%2i %2i %2i %4.2f %6.2f %4.0f %4.0f %4.0f %5.0f\n",th,p,e,cut.getEfficiency(),cut.getSig2Back(),cut.getMdcClsSizeCut(),cut.getMdcContWiresCut(),cut.getMdcMergedClsCut(),cut.getRichIntChargeCut());
	}
      }
    }
  }
  
}



Bool_t HMdcClosePairCutsPar::passCuts(Float_t mdc_level,Float_t mdc_the,Float_t mdc_phi,Float_t min_eff,Float_t mdc_clustersize,Float_t mdc_nwires,Float_t rich_intcharge){
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  // The usefull function to check whether values in cluster pass the CPR cuts:                     //
  // mdc_level ... 4-6                                                                              //
  // mdc_the,mdc_phi angles in degrees                                                              //
  // min_eff 80,85,90 or 95                                                                         //
  ////////////////////////////////////////////////////////////////////////////////////////////////////
  Float_t mdc_cluster_cut,mdc_wires_cut,rich_charge_cut; 
  HMdcClosePairCutsParCut  cut; 
  Int_t mdc_theta_bin=calculateMdcThetaBin(mdc_the);
  Int_t mdc_phi_bin=calculateMdcPhiBin(mdc_phi);
  Int_t mdc_level_bin=calculateMdcLevelBin(mdc_level); 
  Int_t min_eff_bin=calculateMdcEffBin(min_eff);
 

 
  if(mdc_theta_bin>-1&&mdc_phi_bin>-1&&min_eff_bin>-1&&mdc_level_bin>-1){
    
    cut=(*this)[mdc_level_bin][mdc_theta_bin][mdc_phi_bin][min_eff_bin];
    mdc_cluster_cut=cut.getMdcClsSizeCut();
    mdc_wires_cut=cut.getMdcContWiresCut();
    rich_charge_cut=cut.getRichIntChargeCut();
    if(mdc_clustersize<mdc_cluster_cut&&mdc_nwires<mdc_wires_cut&&rich_intcharge<rich_charge_cut) return kTRUE;
  }  
  
  return kFALSE;
}


Int_t HMdcClosePairCutsPar::calculateMdcThetaBin(Float_t mdc_the){
  
  Int_t theta_bin=-1;

  mdc_the=TMath::Abs(mdc_the);
  theta_bin=(Int_t) mdc_the/theta_bin_size-1;
  if(theta_bin<theta_nbin){
    return   theta_bin;
  } else {
    return  -1;
  }
}


Int_t HMdcClosePairCutsPar::calculateMdcPhiBin(Float_t mdc_phi){
  Int_t phi_bin=-1;
  mdc_phi=TMath::Abs(mdc_phi);
  if(((Int_t) mdc_phi%60)<30.0) {
    phi_bin=(Int_t) mdc_phi%60/phi_bin_size;
  }else{
    phi_bin=(Int_t) (60.0-(Int_t)mdc_phi%60)/phi_bin_size; 
  }
  if(phi_bin<phi_nbin){
    return   phi_bin;
  } else {
    return  -1;
  } 
} 

Int_t HMdcClosePairCutsPar::calculateMdcLevelBin(Float_t mdc_level){
  if(mdc_level==4) return 0;
  if(mdc_level==5) return 1;
  if(mdc_level==6) return 2;
  return -1;
}


Int_t HMdcClosePairCutsPar::calculateMdcEffBin(Float_t min_eff){
  if( min_eff>=80&&min_eff<85)  return 0;
  if( min_eff>=85&&min_eff<90)  return 1;
  if( min_eff>=90&&min_eff<95)  return 2;
  if( min_eff>=95)  return 3;
  return -1;
}
