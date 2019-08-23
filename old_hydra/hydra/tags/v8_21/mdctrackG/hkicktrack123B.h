#ifndef HKICKTRACK123B_H
#define HKICKTRACK123B_H
#include "TObject.h"
#include "hbasetrack.h"
class HKickTrack123B : public HBaseTrack 
{
private:
   //Char_t   quality;   // Track quality. q=0 all reasonable combinations, q=1 - good ones
   //Float_t  pTof;      // Track momentum using determined by Time Of Flight using inverse kinematics
   //Float_t  errPTof;   // Error in pTof
   //Float_t  pull;      // A variable used for matching
   Float_t  metaeloss;   // Energy loss in meta
   Int_t    outherHitId; // Id of the hit in META used to build the track
   Float_t  pMeta;       // Momentum identified by deflection MDC3-META as in segment after magnetic field
   
   
public:
   HKickTrack123B(void);
   //HKickTrack123B(HKickTrackB &aTrack);   
  ~HKickTrack123B(void);
   
   //Please notice these functions are from original HKickTrack, they does not exsist in common HBaseTrack structure
   //void setQuality(Char_t  q){ quality   = q; }
   //void setPTof(   Float_t p){ pTof      = p; }
   //void setErrPTof(Float_t e){ errPTof   = e; }
   //void setPull(   Float_t p){ pull      = p; }
   void setMetaEloss(Float_t t){ metaeloss = t; }
   void setOuterHitId( Int_t i){ outherHitId=i; }
   void setPMeta(Float_t pm){ pMeta = pm; }

   //Char_t  getQuality(void){ return quality;  }
   //Float_t getPTof(	void){ return pTof;     }
   //Float_t getErrPTof(void){ return errPTof;  }
   //Float_t getPull(	void){ return pull;     }
   Float_t getMetaEloss(void){ return metaeloss;}
   Int_t   getOuterHitId( void){ return outherHitId; }
   Float_t getPMeta(void){       return pMeta;       }   
   ClassDef (HKickTrack123B,2); // middle-or-high resolution KickTrack within HBaseTrack scheme
};
#endif


  
