#include "hshowergrootreader.h"
#include "hades.h"
#include "hdebug.h"
#include "showerdef.h"
#include <TFile.h>
#include "hspectrometer.h"
#include "hdetector.h"
#include "hevent.h"
#include "hdatasource.h"
#include <iostream.h>


///////////////////////////////////////////////
//     HShowerGRootReader 
///////////////////////////////////////////////


ClassImp(HShowerGRootReader);
 

HShowerGRootReader:: HShowerGRootReader(void) {
                                             // Constructor
   fShowGeantHit=NULL;
   tr = NULL;
   hit = NULL;
   fEventId = 0;
}
HShowerGRootReader:: ~HShowerGRootReader(void) {};


struct geant_shower {
 HMatrixCategory oHMatrixCategory;
 HIndexTable     oHIndexTable;
 Int_t           fNDataObjs;
 Int_t           trackNumber[720];            // GEANT track number
 Float_t         eHit[720];                   // Energy loss (GeV)
 Float_t         xHit[720];                   // x - corr (in cm)
 Float_t         yHit[720];                   // y - corr (in cm)
 Float_t         thetaHit[720];               // theta angle (0 - 180 deg)
 Float_t         phiHit[720];                 // phi angle   (0 - 360 deg)
 Float_t         betaHit[720];                 // time of flight (in ns)
 Char_t          sector[720];                 // sector number (0 - 5)
 Char_t          module[720];                 // module number (0 - 2)
} show_root;
  
     
Bool_t HShowerGRootReader::init(void)  {
                                             // init function
   fShowGeantHit = gHades->getCurrentEvent()->getCategory(catShowerGeantRaw);
   if (!fShowGeantHit) {
   fShowGeantHit = gHades->getSetup()->getDetector("Shower")->buildCategory(catShowerGeantRaw);
   if(!fShowGeantHit) return kFALSE;
   else gHades->getCurrentEvent()->addCategory(catShowerGeantRaw,fShowGeantHit,"Shower"); 
 }
   tr=(TTree*)fInputFile->Get("T"); 
   tr->SetBranchAddress("HGeantShower",&show_root.oHMatrixCategory);
   tr->SetBranchAddress("GeantShower.*fIndexTable",&show_root.oHIndexTable);
   tr->SetBranchAddress("HGeantShower.fNDataObjs",&show_root.fNDataObjs);
   tr->SetBranchAddress("HGeantShower.fData.trackNumber",show_root.trackNumber);
   tr->SetBranchAddress("HGeantShower.fData.eHit",show_root.eHit);
   tr->SetBranchAddress("HGeantShower.fData.xHit",show_root.xHit);
   tr->SetBranchAddress("HGeantShower.fData.yHit",show_root.yHit);
   tr->SetBranchAddress("HGeantShower.fData.thetaHit",show_root.thetaHit);
   tr->SetBranchAddress("HGeantShower.fData.phiHit",show_root.phiHit);
   tr->SetBranchAddress("HGeantShower.fData.betaHit",show_root.betaHit);
   tr->SetBranchAddress("HGeantShower.fData.sector",show_root.sector);
   tr->SetBranchAddress("HGeantShower.fData.module",show_root.module);
  
   return kTRUE;

}

Bool_t HShowerGRootReader::execute(void) {
                                         // execute function 
 fprintf(stderr,"\rProcessing event: %d", fEventId+1);
 if (tr->GetEvent(fEventId)==0) return kDsEndData;  // end of tree reached

  if ((show_root.fNDataObjs)>720) {
      printf("\n**** Number of tracks too large\n");
      return kDsError;
    } 


 for(Int_t j = 0; j < (show_root.fNDataObjs); j++) {

   loc.set(2,show_root.sector[j],j); 
   hit=(HShowerGeantHit*)fShowGeantHit->getObject(loc);
   if(!hit){
   hit = (HShowerGeantHit*)fShowGeantHit->getSlot(loc);
   hit = new(hit) HShowerGeantHit;
   }
   hit->setXY(show_root.xHit[j],show_root.yHit[j]);
   hit->setAngles(show_root.thetaHit[j],show_root.phiHit[j]);
   hit->setEnerLos(show_root.eHit[j]);
   hit->setBeta(show_root.betaHit[j]);
   hit->setDetector(Int_t(show_root.module[j]));
   hit->setSector(Int_t(show_root.sector[j]));

   hit->setTrack(Int_t(show_root.trackNumber[j]));
 
}
   fEventId++; 


return kDsOk;
}   
  


