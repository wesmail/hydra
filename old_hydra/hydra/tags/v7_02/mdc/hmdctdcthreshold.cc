//*-- AUTHOR : J. Kempter
//*-- Modified : 17/01/2002 by I. Koenig

////////////////////////////////////////////////////////////////////////////
// HMdcTdcThreshold
//
// Container class for the TDC threshold parameters of the MDC
//
////////////////////////////////////////////////////////////////////////////
using namespace std;
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
#include "hmdcmboreadout.h"
#include <iostream> 
#include <iomanip>

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
  mbo_readout=(HMdcMboReadout*)gHades->getRuntimeDb()->getContainer("MdcMboReadout");
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
  if (input) if  (!input->init(this,set)) return kFALSE;
  if ((  raw_struct=(HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct")) == NULL) return kFALSE;
  return kTRUE;
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
  Int_t thresh;
  char mboName[10];
  sscanf(buf,"%i%i%i%s%i%x",
         &sec, &mod, &mbo, mboName, &dbo, &thresh);
  Int_t n=sec*4+mod;
  if (!set[n]) return;
  HMdcTdcThresholdMbo& rMbo=(*this)[sec][mod][mbo];
  rMbo.SetName(mboName);
  HMdcTdcThresholdDbo& db=rMbo[dbo];
  db.fill(thresh);
  set[n]=999;
}

void HMdcTdcThreshold::readlinefromfile(const char* buf) {
  // decodes one line read from ascii file I/O and calls HMdcTdcThresholdDbo::fill(...) 
  Int_t   sec, mod, mbo, dbo;
  Int_t thresh;
  sscanf(buf,"%i%i%i%i%x",
         &mod, &sec, &mbo, &dbo, &thresh);  
  HMdcRawModStru & rawMod=(*raw_struct)[sec][mod];
  //  cout << rawMod.getSize() << endl;
  if(rawMod.getSize()>mbo)
    {
      HMdcRawMothStru &rawMbo=rawMod[mbo];
      // cout << rawMbo.getNTdcs() << endl;  
      if (rawMbo.getNTdcs() > dbo*16 )
	{ 
	  HMdcTdcThresholdMbo& rMbo=(*this)[sec][mod][mbo];
	  HMdcTdcThresholdDbo& db=rMbo[dbo];
	  db.fill(thresh);
	}
      //      else{cout<<"dbo number to big"<<endl;};
    }
  //  else{cout<<"mbo number"<<endl;};
}

void HMdcTdcThreshold::writelinetofile(char* buf, int sec, int mod, int mbo, int dbo) {
  // writes one line of threshold information into buffer, based on sec mod mbo dbo address. If no dbo found sets threshold to 60!
  Int_t thresh;

  HMdcRawModStru & rawMod=(*raw_struct)[sec][mod];
  //  cout << rawMod.getSize() << endl;
  if(rawMod.getSize()>mbo)
    {
      HMdcRawMothStru &rawMbo=rawMod[mbo];
      // cout << rawMbo.getNTdcs() << endl;  
      if (rawMbo.getNTdcs() > dbo*16 )
				{ 
					HMdcTdcThresholdMbo& rMbo=(*this)[sec][mod][mbo];
					HMdcTdcThresholdDbo& db=rMbo[dbo];
					thresh = db.getThreshold();
					sprintf(buf,"%i %i %i %i %x",
									mod, sec, mbo, dbo, thresh);  
					
				}
			else sprintf(buf,"%i %i %i %i 60",mod,sec,mbo,dbo);
      //      else{cout<<"dbo number to big"<<endl;};
    }
	else sprintf(buf,"%i %i %i %i 60",mod,sec,mbo,dbo);
  //  else{cout<<"mbo number"<<endl;};
}


void HMdcTdcThreshold::addvalue(Int_t factor) {
  //to incrise or decrise value of threshold by the constant factor
  //if you want to increast threshold then yhe value of the factor 
  //should be positive, for decreasing the factor must be negative.
 
  Int_t sec, mbo, mod, dbo;
 for(sec=0; sec<6; sec++)
    {
      //     cout<<"sec: "<<sec<<endl;
      HMdcTdcThresholdSec & rawSec= (*this)[sec];
      for(mod=0; mod<rawSec.getSize(); mod++)
      {
	//	cout<<"mod: "<<mod<<endl;
	HMdcTdcThresholdMod & rawMod=rawSec[mod];
	for(mbo =0 ; mbo < rawMod.getSize() ; mbo++)  
        {
	  //	  cout<<"mbo: "<<mbo<<endl;
	  HMdcTdcThresholdMbo &rawMbo=rawMod[mbo];
	  for(dbo =0; dbo < rawMbo.getSize(); dbo++ )
	    {
	      //	      cout<<"dbo: "<<dbo<<endl;
	      //  HMdcTdcThresholdDbo& rMbo=(*this)[sec][mod][mbo];
	    HMdcTdcThresholdDbo& db=rawMbo[dbo];
	    // cout<<"FACTOR IS: "<<factor<<endl;
	    //	    cout <<db.getThreshold()<<endl;
	    db.fill((db.getThreshold())+factor);
	    //	    cout <<db.getThreshold()<<endl;
	    }	     
	    }
	}
      }
     }

void HMdcTdcThreshold::putAsciiHeader(TString& header) {
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Calibration parameters of the MDC\n"
    "# Format:\n"
    "# sector  module  mbo  mboName  threshold\n";
}

Bool_t  HMdcTdcThreshold::write_thresholds(char *ofilename, Int_t  plane, Int_t sector){
 
  FILE *Ofile;
  Int_t module = plane -1;
  char *buf, *bufn, *fileout;
  Int_t  mbo, port, i, roc, lvl1;
  i = 0;
  buf = new char[256];
  bufn= new char[256]; 
  fileout= new char[256];
  //  mbo_readout->print();
  Bool_t isFirst; 

	if(status)
		{
			sprintf(fileout,"%s",ofilename);
			switch(plane){
			case 1 :
				strcat(fileout,"I");
				break;
			case 2 :
				strcat(fileout,"II");
				break;
			case 3 :
				strcat(fileout,"III");
				break;
			case 4 :
				strcat(fileout,"IV");
				break;
			}
			sprintf(fileout,"%s%d%s",fileout,sector+1,"/threshold.conf");
			Ofile = fopen(fileout,"w");
			if(Ofile == NULL)
				return kFALSE;
			sprintf(buf,"%s\t%d\n","plane",plane);
			fputs(buf,Ofile);
			sprintf(buf,"%s  %d\n","thresholdversion",oraVersion);
			fputs(buf,Ofile);
			// for (Int_t module = 0; module <4; module++ ){
			HMdcMboReadoutMod & rMod = (*mbo_readout)[sector][module];
			//  HMdcTdcThreshold& r= *this;
			HMdcTdcThresholdMod& rModThr=(*this)[sector][module];
			//  HMdcTdcThresholdMod & rModTh = ((HMdcTdcThreshold *)((*array)[0]))[sector][module];
			sprintf(buf,"-----------------------------------\n");
			fputs(buf,Ofile);
			sprintf(buf,"%s%s%d%s\n","------------"," Plane  ",plane,"--------------");
			fputs(buf,Ofile);
			sprintf(buf,"-----------------------------------\n");
			fputs(buf,Ofile);
			sprintf(buf,"\n");
			for(Int_t i=0;i<6;i++)
				fputs(buf,Ofile);
			for(roc = 1; roc <= 5; roc++){
				isFirst=kTRUE;
				for(port = 0; port <2; port++){
					for(lvl1=1;lvl1<4;lvl1++)
						{
							for(mbo =0 ; mbo < rMod.getSize() ; mbo++){
								HMdcMboReadoutMoth & rMoth = rMod[mbo];
								if (mbo < rModThr.getSize()){
									if ((rMoth.getRoc()==roc) &&
											(rMoth.getBusPos()==port) &&
											(rMoth.getBus()==lvl1)){
										if (isFirst) {
											sprintf(buf,"ROC  %d\n",roc);
											fputs(buf,Ofile);
											isFirst=kFALSE;
										}
										sprintf(bufn,"mbo  %s  %d  %x  %x  %x  %x ",rMoth.GetName(),port,
														rModThr[mbo][0].getThreshold(),rModThr[mbo][1].getThreshold(),
														rModThr[mbo][2].getThreshold(),rModThr[mbo][3].getThreshold());
										Int_t ndbo;
										ndbo = rModThr[mbo].getSize();
										if(ndbo > 4) {
											if(rModThr[mbo][4].getThreshold() != -1)
												sprintf(buf,"%s%x  %x\n",bufn,rModThr[mbo][4].getThreshold(),rModThr[mbo][5].getThreshold());
											else sprintf(buf,"%s%d  %d\n",bufn,-1,-1);
										} else sprintf(buf,"%s%d  %d\n",bufn,-1,-1);
										fputs(buf,Ofile); 
									} 
								} else
									cout << " Error " <<endl; 
							}
						}
				}
			}
			sprintf(buf,"###\n");
			fputs(buf,Ofile);
			delete buf;
			delete bufn;
			fclose(Ofile);
			delete fileout;
			return kTRUE;
		}
	else
		{
			ERROR_msg(HMessageMgr::DET_MDC,"This version is no longer valid for DAQ!");
			return kFALSE;
		}
}


Bool_t HMdcTdcThreshold::writeline(char *buf, Int_t sec, Int_t mod, Int_t mbo, Int_t dbo) {
  // writes one line to the buffer used by ascii file I/O
  HMdcTdcThresholdMbo& rMbo=(*this)[sec][mod][mbo];
  HMdcTdcThresholdDbo& db=rMbo[dbo];
  sprintf(buf,"%1i %1i %2i %s %2i %3x\n",
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
    INFO_msg(10,HMessageMgr::DET_MDC,"HMdcTdcThreshold");
    //printf("--------------------HMdcTdcThreshold--------------------------\n");
    for(Int_t s=0;s<getSize();s++) {
	HMdcTdcThresholdSec& sec=(*this)[s];
	for(Int_t m=0;m<sec.getSize();m++) {
	    HMdcTdcThresholdMod& mod=sec[m];
	    for(Int_t l=0;l<mod.getSize();l++) {
		HMdcTdcThresholdMbo& mbo=mod[l];
		for(Int_t c=0;c<mbo.getSize();c++){
		  gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName(),
			"%1i %1i %2i %s % 1i %3x",
			   s, m, l, mbo.GetName(), c, mbo[c].getThreshold());
		}
	    }
	}
    }
    SEPERATOR_msg("-",60);
    //printf("-----------------------------------------------------------\n");

}
