#include "hkicktask.h"
#include "hkickparasciifileio.h"
#include "hparasciifileio.h"
#include "hpario.h"
#include "hruntimedb.h"
#include "TClass.h"

void HKickTask::setKickParIo(HRuntimeDb *rtdb) {
  //Adds kickplane's Parameter Io to the runtime database.
  HParIo *io=0;
  io=rtdb->getFirstInput();
  if (io != 0) {
    if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
      HKickParAsciiFileIo* p=
	new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
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
  }
  io=rtdb->getOutput();
  if (io != 0) {
    if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
      HKickParAsciiFileIo* p=
	new HKickParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
      io->setDetParIo(p);
    }
  }
} 


ClassImp(HKickTask)
