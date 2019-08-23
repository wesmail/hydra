using namespace std;
#include "hmdc34clfinder.h"
#include "hades.h"
#include "hmatrixcategory.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include <iostream>
#include <iomanip>
#include "hmdctrackddef.h"
#include "hmdcgeomobj.h"
#include "hmdcgetcontainers.h"
#include "hmdccal1sim.h"
#include "hmdcclussim.h"
#include "hmdccluster.h"
#include "hmdcseg.h"
#include "TH2.h"
#include "TMath.h"
#include "hkickplane2.h"
#include "hgeomtransform.h"
#include "hmdcgeomstruct.h"
#include "hspecgeompar.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdclayergeompar.h"
#include "hmdcsizescells.h"
#include "hmdcgeompar.h"
#include "hmdcclfnstack.h"
#include "hmdctrackdset.h"
#include "hmdcgeanttrack.h"
#include <stdlib.h>

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 07/02/2003 by V. Pechenov
//*-- Modified : 05/06/2002 by V. Pechenov
//*-- Modified : 10/05/2001 by V. Pechenov
//*-- Modified : 07/03/2001 by V. Pechenov
//*-- Modified : 12/07/2000 by V.Pechenov

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdc34ClFinder
//
// Track finder for outer segment MDC:
// Constructor:
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcProjPlot)
ClassImp(HMdc34ClFinderLayer)
ClassImp(HMdc34ClFinderMod)
ClassImp(HMdc34ClFinderSec)
ClassImp(HMdc34ClFinder)

using namespace TMath;

Char_t* HMdcProjPlot::weights=0;
Int_t   HMdcProjPlot::wtArrSize=0;

HMdcProjPlot::HMdcProjPlot(UChar_t mSeg, Int_t inBinX, Int_t inBinY) {
  nBinX = inBinX;
  nBinY = inBinY;
  size  = nBinX*nBinY;
  size  = (size/32 + ((size%32 > 0) ? 1:0))*32;
  xMinL = new Short_t [nBinY];
  xMaxL = new Short_t [nBinY];
  yMinL = new Short_t [nBinX];
  if(mSeg&1) plModF=new UChar_t [size];
  else plModF=0;
  if(mSeg&2) plModS=new UChar_t [size];
  else plModS=0;
  clearArr();
  sizeBAr     = size/8;
  plotBArSc   = new UChar_t [sizeBAr];
  plotBArSc4b = (Int_t *)plotBArSc;
  clearBitArr();
  rootPlot   = 0;
  rootPlotV2 = 0;
  if(size>wtArrSize) {
    if(weights) delete [] weights;
    weights   = new Char_t [size];
    wtArrSize = size;
  }
  xBinsPos = new Double_t [nBinX];
  yBinsPos = new Double_t [nBinY];
}

void HMdcProjPlot::clearArr(void) {
  if(plModF) memset(plModF,0,size);
  if(plModS) memset(plModS,0,size);
}

void HMdcProjPlot::clearBitArr(void) {
  memset(plotBArSc,0,sizeBAr);
}

HMdcProjPlot::~HMdcProjPlot() {
  if(xMinL) delete [] xMinL;
  if(xMaxL) delete [] xMaxL;
  if(yMinL) delete [] yMinL;
  xMinL=xMaxL=yMinL=0;
  if(plModF) delete [] plModF;
  if(plModS) delete [] plModS;
  plModF=plModS=0;
  if(plotBArSc) delete [] plotBArSc;
  plotBArSc=0;
  if(rootPlot) {
    delete rootPlot;
    rootPlot=0;
  }
  if(rootPlotV2) {
    delete rootPlotV2;
    rootPlotV2=0;
  }
  if(weights) {
    delete [] weights;
    weights=0;
    wtArrSize=0;
  }
  if(xBinsPos) delete [] xBinsPos;
  if(yBinsPos) delete [] yBinsPos;
  xBinsPos=yBinsPos=0;
}

void HMdcProjPlot::setEdges(Double_t iyMin, Double_t ixMinD, Double_t ixMaxD,
                            Double_t iyMax, Double_t ixMin,  Double_t ixMax) {
  stY          = (iyMax-iyMin)/(nBinY-2);
  stX          = (ixMax-ixMin)/(nBinX-2);
  xHStep2      = stX*stX*0.25;            // (stX/2)^2
  yHStep2      = stY*stY*0.25;            // (stY/2)^2
      
  yMin         = iyMin-stY;
  yMax         = iyMax+stY;
  xMin         = ixMin-stX;
  xMax         = ixMax+stX;
  xMinD        = ixMinD;
  xMaxD        = ixMaxD;
  Double_t aL23 = (xMinD-xMin)/(yMin-yMax);
  Double_t aL01 = (xMaxD-xMax)/(yMin-yMax);
  for(Int_t ny=0; ny<nBinY; ny++) {
    Double_t yl = (ny+1)*stY+yMin;
    xMinL[ny]  = Short_t((((yl-yMax)*aL23+xMin)-xMin)/stX);
    if(xMinL[ny] < 1) xMinL[ny]=1;
    xMaxL[ny]  = Short_t((((yl-yMax)*aL01+xMax)-xMin)/stX);
    if(xMaxL[ny] > nBinX-2) xMaxL[ny]=nBinX-2;
  }

  for(Int_t nx=0; nx<nBinX; nx++) {
    yMinL[nx]=nBinY;
    for(Int_t ny=nBinY-2; ny>=0; ny--) {
      if(nx<xMinL[ny] || nx>xMaxL[ny]) break;
      yMinL[nx] = ny==0 ? 1 : ny;
    }
  }
  for(Int_t n=0; n<nBinX; n++) xBinsPos[n]=(((Double_t)n)+0.5)*stX+xMin;
  for(Int_t n=0; n<nBinY; n++) yBinsPos[n]=(((Double_t)n)+0.5)*stY+yMin;
}

void HMdcProjPlot::print(void) {
  printf("----- Project plane: -----\n");
  HMdcPlane::print();
  printf(" Sizes: nBinX=%i, nBinY=%i, stepX=%g, stepY=%g\n",nBinX,nBinY,stX,stY);
  printf(" Points (x:y) n. 0=(%.1f:%.1f)  1=(%.1f:%.1f)\n",xMaxD,yMin,xMax,yMax);
  printf("                 2=(%.1f:%.1f)  3=(%.1f:%.1f)\n",xMin,yMax,xMinD,yMin);
}


TH2C* HMdcProjPlot::getPlot(Char_t* name, Char_t* title){
  if(!rootPlot) rootPlot=new TH2C(name,title,nBinX,xMin,xMax,nBinY,yMin,yMax);
  else {
    rootPlot->Reset();
    rootPlot->SetName(name);
    rootPlot->SetTitle(title);
  }
  rootPlot->SetMaximum(12.);
  rootPlot->SetMinimum(0.);
  rootPlot->Fill(0.,0.,0);
  for (Int_t nBin=0; nBin<size; nBin++) {
    Int_t wt=0;
    if(plModF && plModF[nBin]) wt+=HMdcBArray::getNSet(plModF[nBin]);
    if(plModS && plModS[nBin]) wt+=HMdcBArray::getNSet(plModS[nBin]);
    if(wt==0) continue;
    rootPlot->Fill(xBinsPos[nBin/nBinX],yBinsPos[nBin%nBinX],(Stat_t)wt);
  }
  return rootPlot;
}

TH2C* HMdcProjPlot::getPlotV2(Char_t* name, Char_t* title){
  if(!rootPlotV2) rootPlotV2=new TH2C(name,title,nBinX,xMin,xMax,nBinY,yMin,yMax);
  else {
    rootPlotV2->Reset();
    rootPlotV2->SetName(name);
    rootPlotV2->SetTitle(title);
  }
  rootPlotV2->SetMaximum(18.);
  rootPlotV2->SetMinimum(0.);
  rootPlotV2->Fill(0.,0.,0);
  for (Int_t nBin=0; nBin<size; nBin++) {
    Int_t wt=0;
    if(plModF && plModF[nBin]) wt+=HMdcBArray::getNSet(plModF[nBin]);
    if(plModS && plModS[nBin]) wt+=HMdcBArray::getNSet(plModS[nBin])+6;
    if(wt==0) continue;
    rootPlotV2->Fill(xBinsPos[nBin/nBinX],yBinsPos[nBin%nBinX],(Stat_t)wt);
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
  nCells     = ((*pMdc)[sec][mod])[layer];
  cellShUp   = new Short_t [nCells];
  cellShDown = new Short_t [nCells];
  yBin       = 0;
  nBinX      = 0;
  xBin1      = 0;
  xBin2      = 0;
  cells      = 0;
}

HMdc34ClFinderLayer::~HMdc34ClFinderLayer() {
  if(yBin)       delete [] yBin;
  if(xBin1)      delete [] xBin1;
  if(xBin2)      delete [] xBin2;
  if(cellShUp)   delete [] cellShUp;
  if(cellShDown) delete [] cellShDown;
  yBin       = 0;
  xBin1      = 0;
  xBin2      = 0;  
  cellShUp   = 0;
  cellShDown = 0;
  nBinX      = 0;
}

Bool_t HMdc34ClFinderLayer::createArrayBins(Short_t nBins) {
  if(nBinX == nBins) return kTRUE;
  if(yBin) delete [] yBin;
  nBinX = nBins;
  yBin  = new Short_t [nBins];
  if(yBin==0) return kFALSE;
  return kTRUE;
}

//------------Module----------------------------
HMdc34ClFinderMod::HMdc34ClFinderMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type HMdc34ClFinderLayer
  //
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++) {
    (*array)[layer] = new HMdc34ClFinderLayer(sec, mod, layer);
  }
}

HMdc34ClFinderMod::~HMdc34ClFinderMod() {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdc34ClFinderMod::getNCells(void) {
  Int_t nHits = 0;
  for(Int_t lay=0; lay<6; lay++) nHits += (*this)[lay].cells->getNCells();
  return nHits;
}

//----------Sector------------------------------------

HMdc34ClFinderSec::HMdc34ClFinderSec(Int_t sec, Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdc34ClFinderMod
  //
  sector = sec;
  HMdcDetector* fMdcDet = HMdcGetContainers::getObject()->getMdcDetector();
  const Int_t* nLM      = HMdcTrackDSet::getTrFnNLayersInMod()+sector*4;
  for(Int_t mod=0;mod<4;mod++)
      lMods[mod]=(fMdcDet->getModule(sector,mod)&&nLM[mod]>0) ? 1:0;
  mSeg[0] = (lMods[0] ? 1:0) + (lMods[1] ? 2:0);
  mSeg[1] = (lMods[2] ? 1:0) + (lMods[3] ? 2:0);
  array = new TObjArray(4);
  typeClFinder=0;
  for (Int_t mod = 2; mod < 4; mod++) {
    if(lMods[mod]) (*array)[mod] = new HMdc34ClFinderMod(sector,mod);
  }
  prPlotSeg2 = 0;
  if(mSeg[1] > 0) {
    prPlotSeg2=new HMdcProjPlot(mSeg[1],inBinX,inBinY);
    nBinX=prPlotSeg2->nBinX;
    nearbyBins[0] = -1;
    nearbyBins[1] = +1;
    nearbyBins[2] = -nBinX;
    nearbyBins[3] = +nBinX;
    nearbyBins[4] = -1-nBinX;
    nearbyBins[5] = -1+nBinX;
    nearbyBins[6] = 1-nBinX;
    nearbyBins[7] = 1+nBinX;
  }
  isGeant = HMdcGetContainers::isGeant();
  
  locClus.set(3,sector,0,0);
  for(Int_t mod=2;mod<4;mod++) {
    if(lMods[mod]) {
      xCMin[mod] = new Int_t [prPlotSeg2->nBinY];
      xCMax[mod] = new Int_t [prPlotSeg2->nBinY];
      for(Int_t y=0;y<prPlotSeg2->nBinY;y++) {
        xCMin[mod][y] = prPlotSeg2->size;
        xCMax[mod][y] = -1;
      }
    } else {
      xCMin[mod] = 0;
      xCMax[mod] = 0;
    }
  }
  xMinM2 = xCMin[2];
  xMaxM2 = xCMax[2];
  pClustersArrs = HMdcClustersArrs::getObject();
  clusArr = pClustersArrs->getArray1();
  if(clusArr == 0) clusArr = pClustersArrs->createArray1(500); // Array size
  clusArrSize = pClustersArrs->getArray1Size();
  mergeFlag   = kTRUE;
}

HMdc34ClFinderSec::~HMdc34ClFinderSec() {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  if(prPlotSeg2) delete prPlotSeg2;
  prPlotSeg2=0;
  for(Int_t mod=2; mod<4; mod++) {
    if(xCMin[mod]) delete [] xCMin[mod];
    xCMin[mod]=0;
    if(xCMax[mod]) delete [] xCMax[mod];
    xCMax[mod]=0;
  }
}

void HMdc34ClFinderSec::clear(void) {
  counter      = 0;
  maxAmp[2]    = -1;
  maxAmp[3]    = -1;
  notEnoughWrs = kTRUE;
}

void HMdc34ClFinderSec::setMinBin(Int_t *mBin) {
  for(Int_t i=0;i<4;i++) minAmp[i]=mBin[i];
  notEnoughWrs = testMaxAmp();
}

Bool_t HMdc34ClFinderSec::testMaxAmp(void) {
  // Calculating checking max.amplitude
  for(Int_t mod=2;mod<4;mod++) {
    maxAmp[mod]=0;
    if(minAmp[mod]<=0) continue;
    HMdc34ClFinderMod& fMod=(*this)[mod];
    for(Int_t lay=0;lay<6;lay++) {
      HMdc34ClFinderLayer& fLay=fMod[lay];
      if(fLay.cells->getFirstCell()>=0) maxAmp[mod]++;
    }
  }
  if(minAmp[2]>0 && minAmp[3]>0) {
    if(minAmp[2]>maxAmp[2] || minAmp[3]>maxAmp[3]) return kTRUE;
  } else {
    if(minAmp[2]>maxAmp[2]) return kTRUE;
    if(minAmp[3]>maxAmp[3]) return kTRUE;
  }
  return kFALSE;
}

Int_t HMdc34ClFinderSec::findClustersSeg2(HMdcSeg* fSeg, HMdcClus* fClus,
    Int_t *mBin){
  if(mBin!=0) setMinBin(mBin);
  if(notEnoughWrs) return 0;
  Float_t z0,errZ0, r0,errR0, theta,errTh, phi,errPh;
  fSeg->getZR(z0,errZ0,r0,errR0);
  fSeg->getThetaPhi(theta,errTh,phi,errPh);
  // Errors ????????????????????????????????????????????????
  Double_t cosPhiS = cos(phi+Pi()*0.5);
  Double_t sinPhiS = sin(phi+Pi()*0.5);
  Double_t x0      = r0*cosPhiS;
  Double_t y0      = r0*sinPhiS;
  // let length=1.
  Double_t cosPhi  = cos(phi);
  Double_t sinPhi  = sin(phi);
  Double_t cosTh   = cos(theta);
  Double_t sinTh   = sin(theta);
  Double_t dirX    = cosPhi*sinTh;
  Double_t dirY    = sinPhi*sinTh;
  Double_t dirZ    = cosTh;

  Double_t errX0   = sqrt(Power(cosPhiS*errR0,2)+Power(y0*errPh,2));
  Double_t errY0   = sqrt(Power(sinPhiS*errR0,2)+Power(x0*errPh,2));
//errY0=errY0/10.; //!!!!!!!!!!!!!!!!!!!!!!!! ??????????
  Double_t errDirX = sqrt(Power(sinPhi*sinTh*errPh,2)+
                          Power(cosPhi*cosTh*errTh,2));
  Double_t errDirY = sqrt(Power(cosPhi*sinTh*errPh,2)+
                          Power(sinPhi*cosTh*errTh,2));
//errDirY=errDirY/10.; //!!!!!!!!!!!!!!!!!!!!!!!! ??????????
  Double_t errDirZ = Abs(sinTh*errTh);
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
  indexPar=fClusCat->getIndex(fClus);
  indexFCh=-1;
  indexLCh=-2;
  Int_t nClustCh=findClustersSeg2();
  if(nClustCh>0) fClus->setIndexChilds(indexFCh,indexLCh);
  return nClustCh;
}

Int_t HMdc34ClFinderSec::findClustersSeg2(HMdcClus* fClus, Int_t *mBin){
  if(fClus->getIOSeg() != 0) return 0;
  if(mBin!=0) setMinBin(mBin);
  if(notEnoughWrs) return 0;
  Double_t dirX    = fClus->getX();
  Double_t dirY    = fClus->getY();
  Double_t dirZ    = fClus->getZ();
  Double_t errDirX = fClus->getErrX();
  Double_t errDirY = fClus->getErrY();
  // Double_t errDirZ=fClus->getZ();
  Double_t x0      = fClus->getXTarg();
  Double_t y0      = fClus->getYTarg();
  Double_t z0      = fClus->getZTarg();
  Double_t errX0   = fClus->getErrXTarg();
  Double_t errY0   = fClus->getErrYTarg();
  Double_t errZ0   = fClus->getErrZTarg();
  // segOnkick[4]:
  //    [0]---[1]  ^ y
  //     |     |   |
  //     |     |   |
  //    [2]---[3]  |
  // X<-------------
  seg1[0].setXYZ(x0-errX0,y0-errY0,z0-errZ0);
  dirSeg1[0].setXYZ(dirX+errDirX,dirY+errDirY,
                    fClus->getZOnPrPlane(dirX+errDirX,dirY+errDirY));
  seg1[1].setXYZ(x0+errX0,y0-errY0,z0-errZ0);
  dirSeg1[1].setXYZ(dirX-errDirX,dirY+errDirY,
                    fClus->getZOnPrPlane(dirX-errDirX,dirY+errDirY));
  seg1[2].setXYZ(x0-errX0,y0+errY0,z0-errZ0);
  dirSeg1[2].setXYZ(dirX+errDirX,dirY-errDirY,
                    fClus->getZOnPrPlane(dirX+errDirX,dirY-errDirY));
  seg1[3].setXYZ(x0+errX0,y0+errY0,z0-errZ0);
  dirSeg1[3].setXYZ(dirX-errDirX,dirY-errDirY,
                    fClus->getZOnPrPlane(dirX-errDirX,dirY-errDirY));
  seg1[4].setXYZ(x0,y0,z0);               // hit
  dirSeg1[4].setXYZ(dirX,dirY,dirZ);      // hit
  for(Int_t np=0; np<5; np++) dirSeg1[np]-=seg1[np];
  indexPar = fClusCat->getIndex(fClus);
  indexFCh = -1;
  indexLCh = -2;
  Int_t nClustCh=findClustersSeg2();
  if(nClustCh>0) fClus->setIndexChilds(indexFCh,indexLCh);
  return nClustCh;
}

Int_t HMdc34ClFinderSec::findClustersSeg2(void){
  for(Int_t np=0; np<5; np++) {
    if( !(fkick->calcIntersection(seg1[np],dirSeg1[np],segOnKick[np]))) {
      Error("findClustersSeg2",
        "Sec.%i: The point %i of HMDCSeg hit on the kick plane can't be calculated.",
        sector+1,np);
      return 0;
    }
    if(np==4) {
      dirSeg1[4].setXYZ( (Abs(segOnKick[0](0)-segOnKick[1](0))+
                          Abs(segOnKick[2](0)-segOnKick[3](0)))*0.25,
                         (Abs(segOnKick[0](1)-segOnKick[2](1))+
                          Abs(segOnKick[1](1)-segOnKick[3](1)))*0.25,
                         (Abs(segOnKick[0](2)-segOnKick[2](2))+
                          Abs(segOnKick[1](2)-segOnKick[3](2)))*0.25);
      break;
    }
    if(segOnKick[np](0)<prPlotSeg2->xMin || segOnKick[np](0)>prPlotSeg2->xMax) {
      Warning("findClustersSeg2","S%i: The X=%g of the point %i out of the region (%g - %g).",
         sector+1, segOnKick[np](0), np, prPlotSeg2->xMin, prPlotSeg2->xMax);
      return 0;
    }
    Double_t tmp=prPlotSeg2->D() - prPlotSeg2->A()*segOnKick[np](0); //D-A*x
    al[np]      = tmp-segOnKick[np](2);                              //D-A*x-z
    bl[np]      = tmp*segOnKick[np](1);                              //(D-A*x)*y
    cl[np]      = segOnKick[np](2)+prPlotSeg2->B()*segOnKick[np](1); //z+B*y
    nbX[np]     = Int_t((segOnKick[np](0)-prPlotSeg2->xMin)/prPlotSeg2->stX);
  }
#if DEBUG_LEVEL>2
  printf("---Sec.%i---- The region of HMdcSeg-hit on the kick plane:\n",sector+1);
  for(Int_t np=0; np<4; np++) segOnKick[np].print();
  printf("x-x(0,1)=%g x-x(2,3)=%g y-y(0,2)=%g y-y(1,3)=%g\n",
        segOnKick[0](0)-segOnKick[1](0),segOnKick[2](0)-segOnKick[3](0),
        segOnKick[0](1)-segOnKick[2](1),segOnKick[1](1)-segOnKick[3](1));
#endif
  realTypeClFinder=0;
  if(minAmp[2]>0 && minAmp[3]>0) {
    if(typeClFinder==2) {
      realTypeClFinder=2;
      makeSeg2PlotTp2();
    } else makeSeg2Plot();
  }
  else if(minAmp[2]>0) makeModS2Plot(2);
  else if(minAmp[3]>0) makeModS2Plot(3);
  else return 0;

  Int_t nClusters = scanPlotSeg2();
  if(realTypeClFinder==2) clearPrSegTp2(); //!!!!!!
  else clearPrMod();
  return nClusters;
}

void HMdc34ClFinderSec::clearPrMod(Int_t mod) {
  Int_t m1=(mod==-1) ? 2:mod;
  Int_t m2=(mod==-1) ? 3:mod;
  if(m1<2 || m2>3) return;
  for(Int_t m=m1;m<=m2;m++) {
    UChar_t* hPlModM=(m==2) ? prPlotSeg2->plModF:prPlotSeg2->plModS;
    if(!hPlModM) continue;
    Int_t* xMaxM=xCMax[m];
    Int_t* xMinM=xCMin[m];
    for(Int_t y=0;y<prPlotSeg2->nBinY;y++) {
      if(xMaxM[y]<0) continue;
      memset(hPlModM+xMinM[y],0,xMaxM[y]-xMinM[y]+1);
      xMinM[y]=prPlotSeg2->size;
      xMaxM[y]=-1;
    }
  }
}

void HMdc34ClFinderSec::clearPrSegTp2(void) {
  for(Int_t y=0;y<prPlotSeg2->nBinY;y++) {
    if(xMaxM2[y]<0) continue;
    memset(prPlotSeg2->plModF+xMinM2[y],0,xMaxM2[y]-xMinM2[y]+1);
    memset(prPlotSeg2->plModS+xMinM2[y],0,xMaxM2[y]-xMinM2[y]+1);
    xMinM2[y]=prPlotSeg2->size;
    xMaxM2[y]=-1;
  }
}

void HMdc34ClFinderSec::makeModS2Plot(Int_t mod) {
  // Filling proj.plot for one MDC
  cPlMod=(mod==2) ? prPlotSeg2->plModF:prPlotSeg2->plModS;
  if(cPlMod==0) return;
  cFMod=&((*this)[mod]);
  cXMinM = xCMin[mod];
  cXMaxM = xCMax[mod];

  Int_t maxB4Sc = 0;
  Int_t minB4Sc = prPlotSeg2->size;
  Int_t minAm   = minAmp[mod];
  if(minAm<3) minAm=3;
  
  Char_t*  weights   = prPlotSeg2->weights;
  UChar_t* plotBArSc = prPlotSeg2->plotBArSc;

  Int_t layList[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling

  Int_t nFiredLay=0;
  for(Int_t il=0; il<6; il++) {
    Int_t lay=layList[il];
    setLayerVar(lay);
    if(!test) continue;
    nFiredLay++;
    if(maxAmp[mod]-nFiredLay+1>=minAm) {  // determination min(max)Bin[y]
      if(nFiredLay<minAm) makeLayProjV1(lay);  // filling pr.plot
      else {                                       // ...+ amp.checking
        while(test) {
          if(setNextCell()) {
            setYLimits();
            for(Int_t ny=ny1; ny<=ny2; ny++) {
              if(setXLimits(ny,prPlotSeg2->xMinL[ny],
                               prPlotSeg2->xMaxL[ny])) continue;
              if(nbF<cXMinM[ny]) cXMinM[ny]=nbF;
              if(nbL>cXMaxM[ny]) cXMaxM[ny]=nbL;
              UChar_t* bt=cPlMod+nbF;
              for(Int_t nb=nbF; nb<=nbL; nb++) {
                *bt |= add;
                Char_t wt = HMdcBArray::getNSet(bt);
                bt++;
                if( wt<minAm ) continue;
                weights[nb] = wt;
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minB4Sc) minB4Sc=nb;
                if(nb>maxB4Sc) maxB4Sc=nb;
              }
            }
            shDown=shDownN;
          }
          shUp=shUpN;
        }
      }
    } else {                         // filling in minBin[y]-maxBin[y] only
      if(nFiredLay<minAm) makeLayProjV2(lay);  // filling pr.plot
      else {                                       // ...+ amp.checking
        while(test) {
          if(setNextCell()) {
            setYLimits();
            for(Int_t ny=ny1; ny<=ny2; ny++) {
              if(cXMaxM[ny]<0) continue;
              if(setXLimits(ny,cXMinM[ny]%nBinX,cXMaxM[ny]%nBinX)) continue;
              UChar_t* bt=cPlMod+nbF;
              for(Int_t nb=nbF; nb<=nbL; nb++) {
                *bt |= add;
                Char_t wt=HMdcBArray::getNSet(bt);
                bt++;
                if( wt<minAm ) continue;
                weights[nb]=wt;
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minB4Sc) minB4Sc=nb;
                if(nb>maxB4Sc) maxB4Sc=nb;
              }
            }
            shDown=shDownN;
          }
          shUp=shUpN;
        }
      }
    }
  }
  prPlotSeg2->minBin4Sc=minB4Sc/32;
  prPlotSeg2->maxBin4Sc=maxB4Sc/32;
}

void HMdc34ClFinderSec::setLayerVar(Int_t lay) {
  // Setting of layer var. for filling proj.plot
  cFLay=&((*cFMod)[lay]);
  cell=cFLay->cells->getFirstCell();
  if(cell<0) {
    test = kFALSE;
    return;
  }
  if(cell >= cFLay->nCells) {
    Warning("makePlotSeg2","Cell %i >= num.cells(=%i)",cell,cFLay->nCells);
    test = kFALSE;
    return;
  }
  nBinYM2   = prPlotSeg2->nBinY-2;
  shUp      = Max(calcYbin(0,0,cell+1),calcYbin(0,1,cell+1));
  shDown    = Min(calcYbin(1,0,cell),calcYbin(1,1,cell));
  add       = 1<<lay;
  nYLinesM1 = cFLay->nYLines-1;
  xBin1L    = cFLay->xBin1;
  xBin2L    = cFLay->xBin2;
  shUpN     = 0;
  shDownN   = 0;
  test      = kTRUE;
  cFLay->cellShUp[cell]   = shUp;
  cFLay->cellShDown[cell] = shDown;
}

Bool_t HMdc34ClFinderSec::setNextCell(void) {
  cell=cFLay->cells->next(cell);
  if(cell>=0) {
    shDownN = Min(calcYbin(1,0,cell),calcYbin(1,1,cell));
    shUpN   = Max(calcYbin(0,0,cell+1),calcYbin(0,1,cell+1));
    cFLay->cellShUp[cell]   = shUpN;
    cFLay->cellShDown[cell] = shDownN;
    if(shDownN-shUp<=1) return kFALSE;
  } else {
    test    = kFALSE;
    shDownN = shUp+1;
  }
  return kTRUE;
}

void HMdc34ClFinderSec::setYLimits(void) {
  shDown+=cFLay->yFirst;
  shUp+=cFLay->yFirst;
  ny1=(shDown<=0) ? 1 : shDown;
  ny2=(nYLinesM1+shUp>nBinYM2) ? nBinYM2 : nYLinesM1+shUp;
}

Bool_t HMdc34ClFinderSec::setXLimits(Int_t ny, Short_t nx1,Short_t nx2) {
  // Determination and testing X region for pr.plot filling
  Int_t iy1=ny-shUp;
  Int_t iy2=ny-shDown;
  if(cFLay->wOrType>0) {
    if(iy1>=0 && xBin1L[iy1]>nx1) nx1=xBin1L[iy1];
    if(iy2<=nYLinesM1 && xBin2L[iy2]<nx2) nx2=xBin2L[iy2];
  } else {
    if(iy2<=nYLinesM1 && xBin1L[iy2]>nx1) nx1=xBin1L[iy2];
    if(iy1>=0 && xBin2L[iy1]<nx2) nx2=xBin2L[iy1];
  }
  if(nx2<nx1) return kTRUE;
  Int_t shift=ny * nBinX;
  nbF=nx1+shift;
  nbL=nx2+shift;
  return kFALSE;
}

void HMdc34ClFinderSec::makeLayProjV1(Int_t lay) {
  // plot filling and filled region determination
  while(test) {
    if(setNextCell()) {
      setYLimits();
      for(Int_t ny=ny1; ny<=ny2; ny++) {
        if(setXLimits(ny,prPlotSeg2->xMinL[ny],prPlotSeg2->xMaxL[ny])) continue;
        if(nbF<cXMinM[ny]) cXMinM[ny]=nbF;
        if(nbL>cXMaxM[ny]) cXMaxM[ny]=nbL;
        UChar_t* bin   =cPlMod+nbF;
        UChar_t* binMax=cPlMod+nbL;
        for(; bin<=binMax; bin++) *bin |= add;
      }
      shDown=shDownN;
    }
    shUp=shUpN;
  }
}

void HMdc34ClFinderSec::makeLayProjV1b(Int_t lay) {
  // plot filling and filled region determination ofMDC4 in region determined
  // in MDC3
  while(test) {
    if(setNextCell()) {
      setYLimits();
      for(Int_t ny=ny1; ny<=ny2; ny++) {
        if(xMaxM2[ny]<0) continue;
        if(setXLimits(ny,xMinM2[ny]%nBinX,xMaxM2[ny]%nBinX)) continue;
        if(nbF<cXMinM[ny]) cXMinM[ny]=nbF;
        if(nbL>cXMaxM[ny]) cXMaxM[ny]=nbL;
        UChar_t* bin   =cPlMod+nbF;
        UChar_t* binMax=cPlMod+nbL;
        for(; bin<=binMax; bin++) *bin |= add;
      }
      shDown=shDownN;
    }
    shUp=shUpN;
  }
}

void HMdc34ClFinderSec::makeLayProjV2(Int_t lay) {
  // plot filling in filled regions only
  while(test) {
    if(setNextCell()) {
      setYLimits();
      for(Int_t ny=ny1; ny<=ny2; ny++) {
        if(cXMaxM[ny]<0) continue;
        if(setXLimits(ny,cXMinM[ny]%nBinX,cXMaxM[ny]%nBinX)) continue;
        UChar_t* bin   =cPlMod+nbF;
        UChar_t* binMax=cPlMod+nbL;
        for(; bin<=binMax; bin++) *bin |= add;
      }
      shDown=shDownN;
    }
    shUp=shUpN;
  }
}

void HMdc34ClFinderSec::makeSeg2Plot(void) {
  Int_t mBm3    = minAmp[2];
  Int_t maxB4Sc = 0;
  Int_t minB4Sc = prPlotSeg2->size;
  
  Char_t*  weights   = prPlotSeg2->weights;
  UChar_t* plotBArSc = prPlotSeg2->plotBArSc;

  UChar_t* plModF=prPlotSeg2->plModF;
  Int_t layList[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling
  for(Int_t mod=2; mod<4; mod++) {
    cFMod=&((*this)[mod]);
    cXMinM=xCMin[mod];
    cXMaxM=xCMax[mod];
    Int_t minAm=minAmp[mod];
    if(minAm<3) minAm=3;
    cPlMod=(mod==2) ? prPlotSeg2->plModF:prPlotSeg2->plModS;
    if(cPlMod==0) continue;
    Int_t nFiredLay=0;
    for(Int_t il=0; il<6; il++) {
      Int_t lay=layList[il];
      setLayerVar(lay);
      if(!test) continue;
      nFiredLay++;
      if(maxAmp[mod]-nFiredLay+1>=minAm) {
        if(mod==2) makeLayProjV1(lay);
        else if(nFiredLay<minAm) makeLayProjV1b(lay);
        else {
          while(test) {
            if(setNextCell()) {
              setYLimits();
              for(Int_t ny=ny1; ny<=ny2; ny++) {
                if(setXLimits(ny,prPlotSeg2->xMinL[ny],
                                 prPlotSeg2->xMaxL[ny])) continue;
                if(nbF<cXMinM[ny]) cXMinM[ny]=nbF;
                if(nbL>cXMaxM[ny]) cXMaxM[ny]=nbL;
                UChar_t* bt=cPlMod+nbF;
                for(Int_t nb=nbF; nb<=nbL; nb++) {
                  *bt |= add;
                  Char_t wt=HMdcBArray::getNSet(bt);
                  bt++;
                  if( wt<minAm ) continue;
                  Char_t wt2=HMdcBArray::getNSet(plModF[nb]);
                  if(wt2<mBm3) continue;
                  weights[nb]=wt+wt2;
                  HMdcBArray::set(plotBArSc,nb);
                  if(nb<minB4Sc) minB4Sc=nb;
                  if(nb>maxB4Sc) maxB4Sc=nb;
                }
              }
              shDown=shDownN;
            }
            shUp=shUpN;
          }
        }
      } else {
        if(mod==2 || nFiredLay<minAm) makeLayProjV2(lay);
        else {
          while(test) {
            if(setNextCell()) {
              setYLimits();
              for(Int_t ny=ny1; ny<=ny2; ny++) {
                if(cXMaxM[ny]<0) continue;
                if(setXLimits(ny,cXMinM[ny]%nBinX,cXMaxM[ny]%nBinX)) continue;
                UChar_t* bt=cPlMod+nbF;
                for(Int_t nb=nbF; nb<=nbL; nb++) {
                  *bt |= add;
                  Char_t wt=HMdcBArray::getNSet(bt);
                  bt++;
                  if( wt<minAm ) continue;
                  Char_t wt2=HMdcBArray::getNSet(plModF[nb]);
                  if(wt2<mBm3) continue;
                  weights[nb]=wt+wt2;
                  HMdcBArray::set(plotBArSc,nb);
                  if(nb<minB4Sc) minB4Sc=nb;
                  if(nb>maxB4Sc) maxB4Sc=nb;
                }
              }
              shDown=shDownN;
            }
            shUp=shUpN;
          }
        }
      }
    }
  }
  prPlotSeg2->minBin4Sc=minB4Sc/32;
  prPlotSeg2->maxBin4Sc=maxB4Sc/32;
}


void HMdc34ClFinderSec::makeSeg2PlotTp2(void) {
  Int_t minAm=minAmp[2]+minAmp[3];
  if(minAm<3) minAm=3;
  Int_t maxAm=maxAmp[2]+maxAmp[3];
  Int_t maxB4Sc=0;
  Int_t minB4Sc=prPlotSeg2->size;
  
  Char_t* weights=prPlotSeg2->weights;
  UChar_t* plotBArSc=prPlotSeg2->plotBArSc;

  UChar_t* plModF=prPlotSeg2->plModF;
  UChar_t* plModS=prPlotSeg2->plModS;
  Int_t nFiredLay=0;
  cXMinM=xMinM2;
  cXMaxM=xMaxM2;
  Int_t layList[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling
  for(Int_t il=0; il<6; il++) {
    Int_t lay=layList[il];
    for(Int_t mod=2; mod<4; mod++) {
      cFMod=&((*this)[mod]);
      Bool_t isMod4 = mod!=2;
      cPlMod=(!isMod4) ? prPlotSeg2->plModF:prPlotSeg2->plModS;
      if(cPlMod==0) continue;
      setLayerVar(lay);
      if(!test) continue;
      nFiredLay++;
      if(maxAm-nFiredLay+1>=minAm) {
        if(nFiredLay<minAm) makeLayProjV1(lay);
        else {
          while(test) {
            if(setNextCell()) {
              setYLimits();
              for(Int_t ny=ny1; ny<=ny2; ny++) {
                if(setXLimits(ny,prPlotSeg2->xMinL[ny],
                                 prPlotSeg2->xMaxL[ny])) continue;
                if(nbF<cXMinM[ny]) cXMinM[ny]=nbF;
                if(nbL>cXMaxM[ny]) cXMaxM[ny]=nbL;
                UChar_t* bt=cPlMod+nbF;
                for(Int_t nb=nbF; nb<=nbL; nb++) {
                  *bt |= add;
                  bt++;
                  Char_t wt=HMdcBArray::getNSet(plModF[nb])+
                            HMdcBArray::getNSet(plModS[nb]);
                  if(wt<minAm ) continue;
                  weights[nb]=wt;
                  HMdcBArray::set(plotBArSc,nb);
                  if(nb<minB4Sc) minB4Sc=nb;
                  if(nb>maxB4Sc) maxB4Sc=nb;
                }
              }
              shDown=shDownN;
            }
            shUp=shUpN;
          }
        }
      } else {
        if(nFiredLay<minAm) makeLayProjV2(lay);
        else {
          while(test) {
            if(setNextCell()) {
              setYLimits();
              for(Int_t ny=ny1; ny<=ny2; ny++) {
                if(cXMaxM[ny]<0) continue;
                if(setXLimits(ny,cXMinM[ny]%nBinX,cXMaxM[ny]%nBinX)) continue;
                UChar_t* bt=cPlMod+nbF;
                for(Int_t nb=nbF; nb<=nbL; nb++) {
                  *bt |= add;
                  bt++;
                  Char_t wt=HMdcBArray::getNSet(plModF[nb])+
                            HMdcBArray::getNSet(plModS[nb]);
                  if(wt<minAm ) continue;
                  weights[nb]=wt;
                  HMdcBArray::set(plotBArSc,nb);
                  if(nb<minB4Sc) minB4Sc=nb;
                  if(nb>maxB4Sc) maxB4Sc=nb;
                }
              }
              shDown=shDownN;
            }
            shUp=shUpN;
          }
        }
      }
    }
  }
  prPlotSeg2->minBin4Sc=minB4Sc/32;
  prPlotSeg2->maxBin4Sc=maxB4Sc/32;
}

TH2C* HMdc34ClFinderSec::getPlot(Char_t* name, Char_t* title,Int_t ver) {
  for(Int_t mod=2; mod<4; mod++) {
    cPlMod=(mod==2) ? prPlotSeg2->plModF:prPlotSeg2->plModS;
    if(cPlMod==0) continue;
    cFMod=&((*this)[mod]);
    cXMinM=xCMin[mod];
    cXMaxM=xCMax[mod];
    for(Int_t lay=0; lay<6; lay++) {
      setLayerVar(lay);
      if(!test) continue;
      makeLayProjV1(lay);
    }
  }
  TH2C* plt;
  if(ver==0) plt=prPlotSeg2->getPlot(name,title);
  else plt=prPlotSeg2->getPlotV2(name,title);
  clearPrMod();
  return plt;
}

Int_t HMdc34ClFinderSec::calcYbin(Int_t upDo, Int_t leRi, Int_t cell) {
  //upDo=0 - up rib, =1 down rib
  Int_t np=cFLay->nPSegOnKick[upDo][leRi];
  Double_t xp=segOnKick[np](0);
  Double_t yp=cFLay->tgY*xp+cell*cFLay->yStep+cFLay->y0[leRi];
  Double_t zp=cFLay->tgZ*xp+cell*cFLay->zStep+cFLay->z0[leRi];
  Double_t yPrPl=(al[np]*yp-bl[np]+segOnKick[np](1)*zp)/
                 (zp+prPlotSeg2->B()*yp-cl[np]);
  return Int_t((yPrPl-prPlotSeg2->yMin)/prPlotSeg2->stY) - cFLay->yBin[nbX[np]];
}

Int_t HMdc34ClFinderSec::scanPlotSeg2(void) {
  nClsArr=0;
  locClus[1]=1;
  Bool_t useFixedLevel = HMdc34ClFinder::getUseFixedLevelFlag();
  Int_t maxB4Sc=prPlotSeg2->maxBin4Sc;
  for(Int_t n4=prPlotSeg2->minBin4Sc; n4<=maxB4Sc; n4++) {
    if(prPlotSeg2->plotBArSc4b[n4]==0) continue;
    UChar_t *b1=prPlotSeg2->plotBArSc+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t n=-1;
    if(useFixedLevel) while( (n=HMdcBArray::nextAndUnset(b1,b2,n))>=0 &&
                             calcClusterSeg2FixedLevel(nBin4+n) );
    else              while( (n=HMdcBArray::nextAndUnset(b1,b2,n))>=0 &&
                              calcClusterSeg2FloatLevel(nBin4+n) );
    if(nClsArr >= clusArrSize) break;
  }
  mergeClustSeg2();
  return fillClusCat();
}

Int_t HMdc34ClFinderSec::fillClusCat(void) {
  // Filling containers:
  Int_t nClusters      = 0;
  Bool_t isFirst       = isGeant;
  Bool_t useFixedLevel = HMdc34ClFinder::getUseFixedLevelFlag();
  for(Int_t cl=0; cl<nClsArr; cl++) if(clusArr[cl].status) {
    if(isFirst) {
      HMdcGeantEvent* pGEvent = HMdcGeantEvent::getExObject();
      if(pGEvent) pGEvent->clearOSegClus();
      isFirst = kFALSE;
    }
    HMdcCluster& cluster = clusArr[cl];
    cluster.calcClusParam(prPlotSeg2->xHStep2,prPlotSeg2->yHStep2);
    locClus[2]=counter;
    HMdcClus* clus = (HMdcClus*)fClusCat->getSlot(locClus);
    if(!clus) {
      Warning("fillClusCat","S.%i No slot HMdcClus available!",sector+1);
      return cl-1;
    }
    if(isGeant) clus = (HMdcClus*)(new(clus) HMdcClusSim(cluster.lCells2));
    else        clus = new(clus) HMdcClus(cluster.lCells2);
    indexLCh=fClusCat->getIndex(clus);
    if(indexFCh<0) indexFCh=indexLCh;
    counter++;
    nClusters++;
    Int_t nLayM1=clus->getNLayersMod(0);
    Int_t nLayM2=clus->getNLayersMod(1);
    clus->setAddress(locClus);
    if(realTypeClFinder==2 && (nLayM1<minAmp[2] || nLayM2<minAmp[3])) {
      clus->setMod( (nLayM1>=nLayM2) ? 2:3);
      if(useFixedLevel) clus->setTypeClFinder(2);
      else              clus->setTypeClFinder(128+2);
    } else {
      clus->setMod((mSeg[1]==3) ? -2 : (mSeg[1]+1) );  // mSeg[1]+1=mSeg[1]-1+2
      if(useFixedLevel) clus->setTypeClFinder(0);
      else              clus->setTypeClFinder(128);
    }
    clus->setMinCl(minAmp[2],minAmp[3]);
    clus->setPrPlane(prPlotSeg2->A(),prPlotSeg2->B(),prPlotSeg2->D());
    clus->setTarg(segOnKick[4]);
    clus->setErrTarg(dirSeg1[4]);
    clus->setIndexParent(indexPar);
    cluster.fillClus(clus,1,isGeant); // ",1," - outer segment
  }
  return nClusters;
}

void HMdc34ClFinderSec::mergeClustSeg2(void) {
  if(!mergeFlag) return;
  Int_t nClus=nClsArr;
  Int_t mergeLevM3 = (minAmp[2]>4) ? 4:3; //???
  Int_t mergeLevM4 = (minAmp[3]>4) ? 4:3; //???
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      HMdcCluster& cls1=clusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& cLCSeg2 = cls1.lCells2;
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        HMdcCluster& cls2=clusArr[cl2];
        if(!cls2.status) continue;
        HMdcList12GroupCells* cLCSeg2s = &(cls2.lCells2);
        Float_t dY=cls1.y-cls2.y;
//???optimizaciya???        if(dY>100.) break;
        if(Abs(dY) > 30.) continue;                     //  30. mm !???
        if(Abs(cls1.x-cls2.x) > 100.) continue;         // 100. mm !???
        if(realTypeClFinder==2) { // realTypeClFinder==2 when typeClFinder==2 +...!
          if(/*typeClFinder==2 &&*/ cLCSeg2.compare(cLCSeg2s,0,11)<6) continue;
        } else {
          if(minAmp[2]>0 && cLCSeg2.compare(cLCSeg2s,0, 5)<mergeLevM3) continue;
          if(minAmp[3]>0 && cLCSeg2.compare(cLCSeg2s,6,11)<mergeLevM4) continue;
        }
        cls1.addClus(cls2);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdc34ClFinderSec::initCluster(Int_t nBin,Int_t amp) {
  cluster = &(clusArr[nClsArr]);
  cluster->clear(1);   // segmet number: 1 - outer MDCs seg.
  addBinInCluster(nBin,amp);
  Int_t ny = prPlotSeg2->ybin(nBin);
  nLMinCl = nLMaxCl = ny;
  xMinClLines[ny] = xMaxClLines[ny] = prPlotSeg2->xbin(nBin);
  isClstrInited = kTRUE;
}

void HMdc34ClFinderSec::reinitCluster(Int_t nBin,Int_t amp) {
  // Use it if list of wires empty yet!
  cluster->clearBinStat();
  addBinInCluster(nBin,amp);
  Int_t ny = prPlotSeg2->ybin(nBin);
  nLMinCl = nLMaxCl = ny;
  xMinClLines[ny] = xMaxClLines[ny] = prPlotSeg2->xbin(nBin);
}

Bool_t HMdc34ClFinderSec::calcClusterSeg2FixedLevel(Int_t nBin) {
  // claster finder with fixed level of searching
  initCluster(nBin,prPlotSeg2->weights[nBin]);
  stack->init();
  do {
    for(Int_t ib=0; ib<8; ib++) {
      Int_t nb=nBin+nearbyBins[ib];
      if(prPlotSeg2->testBin(nb)) {
        stack->push(nb);
        addBinInCluster(nb,prPlotSeg2->weights[nb]);
      }
    }
  } while((nBin=stack->pop()) >= 0);
  if( fillClusterSeg2() ) return kTRUE;  // Filling of the cluster
  return kFALSE; 
}

Bool_t HMdc34ClFinderSec::calcClusterSeg2FloatLevel(Int_t nBin) {
  // claster finder with floating level of searching
  Int_t amp = prPlotSeg2->weights[nBin];
  initCluster(nBin,amp);
  stacksArr->init(12);
  while (kTRUE) {
    for(Int_t ib=0; ib<8; ib++) {
      Int_t nb=nBin+nearbyBins[ib];
      if( !prPlotSeg2->testBin(nb) ) continue;
      Int_t ampN = prPlotSeg2->weights[nb];
      stacksArr->push(nb,ampN);
      if(ampN > amp) {    // Increasing amplitude:
        if(ib<7) stacksArr->push(nBin,amp);
        amp = ampN;
        if(isClstrInited) reinitCluster(nb,amp);
        else              initCluster(nb,amp);
        break;
      } if(ampN==amp && isClstrInited) addBinInCluster(nb,amp);
    }
    Int_t ampP = stacksArr->pop(nBin);
    if(ampP == amp) continue;
    //-End of cluster region. Filling of cluster:-----------------------------
    if(isClstrInited && !fillClusterSeg2()) return kFALSE;
    if(ampP<0) break; // No more bins in stack!
    amp = ampP;
  }
  return kTRUE;
}

Bool_t HMdc34ClFinderSec::increaseClusterNum(void) {
  if(nClsArr > 0) pClustersArrs->testCluster(0,nClsArr);
  nClsArr++;
  isClstrInited = kFALSE;
  if(nClsArr < clusArrSize) return kTRUE;
  nClsArr = clusArrSize;
  Warning("increaseClusterNum"," Num.of clusters in sector %i > max\n",sector);
  prPlotSeg2->clearBitArr();
  return kFALSE;
}

void HMdc34ClFinderSec::addBinInCluster(Int_t nBin,Char_t wt) {
  Int_t nx = prPlotSeg2->xbin(nBin);
  Int_t ny = prPlotSeg2->ybin(nBin);
  cluster->addBin(prPlotSeg2->xBinsPos[nx], prPlotSeg2->yBinsPos[ny], wt);
  // At the scaning of the cluster y step(in bins) can't be > 1
  if(ny<nLMinCl) {
    nLMinCl = ny;
    xMinClLines[ny] = xMaxClLines[ny] = nx;
  } else if(ny>nLMaxCl) {
    nLMaxCl = ny;
    xMinClLines[ny] = xMaxClLines[ny] = nx;
  }
  else if(nx < xMinClLines[ny]) xMinClLines[ny] = nx;
  else if(nx > xMaxClLines[ny]) xMaxClLines[ny] = nx;
}


Bool_t HMdc34ClFinderSec::fillClusterSeg2(void) {
  // Filling of cluster:
  Int_t xMinCl = MinElement(nLMaxCl-nLMinCl+1,xMinClLines+nLMinCl);
  Int_t xMaxCl = MaxElement(nLMaxCl-nLMinCl+1,xMaxClLines+nLMinCl);

  for(Int_t mod=2; mod<4; mod++) {
    if(!lMods[mod]) continue;
    HMdc34ClFinderMod& fMod=(*this)[mod];
    Int_t startLay=(mod-2)*6;
    for(Int_t lay=0; lay<6; lay++) {
      Int_t layer=startLay+lay;
      HMdc34ClFinderLayer& fLay=fMod[lay];
      HMdcLayListCells* cells=fLay.cells;
      for(Int_t cell=cells->getFirstCell();cell>=0;cell=cells->next(cell)) {
        if( fLay.yBin[xMinCl]+fLay.cellShUp[cell]   < nLMinCl &&
            fLay.yBin[xMaxCl]+fLay.cellShUp[cell]   < nLMinCl ) continue;
        if( fLay.yBin[xMinCl]+fLay.cellShDown[cell] > nLMaxCl &&
            fLay.yBin[xMaxCl]+fLay.cellShDown[cell] > nLMaxCl ) break;
        for(Int_t ny=nLMinCl; ny<=nLMaxCl; ny++) {
//          Int_t nYbin = line+nDownLine;
          Int_t tYmin = fLay.yBin[xMinClLines[ny]]-ny;
          Int_t tYmax = fLay.yBin[xMaxClLines[ny]]-ny;
          Int_t dY1b  = tYmin+fLay.cellShUp[cell];
          Int_t dY2b  = tYmax+fLay.cellShUp[cell];
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
            // if dY1a*dY2a<=0 the line cross the cell projection
            // dY1a*dY1b<0 The line is in the cell projection
            if(dY1a*dY2a>0 && dY1a*dY1b > 0) continue;
          }
          Int_t nDeleted =
              cluster->lCells2.setTime(layer,cell,cells->getTime(cell));
	  if(nDeleted && !HMdc34ClFinder::getQuietMode()) Warning(
            "fillClusterSeg2","Too big cluster, %i cells was removed",nDeleted);
          break;
        }
      }
    }
  }
  cluster->calcXY();
  return increaseClusterNum();
}

//---------------------------------------------------------

HMdc34ClFinder* HMdc34ClFinder::fMdc34ClFinder = 0;
Bool_t          HMdc34ClFinder::quietmode      = kTRUE;
Bool_t          HMdc34ClFinder::useFixedLevel  = kTRUE;

HMdc34ClFinder::HMdc34ClFinder(const Char_t* name,const Char_t* title,
                           const Char_t* context)
              : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HMdc34ClFinderSec
  strcpy(detName,"Mdc");

  array = new TObjArray(6);
  fGetCont=HMdcGetContainers::getObject();
  if( !fGetCont ) return;
  fMdcDet       = fGetCont->getMdcDetector();
  fSpecGeomPar  = fGetCont->getSpecGeomPar();
  fSizesCells   = HMdcSizesCells::getObject();
  fKickPlane    = fGetCont->getKickPlane();
  fMdcGeomPar   = fGetCont->getMdcGeomPar();
  fMdcClusCat   = fGetCont->getCatMdcClus(kTRUE);
  xMinClLines   = 0;
  xMaxClLines   = 0;
  stack         = 0;
  stacksArr     = 0 ;
  mergeFlag     = kTRUE;
}

Bool_t HMdc34ClFinder::initContainer(HMdcEvntListCells& event) {
  if( !fMdcDet || !fSizesCells->initContainer() )  return kFALSE;
  if( !HMdcGetContainers::isInited(fSpecGeomPar) ) return kFALSE;
  if( !HMdcGetContainers::isInited(fKickPlane) )   return kFALSE;
  if( !HMdcGetContainers::isInited(fMdcGeomPar) )  return kFALSE;
  if( !status && (fSizesCells->hasChanged() || fSpecGeomPar->hasChanged() ||
      fKickPlane->hasChanged()) || fMdcGeomPar->hasChanged()) {
    changed=kTRUE;
    if(!fMdcClusCat) return kFALSE;
    Int_t *mods=fMdcDet->getModules();
    for (Int_t sec = 0; sec < 6; sec++) {
      if(!fMdcDet->isSectorActive(sec))  continue;
      if( (*array)[sec] == 0) {
        Int_t nLM2 = HMdcTrackDSet::getTrFnNLayersInMod()[sec*4+2];
        Int_t nLM3 = HMdcTrackDSet::getTrFnNLayersInMod()[sec*4+3];
        if((mods[sec*4+2]==0||nLM2==0) && (mods[sec*4+3]==0||nLM3==0)) continue;
        // HMdc34ClFinderSec object creating:
        Int_t nPrPlLines = 886;
        HMdc34ClFinderSec* secF = new HMdc34ClFinderSec(sec,320,886); //????0,0???
        (*array)[sec] = secF;
        if(!mergeFlag) secF->donotMergeClusters();
        secF->fClusCat = fMdcClusCat;
        secF->fkick    = fKickPlane;
        secF->setCellsList(event[sec]);
        if(xMinClLines == 0) xMinClLines = new Short_t [nPrPlLines];
        if(xMaxClLines == 0) xMaxClLines = new Short_t [nPrPlLines];
        secF->xMinClLines = xMinClLines;
        secF->xMaxClLines = xMaxClLines;
        if(stacksArr==0) {
          stacksArr = new HMdcClFnStacksArr(10000);
          stack     = stacksArr->getOneStack();
        }
        secF->stacksArr = stacksArr;
        secF->stack     = stack;
      }
      // initialization of container ---
      if(!calcTarget(sec))        return kFALSE;
      if(!calcProjPlaneSeg2(sec)) return kFALSE;
      if(!calcSizePlotSeg2(sec))  return kFALSE;
      if(!calcWiresProj(sec))     return kFALSE;
#if DEBUG_LEVEL>2
      (*this)[sec].prPlotSeg2->print();
#endif
      // --------------------------------
    }
    if(versions[1]<0 || versions[2]<0) versions[1]=versions[2]=0;
    else versions[2]++;
  } else changed=kFALSE;
  return kTRUE;
}

HMdc34ClFinder::~HMdc34ClFinder() {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  fMdc34ClFinder=0;
  if(xMinClLines) delete xMinClLines;
  if(xMaxClLines) delete xMaxClLines;
  xMinClLines = 0;
  xMaxClLines = 0;
  if(stacksArr) delete stacksArr;
  stacksArr=0;
}

HMdc34ClFinder* HMdc34ClFinder::getObject(void) {
  if(!fMdc34ClFinder) fMdc34ClFinder=new HMdc34ClFinder();
  return fMdc34ClFinder;
}

HMdc34ClFinder* HMdc34ClFinder::getExObject(void) {
  return fMdc34ClFinder;
}

void HMdc34ClFinder::deleteCont(void) {
  if(fMdc34ClFinder) delete fMdc34ClFinder;
}

void HMdc34ClFinder::clear(void) { // *!*
  // clears the container
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].clear();
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
  //  Double_t par=-7.; //In a future will be geted from Par.Container
  if(!fSizesCells->hasChanged()) return kTRUE;
  HMdc34ClFinderSec& fsec=(*this)[sec];
  HMdcSizesCellsSec& fSCellsSec=(*fSizesCells)[sec];
  if( !&fSCellsSec ) return kFALSE;
  Double_t par=10.; //In a future will be geted from Par.Container

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
  fsec.prPlotSeg2->setPlanePar(prPl);
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
  HMdcPlane plane;
  for(Int_t mod=0; mod<2; mod++) {
    if(!(fsec.lMods[mod])) continue;
    HGeomCompositeVolume *fVolMdc=fGetCont->getGeomCompositeVolume(mod);
    if(!fVolMdc) return kFALSE;
    HGeomVolume* fVolLayer=fVolMdc->getComponent(5); // layer 5 only
    if(!fVolLayer) return kFALSE;
    trans=fSCellsSec[mod].getSecTrans();
    plane.setPlanePar(*trans);
    for(Int_t point=0; point<4; point++) {
      HGeomVector *fpoint=fVolLayer->getPoint(point);
      if(!fpoint) return kFALSE;
      Int_t indx=(fsec.mSeg[0]==3) ? mod : 0; // for one  MDC2 in seg. only
      vect[indx][point] = *fpoint;
      vect[indx][point].setZ(0.);
      vect[indx][point] = trans->transFrom(vect[indx][point]);
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
  Double_t yMin=1.e+10;
  Double_t yMax=-1.e+10;
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
  Double_t xMaxD=xLine(vect[0][0],vect[0][1],yMin);
  Double_t xMinD=xLine(vect[0][2],vect[0][3],yMin);
  Double_t xMax=xLine(vect[0][0],vect[0][1],yMax);
  Double_t xMin=xLine(vect[0][2],vect[0][3],yMax);
  if(fsec.mSeg[1]==3) {
    Double_t xnew=xLine(vect[1][0],vect[1][1],yMin);
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
  Double_t a1 = (p1l1(1)-p2l1(1))/(p1l1(0)-p2l1(0));
  Double_t b1 = p1l1(1)-a1*p1l1(0);
  Double_t a2 = (p1l2(1)-p2l2(1))/(p1l2(0)-p2l2(0));
  Double_t b2 = p1l2(1)-a2*p1l2(0);
  Double_t x  = (b2-b1)/(a1-a2);
  Double_t y  = a2*x+b2;
  cross.setXYZ(x,y,0.);
}

Double_t HMdc34ClFinder::xLine(HGeomVector& p1,HGeomVector& p2,Double_t yi) {
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
  Int_t mod = (fsec.mSeg[0]>1) ? 1:0;
  const HGeomTransform* trans=fSCellsSec[mod].getSecTrans();
  if(!trans) return kFALSE;
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
    HMdcSizesCellsMod& fSCellsMod=fSCellsSec[mod];
    if(!&fSCellsMod) return kFALSE;
    HMdc34ClFinderMod& fMod=fsec[mod];
    for(Int_t lay=0; lay<6; lay++) {
      HMdcSizesCellsLayer& fSCellsLay=fSCellsMod[lay];
      const HGeomTransform* secsys = fSCellsLay.getSecTrans();
      Double_t catDist = fSCellsLay.getHalfCatDist();
      Double_t pitch   = fSCellsLay.getPitch();
      Double_t cosWiOr = fSCellsLay.getCosWireOr();
      Double_t wOfSet  = fSCellsLay.getWireOffSet();
      Double_t aWire   = fSCellsLay.getTanWireOr();   // y=a*x+ln[1]

      // calc. of the wire's projection
      Double_t bWire=(100.*pitch-wOfSet)/cosWiOr;  // 100.-cell n.100
      wire[0].setXYZ( 500., aWire*500.+bWire,0.);
      wire[1].setXYZ(-500.,-aWire*500.+bWire,0.);
      for(Int_t np=0; np<2; np++) {
        wire[np]=secsys->transFrom(wire[np]);
        wire[np]-=pKick;
        fsec.prPlotSeg2->calcIntersection(pKick,wire[np],point[np]);
      }
#if DEBUG_LEVEL>2
      printf("M%i L%i C%i Size of wire on the proj.plane (poin1&point2):\n",
        mod+1,lay+1,100+1);
      point[0].print();
      point[1].print();
#endif
      Double_t al      = (point[0](1)-point[1](1))/(point[0](0)-point[1](0));
      Double_t x       = (fsec.prPlotSeg2->nBinX-1) * fsec.prPlotSeg2->stX +
                         fsec.prPlotSeg2->xMin;
      Double_t yLeft   = al*(x - point[1](0))+point[1](1); // (x-x2)*a+y2
      Double_t yRight  = al*(fsec.prPlotSeg2->xMin - point[1](0))+point[1](1);
      Short_t  nYLeft  = Short_t((yLeft  - fsec.prPlotSeg2->yMin) /
                                 fsec.prPlotSeg2->stY);
      Short_t  nYRight = Short_t((yRight - fsec.prPlotSeg2->yMin) /
                                 fsec.prPlotSeg2->stY);
      Short_t  yShift  = Min(nYLeft,nYRight);
      if(Abs(nYLeft-nYRight) >= fsec.prPlotSeg2->nBinY) {
        Error("calcWiresProj",
        "S%i M%i L%i The region of Y bins of proj. wire >= nBinY(%i).",
        sec+1,mod+1,lay+1,fsec.prPlotSeg2->nBinY);
        return kFALSE;
      }
      HMdc34ClFinderLayer& fLayer=fMod[lay];
      if( !fLayer.createArrayBins(fsec.prPlotSeg2->nBinX) ) return kFALSE;
      for(Int_t nx=0; nx<fLayer.nBinX; nx++) {
        Double_t x  = nx * fsec.prPlotSeg2->stX + fsec.prPlotSeg2->xMin;
        Double_t y  = al*(x-point[1](0))+point[1](1);
        Short_t  nY = Short_t((y-fsec.prPlotSeg2->yMin) / fsec.prPlotSeg2->stY);
        fLayer.yBin[nx]=nY-yShift;
      }

      Int_t nLines   = fLayer.yBin[fLayer.nBinX - 1]-fLayer.yBin[0];
      fLayer.nYLines = Abs(nLines)+1;
      fLayer.wOrType = (nLines>=0) ? 1:-1;
      fLayer.yFirst  = (nLines>=0) ? fLayer.yBin[0]:fLayer.yBin[fLayer.nBinX-1];
      if(fLayer.xBin1) {
        delete [] fLayer.xBin1;
        delete [] fLayer.xBin2;
      }
      fLayer.xBin1 = new Short_t [fLayer.nYLines];
      fLayer.xBin2 = new Short_t [fLayer.nYLines];
      for(Int_t yb=0;yb<fLayer.nYLines;yb++) {
        Int_t yl=yb+fLayer.yFirst;
        fLayer.xBin1[yb]=-1;
        fLayer.xBin2[yb]=-1;
        for(Int_t xb=0;xb<fLayer.nBinX;xb++) {
          Int_t dy=(fLayer.wOrType>=0) ? fLayer.yBin[xb]-yl:yl-fLayer.yBin[xb];
          if(dy<0) continue;
          if(dy>0 && fLayer.xBin2[yb]>=0) break;
          if(fLayer.xBin1[yb]<0) fLayer.xBin1[yb]=xb;
          fLayer.xBin2[yb] = (dy==0) ? xb:xb-1;
        }
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
          Double_t x = ((n&1) == 0) ? 500.: -500.;
          Double_t z = (n < 2) ? -catDist : catDist;
          rib[n].setXYZ(x, aWire*x+bWire, z);
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

void HMdc34ClFinder::setCellsList(HMdcEvntListCells& event) {
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].setCellsList(event[s]);
}

void HMdc34ClFinderSec::setCellsList(HMdcSecListCells& event) {
  counter=0;
  notEnoughWrs=kTRUE;
  for(Int_t m=2;m<4;m++) {
    if((*array)[m]) (*this)[m].setCellsList(event[m]);
    maxAmp[m]=-1;
  }
}

void HMdc34ClFinderMod::setCellsList(HMdcModListCells& event) {
  for(Int_t l=0;l<6;l++) if((*array)[l]) (*this)[l].setCellsList(event[l]);
}
