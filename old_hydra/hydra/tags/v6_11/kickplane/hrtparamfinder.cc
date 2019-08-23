#include "hrtparamfinder.h"
#include <TNtuple.h>
#include <TFile.h>
#include <iostream.h>

Int_t HRtMatchCell::Compare(const TObject *obj) const {
  const HRtMatchCell *cell = dynamic_cast<const HRtMatchCell *>(obj);
  Float_t diff = getSignalBackground() - cell->getSignalBackground();
  Int_t r = 0;

  if (diff<0.) r = -1;
  else if (diff>0.) r = +1;
  
  return r;
}

//////////////////////////////////////////////////////////

HRtMatchGrid::HRtMatchGrid(void) {
  fCells = 0;
  fMin = fMax = fBinSize = 0;
  fBins = 0;
}

HRtMatchGrid::~HRtMatchGrid(void) {
  delete[] fCells;
  delete[] fMin;
  delete[] fMax;
  delete[] fBinSize;
  delete[] fBins;
}

void HRtMatchGrid::setDimension(Int_t n,Int_t bins[]) {
  fDimension = n;

  //Reallocate memory memory
  delete[] fMin;     fMin = new Float_t[fDimension];
  delete[] fMax;     fMax = new Float_t[fDimension];
  delete[] fBinSize; fBinSize = new Float_t[fDimension];
  delete[] fBins;    fBins = new Int_t[fDimension];

  fTotalBins = 1;
  for (int i=0;i<n;i++) {
    fBins[i] = bins[i];
    fTotalBins *= fBins[i];
  }
  fSize = fTotalBins + 1;
  delete[] fCells;
  fCells = new HRtMatchCell[fSize];
}

void HRtMatchGrid::copyTo(TObjArray &list) {
  list.Expand(fTotalBins);
  for (unsigned int i=0;i<fTotalBins;i++) list.AddAt(&fCells[i],i);
}

void HRtMatchGrid::setRange(UInt_t i,Float_t min, Float_t max) {
  if (i<fDimension) {
    fMin[i] = min;
    fMax[i] = max;
    fBinSize[i] = (max - min) / fBins[i];
  } else Warning("setRange","Index too large");
}


HRtMatchCell &HRtMatchGrid::bin(Int_t idx[]) {
  UInt_t address = idx[0];

  if (idx[0]<fBins[0]) {
    for (unsigned int i=1;i<fDimension;i++) {
      if (!(idx[i]<fBins[i])) {
	address = fTotalBins;
	break;
      }
      address = address*fBins[i] + idx[i];
    }
  } else address = fTotalBins;

  return fCells[address];
}

HRtMatchCell &HRtMatchGrid::bin(Float_t a[]) {
  Int_t idx[fDimension];
  
  for (unsigned int i=0;i<fDimension;i++) {
    if (a[i] < fMin[i]) {
      Error("bin","Out of bounds");
      return fCells[fTotalBins];
    } else {
      idx[i] = int(floor((a[i]-fMin[i])/fBinSize[i]));
    }
  }    
  return bin(idx);
}

HRtMatchingPar *HRtMatchGrid::syntetize(void) {
  HRtMatchingPar *r = new HRtMatchingPar();

  //setup container
  r->setDimension(fDimension,fBins);
  for (unsigned int i=0;i<fDimension;i++) {
    r->setRange(i,fMin[i],fMax[i]);
  }

  //copy data
  for (unsigned int i=0;i<fTotalBins;i++) 
    r->bin(i) = fCells[i].weight;
  r->bin(fTotalBins) = 0;
  
  return r;
}
////////////////////////////////////////////////////////
HRtParamFinder::HRtParamFinder(void) {
  bnValid  = "valid";
  fTotalGood = 0;
  bnVar = 0;
  lsVar = 0;
}

HRtParamFinder::~HRtParamFinder(void) {
  delete[] bnVar;
  delete[] lsVar;
}

void HRtParamFinder::setDimension(Int_t n,...) {
  va_list ap;
  Int_t *bins = new Int_t[n];

  va_start(ap,n);
  for (int i=0;i<n;i++) {
    bins[i] = va_arg(ap,int);
  }
  va_end(ap);
  
  setDimensionVector(n,bins);
  
  delete[] bins;
}

void HRtParamFinder::setDimensionVector(Int_t n, Int_t bins[]) {
  fDimension = n;

  fParamSpace.setDimension(n,bins);
  fParamSpace.copyTo(fCellList);
  
  delete[] bnVar;
  delete[] lsVar;
  bnVar = new TString[fDimension];
  lsVar = new Float_t[fDimension];
}

void HRtParamFinder::setRange(UInt_t i,Text_t name[],
			      Float_t min, Float_t max) {
  if (i<fDimension) {
    fParamSpace.setRange(i,min,max);
    bnVar[i] = name;
  } else Warning("setRange","Index too large");
}

void HRtParamFinder::setValidRange(Text_t name[], Float_t min, Float_t max) { 
  fMinValid = min; 
  fMaxValid = max; 
  bnValid = name;
}

void HRtParamFinder::loadFile(Text_t fileName[],Int_t maxEntries) {
  TFile f(fileName);
  TNtuple *tuple = (TNtuple *)f.Get(stTupleName.Data());
  Stat_t entries = 0;
  Float_t valid;
  Int_t totalFakes=0;

  if (!tuple) {
    Error("loadFile","Tuple %s not found in input file",stTupleName.Data());
  }

  for (unsigned int i=0; i<fDimension;i++) {
    tuple->SetBranchAddress(bnVar[i].Data(), &lsVar[i]);
  }
  tuple->SetBranchAddress(bnValid.Data(), &valid);

  entries = tuple->GetEntries();
  if (entries > maxEntries) entries = maxEntries;
  for (int i=0;i<entries; i++) {
    tuple->GetEntry(i);
    for (unsigned int j=0; j<fDimension; j++) lsVar[j] = fabs(lsVar[j]);

    HRtMatchCell &cell = fParamSpace.bin(lsVar);
    
    if (valid>fMinValid && valid<fMaxValid) {
      cell.nGood++;
      fTotalGood++;
    } else {
      cell.nFakes++;
      totalFakes++;
    }
  }

  totalFakes -= fParamSpace.getLastBin().nFakes;

  cout << entries << " read with a total of " << fTotalGood;
  cout << " good tracks and " << totalFakes << " fakes" << endl;

  fCellList.Sort(); //Sort cells in SignalBackground increasing order
}

void HRtParamFinder::computeWeights(Float_t targetEfficiency) {
  TIter next(&fCellList,kIterBackward);  //Iterate from large S/B to smaller
  Float_t efficiency=0.,noise=0.;
  Int_t accumGood=0, accumFakes=0;
  HRtMatchCell *cell=0;

  if (targetEfficiency>0.) {
    while (efficiency < targetEfficiency && (cell=(HRtMatchCell *)next())!=0) {
      accumGood += cell->nGood;
      accumFakes += cell->nFakes;
      efficiency = float(accumGood) / float(fTotalGood);
      cell->weight = 1;
    }
  } else {
    while ((cell=(HRtMatchCell *)next())!=0) {
      if (cell->getSignalBackground()>=1.) {
	accumGood += cell->nGood;
	accumFakes += cell->nFakes;
	efficiency = float(accumGood) / float(fTotalGood);
	cell->weight = 1;
      }
    }
  }
  while ((cell=(HRtMatchCell *)next())!=0) cell->weight = 0;

  noise = float(accumFakes) / float(accumGood + accumFakes);

  cout << "Target efficiency was: " << targetEfficiency << endl;
  cout << "Achieved efficiency:   " << efficiency << " = ";
  cout << accumGood << "/" << fTotalGood << endl;
  cout << "Noise ratio:           " << noise << " = ";
  cout << accumFakes << "/( " << accumFakes << " + " << accumGood << ")"<<endl;
}



////////////////////////////////////////////////
////////////////////////////////////////////////

ClassImp(HRtParamFinder)
ClassImp(HRtMatchGrid)
ClassImp(HRtMatchCell)
