// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HMdcFillTime1Hists
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcfilltime1hists.h"
#include "hmdcdef.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlocation.h"
#include "hmdccal1.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"

#include <iostream.h>
#include <stdlib.h>
#include <TH2.h>
#include <TDirectory.h>
#include <TStopwatch.h>

ClassImp(HMdcFillTime1Hists)

HMdcFillTime1Hists::HMdcFillTime1Hists(void) {
  // Default constructor calls the function setParContainers().
  calCat=0;
  iter=0;
  lookup=0;
  lookupRaw=0;

}

HMdcFillTime1Hists::HMdcFillTime1Hists(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments. It also calls the function setParContainers().
  calCat=0;
  iter=0;
  lookup=0;
  lookupRaw=0;

}

HMdcFillTime1Hists::~HMdcFillTime1Hists(void) {
  // destructor deletes the iterator
  if (iter) delete iter;
  iter=0;
}


void HMdcFillTime1Hists::setParContainers() {
  // creates the parameter containers MdcLookupGeom/MdcLookupRaw if they do not
  // exist and adds them to the list of parameter containers in the rumtime
  // database
    lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
    lookupRaw=(HMdcLookupRaw*)gHades->getRuntimeDb()->getContainer("MdcLookupRaw");
}

void HMdcFillTime1Hists::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcFillTimeHists where all created histograms were written.
    //
    if (fNameRootHists) delete fNameRootHists;
    fNameRootHists = new Char_t[strlen(c)+1];
    strcpy(fNameRootHists, c);
}

Bool_t HMdcFillTime1Hists::init(void) {
  // creates the MdcRaw  and MdcCal1 categories and adds them to the current
  // event
  // creates an iterator which loops over all fired cells
  // calls the function setParContainers()
  setParContainers();
  calCat=gHades->getCurrentEvent()->getCategory(catMdcCal1);
  if (!calCat) {
    calCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcCal1);
    if (!calCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcCal1,calCat,"Mdc");
  }

  iter=(HIterator *)calCat->MakeIterator("native");
  loc.set(4,0,0,0,0);


   Char_t name [100];
   Char_t title[100];

   for(Int_t s=0;s<6;s++)
   {
       for(Int_t m=0;m<4;m++)
       {
	   for(Int_t mb=0;mb<16;mb++)
	   {
	       sprintf(name,"%s%i%s%i%s%02i%s","time_tdc[",s,"][",m,"][",mb,"]");
	       sprintf(title,"%s%i%s%i%s%02i","time_vers_tdc_",s,"_",m,"_",mb);
	       hmdctime_tdc[s][m][mb]=new TH2F(name,title,600,-50,550,100,0,100);
	   }
       }
   }
   for(Int_t s=0;s<6;s++)
   {
       for(Int_t m=0;m<4;m++)
       {
	   for(Int_t l=0;l<6;l++)
	   {
	       sprintf(name,"%s%i%s%i%s%02i%s","time_layer[",s,"][",m,"][",l,"]");
	       sprintf(title,"%s%i%s%i%s%02i","time_vers_layer_",s,"_",m,"_",l);
	       hmdctime_layer[s][m][l]=new TH2F(name,title,600,-50,550,220,0,220);
	   }
       }
   }

   fActive=kTRUE;
   return kTRUE;
}
TDirectory *HMdcFillTime1Hists::Mkdir(TDirectory *dirOld, Char_t *c, Int_t i, Int_t p=1)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
{
    // Function to create subdirectories
    static Char_t sDir[255];// = new Char_t[strlen(c)+3];
    static Char_t sFmt[10];
    sprintf(sFmt, "%%s %%0%1ii", p);
    sprintf(sDir, sFmt, c, i);
    TDirectory *dirNew = dirOld->mkdir(sDir);
    dirOld->cd(sDir);
    //  free (sDir);
    return dirNew;
}

Bool_t HMdcFillTime1Hists::finalize(void)
{
    TFile *file = NULL;
    if (fNameRootHists)
	file = new TFile(fNameRootHists,"RECREATE");
    file->cd();
    HDetector *mdcDet = gHades->getSetup()->getDetector("Mdc");

    if (!mdcDet)
        cout << "Error in HMdcOffsetCheck::finalize: Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing." << endl;
    else
	for(Int_t s=0; s<6; s++) {  //loop over sectors
            cout<<"Sector "<<s<<endl;
	    TDirectory *dirSec=NULL;
	    if (file) dirSec=Mkdir(file, "sector", s);
            for(Int_t mo=0; mo<4; mo++) {  //loop over modules
		cout<<"Module "<<mo<<endl;
		if (!mdcDet->getModule(s, mo)) continue;
                TDirectory *dirMod=NULL;
		if (dirSec) dirMod=Mkdir(dirSec, "module", mo);
		//Int_t nMb=(*calparraw)[s][mo].getSize();
		    for(Int_t l=0;l<6;l++)
		    {
			hmdctime_layer[s][mo][l]->Write();
		    }
		for(Int_t mb=0; mb<16; mb++) {  //loop over layer
		   // TDirectory *dirMbo=NULL;
		   // if (dirMod)
		   // {
		   //     dirMbo=Mkdir(dirMod, "mbo", mb, 2);
		   // }
		    hmdctime_tdc[s][mo][mb]->Write();

		   // if (dirMbo) dirMbo->TDirectory::Cd("..");
		    cout << "." << flush;
		}
		cout<<" "<<endl;
		if (dirMod) dirMod->TDirectory::Cd("..");
	    }
	    cout<<" "<<endl;
	    if (dirSec) dirSec->TDirectory::Cd("..");
	}
    cout << endl;
    file->Save();
    file->Close();
    return kTRUE;

}

Int_t HMdcFillTime1Hists::execute()
{

    Bool_t debug = kFALSE;
	HMdcCal1* cal=0;
	Int_t mb, ch;
        Int_t l, c;
        Int_t s,m;
        Float_t time1=-99;
	iter->Reset();
	while ((cal=(HMdcCal1 *)iter->Next())!=0) {
	    cal->getAddress(s,m,l,c);
	    HMdcLookupCell& cell=(*lookupRaw)[s][m][l][c];
	    mb=cell.getNMoth();
	    ch=cell.getNChan();
            time1=cal->getTime1();
	    hmdctime_tdc[s][m][mb]->Fill(time1,ch);
            hmdctime_layer[s][m][l]->Fill(time1,c);

	    time1=-99;

	}//

    return 0;
}

