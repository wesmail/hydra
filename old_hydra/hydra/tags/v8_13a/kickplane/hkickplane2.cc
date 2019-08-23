using namespace std;
#include "hkickplane2.h"

#include "hgeomvector.h"
#include "hdetpario.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hkicktask.h"
#include <TClass.h>
#include <fstream> 

//*-- Author : M. Sanchez (04/07/2001)

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////
// HKickPlane2
//
//  Descriptor of the magnetic kick plane of HADES.
//  Contains a parametrization of the kick plane itself as well as
// parametrization of the momentum kicks in X and Y directions as a function of
// the position in the kick plane.
///////////////////////////////////////////////

HKickPlane2 *HKickPlane2::getMDC3(void) {
  HKickPlane2 *kick = 0;

  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) {
      HKickTask::setKickParIo(rtdb);
      kick = (HKickPlane2 *)rtdb->getContainer("KickPlane2MDC3");
      if (!kick) {
	kick = new HKickPlane2("KickPlane2MDC3");
	rtdb->addContainer(kick);
      }
    }
  }
  return kick;
}

HKickPlane2 *HKickPlane2::getMeta(void) {
  HKickPlane2 *kick = 0;

  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) {
      HKickTask::setKickParIo(rtdb);
      kick = (HKickPlane2 *)rtdb->getContainer("KickPlane2Meta");
      if (!kick) {
	kick = new HKickPlane2("KickPlane2Meta");
	rtdb->addContainer(kick);
      }
    }
  }
  return kick;
}

HKickPlane2::HKickPlane2(const char *name,
			 const char *title, 
			 const char *context) : HParSet (name,title,context) {
  //Default constructor
  
  fName=name;
  fTitle="Parameter container for the kick plane";
  strcpy(detName,"Mdc");
  if (gHades) {
    HRuntimeDb *rtdb = gHades->getRuntimeDb();
    if (rtdb) HKickTask::setKickParIo(rtdb);
  }
 
  fSurface=0;
  fScaleFactor = 1.0;
}

HKickPlane2::HKickPlane2(const HKickPlane2 &pl) {
  readFrom(pl);
}

HKickPlane2::~HKickPlane2(void) {
  if (fSurface) delete fSurface;
}

Double_t HKickPlane2::getP(const HGeomVector &pos,Double_t stheta,
			   const Int_t polarity) {
  Double_t phi=TMath::ATan2(pos.getX(),pos.getY()) * 180 / TMath::Pi();
  Double_t the=TMath::ACos(pos.getZ()/pos.length()) * 180 / TMath::Pi();

  Double_t a,b,c,p;
  if (polarity == 1) {
    a = fGridA.getValue(the,phi);
    b = fGridB.getValue(the,phi);
    c = fGridC.getValue(the,phi);
  } else {
    a = fGridANeg.getValue(the,phi);
    b = fGridBNeg.getValue(the,phi);
    c = fGridCNeg.getValue(the,phi);
  }
  //FIXME: Apply scale factor

// Si p*stheta = a/p+b
//  p = (ofs + TMath::Sqrt(ofs*ofs + 4 * slo * stheta)) / (2 * stheta);
  p = a/stheta + b*stheta + c;
//   if (p<0.)
//     Warning("getP","Negative momentum a=%f b=%f c=%f. sin=%f",
// 	    a,b,c,stheta);

  return p*fScaleFactor;
}

void HKickPlane2::clear(void) {
  //Clears the container
  if (fSurface) delete fSurface;
  fSurface=0;
  fScaleFactor = 1.0;
  fGridA.clear();
  fGridB.clear();
  fGridC.clear();
  fGridANeg.clear();
  fGridBNeg.clear();
  fGridCNeg.clear();
}

Bool_t HKickPlane2::init(HParIo *inp, Int_t *set) {
  //See HParSet::init
  HDetParIo *input=inp->getDetParIo("HKickParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}

Int_t HKickPlane2::write(HParIo *io) {
  //See HParSet::write
  HDetParIo *out=io->getDetParIo("HKickParIo");
  if (out) return (out->write(this));
  return -1;
}

Int_t HKickPlane2::writeFile(fstream *pFile) {
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
    oFile << endl;
    fGridA.print(oFile);
    fGridB.print(oFile);
    fGridC.print(oFile);
    fGridANeg.print(oFile);
    fGridBNeg.print(oFile);
    fGridCNeg.print(oFile);

    sprintf(buf,"###############################################################################\n");
    pFile->write(buf,strlen(buf));
    setChanged(kFALSE);
    return 1;
  }
  cerr<<"Output is not writable"<<endl;
  return -1;
}

void HKickPlane2::readFrom(const HKickPlane2 &pl) {
  if (fSurface) { delete fSurface; fSurface=0; }
  fSurface = pl.fSurface->clone();

  fName = pl.GetName();
  fScaleFactor = pl.fScaleFactor;
  fGridA.readFrom(pl.fGridA);
  fGridB.readFrom(pl.fGridB);
  fGridC.readFrom(pl.fGridC);
  fGridANeg.readFrom(pl.fGridANeg);
  fGridBNeg.readFrom(pl.fGridBNeg);
  fGridCNeg.readFrom(pl.fGridCNeg);
}

Bool_t HKickPlane2::read(fstream *pFile,Int_t* set,Int_t inputNumber) {
  Text_t className[256];
  if (pFile) {
    fstream &iFile=*pFile;
    const Int_t maxbuf=1024;
    Text_t buf[maxbuf], key[25];
    Bool_t all=kFALSE;

    if (!iFile) {
      Error("read","Input file is not readable");
      return kFALSE;
    }
    
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
	  } else if (strcmp(className, "HSurfPlaneV") == 0) {
	    fSurface = new HSurfPlaneV;
	  } else if (strcmp(className, "HSurfPlaneTriV") == 0) {
	    fSurface = new HSurfPlaneTriV;
	  } else {
	    Error("read","Unknown surface %s",className);
	    return kFALSE;
	  }
	  fSurface->readFrom(iFile);
	  iFile >> fScaleFactor;
	  fGridA.read(iFile);
	  fGridB.read(iFile);
	  fGridC.read(iFile);
	  fGridANeg.read(iFile);
	  fGridBNeg.read(iFile);
	  fGridCNeg.read(iFile);
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

ClassImp(HKickPlane2)
