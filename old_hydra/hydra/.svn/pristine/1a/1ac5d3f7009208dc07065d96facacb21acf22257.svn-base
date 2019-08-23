//*-- Author : M. Sanchez (06.2000)

/////////////////////////////////////////////////////
//HKickGrid
//
//    A grid
/////////////////////////////////////////////////////

#include "hkickgrid.h"
#include <TMath.h>
#include <TString.h>

HKickGrid::HKickGrid(Float_t xMin,Float_t xMax,Float_t yMin,Float_t yMax,
		     Float_t gran) {
  fMinX=xMin;
  fMinY=yMin;
  fMaxX=xMax;
  fMaxY=yMax;
  fGranularity=gran;
  init();
}

void HKickGrid::init(void) {
  fDimX=calcDimX();
  fDimY=calcDimY();
  fValues=0;
  fValues=new Float_t[fDimX*fDimY];
  for (Int_t i=0; i<fDimX*fDimY; i++) fValues[i]=0;
}

HKickGrid::~HKickGrid(void) {
  if (fValues) delete[] fValues;
}

Double_t HKickGrid::getValue(const Double_t x,const Double_t y) {
  Int_t bX = getBinX(x);
  Int_t bY = getBinY(y);
  Double_t r;
  if (bX>=0 && bX<fDimX && bY>=0 && bY<fDimY) {
    Double_t verX = fMinX + bX * fGranularity; //X and y in 
    Double_t verY = fMinY + bY * fGranularity; // vertex
    Double_t t = (x - verX) / fGranularity;
    Double_t u = (y - verY) / fGranularity;

    r = (1 - t) * (1 - u) * bin(bX,bY) + t * (1 - u) * bin(bX+1,bY) +
      t * u * bin(bX+1, bY+1) + (1 -t) * u * bin(bX,bY+1);
  } else {
    //Warning("getValue","Trying to acces data outside grid (%i, %i)",bX,bY);
    r=0.;
  }
  return r;
}

ostream &HKickGrid::print(ostream &out) {
  out << "[KickGrid]" << endl;
  out << fMinX << "\t" << fMaxX << endl;
  out << fMinY << "\t" << fMaxY << endl;
  out << fGranularity << endl;
  for (int i=0; i<getDimX(); i++) {
    for (int j=0; j<getDimY(); j++) 
      out << bin(i,j) << "\t";
    out << endl;
  }
  return out;
}

istream &HKickGrid::read(istream &in) {
  TString title;
  in >> title;
  in >> fMinX >> fMaxX;
  in >> fMinY >> fMaxY;
  in >> fGranularity;
  init();
  for (int i=0; i<getDimX(); i++) {
    for (int j=0; j<getDimY(); j++) 
      in >> bin(i,j);
  }
  return in;
}

////
HKickGridTrainer::HKickGridTrainer(void) {
  fGrid=0;
  fEntries=0;
}

HKickGridTrainer::HKickGridTrainer(HKickGrid *grid) {
  fGrid=grid;
  fEntries=new Int_t[grid->getDimX()*grid->getDimY()];
}

HKickGridTrainer::~HKickGridTrainer(void) {
  if (fEntries) delete[] fEntries;
}

void HKickGridTrainer::addPoint(Double_t x,Double_t y,Double_t val) {
  if (fEntries && fGrid) {
    Int_t binX = fGrid->getBinX(x);
    Int_t binY = fGrid->getBinY(y);
    entries(binX,binY)++;
    fGrid->bin(binX, binY)+=val;
  } else Warning("addPoint","Point not added");
} 

void HKickGridTrainer::endInput(void) {
  if (fEntries) {
    Int_t dimX = fGrid->getDimX();
    Int_t dimY = fGrid->getDimY();
    for (int i=0; i<dimX; i++) {
      for (int j=0; j<dimY; j++) {
	if (entries(i, j)!=0) {
	  fGrid->bin(i,j)/=double(entries(i, j));
	} else {
	  if (entries(TMath::Max(i-1,0), TMath::Max(j-1,0)) != 0)
	    fGrid->bin(i, j) = fGrid->bin(TMath::Max(i-1,0), 
					  TMath::Max(j-1,0));
	  else if (entries(i, TMath::Max(j-1,0)) != 0)
	    fGrid->bin(i, j) = fGrid->bin(i, TMath::Max(j-1,0));
	  else if (entries(TMath::Max(i-1,0), j) != 0)
	    fGrid->bin(i, j) = fGrid->bin(TMath::Max(i-1,0), j);
	  else 
	    Error("endInput","Unrecoverable error at (%i,%i)",i,j);
	}
      }
    }
  }
}

ClassImp(HKickGrid)
ClassImp(HKickGridTrainer)
