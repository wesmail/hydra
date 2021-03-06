//*-- Author : Jacek Otwinowski 01/09/2003
//*-- Modified : Y.C.Pachmayer 3/09/2006
//
// $Id: hpidskiptrigger.cc,v 1.10 2009-07-15 11:40:25 halo Exp $

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidSkipTrigger                                                      //
//                                                                            //    
////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hades.h"
#include "haddef.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "heventheader.h"
#include "hpidskiptrigger.h"

// -----------------------------------------------------------------------------

ClassImp(HPidSkipTrigger)

// -----------------------------------------------------------------------------

HPidSkipTrigger::HPidSkipTrigger(const Text_t *name,const Text_t *title,const Option_t *option):HReconstructor(name,title)
{
    setDefault();
    setOptions(option);
}
HPidSkipTrigger::HPidSkipTrigger(const Text_t *option):HReconstructor("Skip Trigger","Skip Trigger")
{
    setDefault();
    setOptions(option);

}
// -----------------------------------------------------------------------------
//reset all counters and disable event skipping
void HPidSkipTrigger::setDefault(void)
{
	nTot = 0;
	nSkip = 0;
        nAccept =0;
	bSkip1stLvlEvents=kFALSE;
	bSkip2ndLvlEvents=kFALSE;
	bSkipLowMult=kFALSE;
        bSkipTrigM4=kFALSE;
        bSkipNotTrigM18=kFALSE;
}

//enable skip-flags given by user-supplied argument
void HPidSkipTrigger::setOptions(const Option_t* option)
{
  TString s = option;
  s.ToUpper();
  bSkip2ndLvlEvents=(strstr(s.Data(),"LVL2") ? kTRUE : kFALSE);
  bSkip1stLvlEvents=(strstr(s.Data(),"LVL1") ? kTRUE : kFALSE);
  bSkipLowMult     =(strstr(s.Data(),"LOWMULT") ? kTRUE : kFALSE);
  bSkipTrigM4      =(strstr(s.Data(),"TRIGM4") ? kTRUE : kFALSE); 
  bSkipNotTrigM18  =(strstr(s.Data(),"NOTTRIGM18") ? kTRUE : kFALSE); 
}

// -----------------------------------------------------------------------------
Bool_t HPidSkipTrigger::init(void)
{
// initialize the reconstructor 

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidSkipTrigger::reinit(void)
{
// Dummy method

    return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidSkipTrigger::finalize(void)
{
  printf("nTot:%d \n",nTot);
  printf("nSkip:%d \n",nSkip);
  printf("nAccept:%d \n",nAccept);
  return kTRUE;
}

Int_t HPidSkipTrigger::execute(void)
{
  
   HEventHeader *pEventHead = NULL;
   Int_t DownFlag = -1;
   Int_t TrigDecision = -1;
   Int_t TBit = 0;

   // get event header to determine trigger decision
   pEventHead = (HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader());
  
   if (pEventHead != NULL)
   {
      DownFlag     = pEventHead->getDownscalingFlag();
      TrigDecision = pEventHead->getTriggerDecision();
      TBit         = pEventHead->getTBit();
   }
   else 
   {
      Error("execute"," EventHeader does not exist: %p ",
	    ((HEventHeader*)((HEvent*)(gHades->getCurrentEvent())->getHeader())));
   }
   nTot++;
  
   if(TrigDecision==0 && bSkip1stLvlEvents)
   {
      //skip first level events
      nSkip++;
      return kSkipEvent;
   }

   // skip second level events, only lvl1 events are selected with multilicity tbit is set
   if(bSkip2ndLvlEvents)
   {
      if(DownFlag==0)
      {
	 nSkip++;
	 return kSkipEvent;
      }
      else
      {
	 if( ((TBit&(1<<0))>0) )
	 {
	    nAccept++;
	    return 0;
	 }
	 else
	 {
	    nSkip++;
	    return kSkipEvent;
	 }
      }
   }

#warning THIS MUST BE CORRECTED - SKIPPING LOW MULTIPLICITY EVENTS IS NOT IMPLEMENTED CORRECTLY

   if( ((TBit&(1<<0))==0) && bSkipLowMult)
   {
      return kSkipEvent;
   }

   // events are selected if trigger decision is positive and multilicity
   // tbit is set
   if(bSkipTrigM4)
   {
      if(TrigDecision==0)
      {
	 nSkip++;
	 return kSkipEvent;
      }
      else
      {
	 if( ((TBit&(1<<0))>0) )
	 {
	    nAccept++;
	    return 0;
	 }
	 else
	 {
	    nSkip++;
	    return kSkipEvent;
	 }
      }
   }

   // events are selected if trigger decision is positive and multilicity-18
   // trigger bit is set: accept only M18 events
   if (bSkipNotTrigM18)
   {
      if (TBit & 1<<4)
      {
	 nAccept++;
	 return 0;
      }
      else
      {
	 nSkip++;
	 return kSkipEvent;
      }
   }

   return 0;
}
