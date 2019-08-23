//*-- Author : Joern Wuestenfeld
//*-- Modified : 15/04/2002 by Joern Wuestenfeld
//*-- Modified : 04/04/2002 by Joern Wuestenfeld
//*-- Modified : 28/03/2002 by Joern Wuestenfeld

/////////////////////////////////////////
//
//  HMessageMgr
//
//  This class manages three different outputs for all kind of messages.
//  Besides the standart output, which is the default, one can also
//  write the messages to a file, or via a socket to a remote logging process.
//
//  The usage of this class should be done via the four macros that are defined
//  in the hmessagemgr.h file.
//  DEBUG_msg(level, det, text)
//  ERROR_msg(det, text)
//  WARNING_msg(level, det, text)
//  INFO_msg(level, det, text)
//
//  The meaning of the parameters is:
//  level - level of this message, never set it to zero because this means 
//          no message at all will be printed
//  det   - detector which sends this message, can be one of:
//          DET_RICH DET_START DET_MDC DET_TOF DET_TOFINO DET_SHOWER
//          DET_TRIGGER
//  text  - text to print
//
/////////////////////////////////////////////////////

#include "TSystem.h"
#include "hmessagemgr.h"
#include <iostream.h>

ClassImp(HMessageMgr)

HMessageMgr::HMessageMgr(const char *name, const char *title)
			 : TNamed(name,title)
{
  // This is the default constructor inherited from TNamed.
  // Sets default logging to stdout and stderr.
  // The default levels are: no debugging, but all of error, warning and info.

  msgFile = NULL;
  errorFile = NULL;
  msgSocket = NULL;
  errorSocket = NULL;
  
  debugLevel = 0;
  warningLevel = LEVELMAX;
  infoLevel = LEVELMAX;
  detectorMsg = DET_ALL;
  line = 0;
  file = (char *)calloc(1,80);
}

HMessageMgr::~HMessageMgr(void)
{
  // This is the default destructor
  setDefault();
  free(file);
}

Bool_t HMessageMgr::setDefault(void)
{
  // Resets the message logging to ist default. Closses all loggfiles ore
  // open sockets, and loggs further messages to stdout and stderr.
  // Also the loglevels are set to the same values as in the constructor.

  if(msgFile)
    {
      fclose(msgFile);
      msgFile = NULL;
    }
  if(errorFile)
    {
      fclose(errorFile);
      errorFile = NULL;
    }
  if(msgSocket)
    {
      msgSocket->Close();
      delete msgSocket;
      msgSocket = NULL;
    }
  if(errorSocket)
    {
      errorSocket->Close();
      delete errorSocket;
      errorSocket = NULL;
    }

  debugLevel = 0;
  warningLevel = LEVELMAX;
  infoLevel = LEVELMAX;
  detectorMsg = DET_ALL;

  return kTRUE;
}

Bool_t HMessageMgr::setFile(const char *dir)
{
  // Sets the logfile.
  // Input is the directory to where to log to.
  // The filename is derived from the name of this object, that is given
  // when creating it.
  // Two files are created. One with extension .msg for all messages of
  // type info, warning and debug.
  // The other one with extension .err fo the error messages.

  char *fname;

  fname = (char *)calloc(1,200);
  if(msgFile)
    {
      fclose(msgFile);
      msgFile = NULL;
    }
  if(errorFile)
    {
      fclose(errorFile);
      errorFile  = NULL;
    }
  if(!msgFile)
    {
      if((UInt_t)strlen(dir) > (UInt_t)(200 - fName.Length() + 5))
	{
	  free(fname);
	  return kFALSE;
	}
      else
	{
	  sprintf(fname,"%s%s%s",dir,fName.Data(),".msg");
	}
      msgFile = fopen(fname,"w");
      if(!msgFile)
	{
	  free(fname);
	  return kFALSE;
	}
    }
  else
    {
      free(fname);
      return kFALSE;
    }
  if(!errorFile)
    {
      if((UInt_t )strlen(dir) > (UInt_t)(200 - fName.Length() + 5))
	{
	  free(fname);
	  return kFALSE;
	}
      else
	{
	  sprintf(fname,"%s%s%s",dir,fName.Data(),".err");
	}
      errorFile = fopen(fname,"w");
      if(!errorFile)
	{
	  free(fname);
	  if(msgFile)
	    {
	      fclose(msgFile);
	      msgFile = NULL;
	    }
	  return kFALSE;
	}
    }
  else
    {
      free(fname);
      return kFALSE;
    }

  return kTRUE;
}

Bool_t HMessageMgr::setPort(const char *host, Int_t port)
{
  // Opens two socket connections to a logging server on the specified host.
  // The passed port is used for the message logging. Errors are send to
  // the portaddress port + 10.
  // eventually opened logfiles are closed, if the connection could be opened.
  // Otherwise the current logging stays active.

  msgSocket = new TSocket(host,port);
  errorSocket = new TSocket(host,port+10);
  thisHost = new TString(gSystem->HostName());
  if(msgSocket->IsValid() && errorSocket->IsValid())
    {
      if(msgFile)
	{
	  fclose(msgFile);
	  msgFile = NULL;
	}
      if(errorFile)
	{
	  fclose(errorFile);
	  errorFile = NULL;
	}
      msgSocket->Send(thisHost->Data());
      errorSocket->Send(thisHost->Data());
      return kTRUE;
    }
  else
    {
      delete msgSocket;
      delete errorSocket;
      msgSocket = NULL;
      errorSocket = NULL;
      return kFALSE;
    }
}

Bool_t HMessageMgr::switchOff(void)
{
  // Switches of the logging completly by setting the output to /dev/null.
  // Could probably done also by redirecting stdout and stderr to /dev/null.
  // Currently opened files or sockets are closed.

  char *fname;

  fname = (char *)calloc(1,200);
  setDefault();
  if(msgFile)
    {
      fclose(msgFile);
      msgFile = NULL;
    }
  if(errorFile)
    {
      fclose(errorFile);
      errorFile  = NULL;
    }
  if(!msgFile)
    {
      sprintf(fname,"%s","/dev/null");
    }
  msgFile = fopen(fname,"w");
  if(!msgFile)
    {
      free(fname);
      return kFALSE;
    }
  else
    {
      free(fname);
      return kFALSE;
    }
  if(!errorFile)
    {
      sprintf(fname,"%s","/dev/null");
    }
  errorFile = fopen(fname,"w");
  if(!errorFile)
    {
      free(fname);
      return kFALSE;
    }
  else
    {
      free(fname);
      return kFALSE;
    }

  return kTRUE;
}

void HMessageMgr::setDebugLevel(Char_t level)
{
  // Sets the level of debug messages to level.

  if(level > 10)
    debugLevel = 10;
  else
    if(level <= 0)
      debugLevel = 1;
  else
    debugLevel = level;
}

void HMessageMgr::setInfoLevel(Char_t level)
{
  //Sets the level of info messages to level.

  if(level > 1)
    infoLevel = 10;
  else
    if(level <= 0)
      infoLevel = 1;
  else
    infoLevel = level;
}

void HMessageMgr::setWarningLevel(Char_t level)
{
  // Sets the level of warning messages to level.

  if(level > 10)
    warningLevel = 10;
  else
    if(level <= 0)
      warningLevel = 1;
  else
    warningLevel = level;
}

void HMessageMgr::enableDetectorMsg(Int_t det)
{
  // Selects for which detector messages should be displayed.
  // det is a or'd pattern of the DET_* constants.

  detectorMsg = det;
}

void HMessageMgr::messageSeperator(Char_t *seperator, Int_t num)
{
  // This function prints a line with character seperator.
  // The seperator character is repeated num times on this line.

  int i;

  msg1 = (char *)calloc(1,200);
  for(i=0;i<num;i++)
    {
      strcat(msg1,seperator);
    }
  write(msg1);
  free(msg1);
}

void HMessageMgr::hydraErrorHandler(int level, Bool_t abort, const char *location, const char *msg)
{
  // Message handler for ROOT. Can be installed via gSystem->SetErrorHandler()
  // Used by all ROOT classes to do message logging.
  // Gives a common message layout.
 
  if (level < gErrorIgnoreLevel)
      return;

  if((level > kError) | (level > kSysError) | (level > kFatal))
      {
	gHades->getMsg()->error(10,DET_ALL,location,msg);
      }
  else
    if(level > kWarning)
      {
	gHades->getMsg()->warning(10,DET_ALL,location,msg);
      }
    else
      if(level > kInfo)
	{
	  gHades->getMsg()->info(10,DET_ALL,location,msg);
	}
}

void HMessageMgr::debug(Char_t level, Int_t det, const char *className, const char *text)
{
  // This function should never be called directly, use the DEBUG macro instead.
  // This function writes a debug message to the output.
  // It looks like this example:
  // debug  <Hades          >:No input specified

  if(level <= 0)
    level = 1;
  if((level <= debugLevel) && (detectorMsg & det))
    {
      msg1 = (char*)calloc(1,strlen((char *)text)+30);
      if(msg1)
	{
	  if(line == 0)
	    sprintf(msg1,"%7s%s%15s%s%s","debug","<",className,">:",text);
	  else
	    sprintf(msg1,"%7s%s%15s %s%s%d%s%s","debug","<",className,file,":",line,">:",text);
	  write(msg1);
	  free(msg1);
	}
    }
}

void HMessageMgr::error(Char_t level, Int_t det, const char *className, const char *text)
{
  // This function should never be called directly, use the ERROR macro instead.
  // This function writes a error message to the output.
  // It looks like this example:
  // error  <Hades          >:No input specified

  if(level <= 0)
    level = 1;
  if(detectorMsg & det)
    {
      msg1 = (char*)calloc(1,strlen((char *)text)+30);
      if(msg1)
	{
	  sprintf(msg1,"%7s%s%15s%s%s","error","<",className,">:",text);
	  ewrite(msg1);
	  free(msg1);
	}
    }
}

void HMessageMgr::warning(Char_t level, Int_t det, const char *className, const char *text)
{
  // This function should never be called directly, use the WARNING macro instead.
  // This function writes a warning message to the output.
  // It looks like this example:
  // warning<Hades          >:No input specified

  if(level <= 0)
    level = 1;
  if((level <= warningLevel)  && (detectorMsg & det))
    {
      msg1 = (char*)calloc(1,strlen((char *)text)+30);
      if(msg1)
	{
	  sprintf(msg1,"%7s%s%15s%s%s","warning","<",className,">:",text);
	  write(msg1);
	  free(msg1);
	}
    }
}

void HMessageMgr::info(Char_t level, Int_t det, const char *className, const char *text)
{
  // This function should never be called directly, use the INFO macro instead.
  // This function writes a info message to the output.
  // It looks like this example:
  // info<         Hades>:No input specified

  if(level <= 0)
    level = 1;
  if((level <= infoLevel)  && (detectorMsg & det))
    {
      msg1 = (char*)calloc(1,strlen((char *)text)+30);
      if(msg1)
	{
	  sprintf(msg1,"%7s%s%15s%s%s","info","<",className,">:",text);
	  write(msg1);
	  free(msg1);
	}
    }
}

void HMessageMgr::debug(Int_t level, Int_t det, const char *className, const char *text, ...)
{
  // This function put out an error message including some variable arguments.
  // The text parameter takes the standart parameters as in printf().
  // The output looks like:
  // debug<         Hades>:No input specified

  va_list ap;

  if(level <= 0)
    level = 1;
  if((level <= debugLevel)  && (detectorMsg & det))
    {
      msg1 = (char *)calloc(1,200);
      msg2 = (char *)calloc(1,200);
      if(line == 0)
	sprintf(msg1,"%7s%s%15s%s","debug","<",className,">:");
      else
	sprintf(msg1,"%7s%s%15s %s%s%d%s","debug","<",className,file,":",line,">:");
      va_start(ap,text);
      vsprintf(msg2,text,ap);
      va_end(ap);
      strcat(msg1,msg2);
      write(msg1);
      free(msg1);
      free(msg2);
    }
}

void HMessageMgr::error(Int_t level, Int_t det, const char *className, const char *text, ...)
{
  // This function put out an error message including some variable arguments.
  // The text parameter takes the standart parameters as in printf().
  // The output looks like:
  // error<         Hades>:No input specified

  va_list ap;

  if(level <= 0)
    level = 1;
  if(detectorMsg & det)
    {
      msg1 = (char *)calloc(1,200);
      msg2 = (char *)calloc(1,200);
      sprintf(msg1,"%7s%s%15s%s","error","<",className,">:");
      va_start(ap,text);
      vsprintf(msg2,text,ap);
      va_end(ap);
      strcat(msg1,msg2);
      write(msg1);
      free(msg1);
      free(msg2);
    }
}

void HMessageMgr::warning(Int_t level, Int_t det, const char *className, const char *text, ...)
{
  // This function put out an warning message including some variable arguments.
  // The text parameter takes the standart parameters as in printf().
  // The output looks like:
  // warning<         Hades>:No input specified

  va_list ap;

  if(level <= 0)
    level = 1;
  if((level <= warningLevel)  && (detectorMsg & det))
    {
      msg1 = (char *)calloc(1,200);
      msg2 = (char *)calloc(1,200);
      sprintf(msg1,"%7s%s%15s%s","warning","<",className,">:");
      va_start(ap,text);
      vsprintf(msg2,text,ap);
      va_end(ap);
      strcat(msg1,msg2);
      write(msg1);
      free(msg1);
      free(msg2);
    }
}

void HMessageMgr::info(Int_t level, Int_t det, const char *className, const char *text, ...)
{
  // This function put out an info message including some variable arguments.
  // The text parameter takes the standart parameters as in printf().
  // The output looks like:
  // info<         Hades>:No input specified

  va_list ap;

  if(level <= 0)
    level = 1;
  if((level <= infoLevel)  && (detectorMsg & det))
    {
      msg1 = (char *)calloc(1,200);
      msg2 = (char *)calloc(1,200);
      sprintf(msg1,"%7s%s%15s%s","info","<",className,">:");
      va_start(ap,text);
      vsprintf(msg2,text,ap);
      va_end(ap);
      strcat(msg1,msg2);
      write(msg1);
      free(msg1);
      free(msg2);
    }
}

void HMessageMgr::debug(Int_t level, Int_t det, const char *className, const char *file, Int_t line, const char *text, ...)
{
  // This function put out an error message including some variable arguments.
  // The text parameter takes the standart parameters as in printf().
  // The output looks like:
  // debug<         Hades>:No input specified

  va_list ap;

  if(level <= 0)
    level = 1;
  if((level <= debugLevel)  && (detectorMsg & det))
    {
      msg1 = (char *)calloc(1,200);
      msg2 = (char *)calloc(1,200);
      sprintf(msg1,"%7s%s%15s %s%s%d%s","debug","<",className,file,":",line,">:");
      va_start(ap,text);
      vsprintf(msg2,text,ap);
      va_end(ap);
      strcat(msg1,msg2);
      write(msg1);
      free(msg1);
      free(msg2);
    }
}

void HMessageMgr::write(const char *text)
{
  // Internal function used to write to the correct output.
  // Used for info, warning and debug messages.
  // Adds the name of the host the code is running on, if output is send via
  // socket to a remote logging process.

  Char_t *msg;

  if(msgFile)
    {
      fprintf(msgFile,text);
      fprintf(msgFile,"\n");
    }
  else
    {
      if(msgSocket)
	{
	  msg =(Char_t *)calloc(1,strlen(text) + 20);
	  sprintf(msg,"<%10s>%s",thisHost->Data(),text);
	  msgSocket->Send(msg);
	  free(msg);
	}
      else
	{
	  fprintf(stdout,"%s\n",text);
	}
    }
}

void HMessageMgr::ewrite(const char *text)
{
  // Internal function used to write to the correct output.
  // Used for error messages.
  // Adds the name of the host the code is running on, if output is send via
  // socket to a remote logging process.

  Char_t *msg;

  if(errorFile)
    {
      fprintf(errorFile,text);
      fprintf(errorFile,"\n");
    }
  else
    {
      if(errorSocket)
	{
	  msg =(Char_t *)calloc(1,strlen(text) + 20);
	  sprintf(msg,"<%10s>%s",thisHost->Data(),text);
	  errorSocket->Send(msg);
	  free(msg);
	}
      else
	{
	  fprintf(stderr,"%s\n",text);
	}
    }
}

void HMessageMgr::setF(const char *name)
{
  // Sets the filename for the debug messages.
  // This filenames are printed when the debug level is set.

  if(name && (strlen(name) != 0))
    strncpy(file, name,79);
  else file[0] = '\0';
}

void HMessageMgr::setL(int num)
{
  // Sets the linenumber for the debug message.
  // This linenumber is printed when the debug level is set.

  line = num;
}
