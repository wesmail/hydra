#include "htrendhist.h"
#include "iostream.h"

ClassImp(HTrendHist)

HTrendHist::HTrendHist(const char* name   ="",
		       const char* title  ="",
		       Int_t active=0,Int_t resetable=0,
		       Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0)
    :  HMonHist(name,title,active,resetable,nbinsX,xlo,xup)
{
}
HTrendHist::HTrendHist(HMonHistAddon& add)
{
    copySet(add);
    h=0;
    create();
}
void HTrendHist::fill(Float_t a, Float_t b)
{
    for(Int_t i=0;i<h->GetNbinsX();i++){h->SetBinContent(i,h->GetBinContent(i+1) );}
    h->SetBinContent(h->GetNbinsX(),a);
    //h->SetBinError(h->GetNbinsX(),b);
}
HTrendHist::~HTrendHist(){}
