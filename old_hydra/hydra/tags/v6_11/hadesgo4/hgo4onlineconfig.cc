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
    triggerrefresh=-99;
    startrefresh  =-99;
    richrefresh   =-99;
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
    mdcRaw      =-99;
    mdcCal      =-99;
    mdcHit      =-99;
    tof         =-99;
    tofRaw      =-99;
    tofCal      =-99;
    tofHit      =-99;
    tofino      =-99;
    tofinoRaw   =-99;
    tofinoCal   =-99;
    shower      =-99;
    showerRaw   =-99;
    showerCal   =-99;
    showerHit   =-99;
}
void HGo4OnlineParam::putParams(HParamList* l)
{
    // Puts all params of HGo4OnlineParam to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    l->add("nLoop"          ,nLoop);
    l->add("triggerrefresh" ,triggerrefresh);
    l->add("startrefresh"   ,startrefresh);
    l->add("richrefresh"    ,richrefresh);
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
    l->add("mdcRaw"      ,mdcRaw);
    l->add("mdcCal"      ,mdcCal);
    l->add("mdcHit"      ,mdcHit);
    l->add("tof"         ,tof);
    l->add("tofRaw"      ,tofRaw);
    l->add("tofCal"      ,tofCal);
    l->add("tofHit"      ,tofHit);
    l->add("tofino"      ,tofino);
    l->add("tofinoRaw"   ,tofinoRaw);
    l->add("tofinoCal"   ,tofinoCal);
    l->add("shower"      ,shower);
    l->add("showerRaw"   ,showerRaw);
    l->add("showerCal"   ,showerCal);
    l->add("showerHit"   ,showerHit);
}
Bool_t HGo4OnlineParam::getParams(HParamList* l)
{
    if(!l) return kFALSE;

    if(!(l->fill("nLoop"          ,&nLoop          ))) return kFALSE;
    if(!(l->fill("triggerrefresh" ,&triggerrefresh ))) return kFALSE;
    if(!(l->fill("startrefresh"   ,&startrefresh   ))) return kFALSE;
    if(!(l->fill("richrefresh"    ,&richrefresh    ))) return kFALSE;
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
    if(!(l->fill("mdcRaw"    ,&mdcRaw    ))) return kFALSE;
    if(!(l->fill("mdcCal"    ,&mdcCal    ))) return kFALSE;
    if(!(l->fill("mdcHit"    ,&mdcHit    ))) return kFALSE;
    if(!(l->fill("tof"       ,&tof       ))) return kFALSE;
    if(!(l->fill("tofRaw"    ,&tofRaw    ))) return kFALSE;
    if(!(l->fill("tofCal"    ,&tofCal    ))) return kFALSE;
    if(!(l->fill("tofHit"    ,&tofHit    ))) return kFALSE;
    if(!(l->fill("tofino"    ,&tofino    ))) return kFALSE;
    if(!(l->fill("tofinoRaw" ,&tofinoRaw ))) return kFALSE;
    if(!(l->fill("tofinoCal" ,&tofinoCal ))) return kFALSE;
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
    printf("nLoop       : det %i\n" ,nLoop   );
    printf("refresh     : trigger %i, start %i, rich %i, mdc %i, tof %i, tofino %i, shower %i \n"
	   ,triggerrefresh,startrefresh,richrefresh,mdcrefresh,tofrefresh,tofinorefresh,showerrefresh);
    printf("trigger     : det %i\n" ,trigger   );
    printf("start       : det %i, Raw %i, Cal %i, Hit %i\n",start,startRaw,startCal,startHit);
    printf("rich        : det %i, Raw %i, Cal %i, Hit %i\n",rich,richRaw,richCal,richHit);
    printf("mdc         : det %i, Raw %i, Cal %i, Hit %i\n",mdc,mdcRaw,mdcCal,mdcHit);
    printf("tof         : det %i, Raw %i, Cal %i, Hit %i\n",tof,tofRaw,tofCal,tofHit);
    printf("tofino      : det %i, Raw %i, Cal %i\n"        ,tofino,tofinoRaw,tofinoCal);
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
