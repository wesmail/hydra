//*-- AUTHOR : Ilse Koenig
//*-- Modified : 02/11/98

/////////////////////////////////////////////////////////////
//HParIo
//
//  Base class for different parameter input/output sources:
//  Oracle, Rootfiles, Ascifiles
//  It contains a list of detector I/Os.
//
/////////////////////////////////////////////////////////////

#include "hpario.h"
#include "hdetpario.h"

ClassImp(HParIo)

HParIo::HParIo() {
  //default constructor creates an empty list of detector I/Os
  detParIoList=0;
}

HParIo::~HParIo() {
  // default destructor
  detParIoList->Delete();
  detParIoList=0;
}

void HParIo::setDetParIo(HDetParIo* detParIo) {
  // stores pointer of the input/output class for a detector
  // used for I/O from ROOT file or Asci file
  Text_t* detName=detParIo->GetName();
  if (!detParIoList->FindObject(detName)) detParIoList->Add(detParIo);
}

void HParIo::setInputNumber(Int_t num) {
  // sets in all detector I/Os the number of the input
  TIter next(detParIoList);
  HDetParIo* io;
  while ((io=(HDetParIo*)next())) {
    io->setInputNumber(num);
  }
} 

HDetParIo* HParIo::getDetParIo(Text_t* detName) {
  // returns pointer to input/output class for a detector
  return ((HDetParIo*)detParIoList->FindObject(detName));
}

void HParIo::removeDetParIo(Text_t* detName) {
  // removes input/output class for a detector
  TObject* p=detParIoList->FindObject(detName);
  if (p) {
    delete p;
    p=0;
  }
}
