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
#include "hmdcclussim.h"
#include "hmdcseg.h"
#include "TH2.h"
#include "hkickplane.h"
#include "hgeomtransform.h"
#include "hmdcgeomstruct.h"
#include "hspecgeompar.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdclayergeompar.h"
#include "hmdcsizescells.h"
#include "hmdcgeompar.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 10/05/2001 by V. Pechenov
//*-- Modified : 07/03/2001 by V. Pechenov
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
  plotBArSc=new UChar_t [sizeBAr];
  plotBArSc4b=(Int_t *)plotBArSc;
  memset(plotBArSc,0,sizeBAr);
  rootPlot=0;
  rootPlotV2=0;
}

HMdcProjPlot::~HMdcProjPlot() {
  delete [] xMinL;
  delete [] xMaxL;
  delete [] plot;
  delete [] plotBArSc;
  if(rootPlot) {
    delete rootPlot;
    rootPlot=0;
  }
  if(rootPlotV2) {
    delete rootPlotV2;
    rootPlotV2=0;
  }
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

//----------Layer-----------------------------

HMdc34ClFinderLayer::HMdc34ClFinderLayer(Int_t sec, Int_t mod, Int_t layer) {
  // Geting of pointers to par. cont.
  cellShUp=cellShDown=0;
  HMdcGetContainers* fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return;
  HMdcGeomStruct* pMdc=fGetCont->getMdcGeomStruct();
  if( !pMdc ) return;
  nCells=((*pMdc)[sec][mod])[layer];
  cellShUp=new Short_t [nCells];
  cellShDown=new Short_t [nCells];
  yBin=0;
  nBinX=0;
}

HMdc34ClFinderLayer::~HMdc34ClFinderLayer() {
  if(yBin) {
    delete yBin;
    yBin=0;
  }
  if(cellShUp) {
    delete [] cellShUp;
    delete [] cellShDown;
    cellShUp=cellShDown=0;
  }
  yBin=0;
  nBinX=0;
}

Bool_t HMdc34ClFinderLayer::createArrayBins(Short_t nBins) {
  if( nBinX != nBins ) {
    if(yBin) delete yBin;
    nBinX=nBins;
    yBin=new Short_t [nBins];
    if(yBin==0) return kFALSE;
  }
  return kTRUE;
}

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
  sector=sec;
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  lMods=fMdcDet->getModules()+sector*4;
  mSeg[0]=((lMods[0]) ? 1:0) + ((lMods[1]) ? 2:0);
  mSeg[1]=((lMods[2]) ? 1:0) + ((lMods[3]) ? 2:0);
  array = new TObjArray(4);
  for (Int_t mod = 2; mod < 4; mod++) {
    if(lMods[mod]) (*array)[mod] = new HMdc34ClFinderMod(sector,mod);
  }
  prPlotSeg2=0;
  if(mSeg[1]) {
    prPlotSeg2=new HMdcProjPlot(inBinX,inBinY); // nBinX,inBinY will geted from ASCII file
    if(stackSeg2==0) {
      stackSizeSeg2=1000;
      stackSeg2=new Int_t [1000+50];
    }
    nearbyBins[0]=-1;
    nearbyBins[1]=+1;
    nearbyBins[2]=-prPlotSeg2->nBinX;
    nearbyBins[3]=+prPlotSeg2->nBinX;
    nearbyBins[4]=-1-prPlotSeg2->nBinX;
    nearbyBins[5]=-1+prPlotSeg2->nBinX;
    nearbyBins[6]=1-prPlotSeg2->nBinX;
    nearbyBins[7]=1+prPlotSeg2->nBinX;
  }
  isGeant = HMdcGetContainers::isGeant();
  locClus.set(3,sector,0,0);
}

HMdc34ClFinderSec::~HMdc34ClFinderSec() {
  // destructor
  if(array) array->Delete();
  if(prPlotSeg2) {
    delete prPlotSeg2;
    prPlotSeg2=0;
  }
  if(stackSeg2) {
    delete [] stackSeg2;
    stackSeg2=0;
  }
}

void HMdc34ClFinderSec::clear() {
  counter=0;
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
  return findClustersSeg2();
}

Int_t HMdc34ClFinderSec::findClustersSeg2(HMdcClus* fClus, Int_t *mBin){
  minBin=mBin;
  if(fClus->getIOSeg() != 0) return 0;
  Double_t dirX=fClus->getX();
  Double_t dirY=fClus->getY();
  Double_t dirZ=fClus->getZ();
  Double_t errDirX=fClus->getErrX();
  Double_t errDirY=fClus->getErrY();
//  Double_t errDirZ=fClus->getZ();
  Double_t x0=fClus->getXTarg();
  Double_t y0=fClus->getYTarg();
  Double_t z0=fClus->getZTarg();
  Double_t errX0=fClus->getErrXTarg();
  Double_t errY0=fClus->getErrYTarg();
  Double_t errZ0=fClus->getErrZTarg();
  // segOnkick[4]:
  //    [0]---[1]  ^ y
  //     |     |   |
  //     |     |   |
  //    [2]---[3]  |
  // X<-------------
  seg1[0].setXYZ(x0-errX0,y0-errY0,z0-errZ0);
  dirSeg1[0].setXYZ(dirX+errDirX,dirY+errDirY,
                    fClus->getZOnPlane(dirX+errDirX,dirY+errDirY));
  seg1[1].setXYZ(x0+errX0,y0-errY0,z0-errZ0);
  dirSeg1[1].setXYZ(dirX-errDirX,dirY+errDirY,
                    fClus->getZOnPlane(dirX-errDirX,dirY+errDirY));
  seg1[2].setXYZ(x0-errX0,y0+errY0,z0-errZ0);
  dirSeg1[2].setXYZ(dirX+errDirX,dirY-errDirY,
                    fClus->getZOnPlane(dirX+errDirX,dirY-errDirY));
  seg1[3].setXYZ(x0+errX0,y0+errY0,z0-errZ0);
  dirSeg1[3].setXYZ(dirX-errDirX,dirY-errDirY,
                    fClus->getZOnPlane(dirX-errDirX,dirY-errDirY));
  seg1[4].setXYZ(x0,y0,z0);               // hit
  dirSeg1[4].setXYZ(dirX,dirY,dirZ);      // hit
  for(Int_t np=0; np<5; np++) dirSeg1[np]-=seg1[np];
  indexPar=fClus->getIndex();
  Int_t firstInd=counter;
  Int_t nClustCh=findClustersSeg2();
  if(nClustCh>0) fClus->setIndexChilds(firstInd,counter-1);
  return nClustCh;
}


Int_t HMdc34ClFinderSec::findClustersSeg2(){
  for(Int_t np=0; np<5; np++) {
    dirSeg1[np]=dirSeg1[np]-seg1[np];
    if( !(fkick->calcIntersection(seg1[np],dirSeg1[np],segOnKick[np]))) {
      Error("findClustersSeg2",
        "Sec.%i: The point %i of HMDCSeg hit on the kick plane can't be calculated.",
        sector+1,np);
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
    if(segOnKick[np](0)<prPlotSeg2->xMin || segOnKick[np](0)>prPlotSeg2->xMax) {
      Warning("findClustersSeg2","S%i: The X=%g of the point %i out of the region (%g - %g).",
         sector+1, segOnKick[np](0), np, prPlotSeg2->xMin, prPlotSeg2->xMax);
      return 0;
    }
    Double_t tmp=prPlotSeg2->D() - prPlotSeg2->A()*segOnKick[np](0); // D-A*x
    al[np]=tmp-segOnKick[np](2);                       // D-A*x-z
    bl[np]=tmp*segOnKick[np](1);                       // (D-A*x)*y
    cl[np]=segOnKick[np](2)+prPlotSeg2->B()*segOnKick[np](1); // z+B*y
    nbX[np]=Int_t((segOnKick[np](0)-prPlotSeg2->xMin)/prPlotSeg2->stX);
  }
#if DEBUG_LEVEL>2
  Printf("---Sec.%i---- The region of HMdcSeg-hit on the kick plane:",sector+1);
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
  prPlotSeg2->clearArr();
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
          for(Int_t nx=0; nx<prPlotSeg2->nBinX; nx++) {
            Int_t nyU=TMath::Min(shUp+fLay.yBin[nx],prPlotSeg2->nBinY-2); //bins nBinY-1 = 0 !
            Int_t ny=TMath::Max(shDown+fLay.yBin[nx],1);  //bins ny=0 eq. 0 !
            for(; ny<=nyU; ny++) {
              if(nx<prPlotSeg2->xMinL[ny] || nx>prPlotSeg2->xMaxL[ny]) continue;
              //Fill !!!
              Int_t binNum=ny*prPlotSeg2->nBinX+nx;
              prPlotSeg2->plot[binNum] |= add;
              if(nLayers>=minBinS) {
                Int_t b1=binNum*2;
//Printf("nLayers=%i minBins=%i mBm3=%i mBm4=%i 3=%i 4=%i",
//nLayers,minBinS,mBm3,mBm4,HMdcBArray::getNSet(prPlotSeg2->plot1b[b1]),
//HMdcBArray::getNSet(prPlotSeg2->plot1b[b1+1]));
                if(HMdcBArray::getNSet(prPlotSeg2->plot1b[b1+1])>=mBm4 &&
                   HMdcBArray::getNSet(prPlotSeg2->plot1b[b1])>=mBm3) {
                   HMdcBArray::set(prPlotSeg2->plotBArSc,binNum);
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
                 (zp+prPlotSeg2->B()*yp-cl[np]);
  return Int_t( (yPrPl-prPlotSeg2->yMin)/prPlotSeg2->stY )-fLay.yBin[nbX[np]];
}

Int_t HMdc34ClFinderSec::scanPlotSeg2() {
  Int_t nClusters=0;
  Int_t loc2FirstCl=counter;
  locClus[1]=1;
  for(Int_t n4=0; n4<prPlotSeg2->sizeBAr4b; n4++) {
    if(prPlotSeg2->plotBArSc4b[n4]==0) continue;
    UChar_t *b1=prPlotSeg2->plotBArSc+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t n=-1;
    while((n=HMdcBArray::nextAndUnset(b1,b2,n))>=0) {
if(counter >= 460) {
  Printf("Sec.%i !! counter >= 460",sector);
  memset(prPlotSeg2->plotBArSc,0,prPlotSeg2->sizeBAr);
  continue;
}
      locClus[2]=counter;
      HMdcClus* clus = (HMdcClus*)fClusCat->getSlot(locClus);
      if(!clus) {
        Warning("calcClusterSeg2","S.%i No slot HMdcClus available",sector+1);
        continue;  // continue(not break) -> HMdcBArray must be cleaned !
      }
      if(isGeant) clus = (HMdcClus *) (new(clus) HMdcClusSim);
      else  clus = new(clus) HMdcClus;
      calcClusterSeg2(nBin4+n,clus);
      nClusters++;
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
  Int_t nDownLine=nBin/prPlotSeg2->nBinX;
  xMinClLines[0]=xMaxClLines[0]=nBin%prPlotSeg2->nBinX;
  stackSeg2[stackB++]=nBin;
  Int_t xMinCl=prPlotSeg2->nBinX;
  Int_t xMaxCl=0;
  while(stackT != stackB) {
    nBin=stackSeg2[stackT++];
    nx=nBin%prPlotSeg2->nBinX;
    ny=nBin/prPlotSeg2->nBinX;
    Float_t wt=(Float_t)(HMdcBArray::getNSet2B(prPlotSeg2->plot1b+nBin*2)-minBinS);
    binsInCl+=wt;
    x=(nx+0.5)*prPlotSeg2->stX+prPlotSeg2->xMin;
    y=(ny+0.5)*prPlotSeg2->stY+prPlotSeg2->yMin;
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
             sector+1);
    }
    else {
      if(nx<xMinClLines[nyL]) xMinClLines[nyL]=nx;
      else if(nx>xMaxClLines[nyL]) xMaxClLines[nyL]=nx;
      if(nx<xMinCl) xMinCl=nx;
      else if(nx>xMaxCl) xMaxCl=nx;
    }
    for(Int_t ib=0; ib<8; ib++) {
      Int_t nBinTs=nBin+nearbyBins[ib];
      if( HMdcBArray::testAndUnset(prPlotSeg2->plotBArSc,nBinTs) ) {
         stackSeg2[stackB++]=nBinTs;
      }
    }
    // If stack overflowed one will increased on 50%:
    if(stackB>stackSizeSeg2){
      Printf("HMdc34ClFinderSec: Sec.%i Stack is increased on 50\% ",sector+1);
      stackSizeSeg2 += stackSizeSeg2/2;
      Int_t* stackNew=new Int_t [stackSizeSeg2+50];
      for(Int_t n=0; n<=stackB; n++) stackNew[n]=stackSeg2[n];
      delete [] stackSeg2;
      stackSeg2=stackNew;
    }
  }
  // Filling of cluster:
  clus->setAddress(locClus);
  clus->setIndexParent(indexPar);
  averageX/=binsInCl;
  averageY/=binsInCl;
  rmsX=sqrt(rmsX/binsInCl-averageX*averageX+prPlotSeg2->stX*prPlotSeg2->stX*0.25);
  rmsY=sqrt(rmsY/binsInCl-averageY*averageY+prPlotSeg2->stY*prPlotSeg2->stY*0.25);
  clus->setXY(averageX,rmsX,averageY,rmsY);
  clus->setMinCl(minBin);
  clus->setPrPlane(*((HMdcPrPlane*)prPlotSeg2));
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
HMdc34ClFinder* HMdc34ClFinder::fMdc34ClFinder=0;
    
HMdc34ClFinder::HMdc34ClFinder() : HParSet() {
  // constructor creates an array of pointers of type HMdc34ClFinderSec
  fName="Mdc34ClFinder";
  fTitle="LookupTable & cluster finder for Mdc3,4 segment";
  strcpy(detName,"Mdc");

  array = new TObjArray(6);
  fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return;
  fMdcDet       = fGetCont->getMdcDetector();
  fSpecGeomPar  = fGetCont->getSpecGeomPar();
  fSizesCells   = HMdcSizesCells::getObject();
  fKickPlane    = fGetCont->getKickPlane();
  fMdcGeomPar   = fGetCont->getMdcGeomPar();
  fMdcClusCat = HMdcGetContainers::getCatMdcClus(kTRUE);
}

Bool_t HMdc34ClFinder::initContainer() {
  if( !fMdcDet || !fSizesCells->initContainer() ||
      !HMdcGetContainers::isInited(fSpecGeomPar) ||
      !HMdcGetContainers::isInited(fKickPlane)   ||
      !HMdcGetContainers::isInited(fMdcGeomPar) )  return kFALSE; 
  if( !status && (fSizesCells->hasChanged() || fSpecGeomPar->hasChanged() ||
      fKickPlane->hasChanged()) || fMdcGeomPar->hasChanged()) {
    changed=kTRUE;
    if(!fMdcClusCat) return kFALSE;
    Int_t *mods=fMdcDet->getModules();
    HMdc34ClFinderSec::fkick=fKickPlane;
    for (Int_t sec = 0; sec < 6; sec++) {
      if(!fMdcDet->isSectorActive(sec))  continue;
      if( !(*array)[sec] ) {
        if(mods[sec*4+2] || mods[sec*4+3]) { // for mod.3&4 now !!!
          (*array)[sec] = new HMdc34ClFinderSec(sec,320,886); //????0,0???
          (*this)[sec].fClusCat=fMdcClusCat;
        } else continue;
      }
      // initialization of container ---
      if(!calcTarget(sec)) return kFALSE;
      if(!calcProjPlaneSeg2(sec)) return kFALSE;
      if(!calcSizePlotSeg2(sec)) return kFALSE;
      if(!calcWiresProj(sec)) return kFALSE;
#if DEBUG_LEVEL>2
      (*this)[sec].prPlotSeg2->print();
#endif
      // --------------------------------
      if(versions[0]<0) versions[0]=1;
      else versions[0]++;
    }
  } else changed=kFALSE; 
  return kTRUE;
}

HMdc34ClFinder::~HMdc34ClFinder() {
  // destructor
  if(array) array->Delete();
  fMdc34ClFinder=0;
}

HMdc34ClFinder* HMdc34ClFinder::getObject() {
  if(!fMdc34ClFinder) fMdc34ClFinder=new HMdc34ClFinder();
  return fMdc34ClFinder;
}

HMdc34ClFinder* HMdc34ClFinder::getExObject() {
  return fMdc34ClFinder;
}

void HMdc34ClFinder::deleteCont() {
  if(fMdc34ClFinder) delete fMdc34ClFinder;
}

void HMdc34ClFinder::clear() { // *!*
  // clears the container
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].clear();
}

void HMdc34ClFinder::printClusters(Int_t sec=-1){
  Int_t s1=0;
  Int_t s2=5;
  if( sec>=0 && sec<=5 ) s1=s2=sec;
  for(Int_t s=s1;s<=s2;s++) {
    if( (*array)[s] ) {
      printf("-Sector %i  ----Clusters:\n",s+1);
      HMdc34ClFinderSec& sec=(*this)[s];
      sec.printClusters();
    }
  }
}

Bool_t HMdc34ClFinder::calcTarget(Int_t sec){
  //Geting size of target
  if(!fSizesCells->hasChanged() && !fSpecGeomPar->hasChanged()) return kTRUE;
  Int_t nT=fSpecGeomPar->getNumTargets()-1;
  if( nT < 0 ) {
    Error("calcTarget","Number of targets = %i!",nT+1);
    return kFALSE;
  }
  HMdc34ClFinderSec& fsec=(*this)[sec];
  fsec.target[0]=(fSpecGeomPar->getTarget(0)->getTransform()).getTransVector();
  fsec.target[0].setZ( fsec.target[0].getZ() +
      fSpecGeomPar->getTarget(0)->getPoint(0)->getZ());
  fsec.target[1]=(fSpecGeomPar->getTarget(nT)->getTransform()).getTransVector();
  fsec.target[1].setZ( fsec.target[1].getZ() +
      fSpecGeomPar->getTarget(nT)->getPoint(2)->getZ());
//  const HGeomTransform& trans=fGetCont->getLabTransSec(sec);
  const HGeomTransform* trans=(*fSizesCells)[sec].getLabTrans();
  if(&trans == 0) return kFALSE;
  fsec.target[0]=trans->transTo(fsec.target[0]);
  fsec.target[1]=trans->transTo(fsec.target[1]);
  return kTRUE;
}


Bool_t HMdc34ClFinder::calcProjPlaneSeg2(Int_t sec){
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
  if(!fSizesCells->hasChanged()) return kTRUE;
  HMdc34ClFinderSec& fsec=(*this)[sec];
  HMdcSizesCellsSec& fSCellsSec=(*fSizesCells)[sec];
  if( !&fSCellsSec ) return kFALSE;
  Float_t par=10.; //In a future will be geted from Par.Container

  Int_t nL=(Int_t)par;
  //                (mdc3 don't exist)           (mdc4 don't exist)
  if(nL<-7||nL>7 || (nL<0 && fsec.mSeg[1]==2) || (nL>0 && fsec.mSeg[1]==1) ||
    //(one mdc exist only)
      (nL==0 && fsec.mSeg[1]!=3)) {
    nL=0;
    par=0.1;
  }
  Char_t *text="as projection plane";
  HGeomTransform prPl;
  if( (nL==0 && fsec.mSeg[1]<3) || nL==-7 || nL==7 ) {
    // the project plane - the middle plane of MDC3 or MDC4
    Int_t mod=(nL==0) ? fsec.mSeg[1]+1 : (nL+7)/14+2;
    
    prPl=*(fSCellsSec[mod].getSecTrans());
    printf("\n===> Sec.%i Seg.2: Using middle plane of MDC%i %s\n",
                              sec+1,mod+1,text);
  } else if( nL==0 && fsec.mSeg[1]==3 ) {
    // the project plane - between MDC3 & MDC4
    const HGeomTransform* trLayer6=fSCellsSec[2][5].getSecTrans();
    const HGeomTransform* trLayer1=fSCellsSec[3][0].getSecTrans();
    // MDC3 & MDC4 are ~parallel:
    Double_t distToL6=trLayer6->getTransVector().length();
    Double_t distToL1=trLayer1->getTransVector().length();
    Double_t newDist=distToL6+(distToL1-distToL6)*(1.+par)/2.;
    Double_t mult=1.;
    if(par<=0) {    // proj.plane will parallel to MDC3
      prPl.setTransform(*trLayer6);
      mult=newDist/distToL6;
    } else {        // proj.plane will parallel to MDC4
      prPl.setTransform(*trLayer1);
      mult=newDist/distToL1;
    }
    HGeomVector prTr(prPl.getTransVector());
    prTr*=mult;
    prPl.setTransVector(prTr);
    printf("\n===> Sec.%i Seg.2: Using plane between MDC 3 & 4 (p=%g) %s\n",
     sec+1,par,text);
  } else {
    // the project plane - one of the layers
    Int_t mod=(nL<0) ? 2:3;
    if(nL<0) nL+=7;
    prPl=*(fSCellsSec[mod][nL-1].getSecTrans());
    printf("\n===> Sec.%i Seg.2: Using MDC%i, layer %i %s\n",
        sec+1,mod+1,nL,text);
  }
  fsec.prPlotSeg2->setPrPlane(prPl);
  return kTRUE;
}

Bool_t HMdc34ClFinder::calcSizePlotSeg2(Int_t sec){
  // Calculation of plot's size:
  if( !fKickPlane->hasChanged() && !fMdcGeomPar->hasChanged() &&
      !fSizesCells->hasChanged()) return kTRUE;
  HMdc34ClFinderSec& fsec=(*this)[sec];
  if(!fsec.mSeg[0]) return kFALSE;
  HMdcSizesCellsSec& fSCellsSec=(*fSizesCells)[sec];
  if( !&fSCellsSec ) return kFALSE;
  HGeomVector vect[2][4];
  HGeomVector pKick[4];
  const HGeomTransform* trans=0;
  HGeomVector newP[4][2]; //[4]-num.lines, [2]-the firt&last points of the line
  HMdcPrPlane plane;
  for(Int_t mod=0; mod<2; mod++) {
    if(!(fsec.lMods[mod])) continue;
    HGeomCompositeVolume *fVolMdc=fGetCont->getGeomCompositeVolume(mod);
    if(!fVolMdc) return kFALSE;
    HGeomVolume* fVolLayer=fVolMdc->getComponent(5);
    if(!fVolLayer) return kFALSE;
    trans=fSCellsSec[mod].getSecTrans();
    plane.setPrPlane(*trans);
    for(Int_t point=0; point<4; point++) {
      HGeomVector *fpoint=fVolLayer->getPoint(point);
      if(!fpoint) return kFALSE;
      Int_t indx=(fsec.mSeg[0]==3) ? mod : 0; // for one  MDC2 in seg. only
      vect[indx][point]=*fpoint;
      vect[indx][point].setZ(0.);
      vect[indx][point]=trans->transFrom(vect[indx][point]);
      if(mod==1 && fsec.mSeg[0]==3) {
        Int_t nTag=(point==0 || point==3) ? 0:1;
        vect[0][point]-=fsec.target[nTag];
        plane.calcIntersection(fsec.target[nTag],vect[0][point],vect[0][point]);
      }
    }
  }
  if(fsec.mSeg[0]==3) {
    for(Int_t mod=0; mod<2; mod++) {
      for(Int_t point=0; point<4; point++)
                       vect[mod][point]=trans->transTo(vect[mod][point]);
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
                      vect[0][point]=trans->transFrom(vect[0][point]);
  }
  HGeomVector dir;
  for(Int_t point=0; point<4; point++) {
    if(point==0 || point==3) dir=vect[0][point]-fsec.target[0];
    else dir=vect[0][point]-fsec.target[1]; //???
    if( !(fKickPlane->calcIntersection(vect[0][point],dir,pKick[point]))) {
      Error("calcSizePlotSeg2",
        "Sec.%i: The point %i on the kick plane can't be calculated.",
        sec+1,point);
      return kFALSE;
    }
  }
  // pKick[0-3] pointers on the kick-plane
  Float_t yMin=1.e+10;
  Float_t yMax=-1.e+10;
  for(Int_t mod=2; mod<4; mod++) {
    if(!(fsec.lMods[mod])) continue;
    HGeomCompositeVolume *fVolMdc=fGetCont->getGeomCompositeVolume(mod);
    Int_t indx=(fsec.mSeg[1]==3) ? mod-2 : 0; // for one  MDC2 in seg. only
    trans=fSCellsSec[mod].getSecTrans();
    for(Int_t point=0; point<4; point++) {
      vect[indx][point]=*(fVolMdc->getPoint(point));
      vect[indx][point].setZ(0.);  //midplane of MDC
      vect[indx][point]=trans->transFrom(vect[indx][point]);
      vect[indx][point]-=pKick[point];
      fsec.prPlotSeg2->calcIntersection(pKick[point],vect[indx][point],
          vect[indx][point]);
      if(vect[indx][point](1)<yMin) yMin=vect[indx][point](1);
      if(vect[indx][point](1)>yMax) yMax=vect[indx][point](1);
    }
  }
  Float_t xMaxD=xLine(vect[0][0],vect[0][1],yMin);
  Float_t xMinD=xLine(vect[0][2],vect[0][3],yMin);
  Float_t xMax=xLine(vect[0][0],vect[0][1],yMax);
  Float_t xMin=xLine(vect[0][2],vect[0][3],yMax);
  if(fsec.mSeg[1]==3) {
    Float_t xnew=xLine(vect[1][0],vect[1][1],yMin);
    if(xnew>xMaxD) xMaxD=xnew;
    xnew=xLine(vect[1][2],vect[1][3],yMin);
    if(xnew<xMinD) xMinD=xnew;
    xnew=xLine(vect[1][0],vect[1][1],yMax);
    if(xnew>xMax) xMax=xnew;
    xnew=xLine(vect[1][2],vect[1][3],yMax);
    if(xnew<xMin) xMin=xnew;
  }
  fsec.prPlotSeg2->setEdges(yMin, xMinD, xMaxD, yMax, xMin, xMax);
  return kTRUE;
}

void HMdc34ClFinder::calcCrossLines(HGeomVector& p1l1, HGeomVector& p2l1,
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

Float_t HMdc34ClFinder::xLine(HGeomVector& p1,HGeomVector& p2,Float_t yi) {
  return (yi-p2(1))/(p1(1)-p2(1))*(p1(0)-p2(0))+p2(0);
}

Bool_t HMdc34ClFinder::calcWiresProj(Int_t sec) {
  if( !fKickPlane->hasChanged() && !fMdcGeomPar->hasChanged() &&
      !fSizesCells->hasChanged()) return kTRUE;  
  HMdc34ClFinderSec& fsec=(*this)[sec];
  if(!fsec.mSeg[0]) return kFALSE;
  HMdcSizesCellsSec& fSCellsSec=(*fSizesCells)[sec];
  if( !&fSCellsSec ) return kFALSE;
  HGeomVector wire[2];
  HGeomVector rib[4];
  HGeomVector point[2];
  Int_t mod=(fsec.mSeg[0]>1) ? 1:0;
  const HGeomTransform* trans=fSCellsSec[mod].getSecTrans();
  if(!trans) return kFALSE;
  HGeomCompositeVolume *fVolMdc=fGetCont->getGeomCompositeVolume(mod);
  if(!fVolMdc) return kFALSE;
  HGeomVector tag=fsec.target[0]+fsec.target[1];
  tag/=2.;
  HGeomVector dir=trans->getTransVector()-tag;
  dir/=dir.length();
  HGeomVector pKick;
  if(!(fKickPlane->calcIntersection(tag,dir,pKick))) {
    Error("calcWiresProj",
      "Sec.%i: The mid.point on the kick plane can't be calculated.",sec+1);
    return kFALSE;
  }
  for(Int_t mod=2; mod<4; mod++) {
    if(!fsec.lMods[mod]) continue;
    fVolMdc=fGetCont->getGeomCompositeVolume(mod);
    if(!fVolMdc) return kFALSE;
    HMdcSizesCellsMod& fSCellsMod=fSCellsSec[mod];
    if(!&fSCellsMod) return kFALSE;
    HMdc34ClFinderMod& fMod=fsec[mod];
    for(Int_t lay=0; lay<6; lay++) {
      HMdcSizesCellsLayer& fSCellsLay=fSCellsMod[lay];
      HGeomVolume* fVolLayer=fVolMdc->getComponent(lay);
      if(!fVolLayer) {
        Error("calcWiresProj",
            "The HGeomVolume (S%i|M%i|L%i) doesn't exist!\n",
            sec+1,mod+1,lay+1);
        return kFALSE;
      }      
      const HGeomTransform* secsys=fSCellsLay.getSecTrans();
      Double_t catDist=fSCellsLay.getHalfCatDist();
      Double_t pitch=fSCellsLay.getPitch();
      Double_t cosWiOr=fSCellsLay.getCosWireOr();
      Double_t wOfSet=fSCellsLay.getWireOffSet();
      Double_t aWire=fSCellsLay.getTanWireOr();   // y=a*x+ln[1]

      // calc. of the wire's projection
      Double_t bWire=(100.*pitch-wOfSet)/cosWiOr;  // 100.-cell n.100
      wire[0].setXYZ(500.,aWire*500.+bWire,0.);
      wire[1].setXYZ(-500.,-aWire*500.+bWire,0.);
      for(Int_t np=0; np<2; np++) {
        wire[np]=secsys->transFrom(wire[np]);
//        wire[np]-=pKick;
        fsec.prPlotSeg2->calcIntersection(pKick,wire[np],point[np]);
      }
#if DEBUG_LEVEL>2
      Printf("M%i L%i C%i Size of wire on the proj.plane (poin1&point2):",
        mod+1,lay+1,100+1);
      point[0].print();
      point[1].print();
#endif
      Float_t al=(point[0](1)-point[1](1))/(point[0](0)-point[1](0)); // (y1-y2)/(x1-x2)
      Float_t x=(fsec.prPlotSeg2->nBinX-1) * fsec.prPlotSeg2->stX + 
          fsec.prPlotSeg2->xMin;
      Float_t yLeft=al*( x - point[1](0))+point[1](1); // (x-x2)*a+y2
      Float_t yRight=al*(fsec.prPlotSeg2->xMin - point[1](0))+point[1](1);
      Short_t nYLeft =Short_t((yLeft  - fsec.prPlotSeg2->yMin) / 
          fsec.prPlotSeg2->stY);
      Short_t nYRight=Short_t((yRight - fsec.prPlotSeg2->yMin) / 
          fsec.prPlotSeg2->stY);
      Short_t yShift=TMath::Min(nYLeft,nYRight);
      if(TMath::Abs(nYLeft-nYRight)>=fsec.prPlotSeg2->nBinY) {
        Error("calcWiresProj",
        "S%i M%i L%i The region of Y bins of proj. wire >= nBinY(%i).",
        sec+1,mod+1,lay+1,fsec.prPlotSeg2->nBinY);
        return kFALSE;
      }
      HMdc34ClFinderLayer& fLayer=fMod[lay];
      if( !fLayer.createArrayBins(fsec.prPlotSeg2->nBinX) ) return kFALSE;
      for(Int_t nx=0; nx<fsec.prPlotSeg2->nBinX; nx++) {
        Float_t x=nx * fsec.prPlotSeg2->stX + fsec.prPlotSeg2->xMin;
        Float_t y=al*(x-point[1](0))+point[1](1);
        Short_t nY=Short_t((y - fsec.prPlotSeg2->yMin) / fsec.prPlotSeg2->stY);
        fLayer.yBin[nx]=nY-yShift;
      }

      // calc. of the position of the  cells' ribs:
      // Z(x)=A1*x+Y0[nCell]; Z(x)=A2*x+Z0[nCell]; Y0&Z0 - at x=0;
      // Y0[nCell]=Ystep*nCell + Yshift; Z0[nCell]=Zstep*nCell + Zshift(=0!);
      //--- cells 0 & 100 ---
      for(Int_t cell=0; cell<101; cell+=100) {
        bWire=(cell*pitch-wOfSet-pitch/2.)/cosWiOr;
        // rib[0]-rib[1] - the down and left rib of cell
        // rib[2]-rib[3] - the down and right rib of cell
        for(Int_t n=0; n<4; n++) {
          Double_t x=((n&1) == 0) ? 500.: -500.;
          Double_t z=(n < 2) ? -catDist : catDist;
          rib[n].setXYZ( x,  aWire*x+bWire, z);
          rib[n]=secsys->transFrom(rib[n]);
        }
        Double_t dX=rib[0](0)-rib[1](0);
        rib[1].setXYZ(0.,(rib[0](0)*rib[1](1)-rib[1](0)*rib[0](1))/dX,
                         (rib[0](0)*rib[1](2)-rib[1](0)*rib[0](2))/dX);
        dX=rib[2](0)-rib[3](0);
        rib[3].setXYZ(0.,(rib[2](0)*rib[3](1)-rib[3](0)*rib[2](1))/dX,
                         (rib[2](0)*rib[3](2)-rib[3](0)*rib[2](2))/dX);
        if(cell==0) {
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
          if(rib[1](2)+fsec.prPlotSeg2->B()*rib[1](1)-fsec.prPlotSeg2->D()<0) {
            fLayer.nPSegOnKick[0][0]=(fLayer.tgY>0) ? 2 : 3;
            fLayer.nPSegOnKick[1][0]=(fLayer.tgY>0) ? 1 : 0;
          } else {
            fLayer.nPSegOnKick[0][0]=(fLayer.tgY>0) ? 1 : 0;
            fLayer.nPSegOnKick[1][0]=(fLayer.tgY>0) ? 2 : 3;
          }
          if(rib[3](2)+fsec.prPlotSeg2->B()*rib[3](1)-fsec.prPlotSeg2->D()<0) {
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
