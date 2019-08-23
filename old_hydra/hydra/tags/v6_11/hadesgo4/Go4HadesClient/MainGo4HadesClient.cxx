#include "MainGo4HadesClient.h"
#include "TGo4AnalysisClient.h"
#include "TGo4Hades.h"

void usage();

      
extern void InitGui();
VoidFuncPtr_t initfuncs[] = {InitGui, 0 };


TROOT threadtest("AnalysisClient","Go4 Application", initfuncs);  

int main(int argc, char **argv)
{


   TApplication theApp("App", &argc, argv);

   TGo4Trace::set_trace_level(100);
   TGo4Trace::SetTracePriority(120);
   TGo4Trace::StopTracing(); 
   if(argc<4){
		usage();
         
		}
	else{      
		Text_t* name=argv[1];
		Text_t* hostname=argv[2];
		Text_t* connector=argv[3];
		UInt_t con=atoi(connector);
		cout << "Client:"<<name<<",\tHost:"<<hostname<<",\tConnector:"<<con<<endl;
      
		TGo4Hades *go4Hades =  new TGo4Hades();  
		//		go4Hades->SetAutoSaveInterval(0); //get ride of autosawing
		TGo4AnalysisClient* myclient = new TGo4AnalysisClient(name,go4Hades,hostname,con);     
		cout<<"Created Analysis HadesClient Instance: "<<myclient->GetName()<<endl; 
		theApp.Run();
            
		return 0;
         
   } // if(argc<4)
   return 1; 
}


void usage()
{
cout << "usage: MainGo4HadesClient clientname serverhostname connectorport"<<endl;

}
