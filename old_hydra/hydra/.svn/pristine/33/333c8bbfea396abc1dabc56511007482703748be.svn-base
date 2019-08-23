//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HTriggerHistFiller
//
// Container class for the trigger hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "htriggerhistfiller.h"
#include "hparamlist.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"

#include "triggerinfodef.h"
#include "hmatchurich.h"
#include "hmatchutof.h"
#include "hmatchushower.h"
#include "hmuleptons.h"
#include "hiterator.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include <TList.h>
#include <TArrayI.h>
#include <TString.h>

// Color definitions
TArrayI HTriggerHistFiller::colorsModule;
TArrayI HTriggerHistFiller::colorsCuts;
TArrayI HTriggerHistFiller::colorsIPUtrend;

// Hist Pointers
HMonHist*        HTriggerHistFiller::htof=0;
HHistArray*      HTriggerHistFiller::htriggermultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggerrichringmultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggertofhitmultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggershowerhitmultdistrib=0;
HHistArray*      HTriggerHistFiller::htriggermatchuhitmultdistrib=0;
HMonHist*        HTriggerHistFiller::htofnumbers=0;
HMonHist*        HTriggerHistFiller::hrichnumbers=0;
HTrendArray*     HTriggerHistFiller::hiputrendRich=0;
HTrendArray*     HTriggerHistFiller::hiputrendTof=0;
HTrendArray*     HTriggerHistFiller::hiputrendShower=0;
HTrendArray*     HTriggerHistFiller::hiputrendMatchU=0;

// Hist Addon Pointers
HMonHistAddon* HTriggerHistFiller::htofA=0;
HMonHistAddon* HTriggerHistFiller::htriggermultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggerrichringmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggertofhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggershowerhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htriggermatchuhitmultdistribA=0;
HMonHistAddon* HTriggerHistFiller::htofnumbersA=0;
HMonHistAddon* HTriggerHistFiller::hrichnumbersA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendRichA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendTofA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendShowerA=0;
HMonHistAddon* HTriggerHistFiller::hiputrendMatchUA=0;

ClassImp(HTriggerHistFiller)

HTriggerHistFiller::HTriggerHistFiller(const char* name,const char* title)
    : TNamed(name,title)
{
    colorsModule.Set(2);
    colorsCuts.Set(4);
    colorsIPUtrend.Set(7);
    resetCounter();
}
HTriggerHistFiller::~HTriggerHistFiller()
{
}
void HTriggerHistFiller::fillRich(HIterator* iterrich)
{
    HMatchURich* rich=0;
    Int_t s=0;
    Int_t count[7]; //count number of rings per sector (0-5) and global (6)
    for(Int_t i=0;i<7;i++) { count[i]=0; }
    while ((rich=(HMatchURich *)iterrich->Next())!=0) {
	s=rich->getSegmentId();
	iputrendhit[0][s]++;
        iputrendhit[0][6]++;
	count[6]++;
	if((s>=0) && (s<=5)) {count[s]++;} else { cout << "---trigger: ---fillRich: ---Sector out of bounds... should be 0-5 but is: " << s << endl; }
    }
    for(Int_t i=0;i<7;i++){iputrendevent[0][i]++;}
    for(Int_t i=0;i<3;i++){richCounter[0][i]++;  }

    if(count[6]>=2)     { for(Int_t i=0;i<3;i++){ richCounter [1][i]++; richCounter [2][i]++; } }
    else if(count[6]>=1){ for(Int_t i=0;i<3;i++){ richCounter [1][i]++;                       } }

    if(richCounter[0][0]==100)  { richCounter[0][0]=0; for(Int_t i=0;i<2;i++){ richResult[i][0]=(Float_t)richCounter[i+1][0]/100.0;  richCounter[i+1][0]=0; } }
    if(richCounter[0][1]==1000) { richCounter[0][1]=0; for(Int_t i=0;i<2;i++){ richResult[i][1]=(Float_t)richCounter[i+1][1]/1000.0; richCounter[i+1][1]=0; } }
    if(richCounter[0][2]==10000){ richCounter[0][2]=0; for(Int_t i=0;i<2;i++){ richResult[i][2]=(Float_t)richCounter[i+1][2]/10000.0;richCounter[i+1][2]=0; } }

    if(htriggerrichringmultdistrib) { for(Int_t i=0;i<6;i++) { htriggerrichringmultdistrib->fill(0,i,count[i]); } }
    if(htriggermultdistrib)         {                          htriggermultdistrib        ->fill(0,0,count[6]);   }
}
void HTriggerHistFiller::fillTof(HIterator* itertof)
{
    HMatchUTof * tof=0;
    Int_t s=0;
    Int_t count[7]; //count number of hits per sector (0-5) and global (6)
    for(Int_t i=0;i<7;i++) { count[i]=0; }
    Int_t pidTof=0;
    Float_t time;
    while ((tof=(HMatchUTof *)itertof->Next())!=0) {
	time=tof->getTime();
	if(htof)htof->getP()->Fill(time);
	s      = tof->getSector();
	pidTof = tof->getPID();
	iputrendhit[1][s]++;
        iputrendhit[1][6]++;
	if(pidTof==1) { tofLeptons[s]++; tofLeptons[6]++; }
	count[6]++;
	if((s>=0) && (s<=5)) {count[s]++;} else { cout << "---trigger: ---fillTof: ---Sector out of bounds... should be 0-5 but is: " << s << endl; }
    }
    for(Int_t i=0;i<7;i++){iputrendevent[1][i]++;}

    if(htriggertofhitmultdistrib) { for(Int_t i=0;i<6;i++) { htriggertofhitmultdistrib->fill(0,i,count[i]); } }
    if(htriggermultdistrib)       {                          htriggermultdistrib      ->fill(0,1,count[6]);   }
}
void HTriggerHistFiller::fillShower(HIterator* itershower)
{
    HMatchUShower* shower=0;
    Int_t s=0;
    Int_t count[7]; //count number of hits per sector (0-5) and global (6)
    for(Int_t i=0;i<7;i++) { count[i]=0; }
    while ((shower=(HMatchUShower*)itershower->Next())!=0) {
	s=shower->getSector();
	iputrendhit[2][s]++;
	iputrendhit[2][6]++;
	count[6]++;
	if((s>=0) && (s<=5)) {count[s]++;} else { cout << "---trigger: ---fillShower: ---Sector out of bounds... should be 0-5 but is: " << s << endl; }
    }
    for(Int_t i=0;i<7;i++){iputrendevent[2][i]++;}

    if(htriggershowerhitmultdistrib) { for(Int_t i=0;i<6;i++) { htriggershowerhitmultdistrib->fill(0,i,count[i]); } }
    if(htriggermultdistrib)          {                          htriggermultdistrib         ->fill(0,2,count[6]);   }
}
void HTriggerHistFiller::fillMatchU(HIterator* itermatchu )
{
    HMULeptons* matchu=0;
    Int_t s=0;
    Int_t count[7]; //count number of hits per sector (0-5) and global (6)
    for(Int_t i=0;i<7;i++) { count[i]=0; }
    while ((matchu=(HMULeptons*)itermatchu->Next())!=0) {
	s=matchu->getSector();
	iputrendhit[3][s]++;
	iputrendhit[3][6]++;
	count[6]++;
	if((s>=0) && (s<=5)) {count[s]++;} else { cout << "---trigger: ---fillMatchU: ---Sector out of bounds... should be 0-5 but is: " << s << endl; }
    }
    for(Int_t i=0;i<7;i++){iputrendevent[3][i]++;}

    if(htriggermatchuhitmultdistrib) { for(Int_t i=0;i<6;i++) { htriggermatchuhitmultdistrib->fill(0,i,count[i]); } }
    if(htriggermultdistrib)          {                          htriggermultdistrib         ->fill(0,3,count[6]);   }
}

void HTriggerHistFiller::fillTrend()
{
    for(Int_t i=0;i<4;i++){
	for(Int_t j=0;j<7;j++){
	    if(iputrendevent[i][j]!=0)
	    {
	      if(hiputrendRich  &&i==0)hiputrendRich  ->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
	      if(hiputrendTof   &&i==1)hiputrendTof   ->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
	      if(hiputrendShower&&i==2)hiputrendShower->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
	      if(hiputrendMatchU&&i==3)hiputrendMatchU->fill(0,j,((Float_t)iputrendhit[i][j])/((Float_t)iputrendevent[i][j]));
	    }
	}
    }
    if(htofnumbers!=0)
      {
	for(Int_t i=0;i<7;i++)
	  {
	    if(iputrendhit[1][i] != 0)htofnumbers->getP()->SetBinContent( i+1, ((Float_t)tofLeptons[i])/((Float_t)iputrendhit[1][i]) );
	  }
      }
    if(hrichnumbers!=0)
      {
	for(Int_t i=0;i<3;i++)
	  {
	    hrichnumbers->getP()->SetBinContent( i+1,   richResult[0][i] );
	    hrichnumbers->getP()->SetBinContent( i+1+3, richResult[1][i] );
	  }
      }

    resetCounter();
}
void   HTriggerHistFiller::putTriggerParams(HParamList* l,TList* histpool)
{
    // Puts all params of HTriggerHistFiller to the parameter list of
    // HParamList (which ist used by the io);

       Text_t mytemp[500];
       l->add("colorsModule"           ,colorsModule    );
       l->add("colorsCuts"             ,colorsCuts      );
       l->add("colorsIPUtrend"         ,colorsIPUtrend  );
       htofA->getText(mytemp);
       l->add("htof"                          ,mytemp,500);
       hiputrendRichA->getText(mytemp);
       l->add("hiputrendRich"                 ,mytemp,500);
       hiputrendTofA->getText(mytemp);
       l->add("hiputrendTof"                  ,mytemp,500);
       hiputrendShowerA->getText(mytemp);
       l->add("hiputrendShower"               ,mytemp,500);
       hiputrendMatchUA->getText(mytemp);
       l->add("hiputrendMatchU"               ,mytemp,500);
       htofnumbersA->getText(mytemp);
       l->add("htofnumbers"                   ,mytemp,500);
       hrichnumbersA->getText(mytemp);
       l->add("hrichnumbers"                  ,mytemp,500);
       htriggermultdistribA->getText(mytemp);
       l->add("htriggermultdistrib"           ,mytemp,500);
       htriggerrichringmultdistribA->getText(mytemp);
       l->add("htriggerrichringmultdistrib"   ,mytemp,500);
       htriggertofhitmultdistribA->getText(mytemp);
       l->add("htriggertofhitmultdistrib"     ,mytemp,500);
       htriggershowerhitmultdistribA->getText(mytemp);
       l->add("htriggershowerhitmultdistrib"  ,mytemp,500);
       htriggermatchuhitmultdistribA->getText(mytemp);
       l->add("htriggermatchuhitmultdistrib"  ,mytemp,500);
}
Bool_t HTriggerHistFiller::getTriggerParams(HParamList* l,TList* histpool,TList* activeHists)
{
    if(!(l->fill("colorsModule"  ,&colorsModule)))     return kFALSE;
    if(!(l->fill("colorsCuts"    ,&colorsCuts  )))     return kFALSE;
    if(!(l->fill("colorsIPUtrend",&colorsIPUtrend  ))) return kFALSE;

    Text_t mytemp[500];
    if(!(l->fill("htof",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofA=new HMonHistAddon(mytemp));
	if(htofA->getActive()==1)
	{
	    activeHists->Add(htof=new HMonHist(*htofA));
	}
    }
    if(!(l->fill("hiputrendRich",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hiputrendRichA=new HMonHistAddon(mytemp));
	if(hiputrendRichA->getActive()==1)
	{
	    activeHists->Add(hiputrendRich=new HTrendArray(*hiputrendRichA));
	    for(Int_t sec=0;sec<7;sec++){
		hiputrendRich->getP(0,sec)->SetLineColor(colorsIPUtrend[sec]);
	    }
	}
    }
    if(!(l->fill("hiputrendTof",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hiputrendTofA=new HMonHistAddon(mytemp));
	if(hiputrendTofA->getActive()==1)
	{
	    activeHists->Add(hiputrendTof=new HTrendArray(*hiputrendTofA));
	    for(Int_t sec=0;sec<7;sec++){
		hiputrendTof->getP(0,sec)->SetLineColor(colorsIPUtrend[sec]);
	    }
	}
    }
    if(!(l->fill("hiputrendShower",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hiputrendShowerA=new HMonHistAddon(mytemp));
	if(hiputrendShowerA->getActive()==1)
	{
	    activeHists->Add(hiputrendShower=new HTrendArray(*hiputrendShowerA));
	    for(Int_t sec=0;sec<7;sec++){
		hiputrendShower->getP(0,sec)->SetLineColor(colorsIPUtrend[sec]);
	    }
	}
    }
    if(!(l->fill("hiputrendMatchU",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hiputrendMatchUA=new HMonHistAddon(mytemp));
	if(hiputrendMatchUA->getActive()==1)
	{
	    activeHists->Add(hiputrendMatchU=new HTrendArray(*hiputrendMatchUA));
	    for(Int_t sec=0;sec<7;sec++){
		hiputrendMatchU->getP(0,sec)->SetLineColor(colorsIPUtrend[sec]);
	    }
	}
    }
    if(!(l->fill("htofnumbers",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htofnumbersA=new HMonHistAddon(mytemp));
	if(htofnumbersA->getActive()==1)
	{
	    activeHists->Add(htofnumbers=new HMonHist(*htofnumbersA));
	}
    }
    if(!(l->fill("hrichnumbers",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(hrichnumbersA=new HMonHistAddon(mytemp));
	if(hrichnumbersA->getActive()==1)
	{
	    activeHists->Add(hrichnumbers=new HMonHist(*hrichnumbersA));
	}
    }
    if(!(l->fill("htriggermultdistrib",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htriggermultdistribA=new HMonHistAddon(mytemp));
	if(htriggermultdistribA->getActive()==1)
	{
	    activeHists->Add(htriggermultdistrib=new HHistArray(*htriggermultdistribA));
	    for(Int_t sec=0;sec<4;sec++){
		htriggermultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
    if(!(l->fill("htriggerrichringmultdistrib",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htriggerrichringmultdistribA=new HMonHistAddon(mytemp));
	if(htriggerrichringmultdistribA->getActive()==1)
	{
	    activeHists->Add(htriggerrichringmultdistrib=new HHistArray(*htriggerrichringmultdistribA));
	    for(Int_t sec=0;sec<6;sec++){
		htriggerrichringmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
    if(!(l->fill("htriggertofhitmultdistrib",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htriggertofhitmultdistribA=new HMonHistAddon(mytemp));
	if(htriggertofhitmultdistribA->getActive()==1)
	{
	    activeHists->Add(htriggertofhitmultdistrib=new HHistArray(*htriggertofhitmultdistribA));
	    for(Int_t sec=0;sec<6;sec++){
		htriggertofhitmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
    if(!(l->fill("htriggershowerhitmultdistrib",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htriggershowerhitmultdistribA=new HMonHistAddon(mytemp));
	if(htriggershowerhitmultdistribA->getActive()==1)
	{
	    activeHists->Add(htriggershowerhitmultdistrib=new HHistArray(*htriggershowerhitmultdistribA));
	    for(Int_t sec=0;sec<6;sec++){
		htriggershowerhitmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
    if(!(l->fill("htriggermatchuhitmultdistrib",mytemp,500))) {return kFALSE;}
    else
    {
	histpool->Add(htriggermatchuhitmultdistribA=new HMonHistAddon(mytemp));
	if(htriggermatchuhitmultdistribA->getActive()==1)
	{
	    activeHists->Add(htriggermatchuhitmultdistrib=new HHistArray(*htriggermatchuhitmultdistribA));
	    for(Int_t sec=0;sec<6;sec++){
		htriggermatchuhitmultdistrib->getP(0,sec)->SetLineColor(17);
	    }
	}
    }
    return kTRUE;
}
