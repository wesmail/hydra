//*-- AUTHOR : Jochen Markert

///////////////////////////////////////////////////////////////////////////////
//
// HMdcDeDxHists
//
// Fills  momentum vs mdcdedx (spline or kicktrack) for
// full tracks (seg1+seg2), seg1, seg2 , system 0/1 and sector wise.
// OPTIONS:
// Momentum version:-----------------------------------------------------------
// The histograms can be filled by HKicktrack or HSplineTrack:
// The selection can be done with
// setUseSpline(Bool_t)      :  kTRUE (default) - Fills from HSplineTrack
//                           :  kFALSE          - Fills from HKickTrack
// dEdx calculation:-----------------------------------------------------------
// The mode of HMdcDeDx::calcDeDx(......., mode,module) can be switched
// via setDeDxMode(Int_t m)  :  0 (default) - Fills from HMdcSeg (only cells kept by the fitter)
//                              1           - Fills from HMdcClusFit (cells with weight 0 AND 1 are used)
// via setDeDxModule(Int_t m):  0           - Fills for 1st module in HMdcSeg
//                              1           - Fills for 2st module in HMdcSeg
//                              2 (default) - Fills for both modules in HMdcSeg
//
// via setDeDxWindow(Float_t w) the window of the truncated mena can be set
// in units of sigma (default 5.0);
// via setDeDxMinimumWires(Int_t n) the minimal number of wires after the truncated
// mean procedure  can be set.
// via setDeDxUseShifts(Bool_t shift) the normalization procedure of the dEdx calculation
// cand be switched on or off (default=ON).
// CPR mode:-------------------------------------------------------------------
// Single wire distribution / ntuple filled for single/doubles of Jaro's CPR condition
// Runs on special pepared input file (Stripped single/doubles with HKickTrack containing
// 1 for singles and 2 for doubles )
// setCPR(Bool_t cpr)        :  kTRUE            run in the special mode
// setCPR(Bool_t cpr)        :  kFALSE (default) run in the normal mode
// DEBUG mode:-----------------------------------------------------------------
// If Debug mode is switched on (default OFF),
//   HMdcTrkCand.getFirstCandInd() =p*charge;
//   HMdcTrkCand.getNCandForSeg1() =single_double (only if cpr, 1=single,2=double);
// The out put dedx of HMdcTrkCand will be refilled with the calculated values.
// setDebug(Bool_t d)
//
///////////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hmdcdedxhists.h"
#include "hmdcdedx.h"
#include "hmdcseg.h"
#include "hmdctrkcand.h"
#include "hkicktrack.h"
#include "hsplinetrack.h"
#include "hmetamatch.h"
#include "hmdcdef.h"
#include "hmdctrackgdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hcategory.h"
#include "kickdef.h"
#include "hmessagemgr.h"
#include <iostream> 
#include <iomanip>
#include <stdlib.h>

ClassImp(HMdcDeDxHists)

HMdcDeDxHists::HMdcDeDxHists(void) {
  // Default constructor .
  initParameters();
}
HMdcDeDxHists::HMdcDeDxHists(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments.
  initParameters();
}

HMdcDeDxHists::~HMdcDeDxHists(void) {
  // destructor deletes the iterator
  if (trkcanditer) delete trkcanditer;
  trkcanditer=0;
  if (kickiter) delete kickiter;
  kickiter=0;
  if (metamatchiter) delete metamatchiter;
  metamatchiter=0;

}
void HMdcDeDxHists::initParameters()
{
    trkcandCat =0;
    segCat     =0;
    trkcanditer=0;
    hasPrinted =kFALSE;
    useSpline  =kTRUE;

    lockick.set(2,0,0);
    locseg.set(3,0,0,0);
    kickCat      =0;
    kickiter     =0;
    splineCat    =0;
    metamatchCat =0;
    metamatchiter=0;
    online=kFALSE;   // no online hists
    ntuple=kFALSE;   // no ntuple
    debug =kFALSE;   // no debugging
    cpr   =kFALSE;   // no single/doubles
    mode  =0;        // fill from segment
    module=2;        // fill for both modules of a segment
    useshifts=kTRUE; // normalize dedx
    minwires=5;      // minimum wires after trunce=ated mean
    window=5.0;      // sigma window arround mean
}
void HMdcDeDxHists::setParContainers() {
    // creates the parameter containers
}
Bool_t HMdcDeDxHists::init(void) {
    // gets the pointers to HMdcTrkC and HMdcSeg categories
    // calls the function setParContainers()
  setParContainers();
  trkcandCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcTrkCand));
  if (!trkcandCat) {
      Error("HMdcDeDxHists:init()","HMdcTrkCand Category not available!");
      exit(1);
  }
  segCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMdcSeg));
  if (!segCat) {
      Error("HMdcDeDxHists:init()","HMdcSeg Category not available!");
      exit(1);
  }

  trkcanditer =(HIterator *)((HCategory*)trkcandCat)->MakeIterator("native");

  if(!useSpline)
  {
      kickCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catKickTrack));
      if(kickCat) {
	  kickiter=(HIterator *)((HCategory*)kickCat)->MakeIterator("native");
      }else{
	  Warning("init()","No catKickTrack in input!");
      }
  }
  if(useSpline)
  {
      metamatchCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catMetaMatch));
      if(metamatchCat) {
	  metamatchiter=(HIterator *)((HCategory*)metamatchCat)->MakeIterator("native");
      }else{
	  Warning("init()","No catMetaMatch in input!");
      }

      splineCat=(HCategory*)(((HEvent*)(gHades->getCurrentEvent()))->getCategory(catSplineTrack));
      if(!splineCat) {
	  Warning("init()","No catSplineTrack in input!");
      }
  }
  makeHists();

  if(!hasPrinted)printStatus();
  fActive=kTRUE;
  return kTRUE;
}
void HMdcDeDxHists::makeHists()
{
    Char_t name [300];
    Char_t title[300];
    Int_t nbinX=200;
    Int_t nbinY=600;
    Float_t xlow=0.;
    Float_t xup=200.;
    Float_t ylow=-1500.;
    Float_t yup =1500.;


    output=new TFile("mdcdedx.root","RECREATE");
    output->cd();
    if(online)
    {
	result=new TCanvas("single_drift_cells","single drift cells",1000,800);
        result->Draw();
    }
    if(ntuple)
    {
	celltuple=new TNtuple("dedx","dedx","type:p:s:phi:theta:seg:ncell:ncelltrunc:dedx:dedxtrunc:sigma:sigmatrunc:c0:c1:c2:c3:c4:c5:c6:c7:c8:c9:c10:c11:c12:c13:c14:c15:c16:c17:c18:c19:c20:c21:c22:c23:c24:c25:c26:c27:c28:c29");
    }
    sprintf(name ,"hsinglecells");
    sprintf(title,"dEdx of single driftcells");
    hdriftcells[0]=new TH1F(name,title,nbinX,xlow,xup);
    hdriftcells[0]->SetXTitle("t2-t1 [ns]");
    hdriftcells[0]->SetLineColor(2);

    sprintf(name ,"hdoublecells");
    sprintf(title,"dEdx of single driftcells");
    hdriftcells[1]=new TH1F(name,title,nbinX,xlow,xup);
    hdriftcells[1]->SetXTitle("t2-t1 [ns]");
    hdriftcells[1]->SetLineColor(4);

    if(online)
    {
	result->cd();
	hdriftcells[0]->Draw();
        hdriftcells[1]->Draw("same");
    }
    sprintf(name,"hdedx_p_all_trk");
    sprintf(title,"dEdx_vs momentum all sectors track");
    hdedx_p_all_trk=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
    hdedx_p_all_trk->SetXTitle("t2-t1 [ns]");
    hdedx_p_all_trk->SetYTitle("momentum * charge [MeV]");
    hdedx_p_all_trk->SetOption("COLZ");


    for(Int_t sys=0;sys<2;sys++){
	sprintf(name,"hdedx_p_all_trk_sys[%i]",sys);
	sprintf(title,"dEdx_vs momentum all sectors track sys %i",sys);
	hdedx_p_all_trk_sys[sys]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
	hdedx_p_all_trk_sys[sys]->SetXTitle("t2-t1 [ns]");
	hdedx_p_all_trk_sys[sys]->SetYTitle("momentum * charge [MeV]");
	hdedx_p_all_trk_sys[sys]->SetOption("COLZ");

	for(Int_t s=0;s<6;s++){
	    sprintf(name,"hdedx_p_sec_trk_sys[%i][%i]",s,sys);
	    sprintf(title,"dEdx_vs momentum sector %i track system %i",s,sys);
	    hdedx_p_sec_trk_sys[s][sys]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
	    hdedx_p_sec_trk_sys[s][sys]->SetXTitle("t2-t1 [ns]");
	    hdedx_p_sec_trk_sys[s][sys]->SetYTitle("momentum * charge [MeV]");
	    hdedx_p_sec_trk_sys[s][sys]->SetOption("COLZ");

	}
    }

    for(Int_t seg=0;seg<2;seg++){
	sprintf(name,"hdedx_p_all_seg[%i]",seg);
	sprintf(title,"dEdx_vs momentum all sectors seg %i",seg);
	hdedx_p_all_seg[seg]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
	hdedx_p_all_seg[seg]->SetXTitle("t2-t1 [ns]");
	hdedx_p_all_seg[seg]->SetYTitle("momentum * charge [MeV]");
	hdedx_p_all_seg[seg]->SetOption("COLZ");

	for(Int_t sys=0;sys<2;sys++){
	    sprintf(name,"hdedx_p_all_seg_sys[%i][%i]",seg,sys);
	    sprintf(title,"dEdx_vs momentum all sectors seg  %i sys %i",seg,sys);
	    hdedx_p_all_seg_sys[seg][sys]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
	    hdedx_p_all_seg_sys[seg][sys]->SetXTitle("t2-t1 [ns]");
	    hdedx_p_all_seg_sys[seg][sys]->SetYTitle("momentum * charge [MeV]");
	    hdedx_p_all_seg_sys[seg][sys]->SetOption("COLZ");
	}
    }


    for(Int_t s=0;s<6;s++){
	sprintf(name,"hdedx_p_sec_trk[%i]",s);
	sprintf(title,"dEdx_vs momentum sector %i track",s);
	hdedx_p_sec_trk[s]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
	hdedx_p_sec_trk[s]->SetXTitle("t2-t1 [ns]");
	hdedx_p_sec_trk[s]->SetYTitle("momentum * charge [MeV]");
	hdedx_p_sec_trk[s]->SetOption("COLZ");

	for(Int_t seg=0;seg<2;seg++){
	    sprintf(name,"hdedx_p_sec_seg[%i][%i]",s,seg);
	    sprintf(title,"dEdx_vs momentum sector %i ioseg %i",s,seg);
	    hdedx_p_sec_seg[s][seg]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
	    hdedx_p_sec_seg[s][seg]->SetXTitle("t2-t1 [ns]");
	    hdedx_p_sec_seg[s][seg]->SetYTitle("momentum * charge [MeV]");
	    hdedx_p_sec_seg[s][seg]->SetOption("COLZ");

	    for(Int_t sys=0;sys<2;sys++){
		sprintf(name,"hdedx_p_sec_seg_sys[%i][%i][%i]",s,seg,sys);
		sprintf(title,"dEdx_vs momentum sector %i ioseg %i system %i",s,seg,sys);
		hdedx_p_sec_seg_sys[s][seg][sys]=new TH2F(name,title,nbinX,xlow,xup,nbinY,ylow,yup);
		hdedx_p_sec_seg_sys[s][seg][sys]->SetXTitle("t2-t1 [ns]");
		hdedx_p_sec_seg_sys[s][seg][sys]->SetYTitle("momentum * charge [MeV]");
		hdedx_p_sec_seg_sys[s][seg][sys]->SetOption("COLZ");
	    }
	}
    }
}
void HMdcDeDxHists::printStatus()
{
    // prints the parameters to the screen
    SEPERATOR_msg("-",60);
    INFO_msg(10,DET_MDC,"HMdcDeDxHistsSetup:");
    /*
    gHades->getMsg()->info(10,DET_MDC,GetName()
			   ,"ModeFlagCal1    =  %i :  1 = NoStartandCal, 2 = StartandCal, 3 = NoStartandNoCal",ModeFlagCal1);
    */
    SEPERATOR_msg("-",60);
    hasPrinted=kTRUE;
}
Int_t HMdcDeDxHists::execute(void)
{

    // calculates dEdx for all Segments contained in HMdcTrkCand.
    // The result is filled to HMdctrk.
    HMdcDeDx* mdcdedx=(HMdcDeDx*)gHades->getRuntimeDb()->getContainer("MdcDeDx");
    mdcdedx->setMinimumWires(minwires);
    mdcdedx->setWindow      (window);
    mdcdedx->setUseShifts   (useshifts);

    Float_t meanold1,sigmaold1,sigma1;
    UChar_t nwire1,nwiretrunc1;
    Float_t meanold2,sigmaold2,sigma2;
    UChar_t nwire2,nwiretrunc2;

    Float_t* array1;
    Float_t* array2;

    HMdcTrkCand* trkcand;
    Float_t dedx1=0;
    Float_t dedx2=0;
    Int_t indseg1,indseg2;
    Int_t nmatched;

    Float_t*   array=0;
    Int_t size;
    Float_t myvalues[42];
    Int_t sizeval=42;
    Int_t firstval=12;
    Int_t sizetuple=30;

    HMdcSeg* seg1=0;
    HMdcSeg*seg2=0;
    Float_t phi1,theta1,phi2,theta2;

    if(!useSpline)
    {
	trkcanditer->Reset();
	while ((trkcand=(HMdcTrkCand *)trkcanditer->Next())!=0)
	{
	    indseg1=trkcand->getSeg1Ind();
	    indseg2=trkcand->getSeg2Ind();

	    array1=0;
	    array2=0;

	    dedx1      =dedx2      =-1;
	    meanold1   =meanold2   =-1;
	    sigmaold1  =sigmaold2  =-1;
	    sigma1     =sigma2     =-1;
	    nwiretrunc1=nwiretrunc2=0;
            nwire1     =nwire2     =0;

	    seg1=(HMdcSeg*)segCat->getObject(indseg1);
	    dedx1=mdcdedx->calcDeDx(seg1,&meanold1,&sigmaold1,&nwire1,&sigma1,&nwiretrunc1,mode,module);
 	    array=mdcdedx->getArray(size);
 	    array1= new (Float_t) [size];
	    for(Int_t i=0;i<size;i++){array1[i]=array[i];}

	    if(indseg2>-0.9)
	    {
		seg2=(HMdcSeg*)segCat->getObject(indseg2);
		dedx2=mdcdedx->calcDeDx(seg2,&meanold2,&sigmaold2,&nwire2,&sigma2,&nwiretrunc2,mode,module);

		array=mdcdedx->getArray(size);
		array2= new (Float_t) [size];
		for(Int_t i=0;i<size;i++){array2[i]=array[i];}
	    }

	    nmatched=trkcand->getNCandForSeg1();
	    if(nmatched>=0)
	    {
		HKickTrack* kick;
		Int_t segindex;
		//------------------------looping over kicktrack to get segment
		kickiter->Reset();
		while ((kick=(HKickTrack *)kickiter->Next())!=0)
		{
		    locseg[0]=kick->getSector();
		    locseg[1]=0;
		    locseg[2]=kick->getSegment();

		    Int_t s       =kick->getSector();
		    Int_t sys     =kick->getSystem();
		    //Int_t pid     =kick->getPID();
		    //Int_t quality =kick->getQuality();
		    Float_t p     =kick->getP();
		    Int_t charge  =kick->getCharge();
		    Int_t single_double=kick->getRingId();
                    
		    segindex=segCat->getIndex(locseg);

		    if(indseg1==segindex)
		    {
			phi1  =seg1->getPhi();
			theta1=seg1->getTheta();

			for(Int_t i=0;i<sizeval;i++){myvalues[i]=-99;}

			// analize first array1 for inner segment
			for(Int_t i=0;i<size;i++)
			{
			    Float_t val=array1[i];
			    if(val>0)
			    {
				if(cpr)hdriftcells[single_double-1]->Fill(val);
				else   hdriftcells[0]              ->Fill(val);
				if(i<sizetuple)
				{
				    myvalues[firstval+i]=val;
				}
			    }

			}

                            // filling ntuple for array1
			if(ntuple)
			{
			    //type:p:s:phi:theta:seg:ncell:ncelltrunc:dedx:dedxtrunc:sigma:sigmatrunc
			    if(cpr)myvalues[0]=single_double;
			    else   myvalues[0]=1;
			    myvalues[1]=p*charge;        
                            myvalues[2]=s;
			    myvalues[3]=phi1;
			    myvalues[4]=theta1;
			    myvalues[5]=0;
			    myvalues[6]=nwire1;
			    myvalues[7]=nwiretrunc1;
			    myvalues[8]=meanold1;
			    myvalues[9]=dedx1;
			    myvalues[10]=sigmaold1;
			    myvalues[11]=sigma1;

			    celltuple->Fill(myvalues);
			}

			if(!cpr&&indseg2!=-1)
			{   // only if no CPR investigation

			    phi2  =seg2->getPhi();
			    theta2=seg2->getTheta();

			    // reset working array for ntuple
			    for(Int_t i=0;i<sizeval;i++){myvalues[i]=-99;}

			    // analize array2 for outer segment
			    for(Int_t i=0;i<size;i++)
			    {
				Float_t val=array2[i];
				if(val>0)
				{
				    hdriftcells[1]->Fill(val);
				    if(i<sizetuple)
				    {
					myvalues[firstval+i]=val;
				    }
				}
			    }

			    // filling ntuple for array2
			    if(ntuple)
			    {
				myvalues[0]=2;
				myvalues[1]=p*charge;
				myvalues[2]=s;
				myvalues[3]=phi2;
				myvalues[4]=theta2;
				myvalues[5]=1;
				myvalues[6]=nwire2;
				myvalues[7]=nwiretrunc2;
				myvalues[8]=meanold2;
				myvalues[9]=dedx2;
				myvalues[10]=sigmaold2;
				myvalues[11]=sigma2;

				celltuple->Fill(myvalues);
			    }
			}
			//  draw online pictures
			if(online&&(gHades->getEventCounter()%100==0))
			{
			    result->Modified();
			    result->Update();
			}

			// write p*charge and single_double flag
			// to HMdcTrkCand for easier investigation
			if(debug)
			{   // not standard info will be over written!!!!!!
			    trkcand->setdedxInnerSeg(dedx1);
			    trkcand->setdedxOuterSeg(dedx2);
			    trkcand->setSigmadedxInnerSeg(sigma1);
			    trkcand->setSigmadedxOuterSeg(sigma2);
			    trkcand->setNWirededxInnerSeg(nwire1);
			    trkcand->setNWirededxOuterSeg(nwire2);
			    trkcand->setNWireCutdedxInnerSeg(nwiretrunc1);
			    trkcand->setNWireCutdedxOuterSeg(nwiretrunc2);
			    trkcand->setNCandForSeg1((Short_t)single_double);
                            trkcand->setFirstCandInd((Short_t)(p*charge));

			}

                        // filling hists
			hdedx_p_sec_seg_sys[s][0][sys]->Fill(dedx1,p*charge);
			hdedx_p_sec_seg    [s][0]     ->Fill(dedx1,p*charge);;
			hdedx_p_all_seg_sys[0][sys]   ->Fill(dedx1,p*charge);
			hdedx_p_all_seg    [0]        ->Fill(dedx1,p*charge);


			if(indseg2>-0.9)
			{
			    hdedx_p_sec_seg_sys[s][1][sys]->Fill(dedx2,p*charge);
			    hdedx_p_sec_seg    [s][1]     ->Fill(dedx2,p*charge);;
			    hdedx_p_sec_trk_sys[s][sys]   ->Fill((dedx1+dedx2)*0.5,p*charge);
			    hdedx_p_sec_trk    [s]        ->Fill((dedx1+dedx2)*0.5,p*charge);

			    hdedx_p_all_seg_sys[1][sys]   ->Fill(dedx2,p*charge);
			    hdedx_p_all_seg    [1]        ->Fill(dedx2,p*charge);
			    hdedx_p_all_trk_sys[sys]      ->Fill((dedx1+dedx2)*0.5,p*charge);
			    hdedx_p_all_trk               ->Fill((dedx1+dedx2)*0.5,p*charge);
			}

		    }
		}
		//-------------------------------------------------------------------
	    }
	    if(array1)delete [] array1;
	    if(array2)delete [] array2;
	}
    }

    if(useSpline)
    {

	HMetaMatch* metamatch;
	HSplineTrack* splinetrack;
	Int_t trkindex;
	Int_t splineindex;

	//------------------------looping over metamatch to get trkcand and spline track
	metamatchiter->Reset();
	while ((metamatch=(HMetaMatch *)metamatchiter->Next())!=0)
	{
	    trkindex      =metamatch->getTrkCandInd();
	    trkcand       =(HMdcTrkCand*)trkcandCat->getObject(trkindex);
	    nmatched      =trkcand->getNCandForSeg1();
	    if(nmatched==1)
	    {   // if only one candidate for inner seg
		splineindex=metamatch->getSplineInd();
		if(splineindex!=-1)
		{   // if spline track exists
		    splinetrack=(HSplineTrack*)splineCat->getObject(splineindex);

		    Int_t s       =splinetrack->getSector();
		    Int_t sys     =splinetrack->getSystem();
		    Float_t p     =splinetrack->getP();
		    Int_t charge  =splinetrack->getPolarity();
		    //Float_t pull  =splinetrack->getPull();

		    if(sys!=-1)
		    {   // if meta hit exists

			indseg1=trkcand->getSeg1Ind();
			indseg2=trkcand->getSeg2Ind();

			array1=0;
			array2=0;

			dedx1      =dedx2      =-1;
			meanold1   =meanold2   =-1;
			sigmaold1  =sigmaold2  =-1;
			sigma1     =sigma2     =-1;
			nwiretrunc1=nwiretrunc2=0;
			nwire1     =nwire2     =0;

			seg1=(HMdcSeg*)segCat->getObject(indseg1);
			dedx1=mdcdedx->calcDeDx(seg1,&meanold1,&sigmaold1,&nwire1,&sigma1,&nwiretrunc1,mode,module);
			array=mdcdedx->getArray(size);
			array1= new (Float_t) [size];
			for(Int_t i=0;i<size;i++){array1[i]=array[i];}

			if(indseg2>-0.9)
			{
			    seg2=(HMdcSeg*)segCat->getObject(indseg2);
			    dedx2=mdcdedx->calcDeDx(seg2,&meanold2,&sigmaold2,&nwire2,&sigma2,&nwiretrunc2,mode,module);

			    array=mdcdedx->getArray(size);
			    array2= new (Float_t) [size];
			    for(Int_t i=0;i<size;i++){array2[i]=array[i];}
			}

			phi1  =seg1->getPhi();
			theta1=seg1->getTheta();

			for(Int_t i=0;i<sizeval;i++){myvalues[i]=-99;}

			// analize first array1 for inner segment
			for(Int_t i=0;i<size;i++)
			{
			    Float_t val=array1[i];
			    if(val>0)
			    {
				hdriftcells[0] ->Fill(val);
				if(i<sizetuple)
				{
				    myvalues[firstval+i]=val;
				}
			    }
			}

			// filling ntuple for array1
			if(ntuple)
			{
			    myvalues[0]=1;
			    myvalues[1]=p*charge;
			    myvalues[2]=s;
			    myvalues[3]=phi1;
			    myvalues[4]=theta1;
			    myvalues[5]=0;
			    myvalues[6]=nwire1;
			    myvalues[7]=nwiretrunc1;
			    myvalues[8]=meanold1;
			    myvalues[9]=dedx1;
			    myvalues[10]=sigmaold1;
			    myvalues[11]=sigma1;

			    celltuple->Fill(myvalues);
			}

			if(indseg2!=-1)
			{
			    phi2  =seg2->getPhi();
			    theta2=seg2->getTheta();
			    // reset working array for ntuple
			    for(Int_t i=0;i<sizeval;i++){myvalues[i]=-99;}

			    // analize array2 for outer segment
			    for(Int_t i=0;i<size;i++)
			    {
				Float_t val=array2[i];
				if(val>0)
				{
				    hdriftcells[1]->Fill(val);
				    if(i<sizetuple)
				    {
					myvalues[firstval+i]=val;
				    }
				}
			    }

			    // filling ntuple for array2
			    if(ntuple)
			    {
				myvalues[0]=2;
				myvalues[1]=p*charge;
				myvalues[2]=s;
				myvalues[3]=phi2;
				myvalues[4]=theta2;
				myvalues[5]=1;
				myvalues[6]=nwire2;
				myvalues[7]=nwiretrunc2;
				myvalues[8]=meanold2;
				myvalues[9]=dedx2;
				myvalues[10]=sigmaold2;
				myvalues[11]=sigma2;

				celltuple->Fill(myvalues);
			    }
			}
			//  draw online pictures
			if(online&&(gHades->getEventCounter()%100==0))
			{
			    result->Modified();
			    result->Update();
			}

			// write p*charge and single_double flag
			// to HMdcTrkCand for easier investigation
			if(debug)
			{   // not standard info will be over written!!!!!!
			    trkcand->setdedxInnerSeg(dedx1);
			    trkcand->setdedxOuterSeg(dedx2);
			    trkcand->setSigmadedxInnerSeg(sigma1);
			    trkcand->setSigmadedxOuterSeg(sigma2);
			    trkcand->setNWirededxInnerSeg(nwire1);
			    trkcand->setNWirededxOuterSeg(nwire2);
			    trkcand->setNWireCutdedxInnerSeg(nwiretrunc1);
			    trkcand->setNWireCutdedxOuterSeg(nwiretrunc2);
                            trkcand->setFirstCandInd((Short_t)(p*charge));
			}

			hdedx_p_sec_seg_sys[s][0][sys]->Fill(dedx1,p*charge);
			hdedx_p_sec_seg    [s][0]     ->Fill(dedx1,p*charge);;
			hdedx_p_all_seg_sys[0][sys]   ->Fill(dedx1,p*charge);
			hdedx_p_all_seg    [0]        ->Fill(dedx1,p*charge);

			if(indseg2>-0.9)
			{
			    hdedx_p_sec_seg_sys[s][1][sys]->Fill(dedx2,p*charge);
			    hdedx_p_sec_seg    [s][1]     ->Fill(dedx2,p*charge);;
			    hdedx_p_sec_trk_sys[s][sys]   ->Fill((dedx1+dedx2)*0.5,p*charge);
			    hdedx_p_sec_trk    [s]        ->Fill((dedx1+dedx2)*0.5,p*charge);

			    hdedx_p_all_seg_sys[1][sys]   ->Fill(dedx2,p*charge);
			    hdedx_p_all_seg    [1]        ->Fill(dedx2,p*charge);
			    hdedx_p_all_trk_sys[sys]      ->Fill((dedx1+dedx2)*0.5,p*charge);
			    hdedx_p_all_trk               ->Fill((dedx1+dedx2)*0.5,p*charge);
			}

			if(array1)delete [] array1;
			if(array2)delete [] array2;
		    }
		}
	    }
	}
    }

    return 0;
}
Bool_t HMdcDeDxHists::finalize()
{
    output->cd();

    if(ntuple)celltuple->Write();

    hdriftcells[0]->Write();
    hdriftcells[1]->Write();

    hdedx_p_all_trk->Write();
    for(Int_t sys=0;sys<2;sys++){
	hdedx_p_all_trk_sys[sys]->Write();

	for(Int_t s=0;s<6;s++){
	    hdedx_p_sec_trk_sys[s][sys]->Write();
	}
    }

    for(Int_t seg=0;seg<2;seg++){
	hdedx_p_all_seg[seg]->Write();

	for(Int_t sys=0;sys<2;sys++){
	    hdedx_p_all_seg_sys[seg][sys]->Write();
	}
    }


    for(Int_t s=0;s<6;s++){
	hdedx_p_sec_trk[s]->Write();

	for(Int_t seg=0;seg<2;seg++){
	    hdedx_p_sec_seg[s][seg]->Write();

	    for(Int_t sys=0;sys<2;sys++){
		hdedx_p_sec_seg_sys[s][seg][sys]->Write();
	    }
	}
    }
    output->Save();
    output->Close();

    return kTRUE;
}
