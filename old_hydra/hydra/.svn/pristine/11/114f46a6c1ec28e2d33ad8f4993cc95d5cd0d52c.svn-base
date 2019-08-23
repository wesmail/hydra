#ifndef HTOFCLUSTER_H
#define HTOFCLUSTER_H

#include "htofhit.h"    	

class HTofCluster : public HTofHit {
private:
Int_t clustSize;  // number of hits in the cluster.

public:
  Int_t getClusterSize(void) {return clustSize;}
  void setClusterSize(Int_t clustsize) {clustSize = clustsize;}

  ClassDef(HTofCluster,1) //Class with data for a TOF cluster
};

#endif /* !HTOFCLUSTER_H */

