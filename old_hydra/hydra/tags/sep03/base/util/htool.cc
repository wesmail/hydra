//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HTool
// Tool Class
////////////////////////////////////////////////////////////////////////////
using namespace std;
#include <stdlib.h>
#include <iostream> 
#include <iomanip>
#include "htool.h"
#include "TDirectory.h"
#include "TFile.h"
#include "TString.h"
#include "TObjString.h"
#include "TList.h"
#include "TString.h"
#include "TROOT.h"
#include "TObject.h"

ClassImp(HTool)

HTool::HTool(const char* name,const char* title)
    : TNamed(name,title)
{
    // constructor for HTool
    initVariables();
}
HTool::~HTool()
{
  // destructor of HTool
}
void HTool::initVariables()
{
    // inits all variables
}
Bool_t HTool::open(TFile** file,TString fName,TString option)
{
    // Opens root file "fName" to the pointer "file" with option
    // New,Read,Recreate and Update. Checks if file exists or is
    // already opened. If everything works fine kTRUE is returned.
    if(*file)
    {   // checking if file is opened
	if((*file)->IsOpen()){
	    cout<<"Error(HTool::open() , SPECIFIED INPUT FILE IS ALREADY OPEN!"<<endl;
            return kFALSE;
	}else{
	    *file=0;
	}
    }

    option.ToLower();
    if(option.CompareTo("read")==0){   // read from file
	*file = (TFile*)gROOT->GetListOfFiles()->FindObject(fName.Data());
	if (!*file){
	    *file= new TFile(fName.Data(),"READ");
	    cout<<"HTool::open()     : Reading from "<<fName.Data()<<endl;
            return kTRUE;
	}else{
	    cout<<"Error(HTool::open() , SPECIFIED INPUT FILE DOES NOT EXIST!"<<endl;;
	    return kFALSE;
	}
    }
    else if(option.CompareTo("update")==0){   // read from file
	*file = (TFile*)gROOT->GetListOfFiles()->FindObject(fName.Data());
	if (!*file){
	    *file= new TFile(fName.Data(),"UPDATE");
	    cout<<"HTool::open()     : Updating "<<fName.Data()<<endl;
            return kTRUE;
	}else{
	    cout<<"Error(HTool::open() , SPECIFIED INPUT FILE DOES NOT EXIST!"<<endl;
	    return kFALSE;
	}
    }
    else if(option.CompareTo("recreate")==0){
	*file= new TFile(fName.Data(),"RECREATE");
        return kTRUE;
    }
    else if(option.CompareTo("create")==0){
	*file= new TFile(fName.Data(),"CREATE");
        return kTRUE;
    }
    else if(option.CompareTo("new")==0){
	*file= new TFile(fName.Data(),"NEW");
        return kTRUE;
    }
    else {
	cout<<"Error(HTool::open() , UNKOWN OPTION!"<<endl;
	return kFALSE;
    }
}
Bool_t HTool::close(TFile** file)
{
    // Close the root file of the pointer file if
    // existing and open and sets the file pointer to 0.
    // Returns kTRUE if OK.
    TString opt;
    if(*file){
	if((*file)->IsOpen()){
            opt=(*file)->GetOption();
	    if(opt.CompareTo("READ")!=0)(*file)->Save();
	    (*file)->Close();
            *file=0;
	    return kTRUE;
	}else{
	    cout<<"Warning(HTool::close() , SPECIFIED FILE IS NOT OPEN!"<<endl;
	    return kTRUE;
	}
    }else{
	cout<<"Error(HTool::close() , SPECIFIED FILE POINTER IS ZERO!"<<endl;
	return kFALSE;
    }
}
Bool_t HTool::openAscii(FILE** file,TString fName,TString option)
{
    // Opens ascii file "fName" to the pointer file with the option
    // "r" and "w". Returns kTRUE if OK.
    if(*file)
    {   // checking if file is opened
	cout<<"Error(HTool::openAscii() , SPECIFIED INPUT FILE IS MAYBE ALREADY OPEN!"<<endl;
	return kFALSE;
    }

    option.ToLower();
    if(option.CompareTo("r")==0){   // read from file
	*file = (FILE*)gROOT->GetListOfFiles()->FindObject(fName.Data());
	if (!*file){
	    *file= fopen(fName.Data(),"r");
	    cout<<"HTool::openAscii()     : Reading from "<<fName.Data()<<endl;
	    return kTRUE;
	}else{
	    cout<<"Error(HTool::openAscii() , SPECIFIED INPUT FILE DOES NOT EXIST!"<<endl;;
	    return kFALSE;
	}
    }
    else if(option.CompareTo("w")==0){
	*file=fopen(fName.Data(),"w");
	cout<<"HTool::openAscii()     : Writing to "<<fName.Data()<<endl;
	return kTRUE;
    }
    else {
	cout<<"Error(HTool::openAscii() , UNKOWN OPTION!"<<endl;
	return kFALSE;
    }
}
Bool_t HTool::closeAscii(FILE** file)
{
    // Close ascii file of the pointer file if file!=0 and
    // sets the file pointr to 0 afterwards. Returns kTRUE
    // if OK.
    if(*file){
	fclose(*file);
        *file=0;
	return kTRUE;
    }else{
	cout<<"Error(HTool::closeAsii() , SPECIFIED FILE POINTER IS ZERO!"<<endl;
	return kFALSE;
    }
}
TDirectory* HTool::Mkdir(TDirectory *dirOld, const Char_t *c, Int_t i, Int_t p)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
{
    // Function to create subdirectories, where dirold is the pointer of the old Directory,
    // "c" the name of the new Directory an "i" the number (c + i) of the directory."p" gives
    // the formating number of digits filled with 0. If "i"==-99 the numbering part is skipped.
    // Checks if the Directrory exists , if so changes into otherwise create it new. The
    // pointer to the new Directory is returned.

    static Char_t sDir[255];// = new Char_t[strlen(c)+3];
    static Char_t sFmt[10];
    if(i!=-99)
    {
	sprintf(sFmt, "%%s %%0%1ii", p);
	sprintf(sDir, sFmt, c, i);
    }
    else
    {
	sprintf(sDir,"%s",c);
    }
    TDirectory *dirNew=0;
    if(!dirOld->FindKey(sDir))
    {
	dirNew = dirOld->mkdir(sDir);
    }
    else
    {
	dirNew=(TDirectory*)dirOld->Get(sDir);
    }
    dirOld->cd(sDir);
    return dirNew;
}

TDirectory* HTool::changeToDir(TString p)
{
    // Changes into the given path. If the Directory is not existing
    // it will be created. The pointer to the new directory will be returned.
    TDirectory *dirNew=0;
    TString s1=p;
    Ssiz_t len=s1.Length();
    if(len!=0)
    {
	char* mystring=(char*)s1.Data();

	Char_t* buffer;
	TList myarguments;
	TObjString *stemp;
	TString argument;
	Int_t count=0;

	while(1) // find all token in option string and put them to a list
	{
	    if(count==0)
	    {
		buffer=strtok(mystring,"/");
		stemp=new TObjString(buffer);
		myarguments.Add(stemp);
	    }

	    if(!(buffer=strtok(NULL,"/")))break;
	    stemp=new TObjString(buffer);
	    myarguments.Add(stemp);

	    count++;
	}
	TIterator* myiter=myarguments.MakeIterator();

	// iterate over the list of arguments
	while ((stemp=(TObjString*)myiter->Next())!= 0)
	{
	    argument=stemp->GetString();
            
	    dirNew=HTool::Mkdir(gDirectory,argument.Data(),-99);
	}
    }
    return dirNew;
}
Bool_t HTool::checkDir(TString p,TFile* input)
{
    // Checks if a given path "p" in file "input" exists.
    // The actual directory will not be changed. Returns
    // kTRUE if OK.
    TDirectory* dirSave=gDirectory;

    TDirectory *dirNew=input;
    TString s1=p;
    Ssiz_t len=s1.Length();
    if(len!=0)
    {
	char* mystring=(char*)s1.Data();

	Char_t* buffer;
	TList myarguments;
	TObjString *stemp;
	TString argument;
	Int_t count=0;

	while(1) // find all token in option string and put them to a list
	{
	    if(count==0)
	    {
		buffer=strtok(mystring,"/");
		stemp=new TObjString(buffer);
		myarguments.Add(stemp);
	    }

	    if(!(buffer=strtok(NULL,"/")))break;
	    stemp=new TObjString(buffer);
	    myarguments.Add(stemp);

	    count++;
	}
	TIterator* myiter=myarguments.MakeIterator();

	// iterate over the list of arguments
	while ((stemp=(TObjString*)myiter->Next())!= 0)
	{
	    argument=stemp->GetString();
	    if(dirNew->FindKey(argument.Data()))
	    {
		dirNew->cd(argument.Data());
                dirNew=gDirectory;
	    }
	    else
	    {
		dirSave->cd();
		return kFALSE;
	    }
	}
    }
    dirSave->cd();
    return kTRUE;
}
void HTool::scanOracle(TString inputname,TString outputname)
{
    // scans oracle runs table source code to extract
    // file names, run ids and number of events

    FILE* input =0;
    FILE* output=0;

    if(!HTool::openAscii(&input,inputname  ,"r"))
    {
	exit(1);
    }
    if(!HTool::openAscii(&output,outputname,"w"))
    {
	exit(1);
    }

    Char_t line[4000];

    TString buffer="";
    TString filename;
    TString fileRunId;
    TString fileNEvents;
    TString newLine;

    Int_t count=0;
    Int_t sumEvts=0;
    Int_t Evts=0;

    while(1)
    {
	if(feof(input)) break;
	fgets(line, sizeof(line), input);
        buffer=line;
	if (buffer.Contains("<TD align=\"center\">100"))
	{
            buffer.ReplaceAll("<TD align=\"center\">","");
            buffer.ReplaceAll("</TD>","");
            buffer.ReplaceAll("\n","");
	    fileRunId=buffer;

	    fgets(line, sizeof(line), input);
            fgets(line, sizeof(line), input);
            fgets(line, sizeof(line), input);
            fgets(line, sizeof(line), input);
            buffer=line;
            buffer.ReplaceAll("<TD align=\"center\"><A href=\"hades_www.show_log_run_info.show?search_string=","");
            buffer.Remove(buffer.First("&"));
            buffer.ReplaceAll("\n","");
	    filename=buffer;

	    fgets(line, sizeof(line), input);
	    buffer=line;
            buffer.ReplaceAll("<TD align=\"center\">","");
	    buffer.ReplaceAll("</TD>","");
            buffer.ReplaceAll("\n","");

	    fileNEvents=buffer;
	    newLine= filename + " " + fileRunId + " " + fileNEvents;
            sscanf(newLine.Data(),"%*s%*s%i",&Evts);
	    sumEvts=sumEvts+Evts;
            count++;
            fprintf(output,"%s%s",newLine.Data(),"\n");
	}
    }
    cout<<count<<" Files with "<<sumEvts<<" Events"<<endl;
    fprintf(output,"%i Files with %i Events\n",count,sumEvts);

    HTool::closeAscii(&input);
    HTool::closeAscii(&output);

}

TObjArray* HTool::slices(TH2F* h2,TF1* f,TString axis,Int_t firstbin,Int_t lastbin,Int_t cut,TString opt,Int_t markerstyle,Int_t markercolor,Float_t markersize)
{
    TObjArray* array=0;
    if(h2==0)
    {
        cout<<"Warning: HTool::slices : ZERO pointer retrieved for histogram!"<<endl;
        return array;
    }
    axis.ToLower();
    if     (axis.CompareTo("x")==0) h2->FitSlicesX(f,firstbin,lastbin,cut,opt);
    else if(axis.CompareTo("y")==0) h2->FitSlicesY(f,firstbin,lastbin,cut,opt);
    else
    {
        cout<<"Warning: HTool::slices : Unknown argument for axis : "<<axis.Data()<<"!"<<endl;
        return array;
    }
    Int_t owner=0;
    if(f==0) {f=new TF1("fit","gaus");owner=1;}

    array=new TObjArray(f->GetNpar()+1);
    TH1D* h[4];
    TString name="";
    name=h2->GetName();
    name+="_0";
    h[0]= (TH1D*)gDirectory->Get(name.Data());
    array->AddAt(h[0],0);
    name="";
    name=h2->GetName();
    name+="_1";
    h[1]= (TH1D*)gDirectory->Get(name.Data());
    array->AddAt(h[1],1);
    name="";
    name=h2->GetName();
    name+="_2";
    h[2]= (TH1D*)gDirectory->Get(name.Data());
    array->AddAt(h[2],2);
    name="";
    name=h2->GetName();
    name+="_chi2";
    h[3]= (TH1D*)gDirectory->Get(name.Data());
    array->AddAt(h[3],3);

    for(Int_t i=0;i<array->LastIndex()+1;i++)
    {
        h[i]->SetLineColor(markercolor);
        h[i]->SetMarkerColor(markercolor);
        h[i]->SetMarkerStyle(markerstyle);
        h[i]->SetMarkerSize(markersize);

    }
    if(owner==1) delete f;
    return array;
}

TObjArray* HTool::projections(TH2F* h2,TString axis,Int_t firstbin,Int_t lastbin,Int_t nsteps,TString opt,Int_t markerstyle,Int_t markercolor,Float_t markersize)
{
    TObjArray* array=0;
    if(h2==0)
    {
        cout<<"Warning: HTool::projections : ZERO pointer retrieved for histogram!"<<endl;
        return array;
    }
    axis.ToLower();

    Int_t stepsize;
    Int_t bin1=firstbin,bin2=lastbin;


    Char_t name[300];
    //---------------------defining range----------------------
    if(firstbin==0||lastbin==-1)
    {   // full range
        bin1=1;
        if(axis.CompareTo("x")==0)bin2=h2->GetNbinsY();
        if(axis.CompareTo("y")==0)bin2=h2->GetNbinsX();
    }else
    {   // part of range
        bin1 = firstbin;
        bin2 = lastbin;
    }
    array=new TObjArray(0);
    //---------------------defining stepsize--------------------
    if(nsteps==-99)stepsize = 1;
    else           stepsize = nsteps;

    //---------------------projecting---------------------------
    for(Int_t i=bin1;i<bin2+1;i+=stepsize)
    {
        if  (axis.CompareTo("x")==0){
            sprintf(name,"%s%s%i",h2->GetName(),"_px_",i);
            array->AddLast(h2->ProjectionX(name,i,i+stepsize,opt)); }
        else if(axis.CompareTo("y")==0){
            sprintf(name,"%s%s%i",h2->GetName(),"_py_",i);
            array->AddLast(h2->ProjectionY(name,i,i+stepsize,opt)); }
       else{
            cout<<"Warning: HTool::slices : Unknown argument for axis : "<<axis.Data()<<"!"<<endl;
            return array;
        }
    }

    //---------------------setting attributes-------------------
    array->Expand(array->GetLast()+1);
    cout<<"number of hists "<<array->LastIndex()+1<<endl;
    for(Int_t i=0;i<array->LastIndex()+1;i++)
    {
        ((TH1D*)array->At(i))->SetLineColor(markercolor);
        ((TH1D*)array->At(i))->SetMarkerColor(markercolor);
        ((TH1D*)array->At(i))->SetMarkerStyle(markerstyle);
        ((TH1D*)array->At(i))->SetMarkerSize(markersize);
    }
    return array;
}
void HTool::cleanHist(TH1* h,Int_t threshold,Int_t val)
{
    if(h!=0)
    {
        TString classname=h->ClassName();
        Int_t type=0;

        if(classname.Contains("TH1")==1)type=1;
        if(classname.Contains("TH2")==1)type=2;

        Int_t binsX=0,binsY=0;

        binsX=h->GetNbinsX();
        if(type==2)binsY=h->GetNbinsY();

        Double_t bincontent;

        if(type==1)
        {
            for(Int_t x=0;x<binsX;x++)
            {

                bincontent= h->GetBinContent(x+1);
                if(bincontent<threshold)h->SetBinContent(x+1,val);
            }
        }
        if(type==2)
        {
            for(Int_t x=0;x<binsX;x++)
            {
                for(Int_t y=0;y<binsY;y++){
                    bincontent= h->GetBinContent(x+1,y+1);
                    if(bincontent<threshold)h->SetBinContent(x+1,y+1,val);
                }
            }
        }
    }
    else
    {
        cout<<"Warning: HTool::cleanHist : ZERO pointer for hist recieved!"<<endl;
    }
}
