#include "hkicktask.h"
#include "hkickparasciifileio.h"
#include "hparasciifileio.h"
#include "hpario.h"
#include "hruntimedb.h"
#include "TClass.h"
#include "hkickparrootfileio.h"
#include "hparrootfileio.h"

Bool_t HKickTask::fParIoInitialized = kFALSE;

void HKickTask::setKickParIo(HRuntimeDb *rtdb) {
  //Adds kickplane's Parameter Io to the runtime database.
  HParIo *io=0;

  if (!fParIoInitialized) {
    fParIoInitialized = kTRUE;
    io=rtdb->getFirstInput();
    if (io != 0) {
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
    }
    io=rtdb->getSecondInput();
    if (io != 0) {
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
    }
    io=rtdb->getOutput();
    if (io != 0) {
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
    }
  } 
}


ClassImp(HKickTask)
