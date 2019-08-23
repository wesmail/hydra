#include "hhistarray.h"
#include "TMath.h"

ClassImp(HHistArray)
ClassImp(HHistArray2)

HHistArray::HHistArray(const char* name   ="",
			     const char* title  ="",
			     Int_t active=0,Int_t resetable=0,Int_t size1=1,Int_t size2=1,
			     Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0)
    :HMonHistAddon (name,title,active,resetable,nbinsX,xlo,xup,0,0.F,0.F)
{
    setSize1(size1);
    setSize2(size2);
    array=0;
}
HHistArray::HHistArray(HMonHistAddon& add)
{
    copySet(add);
    array=0;
    create();
}
void    HHistArray::create()
{
    if(getActive()==1)
    {
	array = new TObjArray(arraySize1*arraySize2);
	Char_t namehist[400];
	for(Int_t i=0;i<arraySize1;i++){
	    for(Int_t j=0;j<arraySize2;j++){
		sprintf(namehist,"%s%s%i%s%i%s",this->GetName(),"_[",i,"][",j,"]");
		if(HistType.CompareTo("1S")==0)array->AddAt(new TH1S(namehist,this->GetTitle(),nBinsX,Xlo,Xup),i*arraySize2+j);
                if(HistType.CompareTo("1F")==0)array->AddAt(new TH1F(namehist,this->GetTitle(),nBinsX,Xlo,Xup),i*arraySize2+j);
                if(HistType.CompareTo("1D")==0)array->AddAt(new TH1D(namehist,this->GetTitle(),nBinsX,Xlo,Xup),i*arraySize2+j);
                ((TH1*)((*array)[i*arraySize2+j]))->SetDirectory(0);
		setHist((TH1*)((*array)[i*arraySize2+j]));
	    }
	}
    }
}
void    HHistArray::drawAll()
{

    if(getActive()==1)
    {
	Float_t* maxvalues=new Float_t [arraySize1*arraySize2];
	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
	    maxvalues[i]=((TH1*)((*array)[i]))->GetMaximum();
	}

	Int_t   maximumloc=TMath::LocMax(arraySize1*arraySize2,maxvalues);

	((TH1*)((*array)[maximumloc]))->Draw();
	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
	    if(i!=maximumloc)((TH1*)((*array)[i]))->Draw("same");
	}
	delete maxvalues;
    }
}
void    HHistArray::fill(Int_t s,Int_t m,Stat_t a){((TH1F*) (*array)[s*arraySize2+m])->Fill(a);}
void    HHistArray::setBinContent(Int_t s,Int_t m,Int_t Nbin,Stat_t a){((TH1*) (*array)[s*arraySize2+m])->SetBinContent(Nbin,a);}
Stat_t  HHistArray::getBinContent(Int_t s,Int_t m,Int_t Nbin)          {return ((TH1*) (*array)[s*arraySize2+m])->GetBinContent(Nbin);}
void    HHistArray::setBinError(Int_t s,Int_t m,Int_t Nbin,Stat_t a)  {((TH1*) (*array)[s*arraySize2+m])->SetBinError(Nbin,a);}
Stat_t  HHistArray::getBinError(Int_t s,Int_t m,Int_t Nbin)            {return ((TH1*) (*array)[s*arraySize2+m])->GetBinError(Nbin);}
void    HHistArray::reset(Int_t level,Int_t count)
{
    switch (level)
    {
    case 0:   if(array){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
    break;
    case 1:   if(array&&getResetable()==1){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
    break;
    case 2:   if(array&&getResetable()==1){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
    break;
    default : Error("HHistArray::reset","UNKNOWN ARGUMENT: Level= %i !",level);
    }
}
void    HHistArray::add(HMonHistAddon* h)
{
    if(array)
    {
	for(Int_t i=0;i<arraySize1;i++){
	    for(Int_t j=0;j<arraySize2;j++){
		((TH1*)((*array)[i*arraySize2+j]))->Add((TH1*)(((HHistArray*)h)->getP(i,j)));
	    }
	}
    }
    else Error("HHistArray::add()","ARRAY DOES NOT EXISI!");
}
HHistArray::~HHistArray(){if(array)array->Delete();array=0;}

HHistArray2::HHistArray2(const char* name   ="",
			       const char* title  ="",
			       Int_t active=0,Int_t resetable=0,Int_t size1=1,Int_t size2=1,
			       Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0)
    :HMonHistAddon (name,title,active,resetable,nbinsX,xlo,xup,0,0.F,0.F)
{
    setSize1(size1);
    setSize2(size2);
    array=0;
}
HHistArray2::HHistArray2(HMonHistAddon& add)
{
    copySet(add);
    array=0;
    create();
}
void    HHistArray2::create()
{
    if(getActive()==1)
    {
	array = new TObjArray(arraySize1*arraySize2);
	Char_t namehist[400];
	for(Int_t i=0;i<arraySize1;i++){
	    for(Int_t j=0;j<arraySize2;j++){
		sprintf(namehist,"%s%s%i%s%i%s",this->GetName(),"_[",i,"][",j,"]");
		if(HistType.CompareTo("2S")==0)array->AddAt(new TH2S(namehist,this->GetTitle(),nBinsX,Xlo,Xup,nBinsY,Ylo,Yup),i*arraySize2+j);
                if(HistType.CompareTo("2F")==0)array->AddAt(new TH2F(namehist,this->GetTitle(),nBinsX,Xlo,Xup,nBinsY,Ylo,Yup),i*arraySize2+j);
                if(HistType.CompareTo("2D")==0)array->AddAt(new TH2D(namehist,this->GetTitle(),nBinsX,Xlo,Xup,nBinsY,Ylo,Yup),i*arraySize2+j);
                ((TH2*)((*array)[i*arraySize2+j]))->SetDirectory(0);
		setHist((TH2*)((*array)[i*arraySize2+j]));
	    }
	}
    }
}
void    HHistArray2::fill(Int_t s,Int_t m,Stat_t a,Stat_t b){((TH2*) (*array)[s*arraySize2+m])->Fill(a,b);}
void    HHistArray2::setBinContent(Int_t s,Int_t m,Int_t Nbin,Int_t N2bin,Stat_t a){((TH2*) (*array)[s*arraySize2+m])->SetBinContent(Nbin,N2bin,a);}
Stat_t  HHistArray2::getBinContent(Int_t s,Int_t m,Int_t Nbin,Int_t N2bin)          {return ((TH2*) (*array)[s*arraySize2+m])->GetBinContent(Nbin,N2bin);}
void    HHistArray2::setBinError(Int_t s,Int_t m,Int_t Nbin,Int_t N2bin,Stat_t a)  {((TH2*) (*array)[s*arraySize2+m])->SetBinError(Nbin,N2bin,a);}
Stat_t  HHistArray2::getBinError(Int_t s,Int_t m,Int_t Nbin,Int_t N2bin)            {return ((TH2*) (*array)[s*arraySize2+m])->GetBinError(Nbin,N2bin);}
void    HHistArray2::reset(Int_t level,Int_t count)
{
    switch (level)
    {
    case 0:   if(array){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH2*)(*array)[i])->Reset();}}
    break;
    case 1:   if(array&&getResetable()==1){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH2*)(*array)[i])->Reset();}}
    break;
    case 2:   if(array&&getResetable()==1){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH2*)(*array)[i])->Reset();}}
    break;
    default : Error("HHistArray2::reset","UNKNOWN ARGUMENT: Level= %i !",level);
    }
}
void    HHistArray2::add(HMonHistAddon* h)
{
    if(array)
    {
	for(Int_t i=0;i<arraySize1;i++){
	    for(Int_t j=0;j<arraySize2;j++){
		((TH2*)((*array)[i*arraySize2+j]))->Add(((HHistArray2*)h)->getP(i,j));
	    }
	}
    }
    else Error("HHistArray2::add()","ARRAY DOES NOT EXISI!");
}
HHistArray2::~HHistArray2(){if(array)array->Delete();array=0;}
