#ifndef HMDCANROOT_H
#define HMDCANROOT_H

#include "hreconstructor.h"
//#include "hmdctimecut.h"

class HIterator;
class HCategory;
class HLocation;
class TH2F;
class TH1F;
class TProfile;
class HKickPlane;
class HMdcSimEventInf;
class TNtuple;

class HMdcAnRoot : public HReconstructor {
private:

  HCategory* fClusCat;                   // pointer to cal data category
  HIterator* iter;  
  HLocation loc;                        // location for new object 
  TH2F *plot[100];
  TProfile *prFile[100];
  TH1F* hst[100];
  Bool_t isGeant;
  HKickPlane *fKick;
  HMdcSimEventInf *inform;
  TNtuple *hitTuple;

public:

  TH2F* getPlot(Int_t n){return plot[n];}
  TProfile* getProfile(Int_t n){return prFile[n];}
  TH1F* getHist(Int_t n){return hst[n];}

  HMdcAnRoot();
  HMdcAnRoot(Text_t *name,Text_t *title);
  ~HMdcAnRoot(void);


  Bool_t init(void);
  Bool_t finalize(void);
  Int_t  execute(void);
  void drawProjPlots(Int_t sec);
  void drawHitsPlots(Int_t sec);
  ClassDef(HMdcAnRoot,0) //Digitizes MDC data.
};

#endif
