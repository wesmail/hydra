//*--AUTHOR :  Anar Rustamov
//class fo calculation of spline parameters.
//In case of 4 MDC we take points from each MDC(the senter of MDC), and calculate intersection of line formed with thiese points with fixed plane, and the later points are then taken as an input.
//In case of three MDC the procedure is the same.
//In case of META hit the third point is from the kickplane
using namespace std;
#include <iostream>
#include <iostream>
#include "hades.h"
#include "hcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "hiterator.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "hspecgeompar.h" 
#include "hmdctrackgcorrpar.h"
#include "hgeantmdc.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hmdctrackddef.h"
#include "hruntimedb.h"
#include "hevent.h"
#include "hmdcgetcontainers.h"
#include "hmdctrackgspline.h"
#include "hmdctrackgcorrpar.h"
#include "hgeomvector.h"
#include "TNtuple.h"
#include "hmdctrackgsplinecorr.h"
#include "TFile.h"
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
}

HMdcTrackGSplineCorr::~HMdcTrackGSplineCorr(void)
{
//  if (fMdcIter) delete fMdcIter;
//  if (fKineIter) delete fKineIter;
}
Bool_t HMdcTrackGSplineCorr::init(void)
{  //init(void)
  if(gHades) 
    {
     
     // HMdcTrackGSpline *Spline=new HMdcTrackGSpline;
      nt = new TNtuple("corr","corr","theta:phi:pGeant:pRecon:pKine:ID:dist0:dist1:dist2:dist3");
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
    cout<<"INIT DONE"<<endl;
  return kTRUE;
}

Bool_t HMdcTrackGSplineCorr::reinit(void)
{
Double_t xvec[3]={53.,31.,44.};
	  Double_t fvec[3];
field->calcField(xvec,fvec,1.);
	  cout<<"FIELD"<<fvec[0]<<"  "<<fvec[1]<<endl;
corr->getSPline()->setDataPointer(field->getPointer(), corr->getCorr());

return kTRUE;
}

Bool_t HMdcTrackGSplineCorr::finalize(void)
{
TFile f("test.root","recreate");
nt->Write();
f.Close();

return kTRUE;
}
Int_t HMdcTrackGSplineCorr::execute(void)
{
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
  HGeomVector POINT[4];
  HGeantKine *pGeantKine=0;
  HGeantMdc *pGeantMdc=0;
  fKineIter->Reset();
  while((pGeantKine=(HGeantKine*)fKineIter->Next())!=0)
    { //loop over kine
      Int_t flag1=0,flag2=0,flag3=0,flag4=0;
      pKine=pGeantKine->getTotalMomentum();
      pGeantKine->getMomentum(px,py,pz);
      phi=atan2(py,px);
      theta=acos(pz/pKine);
      pGeantKine->getParticle(tracknd,ID);
      pGeantKine->resetMdcIter();
      while((pGeantMdc=(HGeantMdc*)pGeantKine->nextMdcHit())!=0)
	{ //loop over geant 
	  
	  
	  if(pGeantMdc->getSector()==0 && pGeantMdc->getModule()==0 && pGeantMdc->getLayer()==6)
	    {
	      
	      //ut<<"First point"<<endl;
	      
	      flag1=1;
	      takePoint(pGeantMdc,POINT[0],p06,0,0,6);
	    }
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==1 && pGeantMdc->getLayer()==6)
	    {
	      
	      //ut<<"Second point"<<endl;
	      
	      
	      flag2=1;
	      takePoint(pGeantMdc,POINT[1],p16,0,1,6);
	    }
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==2 && pGeantMdc->getLayer()==6)
	    {
	      
	      //   cout<<"Third point"<<endl;
	      
	      flag3=1;
	      takePoint(pGeantMdc,POINT[2],p26,0,2,6);
	    }
	  if(pGeantMdc->getSector()==0 &&pGeantMdc->getModule()==3 && pGeantMdc->getLayer()==6)
	    {
//   cout<<"Fourth point"<<endl;

	      flag4=1;
	      takePoint(pGeantMdc,POINT[3],p36,0,3,6);
	      
//   cout<<"again"<<endl;
	          
	      break;		    
	    }
	}
      if(flag1 && flag2 && flag3 && flag4)
	{
	  //	  POINT[0].print();
	  //	  POINT[1].print();
	  //	  POINT[2].print();
	  //	  POINT[3].print();
	  Double_t dist[4]; 
	  
	  pRecon=corr->calcMomentum(POINT,kTRUE);
	  //calculations in case of 3 chambers
	  Double_t pp[12];

          pp[0]=POINT[0].getX();
          pp[1]=POINT[0].getY();
	  pp[2]=POINT[0].getZ();
  

          pGeant=(p16+p26)/2;
          corr->getDistance(dist);

	  




	  nt->Fill(theta,phi,pGeant,pRecon,pKine,ID,dist[0],dist[1],dist[2],dist[3]);
	  
	  // cout<<"prec=  "<<pRecon<<endl;
	  // cout<<"pgeant=  "<<pGeant<<endl;
	}
    }
  return 0;
}

//For the case with 3 chambers;









void HMdcTrackGSplineCorr::takePoint(HGeantMdc *data,HGeomVector &plab, Float_t &momentum,
Int_t sector,Int_t module,Int_t layer)
{ // void takePoint
  //  trans=kFALSE;
  Float_t mom; 
  HGeomVector p;
  Float_t x=0,y=0,z=0,tof=0;
  data->getHit(x,y,tof,mom);
  momentum=mom;
  p.setXYZ(x,y,z);
//  HModGeomPar *module=fMdcGeometry->getModule(data->getSector(),data->getModule());
//  HGeomTransform &transMod = module->getLabTransform();
//  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
//  HGeomTransform modTrans(transMod);
//  // if(!modTrans) trans=kFALSE;
//  modTrans.transTo(transSec);
//  plab=modTrans.transFrom(p);
HGeomTransform trans;
fGetCont->getLabTransMod(trans,sector,module,layer);
plab=trans.transFrom(p); 


  Float_t xlab=plab.getX();
  Float_t ylab=plab.getY();
  Float_t zlab=plab.getZ();
  plab.setXYZ(0.1*xlab,0.1*ylab,0.1*zlab);
}
