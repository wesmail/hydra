using namespace std;
#include "hmdc34clfinder.h"
#include "hades.h"
#include "hmatrixcategory.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include <iostream>
#include <iomanip>
#include "hmdcgeomobj.h"
#include "hmdcgetcontainers.h"
#include "hmdccal1sim.h"
#include "hmdcclussim.h"
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
#include <stdlib.h>

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 07/02/2003 by V. Pechenov
//*-- Modified : 05/06/2002 by V. Pechenov
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
ClassImp(HMdc34ClFinderLayer)
ClassImp(HMdc34ClFinderMod)
ClassImp(HMdc34ClFinderSec)
ClassImp(HMdc34ClFinder)


Char_t* HMdcProjPlot::weights=0;
Int_t HMdcProjPlot::wtArrSize=0;

HMdcProjPlot::HMdcProjPlot(UChar_t mSeg, Int_t inBinX, Int_t inBinY) {
  nBinX=inBinX;
  nBinY=inBinY;
  size=nBinX*nBinY;
  size=(size/32 + ((size%32 > 0) ? 1:0))*32;
  xMinL=new Short_t [nBinY];
  xMaxL=new Short_t [nBinY];
  yMinL=new Short_t [nBinX];
  if(mSeg&1) plModF=new UChar_t [size];
  else plModF=0;
  if(mSeg&2) plModS=new UChar_t [size];
  else plModS=0;
  clearArr();
  sizeBAr=size/8;
  plotBArSc=new UChar_t [sizeBAr];
  plotBArSc4b=(Int_t *)plotBArSc;
  memset(plotBArSc,0,sizeBAr);
  rootPlot=0;
  rootPlotV2=0;
  if(size>wtArrSize) {
    if(weights) delete [] weights;
    weights=new Char_t [size];
    wtArrSize=size;
  }
  xBinsPos=new Double_t [nBinX];
  yBinsPos=new Double_t [nBinY];
}

void HMdcProjPlot::clearArr(void) {
  if(plModF) memset(plModF,0,size);
  if(plModS) memset(plModS,0,size);
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

  for(Int_t nx=0; nx<nBinX; nx++) {
    yMinL[nx]=nBinY;
    for(Int_t ny=nBinY-2; ny>=0; ny--) {
      if(nx>=xMinL[ny] && nx<=xMaxL[ny]) {
        if(ny==0) yMinL[nx]=1;
        else  yMinL[nx]=ny;
      } else break;
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
  nCells=((*pMdc)[sec][mod])[layer];
  cellShUp=new Short_t [nCells];
  cellShDown=new Short_t [nCells];
  yBin=0;
  nBinX=0;
  xBin1=0;
  xBin2=0;
}

HMdc34ClFinderLayer::~HMdc34ClFinderLayer() {
  if(yBin) {
    delete [] yBin;
    yBin=0;
  }
  if(xBin1) {
    delete [] xBin1;
    xBin1=0;
    delete [] xBin2;
    xBin2=0;
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
    if(yBin) delete [] yBin;
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
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdc34ClFinderMod::getNCells(void) {
  Int_t nHits=0;
  for(Int_t lay=0; lay<6; lay++) nHits+=(*this)[lay].getNCells();
  return nHits;
}

void HMdc34ClFinderMod::clear(void) {
  for(Int_t lay=0; lay<6; lay++) if((*array)[lay]) (*this)[lay].clear();
}

//----------Sector------------------------------------
HMdcClFnStack* HMdc34ClFinderSec::stack=0;
Short_t HMdc34ClFinderSec::xMinClLines[300];
Short_t HMdc34ClFinderSec::xMaxClLines[300];
HKickPlane2* HMdc34ClFinderSec::fkick=0;
HMdcList12GroupCells HMdc34ClFinderSec::cLCells2[500];
Bool_t HMdc34ClFinderSec::cStatus[500];
Short_t HMdc34ClFinderSec::cNMergedClus[500];
Double_t HMdc34ClFinderSec::cSumWt[500];
Int_t HMdc34ClFinderSec::cNBins[500];
Double_t HMdc34ClFinderSec::cX[500];
Double_t HMdc34ClFinderSec::cY[500];
Double_t HMdc34ClFinderSec::cMeanX[500];
Double_t HMdc34ClFinderSec::cMeanY[500];
Double_t HMdc34ClFinderSec::cMeanXX[500];
Double_t HMdc34ClFinderSec::cMeanYY[500];
Double_t HMdc34ClFinderSec::cMeanYX[500];
Double_t HMdc34ClFinderSec::cMeanXWt[500];
Double_t HMdc34ClFinderSec::cMeanYWt[500];
Double_t HMdc34ClFinderSec::cMeanXXWt[500];
Double_t HMdc34ClFinderSec::cMeanYYWt[500];

HMdc34ClFinderSec::HMdc34ClFinderSec(Int_t sec, Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdc34ClFinderMod
  //
  sector=sec;
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  lMods=fMdcDet->getModules()+sector*4;
  mSeg[0]=((lMods[0]) ? 1:0) + ((lMods[1]) ? 2:0);
  mSeg[1]=((lMods[2]) ? 1:0) + ((lMods[3]) ? 2:0);
  array = new TObjArray(4);
  typeClFinder=0;
  for (Int_t mod = 2; mod < 4; mod++) {
    if(lMods[mod]) (*array)[mod] = new HMdc34ClFinderMod(sector,mod);
  }
  prPlotSeg2=0;
  if(mSeg[1]) {
    prPlotSeg2=new HMdcProjPlot(mSeg[1],inBinX,inBinY);
    nBinX=prPlotSeg2->nBinX;
    if(stack==0) stack=new HMdcClFnStack(1000);
    nearbyBins[0]=-1;
    nearbyBins[1]=+1;
    nearbyBins[2]=-nBinX;
    nearbyBins[3]=+nBinX;
    nearbyBins[4]=-1-nBinX;
    nearbyBins[5]=-1+nBinX;
    nearbyBins[6]=1-nBinX;
    nearbyBins[7]=1+nBinX;
  }
  isGeant = HMdcGetContainers::isGeant();
  locClus.set(3,sector,0,0);
  radToDeg=180./acos(Double_t(-1.));
  for(Int_t mod=2;mod<4;mod++) {
    if(lMods[mod]) {
      xCMin[mod]=new Int_t [prPlotSeg2->nBinY];
      xCMax[mod]=new Int_t [prPlotSeg2->nBinY];
      for(Int_t y=0;y<prPlotSeg2->nBinY;y++) {
        xCMin[mod][y]=prPlotSeg2->size;
        xCMax[mod][y]=-1;
      }
    } else {
      xCMin[mod]=0;
      xCMax[mod]=0;
    }
  }
  xMinM2=xCMin[2];
  xMaxM2=xCMax[2];
}

HMdc34ClFinderSec::~HMdc34ClFinderSec() {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  if(prPlotSeg2) {
    delete prPlotSeg2;
    prPlotSeg2=0;
  }
  if(stack) delete stack;
  stack=0;
  for(Int_t mod=2; mod<4; mod++)
  {
      if(xCMin[mod]) {
	  delete [] xCMin[mod];
	  xCMin[mod]=0;
      }
      if(xCMax[mod]) {
	  delete [] xCMax[mod];
	  xCMax[mod]=0;
      }
  }
}

void HMdc34ClFinderSec::clear(void) {
  counter=0;
  for(Int_t mod=2; mod<4; mod++) {
    if((*array)[mod]) (*this)[mod].clear();
    maxAmp[mod]=-1;
  }
  notEnoughWrs=kTRUE;
}

void HMdc34ClFinderSec::setMinBin(Int_t *mBin) {
  for(int i=0;i<4;i++) minAmp[i]=mBin[i];
  notEnoughWrs=testMaxAmp();
}

Bool_t HMdc34ClFinderSec::testMaxAmp(void) {
  // Calculating checking max.amplitude
  for(Int_t mod=2;mod<4;mod++) {
    maxAmp[mod]=0;
    if(minAmp[mod]<=0) continue;
    HMdc34ClFinderMod& fMod=(*this)[mod];
    for(Int_t lay=0;lay<6;lay++) {
      HMdc34ClFinderLayer& fLay=fMod[lay];
      if(fLay.getFirstCell()>=0) maxAmp[mod]++;
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
  Double_t cosPhiS=cos(phi+TMath::Pi()*0.5);
  Double_t sinPhiS=sin(phi+TMath::Pi()*0.5);
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
  indexPar=fClusCat->getIndex(fClus);
  indexFCh=-1;
  indexLCh=-2;
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

  Int_t nClusters=scanPlotSeg2();
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
  cXMinM=xCMin[mod];
  cXMaxM=xCMax[mod];

  Int_t maxB4Sc=0;
  Int_t minB4Sc=prPlotSeg2->size;
  Int_t minAm=minAmp[mod];
  if(minAm<3) minAm=3;
  
  Char_t* weights=prPlotSeg2->weights;
  UChar_t* plotBArSc=prPlotSeg2->plotBArSc;

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
  cell=cFLay->getFirstCell();
  if(cell<0) {
    test=kFALSE;
    return;
  }
  if(cell >= cFLay->nCells) {
    Warning("makePlotSeg2","Cell %i >= num.cells(=%i)",cell,cFLay->nCells);
    test=kFALSE;
    return;
  }
  nBinYM2=prPlotSeg2->nBinY-2;
  shUp=TMath::Max(calcYbin(0,0,cell+1),calcYbin(0,1,cell+1));
  shDown=TMath::Min(calcYbin(1,0,cell),calcYbin(1,1,cell));
  add=1<<lay;
  cFLay->cellShUp[cell]=shUp;
  cFLay->cellShDown[cell]=shDown;
  nYLinesM1=cFLay->nYLines-1;
  xBin1L=cFLay->xBin1;
  xBin2L=cFLay->xBin2;
  shUpN=0;
  shDownN=0;
  test=kTRUE;
}

Bool_t HMdc34ClFinderSec::setNextCell(void) {
  cell=cFLay->next(cell);
  if(cell>=0) {
    shDownN=TMath::Min(calcYbin(1,0,cell),calcYbin(1,1,cell));
    shUpN=TMath::Max(calcYbin(0,0,cell+1),calcYbin(0,1,cell+1));
    cFLay->cellShUp[cell]=shUpN;
    cFLay->cellShDown[cell]=shDownN;
    if(shDownN-shUp<=1) return kFALSE;
  } else {
    test=kFALSE;
    shDownN=shUp+1;
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
  Int_t mBm3=minAmp[2];
  Int_t maxB4Sc=0;
  Int_t minB4Sc=prPlotSeg2->size;
  
  Char_t* weights=prPlotSeg2->weights;
  UChar_t* plotBArSc=prPlotSeg2->plotBArSc;

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
      Bool_t isMod4=(mod==2) ? kFALSE:kTRUE;
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
  Int_t nClusters=0;
  nClsArr=0;
  locClus[1]=1;
  Int_t maxB4Sc=prPlotSeg2->maxBin4Sc;
  for(Int_t n4=prPlotSeg2->minBin4Sc; n4<=maxB4Sc; n4++) {
    if(prPlotSeg2->plotBArSc4b[n4]==0) continue;
    UChar_t *b1=prPlotSeg2->plotBArSc+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t n=-1;
    while((n=HMdcBArray::nextAndUnset(b1,b2,n))>=0) {
      calcClusterSeg2(nBin4+n);
      nClsArr++;
      if(nClsArr >= 500) break;
    }
    if(nClsArr >= 500) {
      Warning("scanPlotSeg2"," Num. of clusters in sector %i > 500\n",sector);
      memset(prPlotSeg2->plotBArSc,0,prPlotSeg2->sizeBAr);
      break;
    }
  }
  Int_t nClus=nClsArr;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      if(!cStatus[cl1]) continue;
      HMdcList12GroupCells& cLCSeg2=cLCells2[cl1];
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        if(!cStatus[cl2]) continue;
        HMdcList12GroupCells* cLCSeg2s=&(cLCells2[cl2]);
        Float_t dY=cY[cl1]-cY[cl2];
        if(dY>100.) break;
        if(fabs(dY) > 30.) continue;                     //  30. mm !???
        if(fabs(cX[cl1]-cX[cl2]) > 100.) continue;       // 100. mm !???
        if(realTypeClFinder==2) { // realTypeClFinder==2 when typeClFinder==2 +...!
          if(/*typeClFinder==2 &&*/ cLCSeg2.compare(cLCSeg2s,0,11)<6) continue;
        } else {
          Int_t lev=(minAmp[2]>4) ? 4:3; //???
          if(minAmp[2]>0 && cLCSeg2.compare(cLCSeg2s,0, 5)<lev) continue;
          lev=(minAmp[3]>4) ? 4:3; //???
          if(minAmp[3]>0 && cLCSeg2.compare(cLCSeg2s,6,11)<lev) continue;
        }
        cLCSeg2.add(cLCSeg2s);
        cStatus[cl2]=kFALSE;
        cSumWt[cl1] += cSumWt[cl2];
        cNBins[cl1] += cNBins[cl2];
        cMeanX[cl1] += cMeanX[cl2];
        cMeanY[cl1] += cMeanY[cl2];
        cMeanXX[cl1] += cMeanXX[cl2];
        cMeanYY[cl1] += cMeanYY[cl2];
        cMeanYX[cl1] += cMeanYX[cl2];
        cMeanXWt[cl1] += cMeanXWt[cl2];
        cMeanYWt[cl1] += cMeanYWt[cl2];
        cMeanXXWt[cl1] += cMeanXXWt[cl2];
        cMeanYYWt[cl1] += cMeanYYWt[cl2];
        cNMergedClus[cl1] += cNMergedClus[cl2];
        cX[cl1]=cMeanXWt[cl1]/cSumWt[cl1];
        cY[cl1]=cMeanYWt[cl1]/cSumWt[cl1];
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }

  // Filling containers:
  for(Int_t cl=0; cl<nClsArr; cl++) {
    if(!cStatus[cl]) continue;
    HMdcList12GroupCells& cLCSeg2=cLCells2[cl];
    Float_t errX=sqrt(cMeanXXWt[cl]/cSumWt[cl]-cX[cl]*cX[cl] +
        prPlotSeg2->stX*prPlotSeg2->stX*0.25);
    Float_t errY=sqrt(cMeanYYWt[cl]/cSumWt[cl]-cY[cl]*cY[cl] +
        prPlotSeg2->stY*prPlotSeg2->stY*0.25);
    // Cluster shape:-------------------------------------------------
    //  eXX,eYY,eXY=eYX - covariance matrix componets
    //
    //    | eXX-sigma   aXY       |           | E1 |
    // A= |                       |         B=|    |
    //    | eYX         eYY-sigma |           | E2 |
    //
    // A^2=0;  ==> sigma1 & sigma2  - (sigma1 > sigma2)
    //
    //    | 0 |        | E1 |
    // AB=|   |;   ==> |    | - direction of main axis
    //    | 0 |        | E2 |
    Double_t sigma1,sigma2,alpha;
    if(cNBins[cl]<2) sigma1=sigma2=alpha=0.;
    else {
      Double_t nBn=cNBins[cl];
      Double_t eXX = cMeanXX[cl]*nBn- cMeanX[cl]*cMeanX[cl];
      Double_t eYX = cMeanYX[cl]*nBn- cMeanY[cl]*cMeanX[cl];
      Double_t eYY = cMeanYY[cl]*nBn- cMeanY[cl]*cMeanY[cl];
      Double_t norm = (eXX+eYY)/2.;
      Double_t eYX2 = eYX*eYX;
      Double_t c=eXX*eYY-eYX2;
      sigma1=norm+sqrt(norm*norm-c);
      sigma2=norm-sqrt(norm*norm-c);
      if(sigma2<0.) sigma2=0.;
      Double_t e1=sigma1-eXX;
      Double_t e2=sigma1-eYY;
      sigma1=sqrt(sigma1)/nBn;
      sigma2=sqrt(sigma2)/nBn;
      alpha=atan2(sqrt(eYX2+e1*e1),sqrt(eYX2+e2*e2))*radToDeg;
      if(eYX<0.) alpha=180.-alpha;
    }
    locClus[2]=counter;
    HMdcClus* clus = (HMdcClus*)fClusCat->getSlot(locClus);
    if(!clus) {
      Warning("fillClusCut2","S.%i No slot HMdcClus available. Size of catMdcClus is %i !",sector+1,fClusCat->getEntries());
      return cl-1;
    }
    if(isGeant) clus=(HMdcClus*)(new(clus) HMdcClusSim(cLCSeg2));
    else clus=new(clus) HMdcClus(cLCSeg2);
    indexLCh=fClusCat->getIndex(clus);
    if(indexFCh<0) indexFCh=indexLCh;
    counter++;
    nClusters++;
    Int_t nLayM1=clus->getNLayersMod(0);
    Int_t nLayM2=clus->getNLayersMod(1);
    clus->setAddress(locClus);
    if(realTypeClFinder==2 && (nLayM1<minAmp[2] || nLayM2<minAmp[3])) {
      clus->setMod( (nLayM1>=nLayM2) ? 2:3);
      clus->setTypeClFinder(2); 
    } else {
      clus->setMod((mSeg[1]==3) ? -2 : (mSeg[1]+1) );  // mSeg[1]+1=mSeg[1]-1+2
      clus->setTypeClFinder(0);
    }
    clus->setMinCl(minAmp[2],minAmp[3]);
    clus->setPrPlane(prPlotSeg2->A(),prPlotSeg2->B(),prPlotSeg2->D());
    clus->setTarg(segOnKick[4]);
    clus->setErrTarg(dirSeg1[4]);
    clus->setSumWt(cSumWt[cl]);
    clus->setNBins(cNBins[cl]);
    clus->setXY(cX[cl],errX,cY[cl],errY);
    clus->setNMergClust(cNMergedClus[cl]);

    Int_t nDrTmM1=clus->getNDrTimes(0,5);
    Int_t nDrTmM2=clus->getNDrTimes(6,11);
    if(nDrTmM1>0) {
      clus->setClusSizeM1(cNBins[cl]);
      clus->setNDrTimesM1(nDrTmM1);
      clus->setNMergClustM1(cNMergedClus[cl]);
      clus->setShapeM1(sigma1,sigma2,alpha);
    } else clus->clearMod1Par();
    if(nDrTmM2>0) {
      clus->setClusSizeM2(cNBins[cl]);
      clus->setNDrTimesM2(nDrTmM2);
      clus->setNMergClustM2(cNMergedClus[cl]);
      clus->setShapeM2(sigma1,sigma2,alpha);
    } else clus->clearMod2Par();

    clus->setIndexParent(indexPar);
    if(clus->isGeant()) {
      ((HMdcClusSim*)clus)->calcTrList();
      ((HMdcClusSim*)clus)->calcTrListMod(cLCSeg2,0);
      ((HMdcClusSim*)clus)->calcTrListMod(cLCSeg2,1);
    }
  }
  return nClusters;
}

void HMdc34ClFinderSec::calcClusterSeg2(Int_t nBinF) {
  Int_t nBin=nBinF;
  stack->init();
  stack->push(nBin);

  Int_t minAmpS=minAmp[2]+minAmp[3]-1;
  Int_t nLinesInCl=1;
  Int_t nDownLine=nBin/prPlotSeg2->nBinX;
  xMinClLines[0]=xMaxClLines[0]=nBin%prPlotSeg2->nBinX;
  Int_t xMinCl=xMinClLines[0];
  Int_t xMaxCl=xMinCl;

  Int_t& cNBinsI=cNBins[nClsArr];
  Double_t& cMeanXI=cMeanX[nClsArr];
  Double_t& cMeanYI=cMeanY[nClsArr];
  Double_t& cMeanXXI=cMeanXX[nClsArr];
  Double_t& cMeanYYI=cMeanYY[nClsArr];
  Double_t& cMeanYXI=cMeanYX[nClsArr];
  Double_t& cSumWtI=cSumWt[nClsArr];
  Double_t& cMeanXWtI=cMeanXWt[nClsArr];
  Double_t& cMeanYWtI=cMeanYWt[nClsArr];
  Double_t& cMeanXXWtI=cMeanXXWt[nClsArr];
  Double_t& cMeanYYWtI=cMeanYYWt[nClsArr];
  HMdcList12GroupCells& list=cLCells2[nClsArr];
  cStatus[nClsArr]=kTRUE;
  cNMergedClus[nClsArr]=1;
  list.clear();
  cNBinsI=0;
  cMeanXI=cMeanYI=cMeanXXI=cMeanYYI=cMeanYXI=0.;
  cSumWtI=cMeanXWtI=cMeanYWtI=cMeanXXWtI=cMeanYYWtI=0.;
  while((nBin=stack->pop()) >= 0) {
    Int_t nx=nBin%prPlotSeg2->nBinX;
    Int_t ny=nBin/prPlotSeg2->nBinX;
    Double_t wt=prPlotSeg2->weights[nBin]-minAmpS;
    //region of cluster ???
    Double_t x=prPlotSeg2->xBinsPos[nx];
    Double_t y=prPlotSeg2->yBinsPos[ny];
    cSumWtI += wt;
    cMeanXWtI += x*wt;
    cMeanYWtI += y*wt;
    cMeanXXWtI += x*x*wt;
    cMeanYYWtI += y*y*wt;
    cNBinsI++;
    cMeanXI += x;
    cMeanYI += y;
    cMeanXXI += x*x;
    cMeanYYI += y*y;
    cMeanYXI += y*x;

    // At the scaning of the cluster y step(in bins) can't be > 1
    Int_t nyL=ny-nDownLine;
    if(nyL>=nLinesInCl) {
      if(nyL<300) {
        xMinClLines[nyL]=xMaxClLines[nyL]=nx;
        nLinesInCl++;
      } else {
	  if(!HMdc34ClFinder::getQuietMode())
	  {
	      Warning("calcClusterSeg2","Sec.%i The cluster size is very big!",
		      sector+1);
	  }
      }
    } else {
      if(nx<xMinClLines[nyL]) xMinClLines[nyL]=nx;
      else if(nx>xMaxClLines[nyL]) xMaxClLines[nyL]=nx;
    }
    if(nx<xMinCl) xMinCl=nx;
    else if(nx>xMaxCl) xMaxCl=nx;
    for(Int_t ib=0; ib<8; ib++) {
      Int_t nBinTs=nBin+nearbyBins[ib];
      if( HMdcBArray::testAndUnset(prPlotSeg2->plotBArSc,nBinTs) ) stack->push(nBinTs);
    }
    stack->checkSize(); // If stack overflowed one will increased on 50%:
  }
  // Filling of cluster:
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
            // if dY1a*dY2a<=0 the line cross the cell projection
            // dY1a*dY1b<0 The line is in the cell projection
            if(dY1a*dY2a>0 && dY1a*dY1b > 0) continue;
          }
          Int_t nDeleted=list.setTime(layer,cell,fLay.getTime(cell));
	  if(nDeleted&&!HMdc34ClFinder::getQuietMode())
	  {
	      Warning("calcClusterSeg2",
		      "%i cells was removed (size of cluster is very big)",
		      nDeleted);
	  }
          break;
        }
      }
    }
  }
  cX[nClsArr]=cMeanXWtI/cSumWtI;
  cY[nClsArr]=cMeanYWtI/cSumWtI;
}

void HMdc34ClFinderSec::printClusters(void){
// clusters.print();
}


//---------------------------------------------------------
HMdc34ClFinder* HMdc34ClFinder::fMdc34ClFinder=0;
Bool_t HMdc34ClFinder::quietmode=kTRUE;

HMdc34ClFinder::HMdc34ClFinder(const char* name,const char* title,
                           const char* context)
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
  fMdcClusCat = HMdcGetContainers::getCatMdcClus(kTRUE);
}

Bool_t HMdc34ClFinder::initContainer(void) {
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

void HMdc34ClFinder::printClusters(Int_t sec){
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
    HGeomVolume* fVolLayer=fVolMdc->getComponent(5);
    if(!fVolLayer) return kFALSE;
    trans=fSCellsSec[mod].getSecTrans();
    plane.setPlanePar(*trans);
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
      printf("M%i L%i C%i Size of wire on the proj.plane (poin1&point2):\n",
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
      for(Int_t nx=0; nx<fLayer.nBinX; nx++) {
        Float_t x=nx * fsec.prPlotSeg2->stX + fsec.prPlotSeg2->xMin;
        Float_t y=al*(x-point[1](0))+point[1](1);
        Short_t nY=Short_t((y - fsec.prPlotSeg2->yMin) / fsec.prPlotSeg2->stY);
        fLayer.yBin[nx]=nY-yShift;
      }

      Int_t nLines=fLayer.yBin[fLayer.nBinX - 1]-fLayer.yBin[0];
      fLayer.nYLines=abs(nLines)+1;
      fLayer.wOrType=(nLines>=0) ? 1:-1;
      fLayer.yFirst=(nLines>=0) ? fLayer.yBin[0]:fLayer.yBin[fLayer.nBinX-1];
      if(fLayer.xBin1) {
        delete [] fLayer.xBin1;
        delete [] fLayer.xBin2;
      }
      fLayer.xBin1=new Short_t [fLayer.nYLines];
      fLayer.xBin2=new Short_t [fLayer.nYLines];
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
