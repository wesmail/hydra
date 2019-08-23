using namespace std;
#include "hmdclookuptb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hmdcgetcontainers.h"
#include <iostream>
#include <iomanip>
#include "hmdcgeomstruct.h"
#include "hspecgeompar.h"
#include "hmdcgeompar.h"
#include "hmdcsizescells.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdclayergeompar.h"
#include "TObjArray.h"
#include "TH2.h"
#include "hmdcclfnstack.h"
#include "hmdccluster.h"
#include "hmdcclussim.h"
#include "hmdctrackdset.h"
#include "hmdclistgroupcells.h"
#include "hmdclistcells.h"

//*-- AUTHOR : Pechenov Vladimir
//*-- Modified : 21/02/2008 by V. Pechenov
//*-- Modified : 17/07/2003 by V. Pechenov
//*-- Modified : 05/02/2003 by V. Pechenov
//*-- Modified : 04/06/2002 by V.Pechenov
//*-- Modified : 09/05/2001 by V.Pechenov
//*-- Modified : 12/07/2000 by V.Pechenov
//*-- Modified : 23/05/2000 by V.Pechenov
//*-- Modified : 07/03/2000 by R. Holzmann
//*-- Modified : 02/12/99 by V.Pechenov
//*-- Modified : 26/10/99 by V.Pechenov
//*-- Modified : 20/05/99

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
// HMdcLookUpTb
//
// Trackfinder for MDC1&2 and MDC3&4 if magnet off
//
////////////////////////////////////////////////////////////////

ClassImp(HMdcLookUpTbCell)
ClassImp(HMdcLookUpTbLayer)
ClassImp(HMdcLookUpTbMod)
ClassImp(HMdcLookUpTbSec)
ClassImp(HMdcLookUpTb)

HMdcLookUpTbCell::HMdcLookUpTbCell(void) {
  nLines=0;
  xBinMin=xBinMax=0;
}

void HMdcLookUpTbCell::init(Int_t yBinMinT, Int_t yBinMaxT) {
  Int_t newNLines=yBinMaxT-yBinMinT+1;
  if(newNLines>nLines) {
    if(xBinMin) delete [] xBinMin;
    if(xBinMax) delete [] xBinMax;
    nLines=newNLines;
    xBinMin=new Short_t [nLines];
    xBinMax=new Short_t [nLines];
  }
  yBinMin=yBinMinT;
  yBinMax=yBinMaxT;
  line=0;
}

HMdcLookUpTbCell::~HMdcLookUpTbCell(void) {
  if(xBinMin) delete [] xBinMin;
  if(xBinMax) delete [] xBinMax;
}

Bool_t HMdcLookUpTbCell::addLine(Short_t nc1, Short_t nc2){
  if(nc1<=nc2 && line>=0 && line<nLines) {
    xBinMin[line]=nc1;
    xBinMax[line]=nc2;
    line++;
    return kTRUE;
  }
  Error("addLine","line number=%i > max.=%i  OR  nBin1=%i > nBin2=%i",
      line,nLines,nc1,nc2);
  return kFALSE;
}

//----------Layer-----------------------------

HMdcLookUpTbLayer::HMdcLookUpTbLayer(Int_t sec, Int_t mod, Int_t layer) {
  // Geting of pointers to par. cont.
  HMdcGetContainers* parCont=HMdcGetContainers::getObject();
  if( !parCont ) return;
  HMdcGeomStruct* fMdcGeomStruct=parCont->getMdcGeomStruct();
  if( !fMdcGeomStruct ) return;
  nCells=((*fMdcGeomStruct)[sec][mod])[layer];
  array = new TObjArray(nCells);
  for(Int_t cell=0; cell<nCells; cell++) (*array)[cell]=new HMdcLookUpTbCell();
}

HMdcLookUpTbLayer::~HMdcLookUpTbLayer(void) {
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcLookUpTbLayer::getSize(void) {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//------------Module----------------------------
HMdcLookUpTbMod::HMdcLookUpTbMod(Int_t sec, Int_t mod) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  array = new TObjArray(6);
  for (Int_t layer = 0; layer < 6; layer++)
    (*array)[layer] = new HMdcLookUpTbLayer(sec, mod, layer);
  nLayers=6;
}

HMdcLookUpTbMod::~HMdcLookUpTbMod(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
}

Int_t HMdcLookUpTbMod::getSize(void) {
  // returns the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//----------Sector------------------------------------
Int_t              HMdcLookUpTbSec::hPlModsSize  = 0;
UChar_t*           HMdcLookUpTbSec::hPlMod[4]    = {0,0,0,0};
Int_t              HMdcLookUpTbSec::sizeBArSt    = 0;
UChar_t*           HMdcLookUpTbSec::plotBArSc    = 0;
UChar_t*           HMdcLookUpTbSec::plotBArM[4]  = {0,0,0,0};
Short_t*           HMdcLookUpTbSec::clusIndM1    = 0;
Int_t              HMdcLookUpTbSec::clIndArrSzM1 = 0;
Short_t*           HMdcLookUpTbSec::clusIndM2    = 0;
Int_t              HMdcLookUpTbSec::clIndArrSzM2 = 0;

HMdcLookUpTbSec::HMdcLookUpTbSec(Int_t sec, Int_t nSegs,
                                 Int_t inBinX, Int_t inBinY) {
  // constructor creates an array of pointers of type HMdcLookUpTbMod
  sector    = sec;
  nSegments = nSegs;
  nModules  = nSegments*2;
  segment   = nSegments==1 ? 0 : -1;
  hist      = 0;
  nBinX     = (inBinX%2 == 0) ? inBinX:inBinX+1;
  nBinY     = (inBinY%2 == 0) ? inBinY:inBinY+1;
  xBinsPos  = new Double_t [nBinX];
  yBinsPos  = new Double_t [nBinY];
  size      = nBinX*nBinY;
  size      = (size/32 + ((size%32 > 0) ? 1:0))*32;
  mergeFlag = kTRUE;
  
  Bool_t resize = size > hPlModsSize;
  if(resize) hPlModsSize=size;
  array = new TObjArray(4);
  HMdcDetector* fMdcDet= HMdcGetContainers::getObject()->getMdcDetector();
  const Int_t* nLM=HMdcTrackDSet::getTrFnNLayersInMod()+sector*4;
  for (Int_t mod = 0; mod < nSegs*2; mod++) {
    if(fMdcDet->getModule(sec,mod) && nLM[mod]>0) {
      HMdcLookUpTbMod* pMod=new HMdcLookUpTbMod(sec,mod);
      (*array)[mod]=pMod;
      pMod->setNLayers(nLM[mod]);
    }
    if(hPlMod[mod] && !resize) continue;
    if(hPlMod[mod])  delete [] hPlMod[mod];
    hPlMod[mod]=new UChar_t [hPlModsSize];
    memset(hPlMod[mod],0,hPlModsSize);
  }

  if(plotBArSc==0 || sizeBArSt<size/8) {
    if(plotBArSc) delete [] plotBArSc;
    for(Int_t mod=0;mod<4;mod++) if(plotBArM[mod]) {
      delete [] plotBArM[mod];
      plotBArM[mod] = 0;
    }
    sizeBArSt=size/8;
    plotBArSc=new UChar_t [sizeBArSt];
    memset(plotBArSc,0,sizeBArSt);
    for(Int_t mod=0;mod<nSegs*2;mod++) {
      plotBArM[mod]=new UChar_t [sizeBArSt];
      memset(plotBArM[mod],0,sizeBArSt);
    }
  }
  sizeBAr       = size/8;
  typeClFinder  = 0;
  neighbBins[0] = -1;
  neighbBins[1] = +1;
  neighbBins[2] = -nBinX;
  neighbBins[3] = +nBinX;
  neighbBins[4] = -1-nBinX;
  neighbBins[5] = -1+nBinX;
  neighbBins[6] = 1-nBinX;
  neighbBins[7] = 1+nBinX;
  isGeant = HMdcGetContainers::isGeant();
  fillTrackList = isGeant;
  locClus.set(3,sec,0,0);
  pClustersArrs = HMdcClustersArrs::getObject();
  clusArrSize   = pClustersArrs->createAllArrays(500); // Clusters array size!
  clusArr       = pClustersArrs->getArray1();
  clusArrM1     = pClustersArrs->getArray2();
  clusArrM2     = pClustersArrs->getArray3();
  if(size>clIndArrSzM1) {
    if(clusIndM1) delete [] clusIndM1;
    clusIndM1    = new Short_t [size];
    clIndArrSzM1 = size;
  }
  if(size>clIndArrSzM2) {
    if(clusIndM2) delete [] clusIndM2;
    clusIndM2    = new Short_t [size];
    clIndArrSzM2 = size;
  }
  for(Int_t mod=0;mod<4;mod++) {
    xMin[mod] = new Int_t [nBinY];
    xMax[mod] = new Int_t [nBinY];
    for(Int_t y=0;y<nBinY;y++) {
      xMin[mod][y] = size;
      xMax[mod][y] = -1;
    }
  }
  xMaxCl = new Short_t [nBinY];
  xMinCl = new Short_t [nBinY];
  
  HMdcEvntListCells* pEvLCells = HMdcEvntListCells::getExObject();
  pListCells = &((*pEvLCells)[sector]);
}

HMdcLookUpTbSec::~HMdcLookUpTbSec(void) {
  // destructor
  if(array) {
    array->Delete();
    delete array;
  }
  for(Int_t mod=0; mod<4; mod++) {
    if(hPlMod[mod]) delete [] hPlMod[mod];
    hPlMod[mod]=0;
  }
  hPlModsSize=0;
  if(hist && hist->IsOnHeap()) delete hist;
  hist=0;
  if(plotBArSc) {
    delete [] plotBArSc;
    plotBArSc=0;
    sizeBArSt=0;
  }
  for(Int_t mod=0;mod<4;mod++) {
    if(plotBArM[mod]) {
      delete [] plotBArM[mod];
      plotBArM[mod]=0;
    }
  }
  delete [] xBinsPos;
  delete [] yBinsPos;
  for(Int_t mod=0;mod<4;mod++) {
    delete [] xMin[mod];
    delete [] xMax[mod];
  }

  HMdcClustersArrs::deleteCont();

  if(clusIndM1) {
    delete [] clusIndM1;
    clusIndM1=0;
    clIndArrSzM1=0;
  }
  if(clusIndM2) {
    delete [] clusIndM2;
    clusIndM2=0;
    clIndArrSzM2=0;
  }
  delete [] xMaxCl;
  delete [] xMinCl;
}

Bool_t HMdcLookUpTbSec::calcXYBounds(Double_t& xL,Double_t& xU,
                                     Double_t& yL,Double_t& yU) {
  HGeomVector senVol;
  HGeomVector cross;
  xL = yL = 1.0e+30;
  xU = yU = -1.0e+30;
  
  HMdcSizesCells *fSizesCells = HMdcSizesCells::getExObject();
  if(fSizesCells == 0) return kFALSE;
  for(Int_t mod=0; mod<4; mod++) if( (*array)[mod] ) {
    for(Int_t layer=0; layer<6; layer++ /*layer+=5*/) {
      // Take size of first and last MDC layer only
      HMdcSizesCellsLayer& fSizesCellsLay=(*fSizesCells)[sector][mod][layer];
      if( !&fSizesCells ) return kFALSE;
      HGeomVolume* fGeomVolLay = fSizesCellsLay.getGeomVolume();
      if(!fGeomVolLay || fGeomVolLay->getNumPoints()!=8) return kFALSE;
      HMdcLayerGeomParLay* pLayGmParLay = fSizesCellsLay.getLayerGeomParLay();
      if(pLayGmParLay == 0) return kFALSE;
      Double_t dstCathPl = pLayGmParLay->getCatDist()*0.5;
      for(Int_t point=0; point<8; point++) {
        senVol = *(fGeomVolLay->getPoint(point)); // mm!
        if(point<4) senVol.setZ(-dstCathPl);
        else        senVol.setZ( dstCathPl);
        senVol = fSizesCellsLay.getSecTrans()->transFrom(senVol);
        for(Int_t targ=0; targ<2; targ++) {
          prPlane.calcIntersection(targVc[targ],senVol-targVc[targ],cross);
          if(cross(0)<xL)      xL = cross(0);
          else if(cross(0)>xU) xU = cross(0);
          if(cross(1)<yL)      yL = cross(1);
          else if(cross(1)>yU) yU = cross(1);
        }
      }
    }
  }
  return kTRUE;
}

void HMdcLookUpTbSec::setPrPlotSize(Double_t xL,Double_t xU,
                                    Double_t yL,Double_t yU) {
  xLow  = xL;
  xUp   = xU;
  yLow  = yL;
  yUp   = yU;
  xStep = (xUp-xLow)/(Double_t)nBinX;
  yStep = (yUp-yLow)/(Double_t)nBinY;
  Double_t xHStep = xStep/2;
  Double_t yHStep = yStep/2;
  xHStep2 = xHStep*xHStep;
  yHStep2 = yHStep*yHStep;
  for(Int_t n=0; n<nBinX; n++) xBinsPos[n] = ((Double_t)n)*xStep+xLow+xHStep;
  for(Int_t n=0; n<nBinY; n++) yBinsPos[n] = ((Double_t)n)*yStep+yLow+yHStep;
}

void HMdcLookUpTbSec::clearPrArrs(void) {
  if(hPlMod[0]) memset(hPlMod[0],0,size);
  if(hPlMod[1]) memset(hPlMod[1],0,size);
  if(hPlMod[2]) memset(hPlMod[2],0,size);
  if(hPlMod[3]) memset(hPlMod[3],0,size);
}

void HMdcLookUpTbSec::clearPrMod(Int_t mod) {
  UChar_t* hPlModM=hPlMod[mod];
  if(!hPlModM) return;
  Int_t* xMaxM=xMax[mod];
  Int_t* xMinM=xMin[mod];
  for(Int_t y=0;y<nBinY;y++) {
    if(xMaxM[y]<0) continue;
    memset(hPlModM+xMinM[y],0,xMaxM[y]-xMinM[y]+1);
    xMinM[y]=size;
    xMaxM[y]=-1;
  }
}

Int_t HMdcLookUpTbSec::getSize(void) {
  // return the size of the pointer array
  if(array) return array->GetEntries();
  else return -1;
}

//May be numhits[i]=0 ... in another sub. ?????????????????
void HMdcLookUpTbSec::clearwk(void) {
  noFiredCells = kFALSE;
  isSlotAv[0] = kTRUE;
  isSlotAv[1] = kTRUE;
  counter[0]  = 0;
  counter[1]  = 0;
}

Int_t HMdcLookUpTbSec::xBinNumInBounds(Double_t x) {
  Int_t bin=xBinNum(x);
  if(bin < 1)            bin = 1;
  else if(bin > nBinX-2) bin = nBinX-2;
  return bin;
}

Int_t HMdcLookUpTbSec::yBinNumInBounds(Double_t y) {
  Int_t bin=yBinNum(y);
  if(bin < 1)            bin = 1;
  else if(bin > nBinY-2) bin = nBinY-2;
  return bin;
}

void HMdcLookUpTbSec::makeSPlot(void) {
  // Making proj.plot in sector(magnetoff) or segment(magneton).
  // Number of MDCs in sector or segment must be >1 !!!
  maxBinBAr4Sc=0;
  minBinBAr4Sc=size;
  UChar_t addm=1;

  Int_t lmod=-1;
  if(minAmp[3]>0) lmod=3;
  else if(minAmp[2]>0) lmod=2;
  else if(minAmp[1]>0) lmod=1;
  else if(minAmp[0]>0) lmod=0;
  if(lmod<0) return;
  Int_t fmod=3;
  if(minAmp[0]>0) fmod=0;
  else if(minAmp[1]>0) fmod=1;
  else if(minAmp[2]>0) fmod=2;
  if(fmod==lmod) return;

  Int_t lay[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling
  Char_t minAm0=minAmp[0];
  Char_t minAm1=minAmp[1];
  Char_t minAm2=minAmp[2];
  Char_t minAmL=minAmp[lmod];
  for(Int_t mod=fmod; mod<=lmod; mod++) {
    HMdcModListCells& pModLCells = (*pListCells)[mod];
    cFMod=&((*this)[mod]);
    if( !cFMod ) continue;
    if(minAmp[mod]==0) continue;
    cHPlModM=hPlMod[mod];
    if(!cHPlModM) continue;
    cXMinM=xMin[mod];
    cXMaxM=xMax[mod];

    Int_t nFiredLay=0;
    for(Int_t indLine=0; indLine<6; indLine++) {
      Int_t layer=lay[indLine];
      HMdcLayListCells& pLayLCells = pModLCells[layer];
      if(pLayLCells.getFirstCell() < 0) continue;
      nFiredLay++;
      if(maxAmp[mod]-nFiredLay+1>=minAmp[mod]) {
        if(mod==fmod) makeLayProjV1(mod,layer);
        else if(mod<lmod || nFiredLay<minAmp[mod]) makeLayProjV1b(mod,layer);
        else {
          UChar_t add=addm<<layer;
          HMdcLookUpTbLayer& fLayer    = (*cFMod)[layer];
          Int_t cell = -1;
          while( pLayLCells.nextCell(cell) ) {
//?W            if(hitsDel[indLay][n]<0) continue;
            HMdcLookUpTbCell& fCell = fLayer[cell];
            for(Int_t ln=0; ln<fCell.line; ln++) {
              Int_t y=fCell.yBinMin+ln;
              if(pXMaxM[y]<0) continue;
              Int_t shift=y * nBinX;
              Int_t nbL=fCell.xBinMax[ln]+shift;
              Int_t nbF=fCell.xBinMin[ln]+shift;
              if(nbL>pXMaxM[y]) nbL=pXMaxM[y];
              if(nbF<pXMinM[y]) nbF=pXMinM[y];
              if(nbF>nbL) continue;
              if(nbF<cXMinM[y]) cXMinM[y]=nbF;
              if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
              UChar_t* bt=cHPlModM+nbF;
              for(Int_t nb=nbF; nb<=nbL; nb++) {
                *bt |= add;
                Char_t wt=HMdcBArray::getNSet(bt);
                bt++;
                if( wt<minAmL ) continue;
                if(fmod==0&&HMdcBArray::getNSet(hPlMod[0][nb])<minAm0) continue;
                if(mod>1) {
                  if(HMdcBArray::getNSet(hPlMod[1][nb])<minAm1) continue;
                  if(mod>2&&HMdcBArray::getNSet(hPlMod[2][nb])<minAm2) continue;
                }
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
                if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
              }
            }
          }
        }
      } else {
        if(mod<lmod || nFiredLay<minAmp[mod]) makeLayProjV2(mod,layer);
        else {
          UChar_t add=addm<<layer;
          HMdcLookUpTbLayer& fLayer    = (*cFMod)[layer];
          Int_t cell = -1;
          while( pLayLCells.nextCell(cell) ) {
//?W            if(hitsDel[indLay][n]<0) continue;
            HMdcLookUpTbCell& fCell = fLayer[cell];
            for(Int_t ln=0; ln<fCell.line; ln++) {
              Int_t y=fCell.yBinMin+ln;
              if(cXMaxM[y]<0) continue;
              Int_t shift=y * nBinX;
              Int_t nbL=fCell.xBinMax[ln]+shift;
              Int_t nbF=fCell.xBinMin[ln]+shift;
              if(nbL>cXMaxM[y]) nbL=cXMaxM[y];
              if(nbF<cXMinM[y]) nbF=cXMinM[y];
              UChar_t* bt=cHPlModM+nbF;
              for(Int_t nb=nbF; nb<=nbL; nb++) {
                *bt |= add;
                Char_t wt=HMdcBArray::getNSet(bt);
                bt++;
                if( wt<minAmL ) continue;
                if(fmod==0&&HMdcBArray::getNSet(hPlMod[0][nb])<minAm0) continue;
                if(mod>1) {
                  if(HMdcBArray::getNSet(hPlMod[1][nb])<minAm1) continue;
                  if(mod>2&&HMdcBArray::getNSet(hPlMod[2][nb])<minAm2) continue;
                }
                HMdcBArray::set(plotBArSc,nb);
                if(nb<minBinBAr4Sc) minBinBAr4Sc=nb;
                if(nb>maxBinBAr4Sc) maxBinBAr4Sc=nb;
              }
            }
          }
        }
      }
    }
    pXMinM=cXMinM;
    pXMaxM=cXMaxM;
  }
  maxBinBAr4Sc/=32;
  minBinBAr4Sc/=32;
}

void HMdcLookUpTbSec::makeModPlot(Int_t mod) {
  Int_t minAm = minAmp[mod];
  if(minAm<2) return;
  cFMod=&((*this)[mod]);
  if( !cFMod ) return;
  cHPlModM=hPlMod[mod];
  cXMinM=xMin[mod];
  cXMaxM=xMax[mod];

  Int_t lay[6]={2,3,1,4,0,5};   // order of layers at the pr.plot filling

  UChar_t* cPlotBAr=plotBArM[mod];
  Int_t& minBin=minBinBAr4M[mod];
  Int_t& maxBin=maxBinBAr4M[mod];
  maxBin=0;
  minBin=size;
  Int_t nFiredLay=0;
  HMdcModListCells& pModLCells = (*pListCells)[mod];
  for(Int_t indLine=0; indLine<6; indLine++) {
    Int_t layer=lay[indLine];
    HMdcLayListCells& pLayLCells = pModLCells[layer];
    if(pLayLCells.getFirstCell() < 0) continue;
    nFiredLay++;
    if(maxAmp[mod]-nFiredLay+1>=minAm) {  // determination min(max)Bin[y]
      if(nFiredLay<minAm) makeLayProjV1(mod,layer);  // filling pr.plot
      else {                                                // ...+ amp.checking
        // filling, min, max and bits seting
        UChar_t add=1<<layer;
        HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
        Int_t cell = -1;
        while( pLayLCells.nextCell(cell) ) {
//?W          if(hitsDel[indLay][n]<0) continue;
          HMdcLookUpTbCell& fCell = fLayer[cell];
          for(Int_t ln=0; ln<fCell.line; ln++) {
            Int_t y=fCell.yBinMin+ln;
            if(cXMaxM[y]<0) continue;
            Int_t shift=y * nBinX;
            Int_t nbL=fCell.xBinMax[ln]+shift;
            Int_t nbF=fCell.xBinMin[ln]+shift;
            if(nbF<cXMinM[y]) cXMinM[y]=nbF;
            if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
            UChar_t* bt=cHPlModM+nbF;
            for(Int_t nb=nbF; nb<=nbL; nb++) {
              *bt |= add;
              Char_t wt=HMdcBArray::getNSet(bt);
              bt++;
              if(wt<minAm) continue;
              HMdcBArray::set(cPlotBAr,nb);
              if(nb<minBin) minBin=nb;
              if(nb>maxBin) maxBin=nb;
            }
          }
        }
      }
    } else {                         // filling in minBin[y]-maxBin[y] only
      if(nFiredLay<minAm) makeLayProjV2(mod,layer);  // filling pr.plot
      else {                                                // ...+ amp.checking
        UChar_t add=1<<layer;
        HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
        Int_t cell = -1;
        while( pLayLCells.nextCell(cell) ) {
//?W          if(hitsDel[indLay][n]<0) continue;
          HMdcLookUpTbCell& fCell = fLayer[cell];
          for(Int_t ln=0; ln<fCell.line; ln++) {
            Int_t y=fCell.yBinMin+ln;
            if(cXMaxM[y]<0) continue;
            Int_t shift=y * nBinX;
            Int_t nbL=fCell.xBinMax[ln]+shift;
            Int_t nbF=fCell.xBinMin[ln]+shift;
            if(nbL>cXMaxM[y]) nbL=cXMaxM[y];
            if(nbF<cXMinM[y]) nbF=cXMinM[y];
            UChar_t* bt=cHPlModM+nbF;
            for(Int_t nb=nbF; nb<=nbL; nb++) {
              *bt |= add;
              Char_t wt=HMdcBArray::getNSet(bt);
              bt++;
              if(wt<minAm) continue;
              HMdcBArray::set(cPlotBAr,nb);
              if(nb<minBin) minBin=nb;
              if(nb>maxBin) maxBin=nb;
            }
          }
        }
      }
    }
  }
  maxBin/=32;
  minBin/=32;
}

void HMdcLookUpTbSec::makeLayProjV1(Int_t mod,Int_t layer) {
  // plot filling and filled region determination
  UChar_t add=1<<layer;
  HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
  HMdcLayListCells& pLayLCells = (*pListCells)[mod][layer];
  Int_t cell = -1;
  while( pLayLCells.nextCell(cell) ) {
//?W    if(hitsDel[indLay][n]<0) continue;
    HMdcLookUpTbCell& fCell = fLayer[cell];
    for(Int_t ln=0; ln<fCell.line; ln++) {
      Int_t y=fCell.yBinMin+ln;
      Int_t shift=y * nBinX;
      Int_t nbL=fCell.xBinMax[ln]+shift;
      Int_t nbF=fCell.xBinMin[ln]+shift;
      if(nbF<cXMinM[y]) cXMinM[y]=nbF;
      if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
      UChar_t* hPlModE=cHPlModM+nbL;
      for(UChar_t* bt=cHPlModM+nbF; bt<=hPlModE; bt++) *bt |= add;
    }
  }
}

void HMdcLookUpTbSec::makeLayProjV1b(Int_t mod,Int_t layer) {
  // plot filling and filled region determination in region determined
  // in previous mdc
  UChar_t add=1<<layer;
  HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
  HMdcLayListCells& pLayLCells = (*pListCells)[mod][layer];
  Int_t cell = -1;
  while( pLayLCells.nextCell(cell) ) {
//?W    if(hitsDel[indLay][n]<0) continue;
    HMdcLookUpTbCell& fCell = fLayer[cell];
    for(Int_t ln=0; ln<fCell.line; ln++) {
      Int_t y=fCell.yBinMin+ln;
      if(pXMaxM[y]<0) continue;
      Int_t shift=y * nBinX;
      Int_t nbL=fCell.xBinMax[ln]+shift;
      Int_t nbF=fCell.xBinMin[ln]+shift;
      if(nbL>pXMaxM[y]) nbL=pXMaxM[y];
      if(nbF<pXMinM[y]) nbF=pXMinM[y];
      if(nbF>nbL) continue;
      if(nbF<cXMinM[y]) cXMinM[y]=nbF;
      if(nbL>cXMaxM[y]) cXMaxM[y]=nbL;
      UChar_t* hPlModE=cHPlModM+nbL;
      for(UChar_t* bt=cHPlModM+nbF; bt<=hPlModE; bt++) *bt |= add;
    }
  }
}

void HMdcLookUpTbSec::makeLayProjV2(Int_t mod,Int_t layer) {
  // plot filling in filled regions only
  UChar_t add=1<<layer;
  HMdcLookUpTbLayer& fLayer=(*cFMod)[layer];
  HMdcLayListCells& pLayLCells = (*pListCells)[mod][layer];
  Int_t cell = -1;
  while( pLayLCells.nextCell(cell) ) {
//?W    if(hitsDel[indLay][n]<0) continue;
    HMdcLookUpTbCell& fCell = fLayer[cell];
    for(Int_t ln=0; ln<fCell.line; ln++) {
      Int_t y=fCell.yBinMin+ln;
      if(cXMaxM[y]<0) continue;
      Int_t shift=y * nBinX;
      Int_t nbL=fCell.xBinMax[ln]+shift;
      Int_t nbF=fCell.xBinMin[ln]+shift;
      if(nbL>cXMaxM[y]) nbL=cXMaxM[y];
      if(nbF<cXMinM[y]) nbF=cXMinM[y];
      UChar_t* hPlModE=cHPlModM+nbL;
      for(UChar_t* bt=cHPlModM+nbF; bt<=hPlModE; bt++) *bt |= add;
    }
  }
}

Int_t HMdcLookUpTbSec::findClusters(Int_t *imax){
  nClusters   = 0;
  //isCoilOff - !!!??? ---------------------------------------
  nMods = 0;
  noFiredCells = kTRUE;
  for(Int_t mod=0; mod<4; mod++) {
    if(mod<nModules) {
      maxAmp[mod] = (*pListCells)[mod].getNLayers();
      minAmp[mod] = imax[mod];
      if(minAmp[mod]>0) nMods++;
      if(maxAmp[mod]>0) noFiredCells = kFALSE;
    } else {
      maxAmp[mod] = 0;
      minAmp[mod] = 0;
    }
  }
  if( noFiredCells ) return 0;
  if(typeClFinder==1) {
    // chamber clust. finding
    for(Int_t m=0; m<4; m++) if(minAmp[m]>0) findClusInMod(m);
  } else {
    // combined clust. finding
    nModSeg[0]=nModSeg[1]=0;
    for(Int_t m=0;m<4;m++) if(minAmp[m]>0) nModSeg[m/2] |= m%2+1;
    if(nModSeg[0]>0 && nModSeg[1]>0) findClusInSec();
    else {
      if(nModSeg[0]==3) {
        if(isCoilOff)                findClusInSeg(0);
        else                         findClusInSeg1();
      } else if(nModSeg[0]>0)        findClusInMod(nModSeg[0]-1);
      if(nModSeg[1]==3)              findClusInSeg(1);
      else if(nModSeg[1]>0)          findClusInMod(nModSeg[1]+1);
    }
  }
  return nClusters;
}

void HMdcLookUpTbSec::initCluster(Int_t nBin) {
  clus = &cClusArr[*cNClusArr];
  clus->clear(segment); // 0-inner segment, -1-sector track
  Int_t ny      = nBin/nBinX;
  nLMinCl       = nLMaxCl    = ny;
  xMinCl[ny]    = xMaxCl[ny] = nBin%nBinX;
  isClstrInited = kTRUE;
}

void HMdcLookUpTbSec::reinitCluster(Int_t nBin) {
  // Use it if list of wires empty yet!
  clus->clearBinStat();
  Int_t ny   = nBin/nBinX;
  nLMinCl    = nLMaxCl    = ny;
  xMinCl[ny] = xMaxCl[ny] = nBin%nBinX;
}

void HMdcLookUpTbSec::initCluster(Int_t nBin,Int_t amp) {
  clus = &cClusArr[*cNClusArr];
  clus->clear(segment);
  Int_t ny=nBin/nBinX;
  nLMinCl = nLMaxCl = ny;
  xMinCl[ny] = xMaxCl[ny] = nBin%nBinX;
  isClstrInited = kTRUE;
  addBinInCluster(nBin,amp);
}

void HMdcLookUpTbSec::reinitCluster(Int_t nBin,Int_t amp) {
  // Use it if list of wires empty yet!
  clus->clearBinStat();
  Int_t ny   = nBin/nBinX;
  nLMinCl    = nLMaxCl    = ny;
  xMinCl[ny] = xMaxCl[ny] = nBin%nBinX;
  addBinInCluster(nBin,amp);
}

void HMdcLookUpTbSec::addBinInCluster(Int_t nBin,Char_t wt) {
  Int_t nx = nBin%nBinX;
  Int_t ny = nBin/nBinX;
  clus->addBin(xBinsPos[nx],yBinsPos[ny],(Double_t)wt);
  // ny-nLMaxCl and nLMinCl-ny can't be >1 !
  if(ny<nLMinCl) {
    nLMinCl = ny;
    xMinCl[ny] = xMaxCl[ny] = nx;
  } else if(ny>nLMaxCl) {
    nLMaxCl = ny;
    xMinCl[ny] = xMaxCl[ny] = nx;
  }
  else if(nx<xMinCl[ny]) xMinCl[ny]=nx;
  else if(nx>xMaxCl[ny]) xMaxCl[ny]=nx;
}

void HMdcLookUpTbSec::initClusterT2(Int_t nBin,Int_t amp) {
  initCluster(nBin);
  addBinInClusterT2(nBin,amp);
}

void HMdcLookUpTbSec::reinitClusterT2(Int_t nBin,Int_t amp) {
  reinitCluster(nBin);
  addBinInClusterT2(nBin,amp);
}

void HMdcLookUpTbSec::addBinInClusterT2(Int_t nBin,Char_t wt) {
  // The same as "addBinInCluster" plus clusters chamber clusters setting
  Int_t nx = nBin%nBinX;
  Int_t ny = nBin/nBinX;
  clus->addBin(xBinsPos[nx],yBinsPos[ny],(Double_t)wt);
  // ny-nLMaxCl and nLMinCl-ny can't be >1 !
  if(ny<nLMinCl) {
    nLMinCl = ny;
    xMinCl[ny] = xMaxCl[ny] = nx;
  } else if(ny>nLMaxCl) {
    nLMaxCl = ny;
    xMinCl[ny] = xMaxCl[ny] = nx;
  }
  else if(nx<xMinCl[ny]) xMinCl[ny]=nx;
  else if(nx>xMaxCl[ny]) xMaxCl[ny]=nx;
  
  if(clus->clusMod1 == 0 && clusIndM1[nBin]>=0) {
    clus->clusMod1=&clusArrM1[clusIndM1[nBin]];
    while(clus->clusMod1->clusMerg) clus->clusMod1=clus->clusMod1->clusMerg;
    clus->clusMod1->flag++;
  }
  if(clus->clusMod2 == 0 && clusIndM2[nBin]>=0) {
    clus->clusMod2=&clusArrM2[clusIndM2[nBin]];
    while(clus->clusMod2->clusMerg) clus->clusMod2=clus->clusMod2->clusMerg;
    clus->clusMod2->flag++;
  }
}

Int_t HMdcLookUpTbSec::getClusterSlot(Int_t seg, HMdcList12GroupCells& list) {
  locClus[1]=seg;
  locClus[2]=counter[seg];
  Int_t index;
  fClus = (HMdcClus*)fClusCat->getSlot(locClus,&index);
  if(!fClus) {
    Warning("getClusterSlot","Sec.%i Segment %i No slot HMdcClus available",
        sector+1,seg+1);
    isSlotAv[seg] = kFALSE;
    return -1;
  }
  if(isGeant) fClus=(HMdcClus*)(new(fClus) HMdcClusSim(list));
  else fClus=new(fClus) HMdcClus(list);
  fClus->setAddress(locClus);
  counter[seg]++;
  return index;
}

Bool_t HMdcLookUpTbSec::fillSecCluster(void) {
  clus->calcXY();
  if(minAmp[0]>0) fillModWiresList(0,clus->lCells1);
  if(minAmp[1]>0) fillModWiresList(1,clus->lCells1);
  if(minAmp[2]>0) fillModWiresList(2,clus->lCells2);
  if(minAmp[3]>0) fillModWiresList(3,clus->lCells2);
  return increaseClusterNum();
}

Bool_t HMdcLookUpTbSec::fillSegCluster(void) {
  clus->calcXY();
  fillModWiresList(cSeg*2,  clus->lCells1);
  fillModWiresList(cSeg*2+1,clus->lCells1);
  return increaseClusterNum();
}

Bool_t HMdcLookUpTbSec::fillModCluster(Int_t mod) {
  clus->calcXY();
  fillModWiresList(mod,clus->lCells1);
  return increaseClusterNum();
}

void HMdcLookUpTbSec::fillModWiresList(Int_t mod, HMdcList12GroupCells& list) {
  HMdcLookUpTbMod& fMod=(*this)[mod];
  if( !(&fMod) ) return;
  HMdcModListCells& pModLCells = (*pListCells)[mod];
  Int_t layAdd=(mod%2)*6;
  for(Int_t layer=0; layer<6; layer++) {
    Int_t clLay=layAdd+layer;
    HMdcLookUpTbLayer& fLayer=fMod[layer];
    HMdcLayListCells& pLayLCells = pModLCells[layer];
    Int_t cell = -1;
    while( pLayLCells.nextCell(cell) ) {
//?W      if(hitsDel[iLayer][i]<0) continue;
      HMdcLookUpTbCell& fCell=fLayer[cell];
      Short_t yBinMax=fCell.yBinMax;
      if( nLMinCl > yBinMax ) continue;
      Short_t yBinMin=fCell.yBinMin;
      if( nLMaxCl < yBinMin ) break;    //        Hits are sorted !!!
      Short_t lMax=(yBinMax < nLMaxCl) ? yBinMax : nLMaxCl;
      Short_t lMin=(yBinMin > nLMinCl) ? yBinMin : nLMinCl;
      for (Short_t nl=lMin; nl<=lMax; nl++) {
        if( fCell.xBinMin[nl-yBinMin] > xMaxCl[nl] ||
            fCell.xBinMax[nl-yBinMin] < xMinCl[nl] ) continue;
        list.setTime(clLay,cell,pLayLCells.getTime(cell));
	break;
      }
    }
  }
}

void HMdcLookUpTbSec::findClusInSeg(Int_t seg){
  cMod1      = seg*2;
  cMod2      = cMod1+1;
  cSeg       = seg;
  if(maxAmp[cMod1]<minAmp[cMod1] || maxAmp[cMod2]<minAmp[cMod2]) return;
  if(minAmp[cMod1]+minAmp[cMod2] < 3) return;
  makeSPlot();
  setCurrentArraySec();
  nClsArr  = 0;
  cPlModF  = hPlMod[cMod1];
  cPlModS  = hPlMod[cMod2];
  cPlotBAr = plotBArSc;
  Bool_t useFixedLevel = HMdcLookUpTb::getUseFixedLevelFlag();
  
  Int_t* plotBAr4b = (Int_t*)cPlotBAr;
  //---Cluster finding-------------------------------------
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    if(useFixedLevel) while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                            calcClusterInSegFixedLevel(nBin4+nLBin));
    else              while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                             calcClusterInSegFloatLevel(nBin4+nLBin));
    if(*cNClusArr >= clusArrSize) break;
  }

  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInSeg();
    calcClParam();
    fillClusCat(-2,seg,0);
  }
  for(Int_t mod=seg*2;mod<(seg+1)*2;mod++) if(minAmp[mod]>0) clearPrMod(mod);
}

Bool_t HMdcLookUpTbSec::calcClusterInSegFixedLevel(Int_t nBin) {
  initCluster(nBin,HMdcBArray::getNSet(cPlModF[nBin])+
                   HMdcBArray::getNSet(cPlModS[nBin]));
  stack->init();
  do {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if(HMdcBArray::testAndUnset(cPlotBAr,nb)) {
        stack->push(nb);
        addBinInCluster(nb,HMdcBArray::getNSet(cPlModF[nb])+
                           HMdcBArray::getNSet(cPlModS[nb]));
      }
    }
  } while ((nBin=stack->pop()) >= 0);
  if( fillSegCluster() ) return kTRUE; // Filling of cluster
  return kFALSE;
}

Bool_t HMdcLookUpTbSec::calcClusterInSegFloatLevel(Int_t nBin) {
  Int_t  amp    = HMdcBArray::getNSet(cPlModF[nBin])+
                  HMdcBArray::getNSet(cPlModS[nBin]);
  initCluster(nBin,amp);
  stacksArr->init(12);
  while (kTRUE) {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if( !HMdcBArray::testAndUnset(cPlotBAr,nb) ) continue;
      Int_t ampN = HMdcBArray::getNSet(cPlModF[nb])+
                   HMdcBArray::getNSet(cPlModS[nb]);
      stacksArr->push(nb,ampN);
      if(ampN  > amp) {
        // Increasing amplitude:
        if(nTs<7) stacksArr->push(nBin,amp);
        amp = ampN;
        if(isClstrInited) reinitCluster(nb,amp);
        else              initCluster(nb,amp);
        break;
      } else if(ampN==amp && isClstrInited) addBinInCluster(nb,amp);
    }
    Int_t ampP = stacksArr->pop(nBin);
    if(ampP == amp) continue;
    //-End of cluster region. Filling of cluster:-------------------------
    if(isClstrInited && !fillSegCluster()) return kFALSE;
    if(ampP<0) break; // No more bins in stack!
    amp = ampP;
  }
  return kTRUE;
}
    
void HMdcLookUpTbSec::findClusInSeg1(void){
  if(typeClFinder==0) {
    if(maxAmp[0]<minAmp[0] || maxAmp[1]<minAmp[1]) return;
  } else if(maxAmp[0]<minAmp[0] && maxAmp[1]<minAmp[1]) return;
  if(minAmp[0]+minAmp[1]<3) return;
  makeModPlot(0);
  makeModPlot(1);

  minBinBAr4Sc=(minBinBAr4M[0]>=minBinBAr4M[1])?minBinBAr4M[0]:minBinBAr4M[1];
  maxBinBAr4Sc=(maxBinBAr4M[0]<=maxBinBAr4M[1])?maxBinBAr4M[0]:maxBinBAr4M[1];
  Int_t* plotBAr4bM1=(Int_t *)(plotBArM[0]);
  Int_t* plotBAr4bM2=(Int_t *)(plotBArM[1]);
  Int_t* plotBArSc4b=(Int_t *)plotBArSc;
  for(Int_t n4=minBinBAr4Sc; n4<=maxBinBAr4Sc; n4++)
      plotBArSc4b[n4] = plotBAr4bM1[n4] & plotBAr4bM2[n4];

  setCurrentArrayMod1();
  scanPlotInMod(0);
  if(nClsArrM1>1) mergeClusInMod(0);
  calcClParam();

  setCurrentArrayMod2();
  scanPlotInMod(1);
  if(nClsArrM2>1) mergeClusInMod(1);
  calcClParam();

  setCurrentArraySec();
  scanPlotInSeg1(0,plotBArSc);

  if(nClsArr>1) mergeClusInSeg();
  testClusMod12toSeg();
  if(nClsArrM1>0 && nClsArrM2>0) mergeClusMod1to2();
  if(nClsArr>0) {
    calcClParam();
    fillClusCat(-2,0,0);
  }
  
  if(typeClFinder==2) {
    setCurrentArrayMod1();
    fillClusCat(0,0,2);
    setCurrentArrayMod2();
    fillClusCat(1,0,2);
  }
  
  clearPrMod(0);
  clearPrMod(1);
}

void HMdcLookUpTbSec::scanPlotInMod(Int_t mod) {
  // Scan proj.plot in one module but determination of wires list in segment(!)
  cMod             = mod;
  (*cNClusArr)     = 0;
  cSeg             = mod/2;
  cPlMod           = hPlMod[mod];
  cPlotBAr         = plotBArM[mod];
  Int_t* plotBAr4b = (Int_t*) cPlotBAr;
  Int_t minBin     = minBinBAr4M[mod];
  clusInd          = mod==0 ? clusIndM1:clusIndM2;
  Bool_t useFixedLevel = HMdcLookUpTb::getUseFixedLevelFlag();
  for(Int_t n4=maxBinBAr4M[mod]; n4>=minBin; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1=cPlotBAr+n4*4;
    UChar_t *b2=b1+3;
    Int_t nBin4=n4*32;
    Int_t nLBin=33;
    if(useFixedLevel) while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                            calcMixedClusterFixedLevel(nBin4+nLBin));
    else              while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                            calcMixedClusterFloatLevel(nBin4+nLBin));
    if((*cNClusArr) >= clusArrSize) break;
  }
}

Bool_t HMdcLookUpTbSec::calcMixedClusterFixedLevel(Int_t nBin) {
  initCluster(nBin,HMdcBArray::getNSet(cPlMod[nBin]));
  clusInd[nBin]=*cNClusArr;
  stack->init();
  do {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if(HMdcBArray::testAndUnset(cPlotBAr,nb)) {
        stack->push(nb);
        addBinInCluster(nb,HMdcBArray::getNSet(cPlMod[nb]));
        clusInd[nb]=*cNClusArr;
      }
    }
  } while ((nBin=stack->pop()) >= 0);
  if(fillSegCluster()) return kTRUE; // Filling of cluster
  return kFALSE;
}

Bool_t HMdcLookUpTbSec::calcMixedClusterFloatLevel(Int_t nBin) {
  Int_t  amp    = HMdcBArray::getNSet(cPlMod[nBin]);
  initCluster(nBin,amp);
  clusInd[nBin]=*cNClusArr;
  stacksArr->init(12);
  stack->init();
  if(HMdcBArray::get(plotBArSc,nBin)) stack->push(nBin);
  while (kTRUE) {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if( !HMdcBArray::testAndUnset(cPlotBAr,nb) ) continue;
      Int_t ampN = HMdcBArray::getNSet(cPlMod[nb]);
      stacksArr->push(nb,ampN);
      if(ampN  > amp) {
        // Increasing amplitude:
        clusInd[nBin] = -1;
        if(nTs<7) stacksArr->push(nBin,amp);
        amp = ampN;
        Int_t bind;
        while((bind=stack->pop()) >= 0) clusInd[bind] = -1;
        if(isClstrInited) reinitCluster(nb,amp);
        else              initCluster(nb,amp);
        clusInd[nb]=*cNClusArr;
        if(HMdcBArray::get(plotBArSc,nb)) stack->push(nb);
        break;
      } else if(ampN==amp && isClstrInited) {
        addBinInCluster(nb,amp);
        clusInd[nb]=*cNClusArr;
        if(HMdcBArray::get(plotBArSc,nb)) stack->push(nb);
      } else clusInd[nb] = -1;
    }
    Int_t ampP = stacksArr->pop(nBin);
    if(ampP == amp) continue;
    //-End of cluster region. Filling of cluster:-------------------------
    if(isClstrInited && !fillSegCluster()) return kFALSE;
    if(ampP<0) break; // No more bins in stack!
    amp = ampP;
    stack->init();
  }
  return kTRUE;
}
    
void HMdcLookUpTbSec::scanPlotInSeg1(Int_t seg, UChar_t* plotBAr) {
  cMod1      = seg*2;
  cMod2      = cMod1+1;
  cSeg       = seg;
  cPlModF    = hPlMod[cMod1];
  cPlModS    = hPlMod[cMod2];
  cPlotBAr   = plotBAr;
  *cNClusArr = 0;        // number of clusters
  Bool_t useFixedLevel = HMdcLookUpTb::getUseFixedLevelFlag();
  
  Int_t* plotBAr4b = (Int_t*)cPlotBAr;
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1    = cPlotBAr+n4*4;
    UChar_t *b2    = b1+3;
    Int_t    nBin4 = n4*32;
    Int_t    nLBin = 33;
    if(useFixedLevel) while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                             calcClusterInSeg1FixedLevel(nBin4+nLBin));
    else              while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                             calcClusterInSeg1FloatLevel(nBin4+nLBin));
    if(*cNClusArr >= clusArrSize) break;
  }
  if(!useFixedLevel) testSeg1ModClMatching();
}

void HMdcLookUpTbSec::testSeg1ModClMatching() {
  HMdcList12GroupCells listIdent;
  for(Int_t segCl=0; segCl<*cNClusArr; segCl++) {
    HMdcCluster& clus = clusArr[segCl];
    for(Int_t imod=0;imod<2;imod++) {
      if(clus.getModCluster(imod) != 0) continue;  // already setted
      Int_t        nClsArrM = imod == 0 ? nClsArrM1 : nClsArrM2;
      HMdcCluster* clusArrM = imod == 0 ? clusArrM1 : clusArrM2;
      Int_t nLayers         = 0;
      Int_t nWires          = 0;
      Int_t nLayersOp       = 0;
      Int_t nWiresOp        = 0;
      HMdcCluster* modClust = 0;
      for(Int_t cl=0;cl<nClsArrM;cl++) {
        HMdcCluster& pCls=clusArrM[cl];
        if(pCls.clusMerg) continue;
        clus.lCells1.compare(&(pCls.lCells1),0,11,&listIdent);
        Int_t nLayersNext   = listIdent.getNLayersMod(imod);
        Int_t nLayersNextOp = listIdent.getNLayersMod(imod^1);
        Int_t nWiresNext    = listIdent.getNDrTimesMod(imod);
        Int_t nWiresNextOp  = listIdent.getNDrTimesMod(imod^1);
        if(nLayersNext==0 || nLayersNext < nLayers) continue;
        if(nLayersNext==nLayers) {
          if(nWiresNext < nWires) continue;
          if(nWiresNext == nWires) {
            if(nLayersNextOp < nLayersOp) continue;
            if(nLayersNextOp == nLayersOp && nWiresNextOp <= nWiresOp) continue;
          }
        }
        modClust  = &pCls;
        nLayers   = nLayersNext;
        nWires    = nWiresNext;
        nLayersOp = nLayersNextOp;
        nWiresOp  = nWiresNextOp;
      }
      if(modClust) {
        clus.setModCluster(imod,modClust);
        modClust->flag++;
      }
    }
  }
}

Bool_t HMdcLookUpTbSec::calcClusterInSeg1FixedLevel(Int_t nBin) {
  initClusterT2(nBin,HMdcBArray::getNSet(cPlModF[nBin])+
                     HMdcBArray::getNSet(cPlModS[nBin]));
  stack->init();            // init stack only
  do {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if(HMdcBArray::testAndUnset(cPlotBAr,nb)) {
        stack->push(nb);
        addBinInCluster(nb,HMdcBArray::getNSet(cPlModF[nb])+
                           HMdcBArray::getNSet(cPlModS[nb]));
      }
    }
  } while ((nBin=stack->pop()) >= 0);
  if( fillSegCluster() ) return kTRUE;
  return kFALSE;
}

Bool_t HMdcLookUpTbSec::calcClusterInSeg1FloatLevel(Int_t nBin) {
  Int_t  amp    = HMdcBArray::getNSet(cPlModF[nBin])+
                  HMdcBArray::getNSet(cPlModS[nBin]);
  initClusterT2(nBin,amp);
  stacksArr->init(12);
  while (kTRUE) {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if( !HMdcBArray::testAndUnset(cPlotBAr,nb) ) continue;
      Int_t ampN = HMdcBArray::getNSet(cPlModF[nb])+
                   HMdcBArray::getNSet(cPlModS[nb]);
      stacksArr->push(nb,ampN);
      if(ampN  > amp) {
        // Increasing amplitude:
        if(nTs<7) stacksArr->push(nBin,amp);
        amp = ampN;
        if(isClstrInited) reinitClusterT2(nb,amp);
        else              initClusterT2(nb,amp);
        break;
      } else if(ampN==amp && isClstrInited) addBinInClusterT2(nb,amp);
    }
    Int_t ampP = stacksArr->pop(nBin);
    if(ampP == amp) continue;
    //-End of cluster region. Filling of cluster:-------------------------
    if(isClstrInited && !fillSegCluster()) return kFALSE;
    if(ampP<0) break; // No more bins in stack!
    amp = ampP;
  }
  return kTRUE;
}
    
Bool_t HMdcLookUpTbSec::increaseClusterNum(void) {
  if(*cNClusArr > 0) pClustersArrs->testCluster(clusArrInd,*cNClusArr);
  (*cNClusArr)++;
  isClstrInited = kFALSE;
  if(*cNClusArr < clusArrSize) return kTRUE;
  *cNClusArr = clusArrSize;
  Warning("increaseClusterNum"," Num.of clusters in sector %i > max\n",sector);
  memset(cPlotBAr,0,sizeBAr);
  return kFALSE;
}

void HMdcLookUpTbSec::findClusInSec(void){
  for(Int_t mod=0;mod<4;mod++)
      if(minAmp[mod]>0 && maxAmp[mod]<minAmp[mod]) return;
  if(minAmp[0]+minAmp[1]+minAmp[2]+minAmp[3] < 3) return;
  makeSPlot();
  setCurrentArraySec();
  nClsArr  = 0;
  cPlotBAr = plotBArSc;
  Bool_t useFixedLevel = HMdcLookUpTb::getUseFixedLevelFlag();
  
  Int_t* plotBAr4b=(Int_t*) cPlotBAr;
  //---Cluster finding-------------------------------------
  for(Int_t n4=maxBinBAr4Sc; n4>=minBinBAr4Sc; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1    = cPlotBAr+n4*4;
    UChar_t *b2    = b1+3;
    Int_t    nBin4 = n4*32;
    Int_t    nLBin = 33;
    if(useFixedLevel) while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                            calcClusterInSecFixedLevel(nBin4+nLBin));
    else              while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                            calcClusterInSecFloatLevel(nBin4+nLBin));
    if(nClsArr >= clusArrSize) break;
  }
  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInSec();
    calcClParam();
    fillClusCat(-nMods,-1,0);
  }
  for(Int_t mod=0;mod<4;mod++) if(minAmp[mod]>0) clearPrMod(mod);
}

Bool_t HMdcLookUpTbSec::calcClusterInSecFixedLevel(Int_t nBin) {
  initCluster(nBin,HMdcBArray::getNSet(hPlMod[0][nBin])+
                   HMdcBArray::getNSet(hPlMod[1][nBin])+
                   HMdcBArray::getNSet(hPlMod[2][nBin])+
                   HMdcBArray::getNSet(hPlMod[3][nBin]));
  stack->init();
  do {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if(HMdcBArray::testAndUnset(cPlotBAr,nb)) {
        stack->push(nb);
        addBinInCluster(nb,HMdcBArray::getNSet(hPlMod[0][nb])+
                           HMdcBArray::getNSet(hPlMod[1][nb])+
                           HMdcBArray::getNSet(hPlMod[2][nb])+
                           HMdcBArray::getNSet(hPlMod[3][nb]));
      }
    }
  } while ((nBin=stack->pop()) >= 0);
  if( fillSecCluster() ) return kTRUE; // Filling of cluster
  return kFALSE;
}

Bool_t HMdcLookUpTbSec::calcClusterInSecFloatLevel(Int_t nBin) {
  Int_t  amp    = HMdcBArray::getNSet(hPlMod[0][nBin])+
                  HMdcBArray::getNSet(hPlMod[1][nBin])+
                  HMdcBArray::getNSet(hPlMod[2][nBin])+
                  HMdcBArray::getNSet(hPlMod[3][nBin]);
  initCluster(nBin,amp);
  stacksArr->init(24);
  while (kTRUE) {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if( !HMdcBArray::testAndUnset(cPlotBAr,nb) ) continue;
      Int_t ampN = HMdcBArray::getNSet(hPlMod[0][nb])+
                   HMdcBArray::getNSet(hPlMod[1][nb])+
                   HMdcBArray::getNSet(hPlMod[2][nb])+
                   HMdcBArray::getNSet(hPlMod[3][nb]);
      stacksArr->push(nb,ampN);
      if(ampN  > amp) {
        // Increasing amplitude:
        if(nTs<7) stacksArr->push(nBin,amp);
        amp = ampN;
        if(isClstrInited) reinitCluster(nb,amp);
        else              initCluster(nb,amp);
        break;
      } else if(ampN==amp && isClstrInited) addBinInCluster(nb,amp);
    }
    Int_t ampP = stacksArr->pop(nBin);
    if(ampP == amp) continue;
    //-End of cluster region. Filling of cluster:-------------------------
    if(isClstrInited && !fillSecCluster()) return kFALSE;
    if(ampP<0) break; // No more bins in stack!
    amp = ampP;
  }
  return kTRUE;
}


void HMdcLookUpTbSec::mergeClusInMod(Int_t mod) {
  if(!mergeFlag) return;
  Int_t nClus=*cNClusArr;
  Int_t fLay=(mod==0 || mod==2) ? 0:6;
  Int_t lLay=fLay+5;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<(*cNClusArr)-1; cl1++) {
      HMdcCluster& cls1=cClusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      for(Int_t cl2=cl1+1; cl2<(*cNClusArr); cl2++) {
        HMdcCluster& cls2=cClusArr[cl2];
        if(!cls2.status) continue;
        HMdcList12GroupCells& listCells2=cls2.lCells1;
        Float_t dY=cls1.y-cls2.y;
//???optimizaciya???        if(dY>100.) break;  //????????? biylo zakomentirovano ???
        if(fabs(dY) > 30.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 100.) continue;        // 40. mm !???
        if(listCells1.compare(&listCells2,fLay,lLay)<4) continue;
        cls1.addClus(cls2);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::mergeClusMod1to2(void) {
  if(!mergeFlag) return;
  for(Int_t cl1=0; cl1<nClsArrM1; cl1++) {
    HMdcCluster& cls1=clusArrM1[cl1];
    if(!cls1.status || cls1.flag>0) continue;
    HMdcList12GroupCells& listCells1=cls1.lCells1;
    for(Int_t cl2=0; cl2<nClsArrM2; cl2++) {
      HMdcCluster& cls2=clusArrM2[cl2];
      if(!cls2.status || cls2.flag>0) continue;
      HMdcList12GroupCells& listCells2=cls2.lCells1;
      Float_t dY=cls1.y-cls2.y;
//???optimizaciya???      if(dY>100.) break;  //????????? biylo zakomentirovano ???
      if(fabs(dY) > 30.) continue;                   // 10. mm !???
      if(fabs(cls1.x-cls2.x) > 100.) continue;        // 40. mm !???
      if(listCells1.compare(&listCells2)<4) continue;
//      if(listCells1.compare(&listCells2,0, 5)<4) continue;
//      if(listCells1.compare(&listCells2,6,11)<4) continue;
      if(nClsArr >= clusArrSize) break;
      HMdcCluster& cls=clusArr[nClsArr++];
      cls.clear(segment);
      cls.sumClus(cls1,cls2);
    }
  }
}

void HMdcLookUpTbSec::testClusMod12toSeg(void) {
  // excluding clusters in MOD with <4 non identical wires to segment clusters
  for(Int_t cl=0; cl<nClsArr; cl++) {
    HMdcCluster& cls=clusArr[cl];
    if(!cls.status || cls.flag>0) continue;
    HMdcList12GroupCells& lCells=cls.lCells1;
    for(Int_t cl1=0; cl1<nClsArrM1; cl1++) {
      HMdcCluster& cls1=clusArrM1[cl1];
      if(!cls1.status || cls1.flag>0) continue;
      HMdcList12GroupCells& lM1Cells1=cls1.lCells1;
      if(fabs(cls.y-cls1.y) > 30.) continue;                   // 10. mm !???
      if(fabs(cls.x-cls1.x) > 100.) continue;        // 40. mm !???
      if(lM1Cells1.getNCells()-lCells.nIdentDrTimes(&lM1Cells1) >= 4) continue; 
      cls1.flag=1;
    }
    for(Int_t cl2=0; cl2<nClsArrM2; cl2++) {
      HMdcCluster& cls2=clusArrM2[cl2];
      if(!cls2.status || cls2.flag>0) continue;
      HMdcList12GroupCells& lM1Cells2=cls2.lCells1;
      if(fabs(cls.y-cls2.y) > 30.) continue;                   // 10. mm !???
      if(fabs(cls.x-cls2.x) > 100.) continue;        // 40. mm !??
      if(lM1Cells2.getNCells()-lCells.nIdentDrTimes(&lM1Cells2) >= 4) continue; 
      cls2.flag=1;
    }
  }
}

void HMdcLookUpTbSec::mergeClusInSeg(void) {
  if(!mergeFlag) return;
  Int_t nClus=nClsArr;
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      HMdcCluster& cls1=clusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        HMdcCluster& cls2=clusArr[cl2];
        if(!cls2.status) continue;

  if(cls1.clusMod1!=0 && cls2.clusMod1!=0 && 
            cls1.clusMod1 != cls2.clusMod1) continue;
  if(cls1.clusMod2!=0 && cls2.clusMod2!=0 && 
            cls1.clusMod2 != cls2.clusMod2) continue;

        HMdcList12GroupCells& listCells2=cls2.lCells1;
        Float_t dY=cls1.y-cls2.y;
//???optimizaciya???        if(dY>100.) break;
        if(fabs(dY) > 30.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 100.) continue;       // 40. mm !???
        Int_t n1=listCells1.compare(&listCells2,0, 5);
        if(n1<3) continue;
        Int_t n2=listCells1.compare(&listCells2,6,11);
        if(n2<3 || n1+n2<7) continue;
//        if(listCells1.compare(&listCells2,0, 5)<4) continue;
//        if(listCells1.compare(&listCells2,6,11)<4) continue;
  if(cls1.clusMod1==0) cls1.clusMod1=cls2.clusMod1;
  if(cls1.clusMod2==0) cls1.clusMod2=cls2.clusMod2; 
        cls1.addClus(cls2);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::mergeClusInSec(void) {
  if(!mergeFlag) return;
  Int_t nClus=nClsArr;
  Int_t m1=minAmp[0];
  Int_t m2=minAmp[1];
  Int_t m3=minAmp[2];
  Int_t m4=minAmp[3];
  while(nClus>1) {
    Bool_t nomerg=kTRUE;
    for(Int_t cl1=0; cl1<nClsArr-1; cl1++) {
      HMdcCluster& cls1=clusArr[cl1];
      if(!cls1.status) continue;
      HMdcList12GroupCells& listCells1=cls1.lCells1;
      HMdcList12GroupCells& listCells2=cls1.lCells2;
      for(Int_t cl2=cl1+1; cl2<nClsArr; cl2++) {
        HMdcCluster& cls2=clusArr[cl2];
        if(!cls2.status) continue;
        Float_t dY=cls1.y-cls2.y;
//???optimizaciya???        if(dY>100.) break;
        if(fabs(dY) > 30.) continue;                   // 10. mm !???
        if(fabs(cls1.x-cls2.x) > 100.) continue;       // 40. mm !???
        HMdcList12GroupCells* listCells1s = &(cls2.lCells1);
        HMdcList12GroupCells* listCells2s = &(cls2.lCells2);
        if(m1>0 && listCells1.compare(listCells1s,0, 5)<4) continue;
        if(m2>0 && listCells1.compare(listCells1s,6,11)<4) continue;
        if(m3>0 && listCells2.compare(listCells2s,0, 5)<4) continue;
        if(m4>0 && listCells2.compare(listCells2s,6,11)<4) continue;
        cls1.addClus(cls2);
        nomerg=kFALSE;
        nClus--;
      }
    }
    if(nomerg || nClus==1) break;
  }
}

void HMdcLookUpTbSec::calcClParam(void) {
  for(Int_t nCl=0; nCl<(*cNClusArr); nCl++)
    if(cClusArr[nCl].status) cClusArr[nCl].calcClusParam(xHStep2,yHStep2);
}

void HMdcLookUpTbSec::fillClusCat(Int_t mod, Int_t segp, Int_t tpClFndr) {
  Int_t seg=(segp>=0) ? segp : 0;
  Int_t m1=minAmp[seg*2];
  Int_t m2=minAmp[seg*2+1];
  Int_t m3=0;
  Int_t m4=0;
  if(mod>=0) {
    if(mod==seg*2) m2=0;
    else if(mod==seg*2+1) m1=0;
  } else if(segp<0) {
    m3=minAmp[2];
    m4=minAmp[3];
  }
  if( !HMdcLookUpTb::getUseFixedLevelFlag() ) tpClFndr |= 128;
  // Filling containers:
  for(Int_t cl=0; cl<(*cNClusArr); cl++) {
    HMdcCluster& cls=cClusArr[cl];
    if(!cls.status || cls.flag>0) continue;
    Int_t index1=getClusterSlot(seg,cls.lCells1);
    if(!fClus) return;
    nClusters++;
    fClus->setMod(mod);
    fClus->setTypeClFinder(tpClFndr); // !!!
    fClus->setMinCl(m1,m2);
    fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
    fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],target[2],eTarg[2]);
    cls.fillClus(fClus,0,fillTrackList);
    if(segp<0) {
      HMdcClus* fClus0=fClus;
      Int_t index2=getClusterSlot(1,cls.lCells2);
      if(!fClus) return;
      fClus->setMod(mod);
      fClus->setTypeClFinder(tpClFndr);
      fClus->setMinCl(m3,m4);
      fClus->setPrPlane(prPlane.A(),prPlane.B(),prPlane.D());
      fClus->setTarget(target[0],eTarg[0],target[1],eTarg[1],
          target[2],eTarg[2]);
      cls.fillClus(fClus,1,fillTrackList);
      fClus->setIndexParent(index1);
      fClus0->setIndexChilds(index2,index2);
    }
  }
}

void HMdcLookUpTbSec::findClusInMod(Int_t mod){
  if(maxAmp[mod]<minAmp[mod]) return;
  if(minAmp[mod]<3) return;
  makeModPlot(mod);
  setCurrentArraySec();
  cMod             = mod;
  nClsArr          = 0;
  cPlMod           = hPlMod[mod];
  cPlotBAr         = plotBArM[mod];
  Int_t* plotBAr4b =(Int_t*) cPlotBAr;
  Int_t minBin     = minBinBAr4M[mod];
  Bool_t useFixedLevel = HMdcLookUpTb::getUseFixedLevelFlag();
  
  for(Int_t n4=maxBinBAr4M[mod]; n4>=minBin; n4--) {
    if(plotBAr4b[n4]==0) continue;
    UChar_t *b1    = cPlotBAr+n4*4;
    UChar_t *b2    = b1+3;
    Int_t    nBin4 = n4*32;
    Int_t    nLBin = 33;
    if(useFixedLevel) while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                             calcClusterInModFixedLevel(nBin4+nLBin));
    else              while((nLBin=HMdcBArray::prevAndUnset(b1,b2,nLBin))>=0 &&
                             calcClusterInModFloatLevel(nBin4+nLBin));
    if(nClsArr >= clusArrSize) break;
  }
  if(nClsArr>0) {
    if(nClsArr>1) mergeClusInMod(mod);
    calcClParam();
    fillClusCat(mod,mod/2,(typeClFinder==1) ? 1:0);
  }
  clearPrMod(mod);
}

Bool_t   HMdcLookUpTbSec::calcClusterInModFixedLevel(Int_t nBin) {
  initCluster(nBin,HMdcBArray::getNSet(cPlMod[nBin]));
  stack->init();
  do {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if(HMdcBArray::testAndUnset(cPlotBAr,nb)) {
        stack->push(nb);
        addBinInCluster(nb,HMdcBArray::getNSet(cPlMod[nb]));
      }
    }
  } while ((nBin=stack->pop()) >= 0);
  if( fillModCluster(cMod) ) return kTRUE; // Filling of cluster
  return kFALSE;
}

Bool_t   HMdcLookUpTbSec::calcClusterInModFloatLevel(Int_t nBin) {
  Int_t  amp = HMdcBArray::getNSet(cPlMod[nBin]);
  initCluster(nBin,amp);
  stacksArr->init(6);
  while (kTRUE) {
    for(Int_t nTs=0; nTs<8; nTs++) {
      Int_t nb=nBin+neighbBins[nTs];
      if( !HMdcBArray::testAndUnset(cPlotBAr,nb) ) continue;
      Int_t ampN = HMdcBArray::getNSet(cPlMod[nb]);
      stacksArr->push(nb,ampN);
      if(ampN  > amp) {
        // Increasing amplitude:
        if(nTs<7) stacksArr->push(nBin,amp);
        amp = ampN;
        if(isClstrInited) reinitCluster(nb,amp);
        else              initCluster(nb,amp);
        break;
      } else if(ampN==amp && isClstrInited) addBinInCluster(nb,amp);
    }
    Int_t ampP = stacksArr->pop(nBin);
    if(ampP == amp) continue;
    //-End of cluster region. Filling of cluster:-------------------------
    if(isClstrInited && !fillModCluster(cMod)) return kFALSE;
    if(ampP<0) break; // No more bins in stack!
    amp = ampP;
  }
  return kTRUE;
}

Int_t HMdcLookUpTbSec::getMaxClus(Int_t m) const {
  return (m>=0&&m<4) ? minAmp[m] : minAmp[0]+minAmp[1]+minAmp[2]+minAmp[3];
}

Bool_t HMdcLookUpTbSec::calcLookUpTb(void) {
  HMdcSizesCells *fSizesCells = HMdcSizesCells::getExObject();
  if(fSizesCells == 0) return kFALSE;
  HMdcSizesCellsSec& fSCSec = (*fSizesCells)[sector];
  HMdcTrap cellSize,cellSPr;
  HMdcTrapPlane cellPr,cellCont[2];
  for(Int_t mod=0;mod<4;mod++) if((*array)[mod]) for(Int_t lay=0;lay<6;lay++) {
    HMdcSizesCellsLayer& fSCLayer = fSCSec[mod][lay];
    Int_t nCells=fSCLayer.getSize();
    if(nCells<1) continue;
    HMdcLookUpTbLayer& fLUpTbLayer=(*this)[mod][lay];
    for(Int_t cell=0; cell<nCells; cell++) {
      HMdcLookUpTbCell& fLUpTbCell=fLUpTbLayer[cell];
      if( !&fLUpTbCell ) return kFALSE;
      fLUpTbCell.clear();
      HMdcSizesCellsCell& fSizesCellsCell = fSCLayer[cell];
      if( !&fSizesCellsCell || !fSizesCellsCell.getStatus()) continue;
      if(!fSizesCells->getCellVol(sector,mod,lay,cell,cellSize)) return kFALSE;      
      for(Int_t t=0; t<2; t++) {
        HGeomVector& targ = targVc[t];
        for(Int_t p=0; p<8; p++)
            prPlane.calcIntersection(targ,cellSize[p]-targ,cellSPr[p]);
        cellSPr.setDbPointNum(cellSize.getDbPointNum());
        if(cellSPr.getXYContour(cellCont[t]) < 3) return kFALSE;
      }
      if(cellPr.twoContoursSum(cellCont[0],cellCont[1]) < 3) return kFALSE;
      fillLookUpTb(mod,lay,cellPr,fLUpTbCell);
    }
  }
  return kTRUE;
}

Bool_t HMdcLookUpTbSec::fillLookUpTb(Int_t m, Int_t l,HMdcTrapPlane& cellPr,
                                       HMdcLookUpTbCell& fCell) {
  // MDC cell lookup table (fCell) filling.
  // m - module; l - layer;
  Int_t yBinMin = yBinNumInBounds(cellPr.getYMin());
  Int_t yBinMax = yBinNumInBounds(cellPr.getYMax());
  fCell.init(yBinMin,yBinMax);
  for(Int_t ny=yBinMin; ny<=yBinMax; ny++) {
    Double_t x1,x2;
    Double_t y1 = ny*yStep + yLow;
    if(!cellPr.getXminXmax(y1,y1+yStep,x1,x2)) return kFALSE;
    if(!fCell.addLine(xBinNumInBounds(x1),xBinNumInBounds(x2)))
        return kFALSE;
  }
  return kTRUE;
}

//---------------------------------------------------------
HMdcLookUpTb* HMdcLookUpTb::fMdcLookUpTb  = 0;
Bool_t        HMdcLookUpTb::useFixedLevel = kTRUE;

HMdcLookUpTb::HMdcLookUpTb(const Char_t* name,const Char_t* title,
                           const Char_t* context)
            : HParSet(name,title,context) {
  // constructor creates an array of pointers of type HMdcLookUpTbSec
  fMdcLookUpTb = this;
  strcpy(detName,"Mdc");
  fGetCont=HMdcGetContainers::getObject();
  if( fGetCont==0 || gHades==0) return;
  array = new TObjArray(6);
  fMdcDet       = fGetCont->getMdcDetector();
  fMdcGeomPar   = fGetCont->getMdcGeomPar();
  fSpecGeomPar  = fGetCont->getSpecGeomPar();
  fLayerGeomPar = fGetCont->getMdcLayerGeomPar();
  fSizesCells   = HMdcSizesCells::getObject();
  setPar(319, kFALSE);   // Project plot size is the same for all sectors!!!
  fMdcClusCat   = fGetCont->getCatMdcClus(kTRUE);
  targLenInc[0] = targLenInc[1]=0.;
  quietMode     = kFALSE;
  stack         = 0;
  stacksArr     = 0;
  mergeFlag     = kTRUE;
}

void HMdcLookUpTb::setPar(Int_t inBinX, Bool_t isCOff,Int_t inBinY) {
  nBinX = inBinX>1 ? inBinX : 319;
  if(nBinX%2 != 0) nBinX++;
  nBinY = inBinY>1 ? inBinY : (nBinX*278)/100;
  if(nBinY%2 != 0) nBinY++;
  isCoilOff=isCOff;
}

Bool_t HMdcLookUpTb::initContainer(void) {
  // It is called from "reinit" of reconstractor!
  if( !HMdcEvntListCells::getExObject() ) return kFALSE;
  if( !fMdcDet || !HMdcGetContainers::isInited(fMdcGeomPar) ||
      !HMdcGetContainers::isInited(fSpecGeomPar) ||
      !HMdcGetContainers::isInited(fLayerGeomPar) ||
      !fSizesCells->initContainer()) return kFALSE;

  if( !status && (fSizesCells->hasChanged() || fSpecGeomPar->hasChanged() ||
      fMdcGeomPar->hasChanged() || fLayerGeomPar->hasChanged()) ) {
    changed=kTRUE;
    if(!fMdcClusCat) return kFALSE;
    for (Int_t sec = 0; sec < 6; sec++) {
      if(!fMdcDet->isSectorActive(sec))  continue;
      if( !(*array)[sec] ) {
        const Int_t* nLM=HMdcTrackDSet::getTrFnNLayersInMod()+sec*4;
        Int_t nSegs=((fMdcDet->getModule(sec,0) && nLM[0]>0) ||
                     (fMdcDet->getModule(sec,1) && nLM[1]>0)) ? 1 : 0;
        if(isCoilOff && ((fMdcDet->getModule(sec,2) && nLM[2]>0) || 
                        (fMdcDet->getModule(sec,3) && nLM[3]>0))) nSegs = 2;
//          nSegs=(fMdcDet->getModule(sec,2) || fMdcDet->getModule(sec,3)) ? 2:1;
        if(nSegs==0) continue;
        
        HMdcLookUpTbSec* secF = new HMdcLookUpTbSec(sec,nSegs,nBinX,nBinY);
        if(!mergeFlag) secF->donotMergeClusters();
        (*array)[sec] = secF;
        secF->fClusCat=fMdcClusCat;
        if(stacksArr==0) {
          stacksArr = new HMdcClFnStacksArr(20000);
          stack     = new HMdcClFnStack(20000);
        }
        secF->stack     = stack;
        secF->stacksArr = stacksArr;
      }
      (*this)[sec].isCoilOff = isCoilOff;
      // initialization of container ---
      if(!calcPrPlane(sec))  return kFALSE;
      if(!calcTarget(sec))   return kFALSE;
      if(!calcPlotSize(sec)) return kFALSE;
      if(!(*this)[sec].calcLookUpTb()) return kFALSE;
      // --------------------------------
    }
    if(versions[1]<0 || versions[2]<0) versions[1]=versions[2]=0;
    else versions[2]++;
  } else changed=kFALSE;
  return kTRUE;
}

HMdcLookUpTb* HMdcLookUpTb::getObject(void) {
  if(!fMdcLookUpTb) fMdcLookUpTb = new HMdcLookUpTb();
  return fMdcLookUpTb;
}

void HMdcLookUpTb::deleteCont(void) {
  if(fMdcLookUpTb==0) return;
  delete fMdcLookUpTb;
  fMdcLookUpTb=0;
}

HMdcLookUpTb::~HMdcLookUpTb(void) {
  // destructor
  HMdcGetContainers::deleteCont();
  HMdcSizesCells::deleteCont();
  if(array) {
    array->Delete();
    delete array;
  }
  if(stacksArr) delete stacksArr;
  stacksArr=0;
  if(stack) delete stack;
  stack=0;
}

Int_t HMdcLookUpTb::getSize(void) {
  // return the size of the pointer array
  return array->GetEntries();
}

void HMdcLookUpTb::clear(void) {
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].clearwk();
}

Int_t  HMdcLookUpTb::findClusters(Int_t *imax){
  Int_t ntot=0;
  for(Int_t sec=0;sec<6;sec++) if((*array)[sec] != 0) {
    HMdcLookUpTbSec& fSec = (*this)[sec];
    ntot += fSec.findClusters(imax+4*sec);
  }
  return ntot;
}

TH2C* HMdcLookUpTbSec::fillTH2C(Char_t* name, Char_t* title, Int_t type,
    Int_t bining){
  // Proj.plot filling without cluster finding.
  for(Int_t mod=0; mod<4; mod++) {
    cFMod=&((*this)[mod]);
    if( !cFMod ) continue;
    cHPlModM=hPlMod[mod];
    if(!hPlMod) continue;
    cXMinM=xMin[mod];
    cXMaxM=xMax[mod];
    for(Int_t layer=0;layer<6;layer++) makeLayProjV1(mod,layer);
  }

  if(!hist) {
    plBining=(bining) ? 2:1;
    if(nBinX<100) plBining=1;
    hist=new TH2C(name,title,nBinX/plBining,xLow,xUp,nBinY/plBining,yLow,yUp);
  } else {
    hist->Reset();
    hist->SetName(name);
    hist->SetTitle(title);
  }
  hist->SetMinimum(0.);
  hist->Fill(0.,0.,0);
  Float_t maximum=6.;
  if(type==2) maximum=4.5;
  else if(type==0) maximum=(hPlMod[2]) ? 24.:12.;
  else if(type==1) maximum=(hPlMod[2]) ? 60.:18.;
  hist->SetMaximum(maximum);
  if(noFiredCells) return hist;
  for (Int_t nx=0; nx<nBinX; nx+=plBining) {
    for (Int_t ny=0; ny<nBinY; ny+=plBining) {
      Int_t nBin=ny*nBinX+nx;
      Int_t nBin2=nBin+nBinX;
      UChar_t binM1=hPlMod[0][nBin];
      UChar_t binM2=hPlMod[1][nBin];
      if(plBining==2) {
        binM1 |= hPlMod[0][nBin+1] | hPlMod[0][nBin2] | hPlMod[0][nBin2+1];
        binM2 |= hPlMod[1][nBin+1] | hPlMod[1][nBin2] | hPlMod[1][nBin2+1];
      }
      UChar_t binM3=0;
      UChar_t binM4=0;
      if(hPlMod[2]) {
        binM3=hPlMod[2][nBin];
        binM4=hPlMod[3][nBin];
        if(plBining==2) {
          binM3|=hPlMod[2][nBin+1] | hPlMod[2][nBin2] | hPlMod[2][nBin2+1];
          binM4|=hPlMod[3][nBin+1] | hPlMod[3][nBin2] | hPlMod[3][nBin2+1];
        }
      }
      if( binM1==0 && binM2==0 && binM3==0 && binM4==0) continue;
      Int_t color=0;
      if(type==1) {
        if(binM4) color=HMdcBArray::getNSet(binM4)+18;
        else if(binM3) color+=HMdcBArray::getNSet(binM3)+12;
        else if(binM2) color+=HMdcBArray::getNSet(binM2)+6;
        else if(binM1) color+=HMdcBArray::getNSet(binM1);

// Int_t m1=HMdcBArray::getNSet(binM1);
// Int_t m2=HMdcBArray::getNSet(binM2);
// color=(m1>m2) ? m2:m1; //m1:m2;
	
      } else if(type==2) {
        if(binM4) color=4;
        else if(binM3) color=3;
        else if(binM2) color=2;
        else if(binM1) color=1;
      } else {
        color=HMdcBArray::getNSet(binM1)+HMdcBArray::getNSet(binM2)+
              HMdcBArray::getNSet(binM3)+HMdcBArray::getNSet(binM4);
      }
      hist->Fill(xBinsPos[nx],yBinsPos[ny], color);
    }
  }
  for(Int_t mod=0;mod<4;mod++) clearPrMod(mod);
  return hist;
}

Bool_t HMdcLookUpTb::calcPrPlane(Int_t sec){
  //  Calc. of projection plane:
  //  It's a plane between first and last MDC in sector
  //  (without magnetic field) or in inner segment (with magnetic fild). 
  //  par=0.0 -> plane=layer 6 of first MDC
  //  par=1.0 -> plane=layer 1 of last MDC
  //  If there is one MDC only - middle plane of MDC will used.
  Int_t firstMod=-1;
  Int_t lastMod=0;
  Int_t nMaxMods=(isCoilOff) ? 4:2;
  Int_t nmod=0;
  const Int_t* nLM=HMdcTrackDSet::getTrFnNLayersInMod()+sec*4;
  for(Int_t m=0; m<nMaxMods; m++) {
    if(fMdcDet->getModule(sec,m) && nLM[m]>0) {
      nmod++;
      if(firstMod<0) firstMod=m;
      lastMod=m;
    }
  }
  if(nmod==0) return kFALSE;
  if(nmod>1) {
    //  Double_t par=0.0;   // <<<===================== use MDC1
    //  Double_t par=1.0;   // <<<===================== use MDC2
    Double_t par=0.553; //0.551232;
    HMdcPlane m1((*fSizesCells)[sec][firstMod][5]);
    HMdcPlane m2((*fSizesCells)[sec][ lastMod][0]);
    Double_t dNew=m1.D()*(1.-par)+m2.D()*par;
    Double_t bNew=dNew/(m1.D()*(1.-par)/m1.B()+m2.D()*par/m2.B());
    ((*this)[sec]).prPlane.setPlanePar( 0., bNew, 1., dNew);
    if(!quietMode) printf(
       "\n===> Sec.%i: Using plane between MDC%i and MDC%i (p=%f) as projection plane\n",
        sec+1,firstMod+1,lastMod+1,par);
  } else {
    ((*this)[sec]).prPlane.setPlanePar((*fSizesCells)[sec][firstMod]);
    if(!quietMode) printf(
        "\n===> Sec.%i: Using middle plane of MDC%i as projection plane\n",
        sec+1,firstMod+1);
  }
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcTarget(Int_t sec){
  // Calculation of target parameters
  HMdcSizesCellsSec& fSCSec=(*fSizesCells)[sec];
  HMdcLookUpTbSec& fsec=(*this)[sec];
  if(&fSCSec==0 || &fsec==0) return kFALSE;
  fsec.targVc[0] = fSCSec.getTargetFirstPoint();
  fsec.targVc[1] = fSCSec.getTargetLastPoint();
  fsec.targVc[0].setZ(fsec.targVc[0].getZ() - targLenInc[0]);
  fsec.targVc[1].setZ(fsec.targVc[1].getZ() + targLenInc[1]);
  for(Int_t i=0;i<3;i++) {
    fsec.target[i] = (fsec.targVc[0](i)+fsec.targVc[1](i))*0.5;
    fsec.eTarg[i]  = fabs(fsec.targVc[0](i)-fsec.targVc[1](i))*0.5;
  }
  return kTRUE;
}

Bool_t HMdcLookUpTb::calcPlotSize(Int_t sec) {
  HMdcLookUpTbSec& fsec=(*this)[sec];
  Double_t xLow,xUp,yLow,yUp;
  if(!fsec.calcXYBounds(xLow,xUp,yLow,yUp)) return kFALSE;

  Double_t del = (xUp-xLow)*0.02;
  xLow -= del;
  xUp  += del;
  del   = (yUp-yLow)*0.01;
  yLow -= del;
  yUp  += del;
  Double_t xStep = (xUp-xLow)/(Double_t)(fsec.getNBinX()-3);
  Double_t yStep = (yUp-yLow)/(Double_t)(fsec.getNBinY()-3);
  yLow -= yStep*1.5;            //must be bin=0 at limits of plot
  yUp  += yStep*1.5;
  xLow -= xStep*1.5;
  xUp  += xStep*1.5;
  
  fsec.setPrPlotSize(xLow,xUp,yLow,yUp);
  return kTRUE;
}

void HMdcLookUpTb::donotFillTrackList(void) {
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].fillTrackList=kFALSE;
}

void HMdcLookUpTb::setTargLenInc(Double_t lf,Double_t rt) {
  targLenInc[0] = lf;
  targLenInc[1] = rt;
}

void  HMdcLookUpTb::setTypeClFinder(Int_t type) {
  for(Int_t s=0;s<6;s++) if((*array)[s]) (*this)[s].typeClFinder=type;
}
