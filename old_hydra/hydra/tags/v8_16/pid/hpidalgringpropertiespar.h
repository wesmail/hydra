// @(#)$Id: hpidalgringpropertiespar.h,v 1.13 2008-02-26 15:30:16 christ Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGRINGPROPERTIESPAR_H
#define HPIDALGRINGPROPERTIESPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgRingPropertiesPar
// This container holds the PDF values required to compute the probability that
// a track is a lepton/hadron/fake based upon RICH information
//
////////////////////////////////////////////////////////////////////////////////
using namespace std;

#include "hpidhistogramscont.h"
#include "hpidtrackcand.h"
#include "piddef.h"
#include <iostream>


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

  //Functions to set and get the parameters required for a future Principal Component Based analysis
  //of RICH data. This is NOT used at the moment, because the degree of correlation of the
  //RICH variables is high enough to pick two (Average Charge and Pattern Matrix) and disregard the
  //rest. Further detailed studies will come.
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

  
  //This is not a nice thing - we assume that we always have two bins in polar angle
  //In priciple one should handle this dynamically - implementation pending
  Int_t getThetaBin(Float_t fTheta) const
    {
      for(Int_t i=0;i<2;i++)
	{
	  if(fTheta<=(i+1)*45.0)
	    {
	      if(i!=getThetaBinDynamic(fTheta))
		{
		  Error("HPidAlgRingPropertiesPar::getThetaBin()","ambiguous theta bin not computed!");
		  exit(-1);
		}
	      return i;
	    }	  
	}
      return -1;
    }

  Int_t getThetaBinDynamic(Float_t fTheta) const
    {
      Int_t nThetaBins = ((Float_t)(getHistsPerPart()-1)) / 2;
      Float_t thetaStep = 90./nThetaBins;

      for(Int_t i=0;i<nThetaBins;i++)
	{
	  if(fTheta<=(i+1)*thetaStep)
	    {
	      return i;
	    }	  
	}
      return -1;
    }


  Float_t getNormalizedPDFValue(Int_t iPartId, Float_t fTheta, Int_t iSector, Bool_t bCentroid, Float_t Coordinate_1, Float_t Coordinate_2) const
    {
      Int_t offset=0;
      Int_t thetaBin =getThetaBin(fTheta);

      if(!bCentroid) offset+=2;
      offset+=thetaBin;

      if(offset>=4) 
	{
	  Error("HPidAlgRingPropertiesPar::getNormalizedPDFValue()","Offset %d out of range!",offset);
	}
      Float_t value = getValue(iPartId, offset, iSector, Coordinate_1, Coordinate_2);
      return value;
    }
  
  
  //Returns the fraction of leptons correlated to a ring in this range of momentum and polar angle
  Float_t getRichLeptonFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 4;
      return (Float_t) getValue(2,offset,sector,momentum,theta);
      /*
	TH1* ptr = getHistogram(2,offset,sector);
	Int_t bin = ptr->FindBin(momentum,theta);
	return  ptr->GetBinContent(bin);
	ugly ugly ugly - this should be done using the intrinsic mechanisms of hpidhistogramscont
      */
    }


  //Returns the fraction of protons correlated to a ring in this range of momentum and polar angle
  Float_t getRichProtonFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 4;
      return (Float_t) getValue(14,offset,sector,momentum,theta);
      /*
	TH1* ptr = getHistogram(14,offset, sector);
	Int_t bin = ptr->FindBin(momentum,theta);
	return  ptr->GetBinContent(bin);
      */
    }


  //Returns the fraction of pi-plus correlated to a ring in this range of momentum and polar angle
  Float_t getRichPiPlusFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 4;
      return (Float_t) getValue(8,offset,sector,momentum,theta);
      /*
	TH1* ptr = getHistogram(8,offset, sector);
	Int_t bin = ptr->FindBin(momentum,theta);
	return  ptr->GetBinContent(bin);
      */
    }


  //Returns the fraction of pi-minus correlated to a ring in this range of momentum and polar angle
  Float_t getRichPiMinusFraction(Float_t momentum, Float_t theta, Int_t sector)
    {
      Int_t offset = 4;
      return (Float_t) getValue(9,offset,sector,momentum,theta);
      /*
	TH1* ptr = getHistogram(9,offset, sector);
	Int_t bin = ptr->FindBin(momentum,theta);
	return  ptr->GetBinContent(bin);
      */
    }


  //Returns the fraction of fakes correlated to a ring in this range of momentum and polar angle
  Float_t getRichFakeFraction(Float_t momentum, Float_t theta,Int_t sector)
    {
      Int_t offset = 4;
      return (Float_t) getValue(-1,offset,sector,momentum,theta);
      /*
	TH1* ptr = getHistogram(-1,offset, sector);
	Int_t bin = ptr->FindBin(momentum,theta);
	return  ptr->GetBinContent(bin);
      */
    }


  //Not used at present - possibly used in the future for an improved RICH pid method
  void setUse4DTransform(Bool_t choice){use4DTransform=choice;}
  void setUse5DTransform(Bool_t choice){use5DTransform=choice;}

  Bool_t getUse4DTransform(){return use4DTransform;}
  Bool_t getUse5DTransform(){return use5DTransform;}

  private:
  
  //These Variables are required to compute the transformation into the 4/5 dimensional
  //Eigenspace of the data points. They are currently NOT used
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
    
