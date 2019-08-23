///////////////////////////////////////////////////////////////////////////////
//Macro for retrieving pid container(s) from Oracle loading Tables.
//Parameters are storing there before one can validate it.
//Retrieving via this method may be useful for testing parameters,
//
// Autor:  T.Wojcik (twojcik@if.uj.edu.pl)
// Last changed: 17 March 2004 
////////////////////////////////////////////////////////////////////////////////

void getPidParamsFromOraLoadingTable(){
    TMap *pContMap = new TMap(10); // Do not change

    //****** User configuration area comes here******//
    //
    //Define output file name
	TString *pOutputFileName = new TString("~/hades/hydra/macros/PID/RESULTS/NOV01_GEN6_2/paramsFromOra.root");
    
    //Define Container names and corresponding version id (from Oracle)
    //like this : pContMap->Add("container_name","version_id")
    //
	//pContMap->Add(new TObjString("PidAlgMomVsBetaPar"),new TObjString("155")); 
	//pContMap->Add(new TObjString("PidTrackFillerPar"),new TObjString("154")); 
	pContMap->Add(new TObjString("PidAlgRingPropertiesPar"),new TObjString("159")); 
	TString  *pAuthor = new TString("Tassilo Christ");
    TString *pDesc	= new TString("Test parameters");
    
    //For which runID? Define nRunId or(and) nRefRunID.
    //Default value for nRefRunID should be always -1 !!!
	Int_t nRunId=1006222448;
    Int_t nRefRunId = -1;



    //****** End of user configuration area ******//   
    //****** Do not change lines below ********************//

	Hades *myHades = new Hades();
	HRuntimeDb *rtdb = gHades->getRuntimeDb();
    //Set output
	HParRootFileIo *pOutFile = new HParRootFileIo();
	pOutFile->open(pOutputFileName->Data(),"RECREATE");
    rtdb->setOutput(pOutFile);
	//Set input 
    HParOraIo* ora= new HParOraIo;
	ora->open();
	rtdb->setFirstInput(ora);
    
    TIterator *pIt = pContMap->MakeIterator();
    TObjString *pObj = NULL;
    TObjString *pValue = NULL;
    HCondParOraIo * pParOraIo=NULL;
    pIt->Reset();
    while( (pObj = (TObjString*)pIt->Next()) !=NULL ){
        pPar =  (HPidParCont *)rtdb->getContainer(pObj->GetString()->Data()); 
        pParOraIo = (HCondParOraIo*)ora->getDetParIo("HCondParIo");
        pValue= (TObjString*)pContMap->GetValue(pObj);
        pParOraIo->readFromLoadingTable(pPar,atoi(pValue->GetString()->Data()));
	    pPar->setAuthor(pAuthor->Data());
	    pPar->setDescription(pDesc->Data());
	    pPar->setChanged();		 
        //pPar->Write();
	    //pPar->print();
    }
    rtdb->initContainers(nRunId);
	
    rtdb->closeOutput();
    ora->print();
    delete myHades;

}



