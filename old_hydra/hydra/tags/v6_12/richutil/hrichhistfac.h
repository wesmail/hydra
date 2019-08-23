// $Id: hrichhistfac.h,v 1.8 2003-04-07 16:24:03 christ Exp $
// Last update by Thomas Eberl: 03/03/13 14:40:38
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
    static TH1F* getHOpeningAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHOpeningAngleBasename(){return "OpeningAngle_";};
    //
    static TH1F* getHInvMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassBasename(){return "InvMass_";};
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
    //static TH2F* getHRingParIdWeight(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingParIdWeightBasename(){return "ParId.Weight_";};
    // ----
    static TH2F* getHMomvsMom(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsMomBasename(){return "Mom.Mom_";};
    //
    static TH2F* getHMomvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsBetaBasename(){return "Mom.Beta_";};
    //
    static TH2F* getHMomChrgvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsBetaBasename(){return "Mom*Chrg.Beta_";};
    //
    static TH2F* getHMomvsEnergy(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsEnergyBasename(){return "Mom.Energy_";};
    //
    static TH2F* getHMomChrgvsEnergy(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomChrgvsEnergyBasename(){return "Mom*Chrg.Energy_";};
    //
    static TH2F* getHMassvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassvsBetaBasename(){return "Mass.Beta_";};
    //
    static TH2F* getHMassChrgvsBeta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMassChrgvsBetaBasename(){return "Mass*Chrg.Beta_";};
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

//      static Char_t* getHPtvsPtotBasename(){return "Pt_vs_Ptot_";};
//      static TH2F* getHPtotvsPtot(const char* c=NULL);
//      static Char_t* getHPtotvsPtotBasename(){return "Ptot_vs_Ptot_";};
//      static TH2F* getHPtvsPt(const char* c=NULL);
//      static Char_t* getHPtvsPtBasename(){return "Pt_vs_Pt_";};
    static TH1* getHistoByBaseName(const char*,const char*);
private:
    
    ClassDef(HRichHistFac,1)
};

#endif //HRICHHISTFAC_H



