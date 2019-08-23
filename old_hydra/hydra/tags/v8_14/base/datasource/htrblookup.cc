//*-- AUTHOR Ilse Koenig
//*-- created : 25/01/2006 by Ilse Koenig

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// HStartLookup
//
// Lookup table for TRB unpacker to map the TRB address
//     subevent id (800..899), channel (0..127)
// to the detector address
//     detector, sector, module, cell side.
//
///////////////////////////////////////////////////////////////////////

using namespace std;
#include "htrblookup.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <stdlib.h>

ClassImp(HTrbLookupChan)
ClassImp(HTrbLookupBoard)
ClassImp(HTrbLookup)

HTrbLookupBoard::HTrbLookupBoard() {
  // constructor creates an array of 128 channels
  array = new TObjArray(128);
  for(Int_t i=0;i<128;i++) array->AddAt(new HTrbLookupChan(),i);
}

HTrbLookupBoard::~HTrbLookupBoard() {
  // destructor deletes the array of channels
  array->Delete();
  delete array;
}

void HTrbLookupBoard::clear() {
  // calls the clear function for each channel
  for(Int_t i=0;i<128;i++) (*this)[i].clear();
}

HTrbLookup::HTrbLookup(const char* name,
                       const char* title,
		       const char* context,
                       Int_t minSubeventId,
                       Int_t nBoards)
           : HParSet(name,title,context) {
  // constructor creates an empty array of size nBoards 
  arrayOffset=minSubeventId;
  array = new TObjArray(nBoards);
}

HTrbLookup::~HTrbLookup() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HTrbLookup::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HSpecParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HTrbLookup::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HSpecParIo");
  if (out) return out->write(this);
  return -1;
}

void HTrbLookup::clear() {
  // deletes all HTrbLookupBoard objects from the array and resets the input versions
  array->Delete();
  status=kFALSE;
  resetInputVersions();
}

void HTrbLookup::printParam() {
  // prints the lookup table
  printf("Lookup table for the TRB unpacker\n");
  printf("subeventId  channel  detector  sector  module  cell  side\n");
  printf("detectors: T-TOF, F-Tofino, S-Start, H-Hodo, W-ForwardWall, R-RPC)\n");
  printf("side: l-left, r-right, m-middle, u-up, d-down\n");
  for(Int_t i=0;i<=array->GetLast();i++) {
    HTrbLookupBoard* b=(*this)[i];
    if (b) {
      for(Int_t j=0;j<b->getSize();j++) {
        HTrbLookupChan& chan=(*b)[j];
        Char_t detector=chan.getDetector();
        if (detector!='\0') {
          printf("%5i%5i    %c%3i%4i%6i     %c\n",
                 arrayOffset+i,j,detector,
                 chan.getSector(),chan.getModule(),chan.getCell(),chan.getSide());
        }
      }
    }
  }
}
Bool_t HTrbLookup::fill(Int_t id,Int_t chan,Char_t det,
                      Int_t sec,Int_t mod,Int_t cell,Char_t side) {
  // creates the HTrbLookupBoard objects, if not existing, and fills the channel
  Bool_t rc=kFALSE;
  HTrbLookupBoard* p=getBoard(id);
  if (!p) {
    p=new HTrbLookupBoard();
    array->AddAt(p,id-arrayOffset);
  }
  HTrbLookupChan* c=p->getChannel(chan);
  if (c) {
    c->fill(det,sec,mod,cell,side);
    rc=kTRUE;
  } else {
    Error("fill","Invalid channel number %i",chan);
  }
  return rc; 
}

Bool_t HTrbLookup::readline(const char *buf) {
  // decodes one line read from ASCII file I/O and fills the channel
  Bool_t rc=kFALSE;
  Int_t id, chan, sec, mod, cell;
  Char_t det='\0', side='\0';
  sscanf(buf,"%i%i %c%i%i%i %c",&id,&chan,&det,&sec,&mod,&cell,&side);
  if (det=='T'||det=='F'||det=='S'||det=='H'||det=='W'||det=='R') {
    if (side=='\0'||side=='l'||side=='r'||side=='m'||side=='u'||side=='d') {
      rc=fill(id,chan,det,sec,mod,cell,side);
    } else {
      Error("readline","Unknown side %c\n",side);
    }
  } else {
    Error("readline","Unknown detector %c\n",det);
  }
  return rc;
}

void HTrbLookup::putAsciiHeader(TString& header) {
  // puts the ASCII header to the string used in HSpecParAsciiFileIo
  header=
    "# Lookup table for the TRB unpacker\n"
    "# Format:\n"
    "# subeventId  channel  detector  sector  module  cell  side\n"
    "# detectors: T-TOF, F-Tofino, S-Start, H-Hodo, W-ForwardWall, R-RPC)\n"
    "# side: l-left, r-right, m-middle, u-up, d-down\n";
}

void HTrbLookup::write(fstream& fout) {
  // writes the information of all non-zero HTrbLookupBoard objects to the ASCII file
  for(Int_t i=0;i<=array->GetLast();i++) {
    HTrbLookupBoard* b=(*this)[i];
    if (b) {
      for(Int_t j=0;j<b->getSize();j++) {
        HTrbLookupChan& chan=(*b)[j];
        Char_t detector=chan.getDetector();
        if (detector!='\0') {
          fout<<setw(3)<<(arrayOffset+i)<<setw(5)<<j<<"   "
              <<detector<<setw(4)<<chan.getSector()<<setw(4)
              <<chan.getModule()<<setw(6)<<chan.getCell();
          if (chan.getSide()!='\0') fout<<"  "<<chan.getSide()<<"\n";
          else fout<<"\n";
        }
      }
    }
  }
}
