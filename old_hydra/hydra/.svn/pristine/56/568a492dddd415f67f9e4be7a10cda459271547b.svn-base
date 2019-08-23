// File: hrichclustercal.cc
//
// Author: Soenke Schroeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/07/03 19:51:46
// based on hydra531 and root 2.25-03
//
#include "hrichclustercal.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichcal.h"
#include "hrichlocal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"

ClassImp(HRichClusterCal)
    
HRichClusterCal::HRichClusterCal(const Text_t *name,const Text_t *title, Int_t clusterMin,
				 Int_t clusterMax, Int_t hitMin, Int_t hitMax):
HReconstructor(name,title) {
    minCluster = clusterMin;
    maxCluster = clusterMax;
    minHit = hitMin;
    maxHit = hitMax;
    fIter = NULL;
    eventNr = 0;
    clusterCount = 0;
    cluster1 = new TObjArray();
    nCluster1 = 0;
    cluster2 = new TObjArray();
    nCluster2 = 0;
    fIter = NULL;
    i = 0;
    j = 0;
    k = 0;
    n = 0;
    m = 0;
    control1 = 0;
    control2 = 0;
}
HRichClusterCal::~HRichClusterCal(void) {
    if (fIter) delete fIter;
    if (cluster1) cluster1 -> Delete();
    if (cluster2) cluster2 -> Delete();
    n = 0;
    while (vPad1[n]) {
	vPad1[n] -> Delete();
	n++;
    }
    n = 0;
    while (vPad2[n]) {
	vPad2[n] -> Delete();
	n++;
    }
}
Bool_t HRichClusterCal::init() {
    printf("initialization of rich cluster\n");
    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup()
	-> getDetector("Rich");
    fCalCat = gHades -> getCurrentEvent() -> getCategory(catRichCal);
    if (!fCalCat) {
	fCalCat = pRichDet -> buildCategory(catRichCal);
	if (!fCalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichCal,
							fCalCat, "Rich");
    }
    fLocalCat = gHades -> getCurrentEvent() -> getCategory(catRichLocal);
    if (!fLocalCat) {
	fLocalCat = pRichDet -> buildCategory(catRichLocal);
	if (!fLocalCat) return kFALSE;
	else gHades -> getCurrentEvent() -> addCategory(catRichLocal, 
							fLocalCat, "Rich");
    }
    fIter = (HMatrixCatIter*) getCalCat() -> MakeIterator();
    return kTRUE;
}
Bool_t HRichClusterCal::finalize() {
    cout<<endl;
    cout<<"Total number of clusters: "<<clusterCount<<endl;
    return kTRUE;
}
HRichClusterCal& HRichClusterCal::operator=(HRichClusterCal &c) {
    return c;
}
Int_t HRichClusterCal::execute() {
    resetmatrix();
    HRichCal *pCal;
    HLocation loc;
    fIter -> Reset();
    while ((pCal = (HRichCal*) fIter -> Next())) {
	loc = fIter -> getLocation();
	fillmatrix(loc);
    }
    m++;
    if (m == 10) {
	cout<<"EventNr: "<<eventNr<<endl<<endl;
	m = 0;
    }
    fIter -> Reset();
    while ((pCal = (HRichCal*) fIter -> Next())) {
	loc = fIter -> getLocation();
	cluster(loc);
    }
    n = 0;
    while (vPad1[n]) {
	vPad1[n] ->Clear();
	n++;
    }
    n = 0;
    while (vPad2[n]) {
	vPad2[n] -> Clear();
	n++;
    }
    return 0;
}
Bool_t HRichClusterCal::resetmatrix() {
    for (iSector = 0; iSector < 6; iSector++) {
	sumHit[i] = 0;
	for (iRow = 0; iRow < 90; iRow++) {
	    for (iCol = 0; iCol < 92; iCol++) {
		mHit[iSector][iRow][iCol] = 0;
		mCharge[iSector][iRow][iCol] = 0;
		mCluster[iSector][iRow][iCol] = 0;
	    }
	}
    }
    return kTRUE;
}
Bool_t HRichClusterCal::fillmatrix(HLocation& fLoc) {
    HRichCal *cal = NULL;
    cal = (HRichCal*)fCalCat -> getObject(fLoc);
    iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    eventNr = cal -> getEventNr();
    if ((cal) && (cal -> getCharge() != 0)) {
	mHit[iSector][iRow][iCol] = 1;
    	mCharge[iSector][iRow][iCol] = cal -> getCharge();
	sumHit[iSector] = sumHit[iSector] + mHit[iSector][iRow][iCol];
    }
    return kTRUE;
}
Bool_t HRichClusterCal::cluster(HLocation& fLoc) {
    HRichCal *cal = NULL;
    cal = (HRichCal*) fCalCat -> getObject(fLoc);
    HRichLocal *local = NULL;
    iCol = cal -> getCol();
    iRow = cal -> getRow();
    iSector = cal -> getSector();
    // control if pad is not used before and if number of hits in sector is 
    // like the specifications
    if ((sumHit[iSector] > minHit) && (sumHit[iSector] < maxHit) && 
	(mCluster[iSector][iRow][iCol] == 0)) {
	j = 0;
	k = 0;
	if (!vPad1[j]) vPad1[j] = new TVector3(iSector,iRow,iCol);
	else vPad1[j] -> SetXYZ(iSector,iRow,iCol);
	    // write pad to cluster1
	cluster1 -> AddLast(vPad1[j]);
	j++;
	mCluster[iSector][iRow][iCol] = 1;
	// write active nightbourpads to cluster2
	for (jRow = iRow-1; jRow < iRow+2; jRow++) {
	    for (jCol = iCol-1; jCol < iCol+2; jCol++) {
		control1 = 0;
		if ((jRow == iRow) && (jCol == iCol)) {
		    control1 = 1;
		}
		if ((mHit[iSector][jRow][jCol] != 0) &&
		    (control1 == 0)) {
		    if (!vPad2[k]) vPad2[k] = new TVector3(iSector,jRow,jCol);
		    else vPad2[k] -> SetXYZ(iSector,jRow,jCol);
		    cluster2 -> AddLast(vPad2[k]);
		    k++;
		}
	    }
	}
	nCluster1 = cluster1 -> GetEntries();
	nCluster2 = cluster2 -> GetEntries();
	//control if there are active nightbourpads
	// at the end of this loop al pads of the cluster are in cluster1
	// cluster2 is empty
	while (nCluster2 != 0) {
	    // take the first pad from cluster2
	    iSector = (Int_t) ((TVector3*) (cluster2 -> First())) -> X();
	    iRow = (Int_t) ((TVector3*) (cluster2 -> First())) -> Y();
	    iCol = (Int_t) ((TVector3*) (cluster2 -> First())) -> Z();
	    // find the active nightbourpads for the nightbourpads
	    for (jRow = iRow-1; jRow < iRow+2; jRow++) {
		for (jCol = iCol-1; jCol < iCol+2; jCol++) {
		    control2 = 0;
		    if ((jRow == iRow) && (jCol == iCol)) {
			control2 = 1;
		    }
		    if ((mHit[iSector][jRow][jCol] != 0) &&
			(control2 == 0)) {
			// control if this nightbourpad is alredy 
			// in cluster2
			if (nCluster2 > 1) {
			    i = 0;
			    nCluster2 = cluster2 -> GetEntries();
			    while ((control2 == 0) && (i < nCluster2)) {
				if ((((Int_t) ((TVector3*) (cluster2 -> At(i)))
				      -> X()) == iSector) && 
				    (((Int_t) ((TVector3*) (cluster2 -> At(i)))
				      -> Y()) == jRow) && 
				    (((Int_t) ((TVector3*) (cluster2 -> At(i)))
				      -> Z()) == jCol)) {
				    control2 = 1;
				    break;
				}
				i++;
			    }
			}
			i = 0;
			control1 = 0;
			nCluster1 = cluster1 -> GetEntries();
			// control if this nightbourpad is alredy 
			// in cluster1
			while ((control1 == 0) && (i < nCluster1)) {
			    if ((((Int_t) ((TVector3*) (cluster1 -> At(i))) 
				  -> X()) == iSector) && 
				(((Int_t) ((TVector3*) (cluster1 -> At(i))) 
				  -> Y()) == jRow) && 
				(((Int_t) ((TVector3*) (cluster1 -> At(i))) 
				  -> Z()) == jCol)) {
				control1 = 1;
				break;
			    }
			    i++;
			}
			// add the new found active nightbourpads 
			// to cluster2
			if ((control1 == 0) && (control2 == 0)) {
			    if (!vPad2[k]) vPad2[k] = new TVector3(iSector,
								   jRow,jCol);
			    else vPad2[k] -> SetXYZ(iSector,jRow,jCol);
			    cluster2 -> AddLast(vPad2[k]);
			    nCluster2 = cluster2 -> GetEntries();
			    k++;
			}
		    }
		}
	    }
	    // add the first pad from cluster2 to cluster1
	    if (!vPad1[j]) vPad1[j] = new TVector3(iSector,iRow,iCol);
	    else vPad1[j] -> SetXYZ(iSector,iRow,iCol);
	    cluster1 -> AddLast(vPad1[j]);
	    j++;
	    nCluster1 = cluster1 -> GetEntries();
	    mCluster[iSector][iRow][iCol] = 1;
	    // remove the first pad from cluster2
	    cluster2 -> RemoveAt(0);
	    cluster2 -> Compress();
	    nCluster2 = cluster2 -> GetEntries();
	} 
	// control if the found cluster is like the specifications
	if ((nCluster1 > minCluster) && (nCluster1 < maxCluster)) {
	    clusterCount++;
	    cout<<"Cluster with "<< nCluster1<<" pads in sector "<<iSector
		<<" found!"<<endl;
	    while (nCluster1 != 0) {
		// take the first pad from cluster1
		iSector = (Int_t) ((TVector3*) (cluster1 -> First())) -> X();
		iRow = (Int_t) ((TVector3*) (cluster1 -> First())) -> Y();
		iCol = (Int_t) ((TVector3*) (cluster1 -> First())) -> Z();
		HLocation loc1;
		loc1.set(3,0,0,0);
		loc1.setOffset(iCol);
		loc1.setIndex(1,iRow);
		loc1.setIndex(0,iSector);
		if ((loc1.getIndex(1) >= 0) && (loc1.getOffset() >= 0)) {
		    local = (HRichLocal*) fLocalCat -> getSlot(loc1);
		    if (local) {
			local = new (local) HRichLocal;
			local -> setRow(iRow);
			local -> setCol(iCol);
			local -> setSector(iSector);
			local -> setEventNr(eventNr);
			local -> setLocalCharge(mCharge[iSector][iRow][iCol]);
		    }
		}
		cluster1 -> RemoveAt(0); 
		cluster1 -> Compress();
		nCluster1 = cluster1 -> GetEntries();
	    }
	    // at the end of this loop cluster1 is empty and the found cluster 
	    // is in local container
	}
    }
    if (cluster1) cluster1 -> Clear();
    if (cluster2) cluster2 -> Clear();
    return kTRUE;
}
