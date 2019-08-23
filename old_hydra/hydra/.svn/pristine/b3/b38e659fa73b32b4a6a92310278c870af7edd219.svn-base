using namespace std;
#include "hkickmatchpar.h"
#include "hgeomvector.h"
#include "hdetpario.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hkicktask.h"
#include <TClass.h>
#include <fstream> 

//*-- Author : M. Sanchez (14/02/2000)

///////////////////////////////////////////////
// HKickMatchPar
//
//  Descriptor of the magnetic kick plane of HADES.
//  Contains a parametrization of the kick plane itself as well as
// parametrization of the momentum kicks in X and Y directions as a function of
// the position in the kick plane.
///////////////////////////////////////////////

HKickMatchPar *HKickMatchPar::getMeta(void) {
  HKickMatchPar *kick = 0;

  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) {
      HKickTask::setKickParIo(rtdb);
      kick = (HKickMatchPar *)rtdb->getContainer("KickMatchParMeta");
      if (!kick) {
	kick = new HKickMatchPar("KickMatchParMeta");
	rtdb->addContainer(kick);
      }
    }
  }
  return kick;
}

HKickMatchPar::HKickMatchPar(const char *name,
			     const char *title,
			     const char *cont) : HParSet(name,title,cont) {
  //Default constructor
  
  fName=name;
  fTitle="Parameter container for the kick plane";
  strcpy(detName,"Mdc");
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) HKickTask::setKickParIo(rtdb);
  }

  fScaleFactor = 1.0;
}

HKickMatchPar::HKickMatchPar(const HKickMatchPar &pl) {
  readFrom(pl);
}

HKickMatchPar::~HKickMatchPar(void) {
}

Float_t HKickMatchPar::getDeflection(HGeomVector &pos,Float_t p,
				     Float_t &err,const Int_t polarity) {
  Float_t phi=TMath::ATan2(pos.getX(),pos.getY()) * 180 / TMath::Pi();
  Float_t the=TMath::ACos(pos.getZ()/pos.length()) * 180 / TMath::Pi();

  Float_t a,b,c,v,pp,radi;
  if (polarity>0) {
    a = fGridA.getValue(the,phi);
    b = fGridB.getValue(the,phi);
    c = fGridC.getValue(the,phi);
  } else {
    a = fGridAN.getValue(the,phi);
    b = fGridBN.getValue(the,phi);
    c = fGridCN.getValue(the,phi);
  }

  pp = p/fScaleFactor -a;
  radi = pp*pp -b;
  
  if (radi<0) 
    v = 0. ;
  else {
    v = c*(pp-sqrt(radi));
    v = (phi<0.)?v:-v;
  }
  
  err=0.01; //relative error
  return v;
}

void HKickMatchPar::clear(void) {
  //Clears the container
  fGridA.clear();
  fGridB.clear();
  fGridC.clear();
  fGridAN.clear();
  fGridBN.clear();
  fGridCN.clear();
  fScaleFactor = 1.;
}

Bool_t HKickMatchPar::init(HParIo *inp, Int_t *set) {
  //See HParSet::init
  HDetParIo *input=inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HKickMatchPar::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (out) return (out->write(this));
  return -1;
}

Int_t HKickMatchPar::writeFile(fstream *pFile) {
  if (pFile) {
    fstream &oFile=*pFile;
    Text_t buf[255];
    Text_t com[]="###############################################################################\n"
      "# Parameters for matching using the kick plane\n"
      "# Format:\n"
      "#  Plane parameters: xSlope, ySlope, offset\n"
      "#  Grids: PtGhi,PtGhiNeg,PtPhi,PtPhiNeg\n"
      "###############################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));

    oFile << fScaleFactor;
    oFile << endl;
    fGridA.print(oFile);
    fGridB.print(oFile);
    fGridC.print(oFile);
    fGridAN.print(oFile);
    fGridBN.print(oFile);
    fGridCN.print(oFile);

    sprintf(buf,"###############################################################################\n");
    pFile->write(buf,strlen(buf));
    setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}

void HKickMatchPar::readFrom(const HKickMatchPar &pl) {
  fName = pl.GetName();
  fScaleFactor = pl.fScaleFactor;
  fGridA.readFrom(pl.fGridA);
  fGridB.readFrom(pl.fGridB);
  fGridC.readFrom(pl.fGridC);
  fGridAN.readFrom(pl.fGridAN);
  fGridBN.readFrom(pl.fGridBN);
  fGridCN.readFrom(pl.fGridCN);
}

Bool_t HKickMatchPar::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
  if (pFile) {
    fstream &iFile=*pFile;
    const Int_t maxbuf=1024;
    Text_t buf[maxbuf], key[25];
    Bool_t all=kFALSE;
    while (!all && !pFile->eof()) {
      pFile->getline(buf,maxbuf);
      if (buf[0]=='[') {
        sscanf(buf,"%s",key);
        if (strstr(buf,GetName())!=0) { //cogntainer found
	  iFile >> fScaleFactor;
	  fGridA.read(iFile);
	  fGridB.read(iFile);
	  fGridC.read(iFile);
	  fGridAN.read(iFile);
	  fGridBN.read(iFile);
	  fGridCN.read(iFile);
	  all=kTRUE;
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("%s initialized from Ascii file\n",GetName());
      return kTRUE;
    }
  }
  return kFALSE;
}


ClassImp(HKickMatchPar)

