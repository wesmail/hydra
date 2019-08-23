// File: hrichclustersim.h
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/03 11:25:10
// based on hydra532 and root 2.25-03
//
#ifndef HRICHCLUSTERSIM_H
#define HRICHCLUSTERSIM_H

#include "hrichclustercal.h"

class HCategory;
class HIterator;
class HRichCalSim;
class HRichLocalSim;

class HRichClusterSim : public HRichClusterCal {
public:
    HRichClusterSim(Text_t *name, Text_t *title, Int_t clusterMin,
		    Int_t clusterMax, Int_t hitMin, Int_t hitMax):
	HRichClusterCal(name,title,clusterMin,clusterMax,hitMin,hitMax){};
    ~HRichClusterSim(){};
    
    Bool_t init();
        
    ClassDef(HRichClusterSim,1) //ROOT extension
	
};

#endif // HRICHCLUSTERSIM_H
