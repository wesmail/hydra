//*-- Created 30.3.2006 by B. Spruck
//*-- Modified 27.3.2007 by B. Spruck

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////
//
//  HHodoPionMom:
//
//  At the moment this code is not really useful.
//  In the future, it should calculate the pion momentum from
//  hits in the two pion hodoscopes and store it /somewhere/.
//  At the moment it is just checking IF the combination of the
//  hits in the pion hodoscopes fulfil some requirement
//  (= is within some momentum range) and return success or not
//  -> reject event if not in desired range
//
////////////////////////////////////////////////////////////////


using namespace std;

#include "hhodopionmom.h"
#include "hododef.h"
#include "hhodohit.h"
#include "hhododetector.h"
#include "hdebug.h"
#include "hades.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hcategory.h"
#include "hlinearcategory.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>


HHodoPionMom::HHodoPionMom(void)
{
  hitHodoCat=0;
  iter=0;
}

HHodoPionMom::HHodoPionMom(const Text_t *name,const Text_t *title) : HReconstructor(name,title)
{
  hitHodoCat=0;
  iter=0;
}

HHodoPionMom::~HHodoPionMom(void)
{
}

Bool_t HHodoPionMom::init(void)
{
   HHodoDetector *hodo;
   hodo=(HHodoDetector *)gHades->getSetup()->getDetector("Hodo");

   if(!hodo){
      Error("init","No Hodo Det. found.");
      return kFALSE;
   }

   // Categories
   hitHodoCat=hodo->buildCategory(catHodoHit);
   if (!hitHodoCat) return kFALSE;

  iter=(HIterator *)hitHodoCat->MakeIterator();

  fActive=kTRUE;

  return kTRUE;
}

Int_t HHodoPionMom::execute(void)
{// What will be done here
 // Copy all Hit data from Hodo module "usemod" to Start module 0
 // Used for may06 where hodo is used as a start det.
 // Do not use unless you know what you are doing!

   HHodoHit *pHodoHit=0;

   iter->Reset();

   //  Hit finding & storing.
    Int_t mod=-1;
    Int_t cnt1=0, cnt2=0;
    Int_t strip1=-1, strip2=-1;

    while ((pHodoHit=(HHodoHit *)iter->Next())!=0) {
      Int_t st;
      pHodoHit->getAddress(mod,st);
      if (mod==usemod1  && st>=0){
        strip1=st;
        cnt1++;
      }else if (mod==usemod2  && st>=0){
        strip2=st;
        cnt2++;
      }
    }

  if (1==cnt1 && 1==cnt2){
    Float_t Mittelwert;
    Mittelwert = 0.5*(strip1+strip2);
    Float_t Differenz;
    Differenz = 0.5*(strip1 - strip2);
    cout << "Mittelwert" << Mittelwert<< "Differenz"<<Differenz<<endl;
    if (Mittelwert<mean_max && Mittelwert>mean_min && Differenz<diff_max && Differenz>diff_min){
      return EXIT_SUCCESS;
    }
  }
  return EXIT_FAILURE;
}

ClassImp(HHodoPionMom)
