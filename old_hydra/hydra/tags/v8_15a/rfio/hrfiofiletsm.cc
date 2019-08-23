//*-- Author : J.Wuestenfeld
//*-- Modified : 11/19/2003 by J.Wuestenfeld
//*-- Modified : 04/06/2005 by J.Wuestenfeld

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////
//HRfioFile
//
//  Class used for reading HLD events from file on taperobot
//
/////////////////////////////////////////////////////

#include <string>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "hrfiofiletsm.h"
#include "hrfiotsmsignalhandler.h"
#include "hmessagemgr.h"

ClassImp(HRFIOFileTSM)

HRFIOFileTSM::HRFIOFileTSM(const char *name, int mode)
  : TObject()
{
  // Default constructor. sET ALL PARAMETERS TO DEFAULT VALUES.
  //
  // Input parameters:
  //    const char* name
  //       Name of the file
  //
  //    int mode
  //       Mode to be used for file opening,
  //       one of the ios::xxx variables
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    none
  fname = NULL;
  fmode = mode;
  status = ios::badbit;
  pos = 0;
  file = 0;
}

HRFIOFileTSM::~HRFIOFileTSM(void)
{
  // Default destructor. cLOSE FILE IF OPEN.
  //
  // Input parameters:
  //    none
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //   none
  if(close())
    cout << "Error closing file" << endl;
  if(fname)
    delete fname;
}

int HRFIOFileTSM::open(const char *name, int mode )
{
  // Open a file on taperobot with name and mode. Tests first if file is accessible.
  //
  // Input parameters:
  //   const char* name
  //      Name of the file to be opened.
  //
  //   int mode
  //       Mode to be used for file opening,
  //       one of the ios::xxx variables
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    0 on success, -1 otherwise.
  int len;
  
  status = ios::badbit;
  if((len=strlen(name)) > 0)
  {
    if(!fname)
      fname = new char[len+1];
    strcpy(fname,name);
    fmode = mode;
    file = rfio_fopen(fname,"r");
    if(file)
    {
      status = ios::goodbit;
      pos = 0;
      handler = new HRFIOTsmSignalHandler(&file);
      return 0;
    }
    else
    {
      status = ios::badbit;
      handler = NULL;
      return -1;
    }
  }
  return -1;
}

int HRFIOFileTSM::close(void)
{
  // Closes file.
  //
  // Input parameters:
  //    none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   0 if closed, -1 if no file opened.
  if(file)
  {
    rfio_fclose(file);
    file = NULL;
    if(fname)
              strcpy(fname,"");
    fmode = ios::in;
    status = ios::badbit;
    pos = 0;
    if (handler) delete handler;
    return 0;
  }
  else
    return -1;
}

int HRFIOFileTSM::access(const char *name, int mode)
{
  // Tests for accessibility of file on taperobot.
  //
  // Input parameters:
  //   const char* name
  //       Name of the file to be tested
  //
  //   int mode
  //       Mode to be used for file opening,
  //       one of the ios::xxx variables
  //
  // Output parameters:
  //    none
  //
  // Return code:
  //    0 on success, -1 in case of failure.
  //    If mode is set to !=0 returns -1.
  Int_t res;

  gHades->getMsg()->info((Int_t)10,HMessageMgr::DET_MDC,"HRFIOFileTSM","TSM access for file: %s",name);
  if (mode == 0)
  {
    RFILE *fRemote = NULL;
        
    char *tmp;

    tmp = new char[400];
    sprintf(tmp,"%s",name);
    // Normally there should only be a call to rfio_access() needed, but this
    // is not implemented. So the sequence of rfio_open(), rfio_close() is used
    // as workaround. If open fails, the file does not exist.
    fRemote = (srawAPIFile *)rfio_fopen(tmp, "r");
    if (fRemote)
    {
      res = rfio_fclose(fRemote);
      if(res!=0)
      {
        delete [] tmp;
        return -1;
      }
      else
      {
        delete [] tmp;
        gHades->getMsg()->info(10,HMessageMgr::DET_MDC,"HRFIOFileTSM","Access succesfully");
        return 0;
      }
    }
    else
    {
      delete [] tmp;
      return -1;
    }
  }
  else
  {
    return -1;
  }
}

int HRFIOFileTSM::read(char *buffer, int len)
{
  // Reads a buffer with length len from the file
  //
  // Input parameters:
  //   int len
  //     Length of buffer, defines maximum number of bytes to be read from file.
  //
  // Output parameters:
  //   char* buffer
  //      Pointer to buffer to store data from file into.
  //
  // Return code:
  //   Number of bytes read from file
  int res=0;

  if(!eof())
  {
    res = rfio_read(file->iFileId,buffer,len);
    if(res < len)
    {
      status = ios::eofbit & ios::failbit;
    }
    else
      status = ios::goodbit;
    pos+=res;
  }
  return res;
}

int HRFIOFileTSM::write(const char *buffer, int len)
{
  // Writes buffer to file. NOT TESTED!!
  //
  // Input Parameters:
  //   const char* buffer
  //     Buffer with data to be written to file.
  //
  //   int len
  //     Number of bytes to write to file.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Number of bytes written.
  int res;

  res = rfio_write(file->iFileId,(char *)buffer,len);
  if(res == 0 )
  {
    status = ios::failbit;
    return -1;
  }
  if(res < len)
  {
    status = ios::eofbit & ios::failbit;
    pos += res;
  }
  else
  {
    status = ios::goodbit;
    pos += res;
  }
  return 0;
}

HRFIOFileTSM& HRFIOFileTSM::seekp(streampos n)
{
  // Moves accesspointer in file relative to current position.
  //
  // Input parameters:
  //   streampos n
  //      Position in stream to move filepointer to.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Pointer to this object.
  return seekg(n,(ios::seekdir)ios::cur);
}

HRFIOFileTSM& HRFIOFileTSM::seekg(streampos offs, int p)
{
  // Moves accesspointer in file relative.
  //
  // Input parameters:
  //   streampos offs
  //     number of bytes to move filepointer relativ to position defined via p.
  //
  //   int p
  //     Relativ to what position to move fileponter. Can be one of : 
  //     ios::beg, ios::cur, ios::end.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Pointer to this object.
  off_t res;

  res = rfio_lseek(file->iFileId,offs,(ios::seekdir)p);
  if(res != (off_t)offs-1)
    status = ios::goodbit;
  else
    status = ios::eofbit | ios::failbit;
  pos += res;
  return *this;
}

streampos HRFIOFileTSM::tellg(void)
{
  // Returns current position in file.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Actual position in file.
  return pos;
}

void HRFIOFileTSM::ignore(int max, int term)
{
  // Ignore max bytes in file. Moves access pointer to new position.
  //
  // Input parameters:
  //   int max
  //     Maximum number of bytes to skip.
  //
  //   int term
  //     Ignored parameter, provided for compatibility.
  //
  // Return code:
  //   none
  seekp(max);
}

Bool_t HRFIOFileTSM::good(void)
{
  // Return status of last operation.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   kTRUE, if last operation was successfull, kFALSE otherwise.
  if(status == ios::goodbit)
    return kTRUE;
  else return kFALSE;
}

Bool_t HRFIOFileTSM::eof(void)
{
  // Check if filepointer has reachjed the end of file marker.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   kTRUE if accesspointer has reached the end of file marker, kFALSE otherwise.
  return (status & ios::eofbit) ? kTRUE:kFALSE;
}

Bool_t HRFIOFileTSM::fail(void)
{
  // Check for file status.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   kTRUE if last operation failed, kFALSE otherwise.
  //   File unusable if kTRUE!
  return (status & ios::failbit) ? kTRUE:kFALSE;
}

Bool_t HRFIOFileTSM::bad(void)
{
  // Check for file status.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //    kTRUE, if last operation failed, but error can be repaired,
  //    otherwise kFALSE. 
  return (status & ios::badbit) ? kTRUE:kFALSE;
}

int HRFIOFileTSM::rdstat(void)
{
  // Return status.
  // Check for file status.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Status of file.
  return status;
}

int HRFIOFileTSM::stat(Long_t *id, Long_t *size, Long_t *flags, Long_t *modtime)
{
  // Get information about file.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   Long_t* id
  //     Contains device descriptor in the bits above 24, and the inode of the
  //     file in the lower 24 bits.
  //
  //   Long_t* size
  //      Size of the file in bytes.
  //
  //   Long_t* flags
  //      Bitmask defining the file type:
  //       bit 0 set if owner has execute permission,
  //       bit 1 set if is a directory,
  //       bit 3 set if not a regular file and not a directory.
  //
  // Return code:
  //   0 on success, -1 otherwise.
  struct stat statbuf;

  if(rfio_fstat(file->iFileId,&statbuf) == 0)
  {
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
  return -1;
}

char *HRFIOFileTSM::serror(void)
{
  // Get the error message of the last operation.
  //
  // Input parameters:
  //   none
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   String describing error from last operation on file.
  return rfio_serror();
}

HRFIOFileTSM& HRFIOFileTSM::operator>>(HRFIOFileTSM& u)
{
  // Operator to write to the file. NOT IMPLEMENTED!
  //
  // Input Parameters:
  //   Buffer with data to be written to file.
  //
  // Output parameters:
  //   none
  //
  // Return code:
  //   Pointer to this object.
  return *this;
}

HRFIOFileTSM& HRFIOFileTSM::operator<<( const HRFIOFileTSM& u)
{
  // Operator to read from the file. NOT IMPLEMENTED!
  //
  // Input Parameters:
  //   none
  //
  // Output parameters:
  //   Buffer to store data from file into.
  //
  // Return code:
  //   Pointer to this object.
  return *this;
}

Int_t HRFIOFileTSM::unlink(char *name)
{
  // Delete a file. NOT IMPLEMENTED
  //
  // Input parameters:
  //   char* name
  //     Name of file to be deleted.
  //
  // Output parameters:
  //   none
  //
  //  Return code:
  //    unknownw, as not implemented in underlying rfio code!
  return rfio_unlink(name);
}
