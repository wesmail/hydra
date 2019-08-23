#ifndef HRPCCALSIM_H
#define HRPCCALSIM_H

#include "hrpccal.h"

class HRpcCalSim : public HRpcCal {

protected:
  Int_t RefL;       //index of hgeantrpc object, created in EBOX by the particle whose daughter made the left signal
  Int_t RefR;       //" " , created in EBOX by the particle whose daughter made the right signal
  Int_t RefLDgtr;   //" " , created in the gap EGij by the particle that contributed most to the left signal
  Int_t RefRDgtr;   //" " , created in the gap EGij by the particle that contributed most to the right signal

  Int_t TrackL;     //track for hgeantrpc object,created in EBOX by the particle whose daughter made the left signal
  Int_t TrackR;     //" " , created in EBOX by the particle whose daughter made the right signal
  Int_t TrackLDgtr; //" " , created in the gap EGij by the particle that contributed most to the left signal
  Int_t TrackRDgtr; //" " , created in the gap EGij by the particle that contributed most to the right signal

public:
  HRpcCalSim(void) : RefL(-1), RefR(-1), RefLDgtr(-1), RefRDgtr(-1),
    TrackL(-1), TrackR(-1), TrackLDgtr(-1), TrackRDgtr(-1) {}
  ~HRpcCalSim(void) {}
  void clear(void);
  
  void  setRefL(Int_t aRefL)             { RefL=aRefL; }
  void  setRefR(Int_t aRefR)             { RefR=aRefR; }
  void  setRefLDgtr(Int_t aRefLDgtr)     { RefLDgtr=aRefLDgtr; }
  void  setRefRDgtr(Int_t aRefRDgtr)     { RefRDgtr=aRefRDgtr; }

  void  setTrackL(Int_t aTrackL)         { TrackL=aTrackL; }
  void  setTrackR(Int_t aTrackR)         { TrackR=aTrackR; }
  void  setTrackLDgtr(Int_t aTrackLDgtr) { TrackLDgtr=aTrackLDgtr; }
  void  setTrackRDgtr(Int_t aTrackRDgtr) { TrackRDgtr=aTrackRDgtr; }
  
  Int_t getRefL()                        { return RefL; }
  Int_t getRefR()                        { return RefR; }
  Int_t getRefLDgtr()                    { return RefLDgtr; }
  Int_t getRefRDgtr()                    { return RefRDgtr; }

  Int_t getTrackL()                      { return TrackL; }
  Int_t getTrackR()                      { return TrackR; }
  Int_t getTrackLDgtr()                  { return TrackLDgtr; }
  Int_t getTrackRDgtr()                  { return TrackRDgtr; }

  ClassDef(HRpcCalSim,3)
};

#endif  /* HRPCCALSIM_H */

