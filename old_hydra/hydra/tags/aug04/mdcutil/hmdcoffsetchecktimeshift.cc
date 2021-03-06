//*-- AUTHOR : J.Kempter
//*-- Modified :

///////////////////////////////////////////////////////////////////////////////
//
// HMdcOffsetCheckTimeShift
//
// Defines the offset  parameter for MDC calibration. Uses HMdcCalParRaw container
// as  input/output for calibration data
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include <math.h>
#include <stdlib.h>
#include <fstream> 
#include <iomanip> 
#include <iostream> 

#include <TF1.h>
#include <TH1.h>
#include <TStopwatch.h>
#include <TDirectory.h>

#include "heventheader.h"
#include "hmdcoffsetchecktimeshift.h"
#include "hmdcdef.h"
#include "hmdcraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdccal1.h"
#include "hmdclookupgeom.h"
#include "hmdccalpar.h"
#include "hmdccalparraw.h"
#include "hmdclookupraw.h"
#include "hstartcal.h"
#include "hmdctimecut.h"

ClassImp(HMdcOffsetCheckTimeShift)

//const Int_t HMdcOffsetCheckTimeShift::cells_Num[] ={184, 192, 172, 172, 192, 192};
const Int_t HMdcOffsetCheckTimeShift::nbin        = 2048;
const Int_t HMdcOffsetCheckTimeShift::nbinp1      = HMdcOffsetCheckTimeShift::nbin+1;
const Int_t HMdcOffsetCheckTimeShift::nbinm1      = HMdcOffsetCheckTimeShift::nbin-1;
const Int_t HMdcOffsetCheckTimeShift::nSubEvents  = (6*192)*3;


HMdcOffsetCheckTimeShift::HMdcOffsetCheckTimeShift(void)
{
    setDefault();
    rawCat = NULL;
    calStartCat=NULL;
    iter   = NULL;
    iter_start= NULL;
    isPulserFile = kFALSE;
    noStart=kFALSE;
    useTimeCut=kTRUE;
    hintegral = (MyFieldMdc*) new MyFieldMdc;
    hreverse  = (MyFieldMdc*) new MyFieldMdc;
    lookupgeom=0;
    timecut=0;
    filecounter=0;
    skipcount=0;
    stepsize=1000;
    step=0;
    numberofevents=1000;
}

HMdcOffsetCheckTimeShift::HMdcOffsetCheckTimeShift(Text_t* name,Text_t* title) : HReconstructor(name,title)
{
    setDefault();
    rawCat = NULL;
    calStartCat=NULL;
    iter   = NULL;
    iter_start= NULL;
    isPulserFile = kFALSE;
    noStart=kFALSE;
    useTimeCut=kTRUE;
    hintegral = (MyFieldMdc*) new MyFieldMdc;
    hreverse  = (MyFieldMdc*) new MyFieldMdc;
    lookupgeom=0;
    timecut=0;
    filecounter=0;
    skipcount=0;
    stepsize=1000;
    step=0;
    numberofevents=1000;
}

HMdcOffsetCheckTimeShift::~HMdcOffsetCheckTimeShift(void)
{
    // Destructor.
    if (iter)       delete iter;
    if (fNameAsciiOffset) delete fNameAsciiOffset;
    if (fNameRootOffset)  delete fNameRootOffset;
    if (hintegral)  delete[] hintegral;
    if (hreverse)   delete[] hreverse;
    iter = NULL;
    iter_start= NULL;

}

void HMdcOffsetCheckTimeShift::setDefault()
{
    // Sets the the default values for the fits.
    setNoise(100, 50);
    setThreshold(0.15, 0.5);
    setRangeGauss(50);
}

void HMdcOffsetCheckTimeShift::setOutputAscii(Char_t *c)
{
    // Sets ascii output of HMdcOffsetCheckTimeShift for debugging output.
    if (fNameAsciiOffset) delete fNameAsciiOffset;
    fNameAsciiOffset = new Char_t[strlen(c)+1];
    strcpy(fNameAsciiOffset, c);
}

void HMdcOffsetCheckTimeShift::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcOffsetCheckTimeShift where all created histograms were written.
    //
    if (fNameRootOffset) delete fNameRootOffset;
    fNameRootOffset = new Char_t[strlen(c)+1];
    strcpy(fNameRootOffset, c);
}

Bool_t HMdcOffsetCheckTimeShift::init(void)
{
    //  Inits HMdcOffsetCheckTimeShift and the needed HMdcCalParRaw, if this container does not exists
    if(!noStart){
	calStartCat=gHades->getCurrentEvent()->getCategory(catStartCal);       //????
	if (!calStartCat) {
	    calStartCat=gHades->getSetup()->getDetector("Start")->buildCategory(catStartCal);
	    if (!calStartCat) return kFALSE;
	    else gHades->getCurrentEvent()->addCategory(catStartCal,calStartCat,"Start");
	}
	iter_start=(HIterator *)calStartCat->MakeIterator("native");
    }

    calparraw=(HMdcCalParRaw*)gHades->getRuntimeDb()->getContainer("MdcCalParRaw");
    timecut=(HMdcTimeCut*)gHades->getRuntimeDb()->getContainer("MdcTimeCut");
    lookupgeom=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");

    rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
    if (!rawCat) {
        rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
        if (!rawCat) return kFALSE;
        else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
    }
    // creates an iterator which loops over all fired cells
    iter=(HIterator *)rawCat->MakeIterator();


    fActive=kTRUE;
    eventcounter=0;
    step=0;
    return kTRUE;
}
Bool_t HMdcOffsetCheckTimeShift::reinit(void)
{
    //  Inits HMdcOffsetCheckTimeShift and the needed HMdcCalParRaw, if this container does not exists
    filecounter++;
    cout<<"file "<<filecounter<<endl;

    strcpy(filenames[filecounter-1],
	   strrchr((Text_t*)gHades->getRuntimeDb()->getCurrentFileName(),47)+1);
    cout<<filenames[filecounter-1]<<endl;
    skipcount=0;
    step=0;
    fActive=kTRUE;
    return kTRUE;
}

void HMdcOffsetCheckTimeShift::createHist(TFile* file,Int_t s, Int_t m, Int_t step,Int_t f)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are created
    // in a subdirectory structure.
    static Char_t title[50], tmp[30];

    sprintf(title,"%s%i%s%i%s%i%s%02i","Sector ",s,"  Module ",m,"  Step ",step," File ",f);
    file->TDirectory::Cd("hinv");
    sprintf(tmp, "%s%i%s%i%s%02i%s%02i%s", "hinv[",s,"][",m,"][",step,"][",f,"]");
    hinv = new MyHist(tmp, title, nbin,-nbin+0.5,0.5); //inverted hist.
    file->TDirectory::Cd("../hint");
    sprintf(tmp, "%s%i%s%i%s%02i%s%02i%s", "hint[",s,"][",m,"][",step,"][",f,"]");
    hint = new MyHist(tmp, title, nbin,-nbin+0.5,0.5); // integrated hist.
    file->TDirectory::Cd("..");
}

void HMdcOffsetCheckTimeShift::deleteHist()
{
    // Created histograms are deleted
    delete hinv;
    delete hint;
}

void HMdcOffsetCheckTimeShift::fillHist(Int_t s, Int_t m, Int_t step,Int_t f)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are filled
    MyInt *hi = &((*hintegral)[s][m][step][f][0]);
    MyInt *hr = &((*hreverse) [s][m][step][f][0]);
    *hi = *hr;
    hint->SetBinContent(1, *hi++);
    hinv->SetBinContent(1, *hr++);
    for(Int_t k=2; k<nbinp1; k++) {
        *hi=*(hi-1) + *hr;
        hint->SetBinContent(k, *hi++);
        hinv->SetBinContent(k, *hr++);
    }
}

Int_t HMdcOffsetCheckTimeShift::fitHist(Int_t s, Int_t m, Int_t step,Int_t file)
{
    // The offset is calculated from two linear fits. Some pictures of the fitted histograms
    // can be found on Begin_Html <a href="http://www-hades.gsi.de/docs/mdc/mdcana/#calibration">MDC calibration page</a>End_Html .
    // The first linear fit (red) is done to find the rising edge of the integrated spectra.
    // The second linear fit (blue) is done to substract the noise.
    // The ranges for both fits can be set through the functions setNoise()(x-range) for the
    // second fit (default values: 100,50) and setThreshold()(y-range) for the first fit
    // (default values :0.15,0.50). The y-range is calculated by the percentage of the maximum
    // height of the spectra.
    // The offset ist calculated from the intersection point of both linear fits.
    // The sigma of the offset is calculated from the sigmas of the two linear fits.
    //
    // If the analyzed file is a pulser file for external calibration a gaussian fit is done.
    // To set this fit the function setPulserFile() has to be used in the macro. The range of
    // the fit is set through the function setRangeGauss()(default value: 50) around the calculated
    // offset.The mean and sigma of the fit is written to the ascii debugging output.
    fitGaussMean  = 0;
    fitGaussSigma = 0;
    crosspointX=0;
    yequalzero=0;
    totalsigma=0;
    fitpar0     =0;
    fitpar0error=0;
    fitpar1     =0;
    fitpar1error=0;
    fitparNoise0     =0;
    fitparNoise1     =0;
    fitparNoise0error=0;
    fitparNoise1error=0;
   


    MyInt *hI = &((*hintegral)[s][m][step][file][0]);
    Int_t   xmaxfitbin=nbinm1;

    Float_t yminfit=minfitthreshold*hI[nbinm1-1];     // setting y thresholds for fitting
    Float_t ymaxfit=maxfitthreshold*hI[nbinm1-1];     // relative to maximum y-value

    while (hI[--xmaxfitbin] > ymaxfit && xmaxfitbin); // find to ymaxfit corresponding bin
    if (!xmaxfitbin) return 1;

    Int_t xminfitbin=xmaxfitbin;
    while (hI[--xminfitbin] > yminfit && xminfitbin); // find to yminfit corresponding bin

    if (!xminfitbin) return 2;

    TF1 *f = new TF1("f1","pol1",hint->GetBinCenter(xminfitbin),hint->GetBinCenter(xmaxfitbin)); // call fit function poynom 1.order in x-range
    hint->Fit("f1","RNQ");
    fitpar0     =0;
    fitpar0error=0;
    fitpar1     =0;
    fitpar1error=0;
    fitpar0     =f->GetParameter(0);  // get fitparameters
    fitpar0error=f->GetParError (0);
    fitpar1     =f->GetParameter(1);  //(f1->GetParameter(1)<0.000001)?-1:(f1->GetParameter(1));
    fitpar1error=f->GetParError (1);  // y=fitpar[1]* x +  fitpar[0]
    delete f;

    if (fitpar1==0) return 3;

    yequalzero=0;
    yequalzero =-fitpar0/fitpar1;

    if (-yequalzero < (offsetfitNoise+widthfitNoise) || -yequalzero > nbin) return 4;

    f = new TF1("f2","pol1",
                yequalzero-(offsetfitNoise+widthfitNoise),
                yequalzero-offsetfitNoise);
    hint->Fit("f2","RNQ");
    fitparNoise0     =0;
    fitparNoise1     =0;
    fitparNoise0error=0;
    fitparNoise1error=0;
    fitparNoise0     =f->GetParameter(0); // get fitparameters
    fitparNoise1     =f->GetParameter(1);
    fitparNoise0error=f->GetParError (0); // get fitparameters
    fitparNoise1error=f->GetParError (1);
    delete f;

    if (fitpar1==fitparNoise1) return 5;

    crosspointX=0;
    crosspointX=-(fitparNoise0-fitpar0)/(fitpar1-fitparNoise1);

    if (crosspointX<0 || crosspointX>nbinm1 || isNaN(crosspointX)) return 6;


    // calculating total sigma
    totalsigma= (Float_t)sqrt((pow(fitparNoise0error,2)/pow((fitparNoise1-fitpar1),2))
                    +(pow(fitpar0error,2)/pow((fitparNoise1-fitpar1),2))
                    +(pow((fitparNoise0-fitpar0),2)*pow(fitpar1error,2)/pow((fitparNoise1-fitpar1),4))
                    +(pow((fitparNoise0-fitpar0),2)*pow(fitparNoise1error,2)/pow((fitparNoise1-fitpar1),4))
                   );
  
    
    if (!isPulserFile) return 0;


    f = new TF1("f3","gaus", -crosspointX-rangeGauss, -crosspointX+rangeGauss);
    hinv->Fit("f3","RNQ");
    fitGaussMean =f->GetParameter(1);  //mean
    fitGaussSigma=f->GetParameter(2);  //sigma
    delete f;

    // FIXME: Error handling for gauss fit ???

    return 0;
}

void HMdcOffsetCheckTimeShift::writeAscii(ofstream &fout, Int_t s, Int_t m, Int_t step,Int_t f)
{
    // The adresses of the channel(sector,module,mbo,tdc),offset, two fitparameters for the
    // two linear fits, the sigma of the offset ,offset and the integral(number of entries for this channel)
    // are written to the ascii debugging output.In the case of an pulser file (external calibration)
    // the mean and the sigma of the gaussian fit are also written to the output.
    if (isPulserFile)
        fout
	    << setw(3)  << s                 << " "
            << setw(4)  << m                 << " "
            << setw(4)  << step              << " "
            << setw(5)  << f                 << " "
	    << setw(10) << -yequalzero       << " "
	    << setw(10) << crosspointX       << " "
            << setw(15) << -fitGaussMean     << " "
            << setw(15) << fitGaussSigma     << " "
	    << setw(25)  <<filenames[f]      << " "
	    << endl;
    else
        fout
	    << setw(3)  << s                  << " "
            << setw(4)  << m                  << " "
            << setw(4)  << step               << " "
            << setw(5)  << f                  << " "
	    << setw(10) << -yequalzero        << " "
	    << setw(10) << crosspointX        << " "
            << setw(10) << totalsigma         << " "
	    << setw(25)  <<filenames[f]       << " "
	  
	    << endl;
}

void HMdcOffsetCheckTimeShift::writeHist(TFile* file)
{
    // All created histograms are written to a rootfile.The file is structured for sector,module,mbo.
    
    file->TDirectory::Cd("hinv");
    hinv->Write();
    file->TDirectory::Cd("../hint");
    hint->Write();
    file->TDirectory::Cd("..");
}

ofstream *HMdcOffsetCheckTimeShift::openAsciiFile()
{
    // Opens the ascii debugging output and writes the headerline.
    ofstream *fout=NULL;
    if (fNameAsciiOffset)
        fout = new ofstream(fNameAsciiOffset, ios::out);
    if (fout)             // changed *fout
        if (isPulserFile)
        {
            *fout //<< "Layer     Cell      crosspoint" << endl;
	        << setw(3)  << "s"              << " "
                << setw(4)  << "mo"             << " "
                << setw(4)  << "step"           << " "
                << setw(5)  << "file"           << " "
		<< setw(10) << "yequalzero"     << " "
		<< setw(10) << "Offset"         << " "
                << setw(15) << "GaussMean"      << " "
                << setw(15) << "GaussSigma"     << " "
		<< setw(25) << "filename"       << endl;
	      
	    *fout << setw(14) << "[MdcCalParRaw]" <<endl;
        }
        else
        {
            *fout //<< "Layer     Cell      crosspoint" << endl;
	        << setw(3)  << "s"              << " "
                << setw(4)  << "mo"             << " "
                << setw(4)  << "step"           << " "
                << setw(5)  << "file"           << " "
		<< setw(10) << "yequalzero"     << " "
		<< setw(10) << "Offset"         << " "
                << setw(10) << "Error"          << " "
		<< setw(25) << "filename"       <<endl;
	     
	    *fout << setw(14) << "[MdcCalParRaw]" <<endl;
        }
    return fout;
}

Bool_t HMdcOffsetCheckTimeShift::finalize(void)
{
    // This function is called after the execute function is finished. At this point
    // the arrays for the drift-time are filled. Froms this arrays the histograms for
    // the drift-time and the integrated drift-time are filled. The fits for the calcutation
    // of the offsets are done and the offsets and the sigma of the offsets are
    // calculated .All histograms are written to a rootfile output and the information
    // of the fits to an ascii debugging output. The offset and the sigma of the offset
    // are filled into the container (HMdcCalParRaw) for the calibration parameters.
    // To characterize the method of calculation of the offsets the function setOffsetMethod()
    // of HMdcCalParRaw is used. As default value for the method 2 is used for  automatic
    // calibration. If no calibration is done this value is set to 0.
    cout << "From HMdcOffsetCheckTimeShift::finalize" << endl;

    // Start Stop Watch
    TStopwatch timer;
    timer.Start();

    // open Root file for writing
    TFile *file = NULL;
    if (fNameRootOffset)
	file = new TFile(fNameRootOffset,"RECREATE");
    // open ascii file for writing
    ofstream *fout = openAsciiFile();
 
    Int_t err;
    Float_t offsets[6][4][20][50];
    Float_t offsetErr[6][4][20][50];
    HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");

    if (!mdcDet)
        cout << "Error in HMdcOffsetCheckTimeShift::finalize: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
    else
	for(Int_t s=0; s<6; s++) {  //loop over sectors
            cout<<"Sector "<<s<<endl;
	    TDirectory *dirSec=NULL;
	    if (file) dirSec=Mkdir(file, "sector", s);
	    for(Int_t mo=0; mo<4; mo++) {  //loop over modules
		cout<<"Module "<<mo<<endl;
		if (!mdcDet->getModule(s, mo)) continue;
                TDirectory *dirMod=NULL;
		if (dirSec) dirMod=Mkdir(dirSec, "module", mo);

		if (dirMod)
		{
		    dirMod->mkdir("hinv");
		    dirMod->mkdir("hint");
		}
		    for(Int_t f=0;f<50;f++){     // loop over files
                       for(Int_t step=0;step<20;step++){ // loop over steps per file
		
			createHist(file, s, mo,step,f);
			fillHist  (s, mo,step,f);
			if ((err=fitHist(s, mo,step,f))) crosspointX = fitpar1 = -err;
			offsets[s][mo][step][f]=crosspointX;
			offsetErr[s][mo][step][f]=totalsigma;

			if (file)  writeHist (file);
			if (*fout) writeAscii(*fout, s, mo,step,f);
			deleteHist();
		    }
                   cout<<"."<<flush;
		}

	    cout<<" "<<endl;
	    if (dirMod) dirMod->TDirectory::Cd("..");
	}
    cout<<" "<<endl;
    if (dirSec) dirSec->TDirectory::Cd("..");
}
cout << endl;

// write offsets to calparraw

    // stop watch
 cout << "Time of cell loop (offset calculation time):" << endl;
 timer.Print();
 timer.Stop();

 // close root file
 file->Save();
 file->Close();
 delete file;

 // close ascii file
 fout->close();
 delete fout;

 return kTRUE;
}

TDirectory *HMdcOffsetCheckTimeShift::Mkdir(TDirectory *dirOld, Char_t *c, Int_t i, Int_t p)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
{
    // Function to create subdirectories
    static Char_t sDir[255];// = new Char_t[strlen(c)+3];
    static Char_t sFmt[10];
    sprintf(sFmt, "%%s %%0%1ii", p);
    sprintf(sDir, sFmt, c, i);
    TDirectory *dirNew = dirOld->mkdir(sDir);
    dirOld->cd(sDir);
    //  free (sDir);
    return dirNew;
}

Float_t HMdcOffsetCheckTimeShift::getstarttime(){
  Int_t i=0;
  HStartCal* startcal=0;
  iter_start->Reset();
  Float_t starttime=0;
  while ((startcal=(HStartCal *)iter_start->Next())!=0) {
      switch (startcal->getStrip()){
      case 0:  starttime=(startcal->getTime());
      break;
      case 1:  starttime=(startcal->getTime());
      break;
      case 2:  starttime=(startcal->getTime());
      break;
      case 3:  starttime=(startcal->getTime());
      break;
      case 4:  starttime=(startcal->getTime());
      break;
      case 5:  starttime=(startcal->getTime());
      break;
      case 6:  starttime=(startcal->getTime());
      break;
      case 7:  starttime=(startcal->getTime());
      break;


      }
      i++;

  }
  if (i!=1)cout<<"Multiplicity in Start > 1"<<endl;
  return starttime;
}

Int_t HMdcOffsetCheckTimeShift::execute()
{
    // Raw data of Time1  multiplied by the slope of the channel taken from the
    // container of the calibration parameters are filled into the array for the
    // drift-time . This array contains 2048 (corresponding to the resulution of
    // the tdc chip) entries for each tdc channel.
    static HMdcRaw *raw=NULL;
    Int_t s, mo, mb, t;
    Float_t testTime1=0;
    Float_t testTime2=0;
    skipcount++;              // starts with 0 for each file
    step=(Int_t)(skipcount/stepsize);

    if(eventcounter%10000==0){
	cout<<eventcounter<<" events analyzed"<<endl;
    }
    eventcounter++; // counts number of calculated events of all files


    if(step>19)return 0;
    if(skipcount>(step*stepsize+numberofevents))return 0;
    if(skipcount%1000==0)cout<<skipcount<<endl;

    iter->Reset();
    if(!noStart)
    {
	Float_t starttime=getstarttime();
	while ((raw=(HMdcRaw*)iter->Next()))
	{
	    s=mo=mb=t=-99;
	    raw->getAddress(s, mo, mb, t);
	    if(((*calparraw)[s][mo][mb][t].getSlope())!=0)
	    {
		testTime1=raw->getTime(1)-(starttime/(*calparraw)[s][mo][mb][t].getSlope());
		testTime2=raw->getTime(2)-(starttime/(*calparraw)[s][mo][mb][t].getSlope());
	    }
	    else
	    {
		testTime1=raw->getTime(1);
		testTime2=raw->getTime(2);
	    }
	    if(useTimeCut)
	    {
		if(timecut->cutTime1(s,mo,testTime1)&&
		   timecut->cutTime2(s,mo,testTime2)&&
		   timecut->cutTimesDif(s,mo,testTime2,testTime1))
		{
		    (*hreverse)[s][mo][step][filecounter-1]
			[
			 (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope())+starttime)
			]++;
		}
	    }
	    else
	    {
		(*hreverse)[s][mo][step][filecounter-1]
		    [
		     (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope())+starttime)
		    ]++;
	    }
	}
    }
    else
    {
	while ((raw=(HMdcRaw*)iter->Next()))
	{
	    s=mo=mb=t=-99;
	    raw->getAddress(s, mo, mb, t);
	    testTime1=raw->getTime(1);
	    testTime2=raw->getTime(2);
	    if(useTimeCut)
	    {
		if(timecut->cutTime1(s,mo,testTime1)&&
		   timecut->cutTime2(s,mo,testTime2)&&
		   timecut->cutTimesDif(s,mo,testTime2,testTime1))
		{

		    // we have to use Float_ts here
		    (*hreverse)[s][mo][step][filecounter-1]
			[
			 (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope()))
			]++;
		}
	    }
	    else
	    {
		(*hreverse)[s][mo][step][filecounter-1]
		    [
		     (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope()))
		    ]++;
	    }
	}
    }

        return 0;
}
