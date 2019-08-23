//*-- Author : R.Schicker
//*-- Modified : M.Sanchez (01.06.2000)

#include "hmdchit.h"

//////////////////////////////////////////
// HMdcHit
//
//	This class represents one hit in one of the MDC
//planes. A hit is defined as a piece of a straight line
//reconstructed by a MDC chamber. It is, therefore, the highest
//level of information an individual chamber can provide.
//
//	The coordinate system used in the MDC is the local
//MDC coordinate system. That is, in the z-midplane. For a
//particle moving outwards in the spectrometer, the z-midplane is
//defined at the entrance into the fourth cathode plane. x and y are
//expressed in mm
//
//	xDir,yDir are the components of the three dimensional
//tangent unit vector to the track in the z-midplane, values equal
//to -1 indicate that the direction has not been calculated
//
//      cov is the 5x5 covariance matrix of the fit.
//
// 	iLayer provides a mechanism to know with cells contributed
//to a particular hit and how. This info is retrieved with the
//following functions	
//
// * Int_t getNCells(Int_t layer)
//	Returns the number of cells in layer "layer" which contributed
//	to the hit.
//
// * Int_t getCell(Int_t layer,Int_t idx)
//      For idx=0 returns the cell number of the first fired cell
//      For idx=1 returns the cell number of the second fired cell
//	.... (maximum idx=3) 
//
// * Int_t HMdcHit::getSignId(Int_t layer,Int_t idx)
//	Returns signal id (time/distance) in cell designed by
//	idx (same criterium as before).
//
// * Int_t setSignId(Int_t layer,Int_t cell, Int_t t1,t2=0,t3=0,t4=0)
//	Sets the signal ids and base cell number for layer "layer"
//	Input:
//	  layer --> Number of layer to set
//	  cell  --> Number of the first cell fired in this layer
//	  t1 - t4 --> SignalId for cells: "cell"+0, "cell"+1 ... "cell"+4
//	              Possible values are:
//	                0 --> This cell has not fired
//	                1 --> First signal (time/distance) was used
//	                2 --> Second signal (time/distance) was used   
////////////////////////////////////////////////////////////
UChar_t HMdcHit::nCellsLookUp[256];
UChar_t HMdcHit::cellPosLookUp[4][256];
Bool_t HMdcHit::lookUpsInitialized=kFALSE;    

HMdcHit::HMdcHit(void) {
  if (!lookUpsInitialized) initLookups();
}

HMdcHit::~HMdcHit(void) {
} 

void HMdcHit::Clear(void) {
  x=0.;
  y=0.;
  xDir=0.;
  yDir=0.;
  offset=0.;
  chi2=0.;
  flag=0.;
  cov.Clear();
  for(Int_t i=0;i<6;i++){ 
    iLayer[i]=0;
  }
}

void HMdcHit::initLookups(void) {
  Int_t j=0,shift;
  for (Int_t i=0;i<256;i++) {
    nCellsLookUp[i]=((i & 0x3)?1:0) + (((i>>2) & 0x3)?1:0) + (((i>>4) & 0x3)?1:0) + (((i>>6) & 0x3)?1:0);           
    shift=0; j=0;
    while (shift<4) {
      Int_t k=i>>(2*shift);
      if (k & 0x3)
	{ cellPosLookUp[j][i]=shift+0; shift++; } 
      else if (k & 0xC)
	{ cellPosLookUp[j][i]=shift+1; shift+=2;}
      else if (k & 0x30)
	{ cellPosLookUp[j][i]=shift+2; shift+=3; }
      else if (k & 0xC0)
	{ cellPosLookUp[j][i]=shift+3; shift+=4; }
      else
	{ cellPosLookUp[j][i]=0; shift+=4; }
      j++;
    }
    /*if (i & 0x3)
      cellPosLookUp[i]=0;
    else if (i & 0xC)
      cellPosLookUp[i]=1;
    else if (i & 0x30)
      cellPosLookUp[i]=2;
    else if (i & 0xC0)
      cellPosLookUp[i]=3;
    else
      cellPosLookUp[i]=0;*/
  }
  lookUpsInitialized=kTRUE;     
}


ClassImp(HMdcHit)











