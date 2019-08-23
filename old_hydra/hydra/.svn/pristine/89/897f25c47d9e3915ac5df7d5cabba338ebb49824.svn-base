//*-- Author : Martin Jurkovic (martin.jurkovic@ph.tum.de)

using namespace std;
#include "hrichunpackercurrent.h"
#include "hades.h"
#include "hcategory.h"
#include "hdebug.h"
#include "hdetector.h"
#include "hevent.h"
#include "heventheader.h"
#include "hiterator.h"
#include "hldsubevt.h"
#include "hlinearcategory.h"
#include "hlocation.h"
#include "hrichdetector.h"
#include "hrichhitheader.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "richdef.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

//////////////////////////////////////////////////////////////////
// HRichUnpackerCurrent
//
// This is the HUnpacker used to read RICH HV current data 
// from .hld files. It unpacks TOF data and writes the 
// current into the HRichHitHeader category.
//
//////////////////////////////////////////////////////////////////

HRichUnpackerCurrent::HRichUnpackerCurrent(Int_t id, Int_t strtEvt) {
  subEvtId = id;
  iEventNr = -1;
  startEvt = strtEvt;
  m_pHitHdrCat = 0;
}

Bool_t HRichUnpackerCurrent::init(void) {
  m_pHitHdrCat = gHades->getCurrentEvent()->getCategory(catRichHitHdr);
  if (!m_pHitHdrCat) {
    m_pHitHdrCat = gHades->getSetup()->getDetector("Rich")->buildCategory(catRichHitHdr);
    if (!m_pHitHdrCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catRichHitHdr, m_pHitHdrCat, "Rich");
  }
  fIterHitHeader = (HIterator *) m_pHitHdrCat->MakeIterator("native");
  return kTRUE;
}

Int_t HRichUnpackerCurrent::execute() {
  HRichHitHeader *pRichHdr = NULL;
  Int_t nCrate       = 0; // Crate with RICH ADC is #4
  Int_t nSlot        = 0; // RICH Current ADC is in slot 13
  Int_t nChannel     = 0; // ADC channel coresponds to sector number
  Int_t nData        = 0; // Unpacked data
  Int_t nSubEvtId    = 0; // SubEvtId 
  Int_t nEvtId       = 0; // Evt Id
  UInt_t uWordCount  = 0; // Number of words processed
  UInt_t uBlockSize  = 0; // Number of data words in one block
  UInt_t *uTmp       = NULL;
  
//==========================================================================
//            VALID SINCE AUG_04 SUBEVENT (SINCE SUBEVT_ID 416)
//==========================================================================

  iEventNr = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
  if(iEventNr >= startEvt && subEvtId == 416) { //New data structures since aug04
    if(pSubEvt) {
      UInt_t* data = pSubEvt->getData();
      UInt_t* end  = pSubEvt->getEnd();

#if DEBUG_LEVEL>4
      printf("Next SubEvt. data between: %p and %p\n",data,end);
      pSubEvt->dumpIt();
#endif

      nEvtId = gHades->getCurrentEvent()->getHeader()->getId();
      nSubEvtId = pSubEvt->getId();

// ========================
// Loop over all data words
// ========================
      while(++data < end && *data != 0x0) {

#if DEBUG_LEVEL>2
	printf("Data word: %p\n",*data);
#endif

	if(((*data >> 24) & 0x6) == 2) {        // Found valid header
	  uBlockSize = ((*data) >> 8 ) & 0xff;  // in the case of RICH ADC it should be max 6
	  nCrate     = ((*data) >> 16) & 0xff;  // Crate with RICH ADC = 4
	  nSlot      = ((*data) >> 27) & 0x1f;  // RICH ADC slot in the crate is 13

// ===============================================
// Check only for RICH ADC, if also trailer exists 
// (the rest is checked in TOF/START unpackers)
// ===============================================
	  if (nCrate == RICH_ADC_CRATE && nSlot == RICH_ADC_SLOT) {
	    if((*(data + uBlockSize + 1) >> 24 & 0x6) != 4) {
	      Error("execute","Event: %d...Header found but no trailer!!", iEventNr);
	      pSubEvt->dumpIt();
	      continue;
	    }

// =======================================================
// Check only for RICH ADC, if, trigger tag in the trailer
// (the rest is checked in TOF/START unpackers)
// =======================================================
	    if((*(data + uBlockSize +1 ) & 0xff) != (UInt_t) pSubEvt->getTrigNr()) {
	      pSubEvt->dumpIt();
	      Error("execute","Evt = %d Trigger tag mismatch!", iEventNr);
	      // exit(EXIT_FAILURE);
	    }

// ===================
// Scan all data words
// ===================
#if DEBUG_LEVEL>4
	    printf("TDC header 0x%08x.Size: %i.Crate: %i.Slot: %i.\n", *data,uBlockSize,nCrate,nSlot);
#endif

	    uTmp = data;
	    uWordCount = 0;

	    while(++data && data < (uTmp + uBlockSize + 1) && *data != 0x0) {
	      uWordCount++;
	      nChannel = (*(data) >> 16) & 0x003f;
	      nData    = *data  & 0xfff;

	      if(nChannel > 5) continue; // Only 6 sectors
	      fIterHitHeader->Reset();
	      while((pRichHdr = (HRichHitHeader *) fIterHitHeader->Next())) {
		if (nChannel == pRichHdr->getSector()) {
		  cout << "Header for this sector already exist" << endl;
		  break;
		} else pRichHdr = NULL;
	      }

	      if (pRichHdr == NULL) {
		HLocation richLoc;
		richLoc.set(1, nChannel);
		pRichHdr = (HRichHitHeader *) (m_pHitHdrCat)->getNewSlot(richLoc);
		if (pRichHdr != NULL) pRichHdr = new(pRichHdr) HRichHitHeader;
		if (!pRichHdr) {
		  Warning("execute","Can not get slot");
		  continue;
		}
	      }
#if DEBUG_LEVEL>1
	      printf("Rich ADC: Sector: %d, Data %d\n", nChannel,nData);
#endif
	      pRichHdr->setCurrentSec(nData);
	      pRichHdr->setEventNr(iEventNr);
	      pRichHdr->setSector(nChannel);
	    }
	    if(uWordCount != uBlockSize) Error("execute()", "Event: %d...Found %i words but expecting %i words!!!", iEventNr, uWordCount, uBlockSize);
	  }
	}
	else 
	  if(((*data >> 24) & 0x6) == 6) {
#if DEBUG_LEVEL>2
	    Error("execute()", "Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x", iEventNr,((*data>>24) & 0x6),*data);
	    pSubEvt->dumpIt();
#endif
	  }
	  else 
	    if(((*data >> 24) & 0x6) == 0) {
#if DEBUG_LEVEL>2
	      Error("execute()", "Evt: %i, Data follow, but no header",iEventNr);
#endif
	    }
	    else {
#if DEBUG_LEVEL>2
	      Error("execute()", "Evt: %i, Unknow raw data type %i", iEventNr, ((*data >> 24) & 0x6));
#endif
	    }
      } //end data
    }
  } //end new data structures
#if DEBUG_LEVEL>4
  gDebuger->leaveFunc("HRichUnpackerCurrent::execute\n");
#endif
  return 1;
}


ClassImp(HRichUnpackerCurrent)
