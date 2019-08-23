//*-- AUTHOR : J.Wuestenfeld
//*-- Modified : 13/08/2003 by J. Wuestenfeld

////////////////////////////////////////////////////////////////////////////
// HMdcTdcChannel
//
// Container class for the TDC channle masks of the MDC
//
////////////////////////////////////////////////////////////////////////////
using namespace std;
#include <stdlib.h>

#include "hmdctdcchannel.h"
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

ClassImp(HMdcTdcChannelTdc)
ClassImp(HMdcTdcChannelMbo)
ClassImp(HMdcTdcChannelMod)
ClassImp(HMdcTdcChannelSec)
ClassImp(HMdcTdcChannel)

void HMdcTdcChannelTdc::fill(HMdcTdcChannelTdc& r)
{
  channelMask[0]=r.getChannelMask(0);
	channelMask[1]=r.getChannelMask(1);
}

HMdcTdcChannelMbo::HMdcTdcChannelMbo(Int_t tdc, Text_t* name)
{
  // constructor takes the number of TDCs  and the name of the MBO
  SetName(name);
  array = new TObjArray(tdc);
  for (Int_t i=0; i<tdc; ++i) array->AddAt(new HMdcTdcChannelTdc(),i);
}

HMdcTdcChannelMbo::~HMdcTdcChannelMbo()
{
  // destructor
  array->Delete();
  delete array;
}

HMdcTdcChannelMod::HMdcTdcChannelMod(Int_t sec, Int_t mod, Int_t mbo)
{
  // constructor takes the sector, module and MBO number
  array = new TObjArray(mbo);
  HMdcRawStruct* pMdc = (HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct");
  if (pMdc)
		{
			HMdcRawModStru& rMod=(*pMdc)[sec][mod];
			for (Int_t i=0; i<rMod.getSize(); i++) {
				HMdcRawMothStru& rMbo=rMod[i];
				array->AddAt(new HMdcTdcChannelMbo((Int_t)(rMod[i].getNTdcs()/8),(char*)rMbo.GetName()),i);
			}
		}
  else
		{
			ERROR_msg(HMessageMgr::DET_MDC,"HMdcTdcChannelMod not created, container MdcRawStruct not found");
			exit(1);
		}
}

HMdcTdcChannelMod::~HMdcTdcChannelMod()
{
  // destructor
  array->Delete();
  delete array;
}

HMdcTdcChannelSec::HMdcTdcChannelSec(Int_t sec, Int_t mod)
{
  // constructor takes the sector, module number
  array = new TObjArray(mod);
  for (Int_t i=0; i<mod; i++)
        array->AddAt(new HMdcTdcChannelMod(sec,i),i);
}

HMdcTdcChannelSec::~HMdcTdcChannelSec()
{
  // destructor
  array->Delete();
  delete array;
}

HMdcTdcChannel::HMdcTdcChannel(const char* name,const char* title,
                             const char* context,Int_t n)
              : HParSet(name,title,context)
{
  // constructor
  strcpy(detName,"Mdc");
  oraVersion=-1;
	calRotateByRocCode = kFALSE;
  array = new TObjArray(n);
	mapNotConnectedChannels = kFALSE;
	status = kTRUE;
	for (Int_t i=0; i<n; i++) array->AddAt(new HMdcTdcChannelSec(i),i);
}

HMdcTdcChannel::~HMdcTdcChannel()
{
  // destructor
  array->Delete();
  delete array;
}

Bool_t HMdcTdcChannel::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  if((raw_struct=(HMdcRawStruct*)gHades->getRuntimeDb()->getContainer("MdcRawStruct")) == NULL) return kFALSE;
// 	if((conWires=(HMdcConnectedWires*)gHades->getRuntimeDb()->getContainer("MdcConnectedWires")) == NULL) return kFALSE;
	if((mbo_readout=(HMdcMboReadout*)gHades->getRuntimeDb()->getContainer("MdcMboReadout")) == NULL) return kFALSE;;

  HDetParIo* input=inp->getDetParIo("HMdcParIo");
  if(input) if  (!input->init(this,set)) return kFALSE;
  return kTRUE;
}

Int_t HMdcTdcChannel::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcTdcChannel::readline(const char* buf, Int_t* set)
{
  // decodes one line read from ascii file I/O and calls HMdcTdcChannelTdc::fill(...) 
  Int_t sec, mod, mbo, dbo;
  Int_t cal,meas;
	Char_t mboName[20];

  sscanf(buf,"%i%i%i%s%i%x%x",&sec, &mod, &mbo, mboName, &dbo, &cal,&meas);
  Int_t n=sec*4+mod;
  if (!set[n]) return;
  HMdcTdcChannelMbo& rMbo=(*this)[sec][mod][mbo];
	rMbo.SetName(mboName);
  HMdcTdcChannelTdc& tdc=rMbo[dbo];
	tdc.fill(cal,0);
  tdc.fill(meas,1);
  set[n]=999;
}

void HMdcTdcChannel::readlinefromfile(const char* buf)
{
  // decodes one line read from ascii file I/O and calls HMdcTdcChannelTdc::fill(...) 
  Int_t sec, mod, mbo, dbo;
  Int_t cal,meas;
  sscanf(buf,"%i%i%i%i%x%x", &mod, &sec, &mbo, &dbo, &cal,&meas);
  HMdcRawModStru & rawMod=(*raw_struct)[sec][mod];
  HMdcRawMothStru &rawMbo=rawMod[mbo];
  if (rawMbo.getNTdcs() >= dbo*16 ) { 
      HMdcTdcChannelMbo& rMbo=(*this)[sec][mod][mbo];
      HMdcTdcChannelTdc& db=rMbo[dbo-1];
			db.fill(cal,0);
      db.fill(meas,1);
    }
}

void HMdcTdcChannel::putAsciiHeader(TString& header)
{
  // puts the ascii header to the string used in HMdcParAsciiFileIo
  header=
    "# Channelmasks of the MDC TDC\n"
    "# Format:\n"
    "# sector  module  mbo  mboName  TDC#  cal. mask  meas. mask\n";
}

Bool_t  HMdcTdcChannel::write_thresholds(char *ofilename, Int_t  plane, Int_t sector)
{ 
  FILE *Ofile;
  Int_t module = plane -1;
  char *buf, *bufn, *fileout;
  Int_t  mbo, port, i, roc, lvl1;
  i = 0;
  buf = new char[256];
  bufn= new char[256]; 
  fileout= new char[256];
  Bool_t isFirst; 

	if(!status)
		{
			ERROR_msg(HMessageMgr::DET_MDC,"This version is no longer valid for DAQ!");
			return kFALSE;
		}
	if(mapNotConnectedChannels)
		mapOutChannels();
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
  sprintf(fileout,"%s%d%s",fileout,sector+1,"/channels.conf");
  Ofile = fopen(fileout,"w");
  if(Ofile == NULL)
    return 2;
  sprintf(buf,"%s\t%d\n","plane",plane);
  fputs(buf,Ofile);
  sprintf(buf,"%s  %d\n","channelsversion",oraVersion);
  fputs(buf,Ofile);
  sprintf(buf,"%s  %d\n","RocDoesCal",getCalRotateByRocCode());
  fputs(buf,Ofile);
  HMdcMboReadoutMod & rMod = (*mbo_readout)[sector][module];
  HMdcTdcChannelMod& rModThr=(*this)[sector][module];
  sprintf(buf,"-----------------------------------\n");
  fputs(buf,Ofile);
  sprintf(buf,"%s%s%d%s\n","------------"," Plane  ",plane,"--------------");
  fputs(buf,Ofile);
  sprintf(buf,"-----------------------------------\n");
  fputs(buf,Ofile);
	sprintf(buf,"\n");
  for(roc = 1; roc <= 5; roc++){
    isFirst=kTRUE;
    for(port = 0; port <2; port++){
			for(lvl1=1;lvl1<4;lvl1++)
				{
					for(mbo =0 ; mbo < rMod.getSize() ; mbo++){
						HMdcMboReadoutMoth& rMoth = rMod[mbo];
						if (mbo < rModThr.getSize()){
							if ((rMoth.getRoc()==roc) &&
									(rMoth.getBusPos()==port) &&
									(rMoth.getBus()==lvl1))
								{
									if (isFirst) {
										sprintf(buf,"ROC  %d\n",roc);
										fputs(buf,Ofile);
										isFirst=kFALSE;
									}
									sprintf(bufn,"mbo  %s  %d  %x  %x  %x  %x  %x  %x  %x  %x",rModThr[mbo].GetName(),port,
													rModThr[mbo][0].getChannelMask(1),rModThr[mbo][1].getChannelMask(1),
													rModThr[mbo][2].getChannelMask(1),rModThr[mbo][3].getChannelMask(1),
													rModThr[mbo][4].getChannelMask(1),rModThr[mbo][5].getChannelMask(1),
													rModThr[mbo][6].getChannelMask(1),rModThr[mbo][7].getChannelMask(1));
									Int_t ndbo;
									ndbo = rModThr[mbo].getSize();
									if(ndbo > 8)
										{
											if(rModThr[mbo][8].getChannelMask(1) != -1)
												{
													sprintf(buf,"%s  %x  %x  %x  %x\n",bufn,
																	rModThr[mbo][8].getChannelMask(1),rModThr[mbo][9].getChannelMask(1),
																	rModThr[mbo][10].getChannelMask(1),rModThr[mbo][11].getChannelMask(1));
												}
											else
												sprintf(buf,"%s  %d  %d  %d  %d\n",bufn,-1,-1,-1,-1);
										}
									else
										sprintf(buf,"%s  %d  %d  %d  %d\n",bufn,-1,-1,-1,-1);
									fputs(buf,Ofile); 
									if(!calRotateByRocCode)
										{
											sprintf(bufn,"mbo  %s  %d  %x  %x  %x  %x  %x  %x  %x  %x",rModThr[mbo].GetName(),port,
															rModThr[mbo][0].getChannelMask(0)&rModThr[mbo][0].getChannelMask(1),
															rModThr[mbo][1].getChannelMask(0)&rModThr[mbo][1].getChannelMask(1),
															rModThr[mbo][2].getChannelMask(0)&rModThr[mbo][2].getChannelMask(1),
															rModThr[mbo][3].getChannelMask(0)&rModThr[mbo][3].getChannelMask(1),
															rModThr[mbo][4].getChannelMask(0)&rModThr[mbo][4].getChannelMask(1),
															rModThr[mbo][5].getChannelMask(0)&rModThr[mbo][5].getChannelMask(1),
															rModThr[mbo][6].getChannelMask(0)&rModThr[mbo][6].getChannelMask(1),
															rModThr[mbo][7].getChannelMask(0)&rModThr[mbo][7].getChannelMask(1));
											Int_t ndbo;
											ndbo = rModThr[mbo].getSize();
											if(ndbo > 8)
												{
													if(rModThr[mbo][8].getChannelMask(0) != -1)
														{
															sprintf(buf,"%s  %x  %x  %x  %x\n",bufn,
																			rModThr[mbo][8].getChannelMask(0)&rModThr[mbo][8].getChannelMask(1),
																			rModThr[mbo][9].getChannelMask(0)&rModThr[mbo][9].getChannelMask(1),
																			rModThr[mbo][10].getChannelMask(0)&rModThr[mbo][10].getChannelMask(1),
																			rModThr[mbo][11].getChannelMask(0)&rModThr[mbo][11].getChannelMask(1));
														}
													else
														sprintf(buf,"%s  %d  %d  %d  %d\n",bufn,-1,-1,-1,-1);
												}
											else
												sprintf(buf,"%s  %d  %d  %d  %d\n",bufn,-1,-1,-1,-1);
											fputs(buf,Ofile);
										} 
								} 
						}
						else
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
  return 0;
}


Bool_t HMdcTdcChannel::writeline(char *buf, Int_t sec, Int_t mod, Int_t mbo, Int_t dbo) {
  // writes one line to the buffer used by ascii file I/O
  HMdcTdcChannelMbo& rMbo=(*this)[sec][mod][mbo];
  HMdcTdcChannelTdc& db=rMbo[dbo];
  sprintf(buf,"%1i %1i %2i %s %2i %2x %2x\n",
          sec, mod, mbo, rMbo.GetName(), dbo, db.getChannelMask(0),db.getChannelMask(1));
  return kTRUE;
}

void HMdcTdcChannel::clear() {
  // clears the container
  oraVersion=-1;
  for(Int_t s=0;s<getSize();s++) {
    HMdcTdcChannelSec& sec=(*this)[s];
    for(Int_t m=0;m<sec.getSize();m++) {
      HMdcTdcChannelMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
        HMdcTdcChannelMbo& mbo=mod[l];
        for(Int_t c=0;c<mbo.getSize();c++) mbo[c].clear();
      }
    }
  }
  status=kFALSE;
  resetInputVersions();
}
void HMdcTdcChannel::printParam() {
  // prints the container
	SEPERATOR_msg("-",60);
	INFO_msg(10,HMessageMgr::DET_MDC,"HMdcTdcChannel");
	for(Int_t s=0;s<getSize();s++) {
		HMdcTdcChannelSec& sec=(*this)[s];
		for(Int_t m=0;m<sec.getSize();m++) {
	    HMdcTdcChannelMod& mod=sec[m];
	    for(Int_t l=0;l<mod.getSize();l++) {
				HMdcTdcChannelMbo& mbo=mod[l];
				for(Int_t c=0;c<mbo.getSize();c++){
					gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName(),
																 "%1i %1i %2i %s % 1i %2x %2x",
																 s, m, l, mbo.GetName(), c, mbo[c].getChannelMask(0),
																 mbo[c].getChannelMask(1));
				}
	    }
		}
	}
	SEPERATOR_msg("-",60);
}

void HMdcTdcChannel::mapOutChannels(void)
{
// 	// does not work yet!
//   for(Int_t s=0;s<getSize();s++)
// 		{
// 			HMdcTdcChannelSec& sec=(*this)[s];
// 			for(Int_t m=0;m<sec.getSize();m++)
// 				{
// 					HMdcTdcChannelMod& mod=sec[m];
// 					for(Int_t l=0;l<mod.getSize();l++)
// 						{
// 							HMdcTdcChannelMbo& mbo=mod[l];
// 							for(Int_t c=0;c<mbo.getSize();c++)
// 								{
// 									HMdcTdcChannelTdc &tdc=mbo[c];
// 									for(Int_t ch=0;ch<8;ch++)
// 										{
// // 											if((s==0)&&(m==0)&&(l==0))
// // 												cout << tdc.getChannelMask(1) << endl;
// 											if(tdc.getChannelMask(1)&(0x1<<ch) > 0)
// 												{
// 													if((s==0)&&(m==0)&&(l==0))
// 														cout  << s << " " << m << " " << l << " " << c << " " << ch << " " << (*conWires)[s][m][l][c*8+ch].getNCell() << endl;
// 													if((*conWires)[s][m][l][c*8+ch].getNCell() < 0)
// 														{
// 															tdc.setChannelMask(tdc.getChannelMask(0)&(!(0x1<<ch)),0);
// 															tdc.setChannelMask(tdc.getChannelMask(1)&(!(0x1<<ch)),1);
// 														}
// 												}
// 										}
// 								}
// 						}
// 				}
// 		}
}
