//*-- AUTHOR : J.Kempter
//*-- Modified :

///////////////////////////////////////////////////////////////////////////////
//
// HMdcOffset
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
#include <TH2.h>
#include <TStopwatch.h>
#include <TDirectory.h>
#include <TNtuple.h>

#include "heventheader.h"
#include "hmdcoffset.h"
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

ClassImp(HMdcOffset)

const Int_t HMdcOffset::nbin        = 2048;
const Int_t HMdcOffset::nbinp1      = HMdcOffset::nbin+1;
const Int_t HMdcOffset::nbinm1      = HMdcOffset::nbin-1;
const Int_t HMdcOffset::nSubEvents  = (6*192)*3;


HMdcOffset::HMdcOffset(void)
{
    setDefault();
    initVariables();
    //initMemory();
}

HMdcOffset::HMdcOffset(Text_t* name,Text_t* title) : HReconstructor(name,title)
{
    setDefault();
    initVariables();
    //initMemory();
}
HMdcOffset::~HMdcOffset(void)
{
    // Destructor.
    if (iter)       delete iter;
    if (fNameAsciiOffset) delete fNameAsciiOffset;
    if (fNameRootOffset)  delete fNameRootOffset;
    if (hreverse)   delete[] hreverse;
    iter = NULL;
    iter_start= NULL;

}
void HMdcOffset::initVariables()
{
    // pointer to iterators and categories
    rawCat       =0;
    calStartCat  =0;
    iter         =0;
    iter_start   =0;

    // switch options
    isPulserFile =kFALSE;
    noStart      =kFALSE;
    useTimeCuts  =kTRUE;

    // pointers to fileds and hists
    hreverse     = (MyField*) new MyField;
    hint         =0;
    hinv         =0;

    // container pointers
    calparraw    =0;
    lookupgeom   =0;
    timecut      =0;

    // varables in finalize
    validRange   =1000.;
    meanhOffset  =0;
    myoffset=0;
    myerror=0;

    // counters
    eventcounter =0;
    skipcounter  =0;
    nSkipEvents  =0;

    // output file names
    fNameAsciiOffset =0;
    fNameRootOffset  =0;
    ferrorlog=0;
    offsetTuple      =0;
    offsetPulserTuple=0;

    // fit variables
    yequalzero       =0;
    crosspointX      =0;
    fitpar0          =0;
    fitpar0error     =0;
    fitpar1          =0;
    fitpar1error     =0;

    fitparNoise0     =0;
    fitparNoise0error=0;
    fitparNoise1     =0;
    fitparNoise1error=0;
    totalsigma       =0;

    fitGaussMean     =0;
    fitGaussSigma    =0;

    
}
void HMdcOffset::setDefault()
{
    // Sets the the default values for the fits.
    setNoise(100, 50);
    setThreshold(0.15, 0.5);
    setRangeGauss(50);
}

void HMdcOffset::setOutputAscii(Char_t *c)
{
    // Sets ascii output of HMdcOffset for debugging output.
    if (fNameAsciiOffset) delete fNameAsciiOffset;
    fNameAsciiOffset = new Char_t[strlen(c)+1];
    strcpy(fNameAsciiOffset, c);
}

void HMdcOffset::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcOffset where all created histograms were written.
    //
    if (fNameRootOffset) delete fNameRootOffset;
    fNameRootOffset = new Char_t[strlen(c)+1];
    strcpy(fNameRootOffset, c);
}
void HMdcOffset::printStatus()
{
    printf("**************** HMdcOffset ***********************\n");
    printf("* ACTUAL SETTINGS                                 *\n");
    if(useTimeCuts)
    {
	printf("*    TIME CUTS are used                           *\n");
    }
    if(!useTimeCuts)
    {
	printf("*    NO TIME CUTS are used                        *\n");
    }
    if(isPulserFile)
    {
	printf("*    Mode set to PULSER FILE                      *\n");
    }
    if(!isPulserFile)
    {
	printf("*    Mode set to REAL DATA FILE                   *\n");
    }

    if(!noStart)
    {
	printf("*    START TIMES are used                         *\n");
    }
    if(noStart)
    {
	printf("*    NO START TIMES are used                      *\n");
    }
    printf("*    VALID OFFSET RANGE = %5.1f                  *\n",validRange);
    printf("*    NOISE OFFSET = %4i                          *\n",offsetfitNoise);
    printf("*    NOISE WIDTH  = %4i                          *\n",widthfitNoise);
    printf("*    MIN Threshold   = %1.2f MAX Threshold = %1.2f  *\n",minfitthreshold,maxfitthreshold);
    printf("*    Range GAUSS FIT = %4i                       *\n",rangeGauss);
    printf("**************** HMdcOffset ***********************\n");
}
Bool_t HMdcOffset::init(void)
{
    //  Inits HMdcOffset and the needed HMdcCalParRaw, if this container does not exists
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
    skipcounter=0;
    printStatus();
    return kTRUE;
}

Bool_t HMdcOffset::reinit(void)
{
    //
    cout<<"skipcount "<<skipcounter<<endl;
    skipcounter=0;

    fActive=kTRUE;
    return kTRUE;
}



void HMdcOffset::createHist(TFile* file,Int_t s, Int_t m, Int_t l, Int_t c)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are created
    // in a subdirectory structure.
    static Char_t title[50], tmp[30];

    sprintf(title,"%s%i%s%i%s%i%s%i","Sector ",s,"  Module ",m,"  Mbo ", l,"  Tdc ", c);
    file->TDirectory::Cd("hinv");
    sprintf(tmp, "%s%i%s%i%s%02i%s%02i%s", "hinv[",s,"][",m,"][",l,"][",c,"]");
    hinv = new MyHist(tmp, title, nbin,-nbin+0.5,0.5); //inverted hist.
    file->TDirectory::Cd("../hint");
    sprintf(tmp, "%s%i%s%i%s%02i%s%02i%s", "hint[",s,"][",m,"][",l,"][",c,"]");
    hint = new MyHist(tmp, title, nbin,-nbin+0.5,0.5); // integrated hist.
    file->TDirectory::Cd("..");
}
void HMdcOffset::createHist_2D(Int_t s, Int_t m)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are created
    // in a subdirectory structure.
    static Char_t title[50], tmp[30];
    for(Int_t mb=0;mb<16;mb++)
    {
	sprintf(title,"%s%i%s%i%s%i","Sector ",s,"  Module ",m,"  Mbo ", mb);
	sprintf(tmp, "%s%i%s%i%s%02i%s", "htime1_mbo[",s,"][",m,"][",mb,"]");
	htime1_mbo[mb] = new TH2F(tmp, title,700,-200,500,96,1,97); //hist time vers tdc.
    }
    for(Int_t lay=0;lay<6;lay++)
    {
	sprintf(title,"%s%i%s%i%s%i","Sector ",s,"  Module ",m,"  Lay ", lay);
	sprintf(tmp, "%s%i%s%i%s%02i%s", "htime1_lay[",s,"][",m,"][",lay,"]");
	htime1_lay[lay] = new TH2F(tmp, title,700,-200,500,210,1,211); //hist time vers tdc.
    }

}
void HMdcOffset::deleteHist()
{
    // Created histograms are deleted
    delete hinv;
    delete hint;
}
void HMdcOffset::deleteHist_2D()
{
    // Created histograms are deleted
    for(Int_t mb=0;mb<16;mb++)
    {
      delete htime1_mbo[mb];
    }
    for(Int_t lay=0;lay<6;lay++)
    {
      delete htime1_lay[lay];
    }
}

void HMdcOffset::fillHist(Int_t s, Int_t m, Int_t l, Int_t c)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are filled

    hint->SetBinContent(1,(Double_t)(*hreverse)[s][m][l][c][0]);
    hinv->SetBinContent(1,(Double_t)(*hreverse)[s][m][l][c][0]);

    for(Int_t k=2; k<nbinp1; k++)
    {
	hinv->SetBinContent(k,(Double_t)(*hreverse)[s][m][l][c][k-1]);
	hint->SetBinContent(k,(hint->GetBinContent(k-1)+hinv->GetBinContent(k)));
    }
    integral[s][m][l][c]=(Int_t)hint->GetBinContent(nbin-2);
}
void HMdcOffset::fillHist_2D(Int_t s, Int_t m, Int_t l, Int_t c)
{
    // Histograms for Time1 vers Tdc-Channel and cell are filled
    HMdcLookupChan& chan=(*lookupgeom)[s][m][l][c];
    Int_t layer= chan.getNLayer();
    Int_t cell= chan.getNCell();
    Int_t myoffbin=(Int_t)(2048-offsets[s][m][l][c]); // convert offset x to offset bin
    for(Int_t k=1; k<nbinp1; k++) // loop over all bins of hinv  (x from -2048 to 0)
    {
	if(k-myoffbin<500&&k-myoffbin>-200) // check if bin in range
	{
	    Int_t mybin=200+k-myoffbin;
	    Float_t temp=htime1_mbo[l]->GetCellContent(mybin,c+1);
	    htime1_mbo[l]    ->SetCellContent(mybin,c+1   ,hinv->GetBinContent(k)+temp);
	    temp=htime1_lay[layer]->GetCellContent(mybin,cell+1);
	    htime1_lay[layer]->SetCellContent(mybin,cell+1,hinv->GetBinContent(k)+temp);
	}
    }
}

Int_t HMdcOffset::fitHist(Int_t s, Int_t m, Int_t l, Int_t c)
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
    // the fit is set through the function setRangeGauss()(default value: 50) around the maximum.
    // The mean and sigma of the fit is written to the ascii debugging output.
    // The hist is checked for multiple peaks.The peaks which have been found are written to an array.

    fitGaussMean  = 0;
    fitGaussSigma = 0;
    yequalzero=0;
    crosspointX=0;

    if(hint->GetMinimum()<0)
    {
        cout<<" "<<endl;
	cout<<"WARNING: IN HINT "<<s<<" "<<m<<" "<<l<<" "<<c<<" NEGATIV VALUES DETECTED!"<<endl;
        cout<<" "<<endl;
	fprintf(ferrorlog,"%i %i %i %2i %s\n",s,m,l,c,"fit: in hint negative values detected!");
    }

    Int_t   xmaxfitbin=nbinm1;

    Float_t yminfit=minfitthreshold*hint->GetBinContent(nbinm1-1);     // setting y thresholds for fitting
    Float_t ymaxfit=maxfitthreshold*hint->GetBinContent(nbinm1-1);     // relative to maximum y-value
    while (hint->GetBinContent(--xmaxfitbin) > ymaxfit && xmaxfitbin); // find to ymaxfit corresponding bin

    if (!xmaxfitbin)
    {
	yequalzero=1;
        crosspointX=1;
	return 1;
    }

    Int_t xminfitbin=xmaxfitbin;
    while (hint->GetBinContent(--xminfitbin) > yminfit && xminfitbin); // find to yminfit corresponding bin

    if (!xminfitbin)
    {
	yequalzero=2;
	crosspointX=2;
	return 2;
    }

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

    if (fitpar1==0)
    {
	yequalzero=3;
	crosspointX=3;
	return 3;
    }
    yequalzero=0;
    yequalzero =-fitpar0/fitpar1;

    if (-yequalzero < (offsetfitNoise+widthfitNoise) || -yequalzero > nbin)
    {
	crosspointX=4;
	return 4;
    }

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

    if (fitpar1==fitparNoise1)
    {
	crosspointX=5;
	return 5;
    }

    crosspointX=0;
    crosspointX=-(fitparNoise0-fitpar0)/(fitpar1-fitparNoise1);

    if (crosspointX<0 || crosspointX>nbinm1 || isNaN(crosspointX))
    {
        crosspointX=6;
	return 6;
    }

    // calculating total sigma
    totalsigma= (Float_t)sqrt((pow(fitparNoise0error,2)/pow((fitparNoise1-fitpar1),2))
                    +(pow(fitpar0error,2)/pow((fitparNoise1-fitpar1),2))
                    +(pow((fitparNoise0-fitpar0),2)*pow(fitpar1error,2)/pow((fitparNoise1-fitpar1),4))
                    +(pow((fitparNoise0-fitpar0),2)*pow(fitparNoise1error,2)/pow((fitparNoise1-fitpar1),4))
                   );

    // check if fitpars are meaningful
    if(fitpar1<100000&&fitpar1>-100000)
    {
	fitslope1[s][m][l][c]=fitpar1;
    }
    else
    {
	fitslope1[s][m][l][c]=0;
    }
    if(fitparNoise1<100000&&fitpar1>-100000)
    {
	fitslope2[s][m][l][c]=fitparNoise1;
    }
    else
    {
	fitslope2[s][m][l][c]=0;
    }

    if (!isPulserFile) return 0;


    //f = new TF1("f3","gaus", -crosspointX-rangeGauss, -crosspointX+rangeGauss);
    // If there are more than 1 peak only the most prominant is fitted
    Float_t localmax=(2048-(hinv->GetMaximumBin()));
    f = new TF1("f3","gaus",-localmax-rangeGauss, -crosspointX+rangeGauss);

    hinv->Fit("f3","RNQ");
    fitGaussMean =f->GetParameter(1);  //mean
    fitGaussSigma=f->GetParameter(2);  //sigma

    findMultiplePeaks(s,m,l,c);

    delete f;

    // FIXME: Error handling for gauss fit ???

    return 0;
}
void HMdcOffset::findMultiplePeaks(Int_t s,Int_t m,Int_t l,Int_t c)
{
    for(Int_t j=0;j<2047;j++)
    {
	htime1temp->SetBinContent(j,hinv->GetBinContent(j));// copy hinv to temp hist
    }
    htime1temp->Smooth(10);// make it smooth
    Char_t address[50];
    Float_t binlowold[5];
    Float_t binhighold[5];
    Float_t binlow[5];
    Float_t binhigh[5];
    Int_t binmax[5];
    Float_t offout[5];

    Float_t max=0;
    Float_t min=0;
    Float_t threshold=0.05;

    for(Int_t j=0;j<5;j++)// reset helper arrays
    {
	binlowold[j]=0;
	binhighold[j]=0;
	binlow[j]=0;
	binhigh[j]=0;
	binmax[j]=0;
	offout[j]=0;
    }
    max=min=0;
    max=htime1temp->GetMaximum(); // get max height
    min=threshold*max;
    Int_t count=0;
    for(Int_t bin=0;bin<2047;bin++)
    {
	if(count<5)
	{
	    binlow [count]=htime1temp->GetBinContent(bin);
	    binhigh[count]=htime1temp->GetBinContent(bin+1);

	    if(bin>40 && binlow[count]>20  // skip the first 40 tdc bins and take into account only if > 20 counts
	       && fabs(binhigh[count]-binlow[count])>1
	       && (binhighold[count]-binlowold[count]>0)
	       && (binhigh[count]-binlow[count]<0) ) // if zero was crossed
	    {
		binmax[count]=bin;
		count++;
	    }
	    if(count<5)
	    {
		binlowold [count]=binlow [count];
		binhighold[count]=binhigh[count];
	    }
            else
	    {
		cout<<"MORE THAN 5 PEAKS!"<<endl;
	    }
	}
    }
    if(count>1)
    {
        cout<<" "<<endl;
	cout<<"sector "<<s<<" module "<<m<<" mbo "<<l<<" tdc "<<c<<endl;
	for(Int_t i=0;i<count;i++)
	{
	    if(i<5)
	    {
		cout<<"peak found: "<<i+1<<" bin="<<binmax[i]<<" ,x="<<(2048-binmax[i])<<" max="<<binlow[i]<<endl;
                sprintf(address,"%i %i %i %2i",s,m,l,c);
		fprintf(ferrorlog,"%s %i %s %4i %s %4i %s %8.0f\n",address,i+1,"multiple peaks: bin=",binmax[i]," ,x=",(2048-binmax[i])," max=",binlow[i]);
	    }
	    else
	    {
		cout<<"MORE THAN 5 PEAKS!"<<endl;
	    }
	}
	cout<<" "<<endl;
    }

    for(Int_t j=0;j<5;j++)// coppy to global array
    {
	if(binmax[j]>0)
	{
	    offsetpulser[s][m][l][c][j]=(2048-binmax[j]);
	}
	else
	{
	    offsetpulser[s][m][l][c][j]=0;
	}
    }
}
void HMdcOffset::writeAscii(ofstream &fout, Int_t s, Int_t m, Int_t l, Int_t c)
{
    // The adresses of the channel(sector,module,mbo,tdc),offset, two fitparameters for the
    // two linear fits, the sigma of the offset ,offset and the integral(number of entries for this channel)
    // are written to the ascii debugging output.In the case of an pulser file (external calibration)
    // the mean and the sigma of the gaussian fit are also written to the output.
    if (isPulserFile)
        fout
            << setw(3)  << s                 << " "
            << setw(4)  << m                 << " "
            << setw(4)  << l                 << " "
            << setw(5)  << c                 << " "
	    << setw(10) << -yequalzero       << " "
            << setw(10) << yequalzero        << " "
	    << setw(10) << crosspointX       << " "
            << setw(15) << -fitGaussMean     << " "
            << setw(15) << fitGaussSigma     << " "
            << setw(15) << fitpar0           << " "
            << setw(15) << fitpar1           << " "
            << setw(15) << fitparNoise0      << " "
            << setw(15) << fitparNoise1      << " "
            << setw(15) << totalsigma        << " "
            << setw(10) << integral[s][m][l][c]

            << endl;
    else
        fout
            << setw(3)  << s                  << " "
            << setw(4)  << m                  << " "
            << setw(4)  << l                  << " "
            << setw(5)  << c                  << " "
	    << setw(10) << -yequalzero        << " "
	    << setw(10) << crosspointX        << " "
            << setw(15) << fitpar0            << " "
            << setw(15) << fitpar1            << " "
            << setw(15) << fitparNoise0       << " "
            << setw(15) << fitparNoise1       << " "
            << setw(15) << totalsigma         << " "
            << setw(10) << integral[s][m][l][c]
            << endl;
}

void HMdcOffset::writeHist(TFile* file)
{
    // All created histograms are written to a rootfile.The file is structured for sector,module,mbo.
    
    file->TDirectory::Cd("hinv");
    hinv->Write();
    file->TDirectory::Cd("../hint");
    hint->Write();
    file->TDirectory::Cd("..");
}
void HMdcOffset::writeHist_2D()
{
    // All created histograms are written to a rootfile.
    for(Int_t mb=0;mb<16;mb++)
    {
	htime1_mbo[mb]->Write();
    }
    for(Int_t lay=0;lay<6;lay++)
    {
	htime1_lay[lay]->Write();
    }
}

ofstream *HMdcOffset::openAsciiFile()
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
                << setw(4)  << "mb"             << " "
                << setw(5)  << "t"              << " "
		<< setw(10) << "yequalzero"     << " "
		<< setw(10) << "Offset"         << " "
                << setw(15) << "GaussMean"      << " "
                << setw(15) << "GaussSigma"     << " "
                << setw(15) << "fitpar0"        << " "
                << setw(15) << "fitpar1"        << " "
                << setw(15) << "fitparNoise0"   << " "
                << setw(15) << "fitparNoise1"   << " "
                << setw(15) << "totalsigma"     << " "
                << setw(10) << "Integral"       << " "<< endl;
          *fout << setw(14) << "[MdcCalParRaw]" <<endl;
        }
        else
        {
            *fout //<< "Layer     Cell      crosspoint" << endl;
                << setw(3)  << "s"              << " "
                << setw(4)  << "mo"             << " "
                << setw(4)  << "mb"             << " "
                << setw(5)  << "t"              << " "
		<< setw(10) << "yequalzero"     << " "
		<< setw(10) << "Offset"         << " "
                << setw(15) << "fitpar0"        << " "
                << setw(15) << "fitpar1"        << " "
                << setw(15) << "fitparNoise0"   << " "
                << setw(15) << "fitparNoise1"   << " "
                << setw(15) << "totalsigma"     << " "
                << setw(10) << "Integral"       << " "<< endl;
          *fout << setw(14) << "[MdcCalParRaw]" <<endl;
        }
    return fout;
}
void HMdcOffset::fillNTuples(Int_t s,Int_t mo,Int_t mb,Int_t t)
{
    // fill NTuples for offsets and multiple peaks
    HMdcLookupChan& chan=(*lookupgeom)[s][mo][mb][t];
    offsetTuple->Fill(s,mo,mb,t,
		      chan.getNLayer(),
		      chan.getNCell(),
		      (*calparraw)[s][mo][mb][t].getSlope(),
		      (*calparraw)[s][mo][mb][t].getSlopeErr(),
		      offsets[s][mo][mb][t],
		      offsetErr[s][mo][mb][t],
		      integral[s][mo][mb][t],
		      offset1[s][mo][mb][t],
		      fitslope1[s][mo][mb][t],
		      fitslope2[s][mo][mb][t]);

    offsetPulserTuple->Fill(s,mo,mb,t,
			    chan.getNLayer(),
			    chan.getNCell(),
			    offsets[s][mo][mb][t],
			    offsetErr[s][mo][mb][t],
			    integral[s][mo][mb][t],
			    offsetpulser[s][mo][mb][t][0],
			    offsetpulser[s][mo][mb][t][1],
			    offsetpulser[s][mo][mb][t][2],
			    offsetpulser[s][mo][mb][t][3],
			    offsetpulser[s][mo][mb][t][4]);

}
void HMdcOffset::fillArrays(TH1F *hOffset, Int_t s,Int_t mo,Int_t mb,Int_t t)
{
    if(!isPulserFile)
    {
	hOffset->Fill(crosspointX);
	offsets[s][mo][mb][t]=crosspointX;
	offsetErr[s][mo][mb][t]=totalsigma;
	offset1[s][mo][mb][t]=-yequalzero;
    }
    else
    {
	hOffset->Fill(fitGaussMean);
	offsets[s][mo][mb][t]=-fitGaussMean;
	offsetErr[s][mo][mb][t]=fitGaussSigma;
	offset1[s][mo][mb][t]=-yequalzero;
    }
}
void HMdcOffset::fillCalParRaw(TH1F *hOffsetcorr, Int_t s,Int_t mo,Int_t mb,Int_t t)
{
    // fills CalParRaw with offsets, offseterrors and Methods
    // Checks if offset is in valid range arround mean value of offsets.
    // If it is out of range the value is replaced by the mean value.
    // Not connected channels are marked with -7.
    myoffset=offsets  [s][mo][mb][t];
    myerror =offsetErr[s][mo][mb][t];
    HMdcLookupChan& chan=(*lookupgeom)[s][mo][mb][t];
    Int_t connected=0;
    connected=chan.getNCell();
    if(!myoffset){myoffset=0;}

    if(myoffset>=0)   // all offsets where no error showed up
    {
	if((myoffset<meanhOffset-validRange) ||
	   (myoffset>meanhOffset+validRange))
	{   // if offset out of alowed range replace it by mean

	    (*calparraw)[s][mo][mb][t].setOffset(meanhOffset);
	    (*calparraw)[s][mo][mb][t].setOffsetMethod(0);
	    (*calparraw)[s][mo][mb][t].setOffsetErr(100);
	    offsets[s][mo][mb][t]=meanhOffset;

	    hOffsetcorr->Fill(meanhOffset);
	}
	else  // if offsets in allowed range and no error flag
	{
	    hOffsetcorr->Fill(myoffset);
	    (*calparraw)[s][mo][mb][t].setOffset(myoffset);
	    (*calparraw)[s][mo][mb][t].setOffsetMethod(2);
	    if(myerror>99) // if some nonsense value shows up in Err
	    {
		(*calparraw)[s][mo][mb][t].setOffsetErr(100);
		offsetErr[s][mo][mb][t]=100;
	    }
	    else // if Err gives nomal values
	    {
		(*calparraw)[s][mo][mb][t].setOffsetErr(myerror);
	    }
	}
    }
    else
    {  // if offsets contain error flags
	if(connected==-1) // if no wire is connected
	{
	    (*calparraw)[s][mo][mb][t].setOffset(-7);
	    offsets[s][mo][mb][t]=-7;
	    hOffsetcorr->Fill(-7);
	}
	else // if wire is connected but error showed up
	{
	    (*calparraw)[s][mo][mb][t].setOffset(myoffset);
	    hOffsetcorr->Fill(myoffset);
	}
	(*calparraw)[s][mo][mb][t].setOffsetMethod(2);
	(*calparraw)[s][mo][mb][t].setOffsetErr(100);
	offsetErr[s][mo][mb][t]=100;
    }
}

Bool_t HMdcOffset::finalize(void)
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
    cout << "From HMdcOffset::finalize" << endl;

    // Start Stop Watch
    TStopwatch timer;
    timer.Start();

    // open Root file for writing
    htime1temp = new MyHist("time1temp","time1temp", nbin,-nbin+0.5,0.5); //temp time1 hist.

    TFile *file = NULL;
    offsetTuple=NULL;
    offsetPulserTuple=NULL;
    ferrorlog=fopen("offset_error_log.txt","w"); // open file for log of errors/warnings

    if (fNameRootOffset)
    {
	file = new TFile(fNameRootOffset,"RECREATE");
	file->cd();
	offsetTuple=new TNtuple("offset","offset","s:m:mb:tdc:lay:cell:slope:slopeErr:off:offErr:integral:off1:fit1slope:fit2slope");
	offsetPulserTuple=new TNtuple("multipeaks","multipeaks","s:m:mb:tdc:lay:cell:off:offErr:integral:p1:p2:p3:p4:p5");


	// open ascii file for writing
        ofstream *fout=NULL;
	if (fNameAsciiOffset)
        {
	    fout = openAsciiFile();
        }
	TH1F *hOffset = new TH1F("Offset", "Offset", 512, 0, 2048);
	TH1F *hOffsetcorr = new TH1F("Offsetcorr", "Offsetcorr", 512, 0, 2048);

	Int_t err=-3;

	meanhOffset=0;

        initArrays();

	HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");
	if (!mdcDet)
	    cout << "Error in HMdcOffset::finalize: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
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
                    createHist_2D(s,mo);
		    Int_t nMb=(*calparraw)[s][mo].getSize();
		    for(Int_t mb=0; mb<nMb; mb++) {  //loop over layer
			TDirectory *dirMbo=NULL;
			if (dirMod)
			{
			    dirMbo=Mkdir(dirMod, "mbo", mb, 2);
			    dirMbo->mkdir("hinv");
			    dirMbo->mkdir("hint");
			}
			Int_t nTdc=(*calparraw)[s][mo][mb].getSize();
			for(Int_t t=0; t<nTdc; t++) {  //loop over cells
			    createHist(file, s, mo, mb, t);
			    fillHist  (s, mo, mb, t);
			    if ((err=fitHist(s, mo, mb, t))) crosspointX = fitpar1 = -err;
                            fillArrays(hOffset,s,mo,mb,t);
                            fillHist_2D(s,mo,mb,t);
			    if (file)  writeHist (file);
			    if (fNameAsciiOffset){writeAscii(*fout, s, mo, mb, t);}
			    deleteHist();
			}
			if (dirMbo) dirMbo->TDirectory::Cd("..");
			cout << "." << flush;
		    }
		    writeHist_2D();
		    deleteHist_2D();
		    cout<<" "<<endl;
		    if (dirMod) dirMod->TDirectory::Cd("..");
		}
		cout<<" "<<endl;
		if (dirSec) dirSec->TDirectory::Cd("..");
	    }
	cout << endl;

	// write offsets to calparraw
	myoffset=0;
	myerror=0;

	meanhOffset=(Float_t)((Int_t)(hOffset->GetMean()));

	for(Int_t s=0; s<6; s++)
	{  //loop over sectors
	    for(Int_t mo=0; mo<4; mo++)
	    {  //loop over modules
		if (!mdcDet->getModule(s, mo)) continue;
		Int_t nMb=(*calparraw)[s][mo].getSize();
		for(Int_t mb=0; mb<nMb; mb++)
		{  //loop over layer
		    Int_t nTdc=(*calparraw)[s][mo][mb].getSize();
		    for(Int_t t=0; t<nTdc; t++)
		    {  //loop over cells
                        fillCalParRaw(hOffsetcorr,s,mo,mb,t);
			fillNTuples(s,mo,mb,t);
		    }
		}
	    }
	}

	// stop watch
	cout << "Time of cell loop (offset calculation time):" << endl;
	timer.Print();
	timer.Stop();

	hOffset->Write();
	hOffsetcorr->Write();
	offsetTuple->Write();
        offsetPulserTuple->Write();

	delete hOffset;
	delete hOffsetcorr;

	// close root file
	file->Save();
	file->Close();
	delete file;

	if (fNameAsciiOffset)
	{ // close ascii file
	    fout->close();
	    delete fout;
	}
	else
	{
	    cout<<"WARNING: NO ASCII OUTPUT SET, NO FILE WRITTEN TO DISK!"<<endl;
            fprintf(ferrorlog,"%s\n","WARNING: NO ASCII OUTPUT SET, NO FILE WRITTEN TO DISK!");
	}
    }
    else
    {
	cout<<"WARNING: NO ROOT OUTPUT SET, NO FILE WRITTEN TO DISK!"<<endl;
        fprintf(ferrorlog,"%s\n","WARNING: NO ROOT OUTPUT SET, NO FILE WRITTEN TO DISK!");
    }
    
    fclose(ferrorlog);
    return kTRUE;
}

TDirectory *HMdcOffset::Mkdir(TDirectory *dirOld, Char_t *c, Int_t i, Int_t p)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
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

Float_t HMdcOffset::getstarttime(){
  Int_t i=0;
  HStartCal* startcal=0;
  iter_start->Reset();
  Float_t starttime=0;
  while ((startcal=(HStartCal *)iter_start->Next())!=0) {
      switch (startcal->getStrip()){
      case 0:  starttime=(startcal->getTime()/*-0*/);
      break;
      case 1:  starttime=(startcal->getTime()/*-0*/);
      break;
      case 2:  starttime=(startcal->getTime()/*-0.98*/);
      break;
      case 3:  starttime=(startcal->getTime()/*-0.26*/);
      break;
      case 4:  starttime=(startcal->getTime()/*-0*/);
      break;
      case 5:  starttime=(startcal->getTime()/*-0*/);
      break;
      case 6:  starttime=(startcal->getTime()/*-0.98*/);
      break;
      case 7:  starttime=(startcal->getTime()/*-0.26*/);
      break;


      }
      i++;

  }
  //if (i!=1)cout<<"Multiplicity in Start > 1"<<endl;
  return starttime;
}

Int_t HMdcOffset::execute()
{
    // Raw data of Time1  multiplied by the slope of the channel taken from the
    // container of the calibration parameters are filled into the array for the
    // drift-time . This array contains 2048 (corresponding to the resulution of
    // the tdc chip) entries for each tdc channel.
    static HMdcRaw *raw=NULL;
    Int_t s, mo, mb, t;
    Float_t testTime1=0;
    Float_t testTime2=0;

    iter->Reset();
    if(skipcounter>nSkipEvents)
    {
	if(!noStart) // Start Time is used
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


		if(useTimeCuts)  // if Time Cuts and Start Time
		{
		    if(testTimeCuts(s,mo,testTime1,testTime2))
		    {
			// we have to use Float_ts here
			(*hreverse)[s][mo][mb][t]
			    [
			     (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope())+starttime)
			    ]++;
		    }
		}
		else
		{   // No Time Cuts and Start Time
		    (*hreverse)[s][mo][mb][t]
			[
			 (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope())+starttime)
			]++;
		}
	    }
	}
	else
	{ // NO Start is used
	    while ((raw=(HMdcRaw*)iter->Next()))
	    {
		s=mo=mb=t=-99;
		raw->getAddress(s, mo, mb, t);
		testTime1=raw->getTime(1);
		testTime2=raw->getTime(2);
		if(useTimeCuts) // if No Start and Time Cuts
		{
		    if(testTimeCuts(s,mo,testTime1,testTime2))
		    {
			// we have to use Float_ts here
			(*hreverse)[s][mo][mb][t]
			    [
			     (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope()))
			    ]++;
		    }
		}
		else   // if No Start and No Time Cuts
		{
		    (*hreverse)[s][mo][mb][t]
			[
			 (Int_t)(2048-((Float_t)(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope()))
			]++;
		}
	    }
	}
	if(eventcounter%10000==0){
	    cout<<eventcounter<<" events analyzed"<<endl;
	}
    }
    skipcounter++;
    eventcounter++;
    return 0;
}
