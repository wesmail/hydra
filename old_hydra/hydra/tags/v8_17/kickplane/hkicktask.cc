#include "hkicktask.h"
#include "hkickparasciifileio.h"
#include "hparasciifileio.h"
#include "hpario.h"
#include "hruntimedb.h"
#include "TClass.h"
#include "hkickparrootfileio.h"
#include "hparrootfileio.h"

void HKickTask::setKickParInput(HParIo *io,Int_t inputN) {
  if (io != 0) {
    if (io->getDetParIo("HKickParIo") == 0) {	
      if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	HKickParAsciiFileIo* p=
	  new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	io->setDetParIo(p);
        io->setInputNumber(inputN);
      }
      if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	HKickParRootFileIo* p=
	  new HKickParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	io->setDetParIo(p);
        io->setInputNumber(inputN);
      }
      if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
        io->setDetParIo("HKickParIo");
        io->setInputNumber(inputN);
      }
    }
  }
}

void HKickTask::setKickParOutput(HParIo *io) {
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

void HKickTask::setKickParIo(HRuntimeDb *rtdb) {
  //Adds kickplane's Parameter Io to the runtime database.
  HParIo *io=0;

  io=rtdb->getFirstInput();
  setKickParInput(io,1);
  
  io=rtdb->getSecondInput();
  setKickParInput(io,2);
  
  io=rtdb->getOutput();
  setKickParOutput(io);
} 



ClassImp(HKickTask)
