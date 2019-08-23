#include "hmetaalignerutil.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdcdef.h"
#include "hevent.h"
#include "tofdef.h"
#include "showerdef.h"

Float_t g_fSqrt_12_1 = (Float_t)(1.0 / sqrt(12.0));

HMetaAlignerKickShowerTransform::HMetaAlignerKickShowerTransform(void) {
}

HMetaAlignerKickShowerTransform::HMetaAlignerKickShowerTransform(Float_t showerDX, Float_t showerDY,
                       Float_t showerDZ) {
    fShowerDX = showerDX;
    fShowerDX = showerDY;
    fShowerDX = showerDY;
}

HMetaAlignerKickShowerTransform::~HMetaAlignerKickShowerTransform(void) {
}

void HMetaAlignerKickShowerTransform::setGeometry(HShowerGeometry *geom) {
}

void HMetaAlignerKickShowerTransform::transform(HShowerHit *hit, HGeomVector &r) const
{
  Float_t x=0,y=0;

  HGeomVector local;

  const HGeomTransform &trans = getTransform(hit->getSector());
  hit->getXY(&x, &y);

  local.setXYZ(x,y,0);
  r = trans.transFrom(local);
}

void HMetaAlignerKickShowerTransform::getSigmas(HShowerHit *hit,
            Float_t &fSigmaX, Float_t &fSigmaY) const
{
HShowerPad *pPad;

    pPad = fGeometry->getPadParam(0)->getPad(hit->getRow(), hit->getCol());

    // height
    fSigmaY = (pPad->getYlu_mm() - pPad->getYld_mm()) * g_fSqrt_12_1;
    // length of upper side
    fSigmaX = (pPad->getXru_mm() - pPad->getXlu_mm()) * g_fSqrt_12_1;
/*
    printf("%2d %2d -> %6.2f %6.2f\n", hit->getRow(), hit->getCol(),
                        pPad->getXru_mm() - pPad->getXlu_mm(),
                        pPad->getYlu_mm() - pPad->getYld_mm());
*/
}

Bool_t HMetaAlignerKickShowerTransform::init(void) {
  HRuntimeDb *rtdb = 0;
  Bool_t r = kTRUE;

  if (gHades)
    if ( (rtdb = gHades->getRuntimeDb()) != 0) {
      HSpectrometer *spec = gHades->getSetup();

      if (spec) {
    fGeometry = (HShowerGeometry *)rtdb->getContainer("ShowerGeometry");
    if (!fGeometry) {
      fGeometry = new HShowerGeometry;
      rtdb->addContainer(fGeometry);
    }

      } else r = kFALSE;
    } else r = kFALSE;
  return r;
}

void HMetaAlignerKickShowerTransform::toLocal(HGeomVector &r,Int_t sector,
                   HGeomVector &out) const {

  const HGeomTransform &trans = getTransform(sector);
  out = trans.transTo(r);
}

HGeomTransform g_showerTransform;

const HGeomTransform &HMetaAlignerKickShowerTransform::getTransform(Int_t sector) const {
  HGeomRotation alig;

  alig.setEulerAngles(0,0,0);
  HGeomVector desp(fShowerDX, fShowerDY, fShowerDZ);

  const HGeomTransform &modTrans = fGeometry->getTransform(sector,0);

  g_showerTransform.setRotMatrix(modTrans.getRotMatrix()*alig);
  g_showerTransform.setTransVector(desp + modTrans.getTransVector());
  return g_showerTransform;
}

HMetaAlignerKickTofTransform::HMetaAlignerKickTofTransform(void) {
  fGeometry = 0;
}

HMetaAlignerKickTofTransform::~HMetaAlignerKickTofTransform(void) {
  fGeometry = 0;
}

void HMetaAlignerKickTofTransform::setGeometry(HTofGeomPar *geom) {
  fGeometry = geom;
}

void HMetaAlignerKickTofTransform::transform(HTofHit *data,
                  HGeomVector &r) const {
  HGeomVector rLocal;

  HModGeomPar *module=fGeometry->getModule(data->getSector(), //Sector
                       data->getModule());
  HGeomTransform &modTrans = getTransform(data->getSector(),
                      data->getModule());
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();

  r.setX(data->getXpos());
  r.setY(0.);
  r.setZ(0.);

  rLocal=rodTrans.transFrom(r); //Rod to modue system

  r=modTrans.transFrom(rLocal); //Module to LAB system
}

HGeomTransform tofTransform;

HGeomTransform &HMetaAlignerKickTofTransform::getTransform(Int_t sector,Int_t module) const
{
/*
  //Float_t ymis[] = {3,2,1,0,0,-3,-6,-9}; //FIXME>: Not for DST
  HGeomRotation alig;

  alig.setEulerAngles(0,0,0);
  //HGeomVector desp(0, 2, 2);      // <-- params like for shower
  HGeomVector desp(0, 0, 0);        // <-- params like for shower

  HModGeomPar *mod=fGeometry->getModule(sector,module);
  HGeomTransform &modTrans = mod->getLabTransform();

  tofTransform.setRotMatrix(modTrans.getRotMatrix()*alig);
  tofTransform.setTransVector(desp + modTrans.getTransVector());
  return tofTransform;
*/

HModGeomPar    *mod      = fGeometry->getModule(sector, module);
HGeomTransform &modTrans = mod->getLabTransform();

    tofTransform.setRotMatrix(modTrans.getRotMatrix());
    tofTransform.setTransVector(modTrans.getTransVector());

    return tofTransform;
}


void HMetaAlignerKickTofTransform::toLocal(HGeomVector &r, HTofHit *data,
                HGeomVector &out) const {
  HGeomVector rLocal;
  HGeomTransform &modTrans = getTransform(data->getSector(),
                      data->getModule());
  //HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  //HGeomTransform &rodTrans=rodVol->getTransform();

  out = modTrans.transTo(r);
}

void HMetaAlignerKickTofTransform::getLocal(HTofHit *data,
                  HGeomVector &r) const {
  HGeomVector rRod;

  HModGeomPar *module=fGeometry->getModule(data->getSector(), //Sector
                       data->getModule());
//HGeomTransform &modTrans = getTransform(data->getSector(), data->getModule());
  HGeomVolume *rodVol=module->getRefVolume()->getComponent(data->getCell());
  HGeomTransform &rodTrans=rodVol->getTransform();

  rRod.setX(data->getXpos());
  rRod.setY(0.);
  rRod.setZ(0.);

  r = rodTrans.transFrom(rRod); //Rod to modue system
}


void HMetaAlignerKickTofTransform::getSigmas(HTofHit *hit,
            Float_t &fSigmaX, Float_t &fSigmaY) const
{
HModGeomPar *module=fGeometry->getModule(hit->getSector(),
                       hit->getModule());
HGeomVolume *rodVol=module->getRefVolume()->getComponent(hit->getCell());
Double_t     dMin = 0.0, dMax = 0.0;
HGeomVector *pPoint = NULL;

    for(Int_t i = 0; i < rodVol->getNumPoints(); i++)
    {
        pPoint = rodVol->getPoint(i);
        if(i == 0)
        {
            dMax = dMin = pPoint->getY();
            continue;
        }

        if(dMin > pPoint->getY())
            dMin = pPoint->getY();
        else
            if(dMax < pPoint->getY())
                dMax = pPoint->getY();
    }

    fSigmaY = (dMax - dMin) * g_fSqrt_12_1; // rod height
    fSigmaX = 25.0f; // TOF resolution
}


Bool_t HMetaAlignerKickTofTransform::init(void) {
  HRuntimeDb *rtdb = 0;
  Bool_t r = kTRUE;

  if (gHades)
    if ( (rtdb = gHades->getRuntimeDb()) != 0) {
      HSpectrometer *spec = gHades->getSetup();

      if (spec) {
    fGeometry = (HTofGeomPar *)rtdb->getContainer("TofGeomPar");
    if (!fGeometry) {
      fGeometry = (HTofGeomPar *)spec->createDetGeomPar("Tof");
      if (!fGeometry) r = kFALSE;
    }

    fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
    if (!fSpecGeometry) {
      fSpecGeometry = spec->createSpecGeomPar();
      if (!fSpecGeometry) r = kFALSE;
    }

      } else r = kFALSE;
    } else r = kFALSE;
  return r;
}

HMetaAlignerKickMdcTransform::HMetaAlignerKickMdcTransform(void) {
  fGeometry = 0;
}

HMetaAlignerKickMdcTransform::~HMetaAlignerKickMdcTransform(void) {
  fGeometry = 0;
}

void HMetaAlignerKickMdcTransform::setGeometry(HMdcGeomPar *geom) {
  fGeometry = geom;
}

void HMetaAlignerKickMdcTransform::transform(HMdcSeg *hit,
                  HGeomVector &r,HGeomVector &alpha) const {
  HGeomTransform &transSec = fSpecGeometry->getSector(hit->getSec())
    ->getTransform();
  Float_t theta,phi;
  Double_t pi2=TMath::Pi()/2.;
  HGeomVector rLocal,alphaLocal;

  theta=hit->getTheta();
  phi=hit->getPhi();

  rLocal.setZ(hit->getZ());
  rLocal.setX(hit->getR()*TMath::Cos(phi+pi2));
  rLocal.setY(hit->getR()*TMath::Sin(phi+pi2));
  r = transSec.transFrom(rLocal);

  alphaLocal.setZ(TMath::Cos(theta));
  alphaLocal.setY(TMath::Sin(theta)*TMath::Sin(phi));
  alphaLocal.setX(TMath::Sin(theta)*TMath::Cos(phi));
  alpha = transSec.getRotMatrix() * alphaLocal;
}

Bool_t HMetaAlignerKickMdcTransform::init(void) {
  HRuntimeDb *rtdb = 0;
  Bool_t r = kTRUE;

  if (gHades)
    if ( (rtdb = gHades->getRuntimeDb()) != 0) {
      HSpectrometer *spec = gHades->getSetup();

      if (spec) {
    fGeometry = (HMdcGeomPar *)rtdb->getContainer("MdcGeomPar");
    if (!fGeometry) {
      fGeometry = (HMdcGeomPar *)spec->createDetGeomPar("Mdc");
      if (!fGeometry) r = kFALSE;
    }

    fSpecGeometry = (HSpecGeomPar *)rtdb->getContainer("SpecGeomPar");
    if (!fSpecGeometry) {
      fSpecGeometry = spec->createSpecGeomPar();
      if (!fSpecGeometry) r = kFALSE;
    }

      } else r = kFALSE;
    } else r = kFALSE;
  return r;
}

HMetaAlignerTask::HMetaAlignerTask(void) : HReconstructor("align","align"),
               fShowerGeom(0.,0.,0.) {
  fMdcHits = 0;
  fShowerHits = 0;
  fTofHits = 0;
  fMdcIter = 0;
  fShowerIter = 0;
  fTofIter = 0;
}

HMetaAlignerTask::HMetaAlignerTask(Float_t fShowerDX, Float_t fShowerDY, Float_t fShowerDZ)
  : HReconstructor("align","align"), fShowerGeom(fShowerDX, fShowerDY,
                         fShowerDZ)
{
    fMdcHits = 0;
    fShowerHits = 0;
    fTofHits = 0;
    fMdcIter = 0;
    fShowerIter = 0;
    fTofIter = 0;
}

HMetaAlignerTask::~HMetaAlignerTask(void) {
}

Float_t HMetaAlignerTask::distance(HGeomVector &r, HGeomVector &a, HGeomVector &p) {
  //Distance from line (r,a) to p
  return ((r-p).vectorProduct(a)).length();
}

void HMetaAlignerTask::calcClosestPoint(HGeomVector &r, HGeomVector &a,
                HGeomVector &r2,HGeomVector &p) {
  // sets p as the closest point in line (r,a) to the point r2.
  HGeomVector delta = a;
  delta *= (r - r2).scalarProduct(a);
  p = r - delta;
  //p.setZ( r2.getZ() );
  //p.setX( r.getX() + (a.getX() / a.getZ()) * (r2.getZ() - r.getZ()) );
  //p.setY( r.getY() + (a.getY() / a.getZ()) * (r2.getZ() - r.getZ()) );
}

Float_t HMetaAlignerTask::calcSign(HGeomVector &r,HGeomVector &alpha,HGeomVector &rOut)
{
  Float_t test = (r.getZ() - rOut.getZ()) * alpha.getY() -
    (r.getY() - rOut.getY()) * alpha.getZ();
  return (test>0)?1:-1;
}

Int_t HMetaAlignerTask::execute(void) {
  HMdcSeg *seg = 0;
  HTofHit *tofHit = 0;
  HShowerHit *shoHit = 0;
  Int_t id=0,sys=2;
  HGeomVector r,alpha,rOut,rTemp,rClosest;
  HGeomVector rLocal,rOutLocal,alphaLocal;
  Float_t dist=5000,temp=5000,sig=0,beta;

  //Float_t distMax = 1000.;
  Float_t distMax = 300.;

  for (Int_t sector=0; sector<6; sector++) {
    fInnerOuterLoc[0] = sector;
    fMdcIter->Reset();
    fMdcIter->gotoLocation(fInnerOuterLoc);
    while ( (seg = (HMdcSeg *)fMdcIter->Next()) != 0) {
      id = 0;
      fMdcGeom.transform(seg,r,alpha);

      fTofIter->Reset();
      while ( (tofHit = (HTofHit *)fTofIter->Next()) != 0) {
    if (tofHit->getSector() == sector) {
      fTofGeom.transform(tofHit,rTemp);
      temp = distance(r,alpha,rTemp);
      if ( temp < distMax ) {
        HGeomTransform &t=fTofGeom.getTransform(sector,tofHit->getModule());
        rOut = rTemp;
        calcClosestPoint(r,alpha,rOut,rClosest);
        sig = calcSign(rClosest,alpha,rOut);
        dist = temp;
        id++;
        sys = 1;
        fTofGeom.toLocal(rClosest,tofHit,rLocal);
        fTofGeom.toLocal(rOut,tofHit,rOutLocal);
        alphaLocal = t.getRotMatrix() * alpha;
        // Calculate intersection with detector plane
        beta = alphaLocal.getX() / alphaLocal.getZ();
        rLocal.setX(rLocal.getX() +
            beta * (rOutLocal.getZ() - rLocal.getZ()));
        beta = alphaLocal.getY() / alphaLocal.getZ();
        rLocal.setY(rLocal.getY() +
            beta * (rOutLocal.getZ() - rLocal.getZ()));
        rLocal.setZ(rOutLocal.getZ());
      }
    }
      }

      fShowerIter->Reset();
      while ( (shoHit = (HShowerHit *)fShowerIter->Next()) != 0) {
    if (shoHit->getSector() == sector && shoHit->getModule() == 0) {
      //fShowerGeom.transform(shoHit,rTemp);
      fShowerGeom.transform(shoHit, rTemp);
      temp = distance(r,alpha,rTemp);
      if ( temp < distMax ) {
        const HGeomTransform &t=fShowerGeom.getTransform(sector);
        rOut = rTemp;
        dist = temp;
        calcClosestPoint(r,alpha,rOut,rClosest);
        fShowerGeom.toLocal(rClosest,sector,rLocal);
        fShowerGeom.toLocal(rOut,sector,rOutLocal);
        alphaLocal = t.getRotMatrix() * alpha;
        sig = calcSign(rClosest,alpha,rOut);
        id++;
        sys = 0;
        // Calculate intersection with detector plane
        beta = alphaLocal.getX() / alphaLocal.getZ();
        rLocal.setX(rLocal.getX() +
            beta * (rOutLocal.getZ() - rLocal.getZ()));
        beta = alphaLocal.getY() / alphaLocal.getZ();
        rLocal.setY(rLocal.getY() +
            beta * (rOutLocal.getZ() - rLocal.getZ()));
        rLocal.setZ(rOutLocal.getZ());
      }
    }
      }
      if (id == 1) {
    fData->Fill(dist,rClosest.getX(),rClosest.getY(),rClosest.getZ(),
            rOut.getX(),rOut.getY(),rOut.getZ(),id,
            sector,sys,sig);
    fDataLocal->Fill(rLocal.getX(),rLocal.getY(),rLocal.getZ(),
             rOutLocal.getX(),rOutLocal.getY(),rOutLocal.getZ(),
             sector,sys);
      }
    }
  }

  return 0;
}

Bool_t HMetaAlignerTask::init(void) {
  Bool_t r = kFALSE;
  HEvent *ev = 0;

  if (!gHades) return kFALSE;
  ev = gHades->getCurrentEvent();

  r = fMdcGeom.init() && fTofGeom.init() && fShowerGeom.init();

  fData = new TNtuple("metaAlign","Meta Alignment","d:xc:yc:zc:xo:yo:zo:n:s:sys:sig");
  fDataLocal = new TNtuple("metaAlignLoc","Meta Alignment Local",
               "xc:yc:zc:xo:yo:zo:s:sys");

  fMdcHits = ev->getCategory(catMdcSeg);
  fShowerHits = ev->getCategory(catShowerHit);
  fTofHits = ev->getCategory(catTofHit);

  if (!(fMdcHits && fShowerHits && fTofHits)) return kFALSE;

  fMdcIter = (HIterator *)fMdcHits->MakeIterator();
  fShowerIter = (HIterator *)fShowerHits->MakeIterator();
  fTofIter = (HIterator *)fTofHits->MakeIterator();

  fInnerOuterLoc.set(2,0,0);
  return r;
}

Bool_t HMetaAlignerTask::reinit(void) {
  Bool_t r = kTRUE;

  return r;
}

Bool_t HMetaAlignerTask::finalize(void) {
  Bool_t r = kTRUE;

  gHades->getOutputFile()->cd();
  fData->Write();
  fDataLocal->Write();

  return r;
}


ClassImp(HMetaAlignerKickShowerTransform)
ClassImp(HMetaAlignerKickTofTransform)
ClassImp(HMetaAlignerKickMdcTransform)
ClassImp(HMetaAlignerTask)
