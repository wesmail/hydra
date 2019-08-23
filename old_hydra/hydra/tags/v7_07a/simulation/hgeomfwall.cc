//*-- AUTHOR : Ilse Koenig
//*-- Created : 08/11/2004

/////////////////////////////////////////////////////////////
// HGeomFWall
//
// Class for geometry of Forward Wall
/////////////////////////////////////////////////////////////

#include "hgeomfwall.h"
#include "hgeomnode.h"
#include "hgeommedia.h"
#include "hgeomfwallhit.h"

ClassImp(HGeomFWall)

HGeomFWall::HGeomFWall() {
  // Constructor
  fName="fwall";
  strcpy(modName,"WALL");
  strcpy(eleName,"W");
  maxSectors=0;
  maxModules=1;
  pHit=new HGeomFWallHit(this);
}
