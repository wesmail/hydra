//
//
// Last change: 19.4.06  B. Spruck

using namespace std;
#include "htrbbaseunpacker.h"
#include "htrblookup.h"
#include "hdebug.h"
#include "hades.h"
#include "hevent.h"
#include "hldsubevt.h"
#include "heventheader.h"
#include <iostream>
#include <iomanip>
#include <stdlib.h>

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//                     HTrbBaseUnpacker
//            Base class for TRB data unpacking.
//  Any detector connected to TRB board should use this class to decode the subevent. 
//  Decode() method fills the arrays with information about: 
//  channel number (0-127), 
//  measured time, 
//  measured ADC.
//
//  The goal is to provide data which can be directly used by subsystems 
//  (tof, start, hodo, wall) in order to directly fill raw categories.
//
//  The most important information is stored in:	   
//  trbLeadingTime[ TrbChanNb ][ hitNb ],
//  trbADC[ TrbChanNb ][ hitNb ]. 
//  trbLeadingMult[ TrbChanNb ]
//
//  In case of Leading and Trailing mode the leading time and trailing time 
//  are delivered in separate data words. In this mode it can happen that 
//  a trailing time is missing. In such case trailing time is set to -1000000 
//  and ADC is set to -1. 
//
//  The information in Arrays is ordered in following way:
//  1. Leading Time has to exist -> fill trbLeadingTime[ TrbChanNb ][ hitNb ];
//  2. If Trailing time is found we try to marge it with last Leading Time.
//     If Trailing time does not exist trbLeadingTime[ TrbChanNb ][ hitNb ] = -1000000
//     If more than 1 Trailing time exist for one Leading Time the last one is used.
//  3. ADC is calculated only if Trailing time exist. 
//
//  Max. number of hits per channel is currently set to 10. 
//
//
//  In addition this class should perform basic data corrections and error handling.
//  (in preparation)
/////////////////////////////////////////////////////////////////////////////////


ClassImp(HTrbBaseUnpacker)

HTrbBaseUnpacker::HTrbBaseUnpacker(UInt_t id)  {
  // constructor
  debugFlag=0;
  debugFlag1=0;
  subEvtId=id;
  lookup=0;
  clearAll();
}

void HTrbBaseUnpacker::clearAll(void){
  for(Int_t i=0; i<128; i++){
    for(Int_t k=0; k<10; k++){
      trbLeadingTime[i][k]  = -1000000;
      trbTrailingTime[i][k] = -1000000;
      trbADC[i][k]=-1; 
    }    
    trbLeadingMult[i]=0;  
    trbTrailingMult[i]=0;  
  }
}

Int_t HTrbBaseUnpacker::correctRefTimeCh31(void){
  ///////////////////////////////////////////
  //
  // Reference signal from channel 31
  // ist used for TDC correction
  // this is not the final version of
  // hardware design
  // call this function only if channel 31 contains
  // reference time
  ///////////////////////////////////////////
  Int_t refCh; //= (Int_t)(chan / 32); // this will give one of the channels
  Int_t corrTime;
  // now do correction
  // scann all arrays which contain time data and do correction
  // Corr. Times are  stored in:
  // trbLeadingTime[refCh][0]  first hit !!!
  // where refCh = 31, 63, 95, 127;

  for(Int_t jj=0; jj<4; jj++){// For all TDC chips
    Int_t jo;// j offset
    bool flag;
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
      Info("correctRefTimeCh31","No Ref Time! ADC %d",jj);
      continue; // this means we do not have ref. time
//      corrTime=20000;
    }

    for(Int_t ll=0; ll<32; ll++){// For all TDC channels
      Int_t ii;
      ii=jo+ll;
      for(Int_t kk=0; kk<10; kk++){
        trbLeadingTime[ii][kk]  = trbLeadingTime[ii][kk] - corrTime + 20000;// We want poitive times here; 20000 ~ 2us
        trbTrailingTime[ii][kk] = trbTrailingTime[ii][kk] - corrTime + 20000;// thats bigger than max time window of TDC
      }
    }
  }

  return 0;
}

Int_t HTrbBaseUnpacker::correctOverflow(void){
  ///////////////////////////////////////////
  // TDC overflow has to be corrected
  ///////////////////////////////////////////
  // This is not necessary if the substract window bit is set in the TDCs
  return 0;
}


Bool_t HTrbBaseUnpacker::fill_lead(Int_t ch,Int_t time)
{
   ///////////////////////////////////////////
   // Stores the given time in the next data element
   // and sets the multiplicity.
   // Return false if 10 hits are already stored.
   ///////////////////////////////////////////

   if( trbLeadingMult[ch]<10){
      trbLeadingTime[ch][trbLeadingMult[ch]]=time;
   }
   trbLeadingMult[ch]++;

   return(trbLeadingMult[ch]<=10);
}

Bool_t HTrbBaseUnpacker::fill_trail(Int_t ch,Int_t time)
{
   ///////////////////////////////////////////
   // Calculates the time between trailing and LAST(!) leading hit.
   // No other check if its really the right one,
   // i am depending on the TDC to deliver the right order
   // Return kFALSE if no leading yet or more than 4 Hits
   ///////////////////////////////////////////

   Int_t m;// Leading Multiplicity
   m=trbLeadingMult[ch];
   if(m==0) return kFALSE;

   if( m<=10){
      if( trbTrailingMult[ch]!=m){
         trbTrailingTime[ch][m-1]=time;
         trbADC[ch][m-1] = time - trbLeadingTime[ch][m-1];
      }else{
         return kFALSE;// In this case we already have one trailing
      }
   }
   trbTrailingMult[ch]=m;

   return(trbTrailingMult[ch]<=10);
}

void HTrbBaseUnpacker::PrintTdcError(UInt_t e)
{
   Char_t *e_str[15]={
      "Hit lost in group 0 from read-out FIFO overflow",
      "Hit lost in group 0 from L1 buffer overflow",
      "Hit error have been detected in group 0",
      "Hit lost in group 1 from read-out FIFO overflow",
      "Hit lost in group 1 from L1 buffer overflow",
      "Hit error have been detected in group 1",
      "Hit lost in group 2 from read-out FIFO overflow",
      "Hit lost in group 2 from L1 buffer overflow",
      "Hit error have been detected in group 2",
      "Hit lost in group 3 from read-out FIFO overflow",
      "Hit lost in group 3 from L1 buffer overflow",
      "Hit error have been detected in group 3",
      "Hits rejected because of programmed event size limit",
      "Event lost (trigger FIFO overflow)",
      "Internal fatal chip error has been detected"
   };

   if(e==0) return;// No Error

   cout << "=== TRB/TDC Error analysis:" << endl;
   for(Int_t i=0; i<15; i++){
      if( e&0x1){
         cout << e_str[i] << endl;
      }
      e>>=1;
   }
   cout << "===" << endl;
}

Int_t HTrbBaseUnpacker::decode(void)
{
   clearAll();

   UInt_t nEvt        = 0;   //Evt SeqNumber
   Int_t nEvId       = 0;    //Evt Id

   Int_t TdcId;
   Int_t nCountTDC = 0;
//   Int_t nCountTDCHeader = 0;
//   Int_t nCountTDCTrailer = 0;

   UInt_t nEvtNr;

   UInt_t nSizeCounter = 0;
   UInt_t nTdcEvtId = 0;
//   Int_t nTdcBunchId = -1;
   UInt_t TdcDataLen=0;

   UInt_t uBlockSize=0;

   UInt_t* data = pSubEvt->getData();
   UInt_t* end  = pSubEvt->getEnd();
   nEvt = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

   //////////////////////////////////////////////////////////
   //   debugFlag1 is used only                            //
   // for special cases                                    //
   // if set to 1:                                         //
   // actual EVENT ID is overwritten                       //
   // by value 1, that means event will be treated as      //
   // normal data                                          //
   //////////////////////////////////////////////////////////
   if(debugFlag1 > 0){
      gHades->getCurrentEvent()->getHeader()->setId(1);
   }
   nEvId = gHades->getCurrentEvent()->getHeader()->getId();

   if(debugFlag > 0&&!quietMode){
      cout<<endl;
      cout<<"--TRB SubEvent: EvtNb= "<<nEvt<<" --- Evnt Id= "<<nEvId<<endl;
      cout<<"--debugFlag = "<<debugFlag<<endl;
      cout<<"--debugFlag1 = "<<debugFlag1<<endl;

      //Loop over all data in this subevt

      cout<<hex<<"data word: "<<*data<<dec<<endl;
      cout<<hex<<"data word: ---SUBEVENT HEADER: "<<*data<<dec<<endl;
   }

   // first word in subevent contains info about subevent size 0xbeef_6a_20 (magic,EvtId,length)
   // should be used for checking the subevent.

   #if DEBUG_LEVEL>4
   if(!quietMode)printf("=== Unpacker start SubEvtId: %d\n===Header: %X  Nr:%d  Len:%d\n",subEvtId,*data>>16,nEvtNr,uBlockSize);
   #endif

   if((*data&0xFFFF0000)==0xBE010000){
      if(!quietMode)Error("TRB unpack","TRB Buffer overflow, Data truncated. No lethal error, but please report if error is persistend!!!");
      return(kFALSE);
   }

   // BEEF has been replaced ny BE00 before the Beamtime, I only need it for
   // analysis of test files ... remove after Beamtime
   if((*data&0xFFFF0000)!=0xBEEF0000 && (*data&0xFFFF0000)!=0xBE000000){
      if(!quietMode)Error("TRB unpack","NO $BEEF FOUND!!!");
//      exit();
   }
   uBlockSize=*data&0xFF;
   nEvtNr=(*data>>8)&0xFF;

   if(nEvtNr!=(UInt_t)pSubEvt->getTrigNr()){
       if(!quietMode){
	   Error("TRB unpack","TRB EvtNr!=pSubEvt->getTrigNr() ********* Event Mixing *********");
	   printf("=== %d != %d \n",nEvtNr,(UInt_t)pSubEvt->getTrigNr());
       }
      return(kFALSE);
   }

//   if(uBlockSize<10){
#warning "Change value to 10 if all TRBs have 4 TDCs after beamtime may06"
   if(uBlockSize<8){
      if(!quietMode)printf("Error in TRB unpack: Suspicious length (too small for header/trailer of tdcs) %d , might be a overflow!\n",uBlockSize);
   }

   nSizeCounter++;// First one already processed

   while ( ++data<end && *data!=0x0 )
   {
      UInt_t dataword;
      dataword=*data;//[ii];

      nSizeCounter++;

      if(dataword==0xDEADFACE){
         #if DEBUG_LEVEL>4
         if(!quietMode)printf("TRB unpack: Found DEADFACE -> break %08X %08X\n",data,end);
         #endif
         break;
      }

      TdcId=(dataword>>24)&0xF;// might be wrong for TRB board
      TdcId=nCountTDC;


      if(TdcDataLen>0) TdcDataLen++;

      switch(dataword>>28){// Raw TDC Data
         case 0:{// Group Header
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("TRB unpack: Found GLOBAL Header $%08X\n",dataword);
            #endif

            if(!quietMode)Info("TRB unpack","Global Header not expected!");
            break;
         }
         case 1:{// Group Trailer
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("TRB unpack: Found GLOBAL Trailer $%08X\n",dataword);
            #endif

            if(!quietMode)Info("TRB unpack","Global Trailer not expected!");
            break;
         }
         case 2:{// TDC Header
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("TRB unpack: Found TDC %d Header $%04X $%04X\n",TdcId,(dataword>>12)&0xFFF,dataword&0xFFF);
            #endif

            if( nCountTDC>0 && nTdcEvtId!=((dataword>>12)&0xFFF)){
               if(!quietMode)Error("TRB unpack","TDCs have different EventIds ******* Event Mixing *******");
               if(!quietMode)printf("nTdcEvtId: %06X   dataword:  %06X  nEvtNr: %02X\n" , nTdcEvtId ,((dataword>>12)&0xFFF),nEvtNr);
//               exit();
//               return(kFALSE);
            }
            if( nEvtNr!=((dataword>>12)&0xFF)){
               if(!quietMode)Error("TRB unpack","TDC EventIds != Main EventId ******* Event Mixing *******");
               if(!quietMode)printf("nTdcEvtId: %06X   dataword:  %06X  nEvtNr: %02X\n" , nTdcEvtId ,((dataword>>12)&0xFFF),nEvtNr);
//               exit();
//               return(kFALSE);
            }
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("nTdcEvtId: %06X   dataword:  %06X  nEvtNr: %02X\n" , nTdcEvtId ,((dataword>>12)&0xFFF),nEvtNr);
            #endif
            nTdcEvtId=(dataword>>12)&0xFFF;

            TdcDataLen=1;
            break;
         }
         case 3:{// TDC Trailer
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("TRB unpack: Found TDC %d Trailer $%04X $%04X\n",TdcId,(dataword>>12)&0xFFF,dataword&0xFFF);
            #endif
            if(TdcDataLen!=(dataword&0xFFF)){
               if(!quietMode)Error("TRB unpack","TdcDataLen!= length in Trailer!");
               if(!quietMode)printf("TRB unpack: TdcDataLen %d != %d ",TdcDataLen,dataword&0xFFF);
            }
            TdcDataLen=0;
            if( nTdcEvtId!=((dataword>>12)&0xFFF)){
               if(!quietMode)Error("TRB unpack","TDC Header and Trailer have different EventIds");
//               exit();
//               return(kFALSE);
            }
            nCountTDC++;
            break;
         }
         case 4:{// TDC DATA Leading
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("TRB unpack: Found TDC %d Lead Data $%08X\n",TdcId,dataword);
            #endif
            Int_t nData, nChannel;
            nChannel=(dataword>>19)&0x1f; // decode channel
            nChannel+=TdcId*32;
            nData=dataword&0x7ffff;   // decode 19bit data
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("(Chan,Data) %3d, %d\n",nChannel,nData);
            #endif

            // this is for SINGLE LEADING/TRAILING EDGE measurements only!!!
            // No check if the order is correct!!!
            // i am depending on the TDC to deliver the right order
            if(!fill_lead(nChannel,nData)){
               //printf("Too many Hits\n");
            }
            break;
         }
         case 5:{// TDC DATA Trailing
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("TRB unpack: Found TDC %d Trail Data $%08X\n",TdcId,dataword);
            #endif
            Int_t nData, nChannel;
            nChannel=(dataword>>19)&0x1f; // decode channel
            nChannel+=TdcId*32;
            nData=dataword&0x7ffff;   // decode 19bit data
            #if DEBUG_LEVEL>4
            if(!quietMode)printf("(Chan,Data) %3d, %d\n",nChannel,nData);
            #endif
            // this is for SINGLE LEADING/TRAILING EDGE measurements only!!!
            // No check if the order is correct!!!
            // i am depending on the TDC to deliver the right order
            if(!fill_trail(nChannel,nData)){
               //printf("Trailing without Leading or Too many Hits\n");
            }
            break;
         }
         case 6:{// TDC ERROR
            if((dataword&0x7FFF)==0x1000){// special case for non fatal errors
               if(!quietMode)Info("TRB unpack","TDC Event Size Limit exceeded!\n");
               if(!quietMode)printf("(TDC %d Error Event Size Limit: $%08X)\n",TdcId,dataword);
            }else{
               if(!quietMode)Info("TRB unpack","Found TDC Error(s)!\n");
               if(!quietMode)printf("TDC %d Error $%04X ($%08X)\n",TdcId,dataword&0x7FFF,dataword);
               if(!quietMode)PrintTdcError(dataword&0x7FFF);
            }
            break;
         }
         case 7:{// Debug Info
            if(!quietMode)Error("TRB unpack","Found DEBUG Info");
            if(!quietMode)printf("TRB unpack: TDC %d: Found Debug Info $%08X",TdcId,dataword);
            break;
         }
         default:{// not defined!
            if(!quietMode)Error("TRB unpack","Found UNDEFINED data");
            if(!quietMode)printf("TRB unpack: TDC %d: Found undefined $%08X",TdcId,dataword);
            break;
         }
      }
   }

   if(nCountTDC!=4){
      if( nCountTDC<4){
#warning "Comment this in after may06 beamtime"
//         Error("TRB unpack","TDC count <4 -> TDC data missing!!!");
      }else{
         if(!quietMode)Error("TRB unpack","TDC count >4 -> additional TDC data!!!");
      }
   }

   if(uBlockSize!=nSizeCounter){
      if(!quietMode)Error("TRB unpack","Blocksize!=Counted words!!!");
   }
   #if DEBUG_LEVEL>4
   if(!quietMode)printf("==== Unpacker end (%d)\n",subEvtId);
   #endif

   return(kTRUE);
}

/*
// This is "old" code by Jurek
Int_t HTrbBaseUnpacker::decode(void)
{
   clearAll();


   Int_t nCell       =-1;
   Int_t nEvt        = 0;    //Evt Id
   Int_t nEvId       = 0;    //Evt Id

   Int_t nCountTDC = 0;
   Int_t nCountTDCHeader = 0;
   Int_t nCountTDCTrailer = 0;

   UInt_t* data = pSubEvt->getData();
   UInt_t* end  = pSubEvt->getEnd();
   nEvt = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();

   //////////////////////////////////////////////////////////
   //   debugFlag1 is used only                            //
   // for special cases                                    //
   // if set to 1:                                         //
   // actual EVENT ID is overwritten                       //
   // by value 1, that means event will be treated as      //
   // normal data                                          //
   //////////////////////////////////////////////////////////
   if(debugFlag1 > 0){
         gHades->getCurrentEvent()->getHeader()->setId(1);
   }
   nEvId = gHades->getCurrentEvent()->getHeader()->getId();

   if(debugFlag > 0){
      cout<<endl;
      cout<<"--TRB SubEvent: EvtNb= "<<nEvt<<" --- Evnt Id= "<<nEvId<<endl;
      cout<<"--debugFlag = "<<debugFlag<<endl;
      cout<<"--debugFlag1 = "<<debugFlag1<<endl;

      //Loop over all data in this subevt

      cout<<hex<<"data word: "<<*data<<dec<<endl;
      cout<<hex<<"data word: ---SUBEVENT HEADER: "<<*data<<dec<<endl;
   }
   // first word in subevent contains info about subevent size 0xbeef6a20
   // should be used for checking the subevent.
   // NOT DONE :)
   while ( ++data<end && *data!=0x0 ) {
      if(debugFlag > 0){
         cout<<hex<<"data word: "<<*data<<dec<<endl;
      }
      ///////////////////////////////////////////////////////////
      //                  BUG                                  //
      //        Due to known bug (TDC id is always 0 !!)       //
      //        we have to count TDCs                          //
      //     This can be done by searching for TDC Header      //
      //                    and TDC Trailer                    //
      ///////////////////////////////////////////////////////////
      //                    TDC Header:                        //
      ///////////////////////////////////////////////////////////
      // 0x20f6af26     = 0010 0000 1111 0110 1010 1111 0010 0110
      // type of word  -- 0010    <- This is a Header Pattern
      // TDC nb.       --      0000 <- This should give TDC number
      //                            (BUG, currently is not correct)
      // evt id        --           1111 0110 1010
      // buch Id       --                          1111 0010 0110
      ////////////////////////////////////////////////////////////

      /////////////////////////////////////////////////////////////
      //                    TDC Trailer:                         //
      /////////////////////////////////////////////////////////////
      // 0x30f6a019     = 0011 0000 1111 0110 1010 0000 0001 1001
      // type of word  -- 0011    <- This is a Trailer Pattern
      // number of TDC --      0000 <- This should give TDC number
      //                             (BUG, currently is not correct)
      // evt id        --           1111 0110 1010
      // word count    --                          0000 0001 1001
      /////////////////////////////////////////////////////////////
      //if((*data >>28) != 0x4) continue;
      if((*data >>28) == 0x6) {
         if(debugFlag > 0){
            cout<<hex<<"data word:---ERROR PATTERN: "<<*data<<dec<<endl;
         }
         // What to do in this case?
         // At least print an error message, this might be a serious problem!
         Error("decode","data word:---ERROR PATTERN: %08X %d",*data,dec);
         continue; // TDC Error pattern
      }
      if((*data >>28) == 0x2) nCountTDCHeader++;
      if((*data >>28) == 0x3) nCountTDCTrailer++;
      if(nCountTDCHeader == (nCountTDCTrailer+1)) nCountTDC = nCountTDCHeader-1;

      if((*data >>28) == 0x2){
         if(debugFlag > 0){
            cout<<hex<<"data word:---TDC HEADER: "<<*data<<dec<<endl;
         }
         continue; // This is a Header
      }

      if((*data >>28) == 0x3){
         if(debugFlag > 0){
            cout<<hex<<"data word:---TDC TRAILER: "<<*data<<dec<<endl;
         }
         continue; // This is a Trailer
      }
      if((*data ) == 0xdeadface){
         if(debugFlag > 0){
            cout<<hex<<"data word:---SUBEVENT TRAILER: "<<*data<<dec<<endl;
         }
         continue; // This is a Trailer from a whole Subevent
      }

      ////////////////////////////////////////////////////////////
      //               TDC Data word                            //
      ////////////////////////////////////////////////////////////
      //    NO WAY TO DISTINGUISH:                            //
      //  Leading measurement - Single edge                     //
      //  Leading measurement - pairing mode                    //
      //                                //
      ////////////////////////////////////////////////////////////
      // here you have to get location:
      // 0x4075f8f8 ==  b 0100 0000 0111 0101 1111 1000 1111 1000
      // Leading measurement - Single edge
      // type of word  -- 0100
      // number of TDC --     0000
      // channel       --           0111 0
      // leading time  --                 101 1111 1000 1111 1000

      // Leading measurement - pairing mode
      // type of word  -- 0100
      // number of TDC --     0000
      // channel       --           0111 0
      // width         --                 101 1111
      // channel       --                          1000 1111 1000

      Int_t dataType = 0;
      //     what information contains the data word
      //     0100 (dec 4) leading time
      //     0100 (dec 4) pairing mode:
      //             l_time and t_time see above)
      //     0101 (dec 5) trailing time

      Int_t nTdcId =  (*data >>24) & 0xf;      // TDC id BUG in TRB board
      Int_t nTdcChannel = (*data >>19) & 0x1f; // TDC channel (0-32)
      Int_t fAdc=-1;                            // Adc information
      Int_t valTime= (*data) & 0x7ffff;        // Time information
      //nCell= (32 * nTdcId ) + nTdcChannel;   // activate this line when TDC id will be correct in Data
      nCell= (32 * nCountTDC ) + nTdcChannel;  // nCell uniqe channel number for TRB board channel

      if(((*data >>28) & 0xf) == 4){ // =0100 // Leading mode data
         dataType = 4;
         if(debugFlag > 0){
            cout<<hex<<"data word:---Leading mode: "<<*data<<dec<<
                         " chan: "<<nCell<<" time= "<<valTime<<endl;
         }

         trbLeadingTime[nCell][trbLeadingMult[nCell]] = valTime;
         trbLeadingMult[nCell]++;   // Leading mode mult
         //cout<<"===Leading mode data, MultL "<<trbLeadingMult[nCell]<<endl;
      }

      //
      // Filling Trailing data:
      // Trailing data is always marged with leading data.
      // For each Trail.Time we check the trbLeadingMult[nCell] and
      // the trbTrailingTime[ nCell ][ trbLeadingMult[nCell] ] is filled.
      // if for some reason Trailing data are not deliverd the
      // Trailing Time is set to -1000000.
      //
      // observed problem: 1 LeadingTime followed by more than 1 Trailing Times.
      // In this case the last Trailing time will be marged with LeadingTime.
      // BUT THIS IS NOT WHAT WE WANT!!!!! This has to be changed! --- Bjoern
#warning "Change width calculation!"
      // The Reason for Trailing without Leading are probably Spikes AFTER the real signal
      // -> Leading has to be paired with FIRST trailing
      //
      else if(((*data >>28) & 0xf) == 5){ // =0101 // Trailing mode data
         dataType = 5;
         if(debugFlag > 0){
            cout<<hex<<"data word:---Trailing mode: "<<*data<<dec<<
                        " chan: "<<nCell<<" time= "<<valTime<< endl;
         }
         if( trbLeadingMult[nCell] < 1){
            if(debugFlag > 0){
               cout<<"--ERROR--EvtNb= "<<nEvt<<"  Trailing time exist without Leading Time"<<endl;
            }
         }
         else{
            trbTrailingTime[ nCell ][  trbLeadingMult[nCell]-1 ] = valTime;
            trbADC[ nCell ][ trbLeadingMult[nCell]-1 ] = valTime -  trbLeadingTime[ nCell ][  trbLeadingMult[nCell]-1 ];
            trbTrailingMult[ nCell ]++; // Trailing mode mult
            //if(trbADC[ nCell ][ nLeadMult-1 ] < 10)
            //         cout<<"----------ADC= "<<trbADC[ nCell ][ nLeadMult-1 ]<<
            //         " EvtNb= "<<nEvt<<" cell= "<<nCell<<endl;
         }
         if(debugFlag > 0){
            cout<<"   LeadMult= "<<trbLeadingMult[nCell]-1<<endl;
            cout<<" --LeadT="<<  trbLeadingTime[ nCell ][ trbLeadingMult[nCell]-1 ]<<
               " TrailT="<<  trbTrailingTime[ nCell ][ trbLeadingMult[nCell]-1 ]<<
               " ADC="<< trbTrailingTime[ nCell ][ trbLeadingMult[nCell]-1 ] -
               trbLeadingTime[ nCell ][ trbLeadingMult[nCell]-1 ] <<endl;
         }
      }
      else {
         cout<<"==Evnt= "<<nEvt<<" Error: word type not known: "
                                    <<hex<<((*data >>28) & 0xf)<<dec<<endl;
         continue;
      }
#warning "Looking at ifs above, the following line will never be true!"
      if((((*data >>28) & 0xf) != 4) && (((*data >>28) & 0xf) != 5) ){
         cout<<"\n\n-----ERROR TDC mode not known\n\n"<<endl;
      }
   }// while ( ++data<end && *data!=0x0 )

   return 1;
}
*/
