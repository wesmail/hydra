//*-- AUTHOR : Ilse Koenig
//*-- Created : 05/11/2004
//*-- Modified : 24/11/2004 by V. Pechenov

///////////////////////////////////////////////////////////////////////////
//
//  HMetaMatchPar
//
//  Parameter container for HMetaMatchF
//
// Rich:
//   Tr - angle theta of ring from HRichHit (in degree) in lab.coor.sys.
//   Pr - angle phi of ring from HRichHit (in degree) in lab.coor.sys.
//   Tm - angle theta of ring from HRichHit (in degree) in lab.coor.sys.
//   Pm - angle phi of ring from HRichHit (in degree) in lab.coor.sys.
//   SigmaTheta= (richThetaMaxCut - richThetaMinCut)/2.
//   Toffset   = (richThetaMinCut + richThetaMaxCut)/2.
//   qualityRich = 
//     sqrt[((Pr - Pm - richSigmaPhiOffset)*sin(Tm)/richSigmaPhi)^2 +
//          ((Tr - Tm - Toffset)/SigmaTheta)^2]
//
//   Condition of RICH MDC matching:  qualityRich < richQualityCut
//
//   Parameters preparation (for each sector):
//     From distribution of (Tr-Tm) to get 
//     window "richThetaMinCut" - "richThetaMaxCut".
//     From distribution of (Pr-Pm) to get (by Gaus approximation for example)
//     "richSigmaPhiOffset" (mean of approximation).
//     From distribution of (Pr-Pm-richSigmaPhiOffset)*sin(Tm) to get 
//     "richSigmaPhi" (sigma of this distr.).
//     Select "richQualityCut", for example 1.0.
//
//  RichIPU:
//   Tr - angle theta of ring from HRichHitIPU (in degree) in lab.coor.sys.
//   Pr - angle phi of ring from HRichHitIPU (in degree) in lab.coor.sys.
//   Tm - angle theta of ring from HRichHitIPU (in degree) in lab.coor.sys.
//   Pm - angle phi of ring from HRichHitIPU (in degree) in lab.coor.sys.
//   SigmaTheta= (richIPUThetaMaxCut - richIPUThetaMinCut)/2.
//   Toffset   = (richIPUThetaMinCut + richIPUThetaMaxCut)/2.
//   qualityRichIPU = 
//     sqrt[((Pr - Pm - richIPUSigmaPhiOffset)*sin(Tm)/richIPUSigmaPhi)^2 +
//          ((Tr - Tm - Toffset)/SigmaTheta)^2]
//
//   Condition of RICHIPU MDC matching:  qualityIPURich < richIPUQualityCut
//
//   Parameters preparation (for each sector):
//     From distribution of (Tr-Tm) to get 
//     window "richIPUThetaMinCut" - "richIPUThetaMaxCut".
//     From distribution of (Pr-Pm) to get (by Gaus approximation for
//     example) "richIPUSigmaPhiOffset" (mean of this distr.).
//     From distribution of (Pr-Pm-richIPUSigmaPhiOffset)*sin(Tm) to get
//     "richIPUSigmaPhi" (sigma of this distr.).
//     Select "richIPUQualityCut", for example 1.0
//
//
// Shower:
//   Xs,Ys - shower hit position in coordinate system of shower module
//   dXs,dYs - hit position errors (== HShowerHit::getSigmaX(), getSigmaY())
//   Xm,Ym - mdc segment cross point with shower module in coordinate system 
//           of corresponding shower module 
//   qualityShower = 
//     sqrt[((Xs - Xm - showerSigmaXOffset)/sqrt(dXs^2 + showerSigmaXMdc^2))^2 +
//          ((Ys - Ym - showerSigmaYOffset)/sqrt(dYs^2 + showerSigmaYMdc^2))^2]
//
//   Condition of SHOWER MDC matching:  qualityShower < showerQualityCut
//
//   Parameters preparation (for each sector):
//     From distribution of (Xs-Xm) to get "showerSigmaXOffset" 
//     (mean of this distr.).
//     From distribution of (Xs-Xm-showerSigmaXOffset)/dXs (!) to get 
//     sigma of this distribution.
//     If sigma > 1. to calculate "showerSigmaXMdc" = sqrt(sigma^2 - 1.),
//     otherwise set "showerSigmaXMdc" = 0.
//     From distribution of (Ys-Ym) to get "showerSigmaYOffset".
//     From distribution of (Ys-Ym-showerSigmaYOffset)/dYs (!) to get 
//     sigma of this distribution.
//     If sigma > 1. to calculate "showerSigmaYMdc" = sqrt(sigma^2 - 1.),
//     otherwise set "showerSigmaYMdc" = 0.
//     Select "showerQualityCut", for example 5.0 (five sigmas cut).
//
//
// Tof:
//   Xt,Yt - toh hit (or tof cluster position in coordinate system of tof module
//   Xm,Ym - mdc segment cross point with tof module in coordinate system 
//           of corresponding tof module
//   qualityTof = sqrt[ ((Xt - Xm - tofSigmaXOffset)/tofSigmaX)^2 +
//                      ((Yt - Ym - tofSigmaYOffset)/tofSigmaY)^2 ]
//
//   Condition of TOF MDC matching:  qualityTof < tofQualityCut
//
//   Parameters preparation (for each sector):
//     From distribution of (Xt-Xm) to get "tofSigmaX" (sigma of this distr.) 
//     and "tofSigmaXOffset" (mean of this distr).
//     From distribution of (Yt-Ym) to get "tofSigmaY" (sigma of this distr.) 
//     and "tofSigmaYOffset" (mean of this distr).
//     Select "tofQualityCut", for example 5.0 (five sigmas cut).
//
///////////////////////////////////////////////////////////////////////////

#include "hmetamatchpar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"

ClassImp(HMetaMatchPar)

HMetaMatchPar::HMetaMatchPar(const char* name,
                       const char* title,
                       const char* context)
           : HParCond(name,title,context) {
  // constructor
  Float_t a[6]={0.,0.,0.,0.,0.,0.};
  richThetaMinCut.Set(6,a);
  richThetaMaxCut.Set(6,a);
  richSigmaPhi.Set(6,a);
  richSigmaPhiOffset.Set(6,a);
  richQualityCut.Set(6,a);
  richIPUThetaMinCut.Set(6,a);
  richIPUThetaMaxCut.Set(6,a);
  richIPUSigmaPhi.Set(6,a);
  richIPUSigmaPhiOffset.Set(6,a);
  richIPUQualityCut.Set(6,a);
  showerSigmaXMdc.Set(6,a);
  showerSigmaYMdc.Set(6,a);
  showerSigmaXOffset.Set(6,a);
  showerSigmaYOffset.Set(6,a);
  showerQualityCut.Set(6,a);
  tofSigmaX.Set(6,a);
  tofSigmaY.Set(6,a);
  tofSigmaXOffset.Set(6,a);
  tofSigmaYOffset.Set(6,a);
  tofQualityCut.Set(6,a);
}

Bool_t HMetaMatchPar::init(HParIo* inp,Int_t* set) {
 // initializes the container from an input
  HDetParIo* input=inp->getDetParIo("HCondParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HMetaMatchPar::write(HParIo* output) {
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HCondParIo");
  if (out) return out->write(this);
  return kFALSE;
}

void HMetaMatchPar::clear(void) {
  // all parameters are initialized with 0.
  for(Int_t i=0;i<6;i++) {
    richThetaMinCut[i]=0.;
    richThetaMaxCut[i]=0.;
    richSigmaPhi[i]=0.;
    richSigmaPhiOffset[i]=0.;
    richQualityCut[i]=0.;
    richIPUThetaMinCut[i]=0.;
    richIPUThetaMaxCut[i]=0.;
    richIPUSigmaPhi[i]=0.;
    richIPUSigmaPhiOffset[i]=0.;
    richIPUQualityCut[i]=0.;
    showerSigmaXMdc[i]=0.;
    showerSigmaYMdc[i]=0.;
    showerSigmaXOffset[i]=0.;
    showerSigmaYOffset[i]=0.;
    showerQualityCut[i]=0.;
    tofSigmaX[i]=0.;
    tofSigmaY[i]=0.;
    tofSigmaXOffset[i]=0.;
    tofSigmaYOffset[i]=0.;
    tofQualityCut[i]=0.;
  }
  status=kFALSE;
  resetInputVersions();
  changed=kFALSE;
}

void HMetaMatchPar::putParams(HParamList* l) {
  // puts all parameters to the parameter list, which is used by the io
  if (!l) return;
  l->add("richThetaMinCut",      richThetaMinCut);
  l->add("richThetaMaxCut",      richThetaMaxCut);
  l->add("richSigmaPhi",         richSigmaPhi);
  l->add("richSigmaPhiOffset",   richSigmaPhiOffset);
  l->add("richQualityCut",       richQualityCut);
  l->add("richIPUThetaMinCut",   richIPUThetaMinCut);
  l->add("richIPUThetaMaxCut",   richIPUThetaMaxCut);
  l->add("richIPUSigmaPhi",      richIPUSigmaPhi);
  l->add("richIPUSigmaPhiOffset",richIPUSigmaPhiOffset);
  l->add("richIPUQualityCut",    richIPUQualityCut);
  l->add("showerSigmaXMdc",      showerSigmaXMdc);
  l->add("showerSigmaYMdc",      showerSigmaYMdc);
  l->add("showerSigmaXOffset",   showerSigmaXOffset);
  l->add("showerSigmaYOffset",   showerSigmaYOffset);
  l->add("showerQualityCut",     showerQualityCut);
  l->add("tofSigmaX",            tofSigmaX);
  l->add("tofSigmaY",            tofSigmaY);
  l->add("tofSigmaXOffset",      tofSigmaXOffset);
  l->add("tofSigmaYOffset",      tofSigmaYOffset);
  l->add("tofQualityCut",        tofQualityCut);
}

Bool_t HMetaMatchPar::getParams(HParamList* l) {
  // gets all parameters from the parameter list, which is used by the io
  if (!l) return kFALSE;
  if (!(l->fill("richThetaMinCut",      &richThetaMinCut)))       return kFALSE;
  if (!(l->fill("richThetaMaxCut",      &richThetaMaxCut)))       return kFALSE;
  if (!(l->fill("richSigmaPhi",         &richSigmaPhi)))          return kFALSE;
  if (!(l->fill("richSigmaPhiOffset",   &richSigmaPhiOffset)))    return kFALSE;
  if (!(l->fill("richQualityCut",       &richQualityCut)))        return kFALSE;
  if (!(l->fill("richIPUThetaMinCut",   &richIPUThetaMinCut)))    return kFALSE;
  if (!(l->fill("richIPUThetaMaxCut",   &richIPUThetaMaxCut)))    return kFALSE;
  if (!(l->fill("richIPUSigmaPhi",      &richIPUSigmaPhi)))       return kFALSE;
  if (!(l->fill("richIPUSigmaPhiOffset",&richIPUSigmaPhiOffset))) return kFALSE;
  if (!(l->fill("richIPUQualityCut",    &richIPUQualityCut)))     return kFALSE;  
  if (!(l->fill("showerSigmaXMdc",      &showerSigmaXMdc)))       return kFALSE;
  if (!(l->fill("showerSigmaYMdc",      &showerSigmaYMdc)))       return kFALSE;
  if (!(l->fill("showerSigmaXOffset",   &showerSigmaXOffset)))    return kFALSE;
  if (!(l->fill("showerSigmaYOffset",   &showerSigmaYOffset)))    return kFALSE;
  if (!(l->fill("showerQualityCut",     &showerQualityCut)))      return kFALSE;
  if (!(l->fill("tofSigmaX",            &tofSigmaX)))             return kFALSE;
  if (!(l->fill("tofSigmaY",            &tofSigmaY)))             return kFALSE;
  if (!(l->fill("tofSigmaXOffset",      &tofSigmaXOffset)))       return kFALSE;
  if (!(l->fill("tofSigmaYOffset",      &tofSigmaYOffset)))       return kFALSE;
  if (!(l->fill("tofQualityCut",        &tofQualityCut)))         return kFALSE;
  return kTRUE;
}
