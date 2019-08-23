//*-- AUTHOR : M. Sanchez (13.10.2000)


/////////////////////////////////////////////////////////////
// HKickParAsciiFileIo
//
// Class for Kick parameter input/output from/into Ascii file
//
/////////////////////////////////////////////////////////////

#include "hkickparasciifileio.h"
#include "hkickplane.h"
#include "hkickplane2.h"
#include "hkicktrackfpar.h"
#include "hkickmatchpar.h"

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

    if (strncmp(name,"KickPlaneMeta",strlen("KickPlaneMeta"))==0)
      return ((HKickPlane*)pPar)->read(pFile,set,inputNumber);
    if (strncmp(name,"KickPlaneMDC3",strlen("KickPlaneMDC3"))==0)
      return ((HKickPlane*)pPar)->read(pFile,set,inputNumber);
    if (strncmp(name,"KickPlane2MDC3",strlen("KickPlane2MDC3"))==0)
      return ((HKickPlane2*)pPar)->read(pFile,set,inputNumber);
    if (strncmp(name,"KickPlane2Meta",strlen("KickPlane2Meta"))==0)
      return ((HKickPlane2*)pPar)->read(pFile,set,inputNumber);
    if (strncmp(name,"KickMatchParMeta",strlen("KickMatchParMeta"))==0)
      return ((HKickMatchPar*)pPar)->read(pFile,set,inputNumber);
    if (strncmp(name,"KickMatchParMDC3",strlen("KickMatchParMDC3"))==0)
      return ((HKickMatchPar*)pPar)->read(pFile,set,inputNumber);
    if (strncmp(name,"KickTrackFPar",strlen("KickTrackFPar"))==0)
      return HDetParAsciiFileIo::readCond((HParCond*)pPar);
    if (strncmp(name,"RtSavGolPar",strlen("RtSavGolPar"))==0)
      return HDetParAsciiFileIo::readCond((HParCond*)pPar);
    if (strncmp(name,"RtMatchingParMDC3",strlen("RtMatchingParMDC3"))==0)
      return HDetParAsciiFileIo::readCond((HParCond*)pPar);
    if (strncmp(name,"RtMatchingParMeta",strlen("RtMatchingParMeta"))==0)
      return HDetParAsciiFileIo::readCond((HParCond*)pPar);

    Error("init","Container %s NOT initialized",name);
    return kFALSE;
  }
  Error("init","No input file open");
  return kFALSE;
}

Int_t HKickParAsciiFileIo::write(HParSet* pPar) {
  // calls the appropriate write function for the container
  Warning("called %s",pPar->GetName());
  if (pFile) {
    const  Text_t* name=pPar->GetName();

    if (strncmp(name,"KickPlaneMeta",strlen("KickPlaneMeta"))==0)
      return ((HKickPlane*)pPar)->writeFile(pFile);
    if (strncmp(name,"KickPlaneMDC3",strlen("KickPlaneMDC3"))==0)
      return ((HKickPlane*)pPar)->writeFile(pFile);
    if (strncmp(name,"KickPlane2MDC3",strlen("KickPlane2MDC3"))==0)
      return ((HKickPlane2*)pPar)->writeFile(pFile);
    if (strncmp(name,"KickPlane2Meta",strlen("KickPlane2Meta"))==0)
      return ((HKickPlane2*)pPar)->writeFile(pFile);
    if (strncmp(name,"KickMatchParMeta",strlen("KickMatchParMeta"))==0)
      return ((HKickMatchPar*)pPar)->writeFile(pFile);
    if (strncmp(name,"KickMatchParMDC3",strlen("KickMatchParMDC3"))==0)
      return ((HKickMatchPar*)pPar)->writeFile(pFile);
    if (strncmp(name,"KickTrackFPar",strlen("KickTrackFPar"))==0)
      return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    if (strncmp(name,"RtMatchingParMDC3",strlen("RtMatchingParMDC3"))==0)
      return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    if (strncmp(name,"RtMatchingParMeta",strlen("RtMatchingParMeta"))==0)
      return HDetParAsciiFileIo::writeCond((HParCond*)pPar);
    

    Error("write","Container %s could not be written",name);
    return -1;
  }
  Error("write","No output file open");
  return -1;
}

