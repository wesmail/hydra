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
TGraph* HMdcGarReader::readXTdiffusion(TString inputname,Int_t color)
{
    // Reads GARFIELD integrated diffusion curve from ascii input "inputname" into a TGraph
    // with LineColor "color", which is returned

    gStyle->SetPalette(50);
    FILE* input=0;
    if(!HTool::openAscii(&input,inputname,"r"))
    {
	exit(1);
    }
    cout<<"HMdcGarReader::readXTdiffusion() , Reading XT-integrated diffusion data from GARFIELD file\n"<<inputname.Data()<<endl;

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
	scan_count=sscanf(line,"%*s%e%*s%*s%e",&f_x, &f_t);

	if(scan_count==2)
	{
	    distance [count]=f_x*10;
	    drifttime[count]=f_t*1000;
	    printf("%03i distance %7.3f mm integrated diffusion %7.3f ns\n",count,distance [count],drifttime[count]);
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
TGraph* HMdcGarReader::readMagboltzVdrift(TString inputname,Int_t color)
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
    ep.Set(100);
    Vdrift.Set(100);

    Int_t numberOfBins=0;
    Int_t version=0;
    Char_t line[400];
    Int_t scan_count;
    Float_t  f_ep ,f_vdrift;
    Char_t Tag []=" The gas tables follow";
    Char_t Tag1[]=" Dimension : F";
    Char_t Tag2[]=" Version   :";
    Char_t Tag3[]=" E fields";
    Char_t Tag4[]=" E-B angles";
    TString buffer=" ";

    Int_t count=0;
    while(1)
    {
	if(feof(input)) break;
	fgets(line, sizeof(line), input);
	if (!strncmp(line, Tag2 ,strlen(Tag2)))
        {   //  find version of format
            sscanf(line,"%*s%*s%i",&version);
            cout<<"version "<<version<<endl;
        }
	if (!strncmp(line, Tag1 ,strlen(Tag1)))
	{   //  find number of entries
	    sscanf(line,"%*s%*s%*s%2i%*s%*s",&numberOfBins);
	    ep    .Set(numberOfBins);
            Vdrift.Set(numberOfBins);
	    break;
	}
    }
    Int_t found=0;
    while(1&&version==4&&found==0)
    {   // read e-field in version 4
	if(feof(input)) break;
	fgets(line, sizeof(line), input);
	if (!strncmp(line, Tag3 ,strlen(Tag3)))
        {
            found=1;
            while(1)
            {
                fgets(line, sizeof(line), input);
                if (!strncmp(line, Tag4 ,strlen(Tag4)))
                {   // if next topic is entered break
                    break;
                }

                sscanf(line,"%e%e%e%e%e",&ep[count*5+0],&ep[count*5+1],&ep[count*5+2],&ep[count*5+3],&ep[count*5+4]);
                count++;
            }
        }
    }
    count=0;
    while(1)
    {
        if(feof(input)) break;
        fgets(line, sizeof(line), input);
        if (!strncmp(line, Tag ,strlen(Tag))) break;
    }

    
    while (count<numberOfBins)  {
	if (feof(input)) break;
	fgets(line, sizeof(line), input);
        if(version==3)
        {   // read old version 3
            scan_count=sscanf(line,"%e%e%*s",
                              &f_ep, &f_vdrift);
            ep    [count]=f_ep;
            Vdrift[count]=f_vdrift*10;
        }
        else if(version==4)
        {   // read new version 4
            scan_count=sscanf(line,"%e%*s",
                              &f_vdrift);
            Vdrift[count]=f_vdrift*10;
        }
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
TGraph* HMdcGarReader::readMagboltzGasPrint(TString inputname,TString option,Int_t color)
{
    // Reads MAGBOLTZ townsend/E (E/P:Log10) curve from ascii input "inputname" into a TGraph
    // with LineColor "color", which is returned
    TGraph* h=0;

    gStyle->SetPalette(50);
    FILE* input=0;
    if(!HTool::openAscii(&input,inputname,"r"))
    {
	exit(1);
    }
    cout<<"HMdcGarReader::readMagboltzGasPrint() , Reading Gas-Print-data from MAGBOLTZ file\n"<<inputname.Data()<<endl;

    TArrayF ep;
    TArrayF data;
    ep.Set(100);
    data.Set(100);

    Char_t line[400];
    Float_t f_ep, f_data;
    Char_t Tag []="    [V/cm]";
    TString buffer=" ";

    Int_t count=0;
    while(1)
    {
	if(feof(input)) break;
	fgets(line, sizeof(line), input);
	if (!strncmp(line, Tag ,strlen(Tag)))
        {
            //  find first line
	    break;
	}
    }

    while(1)
    {   // read
        if(feof(input)) break;
        fgets(line, sizeof(line), input);
        buffer=line;
        if(buffer.CompareTo("\n")==0)break;
        buffer.ReplaceAll("Not available","-1");
        if(option.CompareTo("vdrift")==0)
        {
            sscanf(buffer.Data(),"%f%f%*s%*s%*s%*s%*s%*s",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=10*f_data;
            printf("%03i ep %12.2f kV/cm Vdrift %7.3f ns/mu\n",count,ep[count],data[count]);
        }
        else if(option.CompareTo("ionmobility")==0)
        {
            sscanf(buffer.Data(),"%f%*s%f%*s%*s%*s%*s%*s",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=f_data;
            printf("%03i ep %12.2f kV/cm ion mobility %7.3f [cm2/V.musec]\n",count,ep[count],data[count]);
      
        }
        else if(option.CompareTo("diffusion_long")==0)
        {
            sscanf(buffer.Data(),"%f%*s%*s%f%*s%*s%*s%*s",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=f_data;
            printf("%03i ep %12.2f kV/cm diffusion long %7.3f [micron for 1 cm]\n",count,ep[count],data[count]);
        }
        else if(option.CompareTo("diffusion_trans")==0)
        {
            sscanf(buffer.Data(),"%f%*s%*s%*s%f%*s%*s%*s",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=f_data;
            printf("%03i ep %12.2f kV/cm diffusion trans %7.3f [micron for 1 cm]\n",count,ep[count],data[count]);
  
        }
        else if(option.CompareTo("townsend")==0)
        {
            sscanf(buffer.Data(),"%f%*s%*s%*s%*s%f%*s%*s",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=f_data;
            printf("%03i ep %12.2f kV/cm townsend %7.3f [1/cm]\n",count,ep[count],data[count]);
        }
        else if(option.CompareTo("attachment")==0)
        {
            sscanf(buffer.Data(),"%f%*s%*s%*s%*s%*s%f%*s",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=f_data;
            printf("%03i ep %12.2f kV/cm attachment %7.3f [1/cm]\n",count,ep[count],data[count]);
        }
        else if(option.CompareTo("lorentz")==0)
        {
            sscanf(buffer.Data(),"%f%*s%*s%*s%*s%*s%*s%f",&f_ep,&f_data);
            ep  [count]=0.001*f_ep;
            data[count]=f_data;
            printf("%03i ep %12.2f kV/cm lorentz angle %7.3f [degrees]\n",count,ep[count],data[count]);
        }
        count++;
    }

    Float_t* gep    =new Float_t[count];
    Float_t* gdata  =new Float_t[count];

    for(Int_t i=0;i<count;i++)
    {
        gep  [i]=ep  [i];
	gdata[i]=data[i];
    }

    h=new TGraph(count,gep,gdata);

    h->SetMarkerStyle(29);
    h->SetMarkerColor(4);
    h->SetMarkerSize(.8);
    h->SetLineColor(color);

    HTool::closeAscii(&input);
    return h;
}
