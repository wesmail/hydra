//*-- AUTHOR : M. Sanchez (13.10.2000)


/////////////////////////////////////////////////////////////
// HKickParAsciiFileIo
//
// Class for Kick parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////

#include "hkickparasciifileio.h"
#include "hkickplane.h"
#include "hkicktrackfpar.h"

ClassImp(HKickParAsciiFileIo)

HKickParAsciiFileIo::HKickParAsciiFileIo(fstream* f) : HDetParAsciiFileIo(f) {
  // constructor calls the base class constructor
  fName="HKickParIo";
}

Bool_t HKickParAsciiFileIo::init(HParSet* pPar,Int_t* set) {
  // calls the appropriate read function for the container
  const  Text_t* name=pPar->GetName();
  if (pFile) {
    pFile->clear();
    pFile->seekg(0,ios::beg);

    if(!strcmp(name,"KickPlaneMeta"))
      return ((HKickPlane*)pPar)->read(pFile,set,inputNumber);
    if(!strcmp(name,"KickPlaneMDC3"))
      return ((HKickPlane*)pPar)->read(pFile,set,inputNumber);
    if (!strcmp(name,"KickTrackFPar"))
      return ((HKickTrackFPar*)pPar)->read(pFile,set,inputNumber);   

    Error("init","Container %s NOT initialized",name);
    return kFALSE;
  }
  Error("init","No input file open");
  return kFALSE;
}

Int_t HKickParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  if (pFile) {
    const  Text_t* name=pPar->GetName();

    if(!strcmp(name,"KickPlane"))
      return ((HKickPlane*)pPar)->writeFile(pFile);
    if(!strcmp(name,"KickTrackFPar"))
      return ((HKickTrackFPar*)pPar)->writeFile(pFile);  

    Error("write","Container %s could not be written",name);
    return -1;
  }
  Error("write","No output file open");
  return -1;
}

