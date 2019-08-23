//*-- Author : A.Sadovski (using Anar's Rustamov HSplineTrackF class as a pattern)
#include <iostream>
#include "hades.h"
//#include "hmetamatchF.h"
#include "hmetamatch.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofhitsim.h"
#include "hsplinetrackF.h"
#include "hkicktrack123B.h"
#include "hiterator.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "htofinogeompar.h"
#include "htofgeompar.h"
#include "hspecgeompar.h" 
#include "tofdef.h"
#include "hmdcseg.h"
#include "showerdef.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgfieldpar.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hgeantmdc.h"
#include "hkickplane2.h"
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
#include "hkicktrackbaseF.h"
using namespace std;
#include <iostream>
  
ClassImp(HKickTrackBaseF)
HKickTrackBaseF::HKickTrackBaseF()
{
   C=299792458;
   pi=acos(-1.);
   //fCatSplineTrack=0;
   fCatKickTrack123B=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   //Spline=0;
   fMetaMatchIter=0;
   field=0;
   corr=0;
}

HKickTrackBaseF::HKickTrackBaseF(const Text_t *name,const Text_t *title) : HReconstructor(name,title)
{
   C=299792458;
   pi=acos(-1.);
   //fCatSplineTrack=0;
   fCatKickTrack123B=0;
   fCatMdcSegSim=0;
   fMdcGeometry=0;
   fSpecGeomPar=0;
   //Spline=0;
   fMetaMatchIter=0;
   field=0;
   corr=0;
}

HKickTrackBaseF::~HKickTrackBaseF()
{
   ;
}

Bool_t HKickTrackBaseF::init()
{
   cout << "Initialisation of HKickTrackBaseF" << endl;
   
   if(gHades)
   {
	 for(Int_t i=0; i<6;i++) tRans[i]=0;
	   
	 HRuntimeDb *rtdb=gHades->getRuntimeDb();
	 HSpectrometer *spec=gHades->getSetup();
	 HEvent *event=gHades->getCurrentEvent();
	 if(rtdb && spec && event)
	    {
	       field =(HMdcTrackGFieldPar*)(rtdb->getContainer("MdcTrackGFieldPar"));
	       corr  =(HMdcTrackGCorrPar* )(rtdb->getContainer("MdcTrackGCorrPar") );
	       fSpecGeomPar=(HSpecGeomPar*)(rtdb->getContainer("SpecGeomPar")      );
	       fGetCont=HMdcGetContainers::getObject();
	       fGetCont->getMdcGeomPar();
	       fGetCont->getSpecGeomPar();
	       kickplane=(HKickPlane2*)rtdb->getContainer("KickPlane2MDC3");
	       
	       HDetector *pDtof=spec->getDetector("Tof");
	       if (pDtof) { //Has the user set up a TOF?
	          fTofGeometry = (HTofGeomPar *)rtdb->getContainer("TofGeomPar");
	       }else{
	          fTofGeometry=0;
	       }
	       
	       HDetector *pDshower = spec->getDetector("Shower");
	       HDetector *pDtofino = spec->getDetector("Tofino");
	       if(pDshower && pDtofino){
	          fTofinoGeometry = (HTofinoGeomPar *)rtdb->getContainer("TofinoGeomPar");
	       }
	       
	    } 
	 fCatMdcTrkCand=event->getCategory(catMdcTrkCand);
	 if(!fCatMdcTrkCand) return kFALSE;
	 fCatMetaMatch=event->getCategory(catMetaMatch);
	 if (!fCatMetaMatch) return kFALSE;
	 fMetaMatchIter=(HIterator*)fCatMetaMatch->MakeIterator();
	 if(!fMetaMatchIter) return kFALSE;
	 fCatMdcSegSim=event->getCategory(catMdcSeg);
	 fCatTof=event->getCategory(catTofHit);
	 if(!fCatTof) return kFALSE;
	 fCatKine=event->getCategory(catGeantKine);
	 if(!fCatKine)
	 {
	    fCatShower=event->getCategory(catShowerHitTof);
	    if(!fCatShower) return kFALSE;
	 }
	 else
	 {
	    fCatShower=event->getCategory(catShowerHitTofTrack);
	    if(!fCatShower) return kFALSE;
	 }

	 //fCatSplineTrack=event->getCategory(catSplineTrack);
	 //if(!fCatSplineTrack)
	 //{
	 //   Int_t size[2]={6,600};
	 //   fCatSplineTrack=new HMatrixCategory("HSplineTrack",2,size,0.5);  
	 //   if(fCatSplineTrack) event->addCategory(catSplineTrack,fCatSplineTrack,"SplineTrack");
	 //}
	 
	 //- Here we get category HKickTrack123B to use it as an output in "execute()"
	 fCatKickTrack123B=event->getCategory(catKickTrack123B);
	 if(!fCatKickTrack123B)
	 {
	    Int_t size[2]={6,600};
	    fCatKickTrack123B=new HMatrixCategory("HKickTrack123B",2,size,0.5);  
	    if(fCatKickTrack123B) event->addCategory(catKickTrack123B,fCatKickTrack123B,"KickTrack123B");
	 }
   }
   
   return kTRUE;
}

Bool_t HKickTrackBaseF::reinit()
{
   
   for(Int_t i=0; i<6; i++) tRans[i]=0;
   
   //This is something strange
   //Spline=corr->getSPline(); 
   //Spline->setDataPointer(field->getPointer(),corr->getCorr());
   //Spline->setKickPointer(kickplane);
   
   return kTRUE;
}

Bool_t HKickTrackBaseF::finalize()
{
   return kTRUE;
}

Int_t HKickTrackBaseF::execute()
{
   //cout << "HKickTrackBaseF::execute()" << endl;
   return kicktrackMDC123();
}

Int_t HKickTrackBaseF::kicktrackMDC123()
{
   HGeomVector fMetaNormVec;
   fMetaMatchIter->Reset();
   
   while((pMetaMatch=(HMetaMatch*)(fMetaMatchIter->Next()))!=0)
      {
         //cout << "loop over MetaMatch" << endl;
	 sector     = pMetaMatch->getSector();     //get current sector
	 indTrkCand = pMetaMatch->getTrkCandInd(); //index of track candidate
	 metaInd    = pMetaMatch->getMetaHitInd(); //index of META
	 pMdcTrkCand=(HMdcTrkCand*)fCatMdcTrkCand->getObject(indTrkCand); //pointer to track candidate
	 if(!pMdcTrkCand) continue;
	 index1=pMdcTrkCand->getSeg1Ind();    //index of MDC12  segment of the current track
	 index2=pMdcTrkCand->getSeg2Ind();    //index of MDC3?4 segment of the current track
	 metaInd=pMetaMatch->getMetaHitInd(); //index of META for the current track
	 sectorloc.set(1,(Int_t)sector);
	 segments[0]=(HMdcSeg*)fCatMdcSegSim->getObject(index1); //pointer to a segment MDC12
	 segments[1]=(HMdcSeg*)fCatMdcSegSim->getObject(index2); //pointer to a segment MDC3?4
	 system=pMetaMatch->getSystem();
	 //cout << "system="<< system << endl;
	 
	 if(!segments[0] || !segments[1]) continue; //if one of segments(mdc12) or (mdc3-?-4) is not defined then skip one loop

         
	 if(metaInd>=0){ 
	    //If we have information from META detector - so first lets retreive it, we'll need it for time of flight, beta and mass computations

  	    if(system==0){ //- TOFINO/SHOWER-case
  		pShowerHitTofTrack=(HShowerHitTofTrack*)fCatShower->getObject(metaInd);
  		tof=pShowerHitTofTrack->getTof();
  		pShowerHitTofTrack->getLabXYZ(&xTof,&yTof,&zTof);
		
		if(fTofinoGeometry){
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
		   fMetaNormVec = rz_lab - r0_lab;
		}else{
		   cout << "No Shower/Tofino geometry specified" << endl;
		}
  	    }


  	    if(system==1){ //- TOF-case
  		pTofHit=(HTofHitSim*)fCatTof->getObject(metaInd);
  		tof=pTofHit->getTof();
  		pTofHit->getXYZLab(xTof,yTof,zTof);

                //for sysstem encoding
		tofSystem =  pTofHit->getSector()*1000 + pTofHit->getModule()*100 + pTofHit->getCell();

		//for MetaEloss calculation
		//HTofGeomPar *fTofGeometry; //!TOF geometry.
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
		   fMetaNormVec = rz_lab - r0_lab;
		}else{
		   cout << "No Tof geometry specified" << endl;
		}
  	    }
  	      	    
  	    pointMeta.setXYZ(xTof,yTof,zTof); //defines point on META detector from TOF or TOFINO/SHOWER in a Laboratory reference system
  	    
  	    if(tRans[(Int_t)sector]==0){ //- if no transformation is defined for the sector, then define it
  	       tRans[(Int_t)sector]=new HGeomTransform();
  	       if(!fGetCont->getLabTransSec(*(tRans[(Int_t)sector]),sector)) continue; //if not possible - skip this sector
  	    }

  	    pointMeta=tRans[(Int_t)sector]->transTo(pointMeta); //we transform META-hit into MDC-sector system
  	    xTof=pointMeta.getX();
  	    yTof=pointMeta.getY();
  	    zTof=pointMeta.getZ();
         }
	 
	 //- Here comes momentum calculation - which can be done without META is we have at least 3 MDCs
	 //- OK, momentum without meta is not so interesting, scince we can not compute mass of particle
	 HGeomVector interkickpoint; //intersection point of track before magnetic field and the kickplane surface
	 HGeomVector dir1, dir2;     //direction of track in a segment before and after the magnetic field
	 HGeomVector point[4];       //four points - two first are on the incoming track (before the magnet), other two are on the outgoing track (after the magnet)
	 if((segments[1]->getHitInd(1)==-1&&segments[1]->getHitInd(0)!=-1)||
	    (segments[1]->getHitInd(1)!=-1&&segments[1]->getHitInd(0)==-1)  )
	 { // FZR but no ORASY or No FZR but ORSAY if there is no 4-th MDC installed
	    //case of MDC12-3
	    //SplineFit stuff
	    //Momentum=corr->calcMomentum123(segments,kTRUE,segments[0]->getZ());
	    
	    //KickPlane stuff see for example: hmdctrackgspline.cc
  	    Float_t tetaseg[2],phiseg[2],zseg[2],roseg[2];
  	    tetaseg[0]=segments[0]->getTheta(); // 
  	    phiseg[ 0]=segments[0]->getPhi();   // Here we get (r,z,theta,phi)
  	    zseg[   0]=segments[0]->getZ();     // from first segment (before the magnet)
  	    roseg[  0]=segments[0]->getR();     //
  	    
  	    tetaseg[1]=segments[1]->getTheta(); //
  	    phiseg[ 1]=segments[1]->getPhi();   // Here we get (r,z,theta,phi)
  	    zseg[   1]=segments[1]->getZ();     // from second segment (after the magnet)
  	    roseg[  1]=segments[1]->getR();     //
  	    
  	    Double_t Xseg[4],Yseg[4],Zseg[4];
  	    
  	    Xseg[0]=roseg[0]*cos(phiseg[0]+pi/2);   //
  	    Yseg[0]=roseg[0]*sin(phiseg[0]+pi/2);   // Point[0] begining of the first segment
  	    Zseg[0]=zseg[0];                        //
  	    Xseg[1]=Xseg[0]+cos(phiseg[0])*sin(tetaseg[0]);  //
  	    Yseg[1]=Yseg[0]+sin(phiseg[0])*sin(tetaseg[0]);  // Point[1] end-point of the first segment
  	    Zseg[1]=Zseg[0]+cos(tetaseg[0]);                 //
	    
	    //computation of track direction from the first segment (before magnetic field)
  	    Double_t dirX=Xseg[1]-Xseg[0];
  	    Double_t dirY=Yseg[1]-Yseg[0];
  	    Double_t dirZ=Zseg[1]-Zseg[0];

	    //Intersection with kickplane
	    HGeomVector pointSeg1; //point and direction {"dir1"} of track in first segment - before magnetic field
	    pointSeg1.setXYZ(Xseg[0],Yseg[0],Zseg[0]);
	    dir1.setXYZ(dirX,dirY,dirZ);
	    //cout << "|dir1|="<< dir1.length() << "must be == 1"<< endl;
	    kickplane->calcIntersection(pointSeg1,dir1,interkickpoint);
	    
	    Xseg[2]=roseg[1]*cos(phiseg[1]+pi/2);  //
  	    Yseg[2]=roseg[1]*sin(phiseg[1]+pi/2);  // Point[2] begining point of the second segment (after magnet)
  	    Zseg[2]=zseg[1];                       //
  	    Xseg[3]=Xseg[2]+cos(phiseg[1])*sin(tetaseg[1]);  //
  	    Yseg[3]=Yseg[2]+sin(phiseg[1])*sin(tetaseg[1]);  // Point[3] ending point of the second segment (after magnet)
  	    Zseg[3]=Zseg[2]+cos(tetaseg[1]);                 //

            //Now we know first and second segment as 4 points representation
	    point[0].setXYZ(0.1*Xseg[0], 0.1*Yseg[0], 0.1*Zseg[0]);
	    point[1].setXYZ(0.1*Xseg[1], 0.1*Yseg[1], 0.1*Zseg[1]);
	    point[2].setXYZ(0.1*Xseg[2], 0.1*Yseg[2], 0.1*Zseg[2]);
	    point[3].setXYZ(0.1*Xseg[3], 0.1*Yseg[3], 0.1*Zseg[3]);
	    
	    //lets calculate track deflection angle in magnetic field
	    dir2 = point[3] - point[2];
	    Float_t dir2length = dir2.length();
	    dir2 /= dir2length; //normalised vector of track direction after the magnet
	    //cout << "=3---------------------123->>     dir2 = " << dir2.getX()     << ", " << dir2.getY()     << ", " << dir2.getZ()     << endl;
	    //cout << "|dir2|="<< dir2.length() << "must be == 1"<< endl;
	    Double_t fAngle = TMath::ACos(dir1.scalarProduct(dir2)); //here we use that dir and dir2 are normalised vectors by construction (cheeckIt!!)
	    
	    //Now lets decide on polarity
	    Double_t beta1, beta2, deltaBeta;
	    beta1 = TMath::ATan2( dir1.getZ(), dir1.getY() ); //possibly need to be exchanged Y<->Z
	    beta2 = TMath::ATan2( dir2.getZ(), dir2.getY() ); //possibly need to be exchanged Y<->Z
	    deltaBeta = beta2 - beta1;
	    if(deltaBeta>=0){polarity=1;}else{polarity=-1;}
	    
	    Momentum = kickplane->getP(interkickpoint, 2.*sin(fAngle/2.), polarity);
	    //cout << "pMDC123 = " << Momentum << endl;
	 }else{
	    //here comes the case then we have 4 MDCs
	    //cout << "MDC12-MDC34 kickplane momentum reconstruction is not implemented yet" << endl;
	    
	    //KickPlane stuff see for example: hmdctrackgspline.cc
  	    Float_t tetaseg[2],phiseg[2],zseg[2],roseg[2];
  	    tetaseg[0]=segments[0]->getTheta(); // 
  	    phiseg[ 0]=segments[0]->getPhi();   // Here we get (r,z,theta,phi)
  	    zseg[   0]=segments[0]->getZ();     // from first segment (before the magnet)
  	    roseg[  0]=segments[0]->getR();     //
  	    
  	    tetaseg[1]=segments[1]->getTheta(); //
  	    phiseg[ 1]=segments[1]->getPhi();   // Here we get (r,z,theta,phi)
  	    zseg[   1]=segments[1]->getZ();     // from second segment (after the magnet)
  	    roseg[  1]=segments[1]->getR();     //
  	    
  	    Double_t Xseg[4],Yseg[4],Zseg[4];
  	    
  	    Xseg[0]=roseg[0]*cos(phiseg[0]+pi/2);   //
  	    Yseg[0]=roseg[0]*sin(phiseg[0]+pi/2);   // Point[0] begining of the first segment
  	    Zseg[0]=zseg[0];                        //
  	    Xseg[1]=Xseg[0]+cos(phiseg[0])*sin(tetaseg[0]);  //
  	    Yseg[1]=Yseg[0]+sin(phiseg[0])*sin(tetaseg[0]);  // Point[1] end-point of the first segment
  	    Zseg[1]=Zseg[0]+cos(tetaseg[0]);                 //
	    
	    //computation of track direction from the first segment (before magnetic field)
  	    Double_t dirX=Xseg[1]-Xseg[0];
  	    Double_t dirY=Yseg[1]-Yseg[0];
  	    Double_t dirZ=Zseg[1]-Zseg[0];

	    //Intersection with kickplane
	    HGeomVector pointSeg1; //point and direction {"dir1"} of track in first segment - before magnetic field
	    pointSeg1.setXYZ(Xseg[0],Yseg[0],Zseg[0]);
	    dir1.setXYZ(dirX,dirY,dirZ);
	    //cout << "|dir1|="<< dir1.length() << "must be == 1"<< endl;
	    kickplane->calcIntersection(pointSeg1,dir1,interkickpoint);
	    
	    Xseg[2]=roseg[1]*cos(phiseg[1]+pi/2);  //
  	    Yseg[2]=roseg[1]*sin(phiseg[1]+pi/2);  // Point[2] begining point of the second segment (after magnet)
  	    Zseg[2]=zseg[1];                       //
  	    Xseg[3]=Xseg[2]+cos(phiseg[1])*sin(tetaseg[1]);  //
  	    Yseg[3]=Yseg[2]+sin(phiseg[1])*sin(tetaseg[1]);  // Point[3] ending point of the second segment (after magnet)
  	    Zseg[3]=Zseg[2]+cos(tetaseg[1]);                 //

            //Now we know first and second segment as 4 points representation
	    point[0].setXYZ(0.1*Xseg[0], 0.1*Yseg[0], 0.1*Zseg[0]);
	    point[1].setXYZ(0.1*Xseg[1], 0.1*Yseg[1], 0.1*Zseg[1]);
	    point[2].setXYZ(0.1*Xseg[2], 0.1*Yseg[2], 0.1*Zseg[2]);
	    point[3].setXYZ(0.1*Xseg[3], 0.1*Yseg[3], 0.1*Zseg[3]);
	    
	    //lets calculate track deflection angle in magnetic field
	    dir2 = point[3] - point[2];
	    Float_t dir2length = dir2.length();
	    dir2 /= dir2length; //normalised vector of track direction after the magnet
    	    //cout << "=3---------------------1234>>     dir2 = " << dir2.getX()     << ", " << dir2.getY()     << ", " << dir2.getZ()     << endl;
	    //cout << "|dir2|="<< dir2.length() << "must be == 1"<< endl;
	    Double_t fAngle = TMath::ACos(dir1.scalarProduct(dir2)); //here we use that dir and dir2 are normalised vectors by construction (cheeckIt!!)
	    
	    //Now lets decide on polarity
	    Double_t beta1, beta2, deltaBeta;
	    beta1 = TMath::ATan2( dir1.getZ(), dir1.getY() ); //possibly need to be exchanged Y<->Z
	    beta2 = TMath::ATan2( dir2.getZ(), dir2.getY() ); //possibly need to be exchanged Y<->Z
	    deltaBeta = beta2 - beta1;
	    if(deltaBeta>=0){polarity=1;}else{polarity=-1;}
	    
	    Momentum = kickplane->getP(interkickpoint, 2.*sin(fAngle/2.), polarity);
	    //cout << "pMDC123 = " << Momentum << endl;
	 }


         if(metaInd<0){ //if no META then we can not read TOF and TOFINO/SHOWER information => just write the momentum only
	    if(Momentum<3000){  //only for reasonable momentum
	       fillData(segments[0],kFALSE); //write the momentum, but do not write META information as it does not exist
	    }
	    continue;
	 }

         //If we come by now - that mean we have META and we analyse it to get beta, and mass
	 //First let us find the track length target-->kickplane-->META
	 const HGeomVector &targetPos = fSpecGeomPar->getTarget(0)->getTransform().getTransVector();
	 Float_t distanceTarget2kickPlane = (interkickpoint - targetPos).length();
	 Float_t distanceKickPlane2META   = (interkickpoint - pointMeta).length();
	 
	 //cout << "Dist(Targ-KickPlane)=" << distanceTarget2kickPlane << endl;
	 //cout << "Dist(KickPlane-META)=" << distanceKickPlane2META   << endl;
	 TOFdistance = distanceTarget2kickPlane + distanceKickPlane2META; //please note there is another way to compute the distance see htrtrackevaluator.cc for more details
	 beta  = 1.0e6*TOFdistance/tof/C;
	 mass2 = Momentum*Momentum*(1-beta*beta)/beta/beta;
	 
	 //And here some auxiliary information: for the case of META we can use META instead of 4-th MDC to compare
	 //with pure-MDC momentum. So here we calculate (segment1)<-->(MDC3-META) momentum
	 //lets calculate track deflection angle in magnetic field using segment before the magnetic field and MDC3-META after the magnet
	 HGeomVector dir2M = pointMeta - point[2];
	 Float_t dir2Mlength = dir2M.length();
	 dir2M /= dir2Mlength; //normalised vector of track direction after the magnet
	 //cout << "|dir2M|="<< dir2M.length() << "must be == 1"<< endl;
	 Double_t fAngleM = TMath::ACos(dir1.scalarProduct(dir2M)); //here we use that dir and dir2M are normalised vectors by construction (cheeckIt!!)

         //We trust the polarity obtained from MDCs and kickpoint obtained from the segment before the magnetic field, OK? 
	 //If not calculate here your polarity and/or new kickplane position.
	 MomentumMETA = kickplane->getP(interkickpoint, 2.*sin(fAngleM/2.), polarity);
	 
	 //Here we calculate Meta energy loss
	 //we use Track after magnet as a direction pointing to META and we use it's angle to META's normal vector
	 HGeomVector metanorm = fMetaNormVec;
	 Float_t zprojvec_lab = dir2.scalarProduct(metanorm);
	 metanorm *= zprojvec_lab;

         Float_t path, edep;
	 if(system==0){ // Shower-Tofino
	    path = ( metanorm.length() * dir2.length() )/dir2.scalarProduct(metanorm);
	    edep = ( (HShowerHitTof *) pShowerHitTofTrack)->getADC();
	    if(path >= 1.0){
	       eloss = edep/path;
	    }else{
	       Warning("evaluateMDC123(4)","particle crossed TOFINO with path < 1.\n");
	       eloss = -1.0;
	    }
	 }
	 
	 
	 if(system==1){ // TOF         
            path = (metanorm.length()/dir2.length()) / dir2.scalarProduct(metanorm);
            if(pTofHit->getAdcFlag() !=0 ){
               edep = pTofHit->getEdep();
               if(path >= 1.0){
                  eloss = edep/path;
               }else{
                  Warning("evaluateMDC123(4)","particle crossed TOF wall with path < 1.\n");
                  eloss = -1.0;
               }
            }else{
               eloss = -1.0;
            }
	 }
	 
	 if(Momentum<3000) fillData(segments[0],kTRUE); //here we write META information
      }
   return 0;
} 

HKickTrack123B * HKickTrackBaseF::fillData(HMdcSeg *segment, Bool_t condition){

   Int_t indexkicktrack123b;
   HKickTrack123B *kt123b = (HKickTrack123B*)fCatKickTrack123B->getNewSlot(sectorloc,&indexkicktrack123b);
   if(!kt123b){
      Error("fillData","No slot available");
      return 0;
   }

   kt123b = (HKickTrack123B*)(new(kt123b)HKickTrack123B);
   if(kt123b){
      //cout << "HKickTrackBaseF::fillData" << endl;
      kt123b->setP(Momentum,0);
      kt123b->setZ(segment->getZ(),segment->getErrZ());
      kt123b->setR(segment->getR(),segment->getErrR());
      kt123b->setTheta(segment->getTheta(),segment->getErrTheta());
      kt123b->setPhi(segment->getPhi(),segment->getErrPhi());
      kt123b->setPolarity(polarity);
      kt123b->setSector(sector);
      pMetaMatch->setKick123Ind(indexkicktrack123b);
      if(condition){
         //with META
         kt123b->setTofDist(TOFdistance);
	 kt123b->setBeta(beta);
	 kt123b->setMass2(mass2,0);
	 kt123b->setTof(tof);
         kt123b->setTofHitInd(pMetaMatch->getTofHitInd());
         kt123b->setShowerHitInd(pMetaMatch->getShowerHitInd());
	 kt123b->setPMeta(MomentumMETA);
	 kt123b->setMetaEloss(eloss);
      }else{
         //without META
         kt123b->setTofDist(-1000.);
	 kt123b->setBeta(-1.);
	 kt123b->setMass2(-1000.,0);
	 kt123b->setTof(-1000.);
	 kt123b->setPMeta(0.);
	 kt123b->setMetaEloss(-1.);
      }
   }else{
      Error("HKickTrackBaseF::FillData","No slots free");
   }
   return kt123b;
}

//- here data are written into the coresponding output container
//HSplineTrack *  HSplineTrackF::fillData(HMdcSeg *segment,Bool_t condition)
//{
//   Int_t indexspline;
//   HSplineTrack *sp=(HSplineTrack*)fCatSplineTrack->
//	getNewSlot(sectorloc,&indexspline);
//   if(!sp)
//	{
//	  cout<<"No slot available"<<endl;
//	  return 0;
//	}
//   sp=(HSplineTrack*)(new(sp)HSplineTrack);
//   if(sp)
//	{
//	   sp->setP(Momentum,0);
//	   sp->setZ(segment->getZ(),segment->getErrZ());
//	   sp->setR(segment->getR(),segment->getErrR());
//	   sp->setTheta(segment->getTheta(),segment->getErrTheta());
//	   sp->setPhi(segment->getPhi(),segment->getErrPhi());
//	   sp->setPolarity(polarity);
//	   sp->setSector(sector);
//	   pMetaMatch->setSplineInd(indexspline);
//	   if(condition)
//	      {
//		// sp->setP(Momentum,0);
//		 sp->setTofDist(TOFdistance);
//		 sp->setBeta(beta);
//		 sp->setBetaCalc(betaCalc);
//		 sp->setMass2(mass2,0);
//		 sp->setTof(tof);
//		 sp->setSystem(system);
//	      }
//	}
//   else
//	Error("FillData","No slots free");
//   return sp;
//}
