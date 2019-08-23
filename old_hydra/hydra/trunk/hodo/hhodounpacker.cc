//*-- AUTHOR : Bjoern Spruck
//*-- Created : 09/05/2005
//*-- Changed : 19.7.2005

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
//
//  HHodoUnpacker
//
//  Unpacker for the Pion Hodoscopes based on the
//  Start detector unpacker code
//  This one is only depacking data from the
//  *** CAEN V1190 *** TDC Board
//  if you need to unpack normal tdc data to a hodo
//  cathegory, you *could* use start unpacker any
//  copy data with hstart2hodo (unly useful for old
//  pion beam test data)
//  This code is NOT used anymore in HADES
//  (=was only used for test beam and cosmic data)
//
/////////////////////////////////////////////////////////////

using namespace std;
#include "hhodounpacker.h"
#include "hododef.h"
#include "hhodolookup.h"
#include "hhodoraw.h"
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

// only for definition of some Board constants
// #include <hstartunpacker.h>
#include <htboxunpacker.h>

#ifndef DEBUG_LEVEL
#define 	DEBUG_LEVEL	2
#endif

ClassImp(HHodoUnpacker)

HHodoUnpacker::HHodoUnpacker(UInt_t id) {
  // constructor
  // sets the subevent id
  subEvtId=id;
  pRawCat=NULL;
  lookup=0;
}

Bool_t HHodoUnpacker::init(void) {
  // creates the raw category and gets the pointer to the lookup table
  pRawCat=gHades->getCurrentEvent()->getCategory(catHodoRaw);
  if (!pRawCat) {
    pRawCat=gHades->getSetup()->getDetector("Hodo")->buildCategory(catHodoRaw);
    if (!pRawCat){
			Error("HodoUnpacker::Init:","no Hodo pRawCat!\n");
			return kFALSE;
		}
    gHades->getCurrentEvent()->addCategory(catHodoRaw,pRawCat,"Hodo");
  }
  loc.set(2,0,0);
  lookup=(HHodoLookup*)(gHades->getRuntimeDb()->getContainer("HodoLookup"));
  if(!lookup){
		Error("HodoUnpacker::Init:","no lookup pRawCat!\n");
		return kFALSE;
	}
  return kTRUE;
}

Bool_t HHodoUnpacker::UnpackTDCv1190(UInt_t *data,UInt_t uBlockSize)
{
	HHodoRaw* pRaw=0;
	Int_t nCrate;
 	UInt_t nLastGeo=0;
	Int_t nLastEvtNr=-1;
	UInt_t nSizeCounter=0;
 Bool_t header_pos_flag=false;
 Bool_t trailer_pos_flag=false;

	if( uBlockSize==0){
		// no place for a start header
		Error("V1190 unpack","TIP Header followed by 0 Bytes -> Empty Event -> Event Counter mismatch!!!\n");
		return(kFALSE);
	}

	// From TIP Header
	// nSlot = (data[0] >>27 ) & 0x1F;// "GEO"
	// !!! nSlot MUST be taken from GLOBAL TDC HEADER !!!
	nCrate = (data[0] >>16 ) & 0xF;

	// Do not use the GEO from TIP because it is only
	// used for modules which have NO GEO information

	HHodoLookupSlot* pTdc;
	pTdc=0;

	for(UInt_t ii=1; ii<=uBlockSize; ii++){
  	UInt_t dataword;
		dataword=data[ii];

		nSizeCounter++;

		switch(dataword>>27){
			case 8:{// Global Header
				UInt_t nSlot;
				Int_t nE;
				#if DEBUG_LEVEL>4
				printf("V1190 unpack: Found GLOBAL Header $%08X\n",dataword);
				#endif
				nSlot=dataword&0x1F;// Geo address
				nE=(dataword>>5)&0x3FFFFF;// Event Counter

				if(nLastEvtNr!=-1){
					if( nE!=nLastEvtNr){
						Error("V1190 unpack",
						"Global Header with different Event Counter!\n");
						printf("(old %d new %d)\n",nLastEvtNr,nE);
//					return EXIT_FAILURE;
					}
				}

				if(nLastGeo==nSlot){
					Error("V1190 unpack",
					"Second Global Header with SAME Geo adress!\n");
//					return EXIT_FAILURE;
				}
				nLastGeo=nSlot;
				nLastEvtNr=nE;

				pTdc=lookup->getSlot(nCrate-1,nSlot-1);
				if (!pTdc){// not pTdc
					printf("V1190 - Crate %d Slot %d - found no lookup Slot\n",nCrate,nSlot);
					return(kFALSE);
				}
				#if DEBUG_LEVEL>4
				printf("V1190 - Crate %d Slot %d\n",nCrate,nSlot);
				#endif

				if(ii==1) header_pos_flag=true;

				nSizeCounter=1;
				break;
			}
			case 1:{// TDC Header
				#if DEBUG_LEVEL>4
				printf("V1190 unpack: Found TDC Header $%08X\n",dataword);
				#endif
				break;
			}
			case 0:{// TDC DATA
				#if DEBUG_LEVEL>4
				printf("V1190 unpack: Found TDC Data $%08X\n",dataword);
				#endif
				Int_t nData, nChannel;
			 Bool_t trailing_flag;

				trailing_flag=(dataword>>26)&0x1;// Leading=0, Trailing=1, Pair=0???
				nChannel=(dataword>>19)&0x7f; // decode channel
				nData=dataword&0x7ffff;   // decode 19bit data
				#if DEBUG_LEVEL>4
				printf("(Chan,Data) %3d, %d\n",nChannel,nData);
				#endif
				if(pTdc){
					HHodoLookupChan* tc=pTdc->getChannel(nChannel);
					if (tc){
						Int_t nMod, nStrip;
						tc->getAddress(nMod,nStrip);
						#if DEBUG_LEVEL>4
						printf("(Mod,Strip) %d, %d\n",nMod,nStrip);
						#endif
						if (nMod>=0 && nStrip>=0){
							loc[0]=nMod;
							loc[1]=nStrip;

							pRaw=(HHodoRaw*)pRawCat->getObject(loc);
							if (!pRaw){
								// was die folgende Zeilen machen ist mir vollkommen unklar!!!
								pRaw=(HHodoRaw*)pRawCat->getSlot(loc);
								if (pRaw) {
									pRaw=new (pRaw) HHodoRaw;
									pRaw->setAddress(nMod,nStrip);
								}
								else {
		                                printf("(Chan,Data) %3d, %d\n",nChannel,nData);
                                                printf("(Mod,Strip) %d, %d\n",nMod,nStrip);
									Error("execute()",
									"Can't get slot\n");
					pSubEvt->dumpIt();
					lookup->printParam();
									return EXIT_FAILURE;
								}
								pRaw->setAddress(nMod,nStrip);
								#if DEBUG_LEVEL>4
								Info("V1190:",
								"Set Mod and Strip!\n");
								#endif
							}
							else {
								// this WILL happen if more than one Hit exists
								// if pRaw is already existing, no need to set Address & so on
								// only need to fill the data

								#if DEBUG_LEVEL>2
								Info("execute()",
								"Slot already exists!\n");
								#endif
							}
#if 1
							// this is for SINGLE LEADING/TRAILING EDGE measurements only!!!
							// No check if the order is correct!!!
							// i am depending on the TDC to deliver the right order
							if(trailing_flag){
								// printf("T");
								if(!pRaw->fill_trail(nData)){
									//printf("Trailing without Leading or Too many Hits\n");
									//printf(".");
								}
							}else{
								// printf("L");
								if(!pRaw->fill_lead(nData)){
									// printf("Too many Hits\n");
									//printf("!");
								}
							}
#endif
#if 0
							// this is for SINGLE LEADING EDGE measurements only!!!
							if(trailing_flag){
								printf("Trailing flag: this should never happen in SINGLE RISING EDGE mode!\n");
							}else{
								if(!pRaw->fill(nData, 0)){
									printf("Too many Hits\n");
								}
							}
#endif
#if 0
							// this is for PAIR measurements only!!!
							if(!trailing_flag){
								// 12 Bit Leading Time, 7 Bit Width
								if(!pRaw->fill(nData&0xFFF, nData>>12)){
									printf("Too many Hits\n");
								}
							}else{
								// this should never happen in PAIR mode
								printf("Trailing flag: this should never happen in PAIR mode!\n");
							}
#endif
						}
						else {
							#if DEBUG_LEVEL>4
							Error("execute()",
							"Wrong address! Mod: %i Strip; %i",
							nMod,nStrip);
							#endif
						}
					}else{// tc=0 -> no channel found
						printf("V1190 - Channel %d not found\n",nChannel);
					}
				}else{
					Error("V1190 unpack","TDC DATA without GLOBAL HEADER\n");
				}
				break;
			}
			case 3:{// TDC Trailer
				if((dataword>>24)&0x7){
					Error("V1190 unpack","Found Error in TDC Trailer!\n");
					printf("TDC Error $%X ($%08X)\n",(dataword>>24)&0x7,dataword);
				}else{
					#if DEBUG_LEVEL>4
					printf("V1190 unpack: Found TDC Trailer $%08X\n",dataword);
					#endif
				}
				pTdc=0;
				break;
			}
			case 4:{// TDC ERROR
				if(dataword&0x1000){
					Info("V1190 unpack","TDC Event Size Limit exceeded!\n");
					printf("(TDC Error $%08X)\n",dataword);
				}else{
					Error("V1190 unpack","Found TDC Error!\n");
					printf("TDC %d Error $%04X ($%08X)\n",(dataword>>24)&0x3,dataword&0x7FFF,dataword);
				}
				break;
			}
			case 17:{//Trigger Time Tag
				#if DEBUG_LEVEL>4
				printf("V1190 unpack: Found Trigger Time Tag $%08X\n",dataword);
				#endif
				break;
			}
			case 16:{// Trailer
				#if DEBUG_LEVEL>4
				printf("V1190 unpack: Found GLOBAL Trailer $%08X\n",dataword);
				#endif

				if(ii==uBlockSize){
					trailer_pos_flag=true;
				}

				if(!pTdc){
					Error("V1190 unpack","GLOBAL TRAILER without GLOBAL HEADER\n");
				}

				if(nLastGeo!=(dataword&0x1F)){
					Error("V1190 unpack",
					"Trailer has different Geo address!\n");
					printf("V1190 unpack: GLOBAL Trailer: $%08X\n",dataword);
//					return EXIT_FAILURE;
				}

				if(nSizeCounter!=((dataword>>5)&0xFFFF)){
					Error("V1190 unpack",
					"Trailer Word Count != Counted Words!\n");
					printf("V1190 unpack: GLOBAL Trailer WordCount %d Counted %d ($%08X)\n",((dataword>>5)&0xFFFF),nSizeCounter,dataword);
				}

				break;
			}
			case 24:{// Filler
				#if DEBUG_LEVEL>4
				printf("V1190 unpack: Found Filler $%08X\n",dataword);
				#endif
				break;
			}
			default:{// not defined!
				printf("V1190 unpack: Found undefined $%08X\n",dataword);
				break;
			}
		}
	}
	if(!header_pos_flag){
		printf("V1190 expected a start header @ start!\n");
	}
	if(!trailer_pos_flag){
		printf("V1190 Trailer is missing @ end\n");
	}

	return(kTRUE);
}

Int_t HHodoUnpacker::execute(void) {
	// decodes the data in the subevent and fills the raw data structure of the Hodoscopes
	Int_t nEvt        = 0;    //Evt Id
	UInt_t uBlockSize = 0;    //Number of data words in one block
	if (gHades->isCalibration()){
		//calibration event
		//nothing to do for hodoscopes
		return 1;
	}
	if(subEvtId==412){
		if(pSubEvt) {
			printf("Old subEvtId 412 not supported!\n");
		}
	}else if(subEvtId==416){ //New data structures since aug04
		if (pSubEvt) {
//			printf("SubEvtNr=416 && pSubEvt\n");
			UInt_t* data = pSubEvt->getData();
			UInt_t* end  = pSubEvt->getEnd();
			nEvt = gHades->getCurrentEvent()->getHeader()->getEventSeqNumber();
			//Loop over all data in this subevt

/*				for(UInt_t *dd=data; dd<end; dd++){
					printf("$%08X ",*dd);
				}
				printf("\n");
*/
			while ( ++data<end && *data!=0x0 ) {
				#if DEBUG_LEVEL>4
				printf("Data word (%p %p): %p\n",data,end,*data);
				#endif

				// Even if I only want to decode CAEN 1190 TDC Events, I have
				// to know all types, because I HAVE TO KNOW the number of
				// words to skip!!!
				// 1190 Events are always within an "Ingo Header"

				//Scan for TIP-block header Scalers have always TIP-Block
				if( ((*data >>24) & 0x6)==0 && (((*data) &  0x3fffff )!=0)){
					uBlockSize = (*data) & 0x0000003ff;
					UInt_t uBoardType = (*data >> 10 ) & 0x3f;
					// UInt_t uCode      = (*data >>20 )  & 0xf;
					// UInt_t uCtrl      = (*data >>20 )  & 0x7;
					#if DEBUG_LEVEL>4
					printf("BoardType: %d\n ",uBoardType);
					#endif
					//new TDC v1190 Type
					if( uBoardType==0x1/*TDC_1190*/ ){// In welchem Headerfile ist das definiert und wie heisst es?

						UnpackTDCv1190(data, uBlockSize);

						data+=uBlockSize;
						continue;//break; //return to main loop
					}else if( uBoardType==SCALER_AUG04 ){
						// do nothing
						data+=uBlockSize;
						continue;//break; //return to main loop
					}
					//next Should be tdc header
					else if(uBoardType==0){
						//Do a normal TDC/latch scanning
						//exit from this statement
						data+=uBlockSize;
						continue;//break; //return to main loop
					}
					else{
						Error("execute","Uknown BoardType=%i",uBoardType);
						data+=uBlockSize;
						continue;//break; //return to main loop
					}
				}//end of TIP-block search
				///Now Scan for TDC header
				else if ( ((*data>>24) & 0x6)==2 ){
					//Found valid TDC header
					//dont give any error messages, just skip it
					//errors are given by TOF or START unpacker
					//no need here
					uBlockSize = ( (*data) >> 8   ) & 0xff;
					//Skip this block
					data+=uBlockSize+1;
				}
				//Check for SIS3829 header (defined by SIS) (latch only)
				else if(((*data) &  0x3fffff )==0){ //
					UInt_t uGeo = (*data)>>27 &0x1F;
					//Latch should have one data word, zero is also possible
					UInt_t *pLatchTmp=data;
					if(++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo){ //trailer has geo
						//no data word. size should be zero
						// do nothing but needed for pLatchTmp increase
					}
					else if (++pLatchTmp && ((*pLatchTmp)>>27 &0x1F)==uGeo){
						// do nothing but needed for pLatchTmp increase
					}
					else{
						//Error("execute()","Wrong latch data!");
						//printf("Hmm 0x%08x\n",*pLatchTmp);
					}//return address
					data=pLatchTmp;
				}
				else if ( ((*data>>24) & 0x6)==6 ){
					#if DEBUG_LEVEL>2
					Error("execute()",
					"Evt: %i, Invalid Data Word! Type: %d,Skipping 0x%08x",
					nEvt,((*data>>24) & 0x6),*data);
					pSubEvt->dumpIt();
					#endif
				}
				else if ( ((*data>>24) & 0x6)==0 ){
					Error("execute()",
					"Evt: %i, Data follow, but no header",nEvt);
				}
				else {
					Error("execute()",
					"Evt: %i, Unknow raw data type %i",
					nEvt,((*data>>24) & 0x6));
				}

			} //end of data
		}
	}else{
		if(pSubEvt) {
			printf("subEvtId %d not supported!\n",subEvtId);
		}
	}
	return(1);
}






