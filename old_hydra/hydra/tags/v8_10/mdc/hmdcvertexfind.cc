//*-- Author : M.Sanchez (14.06.2000)
#include "hmdcvertexfind.h"
#include "hcategory.h"
#include "hmdcgeompar.h"
#include "hades.h"
#include "hevent.h"
#include "hiterator.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hgeomvector.h"
#include "mdcsdef.h"
#include "hmdcseg.h"
#include "hmdchit.h"
#include "hgeomvolume.h"
#include "hrecevent.h"
#include "hpartialevent.h"
#include <TNtuple.h>

//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////
// HMdcVertexFind
//
//   Vertex finder using weighted LSM with optional Tukey weights
//
// To use it in a macro just add
//
// taskset->add(new HMdcVertexFind("name","title",input_mode,tukey)
//
// where 
//
// input_mode is one of HMdcVertexFind::kSegments or HMdcVertexFind::kHits.
// In the first case HMdcSeg objecst will be used as input (recomended for
// now) while in the secon HMdcHit objects will be used
//
// As for 'tukey' it can be kTRUE or kFALSE depending on if Tukey weights
// should be used in the minimization. For low multiplicity environments
// like C+C I would recomend kFALSE; at least until I have found better
// parameters for the Tukey weigths minimization.
/////////////////////////////////////////

HMdcVertexFind::HMdcVertexFind(Text_t name[],Text_t title[],EInputMode m,
			       Bool_t tukey) :
  HReconstructor(name,title),fPos("HGeomVector",200),
  fAlpha("HGeomVector",200) {
  initVars();
  useTukeyWeights(tukey);
  fInputMode=m;
}

HMdcVertexFind::~HMdcVertexFind(void) {
  initVars();
}

void HMdcVertexFind::initVars(void) {
  fInput=0;
  fGeometry=0;
  fIter=0;
  fMaxIterations=100;
  fTukeyConst=6.0;
  fEpsilon=.3;
  fUsingTukey = kFALSE;
  fDebugMode = kFALSE;
}

Bool_t HMdcVertexFind::init(void) {
  HRuntimeDb *rtdb=gHades->getRuntimeDb();
  HRecEvent *ev = dynamic_cast<HRecEvent *>(gHades->getCurrentEvent());
  if (!ev) return kFALSE;

  switch (fInputMode) {
  case kSegments:
    fInput=gHades->getCurrentEvent()->getCategory(catMdcSeg);
    break;
  case kHits:
    fInput=gHades->getCurrentEvent()->getCategory(catMdcHit);
    break;
  default:
    fInput=0;
  }
  if (!fInput) return kFALSE;
  //  else fIter=(HIterator *)fInput->MakeIterator();
  fGeometry=(HMdcGeomPar *)gHades->getRuntimeDb()->getContainer("MdcGeomPar");
  fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

  if (gHades->getOutputFile()) {
    gHades->getOutputFile()->cd();
    //fData----
    // x,y,z,n : posicion del vertice en la iteracion n
    // con : converged??
    //fData=new TNtuple("Data","Data","x:y:z:n:con");
    if (fDebugMode)
      fControl=new TNtuple("con","control","rho:dx:dy:dz:sector:module");
  }

  return kTRUE;
}

Bool_t HMdcVertexFind::finalize(void) {
  return kTRUE;
}

Bool_t HMdcVertexFind::pointsToTarget(const HGeomVector &r,HGeomVector &alpha,
				    Int_t sector,Int_t module) {
  //Gilipollas el HMdcSeg ya tiene esta informacion.....
  Double_t bx=alpha.getX()/alpha.getZ();
  Double_t by=alpha.getY()/alpha.getZ();
  HGeomVector rmin;
  Bool_t res=kTRUE;

  //Evaluate maximum approach to z axis
  rmin.setZ(r.getZ() - (r.getX() * bx + r.getY() * by) / (bx * bx + by * by));
  rmin.setX(r.getX() + bx * (rmin.getZ() - r.getZ()));
  rmin.setY(r.getY() + by * (rmin.getZ() - r.getZ()));
  Double_t rhomin=TMath::Sqrt(rmin.getX() * rmin.getX() + 
			      rmin.getY() * rmin.getY());

  //Impose condition
  //cout << rmin.getZ() << endl;
  if (fDebugMode)
    fControl->Fill(rhomin,rmin.getX(),rmin.getY(),rmin.getZ(),sector,module);
  if (rmin.getZ()<100 && rmin.getZ()>-150 && rhomin<200) res=kTRUE;
  else res=kFALSE;
  return res;
}

void HMdcVertexFind::transform(HMdcSeg *hit,
			     HGeomVector &r,HGeomVector &alpha) {
  //Calculates position and direction vector for a HMdcHit
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;

  theta=hit->getTheta();
  phi=hit->getPhi();
  r.setZ(hit->getZ());
  r.setX(hit->getR()*TMath::Cos(phi+pi2));
  r.setY(hit->getR()*TMath::Sin(phi+pi2));
  alpha.setZ(TMath::Cos(theta));
  alpha.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alpha.setX(TMath::Sin(theta)*TMath::Cos(phi));
}

Bool_t HMdcVertexFind::readSegments(HGeomVector &estimate) {
  HGeomVector *r,*alpha,rLocal,alphaLocal;
  HMdcSeg *data=0;  
  Int_t count=0;
  fInput=gHades->getCurrentEvent()->getCategory(catMdcSeg);

//   r = new(fPos[count]) HGeomVector(0,0,0);
//   alpha = new(fAlpha[count]) HGeomVector(0,0,1);
//   count++;

  if (fInput) {
    //First vertex estimation and filling of fPos, fAlpha.

    fIter=(HIterator *)fInput->MakeIterator();
    fIter->Reset();
    fFitter.reset();
    
    while ( (data=(HMdcSeg *)fIter->Next()) != 0) {
      // Transform from MDC system to LAB system
      if (data->getIOSeg()==0&&data->getChi2()>=0) {
	HGeomTransform &trans=fSpecGeometry->getSector(data->getSec())->getTransform();
	
	transform(data,rLocal,alphaLocal);
	
	r=new(fPos[count]) HGeomVector(trans.transFrom(rLocal));
	alpha=new(fAlpha[count]) HGeomVector(trans.getRotMatrix() * 
					     alphaLocal);
	//Feed to LSM fitter.
	if (pointsToTarget(*r,*alpha,data->getSec(),0)) {
	  fFitter.addLine(*r,*alpha); //Default weigth =1.0
	  count++;
	}
      }
    }
    delete fIter;
    if (count > 1)
      fFitter.getVertex(estimate); 
    else
      estimate.setXYZ(-1000,-1000,-1000);
  } else return kFALSE;
  return kTRUE;
}

Bool_t HMdcVertexFind::readHits(HGeomVector &estimate) {
  HGeomVector *r,*alpha,rLocal,alphaLocal;
  HMdcHit *data=0;  
  Int_t count=0,sector,module;
  Double_t dx,dy;

  fInput=gHades->getCurrentEvent()->getCategory(catMdcHit);
  if (fInput) {
    //First vertex estimation and filling of fPos, fAlpha.
    fIter=(HIterator *)fInput->MakeIterator();
    fIter->Reset();
    fFitter.reset();
    
    while ( (data=(HMdcHit *)fIter->Next()) != 0) {
      // Transform from MDC system to LAB system
      data->getSecMod(sector,module);
      HGeomTransform &trans=fGeometry->
	getModule(sector,module)->getLabTransform();
      rLocal.setX(data->getX());
      rLocal.setY(data->getY());
      rLocal.setZ(0.);
      dx=data->getXDir(); 
      dy=data->getYDir();
      alphaLocal.setX(dx);
      alphaLocal.setY(dy);
      alphaLocal.setZ(TMath::Sqrt(1. - dx*dx - dy*dy));
      r=new(fPos[count]) HGeomVector(trans.transFrom(rLocal));
      alpha=new(fAlpha[count]) HGeomVector(trans.getRotMatrix() * 
					   alphaLocal);
      //Feed to LSM fitter.
      if (pointsToTarget(*r,*alpha,sector,module)) {
	fFitter.addLine(*r,*alpha); //Default weigth =1.0
	count++;
      }
    }
    if (count>1)
      fFitter.getVertex(estimate); 
    else
      estimate.setXYZ(-1000,-1000,-1000);
  } else return kFALSE;
  return kTRUE;
}

Int_t HMdcVertexFind::execute(void) {
  HGeomVector *r,*alpha;
  HGeomVector oldVertex;
  HVertex &event_vertex = gHades->getCurrentEvent()->getHeader()->getVertex();
  HGeomVector &vertex = event_vertex.getPos();
  
  Int_t count=0,i=0,iteration=0;
  Double_t weight,residual2,temp; //Weight and residual^2

  switch (fInputMode) {
  case kSegments:
    if (!readSegments(vertex)) return 1;
    break;
  case kHits:
    if (!readHits(vertex)) return 1;
    break;
  default:
    Error("execute","Unrecognized mode");
  }
  if (fUsingTukey) {
    count=fPos.GetEntriesFast();
    if (count > 1) {
       //Iteration on weights
       Float_t sumOfResiduals = 0;
       Float_t sumOfWeights = 0;
       for (i=0;i<count;i++) {
	  r = (HGeomVector *)fPos.UncheckedAt(i);
	  alpha = (HGeomVector *)fAlpha.UncheckedAt(i);
	  temp = ((r->getY() - vertex.getY()) * alpha->getZ() -
		  (r->getZ() - vertex.getZ()) * alpha->getY());
	  residual2 = temp * temp;
	  temp = ((r->getZ() - vertex.getZ()) * alpha->getX() -
		  (r->getX() - vertex.getX()) * alpha->getZ());
	  residual2 += temp * temp;
	  temp = ((r->getX() - vertex.getX()) * alpha->getY() -
		  (r->getY() - vertex.getY()) * alpha->getX());
	  residual2 += temp * temp;
	  
	  sumOfResiduals += residual2;
	     
	}

       Float_t width = fTukeyConst*sqrt(sumOfResiduals);

       
      iteration=0;
      do {
        sumOfResiduals = 0;
	sumOfWeights = 0;
	oldVertex=vertex;
	//fData->Fill(vertex(0),vertex(1),vertex(2),iteration,0.);
	iteration++;
	fFitter.reset();
	for (i=0;i<count;i++) {
	  r = (HGeomVector *)fPos.UncheckedAt(i);
	  alpha = (HGeomVector *)fAlpha.UncheckedAt(i);
	  temp = ((r->getY() - oldVertex.getY()) * alpha->getZ() -
		  (r->getZ() - oldVertex.getZ()) * alpha->getY());
	  residual2 = temp * temp;
	  temp = ((r->getZ() - oldVertex.getZ()) * alpha->getX() -
		  (r->getX() - oldVertex.getX()) * alpha->getZ());
	  residual2 += temp * temp;
	  temp = ((r->getX() - oldVertex.getX()) * alpha->getY() -
		  (r->getY() - oldVertex.getY()) * alpha->getX());
	  residual2 += temp * temp;



	  temp = sqrt(residual2);
	  weight = (temp < width)?((1. - (temp/width)*(temp/width)) *
				   (1. - (temp/width)*(temp/width))):0.0;
	  sumOfResiduals += weight*residual2;
	  sumOfWeights += weight;
	  fFitter.addLine(*r,*alpha,weight);
	}
	width = fTukeyConst*sqrt(sumOfResiduals/sumOfWeights);
	
	fFitter.getVertex(vertex);
      } 
      while (!hasConverged(vertex,oldVertex) && (iteration < fMaxIterations));

      
      if (iteration<fMaxIterations) {
	 event_vertex.setIterations(iteration);
	 event_vertex.setChi2(sumOfResiduals/sumOfWeights);
	 event_vertex.setSumOfWeights(sumOfWeights);
      }
      else {
	event_vertex.setIterations(-2);
	event_vertex.setChi2(-1);
	event_vertex.setSumOfWeights(-1);
      }
    }
    else {
	event_vertex.setIterations(-1);
	event_vertex.setChi2(-1);
	event_vertex.setSumOfWeights(-1);
    }
      //fData->Fill(vertex(0),vertex(1),vertex(2),iteration,temp);
  } else {
    event_vertex.setIterations(1);
    event_vertex.setChi2(-1);
    event_vertex.setSumOfWeights(-1);       
    //fData->Fill(vertex(0),vertex(1),vertex(2),0,1);
  }

  fPos.Clear();
  fAlpha.Clear();

  return 0;
}

Bool_t HMdcVertexFind::hasConverged(HGeomVector &v,HGeomVector &oldV) {
  Bool_t r=((v-oldV).length() < fEpsilon)?kTRUE:kFALSE;
  return r;
}

ClassImp(HMdcVertexFind)
