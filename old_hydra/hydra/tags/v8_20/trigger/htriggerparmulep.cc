//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $Id: htriggerparmulep.cc,v 1.15 2008-05-09 16:18:42 halo Exp $

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HTriggerParMuLep
//
// Parameters for MU Lepton Emulation
//
///////////////////////////////////////////////////////////////////////////////
#include "htriggerparmulep.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h" 
#include "hmessagemgr.h"

ClassImp(HTriggerParMuLep)


void HTriggerParMuLep::setDefaults(void)
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
  MUtheta=255;
  MUphi_offset=34;
  MUphi_slope=4;
  MUfield=500;
  MUtof=20.0;
}

HTriggerParMuLep::HTriggerParMuLep(const Char_t* name,const Char_t* title, const Char_t* context)
          : HParCond(name,title,context)
{
  // Default constructor
  //
  // Input parameters:
  //   const Char_t *name
  //     Name of the parametercontainer, default "TriggerParMuLep"
  //   const Char_t *title
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


HTriggerParMuLep::~HTriggerParMuLep(void)
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
}

Bool_t HTriggerParMuLep::init(HParIo* inp,Int_t* set)
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

Int_t HTriggerParMuLep::write(HParIo* output)
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


void HTriggerParMuLep::putParams(HParamList* l)
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
  l->add("MUtheta",MUtheta);
  l->add("MUphi_offset",MUphi_offset); 
  l->add("MUphi_slope",MUphi_slope);
  l->add("MUfield",MUfield); 
  l->add("MUtof",MUtof);
}

Bool_t HTriggerParMuLep::getParams(HParamList* l)
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
  if (!l->fill("MUtheta",&MUtheta)) return kFALSE;
  if (!l->fill("MUphi_offset",&MUphi_offset)) return kFALSE;
  if (!l->fill("MUphi_slope",&MUphi_slope)) return kFALSE;
  if (!l->fill("MUfield",&MUfield)) return kFALSE;
  if (!l->fill("MUtof",&MUtof)) return kFALSE;
  return kTRUE;
}


void HTriggerParMuLep::clear()
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
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}

void HTriggerParMuLep::readline(const Char_t *buf, Int_t *set)
{
  // Decodes one line read from ascii file I/O
  //
  // Input parameters:
  //   const Char_t *buf
  //      Buffer with data from file
  //   Int_t *set
  //      Pointer of parameter set to which to write parameters
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   none
  Int_t th,phO,phS,ff;
  Float_t tof;
  sscanf(buf,"%i%i%i%i%f",&th,&phO,&phS,&ff,&tof);
  setMUtheta(th);
  setMUphiOffset(phO);
  setMUphiSlope(phS);
  setMUfield(ff);
  setMUtof(tof);
}

void HTriggerParMuLep::putAsciiHeader(TString& header)
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
    "# Parameters for the MU Lepton emulation\n"
    "# Format:\n"
    "# MUtheta  MUphi_offset  MUphi_slope  MUfield  MUtof\n";
}


void HTriggerParMuLep::printParam(void)
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
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"\n Parameters for the MU Lepton emulation\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," Format:\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," MUtheta  MUphi_offset  MUphi_slope  MUfield  MUtof\n");
  SEPERATOR_msg("=",80);

  gHades->getMsg()->info(10, HMessageMgr::DET_TRIGGER,GetName(),
                          "   %i          %i           %i        %i    %2.2f\n",
                          MUtheta,
                          MUphi_offset,
                          MUphi_slope,
                          MUfield,
                          MUtof);
  SEPERATOR_msg("=",80);
}
