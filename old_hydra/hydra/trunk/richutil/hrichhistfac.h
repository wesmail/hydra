// $Id: hrichhistfac.h,v 1.15 2009-07-15 11:39:22 halo Exp $
// Last update by Thomas Eberl: 04/04/21 13:38:56
//
#ifndef HRICHHISTFAC_H
#define HRICHHISTFAC_H
#include "TObject.h"
#include "TObjArray.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
class HRichHistFac : public TObject {
public:

	HRichHistFac();
	~HRichHistFac();
    
    static TCanvas* makeMicroCanvas(TObjArray*);
    //
    static TH2F* getHMomChrgvsPolarAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomChrgvsPolarAngleBasename(){return "MomChrg.PolarAngle_";};
    //
    static TH2F* getHMomtChrgvsPolarAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomtChrgvsPolarAngleBasename(){return "MomtChrg.PolarAngle_";};
    //
    static TH2F* getHPolarHitChrgvsAzimuth(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHPolarHitChrgvsAzimuthBasename(){return "Polar.Azimuth_";};
    //
    static TH2F* getHGeantMomKickMomDiff(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHGeantMomKickMomDiffBasename(){return "MomentumGeant.DiffMomentumKickGeant_";};
    //
    static TH1F* getHPseudoRapidity(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHPseudoRapidityBasename(){return "PseudoRapidity_";};
    //
    static TH1F* getHCorrCodeGeant(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHCorrCodeGeantBasename(){return "CorrCodeGeant_";};
    //
    static TH2F* getHMomCorrCodeGeant(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomCorrCodeGeantBasename(){return "MomCorrCodeGeant_";};
    //
    static TH2F* getHCorrCodeGeantPolarAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHCorrCodeGeantPolarAngleBasename(){return "CorrCodeGeant.PolarAngle_";};
    //
    static TH2F* getHPartCorrCodeGeant(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHPartCorrCodeGeantBasename(){return "PartCorrCodeGeant_";};
    //
    static TH1F* getHRapidity(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRapidityBasename(){return "Rapidity_";};
    //
    static TH1F* getHBeta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHBetaBasename(){return "Beta_";};
    //
    static TH1F* getHKickPull(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHKickPullBasename(){return "KickPull_";};
    //
    static TH1F* getHOpeningAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHOpeningAngleBasename(){return "OpeningAngle_";};
    //
    static TH1F* getHOpeningAngleVarBin(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHOpeningAngleVarBinBasename(){return "OpeningAngleVarBin_";};
    //
    static TH1F* getHPolarAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHPolarAngleBasename(){return "PolarAngle_";};
    //
    static TH1F* getHAzimuthalAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHAzimuthalAngleBasename(){return "AzimuthalAngle_";};
    //
    static TH1F* getHInvMass(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHInvMassBasename(){return "InvMass_";};
    //
    static TH1F* getHInvMassVarBin(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHInvMassVarBinBasename(){return "InvMassVarBin_";};
    //
    static TH2F* getHInvMassMomt(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHInvMassMomtBasename(){return "InvMass.PairPt_";};
    //
    static TH2F* getHInvMassRapidity(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHInvMassRapidityBasename(){return "InvMass.Rapidity_";};
    //
    static TH2F* getHInvMassOpang(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHInvMassOpangBasename(){return "InvMass.Opang_";};
    //
    static TH2F* getHMomtOpang(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomtOpangBasename(){return "PairPt.OpeningAngle_";};
    //
    static TH2F* getHMomtRapidity(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomtRapidityBasename(){return "PairPt.Rapidity_";};
    //
    static TH2F* getHRapidityOpang(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRapidityOpangBasename(){return "Rapidity.OpeningAngle_";};
    //
    static TH1F* getHTof(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHTofBasename(){return "TimeOfFlight_";};
    //
    static TH2F* getHRingXY(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingXYBasename(){return "RingCenterX.Y_";};
    //
    static TH1F* getHRingRadius(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingRadiusBasename(){return "RingRadius_";};
    //
    static TH2F* getHRingRadiusTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingRadiusThetaBasename(){return "PolarAngle.RingRadius_";};
    //
    static TH1F* getHRingAmplitude(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAmplitudeBasename(){return "RingChargeSum_";};
    //
    static TH2F* getHRingAmplitudeTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAmplitudeThetaBasename(){return "PolarAngle.RingChargeSum_";};
    //
    static TH1F* getHRingPadNr(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingPadNrBasename(){return "RingPadNr_";};
    //
    static TH2F* getHRingPadNrTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingPadNrThetaBasename(){return "PolarAngle.RingPadNr_";};
    //
    static TH1F* getHRingLocMax4(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingLocMax4Basename(){return "RingLocMax4_";};
    //
    static TH2F* getHRingLocMax4Theta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingLocMax4ThetaBasename(){return "PolarAngle.RingLocMax4_";};
    //
    static TH1F* getHRingPM(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingPMBasename(){return "RingPatternMatrix_";};
    //
    static TH2F* getHRingPMTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingPMThetaBasename(){return "PolarAngle.RingPatternMatrix_";};
    //
    static TH1F* getHRingHT(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingHTBasename(){return "RingHoughTrafo_";};
    //
    static TH2F* getHRingHTTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingHTThetaBasename(){return "PolarAngle.RingHoughTrafo_";};
    //
    static TH1F* getHRingCentroid(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingCentroidBasename(){return "RingCentroid_";};
    //
    static TH2F* getHRingCentroidTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingCentroidThetaBasename(){return "PolarAngle.RingCentroid_";};
    //
    static TH1F* getHRingAvChrg(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAvChrgBasename(){return "RingAvChrg_";};
    //
    static TH2F* getHRingAvChrgTheta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAvChrgThetaBasename(){return "PolarAngle.RingAvChrg_";};
    // --
    static TH2F* getHRingAvChrgLocMax4(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAvChrgLocMax4Basename(){return "RingAvChrg.LocMax4_";};
    //
    static TH2F* getHRingAvChrgCentroid(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAvChrgCentroidBasename(){return "RingAvChrg.Centroid_";};
    //
    static TH2F* getHRingAvChrgPatMat(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAvChrgPatMatBasename(){return "RingAvChrg.PatMat_";};
    //
    static TH2F* getHRingAvChrgPadNr(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingAvChrgPadNrBasename(){return "RingAvChrg.PadNr_";};
    //
    static TH2F* getHRingLocMax4Centroid(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingLocMax4CentroidBasename(){return "LocMax4.Centroid_";};
    //
    static TH2F* getHRingLocMax4PatMat(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingLocMax4PatMatBasename(){return "LocMax4.PatMat_";};
    //
    static TH2F* getHRingLocMax4PadNr(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingLocMax4PadNrBasename(){return "LocMax4.PadNr_";};
    //
    static TH2F* getHRingCentroidPatMat(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingCentroidPatMatBasename(){return "Centroid.PatMat_";};
    //
    static TH2F* getHRingCentroidPadNr(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingCentroidPadNrBasename(){return "Centroid.PadNr_";};
    //
    static TH2F* getHRingPatMatPadNr(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingPatMatPadNrBasename(){return "PatMat.PadNr_";};
    // ----
    static TH2F* getHRingParIdWeight(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHRingParIdWeightBasename(){return "ParId.Weight_";};
    // ----
    static TH2F* getHWeightWeight(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHWeightWeightBasename(){return "RICHWeight.MDCWeight_";};
    // ----
    static TH1F* getHParId(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHParIdBasename(){return "ParId_";};
    // ----
    static TH1F* getHMom(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomBasename(){return "Momentum_";};
    // ----
    static TH1F* getHMomt(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomtBasename(){return "MomentumTransverse_";};
    //
    static TH2F* getHMomvsMom(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomvsMomBasename(){return "Mom.Mom_";};
    //
    static TH2F* getHMdcRvsMdcZ(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMdcRvsMdcZBasename(){return "MdcR.MdcZ_";};
    //
    static TH2F* getHMdcRvsMdcChi2(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMdcRvsMdcChi2Basename(){return "MdcR.MdcChi2_";};
    //
    static TH2F* getHMomvsBeta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomvsBetaBasename(){return "Beta.Mom_";};
    //
    static TH2F* getHMomChrgvsBeta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomChrgvsBetaBasename(){return "Beta.MomChrg_";};
    //
    static TH2F* getHMomvsEnergy(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomvsEnergyBasename(){return "Mom.Energy_";};
    //
    static TH2F* getHMomChrgvsEnergy(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMomChrgvsEnergyBasename(){return "MomChrg.Energy_";};
    //
    static TH2F* getHMassvsBeta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMassvsBetaBasename(){return "Mass.Beta_";};
    //
    static TH2F* getHMassChrgvsBeta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMassChrgvsBetaBasename(){return "MassChrg.Beta_";};
    //
    static TH1F* getHMass(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMassBasename(){return "Mass_";};
    //
    static TH1F* getHDeflAngle(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHDeflAngleBasename(){return "DeflectionAngle_";};
    //
    static TH2F* getHDeflAnglevsMass(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHDeflAnglevsMassBasename(){return "DeflectionAngle.Mass_";};
    //
    static TH2F* getHDeflAnglevsBeta(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHDeflAnglevsBetaBasename(){return "DeflectionAngle.Beta_";};
    // 
    static TH2F* getHDeflAnglevsMom(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHDeflAnglevsMomBasename(){return "DeflectionAngle.Mom_";};
    // 
    static TH1F* getHMult(const Char_t* c=NULL,const Char_t* b=NULL);
    static const Char_t* getHMultBasename(){return "Multiplicity_";};
    //    
    static TH1* getHistoByBaseName(const Char_t*,const Char_t*);
private:
    
    ClassDef(HRichHistFac,1)
};

#endif //HRICHHISTFAC_H



