//*-- Author : A.Sadovsky (04.11.2004)
#ifndef HRKTRACKB_H
#define HRKTRACKB_H
#include "TObject.h"
#include "hbasetrack.h"
class HRKTrackB : public HBaseTrack{
private:
  Double_t chiq;     //-- Quality of Runge-Kutta track propagation
  Float_t  tofdist;  //-- Track length from Target-to-META detector

  Float_t  dxRkMeta; //-----------------------------------------------------------------------------------------------------------//
  Float_t  dyRkMeta; //-- {dx,dy,dz} difference between [RK-propagation coordinate on the surface of META pad] and [META hit coordinate]
  Float_t  dzRkMeta; //-- dx = ( x_RK_onMetaSubdetectorSurface_intersection - x_MetaHit) and so on for dy, dz

  ////----------only-for-test-information----------------------------------------------------------//
  //Float_t  tofdistSP; //TEST// Track length from Target-to-META detector
  //Short_t  system;    //TEST// system
  //Float_t  RKthe;  //TEST// Theta RK at vertex point
  //Float_t  RKphi;  //TEST// Phi   RK at vertex point
  //Float_t  RKvertThe;    //TEST// Theta RK at vertex point
  //Float_t  RKvertPhi;    //TEST// Phi   RK at vertex point
  //Float_t  RK_mdc12_the;   //TEST// Theta RK from MDC12 fitted by RungeKutta positions
  //Float_t  RK_mdc12_phi;   //TEST// Phi   RK from MDC12 fitted by RungeKutta positions
  //Float_t  KineThe;   //TEST// GeantKine Theta RK at vertex point
  //Float_t  KinePhi;   //TEST// GeantKine Phi   RK at vertex point
  //Int_t    pid;   //TEST// Particle_Id from GeantKine of best track from HMdcSeg1
  //Float_t  seg2xMeta; //TEST//
  //Float_t  seg2yMeta; //TEST// {dx,dy,dz} on META according to HMdcSeg2 line-to-meta approximation 
  //Float_t  seg2zMeta; //TEST// dx = ( x_mdc34Line_onMetaSubdetectorSurface_intersection - xMetaHit) and so on for dy, dz
  ////----------only-for-test-information----------------------------------------------------------//


public:
  HRKTrackB(void);
 ~HRKTrackB(void);
 
  //-set-functions--
  void setChiq(Double_t Chiq){ chiq=Chiq; }
  void setTofDist(  Float_t TOFDIST){tofdist   = TOFDIST; }
  void setMETAdx( Float_t x){ dxRkMeta = x; }
  void setMETAdy( Float_t y){ dyRkMeta = y; }
  void setMETAdz( Float_t z){ dzRkMeta = z; }

  //-get-functions--
  Float_t getChiq(void){    return chiq;    } // chi^2 of Runge Kutta tracking from x,y points (fitted minus measured) on each of the MDCs
  Float_t getTofDist(void){ return tofdist; } // momentum of track as a result of fit
  Float_t getMETAdx(void){ return dxRkMeta; } // x-deviation of track trajectory of Runge Kutta propagation from the measurement point by META detector
  Float_t getMETAdy(void){ return dyRkMeta; } // y-deviation of track trajectory of Runge Kutta propagation from the measurement point by META detector
  Float_t getMETAdz(void){ return dzRkMeta; } // z-deviation of track trajectory of Runge Kutta propagation from the measurement point by META detector

  //----------only-for-test-information-----------------------------------------------------//
  //void setTofDistSP(Float_t TOFDIST){tofdistSP = TOFDIST; }
  //void setSystem(   Short_t SYSTEM ){system    = SYSTEM;  }
  //void setRKthe( Float_t th ){ RKthe=th; }
  //void setRKphi( Float_t ph ){ RKphi=ph; }
  //void setRKvertThe( Float_t th ){ RKvertThe=th; }
  //void setRKvertPhi( Float_t ph ){ RKvertPhi=ph; }
  //void setRKmdc12_theta( Float_t th ){ RK_mdc12_the=th; }
  //void setRKmdc12_phi(   Float_t ph ){ RK_mdc12_phi=ph; }
  //void setKineThe( Float_t th ){ KineThe=th; }
  //void setKinePhi( Float_t ph ){ KinePhi=ph; }
  //void setSeg2METAdx( Float_t x){ seg2xMeta = x;}
  //void setSeg2METAdy( Float_t y){ seg2yMeta = y;}
  //void setSeg2METAdz( Float_t z){ seg2zMeta = z;}
  //void setPid( Int_t PID ){ pid = PID; }
  //----------only-for-test-information-----------------------------------------------------//                 
  
  
  ClassDef (HRKTrackB,1)
};
#endif
