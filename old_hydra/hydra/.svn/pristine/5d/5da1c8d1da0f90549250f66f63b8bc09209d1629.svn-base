#ifndef HMDCSIMEVENTINF_H
#define HMDCSIMEVENTINF_H

#include "hcategory.h"
#include "iostream.h"
#include "hlocation.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hindextable.h"
#include "hmdccal1sim.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hgeantheader.h"

class HIterator;


class HMdcSimTrackInf : public TObject {
protected:
  Short_t nTr;                 // Track num.
  Short_t nSimHits;            // Numb. of digit. hits.
  Short_t nGeantHits;          // Numb. of geant hits
  HMdcCal1Sim* sim[500]; 
  Short_t nModules;
  Short_t moduleHits[100][4];
  Short_t time[500];         // =1 - first drift time; =2 - second drift time
  HGeantMdc* geant[500];  
  Short_t nPiace;     
  UChar_t piacesTrack[100][5];  // One piace - one module
                                // [][0] -sec.; [][1] -mod; [][2] -direction:
                                // 1,3,5,7 ... - from target (theta < 90)
                                // 2,4,6,8, ... - to target (theta > 90)
			        // Piace: geant[[][3]] - geant[[][4]]
 
  HGeantKine* kine;

  Short_t nSimHitsSector[6];         // Numb. of hits in one sector
  Short_t nSimHitsMod[6][4];         // Numb. of hits in one module
  Short_t nSimHitsLayer[6][4][6];    // Numb. of hits in one layer
  
  Short_t nTimesInMod[6][4];         // Num. times track cross a module
  Short_t nGeantHitsLayer[6][4][6];  // Numb. of hits in one layer
  Short_t quality[6][4];             // =1 - track have hits in 4 - layers
                                     // =2,3... - track cross mod 2 or more 
				     //       times and .......(?)
				     //  [have to chang at new v. cal1Sym]
				     // =0 - recovering this track's segment 
				     //      impossible
  
public: 
  HMdcSimTrackInf(Int_t track);
  ~HMdcSimTrackInf(){;}
  void setSimHit(HMdcCal1Sim* hit, Int_t nTime);
  void sort(void);
  void printTrack(void);
  void setGeantMdc(HGeantMdc* hit);
  void setGeantKine(HGeantKine* hit);
  Short_t getNPiaces(){return nPiace;}
  Int_t getNSec(void);
  Short_t getNGeantHits(){return nGeantHits;}
  Short_t getNSimHits(Int_t sec=-1, Int_t mod=-1,  Int_t layer=-1);
  Short_t getNLayers(Int_t sec,Int_t mod);
  Short_t getNTimes(Int_t sec, Int_t mod,  Int_t layer=-1); 
  Short_t getQuality(Int_t sec, Int_t mod) {return quality[sec][mod];}  
  HGeantKine* getKine(void) {return kine;}
  HGeantMdc* getGeantHit(Int_t sec,Int_t mod,Int_t layer, Int_t ntime);
     
  ClassDef(HMdcSimTrackInf,0)
}; 

class HMdcSimEventInf : public TObject {
private:
  
  HLocation loc;                        // location for new object
  HIterator* iterin;                    // Iterator over input category
  HCategory* fGeantMdcCat;              //! MDC HGeant input data
  HLinearCategory* fGeantKineCat;
//  HCategory* fSimulGeantCat;
  HCategory* fCalCat;                   // pointer to cal data category
  HIterator* iter;
  Int_t isSimEvent;
  Int_t nTracks;                       // Total num. tracks
  Int_t nTracksSeg[6][2];              // Num. tracks in segment (MDC1,2; MDC3,4)
  Int_t listTracks[500];               // List tracks
  Float_t xVertex,yVertex,zVertex;     // Vertex of event
  Int_t nTotHitsSec[6];                   // Num. hits in sector
  Float_t nEvent;
  
  Int_t eventId;
  
  TObjArray *array;
//  void printClust(Int_t nc, HMdcCluster& clst);

public:
  HMdcSimEventInf();
  ~HMdcSimEventInf(void);
  void setParContainers();
   HMdcSimTrackInf& operator[](Int_t i) {
      return *static_cast<HMdcSimTrackInf*>((*array)[i]);
  }
  void collectInf(void);
  Int_t getNTracks(){return nTracks;}
  Int_t getTrackNum(Int_t nl){return listTracks[nl];}
  Int_t getEventId(){return eventId;}
  Int_t getNTracksSeg(Int_t sec, Int_t seg) {return nTracksSeg[sec][seg];}
  Int_t getListTracksSeg(Int_t sec, Int_t seg, Int_t *list);
  void getEventVertex(Float_t &x, Float_t &y, Float_t &z)
                                {x=xVertex; y=yVertex; z=zVertex;}
  Int_t getNTotHitsSec(Int_t sec) {return nTotHitsSec[sec];}
  

  ClassDef(HMdcSimEventInf,0) //Digitizes MDC data.
};

#endif
