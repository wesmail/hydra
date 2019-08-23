#pragma implementation
#include "hades.h"
#include "hruntimedb.h"
#include "hpario.h"
#include "hparrootfileio.h"
#include "hdetpario.h"
#include "hparasciifileio.h"
#include "hparhadasciifileio.h"
#include "hrichparset.h"
#include "hrichparhadasciifileio.h"
#include "hrichparasciifileio.h"
#include "hrichparrootfileio.h"
#include "hrichgeometrypar.h"
#include "hevent.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "richdef.h"
#include "hrichcalibrater.h"
#include "hrichhit.h"
#include "hrichpid.h"
#include "hrichgeantreader.h"
//#include "hrichhitfinder.h"
#include "hrichanalysis.h"
#include "hrichdigitizer.h"
#include "hrichtrack.h"
#include "hrichlocalmaxheader.h"
#include "htaskset.h"
#include "TClass.h"
#include <iostream.h>

HRichDetector::HRichDetector(void) : HDetector("Rich","The RICH detector") {

  maxModules = 1;
//   maxColumns = 92;
//   maxRows = 90;
maxColumns = 96;
maxRows = 96;
// maxColumns = 64;
// maxRows = 64;
  
  modules = new TArrayI(getMaxSectors());

//  createTaskSet("RichTaskSet", "Rich list of tasks");
}

HRichDetector::~HRichDetector(void) {
}


HTask *HRichDetector::buildTask(Text_t *name, Option_t *opt="") {
  if (name && strcmp(name, "default")==0)
       name = NULL;

  HTaskSet *pDetTasks = new HTaskSet();

  if (!name) {//default list of tasks
      HRichCalibrater *cal = new HRichCalibrater("RichCal","rich.cal");
      //HRichDigitizer *dig = new HRichDigitizer("RichDigitisation","Rich digitisation");
      HRichAnalysis *hit=new HRichAnalysis("RichAnalysis","Rich hit finder");

      pDetTasks->connect(cal);
      //pDetTasks->connect(dig);
      pDetTasks->connect(hit, "RichCal");
      //pDetTasks->connect(hit, "RichDigitisation");
      pDetTasks->connect((HTask*)NULL, "RichAnalysis");
 
      return pDetTasks;
  }

  return HDetector::buildTask(name, opt);
}


HCategory *HRichDetector::buildLinearCat(Text_t *classname) {
  HLinearCategory *category=NULL;

  category = new HLinearCategory(classname);
  return category;
}


HCategory *HRichDetector::buildMatrixCat(Text_t *classname,Float_t fillRate) {
  Int_t maxSector=0;
  Int_t i,sizes[3];
  HMatrixCategory *category=NULL;

  for (i = 0; i < 6; i++) if (getModule(i,0) > 0) maxSector++; 
  sizes[0] = maxSector; 
  sizes[1] = sizes[2] = 96;
// sizes[1] = sizes[2] = 64;

  //printf("**************** Max sectors = %d\n", maxSector);
  category = new HMatrixCategory(classname,3,sizes,fillRate);
  return category;
}


HCategory* HRichDetector::buildSimMatrixCat(Text_t *classname,Float_t fillRate,Int_t size) {
 
 HMatrixCategory *category = NULL;
 Int_t ini[2];
  ini[0] = 6;
  ini[1] = size;
 category = new HMatrixCategory(classname, 2, ini, fillRate);
 return category;
}


int HRichDetector::getSectors() {
  int sect;
  getMaxValues(&sect);

  return sect;
}


void HRichDetector::getMaxValues(int* pMaxSector) {
  // Int_t mod,sec=0;
  Int_t i;
  Int_t maxSector=0;
/*
  for (i=0;i<modules->fN;i++) {
    mod=modules->At(i)-1;
    if (mod>-1) {
      sec=mod;
      maxSector=((sec)>maxSector)?(sec):maxSector;
    }
  }
*/
  //for (i = 0; i < 6; i++) if (getModule(i,0) > 0) maxSector++;
  for (i=0;i<6;i++) if (getModule(i,0)>0) maxSector=i+1;
  
  *pMaxSector = maxSector; 
}

HCategory *HRichDetector::buildCategory(Cat_t cat) {
    switch (cat) {
    case catRichRaw: return buildMatrixCat("HRichRaw",1);
    case catRichCal: return buildMatrixCat("HRichCal",1);
    case catRichCalTemp: return buildMatrixCat("HRichCalMax",1);
    case catRichHit: return buildLinearCat("HRichHit");
    case catRichHitSim: return buildLinearCat("HRichHitSim");
    case catRichPID: return buildLinearCat("HRichPID");
    case catRichTrack: return buildLinearCat("HRichTrack");
    case catRichPhotonHit: return buildLinearCat("HRichPhotonHit");//et
    case catRichPhotonHitHeader: return buildLinearCat("HRichPhotonHitHeader");//et
    case catRichLocalMaxHeader :return buildLinearCat("HRichLocalMaxHeader");
    case catRichHitFit: return buildLinearCat("HRichHitFit");//et
    case catMatchHit: return buildLinearCat("HHitMatch");//et
    case catDiMatchHit: return buildLinearCat("HDiHitMatch");//et
    case catMatchHitHeader: return buildLinearCat("HHitMatchHeader");//et
        //  case catHardRichHit: return buildLinearCat("HRichHitIPU");//et
	//  case catRichGeantRaw: return buildLinearCat("HRichGeantCherHit");
	//  case catRichGeantRaw+1: return buildLinearCat("HRichGeantDirHit");
	//  case catRichGeantWire: return buildLinearCat("HRichGeantWire");
    case catRichLocal: return buildMatrixCat("HRichLocal",1);//
    case catRichPhotClus : return buildLinearCat("HPhotonCluster");
    case catRichHitHdr: return buildLinearCat("HRichHitHeader");
    case catRichGeantRaw: return buildSimMatrixCat("HGeantRichPhoton",1,200);
    case catRichGeantRaw+1: return buildSimMatrixCat("HGeantRichDirect",1,200);
    case catRichGeantRaw+2: return buildSimMatrixCat("HGeantRichDirect",1,50);
   
   default: return NULL;
 }
}


////////////////////////////////////////////////////
// initialization according to Ilse's scheme

void HRichDetector::activateParIo(HParIo* io) {
  // activates the input/output class for the parameters
  // needed by the Rich
  if (strcmp(io->IsA()->GetName(),"HOraIo")==0) {
    io->setDetParIo("HRichParIo");
    return;
  }

  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
    HRichParRootFileIo* p=new HRichParRootFileIo(((HParRootFileIo*)io)
                                                        ->getParRootFile());
    io->setDetParIo(p);
  }

  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
    HRichParAsciiFileIo* p=new HRichParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
    io->setDetParIo(p);
  }

  if (strcmp(io->IsA()->GetName(),"HParHadAsciiFileIo")==0) {
    HRichParHadAsciiFileIo* p=new HRichParHadAsciiFileIo(((HParHadAsciiFileIo*)io)->getHadAscii());
    io->setDetParIo(p);
  }

}

Bool_t HRichDetector::write(HParIo* output) {
  // writes the Rich setup to output
  HDetParIo* out=output->getDetParIo("HRichParIo");
  if (out) return out->write(this);
  return kFALSE;
}
//////////////////////////////////////////////////////////



ClassImp(HRichDetector)




