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
#include "htree.h"

#include "richdef.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "TCanvas.h"
#include "TColor.h"
#include "TH2F.h"
#include "TPostScript.h"
#include "TStyle.h"
#include "TSystem.h"
#include "TTree.h"

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////
// HRichUnpackerCurrent
//
// This is the HUnpacker used to read RICH HV current data 
// from .hld files. It unpacks TOF data and writes the 
// current into the HRichHitHeader category.
//
//////////////////////////////////////////////////////////////////

HRichUnpackerCurrent::HRichUnpackerCurrent(Int_t id, Int_t nrEvtHist,
					   Int_t strtEvt, TString psName) {
  subEvtId = id;
  iEventNr = -1;
  startEvt = strtEvt;
  m_pHitHdrCat = 0;
  psFileName = psName;
  histEvt = nrEvtHist;

  h1 = NULL;
  h2 = NULL;
  h3 = NULL;
  h4 = NULL;
  h5 = NULL;
  h6 = NULL;

}

Bool_t HRichUnpackerCurrent::init(void) {
  m_pHitHdrCat = gHades->getCurrentEvent()->getCategory(catRichHitHdr);
  if (!m_pHitHdrCat) {
    m_pHitHdrCat = gHades->getSetup()->getDetector("Rich")->buildCategory(catRichHitHdr);
    if (!m_pHitHdrCat) 
      exit(EXIT_FAILURE);
    else 
      gHades->getCurrentEvent()->addCategory(catRichHitHdr, m_pHitHdrCat, "Rich");
  }
  fIterHitHeader = (HIterator *) m_pHitHdrCat->MakeIterator("native");

  h1 = new TH2F("Current_1", "Current in sec 1",
		(Int_t) (histEvt / 2000), 0, histEvt, 100, 0, 4500);
  h2 = new TH2F("Current_2", "Current in sec 2",
		(Int_t) (histEvt / 2000), 0, histEvt, 100, 0, 4500);
  h3 = new TH2F("Current_3", "Current in sec 3",
		(Int_t) (histEvt / 2000), 0, histEvt, 100, 0, 4500);
  h4 = new TH2F("Current_4", "Current in sec 4",
		(Int_t) (histEvt / 2000), 0, histEvt, 100, 0, 4500);
  h5 = new TH2F("Current_5", "Current in sec 5",
		(Int_t) (histEvt / 2000), 0, histEvt, 100, 0, 4500);
  h6 = new TH2F("Current_6", "Current in sec 6",
		(Int_t) (histEvt / 2000), 0, histEvt, 100, 0, 4500);

  h1->SetXTitle("Event Nr.");
  h2->SetXTitle("Event Nr.");
  h3->SetXTitle("Event Nr.");
  h4->SetXTitle("Event Nr.");
  h5->SetXTitle("Event Nr.");
  h6->SetXTitle("Event Nr.");

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
	    if((*(data + uBlockSize +1 ) & 0xff) != ((UInt_t) pSubEvt->getTrigNr()&0xFF)) {
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
	      pRichHdr = NULL;
	      while((pRichHdr = (HRichHitHeader *) fIterHitHeader->Next())) {
		if (nChannel == pRichHdr->getSector()) {
		  cout << "Header for this sector already exist" << endl;
		  break;
		} else {
		  pRichHdr = NULL;
		}
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
#if DEBUG_LEVEL > 1
	      printf("Rich ADC: Sector: %d, Data %d\n", nChannel,nData);
#endif
	      pRichHdr->setCurrentSec(nData);
	      pRichHdr->setEventNr(iEventNr);
	      pRichHdr->setSector(nChannel);

	      switch (nChannel) {
	      case 0 :
		h1->Fill(iEventNr, nData);
		continue;
	      case 1 :
		h2->Fill(iEventNr, nData);
		continue;
	      case 2 :
		h3->Fill(iEventNr, nData);
		continue;
	      case 3 :
		h4->Fill(iEventNr, nData);
		continue;
	      case 4 :
		h5->Fill(iEventNr, nData);
		continue;
	      case 5 :
		h6->Fill(iEventNr, nData);
		continue;
	      default :
		cerr << "BAD SECTOR!!!" << endl;
		continue;
	      }

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

Bool_t HRichUnpackerCurrent::finalize() {

  if (!writePdfFile()) {
    cerr << "Error in writing to PDF file" << endl;
    exit(EXIT_FAILURE);
  } else
    return kTRUE;
}

Bool_t HRichUnpackerCurrent::writePdfFile(void) {

  TString pdfFile = psFileName;

  setStyle();

  c = new TCanvas("current", "HV Current Plots", 200, 320);
  c->Divide(2,3);

  c->cd(1);
  h1->Draw("colz");
  c->cd(2);
  h2->Draw("colz");
  c->cd(3);
  h3->Draw("colz");
  c->cd(4);
  h4->Draw("colz");
  c->cd(5);
  h5->Draw("colz");
  c->cd(6);
  h6->Draw("colz");

  c->SaveAs(pdfFile.Data());
  if (pdfFile.EndsWith(".ps")) {
    pdfFile.ReplaceAll(".ps", 3, ".pdf",4);
    c->SaveAs(pdfFile.Data());
  }

  return kTRUE;
}

void HRichUnpackerCurrent::setStyle() {
// Set drawing styles for PostScript file

  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);

  // Histogram title formatting
  gStyle->SetTitleTextColor(4);
  gStyle->SetTitleColor(10);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(.2);
  gStyle->SetTitleY(.97);
  gStyle->SetTitleH(.065);
  gStyle->SetTitleW(.6);

  // Canvas and pad formatting
  gStyle->SetPadColor(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);

  gStyle->SetPaperSize(18.,27.);
}


ClassImp(HRichUnpackerCurrent)
