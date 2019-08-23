#ifndef HEVENTHEADER_H
#define HEVENTHEADER_H

#include <TObject.h>
#include <hgeomvector.h>

class HVertex : public TObject {
public:
 HVertex(void) { iterations=-1; }
 ~HVertex(void) {}
 HGeomVector &getPos(void) { return pos; }
 void setPos(HGeomVector &p) { pos = p; }
 void setIterations(Short_t i) { iterations = i; }
 void setChi2(Float_t c) { chi2 = c; }
 void setSumOfWeights(Float_t sw) { sumOfWeights = sw; }
 Short_t getIterations(void) { return iterations; }
 Float_t getChi2(void) { return chi2; }
 Float_t getSumOfWeights(void) { return sumOfWeights; }
 Double_t getX() {return pos.getX();}
 Double_t getY() {return pos.getY();}
 Double_t getZ() {return pos.getZ();}
protected:
 HGeomVector pos;    // Vertex position
 Short_t iterations; // Iterations until convergence. < 0, if no convergence
 Float_t chi2; 
 Float_t sumOfWeights; 
public:
 ClassDef(HVertex,2) // Event primary vertex
};

class HEventHeader : public TObject
{
private:
  UInt_t fEventSize;        // Event size in bytes
  UInt_t fEventDecoding;    // Decoding flag
  UInt_t fId;               // Event id (bits 0...3 of EvtId)
  UInt_t fEventSeqNumber;   // Event number
  UInt_t fDate;             // Date in DAQ format
  UInt_t fTime;             // Time in DAQ format
  UInt_t fEventRunNumber;   // Run number
  UInt_t fEventPad;         // 64 bit alignment
  Int_t fTBit;              // Trigger bits (8 bits of LVL1 trigger box)
  HVertex fVertex;          // Vertex information (x, y, z (in mm) in lab system)
  UInt_t downscaling;       // DAQ downscaling factor
  UInt_t downscalingFlag;   // DAQ downscaling flag (bit 4 of EvtId)
  UInt_t triggerDecision;   // LVL2 decision in lower 16 bit (bits 5...7 of EvtId) MU emulation decission in upper 16 bit
  Int_t timeInSpill;        // Time in spill
  UInt_t fVersion;          // Version info (bits 12...15 of EvtId)

public:
  HEventHeader(void) : fEventSize(0), fEventDecoding(0), fId(0),
  fEventSeqNumber(0), fDate(0), fTime(0), fEventRunNumber(0),
  fEventPad(0), fTBit(0), downscaling(0), downscalingFlag(0),
  triggerDecision(0), fVersion(0) {;}
  ~HEventHeader(void) {;}
  void setEventSize(UInt_t eventSize) {fEventSize=eventSize;}
  void setEventDecoding(UInt_t eventDecod) {fEventDecoding=eventDecod;}
  void setId(UInt_t id) {fId=id;}
  void setEventSeqNumber(UInt_t evSeqNr) {fEventSeqNumber=evSeqNr;}
  void setDate(UInt_t date) {fDate=date;}
  void setTime(UInt_t time) {fTime=time;}
  void setEventRunNumber(UInt_t evFileNr) {fEventRunNumber=evFileNr;}
  void setEventPad(UInt_t evPad) {fEventPad=evPad;}
  UInt_t getErrorBit(void) {return (fId >> 31) & 0x1;};
  UInt_t getEventSize(void) {return fEventSize;}
  UInt_t getEventDecoding(void) {return fEventDecoding;}
  UInt_t getId(void) {return fId & 0xf;}
  UInt_t getEventSeqNumber(void) {return fEventSeqNumber;}
  UInt_t getDate(void) {return fDate;}
  UInt_t getTime(void) {return fTime;}
  UInt_t getEventRunNumber(void) {return fEventRunNumber;}
  UInt_t getEventPad(void) {return fEventPad;}
  HVertex &getVertex(void) { return fVertex; }
  Double_t getVertexZ() {return fVertex.getZ();}
  void setTBit(Int_t bits) { fTBit=bits;}
  Int_t getTBit(void) {return fTBit;} 
  Bool_t IsTBit(Int_t i) {return ((fTBit>>i) & 1);}
  void setDownscaling(UInt_t d) {downscaling=d;}
  void setDownscalingFlag(UInt_t df) {downscalingFlag=df;}
  void setTriggerDecision(UInt_t td) {triggerDecision=(triggerDecision&0xFF00)|(td&0xFF);};
  void setMuEmuTriggerDecision(UInt_t td) {triggerDecision=(triggerDecision&0x00FF)|((td&0xFF)<<16);};
  UInt_t getDownscaling() {return downscaling;}
  UInt_t getDownscalingFlag() {return downscalingFlag;}
  UInt_t getTriggerDecision() {return triggerDecision&0xFF;};
  UInt_t getMuEmuTriggerDecision() {return (triggerDecision >> 16)&0xFF;};
  void setTimeInSpill(Int_t val) {timeInSpill=val;}
  Int_t getTimeInSpill() {return timeInSpill;}
  void setVersion(Int_t v) {fVersion=v;}
  UInt_t getVersion() {return fVersion;}
  inline Bool_t isOldDAQ() {return (fVersion==0);}   // old DAQ version (< Sep03)
  inline Bool_t isNewDAQ() {return (fVersion==1);}   // new DAQ version (>= Sep03)

  ClassDef(HEventHeader,6) // HADES event header class
};

#endif /* !HEVENTHEADER_H */


