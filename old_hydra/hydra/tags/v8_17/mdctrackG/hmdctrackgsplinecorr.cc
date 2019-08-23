//*--AUTHOR :  Anar Rustamov
//class fo calculation of spline parameters.
//In case of 4 MDC we take points from each MDC(the senter of MDC), and calculate intersection of line formed with thiese points with fixed plane, and the later points are then taken as an input.
//In case of three MDC the procedure is the same.
//In case of META hit the third point is from the kickplane
using namespace std;
#include "hmagnetpar.h"
#include "hmetamatchF.h"
#include "hevent.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcdetector.h"
#include "hspecgeompar.h"
#include "tofdef.h"
#include "richdef.h"
#include "triggerinfodef.h"
#include "hmatrixcategory.h"
#include "hbasetrack.h"
#include "showertofinodef.h"
#include "hmetamatch.h"
#include "hgeomtransform.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "hmdcseg.h"
#include "hkickplane2.h"
#include "hmdctrkcand.h"
#include "hmdctrackgspline.h"
#include "hgeomvector.h"
#include "hlocation.h"
#include "hshowerhittoftrack.h"
#include "hmdcgetcontainers.h"
#include "htofhit.h"
#include "htofcluster.h"
#include "hrichhit.h"
#include "hrichhitIPU.h"
#include <iostream>
#include "TFile.h"
#include "TNtuple.h"
#include "hmdctrackgsplinecorr.h"

#include "hgeantkine.h"
#include "hmdcdetector.h"
#include "hlinkeddataobject.h"
#include"hreconstructor.h"
#include "hades.h"
#include <math.h>
#include "hmdcsegsim.h"
#include "hmdchitsim.h"
#include "hkickplane2.h"
#include "hfield.h"
#include "hmdctrackgfieldpar.h"
#include "hmdctrackgcorrpar.h"
#include "hmdctrackgspline.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hmdctrackddef.h"
#include "hevent.h"
#include "hgeomtransform.h"
#include "hmatrixcatiter.h"
#include "hgeantmdc.h"
#include "hmdcgetcontainers.h"
#include "hmatrixcategory.h"
#include "hmdctrackddef.h"
#include "hgeomvector.h"
#include "hgeantkine.h"
#include "TRandom.h"


ClassImp(HMdcTrackGSplineCorr)
HMdcTrackGSplineCorr::HMdcTrackGSplineCorr()
{
fCatMdc=0; 
  fMdcIter=0;
  fCatKine=0;
  fMdcGeometry=0;
  fSpecGeometry=0; 
  field=0;
  corr=0;
    for(Int_t s=0; s<6; s++)
    for(Int_t m=0; m<4; m++)
    for (Int_t l=0; l<7; l++)
    trLS[s][m][l]=0;
SetFileName();
SetOutDir();

}
HMdcTrackGSplineCorr::HMdcTrackGSplineCorr(Text_t *name,Text_t *title)
   : HReconstructor(name,title)
{
  ismdc1234=kTRUE;
  fCatMdc=0; 
  fMdcIter=0;
  fCatKine=0;
  fMdcGeometry=0;
  fSpecGeometry=0; 
  field=0;
  corr=0;
  Spline=0;
  
    for(Int_t s=0; s<6; s++)
    for(Int_t m=0; m<4; m++)
    for (Int_t l=0; l<7; l++)
     trLS[s][m][l]=0;
    SetFileName();
    SetOutDir();
}

HMdcTrackGSplineCorr::~HMdcTrackGSplineCorr(void)
{
//  if (fMdcIter) delete fMdcIter;
//  if (fKineIter) delete fKineIter;
}

void HMdcTrackGSplineCorr::SetFileName(TString cTemp)
{
  fileName=cTemp;
}

void HMdcTrackGSplineCorr::SetOutDir(TString cTemp)
{
  outDir=cTemp;
}
   
   
Bool_t HMdcTrackGSplineCorr::init(void)
{  //init(void)
  if(gHades) 
    {
     
     // HMdcTrackGSpline *Spline=new HMdcTrackGSpline;
      nt4 = new TNtuple("nt4","nt4","theta:phi:pGeant:pRecon:pol:sec");
      nt3 = new TNtuple("nt3","nt3","theta:phi:pGeant:pRecon:pol:sec");
      HRuntimeDb *rtdb=gHades->getRuntimeDb();
      HSpectrometer *spec=gHades->getSetup();
      HEvent *event=gHades->getCurrentEvent();
      if(rtdb && spec && event)
	{
	  //HDetector *mdc=spec->getDetector("Mdc");
	  field=(HMdcTrackGFieldPar*)(rtdb->getContainer("MdcTrackGFieldPar"));
          corr=(HMdcTrackGCorrPar*)(rtdb->getContainer("MdcTrackGCorrPar"));
	  fSpecGeometry = (HSpecGeomPar *)(rtdb->getContainer("SpecGeomPar"));
	  fMdcGeometry=(HMdcGeomPar *)(rtdb->getContainer("MdcGeomPar"));
	  pMagnet=(HMagnetPar*)(rtdb->getContainer("MagnetPar"));
		
	  kickplane = HKickPlane2::getMDC3();
	  if(!kickplane) printf("YESSS DO IT!!!\n");
	  //*************
	  fGetCont=HMdcGetContainers::getObject();
	  fGetCont->getMdcGeomPar();
	  fGetCont->getSpecGeomPar();
	  //***************
	  
	 
	}
      fCatMdc=event->getCategory(catMdcGeantRaw);
      if (!fCatMdc) return kFALSE;
      fCatKine=event->getCategory(catGeantKine);
      if(!fCatKine) return kFALSE;
      fMdcIter=(HIterator*)fCatMdc->MakeIterator();
      if(!fMdcIter) return kFALSE;
      fKineIter=(HIterator*)fCatKine->MakeIterator();
      if(!fCatKine)return kFALSE;
    }
    //cout<<"INIT DONE"<<endl;
  return kTRUE;
}

Bool_t HMdcTrackGSplineCorr::reinit(void)
{
HMdcTrackGCorrections *corrScan[3];
corr->getCorrScan(corrScan);
 
//cout<<"R E I N I T0000000"<<endl;

  //evHeader=gHades->getCurrentEvent()->getHeader();
//for(Int_t i=0; i<6; i++)
//tRans[i]=0;
Spline=corr->getSPline();
Spline->setCorrScan(corrScan);

//cout<<"R E I N I T111111"<<endl;

Spline->initMiddleParams(Spline->takeMiddleParams(fGetCont,0,2));
//cout<<"R E I N I T111111"<<endl;


Spline->setDataPointer(field->getPointer(),corr->getCorr1());

//cout<<"R E I N I T111111"<<endl;

Spline->setKickPointer(kickplane);


//cout<<"R E I N I T111111KICK"<<endl;

if(pMagnet->getPolarity()>=0)
{
Spline->setMagnetScaling(pMagnet->getScalingFactor());
fScal=pMagnet->getScalingFactor();
}
else
{
Spline->setMagnetScaling(pMagnet->getScalingFactor()*(-1.));
fScal=pMagnet->getScalingFactor()*(-1.);
}


//cout<<"R E I N I T"<<endl;
				    
return kTRUE;

}

Bool_t HMdcTrackGSplineCorr::finalize(void)
{
TFile f(outDir+fileName+"outputOld"+".root","recreate");
nt4->Write();
nt3->Write();
f.Close();

return kTRUE;
}
Int_t HMdcTrackGSplineCorr::execute(void)
{

//cout<<"E X E C U T E"<<endl;
  //  if(ismdc1234)  //whith 4 chambers
  return  mdc1234();
  //  if(ismdc123)
  //      mdc123(); //with 3 chambers
  //if (ismdcmeta) //with meta,this is under development, and not ready yet;
  //  mdcmeta();
  //  return 0;
  
}
Int_t HMdcTrackGSplineCorr::mdc1234()
{ //void mdc1234
  
  //ut<<"START EVENT" <<endl;
//  HGeomVector POINT[4],POINT3Cham;
  
  HGeantKine *pGeantKine=0;
  pGeantMdc=0;
  fKineIter->Reset();
  while((pGeantKine=(HGeantKine*)fKineIter->Next())!=0)
    { //loop over kine
       //cout<<"KINEYE GIRDIK"<<endl;
      Int_t flag1=0,flag2=0,flag3=0,flag4=0;
      pKine=pGeantKine->getTotalMomentum();
      pGeantKine->getMomentum(px,py,pz);
      phi=atan2(py,px)*180/acos(-1.);
      theta=acos(pz/pKine)*180/acos(-1.);
      pGeantKine->getParticle(tracknd,ID);
      Float_t xV,yV,zV;
      pGeantKine->getVertex(xV,yV,zV);
     
//      pGeantKine->setMdcCategory(fCatMdc);
	
      pGeantKine->resetMdcIter();
         //cout<<"KINEYE GIRDIK22"<<endl;

//	 cout<<"T E S T "<<pGeantKine->nextMdcHit()<<endl;
	 
      while((pGeantMdc=(HGeantMdc*)pGeantKine->nextMdcHit())!=0)
	{ //loop over geant 
	   //cout<<"pGeantMdc   "<<pGeantMdc<<endl;
	   //cout<<"GeantMdcye girdik"<<endl;
	  
	  if(pGeantMdc->getSector()==0 && pGeantMdc->getModule()==0 && pGeantMdc->getLayer()==6)
	    {
	      
	      //ut<<"First point"<<endl;
	      
	      flag1=1;
	     if(!takePoint(pGeantMdc,POINT[0],p06,0,0,6)) {flag1=0; continue;}
	    }
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==1 && pGeantMdc->getLayer()==5)
	    {
	      
	      //ut<<"Second point"<<endl;
	      
	      
	      flag2=1;
	      if(!takePoint(pGeantMdc,POINT[1],p16,0,1,5)) {flag2=0; continue;}
	      
	    }
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==2 && pGeantMdc->getLayer()==0)
	    {
	      
	      //   cout<<"Third point"<<endl;
	      
	      flag3=1;
	      if(!takePoint(pGeantMdc,POINT[2],p26,0,2,0)) {flag3=0; continue;}
	      
	    }
	  
	  
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==2 && pGeantMdc->getLayer()==6)
	              {
		        //flag3=1;
			if(!takePoint(pGeantMdc,POINT3Cham,p26,0,2,6)) continue;
		       }
									    
	  
	  
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==3 && pGeantMdc->getLayer()==6)
	    {
//   cout<<"Fourth point"<<endl;

	      flag4=1;
	     if (!takePoint(pGeantMdc,POINT[3],p36,0,3,6)) {flag4=0; continue;}
	     
	      
//   cout<<"again"<<endl;
	          
	      break;		    
	    }
	}
      if(flag1 && flag2 && flag3 && flag4)
	{	  
	Spline->setZGlobal(zV);
	  pRecon=Spline->calcMomentum(POINT,kTRUE,zV,4);
	  
	  //POINT[3]=POINT3Cham; 
	    calcChamber3Mode(POINT);
	    POINT[3]=POINT3Cham;
	    POINT[0]*=0.1;     
	  pRecon3Cham=Spline->calcMomentum(POINT,kTRUE,zV,3);
	  //calculations in case of 3 chambers
          
	  pGeant=p16;
	  
	  
	  RadiusV=sqrt((POINT[1].getX()-POINT[0].getX())*(POINT[1].getX()-POINT[0].getX())+
	                (POINT[1].getY()-POINT[0].getY())*(POINT[1].getY()-POINT[0].getY())+
			(POINT[1].getZ()-POINT[0].getZ())*(POINT[1].getZ()-POINT[0].getZ()));
	  
//          theta=acos((POINT[1].getZ()-POINT[0].getZ())/RadiusV)*180/acos(-1.);
//	  phi=atan2((POINT[1].getY()-POINT[0].getY()),(POINT[1].getX()-POINT[0].getX()))*180/acos(-1.);
	  
	  nt4->Fill(theta,phi,pGeant,pRecon,Spline->getPolarity(),pGeantMdc->getSector());
	  nt3->Fill(theta,phi,pGeant,pRecon3Cham,Spline->getPolarity(),pGeantMdc->getSector());
	  // cout<<"prec=  "<<pRecon<<endl;
	  // cout<<"pgeant=  "<<pGeant<<endl;
	}
    }
  return 0;
}

//For the case with 3 chambers;









Bool_t HMdcTrackGSplineCorr::takePoint(HGeantMdc *data,HGeomVector &plab, Float_t &momentum,
Int_t sector,Int_t module,Int_t layer)
{ // void takePoint
  //  trans=kFALSE;
  Float_t mom; 
  HGeomVector p;
  Float_t x=0.,y=0.,z=0.,tof=0.;
  data->getHit(x,y,tof,mom);
  momentum=mom;
  p.setXYZ(x,y,0.);
//  HModGeomPar *module=fMdcGeometry->getModule(data->getSector(),data->getModule());
//  HGeomTransform &transMod = module->getLabTransform();
//  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
//  HGeomTransform modTrans(transMod);
//  // if(!modTrans) trans=kFALSE;
//  modTrans.transTo(transSec);
//  plab=modTrans.transFrom(p);

               
if(trLS[sector][module][layer]==0){

	    trLS[sector][module][layer]=new HGeomTransform();

if(layer==6)
{
 if(!fGetCont->getLabTransMod(*(trLS[sector][module][layer]),sector,module)) return kFALSE;
}
else
{
if(!fGetCont->getLabTransLayerZ0(*(trLS[sector][module][layer]),sector,module,layer)) return kFALSE;
}

}
plab=trLS[sector][module][layer]->transFrom(p); 

plab*=0.1;
//Float_t xlab=plab.getX();
//Float_t ylab=plab.getY();
//Float_t zlab=plab.getZ();
//plab.setXYZ(0.1*xlab,0.1*ylab,0.1*zlab);

return kTRUE;
}

void HMdcTrackGSplineCorr::calcChamber3Mode(HGeomVector *POINT)
{
   Double_t dirX=POINT[1].getX()-POINT[0].getX();
   Double_t dirY=POINT[1].getY()-POINT[0].getY();
   Double_t dirZ=POINT[1].getZ()-POINT[0].getZ();
   HGeomVector point,dir,interkickpoint,interpoint;
//interkickpoint - intersection point with kickplane
	          //interpoint - intersection with the middle of 3 chamber
	POINT[0]*=10;
	point=POINT[0];
	
	dir.setXYZ(10*dirX,10*dirY,10*dirZ);
	kickplane->calcIntersection(point,dir,interkickpoint);
	//interkickpoint in mm
	interkickpoint*=0.1;
	POINT[2]=interkickpoint; //changing to cm
	//Double_t A=0.,B=1.1547, D=210.292;
	//calcInter(A,B,D,PPOINT[2],PPOINT[3],interpoint);
	//interpoint in cm
					     
	//PPOINT[3]=interpoint;
	//   cout<<POINT[3].getZ()<<endl;
}
						

