// @(#)$Id: hpidalgringpropertiespar.h,v 1.10 2005-05-20 21:19:51 christ Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGRINGPROPERTIESPAR_H
#define HPIDALGRINGPROPERTIESPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgRingPropertiesPar
//
////////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hpidhistogramscont.h"
#include "hpidtrackcand.h"
#include "piddef.h"
#include <iostream>
// -----------------------------------------------------------------------------

#define PIDALGRINGPROPERTIESPAR_NAME    "PidAlgRingPropertiesPar"
#define PIDALGRINGPROPERTIESPAR_TITLE   "Parameters for HPidAlgRingProperties"
#define PIDALGRINGPROPERTIESPAR_CONTEXT "HPidAlgRingPropertiesContext"

// -----------------------------------------------------------------------------
#include <TH1.h>
class HPidAlgRingPropertiesPar : public HPidHistogramsCont
{

 public:
  
  HPidAlgRingPropertiesPar(
			   const char *name    = PIDALGRINGPROPERTIESPAR_NAME,
			   const char *title   = PIDALGRINGPROPERTIESPAR_TITLE,
			   const char *context = PIDALGRINGPROPERTIESPAR_CONTEXT);
  virtual    ~HPidAlgRingPropertiesPar(void) {}
  
  // -------------------------------------------------------------------------
  
  virtual void set4DMean(Int_t sector, Int_t index, Float_t value) {MeanValues4D[4*sector+index]=value;}
  virtual void set5DMean(Int_t sector, Int_t index, Float_t value) {MeanValues5D[5*sector+index]=value;}

  virtual void set4DEvec(Int_t sector, Int_t index, Float_t value) {EigenVectors4D[16*sector+index]=value;}
  virtual void set5DEvec(Int_t sector, Int_t index, Float_t value) {EigenVectors5D[25*sector+index]=value;}

  virtual void set4DSigma(Int_t sector, Int_t index, Float_t value) {SigmaValues4D[4*sector+index]=value;}
  virtual void set5DSigma(Int_t sector, Int_t index, Float_t value) {SigmaValues5D[5*sector+index]=value;}


  Float_t get4DMean(Int_t sector, Int_t index) {return MeanValues4D[4*sector+index];}
  Float_t get5DMean(Int_t sector, Int_t index) {return MeanValues5D[5*sector+index];}

  Float_t get4DEvec(Int_t sector, Int_t index) {return EigenVectors4D[16*sector+index];}
  Float_t get5DEvec(Int_t sector, Int_t index) {return EigenVectors5D[25*sector+index];}

  Float_t get4DSigma(Int_t sector, Int_t index) {return SigmaValues4D[4*sector+index];}
  Float_t get5DSigma(Int_t sector, Int_t index) {return SigmaValues5D[5*sector+index];}

  Int_t getThetaBin(Float_t fKickTheta) const
    {
      if(fKickTheta<=30) return 0;
      if(fKickTheta<=50) return 1;
      if(fKickTheta<=90) return 2;
      return -1;
    }


  Float_t getNormalizedPDFValue(Int_t iPartId, Float_t fTheta, Int_t iSector, Bool_t bCentroid, Float_t Coordinate_1, Float_t Coordinate_2) const
    {
      Int_t offset=0;
      Int_t thetaBin =getThetaBin(fTheta);
      if(!bCentroid) offset+=3;
      offset+=thetaBin;
      if(offset>6) cout << "Offset out of range!" << endl;
      Float_t value = getValue(iPartId, offset, iSector, Coordinate_1, Coordinate_2);
      /*cout << "pid: " << iPartId<< endl;
      cout << "Sector: " << iSector << endl;
      cout<< "Coord_1: " << Coordinate_1 << endl;
      cout<< "Coord_2: " << Coordinate_2 << endl;
      cout << "offset: " << offset << endl;
      cout << "value: " << value << endl;*/
      return value;
    }



  Float_t getRichLeptonFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 6;
      TH1* ptr = getHistogram(2,offset,sector);
      Int_t bin = ptr->FindBin(momentum,theta);
      return  ptr->GetBinContent(bin);
    }

  Float_t getRichProtonFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 6;
      TH1* ptr = getHistogram(14,offset, sector);
      Int_t bin = ptr->FindBin(momentum,theta);
      return  ptr->GetBinContent(bin);
    }

  Float_t getRichPiPlusFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 6;
      TH1* ptr = getHistogram(8,offset, sector);
      Int_t bin = ptr->FindBin(momentum,theta);
      return  ptr->GetBinContent(bin);
    }

  Float_t getRichPiMinusFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 6;
      TH1* ptr = getHistogram(9,offset, sector);
      Int_t bin = ptr->FindBin(momentum,theta);
      return  ptr->GetBinContent(bin);
    }

  Float_t getRichFakeFraction(Float_t momentum, Float_t theta,Int_t sector)
    {
      Int_t offset = 6;
      TH1* ptr = getHistogram(-1,offset, sector);
      Int_t bin = ptr->FindBin(momentum,theta);
      return  ptr->GetBinContent(bin);
    }

  void setUse4DTransform(Bool_t choice){use4DTransform=choice;}
  void setUse5DTransform(Bool_t choice){use5DTransform=choice;}

  Bool_t getUse4DTransform(){return use4DTransform;}
  Bool_t getUse5DTransform(){return use5DTransform;}

  private:
  
  //These Variables are required to compute the transformation into the 4/5 dimensional
  //Eigenspace of the data points.
  Float_t EigenVectors4D[16*6];
  Float_t EigenVectors5D[25*6];
  
  Float_t MeanValues4D[4*6];
  Float_t MeanValues5D[5*6];

  Float_t SigmaValues4D[4*6];
  Float_t SigmaValues5D[5*6];

  Bool_t use5DTransform;
  Bool_t use4DTransform;

 public:
  // -------------------------------------------------------------------------
  
  ClassDef(HPidAlgRingPropertiesPar, 1) // Params for HPidReconstructor
    };
    
    // -----------------------------------------------------------------------------
    
#endif  /* !HPIDALGRINGPROPERTIESPAR_H */
    
