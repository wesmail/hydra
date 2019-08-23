//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HShowerHistBooker
//
// Container class for the mdc hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>
#include "hshowerdetector.h"
#include "hshowerhistbooker.h"
#include "hshowerhistfiller.h"
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
HShowerHistBooker*  HShowerHistBooker::fShowerHistBooker=0;
TList*              HShowerHistBooker::showerHistpool=0;
TList*              HShowerHistBooker::showerActiveHists=0;
HShowerHistFiller*  HShowerHistBooker::showerfiller=0;

ClassImp(HShowerHistBooker)

HShowerHistBooker::HShowerHistBooker(const char* name,const char* title,
				 const char* context)
    : HParCond(name,title,context)
{
  strcpy(detName,"Shower");
  if (gHades) {
      fShower =(HShowerDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Shower"));
  } else {
      fShower = 0;
  }
  clear();

  fShowerHistBooker =this;

  showerHistpool    = new TList;
  showerActiveHists = new TList;
  showerfiller      = new HShowerHistFiller("ShowerFiller","ShowerFiller");

}
void HShowerHistBooker::setGo4ParIo(HRuntimeDb *rtdb) {
  //Adds ShowerHistBookers's Parameter Io to the runtime database.
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
Bool_t HShowerHistBooker::initStandAlone()
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
	if (!run->getParVersion((char*)GetName()))
	{
	    vers=new HParVersion((char*)GetName());
	    run->addParVersion(vers);
	}
    }
    else
    {
	Error("HShowerHistBooker:initStandAlone()","NO RUN SET!");
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
HShowerHistBooker::~HShowerHistBooker()
{
  // destructor
  cout<<"~HShowerHistBooker()...Help me I am Killed!"<<endl;
  //delete Histpool;
  //delete Histpool;
    delete showerfiller;
}
HShowerHistBooker* HShowerHistBooker::getExObject()
{
    return fShowerHistBooker;
}
HShowerHistBooker* HShowerHistBooker::getObject()
{
    if(!fShowerHistBooker) fShowerHistBooker=new HShowerHistBooker();
    return fShowerHistBooker;
}
void HShowerHistBooker::resetActiveList(Int_t level,Int_t count)
{
    TIterator* myiter=showerActiveHists->MakeIterator();
    HMonHistAddon* h=0;
    TString buffer;
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	h   ->reset(level,count);
    }
    delete myiter;
}
void HShowerHistBooker::clear()
{
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
Bool_t HShowerHistBooker::check()
{
    // all parameters are checked for non valid values
    return kTRUE;
}
void HShowerHistBooker::printParam(void)
{
    // prints the parameters of HShowerHistBooker to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
}
void HShowerHistBooker::printStatus(void)
{
    // prints the parameters of HShowerHistBooker to the screen.

    cout<<"###############Status ShowerHistbooker#################"<<endl;
    HMonHistAddon* h;
    TIterator* myiter=showerActiveHists->MakeIterator();
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	cout<<"active: "<<h->getActive()<<" resetable: "<<h->getResetable()<<" "<<h->GetName()<<endl;
    }
    delete myiter;
    cout<<"######################################################"<<endl;
}
Bool_t HShowerHistBooker::init(HParIo* inp,Int_t* set)
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
	Error("HShowerHistBooker()","ZERO POINTER FOR RTDB RECEIVED!");
	exit(1);
    }
    HDetParIo* input=inp->getDetParIo("HGo4ParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HShowerHistBooker::write(HParIo* output)
{
  // writes the container to an output

    HDetParIo* out=output->getDetParIo("HGo4ParIo");
    if (out) return out->write(this);
    return -1;
}
void HShowerHistBooker::putParams(HParamList* l)
{
    // Puts all params of HShowerHistBooker to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    showerfiller->putShowerParams(l,showerHistpool);
}
Bool_t HShowerHistBooker::getParams(HParamList* l)
{
    if(!l) return kFALSE;
    if(isStatic()==0)
    {
	if(!showerfiller->getShowerParams(l,showerHistpool,showerActiveHists)) return kFALSE;
	setStatic();
    }
    return kTRUE;
}
