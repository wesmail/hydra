#include "hrtfunctional.h"
#include "hsavitzkygolay.h"
#include "hruntimedb.h"
#include "TFile.h"
#include "TMath.h"

#undef FREEZE_RHO_ZETA
#undef LOCAL_GRID_FILE
#undef RTFUNC_DEBUG

/************************************************************/
/*                    HRTFUNCTIONAL                         */
/************************************************************/

HRtFunctional::HRtFunctional(void) {
  fSavGol = 0;
  fPoint = 0;
  fDeg2Rad = TMath::Pi()/180.;
}

HRtFunctional::~HRtFunctional(void) {
}

Bool_t HRtFunctional::init(void) {
  //Read reference trajectories data
#ifdef LOCAL_GRID_FILE
  TFile f("reftgrid.root");
  fCurrentGrid = (HRtGrid *)gFile->Get("reftgrid_el");
#else
  fData = (HRtData *)HRuntimeDb::instance()->getContainer("RtData");
  if (!fData) return kFALSE;
  fCurrentGrid = fData->getGrid(1);
#endif

  fPolinomPar = (HRtSavGolPar *)HRuntimeDb::instance()->
    getContainer("RtSavGolPar");
  if (!fPolinomPar) return kFALSE;

  return kTRUE;
}

Bool_t HRtFunctional::reinit(void) {
  if (fPolinomPar->hasChanged()) {
    delete fSavGol;
    fSavGol = new HRtSavGolManager(fPolinomPar->getNLeft(),
		fPolinomPar->getNRight(),
		1,
		fPolinomPar->getM());
  }
  return kTRUE;
}

Bool_t HRtFunctional::setPoint(HRtVector &p,Int_t pol) {
  //Sets the evaluation point. Returns kFALSE if the functional is 
  //not known at point p. (p is out of the parametrized region)
  //
  //WARNING: This function modifies the argument. Snaping it to
  //the closest point in the grid.

  //FIXME: Optimize so pp doesn't need to be recalculated every time
  //remove need to convert to grad and move left/right resol. to HRtGrid
  HRtVector pp(kRtParameterDim);

#ifndef LOCAL_GRID_FILE
  fCurrentGrid = fData->getGrid(pol);
#endif
  
  pp(0) = p.at(0);
  pp(1) = p.at(1);
  pp(2) = p.at(2);
  pp(3) = p.at(3)  - 90;//*fDeg2Rad;
  pp(4) = p.at(4);
  if (pp(3)>0) {
    fSectorSide = kRight;
  } else {
    fSectorSide = kLeft;
    pp(3) = -pp(3);
    pp(2) = -pp(2);
  }

  fPointValue = &(fCurrentGrid->bin(pp));
  fPoint = fCurrentGrid->getCurrentBin();

  fCurrentGrid->fillCurrentBinCoord(pp);
  p(0) = pp(0);
  p(1) = pp(1);
  p(4) = pp(4);
  if (fSectorSide == kRight) {
  //  cout << "Right side" << endl;
    p(3) = pp(3) + 90;//*fDeg2Rad;
    p(2) = pp(2);
  } else {
   // cout << "Left side" << endl;
    p(3) = -pp(3) + 90;//*fDeg2Rad;
    p(2) = -pp(2);
  }

#ifdef RTFUNC_DEBUG 
  std::cout << "Internal coord "; 
  for (Int_t i=0;i<5;i++) std::cout << fPoint[i] << ", ";
  std::cout << std::endl;
#endif
  return (fCurrentGrid->getStatus() == HRtGrid::kOk);
}

Float_t HRtFunctional::partialDerivative(Int_t pi,Int_t pj) {
  // Calculates the partial derivative of the functional with 
  // respect to the pj component of p and evaluates it on p.
  Float_t sum = 0;
  Int_t temp = fPoint[pj];
  Int_t idx;
  Int_t nl,nr;
  Float_t parity = 1;
  Float_t iParity = 1;
  Float_t result = 0.;
  const HRtVector *sgCoef; //Savitzky-Golay coefficients

  //If left half sector and coordinate is x_i (even)
  if (fSectorSide == kLeft) {
    if ( (pi & 1)==0) parity = -1;
  }
  
  //FIXME: implement case when we are in the edges
  //FIXME: para golay asimetrico hay q cambiar el significad
  //de izqda y dcha dep. del semisector
  #ifdef FREEZE_RHO_ZETA
    if (pj==1 || pj==2) return 0;
  #endif

  //How many points to the left can be used to calc. the derivative?
  for (nl=1; nl<=fPolinomPar->getNLeft(); nl++) {
    idx = temp - nl;
    if (idx<0) {
      if (pj==3) idx=-idx-1;
      else break; //Point is not valid
    }
    fPoint[pj] = idx;
    HRtMeasurement &m = fCurrentGrid->bin(fPoint);
    if (fCurrentGrid->getStatus() != HRtGrid::kOk ||
	fabs(m[pi]) > 5000.) { //Point is not valid
      break;
    }
  }
  nl-=1;
  //How many points to the right can be used to calc. the derivative?
  for (nr=1; nr<=fPolinomPar->getNRight(); nr++) {
    fPoint[pj] = temp + nr;
    HRtMeasurement &m = fCurrentGrid->bin(fPoint);
    if (fCurrentGrid->getStatus() != HRtGrid::kOk ||
	fabs(m[pi]) > 5000.) { //Point is not valid
      break;
    }
  }
  nr-=1;

  idx = temp-nl;
  if (pj==3) { //Phi coordinate, apply simmetry
    if (fSectorSide == kRight) sgCoef=fSavGol->getCoefficients(nl,nr);
    else sgCoef=fSavGol->getCoefficients(nr,nl);
    if (sgCoef) {
      for (Int_t n=0;n<nl+nr+1;n++,idx++) {
	if (idx<0) {
	  fPoint[pj]=-idx-1;
	  if ( (pi & 1) == 0)  iParity = -parity;
	} else {
	  fPoint[pj]=idx;
	  iParity = parity;
	}
	HRtMeasurement &m=fCurrentGrid->bin(fPoint);
	if (fCurrentGrid->getStatus() != HRtGrid::kOk) {
	  result = 0;
	  break;
	  //fPoint[pj]=temp;
	  //return 0.;
	}

	if (fSectorSide == kRight)
	  sum += sgCoef->at(n) * m[pi] * iParity; //FIXME: Recalc. Parity
	else
	  sum += sgCoef->at(nl+nr-n) * m[pi] * iParity; //FIXME: Recalc. Parity
      }
    }
    //sum *= parity; //for pj==3 derivative sign has to be changed
  } else {
    sgCoef = fSavGol->getCoefficients(nl,nr);
    if (sgCoef) {
      for (Int_t n=0;n<nl+nr+1;n++,idx++) {
	fPoint[pj]=idx;
	HRtMeasurement &m=fCurrentGrid->bin(fPoint);
	if (fCurrentGrid->getStatus() != HRtGrid::kOk) {
	  result = 0.;
	  break;
	  //fPoint[pj]=temp;
	  //return 0.;
	}
	sum += sgCoef->at(n) * m[pi] * parity;
      }
    }
  }
  result = sum / fCurrentGrid->getBinSize(pj);
  fPoint[pj] = temp;
  return result;
}

void HRtFunctional::calcDerivatives(HRtMatrix &result) {
  // Calculates the partial derivatives of the functional 
  // at point p and stores the result in the matrix "result". 
  // p is snaped to the closest point in the grid.
  for (Int_t i=0;i<result.getRows();i++)
    for (Int_t j=0;j<result.getCols();j++)
      result(i,j) = partialDerivative(i,j);
}

void HRtFunctional::calcValue(HRtVector &result) {
  // Calculates the value of the functional at p and stores the 
  // result in "result". p is snaped to the closest point in the 
  // grid.
  if (fSectorSide == kRight) {
    for (Int_t i=0;i<4;i++) {
      result(i<<1) = fPointValue->operator[](i<<1);
      result((i<<1) | 1) = fPointValue->operator[]((i<<1) | 1);
    }
  } else {
    for (Int_t i=0;i<4;i++) {
      result(i<<1) = -fPointValue->operator[](i<<1);
      result((i<<1) | 1) = fPointValue->operator[]((i<<1) | 1);
    }
  }
}


ClassImp(HRtFunctional)
