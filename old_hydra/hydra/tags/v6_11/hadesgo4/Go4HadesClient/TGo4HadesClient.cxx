/* Generated by Together */

#include "TGo4HadesClient.h"

#include "Go4TaskHandler/TGo4ClientStatus.h"


const Text_t TGo4HadesClient::fgcWATCHTHREAD[]="WATCH-";
const Text_t TGo4HadesClient::fgcMAINTHREAD[]="MAIN-";



	

TGo4HadesClient::TGo4HadesClient(Text_t* name, 
                                       TGo4Hades* analysis, 
                                       Text_t* host, 
                                       UInt_t negport)
: TGo4ClientTask(name, host, negport , kTRUE, kFALSE)

{
   TRACE((15,"TGo4HadesClient::TGo4HadesClient(Text_t*,...)",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   if(analysis==0)
      {
         tr->Log("G-OOOO-> !!! AnalysisClient ''%s'': no external analysis specified !!<-OOOO-G",GetName());
         fxAnalysis=analysis;
      }
   else
      {
         fxAnalysis=analysis;
      }      
   
   
   tr->Log("G-OOOO-> AnalysisClient ''%s'' started <-OOOO-G",GetName());
   fcMainName= new Text_t[TGo4ThreadManager::fguTEXTLENGTH];
   fcWatchName= new Text_t[TGo4ThreadManager::fguTEXTLENGTH];      
   Text_t namebuffer[TGo4ThreadManager::fguTEXTLENGTH]; 
   
   
   fxStatusBuffer= new TBuffer(TBuffer::kWrite);
   
 
   
   sprintf(namebuffer,"MainRunnable of %s",GetName());
   TGo4HadesMainRunnable* mainrun= new TGo4HadesMainRunnable(namebuffer, this);    
   sprintf(namebuffer,"WatchRunnable of %s",GetName());
   TGo4HadesWatchRunnable* watchrun= new TGo4HadesWatchRunnable(namebuffer, this);    
   
      // adding runnables to thread handler who takes over the responsibility...:

   sprintf(fcMainName,"%s%s",fgcMAINTHREAD,GetName());
   fxWorkHandler->NewThread(fcMainName,mainrun);

   sprintf(fcWatchName,"%s%s",fgcWATCHTHREAD,GetName());
   fxWorkHandler->NewThread(fcWatchName,watchrun);

   TGo4CommandInvoker::Instance(); // make sure we have an invoker instance!
   TGo4CommandInvoker::Register("AnalysisClient",this); // register as command receiver at the global invoker
    


   Launch();  

}
TGo4HadesClient::~TGo4HadesClient()
{
   TRACE((15,"TGo4HadesClient::~TGo4HadesClient()",__LINE__, __FILE__));
   GetTaskHandler()->DisConnect(); // disconnect before we autosave etc. 
   GetTaskHandler()->SetAborting(); // for superclass dtor: set fast abort mode
   fxAnalysis->LockAutoSave();   // wait until mainthread finishes the
      {                              // final autosaving before we cancel
      delete [] fcMainName;
      delete [] fcWatchName;
      fxWorkHandler->CancelAll(); // make sure threads wont work on application when its deleted
      }
   fxAnalysis->UnLockAutoSave();  
   delete fxAnalysis;

}

Int_t TGo4HadesClient::Initialization() 
{ 
   TRACE((12,"TGo4HadesClient::Initialization()",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();  
   Int_t rev=-1;
   if(fbInitDone)
      // already initialized, return ok value
      {
      TRACE((11,"TGo4HadesClient::Initialization()--already init done",__LINE__, __FILE__));
         rev= 0;
      } //// if(fbInitDone)
   else
      {
      TRACE((11,"TGo4HadesClient::Initialization()--doing init",__LINE__, __FILE__));
         if(fxAnalysis->InitEventClasses())
            {
               rev=TGo4ClientTask::Initialization(); // connect to server and launch threads
         
            }
         else
            {
               tr->Log("G-OOOO-> !!! Analysis Client Initiatlization --  ERROR, Terminating... !!! <-OOOO-G");
               gApplication->Terminate();
            }                     
              
      }// else if(fbInitDone)
      
    return rev;
}



void TGo4HadesClient::UpdateStatus(TGo4ClientStatus* state)
{
   TRACE((12,"TGo4HadesClient::UpdateStatus(TGo4ClientStatus*)",__LINE__, __FILE__));
   TGo4ClientTask::UpdateStatus(state); // fill superclass attributes
// subclass of client status not yet existing:
//   TGo4HadesClientStatus* exstate= (TGo4HadesClientStatus*) state;

//   exstate->SetHistoStatus(GetApplication()->GetHistogram());
//   exstate->SetNames(fcMainName, fcWatchName);

}
TGo4ClientStatus * TGo4HadesClient::CreateStatus()
{
   TRACE((12,"TGo4HadesClient::CreateStatus()",__LINE__, __FILE__));
   TGo4ClientStatus* stat=0;
   stat=TGo4ClientTask::CreateStatus();   
 
// subclass of client status not yet existing:
//   TGo4HadesClientStatus* stat= new TGo4HadesClientStatus((Text_t*) GetName());
//   UpdateStatus(stat); // set the internals

   return stat;

}
void TGo4HadesClient::RestoreStatus(TGo4ClientStatus * stat)
{
   TRACE((12,"TGo4HadesClientTask::RestoreStatus(TGo4ClientStatus*)",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   tr->Log("G-OOOO-> Analysis ClientTask: restoring internal state <-OOOO-G");
   TGo4ClientTask::RestoreStatus(stat); 
   // we will not restore anything in this example...

}

void TGo4HadesClient::Start() 
{
   TRACE((12,"TGo4HadesClient::Start()",__LINE__, __FILE__));
    //cout << "Start of example client!"<<endl;
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
//   tr->Log("G-OOOO-> AnalysisClient ''%s'' executing Start(): start main thread<-OOOO-G",GetName());
   fxWorkHandler->Start(fcMainName); // this is useful anyway...
   tr->Log("G-OOOO-> AnalysisClient ''%s'' executing Start(): start processing analysis <-OOOO-G",GetName());
   TGo4ClientTask::Start();
}

void TGo4HadesClient::SendObject(const Text_t* name)
{
   TRACE((12,"TGo4HadesClient::SendObject(Text_t* name)",__LINE__, __FILE__));
   TNamed* ob=fxAnalysis->GetObject( const_cast<Text_t*> (name) );  
   SendObject(ob);
}

void TGo4HadesClient::SendAnalysisStatus()
{
   TRACE((12,"TGo4HadesClient::SendAnalysisStatus()",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   tr->Log("G-OOOO-> AnalysisClient - sending analysis status <-OOOO-G");
////////  under const
   fxStatusQ->AddBuffer(fxStatusBuffer,kTRUE);
//  fxStatusQ->AddBuffer(fxStatusBuffer);
// cout << "client added to buffer with clone" << endl;
///////
}

void TGo4HadesClient::SendStatusMessage(Text_t * text)
{
   TRACE((12,"TGo4HadesClient::SendStatusMessage(Text_t* name)",__LINE__, __FILE__));
   TGo4Status* message= new TGo4Status(text);   
   SendStatus(message);  
}


void TGo4HadesClient::SendObject(TNamed* obj)
{
   TRACE((12,"TGo4HadesClient::SendObject(TNamed* obj)",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   
   if(obj)
      {
         // object exists, put it into data queue
         if(fxDataQ)
            {
               tr->Log("G-OOOO-> AnalysisClient - sending object: %s <-OOOO-G",obj->GetName());
               fxDataQ->AddBufferFromObject(obj);
            } 
         else
            {
               tr->Log("G-OOOO-> !!! AnalysisClient - ERROR sending object - no data queue !!! <-OOOO-G");
            }
      }
   else
      {
         // object not found, send error message through status
         Text_t buf [128];
         sprintf(buf,"G-OOOO-> !!! AnalysisClient - ERROR sending object - no such object !!! <-OOOO-G");
         tr->Log(buf);
         SendStatusMessage(buf);
      
      }      
      
   
}
void TGo4HadesClient::SendStatus(TGo4Status* stat)
{
   TRACE((12,"TGo4HadesClient::SendStatus(TGo4Status* stat)",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();    
   if(stat)
      {
         // object exists, put it into status queue
         if(fxStatusQ)
            {
               tr->Log("G-OOOO-> AnalysisClient - sending status %s <-OOOO-G", stat->ClassName());
               fxStatusQ->AddBufferFromObject(stat);
            } 
         else
            {
               tr->Log("G-OOOO-> !!! AnalysisClient - ERROR sending status: no status queue !!! <-OOOO-G");
            }
      }
   else
      {
         // tr->Log("G-OOOO-> !!! AnalysisClient - ERROR sending status: no such object!!! <-OOOO-G");

      }      
      
   
}
void TGo4HadesClient::SendNamesList()
{
   TRACE((12,"TGo4HadesClient::SendNamesList()",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   
////////  under const
   //TGo4HadesObjectNames* state= fxAnalysis->GetNamesList();
         
//   if(state)
//      {
//         tr->Log("G-OOOO-> AnalysisClient - sending names list <-OOOO-G");
//         //SendStatus(state);
//         SendObject(state);
//      }
//   else
//      {
//         Text_t buf [128];
//         sprintf(buf,"G-OOOO-> !!! Analysis Client: Send Names List - ERROR:  no nameslist !!! <-OOOO-G");
//         tr->Log(buf);
//         SendStatusMessage(buf);
//      }

}

void TGo4HadesClient::KillMain()
{
   TRACE((12,"TGo4HadesClient::KillMain()",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   tr->Log("G-OOOO-> AnalysisClient ''%s'' killing main thread<-OOOO-G",GetName());
   fxWorkHandler->Cancel(fcMainName);

}

void TGo4HadesClient::RestartMain()
{
   TRACE((12,"TGo4HadesClient::RestartMain()",__LINE__, __FILE__));
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
   tr->Log("G-OOOO-> AnalysisClient ''%s'' killing main thread<-OOOO-G",GetName());
//   fxCommandQ->AddBuffer(new TBuffer(TBuffer::kWrite, 1));
//   cout << "killing main: added dummy buffer to commandq"<< endl;
   fxWorkHandler->ReCreate(fcMainName);

//   fxCommandQ->AddBuffer(new TBuffer(TBuffer::kWrite, 1));
//   cout << "killing main: added dummy buffer to commandq"<< endl;
   // fxWorkHandler->Cancel(fcMainName);
   fxWorkHandler->Start(fcMainName);
}

void TGo4HadesClient::Stop()
{
   TRACE((12,"TGo4HadesClient::Stop()",__LINE__, __FILE__));
    //cout << "Stop of example client!"<<endl;
   TGo4TraceSingleton* tr=TGo4TraceSingleton::Instance();
//   tr->Log("G-OOOO-> AnalysisClient ''%s'' executing Stop(): stop main thread<-OOOO-G",GetName());
//   fxWorkHandler->Stop(fcMainName); // would stop for thread runnable wait

   tr->Log("G-OOOO-> AnalysisClient ''%s'' executing Stop(): stop processing analysis <-OOOO-G",GetName());
   TGo4ClientTask::Stop(); // will stop for command queue wait   

}

void TGo4HadesClient::UpdateStatusBuffer()
{
  TGo4ClientStatus* state=CreateStatus();
   // later use status of analysis here...
   fxStatusBuffer->Reset();
   fxStatusBuffer->InitMap();
   TFile *filsav = gFile;
   gFile = 0;
   fxStatusBuffer->WriteObject(state);      
   gFile = filsav;
   
   

}



ClassImp(TGo4HadesClient)