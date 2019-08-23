// @(#)$Id: hpidalgringpropertiespar.h,v 1.6 2004-07-08 13:45:51 christ Exp $
//*-- Author : Tassilo Christ 23/04/2002

#ifndef HPIDALGRINGPROPERTIESPAR_H
#define HPIDALGRINGPROPERTIESPAR_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgRingPropertiesPar
//
////////////////////////////////////////////////////////////////////////////////

#include "hpidhistogramscont.h"
#include "hpidtrackcand.h"
#include "piddef.h"
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
  
  virtual void set4DMean(Int_t index, Float_t value) {MeanValues4D[index]=value;}
  virtual void set5DMean(Int_t index, Float_t value) {MeanValues5D[index]=value;}

  virtual void set4DEvec(Int_t index, Float_t value) {EigenVectors4D[index]=value;}
  virtual void set5DEvec(Int_t index, Float_t value) {EigenVectors5D[index]=value;}

  virtual void set4DSigma(Int_t index, Float_t value) {SigmaValues4D[index]=value;}
  virtual void set5DSigma(Int_t index, Float_t value) {SigmaValues5D[index]=value;}


  Float_t get4DMean(Int_t index) {return MeanValues4D[index];}
  Float_t get5DMean(Int_t index) {return MeanValues5D[index];}

  Float_t get4DEvec(Int_t index) {return EigenVectors4D[index];}
  Float_t get5DEvec(Int_t index) {return EigenVectors5D[index];}

  Float_t get4DSigma(Int_t index) {return SigmaValues4D[index];}
  Float_t get5DSigma(Int_t index) {return SigmaValues5D[index];}
  
  Float_t getNormalizedPDFValue(Int_t iPartId, Int_t CentroidStatus, Float_t Eigencoord_One, Float_t Eigencoord_Two) const
    {
      return getValue(iPartId, CentroidStatus , Eigencoord_One, Eigencoord_Two);
    }
  
  Float_t getCLValue(Int_t iPartId, Int_t CentroidStatus, Float_t Eigencoord_One, Float_t Eigencoord_Two) const
    {
      return getValue(iPartId, CentroidStatus , Eigencoord_One, Eigencoord_Two);
    }
  

  Float_t getRichLeptonFraction(Float_t momentum, Float_t theta, Bool_t isSec0=kFALSE)
    {
      TH1* ptr = getHistogram(2,17);
      Int_t bin = ptr->FindBin(momentum,theta);
      if(isSec0)
	return  ptr->GetBinContent(bin);
      else
	ptr = getHistogram(2,16);
	return  ptr->GetBinContent(bin);
    }

  Float_t getRichProtonFraction(Float_t momentum, Float_t theta, Bool_t isSec0=kFALSE)
    {
      Int_t bin = getHistogram(14,17)->FindBin(momentum,theta);
      if(isSec0)
	return  getHistogram(14,17)->GetBinContent(bin);
      else
	return  getHistogram(14,16)->GetBinContent(bin);
    }

  Float_t getRichPiPlusFraction(Float_t momentum, Float_t theta, Bool_t isSec0=kFALSE)
    {
      Int_t bin = getHistogram(8,17)->FindBin(momentum,theta);
      if(isSec0)
	return  getHistogram(8,17)->GetBinContent(bin);
      else
	return  getHistogram(8,16)->GetBinContent(bin);
    }

  Float_t getRichPiMinusFraction(Float_t momentum, Float_t theta, Bool_t isSec0=kFALSE)
    {
      Int_t bin = getHistogram(9,17)->FindBin(momentum,theta);
      if(isSec0)
	return  getHistogram(9,17)->GetBinContent(bin);
      else
	return  getHistogram(9,16)->GetBinContent(bin);
    }
  
  Float_t getRichFakeFraction(Float_t momentum, Float_t theta,Bool_t isSec0=kFALSE)
    {
      Int_t bin = getHistogram(-1,17)->FindBin(momentum,theta);
      if(isSec0)
	return  getHistogram(-1,17)->GetBinContent(bin);
      else
	return  getHistogram(-1,16)->GetBinContent(bin);
    }

  /*
  void setRichLeptonFraction(Int_t index, Float_t lepeff)
    {
      //The numbers are just a futch factor to have a return value at all...
	RichLeptonFraction[index] = lepeff;
    }
  void setRichHadronFraction(Int_t index, Float_t hadeff)
    {
	RichHadronFraction[index] = hadeff;
    }

  void setRichFakeFraction(Int_t index, Float_t hadeff)
    {
	RichFakeFraction[index] = hadeff;
    }

  void setRichLeptonFraction_sec0(Int_t index, Float_t lepeff)
    {
      //The numbers are just a futch factor to have a return value at all...
	RichLeptonFraction_sec0[index] = lepeff;
    }
  void setRichHadronFraction_sec0(Int_t index, Float_t hadeff)
    {
	RichHadronFraction_sec0[index] = hadeff;
    }

  void setRichFakeFraction_sec0(Int_t index, Float_t hadeff)
    {
	RichFakeFraction_sec0[index] = hadeff;
    }
  */

  private:
  
  //These Variables are required to compute the transformation into the 4/5 dimensional
  //Eigenspace of the data points.
  Float_t EigenVectors4D[16];
  Float_t EigenVectors5D[25];
  
  Float_t MeanValues4D[4];
  Float_t MeanValues5D[5];

  Float_t SigmaValues4D[4];
  Float_t SigmaValues5D[5];
 
  //These variables are required to scale the yields in the PDFs to the lepton/hadron efficiencies of the RICH!
  Float_t RichLeptonFraction[nMomentumBins];
  Float_t RichHadronFraction[nMomentumBins];
  Float_t RichFakeFraction[nMomentumBins];

  Float_t RichLeptonFraction_sec0[nMomentumBins];
  Float_t RichHadronFraction_sec0[nMomentumBins];
  Float_t RichFakeFraction_sec0[nMomentumBins];

 public:
  // -------------------------------------------------------------------------
  
  ClassDef(HPidAlgRingPropertiesPar, 2) // Params for HPidReconstructor
    };
    
    // -----------------------------------------------------------------------------
    
#endif  /* !HPIDALGRINGPROPERTIESPAR_H */
    
