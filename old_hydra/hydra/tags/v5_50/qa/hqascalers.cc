#include <iostream.h>
#include <math.h>
// Root includes
#include <TObjString.h>
#include <TProfile.h>
// Hydra includes
#include "hqascalers.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

//////////////////////////////////////////////////////////////////////////////
//
// HQAScalers
//
// Contains HQAScaler and HQAScaler6 objects for monitoring scalar quantities,
// used by HQAMaker.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/qa/">QA home page</a>.</font></b> END_HTML
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HQAScalers)
ClassImp(HQAScaler)
ClassImp(HQAScaler6)

HQAScalers::HQAScalers() {

  scalerList = new TList();
  resetPointers();
}

void HQAScalers::resetPointers() {;
// Zero all histogram pointers

  //---- Rich scalers
  richCal_n = 0;     //! richCal: pads/event
  richHit_n = 0;     //! richHit: hits/event

  //---- Mdc scalers
  mdcCal1_n_m0 = 0;  //! mdcCal1: Plane I, avg. mult.
  mdcCal1_n_m1 = 0;  //! mdcCal1: Plane II, avg. mult.
  mdcCal1_n_m2 = 0;  //! mdcCal1: Plane III, avg. mult.
  mdcCal1_n_m3 = 0;  //! mdcCal1: Plane IV, avg. mult.

  //---- Tof scalers
  tofHit_n = 0;      //! tofHit: multiplicity/event

  //---- Tofino scalers

  //---- Shower scalers
  shoHit_n = 0;      //! showerHit: multiplicity/event
  shoHitTof_n = 0;   //! showerHit: multiplicity/event

  //---- Kick scalers
  kickTrack_n = 0;   //! kickTrack: tracks/event

}

void HQAScalers::activateScalers() {
// This function removes scalers from the scalerList TList which
// are not listed in QAscalers.h.

  TList *activeList = new TList();

  Char_t *scalers[1000] = {
    #include "QAscalers.h"
  };

  // Convert Char_t to TList of TObjStrings
  for(Int_t i=0; scalers[i]!=0;i++)
    activeList->Add( new TObjString(scalers[i]) );

  // Iterate over histList, remove entries which aren't in activeList
  TIter next(scalerList);
  while (TNamed *scal = (TNamed*) next()) {
    if( ! activeList->Contains( scal->GetName() ) )
      scalerList->Remove(scal);
  }

}

void HQAScalers::bookScalers() {
// Book scalers and add to TList of active scalers

  //---- Rich scalers
  richCal_n = new HQAScaler6("richCal_n","richCal: pads/event");
  scalerList->Add(richCal_n,"rich--sectorwise");

  richHit_n = new HQAScaler6("richHit_n","richHit: hits/event (*1000)");
  scalerList->Add(richHit_n,"rich--sectorwise");

  //---- Mdc scalers
  mdcCal1_n_m0 = new HQAScaler6("mdcCal1_n_m0","mdcCal1: Plane I, avg. mult.");
  scalerList->Add(mdcCal1_n_m0,"mdc--sectorwise");

  mdcCal1_n_m1 = new HQAScaler6("mdcCal1_n_m1","mdcCal1: Plane II, avg. mult.");
  scalerList->Add(mdcCal1_n_m1,"mdc--sectorwise");

  mdcCal1_n_m2 = new HQAScaler6("mdcCal1_n_m2","mdcCal1: Plane III, avg. mult.");
  scalerList->Add(mdcCal1_n_m2,"mdc--sectorwise");

  mdcCal1_n_m3 = new HQAScaler6("mdcCal1_n_m3","mdcCal1: Plane IV, avg. mult.");
  scalerList->Add(mdcCal1_n_m3,"mdc--sectorwise");

  //---- Tof scalers
  tofHit_n = new HQAScaler6("tofHit_n","tofHit: multiplicity/event");
  scalerList->Add(tofHit_n,"tof--sectorwise");

  //---- Shower scalers
  shoHit_n = new HQAScaler6("shoHit_n","showerHit: multiplicity/event");
  scalerList->Add(shoHit_n,"shower--sectorwise");

  shoHitTof_n = new HQAScaler6("shoHitTof_n","showerHitTof: multiplicity/event");
  scalerList->Add(shoHitTof_n,"shower--sectorwise");

  //---- Kick scalers
  kickTrack_n = new HQAScaler6("kickTrack_n","kickTrack: tracks/event");
  scalerList->Add(kickTrack_n,"kick--sectorwise");


  activateScalers();
}
