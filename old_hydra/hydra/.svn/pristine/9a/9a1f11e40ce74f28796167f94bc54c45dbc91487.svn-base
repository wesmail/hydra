#ifndef HMDCCAL1SIM_H
#define HMDCCAL1SIM_H

#include "hmdccal1.h"

class HMdcCal1Sim : public HMdcCal1 {
protected:
  Int_t nTrack1;   // number of Track for first hit
  Int_t nTrack2;   // number of Track for second hit
  Int_t status1;    // status of cell (e.g. 1 if ok)
  Int_t status2;    // status of cell (e.g. 1 if ok)
  Int_t listTrack [5]; // list of 5 tracknumbers for tracks
  Int_t listStatus[5]; // list of status for 5 tracks
public:
  HMdcCal1Sim(void) : nTrack1(-99), nTrack2(-99), status1(0), status2(0) {}
  ~HMdcCal1Sim(void) {}
  void clear(void);
  void setNTrack1(const Int_t n) {nTrack1=n;}
  void setNTrack2(const Int_t n) {nTrack2=n;}
  void setStatus1(const Int_t f) { status1=f; }
  void setStatus2(const Int_t f) { status2=f; }
  void setTrackList(Int_t* array)
  {
   for(Int_t i=0;i<5;i++)
     {
	 if(array[i]!=0 && array[i]>=0)
	 {
	     listTrack[i]=array[i];
	 }
	 else
	 {
	     listTrack[i]=-99;
	 }
     }
  };
  void getTrackList(Int_t* array)
  {
   for(Int_t i=0;i<5;i++)
     {
	 array[i]=listTrack[i];
     }
  };
  void setStatusList(Int_t* array)
  {
   for(Int_t i=0;i<5;i++)
     {
	 listStatus[i]=array[i];
     }
  };
  void getStatusList(Int_t* array)
  {
   for(Int_t i=0;i<5;i++)
     {
	 array[i]=listStatus[i];
     }
  };
  Int_t getNTracks()
  {
      Int_t i=0;
      while(listTrack[i]!=-99 && i<5)
      {
	  i++;
      }
      return i;
  };
  Int_t* getStatusList(){return (Int_t*)listStatus;}
  Int_t* getTrackList() {return (Int_t*)listTrack;}

  Int_t getTrackFromList (Int_t element){return listTrack [element];}
  Int_t getStatusFromList(Int_t element){return listStatus[element];}
  Int_t getNTrack1(void) const {return nTrack1;}
  Int_t getNTrack2(void) const {return nTrack2;}
  Int_t getStatus1(void) const { return status1; }
  Int_t getStatus2(void) const { return status2; }
  Int_t getStatus(void) const { return status1; }  // will be removed later

  ClassDef(HMdcCal1Sim,1) // simulated cal1 hit on a MDC
};

#endif  /* HMDCCAL1SIM_H */

