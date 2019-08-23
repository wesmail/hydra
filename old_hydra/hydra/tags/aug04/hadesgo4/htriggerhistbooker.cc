//*-- AUTHOR : J. Markert

////////////////////////////////////////////////////////////////////////////
// HTriggerHistBooker
//
// Container class for the mdc hists
////////////////////////////////////////////////////////////////////////////
//#include <stdlib.h>
#include "htriggerdetector.h"
#include "htriggerhistbooker.h"
#include "htriggerhistfiller.h"
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
#include <TUnixSystem.h>
#include <TROOT.h>

// Container/List pointers
HTriggerHistBooker*  HTriggerHistBooker::fTriggerHistBooker=0;
TList*               HTriggerHistBooker::triggerHistpool=0;
TList*               HTriggerHistBooker::triggerActiveHists=0;
HTriggerHistFiller*  HTriggerHistBooker::triggerfiller=0;

ClassImp(HTriggerHistBooker)

HTriggerHistBooker::HTriggerHistBooker(const char* name,const char* title,
				       const char* context)
    : HParCond(name,title,context)
{
  strcpy(detName,"Trigger");
  if (gHades) {
      fTrigger =(HTriggerDetector*)(((HSpectrometer*)(gHades->getSetup()))->getDetector("Trigger"));
  } else {
      fTrigger = 0;
  }
  clear();

  fTriggerHistBooker=this;

  triggerHistpool    = new TList;
  triggerActiveHists = new TList;
  triggerfiller      = new HTriggerHistFiller("TriggerFiller","TriggerFiller");

}
void HTriggerHistBooker::setGo4ParIo(HRuntimeDb *rtdb) {
  //Adds TriggerHistBookers's Parameter Io to the runtime database.
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
Bool_t HTriggerHistBooker::initStandAlone()
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
	Error("HTriggerHistBooker:initStandAlone()","NO RUN SET!");
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
HTriggerHistBooker::~HTriggerHistBooker()
{
    // destructor
    cout<<"~HTriggerHistBooker()...Help me I am Killed!"<<endl;
    //delete Histpool;
    //delete Histpool;
    delete triggerfiller;
}
HTriggerHistBooker* HTriggerHistBooker::getExObject()
{
    return fTriggerHistBooker;
}
HTriggerHistBooker* HTriggerHistBooker::getObject()
{
    if(!fTriggerHistBooker) fTriggerHistBooker=new HTriggerHistBooker();
    return fTriggerHistBooker;
}
void HTriggerHistBooker::resetActiveList(Int_t level,Int_t count)
{
    TIterator* myiter=triggerActiveHists->MakeIterator();
    HMonHistAddon* h=0;
    TString buffer;
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	h->reset(level,count);
    }
    delete myiter;
}
void HTriggerHistBooker::clear()
{
    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HTriggerHistBooker::printParam(void)
{
    // prints the parameters of HTriggerHistBooker to the screen.
    // The printParam() of the subobjects are called to print the
    // parameters of the corresponding subobject.
}
void HTriggerHistBooker::printStatus(void)
{
    // prints the parameters of HTriggerHistBooker to the screen.

    cout<<"###############Status TriggerHistbooker###############"<<endl;
    HMonHistAddon* h;
    TIterator* myiter=triggerActiveHists->MakeIterator();
    while ((h=(HMonHistAddon*)myiter->Next())!= 0)
    {
	cout<<"active: "<<h->getActive()<<" resetable: "<<h->getResetable()<<" "<<h->GetName()<<endl;
    }
    delete myiter;
    cout<<"######################################################"<<endl;
}
Bool_t HTriggerHistBooker::init(HParIo* inp,Int_t* set)
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
	Error("HTriggerHistBooker()","ZERO POINTER FOR RTDB RECEIVED!");
	exit(1);
    }
    HDetParIo* input=inp->getDetParIo("HGo4ParIo");
    if (input) return (input->init(this,set));
    return kFALSE;
}
Int_t HTriggerHistBooker::write(HParIo* output)
{
  // writes the container to an output

    HDetParIo* out=output->getDetParIo("HGo4ParIo");
    if (out) return out->write(this);
    return -1;
}
void HTriggerHistBooker::putParams(HParamList* l)
{
    // Puts all params of HTriggerHistBooker to the parameter list of
    // HParamList (which ist used by the io);

    if(!l) return;
    triggerfiller->putTriggerParams(l,triggerHistpool);
}
Bool_t HTriggerHistBooker::getParams(HParamList* l)
{
    if(!l) return kFALSE;
    if(isStatic()==0)
    {
	if(!triggerfiller->getTriggerParams(l,triggerHistpool,triggerActiveHists)) return kFALSE;
	setStatic();
    }
    return kTRUE;
}

// writes hists to file returns 0 on success else 1
Bool_t HTriggerHistBooker::writeHists( const TString &option )
{
  TDirectory *oldDir = gDirectory;

  if( gSystem->AccessPathName( "./rootFileTrigger", (EAccessMode)F_OK ) == 1 ) // checks if ./triggerfile exists. (true if not)
    {
      cout << "---trigger: ./rootFileTrigger does not exist... try to create..." << endl;
      if( gSystem->AccessPathName( ".", (EAccessMode)W_OK ) == 0 ) // checks if current dir (exists...) and is writable (true if it is)
	{
	  gSystem->Exec( "mkdir ./rootFileTrigger" );  
	}
      else { cout << "---trigger: current directory does exist (surprise!) but you have not the permission to write." << endl; return(1); }
    }
  else
    {
      if( gSystem->AccessPathName( "./rootFileTrigger", (EAccessMode)W_OK ) == 1 ) // checks if ./rootFileTrigger is writable (true if not)
	{
	  cout << "---trigger: the directory ./rootFileTrigger exists, but you have not the permission to write..." << endl;
	  return(1);
	}
    }

  TFile *triggerFile = new TFile( "./rootFileTrigger/triggerFile.root", "recreate" );
  if( triggerFile ) triggerFile->cd();

  if( option == "active" )
    {
      TIterator* myiter=this->getListOfActiveHists()->MakeIterator();
      HMonHistAddon* h=0;
      if(myiter)
	{
	  while ( (h=(HMonHistAddon*)myiter->Next()) != 0 )
	    {
	      h->Write();
	    }
	}
      else { cout << "---trigger: could not create iterator -> no hists written to .root file" << endl; }
    }
  
  
  triggerFile->Write();
  gDirectory = oldDir;
  delete triggerFile;

  return(0);
}

