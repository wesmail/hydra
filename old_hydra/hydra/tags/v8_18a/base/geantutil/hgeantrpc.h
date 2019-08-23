///////////////////////////////////////////////////////////////////////////
//  HGeantRpc
//
//  GEANT RPC event data
//
//  last modified on 10/08/2007 by D. Gonzalez-Diaz
///////////////////////////////////////////////////////////////////////////
#ifndef HGEANTRPC_H
#define HGEANTRPC_H

#include "hlinkeddataobject.h"

class HGeantRpc : public HLinkedDataObject
{
private:
  Int_t    trackNumber;     // GEANT track number
  Float_t  trackLength;     // track length at the RPC gap.                                  [mm]
  Float_t  loctrackLength;  // local track length (inside the gap).                          [mm]
  Float_t  eHit;            // energy deposited in the RPC gap.                              [MeV]
  Float_t  xHit;            // x at the RPC gap, in module ref. system (EBOX).               [mm]
  Float_t  yHit;            // y at the RPC gap, in module ref. system (EBOX).               [mm]
  Float_t  zHit;            // z at the RPC gap, in module ref. system (EBOX).               [mm]
  Float_t  tofHit;          // time of flight at the RPC gap.                                [ns]
  Float_t  momHit;          // momentum at the RPC gap.                                      [MeV/c]
  Float_t  thetaHit;        // polar angle at the RPC gap, in module ref. system (EBOX).     [deg]
  Float_t  phiHit;          // azimuthal angle at the RPC gap, in module ref. system (EBOX). [deg]
  Short_t  detectorID;      // detector ID (codified sector/column/cell/gap info)
  
public:
  HGeantRpc(void);
  HGeantRpc(HGeantRpc &aRpc);
  ~HGeantRpc(void);

  inline void   setTrack(Int_t atrackNumber) {trackNumber = atrackNumber;}
  inline void   setDetectorID (Short_t adetectorID) {detectorID = adetectorID;}
  inline void   setAddress    (Int_t sec, Int_t col, Int_t cel, Int_t gap);
         void   setHit(Float_t axHit, Float_t ayHit, Float_t azHit, Float_t atofHit, 
		Float_t amomHit, Float_t eHit);
         void   setIncidence(Float_t athetaHit, Float_t aphiHit);
	 void   setTLength(Float_t tracklength, Float_t loctracklength);

  inline Int_t  getSector(void) const;
  inline Int_t  getColumn(void) const;
  inline Int_t  getCell(void)   const;
  inline Int_t  getGap(void)    const;

  inline Int_t  getTrack(void)      {return trackNumber;}  
  inline Int_t  getDetectorID(void) {return detectorID;}
         void   getHit(Float_t& axHit, Float_t& ayHit, Float_t& azHit,
		       Float_t& atofHit, Float_t& amomHit, Float_t& aeHit);
	 void   getIncidence(Float_t& athetaHit, Float_t& aphiHit);
	 void   getTLength(Float_t& atracklength, Float_t& aloctracklength);
  inline Int_t  getNLocationIndex(void)   {return 4;}
  inline Int_t  getLocationIndex(Int_t i); 

  ClassDef(HGeantRpc,4)
};

inline Int_t  HGeantRpc::getSector(void) const{
  if (detectorID<0) return -1*(detectorID+1);
  else return detectorID>>11;  
}

inline Int_t  HGeantRpc::getColumn(void) const{
  if (detectorID<0) return -1;
  else return ((detectorID>>8)  & 7);
}

inline Int_t  HGeantRpc::getCell(void) const{
  if (detectorID<0) return -1;
  else return ((detectorID>>2)  & 63);
}

inline Int_t  HGeantRpc::getGap(void) const {
  if (detectorID<0) return -1;
  else return (detectorID & 3);
}

inline void HGeantRpc::setAddress(Int_t sec, Int_t col, Int_t cel, Int_t gap) {
  detectorID = (sec<<11) + (col<<8) + (cel<<2) + gap;  
}

inline Int_t HGeantRpc::getLocationIndex(Int_t i) {
  switch (i) {
    case 0 : return getSector();
    case 1 : return getColumn();
    case 2 : return getCell();
    case 3 : return getGap();
  }
  return -1;
}


#endif  /*! HGEANTRPC_H */











