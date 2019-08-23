//*-- Author: Joern Wuestenfeld 04/26/2005
//*-- Modified:
//*-- Version: $Id: htriggersetup.h,v 1.3 2006-02-16 08:48:35 wuestenf Exp $

#ifndef HTRIGGERSETUP_H
#define HTRIGGERSETUP_H

#include "hparcond.h"
#include "TArrayC.h"

class HParIo;
class HParamList;

class HTriggerSetup : public HParCond
{
protected:
  TArrayI *richIpuMap;             // Map of RICH IPU address to sector
  Int_t    richRowOffset;          // Row offset of RICH pad rows
  Int_t    richRingFinderVersion;  // Version of RICH ringfinder code

  Int_t muDileptons;    // minimum nunber of dileptons for positiv trigger
  Int_t muLeptons;      // Minimum number of leptons for positiv trigger
  Int_t muMaxLeptons;   // Number of leptons which will lead to a positiv LVL2 trigger
  Int_t muRichHits;     // Minimum number of RICHhits for positiv trigger
  Int_t muRichTotalHits;// Number of rings inRICH leadin g to a positiv LVL2 trigger
  Int_t muMetaHits;     // Minimum number of META hits for positiv trigger
	Int_t muDataFormat;   // Version of MU dataformat

public:
  HTriggerSetup(const char* name ="TriggerSetup", const char* title ="Variouse trigger setup parameters", const char* context ="TriggerProduction");
  ~HTriggerSetup(void);

  Bool_t check(void);
  void clear(void);
  void printParam(void);
  void putParams(HParamList *l);
  Bool_t getParams(HParamList *l);
  Bool_t init(HParIo *pPar, Int_t *set);
  Int_t write(HParIo* output);

  TArrayI *getMap(void){return richIpuMap;};
  Int_t    getRowOffset(void){return richRowOffset;};
  Int_t   getRingFinderVersion(void){return richRingFinderVersion;};
  Int_t getMuDileptons(void){return muDileptons;};
  Int_t getMuLeptons(void){return muLeptons;};
  Int_t getMuMaxLeotons(void){return muMaxLeptons;};
  Int_t getMuRichHits(void){return muRichHits;};
  Int_t getMuMetaHits(void){return muMetaHits;};
  Int_t getMuRichTotalHits(void){return muRichTotalHits;};
	Int_t getMuDataFormat(void){return muDataFormat;};

  void setMap(Int_t *m){richIpuMap->Set(7,m);};
  void setRowOffset(Int_t o){richRowOffset = o;};
  void setRingFinderVersion(Int_t val){richRingFinderVersion = val;};
  void setMuDileptons(Int_t val){muDileptons = val;};
  void setMuLeptons(Int_t val){muLeptons = val;};
  void setMuMaxLeptons(Int_t val){muMaxLeptons = val;};
  void setMuRichHits(Int_t val){muRichHits = val;};
  void setMuMetaHits(Int_t val){muMetaHits = val;};
  void setMuRichTotalHits(Int_t val){muRichTotalHits = val;};
	void setMuDataFormat(Int_t val){muDataFormat = val;};

  ClassDef(HTriggerSetup,1) // Various Parameters for the trigger tasks
};


#endif
