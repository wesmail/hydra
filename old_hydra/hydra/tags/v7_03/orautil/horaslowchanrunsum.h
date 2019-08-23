#ifndef HORASLOWCHANRUNSUM_H
#define HORASLOWCHANRUNSUM_H

using namespace std;
#include "TNamed.h"
#include "TString.h"
#include <iostream>
#include <iomanip>
#include <fstream>

class HOraSlowPeriod;

class HOraSlowChanRunSum : public TNamed {
private:
  Int_t    periodIndex;  // Index of the corresponding period
  Double_t mean;         // Mean value (weighted mean over period)
  Double_t sigma;        // Sigma of mean value
  Double_t minVal;       // Minimum value in the period
  Double_t maxVal;       // Maximum value in the period
  Int_t    nData;        // Number of raw data in the corresponding period
  Int_t    status;       // Status flag
public:
  HOraSlowChanRunSum();
  ~HOraSlowChanRunSum() {}
  void setPeriodIndex(Int_t n) {periodIndex=n;}
  void setMean(Double_t v) {mean=v;}
  void setSigma(Double_t v) {sigma=v;}
  void setMinValue(Double_t v) {minVal=v;}
  void setMaxValue(Double_t v) {maxVal=v;}
  void setNData(Int_t n) {nData=n;}
  void setStatus(Int_t n) {status=n;}
  void fill(Int_t,Double_t,Double_t,Double_t,Double_t,Int_t,Int_t);
  Int_t getPeriodIndex() {return periodIndex;}
  Double_t getMean() {return mean;}
  Double_t getSigma() {return sigma;}
  Double_t getMinVal() {return minVal;}
  Double_t getMaxVal() {return maxVal;}
  Int_t getNData() {return nData;}
  Int_t getStatus() {return status;}
  void print(Int_t valPrec=3,HOraSlowPeriod* period=0);
  void write(fstream& fout,Int_t valPrec=3,HOraSlowPeriod* period=0);
private:
  ClassDef(HOraSlowChanRunSum,0) // Class for run based summary data for a slowcontrol channel
};

#endif  /* !HORASLOWCHANRUNSUM */
