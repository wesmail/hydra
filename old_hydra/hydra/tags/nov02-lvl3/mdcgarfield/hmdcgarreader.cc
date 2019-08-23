//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HMdcGarReader
// Class to read several GARFIELD formats
////////////////////////////////////////////////////////////////////////////
using namespace std;
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
#include "hmdcgarreader.h"
#include "htool.h"
#include "TMath.h"
#include "TArray.h"
#include "TStyle.h"

ClassImp(HMdcGarReader)

HMdcGarReader::HMdcGarReader(const char* name,const char* title)
    : TNamed(name,title)
{
    // constructor for HMdcGarReader
    initVariables();
}
HMdcGarReader::~HMdcGarReader()
{
  // destructor of HMdcGarReader
}
void HMdcGarReader::initVariables()
{
    // inits all variables
}
TGraph* HMdcGarReader::readXT(TString inputname,Int_t color)
{
    // Reads GARFIELD XT curve from ascii input "inputname" into a TGraph
    // with LineColor "color", which is returned

    gStyle->SetPalette(50);
    FILE* input=0;
    if(!HTool::openAscii(&input,inputname,"r"))
    {
	exit(1);
    }
    cout<<"HMdcGarReader::readXT() , Reading XT-data from GARFIELD file\n"<<inputname.Data()<<endl;

    TArrayF distance;
    TArrayF drifttime;

    Int_t numberOfBins=0;

    Char_t line[400];
    Int_t scan_count;
    Float_t  f_x ,f_t;
    Char_t Tag[]=" This member contains";

    while(1)
    {
	if(feof(input)) break;
	fgets(line, sizeof(line), input);

	if (!strncmp(line, Tag ,strlen(Tag)))
	{
            sscanf(line,"%*s%*s%*s%i%*s",&numberOfBins);
	    distance .Set(numberOfBins);
            drifttime.Set(numberOfBins);
	    break;
	}
    }
    Int_t count=0;
    while (count<numberOfBins)  {
	if (feof(input)) break;

	fgets(line, sizeof(line), input);
	scan_count=sscanf(line,"%*s%e%e",&f_x, &f_t);

	if(scan_count==2)
	{
	    distance [count]=f_x*10;
	    drifttime[count]=f_t*1000;
	    printf("%03i distance %7.3f mm drift time %7.3f ns\n",count,distance [count],drifttime[count]);
	    count++;
	}
    }

    Float_t* gdist =new Float_t[count];
    Float_t* gdrift=new Float_t[count];

    for(Int_t i=0;i<count;i++)
    {
        gdist [i]=distance [i];
        gdrift[i]=drifttime[i];
    }

    TGraph* h=new TGraph(count,gdist,gdrift);

    h->SetMarkerStyle(29);
    h->SetMarkerColor(4);
    h->SetMarkerSize(.8);
    h->SetLineColor(color);

    HTool::closeAscii(&input);
    return h;
}
TGraph* HMdcGarReader::readMagboltz(TString inputname,Int_t color)
{
    // Reads MAGBOLTZ vdrift/E (E/P:Log10) curve from ascii input "inputname" into a TGraph
    // with LineColor "color", which is returned
    TGraph* h=0;

    gStyle->SetPalette(50);
    FILE* input=0;
    if(!HTool::openAscii(&input,inputname,"r"))
    {
	exit(1);
    }
    cout<<"HMdcGarReader::readMagboltz() , Reading Vdrift/E-data from MAGBOLTZ file\n"<<inputname.Data()<<endl;

    TArrayF ep;
    TArrayF Vdrift;

    Int_t numberOfBins=0;

    Char_t line[400];
    Int_t scan_count;
    Float_t  f_ep ,f_vdrift;
    Char_t Tag []=" The gas tables follow";
    Char_t Tag1[]=" Dimension : F";
    TString buffer=" ";
    while(1)
    {
	if(feof(input)) break;
	fgets(line, sizeof(line), input);
	if (!strncmp(line, Tag1 ,strlen(Tag1)))
	{
	    sscanf(line,"%*s%*s%*s%2i%*s%*s",&numberOfBins);
	    ep    .Set(numberOfBins);
            Vdrift.Set(numberOfBins);
	    break;
	}
    }

    while(1)
    {
        if(feof(input)) break;
        fgets(line, sizeof(line), input);
        if (!strncmp(line, Tag ,strlen(Tag))) break;
    }

    Int_t count=0;
    while (count<numberOfBins)  {
	if (feof(input)) break;
	fgets(line, sizeof(line), input);
	scan_count=sscanf(line,"%e%e%*s",
			  &f_ep, &f_vdrift);
        ep    [count]=f_ep;
        Vdrift[count]=f_vdrift*10;
	printf("%03i ep %12.2f kV/cm*bar Vdrift %7.3f ns/mu\n",count,ep[count],Vdrift[count]);

	fgets(line, sizeof(line), input);
	fgets(line, sizeof(line), input);
	count++;
    };

    Float_t* gep    =new Float_t[count];
    Float_t* gVdrift=new Float_t[count];

    for(Int_t i=0;i<count;i++)
    {
        gep    [i]=ep    [i];
	gVdrift[i]=Vdrift[i];
    }

    h=new TGraph(count,gep,gVdrift);

    h->SetMarkerStyle(29);
    h->SetMarkerColor(4);
    h->SetMarkerSize(.8);
    h->SetLineColor(color);

    HTool::closeAscii(&input);
    return h;
}
