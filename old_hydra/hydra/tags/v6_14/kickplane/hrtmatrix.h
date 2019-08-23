#ifndef HRTMATRIX_H
#define HRTMATRIX_H

#include <iostream>
class HRtVector;

class HRtFloatMemoryBlock {
private:
  Double_t *fData;
  Int_t fReferences;
public:
  HRtFloatMemoryBlock(Int_t size) {fData = new Double_t[size]; fReferences=1;}
  ~HRtFloatMemoryBlock(void) { delete[] fData; fData = 0;}
  void reference(void) { fReferences++; }
  void unReference(void) { fReferences--; if (fReferences==0) delete this;}
  Double_t &at(Int_t i) { return fData[i]; }
  void set(Int_t i,Double_t j) { fData[i] = j; }
};



class HRtMatrix {
private:
  HRtFloatMemoryBlock *fData;
  Int_t fSizes[2];  //Rows and Columns
  Int_t fStride[2]; //Strides for each dimension

public:
  HRtMatrix(Int_t rows,Int_t cols);
  HRtMatrix(const HRtMatrix &m);
  ~HRtMatrix(void);
  inline Double_t &operator()(Int_t i,Int_t j);
  inline HRtMatrix &operator=(HRtMatrix &a);
  inline HRtMatrix transpose(void);
  inline void transposeSelf(void);
  inline void multiply(HRtMatrix &a,HRtMatrix &b);
  inline HRtMatrix &operator*=(HRtMatrix &b);
  Int_t getRows(void) { return fSizes[0]; }
  Int_t getCols(void) { return fSizes[1]; }
  Double_t convolute(HRtVector &v);
  void identity(void);
  void print(Text_t name[]="");
};


inline void HRtMatrix::identity(void) {
  HRtMatrix &a=*this;

  for (Int_t i=0;i<fSizes[0];i++) {
    for (Int_t j=0;j<fSizes[1];j++)
      a(i,j) = (i==j)?1.:0.;
  }
}

class HRtVector {
 public:
  HRtVector(Int_t size) { fData = new Double_t[size]; fSize = size;}
  ~HRtVector(void) { delete[] fData; }
  HRtVector &operator=(HRtVector &v);
  void operator-=(HRtVector &v);
  void operator+=(HRtVector &v);
  void operator/=(Double_t f);
  Double_t &operator()(int i) { return fData[i]; }
  const Double_t at(int i) const { return fData[i]; }
  Int_t getSize(void) const { return fSize; }
  inline void multiply(HRtMatrix &a,HRtVector &v);
  void zero(void) { for(int i=0;i<fSize;i++) fData[i]=0.; }
  inline void resizeTo(Int_t n); 
  void print(void) { 
    for(int i=0;i<fSize;i++) std::cout << fData[i] << "\t"; 
    std::cout << std::endl; 
  }
 private:
  Double_t *fData;
  Int_t fSize;
};

inline void HRtVector::operator-=(HRtVector &v) {
  for (int i=0;i<fSize;i++) fData[i]-=v(i);
}

inline void HRtVector::operator+=(HRtVector &v) {
  for (int i=0;i<fSize;i++) fData[i]+=v(i);  
}

inline void HRtVector::operator/=(Double_t f) {
  for (int i=0;i<fSize;i++) fData[i]/=f;
}

inline HRtVector &HRtVector::operator=(HRtVector &v) {
  HRtVector &r = *this;
  r.resizeTo(v.getSize());
  for (int i=0;i<getSize();i++) fData[i]=v(i);
  return v;
}

inline void HRtVector::resizeTo(Int_t n) { 
  if (n!=fSize) { 
    delete fData; 
    fSize=n; 
    fData = new Double_t[fSize]; 
  }
}

inline void HRtVector::multiply(HRtMatrix &a,HRtVector &v) {
  HRtVector &r = *this;
  for (int i=0;i<a.getRows();i++) {
    r(i) = 0.;
    for (int j=0;j<a.getCols();j++) r(i) += a(i,j) * v(j);
  }
}

inline Double_t HRtMatrix::convolute(HRtVector &v) {
  Double_t sum=0,sum2=0;
  HRtMatrix &a = *this;

  for (Int_t i=0;i<getRows();i++) {
    sum2 = 0;
    for (Int_t j=0;j<getCols();j++) {
      sum2 += a(i,j) * v(j);
    }
    sum += v(i) * sum2;
  }
  return sum;
}

inline HRtMatrix &HRtMatrix::operator=(HRtMatrix &m) {
  Int_t entries;
  fSizes[0]  = m.fSizes[0];  fSizes[1] = m.fSizes[1];
  fStride[0] = m.fStride[0]; fStride[1] = m.fStride[1];
  
  entries = fSizes[0] * fSizes[1];
  fData->unReference();
  fData = new HRtFloatMemoryBlock(entries);
  for (Int_t i=0; i<entries; i++) fData->set(i,m.fData->at(i));
  return m;
}

inline void HRtMatrix::multiply(HRtMatrix &a,HRtMatrix &b) {
  HRtMatrix &r=*this;
  Double_t sum;

  for (Int_t i=0; i<fSizes[0]; i++)
    for (Int_t j=0; j<fSizes[1]; j++) {
      sum = 0;
      for (Int_t k=0; k<a.getCols(); k++) sum += a(i,k) * b(k,j);
      r(i,j) = sum;
    }
}

inline HRtMatrix &HRtMatrix::operator*=(HRtMatrix &b) {
  HRtMatrix &r=*this;
  Double_t col[fSizes[1]];
  Double_t sum;

  for (Int_t i=0; i<fSizes[0]; i++) {
    for (Int_t j=0; j<fSizes[1]; j++) col[j] = r(i,j);
    for (Int_t j=0; j<fSizes[1]; j++) {
      sum = 0;
      for (Int_t k=0; k<r.getCols(); k++) {
	sum += col[k] * b(k,j);
      }
      r(i,j) = sum;
    } 
  }
  return r;
}

inline Double_t &HRtMatrix::operator()(Int_t i,Int_t j) {
  return fData->at(i*fStride[0]+j*fStride[1]);
}

inline HRtMatrix HRtMatrix::transpose(void) {
  HRtMatrix r(*this);
  r.transposeSelf();
  return r;
}

inline void HRtMatrix::transposeSelf(void) {
  Int_t temp = fStride[0];
  fStride[0] = fStride[1];
  fStride[1] = temp;
  temp = fSizes[0];
  fSizes[0] = fSizes[1];
  fSizes[1] = temp;
}

inline HRtMatrix::HRtMatrix(const HRtMatrix &m) {
  fSizes[0]  = m.fSizes[0];  fSizes[1] = m.fSizes[1];
  fStride[0] = m.fStride[0]; fStride[1] = m.fStride[1];
  
  fData = m.fData;
  fData->reference();
}

inline HRtMatrix::HRtMatrix(Int_t rows,Int_t cols) {
  fSizes[0] = rows;
  fSizes[1] = cols;
  fStride[0] = cols;
  fStride[1] = 1;
  fData = new HRtFloatMemoryBlock(fSizes[0] * fSizes[1]);
}

inline HRtMatrix::~HRtMatrix(void) {
  fData->unReference();
}

inline void HRtMatrix::print(Text_t name[]) {
  HRtMatrix &a=*this;

  printf("\nMatrix %s is:",name);
  printf("\n     |");
  for (int j=0;j<fSizes[1];j++)
    printf("   %6d  |",j);
  printf("\n%s\n",
      "------------------------------------------------------------------");
  for (int i=0;i<fSizes[0]; i++) {
    printf("%4d |",i);
    for (int j=0;j<fSizes[1]; j++) 
      printf("%11.4g ",a(i,j));
    printf("\n");
  }
}

#endif
