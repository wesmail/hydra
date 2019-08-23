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
#include <iostream.h>
#include <TMath.h>
#include <TFile.h>

//*-- Author : A.Nekhaev
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
//
//
//  transform() - check either the particle go through 2
//                neigboring cells or not, at the present
//                only situation with two crossed neighboring
//                cells is taken into account
//  perp()      - define the shortest distance between sense wire
//                and trajectory of the particle.If the calculated minimum
//                distance is outside the cell,the distance and the impact angle
//                is recalculated to the shortest distance inside cell.
//
//  cellNum()   - define the number of crossed cell
//  storeCell() - put the data into GeantCell category 
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
  alpha=85.;
  fntuple = kFALSE;
  modetdc=1;
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
  alpha=85.;
  fntuple = hst;
  modetdc=mode;
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
}

void HMdcDigitizer::setNTuples(void) {
    myoutput->cd();
    distance_time = new TNtuple("cal2sim", "cal2sim", "sec:mod:lay:cell:dist:angle:time1:time1Err:time2:time2Err:tof:olddist:oldangle:cutEdge");
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
 // Float_t spaceResol; // space resolution for MDC
  Float_t dTime [15];  // drift time1 + tof
  Float_t dTime2[15];  // drift time2 + tof
  Float_t dTimeErr [15];  // error of drift time1
  Float_t dTime2Err[15];  // error of drift time2

  Int_t track[15];    // track numbers
  Float_t timeOfFlight[15]; // tof
  Float_t angle[15]; // impact angle in coordinate system of the cell
  Float_t oldangle[15]; // old impact angle in coordinate system of the cell

  // Position resolution, here one can define the type of this function.
  // Now the function has 5 parameters:
  // fSpacepar0, fSpacepar1, fSpacepar2, min, max
  for(Int_t i=0;i<15;i++)
  {
   dTime [i]=0.;
   dTime2[i]=0.;
   dTimeErr [i]=0.;
   dTime2Err[i]=0.;
   track[i]=0;
   timeOfFlight[i]=0.;
   angle[i]=0.;
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

// Efficiency of MDC can be less then 100%...
        (gRandom->Rndm()> layer.getMdcEfficiency()) ?
              fCal->setStatus(-1) : fCal->setStatus(1);


// Position resolution

	Float_t minDist = fCell->getMinDist(ihit);

	angle[ihit] = fCell->getImpactAngle(ihit);
        oldangle[ihit] = fCell->getOldImpactAngle(ihit);

	track[ihit] = fCell->getNTrack(ihit);
	timeOfFlight[ihit]= fCell->getTimeFlight(ihit);

	fCal2ParSim->calcTimeDigitizer(loc[0],loc[1],angle[ihit],oldangle[ihit],(minDist),&time1,&time1Error);
	dTime[ihit] =time1 + time1Error + fCell->getTimeFlight(ihit);
        dTimeErr[ihit] =time1Error;

	fCal2ParSim->calcTime2Digitizer(loc[0],loc[1],angle[ihit],oldangle[ihit],(minDist),&time2,&time2Error);
	dTime2[ihit] =time2 + time2Error + fCell->getTimeFlight(ihit);
        dTime2Err[ihit] =time2Error;
 

	if(fntuple)
	 {

            if(oldangle[ihit]<=180&&oldangle[ihit]>90)oldangle[ihit]=180-oldangle[ihit];
            if(angle   [ihit]<=180&&   angle[ihit]>90)   angle[ihit]=180-angle   [ihit];

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
				    fCell->getOldMinDist(ihit),
				    oldangle[ihit],
				    fCell->getFlagCutEdge(ihit)
				   );
	    }

	 }

      }


      if (nHits>1) select(dTime,dTimeErr,dTime2,dTime2Err,timeOfFlight,angle,oldangle,track,nHits);

      fCell->clear(); // clear the container
      if(modetdc==1||modetdc==2)  // both TDC modes
      {
	  if(useOffsets)
	  {
	      //cout<<"1"<<endl;
	      if(noTof)  fCal->setTime1(dTime[0]-timeOfFlight[0]-offsets[loc[1]]); // only drift times are stored
	      if(!noTof) fCal->setTime1(dTime[0]-offsets[loc[1]]); // only drift times are stored
	  }
	  if(!useOffsets)
	  {
	      //cout<<"2"<<endl;
	      if(noTof)  fCal->setTime1(dTime[0]-timeOfFlight[0]); // only drift times are stored
	      if(!noTof) fCal->setTime1(dTime[0]); // only drift times are stored
	  }
      fCal->setNTrack1(track[0]);
      }

      if(modetdc==2)  // leading and trailing edge
      {
	  if(useOffsets)
	  {
              //cout<<"3"<<endl;
	      if(noTof)  fCal->setTime2(dTime2[0]-timeOfFlight[0]-offsets[loc[1]]); // only drift times are stored
	      if(!noTof) fCal->setTime2(dTime2[0]-offsets[loc[1]]); // only drift times are stored
	  }
	  if(!useOffsets)
	  {
	      //cout<<"4"<<endl;
	      if(noTof)  fCal->setTime2(dTime2[0]-timeOfFlight[0]); // only drift times are stored
	      if(!noTof) fCal->setTime2(dTime2[0]); // only drift times are stored
	  }
      fCal->setNTrack2(track[0]);
      fCal->setNHits(2);
      }




      if (nHits==1 && modetdc==1) fCal->setNHits(1);
      else {

	  // TDC can distinguish two tracks if the distance between these tracks
	  // more than 50 microm.


	  const Float_t fac = 50.F/layer.getDriftVelocity();
	  Int_t second = 0;
	  Int_t i = 1;
	  while (second==0 && i<nHits) {
	      if ((dTime[i]-dTime[i-1])>=fac) second = i;
	      i++;
	  }
	  if (!second && modetdc==1) fCal->setNHits(1);
	  else {
              if(modetdc==1)  // two times leading edge
	      {
                  fCal->setNHits(2);
		  if(useOffsets)
		  {
                      //cout<<"5"<<endl;
		      if(noTof)  fCal->setTime2(dTime[second]-timeOfFlight[second]-offsets[loc[1]]);  // only drift times are stored
		      if(!noTof) fCal->setTime2(dTime[second]-offsets[loc[1]]);  // only drift times are stored

		      fCal->setNTrack2(track[second]);

		  }
		  if(!useOffsets)
		  {
                      //cout<<"6"<<endl;
		      if(noTof)  fCal->setTime2(dTime[second]-timeOfFlight[second]);  // only drift times are stored
		      if(!noTof) fCal->setTime2(dTime[second]);  // only drift times are stored

		      fCal->setNTrack2(track[second]);
		  }
	      }
	  }
      }
    }
  }
  return 0;
}

void HMdcDigitizer::select(Float_t* tof,Float_t* tofErr,Float_t* tof2,Float_t* tof2Err,Float_t* tdiff,Float_t* angle,Float_t* oldangle,Int_t* track,Int_t count)
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

  Int_t n;
  Float_t flight;
  Float_t angleLocal;
  Float_t oldangleLocal;

  for(a=0;a<count-1;++a) {
     exchange = 0;
     c = a;
     t = tof[a];
     tErr = tofErr[a];
     t2 = tof2[a];
     t2Err = tof2Err[a];
     n = track[a];
     flight=tdiff[a];
     angleLocal=angle[a];
     oldangleLocal=angle[a];

     for(b=a+1;b<count;++b) {
        if(tof[b]<t) {
	   c = b;
  	   t = tof[b];
           tErr = tofErr[b];
	   t2 = tof[b];
           t2Err = tofErr[b];
	   n = track[b];
           flight=tdiff[b];
           angleLocal=angle[b];
           oldangleLocal=angle[b];

	   exchange = 1;
        }
     }
     if(exchange) {
        tof[c] = tof[a];
        tof[a] = t;
        tofErr[c] = tofErr[a];
        tofErr[a] = tErr;
	tof2[c] = tof2[a];
        tof2[a] = t2;
        tof2Err[c] = tof2Err[a];
        tof2Err[a] = t2Err;
	track[c] = track[a];
        track[a] = n;
	tdiff[c] = tdiff[a];
        tdiff[a] = flight;
        angle[c] = angle[a];
        angle[a] = angleLocal;
        oldangle[c] = oldangle[a];
        oldangle[a] = oldangleLocal;

     }
  }
}
void HMdcDigitizer::getMdcSetup()
// Gets Mdc detector setup
//
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
	//per<0.0 ? per=-per : per;
	Float_t oldAngle=myalpha;
        Float_t oldDist=per;
	Bool_t flagCutEdge=kFALSE;
	if(per*sinAlpha > halfPitch) {  // check if per is inside cell (y)
	    /*
	     Float_t z1 = (yDist+halfPitch)/ctanalpha;
	     Float_t z2 = (yDist-halfPitch)/ctanalpha;
	     (z1*=z1)<(z2*=z2) ? per = sqrt(halfPitch*halfPitch+z1) :
	     per = sqrt(halfPitch*halfPitch+z2);
            */

	    // recalculate angle to minimum distance inside cell
	    flagCutEdge=kTRUE;
            /*
	    if(myalpha>=0&&myalpha<=90)
	    {
	       myalpha =90-(acos(halfPitch/per)*180./acos(-1.));
	    }
	    else
	    {
                myalpha =180-(90-(acos(halfPitch/per)*180./acos(-1.)));
	  
	    }    //per=per-1; // has to fixed
            */

	} else if(per*cosAlpha > halfCatDist) { // check if per is inside cell (z)
	    /*
	     Float_t y1 = halfCatDist*ctanalpha+yDist;
	     Float_t y2 = halfCatDist*ctanalpha-yDist;
	     (y1*=y1)<(y2*=y2) ? per = sqrt(y1+halfCatDist*halfCatDist) :
	     per = sqrt(y2+halfCatDist*halfCatDist);
	     */
	    // recalculate angle to minimum distance inside cell
	    flagCutEdge=kTRUE;
            /*
	    if(myalpha>=0&&myalpha<=90)
	    {
	       myalpha =90-(asin(halfCatDist/per)*180./acos(-1.));
	    }
	    else
	    {
	       myalpha =180-(90-(asin(halfCatDist/per)*180./acos(-1.)));
	    }
            */
	}
	storeCell(per,tof,myalpha,trkNum,oldDist,oldAngle,flagCutEdge);//store the final values in container
    }
    return kTRUE;
}

void HMdcDigitizer::storeCell(Float_t per,Float_t tof,Float_t myangle,Int_t trkNum,Float_t oldDist,Float_t oldAngle,Bool_t flagCutEdge) {
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
        hit->setOldMinDist(oldDist,nHit);
	hit->setOldImpactAngle(oldAngle,nHit);
	hit->setFlagCutEdge(flagCutEdge,nHit);
    }
}

ClassImp(HMdcDigitizer)
