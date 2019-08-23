// $Id: hrichhistfac.cc,v 1.12 2003-09-09 15:40:47 eberl Exp $
// Last update by Thomas Eberl: 03/08/22 22:24:55
//
#include "hrichhistfac.h"
#include "hdebug.h"
#include "hades.h"
#include <TH1.h>
#include <string>
#include <iostream.h>
///////////////////////////////////////////////////////////
// HRichHistFac
//
// HRich Histogram Factory
// this class is a helper that contains functionality
// that can be shared by different reconstructors
// 
///////////////////////////////////////////////////////////

ClassImp(HRichHistFac)

HRichHistFac::HRichHistFac()
{

}


HRichHistFac::~HRichHistFac(void) 
{

}


TH1*  HRichHistFac::getHistoByBaseName(const char* suppliedname,const char* add)
{
    TH1 *h=0;
    if (!strcmp(suppliedname,getHPolarAngleBasename())) h= getHPolarAngle(add);
    if (!strcmp(suppliedname,getHAzimuthalAngleBasename())) h= getHAzimuthalAngle(add);
    if (!strcmp(suppliedname,getHOpeningAngleBasename())) h= getHOpeningAngle(add);
    if (!strcmp(suppliedname,getHInvMassBasename())) h= getHInvMass(add);
    if (!strcmp(suppliedname,getHInvMassMomtBasename())) h= getHInvMassMomt(add);
    if (!strcmp(suppliedname,getHRapidityBasename())) h= getHRapidity(add);
    if (!strcmp(suppliedname,getHInvMassRapidityBasename())) h= getHInvMassRapidity(add);
    if (!strcmp(suppliedname,getHMomtRapidityBasename())) h= getHMomtRapidity(add);
    if (!strcmp(suppliedname,getHInvMassOpangBasename())) h= getHInvMassOpang(add);
    if (!strcmp(suppliedname,getHMomtOpangBasename())) h= getHMomtOpang(add);
    if (!strcmp(suppliedname,getHRapidityOpangBasename())) h= getHRapidityOpang(add);
    if (!strcmp(suppliedname,getHTofBasename())) h= getHTof(add);
    if (!strcmp(suppliedname,getHRingXYBasename())) h= getHRingXY(add);
    if (!strcmp(suppliedname,getHRingRadiusBasename())) h= getHRingRadius(add);
    if (!strcmp(suppliedname,getHRingRadiusThetaBasename())) h= getHRingRadiusTheta(add);
    if (!strcmp(suppliedname,getHRingAmplitudeBasename())) h= getHRingAmplitude(add);
    if (!strcmp(suppliedname,getHRingAmplitudeThetaBasename())) h= getHRingAmplitudeTheta(add);
    if (!strcmp(suppliedname,getHRingPadNrBasename())) h= getHRingPadNr(add);
    if (!strcmp(suppliedname,getHRingPadNrThetaBasename())) h= getHRingPadNrTheta(add);
    if (!strcmp(suppliedname,getHRingLocMax4Basename())) h= getHRingLocMax4(add);
    if (!strcmp(suppliedname,getHRingLocMax4ThetaBasename())) h= getHRingLocMax4Theta(add);
    if (!strcmp(suppliedname,getHRingPMBasename())) h= getHRingPM(add);
    if (!strcmp(suppliedname,getHRingPMThetaBasename())) h= getHRingPMTheta(add);
    if (!strcmp(suppliedname,getHRingHTBasename())) h= getHRingHT(add);
    if (!strcmp(suppliedname,getHRingHTThetaBasename())) h= getHRingHTTheta(add);
    if (!strcmp(suppliedname,getHRingCentroidBasename())) h= getHRingCentroid(add);
    if (!strcmp(suppliedname,getHRingCentroidThetaBasename())) h= getHRingCentroidTheta(add);
    if (!strcmp(suppliedname,getHRingAvChrgBasename())) h= getHRingAvChrg(add);
    if (!strcmp(suppliedname,getHRingAvChrgThetaBasename())) h= getHRingAvChrgTheta(add);
    if (!strcmp(suppliedname,getHRingAvChrgLocMax4Basename())) h= getHRingAvChrgLocMax4(add);
    if (!strcmp(suppliedname,getHRingAvChrgCentroidBasename())) h= getHRingAvChrgCentroid(add);
    if (!strcmp(suppliedname,getHRingAvChrgPatMatBasename())) h= getHRingAvChrgPatMat(add);
    if (!strcmp(suppliedname,getHRingAvChrgPadNrBasename())) h= getHRingAvChrgPadNr(add);
    if (!strcmp(suppliedname,getHRingLocMax4CentroidBasename())) h= getHRingLocMax4Centroid(add);
    if (!strcmp(suppliedname,getHRingLocMax4PatMatBasename())) h= getHRingLocMax4PatMat(add);
    if (!strcmp(suppliedname,getHRingLocMax4PadNrBasename())) h= getHRingLocMax4PadNr(add);
    if (!strcmp(suppliedname,getHRingCentroidPatMatBasename())) h= getHRingCentroidPatMat(add);
    if (!strcmp(suppliedname,getHRingCentroidPadNrBasename())) h= getHRingCentroidPadNr(add);
    if (!strcmp(suppliedname,getHRingPatMatPadNrBasename())) h= getHRingPatMatPadNr(add);
    if (!strcmp(suppliedname,getHRingParIdWeightBasename())) h= getHRingParIdWeight(add);
    if (!strcmp(suppliedname,getHWeightWeightBasename())) h= getHWeightWeight(add);
    if (!strcmp(suppliedname,getHParIdBasename())) h= getHParId(add);
    if (!strcmp(suppliedname,getHMomvsMomBasename())) h= getHMomvsMom(add);
    if (!strcmp(suppliedname,getHMdcRvsMdcZBasename())) h= getHMdcRvsMdcZ(add);
    if (!strcmp(suppliedname,getHMdcRvsMdcChi2Basename())) h= getHMdcRvsMdcChi2(add);
    if (!strcmp(suppliedname,getHMomvsBetaBasename())) h= getHMomvsBeta(add);
    if (!strcmp(suppliedname,getHMomChrgvsBetaBasename())) h= getHMomChrgvsBeta(add);
    if (!strcmp(suppliedname,getHMomChrgvsPolarAngleBasename())) h= getHMomChrgvsPolarAngle(add);
    if (!strcmp(suppliedname,getHMomtChrgvsPolarAngleBasename())) h= getHMomtChrgvsPolarAngle(add);
    if (!strcmp(suppliedname,getHPolarHitChrgvsAzimuthBasename())) h= getHPolarHitChrgvsAzimuth(add);
    if (!strcmp(suppliedname,getHMomvsEnergyBasename())) h= getHMomvsEnergy(add);
    if (!strcmp(suppliedname,getHMomChrgvsEnergyBasename())) h= getHMomChrgvsEnergy(add);
    if (!strcmp(suppliedname,getHMassvsBetaBasename())) h= getHMassvsBeta(add);
    if (!strcmp(suppliedname,getHMassChrgvsBetaBasename())) h= getHMassChrgvsBeta(add);
    if (!strcmp(suppliedname,getHMassBasename())) h= getHMass(add);
    if (!strcmp(suppliedname,getHMomBasename())) h= getHMom(add);
    if (!strcmp(suppliedname,getHMomtBasename())) h= getHMomt(add);
    if (!strcmp(suppliedname,getHDeflAngleBasename())) h= getHDeflAngle(add);
    if (!strcmp(suppliedname,getHDeflAnglevsMassBasename())) h= getHDeflAnglevsMass(add);
    if (!strcmp(suppliedname,getHDeflAnglevsBetaBasename())) h= getHDeflAnglevsBeta(add);
    if (!strcmp(suppliedname,getHDeflAnglevsMomBasename())) h= getHDeflAnglevsMom(add);
    if (!strcmp(suppliedname,getHMultBasename())) h= getHMult(add);
    if (!strcmp(suppliedname,getHCorrCodeGeantBasename())) h= getHCorrCodeGeant(add);
    if (!strcmp(suppliedname,getHKickPullBasename())) h= getHKickPull(add);
    if (!strcmp(suppliedname,getHBetaBasename())) h= getHBeta(add);
    if (!strcmp(suppliedname,getHPseudoRapidityBasename())) h= getHPseudoRapidity(add);
    if (!strcmp(suppliedname,getHGeantMomKickMomDiffBasename())) h= getHGeantMomKickMomDiff(add);
    if (!strcmp(suppliedname,getHMomCorrCodeGeantBasename())) h= getHMomCorrCodeGeant(add);
    if (!strcmp(suppliedname,getHCorrCodeGeantPolarAngleBasename())) h= getHCorrCodeGeantPolarAngle(add);
    if (!strcmp(suppliedname,getHPartCorrCodeGeantBasename())) h= getHPartCorrCodeGeant(add);
    if (h) cout<<"Histogram created: "<<h->GetName()<<endl;
    else cout<<"Error in HRichHistFac::getHistoByBaseName: could not create requested histo: "<<suppliedname<<" "<<add<<endl;
    return h;

}
TH1F* HRichHistFac::getHCorrCodeGeant(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHCorrCodeGeantBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=20;
    Int_t nlower=-10;
    Int_t nupper=10;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMomCorrCodeGeant(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomCorrCodeGeantBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=600;
    Float_t n1lower=-1500;
    Float_t n1upper=1500;
    Int_t n2bins=20;
    Int_t n2lower=-10;
    Int_t n2upper=10;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHCorrCodeGeantPolarAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHCorrCodeGeantPolarAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=20;
    Int_t n1lower=-10;
    Int_t n1upper=10;
    Int_t n2bins=90;
    Int_t n2lower=0;
    Int_t n2upper=90;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHPartCorrCodeGeant(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHPartCorrCodeGeantBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=50;
    Float_t n1lower=0;
    Float_t n1upper=50;
    Int_t n2bins=20;
    Int_t n2lower=-10;
    Int_t n2upper=10;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHGeantMomKickMomDiff(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHGeantMomKickMomDiffBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Float_t n1lower=0;
    Float_t n1upper=1500;
    Int_t n2bins=2000;
    Int_t n2lower=-10;
    Int_t n2upper=10;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHPseudoRapidity(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHPseudoRapidityBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=-5;
    Int_t nupper=5;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRapidity(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRapidityBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=-5;
    Int_t nupper=5;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=400;
    Int_t nlower=0;
    Int_t nupper=2;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHKickPull(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHKickPullBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=200;
    Int_t nlower=-10;
    Int_t nupper=10;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMomChrgvsPolarAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomChrgvsPolarAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=600;
    Float_t n1lower=-1500;
    Float_t n1upper=1500;
    Int_t n2bins=90;
    Int_t n2lower=0;
    Int_t n2upper=90;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMomtChrgvsPolarAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomtChrgvsPolarAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=600;
    Float_t n1lower=-1500;
    Float_t n1upper=1500;
    Int_t n2bins=90;
    Int_t n2lower=0;
    Int_t n2upper=90;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHPolarHitChrgvsAzimuth(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHPolarHitChrgvsAzimuthBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=200;
    Int_t nxlower=-100;
    Int_t nxupper=100;
    Int_t nybins=360;
    Int_t nylower=0;
    Int_t nyupper=360;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}

TH1F* HRichHistFac::getHOpeningAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHOpeningAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    //    Int_t nbins=180;
    Int_t nbins=24;
    Float_t xbins[] = {0.,2.,4.,6.,8.,10.,12.,14.,16.,18.,20.,
    25.,30.,35.,40.,45.,50.,60.,70.,80.,90.,
		       100.,120.,140.,160.,180.};
    
    //Int_t nlower=0;
    //Int_t nupper=180;
//     TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
     TH1F* h = new TH1F(s->Data(),s->Data(),nbins,xbins);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHPolarAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHPolarAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=180;
    Int_t nlower=-90;
    Int_t nupper=90;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHAzimuthalAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHAzimuthalAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=720;
    Int_t nlower=-360;
    Int_t nupper=360;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH1F* HRichHistFac::getHInvMass(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHInvMassBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=17;
    Float_t xbins[] = {0.,20.,40.,
		       60.,80.,
		       100.,120.,140.,
		       160.,
		       260.,
		       360.,460.,
		       560.,660.,
		       760.,860.,960.,1000.};

//     Int_t nbins=200;
//     Int_t nlower=0;
//     Int_t nupper=1000;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,xbins);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHTof(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHTofBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=0;
    Int_t nupper=50;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingXY(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingXYBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=96;
    Int_t nxlower=1;
    Int_t nxupper=96;
    Int_t nybins=96;
    Int_t nylower=1;
    Int_t nyupper=96;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingRadius(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingRadiusBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=60;
    Int_t nlower=1;
    Int_t nupper=7;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingRadiusTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingRadiusThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=60;
    Int_t nylower=1;
    Int_t nyupper=7;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHInvMassMomt(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHInvMassMomtBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=50;
    Int_t nxlower=0;
    Int_t nxupper=1000;
    Int_t nybins=50;//pt on x-axis
    Int_t nylower=0;
    Int_t nyupper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMomtRapidity(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomtRapidityBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=3;
    Int_t nxlower=0;
    Int_t nxupper=3;
    Int_t nybins=20;//pt on x-axis
    Int_t nylower=0;
    Int_t nyupper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHInvMassRapidity(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHInvMassRapidityBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=3;
    Int_t nxlower=0;
    Int_t nxupper=3;
    Int_t nybins=30;//pt on x-axis
    Int_t nylower=0;
    Int_t nyupper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHInvMassOpang(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHInvMassOpangBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=45;
    Int_t nxlower=0;
    Int_t nxupper=180;
    Int_t nybins=30;//pt on x-axis
    Int_t nylower=0;
    Int_t nyupper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMomtOpang(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomtOpangBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=45;
    Int_t nxlower=0;
    Int_t nxupper=180;
    Int_t nybins=50;//pt on x-axis
    Int_t nylower=0;
    Int_t nyupper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRapidityOpang(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRapidityOpangBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=45;
    Int_t nxlower=0;
    Int_t nxupper=180;
    Int_t nybins=3;//pt on x-axis
    Int_t nylower=0;
    Int_t nyupper=3;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingAmplitude(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAmplitudeBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=150;
    Int_t nlower=0;
    Int_t nupper=1500;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingAmplitudeTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAmplitudeThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=150;
    Int_t nylower=0;
    Int_t nyupper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingPadNr(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingPadNrBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=70;
    Int_t nlower=0;
    Int_t nupper=70;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingPadNrTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingPadNrThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=70;
    Int_t nylower=0;
    Int_t nyupper=70;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingLocMax4(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingLocMax4Basename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=20;
    Int_t nlower=0;
    Int_t nupper=20;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingLocMax4Theta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingLocMax4ThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=20;
    Int_t nylower=0;
    Int_t nyupper=20;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingPM(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingPMBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=150;
    Int_t nlower=0;
    Int_t nupper=1500;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingPMTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingPMThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=150;
    Int_t nylower=0;
    Int_t nyupper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingHT(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingHTBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=80;
    Int_t nlower=0;
    Int_t nupper=800;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingHTTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingHTThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=80;
    Int_t nylower=0;
    Int_t nyupper=800;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingCentroid(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingCentroidBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=0;
    Int_t nupper=5;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingCentroidTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingCentroidThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=5;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHRingAvChrg(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAvChrgBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=0;
    Int_t nupper=100;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHRingAvChrgTheta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAvChrgThetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=15;
    Int_t nxlower=0;
    Int_t nxupper=90;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=100;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
//--------------------------------
TH2F* HRichHistFac::getHRingAvChrgLocMax4(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAvChrgLocMax4Basename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=100;
    Int_t nybins=20;
    Int_t nylower=0;
    Int_t nyupper=20;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingAvChrgCentroid(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAvChrgCentroidBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=100;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=5;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingAvChrgPatMat(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAvChrgPatMatBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=100;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingAvChrgPadNr(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingAvChrgPadNrBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=100;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=100;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingLocMax4Centroid(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingLocMax4CentroidBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=20;
    Int_t nxlower=0;
    Int_t nxupper=20;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=5;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingLocMax4PatMat(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingLocMax4PatMatBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=20;
    Int_t nxlower=0;
    Int_t nxupper=20;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingLocMax4PadNr(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingLocMax4PadNrBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=20;
    Int_t nxlower=0;
    Int_t nxupper=20;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=100;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingCentroidPatMat(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingCentroidPatMatBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=5;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingCentroidPadNr(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingCentroidPadNrBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=5;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=100;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingPatMatPadNr(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingPatMatPadNrBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=100;
    Int_t nxlower=0;
    Int_t nxupper=1000;
    Int_t nybins=100;
    Int_t nylower=0;
    Int_t nyupper=100;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHRingParIdWeight(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHRingParIdWeightBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=40;
    Int_t nxlower=-10;
    Int_t nxupper=30;
    Int_t nybins=60;
    Int_t nylower=-1;
    Float_t nyupper=1.5;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHWeightWeight(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHWeightWeightBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=60;
    Int_t nxlower=-1;
    Int_t nxupper=1.5;
    Int_t nybins=60;
    Int_t nylower=-1;
    Float_t nyupper=1.5;
    TH2F* h = new TH2F(s->Data(),s->Data(),nxbins,nxlower,nxupper,
		                           nybins,nylower,nyupper);
    delete s;
    return h;
}

//----------------------------
TH1F* HRichHistFac::getHParId(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHParIdBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nxbins=20;
    Int_t nxlower=0;
    Int_t nxupper=20;
    TH1F* h = new TH1F(s->Data(),s->Data(),nxbins,nxlower,nxupper);
    delete s;
    return h;
}

//----------------------------
TH1F* HRichHistFac::getHMom(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=300;
    Int_t nlower=0;
    Int_t nupper=1500;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHMomt(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomtBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=300;
    Int_t nlower=0;
    Int_t nupper=1500;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMomvsMom(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomvsMomBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=600;
    Int_t n1lower=-1500;
    Int_t n1upper=1500;
    Int_t n2bins=600;
    Int_t n2lower=-1500;
    Int_t n2upper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMdcRvsMdcZ(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMdcRvsMdcZBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=1500;
    Int_t n1lower=-100;
    Int_t n1upper=50;
    Int_t n2bins=2000;
    Int_t n2lower=-10;
    Int_t n2upper=10;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMdcRvsMdcChi2(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMdcRvsMdcChi2Basename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=110;
    Int_t n1lower=-10;
    Int_t n1upper=100;
    Int_t n2bins=2000;
    Int_t n2lower=-10;
    Int_t n2upper=10;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMomvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=140;
    Float_t n1lower=0.;
    Float_t n1upper=1.4;
    Int_t n2bins=300;
    Int_t n2lower=0;
    Int_t n2upper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMomChrgvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomChrgvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=140;
    Float_t n1lower=0.;
    Float_t n1upper=1.4;
    Int_t n2bins=600;
    Int_t n2lower=-1500;
    Int_t n2upper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMomvsEnergy(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomvsEnergyBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Int_t n1lower=0;
    Int_t n1upper=1500;
    Int_t n2bins=300;
    Int_t n2lower=0;
    Int_t n2upper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMomChrgvsEnergy(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomChrgvsEnergyBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=600;
    Int_t n1lower=-1500;
    Int_t n1upper=1500;
    Int_t n2bins=300;
    Int_t n2lower=0;
    Int_t n2upper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HRichHistFac::getHMassvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMassvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Int_t n1lower=0;
    Int_t n1upper=1500;
    Int_t n2bins=140;
    Int_t n2lower=0;
    Float_t n2upper=1.4;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHMassChrgvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMassChrgvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Int_t n1lower=-1500;
    Int_t n1upper=1500;
    Int_t n2bins=140;
    Int_t n2lower=0;
    Float_t n2upper=1.4;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHMass(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMassBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=150;
    Int_t nlower=0;
    Int_t nupper=1500;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HRichHistFac::getHDeflAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHDeflAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=150;
    Int_t nlower=-30;
    Int_t nupper=30;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHDeflAnglevsMass(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHDeflAnglevsMassBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=150;
    Int_t n1lower=-30;
    Int_t n1upper=30;
    Int_t n2bins=150;
    Int_t n2lower=0;
    Int_t n2upper=1500;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHDeflAnglevsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHDeflAnglevsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=150;
    Int_t n1lower=-30;
    Int_t n1upper=30;
    Int_t n2bins=140;
    Int_t n2lower=0;
    Float_t n2upper=1.4;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HRichHistFac::getHDeflAnglevsMom(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHDeflAnglevsMomBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=1200;
    Int_t n1lower=-3;
    Int_t n1upper=3;
    Int_t n2bins=200;
    Int_t n2lower=-1000;
    Float_t n2upper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
//--------------------------------------------------------------------
TH1F* HRichHistFac::getHMult(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMultBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=20;
    Int_t nlower=1;
    Int_t nupper=20;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}

TCanvas* HRichHistFac::makeMicroCanvas(TObjArray* pHistArray)
{
    TH1 *h1;
    Int_t cntr = pHistArray->GetLast()+1;
    TCanvas * c1 = new TCanvas("microCanvas","microCanvas",10,10,1200,1000); 
    Float_t help = cntr;
    Int_t plusone=0;
    if (TMath::Sqrt(help)-TMath::Nint(TMath::Sqrt(help)) >0) plusone=1;
    Int_t div = TMath::Nint(TMath::Sqrt(help));
    c1->Divide(div,div+plusone);
    for (Int_t i=0;i<cntr;i++)
    {

	h1 = (TH1*)(*pHistArray)[i];
	c1->cd(i+1);
	if ( 
	    !strcmp((h1->IsA())->GetName(),"TH2F") || 
	    !strcmp((h1->IsA())->GetName(),"TH2D") ||
	    !strcmp((h1->IsA())->GetName(),"TH2S")   )
	{
	    h1->DrawCopy("colz");
	}else{
	    h1->DrawCopy();
	}
    }
    c1->Modified();
    c1->Update();
    return c1;
}







