#include "hrtmatchingpar.h"
#include <math.h>
#include "hruntimedb.h"
#include "hades.h"
#include "hkicktask.h"
#include "hparamlist.h"
#include "hdetpario.h"
#include "hpario.h"
#include <iomanip>

////////////////////////////////////////////////

HRtMatchingPar::HRtMatchingPar(const char* name, const char *title,
		const char *context) : HParCond(name,title,context) {
	strcpy(detName,"Mdc");
	HRuntimeDb *rtdb=Hades::instance()->getRuntimeDb();
	if (rtdb) HKickTask::setKickParIo(rtdb);
}

HRtMatchingPar::~HRtMatchingPar(void) {
}

void HRtMatchingPar::printTo(ostream &out) {
  using namespace std;
  out << "Total bins " << fTotalBins << endl;
  out << "Cells: ";
  for (unsigned int i=0;i<fTotalBins;i++) { out << int(fCells[i]); }
  out << endl;
  out << "Binning: \n";
  for (int i=0;i<fBins.GetSize();i++) {
    cout << fBins[i] << "\t(" << fMin[i] << ", " << fMax[i] << ");  " << endl;
  }
}

Bool_t HRtMatchingPar::init(HParIo *inp, Int_t *set) {
  //See HParSet::init
  HDetParIo *input=inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HRtMatchingPar::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (io) return (out->write(this));
  return -1; 
}

void HRtMatchingPar::readFrom(const HRtMatchingPar &par) {
  fTotalBins = par.fTotalBins;
  fIdx = par.fIdx;
  fCells = par.fCells;
  fMin = par.fMin;
  fMax = par.fMax;
  fBinSize = par.fBinSize;
  fBins = par.fBins;
}

void HRtMatchingPar::putParams(HParamList* l) {
  if (!l) return;
  l->add("fTotalBins", fTotalBins);
  l->add("fIdx", fIdx);
  l->add("fCells", fCells);
  l->add("fMin", fMin);
  l->add("fMax", fMax);
  l->add("fBinSize", fBinSize);
  l->add("fBins", fBins);
} 

Bool_t HRtMatchingPar::getParams(HParamList* l) {
  if (!l) return kFALSE;
  if (!l->fill("fTotalBins", &fTotalBins)) return kFALSE;
  if (!l->fill("fIdx", &fIdx)) return kFALSE;
  if (!l->fill("fCells", &fCells)) return kFALSE;
  if (!l->fill("fMin", &fMin)) return kFALSE;
  if (!l->fill("fMax", &fMax)) return kFALSE;
  if (!l->fill("fBinSize", &fBinSize)) return kFALSE;
  if (!l->fill("fBins", &fBins)) return kFALSE;
  return kTRUE;
}

char &HRtMatchingPar::bin(Int_t idx[]) {
  Int_t address = idx[0];
  Int_t *bins = fBins.GetArray();

  if (idx[0]<fBins[0]) {
    for (int i=1;i<fBins.GetSize();i++) {
      if (!(idx[i]<bins[i])) {
	address = fTotalBins;
	break;
      }
      address = address*bins[i] + idx[i];
    }
  } else address = fTotalBins;

  return fCells[address];
}

char &HRtMatchingPar::bin(Float_t a[]) {
  Int_t *idx = fIdx.GetArray();

  for (int i=0;i<fMin.GetSize();i++) {
    if (a[i] < fMin[i] || a[i]>fMax[i]) {
      //Error("bin","Out of bounds %f>%f",a[i],fMin[i]);
      return fCells[fTotalBins];
    } else {
      idx[i] = int(floor((a[i]-fMin[i])/fBinSize[i]));
    }
  }    
  return bin(idx);
}

void HRtMatchingPar::setDimension(Int_t n,Int_t bins[]) {
  fMin.Set(n);
  fIdx.Set(n);
  fMax.Set(n);
  fBinSize.Set(n);
  fBins.Set(n);

  fTotalBins = 1;
  for (int i=0;i<n;i++) {
    fBins[i] = bins[i];
    fTotalBins *= fBins[i];
  }
  fCells.Set(fTotalBins+1);
}

void HRtMatchingPar::setRange(UInt_t i,Float_t min, Float_t max) {
    fMin[i] = min;
    fMax[i] = max;
    fBinSize[i] = (max - min) / fBins[i];
}

ClassImp(HRtMatchingPar)
