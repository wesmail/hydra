#ifndef HGEOMASCIIIO_H
#define HGEOMASCIIIO_H

using namespace std;
#include "hgeomio.h"
#include "TString.h"
#include <fstream>
#include <iomanip>

class HGeomAsciiIo : public HGeomIo {
  TString  filename;
  TString  filedir;
  Bool_t   writable;
  fstream* file;
public:
  HGeomAsciiIo();
  ~HGeomAsciiIo();
  void setDirectory(const char* fDir) {filedir=fDir;}
  const char* getDirectory() {return filedir.Data();}
  const char* getFilename() {return filename.Data();}
  Bool_t open(const char*,Text_t* status="in");
  Bool_t isOpen();
  Bool_t isWritable();
  void close();
  void print();
  Bool_t read(HGeomMedia*);
  Bool_t read(HGeomSet*,HGeomMedia*);
  Bool_t read(HGeomHit*); 
  Bool_t write(HGeomMedia*);
  Bool_t write(HGeomSet* set);
  Bool_t readGeomConfig(HGeomInterface*);
  Bool_t readDetectorSetup(HGeomInterface*);
  Bool_t setSimulRefRun(const char*) {return kTRUE;}
  Bool_t setHistoryDate(const char*) {return kTRUE;}
private:
  ClassDef(HGeomAsciiIo,0) // Class for geometry I/O from ASCII file
};

#endif  /* !HGEOMASCIIIO_H */
