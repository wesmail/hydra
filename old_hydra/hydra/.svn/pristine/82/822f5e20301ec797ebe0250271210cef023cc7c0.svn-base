using namespace std;
#include "TRandom.h"
#include "TMath.h"
#include <time.h>
#include <iostream>
#include <iomanip>
#include "htofinodigitizer.h"
#include "tofinodef.h"
#include "hdebug.h"
#include "hades.h"
#include "htool.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "htofinodetector.h"
#include "hgeanttof.h"
#include "hgeantkine.h"
#include "htofinocalsim.h"
#include "hevent.h"
#include "hpartialevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hrecevent.h"
#include "hlocation.h"
#include "htofinocalpar.h"
#include "htofinocalsimfilter.h"

//*-- Author : D.Vasiliev
//*-- Modified: 08/07/2006 by J.Otwinowski
//*-- Modified: 05/04/2003 by J.Otwinowski
// offset subtraction added
//*-- Modified: 01/03/2003 by J.Otwinowski && D.Zovinec
// ADC and TDC parts were modified to get ADC and TDC in channels
//
//*-- Modified: 26/10/2000 by M.Golubeva
//*-- Modified: 28/06/2000 by R.Holzmann
//*-- Modified: 24/06/2000 by M.Golubeva
//*-- Modified: 1/12/99 by V.Charriere
//*-- Modified: 24/10/99 by D.Vasiliev

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////
//
//  HTofinofDigitizer digitizes HGeant data, puts output values into
//  cal data category
//  The input data are read from the HGeantTof category. After calculating
//  TOF and Charge of the hit etc the output is stored in the HTofinoCalSim
//  category.
//
//   ----------------------                                 -----------
//  |   HTofinoCalibrater  |                               | HGeantTof |
//  |   (embedding mode)   | \                              -----------
//  |                      |  \                                 ||
//   ----------------------    \                                || input to digitizer
//                              \                               \/
//                           ---------------    read real     ------------------
//                          | HTofinoCalSim | ------------>  | HTofinoDigitizer |
//                           ---------------  <-----------   |                  |
//                                            write output    ------------------
//
//
//  In the case of TRACK EMBEDDING of simulated tracks into
//  experimental data the real data are written by the HTofinoCalibrater into
//  HTofinoCalSim category. The real hits are taken into
//  account by the digitizer (adding of charges, sorting by first hit in
//  photo multiplier). The embedding mode is recognized
//  automatically by analyzing the
//  gHades->getEmbeddingMode() flag.
//            Mode ==0 means no embedding
//                 ==1 realistic embedding (first real or sim hit makes the game)
//                 ==2 keep GEANT tracks   (as 1, but GEANT track numbers will always
//                     win against real data. besides the tracknumber the output will
//                     be the same as in 1)
//
//  The output object HTofinoCalSim contains additional to the tof and
//  charge a list of up to 5 tracks hitting the tofino cell. The track numbers
//  are stored by increasing tof. In parallel the tof from GEANT is stored in a
//  list. In case of embedding the first and second track recieved via getTrack()
//  contains real or geant data depending wheater or not there has been a
//  embedded track and sim or real was first. In case of no embedded track in the cell
//  the list contains only the geant hits.
//
//  tofinocal->getNHit()      number of hits in the cell (val 1-5)
//  tofinocal->getTrack(0)    contains real data / geant data (real if real was faster)
//  tofinocal->getTrack(1)    contains real data / geant data (sim  if real was faster)
//  tofinocal->getTrack(2)    contains geant data
//      .
//      .
//      .
//  tofinocal->getTrack(4)    contains geant data
//
// For the suppression of secondaries produced in the TOF itself there are
// several configuration possibilities:
// They can be switched via setStoreFirstTrack(Int_t flag):
//     flag ==  0 realistic (secondaries included)
//              1 primary particle is stored
//              2 (default) the first track number entering the tof in SAME SECTOR is stored
//              3 as 2 but SAME SECTOR && CELL
//
// To make the influence of the above selections easily visible
// in the output HTofCalSim a debug mode can be switched with
// void   setDebug(Bool_t flag). In this case the track numbers
// affected by the criteria 1-3 will have negative track numbers
// (multiples of -200). The multiplication factor gives the number
// of steps from the start point to the stored track (chain of
// secondaries). In case 1 it is always -200 if a particle has not
// been a primary one.
/////////////////////////////////////////////////////////////////////

ClassImp(HTofinoDigitizer)

HTofinoDigitizer*  HTofinoDigitizer::pTofinoDigi=NULL;

HTofinoDigitizer::HTofinoDigitizer(Text_t *name,Text_t *title) :
               HReconstructor(name,title) {

  fGeantCat      =NULL;
  fCalCat        =NULL;
  pTofinoDigitPar= NULL;
  fLoc.set(3,0,0,0);
  ffLoc.set(2,-1,-1);
  iterGeant      =NULL;
  iterCal        =NULL;
  array          =NULL;
  pTofinoDigi    =this;
  storeFirstTrack=2;
  debug          =kFALSE;

}

HTofinoDigitizer::HTofinoDigitizer(void) {

  fGeantCat      =NULL;
  fCalCat        =NULL;
  pTofinoDigitPar= NULL;
  fLoc.set(3,0,0,0);
  ffLoc.set(2,-1,-1);
  iterGeant      =NULL;
  iterCal        =NULL;
  array          =NULL;
  pTofinoDigi    =this;
  storeFirstTrack=2;
  debug          =kFALSE;

}

HTofinoDigitizer::~HTofinoDigitizer(void) {

	if(iterGeant) delete iterGeant;
	if(iterCal)   delete iterCal;
	if(array){
	    array->Delete();
	    delete array;
            array=NULL;
	}
}

Bool_t HTofinoDigitizer::init(void) {

  time_t curtime;
  initParContainer();

  fGeantCat = gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
  if (!fGeantCat) 
  {
 	fGeantCat = gHades->getSetup()->getDetector("Tof")->buildCategory(catTofGeantRaw);

    if (!fGeantCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofGeantRaw,fGeantCat,"Tof");
  }

  fGeantKineCat=(HLinearCategory*)gHades->getCurrentEvent()->getCategory(catGeantKine);
  if(!fGeantKineCat){
      Error("HTofinoDigitizer::init()","No catGeantKine in input!");
      return kFALSE;
  }

  fCalCat = gHades->getCurrentEvent()->getCategory(catTofinoCal);
  if (!fCalCat) 
  {
    fCalCat = ((HTofinoDetector*)(gHades->getSetup()->getDetector("Tofino")))
                                ->buildMatrixCategory("HTofinoCalSim");
    if (!fCalCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catTofinoCal,fCalCat,"Tofino");
  }

  iterGeant = (HIterator *)fGeantCat->MakeIterator("native");
  iterCal = (HIterator *)fCalCat->MakeIterator("native");

  time(&curtime);

  //-----------------------------------------------------------
  // create temporary array for HTofinoCalSim objects
  // the array will be used to collect the data before writing
  // to the output category
  if(array){
      Error("init()","Temporary already initialized! Check your setup...running TofinoDigitizer twice?");
      return kFALSE;
  }
  array=new TObjArray(24);
  for(Int_t i=0;i<24;i++){
      array->AddLast(new HTofinoCalSim);
  }
  //-----------------------------------------------------------




  return kTRUE;
}

Bool_t HTofinoDigitizer::initParContainer() {

  pTofinoDigitPar = (HTofinoDigitPar *) gHades->getRuntimeDb()->getContainer("TofinoDigitPar");
  if(pTofinoDigitPar == NULL)
  {
  	Error("Error in HTofinoDigitizer::initParContainer()","Container TofinoDigitPar not initialized");
	return kFALSE;
  }
  return kTRUE;
}

Int_t HTofinoDigitizer::execute(void) 
{
  const Int_t embeddingmode=gHades->getEmbeddingMode(); // flag embedding 1=realistic 2:keep geant

  HTofinoCalSim* pCal = NULL;
  HTofinoCalSimFilter fCalFilter;


  //-------------------------------------------------
  // loop over raw sim category and set the default values
  // for embedding mode the real data
  // have -1 as track id -> set to standard
  // track number for embedding and increment the the
  // hit counter
  if(embeddingmode>0){
      iterCal->Reset();
      while ((pCal=(HTofinoCalSim *)iterCal->Next())!=0)
      {
	  pCal->resetTrack(-1);  // reset tracknumber array to default
	  pCal->resetGeaTof(-1); // reset tof array to default
	  pCal->setNHit(1);      // increment counter for additional hits
	                         // (counting from 1 for first hit!)
	  pCal->fillTrack(0,gHades->getEmbeddingRealTrackId());
      }
  }
  //-------------------------------------------------


  //-------------------------------------------------
  // fill temporary array from geant.
  // all sim data will be collected and in the end
  // will be a check on the total charge etc.
  // before it is written to the output category (and in
  // embedding mode merged with the real data)
  fillArray();
  //-------------------------------------------------
  // now do the final check on collected data and
  // convert times from channel to nanosecond to write
  // to output category (and merge with real data)
  // fill list of tracks/geaTof other than the fastest hit
  doFinalCheckOnArray();
  fillOutput();
  //-------------------------------------------------

 
  //-------------------------------------------------
  // HTofinoCalSimFilter is a HFilter to reduce the number of the
  // HTofinoCalSim data in the catTofinocal category.
  // (If HTofinoCalSim object has Charge==0 && Time==0
  // it is deleted from the category.)
  fCalCat->filter(fCalFilter);
  //-------------------------------------------------

  return 0;
} // htofinodigitizer

Bool_t HTofinoDigitizer::finalize(void) 
{
return kTRUE;
}

void  HTofinoDigitizer::fillArray()
{
    //-------------------------------------------------
    // fill temporary array from geant.
    // all sim data will be collected and in the end
    // will be a check on the total charge etc. in doFinalCheckOnArray()
    // before it is written to the output category (and in
    // embedding mode merged with the real data) in fillOutput()

    const Float_t al = 2400.0;           // attenuation length
    const Float_t relAmplResol = 0.08;   // sigma of Gaus distribution.
    const Float_t minEnerRelease = 1.8;  // minimum energy release (MeV/cm)


    Float_t geaTof, geaEner, geaX, geaY, geaMom, trackLen;  // Geant output
    Int_t   numTrack, adcTofinoCh;
    Float_t tdcTofino, tdcTofinoCh, adcTofino, prevTdcTofino, prevAdcTofino;
    Float_t timeTofino, timeResol, amplResol;
    Float_t distPMT, slope;

    geaTof      = 0.0;
    geaEner     = 0.0;
    geaX        = 0.0;
    geaY        = 0.0;
    geaMom      = 0.0;
    numTrack    = 0;
    trackLen    = 0.0;
    tdcTofino   = 0.0;
    tdcTofinoCh = 0;
    adcTofino   = 0;
    adcTofinoCh = 0;
    distPMT     = 0;
    slope       = 0;

    prevTdcTofino = 100000.;
    prevAdcTofino = 0.;

    HTofinoCalSim* pCal;
    for(Int_t s=0;s<6;s++){
	for(Int_t cell=0;cell<4;cell++){
	    // nhits=0, track=-1
	    pCal=(HTofinoCalSim*)array->At(s*4+cell);
	    pCal->clear();
	    pCal->setSector(s);
	    pCal->setModule(0);
	    pCal->setCell(cell);
	    pCal->setTime(0.0);
	    pCal->setCharge(0.0);

	    ctTracks    [s][cell]=0;
	    for(Int_t hit=0;hit<MAXHIT;hit++){
		numTracks   [s][cell][hit]=0;
		geaTofTracks[s][cell][hit]=0.;
		tdcTofTracks[s][cell][hit]=100000.;
	    }
	}
    }
    //-------------------------------------------------
    // loop over geant category
    iterGeant->Reset();
    HGeantTof* pGeant = NULL;

    while ( (pGeant=(HGeantTof *)iterGeant->Next()) != NULL)
    {  // loop over HGeant hits

	fLoc[1] = pGeant->getModule();
	if(fLoc[1]<22) continue;

	fLoc[0] = pGeant->getSector();
	fLoc[2] = pGeant->getCell();
	fLoc[2] = fLoc[1] - 22; // to start counting from 0

	fLoc[1] = 0;
	Int_t index=fLoc[0]*4+fLoc[2];
	//-------------------------------------------------
	// get GEANT values of the hit
	pGeant->getHit(geaEner,geaX,geaY,geaTof,geaMom,trackLen);
	numTrack = pGeant->getTrack();
	//-------------------------------------------------

	//-------------------------------------------------
	// get TofCalSim object
	// if the same adress object was hit already sorting has
	// be done to find the earliest hit
	pCal = (HTofinoCalSim*) array->At(index);
	if (pCal->getNHit()!=0) {   // test if this cell has already seen a hit
	    pCal->incNHit();        // increment hit counter
	    prevTdcTofino = pCal->getTime();
	    prevAdcTofino = pCal->getCharge();
	} else {                    // this cell has not yet been hit, ask for a new slot in cat
	    prevTdcTofino = 100000.;
	    prevAdcTofino = 0.;
	    pCal->setNHit(1);
	}
	//-------------------------------------------------


	//-------------------------------------------------
	// calulation of measured times
	timeTofino = ((HTofinoDigitPar*)pTofinoDigitPar)
	    ->calcLightPropTime(fLoc[0],fLoc[2],geaY);
	timeResol = ((HTofinoDigitPar*)pTofinoDigitPar)
	    ->calcDeltaLightPropTime(fLoc[0],fLoc[2],geaY);
	//-------------------------------------------------

	ffLoc[0] =  fLoc[0] ;
	ffLoc[1] =  fLoc[2] ;

	//-------------------------------------------------
	// calculate distance to the PMT
	slope=((HTofinoDigitPar*)pTofinoDigitPar)->getSlope(ffLoc);
	if(slope != 0.0) distPMT=(timeTofino/slope);
	else Error("execute","slope equal zero, cannot calculate distance to PMT");
	//-------------------------------------------------

	//-------------------------------------------------
	// TDC information
	timeTofino = gRandom->Gaus(timeTofino,timeResol); // prop. time
	tdcTofino = timeTofino + geaTof;    // time in ns

	tdcTofinoCh = (Int_t) ((HTofinoDigitPar*)pTofinoDigitPar)->
	    timeToChannel(fLoc[0],fLoc[2],tdcTofino);

	if(tdcTofinoCh<   0) tdcTofinoCh = 0;
	if(tdcTofinoCh>4095) tdcTofinoCh = 4095;
	tdcTofino = (Float_t) (tdcTofinoCh)+0.5;
	//-------------------------------------------------

	//-------------------------------------------------
	// find the first track ID entering the TOF
	// depending on storeFirstTrack flag
	// if replacing of tof track number is used
	HGeantTof* pOld=pGeant;
	HGeantTof* pNew=pGeant;
	Int_t tempTrack=0;
	Int_t count    =0;
	tempTrack=findFirstHitInTofino(pOld,&pNew,&count);
	numTrack =tempTrack;
	//-------------------------------------------------

	//-------------------------------------------------
	// store all track numbers
	if(ctTracks[fLoc[0]][fLoc[2]]<MAXHIT){
	    numTracks   [fLoc[0]][fLoc[2]][ctTracks[fLoc[0]][fLoc[2]]]=numTrack;
	    geaTofTracks[fLoc[0]][fLoc[2]][ctTracks[fLoc[0]][fLoc[2]]]=geaTof;
	    tdcTofTracks[fLoc[0]][fLoc[2]][ctTracks[fLoc[0]][fLoc[2]]]=tdcTofino;
	    ctTracks    [fLoc[0]][fLoc[2]]++;

	} else {
	    //Warning("execute()","Number of Tracks in HTofinoCalSim > 10. Tracknumber will not be stored to list!");
	}
	//-------------------------------------------------


	//-------------------------------------------------
	// sort fastest times . keep always
	// the fastest one and store it in tracklist at
	// first position. In this case it will be never lost
	// even if there are more than 5 hits in a cell.
	if(tdcTofino > prevTdcTofino) {
	    numTrack  = pCal->getTrack(0);
	    tdcTofino = prevTdcTofino;
	    geaTof    = pCal->getGeaTof(0);
	}
	//-------------------------------------------------

	//-------------------------------------------------
	// calulation of measured charge
	// ADC information
	adcTofino = geaEner*exp(-distPMT/al);
	amplResol = adcTofino*relAmplResol;

	adcTofino = gRandom->Gaus(adcTofino,amplResol);

	adcTofinoCh = (Int_t) ((adcTofino/minEnerRelease)*512. + prevAdcTofino);

	if (adcTofinoCh <    0) adcTofinoCh = 0;
	if (adcTofinoCh > 4095) adcTofinoCh = 4095;
	adcTofino = (Float_t) (adcTofinoCh)+0.5;
	//-------------------------------------------------


	//-------------------------------------------------
	// fill temp out object.
	// time is in channels still.
	// list of tracks only contains fastest track
	// other hits/geaTofs will be filled later
	pCal->setTime(tdcTofino);
	pCal->setCharge(adcTofino);
	pCal->setSector(fLoc[0]);
	pCal->setModule(fLoc[1]);
	pCal->setCell(fLoc[2]);
	pCal->fillTrack(0,numTrack);
	pCal->fillGeaTof(0,geaTof);
	//-------------------------------------------------

    } // end loop over geant category
}

void HTofinoDigitizer::doFinalCheckOnArray()
{
    //-------------------------------------------------
    // now do the final check on collected data and
    // convert times from channel to nanosecond to write
    // to output category (and merge with real data in fillOutput() )
    // fill list of tracks/geaTof other than the fastest hit

    Int_t thrCFD = 0 ; // cfd threshold
    Int_t thrADC = 0; // adc  threshold
    Float_t tdcTofino=0.;
    Float_t adcTofino=0.;

    HTofinoCalSim* pCal;

    for(Int_t i=0;i<24;i++){
	// nhits=0, track=-1
	pCal=(HTofinoCalSim*)array->At(i);
	if(pCal->getNHit()==0) continue;

	fLoc[0] = pCal->getSector();
	fLoc[1] = pCal->getModule();
	fLoc[2] = pCal->getCell();
	tdcTofino = pCal->getTime();
	adcTofino = pCal->getCharge();


	// TDC overflow suppression
	if(Int_t(tdcTofino) >= 4095) pCal->setTime(0.0);

	ffLoc[0]=fLoc[0];
	ffLoc[1]=fLoc[2];

	// CFD threshold
	thrCFD = (Int_t)((HTofinoDigitPar*)pTofinoDigitPar)->getCFDThreshold(ffLoc);
	thrADC = (Int_t)((HTofinoDigitPar*)pTofinoDigitPar)->getADCThreshold(ffLoc);

	if(((Int_t)adcTofino)<thrCFD) pCal->setTime(0.0);
	if(((Int_t)adcTofino)<thrADC) pCal->setCharge(0.0);

	// recalibrate tdc (ch) --> tdc (ns)
	tdcTofino = ((HTofinoDigitPar*)pTofinoDigitPar)->
	    channelToTime(fLoc[0],fLoc[2],pCal->getTime()); // in ns

	tdcTofino -= ((HTofinoDigitPar*)pTofinoDigitPar)->getOffset(ffLoc);

	if(tdcTofino > 0) pCal->setTime(tdcTofino);
	else              pCal->setTime(0.0);

	//-------------------------------------------------
	// fill the list of tracks and geaTof other
	// than the fastest hit
	// sort measurements of the cell with increasing tdcTofino
	Int_t* ind=new Int_t[ctTracks[fLoc[0]][fLoc[2]]];
	HTool::sort(ctTracks[fLoc[0]][fLoc[2]],&tdcTofTracks[fLoc[0]][fLoc[2]][0],ind,kFALSE);
        Int_t indOut=1;
	for(Int_t j=1;j<ctTracks[fLoc[0]][fLoc[2]];j++){
            // looping over full temp array

	    //-------------------------------------------------
	    // if output has already stored the max number of
	    // tracks we have to leave the loop
	    if(indOut>=5) {break;}
	    //-------------------------------------------------

	    //-------------------------------------------------
	    // looking for same track number
            // only new tracks should new stored
            Bool_t found=kFALSE;
	    for(Int_t i=0;i<indOut;i++){
		// loop over already stored indices

		//-------------------------------------------------
		// if the actual track number was already stored
                // in the output mark it found and leave the loop
		if(pCal->getTrack(i)==numTracks[fLoc[0]][fLoc[2]][ind[j]]){
		    found=kTRUE;
		    break;
		}
		//-------------------------------------------------
	    }
	    //-------------------------------------------------

	    //-------------------------------------------------
	    // if it is a new track store it in output
            // and count the number of hits up
	    if(!found)
	    {
		// finally fill the select track to output
		pCal->fillTrack (indOut,numTracks   [fLoc[0]][fLoc[2]][ind[j]]);
		pCal->fillGeaTof(indOut,geaTofTracks[fLoc[0]][fLoc[2]][ind[j]]);
		indOut++;
	    }
	    //-------------------------------------------------

	}
	// restore the real number of
        // tracks written to output
	pCal->setNHit(indOut);
	delete [] ind;
	//-------------------------------------------------
    }
}

void HTofinoDigitizer::fillOutput()
{
    // write to output category (and merge with real data)
    // fill list of tracks/geaTof other than the fastest hit

    const Int_t embeddingmode=gHades->getEmbeddingMode(); // flag embedding 1=realistic 2:keep geant

    Float_t tdcTofino     = 0.;
    Float_t prevTdcTofino = 100000.;
    Float_t prevAdcTofino = 0.;

    HTofinoCalSim* pCal;


    for(Int_t i=0;i<24;i++)
    {
	pCal=(HTofinoCalSim*)array->At(i);
	if(pCal->getNHit()==0) continue;

	fLoc[0] = pCal->getSector();
	fLoc[1] = pCal->getModule();
	fLoc[2] = pCal->getCell();
	//-------------------------------------------------
	// get TofCalSim object from output category
	// if the same adress object exists already sorting has
	// to be done to find the earliest hit
	HTofinoCalSim* pCalOut = (HTofinoCalSim*) fCalCat->getObject(fLoc);
	if (pCalOut) {
	    // test if this cell has already seen a hit
	    // this should happen only in embedding mode
	    // when the cell has been hit by real data before
	    if(embeddingmode>0)
	    {
		prevTdcTofino = pCalOut->getTime();
		prevAdcTofino = pCalOut->getCharge();
		tdcTofino     = pCal   ->getTime();
		if(tdcTofino==0)tdcTofino=100000;


		//-------------------------------------------------
		// comparing the measured tof (sim) to the previous (real)
		// for embeddingmode=2 the GEANT hits should be kept anyway


		// fill output
		// sort times
		if(tdcTofino > prevTdcTofino)
		{
		    // real date first
		    tdcTofino = prevTdcTofino;
		    if(embeddingmode==2)
		    {
			// keep geant
			pCalOut->fillTrack(1,pCalOut->getTrack(0));
			pCalOut->fillTrack(0,pCal   ->getTrack(0));
		    }
		    else
		    {
			pCalOut->fillTrack(1,pCal   ->getTrack(0));
			pCalOut->fillTrack(0,pCalOut->getTrack(0));
		    }
		}
		else
		{
		    // sim data first
		    pCalOut->fillTrack(1,pCalOut->getTrack(0));
		    pCalOut->fillTrack(0,pCal   ->getTrack(0));
		}
		pCalOut->incNHit();  // for the fastest sim hit
		for(Int_t i=1;i<pCal->getNHit();i++)
		{   // copy the rest of the sim hits
		    // after fastest sim + real hit
		    if(i+2>=4) break;  // out of range
		    pCalOut->incNHit();
		    pCalOut->fillTrack (i+2,pCal->getTrack(i));
		    pCalOut->fillGeaTof(i+2,pCal->getGeaTof(i));
		}
		pCalOut->setTime  (tdcTofino);
		pCalOut->setCharge(prevAdcTofino + pCal->getCharge());

		//-------------------------------------------------
	    }
	    else
	    {
		Error("execute()","Slot already used in Category catTofinoCal! \
		      You are running NO EMBEDDING...will over write object");
		pCalOut = new(pCalOut) HTofinoCalSim(*pCal);
	    }

	}
	else
	{
	    // this cell has not yet been hit, ask for a new slot in cat
	    // this case should be true in case we are running pure
	    // simulation or cell has not not been hit by real data
	    // in both cases we have to simply copy the calsim object
	    // from the temporay array to the output
	    pCalOut = (HTofinoCalSim*) fCalCat->getSlot(fLoc);  // get new slot
	    if(!pCalOut)
	    {
		Error("execute()","Could not retrieve new Slot in Category catTofinoCal!");
		continue;
	    }
	    pCalOut = new(pCalOut) HTofinoCalSim(*pCal);
	}
	//-------------------------------------------------
    }
    //-------------------------------------------------
}

Int_t HTofinoDigitizer::findFirstHitInTofino(HGeantTof* poldTof,HGeantTof** pnewTof,Int_t* count)
{
    //-------------------------------------------------
    // find the first track ID entering the TOFINO
    // Used to suppress the secondaries created in the
    // TOF itself.
    //        0 (default) = realistic (secondaries included)
    //        1 primary particle is stored
    //        2 the first track number entering the tof in SAME SECTOR is stored
    //        3 as 2 but SAME SECTOR && ROD

    Int_t numTrack=poldTof->getTrack();
    
    if(numTrack<=0) return numTrack; // nothing to do for real data
    HGeantKine* kine=0;
    *count=0;

    //--------------------------------------------------------
    // return the track number for
    // the selected option storeFirstTrack

    //--------------------------------------
    // case 0
    if(storeFirstTrack==0) return numTrack;
    //--------------------------------------
    // case 1
    if(storeFirstTrack==1)
    {   // return track number of primary particle
	// of the given track
	kine=(HGeantKine*)fGeantKineCat->getObject(numTrack-1);
	Int_t parent=kine->getParentTrack();


	kine=HGeantKine::getPrimary(numTrack,fGeantKineCat);
	if(parent>0)(*count)--; // mark only those which change
	if(debug)
	{
	    // make the changed track numbers easily
	    // visible in output. ONLY DEBUGGING!!!!
	    return (*count)*200;
	}
	else return kine->getTrack();
    }

    //--------------------------------------
    // case 2 and 3
    kine=(HGeantKine*)fGeantKineCat->getObject(numTrack-1);
    if(kine->getParentTrack()==0){return numTrack;} // nothing to do

    Int_t s,m,c;
    s= poldTof->getSector();
    m= 0;
    c= poldTof->getModule()-22;

    Int_t first=0;
    Int_t tempTrack=numTrack;
    while((kine=kine->getParent(tempTrack,fGeantKineCat))!=0)
    {
	first=kine->getFirstTofHit();
	if(first!=-1)
	{
	    // track is still in TOF/TOFINO

	    // now we have to check if it is in TOF or TOFINO
	    HGeantTof* gtof=(HGeantTof*)fGeantCat->getObject(first);

	    Int_t s1,m1,c1;
	    s1=m1=c1=0;
	    m1 = 0;
	    if(gtof->getModule()<=22)
	    {
		// inside TOFINO
		s1= gtof->getSector();
		c1= gtof->getModule()-22;

		if(storeFirstTrack==2&&
		   s==s1)
		{   // case 2 :: check only sector
		    tempTrack  =kine->getTrack();
		    (*pnewTof)=gtof;
		    (*count)--;
		}
		else if(storeFirstTrack==3&&
			s==s1&&c==c1)
		{   // case 3 :: check for same rod
		    tempTrack  =kine->getTrack();
		    (*pnewTof)=gtof;
		    (*count)--;
		}
		else {
		    // track has no TOFINO hit any longer
		    // which fulfills the condition
		    break;
		}

	    } else {
		// track is in TOF
		break;
	    }
	}
	else {
	    // track has no TOFINO hit any longer,
	    // so the previous object was the one we
	    // searched  for
	    break;
	}
    }
    if(debug&&(*count)<0)
    {   // make the changed track numbers easily
	// visible in output. ONLY DEBUGGING!!!!
	return (*count)*200;
    }
    else return tempTrack;
}


