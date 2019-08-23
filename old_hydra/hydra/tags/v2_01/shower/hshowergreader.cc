#include "hshowergreader.h"
#include "hades.h"
#include "hshowergeanthit.h"
#include "hdebug.h"
#include "showerdef.h"
#include <TFile.h>

//*-- Author : 
//*-- Modified : 18/06/98 by Manuel Sanchez

//////////////////////////////
//HShowerGReader
//
//  Reader for GEANT TOF data.
//  adapted from /u/halo/packages/abase/new
//////////////////////////////

HShowerGReader::HShowerGReader(void) {
  //Default constructor
//  fHitCat=gHades->getCurrentEvent()->getCategory(catShowerGeantHit);
  fEventId=0;
  tr = NULL;
  shower=new shower_tuple;
}

HShowerGReader::~HShowerGReader(void) {
  //Destructor
  delete shower;
}


Bool_t HShowerGReader::init(void) {
      char *trname={"h4001"};

      if (shower==NULL) gDebuger->message("Chungo");

      tr=(TTree*)fInputFile->Get(trname);

      tr->SetBranchAddress("Idevt",&shower->idevt);
      tr->SetBranchAddress("Nhit",&shower->nhit);
      tr->SetBranchAddress("Showbeta",&shower->betahit);
      tr->SetBranchAddress("Showmom",&shower->momhit);
      tr->SetBranchAddress("Showde",&shower->eloshit);
      tr->SetBranchAddress("Showdet",&shower->dethit);
      tr->SetBranchAddress("Showphi",&shower->phihit);
      tr->SetBranchAddress("Showtheta",&shower->thetahit);

      return kTRUE;
}

Bool_t HShowerGReader::execute(void) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HShowerGReader::execute");
#endif
      TTree *tr;
      char trname[6];

      if (shower==NULL) gDebuger->message("Chungo");

      for (int i=0; i < 6; i++) {
         sprintf(trname, "h400%d", i + 1);
         tr=(TTree*)fInputFile->Get(trname);

         tr->SetBranchAddress("Idevt",&shower->idevt);
         tr->SetBranchAddress("Nhit",&shower->nhit);
         tr->SetBranchAddress("Showbeta",&shower->betahit);
         tr->SetBranchAddress("Showmom",&shower->momhit);
         tr->SetBranchAddress("Showde",&shower->eloshit);
         tr->SetBranchAddress("Showdet",&shower->dethit);
         tr->SetBranchAddress("Showphi",&shower->phihit);
         tr->SetBranchAddress("Showtheta",&shower->thetahit);
         tr->SetBranchAddress("Showx",&shower->xhit);
         tr->SetBranchAddress("Showy",&shower->yhit);
#if DEBUG_LEVEL>2
	gDebuger->message("Reading event %i",fEventId);
#endif
	tr->GetEvent(fEventId);

        fillHit(shower, i);
     }
     fEventId++;

#if DEBUG_LEVEL>2
      gDebuger->leaveFunc("HShowerGReader::execute");
#endif
      return kTRUE;
}         

void HShowerGReader::fillHit(shower_tuple *shower, int sect) {
    HShowerGeantHit *hit=NULL;
    int i;
    HLocation loc;

    for(i = 0; i < shower->nhit; i++) {
        hit=(HShowerGeantHit *)fHitCat->getNewSlot(loc);
        hit= new(hit) HShowerGeantHit;

        hit->setXY(shower->xhit[i], shower->yhit[i]);
        hit->setAngles(shower->thetahit[i], shower->phihit[i]);
        hit->setEnerLos(shower->eloshit[i]);
        hit->setBeta(shower->betahit[i]);
        hit->setMom(shower->momhit[i]);
        hit->setDetector(shower->dethit[i] - 1);
        hit->setSector(sect); 
   }
} 

ClassImp(HShowerGReader)
 
