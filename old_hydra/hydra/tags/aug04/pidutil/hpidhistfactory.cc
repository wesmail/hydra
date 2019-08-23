// $Id: 
// Last update by Thomas Eberl: 03/02/26 09:31:41
//
using namespace std;
#include "hpidhistfactory.h"
#include "hdebug.h"
#include "hades.h"
#include <TH1.h>
#include <string>
#include <iostream>
#include <iomanip>
///////////////////////////////////////////////////////////
// HPidHistFactory
//
// HPid Histogram Factory
//
// this class defines default histograms for detector specific and
// higher level analysis.
// there is a "get"-function for each histogram
// the user can get a pointer to a histogram either directly
// via a get function or via supplying the basename of the histogram 
///////////////////////////////////////////////////////////

ClassImp(HPidHistFactory)

HPidHistFactory::HPidHistFactory()
{

}


HPidHistFactory::~HPidHistFactory(void) 
{

}


TH1*  HPidHistFactory::getHistoByBaseName(const char* suppliedname,const char* add)
{
    TH1 *h=0;
    if (!strcmp(suppliedname,getHOpeningAngleBasename())) h= getHOpeningAngle(add);
    if (!strcmp(suppliedname,getHInvMassBasename())) h= getHInvMass(add);
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
    if (!strcmp(suppliedname,getHMomvsMomBasename())) h= getHMomvsMom(add);
    if (!strcmp(suppliedname,getHMomvsBetaBasename())) h= getHMomvsBeta(add);
    if (!strcmp(suppliedname,getHMomChrgvsBetaBasename())) h= getHMomChrgvsBeta(add);
    if (!strcmp(suppliedname,getHMomvsEnergyBasename())) h= getHMomvsEnergy(add);
    if (!strcmp(suppliedname,getHMomChrgvsEnergyBasename())) h= getHMomChrgvsEnergy(add);
    if (!strcmp(suppliedname,getHMassvsBetaBasename())) h= getHMassvsBeta(add);
    if (!strcmp(suppliedname,getHMassChrgvsBetaBasename())) h= getHMassChrgvsBeta(add);
    if (!strcmp(suppliedname,getHMassBasename())) h= getHMass(add);
    if (!strcmp(suppliedname,getHDeflAngleBasename())) h= getHDeflAngle(add);
    if (!strcmp(suppliedname,getHDeflAnglevsMassBasename())) h= getHDeflAnglevsMass(add);
    if (!strcmp(suppliedname,getHDeflAnglevsBetaBasename())) h= getHDeflAnglevsBeta(add);
    if (h) cout<<"Histogram created: "<<h->GetName()<<endl;
    else cout<<"Error in HPidHistFactory::getHistoByBaseName: could not create requested histo: "<<suppliedname<<" "<<add<<endl;
    return h;

}

TH1F* HPidHistFactory::getHOpeningAngle(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHOpeningAngleBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=0;
    Int_t nupper=100;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HPidHistFactory::getHInvMass(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHInvMassBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t nbins=100;
    Int_t nlower=0;
    Int_t nupper=1000;
    TH1F* h = new TH1F(s->Data(),s->Data(),nbins,nlower,nupper);
    delete s;
    return h;
}
TH1F* HPidHistFactory::getHTof(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingXY(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingRadius(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingRadiusTheta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingAmplitude(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingAmplitudeTheta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingPadNr(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingPadNrTheta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingLocMax4(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingLocMax4Theta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingPM(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingPMTheta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingHT(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingHTTheta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingCentroid(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingCentroidTheta(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHRingAvChrg(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHRingAvChrgTheta(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingAvChrgLocMax4(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingAvChrgCentroid(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingAvChrgPatMat(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingAvChrgPadNr(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingLocMax4Centroid(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingLocMax4PatMat(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingLocMax4PadNr(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingCentroidPatMat(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingCentroidPadNr(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHRingPatMatPadNr(const char * c,const char *b)
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
//----------------------------
TH2F* HPidHistFactory::getHMomvsMom(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomvsMomBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=200;
    Int_t n1lower=0;
    Int_t n1upper=1000;
    Int_t n2bins=200;
    Int_t n2lower=0;
    Int_t n2upper=1000;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HPidHistFactory::getHMomvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Int_t n1lower=0;
    Int_t n1upper=1500;
    Int_t n2bins=200;
    Int_t n2lower=0;
    Int_t n2upper=2;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TH2F* HPidHistFactory::getHMomChrgvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMomChrgvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=600;
    Int_t n1lower=-1500;
    Int_t n1upper=1500;
    Int_t n2bins=200;
    Int_t n2lower=0;
    Int_t n2upper=2;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HPidHistFactory::getHMomvsEnergy(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHMomChrgvsEnergy(const char * c,const char *b)
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

TH2F* HPidHistFactory::getHMassvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMassvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Int_t n1lower=0;
    Int_t n1upper=1500;
    Int_t n2bins=200;
    Int_t n2lower=0;
    Int_t n2upper=2;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH2F* HPidHistFactory::getHMassChrgvsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHMassChrgvsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=300;
    Int_t n1lower=-1500;
    Int_t n1upper=1500;
    Int_t n2bins=200;
    Int_t n2lower=0;
    Int_t n2upper=2;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}
TH1F* HPidHistFactory::getHMass(const char * c,const char *b)
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
TH1F* HPidHistFactory::getHDeflAngle(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHDeflAnglevsMass(const char * c,const char *b)
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
TH2F* HPidHistFactory::getHDeflAnglevsBeta(const char * c,const char *b)
{
    TString *s=0;
    if (b==NULL) s=new TString(getHDeflAnglevsBetaBasename());
    else s=new TString(b);
    if (c) s->Append(c);
    Int_t n1bins=150;
    Int_t n1lower=-30;
    Int_t n1upper=30;
    Int_t n2bins=200;
    Int_t n2lower=0;
    Int_t n2upper=2;
    TH2F* h = new TH2F(s->Data(),s->Data(),n1bins,n1lower,n1upper,
		                           n2bins,n2lower,n2upper);
    delete s;
    return h;
}

TCanvas* HPidHistFactory::makeMicroCanvas(TObjArray* pHistArray)
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







