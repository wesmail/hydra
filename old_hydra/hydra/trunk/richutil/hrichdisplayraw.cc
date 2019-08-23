// File: hrichdisplayraw.cc
//
// Author: Walter Schoen <W.Schoen@physik.tu-muenchen.de>
// Last update by Walter Schoen: 99/09/10 11:32:03
// Modified by Thomas Eberl: 99/09/02
//
#include "hrichdisplayraw.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hcategory.h"
#include "hmatrixcatiter.h"
#include "hlocation.h"
#include "hrichraw.h"
#include "hrichcal.h"
#include "hrichcalibrater.h"
#include "hdebug.h"
#include "richdef.h"
#include "hrichgeometrypar.h"
#include "hdetgeompar.h"
#include "hruntimedb.h"

Int_t HRichDisplayRaw::producer(void){     
sharedFile = TMapFile::Create("RICHonline.map","RECREATE", 1000000,"shared memory file");

 counter = new TVector(1);
 (*counter)(0)=0;
    for (Int_t i=0;i<sectorNum;i++){
         for (Int_t j=0;j<2;j++){
             richHisto[i][j] = 0;
        }
    }

    Char_t hname1[40];
    Char_t hname2[40];
    

    for (Int_t i =0; i< sectorNum ; i++){

         sprintf(hname1,"singleEvt_%d",i + 1);
         sprintf(hname2,"accumEvt_%d",i + 1);
         
	
         richHisto[i][0] = new TH2F(hname1,"single event",92,1,92,90,1,90);
         richHisto[i][1] = new TH2F(hname2,"accumulated event",92,1,92,90,1,90);
        
    }
  
   sharedFile->Add(counter,"counter");
   sharedFile->Update();
   
    return 0;
}

HRichDisplayRaw::HRichDisplayRaw(const Text_t *name, const Text_t *title) :
 HReconstructor(name,title) 
{ 

   pRichRaw = 0;
}
HRichDisplayRaw::HRichDisplayRaw()
{

   pRichRaw = 0;
}
HRichDisplayRaw::~HRichDisplayRaw(void) {
  
  
   for ( Int_t i =0; i<sectorNum; i++){

  delete richHisto[i][0]; 
  delete richHisto[i][1]; 

   } 
}

Bool_t HRichDisplayRaw::init() {
  
    HRichDetector *pRichDet = 
	(HRichDetector*)gHades->getSetup()->getDetector("Rich");
    HRuntimeDb* rtdb=gHades->getRuntimeDb();
    HRichGeometryPar *pGeomPar = (HRichGeometryPar*)rtdb->
                                  getContainer("RichGeometryParameters");
      
  
    pRichRaw=gHades->getCurrentEvent()->getCategory(catRichRaw);
    sectorNum= pGeomPar->getSectorsNr(); 
    ncounter=0;
    producer();
    
  
  
    if (!pRichRaw) {
      pRichRaw=pRichDet->buildCategory(catRichRaw);

      if (!pRichRaw) return kFALSE;
      else gHades->getCurrentEvent()
                         ->addCategory(catRichRaw, pRichRaw, "Rich");
    }
    return kTRUE;
} 
Bool_t HRichDisplayRaw::finalize(void) {
    return kTRUE;
}
Bool_t HRichDisplayRaw::display(HLocation &refLoc){
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HRichDisplay::display");
  gDebuger->message("Raw cat points to %p",fRawCat);
#endif


  HRichRaw *pRaw=0;

  pRaw=(HRichRaw *)pRichRaw->getObject(refLoc);
  
  if(pRaw) {
    Int_t sec = pRaw->getSector();
    Float_t charge = pRaw->getCharge();
    // if (charge != 0) cout<<" **************** charge from Raw:"<<charge<<endl;
    Int_t row = pRaw->getRow();
    Int_t col = pRaw->getCol();

    richHisto[sec][0]->Fill(col,row,charge);
    richHisto[sec][1]->Fill(col,row,charge);
  }
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HRichDisplay::display");
#endif
  return kTRUE;

}
Int_t HRichDisplayRaw::execute() {
    HMatrixCatIter *pIter;
    HRichRaw *pRaw=NULL;
    HLocation loc;    
    pIter = (HMatrixCatIter*)getRawCat()->MakeIterator(); 
    Int_t n =0;
    for (Int_t k =0; k< sectorNum ; k++){ 
	richHisto[k][0] -> Reset();
    } 
    while((pRaw = (HRichRaw *)pIter->Next()))
    {
	loc = pIter->getLocation();
	display(loc);
	n++;
    }
    ncounter++;
    (*counter)(0)=ncounter;
    cout << "event: " <<ncounter<< endl;
    sharedFile->Update();
    return kTRUE;
}
ClassImp(HRichDisplayRaw)














