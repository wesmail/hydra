#include "hmatchumonitor.h"
#include "hdebug.h"
#include "hades.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hevent.h"
#include "hmatrixcatiter.h"
#include "triggerinfodef.h"          

#include <iostream.h>       

//----------------------------------------------------------------------------
HMatchUMonitor::HMatchUMonitor(){

TChain chain("T");
chain.Add("mu.root");

HMatchURich mu;
T->SetBranchAddress("HMatchURich",&mu);
Int_t mu_hits;
T->SetBranchAddress("HMatchURich.fNDataObjs",&mu_hits);   

HRichHit ana;
T->SetBranchAddress("HRichHit",&ana);
Int_t ana_hits;
T->SetBranchAddress("HRichHit.fNDataObjs",&ana_hits);       

TClonesArray *mudata = new TClonesArray("HMatchURich",400);
T->SetBranchAddress("HMatchURich.fData",&mudata);

TClonesArray *anadata =new TClonesArray("HRichHit",400);
T->SetBranchAddress("HRichHit.fData",&anadata);   

}
//============================================================================

------------------------------------------------------------------------------
HMatchUMonitor::~HMatchUMonitor(void){
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HMatchUMonitor::execute(){

HMatchURich *muhit[400];
HRichHit *anahit[400];   

richcorr();
tofcorr();
showercorr();

}
//============================================================================

//----------------------------------------------------------------------------
void HMatchUMonitor::richcorr(){

Int_t myX, myY, mySect, myEvent; 
Int_t ipucoord[800][3];
Int_t anacoord[80][3];
Int_t diffX, diffY, diffSquared;
Int_t diffList[9000][3];
Int_t corrList[9000][5];
Int_t nonCorrelatedIpu, nonCorrelatedAna;
Int_t listLen, listPos, listPos2;
Int_t actAna, actIpu;
Int_t ipuSec, anaSec;
Int_t ipuMax, anaMax;    

for(Int_t k=0; k<ana_hits;k++) {      // get all analysis rings
  anahit[k]=(HRichHit*)anadata.At(k);
  myX = anahit[k].iRingX;
  myY = anahit[k].iRingY;
  mySect =  anahit[k].nSector;
  anacoord[k][0]=myX; anacoord[k][1]=myY; anacoord[k][2]=mySect;
  cout << "AnaRing at x=" << myX << " y=" << myY << " sect="
       << mySect <<endl;
}
  
for(Int_t k=0; k<mu_hits;k++) {      // get all IPU rings
  muhit[k]=(HMatchURich*)mudata.At(k);
  myX = muhit[k].getRow();
  myY = muhit[k].getColumn();
  mySect =  muhit[k].getSegmentId();
  ipucoord[k][0]=myX; ipucoord[k][1]=myY; ipucoord[k][2]=mySect;
  cout << "IPURing at x=" << myX << " y=" << myY << " sect="
       << mySect <<endl;
}    

//- ****** Correlate closest ipu/ana pairs *****

  // ipucoord[i][] contains rings from IPU
  // anacoord[i][] contains rings from offline analysis

listLen = 0;   // current number of array elements in difflist
listPos = 0;                          

//- ****** Create sorted list (diffList) of squared distances for all 
// ipu-ana-hit combinations, sorted by increasing squared distance

for (Int_t sec=0; sec<6; sec++){ // loop over sector
  // Reset difflist (all array elements set to zero)
  for ( Int_t k=0; k<1000;k++) { //100 arbitrary value,1600 would be consistent
    diffList[k][0] = 0;
  }
  for( actAna=0; actAna<ana_hits;actAna++) {    // loop over  softrings
    for( actIpu=0; actIpu<mu_hits;actIpu++) {   // loop over  IPUrings
      if ( (anacoord[actAna][2]==sec) && (ipucoord[actIpu][2]==sec)) {
	diffX = ipucoord[actIpu][0] - anacoord[actAna][0];
	diffY = ipucoord[actIpu][1] - anacoord[actAna][1];
	diffSquared = diffX*diffX + diffY*diffY;   // squared distance ana-ipu
	diffx->Fill(diffX);
	diffy->Fill(diffY);
	// cout << "Calc " << listPos << " " << diffSquared << " " << actAna 
	// << " " << actIpu << endl; 
	// Now determine in which position (listPos) the new value
	// must be placed in the SORTED list
	if ( listLen == 0 ) {
	  listPos = 0;  // special case: first entry goes to position zero in
	} else {
	  // loop over existing entries in difflist until new value
	  // (diffSquared in bigger)
	  while ( (diffSquared > diffList[listPos][0]) && ( listPos < listLen)
		  listPos++;
		  }
	  // now we found the position where the new array element
	  // has to be placed in difflist
	  // and we copy all following element to the next position
	  // in order to get a free array element for the new entry
	  for(Int_t listPos2=listLen; listPos2>listPos;listPos2--) {
	    diffList[listPos2][0] = diffList[listPos2-1][0];
	    diffList[listPos2][1] = diffList[listPos2-1][1];
	    diffList[listPos2][2] = diffList[listPos2-1][2];
	  }
	}
	
	// now we can fill the new array elemtns at position listPos
	listLen++;       // increase number of array elements by one

          // diffList contains:
//    cout << "Fill " << listPos << " " << diffSquared << " " << actAna 
//	<< " " << actIpu << endl;  
	diffList[listPos][0] = diffX*diffX + diffY*diffY;   // squared distance
	diffList[listPos][1] = actAna;  //  corresponding ana ring (anacoord)
	diffList[listPos][2] = actIpu;  //  corresponding ipu ring (ipucoord)
	listPos = 0;
}

    } // end of loop over IPUrings
  } // end of loop over softrings
} // end of loop over sectors

//- ****** now we can start to correlate ipu and ana rings based on diffList

//- ****** group ipu-ana hit pairs with smallest distances **   
      
nonCorrelatedIpu = mu_hits;
nonCorrelatedAna = ana_hits;
listPos = 0; 
listPos2 = 0;                                                    
while ( ( nonCorrelatedIpu > 0 ) && ( nonCorrelatedAna > 0 ) && ( listPos<listLen) ) { 
  actAna = diffList[listPos][1];
  actIpu = diffList[listPos][2];
  if ( (ipucoord[actIpu][0] >0) && (anacoord[actAna][0]>0) ) {
    corrList[listPos2][1] = anacoord[actAna][0];
    corrList[listPos2][2] = anacoord[actAna][1];
    corrList[listPos2][3] = ipucoord[actIpu][0];
    corrList[listPos2][4] = ipucoord[actIpu][1];
    listPos2++;
    anacoord[actAna][0] = 0;
    anacoord[actAna][1] = 0;
    ipucoord[actIpu][0] = 0;
    ipucoord[actIpu][1] = 0;
    nonCorrelatedIpu--; nonCorrelatedAna--;
  }
  listPos++;
}                       
//  cout << " non correlated: " << nonCorrelatedIpu << " " << nonCorrelatedAna << endl;

//- ****** group all remaining ipu (or ana) hits with (0,0) **
   
    for( actAna=0; actAna<ana_hits;actAna++) {
      // cout << "actAna: " << actAna << endl;
      if ( (anacoord[actAna][0] != 0) && (anacoord[actAna][1] != 0) ) {
//      cout << anacoord[actAna][0] << " "
//           << anacoord[actAna][1] << endl;
        corrList[listPos2][1] = anacoord[actAna][0];
        corrList[listPos2][2] = anacoord[actAna][1];
        corrList[listPos2][3] = 0;
        corrList[listPos2][4] = 0;
	hnca->Fill((Float_t)corrList[listPos2][1],(Float_t)corrList[listPos2][2],(Stat_t)1);
        //if (corrList[listPos][1]==0 && corrList[listPos][2]==0 ){
	//      cout << "CorrAna " << corrList[listPos2][1] << " "
	//           << corrList[listPos2][2] << " " << corrList[listPos2][3] << " "
	//           << corrList[listPos2][4] << " " << anacoord[actAna][0] << " "
	//           << anacoord[actAna][1] << endl;
        //}
        listPos2++;
      }
    }  

    for( actIpu=0; actIpu<mu_hits;actIpu++) {
      if ( (ipucoord[actIpu][0] != 0) && (ipucoord[actIpu][1] != 0) ) {
        corrList[listPos2][1] = 0;
        corrList[listPos2][2] = 0;
        corrList[listPos2][3] = ipucoord[actIpu][0];
        corrList[listPos2][4] = ipucoord[actIpu][1];
        hnci->Fill((Float_t)corrList[listPos2][3],(Float_t)corrList[listPos2][4],(Stat_t)1);
	//      cout << "CorrIPu " << corrList[listPos2][1] << " "
	//           << corrList[listPos2][2] << " " << corrList[listPos2][3] << " "
	//           << corrList[listPos2][4] << " " << ipucoord[actIpu][0] << " "
	//           << ipucoord[actIpu][1] << endl;
        listPos2++;
      }
    }                               
    //    cout << " ListPos2: " << listPos2 << endl;
    
//- ****** fill histograms from list of correlated pairs (corrList) **
    for ( listPos = 0; listPos<listPos2; listPos++ ) {
      //        cout << "Corr " << corrList[listPos][1] << " "
      //         << corrList[listPos][2] << " " << corrList[listPos][3] << " "
      //         << corrList[listPos][4] << endl;
      hshcx->Fill((Float_t) corrList[listPos][1],(Float_t) corrList[listPos][3],(Stat_t)1);
      hshcy->Fill((Float_t) corrList[listPos][2],(Float_t) corrList[listPos][4],(Stat_t)1);
    }

  //- *********** End of Correlation ************


}
//============================================================================

//----------------------------------------------------------------------------
void HMatchUMonitor::tofcorr(){
}
//============================================================================

//----------------------------------------------------------------------------
void HMatchUMonitor::showercorr(){
}
//============================================================================


//----------------------------------------------------------------------------

ClassImp(HMatchUMonitor)


               
