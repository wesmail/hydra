// Macro to modify/update PidAlgStandCutsPar paramter container. As the input it requries 
// the root file with PidAlgStandCutsPar PID parameters. Before runing the macro user must set
// input and output files, runId and write code which modify the parameters. The modified 
// PidAlgStandCutsPar parameter container will be stored in the output file.
//
// To run this macro the ROOT and HYDRA enviroment must be set. 
// To start this macro use:
// 
// root -l updateHardCutPidParams()
// or
// root> .x updateHardCutPidParams()

int updateHardCutPidParams()
{
// -- start user configuration

  // input file
  TString inParFile = "test_hard_cuts_aug04.root"; 

  // output file
  TString outParFile = "test_hard_cuts_aug04_modify.root"; 
  
  //ref nRunId
  Int_t nRunId=7003;
  //Int_t nRunId=621306858;

// -- end user settings
  
  TStopwatch timer;
  Int_t evN=0;

  // set gHades
  if(gHades == NULL) new Hades;
  gHades->setTreeBufferSize(8000);

  //Set batch (needed for TCanvas's)

  gROOT->SetBatch();

  HRuntimeDb* rtdb=gHades->getRuntimeDb();

  HParRootFileIo *input=new HParRootFileIo;
  input->open((Text_t*)inParFile.Data());
  rtdb->setFirstInput(input);

  HParRootFileIo *output=new HParRootFileIo;
  output->open((Text_t*)outParFile.Data(),"RECREATE");
  rtdb->setOutput(output);

  // init parameter containers

  HPidAlgStandCutsPar *par = rtdb->getContainer("PidAlgStandCutsPar");
  rtdb->initContainers(nRunId);

  // User can modify/update parameters here
  // Example for the Rich:
    /*
  	// new Rich paramters
    TH1F *h = new TH1F("h","new rich",4,0,4);
    Float_t y[4] = {200,20,20,5};
    for(int i=0; i<4; ++i) h->Fill(i,y[i]);
  
  	// update Rich parameters
	Int_t aPart[2]={2,3};
	for(int sec=0; sec<6; ++sec) {
		par->addHistogram(aPart[0],0,sec,h);
		par->addHistogram(aPart[1],0,sec,h);
	}
	*/
  //--
  par->setChanged();
  rtdb->setContainersStatic();
  rtdb->saveOutput();
  rtdb->print();
  
  delete gHades;

  return 0;
} 
