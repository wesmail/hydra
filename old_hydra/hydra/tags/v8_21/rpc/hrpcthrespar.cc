//*-- AUTHOR Diego Gonzalez-Diaz
//*-- created : 01/02/2008 by Diego Gonzalez-Diaz

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// HRpcThresPar
//
// Container class for RPC thresholds
//
///////////////////////////////////////////////////////////////////////

using namespace std;
#include "hrpcthrespar.h"
#include "hpario.h"
#include "hdetpario.h"
#include <iostream>
#include <iomanip>
#include <ctype.h>
#include <stdlib.h>

ClassImp(HRpcThresChan)
ClassImp(HRpcThresBoard)
ClassImp(HRpcThresPar)

HRpcThresBoard::HRpcThresBoard() {
  // constructor creates an array of 128 channels
  array = new TObjArray(128);
  for(Int_t i=0;i<128;i++) array->AddAt(new HRpcThresChan(),i);
}

HRpcThresBoard::~HRpcThresBoard() {
  // destructor deletes the array of channels
  array->Delete();
  delete array;
}

void HRpcThresBoard::clear() {
  // calls the clear function for each channel
  for(Int_t i=0;i<128;i++) (*this)[i].clear();
}

HRpcThresPar::HRpcThresPar(const Char_t* name, const Char_t* title, const Char_t* context,
			   Int_t a_offset, Int_t a_nBoards)
  : HParSet(name,title,context) {
  offset  = a_offset;
  nBoards = a_nBoards;
  // constructor creates an empty array of size nBoards 
  array = new TObjArray(nBoards);
  for(Int_t i=0; i<nBoards; i++) array->AddAt(new HRpcThresBoard(),i);
  
}

HRpcThresPar::~HRpcThresPar() {
  // destructor
  array->Delete();
  delete array;
}

Bool_t HRpcThresPar::init(HParIo* inp,Int_t* set) {
  // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HRpcParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRpcThresPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HRpcParIo");
  if (out) return out->write(this);
  return -1;
}

void HRpcThresPar::clear() {
  // deletes all HRpcThresBoard objects from the array and resets the input versions
  array->Delete();
  status=kFALSE;
  resetInputVersions();
}

void HRpcThresPar::fill(Int_t id,Int_t chan, Float_t TofTh, Float_t TotTh) {
  // creates the HRpcThresBoard objects, if not existing, and fills the channel

  HRpcThresBoard* p=getBoard(id);
  if (!p) {
    p=new HRpcThresBoard();
    array->AddAt(p,id-offset);
  }
  HRpcThresChan* c=p->getChannel(chan);
  if (c) {
    c->fill(TofTh,TotTh);
  } else {
    Error("Thresholds filling","Invalid channel number %i (ignore)",chan);
  }
}

void HRpcThresPar::readline(const Char_t *buf, Int_t *set) {
  // reads one line from ASCII file I/O and fills the channel
  Int_t id, chan;
  Float_t TofTh, TotTh;
  sscanf(buf,"%i%i %f%f",&id,&chan,&TofTh,&TotTh);
  fill(id,chan,TofTh,TotTh);
}


void HRpcThresPar::putAsciiHeader(TString& header) {
  header=
    "#Parameter container for the thresholds\n"
    "#Format:\n"
    "#subeventId  TrbChannel  threshold_tof threshold_tot\n";
}

void HRpcThresPar::write(fstream& fout) {
  // writes the information of all non-zero HRpcThresBoard objects to the ASCII file

  for(Int_t i=0;i<nBoards;i++) {
    HRpcThresBoard& b=(*this)[i];
    if (b[0].getTofTh()<-100) continue; //If the first channel of the board is not initialized
                                        //it means that the board itself is not.

    for(Int_t j=0;j<b.getSize();j++) {
      HRpcThresChan& chan = b[j];
      if (chan.getTofTh()>=-100 && chan.getTotTh()>=-100) {//In principle only positive values are used
	fout<<setw(3)<<(offset+i)<<setw(5)<<j<<"   "
	    <<chan.getTofTh()<<setw(3)<<chan.getTotTh()<<"\n";
      }
      else fout<<"\n";
    }
  }
}

void HRpcThresPar::printParam() {
  // prints the container
  printf("Parameter container for the thresholds\n");
  printf("Format:\n");
  printf("subeventId  TrbChannel  threshold_tof threshold_tot\n");

  Text_t buf[155];
  Int_t n0 = getSize();
  for(Int_t i0=0; i0<n0; i0++) {    
    Int_t n1     = (*this)[i0].getSize();
    if((*this)[i0][0].getTofTh()<-100) continue; //If the first channel of the board is not initialized
                                                 //it means that the board itself is not.
    for(Int_t i1=0; i1<n1; i1++) {
      HRpcThresChan& pChan=(*this)[i0][i1];
      sprintf(buf, "%4i %4i %5.1f %5.1f\n",i0+offset, i1, pChan.getTofTh(), pChan.getTotTh());
      printf(buf,strlen(buf));
    }
  }
}
