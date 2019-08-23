//*-- AUTHOR : J. Kempter
//*-- Modified : 17/01/2002 by I. Koenig

////////////////////////////////////////////////////////////////////////////
// HMdcTdcThreshold
//
// Container class for the TDC threshold parameters of the MDC
//
////////////////////////////////////////////////////////////////////////////
#include <stdlib.h>

#include "hmdctdcthreshold.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcrawstruct.h"
#include "hmessagemgr.h"
#include <iostream.h>

ClassImp(HMdcTdcThresholdDbo)
ClassImp(HMdcTdcThresholdMbo)
ClassImp(HMdcTdcThresholdMod)
ClassImp(HMdcTdcThresholdSec)
ClassImp(HMdcTdcThreshold)

void HMdcTdcThresholdDbo::fill(HMdcTdcThresholdDbo& r) {
  threshold=r.getThreshold();
}

HMdcTdcThresholdMbo::HMdcTdcThresholdMbo(Int_t dbo, Text_t* name) {
  // constructor takes to number of DBOs  and the name of the MBO
  SetName(name);
  array = new TObjArray(dbo);
  for (Int_t i=0; i<dbo; ++i) array->AddAt(new HMdcTdcThresholdDbo(),i);
}

HMdcTdcThresholdMbo::~HMdcTdcThresholdMbo() {
  // destructor
  array->Delete();
  delete array;
}

HMdcTdcThresholdMod::HMdcTdcThresholdMod(Int_t sec, Int_t mod, Int_t mbo) {
  // constructor takes the sector, module and MBO number
  array = new TObjArray(mbo);
  HMdcRawStruct* pMdc=
       (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
  if (pMdc) {
    HMdcRawModStru& rMod=(*pMdc)[sec][mod];
    for (Int_t i=0; i<rMod.getSize(); i++) {
      HMdcRawMothStru& rMbo=rMod[i];
      array->AddAt(new HMdcTdcThresholdMbo((Int_t)(rMod[i].getNTdcs()/16),(char*)rMbo.GetName()),i);
    }
  }
  else cerr<<"HMdcTdcThreshold not created, container MdcRawStruct not found"<<endl;
}

HMdcTdcThresholdMod::~HMdcTdcThresholdMod() {
  // destructor
  array->Delete();
  delete array;
}

HMdcTdcThresholdSec::HMdcTdcThresholdSec(Int_t sec, Int_t mod) {
  // constructor takes the sector, module number
  array = new TObjArray(mod);
  for (Int_t i=0; i<mod; i++)
        array->AddAt(new HMdcTdcThresholdMod(sec,i),i);
}

HMdcTdcThresholdSec::~HMdcTdcThresholdSec() {
  // destructor
  array->Delete();
  delete array;
}

HMdcTdcThreshold::HMdcTdcThreshold(const char* name,const char* title,
                             const char* context,Int_t n)
              : HParSet(name,title,context) {
  // constructor
  strcpy(detName,"Mdc");
  oraVersion=-1;
  array = new TObjArray(n);
  for (Int_t i=0; i<n; i++) array->AddAt(new HMdcTdcThresholdSec(i),i);
}

HMdcTdcThreshold::~HMdcTdcThreshold() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcTdcThreshold::init(HParIo* inp,Int_t* set) {
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMdcTdcThreshold::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcTdcThreshold::readline(const char* buf, Int_t* set) {
  // decodes one line read from ascii file I/O and calls HMdcTdcThresholdDbo::fill(...) 
  Int_t   sec, mod, mbo, dbo;
  Float_t thresh;
  char mboName[10];
  sscanf(buf,"%i%i%i%s%i%f",
         &sec, &mod, &mbo, mboName, &dbo, &thresh);
  Int_t n=sec*4+mod;
  if (!set[n]) return;
  HMdcTdcThresholdMbo& rMbo=(*this)[sec][mod][mbo];
  rMbo.SetName(mboName);
  HMdcTdcThresholdDbo& db=rMbo[dbo];
  db.fill(thresh);
  set[n]=999;
}

void HMdcTdcThreshold::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Calibration parameters of the MDC\n"
    "# Format:\n"
    "# sector  module  mbo  mboName  threshold\n";
}

Bool_t HMdcTdcThreshold::writeline(char *buf, Int_t sec, Int_t mod, Int_t mbo, Int_t dbo) {
  // writes one line to the buffer used by ascii file I/O
  HMdcTdcThresholdMbo& rMbo=(*this)[sec][mod][mbo];
  HMdcTdcThresholdDbo& db=rMbo[dbo];
  sprintf(buf,"%1i %1i %2i %s %2i %3i\n",
          sec, mod, mbo, rMbo.GetName(), dbo, db.getThreshold());
  return kTRUE;
}

void HMdcTdcThreshold::clear() {
  // clears the container
  oraVersion=-1;
  for(Int_t s=0;s<getSize();s++) {
    HMdcTdcThresholdSec& sec=(*this)[s];
    for(Int_t m=0;m<sec.getSize();m++) {
      HMdcTdcThresholdMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcTdcThresholdMbo& mbo=mod[l];
        for(Int_t c=0;c<mbo.getSize();c++) mbo[c].clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}
void HMdcTdcThreshold::printParam() {
  // prints the container
    SEPERATOR_msg("-",60);
    INFO_msg(10,DET_MDC,"HMdcTdcThreshold");
    //printf("--------------------HMdcTdcThreshold--------------------------\n");
    for(Int_t s=0;s<getSize();s++) {
	HMdcTdcThresholdSec& sec=(*this)[s];
	for(Int_t m=0;m<sec.getSize();m++) {
	    HMdcTdcThresholdMod& mod=sec[m];
	    for(Int_t l=0;l<mod.getSize();l++) {
		HMdcTdcThresholdMbo& mbo=mod[l];
		for(Int_t c=0;c<mbo.getSize();c++){
		  gHades->getMsg()->info(10,DET_MDC,GetName(),
			"%1i %1i %2i %s % 1i %3i",
			   s, m, l, mbo.GetName(), c, mbo[c].getThreshold());
		}
	    }
	}
    }
    SEPERATOR_msg("-",60);
    //printf("-----------------------------------------------------------\n");

}
