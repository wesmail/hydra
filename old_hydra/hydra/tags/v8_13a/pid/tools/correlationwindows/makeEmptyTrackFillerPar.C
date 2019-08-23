//This macro creates an empty instance of HPidTrackFillerPar for initialization purposes
makeEmptyTrackFillerPar(){
  TString sOutput("../output/Aug04/EmptyTrackFillerPar.root");


  Int_t nRunId= 1;//April06 dummy
   Hades myHades;
   HRuntimeDb *rtdb = gHades->getRuntimeDb();
   HPidTrackFillerPar* pPar = rtdb->getContainer("PidTrackFillerPar");

   if(pPar==NULL){
       Error("createPidParams","Container HPidTrackFillerPar  not created");
       exit(EXIT_FAILURE);
   }
   HParRootFileIo *pOutFile = new HParRootFileIo();
   pOutFile->open(sOutput.Data(), "RECREATE");
   rtdb->setOutput(pOutFile);

   HRun *run = new HRun(nRunId);
   rtdb->addRun(nRunId);
   rtdb->print();
   pPar->setInputVersion(1,0);
   HParVersion *version = new HParVersion(((char*)pPar->GetName()));
   rtdb->getRun(nRunId)->addParVersion(version);
   pPar->setChanged();
   rtdb->Write();
   //delete gHades;
}
