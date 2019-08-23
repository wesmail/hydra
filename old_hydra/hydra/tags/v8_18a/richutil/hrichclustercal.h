// File: hrichclustercal.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/03 19:52:05
// based on hydra531 and root 2.25-03
//
#ifndef HRICHCLUSTERCAL_H
#define HRICHCLUSTERCAL_H
using namespace std;
#include "hreconstructor.h"
#include "hlocation.h"
#include "hparset.h"
#include <fstream> 
#include "TVector3.h"
#include "TObjArray.h"

class HCategory;
class HIterator;
class HRichCal;
class HRichLocal;

class HRichClusterCal : public HReconstructor {
public:
    HRichClusterCal(Text_t *name, Text_t *title, Int_t clusterMin,
		    Int_t clusterMax, Int_t hitMin, Int_t hitMax);
    ~HRichClusterCal();
    
    Bool_t init();
    Bool_t finalize();
    Int_t execute();
    Bool_t resetmatrix();
    Bool_t fillmatrix(HLocation &fLoc);
    Bool_t cluster(HLocation &fLoc);
        
    HRichClusterCal &operator=(HRichClusterCal &c);
    
    HCategory* getCalCat(){return fCalCat;}
    HCategory* getLocalCat(){return fLocalCat;}
        
    void setCalCat(HCategory* pCalCat){fCalCat = pCalCat;}
    void setLocalCat(HCategory* pLocalCat){fLocalCat = pLocalCat;}
        
    ClassDef(HRichClusterCal,1) //ROOT extension
	
protected:
    TObjArray *cluster1;//!
    TObjArray *cluster2;//!
    Int_t nCluster1;
    Int_t nCluster2;
    TVector3 *vPad1[5000];//!
    TVector3 *vPad2[5000];//!
    Int_t minHit;
    Int_t maxHit;
    Int_t minCluster;
    Int_t maxCluster;
    Int_t clusterCount;
    Int_t eventNr;
    Int_t sumHit[6];
    Float_t mCharge[6][90][92];
    Int_t mCluster[6][90][92];
    Int_t mHit[6][90][92];
    Int_t iSector;
    Int_t iCol;
    Int_t iRow;
    Int_t jRow;
    Int_t jCol;
    Int_t i, j, k, n, m;
    Int_t control1;
    Int_t control2;
    
    HCategory *fCalCat; //! 
    HCategory *fLocalCat; //!
    HIterator *fIter; //!
};

#endif // HRICHCLUSTERCAL_H
