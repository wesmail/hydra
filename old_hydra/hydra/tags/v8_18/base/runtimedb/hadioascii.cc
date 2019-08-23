using namespace std;
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
#include <fstream> 
#include <string.h>
#include "hadioascii.h"

ClassImp(HadIoAscii);

HadIoAscii::HadIoAscii() {
 iostatus = 0;
 echo = 0;
 position = 0;
 ipos = 0;
 filesize = 0;
 dataname[0] = '\0';
 recordnr = 0;
 storekey[0] = '\0';
 storesize = 0;
 storetype = '\0';
 floatdata = NULL;
 intdata = NULL;
 chardata = NULL;
}
HadIoAscii::~HadIoAscii() {
 if (floatdata) delete [] floatdata;
 if (intdata) delete [] intdata;
 if (chardata) delete [] chardata;
}

HadIoAscii::HadIoAscii(const HadIoAscii& source) {
 cerr << "No possibility to copy HadIoAscii object!\a\n";
}

void HadIoAscii::setFileName(const Char_t *pFileName){
   strncpy(m_filename, pFileName, sizeof(m_filename));
}

HadIoAscii& HadIoAscii::operator=(const HadIoAscii& source) {
 if (this == &source) return *this;
 cerr << "No possibility to compare HadIoAscii objects!\a\n";
return *this;
}


ifstream& HadIoAscii::openfile(ifstream& readData, const Char_t* filename) {

Char_t tempname[128];
Int_t dataOK;

if (iostatus) {
 cerr << "File (\'" << dataname << "\?\') already opened!\a\n";
 return readData;
}

dataname[0] = '\0';
strcpy(dataname,filename);

 for (dataOK=0; !dataOK; ) {
  readData.open(dataname,ios::in);
  if (!readData) {
   cerr << "File \'" << dataname << "\' not found! \a\n";
   readData.clear(readData.rdstate() & ~ios::badbit);
   cout << "Write valid file name: ";
   cin >> tempname;
   strcpy(dataname,tempname);
  }
  else {
   dataOK = 1;
  }
 }

readData.seekg(0,ios::end);
filesize = readData.tellg();
readData.seekg(0);
iostatus = 1;
if (echo) cout << "File \'" << dataname << "\' opened.\n";
return readData;
} // eof


ifstream& HadIoAscii::open(const Char_t* filename) {
 return openfile(filedata,filename);
}

ifstream& HadIoAscii::open() {
  return openfile(filedata,m_filename);
}


void HadIoAscii::reset() {

 position = 0;
 ipos = 0;
 storekey[0] = '\0';
 storesize = 0;
 storetype = '\0';

 if (floatdata) {
  delete [] floatdata;
  floatdata = NULL;
 }
 if (intdata) {
  delete [] intdata;
  intdata = NULL;
 }
 if (chardata) {
  delete [] chardata;
  chardata = NULL;
 }
}




ifstream& HadIoAscii::closefile(ifstream& readData) {

if (!iostatus) {
 cerr << "No file opened!\a\n";
 return readData;
}

 readData.close();
 if (echo) cout << "File \'" << dataname << "\' closed.\n";

 iostatus = 0;
 echo = 0;
 filesize = 0;
 dataname[0] = '\0';
 reset();

return readData;
} // eof


ifstream& HadIoAscii::close() {
 return closefile(filedata);
} 


ifstream& HadIoAscii::readrecord(ifstream& readData, const Char_t* keyword, const Char_t rtyp, const Int_t rlen) {

 Char_t c1[128],r_type,recname[128],reclong[6],tempname[128];
 Int_t i = 0;
 Int_t j,r_num,i1;
 Float_t f1;

 if (!iostatus) {
  cerr << "No file opened!\a\n";
  return readData;
 }

 storekey[0] = '\0';
 storesize = 0;
 storetype = '\0';
 if (floatdata) {
  delete [] floatdata;
  floatdata = NULL;
 }
 if (intdata) {
  delete [] intdata;
  intdata = NULL;
 }
 if (chardata) {
  delete [] chardata;
  chardata = NULL;
 }

 recname[0] = '\0';
 reclong[0] = '\0';
 ipos = position = readData.tellg();


 while (strcmp(recname,keyword) && ipos<filesize) {
  i=0;
  recname[i] = '\0';

  while (ipos<filesize && (readData.peek()<'A' || (readData.peek()>'Z' && readData.peek()<'a') || readData.peek()>'z')) {
   readData.ignore(1); 
   ipos = readData.tellg();
  }
  if (ipos<filesize) {
   readData >> tempname;
   ipos = readData.tellg();
   if (readData.fail()) {
    readData.clear(readData.rdstate() & ~ios::failbit);
    cerr << "Error during reading keyword (Char_t data) in file \'" << dataname << "\'!\a\n";
    reset();
    return readData;
   }
   while (tempname[i] != ':' && tempname[i]) {
    recname[i] = tempname[i];
    i++;
   }
   recname[i] = '\0';
  }
 }


 if (strcmp(recname,keyword)) {
  readData.seekg(0);
  ipos = readData.tellg();
  while (strcmp(recname,keyword) && ipos<position) {
   i=0;
   recname[i] = '\0';
   while (ipos<position && (readData.peek()<'A' || (readData.peek()>'Z' && readData.peek()<'a') || readData.peek()>'z')) {
    readData.ignore(1);
    ipos = readData.tellg();
   }
   if (ipos<position) {
    readData >> tempname;
    ipos = readData.tellg();
    if (readData.fail()) {
     readData.clear(readData.rdstate() & ~ios::failbit);
     cerr << "Error during reading keyword (Char_t data) in file \'" << dataname << "\'!\a\n";
     reset();
     return readData;
    }
    while (tempname[i] != ':' && tempname[i]) {
     recname[i] = tempname[i];
     i++;
    }
    recname[i] = '\0';
   }
  }
 }

 if (strcmp(recname,keyword)) {
  cerr << "No record with keyword \'" << keyword << "\' found in file \'" << dataname << "\'!\a\n"; 
  return readData;
 } else {
  
  if (!rlen && tempname[i] == ':') {
   i++;
   r_type = (char)tempname[i];
   i++;
   if (tempname[i] == ':') {
    i++;
    j = 0;
    while(tempname[i]) {
     reclong[j] = tempname[i];
     i++;
     j++;
    }
    reclong[j]='\0';
    r_num = atoi(reclong);
   } else {
    r_num = 1;
   }
  } else {
   if (!rlen) {
    r_type = 'f';
    r_num = 1; 
   } else {
    r_type = rtyp;
    r_num = rlen;
   }
  }

  strcpy(storekey,recname);
  storetype = r_type;
  storesize = r_num;
  if (echo) cout << "Record \'" << storekey << "\', type of data \'" << storetype << "\', size of data \'" << storesize << "\'.\n";

  switch (r_type) {
     case 'i': intdata = new int[storesize];
               for (i=0; i<r_num; i++) {
                readData >> i1;
                if (readData.fail()) {
                 readData.clear(readData.rdstate() & ~ios::failbit);
                 cerr << "Error during reading " << i+1 << ". of " << r_num << " Int_t data in record \'" << keyword << "\'!\a\n";
                 reset();
                 return readData;
                }
                intdata[i] = i1;
                if (echo) cout << " " << intdata[i] << ", ";
               }
               if (echo) cout << endl;
               recordnr++;
               break;
     case 'f': floatdata = new float[storesize];
               for (i=0; i<r_num; i++) {
                readData >> f1;
                if (readData.fail()) {
                 readData.clear(readData.rdstate() & ~ios::failbit);
                 cerr << "Error during reading " << i+1 << ". of " << r_num << " Float_t data in record \'" << keyword << "\'!\a\n";
                 reset();
                 return readData;
                }
                floatdata[i] = f1;
                if (echo) cout << " " << floatdata[i] << ", ";
               }
               if (echo) cout << endl;
               recordnr++;
               break;
     case 'c': readData >> c1;
               if (readData.fail()) {
                readData.clear(readData.rdstate() & ~ios::failbit);
                cerr << "Error during reading Char_t data in record \'" << keyword << "\'!\a\n";
                reset();
                return readData;
               }
               i=0;
               while (c1[i]) i++;
               storesize = i;
               chardata = new char[storesize+1];
               strcpy(chardata,c1);
               if (echo) cout << " " << c1 << endl;
               recordnr++;
               break;
      default: cerr << "Type \'" << r_type << "\' not supported!\a\n";
               reset();
               break;
  }
 }

return readData;
} // eof


ifstream& HadIoAscii::read(const Char_t* keyword, const Char_t rtyp, const Int_t rlen) {
 return readrecord(filedata,keyword,rtyp,rlen);
}


Char_t* HadIoAscii::getKeyword() {
 if (storekey) return storekey;
  else
   cerr << "No data available!\a\n";
return '\0';
}


Char_t HadIoAscii::getType() {
 if (storetype) return storetype;
  else
   cerr << "No data available!\a\n";
return '\0';
}


Int_t HadIoAscii::getLength() {
 if (storesize) return storesize;
  else
   cerr << "No data available!\a\n";
return 0;
}


void* HadIoAscii::getData() {
 if (storesize && storetype) 
  switch(storetype) {
   case 'i': return (void*)intdata;
   case 'f': return (void*)floatdata;
   case 'c': return (void*)chardata;
  } else {
   cerr << "No data available!\a\n";
   return NULL; 
  }
 cerr << "Type \'" << storetype << "\' not supported!\a\n";
return NULL;
}


void* HadIoAscii::getData(const Char_t* keyword, const Char_t rtyp, const Int_t rlen) {
 readrecord(filedata,keyword,rtyp,rlen);
return getData();
}


Int_t HadIoAscii::getIntData(Int_t pos) {
 if (storesize) {
  if (storetype=='i') {
   if (storesize>pos) {
    return *(intdata+pos);
   }
   else {
    cerr << "Position " << pos << " out of data range (" << storesize << ") !\a\n";
    return 0;
   } 
  } else {
   cerr << "Data stored are of different (" << storetype << ") type!\a\n";
   return 0;
  }
 } 
 cerr << "No data available!\a\n";
return 0;
}


Int_t HadIoAscii::getIntData(const Char_t* keyword, Int_t pos, const Char_t rtyp, const Int_t rlen) {
 if (strcmp(storekey,keyword)) readrecord(filedata,keyword,rtyp,rlen);
return getIntData(pos);
}



Float_t HadIoAscii::getFloatData(Int_t pos) {
 if (storesize) {
  if (storetype=='f') {
   if (storesize>pos) {
    return *(floatdata+pos);
   }
   else {
    cerr << "Position " << pos << " out of data range (" << storesize << ") !\a\n";
    return 0.0;
   }
  } else {
   cerr << "Data stored are of different (" << storetype << ") type!\a\n";
   return 0.0;
  }
 }
 cerr << "No data available!\a\n";
return 0.0;
}


Float_t HadIoAscii::getFloatData(const Char_t* keyword, Int_t pos, const Char_t rtyp, const Int_t rlen) {
 if (strcmp(storekey,keyword)) readrecord(filedata,keyword,rtyp,rlen);
return getFloatData(pos);
}



Char_t HadIoAscii::getCharData(Int_t pos) {
 if (storesize) {
  if (storetype=='c') {
   if (storesize>pos) {
    return *(chardata+pos);
   }
   else {
    cerr << "Position " << pos << " out of data range (" << storesize << ") !\a\n";
    return 0;
   }
  } else {
   cerr << "Data stored are of different (" << storetype << ") type!\a\n";
   return 0;
  }
 }
 cerr << "No data available!\a\n";
return 0;
}


Char_t HadIoAscii::getCharData(const Char_t* keyword, Int_t pos, const Char_t rtyp, const Int_t rlen) {
 if (strcmp(storekey,keyword)) readrecord(filedata,keyword,rtyp,rlen);
return getCharData(pos);
}

Int_t HadIoAscii::getStringData(Int_t pos, Char_t* buf, Int_t bufsize) {
  if (!storesize) 
    cerr << "No data available!\a\n";

  if (storetype!='c') {
   cerr << "Data stored are of different (" << storetype << ") type!\a\n";
   return 0;
  }

  if (storesize>pos) {
//  Char_t* sppos = strchr(chardata+pos, ' ');
    Int_t nlen = strlen(chardata+pos);
    if(nlen < bufsize) {
       strncpy(buf, chardata+pos, nlen);
       buf[nlen] = 0;
    } else {
       cerr << "Buffer is too small !" << endl;
       return 0;
    }
  }
  else {
    cerr << "Position " << pos << " out of data range (" << storesize << ") !\a\n";
    return 0;
  }

  return 1;
}

Int_t HadIoAscii::getStringData(const Char_t* keyword, Char_t* buf, Int_t bufsize, 
        Int_t pos, const Char_t rtyp, const Int_t rlen) {
 if (strcmp(storekey,keyword)) readrecord(filedata,keyword,rtyp,rlen);
return getStringData(pos, buf, bufsize);
}

















