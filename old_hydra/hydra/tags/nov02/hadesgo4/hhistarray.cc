#include "hhistarray.h"
#include "TMath.h"
#include "TLine.h"
#include "TStyle.h"
#include "TPostScript.h"
#include <TROOT.h>
ClassImp(HNamedLegend)
ClassImp(HLegendDrawer)
ClassImp(HTrendArray)
ClassImp(HHistArray)
ClassImp(HHistArray2)

HNamedLegend::HNamedLegend (const char* name   ="",
			    const char* title  ="",HMonHistAddon* h)
    :TNamed(name,title)
{
    if(h)
    {
	Int_t s1,s2;
	TString temp;
      
	legend=new TLegend(0.8,0.8,0.89,0.89);

	legend->SetFillStyle(0);
        legend->SetFillColor(19);
        legend->SetLineColor(1);
        legend->SetLineStyle(1);
        legend->SetDrawOption("lego");

	if(strcmp(h->ClassName(),"HHistArray")==0)
	{
            ((HHistArray*)h)->getDim(&s1,&s2);
	    for(Int_t i=0;i<s1;i++){
		for(Int_t j=0;j<s2;j++){
		    if(s1>1){sprintf((char*)temp.Data()," %i %i",i,j);}
		    else {sprintf((char*)temp.Data()," %i",j);}
                    TH1* myhist=((TH1*)(((HHistArray*)h)->getP(i,j)));
		    legend->AddEntry(myhist,temp.Data(),"l");
		}
	    }

	}
	else if(strcmp(h->ClassName(),"HTrendArray")==0)
	{
	    ((HTrendArray*)h)->getDim(&s1,&s2);
	    for(Int_t i=0;i<s1;i++){
		for(Int_t j=0;j<s2;j++){
		    if(s1>1){sprintf((char*)temp.Data(),"%i %i",i,j);}
		    else {sprintf((char*)temp.Data(),"%i",j);}
		    TH1* myhist=((TH1*)(((HTrendArray*)h)->getP(i,j)));
		    legend->AddEntry(myhist,temp.Data(),"l");
		}
	    }
	}
    }
}
HNamedLegend::~HNamedLegend()
{
    if(legend)delete legend;
}

//###########################################################

HLegendDrawer::HLegendDrawer(const char* name   ="",
			     const char* title  ="")
    :TNamed(name,title)
{
   array=new TObjArray();
}
HLegendDrawer::~HLegendDrawer()
{
    if(array)array->Delete();
}
void HLegendDrawer::draw(HMonHistAddon* h)
{
    if(strcmp(h->ClassName(),"HHistArray")==0 || strcmp(h->ClassName(),"HTrendArray")==0 )
    {
        TString strname;
	strname="Leg";
	strname+=h->GetName();
	HNamedLegend* myleg=((HNamedLegend*)(array->FindObject(strname.Data())));
	if(myleg)
	{
	    ((TLegend*) (((HNamedLegend*)myleg)->getP()))->Draw("same");
	}
	else
	{
 	    myleg =add(h);
	    ((TLegend*) (((HNamedLegend*)myleg)->getP()))->Draw("same");
	}
    }
}
HNamedLegend* HLegendDrawer::add (HMonHistAddon* h)
{
    TString strname;
    strname="Leg";
    strname+=h->GetName();

    HNamedLegend* myleg=new HNamedLegend(strname.Data(),h->GetTitle(),h);
    array->AddLast(myleg);
    return (HNamedLegend*)myleg;
}

//###################################################################################


HTrendArray::HTrendArray(const char* name   ="",
			 const char* title  ="",
			 Int_t active=0,Int_t resetable=0,Int_t size1=1,Int_t size2=1,
			 Int_t nbinsX=0,Float_t xlo=0,Float_t xup=0)
    :HMonHistAddon (name,title,active,resetable,nbinsX,xlo,xup,0,0.F,0.F)
{
    setSize1(size1);
    setSize2(size2);
    array=0;
}
HTrendArray::HTrendArray(HMonHistAddon& add)
{
    copySet(add);
    array=0;
    create();
}
void    HTrendArray::create()
{
    if(getActive()==1)
    {
	array = new TObjArray((arraySize1*arraySize2)+1);
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
    	sprintf(namehist,"%s%s",this->GetName(),"_dummy");
	array->AddAt(new TH1S(namehist,this->GetTitle(),2,Xlo,Xup),(arraySize1*arraySize2));
	((TH1*)((*array)[arraySize1*arraySize2]))->SetDirectory(0);
	setHist((TH1*)((*array)[arraySize1*arraySize2]));
    }
}
void    HTrendArray::drawAll( Bool_t allowHiddenZero=1 )
{
    if(getActive()==1)
    {
	Float_t* maxvalues=new Float_t [(arraySize1*arraySize2)];
	Float_t* minvalues=new Float_t [(arraySize1*arraySize2)];
        TH1* htemp;
	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
            htemp=((TH1*)((*array)[i]));
	    maxvalues[i]=htemp->GetBinContent(htemp->GetMaximumBin());
	    minvalues[i]=htemp->GetBinContent(htemp->GetMinimumBin());
	}

	Int_t   loc =TMath::LocMax((arraySize1*arraySize2),maxvalues);
	Int_t   loc1=TMath::LocMin((arraySize1*arraySize2),minvalues);

        Float_t MAX,MIN;
	if(max==-99){
	     MAX=maxvalues[loc]*1.1;
	} else MAX=max;

	if(min==-99){
	    if(logY==0)MIN=minvalues[loc1];
	    else MIN=maxvalues[loc]*0.001;
	} else MIN=min;

	htemp=((TH1*)((*array)[arraySize1*arraySize2]));

	if(allowHiddenZero!=0 || logY!=0 ) { htemp->SetMinimum(MIN);   }
	else                               { htemp->SetMinimum(-1111); }

	htemp->SetMaximum(MAX);
        htemp->Draw();

	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
 	    htemp=((TH1*)((*array)[i]));
	    htemp->Draw("same");
	}
	delete [] maxvalues;
	delete [] minvalues;
    }
}
void    HTrendArray::fill(Int_t s,Int_t m,Stat_t a)
{
    if(array)
    {
	TH1* h=((TH1F*) (*array)[s*arraySize2+m]);
	for(Int_t i=0;i<getNBinsX();i++){h->SetBinContent(i,h->GetBinContent(i+1) );}
	h->SetBinContent(h->GetNbinsX(),a);
	//h->SetBinError(h->GetNbinsX(),b);
    }else Error("HTrendArray::fill()","ARRAY DOES NOT EXISI!");
}
void    HTrendArray::reset(Int_t level,Int_t count)
{
    switch (level)
    {
    case 0:   if(array){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
    break;
    case 1:   if(array&&getResetable()==1){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
    break;
    case 2:   if(array&&getResetable()==1 && count%getRefreshRate()==0 && count>0){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
    break;
    default : Error("HTrendArray::reset","UNKNOWN ARGUMENT: Level= %i !",level);
    }
}
void    HTrendArray::add(HMonHistAddon* h)
{
    if(array)
    {
	for(Int_t i=0;i<arraySize1;i++){
	    for(Int_t j=0;j<arraySize2;j++){
		((TH1*)((*array)[i*arraySize2+j]))->Add((TH1*)(((HHistArray*)h)->getP(i,j)));
	    }
	}
    }
    else Error("HTrendArray::add()","ARRAY DOES NOT EXISI!");
}
void    HTrendArray::draw( Bool_t allowHiddenZero=1 )
{
   drawAll(allowHiddenZero);
}
HTrendArray::~HTrendArray(){if(array)array->Delete();array=0;}


//#################################################################################################

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
	array = new TObjArray((arraySize1*arraySize2)+1);
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
	sprintf(namehist,"%s%s",this->GetName(),"_dummy");
	array->AddAt(new TH1S(namehist,this->GetTitle(),nBinsX,Xlo,Xup),(arraySize1*arraySize2));
	((TH1*)((*array)[arraySize1*arraySize2]))->SetDirectory(0);
	setHist((TH1*)((*array)[arraySize1*arraySize2]));
    }
}
void    HHistArray::drawAll(Bool_t allowHiddenZero=1)
{

    if(getActive()==1)
    {
	Float_t* maxvalues=new Float_t [(arraySize1*arraySize2)];
	Float_t* minvalues=new Float_t [(arraySize1*arraySize2)];
        TH1* htemp;
	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
            htemp=((TH1*)((*array)[i]));
	    maxvalues[i]=htemp->GetBinContent(htemp->GetMaximumBin());
	    minvalues[i]=htemp->GetBinContent(htemp->GetMinimumBin());
	}

	Int_t   loc =TMath::LocMax((arraySize1*arraySize2),maxvalues);
	Int_t   loc1=TMath::LocMin((arraySize1*arraySize2),minvalues);

        Float_t MAX,MIN;
	if(max==-99){
	     MAX=maxvalues[loc]*1.1;
	} else MAX=max;
	if(min==-99){
	    if(logY==0)MIN=minvalues[loc1];
	    else MIN=maxvalues[loc]*0.001;
	} else MIN=min;

	htemp=((TH1*)((*array)[arraySize1*arraySize2]));

	if(allowHiddenZero!=0 || logY!=0 ) { htemp->SetMinimum(MIN);   }
	else                               { htemp->SetMinimum(-1111); }

	htemp->SetMaximum(MAX);
        htemp->Draw();

	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
 	    htemp=((TH1*)((*array)[i]));
	    htemp->Draw("same");
	}
	delete [] maxvalues;
	delete [] minvalues;
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
    case 2:   if(array&&getResetable()==1 && count%getRefreshRate()==0 && count>0){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH1*)(*array)[i])->Reset();}}
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
void    HHistArray::draw( Bool_t allowHiddenZero=1 )
{
    drawAll(allowHiddenZero);
}
HHistArray::~HHistArray(){if(array)array->Delete();array=0;}

//############################################################################################################


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
	array = new TObjArray((arraySize1*arraySize2)+1);
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
	sprintf(namehist,"%s%s",this->GetName(),"_dummy");
	array->AddAt(new TH2S(namehist,this->GetTitle(),nBinsX,Xlo,Xup,2,Ylo,Yup),(arraySize1*arraySize2));
	((TH2*)((*array)[arraySize1*arraySize2]))->SetDirectory(0);
	setHist((TH2*)((*array)[arraySize1*arraySize2]));
    }
}
void    HHistArray2::drawAll( Bool_t allowHiddenZero=1 )
{

    if(getActive()==1)
    {
	Float_t* maxvalues=new Float_t [(arraySize1*arraySize2)];
	Float_t* minvalues=new Float_t [(arraySize1*arraySize2)];
        TH2* htemp;
	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
            htemp=((TH2*)((*array)[i]));
	    maxvalues[i]=htemp->GetBinContent(htemp->GetMaximumBin());
	    minvalues[i]=htemp->GetBinContent(htemp->GetMinimumBin());
	}

	Int_t   loc =TMath::LocMax((arraySize1*arraySize2),maxvalues);
	Int_t   loc1=TMath::LocMin((arraySize1*arraySize2),minvalues);

        Float_t MAX,MIN;
	if(max==-99){
	     MAX=maxvalues[loc]*1.1;
	} else MAX=max;
	if(min==-99){
	    if(logY==0)MIN=minvalues[loc1];
	    else MIN=maxvalues[loc]*0.001;
	} else MIN=min;

	htemp=((TH2*)((*array)[arraySize1*arraySize2]));

	if(allowHiddenZero!=0 || logY!=0 ) { htemp->SetMinimum(MIN);   }
	else                               { htemp->SetMinimum(-1111); }

	htemp->SetMaximum(MAX);
        htemp->Draw();

	for(Int_t i=0;i<(arraySize1*arraySize2);i++){
 	    htemp=((TH2*)((*array)[i]));
	    htemp->Draw("same");
	}
	delete [] maxvalues;
	delete [] minvalues;
    }
}
void    HHistArray2::draw( Bool_t allowHiddenZero=1 )
{
    drawAll(allowHiddenZero);
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
    case 2:   if(array&&getResetable()==1 && count%getRefreshRate()==0 && count>0){for(Int_t i=0;i<arraySize1*arraySize2;i++){((TH2*)(*array)[i])->Reset();}}
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
