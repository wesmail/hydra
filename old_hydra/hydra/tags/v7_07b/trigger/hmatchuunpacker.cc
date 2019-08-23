//*-- Author : Alberica Toia
//*-- Modified : Joern Wuestenfeld 04/29/2005
//*-- Version: $Id: hmatchuunpacker.cc,v 1.39 2005-07-15 09:00:24 wuestenf Exp $

#include "hmatchuunpacker.h"
#include "hcategory.h"
#include "hades.h"
#include "hrecevent.h"
#include "heventheader.h"
#include "hmatchurich.h"
#include "hmatchushower.h"
#include "triggerinfodef.h"
#include "hlinearcategory.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "hmatchutofheader.h"
#include "hruntimedb.h"
#include "hpartialevent.h"
#include "htriggerparshowermap.h"
#include "htriggerparrichmap.h"
#include "htriggersetup.h"
#include "hmessagemgr.h"

//*****************************************************************************
//
// HMatchUUnpacker
//
// Unpacks the MatchingUnit data into its corresonding data categories.
//
// The following inline functions are defined:
// Bool_t isDebugMode(void)
//   Returns the status of the global debugging flag.
// Int_t getSubEvtId(void) const
//   Returns the sub event id that the unpacker feels responsible for.
// Int_t getRichEmptyCount()
//   Return the RICH empty counter
//
// To set debugging modes:
// void setDebugMode(Bool_t dm)
//   Set the global debug mode.
// void setRichDebugMode(Bool_t dm)
//   Enable debugging messages from the RICH part.
// void setShowerDebugMode(Bool_t dm)
//   Enable debugging messages from the SHOWER part.
//
//*****************************************************************************

HMatchUUnpacker::HMatchUUnpacker(Int_t id, Int_t units, Bool_t compression, Bool_t flog)
  : HldUnpack()
{
  // Default constructor
  //
  // Input parameters:
  //  Int_t id
  //    Sub event ID of the matching unit
  //  Int_t units
  //    Conversion flag: 0-DEG(default), 1-RAD
  //  Bool_t compression
  //    flag for the new format
  //  Bool_t flog
  //    Flag for logfile: 0 no logfile(default), 1 create logfile
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    none
  setDefaults();
  fUnit = units;
  fLog =flog;
  isCompressed = compression;
  kSubEvtId = id;
  isInitialized = kFALSE;
  setLogging();
}

HMatchUUnpacker::HMatchUUnpacker(Int_t unites, Bool_t compression, Int_t flog, TString vers)
  : HldUnpack()
{
  // Depreciated old constructor, use HMatchUUnpacker(Int_t id, Int_t units,
  //    Bool_t compression, Bool_t flog)  instead.
  //
  // Input Parameters:
  //  Int_t units
  //    Conversion flag: 0-DEG(default) 1-RAD
  //  Bool_t compression
  //    flag for the new format
  //  Bool_t flog
  //    Flag for logfile: 0 no logfile(default), 1 create logfile
  //  TString version
  //    version=old   -> before 309oct02
  //    version=nov02 -> 311oct02 configuration
  //    version=sep03 -> sep03 configuration
  //    version=aug04 (default) -> aug04 configuration
  // parameters:
  //    none
  //
  // Return code:
  //    none

  INFO_msg(10,HMessageMgr::DET_TRIGGER,"Called depreciated constructor, use HMatchUUnpacker(Int_t id, Int_t units, Bool_t compression, Int_t flog) instead!!");
  setDefaults();
  fUnit = unites;
  fLog =flog;
  isCompressed = compression;
  kSubEvtId = 512;
  isInitialized = kFALSE;
  vers.ToLower();
  if(vers.CompareTo("aug04")==0)
    {
      version = 3;
    }
  else
    {
      if(vers.CompareTo("sep03")==0)
        {
          version = 2;
        }
      else
        {
          if(vers.CompareTo("nov02")==0)
            {
              version = 1;
            }
          else
            {
              if(vers.CompareTo("old")==0)
                {
                  version=0;
                }
              else
                {
                  Warning("HMatchUUnpacker","Unknown version: %s, will run in aug04 mode!",vers.Data());
                  version = 3;
                }
            }
        }
    }
}

HMatchUUnpacker::~HMatchUUnpacker(void)
{
  // Destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  if (iterLepton)
    {
      delete iterLepton;
      iterLepton = NULL;
    }

  if(muData)
    {
      delete muData;
      muData = NULL;
    }

  if (data)
    {
      delete[] data;
      data=NULL;
    }
}

void HMatchUUnpacker::setDefaults(void)
{
  // Set default values for the internal variables.
  //
  // Input parameters:
  //    none
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    none

  fCatScalers= NULL;
  fCatRich=NULL;
  fCatShower=NULL;
  fCatTof=NULL;
  fCatLeptons=NULL;
  fCatDiLeptons=NULL;

  fMU_logfile=NULL;
  richc = 0;
  data=NULL;
  event=NULL;
  iterLepton = NULL;
  muData = NULL;
  muScalers = NULL;

  fShowerParam=NULL;
  fTrigParSho=NULL; 
  fTrigParRich=NULL;
  fTrigParRichIpu = NULL;

  fDebugMode=kFALSE;
  fRichDebugMode=kFALSE;
  fShowerDebugMode=kFALSE;

  sec_old=0;

  version = 3;
  isInitialized = kFALSE;

  // Loop over sector wise variables
  for (Int_t z=0; z<6; z++)
  {
    hitpersec[z]=0;
    nShower[z]=0;
    nTof[z]=0;
    for (Int_t y =0; y<100;y++)
    {
      mapNrTof[z][y]=0;
      mapNrShower[z][y]=0;
    }
  }

  evt=0;
  tofNr=tofLep=0;
  showerNr=showerLep=0;
}

void HMatchUUnpacker::setLogging(void)
{
  // logging  mechanism
  if(fLog)
  {
    fMU_logfile = fopen("MU-Bad-Angle.log","w");
    if (fMU_logfile == NULL)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Cannot open MU- log file\n");
      exit(-1);
    }
  }
  else 
    fMU_logfile = stderr;
}

Bool_t HMatchUUnpacker::reinit(void)
{
// 	if(!isInitialized)
// 		{
// 			Rich_Sector_Map = fTrigParRichIpu->getMap();
// 			Row_Offset = fTrigParRichIpu->getRowOffset();
//      isInitialized = kTRUE;
// 		}
  return kTRUE;
}

Bool_t HMatchUUnpacker::init(void)
{
  // Initalization function for unpacker. Called from Hades::init().
  // If one parameter container is not found, a message is printed to identify the missing container.
  //
  // Input parameters:
  //    none
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //  kTRUE if all containers and needed categories are found, otherwise kFALSE.
  if(gHades)
  {
    event= (HRecEvent*) gHades->getCurrentEvent();

    if (event)
    {
      if(event->getPartialEvent(catMatchU))
      {
        muData = (HMUData*)(event->getPartialEvent(catMatchU)->getSubHeader());
      }
      fCatRich=event->getCategory(catMatchURich);
      if (!fCatRich)
      {
        fCatRich=new HLinearCategory("HMatchURich");
        event->addCategory(catMatchURich,fCatRich,"Trigger");
      }

      fCatShower=event->getCategory(catMatchUShower);
      if (!fCatShower)
      {
        fCatShower=new HLinearCategory("HMatchUShower");
        event->addCategory(catMatchUShower,fCatShower,"Trigger");
      }

      fCatTof = event->getCategory(catMatchUTof);
      if (!fCatTof)
      {
        fCatTof = new HLinearCategory("HMatchUTof");
        event->addCategory(catMatchUTof,fCatTof,"Trigger");
        fCatTof->setCatHeader(new HMatchUTofHeader);
      }

      fCatLeptons = event->getCategory(catMatchULeptons);
      if (!fCatLeptons)
      {
        fCatLeptons = new HLinearCategory("HMULeptons");
        event->addCategory(catMatchULeptons,fCatLeptons,"Trigger");
      }
      if (!fCatDiLeptons)
      {
        fCatDiLeptons = new HLinearCategory("HMUDiLeptons");
        event->addCategory(catMatchUDiLeptons,fCatDiLeptons,"Trigger");
      }

      if (!fCatScalers)
      {
        fCatScalers = new HLinearCategory("HMUScalers");
        event->addCategory(catMatchUScalers,fCatScalers,"Trigger");
      }

      if (!muData)
      {
        muData = new HMUData();
        event->getPartialEvent(catMatchU)->setSubHeader(muData);
      }

      iterLepton = fCatLeptons->MakeIterator();

      HRuntimeDb* rtdb=gHades->getRuntimeDb();
      if(rtdb)
      {
      //-------------------------------------

        fTrigParSho=(HTriggerParShowerMap *)rtdb->getContainer("TriggerParShowerMap");
        if (fTrigParSho)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameters SHOWER mapping initialized");
        }
        else
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameter SHOWER mapping initialization failed!!");
          return kFALSE;
        }

        //-------------------------------------

        //fTrigParRichIpu = (HTriggerSetup *)rtdb->getContainer("TriggerSetup");
// 				if(!fTrigParRichIpu)
// 					{
// 						INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameter RICH mapping initialization failed!!");
// 						return kFALSE;
// 					}

        //-------------------------------------

        fTrigParRich=(HTriggerParRichMap *)rtdb->getContainer("TriggerParRichMap");
        if (fTrigParRich)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameters RICH mapping initialized");
        }
        else
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"Trigger parameter RICH mapping initialization failed!!");
          return kFALSE;
        }


        //-------------------------------------
        fShowerParam = (HShowerUnpackParam*)rtdb->getContainer("ShowerUnpackParam");
        if (!fShowerParam)
        {
          Warning("init","ShowerUnpackParam container not found in rtdb "); 
          Warning("init","call default initialisation");
          fShowerParam = new HShowerUnpackParam;
          fShowerParam->defaultInit();
        }
      }//if(rtdb)
      else
      {
        Error("init","no runtimeDb found !");
        return kFALSE;
      }

      //-------------------------------------

    }//if(event)
    else
      return kFALSE;
  }//if(gHades)
  else
    return kFALSE;

  return kTRUE;
}

Int_t  HMatchUUnpacker::decodeMUData(UInt_t *pData,Int_t maxLength)
{
  // Decode the data from the matching unit.
  //
  // Input Parameters:
  //  UInt_t *pData
  //    Pointer to datastructure containing MU part of the MU sub event
  //  Int_t maxLength
  //    Length of the datablock
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  number of bytes read from datablock
  UInt_t *pDataL=NULL;
  UInt_t *pDataD=NULL;
  Int_t length=0;
  Int_t r=0;
  UInt_t *ptr =NULL;
  UInt_t scaler[14], sc[14];
  Int_t number_of_scalers=0;

  triggerCode=-1;

  for (Int_t z =0; z<14; z++)
    {
      scaler[z]=0;
      sc[z]=0;
    }

  if (muData->getTriggerCodeFirstFormat() == 0 )
    {
      length = pData[0]; // in 16 bit  word
      muData->setMULength(length);
      r= length+1;
      ptr = pData+1;
    }
  else
    {
      triggerCode = *pData & 0xf;
      length= pData[1] ; // in 16 bit  word
      muData->setMULength(length);
      ptr= pData +2;
      r= length+2;
    }

  muData->clear();
  muData->setTriggerCode(triggerCode);

  if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MU-Data length :%i %i \n", triggerCode,length );
    }
  if (length == 1 )
    {
      return r; 
    }

  HMULeptons *muLept=NULL;
  HMUDiLeptons *muDiLept=NULL;

  if ( length > maxLength)
    {
      Error("decodeMUData","size inconsistency");
      r=-1; return r; 
    }

  Int_t muDataSize = 0 ;

  Int_t scaler_flag =0;
  Int_t scalers_length = 20;

  if ( (gHades->isCalibration()) && (length >2))
    {
      // for calibration events (in previous times triggerCode != 1) the length
      // of MU data would be 2
      // if it is > than 2, it means that there are scalers
      // in this case we make the decode of mudata with a reduced length (2)
      // the scaler length is added at the end where the scalers are decoded
      // note: scalers can be present only in calibration events!!!
      length -= scalers_length;
      scaler_flag =1;
      if (fDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Scaler event, corrected length to %i\n",length);
        }
    }
  muDataSize =  muData->decode(ptr,length);
  if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MU-Data length :%i %i \n", length, r );
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"mu data size %i",muDataSize);
    }
  if ( muDataSize < 0 )
    { 
      Error("decodeMUData","data word decoding problem ");
      r =-1; return r;
    }

  // no leptons found
  // if (muData->getNLeptons() == 0 ) return r;
  if ( muData->getNLeptons() > 100 )
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," nleptons out of bound. %i \n", muData->getNLeptons());
      return (-1); 
    }

  pDataL=pData+muDataSize+1;

  Int_t dataw=0;
  Int_t sector,RichNr,MetaNr,DetBit,eflag; 
  Float_t moml;
  Float_t *pMass=NULL;

  sector = 0;
  RichNr = 0;
  MetaNr = 0;
  DetBit = 0;
  eflag  = 0;
  moml   = 0.0;
  fLoc.set(0);

  if (length - (muDataSize+1) >=0)
    {

      if (fDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MU-Data decoding %i leptons \n", muData->getNLeptons());
        }
      for (Int_t nl=0; nl<2*(Int_t)muData->getNLeptons(); nl+=2)
        {
          muDataSize+=2;
          dataw = ((pDataL[nl]<<16) | (pDataL[nl+1] & 0x0000FFFF));
          sector = getLeptonSector(dataw);
          RichNr = getLeptonRichNr(dataw);
          MetaNr = getLeptonMetaNr(dataw);
          DetBit = getLeptonDetBit(dataw);
          eflag  = getLeptonFlag(dataw);
          moml   = getLeptonMom(dataw);
          if (fDebugMode)
            {
              gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Lepton dataword: 0x%x\n",dataw);
            }

          muLept=(HMULeptons *)fCatLeptons->getNewSlot(fLoc);
          if (muLept)
            {
              muLept=new (muLept) HMULeptons;
              muLept->setSector(sector);
              muLept->setRichNr(RichNr);
              muLept->setMetaNr(MetaNr);
              muLept->setDetBit(DetBit);
              muLept->setEFlag(eflag);
              muLept->setMomentum(moml);
            }
        }//for(Int_t nl=0; nl<2*(Int_t)muData->getNLeptons(); nl+=2)
    }//if(length - (muDataSize+1) >=0)

  if ( (length==2) || (length ==3) )
    {
      // total length of MU=2 means that there is not even a rich
      muData->setNDiLeptons( 0 );
    }
  else
    {
      if (length - (muDataSize+1) >=0)
        {
          pDataD = pDataL+ (muData->getNLeptons()*2);
          muData->setNDiLeptons( *pDataD );
        }
      else
        {
          muData->setNDiLeptons( 0 );
        }
    }
  Int_t nn1=0;
  Int_t nn2=0; 

  // decode dileptons numbers

  if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MU-Data decoding %i Dileptons \n", muData->getNDiLeptons());
    }


  for (Int_t nl=1; nl<(1+(3*(Int_t)muData->getNDiLeptons())); nl+=3)
    {
      muDataSize += 3;
      dataw = pDataD[nl] & 0x0000FFFF;
      if (fDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"DILepton dataword: 0x%x\n",dataw);
        }
      nn1= dataw & 0xFF;
      nn2= (dataw >> 8 ) & 0xFF;
      dataw = ((pDataD[nl+1]<<16) | (pDataD[nl+2] & 0x0000FFFF));
      muDiLept=(HMUDiLeptons *) fCatDiLeptons->getNewSlot(fLoc);
      if (muDiLept)
        {
          muDiLept=new (muDiLept) HMUDiLeptons;
          muDiLept->setN1( nn1 );
          muDiLept->setN2 ( nn2 );

          pMass = (Float_t *) &dataw;
          muDiLept->setMass(*pMass);

          if (fDebugMode)
            {
              gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"inv mass %f %s %f", muDiLept->getMass(),", Dataw ", *pMass);
            }
        }
    } //for (Int_t nl=1; nl<(1+(3*(Int_t)muData->getNDiLeptons())); nl+=3)

  // -------------- scalers hack -------------------------------
  if (scaler_flag ==1)
    {
      length += scalers_length; // here the scaler_length is added again and the scalers are decoded
    }

  if(muDataSize < length)
    { // there is an additional length reserved for scalers and marked by the scaler_flag
      number_of_scalers=0;
      ptr= pData + 2 + muDataSize;

      //the first 5 scalers are composed of 2 datawords
      for(int i=0; i<10; i+=2)
        {
          if((muDataSize+i)<length)
            {
              sc[number_of_scalers] = (*(ptr+i)<<16) | (*(ptr+i+1));
              number_of_scalers++;
            }
        }

      // the next 8 scalers are single datawords
      for (int i=10;i<18;i++)
        {
          if((muDataSize+i)<length)
            {
              sc[number_of_scalers]=*(ptr+i);
              number_of_scalers++;
            }
        }

      // the last scaler is again composed of 2 datawords
      for(int i=18; i<20; i+=2)
        {
          if((muDataSize+i)<length)
            {
              sc[number_of_scalers] = (*(ptr+i)<<16) | (*(ptr+i+1));
              number_of_scalers++;
            }
        }


      muScalers=(HMUScalers *)fCatScalers->getNewSlot(fLoc);

      if (muScalers)
        {
          muScalers =new (muScalers) HMUScalers();

          for( Int_t i = 0; i< 14; i++)
            {
              scaler[i]=0;
            }

          for( Int_t i = 0; i< number_of_scalers; i++)
            {
              scaler[i]=getScaler(sc[i]);
            }
          muScalers->setScaler0(scaler[0]);
          muScalers->setScaler1(scaler[1]);
          muScalers->setScaler2(scaler[2]);
          muScalers->setScaler3(scaler[3]);
          muScalers->setScaler4(scaler[4]);
          muScalers->setScaler5(scaler[5]);
          muScalers->setScaler6(scaler[6]);
          muScalers->setScaler7(scaler[7]);
          muScalers->setScaler8(scaler[8]);
          muScalers->setScaler9(scaler[9]);
          muScalers->setScaler10(scaler[10]);
          muScalers->setScaler11(scaler[11]);
          muScalers->setScaler12(scaler[12]);	

          Float_t *scaF=NULL;
          scaF = (Float_t *) &scaler[13];
          muScalers->setScaler13(*scaF);
        }//if(muScalers)
    }//if(muDataSize < length)

  return r;
}

Int_t HMatchUUnpacker::decodeRich(UInt_t *pData,Int_t maxLength)
{
  // Decode the RICH part of the MU subevent
  //
  // Input parameters:
  //  UInt_t *pData
  //    Pointer to the datastructure containing the RICH part of the MU subevent
  //  Int_t maxLength
  //    Size of the datablock in bytes
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Number of bytes read from datablock
  Int_t length= pData[0]*2;
  Int_t r= length+1,ipusize; 
  UInt_t* ptr= pData+1;

  UInt_t MU_sec_pattern_Rich =0;
  RichSecPatternMask = 0x0;

  sec_count = 0;

  if ( length == 6 )
    {
      richc++;
    }
  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," decodeRich length:%i pdata0:%x \
            pData1:%x  maxLength:%i\n", length,
            pData[0],pData[1], maxLength);
  }

  if ( length > maxLength)
  {
    Error("decodeRich","size inconsistency");
    r=-1;
  }


  ipusize=0;
  while ( length > 0 && r>0  )
  {
    ipusize=decodeIpuRich(ptr,length);
    sec_count++;

    if(ipusize==-1)
    {
      r=-1;
    }
    else
    {
      length-=ipusize;
      ptr+=ipusize;
    }
  }

  MU_sec_pattern_Rich = muData->getSecHitPatternRich();

  if(  ( muData->getMULength() > 5) && (MU_sec_pattern_Rich != RichSecPatternMask)  &&  (gHades->isReal())
      && ( (muData->getNHitsShower() + muData->getNHitsTof()) > 1 ) )
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," ERROR: Rich Hit Pattern Wrong!!! 0x%x 0x%x %s 0x%d",MU_sec_pattern_Rich, RichSecPatternMask," TRIGGER CODE: ",triggerCode);
  }
  return r;
}

Int_t HMatchUUnpacker::decodeIpuRich(UInt_t *pData,Int_t maxLength)
{
  // Decode the RICH IPU part of the MU subevent.
  // The trigger code inthe first dataword is skipped!
  //
  // Input parameters:
  //  UInt_t *pData
  //    Pointer to the datastructure containing the RICH IPU part of the MU subevent
  //  Int_t maxLength
  //    Number of bytes in the datastructure
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  number of bytes in the Rich subEvent or
  //  -1 for error.

  HMatchURich *muRich=NULL;
  Int_t length=getRichSize(pData[1])*2;  //One 32bit word goes to 2.
  UInt_t sec_header = getRichSecHeader(pData[0]);
  if (length == 0 )
  {
    return 0;
  }
  Int_t r=length,i=0,bc;
  UInt_t fifo,colPat,hardsegId, segId,row,col, segId_old, row_old, col_old;

  // --- here are the rich IPU configuration - hardcoded ---

  UInt_t Rich_Sector_Map_old  [6]={0,5,1,3,2,4}; // before 309oct02
  UInt_t Rich_Sector_Map_nov02[6]={1,5,0,3,2,4}; //311oct02 configuration
  UInt_t Rich_Sector_Map_sep03[7]={0,1,2,3,4,5,7}; // sep03 configuration

  UInt_t Rich_Sector_Map[7];
  for(Int_t i=0;i<7;i++)
  {
    Rich_Sector_Map[i]=0;
  }
  if(version==0)
  {
    for(Int_t i=0;i<6;i++)
    {
      Rich_Sector_Map[i]=Rich_Sector_Map_old  [i];
    }
  }
  else
  {
    if(version==1)
    {
      for(Int_t i=0;i<6;i++)
      {
        Rich_Sector_Map[i]=Rich_Sector_Map_nov02[i];
      }
    }
    else
    {
      if(version==2)
      {
        for(Int_t i=0;i<7;i++)
        {
          Rich_Sector_Map[i]=Rich_Sector_Map_sep03[i];
        }
      }
      else
      {
        if(version==3)
        {
          for(Int_t i=0;i<7;i++)
          {
            Rich_Sector_Map[i]=Rich_Sector_Map_sep03[i];
          }
        }
      }
    }
  }//else (if(version==0))

  if (version<3)
  {
    Row_Offset=23;
  }
  else
  {
    Row_Offset=26;
  }

  Float_t theta, phi;
  Int_t FullCount, ReducedCount;

  segId_old = row_old = col_old = 0; //these are used for internal check
  FullCount = ReducedCount = 0;
  bc = fifo = colPat = hardsegId = segId = row = col =0;
  theta = phi = 0.;
  fLoc.set(0);

  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"decodeIpuRich length:%i", length);
  }

  if (length>maxLength)
  {
    Error("decodeRich","SubEvent size too big");
    if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," decodeIpuRich pData[1]:%i length:%i maxL:%i \n",pData[1], length, maxLength);
    }
    r=-1;
  }
  else
  {
    for (i=2;i<length;i+=2)
    { //i=2 --> first data word
      // Internal checks does not works proprely !!!
      //  if ( checkRichMSW(pData[i]) && checkRichLSW(pData[i+1]) ) {
      if(kTRUE)
      {
        UInt_t tempPat=0;
        fifo=getRichFifoNb(pData[i])-1;
        colPat=getRichColumPattern(pData[i]);
        hardsegId=getRichSegId(pData[i+1]);
        if (colPat==0)
        {
          INFO_msg(10, HMessageMgr::DET_TRIGGER,"HMUUnpack: Problem in RichIPU colpat=0 (not possible)!!! ");
        }
        if ((hardsegId < 1) || (hardsegId >6) )
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %s %i %s %i",evt, " HMUUnpack: Mismatch in RichIPU!!! HardsegId = ", hardsegId, "data word ",pData[i+1]);
          hardsegId = 7;
        }
        segId = Rich_Sector_Map[hardsegId-1];
        row=getRichRow(pData[i+1]);

        if (fRichDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"decodeIpuRich: colpattern: 0x%x", tempPat);
        }

        for(bc=0, tempPat=colPat;bc < 8; tempPat>>=1,bc++)
        {
          if ( (tempPat & 0x1) != 0 )
          {
            if ( bc < 8 )
            {
              col = 95 - fifo * 8 - bc;
              if (fRichDebugMode) 
              {
                gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"decodeIpuRich: col-row: %i %i", col,row);
              }

              phi   = fTrigParRich->getRichPhi(row-Row_Offset,col);
              theta = fTrigParRich->getRichTheta(row-Row_Offset,col);  

              if ( (theta<999)&&(phi<999) )
              {
                if (fRichDebugMode)
                {
                  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"decodeIpuRich: decoding ...  row:%i  column:%i fifo:%i bitpos:%i \
                        segId:%i theta:%f phi:%f",   row-Row_Offset, 95 - fifo * 8 - bc,fifo,bc,segId,theta,phi);
                }
                muRich=(HMatchURich *)fCatRich->getNewSlot(fLoc);
                if (muRich)
                {
                  muRich=new (muRich) HMatchURich;
                  muRich->setFifo(fifo);
                  muRich->setColumnPattern(colPat);	      
                  muRich->setSegmentId(segId);
                  muRich->setRowNb(row);
                  muRich->setColumn(col);
                  muRich->setRow(row-Row_Offset);
                  muRich->setY(col);
                  muRich->setX(row-Row_Offset);

                  RichSecPatternMask |= (1<<segId);
                  if (segId<5)
                  {
                    phi += 60*(segId+1);
                  }    
                  muRich->setTheta(theta);
                  muRich->setPhi(phi); 
                  hitpersec[segId]++;

                  // if there is a mismatch in the RichIPU sector information, it is corrected and reset
                  // a warning message appeare
                  if ( ( segId != sec_count ) || ( segId != Rich_Sector_Map[sec_header-1] ) )
                  {
                    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %s %i %s %i %s %i %s %i %s %i", evt,
                      "HMUUnpack: Mismatch in RichIPU!!! Sector_Count",sec_count," Sector_Header",Rich_Sector_Map[sec_header-1],"(mapped from",
                      sec_header, ") SegId",segId,"Hardseg",hardsegId);
                    muRich->setSegmentId(sec_count);
                  }

                  // internal check on the hit ordering: X(row) has priority and must increase, Y(col) must decrease

                  if (row < row_old)
                  {
                    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %s %i %s %i %s %i",evt,"HMUUnpack: Mismatch in RichIPU!!! in sec",
                      sec_count,"X",row,"after X_old",row_old); 
                  }
                  else
                  {
                    if (row == row_old)
                    {
                      if ( col > col_old )
                      {
                        gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %s %i %s %i %s %i %s %i",evt,"HMUUnpack: Mismatch in RichIPU!!! in sec ",sec_count,"in X", row,"Y",col,"after Y_old",col_old); 
                      }
                    }
                  }
                  if (segId != segId_old)
                    {
                      FullCount=ReducedCount=0;  
                    }

                    row_old = row;
                    col_old = col;
                    segId_old = segId;

                    if(theta>0.1)
                    { // valid range of mapped rings
                      if(ReducedCount>99)
                      {
                        INFO_msg(10,HMessageMgr::DET_TRIGGER,"HMatchUUnpacker::decodeIpuRich():array out of bounds");
                      }
                      mapNrRich[segId][ReducedCount]=FullCount;
                      ReducedCount++;
                    }
                    FullCount++;
                }
              }
              else
                {
                  if (fRichDebugMode)
                    {
                      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),
                                            "decodeIpuRich: !!!!!!! DISCARDED  row:%i  column:%i fifo:%i bitpos:%i segId:%i theta:%f phi:%f",
                                            row-Row_Offset, 95 - fifo * 8 - bc,fifo,bc,segId,theta,phi);
                    }
                }
            }
          }
        }
      }
      else
        {
          Error("decodeRich","Corrupted data word detected");
        }
    }
  }
  return r;
}

Int_t HMatchUUnpacker::decodeTof(UInt_t *pData,Int_t maxLength)
{
  // Decode the TOF part of the MU subevent
  //
  // Input parameters:
  //  UInt_t *pData
  //    Pointer to datastructure containing TOF part of the MU subevent
  //  Int_t maxLength
  //    Number of bytes in the datablock
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  Number of bytes read from datastructure, -1 n case of error.

  Int_t length= pData[0]*2;
  if (length == 0 ) return 0;
  Int_t r= length+1,ipusize=0;
  UInt_t* ptr= pData+1;

  UInt_t MU_sec_pattern_Tof =0;
  TofSecPatternMask = 0x0;

  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," ***********begin MU-Tof %x %x %x %x \n", pData[0], pData[1],pData[2],pData[3]);
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," decodeTof length:%i \n", length);
  }
  if ( length > maxLength)
  {
    Error("decodeTof","size inconsistency");
    r=-1;
  } 
  else
  {
    TofIpuNr = 0;

    while ( length > 0 && r>0 )
    {
      TofIpuNr++;
      ipusize=decodeIpuTof(ptr,length);
    if(ipusize==-1)
    {
        r=-1;
    }
    else
    {
      length-=ipusize;
      ptr+=ipusize;
    }
    }

    MU_sec_pattern_Tof = muData->getSecHitPatternTof();

    if ( (muData->getSecHitPatternRich()>0 )&&  ( MU_sec_pattern_Tof != TofSecPatternMask ) )
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %i",evt,"ERROR: Tof Hit Pattern Wrong!!!",MU_sec_pattern_Tof, TofSecPatternMask );
    }
  }
  return r;
}

Int_t HMatchUUnpacker::decodeIpuTof(UInt_t *pDat,Int_t maxLength)
{
  // Decode the TOF IPU part of the MU subevent
  //
  // Input parameters:
  //  UInt_t *pDat
  //    Pointer to datastructure with TOF IPU data
  //  Int_t maxLength
  //    Number of bytes in data block
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  Number of bytes in Tof subevent. -1 n case of error

  Int_t length =  ((pDat[0]<<16) | (pDat[1] & 0x0000FFFF))*2; 
  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"values of lenght data word %x %x %i",pDat[0],pDat[1] ,length);
  }
  Int_t r=length+2;
  Int_t tofregister = (pDat[2] << 16) | (pDat[3] & 0x0000FFFF);
  HMatchUTofHeader *tofheader = (HMatchUTofHeader*)(fCatTof->getCatHeader());
  fLoc.set(0);

  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," decodeIpuTof length:%i", length);
  }

  if (length > maxLength)
  {
    Error("decodeIpuTof","size inconsistency");
    return -1;
  }

  if (!tofheader)
  {
    Error(" decodeIpuTof","header missing");
    r=-1;
  }
  else
  {
    HMatchUTof *mtof=NULL;
    Int_t headerSize=4;
    UInt_t dataWord=0;
    Int_t i=0,cursor=0;
    tofheader->setTriggers(getTofTriggerTag(tofregister),
                          getTofTriggerCode(tofregister));
    tofheader->setID(getTofId(tofregister));  
    tofheader->setVeto(getTofVeto(tofregister));

    for (cursor=headerSize, i=0; i < (length - 2)/2; cursor+=2, i++)
    {
      mtof = (HMatchUTof*)fCatTof->getNewSlot(fLoc);
      if (mtof)
      {
        mtof = new (mtof) HMatchUTof;
        dataWord = (pDat[cursor] << 16) | (pDat[cursor+1] & 0xFFFFFFFF);
        fillTofData(mtof,dataWord);
      }
      else
      {
        Warning("decodeTof","No memory available for TOF subsubevent");
      }
    } 
  }
  return r;
}

Int_t HMatchUUnpacker::fillTofData( HMatchUTof* mtof, UInt_t dataword)
{
  // Fill data of TOF MU information in datacontainer.
  //
  // Input parameters:
  //  HMatchUTof *mtof
  //    Pointer to TOF datacontainer with MU information
  //
  //  UInt_t dataword
  //    Dataword to be stored in mtof
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //  Number of sector decoded from dataword.

  ULong_t theta = dataword & 0xff00;
  theta = theta >> 8;
  Int_t sec = (dataword & 0xf);
  ULong_t dtheta = (dataword >>15 ) & 0x1;
  ULong_t phi = (dataword >> 16) & 0xff;
  Int_t dphi = (dataword >> 31) & 0x1;
  Float_t time = ((float) ((dataword >> 24) & 0xff))/2.;

  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"ingo time %i %s %f",((dataword >> 24) & 0xff),"cal time",time);
  }
  Int_t PID = (dataword >> 4) & 0x3;
  Float_t atheta =  theta/255. * 90.;
  Float_t aphi =  phi/255. * 60.;  

  if (sec < 5)
  {
    //phi = phi + (sec+1)*60;
    aphi = aphi + (sec+1)*60;
    dphi = dphi + (sec+1)*60;
  }
  if (sec > 5)
  {     // Invalid sector number
    sec = 6;
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %s 0x%x %s %d",evt," *********** MU-Tof Invalid sector: ",dataword,
    "IPU Nr", TofIpuNr);
  }

  mtof->setTheta(atheta, dtheta);
  mtof->setPhi(aphi,dphi);
  mtof->setiTheta(theta);
  mtof->setiPhi(phi);
  mtof->setTime(time);
  mtof->setPID(PID);
  mtof->setSec(sec);

  TofSecPatternMask |= (1<<sec);

  nTof[sec]++;

  if (sec != sec_old)
  {
    tofLep = 0;
  }
  if (PID ==1)
  {
    if(tofLep>99)
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"HMatchUUnpacker::fillTofData():array out of bounds");
    }
    mapNrTof[sec][tofLep]=tofNr;
    tofLep++;
  }
  tofNr++;
  sec_old = sec;

  return sec;
}

Int_t HMatchUUnpacker::storeShowElements(Int_t buildId, Int_t rowPattern, Int_t col)
{
  // Store the Shower information.
  //
  // Input parameters:
  //   Int_t buildId
  //     Number of the IPC module
  //   Int_t rowPattern
  //     pattern which encodes the y information of several hits
  //   Int_t col
  //     Number of the column the row pattern codes.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //  Returns always 0!
  Float_t Sh_phi=0.;
  Int_t bc=0;
  HMatchUShower *muShower= NULL;
  Int_t tempPat=rowPattern;
  Int_t sec,nRowOffset,nSwap;

  bc=sec=nRowOffset=nSwap=0;
  HShowerSubIdPar *pSubIdPar=NULL;

  fLoc.set(0);
  if (fShowerParam)
  {
    pSubIdPar= showerParamConfigure(fShowerParam, buildId);
  }
  else
  {
    Error("storeShowElements","no parameter container found");
  }

  if (fShowerDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%x  row pattern", tempPat);
  }

  //while ( bc < 16 )
  for(bc=0, tempPat=rowPattern; bc < 16; bc++, tempPat>>=1)
  {
    if( (tempPat & 0x1) != 0 )
    {
      muShower=(HMatchUShower *)fCatShower->getNewSlot(fLoc);
      if (muShower)
      {
        muShower=new(muShower) HMatchUShower;
        muShower->setBuildID(buildId);
        muShower->setColumn(col);
        muShower->setRowPattern(rowPattern);

        if(pSubIdPar)
        {
          sec = pSubIdPar->getSector();
          nRowOffset = 16 * pSubIdPar->getPartOfSector();
          nSwap = (pSubIdPar->getSwap()) ? -1 : 1;
          if (nSwap == -1)
          {
            nRowOffset += 8;
          }
          muShower->setSector(sec); 
          muShower->setRowOffset(nRowOffset);
        }
        muShower->setRow(bc+nRowOffset);
        ShowerSecPatternMask |= (1<<sec);

        if (fShowerDebugMode)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"here starts the shower mapping");
        }
        muShower->setTheta(fTrigParSho->getShowerTheta(col,bc+nRowOffset));

        Sh_phi = fTrigParSho->getShowerPhi(col,bc+nRowOffset);
        if (sec < 5)
        {
          Sh_phi += ((sec + 1) *60);
        }
        muShower->setPhi(Sh_phi);
        //muShower->setPhi(fTrigParSho->getShowerPhi(bc+nRowOffset,col));
        if (fShowerDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"shower sec: %i %s %i %s %i %s %f",sec,"row:",bc+nRowOffset,
                                "col :",col,"phi",Sh_phi);
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"row %i %s %i $s %f %s %f",bc+nRowOffset,"col", col,
                                "theta", muShower->getTheta(),"phi",muShower->getPhi());
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"end of the shower mapping");
        }

        nShower[sec]++;
        if (fShowerDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"sector %i %s %i %s %i",sec," nShower[",sec,"]=",nShower[sec]);
        }
        if (sec != sec_old)
        {
          showerLep = 0;
        }
        if(showerLep>99)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"HMatchUUnpacker::storeShowElements(():array out of bounds");
        }
        mapNrShower[sec][showerLep]=showerNr;
        if (fShowerDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"mapNrShower[%i%s %i",sec,"][",showerLep,"]=",showerNr);
        }
        showerLep++;
        showerNr++;
        sec_old = sec;
      }//if(muShower)
    }//if( (tempPat & 0x1) != 0 )

    //tempPat>>=1;
    //bc++; 
  }//while ( bc < 16 )
  return 0;
}

HShowerSubIdPar*  HMatchUUnpacker::showerParamConfigure(HShowerUnpackParam* param, Int_t subEvtId)
{
  // Get pointer to SHOWER SubId parameter container.
  //
  // Input parameters:
  //   HShowerUnpackParams *param
  //     Pointer to SHOWER unpacker parameters
  //
  //   Int_t subEvtId
  //     Sub event ID for which to get the parameters
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //    Pointer to the SHOWER subId parameters.
  //    Return NULL if parameters are not found.
  HShowerSubIdPar *pSubIdPar=NULL;

  if (fDebugMode) 
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," Sub event Id %d",subEvtId);
  }
  pSubIdPar = param->getSubIdPar(subEvtId);
  if (pSubIdPar)
  { 
    return   pSubIdPar;
  }
  else
  {
    if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"showerParamConfigure: not corrected initialization of MUShowerUnpacker id = %d",subEvtId );
    }
    return NULL;
  }
}

Int_t HMatchUUnpacker::decodeShower(UInt_t *pData,Int_t maxLength)
{
  // Decode SHOWER part of MU data.
  //
  // Input parameters:
  //   UInt_t *pData
  //     Pointer to the SHOWER part of the MU data.
  //
  //   Int_t maxLength
  //     Length of the datablock
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //    Number of Bytes read from datablock, -1 in case of error.

  // 32-bit exclusive
  Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF))*2;
  Int_t r=length+2;
  Int_t ipusize=0;
  UInt_t* ptr= pData+2;
  UInt_t MU_sec_pattern_Shower =0;
  ShowerSecPatternMask = 0x0;

  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MU shower length %i", length);
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"begin MU-Shower %x %x %x %x", pData[0], pData[1],pData[2],pData[3]);
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"decodeShower length: %i", length);
  }
  if ( length > maxLength)
  {
    Error("decodeShower","size inconsistency");
    r=-1;
  }

  while ( length > 0 && r>0 )
  {
    ipusize=decodeIpuShower(ptr,length);

    if(ipusize==-1)
    {
      r=-1;
      return r; 
    }
    else
    {
      length-=ipusize;
      ptr+=ipusize;
    }
    if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"SHOWER length &i %s %i",length,"ipusize",ipusize);
    }
  }

  MU_sec_pattern_Shower = muData->getSecHitPatternShower();

  if ( (muData->getSecHitPatternRich()>0 )&&  ( MU_sec_pattern_Shower != ShowerSecPatternMask ) )
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"event %i %s %i %i",evt,"ERROR: Tof Hit Pattern Wrong!!!",MU_sec_pattern_Shower,
      ShowerSecPatternMask);
  }

  return r;
}

Int_t  HMatchUUnpacker::decodeIpuShower(UInt_t *pData,Int_t maxLength)
{
  // Decode SHOWER IPU part of the MU data.
  //
  // Input Parameters:
  //   UInt_t *pData
  //     Pointer to the SHOWER IPU data.
  //
  //   Int_t maxLength
  //     Length of datablock
  //
  // Output Parameters:
  //   none
  //
  // Return code:
  //   Number of bytes read from datablock.
  Int_t col=0,rowPat=0;
  Int_t showBuildId=0;
  // Int_t length =  ((pData[0]<<16) | (pData[1] & 0x0000FFFF));
  Int_t length =  (pData[1] & 0x0000FFFF);
  Int_t r=length; // 16-bit counted inclusive
  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"values of lenght data word %x %x %i",pData[0],pData[1] ,length);
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"decodeIPuShower length: %i", length);
  }
  if ( length > maxLength)
  {
    Error("decodeIpuShower","size inconsistency");
    r=-1;
    return r;
  }

  Int_t showerheader = (pData[2] << 16) | (pData[3] & 0x0000FFFF);
  showBuildId = getShowBuildID( showerheader );
  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"showBuildId",showBuildId);
  }
  length-=4;
  Int_t c=4,dataw=0;
  Int_t ic=0;

  if (length >0)
  { // there is at leat 1 hit or a trailer !!!
    while ( length > 2 )
    { // it is necessarily a hit
      dataw = (pData[c] << 16) | (pData[c+1] & 0x0000FFFF);
      rowPat = getShowRowPattern( dataw );
      col    = getShowColumn( dataw );
      if (fDebugMode)
      {
        gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"shower row col dataword %i %s %i %s %i",dataw,"row pat",rowPat,"col",col);
      }

      storeShowElements(showBuildId,rowPat,col);
      ic++;
      c+=2;
      length-=2;
    }

    dataw = (pData[c] << 16) | (pData[c+1] & 0x0000FFFF);

    if(fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"shower lenght now is: %i",length);
    }
    if (length == 2 )
    {  // there is still a trailer!
      if (pData[c] == 0x7000)
      {
        c+=2;
        length-=2;
        Int_t framec = getShowFrameCount( dataw );
        //   printf(" dataword %x \n", dataw);
        if ( framec != ( r >> 1 ) - 2 )
        {
          if(fDebugMode)
          {
            Error("-I- decodeIpuShower","trailer check failed ");     
          }
        }
      }
      else
      {
        rowPat = getShowRowPattern( dataw ); 
        col    = getShowColumn( dataw );
        if (fDebugMode)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"shower row col dataword  %i %s %i %s %i",dataw,"row pat",rowPat,"col",col);
        }

        storeShowElements(showBuildId,rowPat,col);
        ic++;
        c+=2;
        length-=2;
      }
    } //decode trailers 

    else
    {
      if (fDebugMode)
      {
        gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"ERROR Shower lenght %i %s",length,"(should be 2 )");
      }
    }
  } 
  return r;
}

Int_t HMatchUUnpacker::decodeShowerOld(UInt_t *pData,Int_t maxLength) {
  //Return the number of bytes in the Shower subEvent or
  //-1 for error. pData is a pointer to de subevent's data
  //Each 16bit word goes to a 32bit word.

  //   Int_t length=pData[0]; 
  //   Int_t r=length;
  //   UInt_t *pDataInit= pData;
  //   UInt_t *ptr= pData;
  //   Int_t col,rowPat;
  //   Int_t showBuildId=0;

  //   if (fDebugMode) printf(" decodeShower  lenght : %x %i maxL:%i \n",
  //                            length, length,maxLength);
  //   if ( (length > maxLength) && pData[1] != 0x1111) {
  //     Warning("decodeShower","Inconsistency detected");
  //     r=-1;
  //   } else {

  //     for (;ptr < pDataInit+length;ptr++ ){
  //       // test for the # trailers (debug mode)

  //        switch ( *ptr ){
  //         case 0x2:
  // 	  if (fDebugMode) {printf(" status flag %x \n", *ptr);   
  //                            printf(" status %x \n", *(++ptr));
  //           }else{
  //           }
  //              break;
  //         case 0x3: 
  // 	 if (fDebugMode){ printf(" buildID flag %x \n", *ptr);
  //                           printf(" decode buildID  %i \n",
  //                                  getShowBuildID(++ptr ));
  //          }else{
  // 	   showBuildId = getShowBuildId(++ptr);
  //          }
  //              break;
  //         case 0x4: 
  // 	 if (fDebugMode) {printf(" Row flag %x \n", *ptr);
  // 	                  printf(" Row PAttern %x \n",*(++ptr));
  // 	 }else{
  //            rowPat=getShowRowPattern(++ptr);
  //          }
  //              break;
  //         case 0x5:
  //          if (fDebugMode) {printf(" Col flag %x \n", *ptr);
  //                           printf(" Col %x \n", *(++ptr));
  // 	 }else{
  //            col = getShowColumn(++ptr);
  //            storeShowElements(showBuildId,rowPat,col);
  // 	 }
  //          break;


  // 	   //shows frame trailer
  //         case 0x6:
  //                  if (fDebugMode){ 
  //                           printf(" f counter flag %x \n", *ptr);
  //                           printf(" f counter %x \n", *(++ptr));
  //                  } 
  //              break;
  //         case 0x7:
  //                   if (fDebugMode){
  // 	                  printf(" f marker flag %x \n", *ptr);   
  // 	                  printf(" f marker %x \n", *(++ptr));
  // 		  }
  //              break;
  // 	}

  //      }//!switch
  //     }
  return 0;
}

Int_t HMatchUUnpacker::mapAngles(void)
{
  // Map Angles of Leptons.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Return o always!
  HMULeptons    *pMULept=NULL;
  HMatchURich   *pMURich=NULL;
  HMatchUShower *pMUShower=NULL;
  HMatchUTof    *pMUTof=NULL;

  Int_t offset[6];
  Int_t sec = 0;
  offset[0]=0;
  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Map Angle Rich Sec: 0  hits: %i %s %i",hitpersec[0],"offset:",offset[0]);
  }
  for (sec=1;sec<6; sec++)
  {
    offset[sec]= offset[sec-1] + hitpersec[sec-1];
    if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Map Angle Rich Sec: %i %s %i %s %i",sec,"hits:",hitpersec[sec],"offset:",offset[sec]); 
    }
  }
  iterLepton->Reset();
  while (( pMULept = (HMULeptons *)iterLepton->Next()) != 0)
  {
    sec= pMULept->getSector();
    pMURich = (HMatchURich*) fCatRich->getObject(mapNrRich[sec][pMULept->getRichNr()]+offset[sec]);
    if (pMURich)
    {
      if (fDebugMode)
      {
        gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Rich Nr %i %s %i",pMULept->getRichNr()+offset[sec],"Offset:",offset[sec]);
        gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"Rich theta %i %s %i",pMURich->getTheta(),"Rich phi",pMURich->getPhi());
      }
      pMULept->setThetaRich(pMURich->getTheta());
      pMULept->setPhiRich(pMURich->getPhi());
    }
    // mapping meta angles
    if (fDebugMode)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"sec %i %s %i %s i %s %i %s %i",sec,"meta nr",pMULept->getMetaNr(),
        "det bit",pMULept->getDetBit(),"sh hits",nShower[sec],"tof hits",nTof[sec]);
    }
    if ( ( ( pMULept->getDetBit()==1 ) && ( pMULept->getMetaNr() <= nShower[sec] ) ) ||
    ( ( pMULept->getDetBit()==0 ) && ( pMULept->getMetaNr() <= nTof[sec]    ) ) )
    {
      if (pMULept->getDetBit()==1)
      {
        pMUShower = (HMatchUShower*)fCatShower->getObject(mapNrShower[sec][pMULept->getMetaNr()]);
        if (pMUShower)
        {
          if (fDebugMode)
          {
            gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"sho tot Nr %i %s %i",mapNrShower[sec][pMULept->getMetaNr()],
              "detbit:",pMULept->getDetBit());
          }
          pMULept->setThetaMeta(pMUShower->getTheta());
          pMULept->setPhiMeta(pMUShower->getPhi());
          if (fDebugMode)
          {
            gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MetaS theta %i %s %i",pMUShower->getTheta(),
              " MetaS phi",pMUShower->getPhi());
          }
        }
      }
      else
      {
        if (pMULept->getDetBit()==0)
        {
          pMUTof = (HMatchUTof*)
          fCatTof->getObject(mapNrTof[sec][pMULept->getMetaNr()]);
          if (pMUTof)
          {
            pMULept->setThetaMeta(pMUTof->getTheta());
            pMULept->setPhiMeta(pMUTof->getPhi());
            if (fDebugMode) 
            {
              gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"MetaT theta %i %s %i",pMUTof->getTheta(),
                "MetaT phi",pMUTof->getPhi());
            }
          }
        }
      }
    }
    else
    {
      INFO_msg(10,HMessageMgr::DET_TRIGGER,"Bug in Meta Number!!!!!!");
      continue;
    }
  }
  if (fDebugMode)
  {
    INFO_msg(10,HMessageMgr::DET_TRIGGER,"end of the mapping");
  }
  return 0;
}
//-----------------------------------------------------------------------------------------------

Int_t HMatchUUnpacker::execute(void)
{
  // Execute function of the unpacker. Called for each event.
  //
  // Input Parameters:
  //   none
  //
  // Output parameters:
  //  none
  //
  // Return code:
  //   0 in case of success, -1 else.
  evt++;
  if (fDebugMode)
  {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"*************************** event %i $s", evt,"****************");
  }

  tofNr=tofLep=0;
  showerNr=showerLep=0;

  for (Int_t sec=0;sec<6;sec++)
  {
    hitpersec[sec]=0;
    nShower[sec]=0;
    nTof[sec]=0;
    for (int i=0; i<100; i++)
    {
      mapNrTof[sec][i]=0;
      mapNrShower[sec][i]=0;
      mapNrRich[sec][i]=0;
    }
  }
  if (pSubEvt)
  {
    Int_t length=pSubEvt->getDataLen();
    if (fDebugMode)
    {
    gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"subevt found  with lenght: %i",length);
    }

    UInt_t * pData=NULL;
    if( isCompressed )
    {
      pData = expansion ( pSubEvt->getData(), length);
    }
    else
    {
      pData=pSubEvt->getData();
    }

    Int_t subSize;
    if ( (subSize=decodeMUData(pData,length)) == -1)
    {
      if (fDebugMode)
      {
        INFO_msg(10,HMessageMgr::DET_TRIGGER,"can not decode MU block");
      }
      return kTRUE;
    }
    length-=subSize;
    pData+=subSize;  
    if ( (subSize=decodeRich(pData,length)) == -1)
    {
      if (fDebugMode)
      {
        INFO_msg(10, HMessageMgr::DET_TRIGGER,"can not decode RICH block");
      }
      if ( fLog == 1 )
      {
        pSubEvt->dumpIt(fMU_logfile);
      }
      return kTRUE;
    }
    length-=subSize;
    pData+=subSize;  
    // skip old event format for shower
    if ( *(pData+1) == 0x1111)
    {
      if (fDebugMode)
      {
        INFO_msg(10,HMessageMgr::DET_TRIGGER,"old event format of shower, skip it."); 
      }
      subSize = 6;
    }
    else
    {
      if (fDebugMode)
      {
        gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName()," subsize:%i",subSize); 
      }
      if ( (subSize=decodeShower(pData,length)) == -1)
      {
        if (fDebugMode)
        {
          INFO_msg(10,HMessageMgr::DET_TRIGGER,"can not decode SHOWER block");  
        }
        return kTRUE;
      }
    }
    length-=subSize;
    pData+=subSize;
    if ( (subSize=decodeTof(pData,length)) == -1)
    {
      if (fDebugMode)
      {
        INFO_msg(10,HMessageMgr::DET_TRIGGER,"can not decode TOF block");
      }
      return kTRUE;
    }
    length-=subSize;
    mapAngles();
  }   //! psubEvt
  return kTRUE;
}

UInt_t*  HMatchUUnpacker::expansion(UInt_t* d, Int_t longueur)
{
  // Expand dataword.
  //
  // Input parameters:
  //   UInt_t *d
  //     Dataword to be expanded
  //   Int_t longueur
  //     Length to which the dataword will be expanded
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Pointer to expanded dataword. Has to be deleted by calling function!
  if (data)
    {
      delete[] data;
    }
  data = new UInt_t[2*longueur];
  memset(data,0,2*longueur*sizeof(UInt_t));
  Int_t j=0;
  for(Int_t i=0;i<longueur;i++)
    {
      data[j]=   d[i] & 0xffff;
      data[j+1]=  d[i] >> 16 & 0xffff;
      j+=2;
    }
  return data;
}

ClassImp(HMatchUUnpacker)
