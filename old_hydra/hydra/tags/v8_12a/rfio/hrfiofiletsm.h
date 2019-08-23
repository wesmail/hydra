//////////////////////////////////////////////////////
//  HRfioFile
//
//  Class for reading events from files on taperobot.
//
//////////////////////////////////////////////////////

#ifndef HRFIOFILETSM_H
#define HRFIOFILETSM_H

#include "TObject.h"
#include "rawapin.h"

#include <iostream>
using namespace std;

class HRFIOTsmSignalHandler;

class HRFIOFileTSM : public TObject
{
 protected:
		RFILE *file;   //! pointer to file in taperobot
		int status;    //! status variable
		streampos pos; //! position in file
		char *fname;   //! name of file
		int fmode;     //! mode of file open
        HRFIOTsmSignalHandler *handler;

 public:
		HRFIOFileTSM(const char *name="", int mode = ios::in);
		~HRFIOFileTSM(void);

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
		int stat(Long_t *id, Long_t *size, Long_t *flags, Long_t *modtime);
		char *serror(void);
		HRFIOFileTSM& operator>>(HRFIOFileTSM& u);
		HRFIOFileTSM& operator<<(const HRFIOFileTSM& u);
		static int unlink(char *name);
		ClassDef(HRFIOFileTSM,0) // RFIO file accessing the TSM robot
};

#endif
