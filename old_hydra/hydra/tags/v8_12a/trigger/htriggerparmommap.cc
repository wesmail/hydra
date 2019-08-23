//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/11/2005
//*-- Version: $Id: htriggerparmommap.cc,v 1.11 2006-08-12 13:05:57 halo Exp $

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HTriggerParMomMap
//
// Parameters for TRIGGER Emulation
//
///////////////////////////////////////////////////////////////////////////////
#include "htriggerparmommap.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "htriggerdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"

ClassImp(HTriggerParMomMap)

void HTriggerParMomMap::setDefaults(void)
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
  for (Int_t xx=0; xx < 63; xx++)
    {
      for (Int_t yy=0; yy < 63; yy++)
        {
          for (Int_t zz=0; zz < 7; zz++)
            {
              mu_mom[xx][yy][zz]=0;
            }
        }
    }
}


HTriggerParMomMap::HTriggerParMomMap(const char* name,const char* title, const char* context)
          : HParSet(name,title,context)
{
  //Default constructor
  ////
  // Input parameters:
  //   const char *name
  //     Name of the parametercontainer, default "TriggerParMuMomMap"
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


HTriggerParMomMap::~HTriggerParMomMap(void)
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

Bool_t HTriggerParMomMap::init(HParIo* inp,Int_t* set)
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

Int_t HTriggerParMomMap::write(HParIo* output)
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

void HTriggerParMomMap::clear()
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


void HTriggerParMomMap::readline(const char *buf, Int_t *set)
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
  Int_t th,ph,Delta_th;
  Int_t theta,phi,Delta_theta;
  Float_t mom;


  mom = -111; // dummy value
  sscanf(buf,"%i%i%i%f",&th,&Delta_th,&ph,&mom);

  if (mom == -111)
    {
      theta =th;
      phi=ph;
      Delta_theta=th;
      setTheta_bins(th);
      setDeltatheta_bins(Delta_th);
      setPhi_bins(ph);
      gHades->getMsg()->debug(1,HMessageMgr::DET_TRIGGER,GetName(),"%s %i %s %i %s %i",
                            "READLINE theta bins",th,"delta theta bins",Delta_th,"phi bins",ph);
    }
  else
    {
      if(getTheta_bins()>63||getDeltatheta_bins()>63||getPhi_bins()>7)
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"readline():: array out of bounds! skipp!");
          return;
        }
      if( (Delta_th > -1) && (Delta_th <  getTheta_bins() )
          && (th > -1) && (th < getDeltatheta_bins() ) 
          && (ph > -1) && (ph < getPhi_bins() ) )
        {
          setMom(th,Delta_th,ph,mom);
        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"variables not properly set");
        }
    }
}

void HTriggerParMomMap::putAsciiHeader(TString& header)
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
    "# Parameters for the MU Momentum Mapping\n"
    "# Format:\n"
    "# theta bins  Delta_theta bins  phi bins\n"
    "# theta       Delta_theta       phi       momentum\n";
}

Bool_t HTriggerParMomMap::writeline(char *buf,Int_t s, Int_t m, Int_t c)
{
  // Writes one line to the buffer used by ascii file I/O
  //
  // Input Parameters:
  //    char *buf
  //      Pointer to buffer to which to write the parameters
  //    Int_t s
  //
  //    Int_t m
  //
  //    Int_t c
  //
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Returns kFALSE if s > 63 or m > 63 or c > 7, otherwise kTRUE
    if(s>63||m>63||c>7)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"writeline():: array out of bounds! skipp!");
      return kFALSE;
    }
    sprintf(buf,"%i %i %i %f\n",s,m,c,getMom(s,m,c));
    return kTRUE;
}

Bool_t HTriggerParMomMap::writesize(char *buf)
{
  // writes one line to the buffer used by ascii file I/O
  //
  // Input Parameters:
  //    char *buf
  //      Pointer to buffer to which to write the parameters
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   kTRUE always.
  sprintf(buf,"%i %i %i\n",getTheta_bins(),getDeltatheta_bins(),getPhi_bins());	
  return kTRUE;
}

void HTriggerParMomMap::printParam(void)
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
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"\n Parameters for the MU Momentun Mapping\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," Format:\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," theta bins  Delta_theta bins  phi bins\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," theta       Delta_theta       phi       momentum\n");
  SEPERATOR_msg("=",60);

  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%s %i %s %i %s %i",
                        "PRINTPARAM theta bins",getTheta_bins(),"delta theta bins",getDeltatheta_bins(),
                        "phi bins",getPhi_bins());

  for (Int_t xx=0; xx < getTheta_bins(); xx++)
    {
      for (Int_t yy=0; yy< getDeltatheta_bins(); yy++)
        {
          for (Int_t zz=0; zz< getPhi_bins(); zz++)
            {
              if(xx>63||yy>63||zz>7)
                {
                  ERROR_msg(HMessageMgr::DET_TRIGGER,"printParam():: array out of bounds! skipp!");
                  continue;
                }
              gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),"%i %s%i%s%i%s%f",
                                      xx,"   ",yy,"   ",zz,"   ",getMom(xx,yy,zz));
            }
        }
    }
  SEPERATOR_msg("=",60);
}
