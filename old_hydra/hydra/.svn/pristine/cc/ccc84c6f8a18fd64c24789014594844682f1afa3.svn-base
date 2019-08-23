//*-- AUTHOR : Jerzy Pietraszko, Ilse Koenig
//*-- Created : 03/02/2006
//*-- Modified: Apr 2007 B. Spruck

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
//  HHodoTrbUnpacker
//
//  Class for unpacking TRB data and filling
//    the Hodo Raw category
//  Base subevent decoding is performed by decode function
//  see: htrbbaseunpacker.h (/base/datasource/ )
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhodotrbunpacker.h"
#include "hododef.h"
#include "htrblookup.h"
#include "hhodoraw.h"
#include "hhodorefhitpar.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hruntimedb.h"
#include "hcategory.h"
#include "hldsubevt.h"
#include "heventheader.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

#include "TCanvas.h"
#include "TStyle.h"

ClassImp (HHodoTrbUnpacker)

HHodoTrbUnpacker::HHodoTrbUnpacker (UInt_t id):HTrbBaseUnpacker (id)
{
  // constructor
  pRawCat = NULL;
  fNoTimeRefCorr=false;
  fControlHistograms=false;
  histdiagraw=NULL;
  histdiagdif=NULL;
  forced_TimeRefType=-1;
}

Bool_t HHodoTrbUnpacker::init (void)
{
  // creates the raw category and gets the pointer to the TRB lookup table
  pRawCat = gHades->getCurrentEvent ()->getCategory (catHodoRaw);
  if (!pRawCat) {
    pRawCat = gHades->getSetup ()->getDetector ("Hodo")->buildCategory (catHodoRaw);
    if (!pRawCat)
      return kFALSE;
    gHades->getCurrentEvent ()->addCategory (catHodoRaw, pRawCat, "Hodo");
  }
  loc.set (2,0,0);

  lookup = (HTrbLookup *) (gHades->getRuntimeDb ()->getContainer ("TrbLookup"));
  if (!lookup) return kFALSE;

  refhit=(HHodoRefHitPar*)gHades->getRuntimeDb()->getContainer("HodoRefHitPar");
  if (!refhit) return kFALSE;

  if(fControlHistograms){
    TString name="TRBDiagRaw_";
    name+=getSubEvtId();
    histdiagraw=new TH2F(name,name,256,0,256,2000,0,20000);
    name="TRBDiagDif_";
    name+=getSubEvtId();
    histdiagdif=new TH2F(name,name,256,0,256,1000,30000,40000);
  }

  return kTRUE;
}

Bool_t HHodoTrbUnpacker::finalize(void)
{
  if(fControlHistograms){
    if( histdiagraw){
      histdiagraw->Write();
      gStyle->SetPalette(1);
      TCanvas c1("c1","c1");
      histdiagraw->Draw("colz");
      c1.Pad()->SetLogz();
      c1.Print(histdiagraw->GetName()+TString(".ps"));
      delete histdiagraw;
      histdiagraw=NULL;
    }
    if( histdiagdif){
      histdiagdif->Write();
      gStyle->SetPalette(1);
      TCanvas c1("c1","c1");
      histdiagdif->Draw("colz");
      c1.Pad()->SetLogz();
      c1.Print(histdiagdif->GetName()+TString(".ps"));
      delete histdiagdif;
      histdiagdif=NULL;
    }
  }

  return kTRUE;
}

Int_t HHodoTrbUnpacker::execute (void)
{
   HHodoRaw *pRaw = 0;    // pointer to Raw category
   Int_t nEvt = 0;        // Evt Seq. Nb.


   if (gHades->isCalibration ()) {
      //calibration event
      //cout << " --calibration event nothing to do for hodo " << endl;
      return 1;
   }
   // decode subevent data
   if (pSubEvt) {        // pSubEvt - make sure that there is something for decoding
      nEvt = gHades->getCurrentEvent ()->getHeader ()->getEventSeqNumber ();

      // decode the subevent and fill arrays, see: htrbbaseunpacker.h
      if(!decode()) return(-1);

//		cout << "***** HODO: trbDataExtension *****"<<endl;
//		cout << trbDataVer <<": "<< trbExtensionSize <<endl;

    // Diagnostics
    Int_t wtf_trbLeadingTime[128][10];
    Int_t wtf_trbADC[128][10];
    Int_t wtf_flag[128][10];

    if(fControlHistograms){
      for (Int_t i = 0; i < 128; i++) {
        for(Int_t chmult=0; chmult<trbLeadingMult[i]; chmult++){
          wtf_trbLeadingTime[i][chmult]=trbLeadingTime[i][chmult];
          wtf_trbADC[i][chmult]=trbADC[i][chmult];
          wtf_flag[i][chmult]=false;
        }
      }
    }

    if(!fNoTimeRefCorr){
      Int_t use_TimeRefType;
      use_TimeRefType=trbDataVer;
      if( forced_TimeRefType!=-1) use_TimeRefType=forced_TimeRefType;
      switch(use_TimeRefType){
         case 0:// old is identical to RPC style
         case 2:// Wall/Veto; RPC style
            correctRefTimeCh31();
            break;
         case 3:
            // new scheme for high rates
            correctRefTimeCh127();
            break;
//         case 4:
            // planned for W.K. electronics; never used
//            correctRefTimeWKStyle();
//            break;
         default:
            // do nothing
            break;
        }
      }

      // get the part of the lookup table which belongs to the subevent ID
      // in order to check which TRBchannel belongs to the HodoDetector

      HTrbLookupBoard *board = lookup->getBoard (subEvtId);
      if (!board) {
         // Problem: unpacker for specific SubId was called but for this SubId
         // no channel in lookup table axist.
         //
         Warning ("execute", "SubEvent: %i unpacked but TRB Board in Lookup table does not exist", subEvtId);
         return 1;
      }

      //fill RAW category for Hodo detector
      for (Int_t i = 0; i < 128; i++) {
         if (trbLeadingMult[i] < 1 ) continue;        //Leading Time data for this channel exist
         // MAYBE: && trbTrailingMult[i] < 1
         // I would like to take also trailing data... f.i. debugging, if signal is inverted
         // But this I wont get from the trbunpacker? (just a guess)

         // fill hodo raw category
         HTrbLookupChan *chan = board->getChannel (i);
         if (chan && 'H' == (Char_t) (chan->getDetector ())) {    //Channel belongs to Hodo det

            loc[0] = (Int_t)chan->getModule();
            loc[1] = (Int_t)chan->getCell();

            pRaw = (HHodoRaw *) pRawCat->getObject (loc);

            if (!pRaw) {
               pRaw = (HHodoRaw *) pRawCat->getSlot (loc);
               if (pRaw) {
                  pRaw = new (pRaw) HHodoRaw;
                  pRaw->setAddress(loc[0], loc[1]);
               }
               else {
                  Error ("execute()", "Can't get slot");
                  cerr << "execute(): Can't get slot  Mod: "<<(Int_t)chan->getModule() << " Slot " <<(Int_t)chan->getCell() << endl;
                  return -1;
               }
            } else {
               Error ("execute()", "Slot already exists! This should not happen with TRB board data.");
               cerr << "execute(): Slot already exists! Mod: "<<(Int_t)chan->getModule() << " Slot " <<(Int_t)chan->getCell() << endl;
            }

            HHodoRefHitParMod &pRefHit=(*refhit)[loc[0]];
            Float_t lower, upper;

            lower=pRefHit.getTrigRefLow();// Misuse ... rename function and variable
            upper=pRefHit.getTrigRefHigh();// its more a raw-time-cut

            // fill time info for channel: mod, fiber
            // btw: HHodoRaw will care itself if there are more hits and
            // handle this cases correctly by itself --- B.S.
            // todo: convert invalid time from unpacker -> invalid time in hodo class ... all negative are invalid?
            // now make a cut on the interessting time window or you might get drowned by noise
            // if lower >= higher do not make any cut... needed for determining the right windows
            // note: this should be done by the tdc hardware, but it is not possible to keep the trigger reference
            // in the same window if you make it too small!
            for(Int_t chmult=0; chmult<trbLeadingMult[i]; chmult++){
               if(lower>=upper || (trbLeadingTime[i][chmult]>=lower &&  trbLeadingTime[i][chmult]<=upper))
               {
                  pRaw->fill( trbLeadingTime[i][chmult], trbADC[i][chmult]);
                  wtf_flag[i][chmult]=true;
               }
            }
         }// if Channel belongs to Hodo det
      }// for(Int_t i=0; i<128; i++)

    // Diagnostics
    if(fControlHistograms && histdiagraw && histdiagdif){
      for (Int_t i = 0; i < 128; i++) {
        for(Int_t chmult=0; chmult<trbLeadingMult[i]; chmult++){
          histdiagraw->Fill(i,wtf_trbLeadingTime[i][chmult]);
          histdiagdif->Fill(i,trbLeadingTime[i][chmult]);
          if(wtf_flag[i][chmult]){
            histdiagraw->Fill(i+128,wtf_trbLeadingTime[i][chmult]);
            histdiagdif->Fill(i+128,trbLeadingTime[i][chmult]);
          }
        }
      }
    }// control hist
   }//if(pSubEvt)

   return 1;
}

void HHodoTrbUnpacker::correctRefTimeWKStyle(void)
{
  ///////////////////////////////////////////
  //
  // Reference signal from channel XX is used for TDC correction
  // this is not the final version of hardware design
  // call this function only if channel using Wolfgangs LED+Meantimer boards
  //
  // This code has not been used up to now!!!
  // -> not really tested
  // function should be moved to TRB class if working
  ///////////////////////////////////////////

  Int_t corrTime = 0;
  Float_t mean;
  Int_t mean_cnt;

  // now do correction
  // scan all arrays which contain time data and do correction
  // this is done in two steps:
  // in the first step we scan all channels and select all USEFUL
  // trigger times (time intervall has to be checked; check for chance coincidences)

  mean= 0.0;
  mean_cnt=0;
  for(Int_t ii=0; ii<128; ii++){
    Int_t chn_mult, chn_time=0;

    // valid triggers have:
    // a) lead AND trailing
    // b) are between YYYY < time < ZZZZ
    // c) a "const" width of XXXX
    // d) have no other hit in that intervall (only ONE per channel!)

    chn_mult=0;
    for(Int_t kk=0; kk<10; kk++){
      Int_t l, t,a ;
      l=trbLeadingTime[ii][kk];
      t=trbTrailingTime[ii][kk];
      a=trbADC[ii][kk];
      // cond a)
      if(l>-1000000 && t>-1000000){
        // cond b)
        if(l>9000 && l<10000){// CHECK THIS with DATA!!!
          // cond c)
          if( a>5 && a<50){// CHECK THIS with DATA!!!
            chn_time=l;
            chn_mult++;
          }
        }
      }
    }

    // cond d)
    if( chn_mult==1){// valid, only one hit in time range
      mean+=chn_time;
      mean_cnt++;
    }
  }

  if( mean_cnt>0) mean/=mean_cnt; else mean = -1000000.0;

  // Step two ----- not done yet ------
  // take the mean, discard anything which is too far away from mean
  // take a new mean, this is the correction time

  // Step three:
  // now correct leading and trailing

  corrTime = (Int_t)mean;

  if(corrTime > -1000000 ){
    for(Int_t ii=0; ii<128; ii++){
      for(Int_t kk=0; kk<10; kk++){
        // negating time? This is not what we want!!!
        // check THIS ... time after start ->  time before trigger!
        if( trbLeadingTime[ii][kk]!=-1000000) trbLeadingTime[ii][kk]  = corrTime - trbLeadingTime[ii][kk] +20000;
        if( trbTrailingTime[ii][kk]!=-1000000) trbTrailingTime[ii][kk] = corrTime - trbTrailingTime[ii][kk] +20000;
      }
    }
  }
}
