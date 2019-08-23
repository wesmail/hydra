#ifndef HTRFIOFILE__H
#define HTRFIOFILE__H

#include "TFile.h"
#include "TUrl.h"

class HTRFIOFile : public TFile
{
private:
   TUrl      fUrl;        //URL of file

   HTRFIOFile(void) : fUrl("dummy") { }

   // Interface to basic system I/O routines
   Int_t    SysOpen(const char *pathname, Int_t flags, UInt_t mode);
   Int_t    SysClose(Int_t fd);
   Int_t    SysRead(Int_t fd, void *buf, Int_t len);
   Int_t    SysWrite(Int_t fd, const void *buf, Int_t len);
   Long64_t SysSeek(Int_t fd, Long64_t offset, Int_t whence);
   Int_t    SysStat(Int_t fd, Long_t *id, Long64_t *size, Long_t *flags, Long_t *modtime);
   Int_t    SysSync(Int_t) { /* no fsync for RFIO */ return 0; }

public:
   HTRFIOFile(const char *url, Option_t *option="", const char *ftitle="", Int_t compress=1);
   ~HTRFIOFile();

   Bool_t  ReadBuffer(char *buf, Int_t len);
   Bool_t  WriteBuffer(const char *buf, Int_t len);

   Int_t   GetErrno(void) const;
   void    ResetErrno(void) const;

   ClassDef(HTRFIOFile,0)  //A ROOT file that reads/writes via a rfiod server
};

#endif
