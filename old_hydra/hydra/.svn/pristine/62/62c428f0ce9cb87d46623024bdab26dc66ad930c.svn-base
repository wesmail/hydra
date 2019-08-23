#ifndef HMDCTRACKGSPLINE_H
#define HMDCTRACKGSPLINE_H
#include "TNamed.h"
#include <iostream>
#include <fstream>
#include "TString.h"
#include "hmdctrackgfield.h"
#include "hmdctrackgcorrections.h"
#include "hmdctrackgcorrpar.h"
#include "hgeomvector.h"
#include "hmdcseg.h"
#include "hkickplane2.h"
#include "hmdcgetcontainers.h"
class HMdcTrackGSpline : public TNamed{
private:
   Double_t pi;
   Double_t XX,YY,ZZ;
   Double_t XXdir,YYdir,ZZdir;
   Double_t distfield;
   Double_t errorY;
   Int_t polarity;//polarity of particle
   Double_t target;//targets z position in mm    
   Int_t N,N1,N2;//number of points in the field region
   Double_t *Yequation,*Xequation,*x;
   Double_t *equationz,*equationy,*equationx;
   Double_t *Bx,*By,*Bz;//Field components
   Double_t phi,teta;//in radian
   Double_t *XZ,*YZ,*XZP,*YZP;//parameters of spline fit
   Double_t *FXZ,*FYZ,*FXZP,*FYZP;//parameters of spline fit
   Double_t *dydz,*dxdz;
   Double_t *x2,*zz2,*z2,*y2;
   Double_t precon,preconErr; //reconstructed momentum
   Double_t precon111;
   Double_t *dist;
   Double_t metaDistance;
   Double_t tetadig,phidig,phidigg,teta2dig,phi2dig;//in degrees
   HMdcTrackGField*  b;
   HKickPlane2 *kickplane;
   HMdcTrackGCorrections* c;
   HGeomVector *point; //Intitial points from MDC's
   HGeomVector *fieldpoints,*secDer,*Tfieldpoints,*BB;//points selected in field region
   HGeomVector *field;//field components 
   Float_t fScal;
   void getPoint();
   void transteta();
   void transPhi();
   void spline(HGeomVector *,Double_t *,Double_t *,Double_t *,Double_t *,Int_t);
   void init1();
   void getpoints();
   void fieldequation();
   void solveMomentum();
   Int_t getMomentum(Bool_t,Int_t);
   void transFieldpoints();
   void getcorrE(Int_t,Int_t,Int_t,Double_t &,Int_t);
   void getcorrP(Int_t,Int_t,Int_t,Double_t &,Int_t);
   Double_t det3(Double_t a[3][3]);
   Double_t qSpline;
   void initParams(Int_t,Int_t);
   void clearParams();
   void remove(Double_t *);
   void remove(HGeomVector *);
   Double_t middleA,middleB,middleD;
   void calcErrors(Double_t *);
public:  
   Double_t getFieldDistance();
   Double_t getqSpline() const {return qSpline;}
   Double_t getErrP() {return preconErr;}
   void calcSegPoints(HMdcSeg *seg[2], HGeomVector *);
   void mixPoints(HGeomVector &, HGeomVector &);
   void calcSegPoints123(HMdcSeg *seg[2], HGeomVector *);
   Int_t getPolarity() const {return polarity;}
   void setMagnetScaling(Float_t FScal) {fScal=FScal;}
   void equationofLine(Double_t*  ,Double_t*,Double_t ,Double_t &);
   void equationofLine3D(HGeomVector,HGeomVector,Double_t,Double_t &,Double_t &); 
   HGeomVector calcMetaDir(HMdcSeg *, HGeomVector );
   //Calculates direction from kickplane to META, META hits are suposed
   //to be in sector coordinate system;
   HGeomVector calcKickIntersection(HMdcSeg *);
   //calculates intersection of segment with kickplane;
   Double_t getDistField() const {return distfield;}
   HGeomVector getPointOne() const {return point[1];}
   void calcInter(Double_t,Double_t,Double_t,HGeomVector,HGeomVector,HGeomVector &);
   HMdcTrackGSpline(const char * name="Spline",const char * title="Spline");
   ~HMdcTrackGSpline();
   void setDataPointer(HMdcTrackGField* field = 0,HMdcTrackGCorrections* corr = 0);
   void setCorrPointer(HMdcTrackGCorrections* corr=0);
   void setKickPointer(HKickPlane2 *kickpointer=0);
   Double_t calcMomentum(HGeomVector *,Bool_t cond=kTRUE,Double_t targetz=0.,Int_t nch=4);
   Double_t calcMomentum(HMdcSeg *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   Double_t calcMomentum123(HMdcSeg *seg[2],Bool_t cond=kTRUE,Double_t targetz=0.);
   void getDistance(Double_t  *);
   Double_t getMetaDistance(Double_t, Double_t, Double_t);
   void transSpline(Double_t,Double_t,HGeomVector &,HGeomVector &);
   void initMiddleParams(HGeomVector);
   HGeomVector takeMiddleParams( HMdcGetContainers *,Int_t,Int_t);
   ClassDef(HMdcTrackGSpline,0)
      };
#endif



     
     



