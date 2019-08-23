#include "hrtefficiency.h"
#include <TFile.h>
#include <TNtuple.h>
#include <iostream.h>
#include "hrtparamfinder.h"

HRtEfficiencyMatrix::HRtEfficiencyMatrix(void) {
  fValue = 0;
  fError = 0;
  fBinsTotal = 0;
  fSize = 0;
}

HRtEfficiencyMatrix::~HRtEfficiencyMatrix(void) {
}

Float_t &HRtEfficiencyMatrix::binContent(Float_t p, Float_t theta, Float_t phi) {
  if ( p<fMinP || theta<fMinTheta || phi<fMinPhi) {
//     Warning("binContent","out of bounds (%f, %f, %f) > (%f, %f, %f)",
// 	    p,theta,phi,fMinP, fMinTheta, fMinPhi);
  }

  Int_t binP = int(floor((p - fMinP) /  fBinSizeP));
  Int_t binTheta = int(floor((theta - fMinTheta)) / fBinSizeTheta);
  Int_t binPhi = int(floor((phi - fMinPhi)) / fBinSizePhi);

  return bin(binP,binTheta,binPhi);
}

Float_t &HRtEfficiencyMatrix::bin(UInt_t i, UInt_t j, UInt_t k) {
  if (i<fBinsP && j<fBinsTheta && k<fBinsPhi) {
    return fValue[ (i*fBinsTheta + j) * fBinsPhi + k ];
  } else {
//     Warning("bin","out of bounds (%u, %u, %u) < (%u, %u, %u)",
// 	    i,j,k,fBinsP,fBinsTheta,fBinsPhi);
    return fValue[fBinsTotal];
  }
}

Float_t &HRtEfficiencyMatrix::errBinContent(Float_t p, Float_t theta, 
					    Float_t phi) {
  if ( p<fMinP || theta<fMinTheta || phi<fMinPhi) {
//     Warning("binContent","out of bounds (%f, %f, %f) > (%f, %f, %f)",
// 	    p,theta,phi,fMinP, fMinTheta, fMinPhi);
  }

  Int_t binP = int(floor((p - fMinP) /  fBinSizeP));
  Int_t binTheta = int(floor((theta - fMinTheta)) / fBinSizeTheta);
  Int_t binPhi = int(floor((phi - fMinPhi)) / fBinSizePhi);

  return errBin(binP,binTheta,binPhi);
}

Float_t &HRtEfficiencyMatrix::errBin(UInt_t i, UInt_t j, UInt_t k) {
  if (i<fBinsP && j<fBinsTheta && k<fBinsPhi) {
    return fError[ (i*fBinsTheta + j) * fBinsPhi + k ];
  } else {
//     Warning("bin","out of bounds (%u, %u, %u) < (%u, %u, %u)",
// 	    i,j,k,fBinsP,fBinsTheta,fBinsPhi);
    return fError[fBinsTotal];
  }
}

void HRtEfficiencyMatrix::computePoissonErrors(void) {
  if (fValue == 0) return;
  for (unsigned int i=0;i<fBinsTotal;i++) {
    if (fValue[i] > 0) {
      fError[i] = TMath::Sqrt(fValue[i]);
    } else fError[i] = -1;
  }
}

void HRtEfficiencyMatrix::setDimension(UInt_t binsP,UInt_t binsTheta,UInt_t binsPhi) {
  fBinsP = binsP; fBinsTheta = binsTheta; fBinsPhi = binsPhi;
  fBinsTotal = binsP * binsTheta * binsPhi;
  fSize = fBinsTotal + 1;
  delete[] fValue;
  fValue = new Float_t[fSize];
  fError = new Float_t[fSize];
}

void HRtEfficiencyMatrix::setPRange(Float_t minP,Float_t maxP) {
  fMinP = minP;
  fMaxP = maxP;
  fBinSizeP = (fMaxP - fMinP) / fBinsP;
}

void HRtEfficiencyMatrix::setThetaRange(Float_t minTheta, Float_t maxTheta) {
  fMinTheta = minTheta;
  fMaxTheta = maxTheta;
  fBinSizeTheta = (fMaxTheta - fMinTheta) / fBinsTheta;
}

void HRtEfficiencyMatrix::setPhiRange(Float_t minPhi,Float_t maxPhi) {
  fMinPhi = minPhi;
  fMaxPhi = maxPhi;
  fBinSizePhi = (fMaxPhi - fMinPhi) / fBinsPhi;
}

Bool_t HRtEfficiencyMatrix::isCompatible(HRtEfficiencyMatrix &m) {
  if (fBinsP == m.fBinsP && fBinsTheta == m.fBinsTheta && fBinsPhi == m.fBinsPhi) 
    return kTRUE;
  return kFALSE;
}

Bool_t HRtEfficiencyMatrix::zero(void) {
  if (fValue == 0) return kFALSE;
  for (unsigned int i=0;i<fSize;i++) {
    fValue[i] = 0.;
    fError[i] = -1.;
  }
  return kTRUE;
}

void HRtEfficiencyMatrix::divide(HRtEfficiencyMatrix &num,
				 HRtEfficiencyMatrix &den) {
  if (isCompatible(num) && isCompatible(den)) {
    for (unsigned int i=0;i<fBinsTotal;i++) {
      if (den.fValue[i] != 0 ) {
	fValue[i] = num.fValue[i] / den.fValue[i];
	if (den.fError[i]>=0 && num.fError[i]>=0) {
	  fError[i] = fValue[i] *sqrt( pow(num.fError[i]/num.fValue[i],2) + 
				       pow(den.fError[i]/den.fValue[i],2));
	} else fError[i]=-1;
      } else {
	Warning("divide","Division by zero at %i, %i, %i",
		i/(fBinsTheta*fBinsPhi),
		(i%(fBinsTheta*fBinsPhi)) / fBinsPhi,
		(i%(fBinsTheta*fBinsPhi)) % fBinsPhi);
	fValue[i] = -1;  //FIXME: Deberia ser +inf?
	fError[i] = -1;
      }
    }
  } else {
    Error("divide","Matrixes are not compatible");
    zero();
  }
}

void HRtEfficiencyMatrix::multiply(HRtEfficiencyMatrix &num,
				 HRtEfficiencyMatrix &den) {
  if (isCompatible(num) && isCompatible(den)) {
    for (unsigned int i=0;i<fBinsTotal;i++) {
      fValue[i] = num.fValue[i] * den.fValue[i];
      if (den.fError[i]>=0 && num.fError[i]>=0) {
	fError[i] = fValue[i] * sqrt( pow(num.fError[i]/num.fValue[i],2) + 
				      pow(den.fError[i]/den.fValue[i],2));
      } else fError[i]=-1;
    }
  } else {
    Error("multiply","Matrixes are not compatible");
    zero();
  }
}

Float_t HRtEfficiencyMatrix::sum(void) {
  //Goes only to fBinsTotal because all results are given in the 
  //region of the phase space for which the matrix is defined
  Float_t sum=0.;
  for (unsigned int i=0;i<fBinsTotal;i++) {
    sum += fValue[i];
  }
  return sum;
}

Float_t HRtEfficiencyMatrix::integral(void) {
  //Goes only to fBinsTotal because all results are given in the 
  //region of the phase space for which the matrix is defined
  Float_t sum=0.;
  Int_t n = 0;
  for (unsigned int i=0;i<fBinsTotal;i++) {
    if (fValue[i]>=0) {
      sum += fValue[i];
      n++;
    }
  }
  return sum / float(n);
}

/////////////////////////////////////////////////////////////
///
/////////////////////////////////////////////////////////////
void HRtEfficiencyKit::setMatrixDimension(UInt_t binsP,UInt_t binsTheta,UInt_t binsPhi) {
  fMatchedGood.setDimension(binsP,binsTheta,binsPhi);
  fGeant.setDimension(binsP,binsTheta,binsPhi);
  fMatchedTotal.setDimension(binsP,binsTheta,binsPhi);
  fEfficiency.setDimension(binsP,binsTheta,binsPhi);    
  fCorrection.setDimension(binsP,binsTheta,binsPhi);  
}

void HRtEfficiencyKit::setPRange(Float_t minP,Float_t maxP) {
  fMatchedGood.setPRange(minP,maxP);
  fGeant.setPRange(minP,maxP);
  fMatchedTotal.setPRange(minP,maxP);
  fEfficiency.setPRange(minP,maxP);    
  fCorrection.setPRange(minP,maxP);  
}

void HRtEfficiencyKit::setThetaRange(Float_t minTheta, Float_t maxTheta) {
  fMatchedGood.setThetaRange(minTheta,maxTheta);
  fGeant.setThetaRange(minTheta,maxTheta);
  fMatchedTotal.setThetaRange(minTheta,maxTheta);
  fEfficiency.setThetaRange(minTheta,maxTheta);    
  fCorrection.setThetaRange(minTheta,maxTheta);  
}

void HRtEfficiencyKit::setPhiRange(Float_t minPhi,Float_t maxPhi) {
  fMatchedGood.setPhiRange(minPhi,maxPhi);
  fGeant.setPhiRange(minPhi,maxPhi);
  fMatchedTotal.setPhiRange(minPhi,maxPhi);
  fEfficiency.setPhiRange(minPhi,maxPhi);    
  fCorrection.setPhiRange(minPhi,maxPhi);  
}

Float_t HRtEfficiencyKit::getAverageEfficiency(void) {
  return fEfficiency.integral();
}

Float_t HRtEfficiencyKit::getEfficiency(void) {
  return fMatchedGood.sum() / fGeant.sum();
}

Float_t HRtEfficiencyKit::getAbsoluteEfficiency(void) {
  //Efficiency including 'last bin'. This means it accounts also
  //for the tracks falling outside the space defined in the
  //setRange functions.
  Float_t matched = fMatchedGood.sum() + fMatchedGood.lastBin();
  Float_t geant = fGeant.sum() + fGeant.lastBin();
  return matched / geant;
}

Float_t HRtEfficiencyKit::getNoiseLevel(void) {
  return (fMatchedTotal.sum() - fMatchedGood.sum()) / fMatchedTotal.sum();
}

Float_t HRtEfficiencyKit::getAbsoluteNoiseLevel(void) {
  //noise level including 'last bin'. This means it accounts also
  //for the tracks falling outside the space defined in the
  //setRange functions.
  Float_t matchedTotal = fMatchedTotal.sum() + fMatchedTotal.lastBin();
  Float_t matchedGood = fMatchedGood.sum() + fMatchedGood.lastBin();
  return (matchedTotal - matchedGood) / matchedTotal;
}

void HRtEfficiencyKit::printReport(void) {
  Float_t totalGeant = fGeant.sum();
  Float_t totalMatched = fMatchedTotal.sum();
  Float_t matchedGood = fMatchedGood.sum();

  cout << totalMatched << "(" << matchedGood << "+";
  cout << totalMatched - matchedGood << ") reconstructed tracks out of ";
  cout << totalGeant << " good ones." << endl;
  cout << "_______________________________________________________________";
  cout << endl;
  cout.precision(3);
  cout << "Average efficiency   = " << getAverageEfficiency() << endl;
  cout << "Efficiency           = " << getEfficiency() << endl ;
  cout << "Absolute efficiency  = " << getAbsoluteEfficiency() << endl;
  cout << "Noise level          = " << getNoiseLevel() << endl;
  cout << "Absolute noise level = " << getAbsoluteNoiseLevel() << endl;
}

Bool_t HRtEfficiencyKit::processFile(Text_t fileName[],Stat_t maxEntries) {
  TFile infile(fileName);
  TNtuple *tuple = (TNtuple *)infile.Get(stNtuple.Data());
  Float_t rad2deg = 180/TMath::Pi();

  Float_t geP=-1,geTheta,gePhi;
  Float_t recP,recTheta,recPhi;
  Float_t valid;
  Bool_t isValid;

  //// Clear matrixes
  if ( !( fMatchedGood.zero() && fGeant.zero() && fMatchedTotal.zero())) {
    Error("processFile","Error zeroing matrixes");
    return kFALSE;
  }

  //// Set branc addresses
  tuple->SetBranchAddress(bnGeantMomentum.Data(),&geP);
  tuple->SetBranchAddress(bnGeantTheta.Data(),&geTheta);
  tuple->SetBranchAddress(bnGeantPhi.Data(),&gePhi);
  tuple->SetBranchAddress(bnRecMomentum.Data(),&recP);
  tuple->SetBranchAddress(bnRecTheta.Data(),&recTheta);
  tuple->SetBranchAddress(bnRecPhi.Data(),&recPhi);
  tuple->SetBranchAddress(bnValidity.Data(),&valid);
  preRead(tuple);

  //// Loop on entries
  Stat_t entries = tuple->GetEntries();
  if (entries>maxEntries) entries=maxEntries;
  for (int i=0;i<entries;i++) {
    tuple->GetEntry(i);
    // Translate angles to degrees
    geTheta *= rad2deg;
    gePhi *= rad2deg;
    recTheta *= rad2deg;
    recPhi *= rad2deg;

    //Translate phi from sector coordinates to -30 -> +30
    gePhi -= 90.;
    recPhi -=90.;

    //Sector is symetric. 
    gePhi = TMath::Abs(gePhi);
    recPhi = TMath::Abs(recPhi);

    postRead();
    
    if (valid>fMinValid && valid<fMaxValid) 
      isValid = kTRUE;
    else
      isValid = kFALSE;
    
    if (isReconstructed) {
      // Note the bin corresponding to the reconstructed values is filled
      // rather than the one correspondin to the Geant values.
      fMatchedTotal.binContent(recP,recTheta,recPhi) += 1.0;
    }

    if (isValid) {
      fGeant.binContent(geP,geTheta,gePhi) += 1.0;
      if (isReconstructed) fMatchedGood.binContent(geP,geTheta,gePhi) += 1.0;
    }
  }
  
  fMatchedGood.computePoissonErrors();
  fMatchedTotal.computePoissonErrors();
  fGeant.computePoissonErrors();

  fEfficiency.divide(fMatchedGood,fGeant);
  fCorrection.divide(fGeant,fMatchedTotal);
  
  return kTRUE;
}

TGraph *HRtEfficiencyKit::plotEff(TString &var, Float_t var2, Float_t var3) {
  return plot(fEfficiency,var,var2,var3);
}

TGraph *HRtEfficiencyKit::plotCor(TString &var, Float_t var2, Float_t var3) {
  return plot(fCorrection,var,var2,var3);
}

TGraph *HRtEfficiencyKit::plot(HRtEfficiencyMatrix &matrix, TString &var,
			       Float_t var2,Float_t var3) {
  
  TGraph *gr =0;
  UInt_t nPoints = 0;
  Float_t *lsVar = 0;
  Float_t *lsVal = 0;
  Float_t *lsVarErr = 0;
  Float_t *lsValErr = 0;
  UInt_t i=0,j=0,k=0;

  if (var == "p") {
    nPoints = matrix.getBinsP();
    lsVar = new Float_t[nPoints];
    lsVal = new Float_t[nPoints];
    lsVarErr = new Float_t[nPoints];
    lsValErr = new Float_t[nPoints];

    j = matrix.getBinTheta(var2);
    k = matrix.getBinPhi(var3);
    for (i=0; i < nPoints; i++) {
      lsVar[i] = matrix.getP(i);
      lsVal[i] = matrix.bin(i,j,k);
      lsVarErr[i] = 0.;
      lsValErr[i] = matrix.errBin(i,j,k);
    }

  } else if (var == "theta") {
    nPoints = matrix.getBinsTheta();
    lsVar = new Float_t[nPoints];
    lsVal = new Float_t[nPoints];
    lsVarErr = new Float_t[nPoints];
    lsValErr = new Float_t[nPoints];

    k = matrix.getBinPhi(var2);
    i = matrix.getBinP(var3);
    for (j=0; j < nPoints; j++) {
      lsVar[j] = matrix.getTheta(j);
      lsVal[j] = matrix.bin(i,j,k);
      lsVarErr[j] = 0.;
      lsValErr[j] = matrix.errBin(i,j,k);
    }

  } else if (var == "phi" ) {
    nPoints = matrix.getBinsPhi();
    lsVar = new Float_t[nPoints];
    lsVal = new Float_t[nPoints];
    lsVarErr = new Float_t[nPoints];
    lsValErr = new Float_t[nPoints];

    i = matrix.getBinP(var2);
    j = matrix.getBinTheta(var3);
    for (k=0; k < nPoints; k++) {
      lsVar[k] = matrix.getPhi(k);
      lsVal[k] = matrix.bin(i,j,k);
      lsVarErr[k] = 0.;
      lsValErr[k] = matrix.errBin(i,j,k);
    }
  }

  if (lsVar) {
    gr = new TGraphErrors(nPoints,lsVar,lsVal,lsVarErr,lsValErr);
    gr->SetMinimum(0.);
    gr->SetMarkerStyle(kCircle);
    gr->SetMarkerColor(kBlue);
  }

  delete[] lsVar;
  delete[] lsVal;
  delete[] lsVarErr;
  delete[] lsValErr;

  return gr;
}

TGraph *HRtEfficiencyKit::plotEff(TString &var) {
  return plot(fEfficiency,var);
}

TGraph *HRtEfficiencyKit::plotCor(TString &var) {
  return plot(fCorrection,var);
}

TGraph *HRtEfficiencyKit::plot(HRtEfficiencyMatrix &matrix, TString &var) {
  char buffer[256];
  sprintf(buffer,"Eff vs %s",var.Data());
  TGraphErrors *grEff = 0;
  UInt_t i,j,k;
  Float_t sum=0,sumerr=0;
  UInt_t nPoints = 0;
  UInt_t n = 0;
  Float_t *lsVar = 0;
  Float_t *lsEff = 0;
  Float_t *lsVarErr = 0;
  Float_t *lsEffErr = 0;

  if (var == "p") {
    nPoints  = matrix.getBinsP();
    lsVar = new Float_t[nPoints];
    lsEff = new Float_t[nPoints];
    lsVarErr = new Float_t[nPoints];
    lsEffErr = new Float_t[nPoints];

    for (i=0;i<matrix.getBinsP();i++) {
      sum = 0.;
      sumerr = 0.;
      n = 0;

      for (j=0;j<matrix.getBinsTheta();j++) {
	for (k=0; k<matrix.getBinsPhi(); k++) {
	  if (matrix.bin(i,j,k)>0) {
	    sum += matrix.bin(i,j,k);
	    sumerr += matrix.errBin(i,j,k) * matrix.errBin(i,j,k);
	    n++;
	  }
	}
      }
      lsVar[i] = matrix.getP(i);
      lsEff[i] = sum / float(n);
      lsVarErr[i] = 0.;
      lsEffErr[i] = sqrt(sumerr) / float(n);
    }

  } else if (var == "theta") {
    nPoints = matrix.getBinsTheta();
    lsVar = new Float_t[nPoints];
    lsEff = new Float_t[nPoints];
    lsVarErr = new Float_t[nPoints];
    lsEffErr = new Float_t[nPoints];
    for (j=0; j<matrix.getBinsTheta();j++) {
      sum = 0.;
      sumerr = 0.;
      n = 0;
      for (i=0; i<matrix.getBinsP();i++) {
	for (k=0; k<matrix.getBinsPhi();k++) {
	  if (matrix.bin(i,j,k)>0) {
	    sum += matrix.bin(i,j,k);
	    sumerr += matrix.errBin(i,j,k) * matrix.errBin(i,j,k);
	    n++;
	  }
	}
      }
      lsVar[j] = matrix.getTheta(j);
      lsEff[j] = sum / float(n);
      lsVarErr[j] = 0.;
      lsEffErr[j] = sqrt(sumerr) / float(n);
    }
    
  } else if (var == "phi") {
    nPoints = matrix.getBinsPhi();
    lsVar = new Float_t[nPoints];
    lsEff = new Float_t[nPoints];
    lsVarErr = new Float_t[nPoints];
    lsEffErr = new Float_t[nPoints];
    for (k=0; k<matrix.getBinsPhi(); k++) {
      sum = 0.;
      sumerr = 0.;
      n = 0;
      for (i=0; i<matrix.getBinsP(); i++) {
	for (j=0; j<matrix.getBinsTheta(); j++) {
	  if (matrix.bin(i,j,k)>0) {
	    sum += matrix.bin(i,j,k);
	    sumerr += matrix.errBin(i,j,k) * matrix.errBin(i,j,k);    
	    n++;
	  }
	}
      }
      lsVar[k] = matrix.getPhi(k);
      lsEff[k] = sum / float(n);
      lsVarErr[k] = 0.;
      lsEffErr[k] = sqrt(sumerr) / float(n);
     }
  }

  if (lsVar) {
    grEff = new TGraphErrors(nPoints,lsVar,lsEff,lsVarErr,lsEffErr);
    grEff->SetMinimum(0.);
    grEff->SetMarkerStyle(kCircle);
    grEff->SetMarkerColor(kBlue);
  }

  delete[] lsVar;
  delete[] lsEff;
  delete[] lsVarErr;
  delete[] lsEffErr;

  return grEff;
}

////////////////////////////////////////////////////////////////////////
///HRtEfficiencyKitMeta
////////////////////////////////////////////////////////////////////////
HRtEfficiencyKitMeta::HRtEfficiencyKitMeta(void) {
  bnGeantMomentum = "p"; //Branch name
  bnGeantTheta    = "geTheta";   
  bnGeantPhi      = "gePhi";
  bnValidity      = "valid";
  bnRecMomentum   = "prec";
  bnRecTheta      = "recTheta";
  bnRecPhi        = "recPhi";
  stNtuple        = "metamatcheff"; 
  fMinValid=.98;
  fMaxValid=1.1;
}

void HRtEfficiencyKitMeta::preRead(TNtuple *tuple) {
  tuple->SetBranchAddress("d",&fD);
  tuple->SetBranchAddress("dkick",&fDKick);
  tuple->SetBranchAddress("dphi",&fDPhi);
  tuple->SetBranchAddress("det",&fXPull);
  tuple->SetBranchAddress("dPnorm",&fDPnorm);
}

void HRtEfficiencyKitMeta::postRead(void) {
  isReconstructed = kFALSE;

  if ( fD < fMaxD && fDKick < fMaxDKick && fabs(fDPhi) < fMaxDPhi &&
       fabs(fXPull) < fMaxXPull && fabs(fDPnorm) < fMaxDPnorm) {
    isReconstructed = kTRUE;
  }
}

void HRtEfficiencyKitMeta::setCutValues(Float_t d,Float_t dkick,Float_t dPhi,
					Float_t xPull,Float_t dPnorm) {
  fMaxD = d;
  fMaxDKick = dkick;
  fMaxDPhi = dPhi;
  fMaxXPull = xPull;
  fMaxDPnorm = dPnorm;
}

////////////////////////////////////////////////////////////////////////
///HRtEfficiencyKitMeta2
////////////////////////////////////////////////////////////////////////
HRtEfficiencyKitMeta2::HRtEfficiencyKitMeta2(void) {
  bnGeantMomentum = "p"; //Branch name
  bnGeantTheta    = "geTheta";   
  bnGeantPhi      = "gePhi";
  bnValidity      = "valid";
  bnRecMomentum   = "prec";
  bnRecTheta      = "recTheta";
  bnRecPhi        = "recPhi";
  stNtuple        = "metamatcheff"; 
  fMinValid=.98;
  fMaxValid=1.1;
  fMatchingPar = 0;
}

void HRtEfficiencyKitMeta2::preRead(TNtuple *tuple) {
  tuple->SetBranchAddress("d",&fD);
  tuple->SetBranchAddress("dkick",&fDKick);
  tuple->SetBranchAddress("dphi",&fDPhi);
  tuple->SetBranchAddress("det",&fXPull);
  tuple->SetBranchAddress("dPnorm",&fDPnorm);
}

void HRtEfficiencyKitMeta2::postRead(void) {
  isReconstructed = kFALSE;

  fMatchVar[0] = fabs(fXPull);
  fMatchVar[1] = fabs(fDPnorm);
  fMatchVar[2] = fabs(fDKick);
  fMatchVar[3] = fabs(fDPhi);

  if (fMatchingPar->bin(fMatchVar) != 0) {
    isReconstructed = kTRUE;
  }
}

void HRtEfficiencyKitMeta2::loadParameters(Text_t file[]) {
  TFile f(file,"READ");
  delete fMatchingPar;
  fMatchingPar =(HRtMatchingPar *)f.Get("RtMatchingParMeta");
  fParamFile = file;
}

//////////////////////////////////////////////////////////////////////
///HRtEfficiencyKitMdc
//////////////////////////////////////////////////////////////////////
HRtEfficiencyKitMdc::HRtEfficiencyKitMdc(void) {
  bnGeantMomentum = "pr"; //Branch name
  bnGeantTheta    = "geTheta";   
  bnGeantPhi      = "gePhi";
  bnValidity      = "valid";
  bnRecMomentum   = "pc";
  bnRecTheta      = "recTheta";
  bnRecPhi        = "recPhi";
  stNtuple        = "RtMatching"; 
  fMinValid=.9;
  fMaxValid=1.1;
}

void HRtEfficiencyKitMdc::preRead(TNtuple *tuple) {
  tuple->SetBranchAddress("d",&fD);
  tuple->SetBranchAddress("dKick",&fDKick);
  tuple->SetBranchAddress("dPhi",&fDPhi);
}

void HRtEfficiencyKitMdc::postRead(void) {
  if ( fD < fMaxD && fDKick < fMaxDKick && fabs(fDPhi) < fMaxDPhi) {
    isReconstructed = kTRUE;
  } else
    isReconstructed = kFALSE;
}

void HRtEfficiencyKitMdc::setCutValues(Float_t d,Float_t dkick,Float_t dPhi) {
  fMaxD = d;
  fMaxDKick = dkick;
  fMaxDPhi = dPhi;
}

//////////////////////////////////////////////////////////////////////
///HRtEfficiencyKitMdc2
//////////////////////////////////////////////////////////////////////
HRtEfficiencyKitMdc2::HRtEfficiencyKitMdc2(void) {
  bnGeantMomentum = "pr"; //Branch name
  bnGeantTheta    = "geTheta";   
  bnGeantPhi      = "gePhi";
  bnValidity      = "valid";
  bnRecMomentum   = "pc";
  bnRecTheta      = "recTheta";
  bnRecPhi        = "recPhi";
  stNtuple        = "RtMatching"; 
  fMinValid=.9;
  fMaxValid=1.1;
  fMatchingPar = 0;
}

void HRtEfficiencyKitMdc2::preRead(TNtuple *tuple) {
  tuple->SetBranchAddress("d",&fPar[0]);
  tuple->SetBranchAddress("dKick",&fPar[1]);
  tuple->SetBranchAddress("dPhi",&fPar[2]);
}

void HRtEfficiencyKitMdc2::postRead(void) {
  fPar[2] = fabs(fPar[2]);
  if (fMatchingPar->bin(fPar))
    isReconstructed = kTRUE;
  else
    isReconstructed = kFALSE;
}

void HRtEfficiencyKitMdc2::loadParameters(Text_t file[]) {
  TFile f(file);
  delete fMatchingPar;
  fMatchingPar = (HRtMatchingPar *)f.Get("RtMatchingParMDC3");
  fParamFile = file;
}

/////////////////////////////////////////
ClassImp(HRtEfficiencyMatrix)
ClassImp(HRtEfficiencyKit)
ClassImp(HRtEfficiencyKitMeta)
ClassImp(HRtEfficiencyKitMeta2)
ClassImp(HRtEfficiencyKitMdc)
ClassImp(HRtEfficiencyKitMdc2)
