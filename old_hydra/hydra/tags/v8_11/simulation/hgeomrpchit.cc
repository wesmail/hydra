//*-- AUTHOR : Ilse Koenig
//*-- Created : 10/11/2003

//_HADES_CLASS_DESCRIPTION
/////////////////////////////////////////////////////////////
// HGeomRpcHit
//
// Class for RPC hit definition in GEANT
//
/////////////////////////////////////////////////////////////

#include "hgeomrpchit.h"
#include "hgeomrpc.h"
#include "hgeomnode.h"

ClassImp(HGeomRpcHit)

HGeomRpcHit::HGeomRpcHit(HGeomRpc* p) : HGeomHit(p) {
  // Constructor
}

Int_t HGeomRpcHit::getIdType() {
  // Returns the idType for the current node
  if (!currentNode) return 0;
  const char* volumeName=currentNode->GetName();
  Int_t l1=(Int_t)(volumeName[2]-'0');          // -'0' is mandatory for getting the number
  Int_t l2=(Int_t)(volumeName[3]-'0');         
 
  if (l2>=49) l2=l2-39;                         //This is how a(49) , b(50), c(51), d(52), is encoded ->> 10,11,12

  return 500 + 40*(l1-1) + (l2-1);
}
