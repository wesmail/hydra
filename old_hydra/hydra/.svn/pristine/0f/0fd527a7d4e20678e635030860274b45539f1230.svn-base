using namespace std;
#include <fstream> 


// ************************************************************************************
// CLASS HadIoAscii
//
// This class provides methods for easy reading data from ascii files in free format.
// The types of data supported now are: Int_t ('i'), Float_t ('f') and Char_t ('c')
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
Char_t m_filename[60];

Int_t iostatus;
Long_t position,ipos;
Long_t filesize;
Char_t dataname[128];
ifstream filedata;
Char_t storekey[128];
Int_t storesize;
Char_t storetype;
Float_t* floatdata;
Int_t* intdata;
Char_t* chardata;

ifstream& openfile(ifstream& readData, const Char_t* filename);
ifstream& closefile(ifstream& readData);
ifstream& readrecord(ifstream& readData, const Char_t* keyword, const Char_t rtyp, const Int_t rlen);

  public:

Int_t echo; 
Int_t recordnr; 

HadIoAscii();
virtual ~HadIoAscii();
HadIoAscii(const HadIoAscii& source);

void reset(); 
ifstream& open(const Char_t* filename); 
ifstream& open();
 
ifstream& close(); 
ifstream& read(const Char_t* keyword, const Char_t rtyp=0, const Int_t rlen=0); 
HadIoAscii& operator=(const HadIoAscii& source);

void setFileName(const Char_t *pFileName);
const Char_t* getFileName(){return m_filename;}

Char_t* getKeyword();
Char_t getType();
Int_t getLength();
void* getData();
void* getData(const Char_t* keyword, const Char_t rtyp=0, const Int_t rlen=0);
Int_t getIntData(Int_t pos=0);
Int_t getIntData(const Char_t* keyword, Int_t pos=0, const Char_t rtyp=0, const Int_t rlen=0);
Float_t getFloatData(Int_t pos=0);
Float_t getFloatData(const Char_t* keyword, Int_t pos=0, const Char_t rtyp=0, const Int_t rlen=0);
Char_t getCharData(Int_t pos=0);
Char_t getCharData(const Char_t* keyword, Int_t pos=0, const Char_t rtyp=0, const Int_t rlen=0);
Int_t getStringData(const Char_t* keyword, Char_t* buf, Int_t bufsize, Int_t pos=0, const Char_t rtyp=0, const Int_t rlen=0);
Int_t getStringData(Int_t pos, Char_t* buf, Int_t bufsize);
ClassDef(HadIoAscii, 0) // Parameter I/O from Ascii files by W.Przygoda 
};

#endif
