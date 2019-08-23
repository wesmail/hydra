//*-- Author  : Diego Gonzalez-Diaz
//*-- Created : 20/12/2007

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////
//
//  HRpcCluster
//
//  Class for the RPC Cluster data
//
////////////////////////////////////////////////////


#include "hrpccluster.h"

ClassImp(HRpcCluster)

HRpcCluster::HRpcCluster(void) {
  tof     = charge  = -999;
  xmod    = ymod    = zmod    = -999;
  xsec    = ysec    = zsec    = -999;
  sigma_x = sigma_y = sigma_z = sigma_tof = -999;
  address = type    = -1;
  detID1  = detID2  = -1;
}

HRpcCluster::~HRpcCluster(void) {
}
