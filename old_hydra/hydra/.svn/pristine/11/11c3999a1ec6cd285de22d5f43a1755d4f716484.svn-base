#ifndef HMDCCLUSTER_H
#define HMDCCLUSTER_H

#include "TObject.h"
#include "hmdclistgroupcells.h"

class HMdcClus;

class HMdcCluster : public TObject {
  friend class HMdcLookUpTbSec;
  friend class HMdc34ClFinderSec;
  protected:
    HMdcList12GroupCells lCells1;
    HMdcList12GroupCells lCells2;
    Char_t       segment;     // =1-lCells1 is used; =2-lCells2; 3 - lCells1&2
    Short_t      numCells1;   // number of cells in lCells1
    Short_t      numCells2;   // number of cells in lCells2
    Bool_t       status;      // =kFALSE - removed by merging
    Char_t       flag;        // for combined clusters, >0 - cluster saved
    HMdcCluster* clusMerg;    // clusMerg=0 if status=kTRUE else -merged cluster
    Short_t      nMergedClus; // Number of merged clusters
    Double_t     sumWt;       // sum weights of bins
    Int_t        nBins;       // num. of bins in cluster
    Double_t     meanX;       // var. for calculation of cluster shape
    Double_t     meanY;       // -/-
    Double_t     meanXX;      // -/-
    Double_t     meanYY;      // -/-
    Double_t     meanYX;      // -/-
    Double_t     meanXWt;     // position of cluster (calc. with weights)
    Double_t     meanYWt;     // -/-
    Double_t     meanXXWt;    // X dispersion
    Double_t     meanYYWt;    // Y dispersion
    Double_t     minWt;       // minimal value of Wt
    Float_t      x;           // cluster position
    Float_t      y;           //
    Float_t      errX;        // position errors
    Float_t      errY;        //
    Float_t      sigma1;      // cluster shape
    Float_t      sigma2;      //
    Float_t      alpha;       //
    // Variables for matching:
    HMdcCluster* clusMod1;    // mod.1 cluster address
    HMdcCluster* clusMod2;    // mod.2 cluster address
  public:
    HMdcCluster(void) {}
    ~HMdcCluster(void) {}
    void clear(Char_t seg);
    void clearBinStat(void);
    inline void addBin(Double_t x,Double_t y,Double_t wt);
    void calcXY(void);
    void addClus(HMdcCluster& clst2);
    void sumClus(HMdcCluster& clst1, HMdcCluster& clst2);
    void calcClusParam(Double_t xSt2, Double_t ySt2);
    void fillClus(HMdcClus* clus, Int_t nLst, Bool_t fillTrList);
    void print(void);
    void setModCluster(Int_t im, HMdcCluster* cl); 
    HMdcCluster* getModCluster(Int_t im) {return im==0 ? clusMod1:clusMod2;}
    HMdcCluster* getMod1Clus(void)            {return clusMod1;}
    HMdcCluster* getMod2Clus(void)            {return clusMod2;}
    void         setMod1Clus(HMdcCluster* cl) {clusMod1=cl;}
    void         setMod2Clus(HMdcCluster* cl) {clusMod2=cl;}
    HMdcCluster* getClusMerg(void)            {return clusMerg;}
    void         setClusMerg(HMdcCluster* cl) {clusMerg=cl;}
    void         incFlag(void)                {flag++;}
    HMdcList12GroupCells& getLCells1(void)    {return lCells1;}
    HMdcList12GroupCells& getLCells2(void)    {return lCells2;}
    Bool_t       getStatus(void)              {return status;}
    Char_t       getFlag(void)                {return flag;}
    void         setFlag(Char_t fl)           {flag=fl;}
    Float_t      getX(void)                   {return x;}
    Float_t      getY(void)                   {return y;}
    Bool_t       testForInclude(HMdcCluster& cl2);
    
  ClassDef(HMdcCluster,0)
};

inline void HMdcCluster::addBin(Double_t x,Double_t y,Double_t wt) {
  sumWt    += wt;
  meanX    += x;
  meanY    += y;
  meanXX   += x*x;
  meanYY   += y*y;
  meanYX   += y*x;
  meanXWt  += x*wt;
  meanYWt  += y*wt;
  meanXXWt += x*x*wt;
  meanYYWt += y*y*wt;
  if(wt<minWt) minWt = wt;
  nBins++;
}

class HMdcClustersArrs : public TObject {
  protected:
    static HMdcClustersArrs* pMdcClustersArrs;
    HMdcCluster* clusArr[3];
    Int_t        arrSize[3];
  public:
    static HMdcClustersArrs* getExObject(void)   {return pMdcClustersArrs;}
    static HMdcClustersArrs* getObject(void);
    static void deleteCont(void);
    HMdcClustersArrs(void);
    ~HMdcClustersArrs(void);
    Int_t        createAllArrays(Int_t size=500);
    HMdcCluster* createArray1(Int_t size=500)    {return createArray(0,size);}
    HMdcCluster* createArray2(Int_t size=500)    {return createArray(1,size);}
    HMdcCluster* createArray3(Int_t size=500)    {return createArray(2,size);}
    HMdcCluster* getArray1(void)                 {return clusArr[0];}
    HMdcCluster* getArray2(void)                 {return clusArr[1];}
    HMdcCluster* getArray3(void)                 {return clusArr[2];}
    Int_t        getArray1Size(void)             {return arrSize[0];}
    Int_t        getArray2Size(void)             {return arrSize[1];}
    Int_t        getArray3Size(void)             {return arrSize[2];}
    HMdcCluster* getArr1Cluster(Int_t ind)       {return getCluster(0,ind);}
    HMdcCluster* getArr2Cluster(Int_t ind)       {return getCluster(1,ind);}
    HMdcCluster* getArr3Cluster(Int_t ind)       {return getCluster(2,ind);}
    void         testCluster(Int_t indArr,Int_t indClus);
  protected:
    HMdcCluster* createArray(Int_t ind,Int_t size);
    HMdcCluster* getCluster(Int_t indarr,Int_t  indClus);
  ClassDef(HMdcClustersArrs,0)
};

#endif  /*!HMDCCLUSTER_H*/
