//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

///////////////////////////////////////////////////////////////////////////////
// HRichIPUParPattern
//
// Parameters for RICH IPU Pattern
//
///////////////////////////////////////////////////////////////////////////////
#include "hrichIPUparpattern.h"
#include "hades.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"

ClassImp(HRichIPUParPattern)

void HRichIPUParPattern::setDefaults(void)
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
  //   none
  for (Int_t xx=0; xx < 29; xx++)
    {
      for (Int_t yy=0; yy < 5; yy++)
        {
          for (Int_t zz=0; zz < 2; zz++)
            {
              pattern[xx][yy][zz]=0;
            }
        }
    }
}


HRichIPUParPattern::HRichIPUParPattern(const char* name,const char* title, const char* context)
          : HParSet(name,title,context)
{
  // Default constructor
  // The container name is set to "RichIPUParPattern"
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


HRichIPUParPattern::~HRichIPUParPattern(void)
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

Bool_t HRichIPUParPattern::init(HParIo* inp,Int_t* set)
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
  ERROR_msg(HMessageMgr::DET_TRIGGER,"Did not get input >HTriggerParIo<");
  return kFALSE;
}

Int_t HRichIPUParPattern::write(HParIo* output)
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

void HRichIPUParPattern::clear(void)
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


void HRichIPUParPattern::readline(const char *buf, Int_t *set)
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
  Int_t gr,pg,re;
  Int_t pat;

  pat = -111; // dummy value
  sscanf(buf,"%i%i%i%i",&gr,&pg,&re,&pat);

  if (pat == -111)
    {
      setNGroups(gr);
      setNPadsinGroup(pg);
      setNPadDimensions(re);
    }
  else
    {
      if( (gr > -1) && (gr <  getNGroups() )
          && (pg > -1) && (pg < getNPadsinGroup() )
          && (re > -1) && (re < getNPadDimensions() ) )
        {
          if(gr<29||pg<5||re<2)
            {
              setPattern(gr,pg,re,pat);
            }
          else
            {
              ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichIPUParPattern::readline():: array out of bounds! skipp!");
              return;
            }

        }
      else
        {
          ERROR_msg(HMessageMgr::DET_TRIGGER,"variables not properly set");
        }
    }
}



void HRichIPUParPattern::putAsciiHeader(TString& header)
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
    "# Parameters for RICH IPU Pattern\n"
    "# Format:\n"
    "# number_of_groups number_of_pads_in_group number_of_regions\n"
    "# group  pad  region  pattern\n";
}

Bool_t HRichIPUParPattern::writeline(char *buf,Int_t s, Int_t m, Int_t c)
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
  if(s>=29||m>=5||c>=2)
    {
      ERROR_msg(HMessageMgr::DET_TRIGGER,"HRichIPUParPattern::writeline():: array out of bounds! skipp!");
      return kFALSE;
    }
  sprintf(buf,"%i %i %i %i\n",s,m,c,getPattern(s,m,c));
  return kTRUE;
}

Bool_t HRichIPUParPattern::writesize(char *buf)
{
  // Writes one line to the buffer used by ascii file I/O
  //
  // Input Parameters:
  //   none
  //
  // Output parametrs:
  //   char *buf
  //     Buffer into which to write
  //
  // Return code:
  //   Returns allways kTRUE.
  sprintf(buf,"%i %i %i\n",getNGroups(),getNPadsinGroup(),getNPadDimensions());
  return kTRUE;
}

void HRichIPUParPattern::printParam(void)
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
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"Parameters for RICH IPU Pattern");
  INFO_msg(10,HMessageMgr::DET_TRIGGER,"Format:");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," number_of_groups number_of_pads_in_group number_of_regions\n");
  INFO_msg(10,HMessageMgr::DET_TRIGGER," group  pad  region  pattern\n");
  SEPERATOR_msg("=",60);

  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),
                        "%s %d %s %d %s %d","number of groups",
                        getNGroups(), "number of pads in group",
                        getNPadsinGroup(),"number of regions",getNPadDimensions());
  for (Int_t xx=0; xx < getNGroups(); xx++)
    {
      for (Int_t yy=0; yy < getNPadsinGroup(); yy++)
        {
          for (Int_t zz=0; zz< getNPadDimensions(); zz++)
            {
              if(xx<29||yy<5||zz<2)
                {
                  gHades->getMsg()->info(10,HMessageMgr::DET_TRIGGER,GetName(),
                                          "%d   %d   %d   %d",xx, yy, zz, getPattern(xx,yy,zz));;
                }
              else
                {
                  ERROR_msg(HMessageMgr::DET_TRIGGER,"printParam():: array out of bounds! skipp!");
                }
            }
        }
    }
  SEPERATOR_msg("=",60);
}
