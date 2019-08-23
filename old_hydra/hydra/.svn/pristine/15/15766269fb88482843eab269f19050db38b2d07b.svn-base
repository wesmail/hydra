#ifndef HMETAMATCH_H
#define HMETAMATCH_H
#include "hreconstructor.h"
#include "TString.h"
#include "TObject.h"
class HMetaMatch:public TObject
{
private:
   Short_t trkCandInd;//index of trkCand
   Short_t metaHitInd;//index from META 
   Short_t nCandForMeta;//Number of META candidates,if<0 no META
   Short_t nextCandForMeta;//Index of next MEtamatch object -1 no next object;
   Short_t firstCandForMeta;//Index of first candidate
   Short_t splineInd; //Index of Spline fitting
   Short_t richInd;//index of rich
   Short_t ipuInd;//index of ipu
   Short_t nCandForRich;//Number of RICH candidates
   Short_t nextCandForRich;//the same meaning
   Short_t firstCandForRich;
   Short_t nCandForIpu;
   Short_t nextCandForIpu;
   Short_t firstCandForIpu;
   Short_t rtInd;//Index of rtmetatrack
   Short_t kickInd;//Index of kicktrack
   Char_t sector;//sector number
   Int_t system;//system 1 TOF, system 0 shower,system=-1 no META
public:
   HMetaMatch();
   ~HMetaMatch();
   void setTrkCandInd(Short_t TRKCANDIND){trkCandInd=TRKCANDIND;}
   void setMetaHitInd(Short_t METAHITIND){ metaHitInd=METAHITIND;}
   void setNCandForMeta(Short_t NCANDFORMETA){nCandForMeta=NCANDFORMETA;}
   void setNextCandForMeta(Short_t NEXTCANDFORMETA){nextCandForMeta=NEXTCANDFORMETA;}
   void setFirstCandForMeta(Short_t FIRSTCANDFORMETA){firstCandForMeta=FIRSTCANDFORMETA;}
   void setSplineInd(Short_t SPLINEIND){ splineInd=SPLINEIND;}
   void setRichInd(Short_t RICHIND) {richInd=RICHIND;}
   void setIpuInd(Short_t IPUIND){ipuInd=IPUIND;}
   void setNcandForRich(Short_t NCANDFORRICH){nCandForRich=NCANDFORRICH;}
   void setNcandForIpu(Short_t NCANDFORIPU){nCandForIpu=NCANDFORIPU;}
   void setNextCandForRich(Short_t NEXTCANDFORRICH){nextCandForRich=NEXTCANDFORRICH;}
   void setNextCandForInpu(Short_t NEXTCANDFORIPU){nextCandForIpu=NEXTCANDFORIPU;}
   void setFirstCandForRich(Short_t FIRSTCANDFORRICH){firstCandForRich=FIRSTCANDFORRICH;}
   void setFirstCandForIpu(Short_t FIRSTCANDFORIPU){firstCandForIpu=FIRSTCANDFORIPU;}
   void setRtInd(Short_t RTIND){rtInd=RTIND;}
   void setKickInd(Short_t KICKIND){kickInd=KICKIND;}
   void setSector(Char_t SECTOR) {sector=SECTOR;}
   void setSystem(Int_t SYSTEM) {system=SYSTEM;}
   ////   
   Short_t getTrkCandInd() {return trkCandInd;}
   Short_t getMetaHitInd() {return metaHitInd;}
   Short_t getNCandForMeta() {return nCandForMeta;}
   Short_t getFirstCandForMeta(){return firstCandForMeta;}
   Short_t getNextCandForMeta() {return nextCandForMeta;}
   Short_t getSplineInd() {return splineInd;}
   Short_t getRichInd() {return richInd;}
   Short_t getIpuInd() {return ipuInd;}
   Short_t getNcandForRich() {return nCandForRich;}
   Short_t getNcandForIpu() {return nCandForIpu;}
   Short_t gerNextCandForRich() {return nextCandForRich;}
   Short_t getNextCandForInpu() {return nextCandForIpu;}
   Short_t getFirstCandForRich() {return firstCandForRich;}
   Short_t getFirstCandForIpu() {return firstCandForIpu;} 
   Short_t getRtInd() {return rtInd;}
   Short_t getKickInd() {return kickInd;}
   Char_t getSector() {return sector;}
   Int_t getSystem() {return system;}
   ClassDef(HMetaMatch,1)
      };
#endif


