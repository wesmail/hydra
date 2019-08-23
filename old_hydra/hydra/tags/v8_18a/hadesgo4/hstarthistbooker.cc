//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HStartHistBooker
//
// Container class for the mdc hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>
#include "hstartdetector.h"
#include "hstarthistbooker.h"
#include "hstarthistfiller.h"
#include "hmonhist.h"
#include "htrendhist.h"
#include "hhistarray.h"
#include "hgo4parasciifileio.h"
#include "hgo4parrootfileio.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include "hmessagemgr.h"
#include "hevent.h"
#include "hcategory.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
#include "hrun.h"
#include "hdatasource.h"
#include <TList.h>

// Container/List pointers
HStartHistBooker*  HStartHistBooker::fStartHistBooker=0;
TList*             HStartHistBooker::startHistpool=0;
TList*             HStartHistBooker::startActiveHists=0;
HStartHistFiller*  HStartHistBooker::startfiller=0;

ClassImp(HStartHistBooker)

HStartHistBooker::HStartHistBooker(const Char_t* name,const Char_t* title,
                       const Char_t* context)
    : HParCond(name,title,context)
{
  strcpy(detName,"Start");
  if (gHades) {
      fStart =(HStartDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Start"));
  } else {
      fStart = 0;
  }
  clear();

  fStartHistBooker=this;

  startHistpool    = new TList;
  startActiveHists = new TList;
  startfiller      = new HStartHistFiller("StartFiller","StartFiller");

}
void HStartHistBooker::setGo4ParIo(HRuntimeDb *rtdb) {
  //Adds StartHistBookers's Parameter Io to the runtime database.
  HParIo *io=0;

  io=rtdb->getFirstInput();
  if (io != 0) {
      if (io->getDetParIo("HGo4ParIo") == 0) {
	  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	      HGo4ParAsciiFileIo* p=
		  new HGo4ParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	      io->setDetParIo(p);
	      io->setInputNumber(1);
	  }
	  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	      HGo4ParRootFileIo* p=
		  new HGo4ParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	      io->setDetParIo(p);
	      io->setInputNumber(1);
	  }
	  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
	      io->setDetParIo("HGo4ParIo");
	      io->setInputNumber(1);
	  }
      }
  }

  io=rtdb->getSecondInput();
  if (io != 0) {
      if (io->getDetParIo("HGo4ParIo") == 0) {
	  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	      HGo4ParAsciiFileIo* p=
		  new HGo4ParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	      io->setDetParIo(p);
	      io->setInputNumber(2);
	  }
	  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	      HGo4ParRootFileIo* p=
		  new HGo4ParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	      io->setDetParIo(p);
	      io->setInputNumber(2);
	  }
	  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) {
	      io->setDetParIo("HGo4ParIo");
	      io->setInputNumber(2);
	  }
      }
  }
  
  io=rtdb->getOutput();
  if (io != 0) {
      if (io->getDetParIo("HGo4ParIo") == 0) {
	  if (strcmp(io->IsA()->GetName(),"HParAsciiFileIo")==0) {
	      HGo4ParAsciiFileIo* p=
		  new HGo4ParAsciiFileIo(((HParAsciiFileIo*)io)->getFile());
	      io->setDetParIo(p);
	  }
	  if (strcmp(io->IsA()->GetName(),"HParRootFileIo")==0) {
	      HGo4ParRootFileIo* p=
		  new HGo4ParRootFileIo(((HParRootFileIo*)io)->getParRootFile());
	      io->setDetParIo(p);
	  }
	  if (strcmp(io->IsA()->GetName(),"HParOraIo")==0) io->setDetParIo("HGo4ParIo");
      }
  }
}
Bool_t HStartHistBooker::initStandAlone()
{
    // intitializes the container from an input
    HRuntimeDb* rtdb=0;
    rtdb=gHades->getRuntimeDb();

    HDataSource* source=gHades->getDataSource();
    Int_t refId=source->getCurrentRefId();
    Int_t runId=source->getCurrentRunId();
    HRun* run=rtdb->getRun(runId);
    if(!run)run=rtdb->addRun(runId,refId);
    Bool_t allFound=kFALSE;
    HParVersion* vers;
    if(run)
    {
	if (!run->getParVersion((Char_t*)GetName()))
	{
	    vers=new HParVersion((Char_t*)GetName());
	    run->addParVersion(vers);
	}
    }
    else
    {
	Error("HStartHistBooker:initStandAlone()","NO RUN SET!");
        return kFALSE;
    }
    if (versions[1]!=-1 && versions[2]!=-1) resetInputVersions();

    HParIo* io=rtdb->getFirstInput();
    if (io) allFound=init(io,0);
    if (!allFound)
    {
	io=rtdb->getSecondInput();
	if (io)
	{
	    allFound=init(io,0);
	}

    }else setInputVersion(-1,2);
    if (!allFound)
    {
	cerr<<"********  "<<GetName()<<"  not initialized  ********"<<endl;
	return kFALSE;
    }
    return kTRUE;
}
HStartHistBooker::~HStartHistBooker()
{
  // destructor
  cout<<"~HStartHistBooker()...Help me I am Killed!"<<endl;
  //delete Histpool;
  //delete Histpool;
    delete startfiller;
}
HStartHistBooker* HStartHistBooker::getExObject()
{
    return fStartHistBooker;
}
HStartHistBooker* HStartHistBooker::getObject()
{
    if(!fStartHistBooker) fStartHistBooker=new HStartHistBooker();
    return fStartHistBooker;
}
void HStartHistBooker::resetActiveList(Int_t level,Int_t count)
{
    TIterator* myiter=startActiveHists->MakeIterator();
    HMonHistAddon* h=0;
    TString buffer;
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	h->reset(level,count);
    }
    delete myiter;
}
void HStartHistBooker::clear()
{
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HStartHistBooker::printParam(void)
{
    // prints the parameters of HStartHistBooker to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
}
void HStartHistBooker::printStatus(void)
{
    // prints the parameters of HStartHistBooker to the screen.

    cout<<"###############Status StartHistbooker#################"<<endl;
    HMonHistAddon* h;
    TIterator* myiter=startActiveHists->MakeIterator();
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	cout<<"active: "<<h->getActive()<<" resetable: "<<h->getResetable()<<" "<<h->GetName()<<endl;
    }
    delete myiter;
    cout<<"######################################################"<<endl;
}
Bool_t HStartHistBooker::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input

    HRuntimeDb* rtdb=0;
    rtdb=gHades->getRuntimeDb();
    if(rtdb)
    {
	setGo4ParIo(rtdb);
    }
    else
    {
	Error("HStartHistBooker()","ZERO POINTER FOR RTDB RECEIVED!");
	exit(1);
    }
    HDetParIo* input=inp->getDetParIo("HGo4ParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HStartHistBooker::write(HParIo* output)
{
  // writes the container to an output

    HDetParIo* out=output->getDetParIo("HGo4ParIo");
    if (out) return out->write(this);
    return -1;
}
void HStartHistBooker::putParams(HParamList* l)
{
    // Puts all params of HStartHistBooker to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    startfiller->putStartParams(l,startHistpool);
}
Bool_t HStartHistBooker::getParams(HParamList* l)
{
    if(!l) return kFALSE;
    if(isStatic()==0)
    {
	if(!startfiller->getStartParams(l,startHistpool,startActiveHists)) return kFALSE;
	setStatic();
    }
    return kTRUE;
}
