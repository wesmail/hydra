// $Id: hrichhistfac.h,v 1.4 2002-09-25 16:19:58 eberl Exp $
// Last update by Thomas Eberl: 02/09/25 18:11:05
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
    static TH1F* getHOpeningAngle(const char* c=NULL,const char* b=NULL);
    static Char_t* getHOpeningAngleBasename(){return "OpeningAngle_";};
    static TH1F* getHInvMass(const char* c=NULL,const char* b=NULL);
    static Char_t* getHInvMassBasename(){return "InvMass_";};
    static TH1F* getHTof(const char* c=NULL,const char* b=NULL);
    static Char_t* getHTofBasename(){return "TimeOfFlight_";};
    //
    static TH1F* getHRingRadius(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingRadiusBasename(){return "Ring.Radius_";};
    static TH2F* getHRingRadiusTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingRadiusThetaBasename(){return "Ring.Radius_PolarAngle_";};
    //
    static TH1F* getHRingAmplitude(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAmplitudeBasename(){return "Ring.ChargeSum_";};
    static TH2F* getHRingAmplitudeTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingAmplitudeThetaBasename(){return "Ring.ChargeSum_PolarAngle_";};
    //
    static TH1F* getHRingPadNr(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPadNrBasename(){return "Ring.PadNr_";};
    static TH2F* getHRingPadNrTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPadNrThetaBasename(){return "Ring.PadNr_PolarAngle_";};
    //
    static TH1F* getHRingLocMax4(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4Basename(){return "Ring.LocMax4_";};
    static TH2F* getHRingLocMax4Theta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingLocMax4ThetaBasename(){return "Ring.LocMax4_PolarAngle_";};
    //
    static TH1F* getHRingPM(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPMBasename(){return "Ring.PatternMatrix_";};
    static TH2F* getHRingPMTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingPMThetaBasename(){return "Ring.PatternMatrix_PolarAngle_";};
    //
    static TH1F* getHRingHT(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingHTBasename(){return "Ring.HoughTrafo_";};
    static TH2F* getHRingHTTheta(const char* c=NULL,const char* b=NULL);
    static Char_t* getHRingHTThetaBasename(){return "Ring.HoughTrafo_PolarAngle_";};
    //
    static TH2F* getHMomvsMom(const char* c=NULL,const char* b=NULL);
    static Char_t* getHMomvsMomBasename(){return "Mom.vs.Mom_";};
//      static Char_t* getHPtvsPtotBasename(){return "Pt_vs_Ptot_";};
//      static TH2F* getHPtotvsPtot(const char* c=NULL);
//      static Char_t* getHPtotvsPtotBasename(){return "Ptot_vs_Ptot_";};
//      static TH2F* getHPtvsPt(const char* c=NULL);
//      static Char_t* getHPtvsPtBasename(){return "Pt_vs_Pt_";};
private:
    
    ClassDef(HRichHistFac,1)
};

#endif //HRICHHISTFAC_H



