//*-- Author : J.Wuestenfeld
//*-- Modified : 05/06/2006 by J.Wuestenfeld

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////
//HTRfioFile
//
//  Class used for reading ROOT file on taperobot
//
/////////////////////////////////////////////////////

#include "htrfiofile.h"
#include "TROOT.h"
#include "TSystem.h"
#include <sys/stat.h>
#include <sys/types.h>

ClassImp(HTRFIOFile)

#ifndef RFIO_READOPT
#define RFIO_READOPT 1
#endif

extern "C" {
   int   rfio_open(const char *filepath, int flags, int mode);
   int   rfio_close(int s);
   int   rfio_read(int s, void *ptr, int size);
   int   rfio_write(int s, const void *ptr, int size);
   int   rfio_lseek(int s, int offset, int how);
   int   rfio_access(const char *filepath, int mode);
   int   rfio_unlink(const char *filepath);
   int   rfio_parse(const char *name, char **host, char **path);
   int   rfio_stat(const char *path, struct stat *statbuf);
   int   rfio_fstat(int s, struct stat *statbuf);
   void  rfio_perror(const char *msg);
   char *rfio_serror();
   int   rfiosetopt(int opt, int *pval, int len);
   int   rfio_mkdir(const char *path, int mode);
   void *rfio_opendir(const char *dirpath);
   int   rfio_closedir(void *dirp);
   void *rfio_readdir(void *dirp);
#ifdef R__WIN32
   int  *C__serrno(void);
   int  *C__rfio_errno (void);
#endif
};

#ifdef R__WIN32

// Thread safe rfio_errno. Note, C__rfio_errno is defined in Cglobals.c rather
// than rfio/error.c.
#define rfio_errno (*C__rfio_errno())

// Thread safe serrno. Note, C__serrno is defined in Cglobals.c rather
// rather than serror.c.
#define serrno (*C__serrno())

#else

extern int rfio_errno;
extern int serrno;

#endif

HTRFIOFile::HTRFIOFile(const char *url, Option_t *option, const char *ftitle, Int_t compress)
  : TFile(url, "NET", ftitle, compress), fUrl(url)
{
  // Create a RFIO file object. A RFIO file is the same as a TFile
  // except that it is being accessed via a rfiod server. The url
  // argument must be of the form: rfio:/path/file.root (where file.root
  // is a symlink of type /shift/aaa/bbb/ccc) or rfio:server:/path/file.root.
  // If the file specified in the URL does not exist, is not accessable
  // or can not be created the kZombie bit will be set in the TRFIOFile
  // object. Use IsZombie() to see if the file is accessable.
  // For a description of the option and other arguments see the TFile ctor.
  // The preferred interface to this constructor is via TFile::Open().

  fOption = option;
  fOption.ToUpper();

  // tell RFIO to not read large buffers, ROOT does own buffering
  Int_t readopt = 0;
  ::rfiosetopt(RFIO_READOPT, &readopt, 4);
  if (fOption == "NEW")
    fOption = "CREATE";

  Bool_t create   = (fOption == "CREATE") ? kTRUE : kFALSE;
  Bool_t recreate = (fOption == "RECREATE") ? kTRUE : kFALSE;
  Bool_t update   = (fOption == "UPDATE") ? kTRUE : kFALSE;
  Bool_t read     = (fOption == "READ") ? kTRUE : kFALSE;
  if (!create && !recreate && !update && !read) {
    read    = kTRUE;
    fOption = "READ";
  }

  TString stmp;
  char *fname;
  if ((fname = gSystem->ExpandPathName(fUrl.GetFile()))) {
    if (!strstr(fname, ":/")) {
      char *host;
      char *name;
      if (::rfio_parse(fname, &host, &name))
        stmp = Form("%s:%s", host, name);
      else
        stmp = fname;
    } else
      stmp = fname;
      delete [] fname;
      fname = (char *)stmp.Data();
  } else {
    Error("TRFIOFile", "error expanding path %s", fUrl.GetFile());
    goto zombie;
  }

  if (recreate) {
    if (::rfio_access(fname, kFileExists) == 0)
      ::rfio_unlink(fname);
    recreate = kFALSE;
    create   = kTRUE;
    fOption  = "CREATE";
  }
  if (create && ::rfio_access(fname, kFileExists) == 0) {
    Error("TRFIOFile", "file %s already exists", fname);
    goto zombie;
  }
  if (update) {
    if (::rfio_access(fname, kFileExists) != 0) {
      update = kFALSE;
      create = kTRUE;
    }
    if (update && ::rfio_access(fname, kWritePermission) != 0) {
      Error("TRFIOFile", "no write permission, could not open file %s", fname
           );
      goto zombie;
    }
  }
  if (read) {
    if (::rfio_access(fname, kFileExists) != 0) {
      Error("TRFIOFile", "file %s does not exist", fname);
      goto zombie;
    }
    if (::rfio_access(fname, kReadPermission) != 0) {
      Error("TRFIOFile", "no read permission, could not open file %s", fname)
          ;
      goto zombie;
    }
  }

   // Connect to file system stream
  fRealName = fname;

  if (create || update) {
#ifndef WIN32
      fD = SysOpen(fname, O_RDWR | O_CREAT, 0644);
#else
      fD = SysOpen(fname, O_RDWR | O_CREAT | O_BINARY, S_IREAD | S_IWRITE);
#endif
      if (fD == -1) {
  SysError("TRFIOFile", "file %s can not be opened", fname);
  goto zombie;
      }
      fWritable = kTRUE;
  } else {
#ifndef WIN32
      fD = SysOpen(fname, O_RDONLY, 0644);
#else
      fD = SysOpen(fname, O_RDONLY | O_BINARY, S_IREAD | S_IWRITE);
#endif
      if (fD == -1) {
  SysError("TRFIOFile", "file %s can not be opened for reading", fname);
  goto zombie;
      }
      fWritable = kFALSE;
  }

  Init(create);

  return;

zombie:
   // error in file opening occured, make this object a zombie
    MakeZombie();
   gDirectory = gROOT;
}

HTRFIOFile::~HTRFIOFile()
{
  // RFIO file dtor. Close and flush directory structure.

  Close();

}

Int_t HTRFIOFile::SysOpen(const char *pathname, Int_t flags, UInt_t mode)
{  
  // Interface to system open. All arguments like in POSIX open.

  Int_t ret = ::rfio_open((char *)pathname, flags, (Int_t) mode);
  if (ret < 0)
    gSystem->SetErrorStr(::rfio_serror());
  return ret;
}

Int_t HTRFIOFile::SysClose(Int_t fd)
{
  // Interface to system close. All arguments like in POSIX close.

  Int_t ret = ::rfio_close(fd);
  if (ret < 0)
    gSystem->SetErrorStr(::rfio_serror());
  return ret;
}

Int_t HTRFIOFile::SysRead(Int_t fd, void *buf, Int_t len)
{
  // Interface to system read. All arguments like in POSIX read.

  fOffset += len;
  Int_t ret = ::rfio_read(fd, (char *)buf, len);
  if (ret < 0)
    gSystem->SetErrorStr(::rfio_serror());
  return ret;
}

Int_t HTRFIOFile::SysWrite(Int_t fd, const void *buf, Int_t len)
{
  // Interface to system write. All arguments like in POSIX write.

  fOffset += len;
  Int_t ret = ::rfio_write(fd, (char *)buf, len);
  if (ret < 0)
    gSystem->SetErrorStr(::rfio_serror());
  return ret;

}

Long64_t HTRFIOFile::SysSeek(Int_t fd, Long64_t offset, Int_t whence)
{   
  // Interface to system lseek. All arguments like in POSIX lseek
  // except that the offset and return value are Long_t to be able to
  // handle 64 bit file systems.

  if (whence == SEEK_SET && offset == fOffset) return offset;

  Long64_t ret = ::rfio_lseek(fd, offset, whence);

  if (ret < 0)
    gSystem->SetErrorStr(::rfio_serror());
  else
    fOffset = ret;

  return ret;
}

Int_t HTRFIOFile::SysStat(Int_t fd, Long_t *id, Long64_t *size, Long_t *flags, Long_t *modtime)
{
    // Interface to TSystem:GetPathInfo(). Generally implemented via
   // stat() or fstat().

  struct stat statbuf;

  if (::rfio_fstat(fd, &statbuf) >= 0) {
    if (id)
      *id = (statbuf.st_dev << 24) + statbuf.st_ino;
    if (size)
      *size = statbuf.st_size;
    if (modtime)
      *modtime = statbuf.st_mtime;
    if (flags) {
      *flags = 0;
      if (statbuf.st_mode & ((S_IEXEC)|(S_IEXEC>>3)|(S_IEXEC>>6)))
        *flags |= 1;
      if ((statbuf.st_mode & S_IFMT) == S_IFDIR)
        *flags |= 2;
      if ((statbuf.st_mode & S_IFMT) != S_IFREG &&
           (statbuf.st_mode & S_IFMT) != S_IFDIR)
        *flags |= 4;
    } 
    return 0;
  }

  gSystem->SetErrorStr(::rfio_serror());
  return 1;
}

Bool_t HTRFIOFile::ReadBuffer(char *buf, Int_t len)
{
  // Read specified byte range from remote file via rfiod daemon.
  // Returns kTRUE in case of error.

  if (fCache) {
    Int_t st;
    Long64_t off = GetRelOffset();
    if ((st = fCache->ReadBuffer(off, buf, len)) < 0) {
      Error("ReadBuffer", "error reading from cache");
      return kTRUE;
    }
    if (st > 0) {
      // fOffset might have been changed via TCache::ReadBuffer(), reset it
      Seek(off + len);
      return kFALSE;
    }
  }

  return TFile::ReadBuffer(buf, len);
}

Bool_t HTRFIOFile::WriteBuffer(const char *buf, Int_t len)
{
  // Write specified byte range to remote file via rfiod daemon.
  // Returns kTRUE in case of error.

  if (!IsOpen() || !fWritable) return kTRUE;

  if (fCache) {
    Int_t st;
    Long64_t off = GetRelOffset();
    if ((st = fCache->WriteBuffer(off, buf, len)) < 0) {
      Error("WriteBuffer", "error writing to cache");
      return kTRUE;
    }
    if (st > 0) {
      // fOffset might have been changed via TCache::WriteBuffer(), reset it
      Seek(off + len);
      return kFALSE;
    }
  }

  return TFile::WriteBuffer(buf, len);
}

Int_t HTRFIOFile::GetErrno(void) const
{
  // Method returning rfio_errno. For RFIO files must use this
  // function since we need to check rfio_errno then serrno and finally errno.

  if (rfio_errno)
    return rfio_errno;
  if (serrno)
    return serrno;
  return TSystem::GetErrno();

}

void HTRFIOFile::ResetErrno(void) const
{
  // Method resetting the rfio_errno, serrno and errno.

  rfio_errno = 0;
  serrno = 0;
  TSystem::ResetErrno();

}
