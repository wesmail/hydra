#include "hshowerhit.h"
#include "hshowerhitheader.h"
#include "hshowerhitfinder.h"
#include "hshowerhitfinderold.h"


ClassImp(HShowerHitFinderOld)


HShowerHitFinderOld::HShowerHitFinderOld(Text_t *name,Text_t *title) :
  HShowerHitFinder(name,title)
{
}

HShowerHitFinderOld::HShowerHitFinderOld()
{
}


void HShowerHitFinderOld::calcCoord(HShowerHit* hit, HLocation &fLoc) {
  float fX, fY, fRow, fCol;
  float fXCoeff[2], fYCoeff[2];
  float fXOffset, fYOffset;

  fXCoeff[0] = -51.2;
  fXCoeff[1] = 3.2 ;
  fXOffset = -1.0;

  fYCoeff[0] = 2.25;
  fYCoeff[1] = -4.5;
  fYOffset = 19.0;

  fRow = fLoc[2];
  fCol = fLoc[3];
  
  fX =  fXCoeff[1] * (fCol - fXOffset ) + fXCoeff[0];
  fY =  fYCoeff[1] * (fRow - fYOffset ) + fYCoeff[0];  

  hit->setXY(fX, fY);
}
