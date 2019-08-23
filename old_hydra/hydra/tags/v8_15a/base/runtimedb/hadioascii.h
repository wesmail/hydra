using namespace std;
#include <fstream> 


// ************************************************************************************
// CLASS HadIoAscii
//
// This class provides methods for easy reading data from ascii files in free format.
// The types of data supported now are: int ('i'), float ('f') and char ('c')
// The format of record in which data have to be stored consists of 
//   keyword - which has to be directly before set of data to be read or
//   keyword:x:n - where additionally x = i,f,c (type) n = number of data to be read
// and after that - set of data for reading.
// Default x = f, n = 1, in case of x = c no need to specify n.
// (see description of public functions below)
//
// Author: WP      Updates: -    
// Last changes: 18 Jun 1998
// ************************************************************************************

#ifndef HadIoAscii_H
#define HadIoAscii_H

#include <TNamed.h>

class HadIoAscii : public TNamed {
char m_filename[60];

int iostatus;
long position,ipos;
long filesize;
char dataname[128];
ifstream filedata;
char storekey[128];
int storesize;
char storetype;
float* floatdata;
int* intdata;
char* chardata;

ifstream& openfile(ifstream& readData, const char* filename);
ifstream& closefile(ifstream& readData);
ifstream& readrecord(ifstream& readData, const char* keyword, const char rtyp, const int rlen);

  public:

int echo; 
int recordnr; 

HadIoAscii();
virtual ~HadIoAscii();
HadIoAscii(const HadIoAscii& source);

void reset(); 
ifstream& open(const char* filename); 
ifstream& open();
 
ifstream& close(); 
ifstream& read(const char* keyword, const char rtyp=0, const int rlen=0); 
HadIoAscii& operator=(const HadIoAscii& source);

void setFileName(const char *pFileName);
const char* getFileName(){return m_filename;}

char* getKeyword();
char getType();
int getLength();
void* getData();
void* getData(const char* keyword, const char rtyp=0, const int rlen=0);
int getIntData(int pos=0);
int getIntData(const char* keyword, int pos=0, const char rtyp=0, const int rlen=0);
float getFloatData(int pos=0);
float getFloatData(const char* keyword, int pos=0, const char rtyp=0, const int rlen=0);
char getCharData(int pos=0);
char getCharData(const char* keyword, int pos=0, const char rtyp=0, const int rlen=0);
int getStringData(const char* keyword, char* buf, int bufsize, int pos=0, const char rtyp=0, const int rlen=0);
int getStringData(int pos, char* buf, int bufsize);
ClassDef(HadIoAscii, 0) // Parameter I/O from Ascii files by W.Przygoda 
};

#endif
