void makeDocs()
{
    gROOT->Reset();
    TString vers=gSystem->Getenv("HYDRA_VERSION");
    if(vers.IsNull())
    {
	cout<<"NO HYDRA VERSION SET!!! USE hadenv.sh correctly!....exiting"<<endl;
        exit(1);
    }
    TString rootpath="http://root.cern.ch/root/html303/";
    TString docuout="/misc/halo/web-docs/docs/hydra/classDocumentation/$(HYDRA_VERSION)/";

    cout<<"#################################################"<<endl;
    cout<<"HYDRA_VERSION= "<<vers.Data()<<endl;
    cout<<"OUTPUTPATH= "<<gSystem->ExpandPathName(docuout.Data())<<endl;
    cout<<"ROOTPATH= "<<rootpath.Data()<<endl;
    cout<<"#################################################"<<endl;

    gEnv->SetValue("Root.Html.OutputDir",docuout.Data());
    gEnv->SetValue("Root.Html.Root",rootpath.Data());
    gEnv->SetValue("Unix.*.Root.Html.SourceDir","$(HYDRA_PATHS)");
    gEnv->SetValue("Root.Html.SourceDir","$(HYDRA_PATHS)");
    gEnv->SetValue("Unix.*.Root.IncludePath","-I. $(HYDRA_INCLUDES)");
    gEnv->SetValue("Root.Html.SourcePrefix","$(HADDIR)");
    gEnv->PrintEnv();
    THtml html;
    html.MakeAll();
}
