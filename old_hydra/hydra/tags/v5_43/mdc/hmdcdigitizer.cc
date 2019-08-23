#include "hgeantmdc.h"
#include "hmdcdigitizer.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hmdcgeantcell.h"
#include "hmdccal1sim.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdclayergeompar.h"
#include "hmdcdigitpar.h"
#include "hmdccal2parsim.h"
#include "hmdccelleff.h"
#include <iostream.h>
#include <TMath.h>
#include <TFile.h>

//*-- Author : A.Nekhaev
//*-- Modified: 05/11/2001 by J.Markert
//*-- Modified: 1/12/2000 by R. Holzmann
//*-- Modified: 30/07/99 by Ilse Koenig
//*-- Modified: 28/06/99 by Alexander Nekhaev

////////////////////////////////////////////////////////////////
//
//  HMdcDigitizer digitizes data and puts output values into 
//  CAL1 category for simulated data
//  OPTIONS:
//
//  HMdcDigitizer("","",option,flag)
//
//  option=1   : TDC mode for two measured leading edges of two signals
//  option=2   : TDC mode for  measured leading and trailing edge of one signal
//  flag=kTRUE : NTuple with internal variables of the digitizer will be filled and written
//               to the outputfile digitizer.root inside the working directory
//  flag=kFALSE: No NTuple is filled
//  setOffsets(1.5,2.5,4.5,5.5) sets offsets which will be substracted from the
//                              calculated time1 and time2 for each type of module
//  setEffLevel(10.,10.,10.,10.) sets level of cell efficiency cut for each type of module
//  setNoTof() time of flight will not be added to drift times
//
//  transform() - check whether the particle goes through two
//                neigboring cells or not, at the present
//                only a situation with two crossed neighboring
//                cells is taken into account
//  perp()      - define the shortest distance between the sense wire
//                and the trajectory of the particle.
//
//  cellNum()   - define the number of crossed cell(s)
//  storeCell() - put the data into GeantCell category 
//
//
// SHORT INTRODUCTION to HMdcDigitizer:
//
// The Digitizer retrieves the Geant data from
// HGeantMdc (sector,module,layer, hit: xcoord,ycoord,tof,ptot, incidence:theta,phi,tracknumber).
// HMdcDigitizer::transform(Float_t xcoord, Float_t ycoord, Float_t theta,
//                          Float_t phi   , Float_t tof   , Int_t trkNum) calculates
// which cells have been hit by the track. The Information about the layer geometry
// (pitch, cathod distance, wire orientation, number of wires per layer and number of central wire)
// is taken from HMdcLayerGeomPar. Output of the calculation is the minimum distance from the wire
// and the impact angle in the coordinate system of the cell. All values are stored in the
// HMdcGeantCell for a maximum number of 15 hits per cell via.
// HMdcDigitizer::storeCell(Float_t per, Float_t tof, Float_t myangle, Int_t trkNum,
//			    Bool_t flagCutEdge).
// For each cell the drift time1 and time2 are calculated by corresponding functions of
// the HMdcCal2ParSim container which holds the calibration parameters for the "distance->drift time"
// calculation:
// HMdcCal2ParSim::calcTimeDigitizer (sector,module,angle,minDist,&time1,&time1Error) and
// HMdcCal2ParSim::calcTime2Digitizer(sector,module,angle,minDist,&time2,&time2Error).
// For each cell the efficiency cuts are calculated by a function of
// HMdcCellEff::calcEfficiency(module,minDist,angle,Level) which holds
// the information for the efficiency cuts on cell level. The level of the cuts can be specified
// by  HMdcDigitizer::setEffLevel(10.,10.,10.,10.) per module. The cut is done on the basis of GARFIELD
// simulations which give information on the charge which is collected on the sense wire of
// each cell for all combinations of distance and impact angle. The numbers which have to be set
// are the percentage of maximum charge required to make a signal. Hits which are on the edge of
// the drift cell will not create a big amount of charge and therefore will be kicked out first.
// The second cut on the layer level is an overall layer efficiency (e.g. 0.98 for 98% Efficiency)
// and will reduce the overall number of fired cells. This value is taken from HMdcDigiPar container.
// According to the the cuts a list of status flags for each recorded track is filled.
// After all calculations the list of Tracks is sorted by the arrival time (tof + drift time) by
// HMdcDigitizer::select(Float_t* dTime,  Float_t* dTimeErr,
// 			 Float_t* dTime2, Float_t* dTime2Err,
//			 Float_t* timeOfFlight, Float_t* angle,
//			 Int_t* status, Int_t* track, Int_t nHits)
// because only the first track will create a signal. The first valid hit (status=1) inside
// the track list is be found by
// HMdcDigitizer::findFirstValidHit(Int_t* statusflag, Float_t* time1, Float_t* time2,
// 		        	    Int_t* firsthit, Float_t* firsttime2, Int_t* endlist1)
// which returns the index number of the first valid hit and the the last hit which falls
// into the given time window defined by time2 of first valid hit.
// HMdcDigitizer::findSecondValidHit(Int_t endlist1,Int_t* statusflag, Float_t* time1,
//                                   Float_t* time2,Int_t* secondhit)
// finds a second valid hit starting with the last entry of the list of the first valid hit.
// All variables will return -999 if no valid hits are found.
// According to the two different TDC modes the HMdcCal1Sim category is filled.
//
// MODE 1 (two times LEADING EDGE of the TDC signal)
//           nHits ==  -2 for 2 valid hits
//                 ==  -1 for 1 valid hit
//                 ==   0 for a not filled hit (e.g. 1 hit was kicked out by efficiency)
//         status1 ==   1 for a valid first hit
//                 ==  -3 for a not valid hit
//         status2 ==   1 for a valid second hit
//                 ==  -3 for a not valid hit
//         nTrack1 ==   track number of the first valid hit
//                 ==  -99 if not filled
//         nTrack2 ==   track number of the second valid hit
//                 ==  -99 if not filled
//         time1   ==   drift time1 of the first valid hit
//                 ==  -999 if not filled
//         time2   ==   drift time1 of the second valid hit
//                 ==  -999 if not filled
//     listTrack[5] :   contains the track number of the first 5 hits per cell
//                      == -99 if no hit was filled
//     listStatus[5]:   contains the status flags of the first 5 hits per cell
//                      == -1 if hit was kicked out by cell efficiency cut
//                      == -2 if hit was kicked out by layer efficiency cut
//                      ==  1 if hit is valid
//                      == -99 if no hit was filled
//     both lists will be filled even if no vaild hit was found
//
// MODE 2 (LEADING AND TRAILING EDGE of the TDC signal)
//           nHits ==  +2 for 2 valid hits
//                 ==   0 for not filled hit (e.g. 1 hit was kicked out by efficiency)
//         status1 ==   1 for a valid first hit
//                 ==  -3 for a not valid hit
//         status2 ==   1 for a valid first hit
//                 ==  -3 for a not valid hit
//         nTrack1 ==   track number of first valid hit
//                 ==  -99 if not filled
//         nTrack2 ==   track number of first valid hit
//                 ==  -99 if not filled
//         time1   ==   drift time1 of the first valid hit
//                 ==  -999 if not filled
//         time2   ==   drift time2 of the first valid hit
//                 ==  -999 if not filled
//     listTrack[5] :   contains the track number of the first 5 hits per cell
//                      == -99 if no hit was filled
//     listStatus[5]:   contains the status flags of the first 5 hits per cell
//                      == -1 if hit was kicked out by cell efficiency cut
//                      == -2 if hit was kicked out by layer efficiency cut
//                      ==  1 if hit is valid
//                      ==  0 if no hit was filled
//     both lists will be filled even if no vaild hit was found
//
////////////////////////////////////////////////////////////////


HMdcDigitizer::HMdcDigitizer(void) {
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  offsets[0]=1.5;
  offsets[1]=2.5;
  offsets[2]=4.5;
  offsets[3]=5.5;
  useOffsets=kFALSE;
  noTof=kFALSE;
  time1=0;
  time1Error=0;
  time2=0;
  time2Error=0;
  fntuple = kFALSE;
  modetdc=1;
  Level[0]=10.;
  Level[1]=10.;
  Level[2]=10.;
  Level[3]=10.;
  useCellEff=kFALSE;
}

HMdcDigitizer::HMdcDigitizer(Text_t *name,Text_t *title,Int_t mode,Bool_t hst) :
               HReconstructor(name,title) {
  fGeantCellCat = 0;
  fCalCat = 0;
  fDigitGeomPar = 0;
  fDigitPar = 0;
  iterin = 0;
  itercell = 0;
  fEventId = 0;
  pi = acos(-1.)/180;
  offsets[0]=1.5;
  offsets[1]=2.5;
  offsets[2]=4.5;
  offsets[3]=5.5;
  useOffsets=kFALSE;
  noTof=kFALSE;
  time1=0;
  time1Error=0;
  time2=0;
  time2Error=0;
  fntuple = hst;
  modetdc=mode;
  Level[0]=10.;
  Level[1]=10.;
  Level[2]=10.;
  Level[3]=10.;
  useCellEff=kFALSE;
  }

HMdcDigitizer::~HMdcDigitizer(void) {
  

  if(iterin) delete iterin;
  if(itercell) delete itercell;
}

void HMdcDigitizer::setParContainers() {
    // Get pointers to the needed containers.The containers are
    // created and added to the runtime Database if the are not existing

    fDigitGeomPar = (HMdcLayerGeomPar*)gHades->getRuntimeDb()
	->getContainer("MdcLayerGeomPar");
    if (!fDigitGeomPar) {
	fDigitGeomPar=new HMdcLayerGeomPar;
	gHades->getRuntimeDb()->addContainer(fDigitGeomPar);
    }
    fDigitPar=(HMdcDigitPar*)gHades->getRuntimeDb()->getContainer("MdcDigitPar");
    if (!fDigitPar) {
	fDigitPar=new HMdcDigitPar;
	gHades->getRuntimeDb()->addContainer(fDigitPar);
    }
    fCal2ParSim=(HMdcCal2ParSim*)gHades->getRuntimeDb()->getContainer("MdcCal2ParSim");
    if (!fCal2ParSim) {
	fCal2ParSim=new HMdcCal2ParSim;
	gHades->getRuntimeDb()->addContainer(fCal2ParSim);
    }
    fCellEff=(HMdcCellEff*)gHades->getRuntimeDb()->getContainer("MdcCellEff");
    if (!fCellEff) {
	fCellEff=new HMdcCellEff;
	gHades->getRuntimeDb()->addContainer(fCellEff);
    }
}

void HMdcDigitizer::setNTuples(void) {
    myoutput->cd();
    distance_time = new TNtuple("cal2sim", "cal2sim", "sec:mod:lay:cell:dist:angle:time1:time1Err:time2:time2Err:tof:cutEdge:status");
}

Bool_t HMdcDigitizer::init(void) {
    // The parameter containers and the iterators over
    // the categorys MdcGeanRaw, MdcGeantCell MdcCal1 are created.
    // The actual setup of the Mdc detector in the running analysis
    // is retrieved.
    setParContainers();
    getMdcSetup();

    fGeantMdcCat = (gHades->getCurrentEvent()->getCategory(catMdcGeantRaw));
  if(!fGeantMdcCat) {
      Error("HMdcDigitizer::init()","HGeant MDC input missing");
      return kFALSE;
  }
  iterin = (HIterator*)fGeantMdcCat->MakeIterator("native");

  fGeantCellCat = gHades->getCurrentEvent()->getCategory(catMdcGeantCell);
  if (!fGeantCellCat) {
      fGeantCellCat = gHades->getSetup()->getDetector("Mdc")
	  ->buildCategory(catMdcGeantCell);
      if (!fGeantCellCat) return kFALSE;
      else gHades->getCurrentEvent()->
	  addCategory(catMdcGeantCell,fGeantCellCat,"Mdc");
  }
  fGeantCellCat->setPersistency(kFALSE);   // We don't want to write this one
  itercell = (HIterator*)fGeantCellCat->MakeIterator("native");

  fCalCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!fCalCat) {
      HMdcDetector* mdc=(HMdcDetector*)(gHades->getSetup()->getDetector("Mdc"));
      fCalCat=mdc->buildMatrixCategory("HMdcCal1Sim",0.5F);
      if (!fCalCat) return kFALSE;
      else gHades->getCurrentEvent()->addCategory(catMdcCal1,fCalCat,"Mdc");
  } else {
      if (fCalCat->getClass()!=HMdcCal1Sim::Class()) {
	  Error("HMdcDigitizer::init()","Misconfigured output category");
	  return kFALSE;
      }
  }

  
  if(fntuple)
  {
      myoutput=new TFile("digitizer.root","RECREATE");
      myoutput->cd();
      setNTuples();
  }

  cout<<"****************************HMdcDigitizer****************************"<<endl;
  cout<<"*  options input 1 (default)      two leading edges                 *"<<endl;
  cout<<"*                2                leading and trailing edge         *"<<endl;
  cout<<"*  NTuple        kFALSE (default) no NTuple filled                  *"<<endl;
  cout<<"*                kTRUE       NTuple in digitizer.root filled        *"<<endl;
  cout<<"*                                                                   *"<<endl;
  cout<<"*  actual configuration:                                            *"<<endl;

  if(useOffsets)
  {
  cout<<"*  Offsets for fastest particles will be subtracted                 *"<<endl;
  cout<<"*  offsets: MDC1="<<offsets[0]<<" MDC2="<<offsets[1]<<" MDC3="<<offsets[2]<<" MDC4="<<offsets[3]<<"                     *"<<endl;
  }
  if(!useOffsets)
  {
  cout<<"*  no Offsets will be subtracted (default)                          *"<<endl;
  }
  if(noTof)
  {
  cout<<"*  Time of Flight will be substracted                               *"<<endl;
  }
  if(!noTof)
  {
  cout<<"*  Time of Flight will be included  (default)                       *"<<endl;

  }
  if(fntuple)
  {
  cout<<"*  NTuple in output file digitizer.root will be filled              *"<<endl;
  }
  if(!fntuple)
  {
  cout<<"*  no NTuple will be filled                                         *"<<endl;
  }
  if(modetdc==1) //two times leading edge
  {
  cout<<"*  TDC mode: two leading edges                                      *"<<endl;
  }
  if(modetdc==2) //leading and trailing edge
  {
  cout<<"*  TDC mode: leading and trailing edge                              *"<<endl;
  }
  if(useCellEff)
  {
  cout<<"*  Level  : "<<Level[0]<<","<<Level[1]
      <<","<<Level[2]<<","
      <<Level[3]<<"                                             *"<<endl;
  }
  if(!useCellEff)
  {
  cout<<"*  Level  :  no Cell Efficiency cut used                            *"<<endl;
  }


  cout<<"*********************************************************************"<<endl;

  return kTRUE;
}
Bool_t HMdcDigitizer::finalize(void)
{
    // All histograms/NTuples are written to the output file
    if(fntuple)
    {
	myoutput->cd();
	distance_time->Write();
	myoutput->Save();
	myoutput->Close();
    }
    return kTRUE;
}

Int_t HMdcDigitizer::execute(void) {
  Float_t xcoord, ycoord, tof, theta, phi, ptot;
  Int_t trkNum;
  myalpha=0;
  HGeantMdc* fGeant;
  loc.set(4,0,0,0,0);   // location used to fill the HMdcGeantCell category

  iterin->Reset();
  while((fGeant=(HGeantMdc*)iterin->Next()) != NULL) {// loop over HGeant input
      loc[0] = (Int_t)(fGeant->getSector());
      loc[1] = (Int_t)(fGeant->getModule());

      if(!testMdcSetup(loc[0],loc[1]) ) continue; // checks if the module is present in the setup

      loc[2] = (Int_t)(fGeant->getLayer());
      //   loc[3] is filled in transform(...) with the cell number
      fGeant->getHit(xcoord, ycoord, tof, ptot);
      fGeant->getIncidence(theta, phi);
      trkNum = fGeant->getTrack();
      if(loc[2]<6) transform(xcoord,ycoord,theta,phi,tof,trkNum);// transform and store
  }

  HMdcGeantCell* fCell = 0;
  HMdcCal1Sim* fCal = 0;
  Float_t dTime [15];       // drift time1 + tof
  Float_t dTime2[15];       // drift time2 + tof
  Float_t dTimeErr [15];    // error of drift time1
  Float_t dTime2Err[15];    // error of drift time2

  Int_t track[15];          // track numbers
  Float_t timeOfFlight[15]; // tof
  Float_t angle[15];        // impact angle in coordinate system of the cell
  Int_t statusflag[15];     // flag for efficiency

  for(Int_t i=0;i<15;i++)
  {
   dTime [i]=0.;
   dTime2[i]=0.;
   dTimeErr [i]=0.;
   dTime2Err[i]=0.;
   track[i]=0;
   timeOfFlight[i]=0.;
   angle[i]=0.;
   statusflag[i]=0;
  }

  itercell->Reset();
  while ((fCell=(HMdcGeantCell *)itercell->Next()) != NULL) {
    loc[0] = fCell->getSector();
    loc[1] = fCell->getModule();

    if(!testMdcSetup(loc[0],loc[1]) ) continue; // checks if the module is present in the setup

    loc[2] = fCell->getLayer();
    loc[3] = fCell->getCell();
    //HMdcLayerGeomParLay& geomlayer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
    HMdcDigitParLayer& layer = (*fDigitPar)[loc[0]][loc[1]][loc[2]];
    fCal = (HMdcCal1Sim*)fCalCat->getSlot(loc);
    if (fCal) {
      fCal = new(fCal) HMdcCal1Sim;
      fCal->setAddress(loc[0],loc[1],loc[2],loc[3]);

// Digitisation procedure starts here:


// First TDC signal
      Int_t nHits = fCell->getNumHits();
      for(Int_t ihit = 0;ihit<nHits;ihit++){

	Float_t minDist    = fCell->getMinDist(ihit);
	angle[ihit]        = fCell->getImpactAngle(ihit);
	track[ihit]        = fCell->getNTrack(ihit);
	timeOfFlight[ihit] = fCell->getTimeFlight(ihit);

	fCal2ParSim->calcTimeDigitizer(loc[0],loc[1],angle[ihit],(minDist),&time1,&time1Error);
	dTime[ihit] =time1 + time1Error + fCell->getTimeFlight(ihit);
        dTimeErr[ihit] =time1Error;

	fCal2ParSim->calcTime2Digitizer(loc[0],loc[1],angle[ihit],(minDist),&time2,&time2Error);
	dTime2[ihit] =time2 + time2Error + fCell->getTimeFlight(ihit);
        dTime2Err[ihit] =time2Error;

	if(angle   [ihit]<=180&&   angle[ihit]>90)   angle[ihit]=180-angle   [ihit];

	if(useCellEff)
	{
	   statusflag[ihit]=fCellEff->calcEfficiency(loc[1],minDist,angle[ihit],Level[loc[1]]);
	}
	else
	{
           statusflag[ihit]=1;
	}
        // Efficiency of MDC can be less then 100%...
	if(gRandom->Rndm() >layer.getMdcEfficiency())
	{
	    switch (statusflag[ihit])
	    {

	    case   1: statusflag[ihit]=-2;  // if it is kicked out by layer efficiency
	    break;
	    case  -1: statusflag[ihit]=-1;; // if it was kicked out by cell efficiency
	    break;
	    default : statusflag[ihit]=-3;  // just control
	    break;
	    }

	}

	if(fntuple)
	 {
	    // fill Ntuple with internal information of the Digitizer

	    if(angle[ihit]<=90 && angle[ihit]>=0)
	    {
		distance_time->Fill(loc[0],
				    loc[1],
				    loc[2],
				    loc[3],
				    minDist,
				    angle[ihit],
				    dTime[ihit],
                                    dTimeErr[ihit],
				    dTime2[ihit],
                                    dTime2Err[ihit],
				    timeOfFlight[ihit],
				    fCell->getFlagCutEdge(ihit),
                                    statusflag[ihit]
				   );
	    }

	 }

      }

      if (nHits>1) select(dTime,dTimeErr,dTime2,dTime2Err,                     // sort all hits by arrival time (tof + drifttime)
			  timeOfFlight,angle,statusflag,track,nHits);

      firstHit    =-999;   // number of first valid hit
      secondHit   =-999;   // number of second valid hit
      firstTime2  =-999;   // time2 of first valid hit
      endList1    =-999;   // last hit in window of first valid hit

      fCell->clear();         // clear the container

      findFirstValidHit(statusflag,dTime,dTime2,&firstHit,&firstTime2,&endList1);

      if(modetdc==1||modetdc==2)  // both TDC modes
      {
         if(firstHit!=-999) // if a valid hit was found
	 {
	     if(useOffsets)
	     {
		 if(noTof)  fCal->setTime1(dTime[firstHit]-timeOfFlight[firstHit]-offsets[loc[1]]); // drift times - offsets are stored
		 if(!noTof) fCal->setTime1(dTime[firstHit]-offsets[loc[1]]); //  drift times + tof - offsets are stored
	     }
	     if(!useOffsets)
	     {
		 if(noTof)  fCal->setTime1(dTime[firstHit]-timeOfFlight[firstHit]); // only drift times are stored
		 if(!noTof) fCal->setTime1(dTime[firstHit]); //  drift times + tof are stored
	     }
	     fCal->setNTrack1(track[firstHit]);
             fCal->setStatus1(1);
	 }
	 else
	 {
           fCal->setStatus1(-3); // no vaild hit1 found
	 }

	 fillTrackList(fCal,track,statusflag); // fill list of tracks and statusflags
	                                       // even if no valid hit was in
      }

      if(modetdc==2)  // leading and trailing edge
      {
	  if(firstHit!=-999) // if a valid hit was found
	  {
	      if(useOffsets)
	      {
		  if(noTof)  fCal->setTime2(dTime2[firstHit]-timeOfFlight[firstHit]-offsets[loc[1]]); // drift times - offsets are stored
		  if(!noTof) fCal->setTime2(dTime2[firstHit]-offsets[loc[1]]); // drift times + tof - offsets are stored
	      }
	      if(!useOffsets)
	      {
		  if(noTof)  fCal->setTime2(dTime2[firstHit]-timeOfFlight[firstHit]); // only drift times are stored
		  if(!noTof) fCal->setTime2(dTime2[firstHit]); // drift times + tof are stored
	      }
	      fCal->setNTrack2(track[firstHit]); // fill same track number as for time1
              fCal->setNHits(2);   // second hit = trailing edge
              fCal->setStatus2(1); // status is ok
	  }
	  else
	  {
             fCal->setStatus2(-3); // no vaild hit2 found
	  }
      }

      if (nHits==1 && modetdc==1) fCal->setNHits(-1);  // if only one hit was detected
      else
      {
	  if(nHits>1 && modetdc==1 && firstHit!=-999)  // two times leading edge
	  {
	      findSecondValidHit(endList1,statusflag,dTime,dTime2,&secondHit);

	      if (secondHit==-999 )
	      {
                  fCal->setNHits(-1); // if no valid hit2 was found
	      }
	      else
	      {
	          if(useOffsets)
		  {
		      if(noTof)  fCal->setTime2(dTime[secondHit]-timeOfFlight[secondHit]-offsets[loc[1]]);  // only drift times - offsets are stored
		      if(!noTof) fCal->setTime2(dTime[secondHit]-offsets[loc[1]]);  //  drift times + tof - offsets are stored
		  }
		  if(!useOffsets)
		  {
		      if(noTof)  fCal->setTime2(dTime[secondHit]-timeOfFlight[secondHit]);  // only drift times are stored
		      if(!noTof) fCal->setTime2(dTime[secondHit]);  // drift times + tof are stored
		  }
                  fCal->setNTrack2(track[secondHit]); // number of second track is stored
		  fCal->setNHits(-2);   // second valid hit was found
		  fCal->setStatus2(1); // status of hit2 is ok
	      }
	  }
	  else if(nHits>1 && modetdc==1)
	  {
	      fCal->setStatus2(-3); // no valid second hit was found
	  }
      }
    }
  }
  return 0;
}

void HMdcDigitizer::fillTrackList( HMdcCal1Sim* fCal, Int_t *track, Int_t *status)
{
    // fills track list for tracks in both tdc modes

    Int_t array[5];
    Int_t array1[5];

    for(Int_t i=0;i<5;i++)
    {
	array[i]=track[i];
        array1[i]=status[i];
    }
    fCal->setTrackList (array);  // store array in cal1sim level
    fCal->setStatusList(array1); // store array in cal1sim level
}

void HMdcDigitizer::findFirstValidHit(Int_t* statusflag, Float_t* time1, Float_t* time2,
				      Int_t* firsthit, Float_t* firsttime2, Int_t* endlist1)
// Function to find the first valid hit ( status=1) inside the array for both tdc modes
//Returns number of element of first valid hit and last hit inside the time window to variables.
{
    Int_t hit=0;
    Int_t lasthit=0;
    while(statusflag[hit]!=1) // find first valid hit
    {
	lasthit++;
	hit++;
	if(hit==15)
	{
	    // if last element is reached without found
	    // condtion set flags and break
	    *firsthit=-999;
	    *firsttime2=-999;
            *endlist1=-999;
	    break;
	}
    }
    if(hit<15)
    {
	while(time1[lasthit]<=time2[hit] && lasthit<15)
	{
	    lasthit++; // last hist which falls into window of first hit
	}
	// set output values if condition was true
	*firsthit=hit;
	*firsttime2=time2[hit];

	if(lasthit<15)
	{
	    *endlist1=lasthit;
	}
	else
	{
	    *endlist1=-999;
	}
    }

}
void HMdcDigitizer::findSecondValidHit(Int_t endlist1,
				       Int_t* statusflag, Float_t* time1, Float_t* time2,
				       Int_t* secondhit)
// Function to find the second valid hit ( status=1) inside the array for tdc mode1.
// Returns number of element of second valid hit
{
    Int_t hit=endlist1+1;
    if(hit<15 && endlist1!=-999) // make sure that it is not last element and a valid first hit exist
    {
	while(statusflag[hit]!=1)
	{
	    // stop if status=1 and second hit starts after first hit ends
	    hit++;
	    if(hit==15)
	    {
		// if last element is reached without found
		// condtion set flags and break
		*secondhit=-999;
		break;
	    }
	}
	if(hit<15)
	{
            // set output values if condition was true
	    *secondhit=hit;
	}
    }
    else
    {
	// function was called with las element
        // and skipped to end => no valid secon hit found
	*secondhit=-999;
    }
}



void HMdcDigitizer::select(Float_t* dTime,Float_t* dTimeErr,
			   Float_t* dTime2,Float_t* dTime2Err,
			   Float_t* timeOfFlight, Float_t* angle,
			   Int_t* status,Int_t* track,Int_t nHits)
// Puts the drift time values into increasing order
// Orders the corresponding track number, time of flight and
// impact angle accordingly
{
  register Int_t a,b,c;
  Int_t exchange;
  Float_t t;
  Float_t t2;
  Float_t tErr;
  Float_t t2Err;

  Float_t flight;
  Float_t angleLocal;
  Int_t statlocal;
  Int_t tracklocal;

  for(a=0;a<nHits-1;++a)
  {
      exchange = 0;
      c = a;

      t = dTime[a];
      tErr = dTimeErr[a];
      t2 = dTime2[a];
      t2Err = dTimeErr[a];
      tracklocal = track[a];
      flight=timeOfFlight[a];
      angleLocal=angle[a];
      statlocal=status[a];

      for(b=a+1;b<nHits;++b)
      {
	  if(dTime[b]<t)
	  {
	      c = b;

	      t = dTime[b];
	      tErr = dTimeErr[b];
	      t2 = dTime[b];
	      t2Err = dTimeErr[b];
	      tracklocal = track[b];
	      flight=timeOfFlight[b];
	      angleLocal=angle[b];
	      statlocal=status[b];

	      exchange = 1;
	  }
      }
      if(exchange)
      {
	  dTime[c] = dTime[a];
	  dTime[a] = t;
	  dTimeErr[c] = dTimeErr[a];
	  dTimeErr[a] = tErr;
	  dTime2[c] = dTime2[a];
	  dTime2[a] = t2;
	  dTimeErr[c] = dTimeErr[a];
	  dTimeErr[a] = t2Err;
	  track[c] = track[a];
	  track[a] = tracklocal;
	  timeOfFlight[c] = timeOfFlight[a];
	  timeOfFlight[a] = flight;
	  angle[c] = angle[a];
	  angle[a] = angleLocal;
	  status[c]=status[a];
	  status[a]=statlocal;
      }
  }
}
void HMdcDigitizer::getMdcSetup()
// Gets Mdc detector setup
{
    HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");
    if (!mdcDet)
    {
	cout << "Error in HMdcDigitizer: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
    }
    for(Int_t s=0; s<6; s++) {
	for(Int_t m=0; m<4; m++) {
	    if (!mdcDet->getModule(s, m)) setup[s][m]=0;
	    if ( mdcDet->getModule(s, m)) setup[s][m]=1;
	}
    }
}

Bool_t HMdcDigitizer::testMdcSetup(Int_t s, Int_t m)
// tests the Mdc setup if the modules are present
// in the running analysis
{
    Bool_t result=kFALSE;
    if(setup[s][m]==0) result=kFALSE;
    if(setup[s][m]==1) result=kTRUE;
    return result;
}

Bool_t HMdcDigitizer::transform(Float_t xcoor,Float_t ycoor,Float_t theta,
                                Float_t phi  ,Float_t tof  ,Int_t trkNum) {

    // gets x,y coordinate,theta and phi, time of flight and track number
    // From the coordinates and angles the hits in the cells are calculated.
    // All needed parameters are taken from DigiPar container.

    HMdcLayerGeomParLay& layer = (*fDigitGeomPar)[loc[0]][loc[1]][loc[2]];
    Float_t pitch = layer.getPitch();                // pitch of the sense wires
    Float_t halfPitch   = 0.5*pitch;
    Float_t halfCatDist = 0.5*layer.getCatDist();    // distance of the cathode planes
    Int_t   nWires   = layer.getNumWires();          // number of wires per layer
    Float_t wOrient  = layer.getWireOrient()*pi;     // inclination of the wires
    Float_t centWire = layer.getCentralWireNr()-1.;  // wire number in physical center

    // Transformation from MDC's coordinate system to layer's coordinate system:
    Float_t y_wire = ycoor*cos(wOrient)-xcoor*sin(wOrient);

    Float_t ctanalpha = tan(theta*pi)*sin(phi*pi-wOrient);

    // recalculate the angle to the coordinatessystem of HMdcCal2ParSim
    myalpha=90-(atan((pi/2.)-ctanalpha)*180./acos(-1.));


    Float_t dY = halfCatDist*ctanalpha;
    dY<0.0 ? dY = -dY : dY;
    Float_t tmp = (centWire*pitch + y_wire+dY+halfPitch)/pitch;
    if(tmp < 0.0) return kFALSE;
    Int_t nCmax = (Int_t)tmp;
    tmp = (centWire*pitch + y_wire-dY+halfPitch)/pitch;
    Int_t nCmin;
    tmp < 0.0 ? nCmin=0 : nCmin=(Int_t)tmp;
    if(nCmin >= nWires) return kFALSE;
    nCmax>=nWires ? nCmax=nWires-1 : nCmax;

    for (loc[3]=nCmin; loc[3]<=nCmax; loc[3]++) {
	Float_t yDist = y_wire-(loc[3]*pitch-centWire*pitch);
	Float_t sinAlpha = sqrt(1./(1.+ctanalpha*ctanalpha));
	Float_t cosAlpha = sqrt(1.-sinAlpha*sinAlpha);

	Float_t per = fabs(yDist*sinAlpha);// minimum distance of track to the wire

	Bool_t flagCutEdge=kFALSE;
	if(per*sinAlpha > halfPitch) {  // check if per is inside cell (y)

	    flagCutEdge=kTRUE;

	} else if(per*cosAlpha > halfCatDist) { // check if per is inside cell (z)

	    flagCutEdge=kTRUE;
	}
	storeCell(per,tof,myalpha,trkNum,flagCutEdge);//store the final values in container
    }
    return kTRUE;
}

void HMdcDigitizer::storeCell(Float_t per,Float_t tof,Float_t myangle,Int_t trkNum
			      ,Bool_t flagCutEdge)
{
    // Puts data (minimum distance, time of flight , impact angle,
    // track number) to HMdcGeantCell Category

    hit = (HMdcGeantCell*)fGeantCellCat->getObject(loc);
    if (!hit) {
	hit = (HMdcGeantCell*)fGeantCellCat->getSlot(loc);
	hit = new(hit) HMdcGeantCell;
    }
    Int_t nHit;
    nHit = hit->getNumHits();
    if (nHit < 15 ) {  // only the first 15 hits are stored
	hit->setSector(loc[0]);
	hit->setModule(loc[1]);
	hit->setLayer(loc[2]);
	hit->setCell(loc[3]);
	hit->setNumHits(nHit+1);
	hit->setMinDist(per,nHit);
	hit->setTimeFlight(tof,nHit);
	hit->setImpactAngle(myangle,nHit);
	hit->setNTrack(trkNum,nHit);
	hit->setFlagCutEdge(flagCutEdge,nHit);
    }
}

ClassImp(HMdcDigitizer)
