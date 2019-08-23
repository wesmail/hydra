//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HRichHistFiller
//
// Container class for the rich hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hrichhistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "hrichfancy.h"
#include "hrichpattern.h"
#include "hrichmappingpar.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "richdef.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "hrichhit.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "hruntimedb.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>
#include <TH1.h>

// Color definitions
TArrayI HRichHistFiller::colorsSector;
TArrayI HRichHistFiller::colorsCuts;

// Hist Pointers
HHistArray*      HRichHistFiller::hRichPulseDist=0;
HTrendArray*     HRichHistFiller::hRichMultiplicity=0;
HTrendArray*     HRichHistFiller::hRichChargedParticles=0;
HHistArray2*     HRichHistFiller::hRichPulseheight=0;
HMonHist2*       HRichHistFiller::hRichmult_raw=0;
HMonHist2*       HRichHistFiller::hRichmulttemp_raw=0;
HTrendArray*     HRichHistFiller::hRichmulttrend_raw=0;

// Hist Addon Pointers
HRichPattern*    HRichHistFiller::hRichMyPattern=0;
HRichFancy*      HRichHistFiller::hRichFancyDisplay=0;
HMonHistAddon*   HRichHistFiller::hRichPulseDistAddon=0;
HMonHistAddon*   HRichHistFiller::hRichPulseheightAddon=0;
HMonHistAddon*   HRichHistFiller::hRichChargedParticlesAddon=0;
HMonHistAddon*   HRichHistFiller::hRichMultiplicityAddon=0;
HMonHistAddon*   HRichHistFiller::hRichmult_rawA=0;
HMonHistAddon*   HRichHistFiller::hRichmulttemp_rawA=0;
HMonHistAddon*   HRichHistFiller::hRichmulttrend_rawA=0;

ClassImp(HRichHistFiller)

HRichHistFiller::HRichHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
  colorsSector.Set(6);
  colorsCuts.Set(4);
}
HRichHistFiller::~HRichHistFiller()
{
}

void HRichHistFiller::fillRawCategory(TIterator* RichRawIterator)
{
  //Loop over all raw entries and fill the corresponding histogram
  Int_t nHitsPerSector[6] = {0,0,0,0,0,0};
  Int_t nLoudPadsPerSector[6] = {0,0,0,0,0,0};

  Float_t nChargedParticlesOld=0.0;
  Float_t nChargedParticlesNew=0.0;
  Float_t foundChargedParticle=0.0;
  

  HRichRaw* RichRawData;
  while((RichRawData=(HRichRaw *)RichRawIterator->Next())!=0 ) {
    Int_t s=RichRawData->getSector();
    Float_t c=RichRawData->getCharge();
    nHitsPerSector[s]++;
    
    //We count how many pads had a huge signal. These signals stem from charged
    //particles traversing the backplane and are only visible if the voltage is OK!
    if(c>200) nLoudPadsPerSector[s]++;
  }

  if(hRichMultiplicity) {
    for (Int_t sectorCount=0; sectorCount<6;sectorCount++){
      hRichMultiplicity->fill(0,sectorCount,nHitsPerSector[sectorCount]);
    } 
  }

  if(hRichChargedParticles) {
    for (Int_t sectorCount=0; sectorCount<6;sectorCount++){
      if(nLoudPadsPerSector[sectorCount]>5)
	{
	  //We assume that we have seen a charged particle in this sector in this event.
	  //Therefore we increase the number of seen charged particles
	  foundChargedParticle=1.0;
	}
      else 
	{
	  foundChargedParticle=0.0;
	}
      
      TH1* currentHist = hRichChargedParticles->getP(0,sectorCount);
      nChargedParticlesOld = currentHist->GetBinContent(currentHist->GetNbinsX());
      nChargedParticlesNew = 0.9999*nChargedParticlesOld+0.0001*foundChargedParticle;
      hRichChargedParticles->fill(0,sectorCount,nChargedParticlesNew);
    } 
  }

}

void HRichHistFiller::fillCalCategory(TIterator* RichCalIterator)
{
    // static Int_t maxSec=6; // unused
    static Int_t maxCol=92;
    static Int_t maxRow=90;
    Float_t ChargeArray[6][92][90];
    //Float_t ChargeArray2[6][92][90];
    
    Int_t sec;
    Float_t charge;
    Int_t row;
    Int_t col;
    Int_t evtnr;
    
    HRichCal* listElement;
    if(hRichMyPattern){
	hRichMyPattern->reset();
    }
    
    
    if(hRichPulseheight){
	for (Int_t sectorCount=0; sectorCount<6;sectorCount++){
	    for (Int_t colCount=0; colCount<maxCol;colCount++){
		for (Int_t rowCount=0; rowCount<maxRow;rowCount++){
		    ChargeArray[sectorCount][colCount][rowCount]=0.0;
		}
	    }
	}
	
	//Loop over all cal entries and fill the corresponding arraybin
	HRichCal* RichCalData;
	while((RichCalData=(HRichCal *)RichCalIterator->Next())!=0 ) {
	    sec=RichCalData->getSector();
	    charge=RichCalData->getCharge();
	    row=RichCalData->getRow();
	    col=RichCalData->getCol();
	    evtnr = RichCalData->getEventNr();
	    ChargeArray[sec][col][row]+=charge;
	    
	    if(hRichMyPattern){
		listElement = new HRichCal(0.0);
		listElement->setCharge(charge);
		listElement->setSector(sec);
		listElement->setRow(row);
		listElement->setCol(col);
		listElement->setEventNr(evtnr);
		(hRichMyPattern->getNewList())->Add(listElement);
	    }
	    
	}

	for (Int_t sectorCount=0; sectorCount<6;sectorCount++){
	    for (Int_t colCount=0; colCount<maxCol;colCount++){
		for (Int_t rowCount=0; rowCount<maxRow;rowCount++){
		    Stat_t OldBinContent = hRichPulseheight->getBinContent(0, sectorCount,colCount,rowCount);
		    Stat_t NewBinContent = 1.0e-4 * ChargeArray[sectorCount][colCount][rowCount] + 9999.0e-4 * OldBinContent;
		    hRichPulseheight->setBinContent(0, sectorCount, colCount, rowCount, NewBinContent);
		}
	    }
	}
	
    }
    
}

void HRichHistFiller::fillRaw(HRichRaw* raw)
{
        Int_t    s=raw->getSector();
        Float_t ch=raw->getCharge();
	Int_t  col=raw->getCol();
        Int_t  row=raw->getRow();

	if(hRichPulseDist)hRichPulseDist->fill(0,s,ch);

        HRichMappingPar* richpar=(HRichMappingPar*)(gHades->getRuntimeDb()->findContainer("RichMappingParameters"));
	if(richpar)
	{
	    Int_t roc =richpar->getRC(col,row);
            Int_t port=richpar->getPort(col,row);

	    if(hRichmult_raw)    hRichmult_raw    ->getP()->Fill(roc*8+port,s);
	    if(hRichmulttemp_raw)hRichmulttemp_raw->getP()->Fill(roc*8+port,s);
	}

}
void HRichHistFiller::fillHit(HRichHit* hit)
{
}
void HRichHistFiller::fillCal(HRichCal* cal)
{
}
void HRichHistFiller::fillTrend()
{
    for(Int_t s=0;s<6;s++){
	if(hRichmulttemp_raw&&hRichmulttrend_raw){
	    hRichmulttrend_raw->fill(0,s,hRichmulttemp_raw->getP()->Integral(1,16,s+1,s+1));
	}
    }
    if(hRichmulttemp_raw)hRichmulttemp_raw->getP()->Reset();
}
void   HRichHistFiller::putRichParams(HParamList* l,TList* histpool)
{
    // Puts all params of HRichHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsSector"           ,colorsSector);
       l->add("colorsCuts"             ,colorsCuts  );
       hRichPulseDistAddon->getText(mytemp);
       l->add("hRichPulseDist"         ,mytemp,'s',500);

       hRichMultiplicityAddon->getText(mytemp);
       l->add("hRichMultiplicity"      , mytemp,'s',500);
       hRichPulseheightAddon->getText(mytemp);
       l->add("hRichPulseheight"       ,mytemp,'s',500);
       hRichChargedParticlesAddon->getText(mytemp);
       l->add("hRichChargedParticles"  ,mytemp,'s',500);
       sprintf(mytemp,"%s","s");
       l->add("hRichFancy"             ,mytemp,'s',500);
       sprintf(mytemp,"%s","s");
       l->add("hRichPattern"           ,mytemp,'s',500);
       hRichmult_rawA->getText(mytemp);
       l->add("hRichmult_raw"          ,mytemp,'s',500);
       hRichmulttemp_rawA->getText(mytemp);
       l->add("hRichmulttemp_raw"      ,mytemp,'s',500);
       hRichmulttrend_rawA->getText(mytemp);
       l->add("hRichmulttrend_raw"     ,mytemp,'s',500);



}
Bool_t HRichHistFiller::getRichParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsSector",&colorsSector))) return kFALSE;
    if(!(l->fill("colorsCuts"  ,&colorsCuts  ))) return kFALSE;

    
    Text_t mytemp[500];

    if(!(l->fill("hRichPattern",mytemp,500))) {cout << "No fancy display configured" << endl;}
    else{
      hRichFancyDisplay = new HRichFancy("RichOnlineDisplay","RichOnlineDisplay");
      hRichMyPattern=new HRichPattern("RichPattern","RichPattern");
    }

    if(!(l->fill("hRichPulseDist",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichPulseDistAddon=new HMonHistAddon(mytemp));
	if(hRichPulseDistAddon->getActive()==1)
	{
	    activeHists->Add(hRichPulseDist=new HHistArray(*hRichPulseDistAddon));
	    hRichPulseDist->getP(0,0)->SetLineColor(colorsSector[0]);
	    hRichPulseDist->getP(0,1)->SetLineColor(colorsSector[1]);
	    hRichPulseDist->getP(0,2)->SetLineColor(colorsSector[2]);
	    hRichPulseDist->getP(0,3)->SetLineColor(colorsSector[3]);
	    hRichPulseDist->getP(0,4)->SetLineColor(colorsSector[4]);
	    hRichPulseDist->getP(0,5)->SetLineColor(colorsSector[5]);
	}
    }

       if(!(l->fill("hRichPulseheight",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichPulseheightAddon = new HMonHistAddon(mytemp));
	if(hRichPulseheightAddon->getActive()==1)
	{
	    activeHists->Add(hRichPulseheight=new HHistArray2(*hRichPulseheightAddon));
	}
    } 

   if(!(l->fill("hRichMultiplicity",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichMultiplicityAddon = new HMonHistAddon(mytemp));
	if(hRichMultiplicityAddon->getActive()==1)
	{
	    activeHists->Add(hRichMultiplicity=new HTrendArray(*hRichMultiplicityAddon));
	    hRichMultiplicity->getP(0,0)->SetLineColor(colorsSector[0]);
	    hRichMultiplicity->getP(0,1)->SetLineColor(colorsSector[1]);
	    hRichMultiplicity->getP(0,2)->SetLineColor(colorsSector[2]);
	    hRichMultiplicity->getP(0,3)->SetLineColor(colorsSector[3]);
	    hRichMultiplicity->getP(0,4)->SetLineColor(colorsSector[4]);
	    hRichMultiplicity->getP(0,5)->SetLineColor(colorsSector[5]);
	}
    }

   if(!(l->fill("hRichChargedParticles",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichChargedParticlesAddon = new HMonHistAddon(mytemp));
	if(hRichChargedParticlesAddon->getActive()==1)
	{
	    activeHists->Add(hRichChargedParticles=new HTrendArray(*hRichChargedParticlesAddon));
	    hRichChargedParticles->getP(0,0)->SetLineColor(colorsSector[0]);
	    hRichChargedParticles->getP(0,1)->SetLineColor(colorsSector[1]);
	    hRichChargedParticles->getP(0,2)->SetLineColor(colorsSector[2]);
	    hRichChargedParticles->getP(0,3)->SetLineColor(colorsSector[3]);
	    hRichChargedParticles->getP(0,4)->SetLineColor(colorsSector[4]);
	    hRichChargedParticles->getP(0,5)->SetLineColor(colorsSector[5]);
	}
    }
    if(!(l->fill("hRichmult_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichmult_rawA=new HMonHistAddon(mytemp));
	if(hRichmult_rawA->getActive()==1)
	{
	    activeHists->Add(hRichmult_raw=new HMonHist2(*hRichmult_rawA));
	}
    }
    if(!(l->fill("hRichmulttemp_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichmulttemp_rawA=new HMonHistAddon(mytemp));
	if(hRichmulttemp_rawA->getActive()==1)
	{
	    activeHists->Add(hRichmulttemp_raw=new HMonHist2(*hRichmulttemp_rawA));
	}
    }
    if(!(l->fill("hRichmulttrend_raw",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hRichmulttrend_rawA=new HMonHistAddon(mytemp));
	if(hRichmulttrend_rawA->getActive()==1)
	{
	    activeHists->Add(hRichmulttrend_raw=new HTrendArray(*hRichmulttrend_rawA));
	    for(Int_t s=0;s<6;s++){
		hRichmulttrend_raw->getP(0,s)->SetLineColor(colorsSector[s]);
		hRichmulttrend_raw->getP(0,s)->SetMarkerColor(colorsSector[s]);
	    }
	}
    }




    return kTRUE;
}

