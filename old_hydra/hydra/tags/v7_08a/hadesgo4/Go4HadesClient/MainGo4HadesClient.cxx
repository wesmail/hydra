#include "MainGo4HadesClient.h"
#include "Go4AnalysisClient/TGo4AnalysisClient.h"
#include "TGo4Hades.h"

void usage();

      
extern void InitGui();
VoidFuncPtr_t initfuncs[] = {InitGui, 0 };


TROOT threadtest("AnalysisClient","Go4 Application", initfuncs);  

int main(int argc, char **argv)
{
    Text_t* name;
    Text_t* hostname;
    Text_t* connector;
    UInt_t  con;
    TString param;
    if(argc<4)
    {
	usage();
	return 1;
    }
    else{
	name=argv[1];
	hostname=argv[2];
	connector=argv[3];
	con=atoi(connector);
	param=argv[4];
	cout << "Client:"<<name<<",\tHost:"<<hostname<<",\tConnector:"<<con<<",\n Parameter:"<<param.Data()<<endl;

	TApplication theApp("App", &argc, argv);

	TGo4Trace::set_trace_level(100);
	TGo4Trace::SetTracePriority(120);
	TGo4Trace::StopTracing();

	TGo4Hades *go4Hades =  new TGo4Hades();
        go4Hades->SetConfigFile(param);
	//go4Hades->SetAutoSaveInterval(0); //get ride of autosawing
	TGo4AnalysisClient* myclient = new TGo4AnalysisClient(name,go4Hades,hostname,con);
	cout<<"Created Analysis HadesClient Instance: "<<myclient->GetName()<<endl;
	theApp.Run();

	return 0;
    }
}
void usage()
{
    cout << "usage: MainGo4HadesClient clientname serverhostname connectorport parameterfile"<<endl;
    exit(1);
}
