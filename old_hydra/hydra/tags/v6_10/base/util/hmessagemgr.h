#ifndef HMESSAGEMGR__H
#define HMESSAGEMGR__H

#include "TObject.h"
#include "TString.h"
#include "TSocket.h"
#include "TError.h"
#include "hades.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>


#define LEVELMAX 10

#define DET_RICH      0x001
#define DET_START     0x002
#define DET_MDC       0x004
#define DET_TOF       0x008
#define DET_TOFINO    0x010
#define DET_SHOWER    0x020
#define DET_TRIGGER   0x040
#define DET_RUNTIMEDB 0x080
#define DET_KICKPLANE 0x100
#define DET_QA        0x200

#define DET_ALL DET_RICH + DET_START + DET_MDC + DET_TOF + DET_TOFINO + DET_SHOWER + DET_TRIGGER + DET_RUNTIMEDB + DET_KICKPLANE + DET_QA

#if HDL > 0
#define DEBUG_msg(level, det, text) \
  ((HMessageMgr *)gHades->getMsg())->setL(__LINE__); \
  ((HMessageMgr *)gHades->getMsg())->setF(__FILE__); \
  ((HMessageMgr *)gHades->getMsg())->debug(level,det, this->GetName(), text);
#else
#define DEBUG_msg(level, det, text) \
  ((HMessageMgr *)gHades->getMsg())->setL(0); \
  ((HMessageMgr *)gHades->getMsg())->setF(""); \
  ((HMessageMgr *)gHades->getMsg())->debug(level,det, this->GetName(), text);
#endif

#define ERROR_msg(det, text) ((HMessageMgr *)gHades->getMsg())->error(1, det, this->GetName(), text);

#define WARNING_msg(level, det, text) ((HMessageMgr *)gHades->getMsg())->warning(level,det, this->GetName(), text);

#define INFO_msg(level, det, text) ((HMessageMgr *)gHades->getMsg())->info(level,det, this->GetName(), text);

#define SEPERATOR_msg(sep, num) ((HMessageMgr *)gHades->getMsg())->messageSeperator(sep,num);

class HMessageMgr : public TNamed
{
 public:
  HMessageMgr(const char *name, const char *title);
  ~HMessageMgr(void);

  Bool_t setDefault(void);
  Bool_t setFile(const char *dir);
  Bool_t setPort(const char *host, Int_t port);
  Bool_t switchOff(void);

  void setDebugLevel(Char_t level);
  void setInfoLevel(Char_t level);
  void setWarningLevel(Char_t level);
  void enableDetectorMsg(Int_t det);

  void messageSeperator(Char_t *seperator, Int_t num);

  static void hydraErrorHandler(int level, Bool_t abort, const char *location, const char *msg);

  void debug(Char_t level, Int_t det, const char *className, const char *text);
  void error(Char_t level, Int_t det, const char *className, const char *text);
  void warning(Char_t level, Int_t det, const char *className, const char *text);
  void info(Char_t level, Int_t det, const char *className, const char *text);

  // version with variable arguments list
  void debug(Int_t level, Int_t det, const char *className, const char *text, ...);
  void error(Int_t level, Int_t det, const char *className, const char *text, ...);
  void warning(Int_t level, Int_t det, const char *className, const char *text, ...);
  void info(Int_t level, Int_t det, const char *className, const char *text, ...);
  
  //version with variable argument list including filename and line number
  void debug(Int_t level, Int_t det, const char *className, const char *file, Int_t line, const char *text, ...);

  void setF(const char *name);
  void setL(int num);


 protected:
  void write(const char *text);
  void ewrite(const char *text);

 private:
  Char_t debugLevel;       //! Level up to which the messages wil be printed.
  Char_t warningLevel;     //! Level up to which the messages wil be printed.
  Char_t infoLevel;        //! Level up to which the messages wil be printed.

  FILE *msgFile;           //! File to write messages to (debug,warning,info)
  FILE *errorFile;         //! File to write error messages to

  TSocket *msgSocket;      //! Socket to send messages to (debug,warning,info)
  TSocket *errorSocket;    //! Socket to send error messages to
  TString *thisHost;       //! Name of this host

  Int_t detectorMsg;       //! Bitmask fo which detector to print messages
  char *msg1, *msg2;       //! internal message variable 
  char *file;              //! pointer to store name of file where message comes from
  Int_t line;              //! line in file where message comes from
 
  ClassDef(HMessageMgr,0)  //  Message logging facility
};

#endif
