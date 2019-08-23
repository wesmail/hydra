// @(#)$Id: hpidalgringpropertiespar.h,v 1.1 2003-06-22 14:39:16 christ Exp $
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
// -----------------------------------------------------------------------------

#define PIDALGRINGPROPERTIESPAR_NAME    "PidAlgRingPropertiesPar"
#define PIDALGRINGPROPERTIESPAR_TITLE   "Parameters for HPidAlgRingProperties"
#define PIDALGRINGPROPERTIESPAR_CONTEXT "HPidAlgRingPropertiesContext"

// -----------------------------------------------------------------------------

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
      return getValue(iPartId, CentroidStatus+2 , Eigencoord_One, Eigencoord_Two);
    }
  
  //These two functions are to be implemented in greater detail, once we know
  //how the fake contamination of the ring sample in the RICH is distributed in theta, phi, etc.
  Float_t getRichLeptonEfficiency()
    {
      //The numbers are just a futch factor to have a return value at all...
      return RichLeptonEfficiency;
    }
  Float_t getRichHadronEfficiency()
    {
      return RichHadronEfficiency;
    }

  void setRichLeptonEfficiency(Float_t lepeff)
    {
      //The numbers are just a futch factor to have a return value at all...
      RichLeptonEfficiency = lepeff;
    }
  void setRichHadronEfficiency(Float_t hadeff)
    {
      RichHadronEfficiency = hadeff;
    }

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
  Float_t RichLeptonEfficiency;
  Float_t RichHadronEfficiency;

 public:
  // -------------------------------------------------------------------------
  
  ClassDef(HPidAlgRingPropertiesPar, 1) // Params for HPidReconstructor
    };
    
    // -----------------------------------------------------------------------------
    
#endif  /* !HPIDALGRINGPROPERTIESPAR_H */
    
