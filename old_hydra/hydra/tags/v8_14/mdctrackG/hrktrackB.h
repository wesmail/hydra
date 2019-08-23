//*-- Author : A.Sadovsky (04.11.2004)
#ifndef HRKTRACKB_H
#define HRKTRACKB_H

#include "TObject.h"
#include "hbasetrack.h"

class HRKTrackB : public HBaseTrack{
private:
  Double_t chiq;       // Quality of Runge-Kutta track propagation
  Float_t zSeg1RK;     // z of inner segment from Runge Kutta fit
  Float_t rSeg1RK;     // r of inner segment from Runge Kutta fit
  Float_t thetaSeg1RK; // theta of inner segment from Runge Kutta fit
  Float_t phiSeg1RK;   // phi of inner segment from Runge Kutta fit
  Float_t zSeg2RK;     // z of outer segment from Runge Kutta fit
  Float_t rSeg2RK;     // r of outer segment from Runge Kutta fit
  Float_t thetaSeg2RK; // theta of outer segment from Runge Kutta fit
  Float_t phiSeg2RK;   // phi of outer segment from Runge Kutta fit
  Float_t dxRkMeta;    // x_RK_intersectionPointOnMeta - x_MetaHit
  Float_t dyRkMeta;    // y_RK_intersectionPointOnMeta - y_MetaHit
  Float_t dzRkMeta;    // z_RK_intersectionPointOnMeta - z_MetaHit)


public:
  HRKTrackB(void);
  ~HRKTrackB(void) {}
 
  //-set-functions--
  void setChiq(Double_t c)       { chiq=c;}
  void setZSeg1RK(Float_t v)     { zSeg1RK=v; }
  void setRSeg1RK(Float_t v)     { rSeg1RK=v; }
  void setThetaSeg1RK(Float_t v) { thetaSeg1RK=v; }
  void setPhiSeg1RK(Float_t v)   { phiSeg1RK=v; }
  void setZSeg2RK(Float_t v)     { zSeg2RK=v; }
  void setRSeg2RK(Float_t v)     { rSeg2RK=v; }
  void setThetaSeg2RK(Float_t v) { thetaSeg2RK=v; }
  void setPhiSeg2RK(Float_t v)   { phiSeg2RK=v; }
  void setMETAdx(Float_t x)      { dxRkMeta=x;}
  void setMETAdy(Float_t y)      { dyRkMeta= y;}
  void setMETAdz(Float_t z)      { dzRkMeta=z;}

  //-get-functions--
  Float_t getChiq(void)        { return chiq; }
  Float_t getZSeg1RK(void)     { return zSeg1RK; }
  Float_t getRSeg1RK(void)     { return rSeg1RK; }
  Float_t getThetaSeg1RK(void) { return thetaSeg1RK; }
  Float_t getPhiSeg1RK(void)   { return phiSeg1RK; }
  Float_t getZSeg2RK(void)     { return zSeg2RK; }
  Float_t getRSeg2RK(void)     { return rSeg2RK; }
  Float_t getThetaSeg2RK(void) { return thetaSeg2RK; }
  Float_t getPhiSeg2RK(void)   { return phiSeg2RK; }
  Float_t getMETAdx(void)      { return dxRkMeta; }
  Float_t getMETAdy(void)      { return dyRkMeta; }
  Float_t getMETAdz(void)      { return dzRkMeta; }
  
  ClassDef (HRKTrackB,2) // Runge-Kutta tracking data container
};
#endif
