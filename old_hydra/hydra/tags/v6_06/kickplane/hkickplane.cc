#include "hkickplane.h"
#include "hgeomvector.h"
#include "hdetpario.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hkicktask.h"
#include <TClass.h>
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

HKickPlane *HKickPlane::getMDC3(void) {
  HKickPlane *kick = 0;

  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) {
      //      HKickTask::setKickParIo(rtdb);
      kick = (HKickPlane *)rtdb->getContainer("KickPlaneMDC3");
    }
  }
  return kick;
}

HKickPlane *HKickPlane::getMeta(void) {
  HKickPlane *kick = 0;

  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) {
      //      HKickTask::setKickParIo(rtdb);
      kick = (HKickPlane *)rtdb->getContainer("KickPlaneMeta");
    }
  }
  return kick;
}

HKickPlane::HKickPlane(const char* name,const char* title,
                       const char* context)
           : HParSet(name,title,context) {
  //Default constructor
  
  strcpy(detName,"Mdc");
  //HSurfPlane *surf =new HSurfPlane;
  //  HSurfXCilinder *surf=new HSurfXCilinder;
 
  fSurface=0;
  fScaleFactor = 1.0;
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) HKickTask::setKickParIo(rtdb);
  }
}

HKickPlane::HKickPlane(const HKickPlane &pl) {
  readFrom(pl);
}

HKickPlane::~HKickPlane(void) {
  if (fSurface) delete fSurface;
}

void HKickPlane::initParams(void) {
  //Initializes the kick plane's parameters
  //HSurfXCilinder *surf=new HSurfXCilinder;
  //surf->setParams(-2.51805e-4, -0.885812, 1442.78);
  //  HSurfPlane *surf =new HSurfPlane;
  //surf->setParams(0.0,-0.7933,1392.67);
  //  fSurface=surf;

  ifstream inp("Params/grid.txt");
  fPtGrid.read(inp);
  inp.close();
  inp.open("Params/gridPphi.txt");
  fPtPhiGrid.read(inp);
  inp.close();
  inp.open("Params/gridNeg.txt");
  fPtGridNeg.read(inp);
  inp.close();
  inp.open("Params/gridPphiNeg.txt");
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
  return v*fScaleFactor;   
}

Double_t HKickPlane::getPtPhi(const HGeomVector &pos,Double_t &err,const Int_t polarity) {
  Double_t phi=TMath::ATan2(pos.getX(),pos.getY()) * 180 / TMath::Pi();
  Double_t the=TMath::ACos(pos.getZ()/pos.length()) * 180 / TMath::Pi();
  Double_t v;
  v = (polarity==1) ? fPtPhiGrid.getValue(the,phi) 
    : fPtPhiGridNeg.getValue(the,phi);
  err=0.01; //relative error
  return v*fScaleFactor;   
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

    oFile << fSurface->IsA()->GetName() << endl;
    fSurface->printTo(oFile);
    oFile << fScaleFactor;
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

void HKickPlane::readFrom(const HKickPlane &pl) {
  if (fSurface) { delete fSurface; fSurface=0; }
  fSurface = pl.fSurface->clone();

  fName = pl.GetName();
  fScaleFactor = pl.fScaleFactor;
  fPtGrid.readFrom(pl.fPtGrid);
  fPtGridNeg.readFrom(pl.fPtGridNeg);
  fPtPhiGrid.readFrom(pl.fPtPhiGrid);
  fPtPhiGridNeg.readFrom(pl.fPtPhiGridNeg);
}

Bool_t HKickPlane::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
  Text_t className[256];
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
	  iFile >> className;

	  if (fSurface) delete fSurface;
	  if ( strcmp(className,"HSurfPlane") == 0 ) {
	    fSurface = new HSurfPlane;
	  } else if (strcmp(className, "HSurfXCilinder") == 0) {
	    fSurface = new HSurfXCilinder;
	  } else {
	    Error("read","Unknown surface %s",className);
	    return kFALSE;
	  }
	  fSurface->readFrom(iFile);
	  iFile >> fScaleFactor;
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
      printf("%s initialized from Ascii file\n",GetName());
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

