//*-- Author: Joern Wuestenfeld 04/26/2005
//*-- Modified: Joern Wuestenfeld 11/05/2005
//*-- Version: $Id: htriggersetup.cc,v 1.2 2005-11-01 13:58:27 wuestenf Exp $

#include "htriggersetup.h"
#include "hparamlist.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"
#include "hades.h"

//
// HTriggerSetup
//
// Parameters to configure the trigger analysis..
// Due to hardware design, the address of the RICH IPU, that is coded into the
// dataword, can only be set by a hardware switch, that is not easily
// accessible. Due to recabling needs during variouse beamtimes, the mapping
// changed, which is reflected in this parameter container.
// It also contains the row offset for the rich pad plane, and the version of the
// ring finder code that was used for the trigger.
//
// The parameters can be accessed using the following functions:
// TArrayI *getMap(void)
// Int_t    getRowOffset(void)
// Int_t    getRingFinderVersion(void)
//
// void setMap(Int_t *m)
//   Set the mapping of RICH IPU hardware addresses to sector numbers.
// void setRowOffset(Int_t o)
//   Set the offset between the pad numbers on the rows of the RICH padplane.
// void setRingFinderVersion(Int_t val)
//   Set the version of the ring finder, valid values 1..3.
//

HTriggerSetup::HTriggerSetup(const char* name, const char* title, const char* context)
  : HParCond(name,title,context)
{
  // Default constructor
  //
  // Input parameters:
  //   const char *name
  //      Name of the parameter container
  //   const char *title
  //      Title / description of the container
  //   const char *context
  //      Context(version) of parameters to be read from input
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  richIpuMap = new TArrayI(7);
  clear();
}


HTriggerSetup::~HTriggerSetup(void)
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
  if(richIpuMap)
    {
      delete richIpuMap;
    }
}

Bool_t HTriggerSetup::check(void)
{
  // Check the validity of the parametercontainer.
  // None of the values should be -1, which means uninitialized.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kFALSE if one of the values of the map is -1, kTRUE otherwise.
  for(Int_t i=0;i<richIpuMap->GetSize();i++)
    {
      if(richIpuMap->GetArray()[i] == (Char_t)-1)
        {
          return kFALSE;
        }
    }
  if(richRowOffset == 0) return kFALSE;
	if(richRingFinderVersion == -1) return kFALSE;
	if(muDileptons == -99) return kFALSE;
	if(muLeptons == -99) return kFALSE;
	if(muMaxLeptons == -99) return kFALSE;
	if(muRichHits == -99) return kFALSE;
	if(muRichTotalHits == -99) return kFALSE;
	if(muMetaHits == -99) return kFALSE;
	if(muDataFormat == -99) return kFALSE;
  return kTRUE;
}

void HTriggerSetup::clear(void)
{
  // Resets the contents of the map to -1.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  richIpuMap->Reset(-1);
  richRowOffset = 0;
  richRingFinderVersion = -1;
  muDileptons = -99;
  muLeptons = -99;
  muMaxLeptons = -99;
  muRichHits = -99;
	muRichTotalHits = -99;
  muMetaHits = -99;
	muDataFormat = -99;
}

void HTriggerSetup::printParam(void)
{
  // Print the content of the container.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  SEPERATOR_msg("=",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"Mapping of IPU address to sector");
  for(Int_t i=0;i<7;i++)
    {
      gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s%i%s %i","sector[",i,"]: ",richIpuMap->At(i));
    }
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","Row offset: ",richRowOffset);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","Ring finder version: ",richRingFinderVersion);
  SEPERATOR_msg("-",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"Matchingunit cuts:");
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","Dileptons      : ",muDileptons);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","Leptons        : ",muLeptons);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","max. Leptons   : ",muMaxLeptons);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","RICH hits/sec. : ",muRichHits);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","RICH hits(tot.): ",muRichTotalHits);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","META hits      : ",muMetaHits);
  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i","MU data foramt : ",muDataFormat);
  SEPERATOR_msg("=",60);
}

void HTriggerSetup::putParams(HParamList *l)
{
  // puts all parameters to the parameter list, which is used by the io
  //
  // Input parameters:
  //   HParamlist *l
  //     Pointer to parameter list into which the parameters are written.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  if (!l) return;
  l->add("triggerRichIpuMap",*richIpuMap);
  l->add("triggerRichRowOffset",richRowOffset);
  l->add("triggerRichRignFinderVersion",richRingFinderVersion);
  l->add("muDileptons",muDileptons);
  l->add("muleptons",muLeptons);
  l->add("muMaxLeptons",muMaxLeptons);
  l->add("muRichHits",muRichHits);
	l->add("muRichTotalHits",muRichTotalHits);
  l->add("muMetaHits",muMetaHits);
	l->add("muDataFormat", muDataFormat);
}

Bool_t HTriggerSetup::getParams(HParamList *l)
{
  // gets all parameters from the parameter list, which is used by the io
  //
  // Input parameters:
  //   HParamList *l
  //      Pointer to parameter list from which to read parameters.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kFALSE if no valid parameterlist object passed as input,
  //   or the filling of the parameters failed. Otherwise kTRUE
  if (!l) return kFALSE;
  if (!(l->fill("triggerRichIpuMap",richIpuMap))) return kFALSE;
  if (!(l->fill("triggerRichRowOffset",&richRowOffset))) return kFALSE;
  if (!(l->fill("triggerRichRignFinderVersion",&richRingFinderVersion))) return kFALSE;
  if (!(l->fill("muDileptons",&muDileptons))) return kFALSE;
  if (!(l->fill("muLeptons",&muLeptons))) return kFALSE;
  if (!(l->fill("muMaxLeptons",&muMaxLeptons))) return kFALSE;
  if (!(l->fill("muRichHits",&muRichHits))) return kFALSE;
	if (!(l->fill("muRichTotalHits",&muRichTotalHits))) return kFALSE;
  if (!(l->fill("muMetaHits",&muMetaHits))) return kFALSE;
	if (!(l->fill("muDataFormat",&muDataFormat))) return kFALSE;
  return kTRUE;
}

Bool_t HTriggerSetup::init(HParIo *inp, Int_t *set)
{
  // Initialize parameter container from given input.
  //
  // Input parameters:
  //   HParIo *inp
  //     Input from which to read parameter values.
  //   Int_t *set
  //     Parameter set into which to store the parameters.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   If valid input can be found, returns result of initializatin function
  //   of this input, otherwise kFALSE.
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HTriggerSetup::write(HParIo* output)
{
  // Write content of parameter container to given input.
  //
  // Input parameters:
  //   HParIo *output
  //     Parameter source to witch the parameters should be written.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   If valid parameter source can be found, returns result of the write
  //   function of this source, otherwise kFALSE.
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return -1;
}

ClassImp(HTriggerSetup)
