// $Id: hrichhistfac.h,v 1.11 2003-09-09 15:40:50 eberl Exp $
// Last update by Thomas Eberl: 03/08/21 11:23:37
//
#ifndef HRICHHISTFAC_H
#define HRICHHISTFAC_H
#include <TObject.h>
#include <TObjArray.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
class HRichHistFac : public TObject {
public:

	HRichHistFac();
	~HRichHistFac();
    
    static TCanvas* makeMicroCanvas(TObjArray*);
    //
    static TH2F* getHMomChrgvsPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsPolarAngleBasename(){return "MomChrg.PolarAngle_";};
    //
    static TH2F* getHMomtChrgvsPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtChrgvsPolarAngleBasename(){return "MomtChrg.PolarAngle_";};
    //
    static TH2F* getHPolarHitChrgvsAzimuth(const char* c=NULL,const char* b=NULL);
    static Char_t* getHPolarHitChrgvsAzimuthBasename(){return "Polar.Azimuth_";};
    //
    static TH2F* getHGeantMomKickMomDiff(const char* c=NULL,const char* b=NULL);
    static Char_t* getHGeantMomKickMomDiffBasename(){return "MomentumGeant.DiffMomentumKickGeant_";};
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
    static Char_t* getHCorrCodeGeantPolarAngleBasename(){return "CorrCodeGeant.PolarAngle_";};
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
    static TH1F* getHPolarAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHPolarAngleBasename(){return "PolarAngle_";};
    //
    static TH1F* getHAzimuthalAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHAzimuthalAngleBasename(){return "AzimuthalAngle_";};
    //
    static TH1F* getHInvMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassBasename(){return "InvMass_";};
    //
    static TH2F* getHInvMassMomt(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassMomtBasename(){return "InvMass.PairPt_";};
    //
    static TH2F* getHInvMassRapidity(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassRapidityBasename(){return "InvMass.Rapidity_";};
    //
    static TH2F* getHInvMassOpang(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassOpangBasename(){return "InvMass.Opang_";};
    //
    static TH2F* getHMomtOpang(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtOpangBasename(){return "PairPt.OpeningAngle_";};
    //
    static TH2F* getHMomtRapidity(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomtRapidityBasename(){return "PairPt.Rapidity_";};
    //
    static TH2F* getHRapidityOpang(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRapidityOpangBasename(){return "Rapidity.OpeningAngle_";};
    //
    static TH1F* getHTof(const char* c=NULL,const char* b=NULL);
    static Char_t* getHTofBasename(){return "TimeOfFlight_";};
    //
    static TH2F* getHRingXY(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingXYBasename(){return "RingCenterX.Y_";};
    //
    static TH1F* getHRingRadius(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingRadiusBasename(){return "RingRadius_";};
    //
    static TH2F* getHRingRadiusTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingRadiusThetaBasename(){return "PolarAngle.RingRadius_";};
    //
    static TH1F* getHRingAmplitude(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAmplitudeBasename(){return "RingChargeSum_";};
    //
    static TH2F* getHRingAmplitudeTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAmplitudeThetaBasename(){return "PolarAngle.RingChargeSum_";};
    //
    static TH1F* getHRingPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPadNrBasename(){return "RingPadNr_";};
    //
    static TH2F* getHRingPadNrTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPadNrThetaBasename(){return "PolarAngle.RingPadNr_";};
    //
    static TH1F* getHRingLocMax4(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4Basename(){return "RingLocMax4_";};
    //
    static TH2F* getHRingLocMax4Theta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4ThetaBasename(){return "PolarAngle.RingLocMax4_";};
    //
    static TH1F* getHRingPM(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPMBasename(){return "RingPatternMatrix_";};
    //
    static TH2F* getHRingPMTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPMThetaBasename(){return "PolarAngle.RingPatternMatrix_";};
    //
    static TH1F* getHRingHT(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingHTBasename(){return "RingHoughTrafo_";};
    //
    static TH2F* getHRingHTTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingHTThetaBasename(){return "PolarAngle.RingHoughTrafo_";};
    //
    static TH1F* getHRingCentroid(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidBasename(){return "RingCentroid_";};
    //
    static TH2F* getHRingCentroidTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidThetaBasename(){return "PolarAngle.RingCentroid_";};
    //
    static TH1F* getHRingAvChrg(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgBasename(){return "RingAvChrg_";};
    //
    static TH2F* getHRingAvChrgTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgThetaBasename(){return "PolarAngle.RingAvChrg_";};
    // --
    static TH2F* getHRingAvChrgLocMax4(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgLocMax4Basename(){return "RingAvChrg.LocMax4_";};
    //
    static TH2F* getHRingAvChrgCentroid(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgCentroidBasename(){return "RingAvChrg.Centroid_";};
    //
    static TH2F* getHRingAvChrgPatMat(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgPatMatBasename(){return "RingAvChrg.PatMat_";};
    //
    static TH2F* getHRingAvChrgPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAvChrgPadNrBasename(){return "RingAvChrg.PadNr_";};
    //
    static TH2F* getHRingLocMax4Centroid(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4CentroidBasename(){return "LocMax4.Centroid_";};
    //
    static TH2F* getHRingLocMax4PatMat(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4PatMatBasename(){return "LocMax4.PatMat_";};
    //
    static TH2F* getHRingLocMax4PadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4PadNrBasename(){return "LocMax4.PadNr_";};
    //
    static TH2F* getHRingCentroidPatMat(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidPatMatBasename(){return "Centroid.PatMat_";};
    //
    static TH2F* getHRingCentroidPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingCentroidPadNrBasename(){return "Centroid.PadNr_";};
    //
    static TH2F* getHRingPatMatPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPatMatPadNrBasename(){return "PatMat.PadNr_";};
    // ----
    static TH2F* getHRingParIdWeight(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingParIdWeightBasename(){return "ParId.Weight_";};
    // ----
    static TH2F* getHWeightWeight(const char* c=NULL,const char* b=NULL);
    static Char_t* getHWeightWeightBasename(){return "RICHWeight.MDCWeight_";};
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
    static Char_t* getHMomvsMomBasename(){return "Mom.Mom_";};
    //
    static TH2F* getHMdcRvsMdcZ(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMdcRvsMdcZBasename(){return "MdcR.MdcZ_";};
    //
    static TH2F* getHMdcRvsMdcChi2(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMdcRvsMdcChi2Basename(){return "MdcR.MdcChi2_";};
    //
    static TH2F* getHMomvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsBetaBasename(){return "Beta.Mom_";};
    //
    static TH2F* getHMomChrgvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsBetaBasename(){return "Beta.MomChrg_";};
    //
    static TH2F* getHMomvsEnergy(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsEnergyBasename(){return "Mom.Energy_";};
    //
    static TH2F* getHMomChrgvsEnergy(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsEnergyBasename(){return "MomChrg.Energy_";};
    //
    static TH2F* getHMassvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassvsBetaBasename(){return "Mass.Beta_";};
    //
    static TH2F* getHMassChrgvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassChrgvsBetaBasename(){return "MassChrg.Beta_";};
    //
    static TH1F* getHMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassBasename(){return "Mass_";};
    //
    static TH1F* getHDeflAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAngleBasename(){return "DeflectionAngle_";};
    //
    static TH2F* getHDeflAnglevsMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAnglevsMassBasename(){return "DeflectionAngle.Mass_";};
    //
    static TH2F* getHDeflAnglevsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAnglevsBetaBasename(){return "DeflectionAngle.Beta_";};
    // 
    static TH2F* getHDeflAnglevsMom(const char* c=NULL,const char* b=NULL);
    static Char_t* getHDeflAnglevsMomBasename(){return "DeflectionAngle.Mom_";};
    // 
    static TH1F* getHMult(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMultBasename(){return "Multiplicity_";};
    //    
    static TH1* getHistoByBaseName(const char*,const char*);
private:
    
    ClassDef(HRichHistFac,1)
};

#endif //HRICHHISTFAC_H



