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
#include "hmdcgetcontainers.h"
#include <iostream.h>
//*-- Author : Vladimir Pechenov
//*-- Modified : 30/5/2000 by V. Pechenov
//*-- Modified : 20/5/2000 by V. Pechenov
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
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return;
  HMdcDetector* fMdcDet=parCont->getMdcDetector();
  HMdcGeomStruct* pMdc=parCont->getMdcGeomStruct();
  if( !fMdcDet || !pMdc || !fMdcDet->getModule(sec,mod)) return;
  
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

HMdc12CellProjecSec::HMdc12CellProjecSec(Int_t sec, Int_t maxNMod=2) {
  // constructor creates an array of pointers of type
  // HMdc12CellProjecMod
  sector=sec;
  array = new TObjArray(4);
  HMdcDetector* fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  for (Int_t mod = 0; mod < maxNMod; mod++) {
    if( !fmdc->getModule(sec,mod) ) continue;
    (*array)[mod] = new HMdc12CellProjecMod(sec,mod);
  }
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

HMdc12CellProjec::HMdc12CellProjec(Bool_t coilOff) {
  // constructor creates an array of pointers of type
  // HMdc12CellProjecSec
  array = new TObjArray(6);
  isContainer=kFALSE;
  
  HMdcCalc12CellProjec* calc=new HMdcCalc12CellProjec();
  HMdcDetector* fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  if( !fmdc ) return; 
  for (Int_t nSec = 0; nSec < 6; nSec++) {
    if( (!fmdc->getModule(nSec,0) && !fmdc->getModule(nSec,1)) && (coilOff ||
      (!fmdc->getModule(nSec,2) && !fmdc->getModule(nSec,3)) ) ) continue;
    (*array)[nSec] = new HMdc12CellProjecSec(nSec,(coilOff) ? 4:2);
    if( ! calc->calcMdc12Proj(nSec, this)) {
     delete calc;
     return;
    }
  }
  isContainer=kTRUE;
  delete calc;
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
    if(!&sec) continue;
    for(Int_t m=0;m<4;m++) {
      HMdc12CellProjecMod& mod=sec[m];
      if(!&mod) continue;
      Int_t nl=mod.getSize();
      for(Int_t l=0;l<nl;l++) {
        HMdc12CellProjecLayer& lay=mod[l];
        if(!&lay) continue;
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
  // Geting of pointers to par. cont.
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return;
  fMdcDet= parCont->getMdcDetector();
  fGeomPar=parCont->getMdcGeomPar();
  fSpecGeomPar=parCont->getSpecGeomPar();
  fSizesCells=HMdcSizesCells::getObject();
  return;
}

Bool_t HMdcCalc12CellProjec::calcMdc12Proj(Int_t nSec, 
                                 HMdc12CellProjec* f12CellProjec) {
  HGeomTransform coordSys;

  //Geting size of target
  Int_t nT=fSpecGeomPar->getNumTargets()-1;
  if( nT < 0 ) {
    Printf("Error: Number of targets = %i!\n",nT);
    return kFALSE;
  }
  targetPs[0]=(fSpecGeomPar->getTarget( 0)->getTransform()).getTransVector();
  targetPs[1]=(fSpecGeomPar->getTarget(nT)->getTransform()).getTransVector();
  targetPs[0].setZ(
      targetPs[0].getZ() + fSpecGeomPar->getTarget( 0)->getPoint(0)->getZ());
  targetPs[1].setZ(
      targetPs[1].getZ() + fSpecGeomPar->getTarget(nT)->getPoint(2)->getZ());  
  HGeomVolume* fVolSec=fSpecGeomPar->getSector(nSec);
  targetPs[0]=fVolSec->getTransform().transTo(targetPs[0]);
  targetPs[1]=fVolSec->getTransform().transTo(targetPs[1]);
  
  HMdc12CellProjecSec& p12CPrSec=(*f12CellProjec)[nSec];
  p12CPrSec.setTargetF(targetPs[0]);
  p12CPrSec.setTargetL(targetPs[1]);
  if( !calcProjPlain(nSec, &coordSys, &p12CPrSec) ) return kFALSE;

 //---------------------------------    
//Modules:
//For all MDC !!! (new)
//?!  HMdcDetector* fmdc=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");  
  for(Int_t nMod=0; nMod<4; nMod++) {
   if( !&(p12CPrSec[nMod]) ) continue;
//?!   if( !fmdc->getModule(nSec,nMod) ) continue; 
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
if(y<0. || y>10000.) Printf("s%i m%i l%i c%i p0,3  x===%f y===%f",nSec,nMod,nLayer,nCell,x,y);
        cellproj[0].set(x,y);
        cellproj[3].set(x,y); 
       }
       if(!indu) {
        Double_t x=(pointsProj[tb*8+pu].getX()+pointsProj[tb*8+pu+1].getX())*0.5;
	Double_t y=(pointsProj[tb*8+pu].getY()+pointsProj[tb*8+pu+1].getY())*0.5;
if(y<0. || y>10000.) Printf("s%i m%i l%i c%i p1,2  x===%f y===%f",nSec,nMod,nLayer,nCell,x,y);
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

Bool_t HMdcCalc12CellProjec::calcProjPlain(Int_t sec, HGeomTransform* coordSys,
                                           HMdc12CellProjecSec* p12CPrSec){
//------------------------------------------- 
//Sectors:
//                     HMdcDigitGeomParSec& sec = (*fDigitGeomPar)[nSec];   

//  Calc. of plane for projection:
//  It's the plane between MDC0 and MDC1
//  par=0.0 -> plane=layer 6, MDC1
//  par=1.0 -> plane=layer 1, MDC2

  Double_t par=-100.; 
//  Double_t par=1.0;   // <<<===================== use MDC1
  Int_t nmod=0;
  Int_t firstMod=-1;
  Int_t lastMod=0;
  HGeomCompositeVolume* pComVol[4];
  for(Int_t m=0; m<4; m++) {
    if( &((*p12CPrSec)[m]) ) {
      pComVol[m]=fGeomPar->getRefVolume(m);
      if( !(pComVol[m]) ) return kFALSE;
      nmod++;
      if(firstMod<0) firstMod=m;
      lastMod=m;
    }
  }
  if(nmod==0) return kFALSE;
  Int_t iPar=-1;
  if(nmod==1) iPar=firstMod;
  else if(firstMod==0) par=0.575;   // determ. of par !!!
  else iPar=firstMod;                 // for beem time nov2000 with mdc 3
  Double_t transNew[3];
  if(iPar<0) {  
    Double_t tanTheta1 = 0.0;
    Double_t tanTheta2=0.0;
    Double_t zMdc1=0.0;
    Double_t zMdc2=0.0;
    Double_t tmp; 
    HGeomVolume* pGeomVol;
    pGeomVol=pComVol[firstMod]->getComponent(5);          // Layer 6
    if(!pGeomVol) {
      printf("Error: GeomCompositeVolume for Layer 6 is absent!\n");
      return kFALSE;
    }
    coordSys->setTransform(pGeomVol->getTransform());         //From layer to module
    coordSys->transFrom(pComVol[firstMod]->getTransform());   //From layer to sector
    tmp=(coordSys->getRotMatrix())(8);
    tmp=1./(tmp*tmp);
    tanTheta1=sqrt(tmp-1.);
    zMdc1=(coordSys->getTransVector()).getZ()*tmp; 

    pGeomVol=pComVol[lastMod]->getComponent(0); // Layer 1
    if(!pGeomVol) {
      printf("Error: GeomCompositeVolume for Layer 1 is absent!\n");
      return kFALSE;
    }
    coordSys->setTransform(pGeomVol->getTransform());       //From layer to module
    coordSys->transFrom(pComVol[lastMod]->getTransform());  //From layer to sector
    tmp=(coordSys->getRotMatrix())(8);  tmp=1./(tmp*tmp);
    tanTheta2=sqrt(tmp-1.);
    zMdc2=(coordSys->getTransVector()).getZ()*tmp;

    if(zMdc1==0.0 && zMdc2==0.0) {
      printf("Error in HMdc12CellProjec::calcMdc12Proj: No MDC volume found!\n");
      return kFALSE;
    }
    printf(
    "\n===> Sec.%i: Using plane between MDC%i and MDC%i (p=%f) as projection plane\n",
        sec+1,firstMod+1,lastMod+1,par);

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
    transNew[0]=0;
    transNew[2]=(zMdc1*(1.-par)+zMdc2*par)*cosTheta*cosTheta; // mm
    transNew[1]=transNew[2]*tanTheta;

    coordSys->setRotMatrix(newRot);
    coordSys->setTransVector(transNew); 
  } 
  else {
    coordSys->setTransform(pComVol[iPar]->getTransform());
    printf("\n===> Sec.%i: Using middle plane of MDC%i as projection plane\n",
                              sec+1,iPar+1);
  }
  p12CPrSec->setSys(*coordSys); 
//The plane for projection is (Ap*x+Bp*y+Cp*z+Dp=0):
  const HGeomVector tr=coordSys->getTransVector();
  const HGeomRotation rt=coordSys->getRotMatrix();
  Ap=(rt(3)*rt(7)-rt(6)*rt(4));
  Bp=(rt(6)*rt(1)-rt(0)*rt(7));
  Cp=(rt(0)*rt(4)-rt(3)*rt(1));
  Dp=-(Ap*tr(0)+Bp*tr(1)+Cp*tr(2));
  return kTRUE;
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
