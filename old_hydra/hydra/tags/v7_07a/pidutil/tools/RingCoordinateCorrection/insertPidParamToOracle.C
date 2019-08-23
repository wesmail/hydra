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
      new TString("path_to_parameterfile/PreliminaryPidTrkFillerParamsSimJan04.root");

    //Define containers you want to insert in the following way: 
    

    pListCont->Add(new TObjString("PidTrackFillerPar")); 

    Int_t nRunId = 6003; //--> for jan04
    Int_t nRefRunId = -1;

    //Set author name and description (if needed)
    TString *pAuthor = new TString("Your name");
    TString *pDesc = new TString("Comment here");
    
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



