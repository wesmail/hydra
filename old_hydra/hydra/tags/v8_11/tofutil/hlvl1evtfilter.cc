using namespace std;
//*-- Author : Stefano Spataro

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////
// HLvl1EvtFilter
// Emulator of first level trigger.
// How to use it:
//
// HTaskSet *evtfilter = new HTaskSet("","");
// HLvl1EvtFilter *evtflt = new HLvl1EvtFilter("eventfilter","eventfilter",options,multMeta,multTofino,multTtof);
// evtfilter->connect(evtflt);
// masterTaskSet->add(evtflt);
//
// where options stays for:
// "metamult"   ->  META multiplicity   >= multMeta
// "tofinomult" ->  TOFINO multiplicity >= multTofino
// "tofmult"    ->  TOF multiplicity    >= multTof
// "opsec"  ->  Opposite sectors in META
// "sim"        ->  Simulation mode ON
 
#include "hlvl1evtfilter.h"
#include "hlinearcategory.h"
#include "hmatrixcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hades.h"
#include "tofinodef.h"
#include "htofraw.h"
#include "htofinoraw.h"
#include "htofinocalsim.h"
#include "tofdef.h"
#include <TFile.h>
#include <iostream>

ClassImp(HLvl1EvtFilter)
  HLvl1EvtFilter::HLvl1EvtFilter(Text_t *name,Text_t *title,TString opt,Int_t multMeta, Int_t multTofino, Int_t multTof) : HReconstructor(name,title)
{
  // constructor  options:
  // "metamult"   ->  META multiplicity   >= multMeta
  // "tofinomult" ->  TOFINO multiplicity >= multTofino
  // "tofmult"    ->  TOF multiplicity    >= multTof
  // "opsec"      ->  Opposite sectors in META 
  // "sim"        ->  Simulation mode ON
  
  nMetaMult = 0;
  nTofMult = 0;
  nTofinoMult = 0;
  kOpSec = kFALSE;
  isSimulation = kFALSE;

  if ((opt.Contains("metamult"))   && (multMeta!=0))   nMetaMult=multMeta;
  if ((opt.Contains("tofinomult")) && (multTofino!=0)) nTofinoMult=multTofino;
  if ((opt.Contains("tofmult"))    && (multTof!=0))    nTofMult=multTof;
  if  (opt.Contains("opsec"))      kOpSec = kTRUE;
  if  (opt.Contains("sim"))        isSimulation = kTRUE;

} // end of constructor

HLvl1EvtFilter::HLvl1EvtFilter()
{
  // Default constructor
  
  nMetaMult = 0;
  nTofMult = 0;
  nTofinoMult = 0;
  kOpSec = kFALSE;
  isSimulation = kFALSE;
  
}


HLvl1EvtFilter::~HLvl1EvtFilter(void) {
  // destructor
}

Bool_t HLvl1EvtFilter::init() {
  // initialization function
  
  if (gHades) {
    HEvent *event=gHades->getCurrentEvent();
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *spec=gHades->getSetup();
    
    if (event && rtdb) {
      HDetector *tofino = spec->getDetector("Tofino");
      if (tofino){ 
	if (isSimulation) fTofino = event->getCategory(catTofinoCal);
	else fTofino = event->getCategory(catTofinoRaw);
	
	if (!fTofino) {
	  Error("init","No TOFINO input");
	  return kFALSE;
	}
      }
      fTofinoIter=(HIterator *)fTofino->MakeIterator();
      
      HDetector *tof=spec->getDetector("Tof");
      if (tof) 
	{ 
	  fTofRaw=event->getCategory(catTofRaw);
	  if (!fTofRaw) {
	    Error("init","No TOF input");
	    return kFALSE;
	  }
	}
      fTofIter=(HIterator *)fTofRaw->MakeIterator();
      
    } // end of event && rtdb condition
  } // end of gHades
  
  cout << " HLvl1EvtFilter: *** 1st level trigger event filter initialization ***" << endl;
  cout << " META >= " << nMetaMult << "\tTOFINO >= " << nTofinoMult << "\tTOF >= " << nTofMult<< "\t Opposite sector " << kOpSec << "\t Simulation: "<< isSimulation << endl;
  
  resetCounters();
  
  return kTRUE;
} // end of initialization

Bool_t HLvl1EvtFilter::finalize() 
{
  // Finalize: write number of rejected events 
  cout<<"Number of rejected evts: "<<nCounterNbRejectedEvts<<endl;
  cout<<"Number of remaining evts: "<<nCntProcessedEvents-nCounterNbRejectedEvts<<endl;
  cout<<"Percentage of rejected evts: "<<(((Float_t)nCounterNbRejectedEvts)/((Float_t)nCntProcessedEvents))*100<<"%"<<endl;
  
  return kTRUE;
  
}

Int_t HLvl1EvtFilter::execute()
{
  // Execute function: refer to isFilteredEvt(swt) for filter options
  nCntProcessedEvents++;
  
  Int_t kReturnValue=0;
  
  if (isFilteredEvt())
    {
      nCounterNbRejectedEvts++;
      //	cout<<nCntProcessedEvents<<"  event is skipped"<<endl;
      kReturnValue = kSkipEvent;
    }
  
  return kReturnValue;
}

void HLvl1EvtFilter::resetCounters()
{
  // Reset counters of processed and rejected events
  
  nCntProcessedEvents=0;
  nCounterNbRejectedEvts=0;
}

Bool_t HLvl1EvtFilter::isFilteredEvt()
{
  // if function returns kTRUE evt is skipped
  
  Int_t TofLeftMult[6] = {0,0,0,0,0,0};
  Int_t TofRightMult[6] = {0,0,0,0,0,0};
  Int_t TofinoMult[6]  = {0,0,0,0,0,0};
  Bool_t MetaMult[6]   = {kFALSE,kFALSE,kFALSE,kFALSE,kFALSE,kFALSE};
  Int_t tofraw_count = 0, tofinocal_count = 0, meta_mult = 0, tof_mult = 0, tofino_mult = 0;
  Bool_t opposite_sector = kFALSE;
  Bool_t kSwitch = kFALSE;
  
  HTofRaw *TofRaw = 0;
  HTofinoRaw *TofinoRaw = 0;
  HTofinoCal *TofinoCal = 0;
  
  fTofIter->Reset();
  while ((TofRaw=(HTofRaw *)fTofIter->Next()) != 0) // start TofRaw Loop
    {
      if (TofRaw->getRightTime()>0.) 
	{
	  TofRightMult[(Int_t)TofRaw->getSector()]++;
	  tofraw_count++;
	}
      if (TofRaw->getLeftTime()>0.) 
	{
	  TofLeftMult[(Int_t)TofRaw->getSector()]++;
	  tofraw_count++;
	}
    } // end of HTofRaw loop
  
  fTofinoIter->Reset(); 
  if (isSimulation)
    {
      // in Simulation Tofinoraw does not exist, so TofinoCal category is used
      while ((TofinoCal=(HTofinoCalSim *)fTofinoIter->Next())!= 0) 
	{
	  TofinoMult[TofinoCal->getSector()]++; 
	  tofinocal_count++;
	} // end of HTofinoCalSim loop 
    }
  else   
    while ((TofinoRaw=(HTofinoRaw *)fTofinoIter->Next())!= 0) 
      {
	if (TofinoRaw->getTime()>0.) 
	  { 
	    TofinoMult[TofinoRaw->getSector()]++; 
	    tofinocal_count++;
	  }
      } // end of HTofinoRaw loop 
  
  for (Int_t yyy=0; yyy<6; yyy++) MetaMult[yyy]=(((TofRightMult[yyy]+TofLeftMult[yyy])>=2)||(TofinoMult[yyy]>=1));
  
  opposite_sector = ((MetaMult[0]&&MetaMult[3]) || (MetaMult[1]&&MetaMult[4]) || (MetaMult[2]&&MetaMult[5]));
  meta_mult = int(tofraw_count/2)+tofinocal_count;
  tof_mult = int(tofraw_count/2);
  tofino_mult = tofinocal_count;
  
  if ((meta_mult<nMetaMult) || (tofino_mult<nTofinoMult) || (tof_mult<nTofMult) || (!opposite_sector&&kOpSec)) 
    kSwitch = kTRUE; // if at least one condition is not fulfilled, turn on skip flag
  
  return kSwitch;
}

