// File: hrichhistfac.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/05/10 14:06:12
//

#include "hrichhistfac.h"
#include "hdebug.h"
#include "hades.h"
#include <TH1.h>

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

TH1F* HRichHistFac::getHOpeningAngle(const char * c,const char *b)
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
TH1F* HRichHistFac::getHInvMass(const char * c,const char *b)
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
    Int_t nxbins=30;
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
    Int_t nxbins=30;
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
    Int_t nxbins=30;
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
    Int_t nxbins=30;
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
    Int_t nxbins=30;
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

TH2F* HRichHistFac::getHMomvsMom(const char * c,const char *b)
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
