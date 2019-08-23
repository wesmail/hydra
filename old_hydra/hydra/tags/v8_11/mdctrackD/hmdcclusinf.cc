//*-- AUTHOR : Vladimir Pechenov
//*-- Modified : 04.04.2003 by Vladimir Pechenov
//*-- Modified : 09.07.2002 by Vladimir Pechenov

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HMdcClusInf
//
//  this class keep some information about cluster 
// * Short_t getClusIndex()
//      Returns the index of HMdcClus container
// * Short_t getClusFitIndex()
//      Returns the index of HMdcClusFit container
// * Char_t getFitStat()
//      Returns the fitStatus, 0 - hit filled by cluster, 1 - by fit
// * Short_t getClusSize()
//      Returns the number of bins in cluster (cluster size)
// * Short_t getNMergClus()
//      Returns the number of merged clusters
// * Short_t getNDrTimes()
//      Returns the number of drift times in cluster
// * void getShape(Float_t& sigma1,Float_t& sigma2,Float_t& alpha)
// * Float_t getAlpha()
// * Float_t getSigma1()
// * Float_t getSigma2()
//      Returns the cluster shape (all in sector coor. sys.):
//      sigma1 - long axis, (mm)
//      sigma2 - short axis (mm)
//      alpha - angle (deg.) between long axis of cluster and axis X
// * Float_t getRatio()
//      Returns sigma2/sigma1
// * Char_t getLevelClFinding()
//      Returns the level of cl.finding (minimal num. of layers)
// * UChar_t getNModInCl() {return uch3&0xF;}
//      Returns the num. of modules in cluster
// * UChar_t getNModInFit()
//      Returns the number of modules in fit
//
/////////////////////////////////////////////////////////////////////////

#include "hmdcclusinf.h"
#include <stdlib.h> 

ClassImp(HMdcClusInf)

void HMdcClusInf::print(void) const{
  printf("fit_status=%i clusSize=%i nMergedClus=%i nDrTimes=%i sigma1=%f sigma2=%f alpha=%f\n",
      fitStatus,clusSize,nMergClus,nDrTimes,sigma1,sigma2,alpha);
}
