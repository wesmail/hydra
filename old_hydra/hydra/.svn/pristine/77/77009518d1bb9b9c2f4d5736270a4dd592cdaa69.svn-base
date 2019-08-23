//*-- Author : Ilse Koenig
//*-- Modified: 14/05/01 by Peter Zumbruch

///////////////////////////////////////////////////////////////////////////////
// HMdcRaw
//
// Unpacked raw data of the MDC
// 
// In measurement mode the TDC accepts two hits per channel. The TDC can hereby
// trigger either on two leading edges (hit multiplicity nHits: -1 or -2) or
// on a leading and the trailing edge of a single pulse (nHits: +1 or +2).
// The TDC is able to generate internal calibration events. In this case a
// single channel sends 6 time informations (nHits: +6).
//
// The addresses of the cell can be accessed via the inline functions
//     void setSector(const Int_t n)
//     void setModule(const Int_t n)
//     void setMbo(const Int_t n)
//     void setTdc(const Int_t n)
//     void setAddress(const Int_t sector,const Int_t module,const Int_t mbo,
//                     const Int_t tdc)
//     Int_t getSector(void) const
//     Int_t getModule(void) const
//     Int_t getMbo(void) const
//     Int_t getTdc(void) const
//     void getAddress(Int_t& sector,Int_t& module,Int_t& mbo,Int_t& tdc)
// and the number of hits with the inline function
//     Int_t getNHits(void) const
//
// The inline function clear() sets the data data members to the following
// values:
//          nHits=0;
//          sector=module=mbo=tdc=-1;
//          time1=time2=time3=time4=time5=time6=-999;
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcraw.h"

ClassImp(HMdcRaw)

void HMdcRaw::setTime(const Int_t time,const Int_t mode) {
  // Stores the given time in the next data element time* and sets the
  // multiplicity.
  // The TDC can be operated in 2 different modes:
  // mode 0 (default) : trigger on leading and trailing edge
  //                    The multiplicity nHits is incremented by 1.
  // mode 1           : trigger on 2 leading edges:
  //                    The multiplicity nHits is decremented by 1.
  switch (nHits) {
    case 0:
      time1 = time;
      if (mode) nHits--;
      else nHits++;
      break;
    case -1:
      time2 = time;
      nHits--;
      break;
    case -2:
      time3 = time;
      nHits--;
      break;
    case 1:
      time2 = time;
      nHits++;
      break;
    case 2:
      time3 = time;
      nHits++;
      break;
    case 3:
      time4 = time;
      nHits++;
      break;
    case 4:
      time5 = time;
      nHits++;
      break;
    case 5:
      time6 = time;
      nHits++;
      break;
    default:
      Error("HMdcRaw::setTime(...)","number of hits > 6\n");
      return;
  }
}

Int_t HMdcRaw::getTime(const Int_t n) const {
  // Returns the time of nth hit (1<=n<=6)
  switch (n) {
    case 1: return time1;
    case 2: return time2;
    case 3: return time3;
    case 4: return time4;
    case 5: return time5;
    case 6: return time6;
    default: return -1;
  }
}
