//*-- AUTHOR : A.Rustamov
///////////////////////////////////////////////////////////////////////////
//HMdcTrackGSpline calculates momentum of particle using spline interpolation
//Input data are 4 points  from MDC chambers
using namespace std;
#include <iostream>
#include "hmdctrackgspline.h"
ClassImp(HMdcTrackGSpline)
   HMdcTrackGSpline::HMdcTrackGSpline(const char* name, const char * title ):TNamed(name,title)
{
   N=4;
   N1=50;
   N2=N1-2;
   pi=acos(-1.);
//   polarity=1;
}
HMdcTrackGSpline::~HMdcTrackGSpline() {;}
Double_t det3(Double_t a[3][3])
{
   double sp;
   sp=(a[0][0]*a[1][1]*a[2][2]+a[1][0]*a[2][1]*a[0][2]+
       a[0][1]*a[1][2]*a[2][0]-a[0][2]*a[1][1]*a[2][0]-
       a[2][1]*a[1][2]*a[0][0]-a[1][0]*a[0][1]*a[2][2]);
   return sp; 
}
void HMdcTrackGSpline::init1()
{
   Double_t BBy[N2];
   Double_t fi=60*pi/180;
   for(Int_t i=0;i<N2;i++)
      {
	 BBy[i]=Bx[i]*sin(fi)+By[i]*cos(fi);
	 Bx[i]=Bx[i]*cos(fi)-By[i]*sin(fi);      
	 BB[i].setXYZ(Bx[i],BBy[i],Bz[i]);
      }
}
void HMdcTrackGSpline::transteta()
{
   for (int i=0;i<48;i++)
      {
	 DYDZ[i]=(DYDZ[i]-tan(teta))/(DYDZ[i]*tan(teta)+1);
	 Double_t xxx=fieldpoints[i].getX();
	 Double_t yyy=fieldpoints[i].getY();
	 Double_t zzz=fieldpoints[i].getZ();
	 Double_t xx2=cos(teta)*yyy-sin(teta)*zzz;
	 Double_t xx3=sin(teta)*yyy+cos(teta)*zzz;
	 fieldpoints[i].setXYZ(xxx,xx2,xx3);
      } 
}
void HMdcTrackGSpline::getPoint()
{
   point[0].setXYZ(x[0],x[1],x[2]);
   point[1].setXYZ(x[3],x[4],x[5]);
   point[2].setXYZ(x[6],x[7],x[8]);
   point[3].setXYZ(x[9],x[10],x[11]);
   //   Double_t rr=sqrt(x[0]*x[0]+x[1]*x[1]+x[2]*x[2]);
   Double_t rr=sqrt(x[0]*x[0]+x[1]*x[1]+(x[2]-target)*(x[2]-target));
   phi=atan2(x[1],x[0]);
   //   teta=acos(x[2]/rr);
   teta=acos((x[2]-target)/rr);
   
   Double_t r2=sqrt((x[9]-x[6])*(x[9]-x[6])+
		    (x[10]-x[7])*(x[10]-x[7])+
		    (x[11]-x[8])*(x[11]-x[8]));    
   teta2dig=acos((x[11]-x[8])/r2)*180/pi;
   phi2dig=atan((x[10]-x[7])/(x[9]-x[6]))*180/pi;
   //   tetadig=acos(x[2]/rr)*180/pi;
   tetadig=acos((x[2]-target)/rr)*180/pi;
   if (tetadig<18) tetadig=18;
   phidig=atan2(x[1],x[0])*180/pi;
   phidigg=phidig;
   phi-=pi/2;
}
void HMdcTrackGSpline::transFieldpoints()
{
   Double_t x,y,xx,yy,zz;
   Double_t fi=60*pi/180;
   for (Int_t i=0;i<N2;i++)
      {
	 x=fieldpoints[i].getX();
	 y=fieldpoints[i].getY();
	 xx=cos(fi)*x+sin(fi)*y;
	 yy=-sin(fi)*x+cos(fi)*y;
	 zz=fieldpoints[i].getZ();
	 Tfieldpoints[i].setXYZ(xx,yy,zz);
      }
}
void HMdcTrackGSpline::spline(HGeomVector *p, Double_t *Y,Double_t *YP,
			      Double_t *X,Double_t *XP,Int_t size)
{
   Double_t P[size],R[size],Q[size],h[size];
   Double_t t1,t2;
   for (Int_t i = 0; i<size-1; i++)
      h[i] = p[i+1].getZ() - p[i].getZ();
   for(Int_t i=0;i<size-2;i++)
      R[i]=3*((p[i+2].getY()-p[i+1].getY())/(h[i+1])-(p[i+1].getY()-
						      p[i].getY())/(h[i]))/(h[i]+h[i+1]);
   P[0]=1;
   Q[0]=0;
   for (Int_t i=0;i<size-2;i++)
      {
	 t1=h[i]/(2*(h[i]+h[i+1]));
	 t2=h[i+1]/(2*(h[i]+h[i+1]));
	 P[i+1]= -t2/(1+t1*P[i]);
	 Q[i+1]=(R[i]-t1*Q[i])/(1+t1*P[i]);  
      }
   Y[size-1]=Q[size-2]/(1-P[size-2]);
   Y[size-2]=Y[size-1];
   for (Int_t i=size-3;i>0;i--)
      Y[i]=P[i]*Y[i+1]+Q[i];
   Y[0]=Y[1];
   for (Int_t i=0;i<size-1;i++)
      YP[i]=(p[i+1].getY()-p[i].getY())/(h[i])-h[i]*
	 Y[i]/3-h[i]*Y[i+1]/6;
   ///////////////////////////
	 for(Int_t i=0;i<size-2;i++)
	    R[i]=3*((p[i+2].getX()-p[i+1].getX())/(h[i+1])-
		    (p[i+1].getX()-
		     p[i].getX())/(h[i]))/(h[i]+h[i+1]);
	 P[0]=1;
	 Q[0]=0;
	 for (Int_t i=0;i<size-2;i++)
	    {
	       t1=h[i]/(2*(h[i]+h[i+1]));
	       t2=h[i+1]/(2*(h[i]+h[i+1]));
	       P[i+1]= -t2/(1+t1*P[i]);
	       Q[i+1]=(R[i]-t1*Q[i])/(1+t1*P[i]);  
	    }
	 X[size-1]=Q[size-2]/(1-P[size-2]);
	 X[size-2]=X[size-1];
	 for (Int_t i=size-3;i>0;i--)
	    X[i]=P[i]*X[i+1]+Q[i];
	 X[0]=X[1];
	 for (Int_t i=0;i<size-1;i++)
	    XP[i]=(p[i+1].getX()-p[i].getX())/(h[i])-h[i]*
	       X[i]/3-h[i]*X[i+1]/6;      
}      
void HMdcTrackGSpline::getpoints()
{
  distfield=0;
  Double_t x2[100];
  Double_t h=(point[2].getZ()-point[1].getZ())/(N1);
  
  dist[0]=sqrt(point[0].getX()*point[0].getX()+
               point[0].getY()*point[0].getY()+
               point[0].getZ()*point[0].getZ());
  dist[1]=dist[0]+sqrt((point[1].getX()-point[0].getX())*
                       (point[1].getX()-point[0].getX())+
                       (point[1].getY()-point[0].getY())*
                       (point[1].getY()-point[0].getY())+
                       (point[1].getZ()-point[0].getZ())*
                       (point[1].getZ()-point[0].getZ()));
  for(Int_t i=0;i<N1;i++)
     {
	//points along Z axis
	z2[i]=point[1].getZ()+i*h;
	//points in YZ projection
	y2[i]=point[1].getY()+(z2[i]-point[1].getZ())*YZP[1]
	   +(z2[i]-point[1].getZ())*(z2[i]-point[1].getZ())*YZ[1]/(2)
	   +(z2[i]-point[1].getZ())
	   *(z2[i]-point[1].getZ())
	   *(z2[i]-point[1].getZ())*
	   (YZ[2]-YZ[1])/(6*(point[2].getZ()-point[1].getZ()));
	
	//points int XZ projection
	
	x2[i]=point[1].getX()+(z2[i]-point[1].getZ())*XZP[1]
	   +(z2[i]-point[1].getZ())*(z2[i]-point[1].getZ())*XZ[1]/(2)
	   +(z2[i]-point[1].getZ())
	   *(z2[i]-point[1].getZ())
	   *(z2[i]-point[1].getZ())*
	   (XZ[2]-XZ[1])/(6*(point[2].getZ()-point[1].getZ()));
	dydz[i]=YZP[1]+
	   2*(z2[i]-point[1].getZ())*YZ[1]/(2)+
	   3*(z2[i]-point[1].getZ())*
	   (z2[i]-point[1].getZ())*
	   (YZ[2]-YZ[1])/(6*(point[2].getZ()-point[1].getZ()));
	dxdz[i]=XZP[1]+
	   2*(z2[i]-point[1].getZ())*XZ[1]/(2)+
	   3*(z2[i]-point[1].getZ())*
	   (z2[i]-point[1].getZ())*
	   (XZ[2]-XZ[1])/(6*(point[2].getZ()-point[1].getZ()));    
     }
  for(Int_t i=1;i<N1-1;i++)
     {
	distfield+=sqrt((x2[i]-x2[i-1])*(x2[i]-x2[i-1])+
			(y2[i]-y2[i-1])*(y2[i]-y2[i-1])+
			(z2[i]-z2[i-1])*(z2[i]-z2[i-1]));
	
	fieldpoints[i-1].setXYZ(x2[i],y2[i],z2[i]);
	DYDZ[i-1]=dydz[i];
	DXDZ[i-1]=dxdz[i];
     }
  distfield+=sqrt((x2[N1-1]-x2[N1-2])*(x2[N1-1]-x2[N1-2])+
		  (y2[N1-1]-y2[N1-2])*(y2[N1-1]-y2[N1-2])+
		  (z2[N1-1]-z2[N1-2])*(z2[N1-1]-z2[N1-2]));
  dist[2]=distfield+dist[0]+dist[1];
  dist[3]=dist[2]+sqrt((point[3].getX()-point[2].getX())*
		       (point[3].getX()-point[2].getX())+
		       (point[3].getY()-point[2].getY())*
		       (point[3].getZ()-point[2].getZ())); 
}
Double_t HMdcTrackGSpline::getMetaDistance(Double_t x,Double_t y,Double_t z)
{
   metaDistance=sqrt((x-point[2].getX())*(x-point[2].getX())+
		     (y-point[2].getY())*(y-point[2].getY())+
		     (z-point[2].getZ())*(z-point[2].getZ()))+
      distfield;
   return metaDistance;
}
void HMdcTrackGSpline::fieldequation()
{
   Double_t k;
   Double_t A,B;        
   for (Int_t i=0;i<N2;i++)
      {
	 k=2*sqrt(1+DYDZ[i]*DYDZ[i]+DXDZ[i]*DXDZ[i]);
	 A=k*(BB[i].getZ()*DYDZ[i]+BB[i].getX()*DXDZ[i]*DYDZ[i]-
	      -BB[i].getY()*(1+DXDZ[i]*DXDZ[i]));		        
	 B=k*(BB[i].getX()*(1+DYDZ[i]*DYDZ[i]));
	 field[i].setXYZ(A,B,fieldpoints[i].getZ());
	 equationx[i]=fieldpoints[i].getX();
	 equationy[i]=fieldpoints[i].getY();
	 equationz[i]=fieldpoints[i].getZ();		      
      }
}
void HMdcTrackGSpline::solveMomentum()
{    
   Double_t ypr[N2],xpr[N2];
   Double_t h[N2-1];
   Yequation[0]=0;
   ypr[0]=0; 
   xpr[0]=0;
   h[0]=0;
   for (Int_t i = 1; i<N2-1; i++)
      h[i] = field[i+1].getZ() - field[i].getZ();
   for(Int_t i=0;i<N2-1;i++){
      ypr[i+1]=ypr[i]+h[i]*(field[i+1].getY()+field[i].getY())/(2)-
	 h[i]*h[i]*h[i]*(FYZ[i]+FYZ[i+1])/24;
   }
   for(Int_t i=0;i<N2-1;i++){
      Yequation[i+1]=Yequation[i]+
	 ypr[i]*h[i]+h[i]*h[i]*(2*field[i].getY()+field[i+1].getY())/(6)-
	 h[i]*h[i]*h[i]*h[i]*(0.8*FYZ[i]+0.7*FYZ[i+1])/(36);
   }
}         
void HMdcTrackGSpline::getMomentum(Bool_t cond)
{
   Double_t sumz=0,sumY=0,sumy=0,sumYy=0,sumyz=0,sumYz=0;
   Double_t sumz2=0,sumY2=0,sumy2=0;
   for (Int_t i=1;i<N2;i++)
      {
	 sumz+=equationz[i]-equationz[0];
	 sumY+=Yequation[i];
	 sumYz+=Yequation[i]*(equationz[i]-equationz[0]);
	 sumyz+=equationy[i]*(equationz[i]-equationz[0]);
	 sumy+=equationy[i];
	 sumYy+=Yequation[i]*equationy[i];
	 sumz2+=(equationz[i]-equationz[0])*(equationz[i]-equationz[0]);
	 sumY2+=Yequation[i]*Yequation[i];
	 sumy2+=equationy[i]*equationy[i];
      }
   Double_t delta[3][3]={{N2-1,sumz,sumY},
			 {sumz,sumz2,sumYz},
			 {sumY,sumYz,sumY2}};
   Double_t delta1[3][3]={{N2-1,sumz,sumy},
			  {sumz,sumz2,sumyz},
			  {sumY,sumYz,sumYy}};
   precon=det3(delta)/det3(delta1);
   if (precon<0)
   {
      precon*=-1;
//      cout<<"MI ZDES"<<endl;
      ////////////CHECKING
      polarity=-1;
   }
   
   else polarity=1;
   Double_t corr1=1;
   if (cond){
      for(Int_t k=0;k<33;k++) //teta
	 for(Int_t i=0;i<15;i++) //phi
	    for (Int_t j=0;j<59;j++) //momentum
	       {
		  if(phidig>90)
		     phidigg=180-phidig;
		  if(phidigg >= c->phiI[i] && phidigg < c->phiI[i+1] &&
		     precon >= c->precI[j] && precon < c->precI[j+1] && tetadig >= c->tetaI[k] && tetadig < c->tetaI[k+1])
		     {
			if(teta2dig>tetadig)
			   
			      getcorrE(k,i,j,corr1);
			//      polarity=-1;
			   
			if(teta2dig<tetadig)
			   
			      getcorrP(k,i,j,corr1); 
			//      polarity=1;
			   
			precon/=corr1;
			if(precon<0)precon*=-1;
			break;   
		     }
	       }
   }
}
Double_t HMdcTrackGSpline::calcMomentum(HMdcSeg *seg[2],Bool_t condition,Double_t targ)
{
   HGeomVector segpoints[4];
   calcSegPoints(seg,segpoints);
   return calcMomentum(segpoints,condition,targ);
}

Double_t HMdcTrackGSpline::calcMomentum123(HMdcSeg *seg[2],Bool_t condition,Double_t targ)
{
   HGeomVector segpoints[4];
   calcSegPoints123(seg,segpoints);
   return calcMomentum(segpoints,condition,targ);
}


void HMdcTrackGSpline::calcSegPoints(HMdcSeg *SEG[2],HGeomVector *SEGPOINTS)
{
   Float_t tetaseg[2],phiseg[2],zseg[2],roseg[2];
   tetaseg[0]=SEG[0]->getTheta();
   phiseg[0]=SEG[0]->getPhi();
   zseg[0]=SEG[0]->getZ();
   roseg[0]=SEG[0]->getR();
   
   tetaseg[1]=SEG[1]->getTheta();
   phiseg[1]=SEG[1]->getPhi();
   zseg[1]=SEG[1]->getZ();
   roseg[1]=SEG[1]->getR();
   
   Double_t Xseg[4],Yseg[4],Zseg[4];
   
   Xseg[0]=roseg[0]*cos(phiseg[0]+pi/2);
   Yseg[0]=roseg[0]*sin(phiseg[0]+pi/2);
   Zseg[0]=zseg[0];
   Xseg[1]=Xseg[0]+cos(phiseg[0])*sin(tetaseg[0]);
   Yseg[1]=Yseg[0]+sin(phiseg[0])*sin(tetaseg[0]);
   Zseg[1]=Zseg[0]+cos(tetaseg[0]);
   
   Xseg[2]=roseg[1]*cos(phiseg[1]+pi/2);
   Yseg[2]=roseg[1]*sin(phiseg[1]+pi/2);
   Zseg[2]=zseg[1];
   Xseg[3]=Xseg[2]+cos(phiseg[1])*sin(tetaseg[1]);
   Yseg[3]=Yseg[2]+sin(phiseg[1])*sin(tetaseg[1]);
   Zseg[3]=Zseg[2]+cos(tetaseg[1]);
   
   SEGPOINTS[0].setXYZ(0.1*Xseg[0],0.1*Yseg[0],0.1*Zseg[0]);
   SEGPOINTS[1].setXYZ(0.1*Xseg[1],0.1*Yseg[1],0.1*Zseg[1]);
   SEGPOINTS[2].setXYZ(0.1*Xseg[2],0.1*Yseg[2],0.1*Zseg[2]);
   SEGPOINTS[3].setXYZ(0.1*Xseg[3],0.1*Yseg[3],0.1*Zseg[3]);   
}


void HMdcTrackGSpline::calcSegPoints123(HMdcSeg *SEG[2],HGeomVector *SEGPOINTS)
{
   Float_t tetaseg[2],phiseg[2],zseg[2],roseg[2];
   tetaseg[0]=SEG[0]->getTheta();
   phiseg[0]=SEG[0]->getPhi();
   zseg[0]=SEG[0]->getZ();
   roseg[0]=SEG[0]->getR();
   
   tetaseg[1]=SEG[1]->getTheta();
   phiseg[1]=SEG[1]->getPhi();
   zseg[1]=SEG[1]->getZ();
   roseg[1]=SEG[1]->getR();
   
   Double_t Xseg[4],Yseg[4],Zseg[4];
   
   Xseg[0]=roseg[0]*cos(phiseg[0]+pi/2);
   Yseg[0]=roseg[0]*sin(phiseg[0]+pi/2);
   Zseg[0]=zseg[0];
   Xseg[1]=Xseg[0]+cos(phiseg[0])*sin(tetaseg[0]);
   Yseg[1]=Yseg[0]+sin(phiseg[0])*sin(tetaseg[0]);
   Zseg[1]=Zseg[0]+cos(tetaseg[0]);
   Double_t dirX=Xseg[1]-Xseg[0];
   Double_t dirY=Yseg[1]-Yseg[0];
   Double_t dirZ=Zseg[1]-Zseg[0]; 
   //Intersection with kickplane
   HGeomVector point,dir,interkickpoint;
   point.setXYZ(Xseg[0],Yseg[0],Zseg[0]);
   dir.setXYZ(dirX,dirY,dirZ);
   kickplane->calcIntersection(point,dir,interkickpoint);
   Xseg[2]=roseg[1]*cos(phiseg[1]+pi/2);
   Yseg[2]=roseg[1]*sin(phiseg[1]+pi/2);
   Zseg[2]=zseg[1];
   Xseg[3]=Xseg[2]+cos(phiseg[1])*sin(tetaseg[1]);
   Yseg[3]=Yseg[2]+sin(phiseg[1])*sin(tetaseg[1]);
   Zseg[3]=Zseg[2]+cos(tetaseg[1]);
   //PARAMETERS FOR THE MIDDLE OF THE 3-rd CHAMBER
   Double_t A=0., B=1.547, D=210.292; 
   //Third point from KickPlane, 4-from the middle of 3 chamber
   //calculation of 4 point
   HGeomVector point3, point4,interpoint;//interpoint in sm
   point3.setXYZ(0.1*Xseg[2],0.1*Yseg[2],0.1*Zseg[2]);
   point4.setXYZ(0.1*Xseg[3],0.1*Yseg[3],0.1*Zseg[3]);
   calcInter(A,B,D,point3,point4,interpoint);
   
   Xseg[2]=interkickpoint.getX();
   Yseg[2]=interkickpoint.getY();
   Zseg[2]=interkickpoint.getZ();
   
   Xseg[3]=interpoint.getX();
   Yseg[3]=interpoint.getY();
   Zseg[3]=interpoint.getZ();
   
 
  SEGPOINTS[0].setXYZ(0.1*Xseg[0],0.1*Yseg[0],0.1*Zseg[0]);
  SEGPOINTS[1].setXYZ(0.1*Xseg[1],0.1*Yseg[1],0.1*Zseg[1]);
  SEGPOINTS[2].setXYZ(0.1*Xseg[2],0.1*Yseg[2],0.1*Zseg[2]);
  SEGPOINTS[3].setXYZ(Xseg[3],Yseg[3],Zseg[3]);   
}



void HMdcTrackGSpline::getDistance(Double_t *distance)
{
   for(int i=0; i<4; i++)
      distance[i]=10*dist[i];
}

Double_t HMdcTrackGSpline::calcMomentum(HGeomVector  *pp,Bool_t cond,Double_t targ)
{
   target=targ*0.1; 
   HGeomVector ppp[4];

   Double_t A1=0., B1=0.972306, D1=78.5016;
   Double_t A2=0., B2=1.37611, D2=125.351;
   Double_t A3=0., B3=1.15462, D3=207.245;
   Double_t A4=0., B4=1.15466, D4=256.117;
   
   
   calcInter(A1,B1,D1,pp[0],pp[1],ppp[0]);
   calcInter(A2,B2,D2,pp[0],pp[1],ppp[1]);
   calcInter(A3,B3,D3,pp[2],pp[3],ppp[2]);
   calcInter(A4,B4,D4,pp[2],pp[3],ppp[3]);
   
   Double_t xVector[3],fieldVector[3];
   
   x[0]=ppp[0].getX();
   x[1]=ppp[0].getY();
   x[2]=ppp[0].getZ();
   x[3]=ppp[1].getX();
   x[4]=ppp[1].getY();
   x[5]=ppp[1].getZ();
   x[6]=ppp[2].getX();
   x[7]=ppp[2].getY();
   x[8]=ppp[2].getZ();
   x[9]=ppp[3].getX();
   x[10]=ppp[3].getY();
   x[11]=ppp[3].getZ();
   
   getPoint();
   
   spline(point,YZ,YZP,XZ,XZP,4);
   
  getpoints();
  transFieldpoints();
  
  
  for(Int_t i=0;i<48;i++)
     {
	xVector[0]=Tfieldpoints[i].getX();
	xVector[1]=Tfieldpoints[i].getY();
	xVector[2]=Tfieldpoints[i].getZ();
	if(xVector[0]<0)
	   xVector[0]*=-1;
	b->calcField(xVector,fieldVector,0.7215);
	Bx[i]=fieldVector[0];
	By[i]=fieldVector[1];
	Bz[i]=fieldVector[2];
     }
  init1();
  transteta();
  fieldequation();
  spline(field,FYZ,FYZP,FXZ,FXZP,48);
  solveMomentum();
  getMomentum(cond);
  return precon;
  
} 
void HMdcTrackGSpline::getcorrE(Int_t k,Int_t i, Int_t j, Double_t & corr)
{
   corr=c->corrE[k][i][j]+
      (c->corrE[k][i][j+1]-c->corrE[k][i][j])*(precon-c->precI[j])/
      (c->precI[j+1]-c->precI[j])+(c->corrE[k][i+1][j]-c->corrE[k][i][j])*(phidigg-c->phiI[i])/
      (c->phiI[i+1]-c->phiI[i])+(c->corrE[k+1][i][j]-c->corrE[k][i][j])*(tetadig-c->tetaI[k])/
      (c->tetaI[k+1]-c->tetaI[k]);
}
void HMdcTrackGSpline::getcorrP(Int_t k,Int_t i, Int_t j, Double_t & corr)
{
   corr=c->corrP[k][i][j]+
      (c->corrP[k][i][j+1]-c->corrP[k][i][j])*(precon-c->precI[j])/
      (c->precI[j+1]-c->precI[j])+(c->corrP[k][i+1][j]-c->corrP[k][i][j])*(phidigg-c->phiI[i])/
      (c->phiI[i+1]-c->phiI[i])+(c->corrP[k+1][i][j]-c->corrP[k][i][j])*(tetadig-c->tetaI[k])/
      (c->tetaI[k+1]-c->tetaI[k]);
}


void HMdcTrackGSpline::setDataPointer(HMdcTrackGField* field,HMdcTrackGCorrections* corr)
{
   b=field;
   
   c=corr;
   
}
void HMdcTrackGSpline::setKickPointer(HKickPlane2 *kickpointer)
{
   kickplane=kickpointer;
}
void HMdcTrackGSpline::calcInter(Double_t A,Double_t B,Double_t D,HGeomVector in1 , 
				 HGeomVector in2,HGeomVector &out)
{
  Double_t x1,y1,z1,x2,y2,z2,x,y,z;
  x1=in1.getX();
  y1=in1.getY();
  z1=in1.getZ();
  
  x2=in2.getX();
  y2=in2.getY();
  z2=in2.getZ();

  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t del=1/(A*dX+B*dY+dZ);
  
  x=(dX*(D-z1-B*y1)+x1*(B*dY+dZ))*del;
  y=(dY*(D-z1-A*x1)+y1*(A*dX+dZ))*del;
  z=D-A*x-B*y;
  out.setXYZ(x,y,z);
  //  return out;
}
void HMdcTrackGSpline::equationofLine(Double_t* xz1,Double_t *xz2,Double_t Z,Double_t &X)
{
Double_t A,B;
Double_t delta,deltaA,deltaB;

delta=xz1[1]-xz2[1];
deltaA=xz1[0]-xz2[0];
deltaB=xz1[1]*xz2[0]-xz1[0]*xz2[1];

A=deltaA/delta;
B=deltaB/delta;

X=A*Z+B;
}

void HMdcTrackGSpline::equationofLine3D(HGeomVector point1,HGeomVector point2,Double_t Z,Double_t &X,Double_t &Y)
{
Double_t t;

t=(Z-point1.getZ())/(point2.getZ()-point1.getZ());
X=point1.getX()+t*(point2.getX()-point1.getX());
Y=point1.getY()+t*(point2.getY()-point1.getY());
}
HGeomVector HMdcTrackGSpline::calcMetaDir(HMdcSeg *segment, HGeomVector fTof)
{
  HGeomVector directionToMeta;
  Double_t distanceMeta;
  HGeomVector   kickIntersection;
  kickIntersection=calcKickIntersection(segment);  
  directionToMeta=fTof-kickIntersection;
  distanceMeta=directionToMeta.length();
  directionToMeta/=distanceMeta;
  return directionToMeta;
}
HGeomVector HMdcTrackGSpline::calcKickIntersection(HMdcSeg *segment)
{
  HGeomVector XXpoint;
  HGeomVector XXdirection;
  HGeomVector kickIntersection;
  XX=(segment->getR())*cos(segment->getPhi()+pi/2);
  YY=(segment->getR())*sin(segment->getPhi()+pi/2);
  ZZ=segment->getZ();
  XXdir=cos(segment->getPhi())*sin(segment->getTheta());
  YYdir=sin(segment->getPhi())*sin(segment->getTheta());
  ZZdir=cos(segment->getTheta());
  XXpoint.setXYZ(XX,YY,ZZ);
  XXdirection.setXYZ(XXdir,YYdir,ZZdir);
  kickplane->calcIntersection(XXpoint,XXdirection,XXdirection);
  return XXdirection;
} 







