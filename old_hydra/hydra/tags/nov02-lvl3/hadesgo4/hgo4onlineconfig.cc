//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HGo4OnlineConfig
//
// Container class for the mdc hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>

#include "hgo4onlineconfig.h"
#include "hgo4parasciifileio.h"
#include "hgo4parrootfileio.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hspectrometer.h"
#include "hmdcdetector.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hparamlist.h"
#include "hmessagemgr.h"
#include "hevent.h"
#include "hparasciifileio.h"
#include "hparrootfileio.h"
#include "hrun.h"
#include "hdatasource.h"
#include <TList.h>

HGo4OnlineConfig* HGo4OnlineConfig::fGo4OnlineConfig=0;

ClassImp(HGo4OnlineConfig)
ClassImp(HGo4OnlineParam)

HGo4OnlineParam::HGo4OnlineParam(const char* name,const char* title)
    : TNamed(name,title)
{
    clear();
}
HGo4OnlineParam::~HGo4OnlineParam()
{
}
void HGo4OnlineParam::clear()
{
    nLoop         =-99;
    sleep         =-99;
    maxrate       =-99;
    triggerrefresh=-99;
    startrefresh  =-99;
    richrefresh   =-99;
    richODrefresh =-99;
    mdcrefresh    =-99;
    tofrefresh    =-99;
    tofinorefresh =-99;
    showerrefresh =-99;
    trigger     =-99;
    start       =-99;
    startRaw    =-99;
    startCal    =-99;
    startHit    =-99;
    rich        =-99;
    richRaw     =-99;
    richCal     =-99;
    richHit     =-99;
    mdc         =-99;
    mdcLevel    =-99;
    tof         =-99;
    tofRaw      =-99;
    tofCal      =-99;
    tofHit      =-99;
    tofino      =-99;
    tofinoRaw   =-99;
    tofinoCal   =-99;
    showertofino=-99;
    shower      =-99;
    showerRaw   =-99;
    showerCal   =-99;
    showerHit   =-99;
}
void HGo4OnlineParam::copySet(HGo4OnlineParam* p)
{
    nLoop         =p->getNLoop();
    sleep         =p->getSleep();
    maxrate       =p->getMaxRate();
    triggerrefresh=p->getTriggerRefresh();
    startrefresh  =p->getStartRefresh();
    richrefresh   =p->getRichRefresh();
    richODrefresh =p->getRichODRefresh();
    mdcrefresh    =p->getMdcRefresh();
    tofrefresh    =p->getTofRefresh();
    tofinorefresh =p->getTofinoRefresh();
    showerrefresh =p->getShowerRefresh();
    trigger       =p->getTrigger();
    start         =p->getStart();
    startRaw      =p->getStartRaw();
    startCal      =p->getStartCal();
    startHit      =p->getStartHit();
    rich          =p->getRich();
    richRaw       =p->getRichRaw();
    richCal       =p->getRichCal();
    richHit       =p->getRichHit();
    mdc           =p->getMdc();
    mdcLevel      =p->getMdcLevel();
    tof           =p->getTof();
    tofRaw        =p->getTofRaw();
    tofCal        =p->getTofCal();
    tofHit        =p->getTofHit();
    tofino        =p->getTofino();
    tofinoRaw     =p->getTofinoRaw();
    tofinoCal     =p->getTofinoCal();
    showertofino  =p->getShowerTofino();
    shower        =p->getShower();
    showerRaw     =p->getShowerRaw();
    showerCal     =p->getShowerCal();
    showerHit     =p->getShowerHit();
}
Bool_t HGo4OnlineParam::isEqual(HGo4OnlineParam* p,Int_t det)
{
    if(det==0)
    {
	if(nLoop         !=p->getNLoop())         return kFALSE;
	if(sleep         !=p->getSleep())         return kFALSE;
	if(maxrate       !=p->getMaxRate())       return kFALSE;
	if(triggerrefresh!=p->getTriggerRefresh())return kFALSE;
	if(startrefresh  !=p->getStartRefresh())  return kFALSE;
	if(richrefresh   !=p->getRichRefresh())   return kFALSE;
	if(richODrefresh !=p->getRichODRefresh()) return kFALSE;
	if(mdcrefresh    !=p->getMdcRefresh())    return kFALSE;
	if(tofrefresh    !=p->getTofRefresh())    return kFALSE;
	if(tofinorefresh !=p->getTofinoRefresh()) return kFALSE;
	if(showerrefresh !=p->getShowerRefresh()) return kFALSE;
    }
    if(trigger       !=p->getTrigger())     return kFALSE;
    if(start         !=p->getStart())       return kFALSE;
    if(startRaw      !=p->getStartRaw())    return kFALSE;
    if(startCal      !=p->getStartCal())    return kFALSE;
    if(startHit      !=p->getStartHit())    return kFALSE;
    if(rich          !=p->getRich())        return kFALSE;
    if(richRaw       !=p->getRichRaw())     return kFALSE;
    if(richCal       !=p->getRichCal())     return kFALSE;
    if(richHit       !=p->getRichHit())     return kFALSE;
    if(mdc           !=p->getMdc())         return kFALSE;
    if(mdcLevel      !=p->getMdcLevel())    return kFALSE;
    if(tof           !=p->getTof())         return kFALSE;
    if(tofRaw        !=p->getTofRaw())      return kFALSE;
    if(tofCal        !=p->getTofCal())      return kFALSE;
    if(tofHit        !=p->getTofHit())      return kFALSE;
    if(tofino        !=p->getTofino())      return kFALSE;
    if(tofinoRaw     !=p->getTofinoRaw())   return kFALSE;
    if(tofinoCal     !=p->getTofinoCal())   return kFALSE;
    if(showertofino  !=p->getShowerTofino())return kFALSE;
    if(shower        !=p->getShower())      return kFALSE;
    if(showerRaw     !=p->getShowerRaw())   return kFALSE;
    if(showerCal     !=p->getShowerCal())   return kFALSE;
    if(showerHit     !=p->getShowerHit())   return kFALSE;

    return kTRUE;
}
void HGo4OnlineParam::putParams(HParamList* l)
{
    // Puts all params of HGo4OnlineParam to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    l->add("nLoop"          ,nLoop);
    l->add("sleep"          ,sleep);
    l->add("maxrate"        ,maxrate);
    l->add("triggerrefresh" ,triggerrefresh);
    l->add("startrefresh"   ,startrefresh);
    l->add("richrefresh"    ,richrefresh);
    l->add("richODrefresh"  ,richODrefresh);
    l->add("mdcrefresh"     ,mdcrefresh);
    l->add("tofrefresh"     ,tofrefresh);
    l->add("tofinorefresh"  ,tofinorefresh);
    l->add("showerrefresh"  ,showerrefresh);
    l->add("trigger"     ,trigger);
    l->add("start"       ,start);
    l->add("startRaw"    ,startRaw);
    l->add("startCal"    ,startCal);
    l->add("startHit"    ,startHit);
    l->add("rich"        ,rich);
    l->add("richRaw"     ,richRaw);
    l->add("richCal"     ,richCal);
    l->add("richHit"     ,richHit);
    l->add("mdc"         ,mdc);
    l->add("mdcLevel"    ,mdcLevel);
    l->add("tof"         ,tof);
    l->add("tofRaw"      ,tofRaw);
    l->add("tofCal"      ,tofCal);
    l->add("tofHit"      ,tofHit);
    l->add("tofino"      ,tofino);
    l->add("tofinoRaw"   ,tofinoRaw);
    l->add("tofinoCal"   ,tofinoCal);
    l->add("showertofino",showertofino);
    l->add("shower"      ,shower);
    l->add("showerRaw"   ,showerRaw);
    l->add("showerCal"   ,showerCal);
    l->add("showerHit"   ,showerHit);
}
Bool_t HGo4OnlineParam::getParams(HParamList* l)
{
    if(!l) return kFALSE;

    if(!(l->fill("nLoop"          ,&nLoop          ))) return kFALSE;
    if(!(l->fill("sleep"          ,&sleep          ))) return kFALSE;
    if(!(l->fill("maxrate"        ,&maxrate        ))) return kFALSE;
    if(!(l->fill("triggerrefresh" ,&triggerrefresh ))) return kFALSE;
    if(!(l->fill("startrefresh"   ,&startrefresh   ))) return kFALSE;
    if(!(l->fill("richrefresh"    ,&richrefresh    ))) return kFALSE;
    if(!(l->fill("richODrefresh"  ,&richODrefresh  ))) return kFALSE;
    if(!(l->fill("mdcrefresh"     ,&mdcrefresh     ))) return kFALSE;
    if(!(l->fill("tofrefresh"     ,&tofrefresh     ))) return kFALSE;
    if(!(l->fill("tofinorefresh"  ,&tofinorefresh  ))) return kFALSE;
    if(!(l->fill("showerrefresh"  ,&showerrefresh  ))) return kFALSE;
    if(!(l->fill("trigger"   ,&trigger   ))) return kFALSE;
    if(!(l->fill("start"     ,&start     ))) return kFALSE;
    if(!(l->fill("startRaw"  ,&startRaw  ))) return kFALSE;
    if(!(l->fill("startCal"  ,&startCal  ))) return kFALSE;
    if(!(l->fill("startHit"  ,&startHit  ))) return kFALSE;
    if(!(l->fill("rich"      ,&rich      ))) return kFALSE;
    if(!(l->fill("richRaw"   ,&richRaw   ))) return kFALSE;
    if(!(l->fill("richCal"   ,&richCal   ))) return kFALSE;
    if(!(l->fill("richHit"   ,&richHit   ))) return kFALSE;
    if(!(l->fill("mdc"       ,&mdc       ))) return kFALSE;
    if(!(l->fill("mdcLevel"  ,&mdcLevel  ))) return kFALSE;
    if(!(l->fill("tof"       ,&tof       ))) return kFALSE;
    if(!(l->fill("tofRaw"    ,&tofRaw    ))) return kFALSE;
    if(!(l->fill("tofCal"    ,&tofCal    ))) return kFALSE;
    if(!(l->fill("tofHit"    ,&tofHit    ))) return kFALSE;
    if(!(l->fill("tofino"    ,&tofino    ))) return kFALSE;
    if(!(l->fill("tofinoRaw" ,&tofinoRaw ))) return kFALSE;
    if(!(l->fill("tofinoCal" ,&tofinoCal ))) return kFALSE;
    if(!(l->fill("showertofino" ,&showertofino ))) return kFALSE;
    if(!(l->fill("shower"    ,&shower    ))) return kFALSE;
    if(!(l->fill("showerRaw" ,&showerRaw ))) return kFALSE;
    if(!(l->fill("showerCal" ,&showerCal ))) return kFALSE;
    if(!(l->fill("showerHit" ,&showerHit ))) return kFALSE;

    return kTRUE;
}
void HGo4OnlineParam::printParam(void)
{
    // prints the parameters of HGo4OnlineConfig to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
    printf("nLoop       : %i Events per hades loop\n" ,nLoop   );
    printf("sleep       : %i miliseconds sleep\n" ,nLoop   );
    printf("maxrate     : %i max. events/s sleep\n" ,nLoop   );
    printf("refresh     : trigger %i, start %i, rich %i, richOD %i, mdc %i, tof %i, tofino %i, shower %i \n",
	   triggerrefresh,startrefresh,
	   richrefresh,richODrefresh,
	   mdcrefresh,tofrefresh,
	   tofinorefresh,showerrefresh);
    printf("trigger     : det %i\n" ,trigger   );
    printf("start       : det %i, Raw %i, Cal %i, Hit %i\n",start,startRaw,startCal,startHit);
    printf("rich        : det %i, Raw %i, Cal %i, Hit %i\n",rich,richRaw,richCal,richHit);
    printf("mdc         : det %i, Analysis Level %i  (0=raw, 1=cal1, 2=cal2, 3=hit, 4=fit)\n",mdc,mdcLevel);
    printf("tof         : det %i, Raw %i, Cal %i, Hit %i\n",tof,tofRaw,tofCal,tofHit);
    printf("tofino      : det %i, Raw %i, Cal %i, ShowerTofino %i\n",tofino,tofinoRaw,tofinoCal,showertofino);
    printf("shower      : det %i, Raw %i, Cal %i, Hit %i\n",shower,showerRaw,showerCal,showerHit);
}
void HGo4OnlineParam::printStatus(void)
{
    // prints the parameters of HGo4OnlineConfig to the screen.
    printParam();
}

HGo4OnlineConfig::HGo4OnlineConfig(const char* name,const char* title,
				   const char* context)
    : HParCond(name,title,context)
{
    strcpy(detName,"Mdc");
    if (gHades) {
	fMdc =(HMdcDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Mdc"));
    } else {
	fMdc = 0;
    }

    param= new HGo4OnlineParam(name,title);
    clear();

    fGo4OnlineConfig=this;
}
void HGo4OnlineConfig::setGo4ParIo(HRuntimeDb *rtdb) {
  //Adds HGo4OnlineConfigs's Parameter Io to the runtime database.
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
Bool_t HGo4OnlineConfig::initStandAlone()
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
	Error("HGo4OnlineConfig:initStandAlone()","NO RUN SET!");
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
HGo4OnlineConfig::~HGo4OnlineConfig()
{
  // destructor
  cout<<"~HGo4OnlineConfig()...Help me I am Killed!"<<endl;
  //fGo4OnlineConfig=0;
}
HGo4OnlineConfig* HGo4OnlineConfig::getExObject()
{
    return fGo4OnlineConfig;
}
HGo4OnlineConfig* HGo4OnlineConfig::getObject()
{
    if(!fGo4OnlineConfig) fGo4OnlineConfig=new HGo4OnlineConfig();
    return fGo4OnlineConfig;
}
void HGo4OnlineConfig::clear()
{
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
    param->clear();
}
void HGo4OnlineConfig::printParam(void)
{
    // prints the parameters of HGo4OnlineConfig to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
    param->printParam();
}
void HGo4OnlineConfig::printStatus(void)
{
    // prints the parameters of HGo4OnlineConfig to the screen.

    cout<<"####################Status Go4OnlineConfig ###########"<<endl;
    param->printStatus();
    cout<<"######################################################"<<endl;
}
Bool_t HGo4OnlineConfig::init(HParIo* inp,Int_t* set)
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
	Error("HGo4OnlineConfig()","ZERO POINTER FOR RTDB RECEIVED!");
	exit(1);
    }
    HDetParIo* input=inp->getDetParIo("HGo4ParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HGo4OnlineConfig::write(HParIo* output)
{
  // writes the container to an output

    HDetParIo* out=output->getDetParIo("HGo4ParIo");
    if (out) return out->write(this);
    return -1;
}
void HGo4OnlineConfig::putParams(HParamList* l)
{
    // Puts all params of HGo4OnlineConfig to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    param->putParams(l);
}
Bool_t HGo4OnlineConfig::getParams(HParamList* l)
{
    if(!l) return kFALSE;
    if(isStatic()==0)
    {
        if(!param->getParams(l))return kFALSE;
	setStatic();
    }
    return kTRUE;
}
