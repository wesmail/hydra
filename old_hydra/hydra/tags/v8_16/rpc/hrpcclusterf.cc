//*-- Author   : Diego Gonzalez-Diaz
//*-- Created  : 20/12/07

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////
//
//  HRpcClusterF: RPC detector cluster finder
//
//  Gets hits from RpcHit(Sim) and writes to RpcCluster(Sim)
//
///////////////////////////////////////////////////////////////////

using namespace std;
#include <TRandom.h>
#include <time.h>
#include "hrpcdetector.h"
#include "hrpcclusterf.h"
#include "rpcdef.h"
#include "hrpchit.h"
#include "hrpchitsim.h"
#include "hrpccluster.h"
#include "hrpcclustersim.h"
#include "hgeantrpc.h"
#include "hdebug.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hgeantkine.h"
#include "hcategory.h"
#include <iostream>
#include <iomanip>

void HRpcClusterF::initParContainer() 
{
}

HRpcClusterF::HRpcClusterF(void)
{
  pClusterCat=NULL;
  pHitCat=NULL;  
}

HRpcClusterF::HRpcClusterF(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
  pClusterCat=NULL;
  pHitCat=NULL;
}

HRpcClusterF::~HRpcClusterF(void)
{
}

Bool_t HRpcClusterF::init(void)
{
  initParContainer();

  HRpcDetector *rpc;
  rpc=(HRpcDetector *)gHades->getSetup()->getDetector("Rpc");

  maxcells=rpc->getMaxCells();

  if(!rpc){
    Error("HRpcClusterF::init()","No Rpc Detector");
    return kFALSE;
  }

  // Create category for hit
  pHitCat=gHades->getCurrentEvent()->getCategory(catRpcHit);
  if (!pHitCat) {
    Error("HRpcClusterF::init()","No RpcHit Category");
    return kFALSE;
  }

  // Decide whether we are running for simulation or real data
  HCategory* catKin=gHades->getCurrentEvent()->getCategory(catGeantKine);
  
  if(catKin) simulation=kTRUE;
  else       simulation=kFALSE;

  // Create category for cluster data
  pClusterCat=gHades->getCurrentEvent()->getCategory(catRpcCluster);
  
  if (!pClusterCat) {

    if (simulation) pClusterCat=rpc->buildMatrixCategory("HRpcClusterSim",0.5);  
    else            pClusterCat=rpc->buildMatrixCategory("HRpcCluster",0.5);  
    
    gHades->getCurrentEvent()->addCategory(catRpcCluster,pClusterCat,"Rpc");

   }
  
  
  // Search for hgeantrpc in case of simulation

  if(simulation) {
    pGeantRpcCat = gHades->getCurrentEvent()->getCategory(catRpcGeantRaw);  
    if (!pGeantRpcCat) {
      Error("HRpcClusterF::init()","No HGeant RPC Category");
      return kFALSE;
    }  
  }

  loc.set(3,0,0,0);
  loc_cluster.set(2,0,0);

  return kTRUE;
}

Int_t HRpcClusterF::execute(void)
{

  HRpcHitSim     *pHit        = NULL; //Only reading, use the same class for HRpcHitSim and HRpcHit objects
  HRpcClusterSim *pClusterSim = NULL; //Reading and STORING in category, use 2 different classes.
  HRpcCluster    *pCluster    = NULL; //Reading and STORING in category, use 2 different classes.

  Int_t index_cluster;                //Index of cluster
  Int_t cluster_type  = -1;
  Int_t sign          = 0;            //Function sign;
  Float_t tof, charge, xmod, ymod, zmod, xsec, ysec, zsec;
  
  Float_t tof_neigb, charge_neigb, xmod_neigb, ymod_neigb, xsec_neigb, ysec_neigb, zsec_neigb;
  Float_t tof_neigb2, xmod_neigb2;
  Float_t sigma_X, sigma_Y, sigma_Z, sigma_T;
  Float_t tof_cut, x_cut, D_layers, tof_next, D_lever=2350; //[ns], [mm], [ns], [mm], [ns], [mm]

  Int_t Track1L=-999, Track1R=-999, Track2L=-999, Track2R=-999, Track=-999;
  Int_t Ref1L=-999, Ref1R=-999, Ref2L=-999, Ref2R=-999;
  Int_t Ref1LDgtr=-999, Ref1RDgtr=-999, Ref2LDgtr=-999, Ref2RDgtr=-999;

  Float_t NsigmasT, NsigmasX;
  Int_t cell1=-1, column1=-1, sector1=-1, cell2=-1, column2=-1, sector2=-1;
  //NOTE: Maximum cluster is assumed to include only 2 cells. More rare cases are likely to not be relevant.
  //NOTE: Even if in this very rare cases (i.e. 3 cells firing) the only problem of this assumption is 
  //that only the closest neighbouring cell will be considered for the cluster, the remaining cell will be 
  //counted as a different cluster --> so 1 track produces 2 clusters but even so, this is not conceptually 
  //a problem. Tracking and matching should take care of this.

  Int_t weight[4], array[4], ref_array[4];

  for (Int_t i=0;i<4;i++) weight[i]     = 0;
  for (Int_t i=0;i<4;i++) array[i]      = -999;
  for (Int_t i=0;i<4;i++) ref_array[i]  = -999;

  //FIXME: these should be parameters of the cluster finder container (to be done)
  
  NsigmasT = 3;
  NsigmasX = 3;
  D_layers  = 20;                            //20 mm, ~distance to next layer

  //Simple and robust sequencial algorithm for cluster finding enforcing no double-counting
  //If algorithm becomes slow, this must be done in a more clever way

  for(Int_t i=0;i<6;i++) {                 //6 sectors
    index_cluster =  0;           
    for(Int_t j=0;j<3;j++) {               //3 groups of 2 columns
      for(Int_t k=0;k<(maxcells*2-1);k++){ //2 columns with maxcells

	//Initialization of parameters
	for (Int_t t=0;t<4;t++) weight[t]     = 0;
	for (Int_t t=0;t<4;t++) array[t]      = -999;
	for (Int_t t=0;t<4;t++) ref_array[t]  = -999;

	cell1 = column1 = sector1 = -1;
	cell2 = column2 = sector2 = -1;
	//End of initialization of parameters

	loc[0] = i;	
	loc[1] = 2*j+1-k%2;                //Alternates the columns (down, up, down, up...)
	loc[2] = k/2;
	
	if(j==1) loc[1] = 2+k%2;
	//The cluster search for the inner columns must start from col==2 (k=0)
	//because BC1 is missing in the data stream. 
	//FIXME: this cell is meant for monitring. 
	//So, alternates the columns as up, down, up, down...

	pHit = (HRpcHitSim*)pHitCat->getObject(loc);

	if(!pHit) continue;	

	//Otherwise. A cluster will be created. Check if it can be allocated.

	loc_cluster[0]=i; //sector
	loc_cluster[1]=index_cluster;
	index_cluster++;
	   
	sigma_X= pHit->getXRMS();
	sigma_Y= pHit->getYRMS();
	sigma_Z= pHit->getZRMS();
	sigma_T= pHit->getTOFRMS();

	tof_cut = sigma_T*NsigmasT;  
	x_cut   = sigma_X*NsigmasX;   

	if (simulation) {
	  pClusterSim = (HRpcClusterSim*)pClusterCat->getSlot(loc_cluster);
	  if ( !pClusterSim ) {
	    Error ("execute()", "Can't get slot in cluster finder: sec %i, index %i",
		   loc_cluster[0],loc_cluster[1]);
	    return EXIT_FAILURE;
	  }
	  pClusterSim = new(pClusterSim) HRpcClusterSim;
	}	  
	else {
	  pCluster = (HRpcCluster*)pClusterCat->getSlot(loc_cluster);
	  if ( !pCluster ) {
	    Error ("execute()", "Can't get slot in cluster finder: sec %i, index %i",
		   loc_cluster[0],loc_cluster[1]);
	    return EXIT_FAILURE;
	  }
	  pCluster = new(pCluster) HRpcCluster;
	}

	//Read data members from hit
	  
	tof     = pHit->getTof();
	charge  = pHit->getCharge();
	xmod    = pHit->getXMod();
	ymod    = pHit->getYMod();
	zmod    = D_layers*(0.5-k%2);
	if(j==1) zmod   = D_layers*(k%2-0.5); //Inner column goes viceversa
	xsec    = pHit->getXSec();
	ysec    = pHit->getYSec();
	zsec    = pHit->getZSec();
	sector1 = pHit->getSector();
	column1 = pHit->getColumn();
	cell1   = pHit->getCell();


	cluster_type=1;
	   
	if (simulation){
	  Ref1LDgtr   = pHit->getRefLDgtr();
	  Ref1RDgtr   = pHit->getRefRDgtr();
	  Ref1L       = pHit->getRefL();
	  Ref1R       = pHit->getRefR();
					  
	  Track1L = ((HGeantRpc*)pGeantRpcCat->getObject(Ref1L))->getTrack();
	  Track1R = ((HGeantRpc*)pGeantRpcCat->getObject(Ref1R))->getTrack();
	}

	//Look for next cell in next column. True cluster finder starts here.

	loc[1] = 2*j + 1 - (k+1)%2;
	loc[2] = (k+1)/2;
	if(j==1) loc[1] = 2 + (k+1)%2;
	//The cluster search for the inner columns must start from col==3 (k=0).
	//BC1 is missing. So: Alternates the columns as down, up, down, up...

	pHit = (HRpcHitSim*)pHitCat->getObject(loc);


	if (pHit) {	//Neighbour	   

	  tof_neigb    = pHit->getTof();
	  charge_neigb = pHit->getCharge();
	  xmod_neigb   = pHit->getXMod();
	  ymod_neigb   = pHit->getYMod();
	  xsec_neigb   = pHit->getXSec();
	  ysec_neigb   = pHit->getYSec();
	  zsec_neigb   = pHit->getZSec();
	  sector2      = pHit->getSector();
	  column2      = pHit->getColumn();
	  cell2        = pHit->getCell();
	  
	  //Offset to the next station. Positive or negative depending
	  sign         = (2*loc[2]%2-1);
	  tof_next     = sign*D_layers*tof/D_lever;
      
	  //Cluster condition
	  if((fabs(xmod-xmod_neigb)<x_cut) && fabs(tof+tof_next-tof_neigb)<tof_cut){

	    tof    = (tof    + tof_neigb)/2;    
	    charge = (charge + charge_neigb)/2;
	    xmod   = (xmod   + xmod_neigb)/2;
	    ymod   = (ymod   + ymod_neigb)/2;
	    zmod   =  0;
	    xsec   = (xsec   + xsec_neigb)/2;
	    ysec   = (ysec   + ysec_neigb)/2;
	    zsec   = (zsec   + zsec_neigb)/2;

	    cluster_type=2;

	    if (simulation) {

	      Ref2LDgtr   = pHit->getRefLDgtr();
	      Ref2RDgtr   = pHit->getRefRDgtr();
	      Ref2L       = pHit->getRefL();
	      Ref2R       = pHit->getRefR();
	      Track2L     = ((HGeantRpc*)pGeantRpcCat->getObject(Ref2L))->getTrack();
	      Track2R     = ((HGeantRpc*)pGeantRpcCat->getObject(Ref2R))->getTrack();

	    }	    

	    //FIXME: Calculate also the angles in the sector reference system
	    
	    //At last, it must be checked that the cell can NOT form a cluster with the
	    //next neigbour, then the counter -k- should be increased. Otherwise,
	    //this will be double-counted in the next iteration as a new cluster 
	    //with cluster_type=1 [!!]

	    loc[1] = 2*j + 1 - (k+2)%2;
	    loc[2] = (k+2)/2;
	    if(j==1) loc[1] = 2 + (k+2)%2;
	    
	    pHit = (HRpcHitSim*)pHitCat->getObject(loc);

	    if (pHit) {
	      
	      tof_neigb2   = pHit->getTof();
	      xmod_neigb2  = pHit->getXMod();
	  
	      sign         = (2*loc[2]%2-1);
	      tof_next     = sign*D_layers*tof_neigb/D_lever;
	      
	      if((fabs(xmod-xmod_neigb)>=x_cut) || fabs(tof+tof_next-tof_neigb2)>=tof_cut) k=k+1;
	    }
	    else k=k+1;
	    //Note that only if a new cluster will be formed, the loop goes on normaly.
	    //If not, -k- is increased.
	  }
	}

	//Fill cluster
      	
	//Simulation
	if (simulation) {
	  
	  pClusterSim->setClusterType(cluster_type);
	  pClusterSim->setCluster(tof, charge, xmod, ymod, zmod);  
	  pClusterSim->setRSEC(xsec,ysec,zsec);
	  pClusterSim->setAddress(loc_cluster[0], loc_cluster[1]);

	  //Storing cell info of cluster size, trackID and Ref for simulation:
	  //Note: Important to realize that the trackID of the mother particle
	  //is stored, but ref corresponds to the reference of the DAUGHTER 
	  //that caused each hit (could be the same or not)

	  if(cluster_type==1) {
	    pClusterSim->setDetID1(sector1, column1, cell1);
	    array[0]=Track1L; array[1]=Track1R;
	    ref_array[0]=Ref1LDgtr; ref_array[1]=Ref1RDgtr;
	    
	    pClusterSim->setRMS(sigma_T, sigma_X, sigma_Y, sigma_Z); 
	  }
	  //Just by convenience the first cell upstream is stored as DetID1
	  else{
	    if(column1==0 || column1==2 || column1==4) {
	      pClusterSim->setDetID1(sector1, column1, cell1);
	      pClusterSim->setDetID2(sector2, column2, cell2);
	      array[0]=Track1L; array[1]=Track1R; array[2]=Track2L; array[3]=Track2R;
	      ref_array[0]=Ref1LDgtr; ref_array[1]=Ref1RDgtr; ref_array[2]=Ref2LDgtr; ref_array[3]=Ref2RDgtr;
	    }
	     
	    if(column1==1 || column1==3 || column1==5) {
	      pClusterSim->setDetID1(sector2, column2, cell2);
	      pClusterSim->setDetID2(sector1, column1, cell1);
	      array[0]=Track2L; array[1]=Track2R; array[2]=Track1L; array[3]=Track1R;
	      ref_array[0]=Ref2LDgtr; ref_array[1]=Ref2RDgtr; ref_array[2]=Ref1LDgtr; ref_array[3]=Ref1RDgtr;
	    }
	    pClusterSim->setRMS(sigma_T/sqrt(2.), sigma_X/sqrt(2.), sigma_Y/sqrt(2.), sigma_Z/sqrt(2.)); 
	  }
	  
	  //Algorithm for deciding the preferred track
	  for(Int_t m=0;m<4;m++){
	    for(Int_t n=0;n<4;n++){
	      if(array[m]==array[n]) 		  weight[m]++;
	    }
	  }
	  //Note that double-counting and self-counting does not affect the meaning of weight here :)
	  
	  Track=array[0];
	  
	  for(Int_t m=0;m<3;m++)   if (weight[m+1]>weight[m]) Track=array[m+1];
	  
	  pClusterSim->setTrack(Track);	
	  
	  //Fill the lists
	  for(Int_t m=0;m<4;m++) {
	    pClusterSim->setTrackList(m,array[m]);
	    pClusterSim->setRefList(m,ref_array[m]);
	  }	  
	  
	}
	
	//If real data
	else {
	  
	  pCluster->setClusterType(cluster_type);
	  pCluster->setCluster(tof, charge, xmod, ymod, zmod);  
	  pCluster->setRSEC(xsec,ysec,zsec);
	  pCluster->setAddress(loc_cluster[0], loc_cluster[1]);
	  
	  if(cluster_type==1) {
	    pCluster->setDetID1(sector1, column1, cell1);
	    pCluster->setRMS(sigma_T, sigma_X, sigma_Y, sigma_Z); 
	  }
	  else{
	    if(column1==0 || column1==2 || column1==4) {
	      pCluster->setDetID1(sector1, column1, cell1);
	      pCluster->setDetID2(sector2, column2, cell2);
	    }
	    
	    if(column1==1 || column1==3 || column1==5) {
	      pCluster->setDetID1(sector2, column2, cell2);
	      pCluster->setDetID2(sector1, column1, cell1);
	    }
	    pCluster->setRMS(sigma_T/sqrt(2.), sigma_X/sqrt(2.), sigma_Y/sqrt(2.), sigma_Z/sqrt(2.)); 
	  }
	}
      }
    }
  }

  return EXIT_SUCCESS;
}

ClassImp(HRpcClusterF)
