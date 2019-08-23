//*-- Author : Stefano Spataro
// HTofRec
// In case of experiments where there exists no start hit and the TOF of the
// particles has to be recalculated, the tof variable(getTof()) of all classes derived from
// HBaseTrack and HKickTrack will be overwritten by a this class which recalculates the tof.
// To mark those objects as changed one can ask with getFlag(). The result should be
// 1 or 2 depending on the method which has been used and 0 if it has not been changed.
// Anyhow the original tof can be retrieved direct from the HMdcTofHit referenced by
// tofindex (getTofHitInd()). The flag itself is decoded in the tofindex, but will be
// hidden from the user by the getTofHitInd() function.
//
// To setup the constructor  call HTofRec(Text_t *name,Text_t *title, const char *select),
// where select contains the key words for configuration (just "," separated list)
// nospline  : switch of spline
// nohires   : switch of high resolution kickplane
// nolores   : switch of low  resolution kickplane
// nolowresB : switch of low  resolution kickplaneB
// norich    : switch of rich mode
// field     : switch on field correction 1.29
// debug     : switch on debug mode
// ntuple    : switch on create,fill and write ntuple for debugging
//
// defaults (empty string) are:
//   spline on
//   low  resolution kickplane on
//   low  resolution kickplaneB on
//   high resolution kickplane on
//   fRichMode on
//   field correction 1.0
//   debug mode off
//   ntuple off
//
/////////////////////////////////////////////////////////////////

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
#include "showertofinodef.h"
#include "hshowerhittof.h"
#include "hmdctrackgdef.h"
#include "kickdef.h"

#include "hpidphysicsconstants.h"


ClassImp(HTofRec)

HTofRec::HTofRec()
{
    C=TMath::C()*1e-9;
    correct[0] = 0.;
    correct[1] = 0.;
    rich_params[0] = 0.;
    rich_params[1] = 1.;
    rich_params[2] = 0.;
    rich_params[3] = 1.;

    before   = 0;
    after    = 0;
    richcorr = 0;

    fRichMode        = kTRUE;
    fPlusMode        = kFALSE;
    fMinusMode       = kTRUE;
    fFieldCorrection = 1.;
    fDebugMode       = kFALSE;
    fSpline          = kTRUE;
    fKickLowres      = kTRUE;
    fKickLowresB     = kTRUE;
    fKickHires       = kTRUE;
    fillNTuple       = kFALSE;
}

HTofRec::HTofRec(Text_t *name,Text_t *title, const char *select) : 
  HReconstructor(name,title)
{
    TString sel = select;
    sel.ToLower();

    fRichMode        = kTRUE;
    fPlusMode        = kFALSE;
    fMinusMode       = kTRUE;
    fDebugMode       = kFALSE;
    fFieldCorrection = 1.;
    fSpline          = kTRUE;
    fKickLowres      = kTRUE;
    fKickLowresB     = kTRUE;
    fKickHires       = kTRUE;
    fillNTuple       = kFALSE;

    if (sel.Contains("nospline"))  fSpline          = kFALSE;
    if (sel.Contains("nohires"))   fKickHires       = kFALSE;
    if (sel.Contains("nolowres"))  fKickLowres      = kFALSE;
    if (sel.Contains("nolowresB")) fKickLowresB     = kFALSE;
    if (sel.Contains("field"))     fFieldCorrection = 1.28;
    if (sel.Contains("norich"))    fRichMode        = kFALSE;
    if (sel.Contains("debug"))     fDebugMode       = kTRUE;
    if (sel.Contains("ntuple"))    fillNTuple       = kTRUE;

    C=TMath::C()*1e-9;
    correct[0] = 0.;
    correct[1] = 0.;
   
    rich_params[0] = 0.;
    rich_params[1] = 1.;
    rich_params[2] = 0.;
    rich_params[3] = 1.;

    before   = 0;
    after    = 0;
    richcorr = 0;

}

HTofRec::~HTofRec(void){
}

Bool_t HTofRec::init(void){
    cout << "Tof Reconstructor default initialization *** " << endl;
    if (fSpline) {
	fSplineCat=gHades->getCurrentEvent()->getCategory(catSplineTrack);
	if (!fSplineCat) {
	    Warning("init","No Spline input -> Switching spline OFF");
	    fSpline = kFALSE;
	}
    }
    if (fSpline) iterSpline= (HIterator *)fSplineCat->MakeIterator();

    if (fKickLowres) {
	fKickLowCat= gHades->getCurrentEvent()->getCategory(catKickTrack);
	if (!fKickLowCat) {
	    Warning("init","No lowres Kick input -> Switching lowres Kick OFF");
	    fKickLowres = kFALSE;
	}
    }
    if (fKickLowres) iterKickLowres= (HIterator *)fKickLowCat->MakeIterator();

    if (fKickLowresB) {
	fKickLowBCat= gHades->getCurrentEvent()->getCategory(catKickTrackB);
	if (!fKickLowBCat) {
	    Warning("init","No lowres KickB input -> Switching lowres KickB OFF");
	    fKickLowresB = kFALSE;
	}
    }
    if (fKickLowresB) iterKickLowresB= (HIterator *)fKickLowBCat->MakeIterator();

    if (fKickHires) {
	fKickHiCat= gHades->getCurrentEvent()->getCategory(catKickTrack123B);
	if (!fKickHiCat) {
	    Warning("init","No hires Kick input -> Switching hires Kick OFF");
	    fKickHires = kFALSE;
	}
    }
    if (fKickHires) iterKickHires= (HIterator *)fKickHiCat->MakeIterator();


    if (fRichMode) {
	fRichCat=gHades->getCurrentEvent()->getCategory(catRichHit);
	if (!fRichCat) {
	    Warning("init","No Rich Input -> Switching RICH Mode OFF");
	    fRichMode = kFALSE;
	}
    }
    if (fRichMode) iterRich= (HIterator *)fRichCat->MakeIterator();

    cout << "Spline: " <<fSpline <<" Kick lowres: "<< fKickLowres<<" KickB lowres: "
         << fKickLowresB<< " Kick hires: "<< fKickHires<< endl;
    cout <<" Field Correction: " << fFieldCorrection<< " Debug Mode: "<< fDebugMode;
    cout << "Rich Mode: "<<fRichMode<<" Minus Mode: " << fMinusMode << " Plus Mode: "<<fPlusMode << endl;

    if(fillNTuple)
    {
      fTofCat=gHades->getCurrentEvent()->getCategory(catTofHit);
      if (!fTofCat) Warning("init","No Tof Input");
      fTofinoCat=gHades->getCurrentEvent()->getCategory(catShowerHitTof);
      if (!fTofinoCat) Warning("init","No Tofino/Shower Input");
      
      before   = new TNtuple("before"  ,"hit"      ,"p:pol:beta:tof:m2:sys:flag:track");
      after    = new TNtuple("after"   ,"hit"      ,"p:pol:beta:tof:m2:sys:flag:track:dtof:theta:phi:sec:tofind:tofinomult");
      richcorr = new TNtuple("richcorr","rich corr","theta1:theta2:dtheta:phi1:phi2:dphi:chi2:corr:track");
    }
    
    return kTRUE;
}

Int_t HTofRec::execute(void){
  HKickTrack *kickL;
  HKickTrackB *kickBL;
  HKickTrack123B *kickH;
  HSplineTrack *spline;
  HRichHit *rich;


  Float_t rich_theta[10], rich_phi[10];
  Int_t rich_sec[10];
  Int_t rich_count = 0;
  if (fRichMode)  // begin of RICH loop
    {
      iterRich->Reset();
      while ((rich=(HRichHit *)iterRich->Next()) != 0)
	{
	  if (rich->getRingPatMat()>200&&
	      rich->getRingPadNr()>=4&&
	      rich->getCentroid()<2.8&&
	      rich->getRingAmplitude()>=5)
	    {
	      rich_theta[rich_count] = rich->getTheta();
	      rich_sec  [rich_count] = rich->getSector();
	      rich_phi  [rich_count] = rich->getPhi();
	      rich_count++;
	    }
      }
    } // end of RICH loop
  
  
  // ***************    SPLINE region    *****************
  if (fSpline)  
    {
      Float_t sp_tof[20], sp_p[20], sp_len[20];
      Float_t start_tof = 0., rec_mass = 0.;
      Float_t chi2;
      Int_t sp_pol[20], sp_sys[20];
      Int_t sp_count  = 0;
      Int_t rec_ind   = -1;
      Int_t isMinus   = 0;
      Int_t isRich    = 0;
      iterSpline->Reset();
      while ((spline=(HSplineTrack *)iterSpline->Next()) != 0) // begin of SPLINE loop
	{
	  if (spline->getSystem()!=-1&&spline->getP()>0.)  // if track is correlated to META
	    {  
	      sp_tof[sp_count] = spline->getTof();
	      sp_sys[sp_count] = spline->getSystem();
	      sp_len[sp_count] = sp_tof[sp_count]*spline->getBeta()*C*1000.; 
	      sp_tof[sp_count] = spline->getTof()+correct[sp_sys[sp_count]];
	      sp_p  [sp_count] = spline->getP()*fFieldCorrection;
	      sp_pol[sp_count] = spline->getPolarity();
	      
	      if (fRichMode&&rich_count) // if there are RICH count and RICH mode is on
		{
		  Float_t sp_theta, sp_phi;
		  Int_t sp_sec;
		  sp_theta = spline->getTheta()*TMath::RadToDeg();
		  sp_sec   = spline->getSector();
                  sp_phi=fmod((sp_sec)*60.+spline->getPhi()*TMath::RadToDeg(),360.);
		  
		  for (Int_t rr=0; rr<rich_count; rr++) // begin of rich loop
		    {
		      if (sp_sec==rich_sec[rr])  // if sector correlation
			{
			  if (fDebugMode)
			    chi2 = (sp_theta-rich_theta[rr]-rich_params[0])*(sp_theta-rich_theta[rr]-rich_params[0])/rich_params[1]/rich_params[1]+(sp_phi-rich_phi[rr]-rich_params[2])*(sp_phi-rich_phi[rr]-rich_params[2])/rich_params[3]/rich_params[3]; // chi2 of rich-track
			    {
			      richcorr->Fill(sp_theta,rich_theta[rr],sp_theta-rich_theta[rr],sp_phi,rich_phi[rr],sp_phi-rich_phi[rr],chi2,chi2<rich_params[4],0); // fill debug histos
			    }
			 
			  if (chi2<rich_params[4]) // RICH correlation
			    {
			      rec_mass = HPidPhysicsConstants::mass(3); // e-
			      
			      // search for tracklet with the minimum tof
			      if (!isRich) 
				{
				  rec_ind = sp_count;
				}
			      else if (sp_tof[sp_count]<sp_tof[rec_ind])
				{
				  rec_ind = sp_count;
				} 
			      // end of search
			      isRich = 1;
			    } // end of RICH correlation
			} // end of sector correlation
		    } // end of RICH loop
		} // end of RICH mode on and RICh hits
	      
	      if (!isRich&&fMinusMode&&sp_pol[sp_count]<0) // if track !RICH but MINUS 
		{
		  rec_mass = HPidPhysicsConstants::mass(8); // pi+
		  
		  // search for negative track with the higher momentum
		  if (!isMinus)
		    {
		      rec_ind = sp_count;
		    }
		  else if (sp_tof[sp_count]<sp_tof[rec_ind])
		    {
		      rec_ind = sp_count;
		    }
		  // end of search
		  isMinus = 1;
		} // end of !RICH but MINUS
	      sp_count++;
	    }  // end of META correlated
	} // end of SPLINE loop
      
      if (rec_ind>=0&&sp_p[rec_ind]!=0) // if a track is identified -> calculate start time
	{
	  start_tof = sp_len[rec_ind]*sqrt(sp_p[rec_ind]*sp_p[rec_ind]+rec_mass*rec_mass)/(1000*C*sp_p[rec_ind])-sp_tof[rec_ind];
	}
      // now recalculate tof of all the tracks in the event
      if (isRich)
	{
	  track_recalc(0,start_tof,1);
	}
      else if (isMinus)
	{
	  track_recalc(0,start_tof,2);
	}
      
    } // end of SPLINE loop
  // ************    end of SPLINE region        *************
  
  
  // ************    LOWRES KICKTRACK  region    *************
  
  if (fKickLowres)
    {
      Float_t kl_tof[20], kl_p[20], kl_len[20];
      Float_t start_tof = 0, rec_mass = 0;
      Float_t chi2;
      Int_t kl_pol[20], kl_sys[20];
      Int_t kl_count  = 0;
      Int_t rec_ind   = -1;
      Int_t isMinus   = 0;
      Int_t isRich    = 0;
      
      iterKickLowres->Reset();
      while ((kickL=(HKickTrack *)iterKickLowres->Next()) != 0) // begin of lowres KICKloop
	{
	  if (kickL->getSystem()!=-1&&kickL->getP()>0.) // if META correlated
	    {  
	      kl_tof[kl_count] = kickL->getTof();
	      kl_sys[kl_count] = kickL->getSystem();
	      kl_len[kl_count] = kl_tof[kl_count]*kickL->getBeta()*C*1000.;
	      kl_p[kl_count] = kickL->getP()*fFieldCorrection;
	      kl_pol[kl_count] = kickL->getCharge();
	      kl_tof[kl_count] = kickL->getTof()+correct[kl_sys[kl_count]];
	      
	      if (fRichMode&&rich_count) // if RICH mode and RICH hits
		{
		  Float_t kl_theta, kl_phi;
		  Int_t kl_sec;
		  kl_theta = kickL->getTheta()*TMath::RadToDeg();
		  kl_sec   = kickL->getSector();
		  
		  kl_phi=fmod(kl_sec*60.+kickL->getPhi()*TMath::RadToDeg(),360.);
		  
		  for (Int_t rr=0; rr<rich_count; rr++) // begin of RICH loop
		    {
		      if (kl_sec==rich_sec[rr]) // if sector correlation
			{
			  chi2 = (kl_theta-rich_theta[rr]-rich_params[0])*(kl_theta-rich_theta[rr]-rich_params[0])/rich_params[1]/rich_params[1]+(kl_phi-rich_phi[rr]-rich_params[2])*(kl_phi-rich_phi[rr]-rich_params[2])/rich_params[3]/rich_params[3]; // chi2 of rich-track
			  if (fDebugMode) // fill RICH debug histos
			    {
			      richcorr->Fill(kl_theta,rich_theta[rr],kl_theta-rich_theta[rr],kl_phi,rich_phi[rr],kl_phi-rich_phi[rr],chi2,chi2<rich_params[4],1); 
			    }
			  if (chi2<rich_params[4]) // if RICH correlation
			    {
			      rec_mass = HPidPhysicsConstants::mass(3);  // e-
			      
			      // now search the RICH correlated hit with the minimum tof
			      if (!isRich)
				{
				  rec_ind = kl_count;
				}
			      else if (kl_tof[kl_count]<kl_tof[rec_ind])
				{
				  rec_ind = kl_count;
				}
			      isRich = 1;
			      // end of search
			    } // end of RICH correlation
			} // end of sectror correlation
		    } // end of RICH loop
		} // end of if RICH mode
	      
	      if (!isRich&&fMinusMode&&kl_pol[kl_count]<0) // if !RICH but MINUS
		{
		  rec_mass = HPidPhysicsConstants::mass(8);  // pi+
		  // search for MINUS track with higher momentum
		  if (!isMinus)
		    {
		      rec_ind = kl_count;
		    }
		  else if (kl_tof[kl_count]<kl_tof[rec_ind])
		    {
		      rec_ind = kl_count;
		    }
		  // end of search
		  isMinus = 1;
		} // end of !RICH but MINUS
	      kl_count++;
	    } // end of META correlated
	} // end of LOWRES KICK loop
      
      if (rec_ind>=0&&kl_p[rec_ind]!=0) // if a track is identified -> reconstruct start time
	{
	  start_tof = kl_len[rec_ind]*sqrt(kl_p[rec_ind]*kl_p[rec_ind]+rec_mass*rec_mass)/(1000*C*kl_p[rec_ind])-kl_tof[rec_ind];
	}
      
      // recalculate tof of others tracks in the event
      if (isRich)
	{
	  track_recalc(1,start_tof,1);
	}
      else if (isMinus)
	{
	  track_recalc(1,start_tof,2);
	}
      // end fo recalculation
      
    }
  // ************    end of LOWRES KICKTRACK region        *************
  
  
  // ************    LOWRES KICKTRACK BASE  region    *************
  
  if (fKickLowresB)
    {
      Float_t kBl_tof[20], kBl_p[20], kBl_len[20];
      Float_t start_tof = 0, rec_mass = 0;
      Float_t chi2;
      Int_t kBl_pol[20], kBl_sys[20];
      Int_t kBl_count = 0;
      Int_t rec_ind   = -1;
      Int_t isMinus   = 0;
      Int_t isRich    = 0;
      
      iterKickLowresB->Reset(); // begin of kick lowresB loop
      while ((kickBL=(HKickTrackB *)iterKickLowresB->Next()) != 0)
	{
	  if (kickBL->getSystem()!=-1&&kickBL->getP()>0.) // if META correlated
	    {   
	      kBl_tof[kBl_count] = kickBL->getTof();
	      kBl_sys[kBl_count] = kickBL->getSystem();
	      kBl_len[kBl_count] = kBl_tof[kBl_count]*kickBL->getBeta()*C*1000.;
	      kBl_tof[kBl_count] = kickBL->getTof()+correct[kBl_sys[kBl_count]];
	      kBl_p[kBl_count] = kickBL->getP()*fFieldCorrection;
	      kBl_pol[kBl_count] = kickBL->getPolarity();
	      
	      if (fRichMode&&rich_count) // if RICH mode and RICH hit
		{
		  Float_t kBl_theta, kBl_phi;
		  Int_t kBl_sec;
		  kBl_theta = kickBL->getTheta()*TMath::RadToDeg();
		  kBl_sec   = kickBL->getSector();
		  
		  kBl_phi=fmod(kBl_sec*60.+kickBL->getPhi()*TMath::RadToDeg(),360.);
		  
		  for (Int_t rr=0; rr<rich_count; rr++) // begin of RICH loop
		    {
		      if (kBl_sec==rich_sec[rr]) // if sector correlated
			{
			  chi2 = (kBl_theta-rich_theta[rr]-rich_params[0])*(kBl_theta-rich_theta[rr]-rich_params[0])/rich_params[1]/rich_params[1]+(kBl_phi-rich_phi[rr]-rich_params[2])*(kBl_phi-rich_phi[rr]-rich_params[2])/rich_params[3]/rich_params[3]; // chi2 of rich-track
			  if (fDebugMode) // fill RICH debug histos
			    {
			      richcorr->Fill(kBl_theta,rich_theta[rr],kBl_theta-rich_theta[rr],kBl_phi,rich_phi[rr],kBl_phi-rich_phi[rr],chi2,chi2<rich_params[4],3); 
			    }
			 
			  if (chi2<rich_params[4]) // if RICH correlation
			    {
			      rec_mass = HPidPhysicsConstants::mass(3); // e-
			      // search for tracklet with minimum tof
			      if (!isRich)
				{
				  rec_ind = kBl_count;
				}
			      else if (kBl_tof[kBl_count]<kBl_tof[rec_ind])
				{
				  rec_ind = kBl_count;
				}
			      // end of search
			      isRich = 1;
			    } // end of RICH correlation
			} // end of sector correlation
		    } // end of RICH loop
		} // end of RICH mode on
	      
	      if (!isRich&&fMinusMode&&kBl_pol[kBl_count]<0) // if !RICH but MINUS
		{
		  rec_mass = HPidPhysicsConstants::mass(8);  // pi+
		  // search for track with higher momentum
		  if (!isMinus)
		    {
		      rec_ind = kBl_count;
		    }
		  else if (kBl_tof[kBl_count]<kBl_tof[rec_ind])
		    {
		      rec_ind = kBl_count;
		    }
		  // end of search
		  isMinus = 1;
		} // end of MINUS
	      kBl_count++;
	    } // end of META correlated
	} // end of lowres kick B loop
      
      if (rec_ind>=0&&kBl_p[rec_ind]!=0) // if a track is indentified -> reconstruct start time
	{
	  start_tof = kBl_len[rec_ind]*sqrt(kBl_p[rec_ind]*kBl_p[rec_ind]+rec_mass*rec_mass)/(1000*C*kBl_p[rec_ind])-kBl_tof[rec_ind];
	}
      
      // reconstruct tof of other tracks in the event
      if (isRich)
	{
	  track_recalc(3,start_tof,1);
	}
      else if (isMinus)
	{
	  track_recalc(3,start_tof,2);
	}
    }
  
  // ************  end of LOWRES KICKTRACK BASE region   *************
  
  
  
  // ************    HIRES KICKTRACK   region    *************
  
  if (fKickHires)
    {
      Float_t kh_tof[20], kh_p[20], kh_len[20];
      Float_t start_tof = 0, rec_mass = 0;
      Float_t chi2;
      Int_t kh_pol[20], kh_sys[20];
      Int_t kh_count  = 0;
      Int_t rec_ind   = -1;
      Int_t isMinus   = 0;
      Int_t isRich    = 0;
      
      iterKickHires->Reset();
      while ((kickH=(HKickTrack123B *)iterKickHires->Next()) != 0) // begin of hires loop
	{
	  if (kickH->getSystem()!=-1&&kickH->getP()>0.) // if META correlated
	    {
	      kh_tof[kh_count] = kickH->getTof();
	      kh_sys[kh_count] = kickH->getSystem();
	      kh_len[kh_count] = kh_tof[kh_count]*kickH->getBeta()*C*1000.;
	      kh_tof[kh_count] = kickH->getTof()+correct[kh_sys[kh_count]];
	      kh_p[kh_count] = kickH->getP()*fFieldCorrection;
	      kh_pol[kh_count] = kickH->getPolarity();
	      
	      if (fRichMode&&rich_count) // if RICH mode on and RICh hit
		{
		  Float_t kh_theta, kh_phi;
		  Int_t kh_sec;
		  kh_theta = kickH->getTheta()*TMath::RadToDeg();
		  kh_sec   = kickH->getSector();
		  
		  kh_phi=fmod(kh_sec*60.+kickH->getPhi()*TMath::RadToDeg(),360.);
		  
		  for (Int_t rr=0; rr<rich_count; rr++) // RICH loop
		    {
		      if (kh_sec==rich_sec[rr]) // sector correlation
			{ 
			  chi2 = (kh_theta-rich_theta[rr]-rich_params[0])*(kh_theta-rich_theta[rr]-rich_params[0])/rich_params[1]/rich_params[1]+(kh_phi-rich_phi[rr]-rich_params[2])*(kh_phi-rich_phi[rr]-rich_params[2])/rich_params[3]/rich_params[3]; // chi2 of rich-track
			  if (fDebugMode) // fill RICH debug histos
			    {
			      richcorr->Fill(kh_theta,rich_theta[rr],kh_theta-rich_theta[rr],kh_phi,rich_phi[rr],kh_phi-rich_phi[rr],chi2,chi2<rich_params[4],2);
			    }
			  
			  if (chi2<rich_params[4]) // if RICH correlation
			    {
			      rec_mass = HPidPhysicsConstants::mass(3); // e-
			      // search for tracklet with minimum tof
			      if (!isRich)
				{
				  rec_ind = kh_count;
				}
			      else if (kh_tof[kh_count]<kh_tof[rec_ind])
				{
				  rec_ind = kh_count;
				}
			      // end os search
			      isRich = 1;
			    } // end of RICH correlation
			} // end of sector correlation
		    } // end of RICH loop
		} // end of RICH mode
	      
	      if (!isRich&&fMinusMode&&kh_pol[kh_count]<0) // if !RICH but MINUS
		{
		  rec_mass = HPidPhysicsConstants::mass(8); // pi+
		  // seacrh for track with higher momentum
		  if (!isMinus)
		    {
		      rec_ind = kh_count;
		    }
		  else if (kh_tof[kh_count]<kh_tof[rec_ind])
		    {
		      rec_ind = kh_count;
		    }
		  // end of search
		  isMinus = 1;
		} // end of !RICH but MINUS
	      kh_count++;
	    } // end of META correlated
	} // end of HIRES loop
      
      if (rec_ind>=0&&kh_p[rec_ind]!=0) // if identified track -> reconstruct start time
	{
	  start_tof = kh_len[rec_ind]*sqrt(kh_p[rec_ind]*kh_p[rec_ind]+rec_mass*rec_mass)/(1000*C*kh_p[rec_ind])-kh_tof[rec_ind];
	}
      
      // recalculate tof of other tracks in the event
      if (isRich)
	{
	  track_recalc(2,start_tof,1);
	} 
      else if (isMinus)
	{
	  track_recalc(2,start_tof,2);
	}
      
    }
  // ************   end of  HIRES KICKTRACK   region    *************
  
  return 0;
}


Bool_t HTofRec::finalize(void){
  if(fDebugMode)
    {
      r = TFile::Open("test_tofrec.root","RECREATE");
      after   ->Write();
      before  ->Write();
      richcorr->Write();
      
      r->Save();
      r->Close();
      r->Delete();
    }
  
  return kTRUE;
}


void HTofRec::track_recalc(Int_t tracking, Float_t start, Int_t flag)
{
  Float_t tof_new,tof_old, beta, mass2,p, tof_id;
  Int_t tofino_mult = -1;
    
  if (tracking==0)
    {
      HSplineTrack *spline_ch;
      iterSpline->Reset();
      while ((spline_ch=(HSplineTrack *)iterSpline->Next()) != 0)
	{
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
			       flag,
			       tracking);
		}
	      
	      
	      tof_old = spline_ch->getTof();
	      p       = spline_ch->getP() * fFieldCorrection;
	      tof_new = spline_ch->getTof()+correct[spline_ch->getSystem()]+start;
	      beta    = spline_ch->getTof()*spline_ch->getBeta()/tof_new;
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      
	      spline_ch->setTof(tof_new);
	      spline_ch->setBeta(beta);
	      spline_ch->setMass2(mass2,0);
	      spline_ch->setTofFlag(flag);
	      
	      if(fillNTuple)
		{
		  if (spline_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(spline_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		      tofino_mult = Tofino->getTofinoMult();
		    }
		  else
		    {
		      HTofHit *Tof= (HTofHit *)fTofCat->getObject(spline_ch->getTofHitInd());
		      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(spline_ch->getP(),
			      spline_ch->getPolarity(),
			      spline_ch->getBeta(),
			      spline_ch->getTof(),
			      spline_ch->getMass2(),
			      spline_ch->getSystem(),
			      spline_ch->getTofFlag(),
			      tracking,
			      //			      tof_new-tof_old,
			      start,
			      spline_ch->getTheta(),
			      spline_ch->getPhi(),
			      spline_ch->getSector(),
			      tof_id,
			      tofino_mult);
		}
	    }
	}
    }
  
  if (tracking==1)
    {
      HKickTrack *kick_ch;
      iterKickLowres->Reset();
      while ((kick_ch=(HKickTrack *)iterKickLowres->Next()) != 0)
	{
	  if (kick_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(kick_ch->getP(),
			       kick_ch->getCharge(),
			       kick_ch->getBeta(),
			       kick_ch->getTof(),
			       kick_ch->getMass(),
			       kick_ch->getSystem(),
			       flag,
			       tracking);
		}
	      
	      tof_old = kick_ch->getTof();
	      p       = kick_ch->getP()*fFieldCorrection;
	      tof_new = kick_ch->getTof()+correct[kick_ch->getSystem()]+start;
	      beta    = kick_ch->getTof()*kick_ch->getBeta()/tof_new;
	      
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      kick_ch->setTof(tof_new);
	      kick_ch->setBeta(beta);
	      kick_ch->setMass(mass2,0);
	      kick_ch->setTofFlag(flag);
	      
	      if(fillNTuple)
		{
		  if (kick_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(kick_ch->getOuterHitId());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		      tofino_mult = Tofino->getTofinoMult();
		    }
		  else
		    { 
		      tof_id = 64*kick_ch->getSector()+8*((kick_ch->getOuterHitId()%1000)/100)+((kick_ch->getOuterHitId()%1000)%100);
		    }
		  after->Fill(kick_ch->getP(),
			      kick_ch->getCharge(),
			      kick_ch->getBeta(),
			      kick_ch->getTof(),
			      kick_ch->getMass(),
			      kick_ch->getSystem(),
			      kick_ch->getFlag(),
			      tracking,
			      start,
			      //tof_new-tof_old,
			      kick_ch->getTheta(),
			      kick_ch->getPhi(),
			      kick_ch->getSector(),
			      tof_id,
			      tofino_mult);
		}
	    }
	}
    }
  
  if (tracking==3)
    {
      HKickTrackB *kick_ch;
      iterKickLowresB->Reset();
      while ((kick_ch=(HKickTrackB *)iterKickLowresB->Next()) != 0)
	{
	  if (kick_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(kick_ch->getP(),
			       kick_ch->getPolarity(),
			       kick_ch->getBeta(),
			       kick_ch->getTof(),
			       kick_ch->getMass2(),
			       kick_ch->getSystem(),
			       flag,
			       tracking);
		}
	      
	      tof_old = kick_ch->getTof();
	      p       = kick_ch->getP()*fFieldCorrection;
	      tof_new = kick_ch->getTof()+correct[kick_ch->getSystem()]+start;
	      beta    = kick_ch->getTof()*kick_ch->getBeta()/tof_new;
	      
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      kick_ch->setTof(tof_new);
	      kick_ch->setBeta(beta);
	      kick_ch->setMass2(mass2,0);
	      kick_ch->setTofFlag(flag);
	      
	      if(fillNTuple)
		{ 
		  if (kick_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(kick_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		      tofino_mult = Tofino->getTofinoMult();
		    }
		  else
		    {
                      HTofHit *Tof= (HTofHit *)fTofCat->getObject(kick_ch->getTofHitInd());
                      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(kick_ch->getP(),
			      kick_ch->getPolarity(),
			      kick_ch->getBeta(),
			      kick_ch->getTof(),
			      kick_ch->getMass2(),
			      kick_ch->getSystem(),
			      kick_ch->getTofFlag(),
			      tracking,
			      start,
			      //tof_new-tof_old,
			      kick_ch->getTheta(),
			      kick_ch->getPhi(),
			      kick_ch->getSector(),
			      tof_id,
			      tofino_mult);
		}
	    }
	}
    }
  
  if (tracking==2)
    {
      HKickTrack123B *kick_ch;
      iterKickHires->Reset();
      while ((kick_ch=(HKickTrack123B *)iterKickHires->Next()) != 0)
	{
	  if (kick_ch->getSystem()!=-1)
	    {
	      if(fillNTuple)
		{
		  before->Fill(kick_ch->getP(),
			       kick_ch->getPolarity(),
			       kick_ch->getBeta(),
			       kick_ch->getTof(),
			       kick_ch->getMass2(),
			       kick_ch->getSystem(),
			       flag,
			       tracking);
		}
	      
	      tof_old =  kick_ch->getTof();
	      p       = kick_ch->getP()*fFieldCorrection;
	      tof_new = kick_ch->getTof()+correct[kick_ch->getSystem()]+start;
	      beta    = kick_ch->getTof()*kick_ch->getBeta()/tof_new;
	      mass2 = p*p*(1-beta*beta)/beta/beta;
	      
	      kick_ch->setTof(tof_new);
	      kick_ch->setBeta(beta);
	      kick_ch->setMass2(mass2,0);
	      kick_ch->setTofFlag(flag);
	      
	      if(fillNTuple)
		{
		  if (kick_ch->getSystem()==0) 
		    { 
		      HShowerHitTof *Tofino =  (HShowerHitTof *)fTofinoCat->getObject(kick_ch->getShowerHitInd());
		      tof_id = Tofino->getTofinoCell()+4*Tofino->getSector()+384;
		      tofino_mult = Tofino->getTofinoMult();
		    }
		  else
		    {
                      HTofHit *Tof= (HTofHit *)fTofCat->getObject(kick_ch->getTofHitInd());
                      tof_id = 64*Tof->getSector()+8*Tof->getModule()+Tof->getCell();
		    }
		  after->Fill(kick_ch->getP(),
			      kick_ch->getPolarity(),
			      kick_ch->getBeta(),
			      kick_ch->getTof(),
			      kick_ch->getMass2(),
			      kick_ch->getSystem(),
			      kick_ch->getTofFlag(),
			      tracking,
			      start,
			      //tof_new-tof_old,
			      kick_ch->getTheta(),
			      kick_ch->getPhi(),
			      kick_ch->getSector(),
			      tof_id,
			      tofino_mult);
		}
	    }
	}
    }
  
}


