#include "hkicktask.h"
#include "hkickparasciifileio.h"
#include "hparasciifileio.h"
#include "hpario.h"
#include "hruntimedb.h"
#include "TClass.h"
#include "hkickparrootfileio.h"
#include "hparrootfileio.h"

void HKickTask::setKickParIo(HRuntimeDb *rtdb) {
  //Adds kickplane's Parameter Io to the runtime database.
  HParIo *io=0;

  io=rtdb->getFirstInput();
  if (io != 0) {
    if (io->getDetParIo("HKickParIo") == 0) {	
      if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	HKickParAsciiFileIo* p=
	  new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	io->setDetParIo(p);
        io->setInputNumber(1);
      }
      if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	HKickParRootFileIo* p=
	  new HKickParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	io->setDetParIo(p);
        io->setInputNumber(1);
      }
      if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
        io->setDetParIo("HKickParIo");
        io->setInputNumber(1);
      }
    }
  }
  
  io=rtdb->getSecondInput();
  if (io != 0) {
    if (io->getDetParIo("HKickParIo") == 0) {	
      if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	HKickParAsciiFileIo* p=
	  new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	io->setDetParIo(p);
        io->setInputNumber(2);
      }
      if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	HKickParRootFileIo* p=
	  new HKickParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	io->setDetParIo(p);
        io->setInputNumber(2);
      }
      if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
        io->setDetParIo("HKickParIo");
        io->setInputNumber(2);
      }
    }
  }
  
  io=rtdb->getOutput();
  if (io != 0) {
    if (io->getDetParIo("HKickParIo") == 0) {	
      if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	HKickParAsciiFileIo* p=
	  new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	io->setDetParIo(p);
      }
      if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	HKickParRootFileIo* p=
	  new HKickParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	io->setDetParIo(p);
      }
      if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) io->setDetParIo("HKickParIo");
    }
  }
} 



ClassImp(HKickTask)
