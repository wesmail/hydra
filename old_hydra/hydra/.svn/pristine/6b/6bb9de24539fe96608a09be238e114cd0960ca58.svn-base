// Created by M.Golubeva 21.11.2006
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////
// HWallRefWinPar
//
// Container class for Wall RefWin parameters
//
/////////////////////////////////////////////////////////////
using namespace std;
#include "hwallrefwinpar.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hwalldetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include <iostream>
#include <iomanip>

ClassImp(HWallRefWinPar)

HWallRefWinPar::HWallRefWinPar(const Char_t* name,const Char_t* title,
                           const Char_t* context)
             : HParCond(name,title,context)
{
  // constructor
    clear();
}

Bool_t HWallRefWinPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HWallRefWinPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return -1;
}
void HWallRefWinPar::clear() {
  // clears the container
  time_ref_win_low =0.F;
  time_ref_win_high=0.F;

  status=kFALSE;
  resetInputVersions();
}

void HWallRefWinPar::putParams(HParamList* l) {
  // puts all parameters to the parameter list, which is used by the io
  if (!l) return;
  //l->add("time_ref_win_low",&time_ref_win_low,5);
  //l->add("time_ref_win_high",&time_ref_win_high,5);
  l->add("time_ref_win_low",time_ref_win_low);
  l->add("time_ref_win_high",time_ref_win_high);
}

Bool_t HWallRefWinPar::getParams(HParamList* l) {
 // gets all parameters from the parameter list, which is used by the io
  if (!l) return kFALSE;
  //if (!(l->fill("time_ref_win_low",time_ref_win_low,5))) return kFALSE;
  //if (!(l->fill("time_ref_win_high",time_ref_win_high,5))) return kFALSE;
  if (!(l->fill("time_ref_win_low",&time_ref_win_low))) return kFALSE;
  if (!(l->fill("time_ref_win_high",&time_ref_win_high))) return kFALSE;

  return kTRUE;
}

/*
void HWallRefWinPar::printParam()
{
  // prints the calibration parameters
  printf("Reference time win parameters for the WALL detector\n");
  printf("module  refwin.low refwin.high\n");
  for(Int_t i=0;i<getSize();i++) {
    HWallRefWinParMod& mod=(*this)[i];
    printf("%4i%8.3f%8.3f\n",i,mod.getRefWinLow(),mod.getRefWinHigh());
  }
}
*/
/*
void HWallRefWinPar::readline(const Char_t *buf, Int_t *set)
{
  // decodes one line read from ascii file I/O
  //cout <<"MARINA HWallRefWinPar readline ------- " <<endl;
  Int_t mod;
  Float_t f[2]={-1,-1}; // Wlow, Whigh;
  sscanf(buf,"%i%f%f", &mod,&f[0],&f[1]);
  //cout <<"marina 1 " <<buf <<" " <<set[mod] <<endl;
  //cout <<"MARINA REFWIN ------- " <<(*this)[mod].getRefWinLow() <<endl;
  if (!set[mod]) return;
  //cout <<"marina 2 " <<mod <<" " <<f[0] <<" " <<f[1] <<endl;
  (*this)[mod].fill(f[0],f[1]);
  //cout <<"marina 3 " <<endl;
  set[mod]=999;
  //cout <<"marina 4 " <<endl;

}
*/
/*
void HWallRefWinPar::putAsciiHeader(TString& header)
{
  // puts the ascii header to the string used in HWallParAsciiFileIo
  header=
    "# Reference time Window parameters for the WALL detector\n"
    "# Format:\n"
    "# module   refwin.low   refwin.high\n";
}
*/
/*
Bool_t HWallRefWinPar::writeline(Char_t *buf, Int_t m)
{
  // writes one line to the buffer used by ascii file I/O
  HWallRefWinParMod& mod=(*this)[m];
  sprintf(buf,"%4i%8.3f%8.3f\n",
              m, mod.getRefWinLow(),mod.getRefWinHigh());
  return kTRUE;
}
*/
