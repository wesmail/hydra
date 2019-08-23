//*-- Author : R.Schicker
//*-- Modified : M. Sanchez (8.06.2000)
//*-- Modified : R. Holzmann (23.06.2004)
//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HMdcSeg
//
// This is the SEGment class of MDC. This data
// container holds the information about a straight
// track let between a pair of MDCs. The coordinate systems
// corresponds to the sector coordinate system.
// Besides the parametrization of a straight line (z,r,theta,phi)
// the object contains the Chi2 from the fitter (normalized to
// the number of dgree of freedom) and the covariance
// matrix for the errors.
// A parametrization of the straight line through the middle of
// the target can be retrieved from the variables (zprime,rprime,theta,phi).
//
// With Int_t ind getHitInd(Int_t i) one can obtain
// the index number of the two HMdcHits objects in the Hit category.
// If a Hit doesn't exist -1 is returned.
// Int_t getIOSeg(void) gives 0 for inner segment
// (MDCI+MDCII) and 1 for outer segment (MDCIII+MDCIV).
// This object derives from HMdcCellGroup12, so the information
// of the wires contributing to the Segment can be obtained.
//
//
/////////////////////////////////////////////////////////////////////////

#include "hmdcseg.h"

HMdcSeg::HMdcSeg(void) {
  Clear();
}

HMdcSeg::~HMdcSeg(void) {
} 

void HMdcSeg::Clear(void) {
    // reset data members to default values
    z=0.;
    r=0.;
    theta=0.;
    phi=0.;
    chi2=0.;
    flag=0;
    ind=0;         //Nb of hits = 0
    cov.Clear();
    zPrime=0.;
    rPrime=0;
    clear();
}


void HMdcSeg::Streamer(TBuffer &R__b)
{
   // Stream an object of class HMdcSeg (customized)

   UInt_t R__s, R__c;
   if (R__b.IsReading()) {
      Version_t R__v = R__b.ReadVersion(&R__s, &R__c); if (R__v) { }
      HMdcCellGroup12::Streamer(R__b);
      R__b >> z;
      R__b >> r;
      R__b >> theta;
      R__b >> phi;
      R__b >> chi2;
      R__b >> flag;
      cov.Streamer(R__b);
      R__b >> ind;
      if (R__v > 1) {
        R__b >> zPrime;
        R__b >> rPrime;
        if (chi2>0) {
          Int_t nw=getSumWires()-3; // -3=-4+1 (nw=DOF)
          if (nw>0) chi2/=nw;
        }
      } else {
        zPrime = z;
        rPrime = r;
      }
      R__b.CheckByteCount(R__s, R__c, HMdcSeg::IsA());
   } else {
      R__c = R__b.WriteVersion(HMdcSeg::IsA(), kTRUE);
      HMdcCellGroup12::Streamer(R__b);
      R__b << z;
      R__b << r;
      R__b << theta;
      R__b << phi;
      R__b << chi2;
      R__b << flag;
      cov.Streamer(R__b);
      R__b << ind;
      R__b << zPrime;
      R__b << rPrime;
      R__b.SetByteCount(R__c, kTRUE);
   }
}


ClassImp(HMdcSeg)












