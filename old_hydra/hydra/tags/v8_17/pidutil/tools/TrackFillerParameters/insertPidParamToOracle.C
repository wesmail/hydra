////////////////////////////////////////////////////////////////////////////////
// Macro for writing various pid parameter containers into Oracle DB
// 
// Autor:  T.Wojcik (twojcik@if.uj.edu.pl)
// Last changed: 04 March 2004 
////////////////////////////////////////////////////////////////////////////////

void insertPidParamToOracle(){
    TList *pListCont = new TList(); // Do not change
    
    //****** User configuration area comes here******//
    
    // input root file with your containers
    //TString *pInFileName = 

    TString *pInFileName = new TString("/home/tachrist/hades/projects/current/phd/TrackFillerParameters/DefaultPidTrkFillerPar_RICH_MDC_META.root");


    //Define containers you want to insert in the following way: 
    
    
    //DONE!
    //pListCont->Add(new TObjString("PidAlgShowerPar")); 
    //pListCont->Add(new TObjString("PidAlgMomVsBetaPar")); 
    //pListCont->Add(new TObjString("PidAlgMomVsElossPar")); 
    //pListCont->Add(new TObjString("PidElossMPVShiftPar")); 
    //pListCont->Add(new TObjString("PidAlgRingPropertiesPar")); 
    pListCont->Add(new TObjString("PidTrackFillerPar")); 
    

    // For which runID? Define nRunId or(and) nRefRunID.
    //Default value for nRefRunID should be always -1 !!!
    //Int_t nRunId = 3; --> for nov01
    //Int_t nRunId = 501; //--> for nov02
    Int_t nRunId = 7003; //--> for aug04 simulation
    //Int_t nRunId = 1; //--> for april06 dummy
    Int_t nRefRunId = -1;

    //Set author name and description (if needed)
    TString *pAuthor = new TString("Tassilo Christ");
    TString *pDesc = new TString("Default extended pid trackfiller parameters with META/MDC matching information - refined windows for RICH-MDC and MDC-META correlation no ring-shift applied");
    
    //****** End of user configuration area ******//   
    //****** Do not change lines below ********************//

	Hades *myHades = new Hades();
	HRuntimeDb *rtdb = gHades->getRuntimeDb();
    //Input RTDB
	HParRootFileIo *pInFile = new HParRootFileIo();
	if(!pInFile->open(pInFileName->Data())){
        return;
    }
	rtdb->setFirstInput(pInFile);
    
    //Loop over all defined containers
    TIterator *pIter = pListCont->MakeIterator();
    TObjString *pParName=NULL;
    HParSet *pParSet=NULL;
    pIter->Reset();
    while(( pParName = (TObjString*)pIter->Next() )!=NULL){
        pParSet = rtdb->getContainer(pParName->GetString().Data());
        if ( pParSet !=NULL ){ 
        }
    }
    if ( !rtdb->initContainers(nRunId,nRefRunId)){
        cerr<<"Can't initialize containers"<<endl;
        return;
    }
    pParSet = rtdb->getContainer("PidTrackFillerPar");
    pParSet->setParamContext("PidTrackFillerPar");
    //Output RTDB
    //

    HParOraIo* ora= new HParOraIo;
    if (!ora->open("pid_oper")) { //test account
      cout<<"\nTry again...\n";
      if(!ora->open("pid_oper") ){
	cout<<"\nSorry, password still not correct!!!!\n";
	delete myHades;
	return ;
      }
    }
    
    rtdb->setOutput(ora);
    ora->print();

    pIter->Reset();
    Int_t nStatus=0;
    while(( pParName = (TObjString*)pIter->Next() )){
        pParSet = rtdb->getContainer(pParName->GetString().Data());
        pParSet->setAuthor(pAuthor->Data());
        pParSet->setDescription(pDesc->Data());
	    nStatus = pParSet->write(ora);
        if (nStatus==-1) {
            pParSet->setChanged(kFALSE);
        }
    }
    rtdb->closeOutput();
    delete myHades;
}



