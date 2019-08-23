#include "hkicktrackfpar.h"
#include "hdetpario.h"
#include "hpario.h"

HKickTrackFPar::HKickTrackFPar(const char* name,const char* title,
                               const char* context)
              : HParSet(name,title,context) {
  strcpy(detName,"Mdc");
  clear();
}

HKickTrackFPar::~HKickTrackFPar(void) {
}

void HKickTrackFPar::clear(void) {
  fPMin=0.;
  fPMin2=0.;
  fMaxXPull1=0.;
  fMaxXPull2=0.;
  fXPullStep=0.;
}

Bool_t HKickTrackFPar::init(HParIo *inp, Int_t *set) {
  //See HParSet::init
  HDetParIo *input=inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HKickTrackFPar::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (io) return (out->write(this));
  return -1; 
}

Int_t HKickTrackFPar::writeFile(fstream *pFile) {
    if (pFile) {
    fstream &oFile=*pFile;
    Text_t buf[255];
    Text_t com[]="###############################################################################\n"
      "# Parameters for kick plane track finder\n"
      "# Format:\n"
      "#  pMin1 pMin2 fMaxXPull1 fMaxXPull2 fXPullStep TofResolution fMinTof fMaxTof\n"
      "###############################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));

    printTo(oFile);

    sprintf(buf,"###############################################################################\n");
    pFile->write(buf,strlen(buf));
    setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}

Bool_t HKickTrackFPar::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
   if (pFile) {
    fstream &iFile=*pFile;
    const Int_t maxbuf=255;
    Text_t buf[maxbuf], key[25];
    Bool_t all=kFALSE;
    
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,GetName())!=0) { //container found
	  readFrom(iFile);
	  all=kTRUE;
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("KickTrackFPar initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE; 
}

void HKickTrackFPar::printTo(ostream &out) {
  out << fPMin << " " << fPMin2 << " " << fMaxXPull1;
  out << " " << fMaxXPull2 << " " << fXPullStep << " ";
  out << fTofXResolution << " " << fMinTof << " " << fMaxTof <<endl;
}

void HKickTrackFPar::readFrom(istream &in) {
  in >> fPMin >> fPMin2 >> fMaxXPull1 >> fMaxXPull2 >> fXPullStep;
  in >> fTofXResolution >> fMinTof >> fMaxTof;
}

void HKickTrackFPar::readFrom(const HKickTrackFPar &par) {
  fPMin           = par.fPMin;
  fPMin2          = par.fPMin2; 
  fMaxXPull1      = par.fMaxXPull1; 
  fMaxXPull2      = par.fMaxXPull2 ;
  fXPullStep      = par.fXPullStep;
  fTofXResolution = par.fTofXResolution ;
  fMinTof         = par.fMinTof ;
  fMaxTof         = par.fMaxTof; 
}

ClassImp(HKickTrackFPar)
