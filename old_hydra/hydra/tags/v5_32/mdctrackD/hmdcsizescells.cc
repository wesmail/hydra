#include "hmdcsizescells.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hmdcgeomstruct.h"
#include <iostream.h>
#include "math.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdcgetcontainers.h"
#include "hmdclayergeompar.h"
#include "hmdcgeompar.h"

//*-- Author : Vladimir Pechenov
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
//  in coordinad system of sector and transformations layer<->mod,
//  layer<->sec, ...
//  It has same useful functions:
//  HMdcSizesCellsLayer:
//    HGeomTransform* getSecTrans();
//    HGeomTransform* getModTrans();
//    void transSecToRotLayer(HGeomVector& point);
//    Double_t getAlfa(const HGeomVector& p1, const HGeomVector& p2);
//    Double_t getDist(const HGeomVector& p1, 
//                     const HGeomVector& p2, Int_t cell);
//    void getImpact(const HGeomVector& p1, const HGeomVector& p2, 
//                  Int_t cell, Double_t &alfa, Double_t &per);
//    Int_t getCellNum(Float_t x, Float_t y):
//      calculation the the cell number for the point x,y
//      on the wire plane. (x,y - in coor. sys. of layer, z=0)
//  HMdcSizesCellsMod:
//    HGeomTransform* getSecTrans();
//  HMdcSizesCellsSec:
//    HGeomTransform* getLabTrans();
//  HMdcSizesCells:
//    Bool_t getCellVol(Int_t sec, Int_t mod, Int_t lay,
//    Int_t cell, HMdcTrap& volCell);
//
//////////////////////////////////////////////////////////////

ClassImp(HMdcSizesCellsCell)
ClassImp(HMdcSizesCellsLayer)
ClassImp(HMdcSizesCellsMod)
ClassImp(HMdcSizesCellsSec)
ClassImp(HMdcSizesCells)

void HMdcSizesCellsCell::clear() {
  wireSizes[0].clear();
  wireSizes[1].clear();
  status=kFALSE;
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
}

HMdcSizesCellsLayer::~HMdcSizesCellsLayer() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdcSizesCellsLayer::getSize() {
  // return the size of the pointer array
  return array->GetLast()+1; 
}


Int_t HMdcSizesCellsLayer::calcCellNum(Float_t x, Float_t y) {
  //  calculation the the cell number for the point x,y
  //  on the wire plane. (x,y - in coor. sys. of layer, z=0)
  Float_t wire=(wireOffSet + y*cosWireOr-x*sinWireOr)/pitch+0.5;
  Int_t nWire=(Int_t)wire;
  return (wire<0. || nWire>=nCells) ? -1:nWire;
}

void HMdcSizesCellsLayer::transSecToRotLayer(HGeomVector& point) {
  // transformation to coor. sys. of rotated layer from sector sys.
  point=sysRSec.transTo(point);
  Float_t y = point.getY()*cosWireOr-point.getX()*sinWireOr;
  Float_t x = point.getX()*cosWireOr+point.getY()*sinWireOr;
  point.setX(x);
  point.setY(y);
}



Double_t HMdcSizesCellsLayer::getAlfa(const HGeomVector& p1, 
                                      const HGeomVector& p2) {
  // return the angle Alfa (in degree) .
  HGeomVector tranP1(p1);
  HGeomVector tranP2(p2);
  transSecToRotLayer(tranP1);
  transSecToRotLayer(tranP2);
  Float_t dz=tranP2.getZ()-tranP1.getZ();
  Float_t dy=tranP2.getY()-tranP1.getY();
  Float_t alfa=atan2(dz,dy)*180./acos(-1.);
  return alfa;
}

Double_t HMdcSizesCellsLayer::getDist(const HGeomVector& p1,
                                      const HGeomVector& p2,Int_t cell){
  // return the minimal distance from line (p1-p2) to wire.
  HGeomVector tranP1(p1);
  HGeomVector tranP2(p2);
  transSecToRotLayer(tranP1);
  transSecToRotLayer(tranP2);
  Double_t dz=tranP2.getZ()-tranP1.getZ();
  Double_t dy=tranP2.getY()-tranP1.getY();
  Double_t alfa=atan2(dz,dy)*180./acos(-1.);
  Double_t yZ0=-(tranP1.getZ()*tranP2.getY()-tranP2.getZ()*tranP1.getY())/dz;
  Double_t yDist = yZ0-(cell*pitch-wireOffSet);
  Double_t per = fabs(yDist*sin(alfa));
  return per;
}

void HMdcSizesCellsLayer::getImpact(const HGeomVector& p1, 
    const HGeomVector& p2, Int_t cell, Double_t &alfa, Double_t &per) {
  // return the angle Alfa (in degree) and 
  // the minimal distance from line (p1-p2) to wire.
  HGeomVector tranP1(p1);
  HGeomVector tranP2(p2);
  transSecToRotLayer(tranP1);
  transSecToRotLayer(tranP2);
  Double_t dz=tranP2.getZ()-tranP1.getZ();
  Double_t dy=tranP2.getY()-tranP1.getY();
  alfa=atan2(dz,dy);
  Double_t yZ0=-(tranP1.getZ()*tranP2.getY()-tranP2.getZ()*tranP1.getY())/dz;
  Double_t yDist = yZ0-(cell*pitch-wireOffSet);
  per = fabs(yDist*sin(alfa));
}

HMdcSizesCellsMod::HMdcSizesCellsMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type
  // HMdcSizesCellsLayer
  sector=sec;
  module=mod;
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++) 
      (*array)[layer] = new HMdcSizesCellsLayer(sec,mod,layer);
}

HMdcSizesCellsMod::~HMdcSizesCellsMod() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdcSizesCellsMod::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

HMdcSizesCellsSec::HMdcSizesCellsSec(Int_t sec) {
  // constructor creates an array of pointers of type
  // HMdcSizesCellsMod
  sector=sec;
  array = new TObjArray(4);
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  HMdcDetector* fmdc = fGetCont->getMdcDetector();
  for (Int_t mod = 0; mod < 4; mod++)
    if(fmdc->getModule(sec,mod)) (*array)[mod] = new HMdcSizesCellsMod(sec,mod);
}

HMdcSizesCellsSec::~HMdcSizesCellsSec() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdcSizesCellsSec::getSize() {
  // return the size of the pointer array 
  return array->GetEntries();
}

//==================================================
HMdcSizesCells* HMdcSizesCells::fMdcSizesCells=0;

HMdcSizesCells::HMdcSizesCells() {
  // constructor creates an array of pointers of type HMdcSizesCellsSec
  fGetCont=HMdcGetContainers::getObject();
  fLayerGeomPar = fGetCont->getMdcLayerGeomPar();
  fGeomPar      = fGetCont->getMdcGeomPar();
  fMdcDet       = fGetCont->getMdcDetector();
  fMdcGeomStruct= fGetCont->getMdcGeomStruct();
  changed=kFALSE;
  array = new TObjArray(6);
  return;
}



HMdcSizesCells::~HMdcSizesCells() {
  // destructor
  if(array) array->Delete();
  HMdcGetContainers::deleteCont();
}

Bool_t HMdcSizesCells::initContainer() {
  if( !fMdcDet || !fMdcGeomStruct || !HMdcGetContainers::isInited(fLayerGeomPar)
      || !HMdcGetContainers::isInited(fGeomPar) ) return kFALSE;
  if( fLayerGeomPar->hasChanged() || fGeomPar->hasChanged() ) {
    changed=kTRUE;
    for (Int_t sec = 0; sec < 6; sec++) {
      if( !fMdcDet->isSectorActive(sec) )  continue;
      if( !(*array)[sec] ) (*array)[sec]=new HMdcSizesCellsSec(sec);
      if( !fillCont(sec) )  return kFALSE;
    }
  } else changed=kFALSE;
  return kTRUE;
}

HMdcSizesCells* HMdcSizesCells::getObject() {
  if(!fMdcSizesCells) fMdcSizesCells=new HMdcSizesCells();
  return fMdcSizesCells;
}

void HMdcSizesCells::deleteCont() {
  if(fMdcSizesCells) {
    delete fMdcSizesCells;
    fMdcSizesCells=0;
  }
}

HMdcSizesCells* HMdcSizesCells::getExObject() {
  return fMdcSizesCells;
}

Int_t HMdcSizesCells::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}

void HMdcSizesCells::clear() {
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
  // with wire's sizes (2-pint) in coordinat system of sector.
  HMdcSizesCellsSec& fSizesCellsSec=(*this)[sec];
  if(!fGetCont->getLabTransSec(fSizesCellsSec.sysRLab,sec)) return kFALSE;
  HGeomVector volSeWi[4];
  for(Int_t mod=0; mod<4; mod++) {
    if( ! fMdcDet->getModule(sec,mod) ) continue;
    HGeomCompositeVolume* fComVol=fGetCont->getGeomCompositeVolume(mod);
    if(!fComVol) {
      printf("Error: GeomCompositeVolume for MDC%i is absent!\n",mod+1);
      return kFALSE;
    }
    HMdcSizesCellsMod& fSCellsMod=fSizesCellsSec[mod];
    if(!fGetCont->getSecTransMod(fSCellsMod.sysRSec,sec,mod)) return kFALSE;
    for(Int_t layer=0; layer<6; layer++) {
      HMdcLayerGeomParLay& fLayerGeomParLay = (*fLayerGeomPar)[sec][mod][layer];
      HMdcSizesCellsLayer& fSCellsLayer=fSCellsMod[layer];
      if(!fGetCont->getSecTransLayerZ0(fSCellsLayer.sysRSec,sec,mod,layer)) 
        return kFALSE;
      fSCellsLayer.setLayerPlane();
      if(!fGetCont->getModTransLayerZ0(fSCellsLayer.sysRMod,mod,layer))
        return kFALSE;
      Int_t nCells=fSCellsLayer.getSize();     
      if(nCells<0) continue;
      HGeomVolume* fGeomVol=fComVol->getComponent(layer);
      if(!fGeomVol) {
        printf("Error: GeomCompositeVolume for Layer %i is absent!\n",layer+1);
        return kFALSE;
      }   

      Int_t nPoint=fGeomVol->getNumPoints();
      if(nPoint != 8) { 
        printf("Error NumPoints for layer volume no eg. 8!!!");
        return kFALSE;
      }
      for(nPoint=0; nPoint<4;nPoint++) {
        volSeWi[nPoint]=*(fGeomVol->getPoint(nPoint)); // mm!
        volSeWi[nPoint].setZ(0.0);
      }

      Double_t pitch=fLayerGeomParLay.getPitch();      // mm!
      Double_t wOr=fLayerGeomParLay.getWireOrient()/180.*acos(-1.);
      Double_t cosWiOr=cos(wOr);
      Double_t wOfSet=(fLayerGeomParLay.getCentralWireNr()-1.)*pitch;
      Double_t a,b;
      a=tan(wOr);   // y=a*x+ln[1]
      // ----- calcBLines ----------------     
      Double_t at[4],bt[4];
      for(Int_t i1=0; i1<4; i1++) {
        Int_t i2=i1+1; 
        if(i2==4) i2=0;
        at[i1]=(volSeWi[i2].getY()-volSeWi[i1].getY())/
               (volSeWi[i2].getX()-volSeWi[i1].getX());
        bt[i1]=volSeWi[i1].getY()-at[i1]*volSeWi[i1].getX();
      }
      //----------------------------------
      fSCellsLayer.cosWireOr=cosWiOr;
      fSCellsLayer.sinWireOr=sin(wOr);
      fSCellsLayer.wireOffSet=wOfSet;
      fSCellsLayer.pitch=pitch;
      fSCellsLayer.halfCatDist=fLayerGeomParLay.getCatDist()*0.5; // mm!
 
      Double_t ymax, ymin;
      ymax=-1.e+30;
      ymin=1.e+30;
      for(Int_t i=0; i<4; i++) {
        Double_t y=volSeWi[i].getY()*cosWiOr-volSeWi[i].getX()*sin(wOr);
        if(y>ymax) ymax=y;
        if(y<ymin) ymin=y;
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
        Int_t nLine1=(x1>=volSeWi[0].getX() && x1<=volSeWi[1].getX() && 
                      y1>=volSeWi[0].getY() && y1<=volSeWi[1].getY()) ? 0:-1;
        Int_t nLine2=(x2<=volSeWi[3].getX() && x2>=volSeWi[2].getX() && 
                      y2>=volSeWi[3].getY() && y2<=volSeWi[2].getY()) ? 2:-1;
        if(nLine1<0 && nLine2 <0) {
          if(cell<100) {
            Printf("Warning: S%i M%i L%i: Cell %i is out of MDC!",
            sec+1,mod+1,layer+1,cell+1);
            continue;
          } else {
            Printf("Warning: S%i M%i L%i: Cells %i-%i are out of MDC!",
            sec+1,mod+1,layer+1,cell+1,nCells);
            break;
          }
        }
        if(nLine1<0) {
          x1=(bt[1]-b)/(a-at[1]);                            // Xb    line 1
          if(x1<volSeWi[2].getX())  x1=(bt[3]-b)/(a-at[3]);  // Xb    line 3
          y1=a*x1+b;                                         // Yb
        } else if(nLine2<0) {
          x2=(bt[1]-b)/(a-at[1]);                            // Xe    line 1
          if(x2>volSeWi[1].getX()) x2=(bt[3]-b)/(a-at[3]);   // Xe    line 3
          y2=a*x2+b;                                         // Ye
        }
        lineP1.setXYZ(x1,y1,volSeWi[0].getZ());
        lineP2.setXYZ(x2,y2,volSeWi[3].getZ());   
        lineP1=fSCellsLayer.sysRSec.transFrom(lineP1);
        lineP2=fSCellsLayer.sysRSec.transFrom(lineP2);
        fSizesCellsCell.setStatus(kTRUE);
      }
    }
  }
  return kTRUE;
}


Bool_t HMdcSizesCells::getCellVol(Int_t sec,Int_t mod,Int_t lay,Int_t cell,
                                                         HMdcTrap& volCell) {
  // Return the cell volume as HMdcTrap (8 points) in sector(!) coordinate sys..
  HGeomVector layerVol[8];  // Object for drift gas vol. (8-points)
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
  volCell[0].setXYZ(x=(bt[nLine1]-bLine)/(a-at[nLine1]),a*x+bLine,z);
  volCell[1].setXYZ(x=(bt[nLine1]-tLine)/(a-at[nLine1]),a*x+tLine,z);
  volCell[2].setXYZ(x=(bt[nLine2]-tLine)/(a-at[nLine2]),a*x+tLine,z);
  volCell[3].setXYZ(x=(bt[nLine2]-bLine)/(a-at[nLine2]),a*x+bLine,z);
  for(Int_t i=0; i<4; i++) {
    volCell[i+4]=volCell[i];
    volCell[i+4].setZ(layerVol[4](2));
  }

  volCell.transFrom(fSCellsLay.sysRSec);

  return kTRUE;
}
