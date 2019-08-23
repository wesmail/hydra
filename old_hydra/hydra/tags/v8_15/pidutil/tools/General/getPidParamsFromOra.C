///////////////////////////////////////////////////////////////////////////////
//Macro for retrieving pid container(s) from Oracle.
//Parameters are storing there before one can validate it.
//Retrieving via this method may be useful for testing parameters,
//
// Autor:  T.Wojcik (twojcik@if.uj.edu.pl)
// Last changed: 17 March 2004 
////////////////////////////////////////////////////////////////////////////////

void getPidParamsFromOra(Char_t* outfilename="testparam.root"){
    
  Int_t nRunId=9;
  Hades *myHades = new Hades();
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  //Set output
  HParRootFileIo *pOutFile = new HParRootFileIo();
  pOutFile->open(outfilename,"RECREATE");
  rtdb->setOutput(pOutFile);
  //Set input 
  HParOraIo* ora= new HParOraIo;
  ora->open();
  rtdb->setFirstInput(ora);

  
  HPidTrackFillerPar *pTrackFillerPar
    = (HPidTrackFillerPar *) rtdb->getContainer("PidTrackFillerPar");

  
  rtdb->initContainers(nRunId);
  
  rtdb->closeOutput();
  ora->print();
  delete myHades;

}



