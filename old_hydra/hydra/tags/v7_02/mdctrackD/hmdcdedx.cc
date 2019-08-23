////////////////////////////////////////////////////////////////////////////
//*-- AUTHOR : J. Markert
////////////////////////////////////////////////////////////////////////////
// HMdcDeDx
//
// This transformation class calculates the "pseudo dEdx" from t2-t1 (time above threshold)
// of all fired drift cells included in one HMdcSeg. The transformation is performed doing a
// normalization of the measured t2-t1 with impact angle and minimum distance to wire
// (MDCCAL2 parametrization) and the algorithm to normalize the single measurements and
// average over all cells included in the segment.
//-------------------------------------------------------------------------
// Calculation of dEdx:
// Float_t calcDeDx(HMdcSeg*,Float_t*,Float_t*,UChar_t*,Float_t*,UChar_t*,Int_t,Int_t)
// calculates dedx of a MDC segment by doing normalization for
// impact angle and distance from wire for the fired cells of
// the segment. The measurements are sorted in increasing order,
// the mean and sigma is calculated. Afterwards the truncated mean
// is calculated and returned as dedx. Mean,sigma,number of wires before
// truncating,truncated mean,truncated mean sigma and number of wires
// after truncating can be obtained by the returned pointers.
// Different methods can be selected:
// vers==0 (default): Input filling from HMdcSeg
//                    (wires, thrown out by the fitter are ignored)
// vers==1          : Input filling from HMdcClusFit
//                    (wires, thrown out by the fitter are taken into account)
//                    If the fit has failed (chi2=-1), vers 0 is used
// mod==0           : calc dedx from 1st module in segment
// mod==1           : calc dedx from 2st module in segment
// mod==2 (default) : calc dedx from whole segment
//-------------------------------------------------------------------------
// Settings of the truncated Mean method:
// The truncated mean is calculated according to the set window (in sigma units)
// (setWindow(Float_t window)) arround the mean. For the calculation of the truncated
// mean only drift cells with a dEdx inside the window arround the mean (calculated from
// all drift cells of the segment) will be taken into account.
// With setMinimumWires(Int_t minwires) the algorithm can be forced to keep
// a minimum number of wires. The method will stop removing drift cells from the active
// sample if the minimum number is reached.
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
#include "hmdcsizescells.h"
#include "hpidphysicsconstants.h"
#include "hgeantkine.h"

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
    isInitialized=kFALSE;
    setWindow(3.);
    module=2; // do both modules of a segment
    initArray();
    useShifts=kTRUE;

}
HMdcDeDx::~HMdcDeDx()
{
    // destructor
}
void HMdcDeDx::clear()
{
    hefr    =0;
    ref_dedx=0;

    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            shift_dedx[s][m]=0;
            scale_dedx[s][m]=1;

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
void HMdcDeDx::printParam(TString opt)
{
    // prints the parameters of HMdcDeDx to the screen.

    cout<<"HMdcDeDx:"<<endl;
    if(opt.CompareTo("shift")==0||opt.CompareTo("all")==0)
    {
        cout<<"shift:"<<endl;

        for(Int_t s=0;s<6;s++){
            for(Int_t m=0;m<4;m++){
                for(Int_t a=0;a<18;a++){
                    for(Int_t d=0;d<10;d++){
                        printf("s %i m %i angle %2i dist %3i-%3i : ",s,m,a,d*10,(d+1)*10-1);
                        printf("%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f \n",
                               shift[s][m][a][d*10+0],shift[s][m][a][d*10+1],shift[s][m][a][d*10+2],shift[s][m][a][d*10+3],shift[s][m][a][d*10+4],
                               shift[s][m][a][d*10+5],shift[s][m][a][d*10+6],shift[s][m][a][d*10+7],shift[s][m][a][d*10+8],shift[s][m][a][d*10+9]);
                    }
                }
            }
        }
    }
    if(opt.CompareTo("shiftsigma")==0||opt.CompareTo("all")==0)
    {
        cout<<"shift sigma:"<<endl;
        for(Int_t s=0;s<6;s++){
            for(Int_t m=0;m<4;m++){
                for(Int_t a=0;a<18;a++){
                    for(Int_t d=0;d<10;d++){
                        printf("s %i m %i angle %2i dist %3i-%3i : ",s,m,a,d*10,(d+1)*10);
                        printf("%5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f %5.3f \n",
                               shiftsigma[s][m][a][d*10+0],shiftsigma[s][m][a][d*10+1],shiftsigma[s][m][a][d*10+2],shiftsigma[s][m][a][d*10+3],shiftsigma[s][m][a][d*10+4],
                               shiftsigma[s][m][a][d*10+5],shiftsigma[s][m][a][d*10+6],shiftsigma[s][m][a][d*10+7],shiftsigma[s][m][a][d*10+8],shiftsigma[s][m][a][d*10+9]);
                    }
                }
            }
        }
    }
    cout<<"shift_dedx:"<<endl;
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            printf("s %i m %i shift %7.3f \n",s,m,shift_dedx[s][m]);

        }
    }
    cout<<"scale_dedx:"<<endl;
    for(Int_t s=0;s<6;s++){
        for(Int_t m=0;m<4;m++){
            printf("s %i m %i scale %7.3f \n",s,m,scale_dedx[s][m]);

        }
    }
    cout<<"ref_dedx:"<<endl;
    printf("ref_dedx %7.3f \n",ref_dedx);
    cout<<"hefr:"<<endl;
    printf("hefr %7.3f \n",hefr);


}
Bool_t HMdcDeDx::init(HParIo* inp,Int_t* set)
{
    // intitializes the container from an input
    HDetParIo* input=inp->getDetParIo("HCondParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Bool_t HMdcDeDx::initContainer()
{
    if(!isInitialized)
    {

	catcal     =(HCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcCal1);
	if(!catcal) { Error("calDeDx()","HMdcCal1 Category not found in current Event!"); exit(1);}
	cathit     =(HCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcHit);
	if(!cathit) { Warning("calDeDx()","HMdcHit Category not found in current Event!");}
        catclusinf =(HCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusInf);
	if(!catclusinf){ Warning("calDeDx()","HMdcClusInf Category not found in current Event!");}
        catclusfit =(HCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcClusFit);
	if(!catclusfit){ Warning("calDeDx()","HMdcClusFit Category not found in current Event!");}
        catwirefit =(HCategory*)((HRecEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcWireFit);
	if(!catwirefit){ Warning("calDeDx()","HMdcWireFit Category not found in current Event!");}

   	sizescells=HMdcSizesCells::getExObject();
	if(!sizescells)
	{
	    Error("init()","NO HMDCSIZESCELLS CONTAINER IN INPUT!");
	    return kFALSE;
	}
	//if(!sizescells->hasChanged()) sizescells->initContainer();
	isInitialized=kTRUE;
    }
    return kTRUE;
}
Int_t HMdcDeDx::write(HParIo* output)
{
    // writes the container to an output
    HDetParIo* out=output->getDetParIo("HCondParIo");
    if (out) return out->write(this);
    return -1;
}
void HMdcDeDx::putParams(HParamList* l)
{
    // Puts all params of HMdcDeDx to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;
    Int_t size=6*4*18*100;
    l->addBinary("shift"        ,&shift[0][0][0][0],size);
    l->addBinary("shiftsigma"   ,&shiftsigma[0][0][0][0],size);
    l->add      ("ref_dedx"     ,ref_dedx);
    l->addBinary("shift_dedx"   ,&shift_dedx[0][0],24);
    l->addBinary("scale_dedx"   ,&scale_dedx[0][0],24);
    l->add      ("hefr"         ,hefr);

}
Bool_t HMdcDeDx::getParams(HParamList* l)
{
    if (!l) return kFALSE;
    Int_t size=6*4*18*100;
    if (!( l->fillBinary("shift"        ,&shift[0][0][0][0],size      ))) return kFALSE;
    if (!( l->fillBinary("shiftsigma"   ,&shiftsigma[0][0][0][0],size ))) return kFALSE;
    if (!( l->fill      ("ref_dedx"     ,&ref_dedx                    ))) return kFALSE;
    if (!( l->fillBinary("shift_dedx"   ,&shift_dedx[0][0],24         ))) return kFALSE;
    if (!( l->fillBinary("scale_dedx"   ,&scale_dedx[0][0],24         ))) return kFALSE;
    if (!( l->fill      ("hefr"         ,&hefr                        ))) return kFALSE;

    return kTRUE;
}
Float_t HMdcDeDx::getShiftSegment(Int_t s,Int_t m,Float_t angle,Float_t dist)
{
    // returns the shift in ns for sector,module,impact angle (0-90)
    // and distance from wire (mm). For distances larger 10 mm 0
    // is returned.
    Float_t sh=0;
    if(dist<10&&dist>=0)
    {
        if(angle>=0)
        {
           if(angle<90 )sh=shift[s][m][(Int_t)(angle/5)][(Int_t)(dist/0.1)];
           if(angle==90)sh=shift[s][m][17][(Int_t)(dist/0.1)];
        }
    }
    return sh;
}
Float_t HMdcDeDx::getShiftSigmaSegment(Int_t s,Int_t m,Float_t angle,Float_t dist)
{
    // returns the shift sigma in ns for sector,module,impact angle (0-90)
    // and distance from wire (mm). For distances larger 10 mm 0
    // is returned.
    Float_t shsig=0;
    if(dist<10&&dist>=0)
    {
        if(angle>=0)
        {
            if(angle<90) shsig=shiftsigma[s][m][(Int_t)(angle/5)][(Int_t)(dist/0.1)];
            if(angle==90)shsig=shiftsigma[s][m][17][(Int_t)(dist/0.1)];
        }
    }
   return shsig;
}
Float_t HMdcDeDx::getShift(Int_t s,Int_t m,Int_t anglebin,Int_t distbin)
{
    // returns the shift in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    Float_t sh=0;
    if(anglebin>=0&&anglebin<18&&distbin>=0&&distbin<100)
    {
        sh=shift[s][m][anglebin][distbin];
    }
    else{Warning("HMdcDeDx:getShift()","array out of bounds!");}
    return sh;
}
void HMdcDeDx::setShift(Int_t s,Int_t m,Int_t anglebin,Int_t distbin,Float_t sh)
{
    // sets the shift in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    if(anglebin>=0&&anglebin<18&&distbin>=0&&distbin<100)
    {
        shift[s][m][anglebin][distbin]=sh;
    }
    else{Warning("HMdcDeDx:setShift()","array out of bounds!");}
}
Float_t HMdcDeDx::getShiftSigma(Int_t s,Int_t m,Int_t anglebin,Int_t distbin)
{
    // returns the shift sigma in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    Float_t shsig=0;
    if(anglebin>=0&&anglebin<18&&distbin>=0&&distbin<100)
    {
        shsig=shiftsigma[s][m][anglebin][distbin];
    }
    else{Warning("HMdcDeDx:getShiftSigma()","array out of bounds!");}
    return shsig;
}
void HMdcDeDx::setShiftSigma(Int_t s,Int_t m,Int_t anglebin,Int_t distbin,Float_t sh)
{
    // sets the shift sigma in ns for sector,module,impact angle bin (0-17)
    // and distance from wire bin (0-99).
    if(anglebin>=0&&anglebin<18&&distbin>=0&&distbin<100)
    {
        shiftsigma[s][m][anglebin][distbin]=sh;
    }
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
Float_t HMdcDeDx::calcDeDx(HMdcSeg* seg,Float_t* meanold,Float_t* sigmaold,UChar_t* nwire,Float_t* sigmanew,UChar_t* nwiretrunc,Int_t vers,Int_t mod)
{
    // calculates dedx of a MDC segment by doing normalization for
    // impact angle and distance from wire for the fired cells of
    // the segment. The measurements are sorted in increasing order,
    // the mean and sigma is calculated. Afterwards the truncated mean
    // according to the set window (in sigma units) arround the mean
    // is calculated and returned as dedx. Mean,sigma,number of wires before
    // truncating,truncated mean,truncated mean sigma and number of wires
    // after truncating can be obtained by the returned pointers.
    // Different methods can be selected:
    // vers==0 : Input filling from HMdcSeg (wires, thrown out by the fitter are ignored)
    // vers==1 : Input filling from HMdcClusFit (wires, thrown out by the fitter are taken into account)
    // mod==0  : calc dedx from 1st module in segment
    // mod==1  : calc dedx from 2st module in segment
    // mod==2  : calc dedx from whole segment (default)
    // returns -99 if nothing was calculated

    //to be shure initialized values are returned
    *meanold   =-1.;
    *sigmaold  =-1.;
    *nwire     = 0;
    *sigmanew  =-1.;
    *nwiretrunc= 0;
    Float_t dedx       =-99.;
    Float_t mean       =-99.;
    Float_t sigma      =-99.;
    UChar_t nWire      =  0;
    UChar_t nWireTrunc =  0;

    if(seg==0)
    {
	Error("HMdcDeDx::calcDeDx()","ZERO POINTER FOR HMdcSeg RECEIVED!");
	return mean;
    }

    if(mod>=0&&mod<3)
    {
	module=mod;
    }
    else
    {
	Warning("calcDeDx()","Unknown module type! Will use both modules of Segment!");
        module=2;
    }

    //------------------------getting input--------------------------------------------------
    initArray();
    if(vers==0)
    {   // filling from standard segment
	nWire=fillingInput(seg,measurements);
    }
    else if(vers==1)
    {
	if(catclusfit!=0&&catwirefit!=0&&(seg->getChi2()>-0.9))
	{   // check if categories are available and fit has not failed
	    nWire=fillingInputClusFit(seg,measurements);
	}
	else
	{   // do standard way
	    nWire=fillingInput(seg,measurements);
	}
    }
    else {
	Warning("calcDeDx()","Unknown filling version, will use filling from HMdcSegment instead!");
	nWire=fillingInput(seg,measurements);
    }

    *nwire=nWire;
    if(nWire>0) mean =calcMean(measurements,nWire);
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
	Int_t nWmod0=seg->getSumWires(0);
        Int_t nWmod1=seg->getSumWires(1);
	Warning("calcDeDx()","nWire <=1 : %i nW in mod0, %i nW in mod1,\n skipped %i and %i with t2<=-998 !",
		nWmod0,nWmod1,ctskipmod1,ctskipmod1);
    }
    return dedx;
}
UChar_t HMdcDeDx::fillingInput(HMdcSeg* seg,Float_t* measurements)
{
    // Fills array of measurements from HMdcSeg and returns the number of fired wires in Segment
    // The single measurements are normalized by impact angle and distance from wire. Only the first
    // 50 wires in one segment are accepted. Corresponds to method 0 in calcDeDx(......, version)

    ctskipmod0=0;
    ctskipmod1=0;

    Float_t t1,t2;
    Double_t alpha,mindist;
    Double_t x1,y1,z1,x2,y2,z2;

    UChar_t nWire     =0;
    Float_t corr      =0.;
    //----------------------------------------getting input--------------------------------------------------

    for(Int_t l=0;l<12;l++)
    {
        if(module==0&&l>5) continue;  // fill for 1st module only
        if(module==1&&l<5) continue;  // fill for 2st module only

	Int_t nCell=seg->getNCells(l);
	for(Int_t i=0;i<nCell;i++)
	{
	    loccal[0]=seg->getSec();
	    Int_t ioseg= seg->getIOSeg();
	    if(ioseg==0){
		(l<6)? loccal[1]=0 : loccal[1]=1;
	    }else if(ioseg==1){
		(l<6)? loccal[1]=2 : loccal[1]=3;
	    }
	    (l<6)? loccal[2]=l : loccal[2]=l-6;
	    loccal[3]=seg->getCell(l,i);

	    calcSegPoints(seg,x1,y1,z1,x2,y2,z2);
	    (*sizescells)[loccal[0]][loccal[1]][loccal[2]].getImpact(x1,y1,z1,x2,y2,z2,loccal[3],alpha,mindist);

	    HMdcCal1* cal1 =(HMdcCal1*)catcal->getObject(loccal);
	    if(cal1!=0)
	    {
		t1  =cal1->getTime1();
		t2  =cal1->getTime2();

		if(t2>-998)
		{   // some times t2 can be missing (-999,-998)
		    // calculating mean value

		    if(useShifts){corr=getShiftSegment(loccal[0],loccal[1],alpha,mindist);}
		    if(nWire<49)
		    {
			measurements[nWire]=t2-t1 + corr;
			nWire++;
		    }else{
			Warning("fillingInput()","Number of wires in Segment > 50! Skipped!");
		    }
		} else { ctskipmod0++;ctskipmod1++;}
	    }else{
		Warning("calcDeDx()","ZERO pointer recieved for cal1 object!");
	    }
	}
    }
    return nWire;
}
UChar_t HMdcDeDx::fillingInputClusFit(HMdcSeg* seg,Float_t* measurements)
{
    // Fills array of measurements from HMdcSeg and returns the number of fired wires in Segment
    // The single measurements are normalized by impact angle and distance from wire. Only the first
    // 50 wires in one segment are accepted. The input is filled via the HMdcClusFit container,
    // which contains all wires in a Segment before the Segment fitter starts. The wire objects
    // are stored in HMdcWireFit objects. Wires thrown out by the fitter will have weight==0.
    // These wires will not be filled into the output HMdcSeg. In this method all wires will be used
    // to calculated the dEdx as difference to fillingInput(HMdcSeg* seg,Float_t* measurements).
    // As this method uses data container of mdctrackD which are not created by default, the user
    // has to make shure this containers exists and are filled.
    // Corresponds to method 1 in calcDeDx(......, version)

    ctskipmod0=0;
    ctskipmod1=0;

    Int_t s,m,l,c;
    Float_t t1,t2;
    Float_t alpha,mindist,weight;

    Int_t nWire       =0;
    Float_t corr      =0.;

    Int_t found       = 0;
    Int_t hitindex    =-1;
    Int_t clusinfindex=-1;
    Int_t clusfitindex=-1;
    Int_t first       = 0;
    Int_t last        = 0;
    //----------------------------------------getting input--------------------------------------------------
    for(Int_t i=0;i<2;i++)
    {   // loop over hit in Mdc 1 and 2

	if(module==0&&i==0) continue;  // fill for 1st module only
        if(module==1&&i==1) continue;  // fill for 2st module only

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
                        weight =wirefit->getWeight();

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

				if(useShifts){corr=getShiftSegment(s,m,alpha,mindist);}
                                if(nWire<49)
                                {
					measurements[nWire]=t2-t1 + corr;
					nWire++;
                                }else{
                                    Warning("fillingInput()","Number of wires in Segment > 50! Skipped!");
                                }
                            } else { ctskipmod0++;ctskipmod1++;}
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




Float_t HMdcDeDx::calcSigma(Float_t* measurements,UChar_t nWire,Float_t mean)
{
    // returns sigma from the float array of nWire measurements with a mean value mean.
    // Values equal to -99 are ignored.

    Float_t sum  =0.;
    Float_t sigma=-1;
    Int_t ct=0;
    for(Int_t j=0;j<nWire;j++)
    {
	if(measurements[j]!=-99)
	{
	    sum = sum + (measurements[j]-mean)*(measurements[j]-mean);
            ct++;
	}
    }
    if(ct>1)
    {
	sigma = sqrt(sum / (ct-1));
    }
    return sigma;
}
Float_t HMdcDeDx::calcMean(Float_t* measurements,UChar_t nWire)
{
    // returns the mean value from the float array of nWire measurements.
    // Values equal to -99 are ignored.

    Float_t sum=0.;
    Float_t mean=-99.;
    Int_t ct=0;

    if(nWire>0)
    {
	for(Int_t j=0;j<nWire;j++)
	{
	    if(measurements[j]!=-99)
	    {
		sum = sum + measurements[j];
		ct++;
	    }
	}
	mean = sum / ct;
    }
    return mean;
}
UChar_t HMdcDeDx::select(Float_t* measurements,Float_t mean,Float_t sigma,UChar_t nWire)
{
    // loops over the measurement array with nWire values and puts values to
    // -99 if the measurements are outside the wanted window arround the mean value mean
    // (set with setWindow() (sigma units)).
    // The procedure keeps at least a number of minimum wires set with setMinimumWires().

    UChar_t nWTrunc=nWire;
    UChar_t count=0;
    UChar_t minW=(UChar_t)getMinimumWires();

    if(nWTrunc<minW)
    {
	return nWTrunc;
    }

    while(nWTrunc>minW && (2*count)<=nWire)
    {
        // starting from lowest val
        if(fabs(measurements[count]-mean)>(getWindow()*sigma))
        { // if measurement outside the window
            if(nWTrunc>minW && nWTrunc>0 )
            { // do not throw away to many measurements
                measurements[count]=-99;
                nWTrunc--;
	    }
	    else
	    {
              return nWTrunc;
	    }
        }
        // taking highest val next
        if( 2*count <=nWire )
        {
            if(fabs(measurements[nWire-count]-mean)>(getWindow()*sigma))
            { // if measurement outside the window
                if(nWTrunc>minW && nWTrunc>0)
                { // do not throw away to many measurements
                    measurements[nWire-count]=-99;
                    nWTrunc--;
		}
		else
		{
		    return nWTrunc;
		}
            }
            count++;
        }
    }
    return  nWTrunc;
}
void  HMdcDeDx::calcSegPoints(HMdcSeg * seg,Double_t& x1, Double_t& y1, Double_t& z1, Double_t& x2, Double_t& y2, Double_t& z2)
{
    // calculates 2 coordinate points from segment

    Double_t teta=seg->getTheta();
    Double_t phi =seg->getPhi();
    Double_t z   =seg->getZ();
    Double_t r   =seg->getR();
    Double_t pi2 =acos(-1.)/2.;

    Double_t X=r*cos(phi+pi2);
    Double_t Y=r*sin(phi+pi2);
    Double_t Z=z;

    x1=X;
    y1=Y;
    z1=Z;
    x2=X+cos(phi)*sin(teta);
    y2=Y+sin(phi)*sin(teta);
    z2=Z+cos(teta);

}

//----------------------dEdx-----------------------------------------------

Double_t HMdcDeDx::energyLoss(Int_t id,Double_t p,Double_t hefr)
{
    // Calculates the dEdx (MeV 1/g cm2) of an particle with GEANT ID id
    // and momentum p (MeV) for He/i-butan gas mixture with He fraction hefr
    // (He (hefr) / i-butan (1-hefr)). The fomular is taken from PDG and doesn't
    // include the density correction term. The values for the mean excitation
    // energy are taken from Sauli.

    if(p==0)             return -1;
    if(hefr<0.||hefr>1.) return -1;
    Double_t mass   =HPidPhysicsConstants::mass(id);
    if(mass==0) return -1;

    // Z and A
    Double_t Z_gas  =2.*hefr+(1.-hefr)*34.;
    Double_t A_gas  =4.*hefr+(1.-hefr)*58.;
    // I_0 and I
    Double_t I_0_gas=24.6*hefr+(1.-hefr)*10.8;
    Double_t I2     =pow(I_0_gas*Z_gas*(1.e-6),2); // sauli
    //Double_t I2     =pow(16.*pow(Z_gas,0.9),2); //C.Lippmann thesis


    Double_t K      =0.307075; // MeV cm2 PDG, 4*pi*N_{A}*r_{e}2*m_{e}2*c2
    Double_t mass2  =pow(mass,2);
    Double_t m_ec2  =HPidPhysicsConstants::mass(3);
    Double_t z2     =pow((Float_t)HPidPhysicsConstants::charge(id),2);
    Double_t p2     =pow(p,2);
    Double_t beta2  =1./((mass2/p2)+1.);
    Double_t gamma2 =1./(1-beta2);
    Double_t gamma  =sqrt(gamma2);

    Double_t Tmax   =(2.*m_ec2*beta2*gamma2)/(1.+ 2.*gamma*(m_ec2/mass)+pow((m_ec2/mass),2));
    Double_t term1  =K*z2*(Z_gas/A_gas)*(1./beta2);
    Double_t term2  =((2.*m_ec2*beta2*gamma2*Tmax)/I2);
    Double_t dedx   =term1 * (0.5*log(term2)-beta2);

    return dedx;
}
TGraph* HMdcDeDx::energyLossGraph(Int_t id,Double_t hefr,TString opt,Bool_t exchange,Int_t markerstyle,Int_t markercolor,Float_t markersize)
{
    // creates a TGraph for particle with GEANT ID id and
    // and He/i-butan gas mixture with He fraction hefr
    // (He (hefr) / i-butan (1-hefr) dEdx vs p,beta,1/beta2 or betagamma
    // depending on opt (p,beta,1/beta2,betagamma). exchange=kTRUE
    // will exchange x-axis and y-axis.


    Double_t pmin=50;
    Double_t pmax=100000.;
    Int_t    np  =100;

    Double_t ptot=0;
    Double_t xarray[np];
    Double_t yarray[np];

    Int_t    vers=0;
    Double_t mass=HPidPhysicsConstants::mass(id);


    if(opt.CompareTo("p"             )==0)vers=0;
    else if(opt.CompareTo("beta"     )==0)vers=1;
    else if(opt.CompareTo("1/beta2"  )==0)vers=2;
    else if(opt.CompareTo("betagamma")==0)vers=3;
    else {cout<<"HMdcDedx::calcDeDxGraph():unknow option!"<<endl;}

    for(Int_t i=1;i<=np;i++)
    {
      ptot=pmin*pow((pmax/pmin),(i-1)/(Double_t)(np-1.));
      if(vers==0){xarray[i-1]=ptot;}
      if(vers==1){xarray[i-1]=sqrt(1./((pow(mass,2)/pow(ptot,2))+1.));}
      if(vers==2){xarray[i-1]=((pow(mass,2)/pow(ptot,2))+1.);}
      if(vers==3){xarray[i-1]=(ptot/mass);}
      yarray[i-1]=HMdcDeDx::energyLoss(id,ptot,hefr);
    }
    Char_t name[300];
    sprintf(name,"dedx_%s_He_%5.1f_ibutane_%5.1f",HPidPhysicsConstants::pid(id),hefr*100,(1-hefr)*100);
    TGraph* g=0;
    if(!exchange)g=new TGraph(np,xarray,yarray);
    else         g=new TGraph(np,yarray,xarray);
    g->SetName(name);
    g->SetMarkerStyle(markerstyle);
    g->SetMarkerSize (markersize);
    g->SetMarkerColor(markercolor);

    return g;
}
Double_t HMdcDeDx::scaledTimeAboveThreshold(HGeantKine* kine,Double_t p,Int_t s,Int_t m,Float_t alpha,Float_t mindist)
{

    Int_t pTr=-1,pID=0;
    kine->getParticle(pTr,pID);
    Float_t corr=getShiftSegment(s,m,alpha,mindist);

    if(pID==2||pID==3)
    {   // electrons and positrons
        return getRefEnergyLoss()- corr;
    }

    Double_t mass  =HPidPhysicsConstants::mass  (pID);
    Int_t    charge=HPidPhysicsConstants::charge(pID);

    if(charge==0||mass==0)
    {
        Warning("HMdcDeDx::scaledEnergyLoss()","id %i %s mass %7.5f charge %i p %7.5f skipped!"
                ,pID,HPidPhysicsConstants::pid(pID),mass,charge,p);
        return -99;
    }

    /*
    if(abs(charge)>1)
    {
        Warning("HMdcDigitizer::fillArrays()","Charge larger 1 not supported : id %i %s mass %7.5f charge %i p %7.5f"
                 ,pID,HPidPhysicsConstants::pid(pID),mass,charge,p,sec,mod,l,c);

    }*/
    Double_t bg       = (p/mass);
    Double_t bgsh     = bg+getShiftEnergyLoss(s,m);
    Double_t psh      = bgsh*HPidPhysicsConstants::mass(14);
    Double_t dedx     = energyLoss(14,psh,getHeFraction());
    Double_t dedxsc   = dedx*getScaleEnergyLoss(s,m);
    Double_t dedxcorr = dedxsc-corr;
    /*
    cout<<pID<<" p "<<p<<" psh "<<psh<<" bg "<<bg<<" bgsh "<<bgsh<<" dedx "
        <<dedx<<" dedxsc "<<dedxsc<<" dedxcorr "<<dedxcorr<<endl;
    */
    return dedxcorr;
}

