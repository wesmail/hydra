#ifndef HEVENTHEADER_H
#define HEVENTHEADER_H

#include <TObject.h>

class HEventHeader : public TObject
{
private:
  UInt_t fEventSize;
  UInt_t fEventDecoding;
  UInt_t fId; //Event id
  UInt_t fEventSeqNumber; //Event number
  UInt_t fDate; //date
  UInt_t fTime; //time
  UInt_t fEventFileNumber; //
  UInt_t fEventPad; //64 bit alignment
		    
public:
  HEventHeader(void) : fEventSize(0), fEventDecoding(0),
  fId(0), fEventSeqNumber(0), fDate(0), fTime(0),  fEventFileNumber(0), 
  fEventPad(0) {;}
  ~HEventHeader(void) {}
  void setEventSize(UInt_t eventSize) {fEventSize=eventSize;}
  void setEventDecoding(UInt_t eventDecod) {fEventDecoding=eventDecod;}
  void setId(UInt_t id) {fId=id;}
  void setEventSeqNumber(UInt_t evSeqNr) {fEventSeqNumber=evSeqNr;}
  void setDate(UInt_t date) {fDate=date;}
  void setTime(UInt_t time) {fTime=time;}
  void setEventFileNumber(UInt_t evFileNr) {fEventFileNumber=evFileNr;}
  void setEventPad(UInt_t evPad) {fEventPad=evPad;}
  UInt_t getEventSize(void) {return fEventSize;}
  UInt_t getEventDecoding(void) {return fEventDecoding;}
  UInt_t getId(void) {return fId;}
  UInt_t getEventSeqNumber(void) {return fEventSeqNumber;}
  UInt_t getDate(void) {return fDate;}
  UInt_t getTime(void) {return fTime;}
  UInt_t getEventFileNumber(void) {return fEventFileNumber;}
  UInt_t getEventPad(void) {return fEventPad;}
  ClassDef(HEventHeader,1) //Header info of each event
};

#endif /* !HEVENTHEADER_H */



