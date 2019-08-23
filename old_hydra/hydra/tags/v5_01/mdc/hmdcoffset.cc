//*-- AUTHOR : J.Kempter
//*-- Modified :

///////////////////////////////////////////////////////////////////////////////
//
// HMdcOffset
//
// Defines the offset  parameter for MDC calibration. Uses cal1 container
// as an input data
//
///////////////////////////////////////////////////////////////////////////////
#include <math.h>
#include <stdlib.h>
#include <fstream.h>
#include <iomanip.h>
#include <iostream.h>

#include <TF1.h>
#include <TH1.h>
#include <TStopwatch.h>

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

ClassImp(HMdcOffset)

//const Int_t HMdcOffset::cells_Num[] ={184, 192, 172, 172, 192, 192};
const Int_t HMdcOffset::nbin        = 2048;
const Int_t HMdcOffset::nbinp1      = HMdcOffset::nbin+1;
const Int_t HMdcOffset::nbinm1      = HMdcOffset::nbin-1;
const Int_t HMdcOffset::nSubEvents  = (6*192)*3;


HMdcOffset::HMdcOffset(void)
{
    setDefault();
    rawCat = NULL;
    iter   = NULL;
    isPulserFile = kFALSE;
    hintegral = (MyField*) new MyField;
    hreverse  = (MyField*) new MyField;
}

HMdcOffset::HMdcOffset(Text_t* name,Text_t* title) : HReconstructor(name,title)
{
    setDefault();
    rawCat = NULL;
    iter   = NULL;
    isPulserFile = kFALSE;
    hintegral = (MyField*) new MyField;
    hreverse  = (MyField*) new MyField;
}

HMdcOffset::~HMdcOffset(void)
{
    // Destructor.
    if (iter)       delete iter;
    if (fNameAscii) delete fNameAscii;
    if (fNameRoot)  delete fNameRoot;
    if (hintegral)  delete[] hintegral;
    if (hreverse)   delete[] hreverse;
    iter = NULL;
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
    if (fNameAscii) delete fNameAscii;
    fNameAscii = new Char_t[strlen(c)+1];
    strcpy(fNameAscii, c);
}

void HMdcOffset::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcOffset where all created histograms were written.
    //
    if (fNameRoot) delete fNameRoot;
    fNameRoot = new Char_t[strlen(c)+1];
    strcpy(fNameRoot, c);
}

Bool_t HMdcOffset::init(void)
{
    //  Inits HMdcOffset and the needed HMdcCalParRaw, if this container does not exists
    calparraw=(HMdcCalParRaw*)gHades->getRuntimeDb()->getContainer("MdcCalParRaw");
    if (!calparraw)
    {
        calparraw=new HMdcCalParRaw;
        gHades->getRuntimeDb()->addContainer(calparraw);
    }

    rawCat=gHades->getCurrentEvent()->getCategory(catMdcRaw);
    if (!rawCat) {
        rawCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcRaw);
        if (!rawCat) return kFALSE;
        else gHades->getCurrentEvent()->addCategory(catMdcRaw,rawCat,"Mdc");
    }
    // creates an iterator which loops over all fired cells
    iter=(HIterator *)rawCat->MakeIterator();

    fActive=kTRUE;

    return kTRUE;
}

inline void HMdcOffset::createHist(TFile* file,Int_t s, Int_t m, Int_t l, Int_t c)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are created
    // in a subdirectory structure.
    static Char_t title[50], tmp[30];

    sprintf(title,"%s%i%s%i%s%i%s%i","Sector ",s,"  Module ",m,"  Mbo ", l,"  Tdc ", c);
    file->Cd("hinv");
    sprintf(tmp, "%s%i%s%i%s%02i%s%02i%s", "hinv[",s,"][",m,"][",l,"][",c,"]");
    hinv = new MyHist(tmp, title, nbin,-nbin+0.5,0.5); //inverted hist.
    file->Cd("../hint");
    sprintf(tmp, "%s%i%s%i%s%02i%s%02i%s", "hint[",s,"][",m,"][",l,"][",c,"]");
    hint = new MyHist(tmp, title, nbin,-nbin+0.5,0.5); // integrated hist.
    file->Cd("..");
}

inline void HMdcOffset::deleteHist()
{
    // Created Histograms are deleted
    delete hinv;
    delete hint;
}

void HMdcOffset::fillHist(Int_t s, Int_t m, Int_t l, Int_t c)
{
    // Histograms for inverted Time1 and integrated Time1 per Tdc-Channel are filled
    MyInt *hi = &((*hintegral)[s][m][l][c][0]);
    MyInt *hr = &((*hreverse) [s][m][l][c][0]);
    *hi = *hr;
    hint->SetBinContent(1, *hi++);
    hinv->SetBinContent(1, *hr++);
    for(Int_t k=2; k<nbinp1; k++) {
        *hi=*(hi-1) + *hr;
        hint->SetBinContent(k, *hi++);
        hinv->SetBinContent(k, *hr++);
    }
}

inline Int_t HMdcOffset::fitHist(Int_t s, Int_t m, Int_t l, Int_t c)
{
    // The offset is calculated from two linear fits.
    // The first linear fit is done to find the rising edge of the integrated spectra.
    // The second linear fit is done to substract the noise.
    // The ranges for both fits can be set through the functions setNoise()(x-range) for the
    // second fit (default values: 100,50) and setThreshold()(y-range)for the first
    // fit (default values :0.15,0.50).
    // The y-range is calculated by the percentage of the maximum height of the
    // spectra (default values : 0,15,0.5).
    //
    // The offset ist calculated from the intersection point of both linear fits.
    // The sigma of the offset is calculated from the sigmas of the two linear fits.
    //
    // If the analyzed file is a pulser file for external calibration a gaussian fit is done.
    // To set this fit the function setPulserFile() has to be used in the macro.
    // The range of the fit is set through the function setRangeGauss()(default value: 50) around the
    // calculated offset.The mean and sigma of the fit is written to the ascii debugging output.
    fitGaussMean  = 0;
    fitGaussSigma = 0;

    MyInt *hI = &((*hintegral)[s][m][l][c][0]);
    //if(c<1)cout<<((*hintegral)[s][m][l][c][0])<<" integral"<<endl;
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
    fitpar0     =f->GetParameter(0);  // get fitparameters
    fitpar0error=f->GetParError (0);
    fitpar1     =f->GetParameter(1);  //(f1->GetParameter(1)<0.000001)?-1:(f1->GetParameter(1));
    fitpar1error=f->GetParError (1);  // y=fitpar[1]* x +  fitpar[0]
    delete f;

    if (fitpar1==0) return 3;

    yequalzero =-fitpar0/fitpar1;

    if (-yequalzero < (offsetfitNoise+widthfitNoise) || -yequalzero > nbin) return 4;

    f = new TF1("f2","pol1",
                yequalzero-(offsetfitNoise+widthfitNoise),
                yequalzero-offsetfitNoise);
    hint->Fit("f2","RNQ");
    fitparNoise0     =f->GetParameter(0); // get fitparameters
    fitparNoise1     =f->GetParameter(1);
    fitparNoise0error=f->GetParError (0); // get fitparameters
    fitparNoise1error=f->GetParError (1);
    delete f;

    if (fitpar1==fitparNoise1) return 5;

    crosspointX=-(fitparNoise0-fitpar0)/(fitpar1-fitparNoise1);

    if (crosspointX<0 || crosspointX>nbinm1 || isNaN(crosspointX)) return 6;


    // calculating total sigma
    totalsigma= (Float_t)sqrt((pow(fitparNoise0error,2)/pow((fitparNoise1-fitpar1),2))
                    +(pow(fitpar0error,2)/pow((fitparNoise1-fitpar1),2))
                    +(pow((fitparNoise0-fitpar0),2)*pow(fitpar1error,2)/pow((fitparNoise1-fitpar1),4))
                    +(pow((fitparNoise0-fitpar0),2)*pow(fitparNoise1error,2)/pow((fitparNoise1-fitpar1),4))
                   );
  
    
     if (!isPulserFile) return 0;

    f = new TF1("f3","gaus", crosspointX-rangeGauss, crosspointX+rangeGauss);
    hinv->Fit("f3","RNQ");
    fitGaussMean =f->GetParameter(1);  //mean
    fitGaussSigma=f->GetParameter(2);  //sigma
    delete f;

    // FIXME: Error handling for gauss fit ???

    return 0;
}

inline void HMdcOffset::writeAscii(ofstream &fout, Int_t s, Int_t m, Int_t l, Int_t c)
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
            << setw(10) << crosspointX       << " "
            << setw(15) << fitGaussMean      << " "
            << setw(15) << fitGaussSigma     << " "
            << setw(15) << fitpar0           << " "
            << setw(15) << fitpar1           << " "
            << setw(15) << fitparNoise0      << " "
            << setw(15) << fitparNoise1      << " "
            << setw(15) << totalsigma        << " "
            << setw(10) << ((*hintegral)[s][m][l][c][nbin-2])

            << endl;
    else
        fout
            << setw(3)  << s                  << " "
            << setw(4)  << m                  << " "
            << setw(4)  << l                  << " "
            << setw(5)  << c                  << " "
            << setw(10) << crosspointX        << " "
            << setw(15) << fitpar0            << " "
            << setw(15) << fitpar1            << " "
            << setw(15) << fitparNoise0       << " "
            << setw(15) << fitparNoise1       << " "
            << setw(15) << totalsigma         << " "
            << setw(10) << ((*hintegral)[s][m][l][c][nbin-2])
            << endl;
}

inline void HMdcOffset::writeHist(TFile* file)
{
    // All created histograms are written to a rootfile.The file is structured for sector,module,mbo.
    
    file->Cd("hinv");
    hinv->Write();
    file->Cd("../hint");
    hint->Write();
    file->Cd("..");
}

ofstream *HMdcOffset::openAsciiFile()
{
    // Opens the ascii debugging output and writes the headerline.
    ofstream *fout=NULL;
    if (fNameAscii)
        fout = new ofstream(fNameAscii, ios::out);
    if (*fout)
        if (isPulserFile)
        {
            *fout //<< "Layer     Cell      crosspoint" << endl;
                << setw(3)  << "s"              << " "
                << setw(4)  << "mo"             << " "
                << setw(4)  << "mb"             << " "
                << setw(5)  << "t"              << " "
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

Bool_t HMdcOffset::finalize(void)
{
    // This function is called after the execute function is finished.At this point
    // the arrays for the drit-time are filled.Froms this arrays the histograms for
    // the drift-time and the integrated drift-time filled.The fits for the calcutation
    // of the offsets are done and the offsets and the sigma of the offsets are
    // calculated .All histograms are written to a rootfile output and the information
    // of the fits to an ascii debugging output.The offset and the sigma of the offset
    // are filled into the container (HMdcCalParRaw) for the calibration parameters.
    // To characterize the method of calculation of the offsets the function setOffsetMethod()
    // of HMdcCalParRaw is used.As default value for the method 2 is used for  automatic
    // calibration.If no calibration is done this value is set to 0.
    cout << "From HMdcOffset::finalize" << endl;

    // Start Stop Watch
    TStopwatch timer;
    timer.Start();

    // open Root file for writing
    TFile *file = NULL;
    if (fNameRoot)
        file = new TFile(fNameRoot,"RECREATE");

    // open ascii file for writing
    ofstream *fout = openAsciiFile();

    TH1F *hOffset = new TH1F("Offset", "Offset", 512, 0, 2048);
    
    Int_t err;
    HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");

    if (!mdcDet)
        cout << "Error in HMdcOffset::finalize: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
    else
        for(Int_t s=0; s<6; s++) {  //loop over sectors
            TDirectory *dirSec=NULL;
            if (file) dirSec=Mkdir(file, "sector", s);
            for(Int_t mo=0; mo<4; mo++) {  //loop over modules
                if (!mdcDet->getModule(s, mo)) continue;
                TDirectory *dirMod=NULL;
                if (dirSec) dirMod=Mkdir(dirSec, "module", mo);

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
                        hOffset->Fill(crosspointX);
                        if (file)  writeHist (file);
                        if (*fout) writeAscii(*fout, s, mo, mb, t);
                        deleteHist();

                        if(!crosspointX){crosspointX=0;}
                        if(crosspointX<=0){
                            (*calparraw)[s][mo][mb][t].setOffset(0);
                            (*calparraw)[s][mo][mb][t].setOffsetMethod(0);
                            (*calparraw)[s][mo][mb][t].setOffsetErr(0);
                        }
                        else {
                            (*calparraw)[s][mo][mb][t].setOffset((nbinm1*((*calparraw)[s][mo][mb][t].getSlope()))-crosspointX);
                            (*calparraw)[s][mo][mb][t].setOffsetMethod(2);
                            if(totalsigma>99){
                                (*calparraw)[s][mo][mb][t].setOffsetErr(100);
                            }
                            else {
                                (*calparraw)[s][mo][mb][t].setOffsetErr(totalsigma);
                            }
                        }
                    }
                    if (dirMbo) dirMbo->Cd("..");
                    cout << "." << flush;
                }
                if (dirMod) dirMod->Cd("..");
            }
            if (dirSec) dirSec->Cd("..");
        }
    cout << endl;

    // stop watch
    cout << "Time of cell loop (offset calculation time):" << endl;
    timer.Print();
    timer.Stop();

    hOffset->Write();
    delete hOffset;
    

    // close root file
    file->Save();
    file->Close();
    delete file;
    
    // close ascii file
    fout->close();
    delete fout;
    
    return kTRUE;
}

TDirectory *HMdcOffset::Mkdir(TDirectory *dirOld, Char_t *c, Int_t i, Int_t p=1)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
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

inline Int_t HMdcOffset::execute()
{
    // Raw data of Time1  multiplied by the slope of the channel taken from the
    // container of the calibration parameters are filled into the array for the
    // drift-time .This array contains 2048 (corresponding to the resulution of
    // the tdc chip) entries for each tdc channel.
    static HMdcRaw *raw=NULL;
    static Int_t s, mo, mb, t;
    iter->Reset();
    while ((raw=(HMdcRaw*)iter->Next()))
    {
        raw->getAddress(s, mo, mb, t);

        if(raw->getTime(1)>0 && raw->getTime(2)>0){      // cuts data which have no sense
            if((raw->getTime(1)- raw->getTime(2))>10){   // this are necessary conditions!

                // we have to use Float_ts here
                (*hreverse)[s][mo][mb][t]
                    [
                     (Int_t)((Float_t)(-1.0)*(raw->getTime(1))*(*calparraw)[s][mo][mb][t].getSlope())
                    ]++;
            }
        }
    }
    return 0;
}