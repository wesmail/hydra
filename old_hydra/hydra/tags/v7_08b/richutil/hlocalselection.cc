//
#include "hlocalselection.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmatrixcatiter.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hrichcal.h"
#include "hrichlocal.h"
#include "hdebug.h"
#include "hades.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hrichlocalmaxheader.h"

ClassImp(HLocalSelection)

HLocalSelection::HLocalSelection(Text_t *name, Text_t *title,Float_t lowTheta[6],Float_t highTheta[6],Float_t *minEvt,Float_t *maxEvt,Int_t intSize) : 
HReconstructor(name,title) {

    evtIntSize = intSize;
    minEvent = new Float_t[intSize];
    maxEvent = new Float_t[intSize];
    for(Int_t i = 0;i<intSize;i++){
	minEvent[i] = minEvt[i];
	maxEvent[i] = maxEvt[i];
    }
    for(Int_t i = 0;i<6;i++){
	countSec[i] =0;
	thetaLow[i] = lowTheta[i];
	thetaHigh[i] = highTheta[i];
    }
   
    evtCount = 0;
}
HLocalSelection::~HLocalSelection(void){
    delete [] minEvent;
    delete [] maxEvent;
}
Bool_t HLocalSelection::init() {

    printf("initialization of rich localmaxcal\n");
    //    HRichDetector *pRichDet = (HRichDetector*) gHades -> getSetup() ->
    //getDetector("Rich");
    return kTRUE;
}
Int_t HLocalSelection::execute() {
    evtCount++;
    //cout<<" evtCount "<< evtCount<<endl;
    for (Int_t i = 0;i<evtIntSize;i++){
	// 	cout<<" evt "<<evtCount<<"  maxEvent[i]  "<< maxEvent[i]<< "  minEvent[i] "<<minEvent[i]<<endl;
	if(evtCount<maxEvent[i]){
	    if(evtCount>minEvent[i]){
				//cout<<" evt ok "<<endl;
		break;
	    }
	    else return kSkipEvent;
	}
	else if(i==evtIntSize-1) return kSkipEvent;
    }
 /*   no selection of event with too many photons on the MgF2 low polar angle edge.
 
    for(Int_t i = 0;i<6;i++){
	countSec[i] =0;
    }
    HRichLocal *pLocal;
    fIter1 -> Reset();

    while ((pLocal = (HRichLocal*) fIter1 -> Next())) {
	checkPhoton(pLocal->getLocalTheta(),pLocal->getSector());
    }

    for(Int_t i = 0;i<6;i++){
	//if(countSec[i]>0) cout<<"countSec[i] at i: "<<i<<" "<<countSec[i]<<endl;
	if(countSec[i] >=3) return kSkipEvent;
    }
 */

    return 0;
    
}
void HLocalSelection::checkPhoton(Float_t theta,Int_t sec){
    
    if(theta>thetaLow[sec]&&theta<thetaHigh[sec]){
	//	cout<<" theta "<<theta<<" thetaLow "<<thetaLow[sec]<<"  thetaHigh "<< thetaHigh[sec]<<endl; 
	countSec[sec]++;
    }
}
