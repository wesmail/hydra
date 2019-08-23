#include "hmdcsizescells.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgeomstruct.h"
#include <iostream.h>
#include "math.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
//*-- Author : Vladimir Pechenov
//*-- Modified : 07/03/2000 by R. Holzmann
//*-- Modified : 31/8/99 by V. Pechenov

////////////////////////////////////////////////////////////////
//
//  HMdcSizesCells 
//
//  Container class keep volumes of cells (8-point/vol.) and  
//  sizes of wires (2-points/wire) in coordinad system of sector.
//
//
//  HMdcCalcSizesCells
//
//  This class calculate the volume of cells and sizes of wires
//  and fill the conteiner HMdc12CellProjec
//
//////////////////////////////////////////////////////////////

ClassImp(HMdcSizesCellsCell)
ClassImp(HMdcSizesCellsLayer)
ClassImp(HMdcSizesCellsMod)
ClassImp(HMdcSizesCellsSec)
ClassImp(HMdcSizesCells)
ClassImp(HMdcCalcSizesCells)

void HMdcSizesCellsCell::clear() {
 cellSizes.clear();
 wireSizes[0].clear();
 wireSizes[1].clear();
 set=kFALSE;
}


HMdcSizesCellsLayer::HMdcSizesCellsLayer(Int_t sec, Int_t mod, Int_t layer) {
  // Constructor creates an array of pointers of type 
  // HMdcSizesCellsCell
  
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if( ! fMdcDet->getModule(sec,mod) ) return;
  
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  if ( ! pMdc ) {
    pMdc = new HMdcGeomStruct;
    gHades->getRuntimeDb()->addContainer(pMdc);
  }
  
  Int_t nCells=((*pMdc)[sec][mod])[layer];
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


HMdcSizesCellsMod::HMdcSizesCellsMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type
  // HMdcSizesCellsLayer
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
  for (Int_t mod = 0; mod < 4; mod++)
      (*array)[mod] = new HMdcSizesCellsMod(sec,mod);
}

HMdcSizesCellsSec::~HMdcSizesCellsSec() {
  // destructor
  if(array) array->Delete(); 
}

Int_t HMdcSizesCellsSec::getSize() {
  // return the size of the pointer array 
  return array->GetEntries();
}

HMdcSizesCells::HMdcSizesCells() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdcSizesCellsSec
  // The container name is set to "MdcSizesCells"
  fName="MdcSizesCells";
  fTitle="sizes wires for Mdc";
  strcpy(detName,"Mdc");
  array = new TObjArray(6);
  HMdcCalcSizesCells* calc=new HMdcCalcSizesCells();
  for (Int_t sec = 0; sec < 6; sec++) {
   (*array)[sec] = new HMdcSizesCellsSec(sec);
   if( ! calc->calcVolCells(sec, this) ) {
    delete calc;
    isContainer=kFALSE;
    return;
   }
  }
  delete calc;
  isContainer=kTRUE;
}

HMdcSizesCells::~HMdcSizesCells() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdcSizesCells::getSize() {
  // return the size of the pointer array
  return array->GetEntries();
}


Int_t HMdcSizesCells::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HMdcParIo");
  if (out) return out->write(this);
  return -1;
}

void HMdcSizesCells::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdcSizesCellsSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdcSizesCellsMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdcSizesCellsLayer& lay=mod[l];
	Int_t nc=lay.getSize();
	for(Int_t c=0;c<nc;c++) {
         HMdcSizesCellsCell& cell=lay[c];
	 cell.clear();
	}
      }
    }
  } 
  status=kFALSE;
  resetInputVersions();
}


//-----------------------------------------------

HMdcCalcSizesCells::HMdcCalcSizesCells() { 
  fLayerGeomPar=0;
  fGeomPar=0;
  fMdcDet=0;
  setParContainers();
}


void HMdcCalcSizesCells::setParContainers() {
  fLayerGeomPar=(HMdcLayerGeomPar*)gHades->getRuntimeDb()
                                         ->getContainer("MdcLayerGeomPar");
  if (!fLayerGeomPar) {
    fLayerGeomPar=new HMdcLayerGeomPar;
    gHades->getRuntimeDb()->addContainer(fLayerGeomPar);
  } 
  fGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if (!fGeomPar) {
    fGeomPar=new HMdcGeomPar;
    gHades->getRuntimeDb()->addContainer(fGeomPar);
  }
  fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc"); 
}


Bool_t HMdcCalcSizesCells::calcVolCells(Int_t nSec, HMdcSizesCells* fSizesCells) {
  // calculation of the container HMdcSizesCells
  // with vol. of cells (8-point) and wire's sizes (2-pint)
  // in coordinat system of sector.
  
  HGeomVector vTmp;
  HGeomTransform secsys; 
   for(Int_t nmo=0; nmo<4; nmo++) {
    if( ! fMdcDet->getModule(nSec,nmo) ) continue;
    HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(nmo);
    if(!pComVol) {
     printf("Error: GeomCompositeVolume for MDC%i is absent!\n",nmo+1);
     return kFALSE;
    }
    for(Int_t nla=0; nla<6; nla++) {
     HMdcLayerGeomParLay& pLaGePar = (*fLayerGeomPar)[nSec][nmo][nla];
     HMdcSizesCellsLayer& pSCLayer=(*fSizesCells)[nSec][nmo][nla];
     Int_t nCells=pSCLayer.getSize();     
     if(nCells>0) {
      HGeomVolume* pGeomVol=pComVol->getComponent(nla);
      if(!pGeomVol) {
       printf("Error: GeomCompositeVolume for Layer %i is absent!\n",nla+1);
       return kFALSE;
      }   
   
      Int_t nPoint=pGeomVol->getNumPoints();
      if(nPoint != 8) { 
       printf("Error NumPoints for layer volume no eg. 8!!!");
       return kFALSE;
      }
      Double_t dst=pLaGePar.getCatDist()*0.05;      // mm->cm !!!!!!!!!!
      for(nPoint=0; nPoint<8;nPoint++) {
       volSeWi[nPoint]=*(pGeomVol->getPoint(nPoint));
       volSeWi[nPoint]/=10.;                        // mm->cm !!!!!!!!!!
       volDrGas[nPoint]=volSeWi[nPoint];
//       if(nPoint<4) volDrGas[nPoint].setZ(-dst); 
//       else volDrGas[nPoint].setZ(dst); 
         volSeWi[nPoint].setZ(0.0);
      }
  
      Double_t pitch=pLaGePar.getPitch()*0.1;      // mm->cm !!!!!!!!!!
      Double_t wOr=pLaGePar.getWireOrient()/180.*acos(-1.);
      Double_t cosWiOr=cos(wOr);
      Double_t wOfSet=(pLaGePar.getCentralWireNr()-1.)*pitch;
      a=tan(wOr);   // y=a*x+ln[1]
      calcBLines();
      
      secsys.setTransform(pGeomVol->getTransform());
      secsys.transFrom(pComVol->getTransform());      // to coord.sys.of sector !!!
      
      vTmp=secsys.getTransVector();
      vTmp/=10.;                                  // mm->cm !!!!!!!!!!
      secsys.setTransVector(vTmp);
              
      Double_t ymax, ymin;
      ymax=-1.e+30;
      ymin=1.e+30;
      for(Int_t i=0; i<4; i++) {
       Double_t y=volSeWi[i].getY()*cosWiOr-volSeWi[i].getX()*sin(wOr);
       if(y>ymax) ymax=y;
       if(y<ymin) ymin=y;
      }
      for(Int_t nCell=0; nCell<nCells; nCell++) {
       b=(nCell*pitch-wOfSet)/cosWiOr;
       HMdcSizesCellsCell& pSCCell=pSCLayer[nCell];
       HMdcTrap& cell=pSCCell.cell();
       HGeomVector& wireP1=*(pSCCell.getWirePoint(0));
       HGeomVector& wireP2=*(pSCCell.getWirePoint(1));
       if(calcWireLine(wireP1, wireP2)) {
        bLine=nCell*pitch-wOfSet-pitch/2.;
        tLine=nCell*pitch-wOfSet+pitch/2.;
        if(tLine > ymax) tLine=ymax;
        if(bLine < ymin) bLine=ymin;
        bLine=bLine/cosWiOr;
        tLine=tLine/cosWiOr;
        calcVolCell();
        cell.set(volCell);  //!!!!!!!!!!!!!!!!!!!!!!!!!
        pSCCell.setTRUE();
/*       
printf("S%i M%i L%i C%i Trap x1=%f y1=%f  x4=%f y4=%f\n",nSec,nmo,nla,nCell,
cell[0].getx(),cell[0].gety(),cell[3].getx(),cell[3].gety());
printf("S%i M%i L%i C%i Ware x1=%f y1=%f  x2=%f y2=%f\n",nSec,nmo,nla,nCell,
wire[0].getx(),wire[0].gety(),wire[1].getx(),wire[1].gety());
printf("S%i M%i L%i C%i Trap x2=%f y2=%f  x3=%f y3=%f\n\n",nSec,nmo,nla,nCell,
cell[1].getx(),cell[1].gety(),cell[2].getx(),cell[2].gety());
*/    
        cell.transFrom(secsys);
	wireP1=secsys.transFrom(wireP1);
	wireP2=secsys.transFrom(wireP2);
       }
      }
     }
    } 
   }
   return kTRUE;
}

void HMdcCalcSizesCells::calcBLines() {
//calculation of parameters of extornal lines of plane
 for(Int_t i1=0; i1<4; i1++) {
  Int_t i2=i1+1; 
  if(i2==4) i2=0;
  at[i1]=(volSeWi[i2].getY()-volSeWi[i1].getY())/
         (volSeWi[i2].getX()-volSeWi[i1].getX());
  bt[i1]=volSeWi[i1].getY()-at[i1]*volSeWi[i1].getX();
 }
}

Bool_t HMdcCalcSizesCells::calcWireLine(HGeomVector& lineP1, HGeomVector& lineP2) {
//calculation of parameters of extornal lines of plane
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
 nLine1=-1; nLine2=-1;
 if(x1>=volSeWi[0].getX() && x1<=volSeWi[1].getX() && 
    y1>=volSeWi[0].getY() && y1<=volSeWi[1].getY()) nLine1=0;
 if(x2<=volSeWi[3].getX() && x2>=volSeWi[2].getX() && 
    y2>=volSeWi[3].getY() && y2<=volSeWi[2].getY()) nLine2=2;
 if(nLine1<0 && nLine2 <0) return kFALSE;
 if(nLine1<0) {
   x1=(bt[1]-b)/(a-at[1]);       //Xb    line 1
   nLine1=1;
   if(x1<volSeWi[2].getX()) {
    x1=(bt[3]-b)/(a-at[3]);       //Xb    line 3
    nLine1=3;
   }
   y1=a*x1+b;                    // Yb
 }
 else if(nLine2<0) {
   x2=(bt[1]-b)/(a-at[1]);       //Xe    line 1
   nLine2=1;
   if(x2>volSeWi[1].getX()) {
    x2=(bt[3]-b)/(a-at[3]);       //Xe    line 3
    nLine2=3;
   }
   y2=a*x2+b;                    // Ye
 }
 lineP1.setX(x1);
 lineP2.setX(x2);
 lineP1.setY(y1);
 lineP2.setY(y2);
 return kTRUE;
}

void HMdcCalcSizesCells::calcVolCell() {
 volCell.clear();
 volCell[0].setX( (bt[nLine1]-bLine)/(a-at[nLine1]) );	 // Xp0
 volCell[1].setX( (bt[nLine1]-tLine)/(a-at[nLine1]) );	 // Xp1
 volCell[2].setX( (bt[nLine2]-tLine)/(a-at[nLine2]) );	 // Xp2
 volCell[3].setX( (bt[nLine2]-bLine)/(a-at[nLine2]) );	 // Xp3
 volCell[0].setY( a*volCell[0].getX()+bLine );		       // Yp0
 volCell[1].setY( a*volCell[1].getX()+tLine );		    // Yp1
 volCell[2].setY( a*volCell[2].getX()+tLine );		    // Yp2
 volCell[3].setY( a*volCell[3].getX()+bLine );		    // Yp3
 for(Int_t i=0; i<4; i++) {
  volCell[i+4].setX(volCell[i].getX());
  volCell[i+4].setY(volCell[i].getY());
  volCell[i].setZ(volDrGas[0].getZ());                         // Zp0-3
  volCell[i+4].setZ(volDrGas[4].getZ());                       //Zp4-7
 }
}













