//*-- Modified : 18/12/2001 by I.Koenig
//*-- Modified : 01/12/2000 by I.Koenig
using namespace std;
#include "hhododummy.h"
#include "hododef.h"
#include "hhodoraw.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include <iostream> 
#include <iomanip>

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////
//
//  HHodoDummy: Was intended for testing data w/o real data
//  
////////////////////////////////////////////////////////////////


HHodoDummy::HHodoDummy(void)
{
  rawCat=0;
  iter=0;
}

HHodoDummy::HHodoDummy(Text_t *name,Text_t *title) : HReconstructor(name,title)
{
  rawCat=0;
  iter=0;
  printf("create Dummy!\n");
}

HHodoDummy::~HHodoDummy(void)
{
}

Bool_t HHodoDummy::init(void)
{
  printf("init Dummy!\n");
  rawCat=gHades->getCurrentEvent()->getCategory(catHodoRaw);
  if (!rawCat) {
    rawCat=gHades->getSetup()->getDetector("Hodo")->buildCategory(catHodoRaw);
    if (!rawCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catHodoRaw,rawCat,"Hodo");
  }

  iter=(HIterator *)rawCat->MakeIterator();
  loc.set(2,0,0);
  fActive=kTRUE;

  printf("init OK!\n");
  return kTRUE;
}

Int_t HHodoDummy::execute(void)
{// What will be done here (if uncommented):
 // Copy all data from module 0 to 1 and vice versa
 // Kind of simulates more "hardware"
 // Do not use unless you know what you are doing!
   printf("Exec Dummy\n");
/*
	HHodoRaw *raw=0;
	HHodoRaw *pRaw=0;
	// printf("Exec Dummy\n");

	iter->Reset();

	while ((raw=(HHodoRaw *)iter->Next())!=0) {
		Int_t mod, strip;
		raw->getAddress(mod,strip);
		if (mod>=0 && strip>=0){
			if( mod==0) mod=1; else mod=0;
			loc[0]=mod;
			loc[1]=strip;

			pRaw=(HHodoRaw*)rawCat->getObject(loc);
			if (!pRaw){
				// was die folgende Zeilen machen ist mir vollkommen unklar!!!
				pRaw=(HHodoRaw*)rawCat->getSlot(loc);
				if (pRaw) {
					pRaw=new (pRaw) HHodoRaw;
					pRaw->setAddress(mod,strip);
				}
				else {
					Error("execute()",
					"Can't get slot\n");
					return EXIT_FAILURE;
				}
				pRaw->setAddress(mod,strip);

				Int_t n;
				n=raw->getNHits();
				for(Int_t i=0; i<n; i++){
					Int_t t, w;
					if(i<4){
						raw->getTimeAndWidth(i+1,t,w);
					}else{
						t=-1;
						w=-1;
					}
					pRaw->fill(t,w);
					// das "kopiert" auch den counter noch mit
				}
			}
		}
	}
*/
	return 0;
}

ClassImp(HHodoDummy)
