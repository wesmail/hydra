using namespace std;
#include "hmdcsizescells.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcgeomstruct.h"
#include <iostream> 
#include <iomanip>
#include "math.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdcgetcontainers.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"
#include "hmdclookupraw.h"
#include "hmdcrawstruct.h"
#include "hruntimedb.h"
#include "hspecgeompar.h"
#include <stdlib.h>

//*-- Author : Vladimir Pechenov
//*-- Modified : 11/05/2002 by V. Pechenov
//*-- Modified : 08/05/2001 by V. Pechenov
//*-- Modified : 07/03/2001 by V. Pechenov
//*-- Modified : 27/05/2000 by V. Pechenov
//*-- Modified : 24/05/2000 by V. Pechenov
//*-- Modified : 07/03/2000 by R. Holzmann
//*-- Modified : 31/8/99 by V. Pechenov

////////////////////////////////////////////////////////////////
//
//  HMdcSizesCells
//    HMdcSizesCellsSec
//    HMdcSizesCellsMod
//    HMdcSizesCellsLayer
//    HMdcSizesCellsCell
//
//  Creating the container and geting the pointer to cont.:
//  HMdcSizesCells* fSizesCells=HMdcSizesCells::getObject();
//
//  Geting the pointer to cont. (without creating):
//  HMdcSizesCells* fSizesCells=HMdcSizesCells::getExObject();
//
//  Deleting of container:
//  HMdcSizesCells::deleteCont();
//
//  Container class keep volumes sizes of wires (2-points/wire)
//  in coordinate system of sector and transformations layer<->mod,
//  layer<->sec, ...
//  It has same useful functions:
//  HMdcSizesCellsCell
//    Float_t getWireLength(void)
//
//  HMdcSizesCellsLayer:
//    HGeomTransform* getSecTrans();
//    HGeomTransform* getModTrans();
//    void transSecToRotLayer(HGeomVector& point);
//    Double_t getAlpha(const HGeomVector& p1, const HGeomVector& p2);
//    Double_t getDist(const HGeomVector& p1,
//                     const HGeomVector& p2, Int_t cell);
//    void getImpact(const HGeomVector& p1, const HGeomVector& p2,
//                  Int_t cell, Double_t &alpha, Double_t &per);
//    Double_t getImpact(Double_t xP1,Double_t yP1,Double_t zP1, Double_t xP2,
//        Double_t yP2, Double_t zP2, Int_t cell, Double_t &alphaPerp);
//    Bool_t getImpact(Double_t xP1,Double_t yP1,Double_t zP1,
//        Double_t xP2, Double_t yP2, Double_t zP2,
//        Int_t cell, Double_t &alphaDrDist, Double_t &driftDist);
//    Int_t getCellNum(Double_t x, Double_t y):
//      calculation the the cell number for the point x,y
//      on the wire plane. (x,y - in coor. sys. of layer, z=0)
//    Double_t getXSign(Double_t xP1, Double_t yP1, Double_t zP1, Double_t xP2,
//        Double_t yP2, Double_t zP2, Int_t cell)
//    Float_t getSignPath(Double_t xP1,Double_t yP1,Double_t zP1,
//        Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell);
//    Float_t getSignPath(Double_t xP1,Double_t yP1,Double_t zP1,
//        Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell,
//        Float_t& outside);
//
//  HMdcSizesCellsMod:
//    HGeomTransform* getSecTrans();
//
//  HMdcSizesCellsSec:
//    HGeomTransform* getLabTrans();
//
//  HMdcSizesCells:
//    Bool_t getCellVol(Int_t sec, Int_t mod, Int_t lay,
//        Int_t cell, HMdcTrap& volCell);
//    Char_t findReadOutSide(Int_t s,Int_t m,Int_t l,Int_t c)
//
//////////////////////////////////////////////////////////////

ClassImp(HMdcSizesCellsCell)
ClassImp(HMdcSizesCellsLayer)
ClassImp(HMdcSizesCellsMod)
ClassImp(HMdcSizesCellsSec)
ClassImp(HMdcSizesCells)

Double_t HMdcSizesCellsLayer::radToDeg=0.;

void HMdcSizesCellsCell::clear(void) {
  wireSizes[0].clear();
  wireSizes[1].clear();
  status=kFALSE;
  xReadout=0.;
  readOutSide='0';
}


HMdcSizesCellsLayer::HMdcSizesCellsLayer(Int_t sec, Int_t mod, Int_t lay) {
  // Constructor creates an array of pointers of type HMdcSizesCellsCell
  sector=sec;
  module=mod;
  layer=lay;
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  HMdcGeomStruct* fMdcGeomStruct=fGetCont->getMdcGeomStruct();

  nCells=((*fMdcGeomStruct)[sec][mod])[layer];
  if( nCells > 0 ) {
    array = new TClonesArray("HMdcSizesCellsCell",nCells);
    for (Int_t cell = 0; cell < nCells; cell++)
      new((*array)[cell]) HMdcSizesCellsCell();
  }
  if(radToDeg<0.1) radToDeg=180./TMath::Pi();
}

HMdcSizesCellsLayer::~HMdcSizesCellsLayer(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcSizesCellsLayer::getSize(void) {
  // return the size of the pointer array
  return array->GetLast()+1;
}

Bool_t HMdcSizesCellsLayer::setSecTrans(HGeomTransform * alignTrans) {
  // alignTrans: coord. system of layer will transformed to "alignTrans" sys.
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  if(!fGetCont) return kFALSE;
  if(!fGetCont->getSecTransLayerZ0(sysRSec,sector,module,layer)) return kFALSE;
  if(!fGetCont->getModTransLayerZ0(sysRMod,module,layer)) return kFALSE;
  if(alignTrans) sysRSec.transTo(*alignTrans); // correction for alignment
  const HGeomRotation& rot=sysRSec.getRotMatrix();
  const HGeomVector& vc=sysRSec.getTransVector();
  for(Int_t i=0; i<9; i++) tfSysRSec[i]=rot(i);
  tfSysRSec[ 9]=vc(0);
  tfSysRSec[10]=vc(1);
  tfSysRSec[11]=vc(2);
  Double_t parC=rot(0)*rot(4)-rot(3)*rot(1);
  if( parC==0. ) return kFALSE;
  parA=(rot(3)*rot(7)-rot(6)*rot(4))/parC;
  parB=(rot(6)*rot(1)-rot(0)*rot(7))/parC;
  parD=parA*vc(0)+parB*vc(1)+vc(2);
  return kTRUE;
}

Int_t HMdcSizesCellsLayer::calcCellNum(Double_t x, Double_t y) const{
  //  calculation the the cell number for the point x,y
  //  on the wire plane. (x,y - in coor. sys. of layer, z=0)
  Double_t wire=(wireOffSet + y*cosWireOr-x*sinWireOr)/pitch+0.5;
  Int_t nWire=(Int_t)wire;
  return (wire<0. || nWire>=nCells) ? -1:nWire;
}

Bool_t HMdcSizesCellsLayer::calcCrossedCells(Double_t xP1, Double_t yP1, 
    Double_t zP1, Double_t xP2, Double_t yP2, Double_t zP2, 
    Float_t& cell1, Float_t& cell2) {
  // calculation of cells crossed by line xP1,yP1,zP1-xP2,yP2,zP2
  // xP1,yP1,zP1, xP2,yP2,zP2 - sector coordinate system
  transTo(xP1,yP1,zP1);
  transTo(xP2,yP2,zP2);
  Double_t dX=xP2-xP1;
  Double_t dY=yP2-yP1;
  Double_t dZ=zP2-zP1;
  if(dZ==0.) return kFALSE;
  Double_t tmp=(halfCatDist-zP1)/dZ;
  Double_t x=tmp*dX+xP1;
  Double_t y=tmp*dY+yP1;
  cell1=(wireOffSet + y*cosWireOr-x*sinWireOr)/pitch+0.5;
  tmp=(-halfCatDist-zP1)/dZ;
  x=tmp*dX+xP1;
  y=tmp*dY+yP1;
  cell2=(wireOffSet + y*cosWireOr-x*sinWireOr)/pitch+0.5;
  if(cell1>cell2) {
    Float_t c=cell1;
    cell1=cell2;
    cell2=c;
  }
  if(cell1<0.) {
    if(cell2<0.) return kFALSE;
    cell1=0.;
  }
  if(cell2>=nCells) {
    if(cell1>=nCells) return kFALSE;
    cell2=nCells-0.0001;//nCells-1;
  }
  return kTRUE;
}

void HMdcSizesCellsLayer::transSecToRotLayer(HGeomVector& point) {
  // transformation to coor. sys. of rotated layer from sector sys.
  point=sysRSec.transTo(point);
  Double_t y = point.getY()*cosWireOr-point.getX()*sinWireOr;
  Double_t x = point.getX()*cosWireOr+point.getY()*sinWireOr;
  point.setX(x);
  point.setY(y);
}

Double_t HMdcSizesCellsLayer::getAlpha(const HGeomVector& p1,
                                      const HGeomVector& p2) {
  // return the angle alpha (in degree) between projection of linep1-p2
  // on the Y-Z plane and Y axis in coor.sys. of wires in degree.
  // p1,p2 in coordinate system of sector.
  HGeomVector tranP1(p1);
  HGeomVector tranP2(p2);
  transSecToRotLayer(tranP1);
  transSecToRotLayer(tranP2);
  return atan2( fabs(tranP2.getZ()-tranP1.getZ()),
      fabs(tranP2.getY()-tranP1.getY()) )*radToDeg;
}

Double_t HMdcSizesCellsLayer::getDist(const HGeomVector& p1,
                                      const HGeomVector& p2,Int_t cell){
  // return the minimal distance from line (p1-p2) to wire.
  // p1,p2 are in coordinate system of sector.
  HGeomVector tranP1(p1);
  HGeomVector tranP2(p2);
  transSecToRotLayer(tranP1);
  transSecToRotLayer(tranP2);
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t y1=tranP1.getY() - yWire;
  Double_t y2=tranP2.getY() - yWire;
  Double_t dz=tranP1.getZ()-tranP2.getZ();
  Double_t dy=y1-y2;
  return fabs((tranP1.getZ()*y2-tranP2.getZ()*y1)/sqrt(dz*dz+dy*dy));
}

void HMdcSizesCellsLayer::getImpact(const HGeomVector& p1,
    const HGeomVector& p2, Int_t cell, Double_t &alpha, Double_t &minDist) {
  // calc. the angle alpha (in degree) between projection of line p1-p2
  // on the Y-Z plane and Y axis in coor.sys. of wire (y=z=0) and
  // the minimal distance from line (p1-p2) to wire.
  // p1,p2 - in sector coor.sys.
  HGeomVector tranP1(p1);
  HGeomVector tranP2(p2);
  transSecToRotLayer(tranP1);
  transSecToRotLayer(tranP2);
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t y1=tranP1.getY() - yWire;
  Double_t y2=tranP2.getY() - yWire;
  Double_t dz=tranP2.getZ()-tranP1.getZ();
  Double_t dy=y2-y1;
  Double_t lng=1./sqrt(dz*dz+dy*dy);
  alpha=asin(fabs(dz)*lng)*radToDeg;
  minDist = fabs((tranP1.getZ()*y2-tranP2.getZ()*y1)*lng);
}

Double_t HMdcSizesCellsLayer::getXSign(Double_t xP1, Double_t yP1,
    Double_t zP1, Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell) {
  // return X position of the signal on the wire in coor. sys. of this wire
  // (Ywire=0, Zwire=0), xP1,yP1,zP1 & xP2,yP2,zP2 - track in sector coor.system
  Double_t yWire=cell*pitch-wireOffSet;
  Double_t pmtx=xP1-tfSysRSec[ 9];
  Double_t pmty=yP1-tfSysRSec[10];
  Double_t pmtz=zP1-tfSysRSec[11];
  Double_t x11=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  Double_t y11=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  Double_t z1 =tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  Double_t y1 = y11*cosWireOr-x11*sinWireOr - yWire;
  Double_t x1 = x11*cosWireOr+y11*sinWireOr;
  pmtx=xP2-tfSysRSec[ 9];
  pmty=yP2-tfSysRSec[10];
  pmtz=zP2-tfSysRSec[11];
  Double_t x21=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  Double_t y21=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  Double_t z2 =tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  Double_t y2 = y21*cosWireOr-x21*sinWireOr - yWire;
  Double_t x2 = x21*cosWireOr+y21*sinWireOr;
  Double_t dz=z1-z2;
  Double_t dy=y1-y2;
  return (z1*dz+y1*dy)/(dz*dz+dy*dy)*(x2-x1)+x1;
}

Float_t HMdcSizesCellsLayer::getSignPath(Double_t xP1, Double_t yP1,
    Double_t zP1, Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell) {
  // return the path of signal along wire number "cell"
  // xP1,yP1,zP1 & xP2,yP2,zP2 - track in sector coor.system
  // Path can be <0. or > wire_length !!!
  // For GEANT data and don't connected wires return 0.
  HMdcSizesCellsCell& fCell=(*this)[cell];
  if(!&fCell || fCell.readOutSide=='0') return 0.;
  Float_t x=getXSign(xP1, yP1,zP1, xP2, yP2, zP2,cell);
  return (fCell.readOutSide=='1') ? fCell.xReadout-x : x-fCell.xReadout;
}

Float_t HMdcSizesCellsLayer::getSignPath(Double_t xP1, Double_t yP1,
    Double_t zP1, Double_t xP2, Double_t yP2, Double_t zP2, Int_t cell,
    Float_t& outside) {
  // return the path of signal along wire number "cell"
  // xP1,yP1,zP1 & xP2,yP2,zP2 - track in sector coor.system
  // For GEANT data and don't connected wires return 0.
  //
  // outside:
  // if(path<0) outside=-path;
  // else if(path>wire_length) outside=path-wire_length;
  // else outside=0;
  //
  // path-outside give new path which >=0. and <=wire_length)

  HMdcSizesCellsCell& fCell=(*this)[cell];
  if(!&fCell || fCell.readOutSide=='0') {
    outside=0.;
    return 0.;
  }
  Float_t x=getXSign(xP1, yP1,zP1, xP2, yP2, zP2,cell);
  Float_t path=(fCell.readOutSide=='1') ? fCell.xReadout-x : x-fCell.xReadout;
  if(path<0.) outside=path;
  else if(path>fCell.length) outside=path-fCell.length;
  else outside=0.;
  return path;
}

Bool_t HMdcSizesCellsLayer::getDriftDist(Double_t xP1,Double_t yP1,Double_t zP1,
    Double_t xP2, Double_t yP2, Double_t zP2,
    Int_t cell, Double_t &alphaDrDist, Double_t &driftDist) {
  // ---- esli ponadobitsya, nado proverit' rasschet uglov dlya treka vne cella
  //
  // return kTRUE if line p1-p2 intersect cell or kFALSE if not intersect
  // calc. the angle driftAlpha (in degree) and drift distance in cell
  // from line (p1-p2) to wire.
  // p1,p2 - in sector coor.sys.
  // 1)
  //    Z^
  //     |  |------------*----|
  //     |  | cell        *   |
  //     |  |            / *  |
  //     |  |           /90 * |
  //     |  | driftDist/     *|
  //     |  |         /       *
  //   --|--|--------*^-----/-|*--------->
  //     |  |              /  | *        Y
  //     |  |      alphaDrDist|  *
  //        |                 |   *
  //        |                 |    *
  //        |-----------------|     * track
  //
  // 2)             *** 90
  //    Z^             ***
  //     |  |----------:--***-|
  //     |  | cell     :  /  ***
  //     |  |         :  /    | ***  track
  //     |  |         : /     |    ***
  //     |  | driftDist/      |
  //     |  |        :/       |
  //   --|--|--------*^-------|---------->
  //     |  |         |       |          Y
  //     |  |                 |  alphaDrDist ???
  //        |                 |
  //        |                 |
  //        |-----------------|
  //
  //
  // 3)                        *
  //                            *
  //    Z^                     / * <-- no eg. 90 deg. !
  //     |  |-----------------/   *
  //     |  | cell           /|    *
  //     |  |               / |     *
  //     |  |              /  |      *
  //     |  |   driftDist /   |       *
  //     |  |            /    |        *track
  //     |  |           /     |         *
  //     |  |          /      |          *
  //     |  |         /       |      Alpha*
  //   --|--|--------*^-------|------------->
  //     |  |         |       |          Y
  //     |  |      alphaDrDist|
  //        |                 |
  //        |                 |
  //        |                 |
  //        |                 |
  //        |                 |
  //        |                 |
  //        |-----------------|

  Double_t yWire=cell*pitch-wireOffSet;
  Double_t pmtx=xP1-tfSysRSec[ 9];
  Double_t pmty=yP1-tfSysRSec[10];
  Double_t pmtz=zP1-tfSysRSec[11];
  Double_t x1=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  Double_t y1=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  Double_t z1=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y1 = y1*cosWireOr-x1*sinWireOr - yWire;
//  x = point.getX()*cosWireOr+point.getY()*sinWireOr;
  pmtx=xP2-tfSysRSec[ 9];
  pmty=yP2-tfSysRSec[10];
  pmtz=zP2-tfSysRSec[11];
  Double_t x2=tfSysRSec[0]*pmtx+tfSysRSec[3]*pmty+tfSysRSec[6]*pmtz;
  Double_t y2=tfSysRSec[1]*pmtx+tfSysRSec[4]*pmty+tfSysRSec[7]*pmtz;
  Double_t z2=tfSysRSec[2]*pmtx+tfSysRSec[5]*pmty+tfSysRSec[8]*pmtz;
  y2 = y2*cosWireOr-x2*sinWireOr - yWire;
//  x = point.getX()*cosWireOr+point.getY()*sinWireOr;

  Double_t dz=fabs(z1-z2);
  Double_t dy=fabs(y1-y2);
  Double_t length=sqrt(dz*dz+dy*dy);
  Double_t ctgAlpha=dy/dz;
  Double_t sinAlpha = dz/length;
  Double_t yDist = fabs((z1*y2-z2*y1)/dz);
  driftDist = yDist*sinAlpha;
  alphaDrDist=atan2(dz,dy)*radToDeg;
  if(driftDist*sinAlpha > halfPitch) {
    Double_t z = (dy != 0.0) ? ((yDist-halfPitch)/ctgAlpha) : 1.E+10;
    if(z <= halfCatDist)  {
      driftDist = sqrt(halfPitch*halfPitch+z*z);
      alphaDrDist=atan2((Double_t)halfPitch,z)*radToDeg;
    } else {
      Double_t tgADrDist=halfCatDist/halfPitch;
      alphaDrDist=90.-atan(tgADrDist)*radToDeg;
      driftDist=yDist/(fabs(ctgAlpha*tgADrDist)+1.)*sqrt(1+tgADrDist*tgADrDist);
      return kFALSE;
    }
  } else if(driftDist*dy/length > halfCatDist) { // dy/length = cos(alpha)
    Double_t y = yDist-halfCatDist*ctgAlpha;
    if(y <= halfPitch) {
      driftDist = sqrt(y*y+halfCatDist*halfCatDist);
      alphaDrDist=atan2(y,(Double_t)halfCatDist)*radToDeg;
    } else {
      Double_t tgADrDist=halfCatDist/halfPitch;
      alphaDrDist=90.-atan(tgADrDist)*radToDeg;
      driftDist=yDist/(fabs(ctgAlpha*tgADrDist)+1.)*sqrt(1+tgADrDist*tgADrDist);
      return kFALSE;
    }
  }
  return kTRUE;
}

void HMdcSizesCellsLayer::print(void) {
  printf("HMSCLayer: %iS %iM %iL, %i cells, cat.dist=%5.2f, pitch=%f, ang.=%4.1f\n",
      sector+1,module+1,layer+1,nCells,halfCatDist*2,pitch,
      asin(sinWireOr)*radToDeg);
}

//----------Sector:--------------------------------
HMdcSizesCellsMod::HMdcSizesCellsMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type
  // HMdcSizesCellsLayer
  sector=sec;
  module=mod;
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++)
      (*array)[layer] = new HMdcSizesCellsLayer(sec,mod,layer);
  parA=parB=parD=0.;
}

HMdcSizesCellsMod::~HMdcSizesCellsMod(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcSizesCellsMod::getSize(void) {
  // return the size of the pointer array
  return array->GetEntries();
}

Bool_t HMdcSizesCellsMod::setSecTrans(const HGeomTransform& secTrans) {
  sysRSec=secTrans;
  const HGeomRotation& rot=sysRSec.getRotMatrix();
  const HGeomVector& tv=sysRSec.getTransVector();
  for(Int_t i=0; i<9; i++) tfSysRSec[i]=rot(i);
  tfSysRSec[ 9]=tv(0);
  tfSysRSec[10]=tv(1);
  tfSysRSec[11]=tv(2);
  Double_t parC=rot(0)*rot(4)-rot(3)*rot(1);
  if( parC==0. ) return kFALSE;
  parA=(rot(3)*rot(7)-rot(6)*rot(4))/parC;
  parB=(rot(6)*rot(1)-rot(0)*rot(7))/parC;
  parD=parA*tv(0)+parB*tv(1)+tv(2);
  ct[0]=(tfSysRSec[0]-tfSysRSec[6]*parA);
  ct[1]=(tfSysRSec[3]-tfSysRSec[6]*parB);
  ct[2]=tfSysRSec[6]*(parD-parB*tfSysRSec[10]-tfSysRSec[11]-parA*tfSysRSec[9]);
  ct[3]=(tfSysRSec[1]-tfSysRSec[7]*parA);
  ct[4]=(tfSysRSec[4]-tfSysRSec[7]*parB);
  ct[5]=tfSysRSec[7]*(parD-parB*tfSysRSec[10]-tfSysRSec[11]-parA*tfSysRSec[9]);
  return kTRUE;
}

void HMdcSizesCellsMod::calcMdcHit(Double_t x1, Double_t y1, Double_t z1,
      Double_t x2, Double_t y2, Double_t z2,
      Double_t eX1, Double_t eY1, Double_t eZ1, Double_t eX2, Double_t eY2,
      Double_t dZ1dX1, Double_t dZ1dY1, Double_t dZ2dX2, Double_t dZ2dY2,
      Double_t& x, Double_t& eX, Double_t& y, Double_t& eY,
      Double_t& xDir, Double_t& eXDir, Double_t& yDir, Double_t& eYDir) const {
  // Calcul. a cross of the line with plane MDC (parA*x+parB*y+c*z=parD),
  // transform. this point to mdc coor.sys. (z=0) and calc.
  // hit direction in mdc coor.sys. and errors
  //
  // Input param:
  // x1,y1,z1(P1) - target or point on plane
  // x2,y2,z2(P2) - point on plane
  // eX1,eY1,eZ1, eX2,eY2 - errors (eZ2=0!)
  // dZ2dX2 = -A2  where A2 - plane param. (z2=D2-A2*x2+B2*y2)
  // dZ2dY2 = -B2  where B2 - plane param. (z2=D2-A2*x2+B2*y2)
  // If P1-on plane
  //   eZ1=0;
  //   dZ1dX1 = -A1  where A1 - plane param. (z1=D1-A1*x1+B1*y1)
  //   dZ1dY1 = -B1  where B1 - plane param. (z1=D1-A1*x1+B1*y1)
  // If P1-target:
  //   dZ1dX1 = dZ1/dY1 = 0;
  //---Cross-----------------------------------------------
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t cxz1=parD-z1-parA*x1;
  Double_t cxz2=parD-z2-parA*x2;
  Double_t cyz1=parD-z1-parB*y1;
  Double_t cyz2=parD-z2-parB*y2;
  Double_t del=1/(parA*dX+parB*dY+dZ);
  Double_t xt0=(x2*cyz1 - x1*cyz2)*del;
  Double_t yt0=(y2*cxz1 - y1*cxz2)*del;
  Double_t xt=xt0-tfSysRSec[ 9];
  Double_t yt=yt0-tfSysRSec[10];
  x=ct[0]*xt+ct[1]*yt+ct[2];
  y=ct[3]*xt+ct[4]*yt+ct[5];
  // - Errors - - - - - - - - - - - - - - - - -
  Double_t x1t=x1-xt0;
  Double_t y1t=y1-yt0;
  Double_t xt2=xt0-x2;
  Double_t yt2=yt0-y2;
  Double_t xtA=xt0*parA;
  Double_t ytB=yt0*parB;
  Double_t dXtdX2=x1t*dZ2dX2+cyz1-xtA;
  Double_t dYtdY2=y1t*dZ2dY2+cxz1-ytB;
  Double_t dXtdX1=xt2*dZ1dX1-cyz2+xtA;
  Double_t dYtdY1=yt2*dZ1dY1-cxz2+ytB;
  Double_t dXtdY2=x1t*(dZ2dY2+parB);
  Double_t dYtdX2=y1t*(dZ2dX2+parA);
  Double_t dXtdY1=xt2*(dZ1dY1+parB);
  Double_t dYtdX1=yt2*(dZ1dX1+parA);
  Double_t dXtdZ1=xt2;
  Double_t dYtdZ1=yt2;
  Double_t dXdX1=(ct[0]*dXtdX1+ct[1]*dYtdX1)*eX1;
  Double_t dYdX1=(ct[3]*dXtdX1+ct[4]*dYtdX1)*eX1;
  Double_t dXdY1=(ct[0]*dXtdY1+ct[1]*dYtdY1)*eY1;
  Double_t dYdY1=(ct[3]*dXtdY1+ct[4]*dYtdY1)*eY1;
  Double_t dXdX2=(ct[0]*dXtdX2+ct[1]*dYtdX2)*eX2;
  Double_t dYdX2=(ct[3]*dXtdX2+ct[4]*dYtdX2)*eX2;
  Double_t dXdY2=(ct[0]*dXtdY2+ct[1]*dYtdY2)*eY2;
  Double_t dYdY2=(ct[3]*dXtdY2+ct[4]*dYtdY2)*eY2;
  Double_t dXdZ1=(ct[0]*dXtdZ1+ct[1]*dYtdZ1)*eZ1;
  Double_t dYdZ1=(ct[3]*dXtdZ1+ct[4]*dYtdZ1)*eZ1;
  eX=sqrt(dXdX1*dXdX1+dXdY1*dXdY1+dXdZ1*dXdZ1+dXdX2*dXdX2+dXdY2*dXdY2)*del;
  eY=sqrt(dYdX1*dYdX1+dYdY1*dYdY1+dYdZ1*dYdZ1+dYdX2*dYdX2+dYdY2*dYdY2)*del;
  //---Hit direction----------------------------------------------------
  Double_t length=1/sqrt(dX*dX+dY*dY+dZ*dZ);
  dX *= length; // unit vector
  dY *= length;
  dZ *= length;
  xDir=tfSysRSec[0]*dX+tfSysRSec[3]*dY+tfSysRSec[6]*dZ;
  yDir=tfSysRSec[1]*dX+tfSysRSec[4]*dY+tfSysRSec[7]*dZ;
  // - Errors - - - - - - - - - - - - - - - - -
  Double_t c0xx=(tfSysRSec[0]-xDir*dX);
  Double_t c3xy=(tfSysRSec[3]-xDir*dY);
  Double_t c6xz=(tfSysRSec[6]-xDir*dZ);
  Double_t c1yx=(tfSysRSec[1]-yDir*dX);
  Double_t c4yy=(tfSysRSec[4]-yDir*dY);
  Double_t c7yz=(tfSysRSec[7]-yDir*dZ);
  Double_t dXDdX2 = (c0xx+c6xz*dZ2dX2)*eX2;
  Double_t dXDdY2 = (c3xy+c6xz*dZ2dY2)*eY2;
  Double_t dXDdX1 = (c0xx+c6xz*dZ1dX1)*eX1; // *(-1)
  Double_t dXDdY1 = (c3xy+c6xz*dZ1dY1)*eY1; // *(-1)
  Double_t dXDdZ1 =       c6xz        *eZ1; // *(-1)
  Double_t dYDdX2 = (c1yx+c7yz*dZ2dX2)*eX2;
  Double_t dYDdY2 = (c4yy+c7yz*dZ2dY2)*eY2;
  Double_t dYDdX1 = (c1yx+c7yz*dZ1dX1)*eX1; // *(-1)
  Double_t dYDdY1 = (c4yy+c7yz*dZ1dY1)*eY1; // *(-1)
  Double_t dYDdZ1 =       c7yz        *eZ1; // *(-1)
  eXDir=sqrt(dXDdX2*dXDdX2+dXDdY2*dXDdY2+
             dXDdX1*dXDdX1+dXDdY1*dXDdY1+dXDdZ1*dXDdZ1)*length;
  eYDir=sqrt(dYDdX2*dYDdX2+dYDdY2*dYDdY2+
             dYDdX1*dYDdX1+dYDdY1*dYDdY1+dYDdZ1*dYDdZ1)*length;
}

//----------Sector:--------------------------------
HMdcSizesCellsSec::HMdcSizesCellsSec(Int_t sec, Bool_t* mdcStSec) {
  // constructor creates an array of pointers of type
  // HMdcSizesCellsMod
  sector=sec;
  array = new TObjArray(4);
  mdcStatSec=mdcStSec;
  for (Int_t mod=0; mod<4; mod++)
    if(mdcStatSec[mod]) (*array)[mod] = new HMdcSizesCellsMod(sec,mod);
}

HMdcSizesCellsSec::~HMdcSizesCellsSec(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcSizesCellsSec::getSize(void) {
  // return the size of the pointer array
  return array->GetEntries();
}

//==================================================
HMdcSizesCells* HMdcSizesCells::fMdcSizesCells=0;

HMdcSizesCells::HMdcSizesCells(void) {
  // constructor creates an array of pointers of type HMdcSizesCellsSec
  fGetCont=HMdcGetContainers::getObject();
  fLayerGeomPar = fGetCont->getMdcLayerGeomPar();
  fGeomPar      = fGetCont->getMdcGeomPar();
  fSpecGeomPar  = fGetCont->getSpecGeomPar();
  fMdcDet       = fGetCont->getMdcDetector();
  fMdcGeomStruct= fGetCont->getMdcGeomStruct();
  fMdcRawStruct = fGetCont->getMdcRawStruct();
  if (!fMdcRawStruct->isStatic() || !HMdcGetContainers::isInited(fMdcRawStruct))
      ((HParSet*)fMdcRawStruct)->init(); //Needs explicit initialization 
  fMdcLookupRaw = (HMdcLookupRaw*)gHades->getRuntimeDb()->
      getContainer("MdcLookupRaw");
  changed=kFALSE;
  array = new TObjArray(6);
  for(Int_t i=0;i<3;i++)
      verLayerGeomPar[i]=verGeomPar[i]=verLookupRaw[i]=-111111;
  return;
}

HMdcSizesCells::~HMdcSizesCells(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  HMdcGetContainers::deleteCont();
}

Bool_t HMdcSizesCells::initContainer(void) {
  if( !fMdcDet || !fMdcGeomStruct || !HMdcGetContainers::isInited(fLayerGeomPar)
      || !HMdcGetContainers::isInited(fGeomPar)
      || !HMdcGetContainers::isInited(fMdcLookupRaw)
      || !HMdcGetContainers::isInited(fMdcRawStruct) ) return kFALSE;
  if( fLayerGeomPar->hasChanged() || fGeomPar->hasChanged() ||
      fMdcLookupRaw->hasChanged()) {
    for(Int_t i=1; i<3; i++) {
      if(fLayerGeomPar->getInputVersion(i) == verLayerGeomPar[i] &&
          fGeomPar->getInputVersion(i) == verGeomPar[i] &&
          fMdcLookupRaw->getInputVersion(i) == verLookupRaw[i]) continue;
      for(Int_t j=i; j<3; j++) {
        verLayerGeomPar[j]=fLayerGeomPar->getInputVersion(j);
        verGeomPar[j]=fGeomPar->getInputVersion(j);
        verLookupRaw[j]=fMdcLookupRaw->getInputVersion(j);
      }
      changed=kTRUE;
      for (Int_t sec = 0; sec < 6; sec++) {
        nModsSeg[sec][0]=nModsSeg[sec][1]=0;
        for(Int_t mod=0;mod<4;mod++) {
          mdcStatus[sec][mod]=(fMdcDet->getModule(sec,mod)) ? kTRUE:kFALSE;
          if(mdcStatus[sec][mod]) nModsSeg[sec][mod/2]++;
        }
        if( !fMdcDet->isSectorActive(sec) )  continue;
        if( !(*array)[sec] ) 
          (*array)[sec]=new HMdcSizesCellsSec(sec,mdcStatus[sec]);
        if( !fillCont(sec) )  return kFALSE;
      }
      fillTargetPos();
      break;
    }
  } else changed=kFALSE;
  return kTRUE;
}

HMdcSizesCells* HMdcSizesCells::getObject(void) {
  if(!fMdcSizesCells) fMdcSizesCells=new HMdcSizesCells();
  return fMdcSizesCells;
}

void HMdcSizesCells::deleteCont(void) {
  if(fMdcSizesCells) {
    delete fMdcSizesCells;
    fMdcSizesCells=0;
  }
}

HMdcSizesCells* HMdcSizesCells::getExObject(void) {
  return fMdcSizesCells;
}

Int_t HMdcSizesCells::getSize(void) {
  // return the size of the pointer array
  return array->GetEntries();
}

void HMdcSizesCells::clear(void) {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcSizesCellsSec* sec=&(*this)[s];
    if( !sec ) continue;
    for(Int_t m=0;m<4;m++) {
      HMdcSizesCellsMod* mod=&(*sec)[m];
      if( !mod ) continue;
      Int_t nl=mod->getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcSizesCellsLayer* lay=&(*mod)[l];
        if( !lay ) continue;
	Int_t nc=lay->getSize();
	for(Int_t c=0;c<nc;c++)  (*lay)[c].clear();
      }
    }
  }
  changed=kFALSE;
}

Bool_t HMdcSizesCells::fillCont(Int_t sec) {
  // calculation of the container HMdcSizesCells
  // with wire's sizes (2-pint) in coordinate system of sector.
  HMdcSizesCellsSec& fSizesCellsSec=(*this)[sec];
  if(!fGetCont->getLabTransSec(fSizesCellsSec.sysRLab,sec)) return kFALSE;
  for(Int_t mod=0; mod<4; mod++) if(!fillModCont(sec,mod)) return kFALSE;
  return kTRUE;
}

Bool_t HMdcSizesCells::fillModCont(Int_t sec, Int_t mod, 
    HGeomTransform * alignTrans) { 
  // filling HMdcSizesCellsMod, HMdcSizesCellsLayer, HMdcSizesCellsCell
  // objects for MDC module "mod" in sector "sec"
  // "wireSizes" are (2-point) in coordinate system of sector.
  // if alignTrans>0 - correction of mdc position (must be done after
  // filling HMdcSizesCells!)
  if(sec<0 || sec>5 || mod<0 || mod>3) return kFALSE; 
  HMdcSizesCellsSec& fSizesCellsSec=(*this)[sec];
  if(&fSizesCellsSec==0) return kFALSE;
  if( !mdcStatus[sec][mod] ) return kTRUE;
  
  Double_t x[4],y[4];

  HGeomCompositeVolume* fComVol=fGetCont->getGeomCompositeVolume(mod);
  if(!fComVol) {
    Error("fillCont","GeomCompositeVolume for MDC%i is absent.",mod+1);
    return kFALSE;
  }
  HMdcSizesCellsMod& fSCellsMod=fSizesCellsSec[mod];
  HGeomTransform sysRSec;
  if(!fGetCont->getSecTransMod(sysRSec,sec,mod)) return kFALSE;
  
  if(alignTrans) {
    sysRSec.transTo(*alignTrans);
    changed = kTRUE;
  }
  
  if(!fSCellsMod.setSecTrans(sysRSec)) return kFALSE;
  for(Int_t layer=0; layer<6; layer++) {
    HMdcLayerGeomParLay& fLayerGeomParLay = (*fLayerGeomPar)[sec][mod][layer];
    HMdcSizesCellsLayer& fSCellsLayer=fSCellsMod[layer];
    if(!fSCellsLayer.setSecTrans(alignTrans)) return kFALSE;
    Int_t nCells=fSCellsLayer.getSize();
    if(nCells<0) continue;
    HGeomVolume* fGeomVol=fComVol->getComponent(layer);
    if(!fGeomVol) {
      Error("fillCont","GeomCompositeVol. for lay. %i doesn't exist!",layer+1);
      return kFALSE;
    }

    Int_t nPoint=fGeomVol->getNumPoints();
    if(nPoint != 8) {
      Error("fillCont","NumPoints for layer volume not eg. 8.");
      return kFALSE;
    }
    for(nPoint=0; nPoint<4;nPoint++) {
      const HGeomVector* pnt=fGeomVol->getPoint(nPoint); // mm!
      x[nPoint]=pnt->getX();
      y[nPoint]=pnt->getY();
    }

    Double_t pitch=fLayerGeomParLay.getPitch();      // mm!
    Double_t wOr=fLayerGeomParLay.getWireOrient()*TMath::Pi()/180.;
    Double_t cosWiOr=cos(wOr);
    Double_t sinWiOr=sin(wOr);
    Double_t wOfSet=(fLayerGeomParLay.getCentralWireNr()-1.)*pitch;
    Double_t a,b;
    a=tan(wOr);   // y=a*x+ln[1]
    // ----- calcBLines ----------------
    Double_t at[4],bt[4];
    for(Int_t i1=0; i1<4; i1++) {
      Int_t i2=i1+1;
      if(i2==4) i2=0;
      at[i1]=(y[i2]-y[i1])/(x[i2]-x[i1]);
      bt[i1]=y[i1]-at[i1]*x[i1];
    }
    //----------------------------------
    fSCellsLayer.cosWireOr=cosWiOr;
    fSCellsLayer.sinWireOr=sinWiOr;
    fSCellsLayer.wireOffSet=wOfSet;
    fSCellsLayer.pitch=pitch;
    fSCellsLayer.halfPitch=pitch*0.5;
    fSCellsLayer.halfCatDist=fLayerGeomParLay.getCatDist()*0.5; // mm!

    Double_t ymax, ymin;
    ymax=-1.e+30;
    ymin=1.e+30;
    for(Int_t i=0; i<4; i++) {
      Double_t yp=y[i]*cosWiOr-x[i]*sinWiOr;
      if(yp>ymax) ymax=yp;
      if(yp<ymin) ymin=yp;
    }
    for(Int_t cell=0; cell<nCells; cell++) {
      b=(cell*pitch-wOfSet)/cosWiOr;
      HMdcSizesCellsCell& fSizesCellsCell=fSCellsLayer[cell];
      HGeomVector& lineP1=fSizesCellsCell.wireSizes[0];
      HGeomVector& lineP2=fSizesCellsCell.wireSizes[1];
      //---------- Calculation of wire size ------------------
      //
      //
      //            Line 1
      //          ----------                 ^ (y)
      //          \ Layre  /                 |
      // Line 0 -> \ MDC  /  <- Line 2       |
      //            \____/                   |
      //            Line 3                   |
      //  (x) <--------|----------------------
      //              0,0
      //
      lineP1.clear();
      lineP2.clear();
      Double_t x1=(bt[0]-b)/(a-at[0]);       //Xb    line 0
      Double_t x2=(bt[2]-b)/(a-at[2]);       //Xe    line 2
      Double_t y1=a*x1+b;                    // Yb
      Double_t y2=a*x2+b;                    // Ye
      Int_t nLine1=(x1>=x[0] && x1<=x[1] && y1>=y[0] && y1<=y[1]) ? 0:-1;
      Int_t nLine2=(x2<=x[3] && x2>=x[2] && y2>=y[3] && y2<=y[2]) ? 2:-1;
      if(nLine1<0 && nLine2 <0) {
        if(cell<100) {
          Warning("fillCont","S%i M%i L%i: Cell %i is out of MDC.",
          sec+1,mod+1,layer+1,cell+1);
          continue;
        } else {
          Warning("fillCont","S%i M%i L%i: Cells %i-%i are out of MDC.",
          sec+1,mod+1,layer+1,cell+1,nCells);
          break;
        }
      }
      if(nLine1<0) {
        x1=(bt[1]-b)/(a-at[1]);               // Xb    line 1
        if(x1<x[2])  x1=(bt[3]-b)/(a-at[3]);  // Xb    line 3
        y1=a*x1+b;                            // Yb
      } else if(nLine2<0) {
        x2=(bt[1]-b)/(a-at[1]);               // Xe    line 1
        if(x2>x[1]) x2=(bt[3]-b)/(a-at[3]);   // Xe    line 3
        y2=a*x2+b;                            // Ye
      }
      Char_t readOutSide=findReadOutSide(sec,mod,layer,cell);
      if( readOutSide=='0'){
	if( cell>3 && cell<(((*fMdcGeomStruct)[sec][mod][layer])-4)) {
          if(alignTrans==0) Warning("fillCont",
            " %iS%iM%iL: Wire %i is not connected.",sec+1,mod+1,layer+1,cell+1);
	}
      } else {
        if( (readOutSide=='1' && x1<x2) || (readOutSide=='2' && x1>x2) ||
            (readOutSide=='3' && y1<y2) ) {
          Double_t tmp;
          tmp=x1; x1=x2; x2=tmp;
          tmp=y1; y1=y2; y2=tmp;
        }
        if(readOutSide=='3') readOutSide=(x1>x2) ? '1':'2';
      }
      fSizesCellsCell.readOutSide=readOutSide;
      fSizesCellsCell.xReadout=x1*fSCellsLayer.cosWireOr +
          y1*fSCellsLayer.sinWireOr;
      fSizesCellsCell.length=fabs(x2*fSCellsLayer.cosWireOr +
          y2*fSCellsLayer.sinWireOr-fSizesCellsCell.xReadout);
      lineP1.setXYZ(x1,y1,0.);
      lineP2.setXYZ(x2,y2,0.);
      lineP1=fSCellsLayer.sysRSec.transFrom(lineP1);
      lineP2=fSCellsLayer.sysRSec.transFrom(lineP2);
      fSizesCellsCell.setStatus(kTRUE);
    }
  }
  return kTRUE;
}

Bool_t HMdcSizesCells::getCellVol(Int_t sec,Int_t mod,Int_t lay,Int_t cell,
                                                         HMdcTrap& volCell) {
  // Return the cell volume as HMdcTrap (8 points) in sector(!) coordinate sys..
  HGeomVector layerVol[8];  // Drift gas vol. (8-points)
  if( &((*this)[sec]) == 0 ) return kFALSE;
  HMdcSizesCellsMod& fSCellsMod=(*this)[sec][mod];
  if(!&fSCellsMod) return kFALSE;
  HGeomCompositeVolume* fComVol=fGetCont->getGeomCompositeVolume(mod);
  if(!fComVol) return kFALSE;
  HMdcSizesCellsLayer& fSCellsLay=fSCellsMod[lay];
  if(cell<0 || cell>=fSCellsLay.getSize() || !fSCellsLay[cell].getStatus())
    return kFALSE;
  HGeomVolume* fGeomVol=fComVol->getComponent(lay);
  for(Int_t point=0; point<4; point++) {
    layerVol[point]=*(fGeomVol->getPoint(point)); // mm!
    layerVol[point+4]=layerVol[point];
    layerVol[point].setZ(-fSCellsLay.halfCatDist);
    layerVol[point+4].setZ(+fSCellsLay.halfCatDist);
  }
  Double_t a,b;
  a=fSCellsLay.sinWireOr/fSCellsLay.cosWireOr;   // y=a*x+ln[1]
  // ----- calcBLines ----------------
  Double_t at[4],bt[4];
  for(Int_t i1=0; i1<4; i1++) {
    Int_t i2=i1+1;
    if(i2==4) i2=0;
    at[i1]=(layerVol[i2].getY()-layerVol[i1].getY())/
           (layerVol[i2].getX()-layerVol[i1].getX());
    bt[i1]=layerVol[i1].getY()-at[i1]*layerVol[i1].getX();
  }
  Double_t ymax, ymin;
  ymax=-1.e+30;
  ymin=1.e+30;
  for(Int_t i=0; i<4; i++) {
    Double_t y=layerVol[i].getY()*fSCellsLay.cosWireOr-
               layerVol[i].getX()*fSCellsLay.sinWireOr;
    if(y>ymax) ymax=y;
    if(y<ymin) ymin=y;
  }
  b=(cell*fSCellsLay.pitch-fSCellsLay.wireOffSet)/fSCellsLay.cosWireOr;
  //---------- Calculation of wire size ------------------
  //
  //
  //            Line 1
  //          ----------                 ^ (y)
  //          \ Layre  /                 |
  // Line 0 -> \ MDC  /  <- Line 2       |
  //            \____/                   |
  //            Line 3                   |
  //  (x) <--------|----------------------
  //              0,0
  //
  Double_t x1=(bt[0]-b)/(a-at[0]);       //Xb    line 0
  Double_t x2=(bt[2]-b)/(a-at[2]);       //Xe    line 2
  Double_t y1=a*x1+b;                    // Yb
  Double_t y2=a*x2+b;                    // Ye
  Int_t nLine1=(x1>=layerVol[0].getX() && x1<=layerVol[1].getX() &&
                y1>=layerVol[0].getY() && y1<=layerVol[1].getY()) ? 0:-1;
  Int_t nLine2=(x2<=layerVol[3].getX() && x2>=layerVol[2].getX() &&
                y2>=layerVol[3].getY() && y2<=layerVol[2].getY()) ? 2:-1;
  if(nLine1<0 && nLine2 <0) return kFALSE;
  if(nLine1<0) nLine1=((bt[1]-b)/(a-at[1]) >= layerVol[2].getX()) ? 1:3;
  else if(nLine2<0)  nLine2=((bt[1]-b)/(a-at[1]) <= layerVol[1].getX()) ? 1:3;

  Double_t tLine,bLine;
  bLine=cell*fSCellsLay.pitch-fSCellsLay.wireOffSet-fSCellsLay.pitch/2.;
  tLine=cell*fSCellsLay.pitch-fSCellsLay.wireOffSet+fSCellsLay.pitch/2.;
  if(tLine > ymax) tLine=ymax;
  if(bLine < ymin) bLine=ymin;
  bLine=bLine/fSCellsLay.cosWireOr;
  tLine=tLine/fSCellsLay.cosWireOr;

  Double_t x,z;
  z=layerVol[0](2);
  x=(bt[nLine1]-bLine)/(a-at[nLine1]);
  volCell[0].setXYZ(x,a*x+bLine,z);
  x=(bt[nLine1]-tLine)/(a-at[nLine1]);
  volCell[1].setXYZ(x,a*x+tLine,z);
  x=(bt[nLine2]-tLine)/(a-at[nLine2]);
  volCell[2].setXYZ(x,a*x+tLine,z);
  x=(bt[nLine2]-bLine)/(a-at[nLine2]);
  volCell[3].setXYZ(x,a*x+bLine,z);
  for(Int_t i=0; i<4; i++) {
    volCell[i+4]=volCell[i];
    volCell[i+4].setZ(layerVol[4](2));
  }

  volCell.transFrom(fSCellsLay.sysRSec);

  return kTRUE;
}

Char_t HMdcSizesCells::findReadOutSide(Int_t s,Int_t m,Int_t l,Int_t c) {
  // Function to get the side from which the corresponding wire is read out.
  // Input is sector,module,layer,cell and return values are '1' for left,
  // '2' for right, '3' for top na '0' for GEANT data and not connected wires.
  if(!fMdcLookupRaw || !fMdcRawStruct) return '0';
  HMdcLookupCell &cell=(*fMdcLookupRaw)[s][m][l][c];
  if(!&cell) return '0';
  Int_t mbo=cell.getNMoth();
  if(mbo < 0) return '0';  // don't conected wires have mbo==-1
  HMdcRawMothStru& rMbo=(*fMdcRawStruct)[s][m][mbo];
  if(!&rMbo) return '0';
  Char_t mbn=rMbo.GetName()[0];
  return (mbn=='1' || mbn=='2' || mbn=='3') ? mbn:'0';
}

Bool_t HMdcSizesCells::fillTargetPos(void){
  // Filling target parameters
  Int_t nT=fSpecGeomPar->getNumTargets(); //-1;
  if( nT < 1 ) {
    Error("calcTarget","Number of targets = %i!",nT+1);
    return kFALSE;
  }
  HGeomVolume* tr1=0;
  HGeomVolume* tr2=0;
  for(Int_t t=0;t<nT;t++) {
    HGeomVolume* tr=fSpecGeomPar->getTarget(t);
    if(!tr) {
      Error("calcTarget","GeomVolume for target absent!");
      return kFALSE;
    }
    if(tr1==0) tr1=tr2=tr;
    else if( tr ->getTransform().getTransVector().getZ() <
             tr1->getTransform().getTransVector().getZ() ) tr1=tr;
    else if( tr ->getTransform().getTransVector().getZ() >
             tr2->getTransform().getTransVector().getZ() ) tr2=tr;
  }

  target[0]=tr1->getTransform().getTransVector();
  Double_t dz1=tr1->getPoint(0)->getZ();
  if(dz1 > tr1->getPoint(2)->getZ()) dz1=tr1->getPoint(2)->getZ();
  target[0].setZ(target[0].getZ() + dz1);
  
  target[2]=tr2->getTransform().getTransVector();
  Double_t dz2=tr2->getPoint(2)->getZ();
  if(dz2 < tr2->getPoint(0)->getZ()) dz2=tr2->getPoint(0)->getZ();
  target[2].setZ(target[2].getZ() + dz2);
  
  target[1]=target[0]+target[2];
  target[1] *= 0.5;
  
  for(Int_t sec=0;sec<6;sec++) {
    if((*array)[sec] == 0) continue;
    HMdcSizesCellsSec* fSec=(HMdcSizesCellsSec*)((*array)[sec]);
    const HGeomTransform* trans=fSec->getLabTrans();
    for(Int_t i=0;i<3;i++) fSec->target[i]=trans->transTo(target[i]);
  }
  return kTRUE;
}

void HMdcSizesCells::calcMdcSeg(Double_t x1, Double_t y1, Double_t z1,
                                Double_t x2, Double_t y2, Double_t z2,
    Double_t eX1, Double_t eY1, Double_t eZ1, Double_t eX2, Double_t eY2,
    Double_t dZ1dX1, Double_t dZ1dY1, Double_t dZ2dX2, Double_t dZ2dY2,
    Double_t& zm, Double_t& eZm, Double_t& r0,  Double_t& eR0,
    Double_t& theta, Double_t& eTheta, Double_t& phi, Double_t& ePhi) {
  // Calcul. a HMdcSeg hit.
  // Input and output are in sector coor.sys.
  // theta=atan(sqrt(dX*dX+dY*dY)/dZ);  phi=atan(dY/dX)
  // zm= z1 - cos(theta)/sin(theta) * (x1*cos(phi)+y1*sin(phi))
  // r0=y1*cos(phi)-x1*sin(phi)
  //
  // If (x1,y1,z1)=(x2,y2,z2) dZ will eq.1.!
  // Input param:
  // x1,y1,z1(P1) - target or point on plane
  // x2,y2,z2(P2) - point on plane
  // eX1,eY1,eZ1, eX2,eY2 - errors (eZ2=0!)
  // dZ2dX2 = -A2  where A2 - plane param. (z2=D2-A2*x2+B2*y2)
  // dZ2dY2 = -B2  where B2 - plane param. (z2=D2-A2*x2+B2*y2)
  // If P1-on plane
  //   eZ1=0;
  //   dZ1dX1 = -A1  where A1 - plane param. (z1=D1-A1*x1+B1*y1)
  //   dZ1dY1 = -B1  where B1 - plane param. (z1=D1-A1*x1+B1*y1)
  // If P1-target:
  //   dZ1dX1 = dZ1/dY1 = 0;
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t lenXY=sqrt(dX*dX+dY*dY);
  if(lenXY<2.E-5) {
    dX =x2 * 1.E-5/sqrt(x2*x2+y2*y2);
    dY =y2 * 1.E-5/sqrt(x2*x2+y2*y2);
    lenXY=1.E-5;            // = dX*dX+dY*dY;
    dZ=1.;
  }
  dX /= lenXY;
  dY /= lenXY;
  dZ /= lenXY;
  phi=atan2(dY,dX);
  theta=atan2(1.,dZ);
  Double_t xy2=(x2*dX+y2*dY);
  Double_t xy1=(x1*dX+y1*dY);
  zm=(z1*xy2-z2*xy1)/lenXY;  //= z1-dZ*(x1*dX+y1*dY);
  r0=y1*dX-x1*dY;

  Double_t dR0dX1=(y2-r0*dX)*eX1;  // *(-1)
  Double_t dR0dX2=(y1-r0*dX)*eX2;
  Double_t dR0dY1=(x2+r0*dY)*eY1;
  Double_t dR0dY2=(x1+r0*dY)*eY2;  // *(-1)
  eR0=sqrt(dR0dX1*dR0dX1+dR0dY1*dR0dY1+dR0dX2*dR0dX2+dR0dY2*dR0dY2)/lenXY;

  Double_t zx12=(z1*dX-x1*dZ)-2*zm*dX;
  Double_t zy12=(z1*dY-y1*dZ)-2*zm*dY;
  Double_t dZmdZ1=  xy2*eZ1;
  Double_t dZmdX1=(xy2*dZ1dX1-zx12-z2*dX)*eX1;
  Double_t dZmdX2=(xy1*dZ2dX2-zx12-z1*dX)*eX2;  // *(-1)
  Double_t dZmdY1=(xy2*dZ1dY1-zy12-z2*dY)*eY1;
  Double_t dZmdY2=(xy1*dZ2dY2-zy12-z1*dY)*eY2;  // *(-1)
  eZm=sqrt(dZmdZ1*dZmdZ1+dZmdX1*dZmdX1+dZmdX2*dZmdX2+dZmdY1*dZmdY1+
      dZmdY2*dZmdY2)/lenXY;

  Double_t drvTh=1./(1.+dZ*dZ);
  Double_t dThdX1=(dZ*dX-dZ1dX1)*eX1; // *(-1)
  Double_t dThdY1=(dZ*dY-dZ1dY1)*eY1; // *(-1)
  Double_t dThdX2=(dZ*dX-dZ2dX2)*eX2;
  Double_t dThdY2=(dZ*dY-dZ2dY2)*eY2;
  //  Double_t dThdZ1=eZ1;
  eTheta=sqrt(dThdX1*dThdX1+dThdY1*dThdY1+eZ1*eZ1+dThdX2*dThdX2+dThdY2*dThdY2)*
      drvTh/lenXY;

  Double_t dPhdX1=dY*eX1;
  Double_t dPhdX2=dY*eX2; // *(-1)
  Double_t dPhdY1=dX*eY1; // *(-1)
  Double_t dPhdY2=dX*eY2;
  ePhi=sqrt(dPhdX1*dPhdX1+dPhdX2*dPhdX2+dPhdY1*dPhdY1+dPhdY2*dPhdY2)/lenXY;
}

void HMdcSizesCells::calcRZToTargLine(Double_t x1, Double_t y1, Double_t z1, 
                                Double_t x2, Double_t y2, Double_t z2, 
    Double_t &zm, Double_t &r0) const {
  // Input and output are in lab.sys.
  calcRZToLineXY(x1,y1,z1,x2,y2,z2,target[2].getX(),target[2].getY(),zm,r0);
}

void HMdcSizesCellsSec::calcRZToTargLine(Double_t x1, Double_t y1, Double_t z1, 
                                Double_t x2, Double_t y2, Double_t z2, 
    Double_t &zm, Double_t &r0) const {
  // Input and output are in sector coor.sys.
  HMdcSizesCells::calcRZToLineXY(x1,y1,z1,x2,y2,z2,
      target[2].getX(),target[2].getY(),zm,r0);
}

void HMdcSizesCells::calcRZToLineXY(Double_t x1, Double_t y1, Double_t z1, 
                                Double_t x2, Double_t y2, Double_t z2, 
    Double_t xBeam, Double_t yBeam, Double_t &zm, Double_t &r0) {
  // zm= z1 - cos(theta)/sin(theta) * ((x1-xBeam)*cos(phi)+(y1-yBeam)*sin(phi))
  // r0=(y1-yBeam)*cos(phi)-(x1-xBeam)*sin(phi)
  //
  // If (x1,y1,z1)=(x2,y2,z2) dZ will eq.1.! 
  Double_t dX=x2-x1;
  Double_t dY=y2-y1;
  Double_t dZ=z2-z1;
  Double_t lenXY2=dX*dX+dY*dY;
  Double_t lenXY=sqrt(lenXY2);
  if(lenXY<2.E-5) {
    dX =x2 * 1.E-5/sqrt(x2*x2+y2*y2);
    dY =y2 * 1.E-5/sqrt(x2*x2+y2*y2);
    lenXY=1.E-5;            //dX*dX+dY*dY;
    lenXY2=lenXY*lenXY;
    dZ=1.;
  }
  x1-=xBeam;  // Shifting to beam line
  y1-=yBeam;  // Shifting to beam line
  
  zm=z1-dZ*(x1*dX+y1*dY)/lenXY2;
  r0=(y1*dX-x1*dY)/lenXY;
}
