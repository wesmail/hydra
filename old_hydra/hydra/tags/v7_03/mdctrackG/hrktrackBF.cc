//*-- Author : A.Sadovsky <sadovski@fz-rossendorf.de> (04.11.2004); Special thanks to A.Ivashkin, V.Pechenov and A.Rustamov
#include <iostream>
#include "hades.h"
#include "hmetamatch.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofhit.h"
#include "htofhitsim.h"
#include "htofcluster.h"
#include "hrktrackB.h"
#include "hrktrackBF.h"
#include "hiterator.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "htofinogeompar.h"
#include "htofgeompar.h"
#include "hspecgeompar.h"
#include "tofdef.h"
#include "hmdcseg.h"
//#include "hmdchitsim.h"
#include "hmdchit.h"
#include "showerdef.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgfieldpar.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hgeantmdc.h"
#include "hkickplane2.h"
#include "hrungekutta.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hmdctrkcand.h"
#include "hmdctrackgspline.h"
#include "hmdctrackgcorrpar.h"
#include "hgeomvector.h"
#include "TNtuple.h"
#include "hlocation.h"
#include "hmdctrackgsplinecorr.h"
#include "hsplinetrack.h"
#include "hshowerhittoftrack.h"
#include "TFile.h"

#define PI 3.14159265358979323846
#define C_lighSpeed 299792458.0   //[meter/second]

using namespace std;

/////////////////////////////////////////////////////////////////////////////
//
// This class implements Runge Kutta method of momentum calculation
// into Hydra framework. Output is compartible with General Tracking Scheme
// User valuable output is HRKTrackB class in a ROOT tree
// Now it works with all 4 MDCs present
//
/////////////////////////////////////////////////////////////////////////////

ClassImp(HRKTrackBF)

HRKTrackBF::HRKTrackBF(Text_t *name, Float_t FIELD, Short_t MODE) : HReconstructor(name,name){
  fieldFactor     = FIELD;
  mode            = MODE;
  clear();
}

HRKTrackBF::HRKTrackBF(){
  fieldFactor     = 0.0;    // if user forgot to supply field - lets make it obvious - as if there is no field
  mode            = 0;      // if user forgot to supply initial momentum approximation - let RungeKutta find it itself
  clear();
}

void HRKTrackBF::SetMdcAlignMonitorOn(){ bMdcAlignMonitor=kTRUE;} //this function enables MDC alignment monitoring histograms
void HRKTrackBF::SetDebugOn(){ bRKdebug=kTRUE;} //this function enables debugging information into a separate NTuple [rkDebug]
void HRKTrackBF::MakeGeomSnapshotSector(Int_t s){ if(s>=0 && s<6){ bDumpGeomSector[s]=kTRUE;} }

void HRKTrackBF::clear(){
  field           = 0;
  fSpecGeomPar    = 0;
  fGetCont        = 0;
  fCatMetaMatch   = 0;
  fMetaMatchIter  = 0;
  fCatMdcTrkCand  = 0;
  fCatMdcSegSim   = 0;
  fCatMdcHitSim   = 0;
  fCatRKTrackB    = 0;
  fCatShower      = 0;
  fCatTof         = 0;
  fCatTofCluster  = 0;
  fCatRKTrack     = 0;
  fCatKine        = 0; //needed to decide later if we are in SIM or in EXP analysis (to use apropriative category)

  pTofHit         = NULL;
  pTofCluster     = NULL;
  pRungeKutta     = NULL;

  bRKdebug        = kFALSE;  //untill user asks for RK-debug information by calling SetDebugOn()
  fRKdebug        = 0;

  bMdcAlignMonitor= kFALSE;
  for(Int_t s=0; s<6; s++){
  for(Int_t m=0; m<4; m++){
     dxMDC[ m][s] = 0;
     dyMDC[ m][s] = 0;
     dxxMDC[m][s] = 0;
     dyyMDC[m][s] = 0;
  }
  }


  for(Int_t s=0; s<6; s++){ bDumpGeomSector[s]=kFALSE; }
}

HRKTrackBF::~HRKTrackBF(){
  //
}

Bool_t HRKTrackBF::init(){

  if(gHades){
     for(Int_t i=0; i<6;i++){ tRans[i]=0; }
     HRuntimeDb *rtdb=gHades->getRuntimeDb();
     HSpectrometer *spec=gHades->getSetup();
     HEvent *event=gHades->getCurrentEvent();
     if(rtdb && spec && event){
        field=(HMdcTrackGFieldPar*)(rtdb->getContainer("MdcTrackGFieldPar"));
        fSpecGeomPar=(HSpecGeomPar*)(rtdb->getContainer("SpecGeomPar"));

        fGetCont=HMdcGetContainers::getObject();
        fGetCont->getMdcGeomPar();
        fGetCont->getSpecGeomPar();

        HGeomTransform trans;
        fGetCont->getLabTransMod(trans,0,0);

        //TOF-geometry-for-metaEnergy loss calculation-------------------
        HDetector *pDtof=spec->getDetector("Tof");
        if( pDtof ){ //Has the user set up a TOF?
           fTofGeometry = (HTofGeomPar *)rtdb->getContainer("TofGeomPar");
        }else{
           fTofGeometry = 0;
        }

        //TOFINO/Shower-geometry-for-metaEnergy loss calculation-----------------
        HDetector *pDshower = spec->getDetector("Shower");
        HDetector *pDtofino = spec->getDetector("Tofino");
        if( pDshower && pDtofino ){
           fTofinoGeometry = (HTofinoGeomPar *)rtdb->getContainer("TofinoGeomPar");
        }else{
           fTofinoGeometry = 0;
        }

     }
     //- MDC detectors part
     fCatMetaMatch=event->getCategory(catMetaMatch);
     if(!fCatMetaMatch) return kFALSE;
     fMetaMatchIter=(HIterator*)fCatMetaMatch->MakeIterator();
     if(!fMetaMatchIter) return kFALSE;
     fCatMdcTrkCand=event->getCategory(catMdcTrkCand);
     if(!fCatMdcTrkCand) return kFALSE;
     fCatMdcSegSim=event->getCategory(catMdcSeg);
     if(!fCatMdcSegSim) return kFALSE;
     fCatMdcHitSim=event->getCategory(catMdcHit);
     if(!fCatMdcHitSim) return kFALSE;

     //- momentum guess if any
     if(mode==1){ //- use KickTrack as initial guess for momentum
       fKickTrack=event->getCategory(catKickTrack123B);
       if(!fKickTrack){ Error("init","KickTRack123 is used as initial momentum guess, but was not initialized before Runge-Kutta "); return kFALSE; }
     }
     if(mode==2){ //- use SplineTrack as initial guess for momentum
       fSplineTrack=event->getCategory(catSplineTrack);
       if(!fSplineTrack){ Error("init","Spline is used as initial momentum guess, but was not initialized before Runge-Kutta "); return kFALSE; }
     }

     //- META detectors part
     fCatTof=event->getCategory(catTofHit);
     if(!fCatTof) return kFALSE;
     fCatTofCluster=event->getCategory(catTofCluster);
     if(!fCatTofCluster) Warning("init","No catTofClustter in input!");

     fCatKine=event->getCategory(catGeantKine);
     if(!fCatKine){
       fCatShower=event->getCategory(catShowerHitTof);
       if(!fCatShower) return kFALSE;
     }else{
       fCatShower=event->getCategory(catShowerHitTofTrack);
       if(!fCatShower) return kFALSE;
     }

     //- Here we get category HRKTrackB to use it as an output in "execute()"
     fCatRKTrackB=event->getCategory(catRKTrackB);
     if(!fCatRKTrackB){
        Int_t size[2]={6,600};
        fCatRKTrackB=new HMatrixCategory("HRKTrackB",2,size,0.5);
        if(fCatRKTrackB) event->addCategory(catRKTrackB,fCatRKTrackB,"RKTrackB");
     }

  }//-endif-gHades--

  //
  // Debugging NTuple
  //
  if(bRKdebug){
     printf("HRKTrackBF: RungeKutta debugging information is stored see <<rkDebug>> NTuple...\n");
     fRKdebug = new TNtuple("rkDebug","rkDebug",
     "sec:sys:chiq:pRK:p0:pG1:tof:lRK:lSP:beta:pid1:z:r:theRK:phiRK:theSeg:phiSeg:theG:phiG:mdx:mdy:mdz:mdxSeg:mdySeg:mdzSeg");
  }

  if(bMdcAlignMonitor){
     printf("HRKTrackBF: RungeKutta MDC alignment monitor is enabled, see histograms in root output...\n");

     Float_t xmdcMin[4]={-200.,-350.,-500.,-600.};
     Float_t xmdcMax[4]={ 200., 350., 500., 600.};
     Float_t ymdcMin[4]={ 150., 220.,  400.,  400.};
     Float_t ymdcMax[4]={ 750., 850., 1700., 1900.};
     Char_t name[100], titl[100];

     for(Int_t s=0; s<6; s++){
     for(Int_t m=0; m<4; m++){
        sprintf(name,"dxMDC%is%i",m+1,s+1);
        sprintf(titl,"S%i MDC%i dx [mm] SegmentCoordSys",s+1,m+1);
        dxMDC[m][s] = new TH1F(name , titl, 100,-2.5,2.5); 
        dxMDC[m][s]->SetXTitle("x_{RK}-x_{mdcHit} [mm]");

        sprintf(name,"dyMDC%is%i",m+1,s+1);
        sprintf(titl,"S%i MDC%i dy [mm] SegmentCoordSys",s+1,m+1);
        dyMDC[m][s] = new TH1F(name , titl, 100,-1.5,1.5);
        dyMDC[m][s]->SetXTitle("y_{RK}-y_{mdcHit} [mm]");

        sprintf(name,"dxxMDC%is%i",m+1,s+1);
        sprintf(titl,"S%i MDC%i: x vs. dx [mm] SegmentCoordSys",s+1,m+1);
        dxxMDC[m][s]= new TH2F(name, titl,  50,-1.5,1.5, 40, xmdcMin[m], xmdcMax[m]);
        dxxMDC[m][s]->SetXTitle("x_{RK}-x_{mdcHit} [mm]");
        dxxMDC[m][s]->SetYTitle("x_{mdcHit} [mm]");

        sprintf(name,"dyyMDC%is%i",m+1,s+1);
        sprintf(titl,"S%i MDC%i: y vs. dy [mm] SegmentCoordSys",s+1,m+1);
        dyyMDC[m][s]= new TH2F(name, titl,  50,-1.0,1.0, 40, ymdcMin[m], ymdcMax[m]);
        dyyMDC[m][s]->SetXTitle("y_{RK}-y_{mdcHit} [mm]");
        dyyMDC[m][s]->SetYTitle("y_{mdcHit} [mm]");
     }
     }
  }

  return kTRUE;
}

Bool_t HRKTrackBF::reinit(){
  for(Int_t i=0; i<6; i++){ tRans[i]=0; }

  //
  // Obtaining field map from Hydra
  // See also: hsplinetrackF.cc {reinit() function}
  //
  if(!pRungeKutta){
      pRungeKutta = new HRungeKutta();
      HMdcTrackGField* fieldMap = field->getPointer();
      //cout << ":::::::::::::: Hydra Field Map connected fieldMap = " << fieldMap << endl;
      //+//pRungeKutta->init("field.map");
      pRungeKutta->init(fieldMap);
      pRungeKutta->setFieldFactor(fieldFactor);  //pRungeKutta->setFieldFactor(0.7215);
      pRungeKutta->setdEdxAirCorrection(0);      //no energy loss correction so far
  }

  //
  // Which MDCs are installed?
  //
  //
  HMdcDetector* fMdcDet=(HMdcDetector*)gHades->getSetup()->getDetector("Mdc");
  // Geom-Transform Mod->Sector
  //
  HGeomTransform gtrMod2Sec[6][4]; //Geometry transformation from Module->to->Sector coord system for MDCs
  for(Int_t is=0; is<6; is++){
    Int_t *lMods = lMods=fMdcDet->getModules()+is*4; //pointer to the last MDC module
    for(Int_t im=0; im<4; im++){

      //+//printf("mdc[s=%i][m=%i] = %i\n",is,im,lMods[im]);
      if(lMods[im]){ fGetCont->getSecTransMod(gtrMod2Sec[is][im], is, im); }

      //+//printf("--HGeomTrans(hrktrackBF.cc)--[s=%i][m=%i]--\n",is,im);
      //+//gtrMod2Sec[is][im].print();
      pRungeKutta->readMDCpositionsFromProgram(is, im, gtrMod2Sec[is][im]);
    }
  }
  //pRungeKutta->readMDCpositionsFromASCII("/misc/sadovski/analys/anasim0Nov02/anasim1Nov02sr/mdcNov02_all.txt");
  pRungeKutta->applyMDCgeom();

  return kTRUE;
}

Int_t HRKTrackBF::execute(){
   //cout << "-RK-" << endl;
   success = kTRUE;
   fMetaMatchIter->Reset();
   while((pMetaMatch=(HMetaMatch*)(fMetaMatchIter->Next()))!=0){
     sector     = pMetaMatch->getSector();
     system     = pMetaMatch->getSystem();
     indTrkCand = pMetaMatch->getTrkCandInd();
     pMdcTrkCand=(HMdcTrkCand*)fCatMdcTrkCand->getObject(indTrkCand);
     if(!pMdcTrkCand) continue;
     index1     = pMdcTrkCand->getSeg1Ind();
     index2     = pMdcTrkCand->getSeg2Ind();
     pMdcSeg1   = (HMdcSeg*)fCatMdcSegSim->getObject(index1);
     pMdcSeg2   = (HMdcSeg*)fCatMdcSegSim->getObject(index2);
     if(!pMdcSeg1 || !pMdcSeg2) continue;


     /////////////////////////////////////////////////////////////////////////////
     // This would  not work in case of:
     // HTask *mdcTask  = mdcTaskSet->make("","fit,ChamberClusters");
     // it needs:
     // HTask *mdcTasks = mdcTaskSet->make("","real,fit,magneton,mixedclusters,startandcal,timecuts");
     // One has to revrite this part
     // and test both ways
     /////////////////////////////////////////////////////////////////////////////
     ////cout << "pMdcSeg1 && pMdcSeg1" << endl;
     Int_t indMdcHit[4];
     indMdcHit[0]=pMdcSeg1->getHitInd(0);
     indMdcHit[1]=pMdcSeg1->getHitInd(1);
     indMdcHit[2]=pMdcSeg2->getHitInd(0);
     indMdcHit[3]=pMdcSeg2->getHitInd(1);
     //if(indMdcHit[3]==-1) {printf("pMdcSeg2 Nw1=%i, Nw2=%i\n", pMdcSeg2->getSumWires(1), pMdcSeg2->getSumWires(2));}
     //RK4MDC//if(indMdcHit[0]==-1 || indMdcHit[1]==-1 || indMdcHit[2]==-1 || indMdcHit[3]==-1) continue; //we work with MDC1-MDC2-MDC3-MDC4 combinations only
     if(indMdcHit[0]==-1 || indMdcHit[1]==-1 || indMdcHit[2]==-1) continue; //we work with at least MDC1-MDC2-MDC3 combinations
     Bool_t               mdc1234 = kFALSE;
     if(indMdcHit[3]>=0){ mdc1234 = kTRUE; }

     sectorloc.set(1,(Int_t)sector);

     //
     // GeantKine information for TEST only
     //
     if(bRKdebug){
        HMdcSegSim* pMdcSeg1sim  = (HMdcSegSim*)fCatMdcSegSim->getObject(index1);
        Int_t iTrack1 = pMdcSeg1sim->getTrack(0); //we take 1st (most probable track)
        HGeantKine* pGeantKine = (HGeantKine*)fCatKine->getObject(iTrack1-1);
        Int_t iGtrack1;
        pGeantKine->getParticle(iGtrack1, iGparticleMdcSeg1);
        Float_t momG[3];
        pGeantKine->getMomentum(momG[0], momG[1], momG[2]);
        momGeantABS = sqrt( momG[0]*momG[0] + momG[1]*momG[1] + momG[2]*momG[2] );
        Kine_the = acos( momG[2]/momGeantABS );
        Kine_phi = acos( momG[0]/sqrt(momG[0]*momG[0] + momG[1]*momG[1]) );
     }

     //
     // Geting target point (can be used in tracking and for beta calculation)
     //
     const HGeomVector &targetPos = fSpecGeomPar->getTarget(0)->getTransform().getTransVector();

     //
     // We use segments and MDCs positions to calculate respective points on MDCs:
     // We do not have to access the HMdcHit, scince we can get hit point from the
     // intersection of HMdcSeg with Chamber plane. OK, this is an indirect method...
     //
     Double_t u1[6], u2[6];
     u1[0] = pMdcSeg1->getR()*cos(pMdcSeg1->getPhi()+PI/2); //
     u1[1] = pMdcSeg1->getR()*sin(pMdcSeg1->getPhi()+PI/2); // 1st-tracklet's initial point
     u1[2] = pMdcSeg1->getZ();                              //
     u1[3] = cos(pMdcSeg1->getPhi())*sin(pMdcSeg1->getTheta()); //
     u1[4] = sin(pMdcSeg1->getPhi())*sin(pMdcSeg1->getTheta()); // 1st-tracklet's direction
     u1[5] = cos(pMdcSeg1->getTheta());                         //
     u2[0] = pMdcSeg2->getR()*cos(pMdcSeg2->getPhi()+PI/2); //
     u2[1] = pMdcSeg2->getR()*sin(pMdcSeg2->getPhi()+PI/2); // 2nd-tracklet's initial point
     u2[2] = pMdcSeg2->getZ();                              //
     u2[3] = cos(pMdcSeg2->getPhi())*sin(pMdcSeg2->getTheta()); //
     u2[4] = sin(pMdcSeg2->getPhi())*sin(pMdcSeg2->getTheta()); // 2nd-tracklet's direction
     u2[5] = cos(pMdcSeg2->getTheta());                         //

     HGeomVector dir2(u2[3], u2[4], u2[5]); //(2nd-tracklet's direction stored for MetaEloss)
     Float_t dir2length = dir2.length();
     dir2 /= dir2length;                    //(one needs normal vector - just direction)



     Int_t splineTrackIndex  = -1;
     Int_t kickTrackIndex123 = -1;
     momentumGuess   = -1000000.0; //indicates the absence of initial guess
     if(mode==1){ //--Initial momentum guess from KickTrack123--
        kickTrackIndex123 = pMetaMatch->getKick123Ind();
        if( kickTrackIndex123>=0 ){
           pKickTrack = (HSplineTrack*)fKickTrack->getObject(kickTrackIndex123);
           momentumGuess = pKickTrack->getPolarity() * pKickTrack->getP();
        }else{
           cout << "KickTrack did not provide information for RK, using self guess for initial momenum" << endl;
        }
     }
     if(mode==2){ //--Initial momentum guess from SplineTrack---
        splineTrackIndex = pMetaMatch->getSplineInd();
        if( splineTrackIndex>=0 ){ 
           pSplineTrack = (HSplineTrack*)fSplineTrack->getObject(splineTrackIndex);
           //+//printf("SP(sector=%i):  p=%f, length=%f\n",pSplineTrack->getSector(), pSplineTrack->getPolarity()*pSplineTrack->getP(), pSplineTrack->getTofDist() );
           momentumGuess = pSplineTrack->getPolarity() * pSplineTrack->getP();
        }else{
           cout << "SplineTrack did not provide information for RK, using self guess for initial momenum" << endl;
        }
     }

     //
     // Now do Runge-Kutta momentum tracking and fiting
     //
     if(momentumGuess!=-1000000.0){
         //user provided algorithm for momentum guess, and the algorithm was able to calculate momentum
         if(mdc1234){
            pRungeKutta->momRkgNewDinamicSeg12(u1, u2, sector, momentumGuess); //we use momentum guess as a starting value for RungeKutta fit

            //test//pMdc1Hit = (HMdcHitSim*) fCatMdcHitSim->getObject(indMdcHit[0]);
            //test//pMdc2Hit = (HMdcHitSim*) fCatMdcHitSim->getObject(indMdcHit[1]);
            //test//pMdc3Hit = (HMdcHitSim*) fCatMdcHitSim->getObject(indMdcHit[2]);
            //test////pMdc4Hit = (HMdcHitSim*) fCatMdcHitSim->getObject(indMdcHit[3]);
            //test//Double_t xh[3], yh[3];
            //test//xh[0] = Double_t( pMdc1Hit->getX() ) , yh[0] = Double_t( pMdc1Hit->getY() );
            //test//xh[1] = Double_t( pMdc2Hit->getX() ) , yh[1] = Double_t( pMdc2Hit->getY() );
            //test//xh[2] = Double_t( pMdc3Hit->getX() ) , yh[2] = Double_t( pMdc3Hit->getY() );
            //test////xh[3] = Double_t( pMdc4Hit->getX() ) , yh[3] = Double_t( pMdc4Hit->getY() );
            //test////pRungeKutta->momRkgNewDinamic3hit(xh, yh, sector, momentumGuess);
            //test//if(iGparticleMdcSeg1==9){
            //test//  pRungeKutta->momRkgNewDinamic3hit(xh, yh, sector, -momGeantABS);
            //test//}else{
            //test//  pRungeKutta->momRkgNewDinamic3hit(xh, yh, sector,  momGeantABS);
            //test//}
         }else{ 
            //-in-case-of-MDC123--
            pMdc1Hit = (HMdcHit*) fCatMdcHitSim->getObject(indMdcHit[0]);
            pMdc2Hit = (HMdcHit*) fCatMdcHitSim->getObject(indMdcHit[1]);
            pMdc3Hit = (HMdcHit*) fCatMdcHitSim->getObject(indMdcHit[2]);
            Double_t xh[3], yh[3];
            xh[0] = pMdc1Hit->getX() , yh[0] = pMdc1Hit->getY();
            xh[1] = pMdc2Hit->getX() , yh[1] = pMdc2Hit->getY();
            xh[2] = pMdc3Hit->getX() , yh[2] = pMdc3Hit->getY();
            pRungeKutta->momRkgNewDinamic3hit(xh, yh, sector, momentumGuess);

            //if(iGparticleMdcSeg1==9){
            //  //pRungeKutta->momRkgNewDinamic3hit(xh, yh, sector, -momGeantABS);
            //  //+//printf("(momGuess, momGeant)=(%f,%f)\n",momentumGuess, -momGeantABS );
            //}else{
            //  //pRungeKutta->momRkgNewDinamic3hit(xh, yh, sector,  momGeantABS);
            //  //+//printf("(momGuess, momGeant)=(%f,%f)\n",momentumGuess,  momGeantABS );
            //}
         }
     }else{
         //EITHER user did not supply algorithm for momentum guess OR algorithm failed
         if(mdc1234){
            pRungeKutta->momRkgNewDinamicSeg12(u1, u2, sector); //we try ourself in case of MDC1234 are present
         }else{ 
            //Runge Kutta does not work with MDC123 in case no initial guess was specified
            cout << "HRKTrackBF: MDC123 mode, but user did not provide momentum guess - RK failed!" << endl;
            success = kFALSE; // this prevents from writing garbage into HRKTrackB
         }
     }

     //
     // Now propogate it till vertex (so far we use target point instead of vertex)
     //
     Float_t vertex[3]; //used to propagate track from MDC1 backwards till Z=zTarget 
     vertex[0] = targetPos.getX();
     vertex[1] = targetPos.getY();
     vertex[2] = targetPos.getZ();
     pRungeKutta->traceFromMDC1_to_Vertex(vertex);

     chiqRK = pRungeKutta->getChi2();
     pRK    = fabsf(pRungeKutta->getPfit());
     if(pRungeKutta->getPfit() < 0){qRK =-1;}else{qRK=+1;}
     if(pRK>10000){ pRK=10000.0; }
     //+//printf("RK(sector=%i):  p=%f, chiq=%f\n",sector,pRK,chiqRK);


     //
     // Retrieving {theta,phi} information from MdcSeg with RKfit
     //
     Float_t vDir[3], rDir;
     //RK-at-MDC1--
     vDir[0] = pRungeKutta->getDXtackMDC1FitPos();  //getXdirAtVertex();
     vDir[1] = pRungeKutta->getDYtackMDC1FitPos();  //getYdirAtVertex();
     vDir[2] = pRungeKutta->getDZtackMDC1FitPos();  //getZdirAtVertex();
     rDir = sqrt( vDir[0]*vDir[0] + vDir[1]*vDir[1] + vDir[2]*vDir[2] );
     RK_the = acos( vDir[2]/rDir );
     RK_phi = acos( vDir[0]/sqrt(vDir[0]*vDir[0] + vDir[1]*vDir[1]) );

     //access to RK-Fitted-segment12-output--
     RK_seg_the = pRungeKutta->getMDC12_theta();
     RK_seg_phi = pRungeKutta->getMDC12_phi();

     //RK-propagated till vertex--
     vDir[0] = pRungeKutta->getXdirAtVertex();
     vDir[1] = pRungeKutta->getYdirAtVertex();
     vDir[2] = pRungeKutta->getZdirAtVertex();
     rDir = sqrt( vDir[0]*vDir[0] + vDir[1]*vDir[1] + vDir[2]*vDir[2] );
     RKvert_the = acos( vDir[2]/rDir );
     RKvert_phi = acos( vDir[0]/sqrt(vDir[0]*vDir[0] + vDir[1]*vDir[1]) );


     //+//cout << "META" << endl;

     //
     // Now we take into account META information if any
     //
     metaInd = pMetaMatch->getMetaHitInd();
     if(metaInd<0){//------------------------------------if-META-dees-not-exist-----
        // In case we have no TOF/TOFINO information 
        // we just write tracking part into HRKTrackB
        tof          = -1.0;
        track_length = -1.0;
        beta         = -1.0;
        mass2        = -1.0;
        eloss        = -1.0;
        RKxyzMETA[0] = -1000.0;
        RKxyzMETA[1] = -1000.0;
        RKxyzMETA[2] = -1000.0;

        fillData(pMdcSeg1); //fill HRKTrackB

     }else{//-----------------------------------------------else-if META-exists-----
        HGeomVector metaNormVec;
        if( system==0 ){// for TOFINO/Shower
           pShowerHitTofTrack=(HShowerHitTofTrack*)fCatShower->getObject(metaInd);
           tof=pShowerHitTofTrack->getTof();
           pShowerHitTofTrack->getLabXYZ(&xTof,&yTof,&zTof);

           //Geometry of the rod of TOFINO/SHOWER-Hit (needed by MetaEloss calculation) --
           if( fTofinoGeometry ){
              HModGeomPar *module = fTofinoGeometry->getModule(sector, pShowerHitTofTrack->getModule());
              HGeomTransform &transMod = module->getLabTransform();
              HGeomTransform &transSec = fSpecGeomPar->getSector(sector)->getTransform();
              HGeomTransform modTrans(transMod);
              modTrans.transTo(transSec);
              
              HGeomVector r0_mod, rz_mod, r0_lab, rz_lab;
              r0_mod.setXYZ(0.0, 0.0, 0.0);
              rz_mod.setXYZ(0.0, 0.0, 1.0);
              r0_lab=modTrans.transFrom(r0_mod);
              rz_lab=modTrans.transFrom(rz_mod);
              metaNormVec = rz_lab - r0_lab;
           }else{
              Error("execute","No Shower/Tofino geometry specified");
           }
        }//-end-if-system==0--

        if( system==1 ){// for TOF

           //+//printf("pMetaMatch->getTofClusterSize()=%i\n", pMetaMatch->getTofClusterSize() );
           if(pMetaMatch->getTofClusterSize()<=0 ){//no TofCluster was found
              pTofHit=(HTofHit*)fCatTof->getObject(metaInd);

              tof=pTofHit->getTof();
              pTofHit->getXYZLab(xTof,yTof,zTof);

              //Geometry of the rode of TOF-Hit (needed by MetaEloss calculation) --
              if(fTofGeometry){
                 HModGeomPar *module = fTofGeometry->getModule(sector, pTofHit->getModule());
                 HGeomTransform &transMod = module->getLabTransform();
                 HGeomTransform &transSec = fSpecGeomPar->getSector(sector)->getTransform();
                 HGeomTransform modTrans(transMod);
                 modTrans.transTo(transSec);
                
                 HGeomVector r0_mod, rz_mod, r0_lab, rz_lab;
                 r0_mod.setXYZ(0.0, 0.0, 0.0);
                 rz_mod.setXYZ(0.0, 0.0, 1.0);
                 r0_lab=modTrans.transFrom(r0_mod);
                 rz_lab=modTrans.transFrom(rz_mod);
                 metaNormVec = rz_lab - r0_lab;
              }else{
                 Error("execute","No Tof geometry specified");
              }
           }else{ //RK-is in TofCluster mode and TofCluster was found
              //+//printf("--RK-TofCluster-mode--\n");
              //pTofCluster=(HTofClusterSim*)fCatTof    ->getObject(metaInd);
              pTofCluster = (HTofCluster*)fCatTofCluster->getObject(metaInd);

              tof=pTofCluster->getTof();
              pTofCluster->getXYZLab(xTof,yTof,zTof);

              //Geometry of the TOF-Cluster (needed by MetaEloss calculation) --
              if(fTofGeometry){
                 HModGeomPar *module = fTofGeometry->getModule(sector, pTofCluster->getModule());
                 HGeomTransform &transMod = module->getLabTransform();
                 HGeomTransform &transSec = fSpecGeomPar->getSector(sector)->getTransform();
                 HGeomTransform modTrans(transMod);
                 modTrans.transTo(transSec);
                
                 HGeomVector r0_mod, rz_mod, r0_lab, rz_lab;
                 r0_mod.setXYZ(0.0, 0.0, 0.0);
                 rz_mod.setXYZ(0.0, 0.0, 1.0);
                 r0_lab=modTrans.transFrom(r0_mod);
                 rz_lab=modTrans.transFrom(rz_mod);
                 metaNormVec = rz_lab - r0_lab;
              }else{
                 Error("execute","No Tof geometry specified");
              }
           }
        }//-end-if-system==1--

        //
        // We collected META hit coordinates
        // now teransform it into MdcSegment coordinate system
        //
        pointMeta.setXYZ(xTof,yTof,zTof);
        if(tRans[sector]==0){
           tRans[sector]=new HGeomTransform();
           if(!fGetCont->getLabTransSec(*(tRans[sector]),sector)){continue;}
        }
        pointMeta=tRans[sector]->transTo(pointMeta);
        xTof=pointMeta.getX(); //
        yTof=pointMeta.getY(); // Now it is in MdcSegment coord system
        zTof=pointMeta.getZ(); //
        HGeomVector tofPoint(xTof, yTof, zTof);
        //
        // Now, having MDC and META in the same geometry
        // we get track length from vertex to META-point
        // This mean we get beta=tof/length
        //


        //
        // Geting MDC1 track point after RK-fit
        //
        HGeomVector xyzMDC1( pRungeKutta->getXtackMDC1FitPos(), pRungeKutta->getYtackMDC1FitPos(), pRungeKutta->getZtackMDC1FitPos() );
        HGeomVector xyzMDC4( pRungeKutta->getXtackMDC4FitPos(), pRungeKutta->getYtackMDC4FitPos(), pRungeKutta->getZtackMDC4FitPos() );

        //
        // Test only -begin-
        //
        track_lengthSP = -1.0;
        if(mode==2 && splineTrackIndex>=0 ){
          //patch-test-RK-with trajectory length from Spline
          track_lengthSP = pSplineTrack->getTofDist();        
        }
        Float_t trgTof_length = (targetPos-tofPoint).length(); //very simple TEST
        Float_t T14T_length = (xyzMDC1 - targetPos).length() + (xyzMDC4 - xyzMDC1).length() + (tofPoint-xyzMDC4).length();
        //+//printf("TrgTof ? A14T_length = %f ? %f==%f+%f+%f \n", trgTof_length, T14T_length, (xyzMDC1 - targetPos).length(), (xyzMDC4 - xyzMDC1).length(),(tofPoint-xyzMDC4).length());
        // Test only  -end-

        track_length = (xyzMDC1 - targetPos).length() + pRungeKutta->getMDC1_MDC4length() + (tofPoint-xyzMDC4).length();
        //+//printf("TrgTof ? RK_length   = %f ? %f==%f+%f+%f \n", trgTof_length, track_length, (xyzMDC1 - targetPos).length(), pRungeKutta->getMDC1_MDC4length(), (tofPoint-xyzMDC4).length() );

        beta         = 1.0e6*track_length/tof/C_lighSpeed; //dist [mm], tof[ns]
        mass2        = pRK*pRK*(1-beta*beta)/beta/beta;

        ////////////////////////////////// We use Track after magnet 
        // Meta energy loss calculation // as a direction pointing to META 
        ////////////////////////////////// and we use it's angle to META's normal vector
        Float_t zprojvec_lab = dir2.scalarProduct(metaNormVec);
        metaNormVec *= zprojvec_lab;
        Float_t path, edep;

        if(system==0){ // Shower-Tofino //--
           path = ( metaNormVec.length() * dir2.length() )/dir2.scalarProduct(metaNormVec);
           edep = ( (HShowerHitTof *) pShowerHitTofTrack)->getADC();
           if(path >= 1.0){
              eloss = edep/path;
           }else{
              Warning("evaluateMDC123(4)","particle crossed TOFINO with path < 1.\n");
              eloss = -1.0;
           }
        }//------------// Shower-Tofino //--

        if(system==1){ // TOF //--
           path = (metaNormVec.length()/dir2.length()) / dir2.scalarProduct(metaNormVec);

           if(pMetaMatch->getTofClusterSize()>0){
              //
              // TofCluster exist
              //
              if(pTofCluster->getAdcFlag() !=0 ){
                edep = pTofCluster->getEdep();
                if(path >= 1.0){
                   eloss = edep/path;
                   //+//cout << "Eloss from TofCluster"<< endl;
                }else{
                   Warning("evaluateMDC123(4)","particle crossed TOF (clust) wall with path < 1.\n");
                   eloss = -1.0;
                }
              }else{
                //No TofCluster ADC info, what to do? No information no energy loss...:(
                eloss = -1.0;
                //+//cout << "No ADC" << endl;
              }
           }else{
              //
              // TofHit mode (means either user did not enable TofClusterMode or simply cluster did not appear to happen)
              //
              if(pTofHit->getAdcFlag() !=0 ){
                 edep = pTofHit->getEdep();
                 if(path >= 1.0){
                    eloss = edep/path;
                    //+//cout << "Eloss from TofHit"<< endl;
                 }else{
                    Warning("evaluateMDC123(4)","particle crossed TOF (hit) wall with path < 1.\n");
                    eloss = -1.0;
                 }
              }
           }
        }//------------// TOF //--


        //
        // In addition META-hit/cluster and the corresponding to the rod normal vector
        // can be used to intersect with RK-tracking RK will suply (x,y,z)intersection
        // with META-rod and we can make distance((x_meta-xRK)^2 + (y_meta-yRK)^2 + (z_meta-zRK)^2)
        // as an additional RK-base MDC-META quality measurer
        //
        Float_t xyzMETA[3];
        xyzMETA[0] = xTof;
        xyzMETA[1] = yTof;
        xyzMETA[2] = zTof;
        Float_t vecNormMETA[3];
        vecNormMETA[0] = metaNormVec.getX();
        vecNormMETA[1] = metaNormVec.getY();
        vecNormMETA[2] = metaNormVec.getZ();
        pRungeKutta->traceMDC4_META(xyzMETA, vecNormMETA);
        RKxyzMETA[0] = pRungeKutta->getXtackOnMETA()-xTof;
        RKxyzMETA[1] = pRungeKutta->getYtackOnMETA()-yTof;
        RKxyzMETA[2] = pRungeKutta->getZtackOnMETA()-zTof;

        //
        // Check of HMdcSeg2 direction stored here in u2[6]
        //
        pRungeKutta->findIntersectPointVector6WithPlane(u2, xyzMETA, vecNormMETA, pointSeg2META);
        pointSeg2META[0] = pointSeg2META[0]-xTof;
        pointSeg2META[1] = pointSeg2META[1]-yTof;
        pointSeg2META[2] = pointSeg2META[2]-zTof;

        fillData(pMdcSeg1); //finally we fill HRKTrackB

     }//------------------------------------------------------endif-META-exists-----

   }//-end-while-pMetaMatch-loop--

   return 0;
}

HRKTrackB*  HRKTrackBF::fillData(HMdcSeg *segment){
   TObject  *slot = 0;
   HRKTrackB *rkt = 0;
   Int_t indexRK;
   slot = fCatRKTrackB->getNewSlot(sectorloc,&indexRK);
   
   if(!slot){
      Error("fillData","No slot available");
   }else{
      pMetaMatch->setRungeKuttaInd(indexRK); //ADD index TO METAMATCH
      rkt = new(slot) HRKTrackB;

      if(success){//-success--------------------------------
         rkt->setSector(sector);
         rkt->setShowerHitInd(pMetaMatch->getShowerHitInd());
         rkt->setTofHitInd(   pMetaMatch->getTofHitInd()   );
         rkt->setZ(    segment->getZ()     ,segment->getErrZ()     );
         rkt->setR(    segment->getR()     ,segment->getErrR()     );
         rkt->setTheta(segment->getTheta() ,segment->getErrTheta() );
         rkt->setPhi(  segment->getPhi()   ,segment->getErrPhi()   );

         rkt->setP(       pRK              ,0.0                    );
         rkt->setChiq(  chiqRK);
         rkt->setPolarity(qRK );
         rkt->setTof( tof );
         rkt->setTofDist(track_length);
         rkt->setBeta( beta );
         rkt->setMass2( mass2, 0.0);
         rkt->setMetaEloss( eloss );
         rkt->setMETAdx( RKxyzMETA[0] );
         rkt->setMETAdy( RKxyzMETA[1] );
         rkt->setMETAdz( RKxyzMETA[2] );

         //---------from-here-testing-----------------------------//
         if(bRKdebug){
           Float_t nt[25];  //sec:sys:chiq:pRK:p0:pG1:tof:lRK:lSP:beta:pid1:z:r:theSeg:phiSeg:theRK:phiRK:theG:phiG:mdx:mdy:mdz:mdxSeg:mdySeg:mdzSeg
           nt[ 0] = sector;
           nt[ 1] = system;
           nt[ 2] = chiqRK;
           nt[ 3] = pRK;
           nt[ 4] = momentumGuess; //used initial guess from KickTrack or Spline
           nt[ 5] = momGeantABS;   //momentum taken from Geant Kine
           nt[ 6] = tof;
           nt[ 7] = track_length; //RK
           nt[ 8] = track_lengthSP;
           nt[ 9] = beta;
           nt[10] = iGparticleMdcSeg1;
           nt[11] = segment->getZ();
           nt[12] = segment->getR();
           nt[13] = segment->getTheta();
           nt[14] = segment->getPhi();
           nt[15] = RK_the;
           nt[16] = RK_phi;
           nt[17] = Kine_the;
           nt[18] = Kine_phi;
           nt[19] = RKxyzMETA[0];
           nt[20] = RKxyzMETA[1];
           nt[21] = RKxyzMETA[2];
           nt[22] = Float_t(pointSeg2META[0]);
           nt[23] = Float_t(pointSeg2META[1]);
           nt[24] = Float_t(pointSeg2META[2]);

           fRKdebug->Fill(nt);
         }

         if(bMdcAlignMonitor && beta>0.5 && pMdcSeg1->getChi2()>0. && pMdcSeg2->getChi2()>0. ){
            for(Int_t m=0;m<4;m++){
               dxMDC[ m][sector]->Fill(pRungeKutta->getXfithit(m+1));
               dyMDC[ m][sector]->Fill(pRungeKutta->getYfithit(m+1));
               dxxMDC[m][sector]->Fill(pRungeKutta->getXfithit(m+1), pRungeKutta->getXhit(m+1));
               dyyMDC[m][sector]->Fill(pRungeKutta->getYfithit(m+1), pRungeKutta->getYhit(m+1));
            }
         }
      }else{//-no-success-----------------------
         //somthing failed, or momentum reconstruction was not even called
         rkt->setSector(sector);
         rkt->setShowerHitInd(pMetaMatch->getShowerHitInd());
         rkt->setTofHitInd(   pMetaMatch->getTofHitInd()   );
         rkt->setZ(    segment->getZ()     ,segment->getErrZ()     );
         rkt->setR(    segment->getR()     ,segment->getErrR()     );
         rkt->setTheta(segment->getTheta() ,segment->getErrTheta() );
         rkt->setPhi(  segment->getPhi()   ,segment->getErrPhi()   );

         rkt->setP(    -100000.0           ,0.0                    );
         rkt->setChiq( 1000000.0);
         rkt->setPolarity(0);
         rkt->setTof( tof );
         rkt->setTofDist(-1.0);
         rkt->setBeta(   -1.0);
         rkt->setMass2(   0.0, 0.0);
         rkt->setMetaEloss( 0.0 );
         rkt->setMETAdx( -10000.0 );
         rkt->setMETAdy( -10000.0 );
         rkt->setMETAdz( -10000.0 );
      }//-success-----------------------

      //rkt->setSystem(system);
      //rkt->setTofDistSP(track_lengthSP);
      //rkt->setRKthe(         RK_the );
      //rkt->setRKphi(         RK_phi );
      //rkt->setRKvertThe(     RKvert_the );
      //rkt->setRKvertPhi(     RKvert_phi );
      //rkt->setRKmdc12_theta( RK_seg_the );
      //rkt->setRKmdc12_phi(   RK_seg_phi );
      //rkt->setKineThe( Kine_the );    //TEST//
      //rkt->setKinePhi( Kine_phi );    //TEST// Help info from GeantKine
      //rkt->setPid(iGparticleMdcSeg1); //TEST//
      //rkt->setSeg2METAdx( Float_t(pointSeg2META[0]) );
      //rkt->setSeg2METAdy( Float_t(pointSeg2META[1]) );
      //rkt->setSeg2METAdz( Float_t(pointSeg2META[2]) );     
   }
   return rkt;
}

Bool_t HRKTrackBF::finalize(){
   if(bRKdebug){
      Hades::instance()->getOutputFile()->cd();
      fRKdebug->Write();
   }

   if(bMdcAlignMonitor){
      Hades::instance()->getOutputFile()->cd();
      for(Int_t s=0;s<6;s++){
      for(Int_t m=0;m<4;m++){
         dxMDC[ m][s]->Write();
         dyMDC[ m][s]->Write();
         dxxMDC[m][s]->Write();
         dyyMDC[m][s]->Write();
      }
      }
   }  

   // making snapshot of geometry used for Runge Kutta into ascii file for user to know it explicitly
   for(Int_t s=0; s<6; s++){
      if(bDumpGeomSector[s]){ pRungeKutta->writeGeomSnapshoftToHydraLikeFormSector(s); }
   }
   return kTRUE;
}
