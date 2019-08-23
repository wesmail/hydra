#include "hrichgreader.h"
#include "hades.h"
#include "hevent.h"
#include "hrichgeantdirhit.h"
#include "hrichgeantcherhit.h"
#include "hdebug.h"
#include "richdef.h"
#include <TFile.h>

//*-- Author : 
//*-- Modified : 18/06/98 by Manuel Sanchez

//////////////////////////////
//HRichGReader
//
//  Reader for GEANT TOF data.
//  adapted from /u/halo/packages/abase/new
//////////////////////////////

HRichGReader::HRichGReader(void) {
  //Default constructor
  fDirHitCat=gHades->getCurrentEvent()->getCategory(catRichGeantDirHit);
  fCherHitCat=gHades->getCurrentEvent()->getCategory(catRichGeantCherHit);
  fEventId=0;
  tr = NULL;
  rich=new rich_tuple;
}

HRichGReader::~HRichGReader(void) {
  //Destructor
  printf("dtor\n");
  delete rich;
}

Bool_t HRichGReader::init(void)
{
      TTree *tr;
      char *trname={"TR"};

      if (rich==NULL) gDebuger->message("Chungo");

      tr=(TTree*)fInputFile->Get(trname);

      tr->SetBranchAddress("evt",&rich->idevt);
      tr->SetBranchAddress("cer1",&rich->ncer);
      tr->SetBranchAddress("cer2",&rich->xcer);
      tr->SetBranchAddress("cer3",&rich->ycer);
      tr->SetBranchAddress("cer4",&rich->ecer);
      tr->SetBranchAddress("cer5",&rich->trklength);
      tr->SetBranchAddress("cer6",&rich->mediumnum);
      tr->SetBranchAddress("cer7",&rich->statflag);
      tr->SetBranchAddress("cer8",&rich->partrkcer);
      tr->SetBranchAddress("cer9",&rich->parentcer);
      tr->SetBranchAddress("cer10",&rich->procescer);
      tr->SetBranchAddress("hit1",&rich->nhit);
      tr->SetBranchAddress("hit2",&rich->xhit);
      tr->SetBranchAddress("hit3",&rich->yhit);
      tr->SetBranchAddress("hit4",&rich->zhit);
      tr->SetBranchAddress("hit5",&rich->parthit);
      tr->SetBranchAddress("hit6",&rich->trklenhit);
      tr->SetBranchAddress("hit7",&rich->momhit);
      tr->SetBranchAddress("hit8",&rich->thetahit);
      tr->SetBranchAddress("hit9",&rich->phihit);
      tr->SetBranchAddress("hit10",&rich->eloshit);
      tr->SetBranchAddress("hit11",&rich->tlenhit);
      tr->SetBranchAddress("hit12",&rich->mirhit);
      tr->SetBranchAddress("hit13",&rich->trkhit);
      tr->SetBranchAddress("hit14",&rich->parenthit);
      tr->SetBranchAddress("hit15",&rich->proceshit);

      return kTRUE;
}

Bool_t HRichGReader::execute(void) {
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichGReader::execute");
#endif
      TTree *tr;
      char *trname={"TR"};

      if (rich==NULL) gDebuger->message("Chungo");

      tr=(TTree*)fInputFile->Get(trname);

      tr->SetBranchAddress("evt",&rich->idevt);
      tr->SetBranchAddress("cer1",&rich->ncer);
      tr->SetBranchAddress("cer2",&rich->xcer);
      tr->SetBranchAddress("cer3",&rich->ycer);
      tr->SetBranchAddress("cer4",&rich->ecer);
      tr->SetBranchAddress("cer5",&rich->trklength);
      tr->SetBranchAddress("cer6",&rich->mediumnum);
      tr->SetBranchAddress("cer7",&rich->statflag);
      tr->SetBranchAddress("cer8",&rich->partrkcer);
      tr->SetBranchAddress("cer9",&rich->parentcer);
      tr->SetBranchAddress("cer10",&rich->procescer);
      tr->SetBranchAddress("hit1",&rich->nhit);
      tr->SetBranchAddress("hit2",&rich->xhit);
      tr->SetBranchAddress("hit3",&rich->yhit);
      tr->SetBranchAddress("hit4",&rich->zhit);
      tr->SetBranchAddress("hit5",&rich->parthit);
      tr->SetBranchAddress("hit6",&rich->trklenhit);
      tr->SetBranchAddress("hit7",&rich->momhit);
      tr->SetBranchAddress("hit8",&rich->thetahit);
      tr->SetBranchAddress("hit9",&rich->phihit);
      tr->SetBranchAddress("hit10",&rich->eloshit);
      tr->SetBranchAddress("hit11",&rich->tlenhit);
      tr->SetBranchAddress("hit12",&rich->mirhit);
      tr->SetBranchAddress("hit13",&rich->trkhit);
      tr->SetBranchAddress("hit14",&rich->parenthit);
      tr->SetBranchAddress("hit15",&rich->proceshit);

#if DEBUG_LEVEL>2
	gDebuger->message("Reading event %i",fEventId);
#endif
	tr->GetEvent(fEventId);
        fEventId++;

        fillDirHit(rich);
        fillCherHit(rich);

#if DEBUG_LEVEL>2
      gDebuger->leaveFunc("HRichGReader::execute");
#endif
      return kTRUE;
}         

void HRichGReader::fillDirHit(rich_tuple *rich) {
    HRichGeantDirHit *hit=NULL;
    int i;
    HLocation loc;

    for(i = 0; i < rich->nhit; i++) {
        hit=(HRichGeantDirHit *)fDirHitCat->getNewSlot(loc);
        hit = new(hit) HRichGeantDirHit;

        hit->setXYZ(rich->xhit[i], rich->yhit[i], rich->zhit[i]);
        hit->nPartHit = rich->parthit[i]; 
        hit->fTrkLenHit = rich->trklenhit[i];
        hit->fMomHit = rich->momhit[i];
        hit->setAngles(rich->thetahit[i], rich->phihit[i]);
        hit->setEnerLos(rich->eloshit[i]);
        hit->setTLen(rich->tlenhit[i]);
        hit->setMirror(rich->mirhit[i]); 
        hit->nTrkHit = rich->trkhit[i];
        hit->nParentHit = rich->parenthit[i];
        hit->nProcesHit = rich->proceshit[i];
   }
} 

void HRichGReader::fillCherHit(rich_tuple *rich) {
    HRichGeantCherHit *hit=NULL;
    int i;
    HLocation loc;

    for(i = 0; i < rich->ncer; i++) {
        hit=(HRichGeantCherHit *)fCherHitCat->getNewSlot(loc);
        hit = new(hit) HRichGeantCherHit;

        hit->setXY(rich->xcer[i], rich->ycer[i]);
        hit->setEner(rich->ecer[i]);
        hit->setTrkLen(rich->trklength[i]);
        hit->nMediumNum = rich->mediumnum[i];
        hit->setStatFlag(rich->statflag[i]);
        hit->nPartrkCer = rich->partrkcer[i];
        hit->nParentCer = rich->parentcer[i];
        hit->nProcesCer = rich->procescer[i];
   }
 
} 
ClassImp(HRichGReader)
 
