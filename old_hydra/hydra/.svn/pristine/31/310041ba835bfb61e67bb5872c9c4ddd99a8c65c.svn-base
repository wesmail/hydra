//*-- created  : /19/12/2001 Dusan Zovinec

////////////////////////////////////////////////////////////////////////////
// HTofClusterFPar
//
// Clusterfinder parameters for TOF
// (adapted from /u/halo/anal/v5_45/hkicktrackfpar.cc)
//
////////////////////////////////////////////////////////////////////////////

#include "htofclusterfpar.h"
#include "hdetpario.h"
#include "hpario.h"

HTofClusterFPar::HTofClusterFPar(const char* name,const char* title,
                                 const char* context)
                : HParSet(name,title,context) {
  strcpy(detName,"Tof");
  clear();
}

HTofClusterFPar::~HTofClusterFPar(void) {
}

void HTofClusterFPar::clear(void) {
  diffTmax=0.0;
  diffXmax=0.0;
}

Bool_t HTofClusterFPar::init(HParIo *inp, Int_t *set) {
  //See HParSet::init
  HDetParIo *input=inp->getDetParIo("HTofParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HTofClusterFPar::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HTofParIo");
  if (io) return (out->write(this));
  return -1;
}

Int_t HTofClusterFPar::writeFile(fstream *pFile) {
    if (pFile) {
    fstream &oFile=*pFile;
    Text_t buf[255];
    Text_t com[]="###############################################################################\n"
      "# Parameters for cluster finder\n"
      "# Format:\n"
      "# diffTmax diffXmax\n"
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

Bool_t HTofClusterFPar::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
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
      printf("TofClusterFPar initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
}

void HTofClusterFPar::printTo(ostream &out) {
  out << diffTmax << " " << diffXmax <<endl;
}

void HTofClusterFPar::readFrom(istream &in) {
  in >> diffTmax >> diffXmax;
}

void HTofClusterFPar::readFrom(const HTofClusterFPar &par) {
  diffTmax = par.diffTmax;
  diffXmax = par.diffXmax;
}

ClassImp(HTofClusterFPar)

