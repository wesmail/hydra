//*-- AUTHOR Bjoern Spruck
//*-- created : 19.4.06
// Modified for Wall by M.Golubeva 01.11.2006

#ifndef HWALLREFWINPAR_H
#define HWALLREFWINPAR_H

using namespace std;
#include "hparcond.h"
#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"
#include <iostream>
#include <iomanip>

#define WALL_REFWINPAR 2

class HWallRefWinPar : public HParCond 
{
protected:
   Float_t time_ref_win_low; // Lower bound of time ref window
   Float_t time_ref_win_high;// Upper bound of time ref window
public:
   HWallRefWinPar(const char* name="WallRefWinPar",
                  const char* title="Reference time windows parameters for Forward Wall",
                  const char* context="WallOneHitProduction");
  ~HWallRefWinPar() {;}
   Float_t getRefWinLow(void){  return time_ref_win_low; }
   Float_t getRefWinHigh(void){ return time_ref_win_high;}

   void setRefWinLow( Float_t s){ time_ref_win_low =s;}
   void setRefWinHigh(Float_t s){ time_ref_win_high=s;}

   void fill(Float_t tl, Float_t th) {
     time_ref_win_low  = tl;
     time_ref_win_high = th;
   }
   void fill(Float_t *s) {
      time_ref_win_low =s[0];
      time_ref_win_high=s[1];
   }

   Int_t  getSize() { return WALL_REFWINPAR; }
   Bool_t init(HParIo* input,Int_t* set=0);
   Int_t  write(HParIo*);
   void   clear();
   //void printParam();
   //void readline(const char*, Int_t*);
   //void putAsciiHeader(TString&);
   //Bool_t writeline(char*, Int_t);
   void   putParams(HParamList*);
   Bool_t getParams(HParamList*);
   ClassDef(HWallRefWinPar,1) // Container for the WALL RefWin parameters
};

#endif  /*!HWALLREFWINPAR_H*/
