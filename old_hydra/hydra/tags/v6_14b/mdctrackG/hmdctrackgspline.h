#ifndef HMDCTRACKGSPLINE_H
#define HMDCTRACKGSPLINE_H
#include "TNamed.h"
#include <fstream>
#include "TString.h"
#include "hmdctrackgfield.h"
#include "hmdctrackgcorrections.h"
#include "hmdctrackgcorrpar.h"
#include "hgeomvector.h"
#include "hmdcseg.h"
#include "hkickplane2.h"
class HMdcTrackGSpline : public TNamed{
private:
   Double_t distfield;
   Double_t pi;
   Int_t polarity;//polarity of particle
   Double_t target;//targets z position in mm    
   Int_t N,N1,N2;//number of points in the field region
   Double_t Yequation[48],x[12];
   Double_t equationz[48],equationy[48],equationx[48];
   Double_t Bx[48],By[48],Bz[48];//Field components
   Double_t phi,teta;//in radian
   Double_t XZ[4],YZ[4],XZP[3],YZP[3];//parameters of spline fit
   Double_t FXZ[48],FYZ[48],FXZP[47],FYZP[47];//parameters of spline fit
   Double_t dydz[50],DYDZ[48],dxdz[50],DXDZ[48];
   Double_t x2[50],zz2[50],z2[50],y2[50];
   Double_t precon; //reconstructed momentum
   Double_t dist[4];
   Double_t metaDistance;
   Double_t tetadig,phidig,phidigg,teta2dig,phi2dig;//in degrees
   HMdcTrackGField*  b;
   HKickPlane2 *kickplane;
   HMdcTrackGCorrections* c;
   HGeomVector point[4]; //Intitial points from MDC's
   HGeomVector fieldpoints[48],Tfieldpoints[48],BB[48];//points selected in field region
   HGeomVector field[48];//field components 
   void getPoint();
   void transteta();
   void spline(HGeomVector *,Double_t *,Double_t *,Double_t *,Double_t *,Int_t);
   void init1();
   void getpoints();
   void fieldequation();
   void solveMomentum();
   void getMomentum(Bool_t);
   void transFieldpoints();
   void getcorrE(Int_t,Int_t,Int_t,Double_t &);
   void getcorrP(Int_t,Int_t,Int_t,Double_t &);
   
   

//the equaation of Line in ZX plane in the form X=A*Z+B;
//This part is needed for matching;
public:

void calcSegPoints(HMdcSeg *seg[2], HGeomVector *);
   void calcSegPoints(HMdcSegSim *seg[2], HGeomVector *);
   void calcSegPoints123(HMdcSeg *seg[2], HGeomVector *);
   void calcSegPoints123(HMdcSegSim *seg[2], HGeomVector *);


   Int_t getPolarity(){return polarity;}
   void equationofLine(Double_t*  ,Double_t*,Double_t ,Double_t &);
   void equationofLine3D(HGeomVector,HGeomVector,Double_t,Double_t &,Double_t &);   
   
   
   Double_t getDistField() {return distfield;}
   HGeomVector getPointOne() {return point[1];}

//   void calculateLinePoint(Double_t,Double_t &);//corresponding X for given Z
//////////////////////////////   



   //HGeomVector calcInter(Double_t,Double_t,Double_t,HGeomVector,HGeomVector,HGeomVector &);
   
   void calcInter(Double_t,Double_t,Double_t,HGeomVector,HGeomVector,HGeomVector &);
//public:
  /// void initcont
   HMdcTrackGSpline(const char * name="Spline",const char * title="Spline");
   ~HMdcTrackGSpline();
   void     setDataPointer(HMdcTrackGField* field = 0,HMdcTrackGCorrections* corr = 0);
   void setKickPointer(HKickPlane2 *kickpointer=0);
   Double_t calcMomentum(HGeomVector *,Bool_t cond=kTRUE,Double_t targetz=0.);
   Double_t calcMomentum(HMdcSeg *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   Double_t calcMomentum(HMdcSegSim *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   Double_t calcMomentum123(HMdcSeg *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   Double_t calcMomentum123(HMdcSegSim *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   void getDistance(Double_t  *);
   Double_t getMetaDistance(Double_t, Double_t, Double_t);
   ClassDef(HMdcTrackGSpline,0)
      };
#endif








