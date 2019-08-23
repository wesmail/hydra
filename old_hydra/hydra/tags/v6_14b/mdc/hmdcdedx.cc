////////////////////////////////////////////////////////////////////////////
//*-- AUTHOR : J. Markert
////////////////////////////////////////////////////////////////////////////
// HMdcDeDx
//
//  Container class for lookup parameters of the status of the
//  wire (working or dead + noise + efficiency).
//  The parameters are stored in TH2F histograms which can be plotted.
//  This container is needed by HMdcDigitizer and HMdcTrackFinder
//
////////////////////////////////////////////////////////////////////////////
#include "hmdcdedx.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"
#include "hparamlist.h"
#include "hmessagemgr.h"
#include "hmdcdetector.h"
#include "hspectrometer.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hrecevent.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hmdccal1.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hmdcclusinf.h"
#include "hmdcclusfit.h"
#include "hmdcwirefit.h"

#include "TStyle.h"
#include "TH1.h"
#include <stdlib.h>

ClassImp(HMdcDeDx)

HMdcDeDx::HMdcDeDx(const char* name,const char* title,
                           const char* context)
    : HParCond(name,title,context)
{
    //
    strcpy(detName,"Mdc");
    clear();
    loccal.set(4,0,0,0,0);
    minimumWires=5;
}
HMdcDeDx::~HMdcDeDx()
{
    // destructor
}
void HMdcDeDx::clear()
{
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            for(Int_t a=0;a<18;a++){
                for(Int_t d=0;d<100;d++){
                    shift[s][m][a][d]=0;
                    shiftsigma[s][m][a][d]=0;
                }
            }
        }
    }

    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HMdcDeDx::printParam(void)
{
    // prints the parameters of HMdcDeDx to the screen.
    SEPERATOR_msg("#",60);
    INFO_msg(10,DET_MDC,"HMdcDeDx:");
    INFO_msg(10,DET_MDC,"Shift:");
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            for(Int_t a=0;a<18;a++){
                for(Int_t d=0;d<10;d++){
                    printf("s %i m %i angle %2i dist %3i-%3i",s,m,a,d*10,(d+1)*10);
                    printf("%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f \n",
                           shift[s][m][a][d*10+0],shift[s][m][a][d*10+1],shift[s][m][a][d*10+2],shift[s][m][a][d*10+3],shift[s][m][a][d*10+4],
                           shift[s][m][a][d*10+5],shift[s][m][a][d*10+6],shift[s][m][a][d*10+7],shift[s][m][a][d*10+8],shift[s][m][a][d*10+9]);
                }
            }
        }
    }
    INFO_msg(10,DET_MDC,"Shift sigma:");
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            for(Int_t a=0;a<18;a++){
                for(Int_t d=0;d<10;d++){
                    printf("s %i m %i angle %2i dist %3i-%3i",s,m,a,d*10,(d+1)*10);
                    printf("%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f \n",
                           shiftsigma[s][m][a][d*10+0],shiftsigma[s][m][a][d*10+1],shiftsigma[s][m][a][d*10+2],shiftsigma[s][m][a][d*10+3],shiftsigma[s][m][a][d*10+4],
                           shiftsigma[s][m][a][d*10+5],shiftsigma[s][m][a][d*10+6],shiftsigma[s][m][a][d*10+7],shiftsigma[s][m][a][d*10+8],shiftsigma[s][m][a][d*10+9]);
                }
            }
        }
    }
    SEPERATOR_msg("#",60);
}
Bool_t HMdcDeDx::init(HParIo* inp,Int_t* set)
{
    // intitializes the container from an input
    HDetParIo* input=inp->getDetParIo("HMdcParIo");

    HMatrixCategory* catcal     =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1);
    if(!catcal) { Error("calDeDx()","HMdcCal1 Category not found in current Event!"); exit(1);}
    HMatrixCategory* cathit     =(HMatrixCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcHit);
    if(!cathit) { Error("calDeDx()","HMdcHit Category not found in current Event!"); exit(1);}
    HLinearCategory* catclusinf =(HLinearCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusInf);
    if(!catclusinf) { Error("calDeDx()","HMdcCLusInf Category not found in current Event!"); exit(1);}
    HLinearCategory* catclusfit =(HLinearCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusFit);
    if(!catclusfit) { Error("calDeDx()","HMdcCLusFit Category not found in current Event!"); exit(1);}
    HLinearCategory* catwirefit =(HLinearCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcWireFit);
    if(!catwirefit) { Error("calDeDx()","HMdcWireFit Category not found in current Event!"); exit(1);}

    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HMdcDeDx::write(HParIo* output)
{
    // writes the container to an output
    HDetParIo* out=output->getDetParIo("HMdcParIo");
    if (out) return out->write(this);
    return -1;
}
void HMdcDeDx::putParams(HParamList* l)
{
    // Puts all params of HMdcDeDx to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;

}
Bool_t HMdcDeDx::getParams(HParamList* l)
{
    if (!l) return kFALSE;

    return kTRUE;
}
Float_t HMdcDeDx::getShiftSegment(Int_t s,Int_t m,Float_t angle,Float_t dist)
{
    // returns the shift in ns for sector,module,impact angle (0-90)
    // and distance from wire (mm). For distances larger 10 mm 0
    // is returned.
    Float_t sh=0;
    if(dist<10)sh=shift[s][m][(Int_t)(angle/5)][(Int_t)(dist/0.1)];
    return sh;
}
Float_t HMdcDeDx::getShiftSigmaSegment(Int_t s,Int_t m,Float_t angle,Float_t dist)
{
    // returns the shift sigma in ns for sector,module,impact angle (0-90)
    // and distance from wire (mm). For distances larger 10 mm 0
    // is returned.
    Float_t shsig=0;
    if(dist<10)shsig=shiftsigma[s][m][(Int_t)(angle/5)][(Int_t)(dist/0.1)];
    return shsig;
}
Float_t HMdcDeDx::getShift(Int_t s,Int_t m,Int_t anglebin,Int_t distbin)
{
    // returns the shift in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    Float_t sh=0;
    if(anglebin<17&&distbin<100)sh=shift[s][m][anglebin][distbin];
    else{Warning("HMdcDeDx:getShift()","array out of bounds!");}
    return sh;
}
void HMdcDeDx::setShift(Int_t s,Int_t m,Int_t anglebin,Int_t distbin,Float_t sh)
{
    // sets the shift in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    if(anglebin<17&&distbin<100)sh=shift[s][m][anglebin][distbin];
    else{Warning("HMdcDeDx:setShift()","array out of bounds!");}
}
Float_t HMdcDeDx::getShiftSigma(Int_t s,Int_t m,Int_t anglebin,Int_t distbin)
{
    // returns the shift sigma in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    Float_t shsig=0;
    if(anglebin<17&&distbin<100)shsig=shiftsigma[s][m][anglebin][distbin];
    else{Warning("HMdcDeDx:getShiftSigma()","array out of bounds!");}
    return shsig;
}
void HMdcDeDx::setShiftSigma(Int_t s,Int_t m,Int_t anglebin,Int_t distbin,Float_t sh)
{
    // sets the shift sigma in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    if(anglebin<17&&distbin<100)sh=shiftsigma[s][m][anglebin][distbin];
    else{Warning("HMdcDeDx:setShiftSigma()","array out of bounds!");}
}
TCanvas* HMdcDeDx::plotShift(Int_t sec,Int_t mod)
{
    gStyle->SetOptStat(0);
    Char_t name[400];
    sprintf(name,"%s%i%s%i","shift_",sec,"_",mod);
    TCanvas* result=new TCanvas(name,name,1000,800);
    result->Divide(6,3);

    TH1F* hshift[18];
    for(Int_t angle=0;angle<18;angle++){
        sprintf(name,"%s%i%s%i%s%i","hShift_sec",sec,"_mod",mod,"_angle",angle);
        hshift[angle]=new TH1F(name,name,100,0,100);
        hshift[angle]->SetXTitle("distance from wire [mm]");
        hshift[angle]->SetYTitle("shift [ns]");

        for(Int_t d=0;d<100;d++){
            hshift[angle]->SetBinContent(d+1,shift[sec][mod][angle][d]);
            hshift[angle]->SetBinError(d+1,shiftsigma[sec][mod][angle][d]);
        }
    result->cd(angle+1);
    hshift[angle]->DrawCopy();
    }
    return result;
}
void HMdcDeDx::sort(Int_t nHits,Float_t* measurements)
{
    // Puts the measurement values into increasing order.

    register Int_t a,b,c;
    Int_t exchange;
    Float_t val;

    if(nHits<=50)
    {
	for(a=0;a<nHits-1;++a)
	{
	    exchange = 0;
	    c = a;
	    val = measurements[a];

	    for(b=a+1;b<nHits;++b)
	    {
		if(measurements[b]<val)
		{
		    c = b;
		    val = measurements[b];
		    exchange = 1;
		}
	    }
	    if(exchange)
	    {
		measurements[c] = measurements[a];
		measurements[a] = val;
	    }
	}

    }
    else
    {
	Warning("HMdcDeDx:sort(nHits,Float* measurements)","nHits > 50! Entries >50 skipped! ");
    }
}
Float_t HMdcDeDx::calcDeDx(HMdcSeg* seg,Float_t* meanold,Float_t* sigmaold,Int_t* nwire,Float_t* sigmanew,Int_t* nwiretrunc)
{
    // calculates dedx of a MDC segment by doing normalization for
    // impact angle and distance from wire for the fired cells of
    // the segment. The measurements are sorted in increasing order,
    // the mean and sigma is calculated. Afterwards the truncated mean
    // according to the set window (in sigma units) arround the mean
    // is calculated and returned as dedx. Mean,sigma,number of wires before
    // truncating,truncated mean,truncated mean sigma and number of wires
    // after truncating can be obtained by the returned pointers.

    Float_t dedx     =-99.;
    Float_t mean     =-99.;
    Float_t sigma    =-99.;
    Int_t nWire      =  0;
    Int_t nWireTrunc =  0;

    static Float_t measurements[50];

    //------------------------getting input--------------------------------------------------
    nWire=fillingInput(seg,measurements);
    *nwire=nWire;
    mean =calcMean(measurements,nWire);
    *meanold=mean;
    //------------------------calculating sigma
    if(nWire>1)
    {
        sigma=calcSigma(measurements,nWire,mean);
        *sigmaold=sigma;
        //--------------------sorting measurements in increasing order
        sort(nWire,measurements);
        //--------------------truncating measurements outside +- x*sigma
        nWireTrunc=select(measurements,mean,sigma,nWire);
        *nwiretrunc=nWireTrunc;
        //--------------------calculating truncated mean
        dedx =calcMean(measurements,nWire);
        //--------------------calculating new sigma
        sigma=calcSigma(measurements,nWireTrunc,mean);
        *sigmanew=sigma;

    }else{
        Warning("calcDeDx()","nWireTrunc <=1 : dedx,sigmanew,sigmaold,nwiretruncskipped!");
    }
    return dedx;
}
Int_t HMdcDeDx::fillingInput(HMdcSeg* seg,Float_t* measurements)
{
    // Fills array of measurements from HMdcSeg and returns the number of fired wires in Segment
    // The single measurements are normalized by impact angle and distance from wire.

    Int_t s,m,l,c;
    Float_t t1,t2;
    Float_t alpha,mindist;

    Int_t nWire      =0;
    Float_t corr     =  0.;

    Int_t found       = 0;
    Int_t hitindex    =-1;
    Int_t clusinfindex=-1;
    Int_t clusfitindex=-1;
    Int_t first       = 0;
    Int_t last        = 0;
    //----------------------------------------getting input--------------------------------------------------
    for(Int_t i=0;i<2;i++)
    {   // loop over hit in Mdc 1 and 2
        hitindex=seg->getHitInd(i);
        if(hitindex!=-1)
        {   // hit for one module can be missing
            if(found==0)
            {  // we have to do it only once for the list of wires, because ClusFit is used by both clusInfs
                found++;

                HMdcHit* hit=(HMdcHit*)cathit->getObject(hitindex);

                clusinfindex=hit->getClusInfIndex();
                HMdcClusInf* clusinf=(HMdcClusInf*)catclusinf->getObject(clusinfindex);

                clusfitindex=clusinf->getClusFitIndex();

                if(clusfitindex!=-1)
                { // if fit has failed HMdcClusFit does not exist and the index is equal -1
                    HMdcClusFit* clusfit=(HMdcClusFit*)catclusfit->getObject(clusfitindex);

                    first=clusfit->getIndf();
                    last =clusfit->getIndl();

                    for(Int_t w=first;w<last;w++)
                    {   // loop over the list of wires
                        HMdcWireFit* wirefit=(HMdcWireFit*)catwirefit->getObject(w);
                        wirefit->getAddress(s,m,l,c);
                        alpha  =wirefit->getAlpha();
                        mindist=wirefit->getMinDist();

                        loccal[0]=s;
                        loccal[1]=m;
                        loccal[2]=l;
                        loccal[3]=c;

                        HMdcCal1* cal1 =(HMdcCal1*)catcal->getObject(loccal);
                        if(cal1!=0)
                        {
                            t1  =cal1->getTime1();
                            t2  =cal1->getTime2();

                            if(t2>-998)
                            {   // some times t2 can be missing (-999,-998)
                                // calculating mean value

                                corr=getShiftSegment(s,m,alpha,mindist);
                                if(nWire<49)
                                {
                                    measurements[nWire]=t2-t1 + corr;
                                    nWire++;
                                }else{
                                    Warning("fillingInput()","Number of wires in Segment > 50! Skipped!");
                                }
                            }
                        }else{
                            Warning("calcDeDx()","ZERO pointer recieved for cal1 object!");
                        }
                    }
                }
            }
        }
    }
    return nWire;
}
Float_t HMdcDeDx::calcSigma(Float_t* measurements,Int_t nWire,Float_t mean)
{
    // returns sigma from the float array of nWire measurements with a mean value mean.
    // Values equal to -99 are ignored.

    Float_t sum=0.;
    for(Int_t j=0;j<nWire;j++) { if(measurements[j]!=-99) sum = sum + (measurements[j]-mean)*(measurements[j]-mean);}
    Float_t sigma = sqrt(sum / (nWire-1));
    return sigma;
}
Float_t HMdcDeDx::calcMean(Float_t* measurements,Int_t nWire)
{
    // returns the mean value from the float array of nWire measurements.
    // Values equal to -99 are ignored.

    Float_t sum=0.;
    Float_t mean=-99.;
    for(Int_t j=0;j<nWire;j++) { if(measurements[j]!=-99) sum = sum + measurements[j];}
    mean = sum / nWire;
    return mean;
}
Int_t HMdcDeDx::select(Float_t* measurements,Float_t mean,Float_t sigma,Int_t nWire)
{
    // loops over the measurement array with nWire values and puts values to
    // -99 if the measurements are outside the wanted window arround the mean value mean
    // (set with setWindow() (sigma units)).
    // The procedure keeps at least a number of minimum wires set with setMinimumWires().

    Int_t nWireTrunc=nWire;
    Int_t count=0;
    while(nWireTrunc>getMinimumWires() && 2*count<=nWire)
    {
        // starting from lowest val
        if(fabs(measurements[count]-mean)>(getWindow()*sigma))
        { // if measurement outside the window
            if(nWireTrunc>getMinimumWires())
            { // do not throw away to many measurements
                measurements[count]=-99;
                nWireTrunc--;
            }
        }
        // taking highest val next
        if( 2*count <=nWire )
        {
            if(fabs(measurements[nWire-count]-mean)>(getWindow()*sigma))
            { // if measurement outside the window
                if(nWireTrunc>getMinimumWires())
                { // do not throw away to many measurements
                    measurements[nWire-count]=-99;
                    nWireTrunc--;
                }
            }
            count++;
        }
    }
    return  nWireTrunc;
}


