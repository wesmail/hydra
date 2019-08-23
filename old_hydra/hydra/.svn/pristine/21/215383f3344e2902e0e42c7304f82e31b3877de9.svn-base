#include "hkickplane.h"
#include "hgeomvector.h"
#include "hdetpario.h"
#include "hpario.h"
#include <fstream.h>

//*-- Author : M. Sanchez (14/02/2000)

///////////////////////////////////////////////
// HKickPlane
//
//  Descriptor of the magnetic kick plane of HADES.
//  Contains a parametrization of the kick plane itself as well as
// parametrization of the momentum kicks in X and Y directions as a function of
// the position in the kick plane.
///////////////////////////////////////////////

HKickPlane::HKickPlane(void) {
  //Default constructor
  
  fName="KickPlane";
  fTitle="Parameter container for the kick plane";
  strcpy(detName,"Mdc");
  HSurfPlane *surf =new HSurfPlane;
  /*    surf->setParams(0.0,-0.7933,1392.67);*/
// HSurfXCilinder *surf=new HSurfXCilinder;
 
 fSurface=surf;
}

HKickPlane::~HKickPlane(void) {
  if (fSurface) delete fSurface;
}

void HKickPlane::initParams(void) {
  //Initializes the kick plane's parameters
//   HSurfXCilinder *surf=new HSurfXCilinder;
//   surf->setParams(-2.51805e-4, -0.885812, 1442.78);
  //  HSurfPlane *surf =new HSurfPlane;
  //surf->setParams(0.0,-0.7933,1392.67);
  //fSurface=surf;

  ifstream inp("grid.txt");
  fPtGrid.read(inp);
  inp.close();
  inp.open("gridPphi.txt");
  fPtPhiGrid.read(inp);
  inp.close();
  inp.open("gridNeg.txt");
  fPtGridNeg.read(inp);
  inp.close();
  inp.open("gridPphiNeg.txt");
  fPtPhiGridNeg.read(inp);
  inp.close();
}

Double_t HKickPlane::getPt(const HGeomVector &pos,Double_t &err,
			   const Int_t polarity) {
  Double_t phi=TMath::ATan2(pos.getX(),pos.getY()) * 180 / TMath::Pi();
  Double_t the=TMath::ACos(pos.getZ()/pos.length()) * 180 / TMath::Pi();
  Double_t v;
  v = (polarity==1) ? fPtGrid.getValue(the,phi) : fPtGridNeg.getValue(the,phi);
  err=0.01; //Relative error
  return v;   
}

Double_t HKickPlane::getPtPhi(const HGeomVector &pos,Double_t &err,const Int_t polarity) {
  Double_t phi=TMath::ATan2(pos.getX(),pos.getY()) * 180 / TMath::Pi();
  Double_t the=TMath::ACos(pos.getZ()/pos.length()) * 180 / TMath::Pi();
  Double_t v;
  v = (polarity==1) ? fPtPhiGrid.getValue(the,phi) 
    : fPtPhiGridNeg.getValue(the,phi);
  err=0.01; //relative error
  return v;   
}

void HKickPlane::clear(void) {
  //Clears the container
  Warning("clear","Not yet implemented"); //FIXME
}

Bool_t HKickPlane::init(HParIo *inp, Int_t *set) {
  //See HParSet::init
  HDetParIo *input=inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HKickPlane::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (io) return (out->write(this));
  return -1;
}

Int_t HKickPlane::writeFile(fstream *pFile) {
  if (pFile) {
    fstream &oFile=*pFile;
    Text_t buf[255];
    Text_t com[]="###############################################################################\n"
      "# Parameters defining shape and momentum kick in the kick plane\n"
      "# Format:\n"
      "#  Plane parameters: xSlope, ySlope, offset\n"
      "#  Grids: PtGhi,PtGhiNeg,PtPhi,PtPhiNeg\n"
      "###############################################################################\n";
    pFile->write(com,strlen(com));
    sprintf(buf,"[%s]\n",GetName());
    pFile->write(buf,strlen(buf));

    fSurface->printTo(oFile);
    fPtGrid.print(oFile);
    fPtGridNeg.print(oFile);
    fPtPhiGrid.print(oFile);
    fPtPhiGridNeg.print(oFile);

    sprintf(buf,"###############################################################################\n");
    pFile->write(buf,strlen(buf));
    setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}

Bool_t HKickPlane::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
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
	  fSurface->readFrom(iFile);
	  fPtGrid.read(iFile);
	  fPtGridNeg.read(iFile);
	  fPtPhiGrid.read(iFile);
	  fPtPhiGridNeg.read(iFile);
	  all=kTRUE;
        }
      }
    }
    if (all) {
      setInputVersion(1,inputNumber);
      setChanged();
      printf("KickPlane initialized from Ascii file\n");
      return kTRUE;
    }
  }
  return kFALSE;
}


// Double_t HKickPlane::getPt(const HGeomVector &pos) const {
//   //Parametrization of momentum kick in Y direction.
//   Double_t x=pos.getX(), y=pos.getY();
//   return ( fPtPar[0]*y*x*x + fPtPar[1]*x*x + fPtPar[2]*y*y + fPtPar[3]*y +fPtPar[4] );
// }


ClassImp(HKickPlane)

