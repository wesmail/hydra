# include <math.h>
# include <fstream.h>
# include "TMath.h"
# include "hades.h"
# include "hdebug.h"
# include "hdetector.h"
# include "hevent.h"
# include "hlocation.h"
# include "hmatrixcatiter.h"
# include "hmdccal3.h"
# include "hmdccal3sim.h"
# include "hmdcdetector.h"
# include "hreconstructor.h"
# include "hmdcgeomstruct.h"
# include "hmdchitaux.h"
# include "hmdchitauxsim.h"
# include "hmdchitcomp.h"
# include "hmdcmodulegeometry.h"
# include "hmdctargetgeometry.h"
# include "hruntimedb.h"
# include "hspectrometer.h"
# include "mdcsdef.h"

ClassImp(HMdcHitComp)
     


//*-- Author : Beatriz Fuentes Arenaz
//*-- Modified: 27/9/2000 by Beatriz Fuentes (new data levels)
//*-- Modified: 25/2/2000 by Beatriz Fuentes (units in histos: cm --> mm)
//*-- Modified : 14/1/2000 by Beatriz Fuentes
//*-- Date: 11/98
//*-- Copyright : GENP (Univ. Santiago de Compostela)


  /////////////////////////////////////////////////////////////////////////
  //
  // HMdcHitComp
  // This reconstructor is only for internal use of MDC Santiago group.
  // It compares input and output of the trackig algorithm in one module.
  // Please, note that input data for the hit finding and fitting 
  // algorithm are HMdcCal3 data, and the direct output are 
  // HMdcHitAux data. Official HMdcCal2 data are converted into HMdcCal3
  // before the hit finding, and once it is finished, HMdcHitAux are 
  // transformed into the official HMdcHit.
  // It presents some statistics and histograms.
  //
  /////////////////////////////////////////////////////////////////////////


               
HMdcHitComp :: HMdcHitComp(void) : HReconstructor(){
  // default constructor

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitComp::HMdcHitComp()");
#endif

  fRecCat = NULL;
  fSimCat = NULL;
  fCal3Cat = NULL;
  fLoc.setNIndex(2);
  fEventId = 0;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitComp::HMdcHitComp()");
#endif
}

HMdcHitComp :: HMdcHitComp(Text_t* name, Text_t* title) :                                 HReconstructor(name, title){
  // default constructor

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitComp::HMdcHitComp(name,title)");
#endif

  fRecCat = NULL;
  fSimCat = NULL;
  fCal3Cat = NULL;
  fLoc.setNIndex(2);
  fEventId = 0;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitComp::HMdcHitComp(name,title)");
#endif
}




Bool_t HMdcHitComp :: init(void){

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitComp::init");
#endif

  fRecCat = gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (!fRecCat) {
    fRecCat=gHades->getSetup()->getDetector("Mdc")->buildCategory(catMdcHit);
    if (!fRecCat) return kFALSE;
    else gHades->getCurrentEvent()->addCategory(catMdcHit,fRecCat,"Mdc");
  }



  fCal3Cat = gHades->getCurrentEvent()->getCategory(catMdcCalHit);
  if (!fCal3Cat){
    fCal3Cat = NULL;
    printf("HMdcHitComp can not work without a cal3 category\n");
  }

  fSimCat = gHades->getCurrentEvent()->getCategory(catMdcSimSegment);
  if(!fSimCat){

    TArrayI* ind = new TArrayI(4);
    HMdcGeomStruct* p=
      (HMdcGeomStruct*)gHades->getRuntimeDb()->getContainer("MdcGeomStruct");
    if (!p) return 0;
    p->getMaxIndices(ind);
    Int_t nSizes = ind->GetSize();
    Int_t* sizes = new Int_t[nSizes-1];
    for (Int_t i=0;i<nSizes-2;i++) sizes[i] = ind->At(i)+1;
    sizes[nSizes-2] = 1200;  

    fSimCat = new HMatrixCategory("HMdcHitAux",nSizes-1,sizes,0.5);
    fSimCat->setPersistency(kFALSE);
    gHades->getCurrentEvent()->addCategory(catMdcSimSegment,fSimCat,"Mdc");
    delete [] sizes;
    
  }

  setParContainers();
  bookHisto();  

  fActive=kTRUE;
#if DEBUG_LEVEL>2
      gDebuger->leaveFunc("HMdcHitComp::init");
#endif


  return kTRUE;
}


void HMdcHitComp :: setParContainers(void){

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitComp::setParContainers()");
#endif

  geo=(HMdcModuleGeometry*)gHades->getRuntimeDb()->getContainer("MdcModuleGeometry");
  if(!geo){
    geo=new HMdcModuleGeometry;
    gHades->getRuntimeDb()->addContainer(geo);
  }


   target=(HMdcTargetGeometry*)gHades->getRuntimeDb()->getContainer("MdcTargetGeometry");
  if(!target){
     target=new HMdcTargetGeometry;
     gHades->getRuntimeDb()->addContainer(target);
}

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitComp::setParContainers()");
#endif

}



void HMdcHitComp :: bookHisto(void){
  // book histograms 

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitComp::bookHisto()");
#endif

    Int_t layers = ((HMdcDetector* )(gHades->getSetup()->getDetector("Mdc")))->getMaxComponents(); 

    char hname[100];
    char htitle[100];

    fHistograms = new TList;
    hCoord = new TH1F*[layers];
    hx     = new TH1F*[layers];
    hy     = new TH1F*[layers];
    hres   = new TH1F*[layers];

  n6to6=0, n6to5=0, n6to4=0;
  n5to5 = n5to4 = 0; 
  n4to4 = 0;
  norec6=0, norec5=0, norec4=0;
  ghost6=0, ghost5=0, ghost4=0;
  mix6to5 = mix6to4 = mix5to4 = 0;
  n5to6 = n4to5 = n4to6 =0;
  nseg6 = nseg5= nseg4 =0;
  nsim6 = nsim5 = nsim4 = nsim3=nsim2=nsim1=0;
  mix6to4plus1= 0;

  fn6to6 = 0, fn6to5 = 0, fn6to4 = 0;
  fn5to5 = fn5to4 = 0;
  fn4to4 = 0;
  fmix6to5 = fmix6to4 = fmix5to4 = 0;
  fn5to6 = fn4to5 = fn4to6 =0;
  fmix6to4plus1= 0;

  for(Int_t i=0; i< layers; i++){
    sprintf(hname,"hCoord_%d%d%d",fLoc[0],fLoc[1],i);
    sprintf(htitle,"coordinate in plane %d",i);
    hCoord[i] = new TH1F(hname, htitle, 100, -750, 750);
    hCoord[i]->SetXTitle("coordinate (mm)");
    fHistograms->AddLast(hCoord[i]);


    sprintf(hname,"hx_%d%d%d",fLoc[0],fLoc[1],i);
    sprintf(htitle,"x distribution in plane %d, output",i);
    hx[i] = new TH1F(hname, htitle, 100, -600, 600);
    hx[i]->SetXTitle("x (mm)");
    fHistograms->AddLast(hx[i]);


    sprintf(hname,"hy_%d%d%d",fLoc[0],fLoc[1],i);
    sprintf(htitle,"y distribution in plane %d, output",i);
    hy[i] = new TH1F(hname, htitle, 100, -800, 800);
    hy[i]->SetXTitle("y (mm)");
    fHistograms->AddLast(hy[i]);
    
    }

  sprintf(hname,"hNHits_%d%d",fLoc[0],fLoc[1]);
  hNHits = new TH1F(hname, "number of hits per track", 100, 0, 11);
  hNHits->SetXTitle("number of hits");
  
  sprintf(hname,"hxSlope_%d%d",fLoc[0],fLoc[1]);
  hxSlope = new TH1F(hname, "xslope distribution, output", 100, -1.2, 1.2);
  hxSlope->SetXTitle("xslope");

  sprintf(hname,"hySlope_%d%d",fLoc[0],fLoc[1]);
  hySlope = new TH1F(hname, "yslope distribution, output", 100, -1.2, 1.2);
  hySlope->SetXTitle("yslope");

  sprintf(hname,"hRecHits_%d%d",fLoc[0],fLoc[1]);
  hRecHits = new TH1F(hname, "number of hits per track, output", 100, 0, 11);
  hRecHits->SetXTitle("number of hits");

  sprintf(hname,"hChi_%d%d",fLoc[0],fLoc[1]);
  hChi = new TH1F(hname, "chi2",100,-0.001, 5.);
  hChi->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hChi6_%d%d",fLoc[0],fLoc[1]);
  hChi6 = new TH1F(hname, "chi2 of segments with 6 hits",100,-.001, 5.);
  hChi6->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hChi5_%d%d",fLoc[0],fLoc[1]);
  hChi5 = new TH1F(hname, "chi2 of segments with 5 hits",100,-.001, 5.); 
  hChi5->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hTracks_%d%d",fLoc[0],fLoc[1]);
  hTracks = new TH1F(hname, "number of reconstructed tracks per event", 100, 0., 100.);
  hTracks->SetXTitle("number of tracks");

  sprintf(hname,"hChi6to6_%d%d",fLoc[0],fLoc[1]);
  hChi6to6 = new TH1F(hname, "chi2 of 6hit-segments reconstructed with 6 hits",100,-.001,5.);  
  hChi6to6->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hChi6to5_%d%d",fLoc[0],fLoc[1]);
  hChi6to5 = new TH1F(hname, "chi2 of 6hit-segments reconstructed with 5 hits",100,-.001,5.);
  hChi6to5->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hChi5to5_%d%d",fLoc[0],fLoc[1]);
  hChi5to5 = new TH1F(hname, "chi2 of 5hit-segments reconstructed with 5 hits",100,-.001,5.);
  hChi5to5->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hChiGhost6_%d%d",fLoc[0],fLoc[1]);
  hChiGhost6 = new TH1F(hname, "chi2 of ghosts with 6 hits",100,-1.,5.);
  hChiGhost6->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hChiGhost5_%d%d",fLoc[0],fLoc[1]);
  hChiGhost5 = new TH1F(hname, "chi2 of ghosts with 5 hits",100,-.001,5.);
  hChiGhost5->SetXTitle("chi2 per degree of freedom");

  sprintf(hname,"hProb_%d%d",fLoc[0],fLoc[1]);
  hProb = new TH1D(hname, "prob of segments", 100, 0,1.2);
  hProb->SetXTitle("probability");

  sprintf(hname,"hProb6_%d%d",fLoc[0],fLoc[1]);
  hProb6 = new TH1D(hname, "prob of segments with 6 hits",100,0,1.2);
  hProb6->SetXTitle("probability");

  sprintf(hname,"hProb5_%d%d",fLoc[0],fLoc[1]);
  hProb5 = new TH1D(hname, "prob of segments with 5 hits",100,0,1.2);
  hProb5->SetXTitle("probability");

  sprintf(hname, "hres_%d%d%d",fLoc[0],fLoc[1],0);
  hres[0] = new TH1F(hname,"residues in plane 0",100,-0.1,0.1);
  sprintf(hname, "hres_%d%d%d",fLoc[0],fLoc[1],1);
  hres[1] = new TH1F(hname,"residues in plane 1",100,-0.2,0.2);
  sprintf(hname, "hres_%d%d%d",fLoc[0],fLoc[1],2);
  hres[2] = new TH1F(hname,"residues in plane 2",100,-0.3,0.3);
  sprintf(hname, "hres_%d%d%d",fLoc[0],fLoc[1],3);
  hres[3] = new TH1F(hname,"residues in plane 3",100,-0.3,0.3);
  sprintf(hname, "hres_%d%d%d",fLoc[0],fLoc[1],4);
  hres[4] = new TH1F(hname,"residues in plane 4",100,-0.2,0.02);
  sprintf(hname, "hres_%d%d%d",fLoc[0],fLoc[1],5);
  hres[5] = new TH1F(hname,"residues in plane 5",100,-0.1,0.1); 

  for(Int_t i=0; i< layers; i++) hres[i]->SetXTitle("x(real)-x(rec) (mm)");

  sprintf(hname, "hYX_%d%d", fLoc[0],fLoc[1]);
  hYX = new TH2F(hname, "x-y of reconstructed segments",100,-600,600,100,-800,800);
  hYX->SetXTitle("x(mm)");
  hYX->SetYTitle("y(mm)");

  sprintf(hname, "hXslopeX_%d%d", fLoc[0],fLoc[1]);
  hXslopeX = new TH2F(hname, "xslope-x of reconstructed segments",100,-600,600,100,-1.2,1.2);
  hXslopeX->SetYTitle("xslope");
  hXslopeX->SetXTitle("x (mm)");

  sprintf(hname, "hYslopeY_%d%d", fLoc[0],fLoc[1]);
  hYslopeY = new TH2F(hname, "yslope-y of reconstructed segments",100,-800,800,100,-1.2,1.2);
  hYslopeY->SetYTitle("yslope");
  hYslopeY->SetXTitle("y (mm)");

  sprintf(hname, "hXslopeYslope_%d%d", fLoc[0],fLoc[1]);
  hXslopeYslope = new TH2F(hname, "yslope-xslope of reconstructed segments",100,-1.2,1.2,100,-1.2,1.2);
  hXslopeYslope->SetXTitle("yslope");
  hXslopeYslope->SetYTitle("xslope");

  sprintf(hname, "hDistanceToTarget_%d%d", fLoc[0],fLoc[1]);
  hDistanceToTarget = new TH1F(hname, "distance between segments and (0,0,0)",100,0,150);
  hDistanceToTarget->SetXTitle("distance (mm)");

  sprintf(hname, "hDistanceToTargetWellRec_%d%d", fLoc[0],fLoc[1]);
  hDistanceToTargetWellRec = new TH1F(hname, "distance between segments and (0,0,0) for well rec. segments",100,0,150);
  hDistanceToTargetWellRec->SetXTitle("distance (mm)");

  sprintf(hname, "hDistanceToTargetNoWR_%d%d", fLoc[0],fLoc[1]);
  hDistanceToTargetNoWR = new TH1F(hname, "distance between segments and (0,0,0) for not well rec. segments",100,0,150);
  hDistanceToTargetNoWR->SetXTitle("distance (mm)");

  sprintf(hname, "hDistanceToTargetOverErr_%d%d", fLoc[0],fLoc[1]);
  hDistanceToTargetOverErr = new TH1F(hname, "distance between segments and (0,0,0) over error",100,0,10000);
  hDistanceToTargetOverErr->SetXTitle("distance/err");

  sprintf(hname, "hDistanceToTargetOverErrWellRec_%d%d", fLoc[0],fLoc[1]);
  hDistanceToTargetOverErrWellRec = new TH1F(hname, "distance between segments and (0,0,0) over error for well rec. segments",100,0,10000);
  hDistanceToTargetOverErrWellRec->SetXTitle("distance/err");

  sprintf(hname, "hDistanceToTargetOverErrNoWR_%d%d", fLoc[0],fLoc[1]);
  hDistanceToTargetOverErrNoWR = new TH1F(hname, "distance between segments and (0,0,0) over error for not well rec. segments",100,0,10000);
  hDistanceToTargetOverErrNoWR->SetXTitle("distance/err");


  sprintf(hname, "hDistanceToZ_%d%d", fLoc[0],fLoc[1]);
  hDistanceToZ = new TH1F(hname, "distance between segments and z axis (lab)",100,0,150);
  hDistanceToZ->SetXTitle("distance (mm)");

 sprintf(hname, "hDistanceToZWellRec_%d%d", fLoc[0],fLoc[1]);
  hDistanceToZWellRec = new TH1F(hname, "distance between segments and z axis (lab) for well rec. tracks",100,0,150);
  hDistanceToZWellRec->SetXTitle("distance (mm)");

 sprintf(hname, "hDistanceToZNoWR_%d%d", fLoc[0],fLoc[1]);
  hDistanceToZNoWR = new TH1F(hname, "distance between segments and z axis (lab) for not well rec. tracks",100,0,150); 
  hDistanceToZNoWR->SetXTitle("distance (mm)");

  sprintf(hname, "hDistanceToZOverErr_%d%d", fLoc[0],fLoc[1]);
  hDistanceToZOverErr = new TH1F(hname, "distance between segments and z axis over error",100,0,1000);
  hDistanceToZOverErr->SetXTitle("distance/(error in distance)");
  
  sprintf(hname, "hDistanceToZOverErrWellRec_%d%d", fLoc[0],fLoc[1]);
  hDistanceToZOverErrWellRec = new TH1F(hname, "distance between segments and z axis over error for well rec. tracks",100,0,1000);
  hDistanceToZOverErrWellRec->SetXTitle("distance/(error in distance)");

  sprintf(hname, "hDistanceToZOverErrNoWR_%d%d", fLoc[0],fLoc[1]);
  hDistanceToZOverErrNoWR = new TH1F(hname, "distance between segments and z axis over error for not well rec. tracks",100,0,1000);
  hDistanceToZOverErrNoWR->SetXTitle("distance/(error in distance)");

  sprintf(hname, "hDiffRecSimXSlopeOverErr_%d%d", fLoc[0],fLoc[1]);
  hDiffRecSimXSlopeOverErr = new TH1F(hname, " (xslope rec - xslope real)/error xslope rec",100,-5,5);
  hDiffRecSimXSlopeOverErr->SetXTitle("(xslope rec - xslope real)/error xslope rec");

  sprintf(hname, "hDiffRecSimYSlopeOverErr_%d%d", fLoc[0],fLoc[1]);
  hDiffRecSimYSlopeOverErr = new TH1F(hname, " (yslope rec - yslope real)/error yslope rec",100,-5,5);
  hDiffRecSimYSlopeOverErr->SetXTitle("(yslope rec - yslope real)/error yslope rec");


  sprintf(hname, "hDiffRecSimX_%d%d", fLoc[0],fLoc[1]);
  hDiffRecSimX = new TH1F(hname, "x rec - x real",100,-50,50);
  hDiffRecSimX->SetXTitle("x rec - x real (mm)");
  
  sprintf(hname, "hDiffRecSimY_%d%d", fLoc[0],fLoc[1]);
  hDiffRecSimY = new TH1F(hname, "y rec - y real",100,-50,50);
  hDiffRecSimY->SetXTitle("y rec - y real (mm)");

  sprintf(hname, "hDiffRecSimXSlope_%d%d", fLoc[0],fLoc[1]);
  hDiffRecSimXSlope = new TH1F(hname, "xslope rec - xslope real",100,-5,5);
  hDiffRecSimXSlope->SetXTitle("xslope rec - xslope real");

  sprintf(hname, "hDiffRecSimYSlope_%d%d", fLoc[0],fLoc[1]);
  hDiffRecSimYSlope = new TH1F(hname, "yslope rec - yslope real",100,-5,5);
  hDiffRecSimYSlope->SetXTitle("yslope rec - yslope real");

  // add histograms to TList* fHistograms (see HReconstructor)
  //  fHistograms = new TList;

fHistograms->AddLast(hNHits);
fHistograms->AddLast(hxSlope);
fHistograms->AddLast(hySlope);
fHistograms->AddLast(hRecHits);
fHistograms->AddLast(hTracks);
fHistograms->AddLast(hChi);
fHistograms->AddLast(hChi6);
fHistograms->AddLast(hChi5);
fHistograms->AddLast(hChi6to6);
fHistograms->AddLast(hChi6to5);
fHistograms->AddLast(hChi5to5);
fHistograms->AddLast(hChiGhost6);
fHistograms->AddLast(hChiGhost5);
fHistograms->AddLast(hProb);
fHistograms->AddLast(hProb6);
fHistograms->AddLast(hProb5);
fHistograms->AddLast(hres[0]);
fHistograms->AddLast(hres[1]);
fHistograms->AddLast(hres[2]);
fHistograms->AddLast(hres[3]);
fHistograms->AddLast(hres[4]);
fHistograms->AddLast(hres[5]);
fHistograms->AddLast(hYX);
fHistograms->AddLast(hXslopeX);
fHistograms->AddLast(hYslopeY);
fHistograms->AddLast(hXslopeYslope);
fHistograms->AddLast(hDistanceToTarget);
fHistograms->AddLast(hDistanceToTargetOverErr);
fHistograms->AddLast(hDistanceToZ);
fHistograms->AddLast(hDistanceToZOverErr);
fHistograms->AddLast(hDistanceToTargetWellRec);
fHistograms->AddLast(hDistanceToTargetOverErrWellRec);
fHistograms->AddLast(hDistanceToZWellRec);
fHistograms->AddLast(hDistanceToZOverErrWellRec);
fHistograms->AddLast(hDistanceToTargetNoWR);
fHistograms->AddLast(hDistanceToTargetOverErrNoWR);
fHistograms->AddLast(hDistanceToZNoWR);
fHistograms->AddLast(hDistanceToZOverErrNoWR);
fHistograms->AddLast(hDiffRecSimXSlopeOverErr);
fHistograms->AddLast(hDiffRecSimYSlopeOverErr);
fHistograms->AddLast(hDiffRecSimX);
fHistograms->AddLast(hDiffRecSimY);
fHistograms->AddLast(hDiffRecSimXSlope);
fHistograms->AddLast(hDiffRecSimYSlope); 


#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitComp::bookHisto()");
#endif 
}


void HMdcHitComp :: setLoc(HLocation& location){
  // sets location for this reconstructor
  fLoc.setNIndex(2);
  fLoc.readIndexes(location);
}


HMdcHitComp::~HMdcHitComp(void){
  // default destructor: prints statistics to standard output

#if DEBUG_LEVEL>2
  gDebuger->enterFunc("HMdcHitComp::~HMdcHitComp");
#endif

  if(fHistograms){
    fHistograms->Delete();
    //cout << "fHistograms Deleted" << endl;
    delete fHistograms;
    //cout << "deleted fHistograms" << endl;
  }
  //  deleteHisto();

  //  printStat();

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitComp::~HMdcHitComp");
#endif

}

void HMdcHitComp ::  deleteHisto(void){
// delete histograms
  if(hNHits) delete hNHits;

  Int_t layers = ((HMdcDetector* )(gHades->getSetup()->getDetector("Mdc")))->getMaxComponents(); 

  for(Int_t i= 0; i< layers; i++){
    if(hCoord[i]) delete hCoord[i];
    if(hx[i]) delete hx[i];
    if(hy[i]) delete hy[i];
    if(hres[i]) delete hres[i];
}

  if(hxSlope) delete hxSlope;
  if(hySlope) delete hySlope;
  if(hRecHits) delete hRecHits;
  if(hTracks) delete hTracks;
  if(hChi) delete hChi;
  if(hChi6) delete hChi6;
  if(hChi5) delete hChi5;
  if(hChi6to6) delete hChi6to6;
  if(hChi6to5) delete hChi6to5;
  if(hChi5to5) delete hChi5to5;
  if(hChiGhost6) delete hChiGhost6;
  if(hChiGhost5) delete hChiGhost5;
  if(hProb) delete hProb;
  if(hProb6) delete hProb6;
  if(hProb5) delete hProb5;
  if(hYX) delete hYX;
  if(hXslopeX) delete hXslopeX;
  if(hYslopeY) delete hYslopeY;
  if(hXslopeYslope) delete hXslopeYslope;
  if(hDistanceToTarget) delete hDistanceToTarget;
  if(hDistanceToTargetOverErr) delete hDistanceToTargetOverErr;
  if(hDistanceToZ) delete hDistanceToZ;
  if(hDistanceToZOverErr) delete hDistanceToZOverErr;
  if(hDistanceToTargetWellRec) delete hDistanceToTargetWellRec;
  if(hDistanceToTargetOverErrWellRec) delete hDistanceToTargetOverErrWellRec;
  if(hDistanceToZWellRec) delete hDistanceToZWellRec;
  if(hDistanceToZOverErrWellRec) delete hDistanceToZOverErrWellRec;
  if(hDistanceToTargetNoWR) delete hDistanceToTargetNoWR;
  if(hDistanceToTargetOverErrNoWR) delete hDistanceToTargetOverErrNoWR;
  if(hDistanceToZNoWR) delete hDistanceToZNoWR;
  if(hDistanceToZOverErrNoWR) delete hDistanceToZOverErrNoWR;
  if(hDiffRecSimXSlopeOverErr) delete hDiffRecSimXSlopeOverErr;
  if(hDiffRecSimYSlopeOverErr) delete hDiffRecSimYSlopeOverErr;
  if(hDiffRecSimX) delete hDiffRecSimX;
  if(hDiffRecSimY) delete hDiffRecSimY;
  if(hDiffRecSimXSlope) delete hDiffRecSimXSlope;
  if(hDiffRecSimYSlope) delete hDiffRecSimYSlope;

}


void HMdcHitComp::printStat(void){
  // prints the statistical information to the standard output
  cout << "simulated segments with 6, 5, 4 ,3, 2, and 1 hits :" << endl;
  cout << "nsim6 " << nsim6 << endl;
  cout << "nsim5 " << nsim5 << endl;
  cout << "nsim4 " << nsim4 << endl;
  cout << "nsim3 " << nsim3 << endl;
  cout << "nsim2 " << nsim2 << endl;
  cout << "nsim1 " << nsim1 << endl;
  cout << "reconstructed segments with 6, 5 and 4 hits: " << endl;
  cout << "nseg6 " << nseg6 << endl;
  cout << "nseg5 " << nseg5 << endl;
  cout << "nseg4 " << nseg4 << endl;
  cout << "some information..." << endl;
  cout << "n6to6 " << n6to6 << endl;
  cout << "fn6to6 " << fn6to6 << endl;
  cout << "n6to5 " << n6to5 << endl;
  cout << "fn6to5 " << fn6to5 << endl;
  cout << "n6to4 " << n6to4 << endl; 
  cout << "fn6to4 " << fn6to4 << endl; 
  cout << "n5to6 " << n5to6 << endl;
  cout << "fn5to6 " << fn5to6 << endl;
  cout << "n5to5 " << n5to5 << endl;
  cout << "fn5to5 " << fn5to5 << endl;
  cout << "n5to4 " << n5to4 << endl;
  cout << "fn5to4 " << fn5to4 << endl;
  cout << "n4to6 " << n4to6 << endl;
  cout << "fn4to6 " << fn4to6 << endl;
  cout << "n4to5 " << n4to5 << endl;
  cout << "fn4to5 " << fn4to5 << endl;
  cout << "n4to4 " << n4to4 << endl;
  cout << "fn4to4 " << fn4to4 << endl;
  cout << "norec6 " << norec6 << endl;
  cout << "norec5 " << norec5 << endl;
  cout << "norec4 " << norec4 << endl;

  Float_t perGhost6 = 0, perGhost5 = 0, perGhost4 = 0;
  if (nseg6!=0) perGhost6 = ghost6*100./nseg6;
  if (nseg5!=0) perGhost5 = ghost5*100./nseg5;
  if (nseg4!=0) perGhost4 = ghost4*100./nseg4;
  cout << "ghost6 " << ghost6 << " (" << perGhost6 << " %)" << endl;
  cout << "ghost5 " << ghost5 << " (" << perGhost5 << " %)" << endl;
  cout << "ghost4 " << ghost4 << " (" << perGhost4 << " %)" << endl;

  cout << "mix6to5 " << mix6to5 << endl;
  cout << "fmix6to5 " << fmix6to5 << endl;
  cout << "mix6to4plus1 " << mix6to4plus1 << endl;
  cout << "fmix6to4plus1 " << fmix6to4plus1 << endl;
  cout << "mix6to4 " << mix6to4 << endl;
  cout << "fmix6to4 " << fmix6to4 << endl;
  cout << "mix5to4 " << mix5to4 << endl;
  cout << "fmix5to4 " << fmix5to4 << endl;

  Float_t efficiency=0, eff6=0 ,eff5=0 ,eff4=0;
  Int_t totalGhost, totalTracks321, totalTracks654, totalNoRec;
  totalTracks654 = nsim6+nsim5+nsim4;
  totalTracks321 = nsim3 + nsim2 + nsim1;
  totalNoRec = norec6+norec5+norec4;
  if(totalTracks654 == 0) efficiency = 0;
  else efficiency = (totalTracks654-totalNoRec)*100./totalTracks654;
  totalGhost = ghost6+ghost5+ghost4;

  if (nsim6 != 0) eff6 = (nsim6-norec6)*100./nsim6;
  if (nsim5 != 0) eff5 = (nsim5-norec5)*100./nsim5;
  if (nsim4 != 0) eff4 = (nsim4-norec4)*100./nsim4;

  cout << endl << endl;
  cout << "efficiency reconstructing 6-impact hits " << eff6 <<endl;
  cout << "efficiency reconstructing 5-impact hits " << eff5 <<endl;
  cout << "efficiency reconstructing 4-impact hits " << eff4 <<endl;

  cout << endl << endl;
  cout << "total tracks 6,5,4 hits " << totalTracks654 << endl;
  cout << "total tracks 3,2,1 hits " << totalTracks321 << endl;
  cout << "efficiency " << efficiency << endl;
  cout << "total ghosts " << totalGhost << " (" << totalGhost*100./(nseg6+nseg5+nseg4) << " %)"<< endl;
  cout << endl << endl << endl;

}


void HMdcHitComp::printStat(Text_t* file){
  // prints statistical information to file

  ofstream output(file,ios::app);
  output << "STATISTICS FOR RECONSTRUCTION IN SECTOR " << fLoc[0] << " MODULE " << fLoc[1] << endl << endl;
  output << "simulated segments with 6, 5, 4, 3, 2 and 1 hits :" << endl;
  output << "nsim6 " << nsim6 << endl;
  output << "nsim5 " << nsim5 << endl;
  output << "nsim4 " << nsim4 << endl;
  output << "nsim3 " << nsim3 << endl;
  output << "nsim2 " << nsim2 << endl;
  output << "nsim1 " << nsim1 << endl;
  output << "reconstructed segments with 6, 5 and 4 hits: " << endl;
  output << "nseg6 " << nseg6 << endl;
  output << "nseg5 " << nseg5 << endl;
  output << "nseg4 " << nseg4 << endl;
  output << "some information..." << endl;
  output << "n6to6  " << n6to6 << endl;
  output << "fn6to6  " << fn6to6 << endl;
  output << "n6to5  " << n6to5 << endl;
  output << "fn6to5  " << fn6to5 << endl;
  output << "n6to4  " << n6to4 << endl;
  output << "fn6to4  " << fn6to4 << endl;
  output << "n5to6 " << n5to6 << endl;
  output << "fn5to6 " << fn5to6 << endl;
  output << "n5to5  " << n5to5 << endl;
  output << "fn5to5  " << fn5to5 << endl;
  output << "n5to4  " << n5to4 << endl;
  output << "fn5to4  " << fn5to4 << endl;
  output << "n4to6 " << n4to6 << endl;
  output << "fn4to6 " << fn4to6 << endl;
  output << "n4to5 " << n4to5 << endl;
  output << "fn4to5 " << fn4to5 << endl;
  output << "n4to4  " << n4to4 << endl;
  output << "fn4to4  " << fn4to4 << endl;
  output << "norec6 " << norec6 << endl;
  output << "norec5 " << norec5 << endl;
  output << "norec4 " << norec4 << endl;
  Float_t perGhost6 = 0, perGhost5 = 0, perGhost4 = 0;
  if (nseg6!=0) perGhost6 = ghost6*100./nseg6;
  if (nseg5!=0) perGhost5 = ghost5*100./nseg5;
  if (nseg4!=0) perGhost4 = ghost4*100./nseg4;
  output << "ghost6 " << ghost6 << " (" << perGhost6 << " %)" << endl;
  output << "ghost5 " << ghost5 << " (" << perGhost5 << " %)" << endl;
  output << "ghost4 " << ghost4 << " (" << perGhost4 << " %)" << endl;
  output << "mix6to5 " << mix6to5 << endl;
  output << "fmix6to5 " << fmix6to5 << endl;
  output << "mix6to4plus1 " << mix6to4plus1 << endl;
  output << "fmix6to4plus1 " << fmix6to4plus1 << endl;
  output << "mix6to4 " << mix6to4 << endl;
  output << "fmix6to4 " << fmix6to4 << endl;
  output << "mix5to4 " << mix5to4 << endl;
  output << "fmix5to4 " << fmix5to4 << endl;
  Float_t efficiency=0, eff6 =0, eff5=0, eff4=0;
  Int_t totalGhost, totalTracks321, totalTracks654, totalNoRec;
  totalTracks654 = nsim6+nsim5+nsim4;
  totalTracks321 = nsim3 + nsim2 + nsim1;
  totalNoRec = norec6+norec5+norec4;
  if(totalTracks654 != 0) 
    efficiency = (totalTracks654-totalNoRec)*100./totalTracks654;
  totalGhost = ghost6+ghost5+ghost4;



  if (nsim6 != 0) eff6 = (nsim6-norec6)*100./nsim6;
  if (nsim5 != 0) eff5 = (nsim5-norec5)*100./nsim5;
  if (nsim4 != 0) eff4 = (nsim4-norec4)*100./nsim4;

  output << endl << endl;
  output << "efficiency reconstructing 6-impact hits " << eff6 <<endl;
  output << "efficiency reconstructing 5-impact hits " << eff5 <<endl;
  output << "efficiency reconstructing 4-impact hits " << eff4 <<endl;

  output << endl << endl;
  output << "total tracks 6,5,4 hits " << totalTracks654 << endl;
  output << "total tracks 3,2,1 hits " << totalTracks321 << endl;
  output << "efficiency " << efficiency << " %"<< endl;
  output << "total ghosts " << totalGhost << " (" << totalGhost*100./(nseg6+nseg5+nseg4) << " %)" << endl;
  output << endl << endl << endl;

}


void HMdcHitComp :: buildHitAuxSimCat(void){

  // builds sim category with the cal3sim data

#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcHitComp::buildHitAuxSimCat");
#endif

  // iterator over HMdcCal3Sim category

  HIterator* iter = 0;
  iter = (HIterator*)fCal3Cat->MakeIterator();
  iter->Reset();
  iter->gotoLocation(fLoc);

  HMdcCal3Sim* cal;

  while((cal= (HMdcCal3Sim*)iter->Next())!= NULL){
    addCal3ToHitAux(cal);
  }

#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcHitComp::buildHitAuxSimCat");
#endif


}


void HMdcHitComp :: addCal3ToHitAux(HMdcCal3Sim * cal){

  // This function is used to build the hitauxsim category

#if DEBUG_LEVEL > 2
  gDebuger->enterFunc("HMdcHitComp::addCal3ToHitAux");
#endif

  HMdcHitAuxSim* hitsim = NULL;
  HLocation lochit;
  lochit.set(3,fLoc[0],fLoc[1],0);


  HMatrixCatIter* iter = NULL;
  iter=(HMatrixCatIter *)fSimCat->MakeIterator();
  iter->Reset();

  iter->gotoLocation(fLoc);
  
  Int_t nhitaux=0;           // number of hits in the category

  // loop over fHitAuxCat category to add the hit to the corresponding segm.
  Bool_t found = kFALSE;

  while(((hitsim=(HMdcHitAuxSim*)iter->Next())!=NULL) && !found){
    lochit = iter->getLocation();
    nhitaux++;
    if(hitsim->getId()==cal->getId()) found = kTRUE; 
  }

  // add cal to hitauxsim. If the corresponding hitauxsim
  // does not exist yet, it is created and added to
  // the category
 
 
  if(!found){
    if(nhitaux!=0) lochit[2] = lochit.getIndex(2)+1;
    hitsim = (HMdcHitAuxSim*)fSimCat->getSlot(lochit);
    hitsim = new(hitsim) HMdcHitAuxSim;
    hitsim->setId(cal->getId());
    hitsim->addCal(cal,cal->getLayer());
  }
  else{
    hitsim = (HMdcHitAuxSim*)fSimCat->getObject(lochit);
    hitsim->addCal(cal,cal->getLayer());
  }




#if DEBUG_LEVEL > 2
  gDebuger->leaveFunc("HMdcHitComp::addCal3ToHitAux");
#endif

}





Int_t HMdcHitComp::execute(void){
  // execute function

#if DEBUG_LEVEL>2
      gDebuger->enterFunc("HMdcHitComp::execute");
#endif

      buildHitAuxSimCat();


      ofstream output("output.txt",ios::app);
      ofstream norec("norec.txt",ios::app);

      norec << endl;
      norec << "event " << fEventId << endl;
      output << endl;
      output << "suceso " << fEventId << endl;

  HMdcHitAuxSim* sim  = NULL;
  HMdcHitAux *rec = NULL;
  HMatrixCatIter *iter = NULL,*iter2=NULL;
  HMdcCal3Sim *hit1, *hit2;
  Int_t hitid1, hitid2;
  Int_t ncommon;

  Int_t sector = fLoc[0];
  Int_t module = fLoc[1];
  Int_t dim = geo->getNLayers();
  TArrayF *cosn = new TArrayF(dim);
  TArrayF *sinn = new TArrayF(dim);
  TArrayF *zplane= new TArrayF(dim);

  for (Int_t i=0; i< dim; i++){
    cosn->AddAt(geo->getCosn(sector,module,i),i);
    sinn->AddAt(geo->getSinn(sector,module,i),i);
    zplane->AddAt(geo->getZPlane(sector,module,i),i);
  }

#if 1
  char filesim[20], filerec[20];
  sprintf(filesim,"sim%d.txt",fEventId);
  sprintf(filerec,"rec%d.txt",fEventId);
  ofstream simout(filesim,ios::app);
  ofstream recout(filerec,ios::app);

  simout << endl;
  recout << endl;
  simout << "suceso " << fEventId << endl;
  recout << "suceso " << fEventId << endl;
#endif
  //ofstream mix("mix.txt", ios::app);
  //mix << "suceso " << fEventId << endl;
  Int_t simhits, rechits;

  Bool_t mixed, wellrec;
  
  Int_t nrectracks=0;

  Float_t distance = 0;
  HLocation simloc;
  simloc.setNIndex(2);

  iter2=(HMatrixCatIter *)fRecCat->MakeIterator();	
  iter2->Reset();
  iter2->gotoLocation(fLoc);
  while((rec=(HMdcHitAux *)iter2->Next())!=NULL){

    if(fLoc[1] < 2){
      distance = distanceToTarget(rec);
      hDistanceToTarget->Fill(distance);
      
      distance = distanceToTargetOverErr(rec);
      hDistanceToTargetOverErr->Fill(distance);

      distance = distanceToZ(rec);
      hDistanceToZ->Fill(distance);

      distance = distanceToZOverErr(rec);
      if (distance < 20000) hDistanceToZOverErr->Fill(distance);
    }
    //rec->print(recout);

    nrectracks++;
    hxSlope->Fill(rec->getSlopeX());
    hySlope->Fill(rec->getSlopeY());
    
    rechits = rec->getNumHits();
    hRecHits->Fill(rechits);
    switch(rechits){
    case 6 : nseg6++; break;
    case 5 : nseg5++; break;
    case 4 : nseg4++; break;
    }

    Double_t mychi = rec->getChi();
    Int_t dof = rechits - 4;
    Double_t prob = TMath::Prob(mychi,dof);

    hChi->Fill(mychi);
    hProb->Fill(prob);
    if(rechits==6){
      hChi6->Fill(mychi);
      hProb6->Fill(prob);
    }
    else
      if(rechits==5){ 
	hChi5->Fill(mychi);
	hProb5->Fill(prob);
      }
    Float_t theth, residue,x ,y;
    Float_t x0 = rec->getPointX();
    Float_t y0 = rec->getPointY();
    Float_t z0 = rec->getPointZ();
    Float_t xslope = rec->getSlopeX();
    Float_t yslope = rec->getSlopeY();

    hYX->Fill(x0,y0);
    hXslopeX->Fill(x0,xslope);
    hYslopeY->Fill(y0,yslope);
    hXslopeYslope->Fill(yslope,xslope);
    
    rec->print(output);
    
    for(Int_t plane = 0; plane< dim; plane++){
                                        
      if(rec->getCal(plane)!=NULL){
	x = x0 + xslope * (zplane->At(plane) - z0);
	y = y0 + yslope * (zplane->At(plane) - z0);
	theth = sinn->At(plane) * x - cosn->At(plane)*y;
	residue = theth - rec->getCal(plane)->getPos();
	hres[plane]->Fill(residue);
	hx[plane]->Fill(x);
	hy[plane]->Fill(y);
      }
    }
  

    wellrec = kFALSE;

    iter=(HMatrixCatIter *)fSimCat->MakeIterator();
    iter->Reset();
    iter->gotoLocation(fLoc);
    while((sim=(HMdcHitAuxSim *)iter->Next())!=NULL){  
      mixed = kFALSE;
      ncommon = 0;
     output << "simulated: " << endl;
      sim->print(output);

      for(Int_t i=0; i<6; i++){
	if(sim->getCal(i)!=NULL && rec->getCal(i)!=NULL){
	  hit1 = (HMdcCal3Sim*)rec->getCal(i);
	  hitid1 = hit1->getId();
	  hit2 = (HMdcCal3Sim*)sim->getCal(i);
	  hitid2 = hit2->getId();
          output << "id1 " << hitid1 << '\t' << "id2 " << hitid2 << endl;
	  if(hitid1 == hitid2){
	    ncommon++;
	    if(hit1->getHitNumber() % 2 !=0 ) mixed = kTRUE;
            output << "ncommon " << ncommon << '\t' << mixed << endl;
	  }
	}
      }

      //if(mixed) {
      //rec->print(mix);
      //sim->print(mix);
      //mix << endl;
      //}


	  
      rechits = rec->getNumHits();
      simhits = sim->getNumHits();
      output << "rechits " << rechits << endl;
      output << "simhits " << simhits << endl;
      output << "ncommon " << ncommon << endl;

      if(simhits==6) {
	if(rechits==6){
	  if(ncommon ==6){ 
	    n6to6++;
	    sim->setDel();
	    hChi6to6->Fill(rec->getChi());
	    if(mixed) fn6to6++;
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	  }
	  else if(ncommon==5){
	    mix6to5++;
	    sim->setDel();
	    if(mixed) fmix6to5++;
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	  }
	  else if(ncommon==4){
	    mix6to4++;
	    sim->setDel();
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	    if(mixed) fmix6to4++;
	    }
	}                               // end of if rec->numhis==6
	else if(rechits==5){
	  if(ncommon == 5){
	    n6to5++;
	    if(mixed) fn6to5++;
	    sim->setDel();
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	    hChi6to5->Fill(rec->getChi());
	  }
	  else if(ncommon==4){
	    mix6to4plus1++;
	    if(mixed) fmix6to4plus1++;
	    sim->setDel();
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	  }
	}
	else if(rechits==4 && ncommon ==4){
	  n6to4++;
	  sim->setDel();
	  if(mixed) fn6to4++;
	  wellrec = kTRUE;
	  simloc = iter->getLocation();
	}
      }                              // end of if sim has 6 hits
      else if(simhits==5){
	if(rechits==6 && ncommon==5){
	  n5to6++;
	  sim->setDel();
	  if(mixed) fn5to6++;
	  wellrec = kTRUE;
	  simloc = iter->getLocation();
	}
	else if(rechits==5){
	  if(ncommon == 5){
	    n5to5++;
	    sim->setDel();
	    hChi5to5->Fill(rec->getChi());
	    if(mixed) fn5to5++;
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	  }
	  else if(ncommon==4){
	    mix5to4++;
	    sim->setDel();
	    if(mixed) fmix5to4++;
	    wellrec = kTRUE;
	    simloc = iter->getLocation();
	  }
	}                            // end of if rec has 5 hits
	else if(rechits==4 && ncommon ==4){
	  n5to4++;
	  sim->setDel();
	  if(mixed) fn5to4++;
	  wellrec = kTRUE;
	  simloc = iter->getLocation();
	}
      }                              // end of if sim has 5 hits
      else if(simhits==4){ 
	if(rechits==6 && ncommon==4){
	  n4to6++;
	  sim->setDel();
	  wellrec = kTRUE;
	  simloc = iter->getLocation();
	  if(mixed) fn4to6++;
	}
	else if(rechits==5 && ncommon==4){
	  n4to5++;
	  sim->setDel();
	  if(mixed) fn4to5++;
	  wellrec = kTRUE;
	  simloc = iter->getLocation();
	}
	else if(rechits==4 && ncommon==4){
	  n4to4++;
	  sim->setDel();
	  if(mixed) fn4to4++;
	  wellrec = kTRUE;
	  simloc = iter->getLocation();
	}  
      }
    }                    // end of loop in simulated segments
    delete iter;
     
    if(wellrec){

      if(fLoc[1] < 2){
	hDistanceToTargetWellRec->Fill(distanceToTarget(rec));
	hDistanceToTargetOverErrWellRec->Fill(distanceToTargetOverErr(rec));
	hDistanceToZWellRec->Fill(distanceToZ(rec));
	hDistanceToZOverErrWellRec->Fill(distanceToZOverErr(rec));
      }

      sim = (HMdcHitAuxSim*)fSimCat->getObject(simloc);
      if(sim->getNumHits()>3){
	TMatrix param(4,1);
	param=calculateSlopes(sim,z0);
	Float_t err = rec->getErrorSlope1();
	Float_t diff;
	diff = rec->getSlopeX() - param(2,0);
	hDiffRecSimXSlope->Fill(diff);
	if(err !=0){
	  diff = diff/err;
	  hDiffRecSimXSlopeOverErr->Fill(diff);
	}
	err = rec->getErrorSlope2();
	diff = rec->getSlopeY() - param(3,0);
	hDiffRecSimYSlope->Fill(diff);
	if(err !=0){
	  diff = diff/err;
	  hDiffRecSimYSlopeOverErr->Fill(diff);
	}
	hDiffRecSimX->Fill(rec->getPointX() - param(0,0));
	hDiffRecSimY->Fill(rec->getPointY() - param(1,0));
      }
      
    }
    else {
      if(fLoc[1] < 2){
	hDistanceToTargetNoWR->Fill(distanceToTarget(rec));
	hDistanceToTargetOverErrNoWR->Fill(distanceToTargetOverErr(rec));
	hDistanceToZNoWR->Fill(distanceToZ(rec));
	hDistanceToZOverErrNoWR->Fill(distanceToZOverErr(rec));	
      }
	// a rec track with wellrec = kFALSE is a ghost
      
      if(rec->getNumHits()==6){ 
	ghost6++;
	hChiGhost6->Fill(rec->getChi());
      }
      else if(rec->getNumHits()==5) {
	ghost5++;
	hChiGhost5->Fill(rec->getChi());
      }
      else if(rec->getNumHits()==4) ghost4++;
      
    }
    
  }            // end of loop in reconstructed segments
  delete iter2;


  iter=(HMatrixCatIter *)fSimCat->MakeIterator();
  iter->Reset();
  iter->gotoLocation(fLoc);
  while((sim=(HMdcHitAuxSim *)iter->Next())!=NULL){

    sim->printCal(simout);
    simout << endl;
    simhits = sim->getNumHits();
    hNHits->Fill(simhits);
    switch(simhits){
    case 6 : nsim6++; break;
    case 5 : nsim5++; break;
    case 4 : nsim4++; break;
    case 3 : nsim3++; break;
    case 2 : nsim2++; break;
    case 1 : nsim1++; break;
    }


    for(Int_t i = 0; i< dim; i++){
      if(sim->getCal(i)!=NULL)hCoord[i]->Fill(sim->getCal(i)->getPos());
    }
    
    if(sim->isDel()==0){
      if(simhits==6) {
	norec6++;
	norec << "no rec: 6hit-segment " << sim->getId() << endl << endl;
	sim->print(norec);
	cout <<  " no rec: 6hit-segment " << sim->getId() << endl << endl;
      } 
      else if(simhits==5){
	norec5++;
	norec << "norec: 5hit-segment " << sim->getId() << endl << endl;
	sim->print(norec);
	cout <<  "no rec: 5hit-segment " << sim->getId() << endl << endl; 
      }
      else if(simhits==4){
	norec4++;
	norec << "norec: 4hit-segment " << sim->getId() << endl << endl;
	sim->print(norec);
	cout <<  "no rec: 4hit-segment " << sim->getId() << endl << endl; 
      }
    }
  }                                      // end of loop in sim segments
  delete iter;

  hTracks->Fill(nrectracks);

  delete cosn;
  delete sinn;
  delete zplane;

#if 1
  if(fEventId!=0){
    char filename[20];
    sprintf(filename,"parstat%d.txt",fEventId);
    //For debuging purposes
//     ofstream partialstat(filename,ios::app);
//     partialstat << "event " << fEventId << endl;
//     printStat(filename);
//     partialstat << endl;
//     partialstat.close();
  }  
#endif  
    fEventId++;

#if DEBUG_LEVEL>2
  gDebuger->leaveFunc("HMdcHitComp::execute");
#endif

  return 0;
}



Float_t HMdcHitComp :: distanceToTarget(HMdcHitAux* seg){

  Float_t u1[3], points[3],w[3];
  Float_t ztarget = target->getZPos(fLoc[0],fLoc[1]);

  u1[0] = seg->getSlopeX();              //not normalized
  u1[1] = seg->getSlopeY();
  u1[2] = 1;

  Float_t norm = sqrt(u1[0]*u1[0]+u1[1]*u1[1]+u1[2]*u1[2]);  // note: norm!=0


  points[0] = seg->getPointX();                     //xtarget = ytarget = 0
  points[1] = seg->getPointY();
  points[2] = seg->getPointZ() - ztarget;

  Float_t distance = (TMath::NormCross(u1,points,w))/norm;

  return distance;

}


Float_t HMdcHitComp :: distanceToTargetOverErr(HMdcHitAux* seg){

  Float_t u[3], points[3],dist[3];
  Float_t ztarget = target->getZPos(fLoc[0],fLoc[1]);

  u[0] = seg->getSlopeX();              //not normalized
  u[1] = seg->getSlopeY();
  u[2] = 1;


  points[0] = seg->getPointX();                     //xtarget = ytarget = 0
  points[1] = seg->getPointY();
  points[2] = seg->getPointZ() - ztarget;
  
  Float_t norm1=0, norm2=0;

  for(Int_t i=0; i<3; i++){
    norm1 = norm1 + u[i]*u[i];               // note that norm1!=0 always
    norm2 = norm2 + points[i]*points[i];
  }

  norm1 = sqrt(norm1);
  norm2 = sqrt(norm2);

  // normalising u

  for(Int_t i=0; i<3; i++) u[i] = u[i]/norm1;


  TMath::Cross(u,points,dist);          //stores in dist = u x points
  Float_t distance = 0;

  for(Int_t i=0; i<3; i++){
    distance = distance + dist[i]*dist[i];
  }
  distance = sqrt(distance);

  Float_t errdistance=0, errdistance2=0;     // error in d and in d**2

  // derivatives


  Float_t v1[3], v2[3], deriv[4],error[4];
  TMath::Cross(dist,u,v1);
  TMath::Cross(dist,points,v2);
 
  deriv[0] = 2.*v1[0]/norm1;         // deriv. respect to x0
  deriv[1] = 2.*v1[1]/norm1;         // deriv. respect y0
  deriv[2] = -2.*distance*u[0]/pow(norm1,3) - 2.*v1[0]/norm1;  // deriv. xslope
  deriv[3] = -2.*distance*u[1]/pow(norm1,3) - 2.*v1[1]/norm1;  // deriv. yslope

  error[0] = seg->getXError();
  error[1] = seg->getYError();
  error[2] = seg->getErrorSlope1();
  error[3] = seg->getErrorSlope2();

  for(Int_t i=0; i<4; i++){
    errdistance2 +=deriv[i]*deriv[i]*error[i]*error[i];
  }
  
  errdistance2 = sqrt(errdistance2);


  if(distance!=0) errdistance = errdistance2/(2*distance);
  else return 20000;

  if (errdistance!=0) return distance/errdistance;

  else return 20000;

}





Float_t HMdcHitComp :: distanceToZ(HMdcHitAux* seg){
  // funcion de prueba. Si se sigue este metodo, hay que mejorarla.
  // distance to z axis

  // z axis in mdc coordinate system (sector 1)

  Float_t ztarget = target->getZPos(fLoc[0],fLoc[1]);
  Int_t module = fLoc[1];


    Float_t u1[3];         

    u1[0] = seg->getSlopeX();              //not normalized
    u1[1] = seg->getSlopeY();
    u1[2] = 1;


    Float_t ut[3];         // unitary vector of z axis in mdc coord
    ut[0] = 0;
    if (module == 0) {
      ut[1] = -0.697109;
      ut[2] = 0.716965;
    }
    else if(module==1){
      ut[1] = -0.808962;
      ut[2] = 0.587861;
    }
    else Error("distanzeToZ","Unknown module %i",module);
  
    
    Float_t points[3];
    points[0] = seg->getPointX();                     //xtarget = ytarget = 0
    points[1] = seg->getPointY();
    points[2] = seg->getPointZ() - ztarget;
    

    Float_t normcross[3];
    TMath::NormCross(u1,ut,normcross);
    
    
    Float_t distance = 0;
    for(Int_t l=0; l< 3;l++){
      distance = distance + points[l]*normcross[l];
    }
    
    distance = fabs(distance);

    return distance;
  
}




Float_t HMdcHitComp :: distanceToZOverErr(HMdcHitAux* seg){


  Float_t ztarget = target->getZPos(fLoc[0],fLoc[1]);
  Int_t module = fLoc[1];

    Float_t error[4];
    error[0] = seg->getXError();
    error[1] = seg->getYError();
    error[2] = seg->getErrorSlope1();
    error[3] = seg->getErrorSlope2();
    

    Float_t u1[3];         
    
    u1[0] = seg->getSlopeX();              //not normalized
    u1[1] = seg->getSlopeY();
    u1[2] = 1;


    Float_t ut[3];         // unitary vector of z axis in mdc coord
    ut[0] = 0;
    if (module == 0) {
      ut[1] = -6.97109;
      ut[2] = 7.16965;
    }
  else if(module==1){
    ut[1] = -8.08962;
    ut[2] = 5.87861;
  }
    else Error("distanceToZOverErr","Unknown module %i",module);
    

    Float_t points[3];
    points[0] = seg->getPointX();                     //xtarget = ytarget = 0
    points[1] = seg->getPointY();
    points[2] = seg->getPointZ() - ztarget;
  
  
    Float_t normcross[3];
    Double_t crossMod = TMath::NormCross(u1,ut,normcross);
    
    Float_t distance = 0;
  for(Int_t l=0; l< 3;l++){
    distance = distance + points[l]*normcross[l];
  }
  
  distance = fabs(distance);
  
  Float_t deriv[4];        // derivatives of distance respect x,y,slopes
  
  deriv[0] = normcross[0];
  deriv[1] = normcross[1];
  
  if(crossMod!=0){
    deriv[2] = (points[0]*normcross[0]*(normcross[1]-normcross[2]) - 
                points[1]*(1+normcross[1]*(normcross[2]-normcross[1])))/
      crossMod;
    deriv[3] = (points[0]*(1+normcross[0]*(normcross[2]-normcross[0])) - 
		points[1]*normcross[1]*(normcross[2]-normcross[1]))/crossMod;
  }
  else  return 20000;
  
  Float_t errdistance, errdistance2=0;
  
  for(Int_t l=0; l<4; l++){
    errdistance2+= deriv[l]*deriv[l]*error[l]*error[l];
  }
  
  errdistance = sqrt(errdistance2);

  
  if (errdistance!=0) return distance/errdistance;
  else return 20000;

}



TMatrix HMdcHitComp :: calculateSlopes(HMdcHitAuxSim* sim, Float_t z0){
  
  Int_t sector = fLoc[0];
  Int_t module = fLoc[1];


  TMatrix m(4,4);          // matrix to obtain a straight line from four theta
                           // coordinates
  TMatrix param(4,1);      // parameters of the straight line
  TMatrix T(4,1);  

  Int_t nlayers = 6;
  Int_t listPlanes[4];
  Int_t j1 = 0, j2=3;
  for(Int_t i=0; i<nlayers; i++){
    if(j1<2){
      if(sim->getCal(i)!=NULL){listPlanes[j1]=i; j1++;}
    }
  }

  for(Int_t i=5; i>=0; i--){
    if(j2>1){
      if(sim->getCal(i)!=NULL){listPlanes[j2]=i;j2--;} 
    } 
  }


  for(Int_t i=0; i<4; i++){
    m(i,0) = geo->getSinn(sector, module, listPlanes[i]);
    m(i,1) = -geo->getCosn(sector, module,listPlanes [i]);
    m(i,2) = m(i,0) * (geo->getZPlane(sector, module,listPlanes[i])-z0);
    m(i,3) = m(i,1) * (geo->getZPlane(sector, module,listPlanes[i])-z0);
    T(i,0) = sim->getCal(listPlanes[i])->getPos();
  }
  m.Invert();      
  param.Mult(m,T);

  return param;

}










