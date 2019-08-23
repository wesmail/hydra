// File: hrichclusterraw.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/03 11:25:42
// based on hydra531 and root 2.25-03
//
#ifndef HRICHCLUSTERRAW_H
#define HRICHCLUSTERRAW_H

#include "hrichclustercal.h"

class HCategory;
class HIterator;
class HRichRaw;
class HRichLocal;

class HRichClusterRaw : public HRichClusterCal {
public:
    HRichClusterRaw(const Text_t *name,const Text_t *title, Int_t clusterMin,
		    Int_t clusterMax, Int_t hitMin, Int_t hitMax):
	HRichClusterCal(name,title,clusterMin,clusterMax,hitMin,hitMax){};
    ~HRichClusterRaw(){};
    
    Bool_t init();
        
    HCategory* getRawCat(){return fRawCat;}
        
    void setRawCat(HCategory* pRawCat){fRawCat = pRawCat;}
        
    ClassDef(HRichClusterRaw,1) //ROOT extension
	
protected:
    HCategory *fRawCat; //!
};

#endif // HRICHCLUSTERRAW_H
