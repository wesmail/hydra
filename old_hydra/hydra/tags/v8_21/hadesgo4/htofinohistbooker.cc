//*-- AUTHOR : J. Markert

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
// HTofinoHistBooker
//
// Container class for the mdc hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>
#include "htofinodetector.h"
#include "htofinohistbooker.h"
#include "htofinohistfiller.h"
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
#include "TList.h"

// Container/List pointers
HTofinoHistBooker*  HTofinoHistBooker::fTofinoHistBooker=0;
TList*              HTofinoHistBooker::tofHistpool=0;
TList*              HTofinoHistBooker::tofActiveHists=0;
HTofinoHistFiller*  HTofinoHistBooker::toffiller=0;

ClassImp(HTofinoHistBooker)

HTofinoHistBooker::HTofinoHistBooker(const Char_t* name,const Char_t* title,
			       const Char_t* context)
    : HParCond(name,title,context)
{
  strcpy(detName,"Tofino");
  if (gHades) {
      fTofino =(HTofinoDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Tofino"));
  } else {
      fTofino = 0;
  }
  clear();

  fTofinoHistBooker =this;

  tofHistpool    = new TList;
  tofActiveHists = new TList;
  toffiller      = new HTofinoHistFiller("TofinoFiller","TofinoFiller");

}
void HTofinoHistBooker::setGo4ParIo(HRuntimeDb *rtdb) {
  //Adds TofinoHistBookers's Parameter Io to the runtime database.
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
Bool_t HTofinoHistBooker::initStandAlone()
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
	Error("HTofinoHistBooker:initStandAlone()","NO RUN SET!");
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
HTofinoHistBooker::~HTofinoHistBooker()
{
  // destructor
  cout<<"~HTofinoHistBooker()...Help me I am Killed!"<<endl;
  //delete Histpool;
  //delete Histpool;
    delete toffiller;
}
HTofinoHistBooker* HTofinoHistBooker::getExObject()
{
    return fTofinoHistBooker;
}
HTofinoHistBooker* HTofinoHistBooker::getObject()
{
    if(!fTofinoHistBooker) fTofinoHistBooker=new HTofinoHistBooker();
    return fTofinoHistBooker;
}
void HTofinoHistBooker::resetActiveList(Int_t level,Int_t count)
{
    TIterator* myiter=tofActiveHists->MakeIterator();
    HMonHistAddon* h=0;
    TString buffer;
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
        h->reset(level,count);
    }
    delete myiter;
}
void HTofinoHistBooker::clear()
{
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
Bool_t HTofinoHistBooker::check()
{
    // all parameters are checked for non valid values
    return kTRUE;
}
void HTofinoHistBooker::printParam(void)
{
    // prints the parameters of HTofinoHistBooker to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
}
void HTofinoHistBooker::printStatus(void)
{
    // prints the parameters of HTofinoHistBooker to the screen.

    cout<<"###############Status TofinoHistbooker#################"<<endl;
    HMonHistAddon* h;
    TIterator* myiter=tofActiveHists->MakeIterator();
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	cout<<"active: "<<h->getActive()<<" resetable: "<<h->getResetable()<<" "<<h->GetName()<<endl;
    }
    delete myiter;
    cout<<"######################################################"<<endl;
}
Bool_t HTofinoHistBooker::init(HParIo* inp,Int_t* set)
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
	Error("HTofinoHistBooker()","ZERO POINTER FOR RTDB RECEIVED!");
	exit(1);
    }
    HDetParIo* input=inp->getDetParIo("HGo4ParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HTofinoHistBooker::write(HParIo* output)
{
  // writes the container to an output

    HDetParIo* out=output->getDetParIo("HGo4ParIo");
    if (out) return out->write(this);
    return -1;
}
void HTofinoHistBooker::putParams(HParamList* l)
{
    // Puts all params of HTofinoHistBooker to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    toffiller->putTofinoParams(l,tofHistpool);
}
Bool_t HTofinoHistBooker::getParams(HParamList* l)
{
    if(!l) return kFALSE;
    if(isStatic()==0)
    {
	if(!toffiller->getTofinoParams(l,tofHistpool,tofActiveHists)) return kFALSE;
	setStatic();
    }
    return kTRUE;
}
