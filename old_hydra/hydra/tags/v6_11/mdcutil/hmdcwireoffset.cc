// : Author  J.Markert

///////////////////////////////////////////////////////////////////////////////
//
// HMdcWireOffset
//
///////////////////////////////////////////////////////////////////////////////

#include "hmdcwireoffset.h"
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
#include "hmdchit.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcgeomstruct.h"
#include "hmdcgeompar.h"
#include "hmdclayergeompar.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdcsizescells.h"

#include <iostream.h>
#include <iomanip.h>
#include <stdlib.h>
#include <TH2.h>
#include <TDirectory.h>
#include <TStopwatch.h>

ClassImp(HMdcWireOffset)

HMdcWireOffset::HMdcWireOffset(void) {
  // Default constructor calls the function setParContainers().
  calCat=0;
  hitCat=0;
  itercal1=0;
  iterhit=0;
  lookup=0;
  lookupRaw=0;
  mdcLayerGeomPar=0;
  mysector=0;
  mymodule=3;
  fNameRootHists=0;
  windowWidth[0]=100;
  windowWidth[1]=100;
  windowWidth[2]=100;
  windowWidth[3]=100;
}

HMdcWireOffset::HMdcWireOffset(Text_t* name,Text_t* title)
                 :  HReconstructor(name,title) {
  // Constructor calls the constructor of class HReconstructor with the name
  // and the title as arguments. It also calls the function setParContainers().
  calCat=0;
  hitCat=0;
  itercal1=0;
  iterhit=0;
  lookup=0;
  lookupRaw=0;
  mdcLayerGeomPar=0;
  mysector=0;
  mymodule=3;
  fNameRootHists=0;
  windowWidth[0]=100;
  windowWidth[1]=100;
  windowWidth[2]=100;
  windowWidth[3]=100;
  }

HMdcWireOffset::~HMdcWireOffset(void) {
  // destructor deletes the iterator
  if (itercal1) delete itercal1;
  if (iterhit) delete iterhit;
  itercal1=0;
  iterhit=0;
  mysector=0;
  mymodule=3;
}


void HMdcWireOffset::setParContainers() {
  // creates the parameter containers MdcLookupGeom/MdcLookupRaw if they do not
  // exist and adds them to the list of parameter containers in the rumtime
  // database
    lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()->getContainer("MdcLookupGeom");
    lookupRaw=(HMdcLookupRaw*)gHades->getRuntimeDb()->getContainer("MdcLookupRaw");
    geomstruct=(HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
    mdcGeomPar=(HMdcGeomPar*)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
    mdcLayerGeomPar=(HMdcLayerGeomPar*)gHades->getRuntimeDb()->getContainer("MdcLayerGeomPar");
}
void HMdcWireOffset::printStatus()
{
    cout<<"*******************HMdcWireOffset*********************"<<endl;
    cout<<"* SELECTED MODULE: Sector "<<mysector<<" Module "<<mymodule<<"                 *"<<endl;
    cout<<"* window width: "<<setw(3)<<windowWidth[0]<<","<<setw(3)<<windowWidth[1]<<","<<setw(3)<<windowWidth[2]<<","<<setw(3)<<windowWidth[3]<<"                      *"<<endl;
    cout<<"******************************************************"<<endl;
}
void HMdcWireOffset::setOutputRoot(Char_t *c)
{
    // Sets rootfile output of HMdcFillTimeHists where all created histograms were written.
    //
    if (fNameRootHists) delete fNameRootHists;
    fNameRootHists = new Char_t[strlen(c)+1];
    strcpy(fNameRootHists, c);
}

Bool_t HMdcWireOffset::reinit(void)
{

    // get the container HMdcSizesCells, which holds the position of the wire end points
    fsizescells=HMdcSizesCells::getObject();
    const HGeomVector *point1;
    const HGeomVector *point2;

    for(Int_t s=0;s<6;s++)
    {
	HMdcSizesCellsSec* sec=&(*fsizescells)[s];
	if( !sec ) continue;
	for(Int_t m=0;m<4;m++)
	{
	    HMdcSizesCellsMod* mod=&(*sec)[m];
	    if( !mod ) continue;
	    Int_t nl=mod->getSize();
	    for(Int_t l=0;l<nl;l++)
	    {
		HMdcSizesCellsLayer* lay=&(*mod)[l];
		if( !lay ) continue;
		Int_t nc=lay->getSize();
		for(Int_t c=0;c<nc;c++)
		{

		  point1=(*lay)[c].getWirePoint(0);
                  point2=(*lay)[c].getWirePoint(1);

		  if(s==0 && m==3&& l==2)cout<<c<<" "<<point1->getX()<<" "<<point1->getY()<<" p2 "<<point2->getX()<<" "<<point2->getY()<<endl;
		}
	    }
	}
    }
    return kTRUE;
}


Bool_t HMdcWireOffset::init(void) {
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

  hitCat=gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!hitCat) {
    hitCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
    if (!hitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,hitCat,"Mdc");
  }

  itercal1=(HIterator *)calCat->MakeIterator("native");
  iterhit=(HIterator *)hitCat->MakeIterator("native");


  loccal1.set(4,0,0,0,0);

   Char_t name [100];
   Char_t title[100];

   for(Int_t l=0;l<6;l++)
   {
       for(Int_t w=0;w<(*geomstruct)[mysector][mymodule][l];w++)
       {
	   sprintf(name,"%s%i%s%03i%s","htime[",l,"][",w,"][0]");
	   sprintf(title,"%s%i%s%03i","time_pos_left_",l,"_",w);
	   hmdctime_layer[l][w][0]=new TH1F(name,title,1000,-100,900);
           hmdctime_layer[l][w][0]->SetLineColor(2);

	   sprintf(name,"%s%i%s%03i%s","htime[",l,"][",w,"][1]");
	   sprintf(title,"%s%i%s%03i","time_pos_mid_",l,"_",w);
	   hmdctime_layer[l][w][1]=new TH1F(name,title,1000,-100,900);
           hmdctime_layer[l][w][1]->SetLineColor(4);

	   sprintf(name,"%s%i%s%03i%s","htime[",l,"][",w,"][2]");
	   sprintf(title,"%s%i%s%03i","time_pos_right_",l,"_",w);
	   hmdctime_layer[l][w][2]=new TH1F(name,title,1000,-100,900);
           hmdctime_layer[l][w][2]->SetLineColor(8);

       }
   }
   printStatus();
   fActive=kTRUE;
   return kTRUE;
}
TDirectory *HMdcWireOffset::Mkdir(TDirectory *dirOld, Char_t *c, Int_t i, Int_t p=1)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
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

Bool_t HMdcWireOffset::finalize(void)
{
    TFile *file = NULL;
    if (fNameRootHists)
    {
	file = new TFile(fNameRootHists,"RECREATE");
	file->cd();
	for(Int_t l=0;l<6;l++)
	{
	    TDirectory *dirLay=NULL;
	    if (file) dirLay=Mkdir(file, "lay", l);

	    for(Int_t w=0;w<(*geomstruct)[mysector][mymodule][l];w++)
	    {
		hmdctime_layer[l][w][0]->Write();
		hmdctime_layer[l][w][1]->Write();
		hmdctime_layer[l][w][2]->Write();
		/* hmdctime_layer[l][w][1]->Delete();
		 hmdctime_layer[l][w][2]->Delete();
		 hmdctime_layer[l][w][3]->Delete();
		 */
	    }
	    if (dirLay) dirLay->TDirectory::Cd("..");
	    //dirLay->Close();
	}

	file->Save();
	file->Close();
    }else
    {
	cout<<"*******************HMdcWireOffset*********************"<<endl;
	cout<<"HMdcWireOffset:  NO OUTPUT DEFINED!!!!!              *"<<endl;
	cout<<"******************************************************"<<endl;
    }
    return kTRUE;

}
Double_t HMdcWireOffset::calcLength(Double_t x1,Double_t y1,Double_t x2,Double_t y2)
{
 return sqrt( (x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}
Int_t HMdcWireOffset::execute()
{
    Bool_t debug = kFALSE;
    HMdcCal1* cal=0;
    HMdcHit* hit=0;

    Int_t mb, ch;
    Int_t l, c;
    Int_t s,m;
    Float_t xcoor=0;
    Float_t ycoor=0;
    Int_t nCells=0; // number of cells per layer
    const HGeomVector *point1;
    const HGeomVector *point2;

    Float_t time1=-99;

    iterhit->Reset();
    while ((hit=(HMdcHit *)iterhit->Next())!=0) {
	s=hit->getSector();
	m=hit->getModule();
	if(s==mysector&&m==mymodule)
	{
	    xcoor=hit->getX();
	    ycoor=hit->getY();
	    for(Int_t l=0;l<6;l++)
	    {
		loccal1[0]=mysector;
		loccal1[1]=mymodule;
		loccal1[2]=l;

		for(Int_t index=0;index<hit->getNCells(l);index++)
		{
		    c=hit->getCell(l,index);
		    Float_t window=100;
		    loccal1[3]=c;
		    cal=(HMdcCal1*)calCat->getObject(loccal1);
		    time1=cal->getTime1();
		    if( (xcoor<window/2.)&&(xcoor>-window/2.) ) //mid
		    {
			hmdctime_layer[l][c][1]->Fill(time1);
			//cout<<"1"<<flush;
		    }
		    if( (xcoor<-500)&&(xcoor>-(500+window)) ) //left
		    {
			hmdctime_layer[l][c][0]->Fill(time1);
			//cout<<"0"<<flush;
		    }
		    if( (xcoor>500)&&(xcoor<(500+window)) ) //right
		    {
			hmdctime_layer[l][c][2]->Fill(time1);
			//cout<<"2"<<flush;
		    }


		}
	    }
	}

    }
    return 0;
}

