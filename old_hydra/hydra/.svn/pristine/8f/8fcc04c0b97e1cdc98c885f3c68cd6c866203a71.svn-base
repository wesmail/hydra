# include <fstream.h>
# include <math.h>
# include "TH1.h"
# include "TH2.h"
# include "TMatrix.h"
# include "hades.h"
# include "hcategory.h"
# include "hdebug.h"
# include "hdetector.h"
# include "hevent.h"
# include "hlocation.h"
# include "hreconstructor.h"
# include "hmatrixcatiter.h"
# include "hmdcdetector.h"
# include "hmdcmodulegeometry.h"
# include "hmdchit.h"
# include "hmdcleverarmgeometry.h"
# include "hmdcseg.h"
# include "hmdcsegmentf.h"
# include "hmdcsegmentfpar.h"
# include "hruntimedb.h"
# include "hspectrometer.h"
# include "mdcsdef.h"

ClassImp(HMdcSegmentF)

//*-- Author : Beatriz Fuentes Arenaz
//*-- Modified: 27/8/2000 by Beatriz Fuentes (official data levels)
//*-- Modified: 25/2/2000 by Beatriz Fuentes  (units histos: cm --> mm)
//*-- Modified: 23/2/2000 by Beatriz Fuentes
//*-- Modified : 13/12/99 by Beatriz Fuentes
//*-- Date: 11/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)


//////////////////////////////////////////////////////////////////
//
// HMdcSegmentF
//
// An HMdcSegmentF is a reconstructor that finds pieces of tracks
// in two MDC modules. 
//
//////////////////////////////////////////////////////////////////


HMdcSegmentF :: HMdcSegmentF(void) : HReconstructor(){
  // default constructor 

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::HMdcSegmentF()");
#endif

  fHitCat = NULL;
  fSegCat = NULL;
  geoLever = NULL;
  geoModules = NULL;
  fLoc.setNIndex(2);
  fEventId = 0;
  parContainer = NULL;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::HMdcSegmentF()");
#endif

}

HMdcSegmentF :: HMdcSegmentF(Text_t* name, Text_t* title) :                                   HReconstructor(name, title){
  // default constructor 

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::HMdcSegmentF(Text_t*, Text_t*)");
#endif

  fHitCat = NULL;
  fSegCat = NULL;
  geoLever = NULL;
  geoModules = NULL;
  fLoc.setNIndex(2);
  fEventId = 0;
  parContainer = NULL;
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::HMdcSegmentF(Text_t*, Text_t*)");
#endif
}


void HMdcSegmentF ::  setParContainers(void){
#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::setParContainers");
#endif


  parContainer=(HMdcSegmentFPar*)gHades->getRuntimeDb()->getContainer("MdcSegmentFPar");
  if (!parContainer) {
    parContainer=new HMdcSegmentFPar;
    gHades->getRuntimeDb()->addContainer(parContainer);
  }

  geoLever=(HMdcLeverArmGeometry*)gHades->getRuntimeDb()->getContainer("MdcLeverArmGeometry");
  if (!geoLever) {
    geoLever=new HMdcLeverArmGeometry;
    gHades->getRuntimeDb()->addContainer(geoLever);
  }

  geoModules=(HMdcModuleGeometry*)gHades->getRuntimeDb()->getContainer("MdcModuleGeometry");
  if (!geoModules) {
    geoModules=new HMdcModuleGeometry;
    gHades->getRuntimeDb()->addContainer(geoModules);
  }


#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::setParContainers");
#endif
}




void HMdcSegmentF :: bookHisto(void){
  // declare the histograms and add them to the list of histogram

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::bookHisto");
#endif

  char hname[40];
  char htitle[100];
  char title[100];

  Int_t module1, module2;
  if(fLoc.getIndex(1)==0) module1=0;      // first lever arm
  else module1=2;                 // second lever arm

  module2 = module1 +1;



 sprintf(hname,"hDiffXRec_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"x difference in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffXRec = new TH1F(hname, htitle, 100, -30.,30.);
 sprintf(title,"x(MDC%d)-x(MDC%d) (mm)",module2,module1);
 hDiffXRec->SetXTitle(title);


 sprintf(hname,"hDiffYRec_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"y difference in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffYRec = new TH1F(hname, htitle, 100, -30.,30.);
 sprintf(title,"y(MDC%d)-y(MDC%d) (mm)",module2,module1);
 hDiffYRec->SetXTitle(title);

 sprintf(hname,"hDiffSlopeXRec_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"x slope difference in mdc%d, rec. tracks",fLoc[1]+1); 
 hDiffSlopeXRec = new TH1F(hname,htitle,100,-0.5,0.5);
 sprintf(title,"xslope(MDC%d)-xslope(MDC%d)",module2,module1);
 hDiffSlopeXRec->SetXTitle(title);

 sprintf(hname,"hDiffSlopeYRec_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"y slopedifference in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffSlopeYRec = new TH1F(hname,htitle,100,-.5,.5);
 sprintf(title,"yslope(MDC%d)-yslope(MDC%d)",module2,module1);
 hDiffSlopeYRec->SetXTitle(title);

 sprintf(hname,"hDiffYDiffX_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"y1-y2 versus x1-x2 in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffYDiffX = new TH2F(hname, htitle, 100, -20., 20., 1000, -20., 20.);
 sprintf(title,"x(MDC%d)-x(MDC%d) (mm)",module1,module2);
 hDiffYDiffX->SetXTitle(title);
 sprintf(title,"y(MDC%d)-y(MDC%d) (mm)",module2,module1);
 hDiffYDiffX->SetYTitle(title);

 sprintf(hname,"hDiffXSlopeDiffX_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"xslope1-xslope2 versus x1-x2 in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffXSlopeDiffX = new TH2F(hname, htitle, 100, -20., 20., 100, -0.3, 0.3);
 sprintf(title,"x(MDC%d)-x(MDC%d) (mm)",module2,module1);
 hDiffXSlopeDiffX->SetXTitle(title);
 sprintf(title,"xslope(MDC%d)-xslope(MDC%d)",module2,module1);
 hDiffXSlopeDiffX->SetYTitle(title);


 sprintf(hname,"hDiffYSlopeDiffX_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"yslope1-yslope2 versus x1-x2 in mdc%d, rec.tracks",fLoc[1]+1);
 hDiffYSlopeDiffX = new TH2F(hname, htitle, 100, -20., 20., 100, -0.3, 0.3);  
 sprintf(title,"x(MDC%d)-x(MDC%d) (mm)",module2,module1);
 hDiffYSlopeDiffX->SetXTitle(title);
 sprintf(title,"yslope(MDC%d)-yslope(MDC%d)",module2,module1);
 hDiffYSlopeDiffX->SetYTitle(title); 


 sprintf(hname,"hDiffXSlopeDiffY_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"xslope1-xslope2 versus y1-y2 in mdc%d, rec.tracks",fLoc[1]+1);
 hDiffXSlopeDiffY = new TH2F(hname, htitle, 100, -20., 20., 100, -0.3, 0.3);  
 sprintf(title,"y(MDC%d)-y(MDC%d) (mm)",module2,module1);
 hDiffXSlopeDiffY->SetXTitle(title);
 sprintf(title,"xslope(MDC%d)-xslope(MDC%d)",module2,module1);
 hDiffXSlopeDiffY->SetYTitle(title);

 sprintf(hname,"hDiffYSlopeDiffY_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"yslope1-yslope2 versus y1-y2 in mdc%d, rec.tracks",fLoc[1]+1);
 hDiffYSlopeDiffY = new TH2F(hname, htitle, 100, -20., 20., 100, -0.3, 0.3);  
 sprintf(title,"y(MDC%d)-y(MDC%d) (mm)",module2,module1);
 hDiffYSlopeDiffY->SetXTitle(title);
 sprintf(title,"yslope(MDC%d)-yslope(MDC%d)",module2,module1);
 hDiffYSlopeDiffY->SetYTitle(title); 


 sprintf(hname,"hDiffXSlopeDiffYSlope_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"xslope1-xslope2 versus yslope1-yslope2 in mdc%d, rec.tracks",fLoc[1]+1); 
 hDiffXSlopeDiffYSlope = new TH2F(hname, htitle, 100, -0.3, 0.3, 100, -0.3, 0.3);
 sprintf(title,"yslope(MDC%d)-yslope(MDC%d)",module2,module1);
 hDiffXSlopeDiffYSlope->SetXTitle(title);
 sprintf(title,"xslope(MDC%d)-xslope(MDC%d)",module2,module1);
 hDiffXSlopeDiffYSlope->SetYTitle(title);   

 sprintf(hname,"hProbEllipse_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"probability in lever arm %d",fLoc[1]);
 hProbEllipse = new TH1F(hname, htitle, 100,0,100);
 hProbEllipse->SetXTitle("probability");



 sprintf(hname,"hDiffYDiffXOverErr_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"(y1-y2)/err versus (x1-x2)/err in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffYDiffXOverErr = new TH2F(hname, htitle, 100, -5., 5., 100, -5., 5.);
 sprintf(title,"(xslope(MDC%d)-xslope(MDC%d))/error",module2,module1);
 hDiffYDiffXOverErr->SetXTitle(title);
 sprintf(title,"(yslope(MDC%d)-yslope(MDC%d))/error",module2,module1);
 hDiffYDiffXOverErr->SetYTitle(title);


 sprintf(hname,"hDiffXSlopeDiffXOverErr_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"(xslope1-xslope2)/err versus (x1-x2)/err in mdc%d, rec. tracks",fLoc[1]+1);
 hDiffXSlopeDiffXOverErr = new TH2F(hname, htitle, 100, -5., 5., 100, -5, 5);
 sprintf(title,"(x(MDC%d)-x(MDC%d))/error",module2,module1);
 hDiffXSlopeDiffXOverErr->SetXTitle(title);
 sprintf(title,"(xslope(MDC%d)-xslope(MDC%d))/error",module2,module1);
 hDiffXSlopeDiffXOverErr->SetYTitle(title);


 sprintf(hname,"hDiffYSlopeDiffXOverErr_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"(yslope1-yslope2)/err versus (x1-x2)/err in mdc%d, rec.tracks",fLoc[1]+1);
 hDiffYSlopeDiffXOverErr = new TH2F(hname, htitle, 100, -5.,5., 100, -5, 5);
 sprintf(title,"(x(MDC%d)-x(MDC%d))/error",module2,module1);
 hDiffYSlopeDiffXOverErr->SetXTitle(title);
 sprintf(title,"(yslope(MDC%d)-yslope(MDC%d))/error",module2,module1);
 hDiffYSlopeDiffXOverErr->SetYTitle(title); 



 sprintf(hname,"hDiffXSlopeDiffYOverErr_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"(xslope1-xslope2)/err versus (y1-y2)/err in mdc%d, rec.tracks",fLoc[1]+1);
 hDiffXSlopeDiffYOverErr = new TH2F(hname, htitle, 100, -5., 5., 100, -5, 5);  
 sprintf(title,"(y(MDC%d)-y(MDC%d))/error",module2,module1);
 hDiffXSlopeDiffYOverErr->SetXTitle(title);
 sprintf(title,"(xslope(MDC%d)-xslope(MDC%d))/error",module2,module1);
 hDiffXSlopeDiffYOverErr->SetYTitle(title); 

 sprintf(hname,"hDiffYSlopeDiffYOverErr_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"(yslope1-yslope2)/err versus (y1-y2)/err in mdc%d, rec.tracks",fLoc[1]+1);
 hDiffYSlopeDiffYOverErr = new TH2F(hname, htitle, 100, -5., 5., 100, -5, 5);   sprintf(title,"(y(MDC%d)-y(MDC%d))/error",module2,module1);
 hDiffYSlopeDiffYOverErr->SetXTitle(title);
 sprintf(title,"(yslope(MDC%d)-yslope(MDC%d))/error",module2,module1);
 hDiffYSlopeDiffYOverErr->SetYTitle(title); 


 sprintf(hname,"hDiffXSlopeDiffYSlopeOverErr_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"(xslope1-xslope2)/err versus (yslope1-yslope2)/err in mdc%d, rec.tracks",fLoc[1]+1); 
 hDiffXSlopeDiffYSlopeOverErr = new TH2F(hname, htitle, 100, -5, 5, 100, -5, 5);    
 sprintf(title,"(yslope(MDC%d)-yslope(MDC%d))/error",module2,module1);
 hDiffXSlopeDiffYSlopeOverErr->SetXTitle(title);
 sprintf(title,"(xslope(MDC%d)-xslope(MDC%d))/error",module2,module1);
 hDiffXSlopeDiffYSlopeOverErr->SetYTitle(title);   

 sprintf(hname,"hFitDiffX_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"diff. between x(segment) and x(lever arm) in lever arm %d",fLoc[1]);
 hFitDiffX = new TH1F(hname, htitle, 100, -60.,60.);
 sprintf(title,"x(MDC)-x(lever arm) (mm)");
 hFitDiffX->SetXTitle(title);

 sprintf(hname,"hFitDiffY_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"diff. between y(segment) and y(lever arm) in lever arm %d",fLoc[1]);
 hFitDiffY = new TH1F(hname, htitle, 100, -60.,60.);
 sprintf(title,"y(MDC)-y(lever arm) (mm)");
 hFitDiffY->SetXTitle(title);

 sprintf(hname,"hFitDiffXSlope_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"diff. between xslope(segment) and xslope(lever arm) in lever arm %d",fLoc[1]);
 hFitDiffXSlope = new TH1F(hname, htitle, 100, -1.,1.);
 sprintf(title,"xslope(MDC)-xslope(lever arm)");
 hFitDiffXSlope->SetXTitle(title);

 sprintf(hname,"hFitDiffYSlope_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"diff. between yslope(segment) and yslope(lever arm) in lever arm %d",fLoc[1]);
 hFitDiffYSlope = new TH1F(hname, htitle, 100, -1.,1.);
 sprintf(title,"yslope(MDC)-yslope(lever arm) (cm)");
 hFitDiffYSlope->SetXTitle(title);

 sprintf(hname,"hFitChiX_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"chi2 of fit in x-z projection in lever arm %d",fLoc[1]);
 hFitChiX = new TH1F(hname, htitle, 100, 0.,6.);
 sprintf(title,"chi2");
 hFitChiX->SetXTitle(title);

 sprintf(hname,"hFitChiY_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"chi2 of fit in y-z projection in lever arm %d",fLoc[1]);
 hFitChiY = new TH1F(hname, htitle, 100, 0.,6.);
 sprintf(title,"chi2");
 hFitChiY->SetXTitle(title);

 sprintf(hname,"hFitChi_%d%d",fLoc[0],fLoc[1]);
 sprintf(htitle,"chi2 of fit in lever arm %d",fLoc[1]);
 hFitChi = new TH1F(hname, htitle, 100, 0.,6.);
 sprintf(title,"chi2");
 hFitChi->SetXTitle(title);


  fHistograms = new TList;
  fHistograms->AddLast(hDiffXRec);
  fHistograms->AddLast(hDiffYRec);
  fHistograms->AddLast(hDiffSlopeXRec);
  fHistograms->AddLast(hDiffSlopeYRec);
  fHistograms->AddLast(hDiffYDiffX);
  fHistograms->AddLast(hDiffXSlopeDiffX);
  fHistograms->AddLast(hDiffYSlopeDiffX);
  fHistograms->AddLast(hDiffXSlopeDiffY);
  fHistograms->AddLast(hDiffYSlopeDiffY);
  fHistograms->AddLast(hDiffXSlopeDiffYSlope);
  fHistograms->AddLast(hProbEllipse);
  fHistograms->AddLast(hDiffYDiffXOverErr);
  fHistograms->AddLast(hDiffXSlopeDiffXOverErr);
  fHistograms->AddLast(hDiffYSlopeDiffXOverErr);
  fHistograms->AddLast(hDiffXSlopeDiffYOverErr);
  fHistograms->AddLast(hDiffYSlopeDiffYOverErr);
  fHistograms->AddLast(hDiffXSlopeDiffYSlopeOverErr);  
  fHistograms->AddLast(hFitDiffX);
  fHistograms->AddLast(hFitDiffY);
  fHistograms->AddLast(hFitDiffXSlope);
  fHistograms->AddLast(hFitDiffYSlope);
  fHistograms->AddLast(hFitChiX);
  fHistograms->AddLast(hFitChiY);
  fHistograms->AddLast(hFitChi);

  sprintf(hname,"hDiffX_%d%d",fLoc[0],fLoc[1]);
  hDiffX = new TH1F(hname, "x difference in mdc2", 100, -2.,2.);
  sprintf(hname,"hDiffY_%d%d",fLoc[0],fLoc[1]);
  hDiffY = new TH1F(hname, "y difference in mdc2", 100, -2.,2.);
  sprintf(hname,"hDiffSlopeX_%d%d",fLoc[0],fLoc[1]);
  hDiffSlopeX = new TH1F(hname,"x slope difference in mdc2",100,-0.3,0.3);
  sprintf(hname,"hDiffSlopeY_%d%d",fLoc[0],fLoc[1]);
  hDiffSlopeY = new TH1F(hname,"y slope difference in mdc2",100,-0.3,0.3);

  fHistograms->AddLast(hDiffX);
  fHistograms->AddLast(hDiffY);
  fHistograms->AddLast(hDiffSlopeX);
  fHistograms->AddLast(hDiffSlopeY);
#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::bookHisto");
#endif
}


Bool_t HMdcSegmentF :: init(void){

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::init");
#endif

  fHitCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fHitCat) {
    fHitCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
    if (!fHitCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fHitCat,"Mdc");
  }


  fSegCat = gHades->getCurrentEvent()->getCategory(catMdcSeg);
  if (!fSegCat) {
    fSegCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcSeg);
    if (!fSegCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcSeg,fSegCat,"Mdc");
  }

  setParContainers();
  bookHisto();

  fActive=kTRUE;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::init");
#endif

  return kTRUE;
}


HMdcSegmentF :: ~HMdcSegmentF(void){
  // default destructor

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::~HMdcSegmentF");
#endif

  if(fHistograms){
    fHistograms->Delete();
    delete fHistograms;
  }

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::~HMdcSegmentF");
#endif

}
 
void HMdcSegmentF :: setLoc(HLocation& location){
  // set location of the reconstructor
  fLoc.setNIndex(2);
  fLoc.readIndexes(location);
} 



Int_t HMdcSegmentF :: execute(void){
  // execute function 
  // This function takes the category of hits in both modules
  // A hit in the first module is transformed into the reference system
  // of the second module. Then, both hits in the same reference system
  // are compared. 
  // OJO!!
  // They are compatible if the difference in the x,y 
  // coordinates in the first plane and the difference between directionss is 
  // smaller than a given road 

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::execute");
#endif

  Int_t sector = fLoc.getIndex(0);
  Int_t leverArm  = fLoc.getIndex(1);
  Int_t module1,module2;
  if(leverArm==0) module1=0;
  else module1=2;

  module2 = module1+1;


  HLocation loc1,loc2;
  loc1.set(2,sector,module1);
  loc2.set(2,sector,module2); 
  HMatrixCatIter *iter1 =NULL, *iter2=NULL;
  iter2=(HMatrixCatIter *)fHitCat->MakeIterator();
  iter2->Reset();

  HMdcHit* hit1,*hit2;
  Float_t x1,y1,z1,slopext,slopeyt;
  Float_t slopex,slopey, ux, uy;
  Float_t tx,ty,tz,cosn,sinn;
  tx = geoLever->getTrasElement(sector,leverArm,0);
  ty = geoLever->getTrasElement(sector,leverArm,1);
  tz = geoLever->getTrasElement(sector,leverArm,2);

  cosn = geoLever->getCosn(sector,leverArm);
  sinn = geoLever->getSinn(sector,leverArm);


  TMatrix rot(3,3);
  rot(0,0) = 1;
  rot(0,1) = 0;
  rot(0,2) = 0;
  rot(1,0) = 0;
  rot(1,1) = cosn;
  rot(1,2) = sinn;
  rot(2,0) = 0;
  rot(2,1) = -sinn;
  rot(2,2) = cosn;

  TMatrix tras(3,1);
  tras(0,0) = tx;
  tras(1,0) = ty;
  tras(2,0) = tz;


  Float_t diffX,diffY,diffSlopeX,diffSlopeY;
  TMatrix point0(3,1);
  TMatrix pointt(3,1);
  
  // get the z coordinate where hits are going to be compared (z coordinate of hits in second module)
  z1 = 0;                   // Achtung!! insert this in a parameter container


  // get reconstruction parameters from parContainer

  Float_t corrx = parContainer->getCorrX(sector, leverArm);
  Float_t corry = parContainer->getCorrY(sector, leverArm);
  Float_t errDiffX = parContainer->getErrDiffX(sector,leverArm);
  Float_t errDiffY = parContainer->getErrDiffY(sector,leverArm);
  Float_t errDiffSlopeX = parContainer->getErrDiffXSlope(sector,leverArm);
  Float_t errDiffSlopeY = parContainer->getErrDiffYSlope(sector,leverArm);
  Float_t alpha = parContainer->getAlpha(sector,leverArm);


  if(errDiffX == 0 || errDiffY == 0 || errDiffSlopeX == 0 || errDiffSlopeY==0){
    Error("execute","Segment finding can not be done with errors = 0. Please, check the parameter container of type HMdcSegmentFPar.");
    return 1;
  }


  if(corrx == 1 || corry == 1 ){
    Error("execute","Segment finding can not be done with correlation = 1.Please, check the parameter container of type HMdcSegmentFPar.");
    return 1;
  }



  HMdcSeg* segment;
  HLocation loc3;
  Int_t segPos=0;
  loc3.set(3,fLoc.getIndex(0),fLoc.getIndex(1),segPos);

  TObject* slot;
  Float_t prob;

  HMdcHit* temp = new HMdcHit;

  HLocation locInd, locInd2;

  iter1=(HMatrixCatIter *)fHitCat->MakeIterator();
  iter1->Reset();
  iter1->gotoLocation(loc1);
  while((hit1=(HMdcHit *)iter1->Next())!=NULL){

    locInd = iter1->getLocation();

    ux = hit1->getXDir();
    uy = hit1->getYDir();

    // transformation of unitary vector into xlope, yslope (old variables)

    Float_t aux = sqrt(1- ux*ux - uy*uy);

    slopex = ux/aux;
    slopey = uy/aux;
      
    point0(0,0) = hit1->getX();
    point0(1,0) = hit1->getY();
    point0(2,0) = 0;               // convention, but it must not be hard/coded
    pointt.Mult(rot,point0);
    for(Int_t i=0; i<3; i++){
      pointt(i,0) += tras(i,0);
    }
      
    slopext = slopex/cosn;
    slopeyt = tan(atan(slopey)-acos(cosn));

    x1 = pointt(0,0) + slopext*(z1- pointt(2,0));
    y1 = pointt(1,0) + slopeyt*(z1- pointt(2,0));
    
       
    // calculate errors in parameters.
    // errorp[i] is the error in point[i]
    // error1[0] is the error in x1
    // error1[1] is the error in y1
    // error1[2] is the error in xslopet
    // error1[3] is the error in yslopet


    Float_t errorp[3], error1[4];         
    errorp[0] = hit1->getErrX();              // error in point[0]
    errorp[1] = cosn * hit1->getErrY();       // error in point[1]
    errorp[2] = sinn * hit1->getErrY();       // error in point[2]
      

    error1[2] = hit1->getErrXDir()/cosn;     
    error1[3] = (hit1->getErrYDir())/((1+slopey*slopey)*
                (1+pow(cos(atan(slopey)-acos(cosn)),2)));       

    // fills a temp hit, being the hit1 in MDC2 reference system

    temp->setX(pointt(0,0), errorp[0]);
    temp->setY(pointt(1,0), errorp[1]);

    Float_t ztemp = pointt(2,0);
    Float_t zerrtemp = errorp[2];

    temp->setXDir(slopext, error1[2]);
    temp->setYDir(slopeyt, error1[3]);
    temp->setChi2(hit1->getChi2());

    Float_t off, erroff;
    hit1->getOff(off, erroff);
    temp->setOff(off, erroff);
    temp->setSecMod(loc1[0], loc1[1]);
    
   

     iter2->gotoLocation(loc2);
    while((hit2=(HMdcHit *)iter2->Next())!=NULL){

      locInd2 = iter2->getLocation();

      Float_t ux2 = hit2->getXDir();
      Float_t uy2 = hit2->getYDir();
      Float_t aux = sqrt(1 - ux2*ux2 - uy2*uy2);

      Float_t xslope2 = ux2/aux;
      Float_t yslope2 = uy2/aux;

      diffSlopeX = xslope2-slopext;
      diffSlopeY = yslope2-slopeyt;
      
      diffX = hit2->getX()-x1;
      diffY = hit2->getY()-y1;
      
      prob = (pow(diffX/errDiffX,2) + pow(diffSlopeX/errDiffSlopeX,2)-
	     2*corrx*fabs(diffX*diffSlopeX)/(errDiffX*errDiffSlopeX))
	     /(1-corrx*corrx) + 
	     (pow(diffY/errDiffY,2) + pow(diffSlopeY/errDiffSlopeY,2) - 
	     2*corry*fabs(diffY*diffSlopeY)/(errDiffY*errDiffSlopeY))
	     /(1-corry*corry);
      hProbEllipse->Fill(prob);


      if(prob <= pow(alpha,4)){

	hDiffXRec->Fill(diffX);
	hDiffYRec->Fill(diffY);
	hDiffSlopeXRec->Fill(diffSlopeX);
	hDiffSlopeYRec->Fill(diffSlopeY);
	    
	hDiffYDiffX->Fill(diffX, diffY);
	hDiffXSlopeDiffX->Fill(diffX,diffSlopeX);
	hDiffYSlopeDiffX->Fill(diffX,diffSlopeY);
	hDiffXSlopeDiffY->Fill(diffY,diffSlopeX);
	hDiffYSlopeDiffY->Fill(diffY,diffSlopeY);
	hDiffXSlopeDiffYSlope->Fill(diffSlopeY,diffSlopeX);
	
	hDiffYDiffXOverErr->Fill(diffX/errDiffX, diffY/errDiffY);
	hDiffXSlopeDiffXOverErr->Fill(diffX/errDiffX,diffSlopeX/errDiffSlopeX);
	hDiffYSlopeDiffXOverErr->Fill(diffX/errDiffX,diffSlopeY/errDiffSlopeY);
	hDiffXSlopeDiffYOverErr->Fill(diffY/errDiffY,diffSlopeX/errDiffSlopeX);
	hDiffYSlopeDiffYOverErr->Fill(diffY/errDiffY,diffSlopeY/errDiffSlopeY);
	hDiffXSlopeDiffYSlopeOverErr->Fill(diffSlopeY/errDiffSlopeY,diffSlopeX/errDiffSlopeX);
	    	    
	loc3[2] = segPos;
	slot = fSegCat->getSlot(loc3);
	if(slot){
	  segment = new(slot) HMdcSeg;
	  fit(temp, ztemp, hit2, 0., segment);
	  segment->setSec(fLoc[0]);
	  segment->setIOSeg(fLoc[1]);
	  segment->setHitInd(0,locInd[2]);
	  segment->setHitInd(1,locInd2[2]);
	  
	  // fill group of cells
	  
	  Int_t nlayers = geoModules->getNLayers();
	  Int_t ncells = 0;
	  for (Int_t i=0; i< nlayers; i++){
	    ncells = hit1->getNCells(i);
	    for (Int_t j=0; j< ncells; j++){
	      segment->setSignId(i, hit1->getCell(i,j), hit1->getSignId(i,j));
	    }
	    ncells = hit2->getNCells(i);
	    for (Int_t j=0; j<ncells; j++){
	      segment->setSignId(i+nlayers, hit2->getCell(i,j), hit2->getSignId(i,j));
	    }
	  }                   // end of filling cells

	  segPos++;
	}
      }                // end of check in prob

    }                         // end of loop in seg2
  }                               // end of loop in mdc1

  fEventId++;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::execute");
#endif
  return 0;
}



void HMdcSegmentF :: fit(HMdcHit* hit1, Float_t z1, HMdcHit *hit2, Float_t z2, HMdcSeg *segment){

  // fits projections x-z and y-z of the two hits in straight lines. Both projections are combined into a 3-D straight line
  // both hits are given in the same coordinate system

  Int_t layers = geoModules->getNLayers();
  Float_t a, b, errora, errorb, chi1, chi2;

  Int_t n1 = 0;
  Int_t n2 = 0;

  for (Int_t i=0; i< layers; i++){
    n1 += hit1->getNCells(i);
    n2 += hit2->getNCells(i);
  }

  Float_t z0 = z2;            


  // fits x-z projection

  Float_t x1 = hit1->getX();
  Float_t x2 = hit2->getX();
  Float_t errorX1 = hit1->getErrX();
  Float_t errorX2 = hit2->getErrY();

  Float_t ux1 = hit1->getXDir();
  Float_t uy1 = hit1->getYDir();
  Float_t aux1 = sqrt(1- ux1*ux1 - uy1*uy1); 

  Float_t ux2 = hit2->getXDir();
  Float_t uy2 = hit2->getYDir();
  Float_t aux2 = sqrt(1 - ux2*ux2 - uy2*uy2);

  Float_t slope1 = ux1/aux1;
  Float_t slope2 = ux2/aux2;

  Float_t errSlope1 = hit1->getErrXDir();
  Float_t errSlope2 = hit2->getErrXDir();
 

  fitProjection(z1, x1, errorX1, slope1, errSlope1, n1, 
                z2, x2, errorX2, slope2, errSlope2, n2,
                a, b, errora, errorb, chi1);

  Float_t xslopeFit = a;
  Float_t errorXslopeFit = errora;
  Float_t xFit = a*z0+b;
  Float_t errorXFit = sqrt(z0*errora*z0*errora+errorb*errorb);

  //  lever->setXSlope(a);
  //lever->setErrorXSlope(errora);
  //lever->setX(a*z0+b);  
  //lever->setErrorX(sqrt(z0*errora*z0*errora+errorb*errorb));

  Float_t diffX1 = a*z0+b - (x1 + slope1*(z2-z1));
  Float_t diffX2 = a*z0+b -x2;

  hFitDiffX->Fill(diffX1);
  hFitDiffX->Fill(diffX2);
  hFitDiffXSlope->Fill(a-slope1);
  hFitDiffXSlope->Fill(a-slope2);
  hFitChiX->Fill(chi1);			     
  
  // fits y-z projection

  Float_t y1 = hit1->getY();
  Float_t y2 = hit2->getY();
  Float_t errorY1 = hit1->getErrY();
  Float_t errorY2 = hit2->getErrY();
  slope1 = uy1/aux1;
  slope2 = uy2/aux2;
  errSlope1 = hit1->getErrYDir();
  errSlope2 = hit2->getErrYDir();
  fitProjection(z1, y1, errorY1, slope1, errSlope1, n1, 
                z2, y2, errorY2, slope2, errSlope2, n2,
                a, b, errora, errorb, chi2);	      


  //lever->setYSlope(a);
  //lever->setErrorYSlope(errora);
  //lever->setY(a*z0+b);  
  //lever->setErrorY(sqrt(z0*errora*z0*errora+errorb*errorb));  

  Float_t yslopeFit = a;
  Float_t errorYslopeFit = errora;
  Float_t yFit = a*z0+b;
  Float_t errorYFit = sqrt(z0*errora*z0*errora+errorb*errorb);

  // Find the unitary vector 

  Float_t mod = sqrt(xslopeFit*xslopeFit + yslopeFit*yslopeFit +1);
  TMatrix u(3,1);
  u(0,0) = xslopeFit / mod;
  u(1,0) = yslopeFit / mod;
  u(2,0) = 1. / mod;


  // Transform into sector reference system 
  Int_t sector = fLoc[0];
  Int_t leverArm = fLoc[1];


  Float_t tx,ty,tz,cosn,sinn;
  tx = geoLever->getTrasElementS(sector,leverArm,0);
  ty = geoLever->getTrasElementS(sector,leverArm,1);
  tz = geoLever->getTrasElementS(sector,leverArm,2);

  cosn = geoLever->getCosS(sector,leverArm);
  sinn = geoLever->getSinS(sector,leverArm);


  TMatrix rot(3,3);
  rot(0,0) = 1;
  rot(0,1) = 0;
  rot(0,2) = 0;
  rot(1,0) = 0;
  rot(1,1) = cosn;
  rot(1,2) = sinn;
  rot(2,0) = 0;
  rot(2,1) = -sinn;
  rot(2,2) = cosn;

  TMatrix tras(3,1);
  tras(0,0) = tx;
  tras(1,0) = ty;
  tras(2,0) = tz;

  TMatrix point0(3,1);
  TMatrix pointt(3,1);

  //point0(0,0) = hit1->getX();
  //point0(1,0) = hit1->getY();
  point0(0,0) = xFit;
  point0(1,0) = yFit;
  point0(2,0) = 0;               // convention, but it must not be hard/coded
  pointt.Mult(rot,point0);
  for(Int_t i=0; i<3; i++){
    pointt(i,0) += tras(i,0);
  }

  TMatrix ut(3,1);               // u in sector RS
  ut.Mult(rot,u);

  // r, z, theta, phi
  Float_t phi, theta;

  if (ut(0,0) != 0) phi = atan(ut(1,0)/ut(0,0));
  else phi = TMath::Pi()/4.;

  Float_t cosphi = cos(phi);
  if (cosphi != 0) theta = asin(ut(0,0)/cosphi);
  else {
    Float_t sinphi = sin(phi);
    theta = asin(ut(1,0)/sinphi);
  }
  
  Float_t errut2 = sqrt((ut(0,0)*ut(0,0)*errorXslopeFit*errorXslopeFit + 
			 ut(1,0)*ut(1,0)*errorYslopeFit*errorYslopeFit)/
			(1-ut(0,0)*ut(0,0) - ut(1,0)*ut(1,0)));
  Float_t errut[3];
  errut[0] = errorXslopeFit;
  errut[1] = sqrt(cosn*cosn * errorYslopeFit* errorYslopeFit +
		  sinn*sinn * errut2*errut2);
  errut[2] = sqrt(sinn*sinn * errorYslopeFit * errorYslopeFit +
		  cosn*cosn * errut2*errut2);

  Float_t errphi, errtheta;

  errphi = sqrt((pow(ut(1,0)*errut[0]/ut(0,0)*ut(0,0),2) +
		 pow(errut[1]/ut(0,0),2))/pow(1+pow(ut(1,0)/ut(0,0),2),2));
  errtheta = (pow(errut[1]/cosphi,2)+pow(ut(1,0)*sin(phi)*errphi/
	      cosphi*cosphi,2))/(1-pow(ut(1,0)/cosphi,2));

  segment->setPhi(phi, errphi);
  segment->setTheta(theta, errtheta);

  Float_t z =0,r=0, errZ=0, errR=0;

  // Evaluate x,y in plane z=0 in sector reference system

  Float_t x0, y0, errorX0, errorY0;

  x0 = pointt(0,0) - pointt(2,0)*ut(0,0)/ut(2,0);
  y0 = pointt(1,0) - pointt(2,0)*ut(1,0)/ut(2,0);

  errorX0 = errorXFit;
  errorY0 = errorYFit;

  r = y0 * cosphi - x0 * sin(phi);

  errZ = sqrt( pow((cos(phi) * errorY0),2) +
	       pow((sin(phi) * errorX0),2) +
	       pow(((y0 * sin(phi) + x0*cos(phi)) * errphi),2));
  
  if ( sin(theta)!=0) {
    z = y0 * sin(phi) * cos(theta) / sin(theta) -
      x0 * cosphi;

    errR = sqrt(pow(cos(phi) * errorX0,2) +
		pow((y0 * cos(phi) * cos(theta)/sin(theta) + 
		     x0*sin(phi))*errphi,2) +
		pow((y0/sin(theta) * errtheta),2));
		
  }

	      

  segment->setZ(z, errZ);
  segment->setR(r,errR);
  segment->setChi2(chi1+chi2);

  Float_t diffY1 = a*z0+b - (y1 + slope1*(z2-z1));
  Float_t diffY2 = a*z0+b -y2;

  hFitDiffY->Fill(diffY1);
  hFitDiffY->Fill(diffY2);
  hFitDiffYSlope->Fill(a-slope1);
  hFitDiffYSlope->Fill(a-slope2);
  hFitChiY->Fill(chi2);		
  hFitChi->Fill(chi1+chi2);

#if DEBUG_LEVEL>3
  ofstream file("fit.txt",ios::app); 
  seg1->print(file);
  file << endl;
  seg2->print(file);
  file << endl;
  file << lever->getX() << " +- " << lever->getErrorX() << endl;
  file << lever->getXSlope() << " +- " << lever->getErrorXSlope() << endl;
  file << lever->getY() << " +- " << lever->getErrorY() << endl;
  file << lever->getYSlope() << " +- " << lever->getErrorYSlope() << endl;
  file << endl;
  file << "chi1 " << chi1 << endl;
  file << "chi2 " << chi2 << endl;
  file << "chi " << chi1 + chi2 << endl;
  file << endl;
#endif
}


void HMdcSegmentF :: fitProjection( Float_t z1, Float_t x1, Float_t errorX1, 
                                 Float_t slope1, Float_t errSlope1, Int_t n1, 
                                     Float_t z2, Float_t x2, Float_t errorX2, 
                                 Float_t slope2, Float_t errSlope2, Int_t n2,
                                 Float_t &a, Float_t &b, Float_t &errora, 
                                 Float_t &errorb, Float_t &chi){

  Float_t s, sz, szz, sx, szx, delta;
  
  Float_t x1z2 = x1 + slope1*(z2-z1);
  Float_t x2z1 = x2 + slope2*(z1-z2);

  Float_t sigma1 = errorX1*errorX1;
  Float_t sigma2 = sigma1 + pow((z2-z1)*errSlope1,2);
  Float_t sigma4 = errorX2*errorX2;
  Float_t sigma3 = sigma4 + pow((z2-z1)*errSlope2,2);

  s = n1*(1./sigma1 + 1./sigma2) + n2*(1./sigma3 + 1./sigma4);
  sz = n1*(z1/sigma1 + z2/sigma2) + n2*(z1/sigma3+ z2/sigma4);
  szz = n1*(z1*z1/sigma1 + z2*z2/sigma2) + n2*(z1*z1/sigma3 + z2*z2/sigma4);

  sx = n1*(x1/sigma1 + x1z2/sigma2) + n2*(x2z1/sigma3 + x2/sigma4);
  szx = n1*(z1*x1/sigma1 + z2*x1z2/sigma2) + n2*(z1*x2z1/sigma3 + z2*x2/sigma4);

  delta = s*szz - sz*sz;


  // results of the fit

  a = (s*szx - sz*sx)/delta;
  b = (sx*szz - sz*szx)/delta; 

  Float_t dist1 = a*z1+b-x1;
  Float_t dist2 = a*z2+b-x1z2;
  Float_t dist3 = a*z1+b-x2z1;
  Float_t dist4 = a*z2+b-x2;



  //  chi = (n1*(dist1*dist1/sigma1 + dist2*dist2/sigma2) +
  //        n2*(dist3*dist3/sigma3 + dist4*dist4/sigma4))/(n1+n2);

  chi = (n1*(dist1*dist1 + dist2*dist2) +
         n2*(dist3*dist3 + dist4*dist4))/(n1+n2);
#if DEBUG_LEVEL>3
  ofstream file("fit.txt",ios::app);
  file << "dist1 " << dist1 << " +- " << sigma1<< endl;
  file << "dist2 " << dist2 << " +- " << sigma2<< endl;
  file << "dist2 " << dist3 << " +- " << sigma3<< endl;
  file << "dist2 " << dist4 << " +- " << sigma4<< endl;
  file << "chi " << chi << endl;
#endif
  // derivatives and errors

  Float_t dsxdx1 = n1*(1./sigma1 + 1./sigma2); //derivative of sx respect to x1
  Float_t dsxdx2 = n2*(1./sigma3 + 1./sigma4);
  Float_t dsxdslope1 = n1*(z2-z1)/sigma2;
  Float_t dsxdslope2 = n2*(z1-z2)/sigma3;
  Float_t dszxdx1 = n1*(z1/sigma1 + z2/sigma2);
  Float_t dszxdx2 = n2*(z1/sigma3 + z2/sigma4);
  Float_t dszxdslope1 = n1*z2*(z2-z1)/sigma2;
  Float_t dszxdslope2 = n2*z1*(z1-z2)/sigma3;

  Float_t dadx1 = (s*dszxdx1 - sz*dsxdx1)/delta;
  Float_t dadx2 = (s*dszxdx2 - sz*dsxdx2)/delta;
  Float_t dadslope1 = (s*dszxdslope1 - sz*dsxdslope1)/delta;
  Float_t dadslope2 = (s*dszxdslope2 - sz*dsxdslope2)/delta;
  
  Float_t dbdx1 = (szz*dsxdx1 - sz*dszxdx1)/delta;
  Float_t dbdx2 = (szz*dsxdx2 - sz*dszxdx2)/delta;
  Float_t dbdslope1 = (szz*dsxdslope1 - sz*dszxdslope1)/delta;
  Float_t dbdslope2 = (szz*dsxdslope2 - sz*dszxdslope2)/delta;

  errora = sqrt(pow(dadx1*errorX1,2) + pow(dadx2*errorX2,2) + 
                pow(dadslope1*errSlope1,2) + pow(dadslope2*errSlope2,2));

  errorb = sqrt(pow(dbdx1*errorX1,2) + pow(dbdx2*errorX2,2) + 
                pow(dbdslope1*errSlope1,2) + pow(dbdslope2*errSlope2,2));

}





// Borrar para entrega oficial
#if 0
void HMdcSegmentF :: findParameters(void){
   
  // This function takes the category of segments in both modules
  // A segment in the first module is transformed into the reference system
  // of the second module. Then, both segments in the same reference system
  // are compared. They are compatible if the difference in the x,y 
  // coordinates in the first plane and the difference between slopes is 
  // smaller than a given road 

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcSegmentF::findParameters");
#endif

  Int_t sector = fLoc[0];
  Int_t leverArm  = fLoc[1];
  Int_t module1,module2;
  if(leverArm == 0) module1=0;
  else module1=2;

  module2 = module1+1;


  HLocation loc1,loc2;
  loc1.set(2,sector,module1);
  loc2.set(2,sector,module2);

  HMdcHitAux* seg1,*seg2;
  Float_t x1,y1,z1,slopext,slopeyt;
  Float_t slopex,slopey;
  Float_t tx,ty,tz,cosn,sinn;
  tx = geoLever->getTrasElement(sector,leverArm,0);
  ty = geoLever->getTrasElement(sector,leverArm,1);
  tz = geoLever->getTrasElement(sector,leverArm,2);

  cosn = geoLever->getCosn(sector,leverArm);
  sinn = geoLever->getSinn(sector,leverArm);

  TMatrix rot(3,3);
  rot(0,0) = 1;
  rot(0,1) = 0;
  rot(0,2) = 0;
  rot(1,0) = 0;
  rot(1,1) = cosn;
  rot(1,2) = sinn;
  rot(2,0) = 0;
  rot(2,1) = -sinn;
  rot(2,2) = cosn;

  TMatrix tras(3,1);
  tras(0,0) = tx;
  tras(1,0) = ty;
  tras(2,0) = tz;

  Float_t diffX,diffY,diffSlopeX,diffSlopeY;
  Float_t maxDiffX,maxDiffY,maxDiffSlopeX,maxDiffSlopeY;
  maxDiffX = 5.;
  maxDiffY = 1.;
  maxDiffSlopeX = 1.;
  maxDiffSlopeY = 1.;

  
  TMatrix point0(3,1);
  TMatrix pointt(3,1);
  
  // get the z coordinate where hits are going to be compared (z coordinate of hits in second module)

  z1=0;

  HMdcLeverArm* lever;
  HLocation loc3;
  Int_t leverPos=0;
  loc3.set(3,fLoc.getIndex(0),fLoc.getIndex(1),leverPos);

  HMatrixCatIter *iter1 = 0, *iter2 = 0;
  iter1 = (HMatrixCatIter*)fHitCat->MakeIterator();
  iter1->Reset();
  iter1->gotoLocation(loc1);

  iter2 = (HMatrixCatIter*)fHitCat->MakeIterator();
  iter2->Reset();

  #if DEBUG_LEVEL>3
  ofstream file("test.txt",ios::app);
  file << "event " << fEventId << endl;
  file << "sector " << sector << " module1 " << module1 << " module2 " << module2 << endl;
  #endif
  
  TObject* slot;
  
  while((seg1 = (HMdcHitAux*)iter1->Next())!=NULL){

    seg1->print(file);

    slopex = seg1->getSlopeX();
    slopey = seg1->getSlopeY();
      
    point0(0,0) = seg1->getPointX();
    point0(1,0) = seg1->getPointY();
    point0(2,0) = seg1->getPointZ();
    pointt.Mult(rot,point0);
    for(Int_t i=0; i<3; i++){
      pointt(i,0) += tras(i,0);
    }

    slopext = slopex/cosn;
    slopeyt = tan(atan(slopey)-acos(cosn));
    
    x1 = pointt(0,0) + slopext*(z1- pointt(2,0));
    y1 = pointt(1,0) + slopeyt*(z1- pointt(2,0));
    
    #if DEBUG_LEVEL>3
    file << "pointt: " << pointt(0,0) << '\t' << pointt(1,0) << '\t' << pointt(2,0) << endl;
    file << "slopext " << slopext << "  slopeyt " << slopeyt << endl;
    file << "x1 " << x1 << " y1 " << y1 << endl;
    #endif

    iter2 -> gotoLocation(loc2);
    while((seg2 = (HMdcHitAux*)iter2->Next())!=NULL){

      seg2->print(file);
      diffSlopeX = seg2->getSlopeX()-slopext;
      diffSlopeY = seg2->getSlopeY()-slopeyt;
      
      #if DEBUG_LEVEL>3
      file << "diffslopex " << diffSlopeX << "  diffslopey " << diffSlopeY << endl;
      #endif

      hDiffSlopeX->Fill(diffSlopeX);
      hDiffSlopeY->Fill(diffSlopeY);
      
      if(fabs(diffSlopeX) <= maxDiffSlopeX){     
	if(fabs(diffSlopeY) <= maxDiffSlopeY){ 
	  
	  diffX = seg2->getPointX()-x1;
	  diffY = seg2->getPointY()-y1;

	  #if DEBUG_LEVEL>3
	  file << "diffX " << diffX << "  diffY " << diffY << endl;
	  #endif

	  hDiffX->Fill(diffX);
	  hDiffY->Fill(diffY);
	  
	  if(fabs(diffX) <= maxDiffX &&  fabs(diffY) <= maxDiffY){
	    
	    #if DEBUG_LEVEL>3
	    file << "bingo!"<< endl;
	    #endif

	    hDiffXRec->Fill(diffX);
	    hDiffYRec->Fill(diffY);
	    hDiffSlopeXRec->Fill(diffSlopeX);
	    hDiffSlopeYRec->Fill(diffSlopeY);
	    
	    loc3[2] = leverPos;
	    slot = fSegCat->getSlot(loc3);
	    if(slot){
	      lever = new(slot) HMdcLeverArm;
	      lever->addSegments(seg1,seg2);
	      leverPos++;
	    }
	  }                   // end of check coord difference
	}                     // end of checking y slopes
      }                       // end of checking x slopes
    }                         // end of loop in seg2
  }                           // end of loop in mdc1



#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcSegmentF::findParameters");
#endif

}

#endif








