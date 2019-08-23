makeEmptyTrackFillerPar(){
   TString sOutput("./EmptyTrackFillerParJan04Exp.root");

   //Int_t nRunId=1763258097; //Nov02Exp
   //Int_t nRunId=501; //Nov02Sim

   //Int_t nRunId=1274446693;//Aug04Exp

   Int_t nRunId=6003;//Jan04Sim
   //Int_t nRunId=1695873777;//Jan04Exp
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
