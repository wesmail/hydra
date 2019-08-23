// $Id: hpairhisto.h,v 1.1 2005-03-23 12:34:57 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-03-17 18:05:46
//
// ---------------------------------------------------------------------------
#ifndef HPAIRHISTO_H
#define HPAIRHISTO_H
// ---------------------------------------------------------------------------
#include <TObject.h>
// ---------------------------------------------------------------------------
class TH1;
class TH1F;
class TH2F;
class TObjArray;
// ---------------------------------------------------------------------------
class HPairHisto : public TObject {
public:

    HPairHisto();
    ~HPairHisto();
    
    static TH2F* getHMomChrgvsPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsPolarAngleBasename(){return "MomChrg_PolarAngle_";};
    //
    static TH2F* getHMomtChrgvsPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtChrgvsPolarAngleBasename(){return "MomtChrg_PolarAngle_";};
    //
    static TH2F* getHPolarHitChrgvsAzimuth(const char* c=NULL,const char* b=NULL);
    static Char_t* getHPolarHitChrgvsAzimuthBasename(){return "Polar_Azimuth_";};
    //
    static TH2F* getHGeantMomKickMomDiff(const char* c=NULL,const char* b=NULL);
    static Char_t* getHGeantMomKickMomDiffBasename(){return "MomentumGeant_DiffMomentumKickGeant_";};
    //
    static TH1F* getHPseudoRapidity(const char* c=NULL,const char* b=NULL);
    static Char_t* getHPseudoRapidityBasename(){return "PseudoRapidity_";};
    //
    static TH1F* getHCorrCodeGeant(const char* c=NULL,const char* b=NULL);
    static Char_t* getHCorrCodeGeantBasename(){return "CorrCodeGeant_";};
    //
    static TH2F* getHMomCorrCodeGeant(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomCorrCodeGeantBasename(){return "MomCorrCodeGeant_";};
    //
    static TH2F* getHCorrCodeGeantPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHCorrCodeGeantPolarAngleBasename(){return "CorrCodeGeant_PolarAngle_";};
    //
    static TH2F* getHPartCorrCodeGeant(const char* c=NULL,const char* b=NULL);
    static Char_t* getHPartCorrCodeGeantBasename(){return "PartCorrCodeGeant_";};
    //
    static TH1F* getHRapidity(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRapidityBasename(){return "Rapidity_";};
    //
    static TH1F* getHBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHBetaBasename(){return "Beta_";};
    //
    static TH1F* getHKickPull(const char* c=NULL,const char* b=NULL);
    static Char_t* getHKickPullBasename(){return "KickPull_";};
    //
    static TH1F* getHOpeningAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHOpeningAngleBasename(){return "OpeningAngle_";};
    //
    static TH1F* getHOpeningAngleVarBin(const char* c=NULL,const char* b=NULL);
    static Char_t* getHOpeningAngleVarBinBasename(){return "OpeningAngleVarBin_";};
    //
    static TH1F* getHPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHPolarAngleBasename(){return "PolarAngle_";};
    //
    static TH1F* getHAzimuthalAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHAzimuthalAngleBasename(){return "AzimuthalAngle_";};
    //
    static TH1F* getHInvMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassBasename(){return "InvMass_";};
    //
    static TH1F* getHInvMassVarBin(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassVarBinBasename(){return "InvMassVarBin_";};
    //
    static TH2F* getHInvMassMomt(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassMomtBasename(){return "InvMass_PairPt_";};
    //
    static TH2F* getHInvMassRapidity(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassRapidityBasename(){return "InvMass_Rapidity_";};
    //
    static TH2F* getHInvMassOpang(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassOpangBasename(){return "InvMass_Opang_";};
    //
    static TH2F* getHMomtOpang(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtOpangBasename(){return "PairPt_OpeningAngle_";};
    //
    static TH2F* getHMomtRapidity(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtRapidityBasename(){return "PairPt_Rapidity_";};
    //
    static TH2F* getHRapidityOpang(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRapidityOpangBasename(){return "Rapidity_OpeningAngle_";};
    //
    static TH1F* getHTof(const char* c=NULL,const char* b=NULL);
    static Char_t* getHTofBasename(){return "TimeOfFlight_";};
    //
    static TH2F* getHRingXY(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingXYBasename(){return "RingCenterX_Y_";};
    //
    static TH1F* getHRingRadius(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingRadiusBasename(){return "RingRadius_";};
    //
    static TH2F* getHRingRadiusTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingRadiusThetaBasename(){return "PolarAngle_RingRadius_";};
    //
    static TH1F* getHRingAmplitude(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAmplitudeBasename(){return "RingChargeSum_";};
    //
    static TH2F* getHRingAmplitudeTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAmplitudeThetaBasename(){return "PolarAngle_RingChargeSum_";};
    //
    static TH1F* getHRingPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPadNrBasename(){return "RingPadNr_";};
    //
    static TH2F* getHRingPadNrTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPadNrThetaBasename(){return "PolarAngle_RingPadNr_";};
    //
    static TH1F* getHRingLocMax4(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4Basename(){return "RingLocMax4_";};
    //
    static TH2F* getHRingLocMax4Theta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4ThetaBasename(){return "PolarAngle_RingLocMax4_";};
    //
    static TH1F* getHRingPM(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPMBasename(){return "RingPatternMatrix_";};
    //
    static TH2F* getHRingPMTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPMThetaBasename(){return "PolarAngle_RingPatternMatrix_";};
    //
    static TH1F* getHRingHT(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingHTBasename(){return "RingHoughTrafo_";};
    //
    static TH2F* getHRingHTTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingHTThetaBasename(){return "PolarAngle_RingHoughTrafo_";};
    //
    static TH1F* getHRingCentroid(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidBasename(){return "RingCentroid_";};
    //
    static TH2F* getHRingCentroidTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidThetaBasename(){return "PolarAngle_RingCentroid_";};
    //
    static TH1F* getHRingAvChrg(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgBasename(){return "RingAvChrg_";};
    //
    static TH2F* getHRingAvChrgTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgThetaBasename(){return "PolarAngle_RingAvChrg_";};
    // --
    static TH2F* getHRingAvChrgLocMax4(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgLocMax4Basename(){return "RingAvChrg_LocMax4_";};
    //
    static TH2F* getHRingAvChrgCentroid(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgCentroidBasename(){return "RingAvChrg_Centroid_";};
    //
    static TH2F* getHRingAvChrgPatMat(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgPatMatBasename(){return "RingAvChrg_PatMat_";};
    //
    static TH2F* getHRingAvChrgPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgPadNrBasename(){return "RingAvChrg_PadNr_";};
    //
    static TH2F* getHRingLocMax4Centroid(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4CentroidBasename(){return "LocMax4_Centroid_";};
    //
    static TH2F* getHRingLocMax4PatMat(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4PatMatBasename(){return "LocMax4_PatMat_";};
    //
    static TH2F* getHRingLocMax4PadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4PadNrBasename(){return "LocMax4_PadNr_";};
    //
    static TH2F* getHRingCentroidPatMat(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidPatMatBasename(){return "Centroid_PatMat_";};
    //
    static TH2F* getHRingCentroidPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidPadNrBasename(){return "Centroid_PadNr_";};
    //
    static TH2F* getHRingPatMatPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPatMatPadNrBasename(){return "PatMat_PadNr_";};
    // ----
    static TH2F* getHRingParIdWeight(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingParIdWeightBasename(){return "ParId_Weight_";};
    // ----
    static TH2F* getHWeightWeight(const char* c=NULL,const char* b=NULL);
    static Char_t* getHWeightWeightBasename(){return "RICHWeight_MDCWeight_";};
    // ----
    static TH1F* getHParId(const char* c=NULL,const char* b=NULL);
    static Char_t* getHParIdBasename(){return "ParId_";};
    // ----
    static TH1F* getHMom(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomBasename(){return "Momentum_";};
    // ----
    static TH1F* getHMomt(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtBasename(){return "MomentumTransverse_";};
    //
    static TH2F* getHMomvsMom(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsMomBasename(){return "Mom_Mom_";};
    //
    static TH2F* getHMdcRvsMdcZ(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMdcRvsMdcZBasename(){return "MdcR_MdcZ_";};
    //
    static TH2F* getHMdcRvsMdcChi2(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMdcRvsMdcChi2Basename(){return "MdcR_MdcChi2_";};
    //
    static TH2F* getHMomvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsBetaBasename(){return "Beta_Mom_";};
    //
    static TH2F* getHMomChrgvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsBetaBasename(){return "Beta_MomChrg_";};
    //
    static TH2F* getHMomvsEnergy(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsEnergyBasename(){return "Mom_Energy_";};
    //
    static TH2F* getHMomChrgvsEnergy(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsEnergyBasename(){return "MomChrg_Energy_";};
    //
    static TH2F* getHMassvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassvsBetaBasename(){return "Mass_Beta_";};
    //
    static TH2F* getHMassChrgvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassChrgvsBetaBasename(){return "MassChrg_Beta_";};
    //
    static TH1F* getHMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassBasename(){return "Mass_";};
    //
    static TH1F* getHDeflAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAngleBasename(){return "DeflectionAngle_";};
    //
    static TH2F* getHDeflAnglevsMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAnglevsMassBasename(){return "DeflectionAngle_Mass_";};
    //
    static TH2F* getHDeflAnglevsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAnglevsBetaBasename(){return "DeflectionAngle_Beta_";};
    // 
    static TH2F* getHDeflAnglevsMom(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAnglevsMomBasename(){return "DeflectionAngle_Mom_";};
    // 
    static TH1F* getHMult(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMultBasename(){return "Multiplicity_";};
    //    
    static TH1* getHistoByBaseName(const char*,const char*);

private:
    
    ClassDef(HPairHisto,0)
};

#endif //HPAIRHISTO_H



