//////////////////////////////////////////////////////
//  HRfioFile
//
//  Class for reading events from files on taperobot
//
//////////////////////////////////////////////////////

#ifndef HRFIOFILE_H
#define HRFIOFILE_H

#include "hrfiofiletsm.h"
#include "TObject.h"
#include "TString.h"
#include <iostream>

class HRFIOFile : public TObject
{
 protected:
    HRFIOFileTSM *file;  //! filepointer
    int status;    //! status variable
    streampos pos; //! position in file
    TString *fname;   //! name of file
    int fmode;     //! mode of file open

 public:
    HRFIOFile(const char *name, int mode = ios::in);
    ~HRFIOFile(void);

    int open(const char *name, int mode = ios::in);
    int close(void);
    static int access(const char *name, int mode = 0);
    int read(char *buffer, int len);
    int write(const char *buffer, int len);
    HRFIOFileTSM& seekp(streampos n);
    HRFIOFileTSM& seekg(streampos offs, int p);
    streampos tellg(void);
    void ignore(int max=1, int term = EOF);
    Bool_t good(void);
    Bool_t eof(void);
    Bool_t fail(void);
    Bool_t bad(void);
    int rdstat(void);
    HRFIOFile& operator>>(HRFIOFile& u);
    HRFIOFile& operator<<(const HRFIOFile& u);
    ClassDef(HRFIOFile,0) // RFIO file interface
	};

#endif
