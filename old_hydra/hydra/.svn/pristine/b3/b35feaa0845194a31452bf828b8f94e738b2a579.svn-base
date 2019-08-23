#include "hmdc34clfinder.h"
#include "hades.h"
#include "hmatrixcategory.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include <iostream.h>
#include "hmdcgeomobj.h"
#include "hmdcgetcontainers.h"
#include "hmdccal1sim.h"
#include "TStopwatch.h"


//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 12/07/2000 by V.Pechenov

////////////////////////////////////////////////////////////////
// HMdc34ClFinder
//
// Track finder for outer segment MDC:
// Constructor:
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcProjPlot)
//ClassImp(HMdc34ClFinderCell)
ClassImp(HMdc34ClFinderLayer)
ClassImp(HMdc34ClFinderMod)
ClassImp(HMdc34ClFinderSec)
ClassImp(HMdc34ClFinder)

HMdcProjPlot::HMdcProjPlot(Int_t inBinX, Int_t inBinY) {
  nBinX=inBinX;  // adding 2 bins for nBinX[0]=nBinX[nBinX-1]=0 (always)
  nBinY=inBinY;  // ...
  size=nBinX*nBinY;
  size=(size/32 + ((size%32 > 0) ? 1:0))*32;
  xMinL=new Short_t [nBinY];
  xMaxL=new Short_t [nBinY];
  plot=new Short_t [size];
  plot1b=(UChar_t*)plot;
  plot4b=(Int_t*)plot;
  sizeBAr=size/8;
  sizeBAr4b=sizeBAr/4;
//  plotBArCl=new UChar_t [sizeBAr];       // Test in f.???
//  plotBAr4bCl=(Int_t *)plotBArCl;
//  memset(plotBArCl,0,sizeBAr);
  plotBArSc=new UChar_t [sizeBAr];
  plotBArSc4b=(Int_t *)plotBArSc;
  memset(plotBArSc,0,sizeBAr);
  rootPlot=0;
}

HMdcProjPlot::~HMdcProjPlot() {
  delete [] xMinL;
  delete [] xMaxL;
  delete [] plot;
  if(rootPlot) delete rootPlot;
}

void HMdcProjPlot::setEdges(Float_t iyMin, Float_t ixMinD, Float_t ixMaxD,
                            Float_t iyMax, Float_t ixMin, Float_t ixMax) {
  stY=(iyMax-iyMin)/(nBinY-2);
  stX=(ixMax-ixMin)/(nBinX-2);
  yMin=iyMin-stY;
  yMax=iyMax+stY;
  xMin=ixMin-stX;
  xMax=ixMax+stX;
  xMinD=ixMinD;
  xMaxD=ixMaxD;
  Float_t aL23=(xMinD-xMin)/(yMin-yMax);
  Float_t aL01=(xMaxD-xMax)/(yMin-yMax);
  for(Int_t ny=0; ny<nBinY; ny++) {
    Float_t yl=(ny+1)*stY+yMin;
    xMinL[ny]=Short_t((((yl-yMax)*aL23+xMin)-xMin)/stX);
    if(xMinL[ny] < 1) xMinL[ny]=1;
    xMaxL[ny]=Short_t((((yl-yMax)*aL01+xMax)-xMin)/stX);
    if(xMaxL[ny] > nBinX-2) xMaxL[ny]=nBinX-2;
  }
}

void HMdcProjPlot::print() {
  Printf("----- Project plane: -----");
  HMdcPrPlane::print();
  Printf(" Sizes: nBinX=%i, nBinY=%i, stepX=%g, stepY=%g",nBinX,nBinY,stX,stY);
  Printf(" Points (x:y) n. 0=(%.1f:%.1f)  1=(%.1f:%.1f)",xMaxD,yMin,xMax,yMax);
  Printf("                 2=(%.1f:%.1f)  3=(%.1f:%.1f)",xMin,yMax,xMinD,yMin);
}


TH2C* HMdcProjPlot::getPlot(Char_t* name, Char_t* title){
 if(!rootPlot) {
//  TH2C* rootPlot=new TH2C(name,title,nBinX,xLow,xUp,nBinY,yLow,yUp);
   rootPlot=new TH2C(name,title,nBinX,xMin,xMax,nBinY,yMin,yMax);
 }
 else {
   rootPlot->Reset();
   rootPlot->SetName(name);
   rootPlot->SetTitle(title);
 }
// rootPlot->SetMaximum(6.);
// rootPlot->SetMinimum(0.);
 rootPlot->Fill(0.,0.,0);
 for (Int_t nBin=0; nBin<size; nBin++) {
  if(plot[nBin] > 0) {
   Int_t ny=nBin/nBinX;
   Int_t nx=nBin%nBinX;
   rootPlot->Fill((Float_t)nx*stX+stX*0.5+xMin,
              (Float_t)ny*stY+stY*0.5+yMin,
               HMdcBArray::getNSet2B(plot1b+nBin*2));
  }
 }
 return rootPlot;
}

TH2C* HMdcProjPlot::getPlotV2(Char_t* name, Char_t* title){
 if(!rootPlotV2) {
//  TH2C* rootPlotV2=new TH2C(name,title,nBinX,xLow,xUp,nBinY,yLow,yUp);
   rootPlotV2=new TH2C(name,title,nBinX,xMin,xMax,nBinY,yMin,yMax);
 }
 else {
   rootPlotV2->Reset();
   rootPlotV2->SetName(name);
   rootPlotV2->SetTitle(title);
 }
// rootPlotV2->SetMaximum(6.);
// rootPlotV2->SetMinimum(0.);
 rootPlotV2->Fill(0.,0.,0);
 for (Int_t nBin=0; nBin<size; nBin++) {
  if(plot[nBin] > 0) {
   Int_t ny=nBin/nBinX;
   Int_t nx=nBin%nBinX;
   Int_t color=HMdcBArray::getNSet(plot1b[nBin*2]);
   Int_t col4=HMdcBArray::getNSet(plot1b[nBin*2+1]);
   if(col4 > 0) color+=col4+6;
   rootPlotV2->Fill((Float_t)nx*stX+stX*0.5+xMin,
              (Float_t)ny*stY+stY*0.5+yMin,color);
//               HMdcBArray::getNSet2B(plot1b+nBin*2));
  }
 }
 return rootPlotV2;
}

/* void HMdcProjPlot::clearArr() {      // Test in f.???
  for(Int_t nb4=0; nb4<sizeBAr4b; nb4++) {
    if(plotBAr4bCl[nb4]==0) continue;
    Int_t nb=nb4*4;
    Int_t inde=nb+4;
    for(; nb<inde; nb++) {
      if(plotBArCl[nb]==0) continue;
      Int_t* pl4b=plot4b+nb*4;
      *pl4b=0;
      *(++pl4b)=0;
      *(++pl4b)=0;
      *(++pl4b)=0;
    }
    plotBAr4bCl[nb4]=0;
  }
} */

//----------Cell ?-------------------------------
/*
HMdc34ClFinderCell::HMdc34ClFinderCell(Int_t yBinMinT, Int_t yBinMaxT) {
}

HMdc34ClFinderCell::~HMdc34ClFinderCell() {
}

void HMdc34ClFinderCell::addLine(Int_t nc1, Int_t nc2){
}
*/
//----------Layer-----------------------------

HMdc34ClFinderLayer::HMdc34ClFinderLayer(Int_t sec, Int_t mod, Int_t layer) {
  // Geting of pointers to par. cont.
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return;
  HMdcGeomStruct* pMdc=parCont->getMdcGeomStruct();
  if( !pMdc ) return;
  nCells=((*pMdc)[sec][mod])[layer];
  cellShUp=new Short_t [nCells];
  cellShDown=new Short_t [nCells];
//Cell ?  array = new TObjArray(nCells);
  yBin=0;
  nBinX=0;
}

HMdc34ClFinderLayer::~HMdc34ClFinderLayer() {
//Cell ?  if(array) array->Delete();
  if(yBin) delete yBin;
  yBin=0;
  nBinX=0;
}

void HMdc34ClFinderLayer::createArrayBins(Short_t nBins) {
  if(yBin) delete yBin;
  nBinX=nBins;
  yBin=new Short_t [nBins];
}

/* Cell ?
void HMdc34ClFinderLayer::initCell(Int_t nce, Int_t yBinMinT, Int_t yBinMaxT){
  if((*array)[nce]) {
    delete (*array)[nce];
    printf("Delete old and create new HMdc34ClFinderCell: n.cell=%i\n",nce+1);
  }
  (*array)[nce] = new HMdc34ClFinderCell(yBinMinT, yBinMaxT);
}
*/
//------------Module----------------------------
HMdc34ClFinderMod::HMdc34ClFinderMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type HMdc34ClFinderMod
  //
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++) {
    (*array)[layer] = new HMdc34ClFinderLayer(sec, mod, layer);
  }
}

HMdc34ClFinderMod::~HMdc34ClFinderMod() {
  // destructor
  if(array) array->Delete();
}

Int_t HMdc34ClFinderMod::getNCells() {
  Int_t nHits=0;
  for(Int_t lay=0; lay<6; lay++) nHits+=(*this)[lay].getNCells();
  return nHits;
}

void HMdc34ClFinderMod::clear() {
  for(Int_t lay=0; lay<6; lay++) if((*array)[lay]) (*this)[lay].clear();
}

//----------Sector------------------------------------
Int_t* HMdc34ClFinderSec::stackSeg2=0;
Int_t  HMdc34ClFinderSec::stackSizeSeg2;
Short_t HMdc34ClFinderSec::xMinClLines[100];
Short_t HMdc34ClFinderSec::xMaxClLines[100];
HKickPlane* HMdc34ClFinderSec::fkick=0;

HMdc34ClFinderSec::HMdc34ClFinderSec(Int_t sec, Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdc34ClFinderMod
  //
  nSec=sec;
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  lMods=fMdcDet->getModules()+nSec*4;
  mSeg[0]=((lMods[0]) ? 1:0) + ((lMods[1]) ? 2:0);
  mSeg[1]=((lMods[2]) ? 1:0) + ((lMods[3]) ? 2:0);
  array = new TObjArray(4);
  for (Int_t mod = 2; mod < 4; mod++) {
    if(lMods[mod]) (*array)[mod] = new HMdc34ClFinderMod(sec,mod);
  }
  pl34=0;
  if(mSeg[1]) {
    pl34=new HMdcProjPlot(inBinX,inBinY); // nBinX,inBinY will geted from ASCII file
    if(stackSeg2==0) {
      stackSizeSeg2=1000;
      stackSeg2=new Int_t [1000+100];
    }
    nearbyBins[0]=-1;
    nearbyBins[1]=+1;
    nearbyBins[2]=-pl34->nBinX;
    nearbyBins[3]=+pl34->nBinX;
    nearbyBins[4]=-1-pl34->nBinX;
    nearbyBins[5]=-1+pl34->nBinX;
    nearbyBins[6]=1-pl34->nBinX;
    nearbyBins[7]=1+pl34->nBinX;
  }
}

HMdc34ClFinderSec::~HMdc34ClFinderSec() {
  // destructor
  if(array) array->Delete();
  if(fkick) {
    delete fkick;
    fkick=0;
  }
  if(pl34) {
    delete pl34;
    pl34=0;
  }
  if(stackSeg2) {
    delete [] stackSeg2;
    stackSeg2=0;
  }
}

Bool_t HMdc34ClFinderSec::initLookUp() {
#if DEBUG_LEVEL>2
  Printf("<HMdc34ClFinderSec::initLookUp()> Sec.%i",nSec+1);
#endif
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if(!parCont) return kFALSE;
  HSpecGeomPar* fSpecGeomPar=parCont->getSpecGeomPar();

  if( !fSpecGeomPar ) return kFALSE;
  //Initialization of lookup table:
  Int_t nT=fSpecGeomPar->getNumTargets()-1;
  if(nT<0) return kFALSE;
  HGeomVector targ[2];  // First and last points of tar. in coor.sys. of sec.
  targ[0]=(fSpecGeomPar->getTarget( 0)->getTransform()).getTransVector();
  targ[1]=(fSpecGeomPar->getTarget(nT)->getTransform()).getTransVector();
  targ[0].setZ((targ[0](2))+fSpecGeomPar->getTarget( 0)->getPoint(0)->getZ());
  targ[1].setZ((targ[1](2))+fSpecGeomPar->getTarget(nT)->getPoint(2)->getZ());
  HGeomVolume* fVolSec=fSpecGeomPar->getSector(nSec);
  targ[0]=fVolSec->getTransform().transTo(targ[0]);
  targ[1]=fVolSec->getTransform().transTo(targ[1]);
  if(mSeg[1]) {
    if(!fkick) fkick=parCont->getKickPlane();
    if( !fkick || !calcProjPlaneSeg2() ) return kFALSE;
    calcSizePlotSeg2();
    if( !calcWiresProj() ) return kFALSE;
#if DEBUG_LEVEL>2
    pl34->print();
#endif
  }  
  return kTRUE;
}

Bool_t HMdc34ClFinderSec::initCat() {
  fClusCat = HMdcGetContainers::getCatMdcClus(kTRUE);
  if(!fClusCat) return kFALSE;
  isGeant = HMdcGetContainers::isGeant();
  locClus.set(3,nSec,0,0);
  return kTRUE;
  
/*  HCategory *fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if( !fCalCat ) return kFALSE;
  isGeant=(fCalCat->getClass()!=HMdcCal1Sim::Class()) ? kFALSE : kTRUE;
  fClusCat = gHades->getCurrentEvent()->getCategory(catMdcClus);
  if(!fClusCat) {
    HMdcGetContainers* parCont=HMdcGetContainers::getObject();
    HMdcGeomStruct* p=parCont->getMdcGeomStruct();
    TArrayI* ind = new TArrayI(4);
    p->getMaxIndices(ind);
    Int_t sizes[3];   // sec, seg, clust.
    sizes[0]=ind->At(0)+1;
    sizes[1]=ind->At(1)/2+1;
    sizes[2]=500;
    if(isGeant) fClusCat = new HMatrixCategory("HMdcClusSim",3,sizes,0.5);
    else fClusCat = new HMatrixCategory("HMdcClus",3,sizes,0.1);
    fClusCat->setPersistency(per);
    gHades->getCurrentEvent()->addCategory(catMdcClus,fClusCat,"Mdc");
    delete ind;
  }
  locClus.set(3,nSec,0,0);
  return kTRUE; */
}

Bool_t HMdc34ClFinderSec::calcProjPlaneSeg2(){
  // Calculation of project plane:
  //
  //  a plane between MDC3 and MDC4 - -1.<par<1.
  //  plane 1,2,3,4,5 or 6 MDC3 - par=-6,-5,-4,-3,-2 or -1
  //  plane 1,2,3,4,5 or 6 MDC2 - par=+1,+2,+3,+4,+5 or +6
  //  midplane MDC1 - proj.plane=-7
  //  midplane MDC2 - proj.plane=+7
  //  Automatical selection: par>=10. (for two MDC
  //  in sector = 0.15, for one MDC - midplane of this MDC)
//  Float_t par=-7.; //In a future will be geted from Par.Container
  Float_t par=10.; //In a future will be geted from Par.Container

  Int_t nL=(Int_t)par;
  //              (mdc3 don't exist)  (mdc4 don't exist)  (one mdc exist only)
  if(nL<-7||nL>7||(nL<0&&mSeg[1]==2)||(nL>0&&mSeg[1]==1)||(nL==0&&mSeg[1]!=3)) {
    nL=0;
    par=0.1;
  }
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  Char_t *text="as projection plane";
  HGeomTransform prPl;
  if( (nL==0 && mSeg[1]<3) || nL==-7 || nL==7 ) {
    // the project plane - the middle plane of MDC3 or MDC4
    Int_t mod=(nL==0) ? mSeg[1]+1 : (nL+7)/14+2;
    HGeomCompositeVolume *fVolMdc=parCont->getGeomCompositeVolume(mod);
    if(!fVolMdc) return kFALSE;
    prPl.setTransform(fVolMdc->getTransform());
    printf("\n===> Sec.%i Seg.2: Using middle plane of MDC%i %s\n",
                              nSec+1,mod+1,text);
  } else if( nL==0 && mSeg[1]==3 ) {
    // the project plane - between MDC3 & MDC4
    HGeomCompositeVolume *fVolM3=parCont->getGeomCompositeVolume(2);
    HGeomCompositeVolume *fVolM4=parCont->getGeomCompositeVolume(3);
    if(!fVolM3 || !fVolM4) return kFALSE;
    HGeomVolume* fVolLayer6=fVolM3->getComponent(5);
    HGeomVolume* fVolLayer1=fVolM4->getComponent(0);
    if(!fVolLayer6 || !fVolLayer1) return kFALSE;
    HGeomTransform trLayer6(fVolLayer6->getTransform());
    HGeomTransform trLayer1(fVolLayer1->getTransform());
    trLayer6.transFrom(fVolM3->getTransform());
    trLayer1.transFrom(fVolM4->getTransform());
    // MDC3 & MDC4 are ~parallel:
    Double_t distToL6=trLayer6.getTransVector().length();
    Double_t distToL1=trLayer1.getTransVector().length();
    Double_t newDist=distToL6+(distToL1-distToL6)*(1.+par)/2.;
    Double_t mult=1.;
    if(par<=0) {    // proj.plane will parallel to MDC3
      prPl.setTransform(trLayer6);
      mult=newDist/distToL6;
    } else {        // proj.plane will parallel to MDC4
      prPl.setTransform(trLayer1);
      mult=newDist/distToL1;
    }
    HGeomVector prTr(prPl.getTransVector());
    prTr*=mult;
    prPl.setTransVector(prTr);
    printf("\n===> Sec.%i Seg.2: Using plane between MDC 3 & 4 (p=%g) %s\n",
     nSec+1,par,text);
  } else {
    // the project plane - one of the layers
    Int_t nMod=(nL<0) ? 2:3;
    if(nL<0) nL+=7;
    HGeomCompositeVolume *fVolMdc=parCont->getGeomCompositeVolume(nMod);
    if(!fVolMdc) return kFALSE;
    HGeomVolume* fVolLayer=fVolMdc->getComponent(nL-1);
    if(!fVolLayer) return kFALSE;
    prPl.setTransform(fVolLayer->getTransform());
    prPl.transFrom(fVolMdc->getTransform());
    printf(
    "\n===> Sec.%i Seg.2: Using MDC%i, layer %i %s\n",nSec+1,nMod+1,nL,text);
  }
  pl34->setPrPlane(prPl);
  return kTRUE;
}

Bool_t HMdc34ClFinderSec::calcSizePlotSeg2(){
  // Calculation of plot's size:
  if(!mSeg[0]) return kFALSE;
  HGeomVector vect[2][4];
  HGeomVector pKick[4];
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  HGeomTransform trans;
  HGeomVector newP[4][2]; //[4]-num.lines, [2]-the firt&last points of the line
  HMdcPrPlane plane;
  for(Int_t mod=0; mod<2; mod++) {
    if(!(lMods[mod])) continue;
    HGeomCompositeVolume *fVolMdc=parCont->getGeomCompositeVolume(mod);
    if(!fVolMdc) return kFALSE;
    HGeomVolume* fVolLayer=fVolMdc->getComponent(5);
    if(!fVolLayer) return kFALSE;
    trans=fVolLayer->getTransform();
    trans.transFrom(fVolMdc->getTransform());
    plane.setPrPlane(trans);
    for(Int_t point=0; point<4; point++) {
      HGeomVector *fpoint=fVolLayer->getPoint(point);
      if(!fpoint) return kFALSE;
      Int_t indx=(mSeg[0]==3) ? mod : 0; // for one  MDC2 in seg. only
      vect[indx][point]=*fpoint;
      vect[indx][point].setZ(0.);
      vect[indx][point]=trans.transFrom(vect[indx][point]);
      if(mod==1 && mSeg[0]==3) {
        Int_t nTag=(point==0 || point==3) ? 0:1;
        vect[0][point]-=targ[nTag];
        plane.calcIntersection(targ[nTag],vect[0][point],vect[0][point]);
      }
    }
  }
  if(mSeg[0]==3) {
    for(Int_t mod=0; mod<2; mod++) {
      for(Int_t point=0; point<4; point++)
                       vect[mod][point]=trans.transTo(vect[mod][point]);
    }
    newP[0][0]=(vect[0][0](0)>vect[1][0](0)) ? vect[0][0] : vect[1][0];
    newP[0][1]=(vect[0][1](0)>vect[1][1](0)) ? vect[0][1] : vect[1][1];
    newP[1][0]=(vect[0][1](1)>vect[1][1](1)) ? vect[0][1] : vect[1][1];
    newP[1][1]=(vect[0][2](1)>vect[1][2](1)) ? vect[0][2] : vect[1][2];
    newP[2][0]=(vect[0][2](0)<vect[1][2](0)) ? vect[0][2] : vect[1][2];
    newP[2][1]=(vect[0][3](0)<vect[1][3](0)) ? vect[0][3] : vect[1][3];
    newP[3][0]=(vect[0][3](1)<vect[1][3](1)) ? vect[0][3] : vect[1][3];
    newP[3][1]=(vect[0][0](1)<vect[1][0](1)) ? vect[0][0] : vect[1][0];
    calcCrossLines(newP[0][0],newP[0][1],newP[3][0],newP[3][1],vect[0][0]);
    calcCrossLines(newP[0][0],newP[0][1],newP[1][0],newP[1][1],vect[0][1]);
    calcCrossLines(newP[1][0],newP[1][1],newP[2][0],newP[2][1],vect[0][2]);
    calcCrossLines(newP[2][0],newP[2][1],newP[3][0],newP[3][1],vect[0][3]);
    for(Int_t point=0; point<4; point++)
                      vect[0][point]=trans.transFrom(vect[0][point]);
  }
  HGeomVector dir;
  for(Int_t point=0; point<4; point++) {
    if(point==0 || point==3) dir=vect[0][point]-targ[0];
    else dir=vect[0][point]-targ[1]; //???
    if( !(fkick->calcIntersection(vect[0][point],dir,pKick[point]))) {
      Error("calcSizePlotSeg2",
        "Sec.%i: The point %i on the kick plane can't be calculated.",
        nSec+1,point);
      return kFALSE;
    }
  }
  // pKick[0-3] pointers on the kick-plane
  Float_t yMin=1.e+10;
  Float_t yMax=-1.e+10;
  for(Int_t mod=2; mod<4; mod++) {
    if(!(lMods[mod])) continue;
    HGeomCompositeVolume *fVolMdc=parCont->getGeomCompositeVolume(mod);
    Int_t indx=(mSeg[1]==3) ? mod-2 : 0; // for one  MDC2 in seg. only
    for(Int_t point=0; point<4; point++) {
      vect[indx][point]=*(fVolMdc->getPoint(point));
      vect[indx][point].setZ(0.);  //midplane of MDC
      vect[indx][point]=fVolMdc->getTransform().transFrom(vect[indx][point]);
      vect[indx][point]-=pKick[point];
      pl34->calcIntersection(pKick[point],vect[indx][point],vect[indx][point]);
      if(vect[indx][point](1)<yMin) yMin=vect[indx][point](1);
      if(vect[indx][point](1)>yMax) yMax=vect[indx][point](1);
    }
  }
  Float_t xMaxD=xLine(vect[0][0],vect[0][1],yMin);
  Float_t xMinD=xLine(vect[0][2],vect[0][3],yMin);
  Float_t xMax=xLine(vect[0][0],vect[0][1],yMax);
  Float_t xMin=xLine(vect[0][2],vect[0][3],yMax);
  if(mSeg[1]==3) {
    Float_t xnew=xLine(vect[1][0],vect[1][1],yMin);
    if(xnew>xMaxD) xMaxD=xnew;
    xnew=xLine(vect[1][2],vect[1][3],yMin);
    if(xnew<xMinD) xMinD=xnew;
    xnew=xLine(vect[1][0],vect[1][1],yMax);
    if(xnew>xMax) xMax=xnew;
    xnew=xLine(vect[1][2],vect[1][3],yMax);
    if(xnew<xMin) xMin=xnew;
  }
  pl34->setEdges(yMin, xMinD, xMaxD, yMax, xMin, xMax);
  return kTRUE;
}

void HMdc34ClFinderSec::calcCrossLines(HGeomVector& p1l1, HGeomVector& p2l1,
            HGeomVector& p1l2, HGeomVector& p2l2, HGeomVector& cross) {
  // Calculeting a cross of 2 lines (p1l1-p2l1, p1l2-p2l2) on the one (!)
  // plane. Z seted to 0.
  Double_t a1=(p1l1(1)-p2l1(1))/(p1l1(0)-p2l1(0));
  Double_t b1=p1l1(1)-a1*p1l1(0);
  Double_t a2=(p1l2(1)-p2l2(1))/(p1l2(0)-p2l2(0));
  Double_t b2=p1l2(1)-a2*p1l2(0);
  Double_t x=(b2-b1)/(a1-a2);
  Double_t y=a2*x+b2;
  cross.setXYZ(x,y,0.);
}

Float_t HMdc34ClFinderSec::xLine(HGeomVector& p1,HGeomVector& p2,Float_t yi) {
  return (yi-p2(1))/(p1(1)-p2(1))*(p1(0)-p2(0))+p2(0);
}

Bool_t HMdc34ClFinderSec::calcWiresProj() {
  HGeomVector wire[2];
  HGeomVector rib[4];
  HGeomVector point[2];
  HGeomTransform secsys;
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  Int_t mod=(mSeg[0]>1) ? 1:0;
  HGeomCompositeVolume *fVolMdc=parCont->getGeomCompositeVolume(mod);
  if(!fVolMdc) return kFALSE;
  HGeomVector tag=targ[0]+targ[1];
  tag/=2.;
  HGeomVector dir=fVolMdc->getTransform().getTransVector() - tag;
  HGeomVector pKick;
  if(!(fkick->calcIntersection(tag,dir,pKick))) {
    Error("calcWiresProj",
      "Sec.%i: The mid.point on the kick plane can't be calculated.",nSec+1);
    return kFALSE;
  }
  for(Int_t mod=2; mod<4; mod++) {
    if(!lMods[mod]) continue;
    fVolMdc=parCont->getGeomCompositeVolume(mod);
    if(!fVolMdc) return kFALSE;
    HMdc34ClFinderMod& fMod=(*this)[mod];
    for(Int_t lay=0; lay<6; lay++) {
      HGeomVolume* fVolLayer=fVolMdc->getComponent(lay);
      if(!fVolLayer) {
        Error("calcWiresProj",
            "The HGeomVolume (S%i|M%i|L%i) doesn't exist!\n",
            nSec+1,mod+1,lay+1);
        return kFALSE;
      }
      secsys.setTransform(fVolLayer->getTransform());
      secsys.transFrom(fVolMdc->getTransform());      // to coord.sys.of sector
      HMdcLayerGeomParLay* fLay=parCont->getMdcLayerGeomParLay(nSec,mod,lay);
      if(!fLay) {
       Error("calcWiresProj",
          "The HMdcLayerGeomParLay cont. (S%i|M%i|L%i) doesn't exist!\n",
           nSec+1,mod+1,lay+1);
        return kFALSE;
      }
      Double_t catDist=fLay->getCatDist()*0.5;      // mm!
      Double_t pitch=fLay->getPitch();      // mm!
      Double_t wOr=fLay->getWireOrient()/180.*acos(-1.);
      Double_t cosWiOr=cos(fLay->getWireOrient()/180.*acos(-1.));
      Double_t wOfSet=(fLay->getCentralWireNr()-1.)*pitch;
      Double_t aWire=tan(wOr);   // y=a*x+ln[1]

      // calc. of the wire's projection
      Double_t bWire=(100.*pitch-wOfSet)/cosWiOr;  // 100.-cell n.100
      wire[0].setXYZ(500.,aWire*500.+bWire,0.);
      wire[1].setXYZ(-500.,-aWire*500.+bWire,0.);
      for(Int_t np=0; np<2; np++) {
        wire[np]=secsys.transFrom(wire[np]);
        wire[np]-=pKick;
        pl34->calcIntersection(pKick,wire[np],point[np]);
      }
#if DEBUG_LEVEL>2
      Printf("M%i L%i C%i Size of wire on the proj.plane (poin1&point2):",
        mod+1,lay+1,100+1);
      point[0].print();
      point[1].print();
#endif
      Float_t al=(point[0](1)-point[1](1))/(point[0](0)-point[1](0)); // (y1-y2)/(x1-x2)
      Float_t x=(pl34->nBinX-1) * pl34->stX + pl34->xMin;
      Float_t yLeft=al*( x - point[1](0))+point[1](1); // (x-x2)*a+y2
      Float_t yRight=al*(pl34->xMin - point[1](0))+point[1](1);
      Short_t nYLeft =Short_t((yLeft  - pl34->yMin) / pl34->stY);
      Short_t nYRight=Short_t((yRight - pl34->yMin) / pl34->stY);
      Short_t yShift=TMath::Min(nYLeft,nYRight);
      if(TMath::Abs(nYLeft-nYRight)>=pl34->nBinY) {
        Error("calcWiresProj",
        "S%i M%i L%i The region of Y bins of proj. wire >= nBinY(%i).",
        nSec+1,mod+1,lay+1,pl34->nBinY);
        return kFALSE;
      }
      HMdc34ClFinderLayer& fLayer=fMod[lay];
      fLayer.createArrayBins(pl34->nBinX);
      for(Int_t nx=0; nx<pl34->nBinX; nx++) {
        Float_t x=nx * pl34->stX + pl34->xMin;
        Float_t y=al*(x-point[1](0))+point[1](1);
        Short_t nY=Short_t((y - pl34->yMin) / pl34->stY);
        fLayer.yBin[nx]=nY-yShift;
      }

      // calc. of the position of the  cells' ribs:
      // Z(x)=A1*x+Y0[nCell]; Z(x)=A2*x+Z0[nCell]; Y0&Z0 - at x=0;
      // Y0[nCell]=Ystep*nCell + Yshift; Z0[nCell]=Zstep*nCell + Zshift;
      //--- cells 0 & 100 ---
      for(Int_t nCell=0; nCell<101; nCell+=100) {
        bWire=(nCell*pitch-wOfSet-pitch/2.)/cosWiOr;
        // rib[0]-rib[1] - the down and left rib of cell
        // rib[2]-rib[3] - the down and right rib of cell
        for(Int_t n=0; n<4; n++) {
          Double_t x=((n&1) == 0) ? 500.: -500.;
          Double_t z=(n < 2) ? -catDist : catDist;
          rib[n].setXYZ( x,  aWire*x+bWire, z);
          rib[n]=secsys.transFrom(rib[n]);
        }
        Double_t dX=rib[0](0)-rib[1](0);
        rib[1].setXYZ(0.,(rib[0](0)*rib[1](1)-rib[1](0)*rib[0](1))/dX,
                         (rib[0](0)*rib[1](2)-rib[1](0)*rib[0](2))/dX);
        dX=rib[2](0)-rib[3](0);
        rib[3].setXYZ(0.,(rib[2](0)*rib[3](1)-rib[3](0)*rib[2](1))/dX,
                         (rib[2](0)*rib[3](2)-rib[3](0)*rib[2](2))/dX);
        if(nCell==0) {
          fLayer.y0[0]=rib[1](1);
          fLayer.z0[0]=rib[1](2);
          fLayer.y0[1]=rib[3](1);
          fLayer.z0[1]=rib[3](2);
          fLayer.tgY=(rib[0](1)-rib[1](1))/(rib[0](0)-rib[1](0));
          fLayer.tgZ=(rib[0](2)-rib[1](2))/(rib[0](0)-rib[1](0));
        } else {
          fLayer.yStep=(rib[1](1)-fLayer.y0[0])/100.;
          fLayer.zStep=(rib[1](2)-fLayer.z0[0])/100.;
          //Calc. the point(index) number in HMdc34ClFinderSec::segOnKick[4]:          
          if(rib[1](2)+pl34->B()*rib[1](1)-pl34->D()<0) {
            fLayer.nPSegOnKick[0][0]=(fLayer.tgY>0) ? 2 : 3;
            fLayer.nPSegOnKick[1][0]=(fLayer.tgY>0) ? 1 : 0;
          } else {
            fLayer.nPSegOnKick[0][0]=(fLayer.tgY>0) ? 1 : 0;
            fLayer.nPSegOnKick[1][0]=(fLayer.tgY>0) ? 2 : 3;
          }
          if(rib[3](2)+pl34->B()*rib[3](1)-pl34->D()<0) {
            fLayer.nPSegOnKick[0][1]=(fLayer.tgY>0) ? 2 : 3;
            fLayer.nPSegOnKick[1][1]=(fLayer.tgY>0) ? 1 : 0;
          } else {
            fLayer.nPSegOnKick[0][1]=(fLayer.tgY>0) ? 1 : 0;
            fLayer.nPSegOnKick[1][1]=(fLayer.tgY>0) ? 2 : 3;
          }
        }
      }
    }
  }
  return kTRUE;
}

//May be numhits[i]=0 ... in another sub. ?????????????????
void HMdc34ClFinderSec::clear() {
  counter=0;
//counterDel=0;
  for(Int_t mod=2; mod<4; mod++) if((*array)[mod]) (*this)[mod].clear();
}

Int_t HMdc34ClFinderSec::findClustersSeg2(HMdcSeg* fSeg, Int_t *mBin){
  Float_t z0,errZ0, r0,errR0, theta,errTh, phi,errPh;
  fSeg->getZR(z0,errZ0,r0,errR0);
  fSeg->getThetaPhi(theta,errTh,phi,errPh);
  minBin=mBin;
  // Errors ????????????????????????????????????????????????
  Double_t cosPhiS=cos(phi+90.);
  Double_t sinPhiS=sin(phi+90.);
  Double_t x0 = r0*cosPhiS;
  Double_t y0 = r0*sinPhiS;
  // let length=1.
  Double_t cosPhi=cos(phi);
  Double_t sinPhi=sin(phi);
  Double_t cosTh=cos(theta);
  Double_t sinTh=sin(theta);
  Double_t dirX=cosPhi*sinTh;
  Double_t dirY=sinPhi*sinTh;
  Double_t dirZ=cosTh;

  Double_t errX0=sqrt(TMath::Power(cosPhiS*errR0,2)+TMath::Power(y0*errPh,2));
  Double_t errY0=sqrt(TMath::Power(sinPhiS*errR0,2)+TMath::Power(x0*errPh,2));
//errY0=errY0/10.; //!!!!!!!!!!!!!!!!!!!!!!!! ??????????
  Double_t errDirX=sqrt(TMath::Power(sinPhi*sinTh*errPh,2)+
                        TMath::Power(cosPhi*cosTh*errTh,2));
  Double_t errDirY=sqrt(TMath::Power(cosPhi*sinTh*errPh,2)+
                        TMath::Power(sinPhi*cosTh*errTh,2));
//errDirY=errDirY/10.; //!!!!!!!!!!!!!!!!!!!!!!!! ??????????
  Double_t errDirZ=TMath::Abs(sinTh*errTh);
  // segOnkick[4]:
  //    [0]---[1]  ^ y
  //     |     |   |
  //     |     |   |
  //    [2]---[3]  |
  // X<-------------
  seg1[0].setXYZ(x0+errX0,y0+errY0,z0-errZ0);
  dirSeg1[0].setXYZ(dirX+errDirX,dirY+errDirY,dirZ-errDirZ);
  seg1[1].setXYZ(x0-errX0,y0+errY0,z0-errZ0);
  dirSeg1[1].setXYZ(dirX-errDirX,dirY+errDirY,dirZ-errDirZ);
  seg1[2].setXYZ(x0+errX0,y0-errY0,z0-errZ0);
  dirSeg1[2].setXYZ(dirX+errDirX,dirY-errDirY,dirZ+errDirZ);
  seg1[3].setXYZ(x0-errX0,y0-errY0,z0-errZ0);
  dirSeg1[3].setXYZ(dirX-errDirX,dirY-errDirY,dirZ+errDirZ);
  seg1[4].setXYZ(x0,y0,z0);               // hit
  dirSeg1[4].setXYZ(dirX,dirY,dirZ);      // hit
  for(Int_t np=0; np<5; np++) {
    if( !(fkick->calcIntersection(seg1[np],dirSeg1[np],segOnKick[np]))) {
      Error("findClustersSeg2",
        "Sec.%i: The point %i of HMDCSeg hit on the kick plane can't be calculated.",
        nSec+1,np);
      return 0;
    }
    if(np==4) {
      dirSeg1[4].setXYZ(
          (TMath::Abs(segOnKick[0](0)-segOnKick[1](0))+
          TMath::Abs(segOnKick[2](0)-segOnKick[3](0)))*0.25,
          (TMath::Abs(segOnKick[0](1)-segOnKick[2](1))+
          TMath::Abs(segOnKick[1](1)-segOnKick[3](1)))*0.25,
          (TMath::Abs(segOnKick[0](2)-segOnKick[2](2))+
          TMath::Abs(segOnKick[1](2)-segOnKick[3](2)))*0.25);
      break;
    }
    if(segOnKick[np](0)<pl34->xMin || segOnKick[np](0)>pl34->xMax) {
      Warning("findClustersSeg2","S%i: The X=%g of the point %i out of the region (%g - %g).",
         nSec+1, segOnKick[np](0), np, pl34->xMin, pl34->xMax);
      return 0;
    }   
    Double_t tmp=pl34->D() - pl34->A()*segOnKick[np](0); // D-A*x
    al[np]=tmp-segOnKick[np](2);                       // D-A*x-z
    bl[np]=tmp*segOnKick[np](1);                       // (D-A*x)*y
    cl[np]=segOnKick[np](2)+pl34->B()*segOnKick[np](1); // z+B*y
    nbX[np]=Int_t((segOnKick[np](0)-pl34->xMin)/pl34->stX);
  }
#if DEBUG_LEVEL>2
  Printf("---Sec.%i---- The region of HMdcSeg-hit on the kick plane:",nSec+1);
  for(Int_t np=0; np<4; np++) segOnKick[np].print();
  Printf("x-x(0,1)=%g x-x(2,3)=%g y-y(0,2)=%g y-y(1,3)=%g",
        segOnKick[0](0)-segOnKick[1](0),segOnKick[2](0)-segOnKick[3](0),
        segOnKick[0](1)-segOnKick[2](1),segOnKick[1](1)-segOnKick[3](1));
#endif
  makePlotSeg2();
  Int_t nClust=scanPlotSeg2();

  return nClust;
}

void HMdc34ClFinderSec::makePlotSeg2() {
  // cleanig:
 // pl34->clearArr();      // Test in f.???
  pl34->clearArr();
  Int_t shUp,shDown;
  Int_t shUpN=0;
  Int_t shDownN=0;
  Short_t add;
  Int_t mBm3,mBm4;
  mBm3=minBin[2];
  mBm4=minBin[3];
  Int_t minBinS=mBm3+mBm4;
  Int_t nLayers=0;
  for(Int_t mod=2; mod<4; mod++) {
    if(!lMods[mod]) continue;
    HMdc34ClFinderMod& fMod=(*this)[mod];
    add=1<<(mod-2)*8;  // !!!
    for(Int_t lay=0; lay<6; lay++) {

      nLayers++;
      HMdc34ClFinderLayer& fLay=fMod[lay];
      Int_t cell=fLay.getFirstCell();
      shUp=TMath::Max(calcYbin(fLay,0,0,cell+1),calcYbin(fLay,0,1,cell+1));
      shDown=TMath::Min(calcYbin(fLay,1,0,cell),calcYbin(fLay,1,1,cell));
      fLay.cellShUp[cell]=shUp;
      fLay.cellShDown[cell]=shDown;
      Bool_t test=kTRUE;
      while(test) {
        cell=fLay.next(cell);
        if(cell>=0) {
          shDownN=TMath::Min(calcYbin(fLay,1,0,cell),calcYbin(fLay,1,1,cell));
          shUpN=TMath::Max(calcYbin(fLay,0,0,cell+1),calcYbin(fLay,0,1,cell+1));
          fLay.cellShUp[cell]=shUpN;
          fLay.cellShDown[cell]=shDownN;
        } else {
          test=kFALSE;
          shDownN=shUp+1;
        }
        if( (shDownN-shUp)>1 || cell<0 ) {
// Mozhno optimizirovat'!
          for(Int_t nx=0; nx<pl34->nBinX; nx++) {
            Int_t nyU=TMath::Min(shUp+fLay.yBin[nx],pl34->nBinY-2); //bins nBinY-1 = 0 !
            Int_t ny=TMath::Max(shDown+fLay.yBin[nx],1);  //bins ny=0 eq. 0 !
            for(; ny<=nyU; ny++) {
              if(nx<pl34->xMinL[ny] || nx>pl34->xMaxL[ny]) continue;
              //Fill !!!
              Int_t binNum=ny*pl34->nBinX+nx;
              pl34->plot[binNum] |= add;
//            HMdcBArray::set(pl34->plotBArCl,binNum);      // Test in f.???
              if(nLayers>=minBinS) {
                Int_t b1=binNum*2;
//Printf("nLayers=%i minBins=%i mBm3=%i mBm4=%i 3=%i 4=%i",
//nLayers,minBinS,mBm3,mBm4,HMdcBArray::getNSet(pl34->plot1b[b1]),
//HMdcBArray::getNSet(pl34->plot1b[b1+1]));
                if(HMdcBArray::getNSet(pl34->plot1b[b1+1])>=mBm4 &&
                   HMdcBArray::getNSet(pl34->plot1b[b1])>=mBm3) {
                   HMdcBArray::set(pl34->plotBArSc,binNum);
                 }
              }
            }
          }
          shDown=shDownN;
        }
        shUp=shUpN;
      }
      add<<=1;
    }
  }
}

Int_t HMdc34ClFinderSec::calcYbin(HMdc34ClFinderLayer& fLay,
                                  Int_t upDo, Int_t leRi, Int_t cell) {
  //upDo=0 - up rib, =1 down rib
  Int_t np=fLay.nPSegOnKick[upDo][leRi];
  Double_t xp=segOnKick[np](0);
  Double_t yp=fLay.tgY*xp+cell*fLay.yStep+fLay.y0[leRi];
  Double_t zp=fLay.tgZ*xp+cell*fLay.zStep+fLay.z0[leRi];
  Double_t yPrPl=(al[np]*yp-bl[np]+segOnKick[np](1)*zp)/
                 (zp+pl34->B()*yp-cl[np]);
  return Int_t( (yPrPl-pl34->yMin)/pl34->stY )-fLay.yBin[nbX[np]];
}

Int_t HMdc34ClFinderSec::scanPlotSeg2() {
  Int_t nClusters=0;
  Int_t loc2FirstCl=counter;
  locClus[1]=1;
  for(Int_t n4=0; n4<pl34->sizeBAr4b; n4++) {
    if(pl34->plotBArSc4b[n4]==0) continue;
    UChar_t *b1=pl34->plotBArSc+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t n=-1;
    while((n=HMdcBArray::nextAndUnset(b1,b2,n))>=0) {
if(counter >= 460) {
  Printf("Sec.%i !! counter >= 460",nSec);
  memset(pl34->plotBArSc,0,pl34->sizeBAr);
  continue;
}
      locClus[2]=counter;
      HMdcClus* clus = (HMdcClus*)fClusCat->getSlot(locClus);
      if(!clus) {
        Warning("calcClusterSeg2","S.%i No slot HMdcClus available",nSec+1);
        continue;  // continue(not break) -> HMdcBArray must be cleaned !
      }
      if(isGeant) clus = (HMdcClus *) (new(clus) HMdcClusSim);
      else  clus = new(clus) HMdcClus;
      calcClusterSeg2(nBin4+n,clus);
      nClusters++;
//Printf("Sec%i counter=%i deleted=%i",nSec,counter,counterDel);
      counter++;
    }
  }
  if(nClusters>1) {
    for(Int_t loc2c1=loc2FirstCl; loc2c1<counter-1; loc2c1++) {
      locClus[2]=loc2c1;
      HMdcClus* clus1=(HMdcClus*)fClusCat->getObject(locClus);
      if(!clus1->getStatus()) continue;
      for(Int_t loc2c2=loc2c1+1; loc2c2<counter; loc2c2++) {
        locClus[2]=loc2c2;
        HMdcClus* clus2=(HMdcClus*)fClusCat->getObject(locClus);
        Float_t dY=clus2->getY() - clus1->getY();
        if(dY>100.) break;
        if(fabs(dY) > 30.) continue;  // 30. mm !??
        if(fabs(clus1->getX() - clus2->getX()) > 100.) continue;  //100. mm !??
        if(!clus2->getStatus()) continue;
        if(minBin[2]>0 && clus1->compare((HMdcList12GroupCells*)clus2,0,5)<4) 
            continue;
        if(minBin[3]>0 && clus1->compare((HMdcList12GroupCells*)clus2,6,11)<4) 
            continue;
        clus1->associate(clus2);
        nClusters--;
//counterDel++;
        loc2c1=loc2FirstCl;
        if(loc2c2 == counter-1) counter--;
        break;
      }
    }
  }
  
  return nClusters;
}

void HMdc34ClFinderSec::calcClusterSeg2(Int_t nBin, HMdcClus *clus) {
  Int_t stackT,stackB;
  stackT=stackB=0;
  Int_t nx,ny;
  Float_t x,y;
  Float_t averageX=0.;
  Float_t averageY=0.;
  Float_t rmsX=0.;
  Float_t rmsY=0.;
  Float_t binsInCl=0.;
  Int_t minBinS=minBin[2]+minBin[3]-1;
  Int_t nLinesInCl=1;
  Int_t nDownLine=nBin/pl34->nBinX;
  xMinClLines[0]=xMaxClLines[0]=nBin%pl34->nBinX;
  stackSeg2[stackB++]=nBin;
  Int_t xMinCl=pl34->nBinX;
  Int_t xMaxCl=0;
  while(stackT != stackB) {
    nBin=stackSeg2[stackT++];
    nx=nBin%pl34->nBinX;
    ny=nBin/pl34->nBinX;
    Float_t wt=(Float_t)(HMdcBArray::getNSet2B(pl34->plot1b+nBin*2)-minBinS);
    binsInCl+=wt;
    x=(nx+0.5)*pl34->stX+pl34->xMin;
    y=(ny+0.5)*pl34->stY+pl34->yMin;
    //region of cluster ???
    averageX+=x*wt;
    averageY+=y*wt;
    rmsX+=x*x*wt;
    rmsY+=y*y*wt;
    // At the scaning of the cluster y step(in bins) can't be > 1
    Int_t nyL=ny-nDownLine;
    if(nyL>=nLinesInCl) {
      if(nyL<100) {
        xMinClLines[nyL]=xMaxClLines[nyL]=nx;
        xMinCl=xMaxCl=nx;
        nLinesInCl++;
      } else Warning("calcClusterSeg2","Sec.%i The cluster size is very big!",
             nSec+1);
    }
    else {
      if(nx<xMinClLines[nyL]) xMinClLines[nyL]=nx;
      else if(nx>xMaxClLines[nyL]) xMaxClLines[nyL]=nx;
      if(nx<xMinCl) xMinCl=nx;
      else if(nx>xMaxCl) xMaxCl=nx;
    }
    for(Int_t ib=0; ib<8; ib++) {
      Int_t nBinTs=nBin+nearbyBins[ib];
      if( HMdcBArray::testAndUnset(pl34->plotBArSc,nBinTs) ) {
         stackSeg2[stackB++]=nBinTs;
      }
    }
  }
  if(stackB > stackSizeSeg2) Error("calcClusterSeg2",
    "Stack overflowed ( %i > %i )!!!",stackB,stackSizeSeg2);
  // Filling of cluster:
  clus->setAddress(locClus);
  averageX/=binsInCl;
  averageY/=binsInCl;
  rmsX=sqrt(rmsX/binsInCl-averageX*averageX + pl34->stX*pl34->stX*0.25);
  rmsY=sqrt(rmsY/binsInCl-averageY*averageY + pl34->stY*pl34->stY*0.25);
  clus->setXY(averageX,rmsX,averageY,rmsY);
  clus->setMinCl(minBin);
  clus->setPrPlane(*((HMdcPrPlane*)pl34));
  clus->setNBins(stackB);
  clus->setSumWt(binsInCl);
  clus->setTarg(segOnKick[4]);
  clus->setErrTarg(dirSeg1[4]);
  Int_t nUpLine=nDownLine+nLinesInCl-1;
  for(Int_t mod=2; mod<4; mod++) {
    if(!lMods[mod]) continue;
    HMdc34ClFinderMod& fMod=(*this)[mod];
    Int_t startLay=(mod-2)*6;
    for(Int_t lay=0; lay<6; lay++) {
      Int_t layer=startLay+lay;
      HMdc34ClFinderLayer& fLay=fMod[lay];
      for(Int_t cell=fLay.getFirstCell(); cell>=0; cell=fLay.next(cell)) {
        if( fLay.yBin[xMinCl]+fLay.cellShUp[cell]<nDownLine &&
            fLay.yBin[xMaxCl]+fLay.cellShUp[cell]<nDownLine ) continue;
        if( fLay.yBin[xMinCl]+fLay.cellShDown[cell]>nUpLine &&
            fLay.yBin[xMaxCl]+fLay.cellShDown[cell]>nUpLine ) break;
        for(Int_t line=0; line<nLinesInCl; line++) {
          Int_t nYbin=line+nDownLine;
          Int_t tYmin=fLay.yBin[xMinClLines[line]]-nYbin;
          Int_t tYmax=fLay.yBin[xMaxClLines[line]]-nYbin;
          Int_t dY1b=tYmin+fLay.cellShUp[cell];
          Int_t dY2b=tYmax+fLay.cellShUp[cell];
    //if (dY1b==0 || dY2b==0) - optimizaciya, proverit' na time !!!!???
          if(dY1b<0 && dY2b<0) break;
          if(dY1b) dY1b=(dY1b>0) ? 1:-1;
          if(dY2b) dY2b=(dY2b>0) ? 1:-1;
          // if dY1b*dY2b<=0 the line cross cell projection
          if(dY1b*dY2b>0) {
            Int_t dY1a=tYmin+fLay.cellShDown[cell];
            Int_t dY2a=tYmax+fLay.cellShDown[cell];
            if(dY1a) dY1a=(dY1a>0) ? 1:-1;
            if(dY2a) dY2a=(dY2a>0) ? 1:-1;
            // if dY1a*dY2a<=0 the line cross cell projection
            // dY1a*dY1b<0 The line is in the cell projection
            if(dY1a*dY2a>0 && dY1a*dY1b > 0) continue;
          }
          Int_t nDeleted=clus->setTime(layer,cell,fLay.getTime(cell));
          if(nDeleted) Warning("calcClusterSeg2",
              "%i cells was deleted (size of cluster is very big)",nDeleted);
          break;
        }
      }
    }
  }
// clus->print(); //------------
}

void HMdc34ClFinderSec::printClusters(){
// clusters.print();
}


//---------------------------------------------------------
HMdc34ClFinder::HMdc34ClFinder() : HParSet() {
  // constructor creates an array of pointers of type
  // HMdc34ClFinderSec
  // The container name is set to "Mdc34ClFinder"

  fName="Mdc34ClFinder";
  fTitle="LookupTable & cluster finder for Mdc3,4 segment";
  strcpy(detName,"Mdc");
  if(gHades) gHades->getRuntimeDb()->addContainer(this);

  array = new TObjArray(6);
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  Int_t *mods=fMdcDet->getModules();
  for (Int_t nSec = 0; nSec < 6; nSec++) {
    if(mods[nSec*4+2] || mods[nSec*4+3]) { // for mod.3&4 now !!!
      (*array)[nSec] = new HMdc34ClFinderSec(nSec,320,886); //?????????0,0???
    }
  }
  isContainer=initLookUp();
//  isContainer=kTRUE;
//  delete pCalc;
}

HMdc34ClFinder::~HMdc34ClFinder() {
  // destructor
  if(array) array->Delete();
}

HMdc34ClFinder* HMdc34ClFinder::getExObject() { // *!*
  HMdc34ClFinder* flookup=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) flookup=(HMdc34ClFinder*)rdb->getContainer("Mdc34ClFinder");
    if( flookup && !((HParSet*)flookup)->init() ) return 0;
  }
  return flookup;
}

HMdc34ClFinder* HMdc34ClFinder::getObject() { // *!*
  HMdc34ClFinder* flookup=0;
  if( gHades ) {
    HRuntimeDb* rdb=gHades->getRuntimeDb();
    if( rdb ) flookup=(HMdc34ClFinder*)rdb->getContainer("Mdc34ClFinder");
    if( !flookup ) flookup=new HMdc34ClFinder();
    if( !((HParSet*)flookup)->init() ) return 0;
  }
  return flookup;
}

Bool_t HMdc34ClFinder::initLookUp() {
  for(Int_t sec=0; sec<6; sec++)
    if((*array)[sec] && !((*this)[sec].initLookUp()) ) return kFALSE;
  return kTRUE;
}

Bool_t HMdc34ClFinder::initCat() {
  for(Int_t sec=0; sec<6; sec++) {
    if((*array)[sec] && !((*this)[sec].initCat()) ) return kFALSE;
  }
  return kTRUE;
}


void HMdc34ClFinder::clear() { // *!*
  // clears the container
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].clear();
}

void HMdc34ClFinder::printClusters(Int_t nSec=-1){
  Int_t s1=0;
  Int_t s2=5;
  if( nSec>=0 && nSec<=5 ) s1=s2=nSec;
  for(Int_t s=s1;s<=s2;s++) {
    if( (*array)[s] ) {
      printf("-Sector %i  ----Clusters:\n",s+1);
      HMdc34ClFinderSec& sec=(*this)[s];
      sec.printClusters();
    }
  }
}

Int_t  HMdc34ClFinder::findClusters(Int_t imax=12){
  Int_t ntot=0;
  return ntot;
}
