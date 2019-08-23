//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

///////////////////////////////////////////////////////////////////////////////
// HRichIPUParThresholds
//
// Parameters for RICH IPU Thresholds
//
///////////////////////////////////////////////////////////////////////////////
#include "hrichIPUparthresholds.h"
#include "hades.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"

ClassImp(HRichIPUParThresholds)

void HRichIPUParThresholds::setDefaults(void)
{
  // Set the default values
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   none  for (Int_t xx=0; xx < 2; xx++)
  for (Int_t xx=0; xx < 2; xx++)
    {
      for (Int_t yy=0; yy < 6; yy++)
        {
          thresholds[xx][yy]=0;
        }
    }
}


HRichIPUParThresholds::HRichIPUParThresholds(const char* name,const char* title, const char* context)
          : HParSet(name,title,context)
{
  // Default constructor
  // The container name is set to "RichIPUParThreshold"
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   none
  strcpy(detName,"Trigger");
}


HRichIPUParThresholds::~HRichIPUParThresholds(void)
{
  // Destructor
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   none
}

Bool_t HRichIPUParThresholds::init(HParIo* inp,Int_t* set)
{
  // Intitializes the container from an input
  //
  // Input Parameters:
  //   HParIo *inp
  //     Input from which to initialize
  //   Int_t *set
  //     Pointer for parameter set in which to store the parameters.
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   Returns the result result of the initalization of the input, or
  //   kFALSE if no input found.
  HDetParIo* input=inp->getDetParIo("HTriggerParIo");
  if (input) return (input->init(this,set));
  ERROR_msg(HMessageMgr::DET_TRIGGER,"Did not get input >HTriggerParIo< " );
  return kFALSE;
}

Int_t HRichIPUParThresholds::write(HParIo* output)
{
  // Writes the container to an output
  //
  // Input Parameters:
  //   HParIo *output
  //     Pointer to Io into which to write the parameters.
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   Returns result of write function of Io, or -1 if no Io found.
  HDetParIo* out=output->getDetParIo("HTriggerParIo");
  if (out) return out->write(this);
  return -1;
}

void HRichIPUParThresholds::clear()
{
  // Sets the default values
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   none
  setDefaults();
  status=kFALSE;
  resetInputVersions();
}


void HRichIPUParThresholds::readline(const char *buf, Int_t *set)
{
  // Decodes one line read from ascii file I/O
  //
  // Input Parameters:
  //   const char *buf
  //     String which to analyze.
  //   Int_t *set
  //     Unused!
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   none
  Int_t sec,re;
  Int_t thr;


  sscanf(buf,"%i%i%i",&re,&sec,&thr);

  if( (re >= 0) && (re <= 1 )
  && (sec>= 0) && (sec<= 5 ) )
    {
      setThresholds(re,sec,thr);
    }
  else 
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"variables not properly set");
    }
}



void HRichIPUParThresholds::putAsciiHeader(TString& header)
{
  // Puts the ascii header to the string used in HRichIPUParAsciiFileIo
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   TString &header
  //     Buffer into which to store the header.
  //
  // Return code:
  //   none
  header=
    "# Parameters for RICH IPU Thresholds\n"
    "# Format:\n"
    "# region sector  threshold\n";
}

Bool_t HRichIPUParThresholds::writeline(char *buf,Int_t s, Int_t m)
{
  // Writes one line to the buffer used by ascii file I/O
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   char *buf
  //     String into which to write the formated parameters.
  //   Int_t s
  //     Sector for which to write the parameter.
  //   Int_t m
  //     Module for which to write the parameter.
  //   Int_t c
  //     Cell for which to write the parameter.
  //
  // Return code:
  //   Returns kFALSE if s,m and c are out of bounds, otherwise kTRUE.
  sprintf(buf,"%i %i %i\n",s,m,getThresholds(s,m));	
  return kTRUE;
}

void HRichIPUParThresholds::printParam(void)
{
  // Prints the parameters
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   none
  //
  // Return code:
  //   none
  SEPERATOR_msg("=",60);
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"\n Parameters for RICH IPU Thresholds\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"Format:\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"region  sector  threshold\n");
  SEPERATOR_msg("=",60);

  for (Int_t xx=0; xx < 2; xx++)
    {
      for (Int_t yy=0; yy < 6; yy++)
        {
          gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),
                                  "%d   %d   %d",xx, yy, getThresholds(xx,yy));
        }
    }
  SEPERATOR_msg("=",60);
}
