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
    TString *pInFileName = 
      //new TString("~/hades/hydra/production/REAL/PARAMS/pidShowerPar2D_ring.root");
      new TString("~/hades/hydra/macros/PID/RESULTS/NOV01_GEN6_2/AllSimPidContainersNov01_gen6_3D.root");
    
    //Define containers you want to insert in the following way: 
    
    //TO DO

    pListCont->Add(new TObjString("PidReconstructorPar")); 
    pListCont->Add(new TObjString("PidTrackFillerPar")); 
    
    
    //DONE!
    //pListCont->Add(new TObjString("PidAlgShowerPar")); 
    //pListCont->Add(new TObjString("PidAlgMomVsBetaPar")); 
    //pListCont->Add(new TObjString("PidAlgMomVsElossPar")); 
    //pListCont->Add(new TObjString("PidElossMPVShiftPar")); 
    //pListCont->Add(new TObjString("PidAlgRingPropertiesPar")); 
    
    // For which runID? Define nRunId or(and) nRefRunID.
    //Default value for nRefRunID should be always -1 !!!
    Int_t nRunId = 3;
    Int_t nRefRunId = -1;

    //Set author name and description (if needed)
    TString *pAuthor = new TString("Wojcik/Christ");
    TString *pDesc = new TString("Parameters for nov01 simulation data");
    
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
    //Output RTDB
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



