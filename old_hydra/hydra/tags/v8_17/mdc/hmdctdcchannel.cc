//*-- AUTHOR : J.Wuestenfeld
//*-- Modified : 13/08/2003 by J. Wuestenfeld

//_HADES_CLASS_DESCRIPTION 
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
#include "TSystem.h"
#include <iostream> 
#include <iomanip>
#include <cerrno>

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
	array->AddAt(new HMdcTdcChannelMbo((Int_t)(rMbo.getNTdcs()/8),(Char_t*)rMbo.GetName()),i);
      }
    }
  else
    {
      delete array;
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

HMdcTdcChannel::HMdcTdcChannel(const Char_t* name,const Char_t* title,
                             const Char_t* context,Int_t n)
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
	if((conWires=(HMdcConnectedWires*)gHades->getRuntimeDb()->getContainer("MdcConnectedWires")) == NULL) return kFALSE;
	if((mbo_readout=(HMdcMboReadout*)gHades->getRuntimeDb()->getContainer("MdcMboReadout")) == NULL) return kFALSE;
	if((lookupGeom = (HMdcLookupGeom *)gHades->getRuntimeDb()->getContainer("MdcLookupGeom")) == NULL) return kFALSE;
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

void HMdcTdcChannel::readline(const Char_t* buf, Int_t* set)
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

void HMdcTdcChannel::readLineFromFile(const Char_t* buf)
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

Bool_t  HMdcTdcChannel::writeChannelMasks(Char_t *ofilename, Int_t  plane, Int_t sector)
{ 
  FILE *Ofile;
  Int_t module = plane -1;
  Char_t *buf, *bufn, *fileout;
  Int_t  mbo, port, i, roc, lvl1;
  i = 0;
  buf = new char[256];
  bufn= new char[256]; 
  fileout= new char[256];
  Bool_t isFirst; 

  if(!status)
    {
      ERROR_msg(HMessageMgr::DET_MDC,"This version is no longer valid for DAQ!");
      delete []buf;
      delete []bufn;
      delete []fileout;
      return kFALSE;
    }
  if(mapNotConnectedChannels)
    mapOutUnusedChannels();
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
  default:
    delete []buf;
    delete []bufn;
    delete []fileout;
    return kFALSE;
  }
  sprintf(fileout,"%s%d%s",fileout,sector+1,"/channels.conf");
  Ofile = fopen(fileout,"w");
  if(Ofile == NULL)
    {
      delete []buf;
      delete []bufn;
      delete []fileout;
      return kFALSE;
    }
  sprintf(buf,"%s\t%d\n","plane",plane);
  fputs(buf,Ofile);
  sprintf(buf,"%s  %d\n","channelsversion",oraVersion);
  fputs(buf,Ofile);
  sprintf(buf,"%s  %d\n","RocDoesCal",getCalRotateByRocCode());
  fputs(buf,Ofile);
  HMdcMboReadoutMod & rMod = (*mbo_readout)[sector][module];
  HMdcTdcChannelMod& rModMask=(*this)[sector][module];
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
	    if (mbo < rModMask.getSize()){
	      if ((rMoth.getRoc()==roc) &&
		  (rMoth.getBusPos()==port) &&
		  (rMoth.getBus()==lvl1))
		{
		  if (isFirst) {
		    sprintf(buf,"ROC  %d\n",roc);
		    fputs(buf,Ofile);
		    isFirst=kFALSE;
		  }
		  sprintf(bufn,"mbo  %s  %d  %x  %x  %x  %x  %x  %x  %x  %x",rModMask[mbo].GetName(),port,
			  rModMask[mbo][0].getChannelMask(1),rModMask[mbo][1].getChannelMask(1),
			  rModMask[mbo][2].getChannelMask(1),rModMask[mbo][3].getChannelMask(1),
			  rModMask[mbo][4].getChannelMask(1),rModMask[mbo][5].getChannelMask(1),
			  rModMask[mbo][6].getChannelMask(1),rModMask[mbo][7].getChannelMask(1));
		  Int_t ndbo;
		  ndbo = rModMask[mbo].getSize();
		  if(ndbo > 8)
		    {
		      if(rModMask[mbo][8].getChannelMask(1) != -1)
			{
			  sprintf(buf,"%s  %x  %x  %x  %x\n",bufn,
				  rModMask[mbo][8].getChannelMask(1),rModMask[mbo][9].getChannelMask(1),
				  rModMask[mbo][10].getChannelMask(1),rModMask[mbo][11].getChannelMask(1));
			}
		      else
			sprintf(buf,"%s  %d  %d  %d  %d\n",bufn,-1,-1,-1,-1);
		    }
		  else
		    sprintf(buf,"%s  %d  %d  %d  %d\n",bufn,-1,-1,-1,-1);
		  fputs(buf,Ofile); 
		  if(!calRotateByRocCode)
		    {
		      sprintf(bufn,"mbo  %s  %d  %x  %x  %x  %x  %x  %x  %x  %x",rModMask[mbo].GetName(),port,
			      rModMask[mbo][0].getChannelMask(0)&rModMask[mbo][0].getChannelMask(1),
			      rModMask[mbo][1].getChannelMask(0)&rModMask[mbo][1].getChannelMask(1),
			      rModMask[mbo][2].getChannelMask(0)&rModMask[mbo][2].getChannelMask(1),
			      rModMask[mbo][3].getChannelMask(0)&rModMask[mbo][3].getChannelMask(1),
			      rModMask[mbo][4].getChannelMask(0)&rModMask[mbo][4].getChannelMask(1),
			      rModMask[mbo][5].getChannelMask(0)&rModMask[mbo][5].getChannelMask(1),
			      rModMask[mbo][6].getChannelMask(0)&rModMask[mbo][6].getChannelMask(1),
			      rModMask[mbo][7].getChannelMask(0)&rModMask[mbo][7].getChannelMask(1));
		      Int_t ndbo;
		      ndbo = rModMask[mbo].getSize();
		      if(ndbo > 8)
			{
			  if(rModMask[mbo][8].getChannelMask(0) != -1)
			    {
			      sprintf(buf,"%s  %x  %x  %x  %x\n",bufn,
				      rModMask[mbo][8].getChannelMask(0)&rModMask[mbo][8].getChannelMask(1),
				      rModMask[mbo][9].getChannelMask(0)&rModMask[mbo][9].getChannelMask(1),
				      rModMask[mbo][10].getChannelMask(0)&rModMask[mbo][10].getChannelMask(1),
				      rModMask[mbo][11].getChannelMask(0)&rModMask[mbo][11].getChannelMask(1));
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
  delete []buf;
  delete []bufn;
  fclose(Ofile);
  delete []fileout;
  return kTRUE;
}

Bool_t HMdcTdcChannel::writeline(Char_t *buf, Int_t sec, Int_t mod, Int_t mbo, Int_t dbo) {
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
  INFO_msg(10,HMessageMgr::DET_MDC,"sector module layer mbo channel calMask measurementMask");
  for(Int_t s=0;s<getSize();s++) {
    HMdcTdcChannelSec& sec=(*this)[s];
    for(Int_t m=0;m<sec.getSize();m++) {
      HMdcTdcChannelMod& mod=sec[m];
      for(Int_t l=0;l<mod.getSize();l++) {
	HMdcTdcChannelMbo& mbo=mod[l];
	for(Int_t c=0;c<mbo.getSize();c++){
	  gHades->getMsg()->info(10,HMessageMgr::DET_MDC,GetName(),
				 "%1i %1i %2i %s %1i %2x %2x",
				 s, m, l, mbo.GetName(), c, mbo[c].getChannelMask(0),
				 mbo[c].getChannelMask(1));
	}
      }
    }
  }
  SEPERATOR_msg("-",60);
}

void HMdcTdcChannel::mapOutUnusedChannels(void)
{
  // Maps out all channels that have no wire connected. Test is done against HMdcLookupGeom container.
  for(Int_t s=0;s<getSize();s++)
    {
      HMdcTdcChannelSec& sec=(*this)[s];
      for(Int_t m=0;m<sec.getSize();m++)
	{
	  HMdcTdcChannelMod& mod=sec[m];
	  for(Int_t l=0;l<mod.getSize();l++)
	    {
	      HMdcTdcChannelMbo& mbo=mod[l];
	      for(Int_t c=0;c<mbo.getSize();c++)
		{
		  HMdcTdcChannelTdc &tdc=mbo[c];
		  for(Int_t ch=0;ch<8;ch++)
		    {
		      if((*lookupGeom)[s][m][l][c*8+ch].getNCell() < 0)
			{
			  tdc.setChannelMask(tdc.getChannelMask(0)&(~(0x1<<ch)),0);
			  tdc.setChannelMask(tdc.getChannelMask(1)&(~(0x1<<ch)),1);
			}
		    }
		}
	    }
	}
    }
}


Bool_t HMdcTdcChannel::readRocFile(const Char_t *filename)
{
   // This function reads the values of channel masks
   // from the given file (which was created by the SAM control 
   // program by reading the values from the connected ROCs).
   // The channel masks are then stored in the related member 
   // containers of this class. The path should be something
   // like: "bla/foo/I1/ROC1"
   // Postcondition: HMdcTdcChannelSec array modified with channel masks.
   //
   // @param filename Pointer to the buffer with the filename.
   //
   // @return kTRUE, if successfully done (kFALSE otherwise).


   Char_t   *lastDigit    = NULL;
   Char_t   *rocFile      = NULL;
   Char_t   *chamberDir   = NULL;
   Char_t   *tmpfilename  = NULL;
   Char_t   buffer[256]   = { '\0' };
   UInt_t roc           = 0;
   UInt_t sector        = 0;
   UInt_t module        = 0;
   UInt_t sam           = 0;
   UInt_t rocAddr       = 0;
   UInt_t mask          = 0;
   UInt_t l1BusPort     = 0;
   UInt_t mboChainIndex = 0;
   UInt_t tdc           = 0;
   UInt_t mboDBIndex    = 0; 
   FILE *input          = NULL;


   // save filename for later access
   tmpfilename = new char[strlen(filename) + 1 + strlen(".threshold")];
   strcpy( tmpfilename, filename );

   // retrieve roc, sector and module hardware-indexes from filename
   rocFile    = new char[strlen( tmpfilename ) + 1];
   chamberDir = new char[strlen( tmpfilename ) + 1];
   strcpy( rocFile, gSystem->BaseName( tmpfilename ) );      // get ROC? 
   *((Char_t*)gSystem->BaseName( tmpfilename ) - 1) = '\0';    // override "/" 
   strcpy( chamberDir, gSystem->BaseName( tmpfilename ) );   // get I1...IV6

   errno = 0;
   roc = (UInt_t)strtol( &rocFile[3], &lastDigit, 0 );
   if(errno == EINVAL  ||  errno == ERANGE  ||  *lastDigit != 0)
   {
      ERROR_msg( HMessageMgr::DET_MDC,
		 "Error while extracting ROC number from path!" );
      return kFALSE;
   }

   errno = 0;
   sector = (UInt_t)strtol( &chamberDir[strlen( chamberDir ) - 1], 
			    &lastDigit, 0 );
   if(errno == EINVAL  ||  errno == ERANGE  ||  *lastDigit != 0)
   {
      ERROR_msg( HMessageMgr::DET_MDC,
		 "Error while extracting sector number from path!" );
      return kFALSE;
   }
   sector -= 1;

   chamberDir[strlen( chamberDir ) - 1] = '\0';
   module = 0;
   for (UInt_t iChar = 0; iChar < strlen( chamberDir ); iChar++)
      // decode or romain numbering schema
      module += chamberDir[iChar] == 'I'  ?  1  :  3;
   if (module > 4)
   {
      ERROR_msg( HMessageMgr::DET_MDC,
		 "Error while extracting module number from path!" );
      return kFALSE;
   }
   module -= 1;

   delete []rocFile;
   delete []chamberDir;

   // retrieve sam index in database format
   sam = hardwareToDatabaseIndexMap->getModuleAndSectorToSamNumber( module,
								    sector ); 
   // open files containing the masks determined by the ROCs
   strcat( (Char_t *)filename, ".threshold" );
   input = fopen( filename, "r" );
   if (input == NULL)
   {
      ERROR_msg( HMessageMgr::DET_MDC, "Could not open input file!" );
      return kFALSE;
   }

   // read the channel mask data
   while (!feof(input))
   {
      fgets( buffer, 255, input );
      if(strcmp( buffer, "#" ) != 0)
      {
	 rocAddr = 0;
	 mask = 0;
	 sscanf( buffer, "%x %x", &rocAddr, &mask );

	 // decode the address delivered by the ROC: 
	 // - are these data related to the tdc's ?
	 // - and are these data from tdc registers 3 ?
	 if((rocAddr & 0x40) == 0  &&  (rocAddr & 0x3) == 3)
	 {
	    l1BusPort     = (rocAddr >> 9) & 0x1;      // range: 0..1
	    mboChainIndex = (rocAddr >> 7) & 0x3;      // range: 1..3
	    tdc           = (rocAddr >> 2) & 0xF;      // range: 1..12

	    // this was not a mask value (e.g its something from CPLD)
	    if (tdc == 0  ||  tdc > 12) continue;
	 }
	 else 
	 {
	    // this was not a mask value (e.g its something from CPLD)
	    continue;
	 }

	 if ((Int_t)sector != (*hardwareToDatabaseIndexMap)
	     [sam][roc][l1BusPort][mboChainIndex].getSec()  ||
	     (Int_t)module != (*hardwareToDatabaseIndexMap)
	     [sam][roc][l1BusPort][mboChainIndex].getMod()    )
	 {
	    ERROR_msg( HMessageMgr::DET_MDC, 
		       "Missmatch between hadware and database indexes!" );
	    delete []tmpfilename;
	    fclose (input);
	    return kFALSE;
	 }

	 // determine the MBO and DB index in database format and select the
	 // correct daughterboard
	 mboDBIndex = (*hardwareToDatabaseIndexMap)
	    [sam][roc][l1BusPort][mboChainIndex].getMbo();
	 (*this)[sector][module][mboDBIndex][tdc - 1].fill( mask, 0 );
      }
   }

   delete []tmpfilename;
   fclose( input );
   return kTRUE;
}



Int_t HMdcTdcChannel::readRocFileVersion(const Char_t *filename)
{
   // This function reads the channel mask version (used to identify the data
   // in Oracle) from a ROC file which was created by the "tdcsetup" tool.
   // 'filename' should be something like: "bla/foo/I1/ROC1"
   //
   // @param filename Pointer to the buffer with the filename.
   //
   // @return -1, if failed, otherwise the Oracle version number.

   FILE   *input             = NULL;
   Char_t   buffer[256]        = { '\0' };
   Int_t  thresholdVersion   = -1;
   Int_t  channelMaskVersion = -1;

   // open files created by "tdcsetup"
   input = fopen( filename, "r" );
   if (input == NULL)
   {
      ERROR_msg( HMessageMgr::DET_MDC, "Could not open input file!" );
      return kFALSE;
   }

   // read the version
   while (!feof( input ))
   {
      fgets( buffer, 255, input );
      if(strncmp( buffer, "#", 1 ) == 0)
      {
	 thresholdVersion = -1;
	 channelMaskVersion = -1;
	 sscanf( buffer, "# %d %d", &thresholdVersion, &channelMaskVersion );

	 if (thresholdVersion > 0  &&  channelMaskVersion > 0)
	 {
	    // foreget about 'thresholdVersion' here - it is evaluated
	    // in HMdcTdcThreshold
	    fclose (input);
	    return channelMaskVersion;
	 }
      }
   }
   
   fclose( input );
   return -1;
}
