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
		     Float_t gran, Float_t sim) {
  fMinX=xMin;
  fMinY=yMin;
  fMaxX=xMax;
  fMaxY=yMax;
  fMidY = (fMaxY + fMinY) / 2.;
  fGranularity=gran;
  fSim = sim;
  fValues = 0;
  init();
}

HKickGrid::HKickGrid(HKickGrid *grid) {
  fMinX=grid->fMinX;
  fMinY=grid->fMinY;
  fMaxX=grid->fMaxX;
  fMaxY=grid->fMaxY;
  fMidY = (fMaxY + fMinY) / 2.;
  fGranularity=grid->fGranularity;
  fSim = grid->fSim;
  fValues = 0;
  init();
}

void HKickGrid::init(void) {
  if (fValues) delete[] fValues; 
  fDimX=calcDimX();
  fDimY=calcDimY();
  fEntries = fDimX * fDimY;
  fValues=0;
  fValues=new Float_t[fDimX*fDimY];
  for (Int_t i=0; i<fDimX*fDimY; i++) fValues[i]=0.;
}

HKickGrid::~HKickGrid(void) {
  if (fValues) delete[] fValues;
}

void HKickGrid::clear(void) {
  for (Int_t i=0; i<fDimX*fDimY; i++) fValues[i]=0.; 
}

Double_t HKickGrid::getValue(const Double_t x,const Double_t ay) {
  Double_t y = 0.;
  Double_t con = 1.;

  if (ay>fMidY) {
    y =  ay;
    con = 1.;
  } else {
    y = (2*fMidY-ay);
    con = fSim;
  }

  Int_t bY = getBinY(y);
  Int_t bX = getBinX(x);
  Double_t r;
  if (bX>=0 && bX<fDimX && bY>=0 && bY<fDimY) {
    Double_t verX = fMinX + bX * fGranularity; //X and y in 
    Double_t verY = fMidY + bY * fGranularity; // vertex
    Double_t t = (x - verX) / fGranularity;
    Double_t u = (y - verY) / fGranularity;

    r = bin(bX,bY);
//     r = (1 - t) * (1 - u) * bin(bX,bY) + t * (1 - u) * bin(bX+1,bY) +
//       t * u * bin(bX+1, bY+1) + (1 -t) * u * bin(bX,bY+1);
  } else {
    //Warning("getValue","Trying to acces data outside grid (%i, %i)",bX,bY);
    r=0.;
  }
  return con * r;
}

ostream &HKickGrid::print(ostream &out) {
  out << "[KickGrid]" << endl;
  out << fMinX << "\t" << fMaxX << endl;
  out << fMinY << "\t" << fMaxY << endl;
  out << fGranularity << endl;
  out << fSim << endl;
  for (int i=0; i<fDimX; i++) {
    for (int j=0; j<fDimY; j++) 
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
  fMidY = (fMinY + fMaxY) / 2.;
  in >> fGranularity;
  in >> fSim; 
  init();
  for (int i=0; i<fDimX; i++) {
    for (int j=0; j<fDimY; j++) 
      in >> bin(i,j);
  }
  return in;
}

void HKickGrid::readFrom(const HKickGrid &grid) {
  fMinX = grid.fMinX; 
  fMaxX = grid.fMaxX;
  fMinY = grid.fMinY;
  fMaxY = grid.fMaxY;
  fMidY = (fMinY + fMaxY) / 2.;
  fGranularity = grid.fGranularity;
  fSim = grid.fSim; 
  init();  
  for (int i=0; i<fDimX; i++) {
    for (int j=0; j<fDimY; j++) 
      bin(i,j) = grid.getBinContents(i,j);
  }  
}

////
HKickGridTrainer::HKickGridTrainer(void) {
  fGrid=0;
  fEntries=0;
}

HKickGridTrainer::HKickGridTrainer(HKickGrid *grid) {
  fGrid=grid;
  fEntries=new HKickGrid(grid);
  fEntries->fSim=1.;
  fControl = new TH2F("con","con",fEntries->fDimX,0,fEntries->fDimX,
		      fEntries->fDimY,0,fEntries->fDimY);
}

HKickGridTrainer::~HKickGridTrainer(void) {
  if (fEntries) delete fEntries;
}

void HKickGridTrainer::reset(void) {
  fGrid->clear();
  fEntries->clear();
}

void HKickGridTrainer::addPoint(Double_t x,Double_t ay,Double_t f) {
 Double_t y = 0.;
 Double_t val = 0;
 if (ay>fGrid->fMidY) {
   y = ay; 
   val = f;
 } else {
   y = 2*fGrid->fMidY-ay;
   val = fGrid->getSim() * f;
 }
  if (fEntries && fGrid) {
    Int_t binX = fGrid->getBinX(x);
    Int_t binY = fGrid->getBinY(y);
    fEntries->bin(binX,binY) += 1.;
    fGrid->bin(binX, binY) += val;
    fControl->Fill(binX,binY);
  } else Warning("addPoint","Point not added");
} 

void HKickGridTrainer::endInput(void) {
  if (fEntries) {
    Int_t dimX = fGrid->fDimX;
    Int_t dimY = fGrid->fDimY;
    for (int i=0; i<dimX; i++) {
      for (int j=0; j<dimY; j++) {
	if (fEntries->bin(i, j) > 0.) {
	  fGrid->bin(i,j) /= fEntries->bin(i,j);
	} else {
	  if (fEntries->bin(TMath::Max(i-1,0), TMath::Max(j-1,0)) != 0)
	    fGrid->bin(i, j) = fGrid->bin(TMath::Max(i-1,0), 
					  TMath::Max(j-1,0));
	  else if (fEntries->bin(i, TMath::Max(j-1,0)) != 0)
	    fGrid->bin(i, j) = fGrid->bin(i, TMath::Max(j-1,0));
	  else if (fEntries->bin(TMath::Max(i-1,0), j) != 0)
	    fGrid->bin(i, j) = fGrid->bin(TMath::Max(i-1,0), j);
	  else 
	    Error("endInput","Unrecoverable error at (%i,%i)",i,j);
	}
      }
    }
  }
}

//------------------------
HKickGridTrainerCor2::HKickGridTrainerCor2(void) {
  fGridA = 0; // The grid being trained
  fGridB = 0;
  fGridC = 0;
  fSx2 = fS1 = fSx = fSyx = fSx_2 = fSx_1 = fSyx_1 = fSy =0;
  fDebug = kFALSE;
}

HKickGridTrainerCor2::HKickGridTrainerCor2(HKickGrid *a, HKickGrid *b,
					   HKickGrid *c) {
  fGridA = a;
  fGridB = b;
  fGridC = c;
  fDebug = kFALSE;

  fSx2 = new HKickGrid(a); 
  fS1 = new HKickGrid(a); 
  fSx = new HKickGrid(a); 
  fSyx = new HKickGrid(a); 
  fSx_2 = new HKickGrid(a); 
  fSx_1 = new HKickGrid(a); 
  fSyx_1 = new HKickGrid(a); 
  fSy = new HKickGrid(a); 
}

void HKickGridTrainerCor2::reset(void) {
  fSx2->clear(); 
  fS1->clear(); 
  fSx->clear(); 
  fSyx->clear(); 
  fSx_2->clear(); 
  fSx_1->clear(); 
  fSyx_1->clear(); 
  fSy->clear();   
}

HKickGridTrainerCor2::~HKickGridTrainerCor2(void) {
  if (fS1) {
  delete fSx2; 
  delete fS1; 
  delete fSx; 
  delete fSyx; 
  delete fSx_2; 
  delete fSx_1; 
  delete fSyx_1; 
  delete fSy;
  }
}

void HKickGridTrainerCor2::addPoint(Double_t x,Double_t ay,
				    Double_t af, Double_t v, Double_t w) {
  Double_t y = 0.;
  Double_t f = 0;
  if (ay>fS1->fMidY) {
    y = ay; 
    f = af;
  } else {
    y = 2*fS1->fMidY-ay;
    f = fS1->getSim() * af;
  }
  if (fSy) {
    Int_t binX = fS1->getBinX(x);
    Int_t binY = fS1->getBinY(y);
    
    fSx2->bin(binX,binY)   += (v*v)     / (w*w); 
    fS1->bin(binX,binY)    += (1)       / (w*w); 
    fSx->bin(binX,binY)    += (v)       / (w*w); 
    fSyx->bin(binX,binY)   += (f*v)     / (w*w); 
    fSx_2->bin(binX,binY)  += (1/(v*v)) / (w*w); 
    fSx_1->bin(binX,binY)  += (1/v)     / (w*w); 
    fSyx_1->bin(binX,binY) += (f/v)     / (w*w); 
    fSy->bin(binX,binY)    += (f)       / (w*w); 
  } else Warning("addPoint","Point not added");
} 

void HKickGridTrainerCor2::endInput(void) {
  if (fS1) {
    Int_t dimX = fGridA->fDimX;
    Int_t dimY = fGridA->fDimY;

    for (int i=0; i<dimX; i++) {
      for (int j=0; j<dimY; j++) {
	if (fS1->bin(i, j) > 0.) {
	  Float_t sx2   = fSx2->bin(i,j); 
	  Float_t s1    = fS1->bin(i,j); 
	  Float_t sx    = fSx->bin(i,j); 
	  Float_t syx   = fSyx->bin(i,j); 
	  Float_t sx_2  = fSx_2->bin(i,j); 
	  Float_t sx_1  = fSx_1->bin(i,j); 
	  Float_t syx_1 = fSyx_1->bin(i,j); 
	  Float_t sy    = fSy->bin(i,j);
	  Float_t m11   = s1 * sx_2 - sx_1 * sx_1;
	  Float_t m12   = sx_1 * sx - s1 * s1;
	  Float_t m13   = s1 * sx_1 - sx_2 * sx;
	  Float_t m22   = sx2 * s1 - sx * sx;
	  Float_t m23   = s1 * sx - sx2 * sx_1;
	  Float_t m33   = sx2 * sx_2 - s1 * s1;
	  Float_t det   = s1*sx2*sx_2 + 2*s1*sx_1*sx - sx*sx*sx_2 -
	    s1*s1*s1 - sx_1*sx_1*sx2;
	  fGridA->bin(i,j) = (m11*syx + m12*syx_1 + m13*sy) / det;
	  fGridB->bin(i,j) = (m12*syx + m22*syx_1 + m23*sy) / det;
	  fGridC->bin(i,j) = (m13*syx + m23*syx_1 + m33*sy) / det;
	} else {
	  if (fS1->bin(TMath::Max(i-1,0), TMath::Max(j-1,0)) != 0) {
	    fGridA->bin(i, j) = fGridA->bin(TMath::Max(i-1,0), 
					    TMath::Max(j-1,0));
	    fGridB->bin(i, j) = fGridB->bin(TMath::Max(i-1,0), 
					    TMath::Max(j-1,0));
	    fGridC->bin(i, j) = fGridC->bin(TMath::Max(i-1,0), 
					    TMath::Max(j-1,0));
	  } else if (fS1->bin(i, TMath::Max(j-1,0)) != 0) {
	    fGridA->bin(i, j) = fGridA->bin(i, TMath::Max(j-1,0));
	    fGridB->bin(i, j) = fGridB->bin(i, TMath::Max(j-1,0));
	    fGridC->bin(i, j) = fGridC->bin(i, TMath::Max(j-1,0));
	  } else if (fS1->bin(TMath::Max(i-1,0), j) != 0) {
	    fGridA->bin(i, j) = fGridA->bin(TMath::Max(i-1,0), j);
	    fGridB->bin(i, j) = fGridB->bin(TMath::Max(i-1,0), j);
	    fGridC->bin(i, j) = fGridC->bin(TMath::Max(i-1,0), j);
	  } else 
	    if (fDebug) Error("endInput","Unrecoverable error at (%i,%i)",i,j);
	}
      }
    }
  }
}

//Correlation 2
HKickGridTrainerPol1::HKickGridTrainerPol1(void) {
  fGridSlope = 0; // The grid being trained
  fGridOffset = 0;
  fEntries = 0;
  fSumX = 0;
  fSumX2 = 0; 
  fSumY = 0;
  fSumXY = 0;
}

HKickGridTrainerPol1::HKickGridTrainerPol1(HKickGrid *slope, HKickGrid *ofs) {
  fGridSlope = slope;
  fGridOffset = ofs;

  fEntries = new HKickGrid(slope);
  fSumX = new HKickGrid(slope);
  fSumX2 = new HKickGrid(slope); 
  fSumY = new HKickGrid(slope);
  fSumXY = new HKickGrid(slope); 
}

HKickGridTrainerPol1::~HKickGridTrainerPol1(void) {
  if (fEntries) {
    delete fEntries;
    delete fSumX;
    delete fSumX2; 
    delete fSumY;
    delete fSumXY; 
  }
}

void HKickGridTrainerPol1::addPoint(Double_t x,Double_t ay,
				    Double_t af, Double_t v, Double_t w) {
  Double_t y = 0.;
  Double_t f = 0;
  if (ay>fEntries->fMidY) {
    y = ay; 
    f = af;
  } else {
    y = 2*fEntries->fMidY-ay;
    f = fGridSlope->getSim() * af;
  }
  if (fEntries && fGridSlope && fGridOffset) {
    Int_t binX = fEntries->getBinX(x);
    Int_t binY = fEntries->getBinY(y);
    
    fEntries->bin(binX, binY) += 1. / (w*w);
    fSumX->bin(binX,binY) += v / (w*w);
    fSumX2->bin(binX, binY) += v*v / (w*w);
    fSumY->bin(binX,binY) += f / (w*w);
    fSumXY->bin(binX, binY) += v*f / (w*w);
  } else Warning("addPoint","Point not added");
} 

void HKickGridTrainerPol1::endInput(void) {
  if (fEntries) {
    Int_t dimX = fGridSlope->fDimX;
    Int_t dimY = fGridSlope->fDimY;

    for (int i=0; i<dimX; i++) {
      for (int j=0; j<dimY; j++) {
	if (fEntries->bin(i, j) > 0.) {
	  fGridOffset->bin(i,j) = 
	    ( fSumY->bin(i,j) * fSumX2->bin(i,j) - 
	      fSumX->bin(i,j) * fSumXY->bin(i,j) ) /
	    ( fEntries->bin(i,j) * fSumX2->bin(i,j) -
	      fSumX->bin(i,j) * fSumX->bin(i,j) );

	  fGridSlope->bin(i,j) =
// 	    ( fSumY->bin(i,j) - fGridOffset->bin(i,j) * fEntries->bin(i,j) ) /
// 	    ( fSumX->bin(i,j) );
 
	    ( fEntries->bin(i,j) * fSumXY->bin(i,j) - 
	      fSumX->bin(i,j) * fSumY->bin(i,j) ) /
	    ( fEntries->bin(i,j) * fSumX2->bin(i,j) - 
	      fSumX->bin(i,j) * fSumX->bin(i,j) );
	} else {
	  if (fEntries->bin(TMath::Max(i-1,0), TMath::Max(j-1,0)) != 0) {
	    fGridSlope->bin(i, j) = fGridSlope->bin(TMath::Max(i-1,0), 
						    TMath::Max(j-1,0));
	    fGridOffset->bin(i, j) = fGridOffset->bin(TMath::Max(i-1,0), 
						      TMath::Max(j-1,0));
	  } else if (fEntries->bin(i, TMath::Max(j-1,0)) != 0) {
	    fGridSlope->bin(i, j) = fGridSlope->bin(i, TMath::Max(j-1,0));
	    fGridOffset->bin(i, j) = fGridOffset->bin(i, TMath::Max(j-1,0));
	  } else if (fEntries->bin(TMath::Max(i-1,0), j) != 0) {
	    fGridSlope->bin(i, j) = fGridSlope->bin(TMath::Max(i-1,0), j);
	    fGridOffset->bin(i, j) = fGridOffset->bin(TMath::Max(i-1,0), j);
	  } else 
	    Error("endInput","Unrecoverable error at (%i,%i)",i,j);
	}
      }
    }
  }
}

//------------------------
HKickGridTrainerPol2::HKickGridTrainerPol2(void) {
  fGridA = 0; // The grid being trained
  fGridB = 0;
  fGridC = 0;
  fSy = fS1 = fSx_2 = fSx_3 = fSx = fSyx_2 = fSx_4 = fSyx = 0;
  fDebug = kFALSE;
}

HKickGridTrainerPol2::HKickGridTrainerPol2(HKickGrid *a, HKickGrid *b,
					   HKickGrid *c) {
  fGridA = a;
  fGridB = b;
  fGridC = c;
  fDebug = kFALSE;

  fSy = new HKickGrid(a);
  fS1 = new HKickGrid(a);
  fSx_2 = new HKickGrid(a);
  fSx_3 = new HKickGrid(a);
  fSx = new HKickGrid(a);
  fSyx_2 = new HKickGrid(a);
  fSx_4 = new HKickGrid(a);
  fSyx = new HKickGrid(a);
}

void HKickGridTrainerPol2::reset(void) {
  fSy->clear();
  fS1->clear();
  fSx_2->clear();
  fSx_3->clear();
  fSx->clear();
  fSyx_2->clear();
  fSx_4->clear();
  fSyx->clear();
}

HKickGridTrainerPol2::~HKickGridTrainerPol2(void) {
  if (fS1) {
    delete fSy;
    delete fS1;
    delete fSx_2;
    delete fSx_3;
    delete fSx;
    delete fSyx_2;
    delete fSx_4;
    delete fSyx;
  }
}

void HKickGridTrainerPol2::addPoint(Double_t x,Double_t ay,
				    Double_t af, Double_t v, Double_t w) {
  Double_t y = 0.;
  Double_t f = 0;
  if (ay>fS1->fMidY) {
    y = ay; 
    f = af;
  } else {
    y = 2*fS1->fMidY-ay;
    f = fS1->getSim() * af;
  }
  if (fSy) {
    Int_t binX = fS1->getBinX(x);
    Int_t binY = fS1->getBinY(y);
    
    fSy->bin(binX,binY)    += (f)	/ (w*w);
    fS1->bin(binX,binY)    += (1)	/ (w*w);
    fSx_2->bin(binX,binY)  += (v*v)	/ (w*w);
    fSx_3->bin(binX,binY)  += (v*v*v)	/ (w*w);
    fSx->bin(binX,binY)    += (v)	/ (w*w);
    fSyx_2->bin(binX,binY) += (f*v*v)	/ (w*w);
    fSx_4->bin(binX,binY)  += (v*v*v*v)	/ (w*w);
    fSyx->bin(binX,binY)   += (f*v)  / (w*w);
  } else Warning("addPoint","Point not added");
} 

void HKickGridTrainerPol2::finalize(void) {
  if (fS1) {
    Int_t dimX = fGridA->fDimX;
    Int_t dimY = fGridA->fDimY; 
    for (int i=0; i<dimX; i++) {
      for (int j=0; j<dimY; j++) { 
	Float_t a = fGridA->bin(i,j);
	Float_t b = fGridB->bin(i,j);
	Float_t c = fGridC->bin(i,j);

	if (fabs(a)>1e-6 || fabs(b)>1e-6 || fabs(c)>1e-6) { //FIXME: make portable
	  fGridA->bin(i,j) = b;
	  fGridB->bin(i,j) = 4*a*c;
	  fGridC->bin(i,j) = 1/(2*c);
	} else {
	  fGridA->bin(i,j) = 0.;
	  fGridB->bin(i,j) = 0.;
	  fGridC->bin(i,j) = 0.;
	}
      }
    }	
  }
}

void HKickGridTrainerPol2::endInput(void) {
  if (fS1) {
    Int_t dimX = fGridA->fDimX;
    Int_t dimY = fGridA->fDimY;

    for (int i=0; i<dimX; i++) {
      for (int j=0; j<dimY; j++) {
	if (fS1->bin(i, j) > 0.) {
	  Float_t sy    = fSy->bin(i,j);
	  Float_t s1    = fS1->bin(i,j);
	  Float_t sx_2  = fSx_2->bin(i,j);
	  Float_t sx_3  = fSx_3->bin(i,j); 
	  Float_t sx    = fSx->bin(i,j);
	  Float_t syx_2 = fSyx_2->bin(i,j);
	  Float_t sx_4  = fSx_4->bin(i,j);
	  Float_t syx   = fSyx->bin(i,j);

	  Float_t det = s1*sx_4*sx_2 + 2*sx*sx_2*sx_3 -
	    sx_2*sx_2*sx_2 - sx*sx*sx_4 - sx_3*sx_3*s1;

	  if (TMath::Abs(det)==0.) {
	    fGridA->bin(i,j) = 0.;
	    fGridB->bin(i,j) = 0.;
	    fGridC->bin(i,j) = 0.;
	  } else {
	    Float_t m11   = sx_4*sx_2 - sx_3*sx_3;
	    Float_t m12   = sx*sx_3 - sx_2*sx_2;
	    Float_t m13   = sx_2*sx_3 - sx_4*sx;
	    Float_t m22   = s1*sx_2 - sx*sx;
	    Float_t m23   = sx_2*sx - s1*sx_3;
	    Float_t m33   = s1*sx_4 - sx_2*sx_2;
	    Float_t a = (m11*sy + m12*syx_2 + m13*syx) / det;
	    Float_t c = (m12*sy + m22*syx_2 + m23*syx) / det;
	    Float_t b = (m13*sy + m23*syx_2 + m33*syx) / det;

	    fGridA->bin(i,j) = a;
	    fGridB->bin(i,j) = b;
	    fGridC->bin(i,j) = c;
	  }
	} else {
	  if (fS1->bin(TMath::Max(i-1,0), TMath::Max(j-1,0)) != 0) {
	    fGridA->bin(i, j) = fGridA->bin(TMath::Max(i-1,0), 
					    TMath::Max(j-1,0));
	    fGridB->bin(i, j) = fGridB->bin(TMath::Max(i-1,0), 
					    TMath::Max(j-1,0));
	    fGridC->bin(i, j) = fGridC->bin(TMath::Max(i-1,0), 
					    TMath::Max(j-1,0));
	  } else if (fS1->bin(i, TMath::Max(j-1,0)) != 0) {
	    fGridA->bin(i, j) = fGridA->bin(i, TMath::Max(j-1,0));
	    fGridB->bin(i, j) = fGridB->bin(i, TMath::Max(j-1,0));
	    fGridC->bin(i, j) = fGridC->bin(i, TMath::Max(j-1,0));
	  } else if (fS1->bin(TMath::Max(i-1,0), j) != 0) {
	    fGridA->bin(i, j) = fGridA->bin(TMath::Max(i-1,0), j);
	    fGridB->bin(i, j) = fGridB->bin(TMath::Max(i-1,0), j);
	    fGridC->bin(i, j) = fGridC->bin(TMath::Max(i-1,0), j);
	  } else 
	    if (fDebug) Error("endInput","Unrecoverable error at (%i,%i)",i,j);
	}
      }
    }
  }
}


ClassImp(HKickGrid)
ClassImp(HKickGridTrainer)
ClassImp(HKickGridTrainerPol1)
ClassImp(HKickGridTrainerPol2)
ClassImp(HKickGridTrainerCor2)
