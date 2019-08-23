//*-- AUTHOR : Jaroslav Bielcik
//*-- Modified : 18/06/2002 

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// HMdcClosePairCutsPar                                                                                                //
// Container storing the cuts to make close pair rejection                                                             //
// If the all values of the cuts are larger than those from mdc-rich tracklet                                          // 
// than traclet is single. Cuts on mdc cluster size, mdc contributing wires                                            // 
// mdc merged clusters and integrated charge in ring are stored.                                                       //
// Because first two mdc variables are depending on theta,phi angle ,                                                  //
// there are different cuts for several theta,phi regions. There are 8 bins in theta space:                            //
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
#include <stdlib.h>
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream.h>
#include "hmdcclosepaircutspar.h"


ClassImp(HMdcClosePairCutsParCut)
ClassImp(HMdcClosePairCutsParEf)
ClassImp(HMdcClosePairCutsParPhi)
ClassImp(HMdcClosePairCutsPar)

HMdcClosePairCutsParEf::HMdcClosePairCutsParEf(Int_t n=4) {
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

HMdcClosePairCutsParPhi::HMdcClosePairCutsParPhi(Int_t m=3, Int_t n = 4) {
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

HMdcClosePairCutsPar::HMdcClosePairCutsPar(const char* name,const char* title,
                       const char* context,Int_t l,Int_t m,Int_t n) : HParSet(name,title,context) {
  //creating array of the  HMdcClosePairCutsParPhi 
  //container has a tree structure
 
  //fName="MdcClosePairCutsPar";
  //fTitle="close pair rejection  parameters for Mdc-Rich";
  comment="no comment";
  strcpy(detName,"Mdc");
  if (gHades) {
    fMdc = (HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  } else {
    fMdc = 0;
  }
   
  array=new  TObjArray(l);
  for (Int_t i=0; i<l; ++i) array->AddAt(new HMdcClosePairCutsParPhi(m,n),i);
  
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
  for(Int_t m=0;m<getSize();m++) {
    HMdcClosePairCutsParPhi& phi=(*this)[m];
    for(Int_t a=0;a<phi.getSize();a++) {
      HMdcClosePairCutsParEf& ef=phi[a];
      for(Int_t b=0;b<ef.getSize();b++) {
	HMdcClosePairCutsParCut& cut=ef[b];
        cut.clear();
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
    "# thetabin phibin effbin eff signal/background mdcclustersize_cut mdcnwires_cut mdcmergedcls_cut richintcharge_cut\n";
}

Bool_t HMdcClosePairCutsPar::writeline(char *buf, Int_t t, Int_t p, Int_t ef) {
  // writes one line to the buffer used by ascii file I/O
  
  HMdcClosePairCutsParCut &cut=(*this)[t][p][ef];
  sprintf(buf,"%2i %2i %2i %4.2f %6.2f %4.0f %4.0f %4.0f %5.0f\n",t, p,ef,cut.getEfficiency(),cut.getSig2Back(),
          cut.getMdcClsSizeCut(), cut.getMdcContWiresCut(),cut.getMdcMergedClsCut(),cut.getRichIntChargeCut());
  return kTRUE;
  
}

void HMdcClosePairCutsPar::readline (const char* buf,Int_t *) {
  // writes one line to the buffer used by ascii file I/O
  Int_t  t,p,ef;
  Float_t efficiency;      // single recognition efficiency
  Float_t sig2back;        // signal to background ratio
  Float_t mdc_size_cut ;    // mdc cluster size cut
  Float_t mdc_wires_cut;   // mdc cluster contributing wires  cut
  Float_t mdc_nmerged_cut;   // mdc cluster merged clusters  cut
  Float_t rich_charge_cut ;  // rich integrated charge in ring 
  
  sscanf(buf,"%i%i%i%f%f%f%f%f%f\n",&t, &p,&ef,&efficiency, &sig2back,&mdc_size_cut,&mdc_wires_cut,&mdc_nmerged_cut,&rich_charge_cut);
  
  if(t<=getSize()){
    HMdcClosePairCutsParPhi &phi=(*this)[t];
    if (p<=phi.getSize()){
      HMdcClosePairCutsParEf &efb=phi[p];
      if(ef<=efb.getSize()){
	HMdcClosePairCutsParCut &cut=efb[ef];
        
        cut.fill(efficiency,sig2back,mdc_size_cut,mdc_wires_cut,mdc_nmerged_cut,rich_charge_cut);
      }else {
	Error("HMdcClosePairCutsPar::readline()","Eff array out of bounds");
      }      
    }else {
      Error("HMdcClosePairCutsPar::readline()","Phi array out of bounds");
    }
  }else {
    Error("HMdcClosePairCutsPar::readline()","Theta array out of bounds");
  }
  
  
  }

void HMdcClosePairCutsPar::printParam(void) {
  printf ("HMdcClosePairCutsPar:\n");
  printf ("Format:\n");
  printf ("thetabin phibin effbin  efficiency s/b mdcclustersizecut mdccontwirescut mdcmergedclscut richintchargecut:\n");
  for(Int_t th=0;th<getSize();th++) {
    HMdcClosePairCutsParPhi& phi=(*this)[th];
    for(Int_t p=0;p<phi.getSize();p++) {
      HMdcClosePairCutsParEf& ef=phi[p];
      for(Int_t e=0;e<ef.getSize();e++) {
	HMdcClosePairCutsParCut& cut=ef[e];
        printf("%2i %2i %2i %4.2f %6.2f %4.0f %4.0f %4.0f %5.0f\n",th,p,e,cut.getEfficiency(),cut.getSig2Back(),cut.getMdcClsSizeCut(),cut.getMdcContWiresCut(),cut.getMdcMergedClsCut(),cut.getRichIntChargeCut());
      }
    }
  }
}
