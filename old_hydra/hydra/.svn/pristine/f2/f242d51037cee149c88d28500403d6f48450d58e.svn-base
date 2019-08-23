using namespace std;

#include "hkickresolution.h"
#include "hcategory.h"
#include "hmdcgeompar.h"
#include "htofgeompar.h"
#include "hgeanttof.h"
#include "hades.h"
#include "hevent.h"
#include "hiterator.h"
#include "hgeantmdc.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hgeomvector.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hgeantkine.h"
#include "hmdcgeompar.h"
#include "hdetector.h"
#include "htofgeompar.h"
#include <TRandom.h>
#include <iostream> 
#include <iomanip>

HKickResolution::HKickResolution(void) {
  initVars();
}

HKickResolution::HKickResolution(Text_t name[],Text_t title[]) :
  HKickTask(name,title) {
    initVars();
}

HKickResolution::~HKickResolution(void) {
  initVars();
}

void HKickResolution::initVars(void) {
  fInput=0;
  fKine=0;
  fData=0;
  fGeometry=0;
  fIter=0;
  fUsingTofResol=fUsingMdcResol=kTRUE;
}

Bool_t HKickResolution::init(void) {
  setKickParIo(gHades->getRuntimeDb());
  
  fInput=gHades->getCurrentEvent()->getCategory(catMdcGeantRaw);
  if (!fInput) return kFALSE;
  else fIter=(HIterator *)fInput->MakeIterator();
  fKine=gHades->getCurrentEvent()->getCategory(catGeantKine);

  fTofInput=gHades->getCurrentEvent()->getCategory(catTofGeantRaw);
  if (!fTofInput) {
    Warning("init","No TOF input");
    fTofInput = gHades->getSetup()->getDetector("Tof")
      ->buildCategory(catTofGeantRaw);
    if (fTofInput) gHades->getCurrentEvent()->addCategory(catTofGeantRaw,
							fTofInput,
							"Mdc");
    else return kFALSE;
  }
  else fTofIter=(HIterator *)fTofInput->MakeIterator();

  fGeometry=(HMdcGeomPar *)gHades->getRuntimeDb()->getContainer("MdcGeomPar");

  fTofGeometry=(HTofGeomPar *)gHades->getRuntimeDb()
    ->getContainer("TofGeomPar");

  if (gHades->getOutputFile()) { 
    fData=new TNtuple("Data","Datos",
				 "p:pc:flag:a:b:c:x:y:z:ptg:ptf:ptt:pt:ptc:d:et1:et2:pmdc");
    fControl=new TNtuple("con","con","p:t1:t2:f1:f2");
  } else Warning("init","Output file not defined");
  //p ---> actual momentum
  //pmdc --> actual momentum when entering the magnetic field
  //pc --> calculated momentum
  fEvCounter=0;
  return kTRUE;
}

Bool_t HKickResolution::finalize(void) {
  if (gHades->getOutputFile() && fData && fControl) {
    gHades->getOutputFile()->cd();
    fData->Write();
    fControl->Write();
  }
  return kTRUE;
}

//*********************************************************************
//***********************  Coordinate transformations *****************
//*********************************************************************

void HKickResolution::transform(HGeantTof *data,HGeomVector &r,
				HGeomVector &err,Double_t &tanAlpha,
				Double_t &offset) {
  //Transforms a TOF hit from the local coordinate system to the LAB
  //coordinate system and smears the data acording to the resolution.
  HGeomVector rLocal,errLocal;
  Float_t en,x,y,tof,rodHeight,pp,len;
  const Float_t tofXResol=15.5; // TOF resolution in mm
  HModGeomPar *module=fTofGeometry->getModule(data->getSector(),
					      21-data->getModule());
  HGeomTransform &modTrans=module->getLabTransform();
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(7-data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();
  const HGeomRotation &modRot=modTrans.getRotMatrix();
  rodHeight=TMath::Abs(rodVol->getPoint(0)->getY() - rodVol->getPoint(1)->getY());
  
  data->getHit(en,x,y,tof,pp,len);

  if (usingTofResolution()) {
    x+=gRandom->Gaus(0,tofXResol);                 //TOF resolution
    y=0;//gRandom->Uniform(rodHeight)-rodHeight/2.;
    errLocal.setX(tofXResol);
    errLocal.setY(rodHeight/(TMath::Sqrt(12.)));
    errLocal.setZ(0.);
  } else {
    errLocal.setX(0.);
    errLocal.setY(0.);
    errLocal.setZ(0.); 
  }

  r.setX(x);
  r.setY(y);
  r.setZ(0.);
  rLocal=rodTrans.transFrom(r); //Rod to modue system
  r=modTrans.transFrom(rLocal); //Module to LAB system
  err=modRot*errLocal;          //Errors in x,y,z in LAB system
  tanAlpha=modRot(2*3+1)/modRot(1*3+1);                 //Slope of ROD 
  offset=modTrans.getTransVector().getZ() - tanAlpha * 
    modTrans.getTransVector().getY();                   //Rod's offset
}

void HKickResolution::transform(HGeantMdc *data,
			     HGeomVector &r,HGeomVector &alpha) {
  //Transforms a mdc hit from the MDC coordinate system into the LAB coordinate
  //system and smears the data according to the resolution.
  Float_t x,y,tof,mom,theta,phi;
  Double_t k=TMath::Pi()/180.0;
  HGeomVector rLocal,alphaLocal;

  HModGeomPar *module=fGeometry->
    getModule(data->getSector(),data->getModule());
  if (!module) Error("transform",
		     "Module %i:%i invalid\n",
		     data->getSector(),data->getModule());
  HGeomTransform &modTrans=module->getLabTransform();
  //Double_t layerHalfWidth=TMath::Abs(layerVol->getPoint(0)->getZ());

  data->getHit(x,y,tof,mom);
  data->getIncidence(theta,phi);
  if (data->getLayer() != 6) {
    HGeomVolume *layerVol=module->getRefVolume()->
      getComponent(data->getLayer());
    if (!layerVol) Error("transform","Layer %i invalid",
			 data->getLayer());
    HGeomTransform &layerTrans=layerVol->getTransform();

    alphaLocal.setX(TMath::Sin(theta*k)*TMath::Cos(phi*k));
    alphaLocal.setY(TMath::Sin(theta*k)*TMath::Sin(phi*k));
    alphaLocal.setZ(TMath::Cos(theta*k));
    r.setX(x);
    r.setY(y);
    r.setZ(0.0);
    rLocal=layerTrans.transFrom(r);
    r=modTrans.transFrom(rLocal);
    alpha=modTrans.getRotMatrix()*alphaLocal;
  } else {
    alphaLocal.setX(TMath::Sin(theta*k)*TMath::Cos(phi*k));
    alphaLocal.setY(TMath::Sin(theta*k)*TMath::Sin(phi*k));
    alphaLocal.setZ(TMath::Cos(theta*k));
    rLocal.setX(x);
    rLocal.setY(y);
    rLocal.setZ(0.0);
    r = modTrans.transFrom(rLocal);
    alpha = modTrans.getRotMatrix()*alphaLocal;
  }
}

//***************************************************************************
//**************************  EXECUTE functions *****************************
//***************************************************************************

// Int_t HKickResolution::execute(void) {
//   //Resolucion teniendo en cuenta TOF y MDC resoluciones
//   HGeantMdc *data=0;
//   HGeantTof *tofData=0;
//   HGeomVector r[2],alpha[2],mdcR,mdcAlpha,inter,tofR,tofAlpha;
//   HGeantKine *kineTrack=0;
//   HGeomVector errTofR;
//   Double_t errP,k,yR,errPt,deltaY;

//   Int_t count=0,rodNb=-1;
//   Double_t P,Ptc,Ptfc,Pc,Pt,Ptf=0;
//   Float_t Px,Py,Pz;
//   Float_t sintheta;
//   Float_t x,y,tof,mom,mdcXRes=0.16,mdcYRes=0.08; //MDC Resolution (mm).

//   kineTrack=(HGeantKine *)fKine->getObject(0); // There is only one track
//   kineTrack->getMomentum(Px,Py,Pz);
//   P=TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);
  
//   fIter->Reset(); // MDC iterator
//   fTofIter->Reset();
//   count=0;
//   while ( (data=(HGeantMdc *)fIter->Next()) != 0) {
//     data->getHit(x,y,tof,mom);
//     if (usingMdcResolution()) {
//       x+=gRandom->Gaus(0,mdcXRes);
//       y+=gRandom->Gaus(0,mdcYRes);
//     }
//     data->setHit(x,y,tof,mom);
//     if ( (data->getModule() == 0 && data->getLayer() == 0) ) {
//       if (count<1) transform(data,r[count],alpha[count]);
//       count++;
//     }
//     if ( (data->getModule() == 1 && data->getLayer() == 0) ) {
//       if (count <2) transform(data,r[count],alpha[count]);
//       count++;
//     }
//   }

//   if (count == 2) {
//     mdcR=r[0];                    // Track before the magnet
//     mdcAlpha=r[1]-r[0];
//     mdcAlpha/=mdcAlpha.length();
//     //mdcAlpha=alpha[0];
//     fKickPlane->calcIntersection(mdcR,mdcAlpha,inter);
//     //cout << "Befor TOF loop\n";
//     while ( (tofData=(HGeantTof *)fTofIter->Next()) != 0) {
//       if (count <3) {
// 	transform(tofData,tofR,errTofR,k,yR);
// 	tofAlpha=tofR-inter;
// 	tofAlpha/=tofAlpha.length();
// 	count++;
// 	rodNb=tofData->getModule()*8+(7-tofData->getCell());
//       }
//     }

//     if (count == 3) {
//       Double_t D=TMath::Sqrt( (tofR.getY() - inter.getY()) * 
// 			      (tofR.getY() - inter.getY()) +
// 			      (tofR.getZ() - inter.getZ()) *
// 			      (tofR.getZ() - inter.getZ()) );
//       Double_t g1=TMath::ATan2(mdcAlpha.getZ(),mdcAlpha.getY()); 
//       Double_t g2=TMath::ATan2(tofAlpha.getZ(),tofAlpha.getY());
//       Double_t f1=TMath::ATan2(mdcAlpha.getY(),mdcAlpha.getX()); 
//       Double_t f2=TMath::ATan2(tofAlpha.getY(),tofAlpha.getX()); 
//       //      Double_t ttSint = mdcAlpha.vectorProduct(tofAlpha).length();
//       sintheta=sin((g2-g1)/2);
//       Ptc=fKickPlane->getPt(inter,errPt,1);
//       Ptfc=fKickPlane->getPtPhi(inter,errPt,1);
//       //deltaY = cos((g2-g1)/2) * 
//       //(inter.getZ() - k * inter.getY() - yR) * errTofR.getY();
//       //deltaY/= 2 * D * D * sintheta;
//       //errP=deltaY;
//       deltaY = cos((g2-g1)/2.) * (inter.getZ() - k * inter.getY() - yR) * 
// 	errTofR.getY();
//       deltaY /= 2 * D * D * sintheta; 

//       if (usingTofResolution() || usingMdcResolution()) { 
// 	Pt=0.;
// 	Ptf=0.;
//       } else {
// 	//Ptc=P*ttSint;
// 	Pt=2*P*sintheta;
// 	Ptf=2*P*sin((f2-f1)/2.);
//       }
      
//       // Ptc=fKickPlane->getPt(inter,errPt,1);
//       if (TMath::Abs(sintheta)>0.0) {
// 	Pc=Ptc/(2*sintheta);
// 	errP = Pc * TMath::Sqrt(( (0.01)*(0.01) + deltaY*deltaY));
// 	fData->Fill(P,Pc,errP,
// 		    f1,rodNb,0.,
// 		    inter.getX(),inter.getY(),inter.getZ(),
// 		    Pt,Ptf,0,0,Ptc,0.);
//       } else Warning("execute_res","Sintheta equals 0");
//     }
//   }

//   return 0;
// }

Bool_t HKickResolutionTof::init(void) {
  fKickPlane = HKickPlane2::getMeta();
  return HKickResolution::init();
}

Int_t HKickResolutionTof::execute(void) {
  //Calculates momentum using a GEANT MDC hit in MDC 1 and a TOF hit
  //where the x and y coordinates are smeared.
  HGeantMdc *data=0;
  HGeantTof *tofdata=0;
  HGeantKine *kineTrack=0;
  HGeomVector r[2],alpha[2],inter,cross,tofR,tofAlpha;
  HGeomVector pR,pAlpha; // Partner hit
  Int_t tofCell=0;
  HGeomVector errTofR;
  Double_t errP,k,yR;
  //Double_terrPt; // unused
  Float_t en=0,x=0,y=0,tof=0,len=0;
  Float_t Pmdc;
  Double_t rad2grad = 180. / TMath::Pi();

  Int_t pcount=0,count=0,sector=0;
  Double_t sintheta=0.;
  Double_t Pt,Ptt,Ptg,Ptc,Pp,Pcalc,dist;
  Float_t P=0,Ptof;
  Float_t Px=0,Py=0,Pz=0;
  
  kineTrack=(HGeantKine *)fKine->getObject(0); //Solo hay una traza
  kineTrack->getMomentum(Px,Py,Pz);

  tofdata = (HGeantTof *)fTofInput->getObject(0);
  if (!tofdata) return 0;
  tofdata->getHit(en,x,y,tof,Ptof,len);

  P = sqrt(Px*Px + Py*Py + Pz*Pz);
  
  fFitter.reset();
  fIter->Reset();
  fTofIter->Reset();
  while ( (data=(HGeantMdc *)fIter->Next()) != 0) {
    if ( (data->getModule() == 0 && data->getLayer() == 6) ||
	  (data->getModule() == 3 && data->getLayer() == 6)) {
      if (count <2) {
	sector=data->getSector();
	transform(data,r[count],alpha[count]); 
      } 
      count++;
    } else if ( (data->getModule() == 1 && data->getLayer() == 6) ) {
      transform(data,pR,pAlpha);
      pcount++;
      data->getHit(x,y,tof,Pmdc); //Only Pmdc is preserved
    }
  } 
  if (pcount == 1) {
    alpha[0] = pR - r[0];
    alpha[0] /= alpha[0].length();
  }
  fKickPlane->calcIntersection(r[0],alpha[0],inter);
  while ( (tofdata=(HGeantTof *)fTofIter->Next()) != 0) {
    if (tofdata->getSector() == sector && tofdata->getModule()<22 ) {
      if (count <3) {
	transform(tofdata,tofR,errTofR,k,yR);
	tofAlpha=tofR-inter;
	tofAlpha/=tofAlpha.length();
	tofCell=(21 - tofdata->getModule())*22+(7-tofdata->getCell());
	count++;
      }
    }
  }

  if (count > 3) Warning("execute","Inconsistent point fit");
  else if (count == 3 && pcount==1) {
    Double_t D;
    Double_t deltaY,deltaPt; //Contribution from to the error from Y, Pt
    Double_t g1,g2,f1,f2,t1,t2,et1,et2;
    Double_t Ptf;
    Float_t tuple_args[18];

    for (Int_t i=0;i<2;i++) { // Debe dar lo mismo que inter
      fFitter.addLine(r[i],alpha[i]);
    }
    fFitter.getVertex(cross);

    Pp=P*(alpha[0].scalarProduct(alpha[1])); // Viene directamente de la sim.
    Ptg=TMath::Sqrt(P*P-Pp*Pp); //idem

    //Metodo experimental de calculo de momento
    tofAlpha=tofR-inter;
    tofAlpha/=tofAlpha.length();
    D=TMath::Sqrt( (tofR.getY() - inter.getY()) *
		   (tofR.getY() - inter.getY()) +
		   (tofR.getZ() - inter.getZ()) *
		   (tofR.getZ() - inter.getZ()) );
    g1=TMath::ATan2(alpha[0].getZ(),alpha[0].getY());
    g2=TMath::ATan2(tofAlpha.getZ(),tofAlpha.getY());
    f1 = TMath::ATan2(alpha[0].getX(), alpha[0].getY());
    f2 = TMath::ATan2(tofAlpha.getX(), tofAlpha.getY());
    t1 = TMath::ACos(alpha[0].getZ());
    t2 = TMath::ACos(tofAlpha.getZ());
    et1 = TMath::ATan2(alpha[0].getX(),alpha[0].getZ());
    et2 = TMath::ATan2(tofAlpha.getX(),tofAlpha.getZ());
//    cout << et1 << " " << et2 << endl;

    sintheta=sin((g2-g1)/2);

    if (usingTofResolution() || usingMdcResolution()) {
      Ptg=0.; Ptf=0.; Ptt=0.; Pt=0.;
    } else {
      Pt = 2 * P * sin( acos(alpha[0].scalarProduct(tofAlpha)) / 2. );
      Ptg = 2 * P * sintheta;
      Ptf = 2 * P * sin( (f2-f1) / 2. );
      Ptt = 2 * P * sin( (t2-t1) / 2. );
    }
    //FIXME: Do properly
    //Ptc=fKickPlane->getPt(inter,errPt,(sintheta>0)?1:-1);
    //Pcalc=Ptc/(2*sintheta);
    Ptc = Pcalc = 0.;

    //Calculo del error en el momento
    deltaPt = 1.52e-2; //FIXME
    deltaY= cos((g2-g1)/2) *
      (inter.getZ() - k * inter.getY() - yR) * errTofR.getY();
    deltaY/= 2 * D * D * sintheta;
    errP=TMath::Sqrt(2.)*TMath::Sqrt(deltaY*deltaY + deltaPt*deltaPt);

    //Calculo de la distancia entre dos rectas
    HGeomVector prod=alpha[0].vectorProduct(alpha[1]);
    prod/=prod.length();
    dist=TMath::Abs((r[0]-r[1]).scalarProduct(prod));

    //Rellenado de datos de salida
    if (fData) {
      tuple_args[0]=P;  tuple_args[1]=Pcalc;   tuple_args[2]=errP;
      tuple_args[3]=cross.getX();
      tuple_args[4]=cross.getY();
      tuple_args[5]=cross.getZ();
      tuple_args[6]=inter.getX();
      tuple_args[7]=inter.getY();
      tuple_args[8]=inter.getZ();
      tuple_args[9]=Ptg;
      tuple_args[10]=Ptf;
      tuple_args[11]=Ptt;
      tuple_args[12]=Pt;
      tuple_args[13]=Ptc;
      tuple_args[14]=dist;
      tuple_args[15]=et1;
      tuple_args[16]=et2;
      tuple_args[17]=Pmdc;
      fData->Fill(tuple_args);
    }
    if (fControl) fControl->Fill(P,(t1 * rad2grad),
				 (TMath::ACos(tofR.getZ()/tofR.length()) * 
				  rad2grad),
				 (TMath::ATan2(alpha[0].getY(),alpha[0].getX())
				  * rad2grad),
				 (TMath::ATan2(tofR.getY(),tofR.getX()) *
				  rad2grad) );
  } else {
    //Warning("execute","Track didn't hit 2 detectors %i %i",count,pcount);
  }
  return 0;
}


///////////////////////////////////////////////////////////////////////////////////

Bool_t HKickResolutionMdc::init(void) {
  fKickPlane = HKickPlane::getMDC3();
  return HKickResolution::init();
}

Int_t HKickResolutionMdc::execute(void) {
  HGeantMdc *data=0;
  HGeantKine *kineTrack=0;
  HGeomVector r[2],alpha[2],inter,cross;
  Int_t count=0,sector=0;
  Double_t sintheta,costheta=0.0;
  Double_t P,Pt,Ptg,Ptf,Ptc,Pp,Pcalc,errPt;
  Float_t Px=0,Py=0,Pz=0;
  Double_t dist=0.;
  Int_t pcount = 0;
  HGeomVector partR,partAlpha;

  kineTrack=(HGeantKine *)fKine->getObject(0); //Solo hay una traza
  kineTrack->getMomentum(Px,Py,Pz);
  P=TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);

  fIter->Reset();
  fFitter.reset();
  while ( (data=(HGeantMdc *)fIter->Next()) != 0) {
    if ( (data->getModule() == 0 && data->getLayer() == 6) ||
	 (data->getModule() == 2 && data->getLayer() == 6) ) {
      if (count <2) {
	sector=data->getSector();
	transform(data,r[count],alpha[count]);
	count++;
      }
    } else if ( data->getModule() == 1 && data->getLayer() == 6 ) {
      transform(data,partR,partAlpha);
      pcount++;
    }

  }
  if (count > 2) Warning("execute","Inconsistent point fit");
  else if (count == 2 && pcount==1) {
    alpha[0] = partR - r[0];
    alpha[0] /= alpha[0].length();

    for (Int_t i=0;i<2;i++) {
      fFitter.addLine(r[i],alpha[i]);
    }
    fFitter.getVertex(cross);

    Double_t g1=TMath::ATan2(alpha[0].getZ(),alpha[0].getY());
    Double_t g2=TMath::ATan2(alpha[1].getZ(),alpha[1].getY());
    Double_t f1 = TMath::ATan2(alpha[0].getY(), alpha[0].getX());
    Double_t f2 = TMath::ATan2(alpha[1].getY(), alpha[1].getX());

    fKickPlane->calcIntersection(r[0],alpha[0],inter);
    Ptc=fKickPlane->getPt(inter,errPt);

    //FIXME #warning "using kick position for momentum calc."
    HGeomVector dir = alpha[1];//r[1] - inter; dir /= dir.length();
    g2 = TMath::ATan2(dir.getZ(), dir.getY());

    costheta=alpha[0].scalarProduct(dir);
    sintheta=TMath::Sqrt(1 - costheta * costheta);

    Pp=P*costheta;
    Pt=TMath::Sqrt(P*P-Pp*Pp);

    Ptg = 2. * P * sin( (g2 - g1) / 2.);
    Ptf = 2. * P * sin( (f2 - f1) / 2.);

    Pcalc=Ptc/(2. * sin( (g2 - g1) / 2.));

    // Distancia entre dos rectas
    HGeomVector prod=alpha[0].vectorProduct(alpha[1]);
    prod/=prod.length();
    dist=TMath::Abs((r[0]-r[1]).scalarProduct(prod));

//     cerr << dist << " == ";
//     cerr << (r[0] - cross).vectorProduct(alpha[0]).length() << " + ";
//     cerr << (r[1] - cross).vectorProduct(alpha[1]).length() << " ?\n";

    if (fData) fData->Fill(P,Pcalc,0.,
			   cross.getX(),cross.getY(),cross.getZ(),
			   inter.getX(),inter.getY(),inter.getZ(),
			   Ptg,Ptf,0,Pt,Pt,dist);

  } else {
    // Warning("execute","Track didn't hit 2 detectors");
  }
  return 0;
}

/////////////////////////////////////////////////////////////////////

Bool_t HKickResolutionMdc2::init(void) {
  fKickPlane = HKickPlane2::getMDC3();
  return HKickResolution::init();
}

Int_t HKickResolutionMdc2::execute(void) {
  HGeantMdc *data=0;
  HGeantKine *kineTrack=0;
  HGeomVector r[2],alpha[2],inter,cross;
  Int_t count=0,sector=0;
  Double_t sintheta,costheta=0.0;
  Double_t P,Pt,Ptg,Ptf,Ptc,Pp,Pcalc;
  //Double_t errPt; // unused
  Float_t Px=0,Py=0,Pz=0;
  Double_t dist=0.;
  Int_t pcount = 0;
  HGeomVector partR,partAlpha;
  HGeantTof *tofdata=0;
  Float_t en,x,y,tof,Ptof,Pmdc,len;
  Double_t k,yR;
  HGeomVector tofR,errTofR;
  Float_t tuple_args[18];

  kineTrack=(HGeantKine *)fKine->getObject(0); //Solo hay una traza
  kineTrack->getMomentum(Px,Py,Pz);
  P=TMath::Sqrt(Px*Px + Py*Py + Pz*Pz);

  fIter->Reset();
  fFitter.reset();
  while ( (data=(HGeantMdc *)fIter->Next()) != 0) {
    if ( (data->getModule() == 0 && data->getLayer() == 6) ||
	 (data->getModule() == 2 && data->getLayer() == 6) ) {
      if (count <2) {
	sector=data->getSector();
	transform(data,r[count],alpha[count]);
	count++;
      }
    } else if ( data->getModule() == 1 && data->getLayer() == 6 ) {
      transform(data,partR,partAlpha);
      pcount++;
      data->getHit(x,y,tof,Pmdc); //Only Pmdc is preserved
    }

  }
  if (count > 2) Warning("execute","Inconsistent point fit");
  else if (count == 2 && pcount==1) {
    alpha[0] = partR - r[0];
    alpha[0] /= alpha[0].length();

    HGeomVector dir;
    
    fKickPlane->calcIntersection(r[0],alpha[0],inter);
    switch (fMode) {
    case kKickPosMode:
      dir = r[1] - inter; 
      dir /= dir.length();
      break;
    case kSlopeMode:
      dir = alpha[1];
      break;
    case kMdcTofMode:
      // I'm only interested on one track point around the TOF region
      //so I don't need to care if there is a double hit (cluster) in TOF
      //or not. In simulations both hits lie on top of the track; so
      //I can take just the first one.
      tofdata = (HGeantTof *)fTofInput->getObject(0);
      if (!tofdata) return 0;
      tofdata->getHit(en,x,y,tof,Ptof,len);
      transform(tofdata,tofR,errTofR,k,yR);
      alpha[1] = tofR - r[1];
      alpha[1] /= alpha[1].length();
      dir = alpha[1];
      break;
    default:
      Error("execute","Internal error 1");
    };

    for (Int_t i=0;i<2;i++) {
      fFitter.addLine(r[i],alpha[i]);
    }
    fFitter.getVertex(cross);

    Double_t g1=TMath::ATan2(alpha[0].getZ(),alpha[0].getY());
    Double_t g2=TMath::ATan2(dir.getZ(),dir.getY());
    Double_t f1 = TMath::ATan2(alpha[0].getY(), alpha[0].getX());
    Double_t f2 = TMath::ATan2(dir.getY(), dir.getX());
    Float_t et1 = TMath::ATan2(alpha[0].getX(), alpha[0].getZ());
    Float_t et2 = TMath::ATan2(dir.getX(), dir.getZ());

    Ptc = 0;

    costheta = alpha[0].scalarProduct(dir);
    sintheta = 2*TMath::Sin(TMath::ACos(costheta)/2.);
    Pp=P*costheta;
    Pt=P*sintheta;//TMath::Sqrt(P*P-Pp*Pp);

    Ptg = 2. * P * sin( (g2 - g1) / 2.);
    Ptf = 2. * P * sin( (f2 - f1) / 2.);

    Pcalc=fKickPlane->getP(inter,sintheta,1);

    // Distancia entre dos rectas
    HGeomVector prod=alpha[0].vectorProduct(dir);
    prod/=prod.length();
    dist=TMath::Abs((r[0]-r[1]).scalarProduct(prod));

/*    cerr << dist << " == ";
    cerr << (r[0] - cross).vectorProduct(alpha[0]).length() << " + ";
    cerr << (r[1] - cross).vectorProduct(dir).length() << " ?\n";
*/
    tuple_args[0] = P;
    tuple_args[1] = Pcalc;
    tuple_args[2] = fMode;
    tuple_args[3] = cross.getX();
    tuple_args[4] = cross.getY();
    tuple_args[5] = cross.getZ();
    tuple_args[6] = inter.getX();
    tuple_args[7] = inter.getY();
    tuple_args[8] = inter.getZ();
    tuple_args[9] = Ptg;
    tuple_args[10] = Ptf;
    tuple_args[11] = 0;
    tuple_args[12] = Pt;
    tuple_args[13] = Pt;
    tuple_args[14] = dist;
    tuple_args[15] = et1;
    tuple_args[16] = et2;
    tuple_args[17] = Pmdc;
    if (fData) fData->Fill(tuple_args);
  } else {
    // Warning("execute","Track didn't hit 2 detectors");
  }
  return 0;
}

ClassImp(HKickResolution)
ClassImp(HKickResolutionTof)
ClassImp(HKickResolutionMdc)
ClassImp(HKickResolutionMdc2)

