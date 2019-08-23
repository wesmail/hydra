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
  multdiag=NULL;
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
    name="MultDiag_";
    name+=getSubEvtId();
    multdiag=new TH2F(name,name,384,0,384,30,0,30);
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
    if( multdiag){
      multdiag->Write();
      gStyle->SetPalette(1);
      TCanvas c1("c1","c1");
      multdiag->Draw("colz");
      c1.Pad()->SetLogz();
      c1.Print(multdiag->GetName()+TString(".ps"));
      delete multdiag;
      multdiag=NULL;
    }
  }

  return kTRUE;
}

Int_t HHodoTrbUnpacker::correctRefTimeChVar(void)
{
  ///////////////////////////////////////////
  //
  // Reference signal from channel 31 of TDC
  // ist used for correction of corresponding TDC chns 
  // If no ref Time is found there the other TDCs
  // are checked for chn31
  // Attention: Timing of the four TCD refHit Chns
  // can be different (+25ps per TDC claimed by Jurek)
  ///////////////////////////////////////////
  Int_t refCh; //= (Int_t)(chan / 32); // this will give one of the channels
  Int_t corrTime;
  Int_t nEvt = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

  // now do correction
  // scann all arrays which contain time data and do correction
  // Corr. Times are  stored in:
  // trbLeadingTime[refCh][0]  first hit !!!
  // where refCh = 31, 63, 95, 127;

  for(Int_t jj=0; jj<4; jj++){// For all TDC chips
    Int_t jo;// j offset
    Bool_t flag;
    // First check if there is ANY data on a TDC, if not, skip it
    flag=false;
    jo=jj*32;
    for(Int_t ll=0; ll<32; ll++){// For all TDC channels
      if( trbLeadingMult[jo+ll]>0){ flag=true; break;}
    }
    if(!flag) continue;

    // Yes there is data, now check reference
    refCh = jo+31;// gives ref Channel back

    corrTime = trbLeadingTime[refCh][0];

    // Think about it! no ref time means -> reject, other suggestions?
    if(corrTime <= -1000000 ){
		//#warning "This Info message should be active for the beamtime!!!!!"
      if(!quietMode){
        Info("correctRefTimeChVar","trying to use another ref channel","No Ref Time! EventNb: %d TRBid %d TDC %d",nEvt,subEvtId,jj);
      }
      if(trbLeadingTime[31][0] > -1000000){
		  corrTime = trbLeadingTime[31][0];
         if(!quietMode){
           Info("correctRefTimeChVar","another ref channel found, EventNb: %d TRBid %d TDC %d",nEvt,subEvtId,jj);
         }
      }
	  else if(trbLeadingTime[63][0] > -1000000){
		  corrTime = trbLeadingTime[63][0];
         if(!quietMode){
           Info("correctRefTimeChVar","another ref channel found, EventNb: %d TRBid %d TDC %d",nEvt,subEvtId,jj);
         }
	  }
	  else if(trbLeadingTime[95][0] > -1000000){
		  corrTime = trbLeadingTime[95][0];
         if(!quietMode){
           Info("correctRefTimeChVar","another ref channel found, EventNb: %d TRBid %d TDC %d",nEvt,subEvtId,jj);
         }
	  }
	  else if(trbLeadingTime[127][0] > -1000000){
		  corrTime = trbLeadingTime[127][0];
         if(!quietMode){
           Info("correctRefTimeChVar","another ref channel found, EventNb: %d TRBid %d TDC %d",nEvt,subEvtId,jj);
         }
	  }
	  else{ 
		  corrTime=-1000000;// very big negative ... this means we do not have ref. time
        if(!quietMode){
          Warning("correctRefTimeChVar","Ref Time Not found ! EventNb: %d TRBid %d TDC %d",nEvt,subEvtId,jj);
        }
//      continue; // this means we do not have ref. time
     }
   }

    for(Int_t ll=0; ll<32; ll++){// For all TDC channels
      Int_t ii;
      ii=jo+ll;
      for(Int_t kk=0; kk<10; kk++){
        trbLeadingTime[ii][kk]  = trbLeadingTime[ii][kk] - corrTime + 40000;// We want poitive times here; 20000 ~ 2us
        trbTrailingTime[ii][kk] = trbTrailingTime[ii][kk] - corrTime + 40000;// thats bigger than max time window of TDC
      }
    }
  }

  return 0;
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
        Int_t m;
        m=trbLeadingMult[i];
        if(m>10) m=10;// no more than 10 Times stored
        for(Int_t chmult=0; chmult<m; chmult++){
          wtf_trbLeadingTime[i][chmult]=trbLeadingTime[i][chmult];
          wtf_trbADC[i][chmult]=trbADC[i][chmult];
          wtf_flag[i][chmult]=false;
        }
      }
    }

    if(!fNoTimeRefCorr){
      Int_t use_TimeRefType;
      use_TimeRefType=trbDataVer;
      // Note: It seems that trbDataVer has NEVER been set correcly in any
      // Beamtime... most likely you want to use case 2(0) anyway
      // cout << "trbDataVer: " <<trbDataVer<<endl;
      if( forced_TimeRefType!=-1) use_TimeRefType=forced_TimeRefType;
      switch(use_TimeRefType){
         case 0:// old is identical to RPC style
         case 2:
            // Wall/Veto; RPC style -- improved
            // Us chn31 of own TCD or fallback any other TDC
            correctRefTimeChVar();
            break;
         case 1:
            // use only Chn31 of TDC
            correctRefTimeCh31();
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
            Int_t m;
            m=trbLeadingMult[i];
            if(m>10) m=10;// no more than 10 Times stored
            
            for(Int_t chmult=0; chmult<m; chmult++){
               if(lower>=upper || (trbLeadingTime[i][chmult]>=lower &&  trbLeadingTime[i][chmult]<=upper))
               {
                  pRaw->fill( trbLeadingTime[i][chmult], trbADC[i][chmult]);
                  wtf_flag[i][chmult]=true;
               }
            }
         }// if Channel belongs to Hodo det
      }// for(Int_t i=0; i<128; i++)

    // Diagnostics
    if(fControlHistograms && histdiagraw && histdiagdif && multdiag){
      for (Int_t i = 0; i < 128; i++) {
        multdiag->Fill(i,trbLeadingMult[i]);
        multdiag->Fill(i+128,trbTrailingMult[i]);
        multdiag->Fill(i+256,trbTrailingTotalMult[i]);
        
        Int_t m;
        m=trbLeadingMult[i];
        if(m>10) m=10;// no more than 10 Times stored
        
        for(Int_t chmult=0; chmult<m; chmult++){
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
