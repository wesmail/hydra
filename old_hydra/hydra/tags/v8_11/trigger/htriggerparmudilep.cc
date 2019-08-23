//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $Id: htriggerparmudilep.cc,v 1.8 2006-08-12 13:05:57 halo Exp $

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HTriggerParMuDilep
//
// Parameters for TRIGGER Emulation
//
///////////////////////////////////////////////////////////////////////////////
#include "htriggerparmudilep.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include "hmessagemgr.h"

ClassImp(HTriggerParMuDilep)

void HTriggerParMuDilep::setDefaults(void)
{
  // Sets the default values
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  MUcutoff=16;
}

HTriggerParMuDilep::HTriggerParMuDilep(const char* name,const char* title, const char* context)
          : HParCond(name,title,context)
{
  // Default constructor
  //
  // Input parameters:
  //   const char *name
  //     Name of the parametercontainer, default "TriggerParMuDilep"
  //   const char *title
  //     Title of the container, used by ROOT
  //   const chat *context
  //     Parameter context to read parameters for
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  strcpy(detName,"Trigger");
}


HTriggerParMuDilep::~HTriggerParMuDilep(void)
{
  // destructor
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
}

Bool_t HTriggerParMuDilep::init(HParIo* inp,Int_t* set)
{
  // Intitializes the container from an input
  //
  // Input parameters:
  //   HParIo *inp
  //     Input from which to read parameters
  //   Int_t *set
  //     Pointer to parameter set, ito which to store the data
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   If valid input can be found, returns result of initialization function of the
  //   input, kFALSE otherwise.
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HTriggerParMuDilep::write(HParIo* output)
{
  // Writes the container to an output
  //
  // Input parameters:
  //   HParIo *output
  //     Pointer to parameter output
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   If valid output can be found, returns result of the write function of
  //   this output, -1 otherwise.
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HTriggerParMuDilep::putParams(HParamList* l)
{   
  // Puts all parameters to the parameter list, which is used by the io
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
  l->add("MUcutoff",MUcutoff);
}

Bool_t HTriggerParMuDilep::getParams(HParamList* l)
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
  if (!l->fill("MUcutoff",&MUcutoff)) return kFALSE;
  return kTRUE;
}



void HTriggerParMuDilep::clear()
{
  // sets the default values
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}

void HTriggerParMuDilep::readline(const char *buf, Int_t *set)
{
  // Decodes one line read from ascii file I/O
  //
  // Input parameters:
  //   const char *buf
  //      Buffer with data from file
  //   Int_t *set
  //      Pointer of parameter set to which to write parameters
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  Int_t ct;
  sscanf(buf,"%i",&ct);
  setMUcutoff(ct);
}

void HTriggerParMuDilep::putAsciiHeader(TString& header)
{
  // Puts the ascii header to the string used in HTriggerParAsciiFileIo
  //
  // Input parameters:
  //   TString &header
  //     Reference of string which should contain the header
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  header=
    "# Parameters for the MU Dilepton emulation\n"
    "# Format:\n"
    "# MUcutoff\n";
}

// Bool_t HTriggerParMuDilep::writeline(char *buf,Int_t s, Int_t m, Int_t c) {
//   // writes one line to the buffer used by ascii file I/O
//   sprintf(buf,"%i\n",
//               getMUcutoff());
//   return kTRUE;
// }

void HTriggerParMuDilep::printParam(void)
{
  // Print the parameters
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"\n Parameters for the Mu Dilepton emulation\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," Format:\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," MUcutoff\n");
  SEPERATOR_msg("=",60);

  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),
                        "TRIGGER PARAMETERS for the MU Dilepton Emulation MUcutoff   = %i\n",
                        MUcutoff);
  SEPERATOR_msg("=",60);
}
