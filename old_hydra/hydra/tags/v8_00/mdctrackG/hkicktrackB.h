#ifndef HKICKTRACKB_H
#define HKICKTRACKB_H
#include "TObject.h"
#include "hbasetrack.h"

class HKickTrack;

class HKickTrackB : public HBaseTrack 
{
private:
   Char_t  quality;     // Track quality. q=0 all reasonable combinations, q=1 - good ones
   Float_t pTof;        // Track momentum using determined by Time Of Flight using inverse kinematics
   Float_t errPTof;     // Error in pTof
   Char_t  pid;         // Particle Id. from low resolution KickTrack
   Float_t pull;        // A variable used for matching
   Int_t   outherHitId; // Id of the hit in META used to build the track
                        // HTofHit or HTofCluster: sec*1000 + mod*100 + cell 
                        // HShowerHitTof: hit index
   Float_t showerSum10; // Shower: sum of 9 charges in post / sum of 9 charges in pre
   Float_t showerSum20; // Shower: sum of 9 charges in post2 / sum of 9 charges in pre
   Float_t flag;        // (Undefined) for target Flag
   
   Short_t segmentId;   // Id of the MDC segment used to build the track
public:
   HKickTrackB(void);
   HKickTrackB(HKickTrack* kickTrk);
//   HKickTrackB(HKickTrackB &aTrack);   
  ~HKickTrackB(void);
   
   //Please notice these functions are from original HKickTrack, they does not exsist in common HBaseTrack structure
   void setSegment(  Int_t   i){ segmentId = i; }
   void setQuality(  Char_t  q){ quality   = q; }
   void setPTof(     Float_t p){ pTof      = p; }
   void setErrPTof(  Float_t e){ errPTof   = e; }
   void setPID(      Char_t  p){ pid       = p; }
   //void setMetaEloss(Float_t t){ metaEloss = t; }
   void setPull(     Float_t p){ pull      = p; }
   void setFlag(     Float_t f){ flag      = f; }
   void setOuterHitId( Int_t   i){ outherHitId = i; }
   void setShowerSum10(Float_t f){ showerSum10 = f; }
   void setShowerSum20(Float_t f){ showerSum20 = f; }
   
   Int_t   getSegment(  void){ return segmentId; }
   Char_t  getQuality(  void){ return quality;   }
   Float_t getPTof(	void){ return pTof;	  }
   Float_t getErrPTof(  void){ return errPTof;   }
   Char_t  getPID(      void){ return pid;	  }
   //Float_t getMetaEloss(void){ return metaEloss; }
   Float_t getPull(	void){ return pull;	  }
   Int_t   getOuterHitId( void){ return outherHitId; }
   Float_t getShowerSum10(void){ return showerSum10; }
   Float_t getShowerSum20(void){ return showerSum20; }
   Float_t getFlag(       void){ return flag;        }
   ClassDef (HKickTrackB,2); // Low resolution KickPlane data container within general tracking scheme
};
#endif


  
