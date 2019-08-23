# include "hdebug.h"
# include "hmatrixcategory.h"
# include "hmatrixcatiter.h"
# include "hmdccal2sim.h"
# include "hmdccal3sim.h"
# include "hmdccaltable.h"
# include "hmdcmodulegeometry.h"
# include "hmdchitfsim.h"
# include <iostream.h>

ClassImp(HMdcHitFSim)



  void HMdcHitFSim :: buildList(void){

// transforms cal2sim data into calhitsim, and
// builds a double linked list with the calhitsim data.

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitFSim::buildList");
#endif

HMdcCal2Sim* cal;

// loop over cal2 category to fill calhit category
// cal2sim data must be filtered: the HMdcDigitizer:: execute function
// sets the data member "status"=1 if cell is ok
 
Int_t nlayers = geo->getNLayers();

Float_t pitch[nlayers];
Float_t centralWire[nlayers];

Int_t sector = fLoc[0];
Int_t module = fLoc[1];
Int_t layer, cell, nHits;
Int_t offset[nlayers];


 for(Int_t i=0; i<nlayers; i++){
   offset[i] = 0;
   pitch[i] = geo->getPitch(sector,module,i);
   centralWire[i] = geo->getCentralWire(sector,module,i) - 1.;
 }



HLocation loc;
 loc.set(4,sector,module,0,0);        // location to be used when filling
                                      // calhitsim category 

HMdcCal3Sim *hit1, *hit2;
Float_t pos1, pos2, coordwire=30000;

#if DEBUG_LEVEL>3
 ofstream file("cal.txt",ios::app);

 file << "suceso " << fEventId << endl;
#endif

HMatrixCatIter* iter=0;
iter = (HMatrixCatIter*)fCalCat->MakeIterator();
iter->Reset();
iter->gotoLocation(fLoc);
 while((cal=(HMdcCal2Sim*)iter->Next())!=NULL){
   #if DEBUG_LEVEL>3
   file << endl;
   file << "status " << cal->getStatus() << endl;
   #endif

   if(cal->getStatus()==1){                       // filtering

     nHits = cal->getNHits();

     #if DEBUG_LEVEL>3
     file << "nHits " << nHits << endl;
     #endif

     if(nHits > 0){
       layer = cal->getLayer();
       cell  = cal->getCell();

       #if DEBUG_LEVEL>3
       file << "l " << layer << '\t' << "c " << cell << endl;
       #endif
       // first calhitsim

       loc[2] = layer;
       loc[3] = offset[layer];
       hit1 = (HMdcCal3Sim*)mdcCal->getSlot(loc);
       offset[layer]++;
       
       // second calhitsim
       loc[3] = offset[layer];
       hit2 = (HMdcCal3Sim*)mdcCal->getSlot(loc);
       offset[layer]++;

       if(hit1 && hit2){
	  hit1 = new(hit1) HMdcCal3Sim;
	  hit1->setId(cal->getNTrack1());
	  hit1->setAddress(sector,module,layer,cell,0);
	  hit1->setError(cal->getErrDist1());
	  
	  coordwire = (cell-centralWire[layer])*pitch[layer];
	  if(layer==0 || layer ==  4) coordwire = -coordwire;

	  pos1 = coordwire + cal->getDist1();
	  pos2 = coordwire - cal->getDist1();

	  hit1->setPos(pos1);

	  hit2 = new(hit2) HMdcCal3Sim(hit2);
	  hit2->setId(cal->getNTrack1());
	  hit2->setPos(pos2);
	  hit2->setAddress(sector,module,layer,cell,1);
	  hit2->setError(cal->getErrDist1());
	  fList[layer]->addHits(hit1,hit2);	 

	  #if DEBUG_LEVEL>3
	  file << "first hit" << endl;
	  file << "id " << cal->getNTrack1() << endl;
	  file << pos1 << " +- " << cal->getErrDist1() << endl;
	  file << pos2 << "+- " << cal->getErrDist1() << endl;
	  #endif
       }

       if(nHits>1){

	 // first calhitsim

	 loc[3] = offset[layer];
	 hit1 = (HMdcCal3Sim*)mdcCal->getSlot(loc);
	 offset[layer]++;
       
	 // second calhitsim
	 loc[3] = offset[layer];
	 hit2 = (HMdcCal3Sim*)mdcCal->getSlot(loc);
	 offset[layer]++;

	 if(hit1 && hit2){
	   hit1 = new(hit1) HMdcCal3Sim;
	   hit1->setId(cal->getNTrack2());
	   hit1->setAddress(sector,module,layer,cell,2);
	   hit1->setError(cal->getErrDist2());

	   pos1 = coordwire + cal->getDist2();
	   pos2 = coordwire - cal->getDist2();

	   hit1->setPos(pos1);

	   hit2 = new(hit2) HMdcCal3Sim(hit2);
	   hit2->setId(cal->getNTrack2());
	   hit2->setPos(pos2);
	   hit2->setAddress(sector,module,layer,cell,3);
	   hit2->setError(cal->getErrDist2());
	   fList[layer]->addHits(hit1,hit2);	 

	   #if DEBUG_LEVEL>3
	   file << "second hit" << endl;
	   file << "id " << cal->getNTrack2() << endl;
	   file << pos1 << " +- " << cal->getErrDist2() << endl;
	   file << pos2 << "+- " << cal->getErrDist2() << endl;
           #endif
	 }	 

       }                                      // end of check in nHits > 1

     }                                         // end of check in nHits > 0
   }                                           // end of check in status
 }                                             // end of loop in cal2 category



#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitFSim::buildList");
#endif


}




