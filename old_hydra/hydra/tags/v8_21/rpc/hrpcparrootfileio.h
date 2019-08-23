#ifndef HRPCPARROOTFILEIO_H
#define HRPCPARROOTFILEIO_H
                                                                                              
#include "hdetparrootfileio.h"
#include "TFile.h"
#include "TArrayI.h"
                                                                                              
class HParRootFile;
class HParSet;
class HRpcCalPar;
class HRpcThresPar;
                                                                                              
class HRpcParRootFileIo : public HDetParRootFileIo {
public:
  HRpcParRootFileIo(HParRootFile* f);
  ~HRpcParRootFileIo();
  Bool_t init(HParSet*,Int_t*);
  Bool_t read(HRpcCalPar*,Int_t*);
  Bool_t read(HRpcThresPar*);   // No modules in this container
  ClassDef(HRpcParRootFileIo,1) // Class for RPC parameter I/O from ROOT file
};
                                                                                              
#endif  /*!HRPCPARROOTFILEIO_H*/

