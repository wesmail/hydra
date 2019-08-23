#include "hmetaalignerutil.h"
#include "hmetaalignerutil2.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hgeomvolume.h"
#include "hgeomcompositevolume.h"
#include "hmdcdef.h"
#include "hevent.h"
#include "tofdef.h"
#include "showerdef.h"

HMetaAlignerTask2::HMetaAlignerTask2(void) : HReconstructor("align","align") {
  fMdcHits = 0;
  fShowerHits = 0;
  fTofHits = 0;
  fMdcIter = 0;
  fShowerIter = 0;
  fTofIter = 0;

  fShowerGeom.setTranslation(0.,0.,0.);
  m_fWindowSize = 300.0;
}

HMetaAlignerTask2::HMetaAlignerTask2(Float_t fShowerDX, Float_t fShowerDY, Float_t fShowerDZ,
             Float_t fWindowSize)
  : HReconstructor("align","align"), fShowerGeom(fShowerDX, fShowerDY,
                         fShowerDZ)
{
  fMdcHits = 0;
  fShowerHits = 0;
  fTofHits = 0;
  fMdcIter = 0;
  fShowerIter = 0;
  fTofIter = 0;

  m_fWindowSize = fWindowSize;
}


HMetaAlignerTask2::~HMetaAlignerTask2(void) {
}

Float_t HMetaAlignerTask2::distance(HGeomVector &r, HGeomVector &a, HGeomVector &p) {
  //Distance from line (r,a) to p
  return ((r-p).vectorProduct(a)).length();
}

Int_t HMetaAlignerTask2::execute(void) {
  HMdcSeg *seg = 0;
  HTofHit *tofHit = 0;
  HShowerHit *shoHit = 0;
  Int_t id=0,sys=2;
  HGeomVector r,alpha,rOut,rTemp,rClosest;
  HGeomVector rLocal,rOutLocal,alphaLocal;
  Float_t dist=5000,temp=5000;

  Float_t fSigmaX, fSigmaY;

  Float_t distMax = m_fWindowSize;

 // printf("******\nWindow size: %f\n******\n", distMax);
  Int_t module=0;

  //  cout << endl;
  for (Int_t sector=0; sector<6; sector++) {
    fInnerOuterLoc[0] = sector;
    fMdcIter->Reset();
    fMdcIter->gotoLocation(fInnerOuterLoc);
    //    cout << "-"; cout.flush();
    while ( (seg = (HMdcSeg *)fMdcIter->Next()) != 0) {
      id = 0;
      fMdcGeom.transform(seg,r,alpha);

      fTofIter->Reset();
      while ( (tofHit = (HTofHit *)fTofIter->Next()) != 0) {
    if (tofHit->getSector() == sector) {

#if 0 // skip all bad modules (May01)
        if((sector == 1) &&
                ((tofHit->getModule() == 0)
                || (tofHit->getModule() == 1)
                || (tofHit->getModule() == 6)
                || (tofHit->getModule() == 7)))
        {
            printf("Event skiped\n");
            continue;
            printf("111\n");
        }
#endif

      fTofGeom.transform(tofHit,rTemp);
      temp = distance(r,alpha,rTemp);

      if ( temp < distMax) {
        //  cout << "."; cout.flush();
        rOut = rTemp;
        dist = temp;
        sys = 1;
        id++;
        module = tofHit->getModule();
        fTofGeom.toLocal(rOut,tofHit,rOutLocal);
        fTofGeom.getSigmas(tofHit, fSigmaX, fSigmaY);
      }
    }
      }

      //      cout << "|"; cout.flush();
      fShowerIter->Reset();
      while ( (shoHit = (HShowerHit *)fShowerIter->Next()) != 0) {
    if (shoHit->getSector() == sector && shoHit->getModule() == 0) {
      fShowerGeom.transform(shoHit,rTemp);

      temp = distance(r,alpha,rTemp);
      if ( temp < distMax ) {
        //  cout << "."; cout.flush();
        rOut = rTemp;
        dist = temp;
        fShowerGeom.toLocal(rOut,sector,rOutLocal);
        sys = 0;
        id++;
        module = 0;
        fShowerGeom.getSigmas(shoHit, fSigmaX, fSigmaY);
      }
    }
      }
      if (id == 1) {
    fData->Fill(dist,r.getX(),r.getY(),r.getZ(),
            alpha.getX(),alpha.getY(),alpha.getZ(),
            rOutLocal.getX(),rOutLocal.getY(),fSigmaX,fSigmaY,
            sector,module,sys);
      }
    }
  }

  return 0;
}

Bool_t HMetaAlignerTask2::init(void) {
  Bool_t r = kFALSE;
  HEvent *ev = 0;

  if (!gHades) return kFALSE;
  ev = gHades->getCurrentEvent();

  r = fMdcGeom.init() && fTofGeom.init() && fShowerGeom.init();

  fData = new TNtuple("metaAlign","Meta Alignment","d:xc:yc:zc:alphax:alphay:alphaz:xo:yo:sigxo:sigyo:s:mod:sys");

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

Bool_t HMetaAlignerTask2::reinit(void) {
  Bool_t r = kTRUE;

  return r;
}

Bool_t HMetaAlignerTask2::finalize(void) {
  Bool_t r = kTRUE;

  gHades->getOutputFile()->cd();
  fData->Write();

  return r;
}


ClassImp(HMetaAlignerTask2)
