#ifndef HEVENTHEADER_H
#define HEVENTHEADER_H

#include <TObject.h>

class HEventHeader : public TObject
{
private:
  UInt_t fEventSize;      // event size in bytes
  UInt_t fEventDecoding;  // decoding flag
  UInt_t fId;             // event id
  UInt_t fEventSeqNumber; // event number
  UInt_t fDate;           // date in DAQ format
  UInt_t fTime;           // time in DAQ format
  UInt_t fEventRunNumber; // run number
  UInt_t fEventPad;       // 64 bit alignment
  Int_t fTBit;            // trigger bits 		

public:
  HEventHeader(void) : fEventSize(0), fEventDecoding(0), fId(0),
  fEventSeqNumber(0), fDate(0), fTime(0), fEventRunNumber(0), 
  fEventPad(0), fTBit(0) {;}
  ~HEventHeader(void) {;}
  void setEventSize(UInt_t eventSize) {fEventSize=eventSize;}
  void setEventDecoding(UInt_t eventDecod) {fEventDecoding=eventDecod;}
  void setId(UInt_t id) {fId=id;}
  void setEventSeqNumber(UInt_t evSeqNr) {fEventSeqNumber=evSeqNr;}
  void setDate(UInt_t date) {fDate=date;}
  void setTime(UInt_t time) {fTime=time;}
  void setEventRunNumber(UInt_t evFileNr) {fEventRunNumber=evFileNr;}
  void setEventPad(UInt_t evPad) {fEventPad=evPad;}
  UInt_t getEventSize(void) {return fEventSize;}
  UInt_t getEventDecoding(void) {return fEventDecoding;}
  UInt_t getId(void) {return fId;}
  UInt_t getEventSeqNumber(void) {return fEventSeqNumber;}
  UInt_t getDate(void) {return fDate;}
  UInt_t getTime(void) {return fTime;}
  UInt_t getEventRunNumber(void) {return fEventRunNumber;}
  UInt_t getEventPad(void) {return fEventPad;}
  void setTBit(Int_t bits) { fTBit=bits;}
  Int_t getTBit(void) {return fTBit;} 
  Bool_t IsTBit(Int_t i) {return ((fTBit>>i) & 1);}
  Bool_t IsTofBit(void) {return ( fTBit & 1);}
  Bool_t IsStartBit(void) {return ((fTBit >> 2) & 1);}
  Bool_t IsTofinoBit(void) {return ((fTBit >> 4) & 1);}
//  HMUData* getMUData() { return &fMUData; }

  ClassDef(HEventHeader,2) // Header info of each event
};

#endif /* !HEVENTHEADER_H */













