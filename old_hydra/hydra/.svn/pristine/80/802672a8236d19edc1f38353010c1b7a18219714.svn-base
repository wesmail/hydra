using namespace std;
//*-- Author : Stefano Spataro (spataro#lns.infn.it)
//*-- Modified : 10-DEC-2004 by Stefano Spataro (RungeKutta tracking included, added common mode)
//*-- Modified : 17-FEB-2005 by Stefano Spataro (removed metamatch->getTofClusterSize()!=2 condition (inside isSplineAccepted now)
//*-- Modified : 22-FEB-2005 by Stefano Spataro (bug in "nochicut" flag (segmentation violation) solved. It didn't work before)
//*-- Modified : 04-JUL-2005 by Stefano Spataro (new chi2 cut flag, new output file options)

// ***************************************************************************
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
// HTofRec
// In case of experiments where there exists no start hit and the TOF of the
// particles has to be recalculated, the tof variable(getTof()) of all classes derived from
// HBaseTrack and HKickTrack will be overwritten by a this class which recalculates the tof.
// To mark those objects as changed one can ask with getFlag(). The result should be
// 1 or 2 depending on the method which has been used and 0 if it has not been changed.
// Anyhow the original tof can be retrieved direct from the HTofHit referenced by
// tofindex (getTofHitInd()). The flag itself is decoded in the tofindex, but will be
// hidden from the user by the getTofHitInd() function.
//
// To setup the constructor  call HTofRec(Text_t *name,Text_t *title, const char *select, const char *track),
// where select contains the key words for configuration (just "," separated list)
//    nospline   : switch of spline
//    nohires    : switch of high resolution kickplane
//    nolowres   : switch of low  resolution kickplane
//    nolowresB  : switch of low  resolution kickplaneB
//    nork       : switch of Runge Kutta tracking
//    norich     : switch of rich mode
//    debug      : switch on debug mode
//    ntuple     : switch on create,fill and write ntuple for debugging
//    chiin      : switch on the chi2>-1 cut for inner MDC segments
//    chiout     : switch on the chi2>-1 cut for outer MDC segments
//    common     : use spline as reconstructor for all the trackings
//    skip       : skip event in DST if start is not reconstructed
//    simulation : swith on simulation mode
//
// if common mode is switched one, "track" is the chosen tracking algorythm for globat tof reconstruction:
//    spline     : spline
//    lowres     : low resolution kickplaneB
//    hires      : high resolution kickplane
//    rk         : Runge Kutta tracking
//
// defaults (empty string) are:
//   spline on
//   low  resolution kickplane   ON
//   low  resolution kickplaneB  ON
//   high resolution kickplane   ON
//   RungeKutta                  ON
//   fRichMode                   ON
//   field correction            1.0
//   debug mode                  OFF
//   ntuple                      OFF
//   chi2 inner cut              OFF
//   chi2 outer cut              OFF
//   simulation mode             OFF
//   skip event                  OFF
//   common                      OFF
//
// Usage:
//
//   TofRec *tofrec = new HTofRec("tofreconstructor","tofreconstructor",option,track);
//   tofrec->setRichParams(0.674189,2.04904,0.,1.82966,5.);
//   tofrec->setQualityRichParams(200.,4.,2.8,5);
//   HTaskSet * tofrectask = new HTaskSet("","");
//   tofrectask->connect(tofrec);
//   masterTaskSet->add(tofrec);
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************


#include "htofrec.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hdebug.h"
#include "tofdef.h"
#include "htofhit.h"
#include "hevent.h"
#include "heventheader.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include <TMath.h>
#include <TNtuple.h>
#include "hreconstructor.h"
#include "hlocation.h"
#include "hsplinetrack.h"
#include "hbasetrack.h"
#include "hrichhit.h"
#include "richdef.h"
#include "hkicktrack.h"
#include "hkicktrackB.h"
#include "hkicktrack123B.h"
#include "hrktrackB.h"
#include "showertofinodef.h"
#include "hshowerhittoftrack.h"
#include "hmdctrackgdef.h"
#include "kickdef.h"
#include "hmdcseg.h"
#include "hmetamatch.h"
#include "hmdcdef.h"
#include "hmdctrkcand.h"

#include "hpidphysicsconstants.h"


ClassImp(HTofRec)

  HTofRec::HTofRec()
{
  // Default constructor

  C=TMath::C()*1e-6;
    
  rich_params[0]  = 0.; rich_params[1]  = 1.; rich_params[2]  = 0.;    rich_params[3]  = 1.; rich_params[4] = 0.;
  rich_qparams[0] = 0.; rich_qparams[1] = 0.; rich_qparams[2] = 1000.; rich_qparams[3] = 0.;
  spline_sec_on[0]    = 1;  spline_sec_on[1]    = 1;  spline_sec_on[2]    = 1;  spline_sec_on[3]    = 1;  spline_sec_on[4]    = 1;  spline_sec_on[5]    = 1;
  spline_sec_field[0] = 1.; spline_sec_field[1] = 1.; spline_sec_field[2] = 1.; spline_sec_field[3] = 1.; spline_sec_field[4] = 1.; spline_sec_field[5] = 1.;
  kick123_sec_on[0]   = 1;  kick123_sec_on[1]   = 1;  kick123_sec_on[2]   = 1;  kick123_sec_on[3]   = 1;  kick123_sec_on[4]   = 1;  kick123_sec_on[5]   = 1;  
  kick_sec_on[0]      = 1;  kick_sec_on[1]      = 1;  kick_sec_on[2]      = 1;  kick_sec_on[3]      = 1;  kick_sec_on[4]      = 1;  kick_sec_on[5]      = 1;  
  rk_sec_on[0]        = 1;  rk_sec_on[1]        = 1;  rk_sec_on[2]        = 1;  rk_sec_on[3]        = 1;  rk_sec_on[4]        = 1;  rk_sec_on[5]        = 1;
  common_flag = -1;
  
  for (Int_t ss=0; ss<6;ss++) 
    {
      mdc_sec[ss] = 0;
      for (Int_t mm=0; mm<4;mm++)
	if (gHades->getSetup()->getDetector("Mdc")->getModule(ss,mm)) mdc_sec[ss]++;
    }
  
  before   = 0;
  after    = 0;
  richcorr = 0;

  fRichMode        = kTRUE;
  fPlusMode        = kFALSE;
  fMinusMode       = kTRUE;
  fDebugMode       = kFALSE;
  fSpline          = kTRUE;
  fKickLowres      = kTRUE;
  fKickLowresB     = kTRUE;
  fKickHires       = kTRUE;
  fRK              = kTRUE;
  fillNTuple       = kFALSE;
  bChiCutInner     = kFALSE;
  bChiCutOuter     = kFALSE;
  fSkip            = kFALSE;
  isSimulation     = kFALSE;
  isCommon         = kFALSE;
}

HTofRec::HTofRec(Text_t *name,Text_t *title, const char *select, const char *track) : 
  HReconstructor(name,title)
{
  // Constructor
  
  TString sel = select;
  sel.ToLower();
  TString tracking = track;
  tracking.ToLower();

  fRichMode        = kTRUE;
  fPlusMode        = kFALSE;
  fMinusMode       = kTRUE;
  fDebugMode       = kFALSE;
  fSpline          = kTRUE;
  fKickLowres      = kTRUE;
  fKickLowresB     = kTRUE;
  fKickHires       = kTRUE;
  fRK              = kTRUE;
  fillNTuple       = kFALSE;
  bChiCutInner     = kFALSE;
  bChiCutOuter     = kFALSE;
  fSkip            = kFALSE;
  isSimulation     = kFALSE;
  isCommon         = kFALSE;
  
  C=TMath::C()*1e-6;
 
  rich_params[0]  = 0.; rich_params[1]  = 1.; rich_params[2]  = 0.;    rich_params[3]  = 1.; rich_params[4] = 0.;
  rich_qparams[0] = 0.; rich_qparams[1] = 0.; rich_qparams[2] = 1000.; rich_qparams[3] = 0.;
  spline_sec_on[0]    = 1;  spline_sec_on[1]    = 1;  spline_sec_on[2]    = 1;  spline_sec_on[3]    = 1;  spline_sec_on[4]    = 1;  spline_sec_on[5]    = 1;
  spline_sec_field[0] = 1.; spline_sec_field[1] = 1.; spline_sec_field[2] = 1.; spline_sec_field[3] = 1.; spline_sec_field[4] = 1.; spline_sec_field[5] = 1.;
  kick123_sec_on[0]   = 1;  kick123_sec_on[1]   = 1;  kick123_sec_on[2]   = 1;  kick123_sec_on[3]   = 1;  kick123_sec_on[4]   = 1;  kick123_sec_on[5]   = 1;  
  kick_sec_on[0]      = 1;  kick_sec_on[1]      = 1;  kick_sec_on[2]      = 1;  kick_sec_on[3]      = 1;  kick_sec_on[4]      = 1;  kick_sec_on[5]      = 1;  
  rk_sec_on[0]        = 1;  rk_sec_on[1]        = 1;  rk_sec_on[2]        = 1;  rk_sec_on[3]        = 1;  rk_sec_on[4]        = 1;  rk_sec_on[5]        = 1;  
  common_flag = -1;
  
  if (sel.Contains("nospline"))   fSpline          = kFALSE;
  if (sel.Contains("nohires"))    fKickHires       = kFALSE;
  if (sel.Contains("nolowres"))   fKickLowres      = kFALSE;
  if (sel.Contains("nolowresb"))  fKickLowresB     = kFALSE;
  if (sel.Contains("nork"))       fRK              = kFALSE;
  if (sel.Contains("norich"))     fRichMode        = kFALSE;
  if (sel.Contains("debug"))      fDebugMode       = kTRUE;
  if (sel.Contains("ntuple"))     fillNTuple       = kTRUE;
  if (sel.Contains("chiin"))      bChiCutInner     = kTRUE;
  if (sel.Contains("chiout"))     bChiCutOuter     = kTRUE;
  if (sel.Contains("skip"))       fSkip            = kTRUE;
  if (sel.Contains("simulation")) isSimulation     = kTRUE;
  if (sel.Contains("common")) {
    if (tracking.Contains("spline"))  common_flag = 0;
    if (tracking.Contains("lowres"))  common_flag = 1;
    if (tracking.Contains("hires"))   common_flag = 2;
    if (tracking.Contains("rk"))      common_flag = 3;
    isCommon         = kTRUE;
  }
  
  for (Int_t ss=0; ss<6;ss++) 
    {
      mdc_sec[ss] = 0;
      for (Int_t mm=0; mm<4;mm++)
	if (gHades->getSetup()->getDetector("Mdc")->getModule(ss,mm)) mdc_sec[ss]++;
    }
  
  before   = 0;
  after    = 0;
  richcorr = 0;
  
  sDir  = "./";
  sFile = "";
}

HTofRec::~HTofRec(void){
  // Destructor.
}

Bool_t HTofRec::init(void){
  cout << "Tof Reconstructor default initialization *** " << endl;
    
  if (fSpline||fKickHires||fKickLowresB) {
    fMetaMatchCat=gHades->getCurrentEvent()->getCategory(catMetaMatch);
    if (!fMetaMatchCat) {
      Warning("init","No HMetaMatch Input -> Switching spline/hires kick/lowresB kick OFF");
      fSpline = kFALSE;
      fKickLowresB = kFALSE;
      fKickHires = kFALSE;
      fRK = kFALSE;
    }
    else iterMetaMatch= (HIterator *)fMetaMatchCat->MakeIterator();
    
    fMdcTrkCandCat=gHades->getCurrentEvent()->getCategory(catMdcTrkCand);
    if (!fMdcTrkCandCat) {
      Warning("init","No HMdcTrkCand Input -> Switching spline/hires kick/lowresB kick OFF");
      fSpline = kFALSE;
      fKickLowresB = kFALSE;
      fKickHires = kFALSE;   
      fRK = kFALSE;
    }
    
    fMdcSegCat=gHades->getCurrentEvent()->getCategory(catMdcSeg);
    if (!fMdcSegCat) 
      Error("init","No HMdcSeg input -> Switching spline OFF");
      
    if (fSpline) {
      fSplineCat=gHades->getCurrentEvent()->getCategory(catSplineTrack);
      if (!fSplineCat) {
	Warning("init","No Spline input -> Switching spline OFF");
	fSpline = kFALSE;
	if (isCommon&&common_flag==0) isCommon = kFALSE;
      }
    }
    
    if (fKickLowresB) {
      fKickLowBCat= gHades->getCurrentEvent()->getCategory(catKickTrackB);
      if (!fKickLowBCat) {
	Warning("init","No lowres KickB input -> Switching lowres KickB OFF");
	fKickLowresB = kFALSE;
	if (isCommon&&common_flag==1) isCommon = kFALSE;
      }
    }
    
    if (fKickHires) {
      fKickHiCat= gHades->getCurrentEvent()->getCategory(catKickTrack123B);
      if (!fKickHiCat) {
	Warning("init","No hires Kick input -> Switching hires Kick OFF");
	fKickHires = kFALSE;
	if (isCommon&&common_flag==2) isCommon = kFALSE;
      }
    }
    
    if (fRK) {
      fRKCat= gHades->getCurrentEvent()->getCategory(catRKTrackB);
      if (!fRKCat) {
	Warning("init","No RungeKutta input -> Switching RungeKutta OFF");
	fRK = kFALSE;
	if (isCommon&&common_flag==3) isCommon = kFALSE;
      }
    }
  }
  
  if (fRichMode) {
    fRichCat=gHades->getCurrentEvent()->getCategory(catRichHit);
    if (!fRichCat) {
      Warning("init","No Rich Input -> Switching RICH Mode OFF");
      fRichMode = kFALSE;
    }
  }
  if (fRichMode) iterRich= (HIterator *)fRichCat->MakeIterator();

  cout << "Spline: " <<fSpline <<" Kick lowres: "<< fKickLowres<<" KickB lowres: "
       << fKickLowresB<< " Kick hires: "<< fKickHires<< " RungeKutta: "<< fRK<< endl;
  cout << " Debug Mode: "<< fDebugMode;
  cout << " Rich Mode: "<<fRichMode<<" Minus Mode: " << fMinusMode << " Plus Mode: "<<fPlusMode << " Chi2Cut Inner " << bChiCutInner  << " Chi2Cut Outer " << bChiCutOuter<< " Common Mode: " << isCommon << endl;
  cout << "Skip Event: " << fSkip << endl;
  cout << " Mdc Sectors:\t\t";
  for (Int_t tt=0; tt<6; tt++) cout << mdc_sec[tt]  << " "; 
  cout << endl;
  if (fSpline) 
    {
      cout << " Spline Sectors:\t";
      for (Int_t tt=0; tt<6; tt++) cout << spline_sec_on[tt]  << " ";
      cout << endl;
    }
  if (fKickLowresB||fKickLowres) 
    {
      cout << " Lowres Kick Sectors:\t";
      for (Int_t tt=0; tt<6; tt++) cout << kick_sec_on[tt]  << " ";
      cout << endl;
    }
  if (fKickHires) 
    {
      cout << " Hires Kick Sectors:\t";
      for (Int_t tt=0; tt<6; tt++) cout << kick123_sec_on[tt]  << " ";
      cout << endl;
    }
  if (fRK) 
    {
      cout << " RungeKutta Sectors:\t";
      for (Int_t tt=0; tt<6; tt++) cout << rk_sec_on[tt]  << " ";
      cout << endl;
    }
  if (fSpline) 
    {
      cout << " Spline Field correction:\t";
      for (Int_t tt=0; tt<6; tt++) cout << spline_sec_field[tt]  << " ";
      cout << endl;
    }
  
  if (fRichMode)
    {
      cout << "Rich correlation parameters: theta0 "<< rich_params[0] << " dtheta " << rich_params[1] << " phi0 ";
      cout << rich_params[2] << " dphi " << rich_params[3] << " chi2_cut " << rich_params[4] << endl;
      cout << "Rich Quality parameters: RingPatMat "<< rich_qparams[0] << " RingPadNr " << rich_qparams[1] << " Centroid ";
      cout << rich_qparams[2] << " RingAmplitude " << rich_qparams[3] << endl;
    }


  fTofCat=gHades->getCurrentEvent()->getCategory(catTofHit);
  if (!fTofCat) Warning("init","No Tof Input");
  if (isSimulation) fTofinoCat=gHades->getCurrentEvent()->getCategory(catShowerHitTofTrack);
    else fTofinoCat=gHades->getCurrentEvent()->getCategory(catShowerHitTof);
  if (!fTofinoCat) Warning("init","No Tofino/Shower Input");

  if(fillNTuple)
    {
      before   = new TNtuple("before"  ,"hit"      ,"p:pol:beta:tof:m2:sys:flag:track");
      after    = new TNtuple("after"   ,"hit"      ,"p:pol:beta:tof:m2:sys:flag:track:dtof:theta:phi:tofind:recsec:id");
      richcorr = new TNtuple("richcorr","rich corr","mtheta:rtheta:mphi:rphi:chi2:corr");
    }

  bIsFirst = kTRUE;
  
  return kTRUE;
}

Int_t HTofRec::execute(void){

  if (bIsFirst && fillNTuple)
    {
      TString dstFile = gHades->getOutputFile()->GetName();
      if (sFile=="") sFile = sDir+"/tofrec_"+dstFile(dstFile.Last('/')+1,dstFile.Last('.')-dstFile.Last('/')-1)+".root";  
      else sFile = sDir + "/" + sFile;
      cout << " *** HTofRec: Ntuple output file:\t" << sFile << "\t ***" << endl;  
      bIsFirst = kFALSE;
    }
   
  Int_t flag[4]         = {0,  0, 0, 0};
  Int_t index[4]        = {-1,-1,-1,-1};
  Float_t start_time[4] = {0.,0.,0.,0.};
  
  Bool_t isRich        = kFALSE;
  Bool_t isRichSpline  = kFALSE; Bool_t isMinusSpline  = kFALSE;
  Bool_t isRichKick123 = kFALSE; Bool_t isMinusKick123 = kFALSE;
  Bool_t isRichKickB   = kFALSE; Bool_t isMinusKickB   = kFALSE; 
  Bool_t isRichRK      = kFALSE; Bool_t isMinusRK      = kFALSE;
  
  HMdcSeg        *mdcI      = 0;
  HMdcSeg        *mdcO      = 0;
  HKickTrackB    *kickB     = 0;
  HKickTrack123B *kick123   = 0;
  HSplineTrack   *spline    = 0;
  HRKTrackB      *runge     = 0;
  HMetaMatch     *metamatch = 0;
  
  
  Int_t spline_ind = -1, kick123_ind = -1, kickB_ind = -1, rk_ind = -1;;
  Float_t spline_tof = 0, spline_start = 0, kick123_tof = 0, rk_tof = 0, kick123_start = 0, kickB_tof = 0, kickB_start = 0, rk_start = 0;
  Int_t spline_sys  = -1, spline_pol  = 0, spline_sec  = -1;
  Int_t kick123_sys = -1, kick123_pol = 0, kick123_sec = -1;
  Int_t kickB_sys   = -1, kickB_pol   = 0, kickB_sec   = -1;
  Int_t rk_sys      = -1, rk_pol      = 0, rk_sec      = -1;
  
  if (fSpline||fKickLowresB||fKickHires||fRK)
    {
      iterMetaMatch->Reset();
      while ((metamatch = (HMetaMatch *)iterMetaMatch->Next()) != 0) // begin of MetaMatch iterator
	{
          mdcI = (HMdcSeg *)fMdcSegCat->getObject(((HMdcTrkCand *)fMdcTrkCandCat->getObject(metamatch->getTrkCandInd()))->getSeg1Ind());
	  if ((!bChiCutInner) || (mdcI->getChi2()>-1))
	    // cut on chi2> -1 for inner segment
	    { 
	      isRich = kFALSE;
	      if ((fRichMode)&&(isRichCorr(mdcI))) isRich = kTRUE;
              mdcO = (HMdcSeg *)fMdcSegCat->getObject(((HMdcTrkCand *)fMdcTrkCandCat->getObject(metamatch->getTrkCandInd()))->getSeg2Ind());
	      if ((!bChiCutOuter) || (mdcO->getChi2()>-1))
		// cut on chi2>-1 for outer segment
		{
		
		  if ((fSpline) && (metamatch->getSplineInd()!=-1) && metamatch->isSplineAccepted()) // if a Spline track is present
		    {
		      spline = (HSplineTrack *)fSplineCat->getObject(metamatch->getSplineInd());
		      if (spline->getQSpline()!=-1 && spline_sec_on[(Int_t)spline->getSector()] &&
			  ((spline->getSystem()==0 && (((HShowerHitTof *)fTofinoCat->getObject(spline->getShowerHitInd()))->getTofinoMult()==1)) ||
			   (spline->getSystem()==1 ))) 
			// cut on QSpline!=-1, no TOF clusters or TOFINO double hits, & sector ON
 	                { 
			  
			  if (isRich)  // if the track is RICh correlated
			    {
			      if ((!isRichSpline) ||   // if there are no other spline tracks rich correlated
				  ((isRichSpline) && (mdc_sec[(Int_t)spline->getSector()]>mdc_sec[spline_sec])) ||  // priority on sectoprs with more MDC chambers
				  ((isRichSpline) && (mdc_sec[(Int_t)spline->getSector()]==mdc_sec[spline_sec]) && (spline->getSystem()==1&&spline_sys==0)) ||   // or if new one TOF, old one TOFINO
				  ((isRichSpline) && (mdc_sec[(Int_t)spline->getSector()]==mdc_sec[spline_sec]) && (spline->getSystem()==spline_sys) && (spline->getPolarity()==-1 && spline_pol==1)) ||   // or if new one pol==1 and old  pol ==1
				  ((isRichSpline) && (mdc_sec[(Int_t)spline->getSector()]==mdc_sec[spline_sec]) && (spline->getSystem()==spline_sys) && (spline->getPolarity()==spline_pol==1) && spline->getTof()<spline_tof)) // faster particle
				
				{
				  isRichSpline = kTRUE;
				  spline_tof = spline->getTof();
				  spline_sys = spline->getSystem();
				  spline_pol = spline->getPolarity();
				  spline_ind = metamatch->getSplineInd();
				  spline_sec = spline->getSector();
				  spline_start = spline->getTofDist()*sqrt(spline->getP()*spline->getP()+HPidPhysicsConstants::mass(3)*HPidPhysicsConstants::mass(3))/(C*spline->getP())-spline->getTof(); // new start
				} // end of priority list for RICH correlated track
			    } // end of RICh region
			  
			  else if (!isRichSpline&&spline->getPolarity()==-1)
			    {
			      if ((!isMinusSpline) ||   // if there are no other negatice charged spline tracks
				  ((isMinusSpline) && (mdc_sec[(Int_t)spline->getSector()]>mdc_sec[spline_sec])) ||
				  ((isMinusSpline) && (mdc_sec[(Int_t)spline->getSector()]==mdc_sec[spline_sec]) && (spline->getSystem()==1&&spline_sys==0)) ||   // or if new one TOF,  old one TOFINO
				  ((isMinusSpline) && (mdc_sec[(Int_t)spline->getSector()]==mdc_sec[spline_sec]) && (spline->getSystem()==spline_sys) && (spline->getTof()<spline_tof))) // faster particle
				{
				  isMinusSpline = kTRUE;
				  spline_tof = spline->getTof();
				  spline_sys = spline->getSystem();
				  spline_ind = metamatch->getSplineInd();
				  spline_sec = spline->getSector();
				  spline_start = spline->getTofDist()*sqrt(spline->getP()*spline_sec_field[(Int_t)spline->getSector()]*spline->getP()*spline_sec_field[(Int_t)spline->getSector()]+HPidPhysicsConstants::mass(8)*HPidPhysicsConstants::mass(8))/(C*spline->getP()*spline_sec_field[(Int_t)spline->getSector()])-spline->getTof(); // new start
				} // end of priority list for negative charged tracks
			    } // end of MINUS region
			  
			} // end of quality cut for spline
		      
		    } // end of ifSpline
		
		  if ((fKickHires) && (metamatch->getKick123Ind()!=-1)) // if a Spline track is present
		    {
		      kick123 = (HKickTrack123B *)fKickHiCat->getObject(metamatch->getKick123Ind());
		      if (kick123_sec_on[(Int_t)kick123->getSector()] &&
			  ((kick123->getSystem()==0 && (((HShowerHitTof *)fTofinoCat->getObject(kick123->getShowerHitInd()))->getTofinoMult()==1)) ||
			   (kick123->getSystem()==1 ))) 
			// no TOF clusters or TOFINO double hits, & sector ON
			{
			  if (isRich)  // if the track is RICH correlated
			    {
			      if ((!isRichKick123) ||   // if there are no other kick123 tracks rich correlated
				  ((isRichKick123) && (mdc_sec[(Int_t)kick123->getSector()]>mdc_sec[kick123_sec])) ||  // priority on sectoprs with more MDC chambers
				  ((isRichKick123) && (mdc_sec[(Int_t)kick123->getSector()]==mdc_sec[kick123_sec]) && (kick123->getSystem()==1&&kick123_sys==0)) ||   // or if new one TOF, old one TOFINO
				  ((isRichKick123) && (mdc_sec[(Int_t)kick123->getSector()]==mdc_sec[kick123_sec]) && (kick123->getSystem()==kick123_sys) && (kick123->getPolarity()==-1 && kick123_pol==1)) ||   // or if new one pol==1 and old  pol ==1
				  ((isRichKick123) && (mdc_sec[(Int_t)kick123->getSector()]==mdc_sec[kick123_sec]) && (kick123->getSystem()==kick123_sys) && (kick123->getPolarity()==kick123_pol==1) && kick123->getTof()<kick123_tof)) // faster particle
				
				{
				  isRichKick123 = kTRUE;
				  kick123_tof = kick123->getTof();
				  kick123_sys = kick123->getSystem();
				  kick123_pol = kick123->getPolarity();
				  kick123_ind = metamatch->getKick123Ind();
				  kick123_sec = kick123->getSector();
				  kick123_start = kick123->getTofDist()*sqrt(kick123->getP()*kick123->getP()+HPidPhysicsConstants::mass(3)*HPidPhysicsConstants::mass(3))/(C*kick123->getP())-kick123->getTof(); // new start
				} // end of priority list for RICH correlated track
			    } // end of RICh region
		
			  else if (!isRichKick123&&kick123->getPolarity()==-1)
			    {
			      if ((!isMinusKick123) ||   // if there are no other negatice charged kick123 tracks
				  ((isMinusKick123) && (mdc_sec[(Int_t)kick123->getSector()]>mdc_sec[kick123_sec])) ||
				  ((isMinusKick123) && (mdc_sec[(Int_t)kick123->getSector()]==mdc_sec[kick123_sec]) && (kick123->getSystem()==1&&kick123_sys==0)) ||   // or if new one TOF,  old one TOFINO
				  ((isMinusKick123) && (mdc_sec[(Int_t)kick123->getSector()]==mdc_sec[kick123_sec]) && (kick123->getSystem()==kick123_sys) && (kick123->getTof()<kick123_tof))) // faster particle
				{
				  isMinusKick123 = kTRUE;
				  kick123_tof = kick123->getTof();
				  kick123_sys = kick123->getSystem();
				  kick123_ind = metamatch->getKick123Ind();
				  kick123_sec = kick123->getSector();
				  kick123_start = kick123->getTofDist()*sqrt(kick123->getP()*kick123->getP()+HPidPhysicsConstants::mass(8)*HPidPhysicsConstants::mass(8))/(C*kick123->getP())-kick123->getTof(); // new start
				} // end of priority list for negative charged tracks
			    } // end of MINUS region
			  
			} // end of quality cut for kick123
		      
		    } // end of ifKick123
		  
		  if ((fRK) && (metamatch->getRungeKuttaInd()!=-1) && metamatch->isSplineAccepted()) // if a RK track is present
		    {
		      runge = (HRKTrackB *)fRKCat->getObject(metamatch->getRungeKuttaInd());
		      if (rk_sec_on[(Int_t)runge->getSector()] &&
			  ((runge->getSystem()==0 && (((HShowerHitTof *)fTofinoCat->getObject(runge->getShowerHitInd()))->getTofinoMult()==1)) ||
			   (runge->getSystem()==1 ))) 
			// no TOF clusters or TOFINO double hits, & sector ON
 	                { 
			  
			  if (isRich)  // if the track is RICH correlated
			    {
			      if ((!isRichRK) ||   // if there are no other spline tracks rich correlated
				  ((isRichRK) && (mdc_sec[(Int_t)runge->getSector()]>mdc_sec[rk_sec])) ||  // priority on sectoprs with more MDC chambers
				  ((isRichRK) && (mdc_sec[(Int_t)runge->getSector()]==mdc_sec[rk_sec]) && (runge->getSystem()==1&&rk_sys==0)) ||   // or if new one TOF, old one TOFINO
				  ((isRichSpline) && (mdc_sec[(Int_t)runge->getSector()]==mdc_sec[rk_sec]) && (runge->getSystem()==rk_sys) && (runge->getPolarity()==-1 && rk_pol==1)) ||   // or if new one pol==1 and old  pol ==1
				  ((isRichSpline) && (mdc_sec[(Int_t)runge->getSector()]==mdc_sec[rk_sec]) && (runge->getSystem()==rk_sys) && (runge->getPolarity()==rk_pol==1) && runge->getTof()<rk_tof)) // faster particle
				
				{
				  isRichRK = kTRUE;
				  rk_tof = runge->getTof();
				  rk_sys = runge->getSystem();
				  rk_pol = runge->getPolarity();
				  rk_ind = metamatch->getRungeKuttaInd();
				  rk_sec = runge->getSector();
				  rk_start = runge->getTofDist()*sqrt(runge->getP()*runge->getP()+HPidPhysicsConstants::mass(3)*HPidPhysicsConstants::mass(3))/(C*runge->getP())-runge->getTof(); // new start
				} // end of priority list for RICH correlated track
			    } // end of RICh region
			  
			  else if (!isRichRK&&runge->getPolarity()==-1)
			    {
			      if ((!isMinusRK) ||   // if there are no other negatice charged spline tracks
				  ((isMinusRK) && (mdc_sec[(Int_t)runge->getSector()]>mdc_sec[rk_sec])) ||
				  ((isMinusRK) && (mdc_sec[(Int_t)runge->getSector()]==mdc_sec[rk_sec]) && (runge->getSystem()==1&&rk_sys==0)) ||   // or if new one TOF,  old one TOFINO
				  ((isMinusRK) && (mdc_sec[(Int_t)runge->getSector()]==mdc_sec[rk_sec]) && (runge->getSystem()==rk_sys) && (runge->getTof()<rk_tof))) // faster particle
				{
				  isMinusRK = kTRUE;
				  rk_tof = runge->getTof();
				  rk_sys = runge->getSystem();
				  rk_ind = metamatch->getRungeKuttaInd();
				  rk_sec = runge->getSector();
				  rk_start = runge->getTofDist()*sqrt(runge->getP()*runge->getP()+HPidPhysicsConstants::mass(8)*HPidPhysicsConstants::mass(8))/(C*runge->getP())-runge->getTof(); // new start
				} // end of priority list for negative charged tracks
			    } // end of MINUS region
			  
			} // end of quality cut for RK
		      
		    } // end of ifRK

		} //end of chi2>-1 outer segment selection

	      if ((fKickLowresB) && (metamatch->getKickInd()!=-1)) // if a lowres kicktrack is present
		{
		  kickB = (HKickTrackB *)fKickLowBCat->getObject(metamatch->getKickInd());
		  if (kick_sec_on[(Int_t)kickB->getSector()] &&
		      ((kickB->getSystem()==0 && (((HShowerHitTof *)fTofinoCat->getObject(kickB->getShowerHitInd()))->getTofinoMult()==1)) ||
		       (kickB->getSystem()==1))) 
		    // no TOF clusters or TOFINO double hits, & sector ON
		    {
		      if (isRich)  // if the track is RICH correlated
			{
			  if ((!isRichKickB) ||   // if there are no other kickB tracks rich correlated
			      ((isRichKickB) && (kickB->getSystem()==1&&kickB_sys==0)) ||   // or if new one TOF, old one TOFINO
			      ((isRichKickB) && (kickB->getSystem()==kickB_sys) && (kickB->getPolarity()==-1 && kickB_pol==1)) ||   // or if new one pol==1 and old  pol ==1
			      ((isRichKickB) && (kickB->getSystem()==kickB_sys) && (kickB->getPolarity()==kickB_pol==1) && kickB->getTof()<kickB_tof)) // faster particle
			    {
			      isRichKickB = kTRUE;
			      kickB_tof = kickB->getTof();
			      kickB_sys = kickB->getSystem();
			      kickB_pol = kickB->getPolarity();
			      kickB_ind = metamatch->getKickInd();
			      kickB_sec = kickB->getSector();
			      kickB_start = kickB->getTof()*kickB->getBeta()*C*sqrt(kickB->getP()*kickB->getP()+HPidPhysicsConstants::mass(3)*HPidPhysicsConstants::mass(3))/(C*kickB->getP())-kickB->getTof(); // new start
			    } // end of priority list for RICH correlated track
			} // end of RICh region
		      
		      else if (!isRichKickB&&kickB->getPolarity()==-1)
			{
			  if ((!isMinusKickB) ||   // if there are no other negatice charged kickB tracks
			      ((isMinusKickB) && (kickB->getSystem()==1&&kickB_sys==0)) ||   // or if new one TOF,  old one TOFINO
			      ((isMinusKickB) && (kickB->getSystem()==kickB_sys) && (kickB->getTof()<kickB_tof))) // faster particle
			    {
			      isMinusKickB = kTRUE;
			      kickB_tof = kickB->getTof();
			      kickB_sys = kickB->getSystem();
			      kickB_ind = metamatch->getKickInd();
			      kickB_sec = kickB->getSector();
			      kickB_start = kickB->getTof()*kickB->getBeta()*C*sqrt(kickB->getP()*kickB->getP()+HPidPhysicsConstants::mass(8)*HPidPhysicsConstants::mass(8))/(C*kickB->getP())-kickB->getTof(); // new start
			    } // end of priority list for negative charged tracks
			} // end of MINUS region
		      
		    } // end of quality cut for kickB
		  
		} // end of ifKickB
	      
	    } // end of chi2>-1 inner segment selection
	} // end oh metamatch iterator
    } // end of base tracking region
  
  
  Bool_t flag_rec = kFALSE;
  if (isRichSpline)
    {
      flag[0] = 1;
      start_time[0] = spline_start;
      index[0] = spline_ind;
      flag_rec = kTRUE;
    } 
  else if (isMinusSpline)
    {
      flag_rec = kTRUE;
      flag[0] = 2;
      start_time[0] = spline_start;
      index[0] = spline_ind;
    }
  
  if (isRichKickB)
    {
      flag[1] = 1;
      start_time[1] = kickB_start;
      index[1] = kickB_ind;
      flag_rec = kTRUE;
    } 
  else if (isMinusKickB)
    {
      flag_rec = kTRUE;
      flag[1] = 2;
      start_time[1] = kickB_start;
      index[1] = kickB_ind;
    }
  if (isRichKick123)
    {
      flag[2] = 1;
      start_time[2] = kick123_start;
      index[2] = kick123_ind;
      flag_rec = kTRUE;
    } 
  else if (isMinusKick123)
    {
      flag_rec = kTRUE;
      flag[2] = 2;
      start_time[2] = kick123_start;
      index[2] = kick123_ind;
    }
  if (isRichRK)
    {
      flag[3] = 1;
      start_time[3] = rk_start;
      index[3] = rk_ind;
      flag_rec = kTRUE;
    } 
  else if (isMinusRK)
    {
      flag_rec = kTRUE;
      flag[3] = 2;
      start_time[3] = rk_start;
      index[3] = rk_ind;
    }
  
  if (isCommon)
    {
      if (flag[common_flag]!=0)
	{
	  for (Int_t tt=0; tt<4; tt++)
	    if (tt!=common_flag)
	      {
		flag[tt] = flag[common_flag];
		start_time[tt] = start_time[common_flag];
		index[tt] = index[common_flag];
	      }
	}
      else flag_rec = kFALSE;
    }
  
  if (flag_rec)    
    track_recalc(start_time, index, flag);
  if (!flag_rec && fSkip) return kSkipEvent;
  
  return 0;
}


Bool_t HTofRec::finalize(void){
  if(fDebugMode)
    {
      TFile *r = TFile::Open(sFile,"RECREATE");
      after   ->Write();
      before  ->Write();
      richcorr->Write();
      
      r->Save();
      r->Close();
      r->Delete();
      
      after   ->Delete();
      before  ->Delete();
      richcorr->Delete();
    }
  
  return kTRUE;
}


void HTofRec::track_recalc(Float_t *start, Int_t *rec_ind, Int_t *flag)
{
  // Tof recalculator
  // 0 - spline
  // 1 - kickplane lowresB
  // 2 - kickplane hires
  // 3 - RungeKutta
 
  HMetaMatch *metamatch = 0;
  
  Float_t tof_new,tof_old, beta, mass2,p, tof_id;
  Int_t rec_sec;
  
  iterMetaMatch->Reset();
  while ((metamatch=(HMetaMatch *)iterMetaMatch->Next()) != 0) // metamatch loop
    {
      if ((flag[0]!=0) && (metamatch->getSplineInd()!=-1)) // spline tracks
	{ 
	  
	  rec_sec = ((HSplineTrack *)fSplineCat->getObject(rec_ind[0]))->getSector();
	  HSplineTrack *spline_ch = (HSplineTrack *)fSplineCat->getObject(metamatch->getSplineInd());
	  if (spline_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(spline_ch->getP(),
			       spline_ch->getPolarity(),
			       spline_ch->getBeta(),
			       spline_ch->getTof(),
			       spline_ch->getMass2(),
			       spline_ch->getSystem(),
			       flag[0],
			       0);
		}
	      
	      tof_old = spline_ch->getTof();
	      p = spline_ch->getP()*spline_sec_field[(Int_t)spline_ch->getSector()];
	      tof_new = spline_ch->getTof()+start[0];
	      beta    = spline_ch->getTof()*spline_ch->getBeta()/tof_new;
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      
	      spline_ch->setTof(tof_new);
	      spline_ch->setBeta(beta);
	      spline_ch->setMass2(mass2,0);
	      spline_ch->setTofFlag(flag[0]);
	      
	      if(fillNTuple)
		{
		  if (spline_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(spline_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		    }
		  else
		    {
		      HTofHit *Tof= (HTofHit *)fTofCat->getObject(spline_ch->getTofHitInd());
		      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(spline_ch->getP()*spline_sec_field[(Int_t)spline_ch->getSector()],
			      spline_ch->getPolarity(),
			      spline_ch->getBeta(),
			      spline_ch->getTof(),
			      spline_ch->getMass2(),
			      spline_ch->getSystem(),
			      spline_ch->getTofFlag(),
			      0,
			      start[0],                                 // tof_new-tof_old,
			      spline_ch->getTheta(),
			      spline_ch->getPhi(),
			      tof_id,
			      rec_sec,                               // sec of identified track
			      (rec_ind[0]==metamatch->getSplineInd())); // is identified track
		} // end of filling "after" ntuple
	    } // end of system!=-1 condition
	} // end of spline track
      
      if ((flag[1]!=0) && (metamatch->getKickInd()!=-1)) // kick tracks
	{ 
	  
	  rec_sec = ((HKickTrackB *)fKickLowBCat->getObject(rec_ind[1]))->getSector();
	  HKickTrackB *kickB_ch = (HKickTrackB *)fKickLowBCat->getObject(metamatch->getKickInd());
	  if (kickB_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(kickB_ch->getP(),
			       kickB_ch->getPolarity(),
			       kickB_ch->getBeta(),
			       kickB_ch->getTof(),
			       kickB_ch->getMass2(),
			       kickB_ch->getSystem(),
			       flag[1],
			       1);
		}
	      
	      tof_old = kickB_ch->getTof();
	      p = kickB_ch->getP();
	      tof_new = kickB_ch->getTof()+start[1];
	      beta    = kickB_ch->getTof()*kickB_ch->getBeta()/tof_new;
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      
	      kickB_ch->setTof(tof_new);
	      kickB_ch->setBeta(beta);
	      kickB_ch->setMass2(mass2,0);
	      kickB_ch->setTofFlag(flag[1]);
	      
	      if(fillNTuple)
		{
		  if (kickB_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(kickB_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		    }
		  else
		    {
		      HTofHit *Tof= (HTofHit *)fTofCat->getObject(kickB_ch->getTofHitInd());
		      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(kickB_ch->getP(),
			      kickB_ch->getPolarity(),
			      kickB_ch->getBeta(),
			      kickB_ch->getTof(),
			      kickB_ch->getMass2(),
			      kickB_ch->getSystem(),
			      kickB_ch->getTofFlag(),
			      1,
			      start[1],                                 // tof_new-tof_old,
			      kickB_ch->getTheta(),
			      kickB_ch->getPhi(),
			      tof_id,
			      rec_sec,                               // sec of identified track
			      (rec_ind[1]==metamatch->getKickInd())); // is identified track
		} // end of filling "after" ntuple
	    } // end of system!=-1 condition
	} // end of kickB track

 
      if ((flag[2]!=0) && (metamatch->getKick123Ind()!=-1)) // kick tracks
	{ 
	  
	  rec_sec = ((HKickTrack123B *)fKickHiCat->getObject(rec_ind[2]))->getSector();
	  HKickTrack123B *kick123_ch = (HKickTrack123B *)fKickHiCat->getObject(metamatch->getKick123Ind());
	  if (kick123_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(kick123_ch->getP(),
			       kick123_ch->getPolarity(),
			       kick123_ch->getBeta(),
			       kick123_ch->getTof(),
			       kick123_ch->getMass2(),
			       kick123_ch->getSystem(),
			       flag[2],
			       2);
		}
	      
	      tof_old = kick123_ch->getTof();
	      p = kick123_ch->getP();
	      tof_new = kick123_ch->getTof()+start[2];
	      beta    = kick123_ch->getTof()*kick123_ch->getBeta()/tof_new;
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      
	      kick123_ch->setTof(tof_new);
	      kick123_ch->setBeta(beta);
	      kick123_ch->setMass2(mass2,0);
	      kick123_ch->setTofFlag(flag[2]);
	      
	      if(fillNTuple)
		{
		  if (kick123_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(kick123_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		    }
		  else
		    {
		      HTofHit *Tof= (HTofHit *)fTofCat->getObject(kick123_ch->getTofHitInd());
		      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(kick123_ch->getP(),
			      kick123_ch->getPolarity(),
			      kick123_ch->getBeta(),
			      kick123_ch->getTof(),
			      kick123_ch->getMass2(),
			      kick123_ch->getSystem(),
			      kick123_ch->getTofFlag(),
			      2,
			      start[2],                                 // tof_new-tof_old,
			      kick123_ch->getTheta(),
			      kick123_ch->getPhi(),
			      tof_id,
			      rec_sec,                               // sec of identified track
			      (rec_ind[2]==metamatch->getKick123Ind())); // is identified track
		} // end of filling "after" ntuple
	    } // end of system!=-1 condition
	} // end of kick123 track
      
      if ((flag[3]!=0) && (metamatch->getRungeKuttaInd()!=-1)) // RungeKutta tracks
	{ 
	  
	  rec_sec = ((HRKTrackB *)fRKCat->getObject(rec_ind[3]))->getSector();
	  HRKTrackB *rk_ch = (HRKTrackB *)fRKCat->getObject(metamatch->getRungeKuttaInd());
	  if (rk_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(rk_ch->getP(),
			       rk_ch->getPolarity(),
			       rk_ch->getBeta(),
			       rk_ch->getTof(),
			       rk_ch->getMass2(),
			       rk_ch->getSystem(),
			       flag[3],
			       3);
		}
	      
	      tof_old = rk_ch->getTof();
	      p = rk_ch->getP();
	      tof_new = rk_ch->getTof()+start[3];
	      beta    = rk_ch->getTof()*rk_ch->getBeta()/tof_new;
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      
	      rk_ch->setTof(tof_new);
	      rk_ch->setBeta(beta);
	      rk_ch->setMass2(mass2,0);
	      rk_ch->setTofFlag(flag[3]);
	      
	      if(fillNTuple)
		{
		  if (rk_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(rk_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		    }
		  else
		    {
		      HTofHit *Tof= (HTofHit *)fTofCat->getObject(rk_ch->getTofHitInd());
		      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(rk_ch->getP(),
			      rk_ch->getPolarity(),
			      rk_ch->getBeta(),
			      rk_ch->getTof(),
			      rk_ch->getMass2(),
			      rk_ch->getSystem(),
			      rk_ch->getTofFlag(),
			      3,
			      start[3],                                 // tof_new-tof_old,
			      rk_ch->getTheta(),
			      rk_ch->getPhi(),
			      tof_id,
			      rec_sec,                               // sec of identified track
			      (rec_ind[3]==metamatch->getRungeKuttaInd())); // is identified track
		} // end of filling "after" ntuple
	    } // end of system!=-1 condition
	} // end of RungeKutta track

    } // end of metamatch loop
}

Bool_t HTofRec::isRichCorr(HMdcSeg* mdc) 
{
  // function which check if a HMdcSeg is correlated to RICH
  
  HRichHit *rich = 0;
  Bool_t isCorr = kFALSE;
  Float_t rich_theta, rich_phi, chi2;
  
  Int_t mdc_sec = mdc->getSec();
  Float_t mdc_theta = mdc->getTheta()*TMath::RadToDeg();
  Float_t  mdc_phi=fmod(mdc_sec*60.+mdc->getPhi()*TMath::RadToDeg(),360.);
  
  iterRich->Reset();
  while ((rich=(HRichHit *)iterRich->Next()) != 0)
    if (rich->getSector()==mdc_sec) // correlation in sector
      {
	if (rich->getRingPatMat()>rich_qparams[0]&&
	    rich->getRingPadNr()>=rich_qparams[1]&&
	    rich->getCentroid()<rich_qparams[2]&&
	    rich->getRingAmplitude()>=rich_qparams[3])
	  {
	    rich_theta = rich->getTheta();
	    rich_phi   = rich->getPhi();
	    
	    chi2 = (mdc_theta-rich_theta-rich_params[0])*(mdc_theta-rich_theta-rich_params[0])/rich_params[1]/rich_params[1]+(mdc_phi-rich_phi-rich_params[2])*(mdc_phi-rich_phi-rich_params[2])/rich_params[3]/rich_params[3]; // chi2 of rich-track
	    if (fDebugMode) // fill RICH debug histos
	      {
		richcorr->Fill(mdc_theta,rich_theta,mdc_phi,rich_phi,chi2,chi2<rich_params[4]);
	      }
	    if (chi2<rich_params[4]) isCorr = kTRUE;// if RICH correlation
	  } // end of cut on ring quality
	
      } // end of RICH loop
  
  return isCorr;
}

