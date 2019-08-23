#include "hmdc12cellprojec.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmdcgeomstruct.h"
#include "hgeomcompositevolume.h"
#include "hgeomvolume.h"
#include <iostream.h>
//*-- Author : Vladimir Pechenov
//*-- Modified : 07/03/2000 by R. Holzmann
//*-- Modified : 14/5/99 by V. Pechenov


////////////////////////////////////////////////////
//
//  HMdc12CellProjec 
//
//  Container class keep the projections of sensitive volume 
//  of cells from MDC1&2 to one plane respecting of target.
//
//
//  HMdcCalc12CellProjec
//
//  This class calculate the volumes of the projections 
//  of sensitive volume and fill the conteiner 
//  HMdc12CellProjec
//
/////////////////////////////////////////////////////

ClassImp(HMdc12CellProjecCell)
ClassImp(HMdc12CellProjecLayer)
ClassImp(HMdc12CellProjecMod)
ClassImp(HMdc12CellProjecSec)
ClassImp(HMdc12CellProjec)
ClassImp(HMdcCalc12CellProjec)

void HMdc12CellProjecCell::clear() {
 cellPr.clear();
 set=kFALSE;
}


HMdc12CellProjecLayer::HMdc12CellProjecLayer(Int_t sec, Int_t mod, Int_t layer) {
  // constructor creates an array of pointers of type
  // HMdc12CellProjecCell
  array=0;
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if( ! fMdcDet->getModule(sec,mod) ) return;
  
  HMdcGeomStruct* pMdc=
       (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
  Int_t nCells=((*pMdc)[sec][mod])[layer];
  if( nCells > 0) {
   array = new TClonesArray("HMdc12CellProjecCell",nCells);
   for (Int_t cell = 0; cell < nCells; cell++) 
        new((*array)[cell]) HMdc12CellProjecCell;
  }
}

HMdc12CellProjecLayer::~HMdc12CellProjecLayer() {
  // destructor
  if( array ) array->Delete();
}

Int_t HMdc12CellProjecLayer::getSize() {
  // return the size of the pointer array
  if(array) return array->GetLast()+1;
  else return -1; 
}


HMdc12CellProjecMod::HMdc12CellProjecMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type
  // HMdc12CellProjecLayer 
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++) 
    (*array)[layer] = new HMdc12CellProjecLayer(sec,mod,layer);
}

HMdc12CellProjecMod::~HMdc12CellProjecMod() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdc12CellProjecMod::getSize() {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

HMdc12CellProjecSec::HMdc12CellProjecSec(Int_t sec) {
  // constructor creates an array of pointers of type
  // HMdc12CellProjecMod
  sector=sec;
  array = new TObjArray(2);
  for (Int_t mod = 0; mod < 2; mod++)
      (*array)[mod] = new HMdc12CellProjecMod(sec,mod);
}

HMdc12CellProjecSec::~HMdc12CellProjecSec() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdc12CellProjecSec::getSize() {
  // return the size of the pointer array 
  if(array) return array->GetEntries();
  else return -1;
}

HMdc12CellProjec::HMdc12CellProjec() {
  // constructor creates an array of pointers of type
  // HMdc12CellProjecSec
  array = new TObjArray(6);
  HMdcCalc12CellProjec* calc=new HMdcCalc12CellProjec();
  for (Int_t nSec = 0; nSec < 6; nSec++) {
    (*array)[nSec] = new HMdc12CellProjecSec(nSec);
    if( ! calc->calcMdc12Proj(nSec, this)) {
     delete calc;
     isContainer=kFALSE;
     return;
    }
  }
  delete calc;
  isContainer=kTRUE;
}

HMdc12CellProjec::~HMdc12CellProjec() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdc12CellProjec::getSize() {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

void HMdc12CellProjec::clear() {
  // clears the container
  for(Int_t s=0;s<6;s++) {
    HMdc12CellProjecSec& sec=(*this)[s];
    for(Int_t m=0;m<4;m++) {
      HMdc12CellProjecMod& mod=sec[m];
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdc12CellProjecLayer& lay=mod[l];
	Int_t nc=lay.getSize();
	for(Int_t c=0;c<nc;c++) {
         HMdc12CellProjecCell& cell=lay[c];
	 cell.clear();
	}
      }
    }
  } 
}


//--------------------------------------------------------
HMdcCalc12CellProjec::HMdcCalc12CellProjec(void) {
  fSizesCells=0;
  fGeomPar=0;
  fMdcDet=0;
  setParContainers();
}


HMdcCalc12CellProjec::~HMdcCalc12CellProjec() {
  // destructor
}


void HMdcCalc12CellProjec::setParContainers() {
  fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  fSizesCells=(HMdcSizesCells*)gHades->getRuntimeDb()
                                     ->getContainer("MdcSizesCells");
  if (!fSizesCells) {
    fSizesCells=new HMdcSizesCells;
    gHades->getRuntimeDb()->addContainer(fSizesCells);
  }
  fGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  if (!fGeomPar) {
    fGeomPar=new HMdcGeomPar;
    gHades->getRuntimeDb()->addContainer(fGeomPar);
  }
  fSpecGeomPar=(HSpecGeomPar*)gHades->getRuntimeDb()
                                    ->getContainer("SpecGeomPar");
  if (!fSpecGeomPar) {
    fSpecGeomPar=new HSpecGeomPar;
    gHades->getRuntimeDb()->addContainer(fSpecGeomPar);
  }
  return;
}

Bool_t HMdcCalc12CellProjec::calcMdc12Proj(Int_t nSec, 
                                 HMdc12CellProjec* f12CellProjec) {

  if(!fMdcDet->getModule(nSec,0) && !fMdcDet->getModule(nSec,1)) return kTRUE;
  HGeomTransform coordSys;

  //Get size of target
  //
  Int_t nT=fSpecGeomPar->getNumTargets()-1;
  if( nT < 0 ) {
    Printf("Error: Number of targets = %i!\n",nT);
    return kFALSE;
  }
  Double_t zF, zL;
  zF=((fSpecGeomPar->getTarget( 0)->getTransform()).getTransVector()).getZ();
  zF+=fSpecGeomPar->getTarget( 0)->getPoint(0)->getZ();
  zL=((fSpecGeomPar->getTarget(nT)->getTransform()).getTransVector()).getZ();
  zL+=fSpecGeomPar->getTarget(nT)->getPoint(2)->getZ();
  Double_t targ1[3]={0.f, 0.f,  zF/10. };     //  mm->cm !!!!!!!!!
  Double_t targ2[3]={0.f, 0.f,  zL/10. };     //  mm->cm !!!!!!!!!
  targetPs[0].setVector(targ1);
  targetPs[1].setVector(targ2);
//------------------------------------------- 
//Sectors:
//                     HMdcDigitGeomParSec& sec = (*fDigitGeomPar)[nSec];   

//  Calc. of plane for projection:
//  It's the plane between MDC0 and MDC1
//  par=0.0 -> plane=layer 6, MDC1
//  par=1.0 -> plane=layer 1, MDC2

  Double_t par=1.0;   // <<<===================== use MDC1
  if(par<0.) par=0.0;
  else if(par>1.) par=1.0;
  
  Double_t tanTheta1 = 0.0;
  Double_t zMdc1=0.0;
  Double_t tmp;
  HGeomCompositeVolume* pComVol=fGeomPar->getRefVolume(0); //MDC1
  HGeomVolume* pGeomVol = 0;
  if(!pComVol) {
    printf("Error: GeomCompositeVolume for MDC1 is absent!\n");
  }
  else {
    pGeomVol=pComVol->getComponent(5);          // Layer 6
    if(!pGeomVol) {
      printf("Error: GeomCompositeVolume for Layer 6 is absent!\n");
      return kFALSE;
    }
    coordSys.setTransform(pGeomVol->getTransform());   //From layer to module
    coordSys.transFrom(pComVol->getTransform());       //From layer to sector
    tmp=(coordSys.getRotMatrix())(8);
    tmp=1./(tmp*tmp);
    tanTheta1=sqrt(tmp-1.);
    zMdc1=(coordSys.getTransVector()).getZ()*tmp;
  } 

  Double_t tanTheta2=0.0;
  Double_t zMdc2=0.0;
  pComVol=fGeomPar->getRefVolume(1); //MDC2
  if(!pComVol) {
    printf("Error: GeomCompositeVolume for MDC2 is absent!\n");
  }
  else {
    pGeomVol=pComVol->getComponent(0); // Layer 1
    if(!pGeomVol) {
      printf("Error: GeomCompositeVolume for Layer 1 is absent!\n");
      return kFALSE;
    }
    coordSys.setTransform(pGeomVol->getTransform());   //From layer to module
    coordSys.transFrom(pComVol->getTransform());       //From layer to sector
    tmp=(coordSys.getRotMatrix())(8);  tmp=1./(tmp*tmp);
    tanTheta2=sqrt(tmp-1.);
    zMdc2=(coordSys.getTransVector()).getZ()*tmp;
  }

  if(zMdc1==0.0 && zMdc2==0.0) {
    printf("Error in HMdc12CellProjec::calcMdc12Proj: No MDC volume found!\n");
    return kFALSE;
  }
  if(par==0.0) printf("\n===> Using MDC1, plane 6 as projection plane\n");
  else printf("\n===> Using MDC2, plane 1 as projection plane\n");

  Double_t tanTheta=tanTheta1*(1.-par)+tanTheta2*par;
  Double_t cosTheta=1/sqrt(tanTheta*tanTheta+1.);
  Double_t sinTheta=tanTheta*cosTheta;
  Double_t newRot[9];
  newRot[0]=1;
  newRot[1]=newRot[2]=newRot[3]=newRot[6]=0;
  newRot[4]=newRot[8]=cosTheta;                           // cos(Theta)
  newRot[5]=sinTheta;                                     // sin(Theta)
  newRot[7]=-sinTheta;                                    // -sin(Theta)
  HGeomRotation rotMatProj(newRot);
  Double_t transNew[3];
  transNew[0]=0;
  transNew[2]=((zMdc1*(1.-par)+zMdc2*par)*cosTheta*cosTheta)/10.; // mm -> cm
  transNew[1]=transNew[2]*tanTheta;
  
  coordSys.setRotMatrix(newRot);
  coordSys.setTransVector(transNew); 
    
  HMdc12CellProjecSec& p12CPrSec=(*f12CellProjec)[nSec];
  p12CPrSec.setSys(coordSys); 
//The plane for projection is (Ap*x+Bp*y+Cp*z+Dp=0):
  Ap=transNew[0]; 
  Bp=transNew[1];
  Cp=transNew[2];
  Dp=-(Ap*Ap+Bp*Bp+Cp*Cp);
  
 //---------------------------------    
//Modules:
 //Only MDC1 and MDC2!   
  for(Int_t nMod=0; nMod<2; nMod++) {
//Layers:
   for(Int_t nLayer=0; nLayer<6; nLayer++) {
    HMdc12CellProjecLayer& p12CPrLayer = p12CPrSec[nMod][nLayer];
    Int_t nCells=p12CPrLayer.getSize();
    if(nCells>0) {   
     Double_t sig=0.001;
//Cells:
     for(Int_t nCell=0; nCell<nCells; nCell++) {    
      HMdcSizesCellsCell& pSCCell=(*fSizesCells)[nSec][nMod][nLayer][nCell];
      HMdc12CellProjecCell& p12CPrCell=p12CPrLayer[nCell];      
      if(pSCCell.isCell()) {     
       p12CPrCell.setTRUE();
       HMdcTrap& cell=pSCCell.cell();
       HMdcTrapPlane& cellproj=p12CPrCell.cellProjec();      
 //Line from 2 points (x-x1)/(x1-x2)=(y-y1)/(y1-y2)=(z-z1)/(z1-z2); 
 //Only for sector coordinat system!!!
       calcProjPoint(cell,coordSys);
       Double_t dist[2][8]; 
//Lines 1,5 for minmum:
       Bool_t indb=kFALSE;
       if(fabs(pointsProj[0*8+0].getX()-pointsProj[0*8+3].getX()) > sig) 
                                                                  indb=kTRUE;        
       for(Int_t i=0; i<2; i++) {       
        for(Int_t j=0; j<2; j++) {
         dist[i][j*4]=pointsProj[i*8+j*4].getY();
	 if(indb) 
	    dist[i][j*4]=calcDist(pointsProj[i*8+j*4],pointsProj[i*8+j*4+3]);	 
	}
       }     
//Lines 1,5 for maximum:       
       Bool_t indu=kFALSE;     
       if(fabs(pointsProj[0*8+1].getX()-pointsProj[0*8+2].getX()) > sig) 
                                                                  indu=kTRUE; 
       for(Int_t i=0; i<2; i++) {
        for(Int_t j=0; j<2; j++) {
         dist[i][j*4+1]=pointsProj[i*8+j*4+1].getY();
	 if(indu) 
	  dist[i][j*4+1]=calcDist(pointsProj[i*8+j*4+1],pointsProj[i*8+j*4+2]);
	}
       }      
// Minimum y:       
       Int_t tb=0; Int_t pb=0;
       if(dist[1][0] < dist[tb][pb])  tb=1;
       if(dist[1][4] < dist[tb][pb]) {tb=1; pb=4;}
       if(dist[0][4] < dist[tb][pb]) {tb=0; pb=4;}      
//Maximum y:	
       Int_t tu=0; Int_t pu=1;
       if(dist[1][1] > dist[tu][pu])  tu=1;
       if(dist[1][5] > dist[tu][pu]) {tu=1; pu=5;}
       if(dist[0][5] > dist[tu][pu]) {tu=0; pu=5;}    
       for(Int_t i=0; i<2; i++) {
        dist[i][0]=fabs(calcDist(pointsProj[i*8+0],pointsProj[i*8+1])); //line 0-1;
	dist[i][4]=fabs(calcDist(pointsProj[i*8+4],pointsProj[i*8+5])); //line 4-5;
	dist[i][2]=fabs(calcDist(pointsProj[i*8+2],pointsProj[i*8+3])); //line 2-3;
	dist[i][6]=fabs(calcDist(pointsProj[i*8+6],pointsProj[i*8+7])); //line 2-3
       }       
// Minimum-minmum for lines 0-1;4-5;  2-3;6-7:
       Int_t tl=0; Int_t pl=0;
       if(dist[1][0] > dist[tl][pl])  tl=1;
       if(dist[1][4] > dist[tl][pl]) {tl=1; pl=4;}
       if(dist[0][4] > dist[tl][pl]) {tl=0; pl=4;}
       Int_t tr=0; Int_t pr=2;
       if(dist[1][2] > dist[tr][pr])  tr=1;
       if(dist[1][6] > dist[tr][pr]) {tr=1; pr=6;}
       if(dist[0][6] > dist[tr][pr]) {tr=0; pr=6;}      
//printf("tu=%i,pu=%i,tb=%i,pb=%i,tl=%i,pl=%i,tr=%i,pr=%i\n",tu,pu,tb,pb,tl,pl,tr,pr);       
       if(indb) {
        calcPoint(cellproj[0],pointsProj[tb*8+pb],pointsProj[tb*8+pb+3],
	                      pointsProj[tl*8+pl],pointsProj[tl*8+pl+1]);
        calcPoint(cellproj[3],pointsProj[tb*8+pb],pointsProj[tb*8+pb+3],
	                      pointsProj[tr*8+pr],pointsProj[tr*8+pr+1]);       
       }      
       if(indu) {
        calcPoint(cellproj[1],pointsProj[tu*8+pu],pointsProj[tu*8+pu+1],
	                      pointsProj[tl*8+pl],pointsProj[tl*8+pl+1]);
        calcPoint(cellproj[2],pointsProj[tu*8+pu],pointsProj[tu*8+pu+1],
	                      pointsProj[tr*8+pr],pointsProj[tr*8+pr+1]);
       }      
       if(!indb) {
        Double_t x=(pointsProj[tb*8+pb].getX()+pointsProj[tb*8+pb+3].getX())*0.5;
	Double_t y=(pointsProj[tb*8+pb].getY()+pointsProj[tb*8+pb+3].getY())*0.5;
        cellproj[0].set(x,y);
        cellproj[3].set(x,y); 
       }
       if(!indu) {
        Double_t x=(pointsProj[tb*8+pu].getX()+pointsProj[tb*8+pu+1].getX())*0.5;
	Double_t y=(pointsProj[tb*8+pu].getY()+pointsProj[tb*8+pu+1].getY())*0.5;
        cellproj[1].set(x,y);
        cellproj[2].set(x,y); 
       }      
      }
     }
    } 
   }
  }
 return kTRUE;
}

Double_t HMdcCalc12CellProjec::calcDist(HGeomVector& p1, HGeomVector& p2) {
 return p1.getY()-p1.getX()*(p1.getY()-p2.getY())/(p1.getX()-p2.getX());
}

void HMdcCalc12CellProjec::calcProjPoint(HMdcTrap& cell, HGeomTransform& coordSys) {
 Double_t x1,y1,z1,x2,y2,z2;
 Double_t xyz[3];
 for (Int_t i=0; i<8; i++) {
  x2=cell[i].getX(); 
  y2=cell[i].getY();
  z2=cell[i].getZ();
  for (Int_t j=0; j<2; j++) {
   x1=targetPs[j].getX(); 
   y1=targetPs[j].getY();
   z1=targetPs[j].getZ();
   xyz[0]=(x1/(x1-x2)*(Bp*(y1-y2)+Cp*(z1-z2))-Bp*y1-Cp*z1-Dp)/
          (Ap+(Bp*(y1-y2)+Cp*(z1-z2))/(x1-x2));
   xyz[1]=(xyz[0]-x1)*(y1-y2)/(x1-x2)+y1;
   xyz[2]=(-Ap*xyz[0]-Bp*xyz[1]-Dp)/Cp;
   pointsProj[j*8+i].setVector(xyz);
//   pointsProj[j*8+i]=coordSys.transTo(pointsProj[j*8+i]);
  }
 }
}

void HMdcCalc12CellProjec::calcPoint(HMdcPointPlane& proj,
 HGeomVector& p1l1, HGeomVector& p2l1, HGeomVector& p1l2, HGeomVector& p2l2) {
  Double_t al1=(p1l1.getY()-p2l1.getY())/(p1l1.getX()-p2l1.getX());
  Double_t bl1=p1l1.getY()-al1*p1l1.getX();
  Double_t al2=(p1l2.getY()-p2l2.getY())/(p1l2.getX()-p2l2.getX());
  Double_t bl2=p1l2.getY()-al2*p1l2.getX();
  Double_t x=(bl2-bl1)/(al1-al2);
  Double_t y=al2*x+bl2;
  proj.set(x,y);
}
