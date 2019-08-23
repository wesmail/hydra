//*-- Author : M. Sanchez (25.05.2000)
//*-- Modified : M.Sanchez (25.05.2000
#include "hkickdetdigi.h"
#include "hades.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hdetector.h"
#include "hevent.h"
#include "htofdetector.h"
#include "hspectrometer.h"
#include "hruntimedb.h"
#include "hmdcgeompar.h"
#include "hmdchitsim.h"
#include "hdetector.h"
#include "htofhitsim2.h"
#include "hgeantmdc.h"
#include "hgeanttof.h"
#include "hgeantkine.h"
#include "hgeomtransform.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hgeomvector.h"
#include "hiterator.h"
#include "hgeomrotation.h"
#include "hmdcsegsim.h"
#include "hspecgeompar.h"
#include "hmdcdef.h"
#include <TRandom.h>
#include <TF1.h>

////////////////////////////////////////////
// HKickMdcDigi
//
//  Simple minded digitization GEEANT --> MDC Hits
//
/////////////////////////////////////////////

HKickDetDigi::HKickDetDigi(Text_t name[],Text_t title[]) :
  HReconstructor(name,title) {
  fMdcInput=0;
  fMdcOut=0;
  fMdcIter=0;
  fTofInput=0;
  fTofOut=0;
  fTofIter=0;
  fFillTof=kTRUE;
  fIsTofPerfect = fIsMdcPerfect = kFALSE;
  setMdcAngErrConst(kFALSE);
  fFillSegments[0] =  fFillSegments[1] = kTRUE;
  fFillMdc = kTRUE;
  fMomPos = 0;
  fSimulateTails = kFALSE;
  fFillMdcHits = kTRUE;
  for (int i=0;i<4;i++) {
	probDistrX[i]=0;
	probDistrY[i]=0;
  }
  fMdcResolX[0] = .16;
  fMdcResolX[1] = .16;
  fMdcResolX[2] = .16;
  fMdcResolX[3] = .16;
  fMdcResolY[0] = .08;
  fMdcResolY[1] = .08;
  fMdcResolY[2] = .08;
  fMdcResolY[3] = .08;
  fMdcTailX[0] = 3;
  fMdcTailX[1] = 3;
  fMdcTailX[2] = 3;
  fMdcTailX[3] = 3;
  fMdcTailY[0] = 1;
  fMdcTailY[1] = 1;
  fMdcTailY[2] = 1;
  fMdcTailY[3] = 1;
}

HKickDetDigi::~HKickDetDigi(void) {
	if (fMdcIter) delete fMdcIter;
	if (fTofIter) delete fTofIter;
}

Int_t HKickDetDigi::execute(void) {
  if (fMdcInput) digiMdc2();
  if (fTofInput && isFillingTof()) digiTof();
  return 0;
}

Int_t HKickDetDigi::digiTof(void) {
  HGeantTof *geTof=0;
  HTofHitSim2 *tofHit=0;
  Float_t x,y,en,tof,tofResol=0.1; //100 ps
  Float_t posResol = 15.5;
  HLocation loc;
  HGeantKine *geKine=0;
  Int_t trackN,pid;
  Float_t px=0,py=0,pz=0,mom,len;

  loc.set(3,0,0,0);
  fTofIter->Reset();
  while ( (geTof=(HGeantTof *)fTofIter->Next()) != 0) {
    geTof->getHit(en,x,y,tof,mom,len);
    if (!isTofPerfect()) x+=gRandom->Gaus(0,posResol);
    loc[0]=geTof->getSector();
    loc[1]=21 - geTof->getModule();
    loc[2]=7 - geTof->getCell();

    if (loc[1]<0) continue; //This is TOFINO. Skip
    if (tof>50) continue;
    tofHit=(HTofHitSim2 *)fTofOut->getObject(loc);
    if (!tofHit) {
      tofHit=(HTofHitSim2 *)fTofOut->getSlot(loc);
      if (tofHit) {
        tofHit=new (tofHit) HTofHitSim2;
        tofHit->setNHits(1);
        tofHit->setXpos(x);
        if (isTofPerfect()) tofHit->setY(y);
        tofHit->setTof(tof+gRandom->Gaus(0,tofResol));
        tofHit->setSector(loc[0]);
        tofHit->setModule(loc[1]);
        tofHit->setCell(loc[2]);
	if (tofHit->getNTrack1() > 0) { //first track nb is used
	  tofHit->setNTrack2(geTof->getTrack());
	} else {
	  tofHit->setNTrack1(geTof->getTrack());
	}
        geKine=(HGeantKine *)fCatKine->getObject(geTof->getTrack()-1);
        if (geKine) {
          geKine->getMomentum(px,py,pz);
          geKine->getParticle(trackN,pid);
          tofHit->setPid(pid);
          tofHit->setMomentum(mom);
        }
      }
    } else { //Coger hit mas rapido
      //if (n > 1) Warning("digiTof","Demasiados hits %i",n);
      geKine=(HGeantKine *)fCatKine->getObject(geTof->getTrack()-1);
      if (geKine) {
        Int_t parent,medium,mech;
        geKine->getCreator(parent,medium,mech);
        //tofHit->setNHits(n+1);
        if (tof < tofHit->getTof()) { //FIXME:Select primary tracks
          tofHit->setXpos(x);
          tofHit->setTof(tof);
	  if (isTofPerfect()) tofHit->setY(y);
          tofHit->setSector(loc[0]);
          tofHit->setModule(loc[1]);
          tofHit->setCell(loc[2]); 
	  if (tofHit->getNTrack1()==0) {
	    tofHit->setNTrack1(geTof->getTrack());
	  } else
	    tofHit->setNTrack2(geTof->getTrack());
          geKine->getMomentum(px,py,pz);
          geKine->getParticle(trackN,pid);
          tofHit->setPid(pid);
          tofHit->setMomentum(TMath::Sqrt(px*px+py*py+pz*pz));
        }
      }        
    }
  }
  return 0;
}

Int_t HKickDetDigi::digiMdc2(void) {
  //Digitizes data from MDC
  const Int_t theModule=0,theLayer=6;


  HGeantMdc *geMdc=0,*geMdc2;
  HGeantKine *geKine=0;
  Int_t mdcSector=0,mdcModule=0,mdcLayer=0;
  Int_t trackN,pid;

  Float_t x=0,y=0,tof=0,px=0,py=0,pz=0;
  Float_t x2=0,y2=0,mom=0,mom2=0;
  HGeomVector rLocal,r,alpha;

  fMdcIter->Reset();

  while ( (geMdc=(HGeantMdc *)fMdcIter->Next()) != 0) {
    mdcSector=geMdc->getSector();
    mdcModule=geMdc->getModule();
    mdcLayer=geMdc->getLayer();
    if (mdcModule % 2 == theModule && mdcLayer == theLayer) {
      geMdc2 = findPartner(geMdc,mdcSector,mdcModule+1,mdcLayer);
      if (geMdc2) { //If partner exists
	geMdc->getHit(x,y,tof,mom);
	geMdc2->getHit(x2,y2,tof,mom2);
	if (!isMdcPerfect()) {
	  x += probDistrX[geMdc->getModule()]->GetRandom();
	  y += probDistrY[geMdc->getModule()]->GetRandom();
	  x2 += probDistrX[geMdc2->getModule()]->GetRandom();
	  y2 += probDistrY[geMdc2->getModule()]->GetRandom();
	}

	fMdcSectorLoc[0]=mdcSector;
	fMdcSecModLoc[0]=mdcSector; fMdcSecModLoc[1]=mdcModule / 2;

	  calcPosDir(geMdc,geMdc2,x,y,x2,y2,r,alpha);
	  geKine=(HGeantKine *)fCatKine->getObject(geMdc->getTrack()-1);
	  if (geKine) {
	    if (fFillSegments[fMdcSecModLoc[1]]) {
	      geKine->getParticle(trackN,pid);
	      if (trackN != geMdc->getTrack())
		Warning("digiMdc","Inconsistent trackN");
	      geKine->getMomentum(px,py,pz);

	      switch (fMomPos) {
		case 0:
		  //Get momentum from target
		  fillData(r,alpha,TMath::Sqrt(px*px + py*py + pz*pz),
		      geMdc->getTrack());
		  break;
		case 1:
		  fillData(r,alpha,mom,geMdc->getTrack());
		  break;
		case 2:
		  fillData(r,alpha,mom2,geMdc->getTrack());
		  break;
		default:
		  Warning("","Momentum determination position unknown");
	      }
	    }
	    //             fillData(r,alpha,TMath::Sqrt(px*px + py*py + pz*pz),
	    //                      geMdc->getTrack());
	    if (fFillMdcHits) {
	      fMdcSecModLoc[1]=geMdc->getModule();
	      fillHit(geMdc, x, y,TMath::Sqrt(px*px + py*py + pz*pz));
	      fMdcSecModLoc[1]=geMdc2->getModule();
	      fillHit(geMdc2, x2, y2,TMath::Sqrt(px*px + py*py + pz*pz));
	    }
	  } else { Error("digiMdc","Inconsistent track number"); return 1;}
      } else { //If partner does not exist
	fMdcSecModLoc[1]=geMdc->getModule();
	geMdc->getHit(x,y,tof,mom);
	if (!isMdcPerfect()) {
	  x += probDistrX[geMdc->getModule()]->GetRandom();
	  y += probDistrY[geMdc->getModule()]->GetRandom();
	}
	if (fFillMdcHits) {
	  fillHit(geMdc,x,y,TMath::Sqrt(px*px+py*py+pz*pz));
	}
      }
    }
  }

  return 0;
}

void HKickDetDigi::fillHit(HGeantMdc *geMdc, Float_t x, Float_t y, Float_t p) {
  Float_t xDir, yDir,zDir;
  Float_t theta,phi;
  Float_t sigma_sx = 0.01;
  Float_t sigma_sy = 0.01;
  Double_t k = TMath::Pi() / 180.;
  TObject *slot = 0;
  HMdcHitSim *hit;
  HGeomVector alpha;
  Int_t lsTracks[5];
  UChar_t lsTimes[5];

  Double_t Sx,Sy;

  geMdc->getIncidence(theta,phi);

  if (!isMdcPerfect()) {
    if(isMdcAngErrConst()){
      // HAPOL - 31/05/02
      // Define a symmetrical angular error distribution around the track
      // direction. Rot is the matrix transforming from the coordinate
      // system where the track vector is (0,0,1) to the MDC coordinate
      // system
      Float_t resTheta = 0.02;
      Float_t resPhi = 0.04;
      Float_t t2,p2;
      HGeomRotation rot;
      rot.setElement( cos(k*theta)*cos(k*phi) , 0);
      rot.setElement( -sin(k*phi) ,             1);
      rot.setElement( sin(k*theta)*cos(k*phi) , 2);
      rot.setElement( cos(k*theta)*sin(k*phi) , 3);
      rot.setElement( cos(k*phi) ,              4);
      rot.setElement( sin(k*theta)*sin(k*phi) , 5);
      rot.setElement( -sin(k*theta) ,           6);
      rot.setElement( 0 ,                       7);
      rot.setElement( cos(k*theta) ,            8);

      t2 = gRandom->Gaus(0,resTheta);
      p2 = gRandom->Uniform(2*360);

      HGeomVector 
	withError(sin(k*t2)*cos(k*p2),sin(k*t2)*sin(k*p2),cos(k*t2));
      HGeomVector withErrorInMDC = rot * withError;

      //cout << " Before theta: "<< theta << "  phi"<< phi << endl;       

      theta = acos(withErrorInMDC.getZ()) / k;
      phi = atan2(withErrorInMDC.getY(),withErrorInMDC.getX()) / k;

      //cout << " After  theta: "<< theta << "  phi"<< phi << endl;   
    }
    //theta += 2;//gRandom->Gaus(0,resTheta);
    // phi += gRandom->Gaus(0,resPhi);
  }


  // Pass to radians
  theta *= k;
  phi *= k;

  slot = fMdcHitOut->getNewSlot(fMdcSecModLoc);
  if (slot != 0) {
    hit = new(slot) HMdcHitSim;

    alpha.setX(TMath::Sin(theta) * TMath::Cos(phi));
    alpha.setY(TMath::Sin(theta) * TMath::Sin(phi));
    alpha.setZ(TMath::Cos(theta));

    Sx = alpha.getX() / alpha.getZ();
    Sy = alpha.getY() / alpha.getZ();
    if (!isMdcPerfect()) {
      Sy += gRandom->Gaus(0,sigma_sy);
      Sx += gRandom->Gaus(0,sigma_sx);
    }

//     alpha.setX(alpha.getX() + 0.01);//gRandom->Gaus(0,0.01));
//     alpha.setY(alpha.getY() + 0.01);//gRandom->Gaus(0,0.01));
//      alpha /= alpha.length();

    xDir = Sx / sqrt(1+Sx*Sx+Sy*Sy);
    yDir = Sy / sqrt(1+Sx*Sx+Sy*Sy);
    zDir = 1. / sqrt(1+Sx*Sx+Sy*Sy);

    Float_t cov_x_x, cov_y_y, cov_ax_ax, cov_ay_ay,cov_ax_ay;
    if (isMdcPerfect()) {
      cov_x_x = cov_y_y = cov_ax_ax = cov_ay_ay = cov_ax_ay = 0;
    } else {
      Float_t d_ax_sx = (1-xDir)*zDir;
      Float_t d_ax_sy = Sx*Sy*zDir; //=d_ay_sx
      Float_t d_ay_sy = (1-yDir)*zDir;
      cov_x_x = fMdcResolX[fMdcSecModLoc[1]] * fMdcResolX[fMdcSecModLoc[1]];
      cov_y_y = fMdcResolY[fMdcSecModLoc[1]] * fMdcResolY[fMdcSecModLoc[1]];
      cov_ax_ax = pow(d_ax_sx*sigma_sx,2) + pow(d_ax_sy*sigma_sy,2);
      cov_ay_ay = pow(d_ax_sy*sigma_sx,2) + pow(d_ay_sy*sigma_sy,2);
      cov_ax_ay = sigma_sx*sigma_sx*d_ax_sx*d_ax_sy + sigma_sy*sigma_sy*d_ay_sy*d_ax_sy;
    }
      
                        
    hit->getCovariance().Clear();
    hit->setX(x, sqrt(cov_x_x));
    hit->setY(y, sqrt(cov_y_y)); 
    hit->setXDir( xDir, sqrt(cov_ax_ax));
    hit->setYDir( yDir, sqrt(cov_ay_ay));
    hit->getCovariance().setElement(2,3,cov_ax_ay);
    hit->setSecMod( fMdcSecModLoc[0], fMdcSecModLoc[1]);
    lsTracks[0] = geMdc->getTrack();
    lsTimes[0] = 6;
    hit->setNTracks(1,lsTracks,lsTimes);
  }

}                

void HKickDetDigi::fillData(HGeomVector &r,HGeomVector &alpha,
                            Double_t p,Int_t track) {
  HMdcSegSim *mdcSeg=0;
  UChar_t nTimes = 12;
  TObject *slot=0;
  Float_t theta,phi;

  //  if (fMdcSecModLoc[1]==0) {
    slot=fMdcOut->getNewSlot(fMdcSecModLoc);
    if (slot != 0) {
      mdcSeg = new(slot) HMdcSegSim; 
      mdcSeg->setSec(fMdcSectorLoc[0]);
      mdcSeg->setIOSeg(fMdcSecModLoc[1]);
      theta=TMath::ACos(alpha.getZ());
      phi=TMath::ATan2(alpha.getY(),alpha.getX());
      mdcSeg->setR(r.getY()*TMath::Cos(phi) - r.getX()*TMath::Sin(phi),0.1);
      mdcSeg->setZ(-(r.getY()*TMath::Sin(phi) + r.getX()*TMath::Cos(phi))*
                   alpha.getZ()/TMath::Sin(theta),0.1);
      mdcSeg->setThetaPhi(theta,0.1,
                          phi,0.1);
      mdcSeg->getCovariance().setCov(fMdcSegCov);
      //Stupid errors
      mdcSeg->setNTracks(1,&track,&nTimes);
    } else { Warning("fillData","No slot available"); fMdcSecModLoc.Dump(); }
    //}
}


void HKickDetDigi::calcPosDir(HGeantMdc *data, HGeantMdc *data2,
                           Float_t x, Float_t y, Float_t x2, Float_t y2,
                           HGeomVector &r,HGeomVector &dir) {
  //Transforms a mdc hit from the MDC coordinate system into the LAB coordinate
  //system and smears the data according to the resolution.
  HGeomVector r1,r2,rLocal;
  
  HModGeomPar *module=fGeometry->getModule(data->getSector(), data->getModule());

  if (!module) Error("calcDir",
                     "Module %i:%i invalid\n",
                     data->getSector(),data->getModule());

  HGeomTransform &transMod = module->getLabTransform();
  HGeomTransform &transSec = fSpecGeometry->getSector(data->getSector())->getTransform();
  HGeomTransform modTrans(transMod);
  modTrans.transTo(transSec);

  HModGeomPar *module2=fGeometry->
    getModule(data2->getSector(),data2->getModule());

  if (!module2) Error("calcDir","Module %i:%i invalid\n",
                      data2->getSector(),data2->getModule());

  HGeomTransform &transMod2 = module2->getLabTransform();
  HGeomTransform &transSec2 = fSpecGeometry->getSector(data2->getSector())->getTransform();
  HGeomTransform modTrans2(transMod2);
  modTrans2.transTo(transSec2);

//   modTrans2.print();
  //Double_t layerHalfWidth=TMath::Abs(layerVol->getPoint(0)->getZ());
  
  rLocal.setX(x);
  rLocal.setY(y);
  rLocal.setZ(0.0);  
  r1 = modTrans.transFrom(rLocal);

  rLocal.setX(x2);
  rLocal.setY(y2);
  rLocal.setZ(0.0);
  r2 = modTrans2.transFrom(rLocal);

  dir = r2 -r1;
  dir /= dir.length();

  //Coordinates at z=0
  r.setX( r1.getX() - (dir.getX() / dir.getZ()) * r1.getZ());
  r.setY( r1.getY() - (dir.getY() / dir.getZ()) * r1.getZ());
  r.setZ( 0. );

  //Covariance matrix (order: x,y,x',y')
  Float_t sx = fMdcResolX[data->getModule()] + fMdcResolX[data2->getModule()];
  Float_t sy = fMdcResolY[data->getModule()] + fMdcResolY[data2->getModule()];
  Float_t dz = r2.getZ() - r1.getZ();
  Float_t zadz = r1.getZ() / dz;
  sx = sx*sx/4.;
  sy = sy*sy/4.;
  fMdcSegCov(0,0) = sx*(1+2*zadz*(1+zadz));
  fMdcSegCov(0,1) = 0;
  fMdcSegCov(0,2) = -(sx/dz)*(1+2*zadz);
  fMdcSegCov(0,3) = 0;
  fMdcSegCov(1,1) = sy*(1+2*zadz*(1+zadz));
  fMdcSegCov(1,2) = 0;
  fMdcSegCov(1,3) = -(sy/dz)*(1+2*zadz);
  fMdcSegCov(2,2) = 2 * sx / (dz*dz);
  fMdcSegCov(2,3) = 0;
  fMdcSegCov(3,3) = 2 * sy / (dz*dz);
}

HGeantMdc *HKickDetDigi::findPartner(HGeantMdc *geMdc, Int_t sector,
                                     Int_t mod, Int_t layer) {
  HGeantKine *geKine;
  HGeantMdc *gePart;
  geKine=(HGeantKine *)fCatKine->getObject(geMdc->getTrack()-1);
  geKine->resetMdcIter();
  while ( (gePart = (HGeantMdc *)geKine->nextMdcHit()) != 0) {
    if (gePart->getSector() == sector &&
        gePart->getModule() == mod &&
        gePart->getLayer() == layer) return gePart;
  }
  return 0;
}

void HKickDetDigi::setMdcResolution(Float_t x1,Float_t y1,Float_t x2,Float_t y2,
    Float_t x3,Float_t y3,Float_t x4,Float_t y4) {
  fMdcResolX[0] = x1;
  fMdcResolX[1] = x2;
  fMdcResolX[2] = x3;
  fMdcResolX[3] = x4;
  fMdcResolY[0] = y1;
  fMdcResolY[1] = y2;
  fMdcResolY[2] = y3;
  fMdcResolY[3] = y4;
}

void HKickDetDigi::setMdcTails(Float_t x1,Float_t y1,Float_t x2,Float_t y2,
    Float_t x3,Float_t y3,Float_t x4,Float_t y4) {
  fMdcTailX[0] = x1;
  fMdcTailX[1] = x2;
  fMdcTailX[2] = x3;
  fMdcTailX[3] = x4;
  fMdcTailY[0] = y1;
  fMdcTailY[1] = y2;
  fMdcTailY[2] = y3;
  fMdcTailY[3] = y4;
}

Bool_t HKickDetDigi::init(void) {
  fMdcSectorLoc.set(1,0);
  fMdcSecModLoc.set(2,0,0);
  if (gHades) {
    HRuntimeDb *rtdb=gHades->getRuntimeDb();
    HSpectrometer *spec=gHades->getSetup();
    HEvent *event=gHades->getCurrentEvent();
    if (rtdb && spec && event) {
      HDetector *mdc=spec->getDetector("Mdc");
      HTofDetector *tof=(HTofDetector *)spec->getDetector("Tof");

      fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");

      if (mdc) {
        fCatKine=event->getCategory(catGeantKine);
        if (!fCatKine) {
	  Error("init","catKine not found");
	  return kFALSE;
	}
        
        if (fFillMdc) {
	  //Setup probability distributions for MDC resolution
/*	  const Float_t resolX[4]={.16,.16,.16,.16};
	  const Float_t resolY[4]={.08,.08,.08,.08};
	  const Float_t resolXTail[4] = {3,3,3,3};
	  const Float_t resolYTail[4] = {1,1,1,1};*/
	  const Float_t tailIntensity = 0.1;
// 	  const Float_t resolX[4]={0.16,0.16,.44,.44};
// 	  const Float_t resolY[4]={0.1,0.1,.18,.18};
// 	  const Float_t resolXTail[4] = {2,2,6,6};
// 	  const Float_t resolYTail[4] = {.2,.2,1,1};
// 	  const Float_t tailIntensity = 250./10000.;

	  if (fSimulateTails) {
	    for (int idi=0;idi<4;idi++) {
	      char buffer[256];
	      sprintf(buffer,"modx%i",idi);
	      probDistrX[idi] = new TF1(buffer,"gaus(0)+gaus(3)",-4*fMdcTailX[idi],
					4*fMdcTailX[idi]);
	      probDistrX[idi]->SetParameters(1,0,fMdcResolX[idi],
					     tailIntensity,0,fMdcTailX[idi]);
	      sprintf(buffer,"mody%i",idi);
	      probDistrY[idi] = new TF1(buffer,"gaus(0)+gaus(3)",-4*fMdcTailY[idi],
					4*fMdcTailY[idi]);
	      probDistrY[idi]->SetParameters(1,0,fMdcResolY[idi],
					     tailIntensity,0,fMdcTailY[idi]);
	    }
	  } else {
	    for (int idi=0;idi<4;idi++) {
	      char buffer[256];
	      sprintf(buffer,"modx%i",idi);
	      probDistrX[idi] = new TF1(buffer,"gaus",-4*fMdcResolX[idi],
					4*fMdcResolX[idi]);
	      probDistrX[idi]->SetParameters(1,0,fMdcResolX[idi]);

	      sprintf(buffer,"mody%i",idi);
	      probDistrY[idi] = new TF1(buffer,"gaus(0)",-4*fMdcResolY[idi],
					4*fMdcResolY[idi]);
	      probDistrY[idi]->SetParameters(1,0,fMdcResolY[idi]);
	    }
	  }

          fMdcInput=event->getCategory(catMdcGeantRaw);
          if (!fMdcInput) {
	    Error("init","MDC input not found");
	    return kFALSE;
	  }
          if ( (fMdcIter=(HIterator *)fMdcInput->MakeIterator("")) == 0 ) {
	    Error("init","Unable to build mdc iterator");
            return kFALSE;
	  }

          if (fFillSegments[0] || fFillSegments[1]) {
            fMdcOut=event->getCategory(catMdcSeg);
            if (!fMdcOut) {
              Int_t sizes[]={6,2,100};
              fMdcOut=new HMatrixCategory("HMdcSegSim",3,sizes,0.5);
              if (!event->addCategory(catMdcSeg,fMdcOut,"Mdc")) {
		Error("init","Unable to add catMdcSeg to event");
                return kFALSE;
	      }
            }
          }
          
          fMdcHitOut = event->getCategory(catMdcHit);
          if (!fMdcHitOut) {
            Int_t sizes[]={6,4,100};
            fMdcHitOut=new HMatrixCategory("HMdcHitSim",3,sizes,0.5);
            if (!event->addCategory(catMdcHit,fMdcHitOut,"Mdc")) {
	      Error("init","Unable to add catMdcHit to event");
              return kFALSE;
	    }
          }      

          fGeometry=(HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
        } else fMdcInput=0;
      } else fMdcInput=0;
      if (tof) {
        if (isFillingTof()) {
	  fTofInput=event->getCategory(catTofGeantRaw);
	  if (!fTofInput) {
	    Error("init","No TOF input data from Geant");
	    return kFALSE;
	  }
	  if ((fTofIter=fTofInput->MakeIterator()) == 0) {
	    Error("init","Unable to build TOF iterator");
	    return kFALSE;
	  }

          fTofOut=event->getCategory(catTofHit);
          if (!fTofOut) {
            fTofOut=tof->buildMatrixCategory("HTofHitSim2",0.5);
            if (!fTofOut) {
	      Error("init","Unable to build catTofHit for output");
	      return kFALSE;
	    }
            event->addCategory(catTofHit,fTofOut,"Tof");
          }
        }
      } else fTofInput=0; //fTofInput=0;
    } else {
      Error("init","is Framework initialized?");
      return kFALSE; //! (rtdb && spec && event)
    }
  } else {
    Error("init","gHades not found: panic!");
    return kFALSE; //! (gHades)
  }
  return kTRUE;
}

Bool_t HKickDetDigi::finalize(void) {
	for (int idi=0;idi<4;idi++) {
		delete probDistrX[idi];
		probDistrX[idi] = 0;
		
		delete probDistrY[idi];
		probDistrY[idi]=0;
	}

  return kTRUE;
}

ClassImp(HKickDetDigi)
