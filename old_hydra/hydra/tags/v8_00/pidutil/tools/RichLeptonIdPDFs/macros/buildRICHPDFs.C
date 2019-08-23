Clean(TH2F* Original, Int_t nDesiredNeighbours){
  
  TH2F* Clone = Original->Clone();
  

  Int_t nBinsX = Original->GetNbinsX();
  Int_t nBinsY = Original->GetNbinsY();
  Float_t locSum=0.0;
  Int_t nLocalNeighbours;

  //Loop over all bins, remove those with less than nDesiredNeighbours
  for(Int_t xCount=1; xCount<nBinsX; xCount++){
    for(Int_t yCount=1; yCount<nBinsY; yCount++){
      nLocalNeighbours=0;
      locSum=0.0;
      //Only bins with nonzero value are of interest
      if(Original->GetBinContent(xCount, yCount) > 0.0){
	
	for(Int_t xNeighbourCount=-1;xNeighbourCount<2;xNeighbourCount++){
	  for(Int_t yNeighbourCount=-1;yNeighbourCount<2;yNeighbourCount++){
	    if(Original->GetBinContent(xCount+xNeighbourCount, yCount+yNeighbourCount) > 0.0){
	      nLocalNeighbours++;
	      locSum+=Original->GetBinContent(xCount+xNeighbourCount, yCount+yNeighbourCount);
	    }
	  }
	}
	if(nLocalNeighbours<1+nDesiredNeighbours){ //Add 1 because the pad itself is counted in the loop
	  Clone->SetBinContent(xCount, yCount, 0.0);
	}
      }
    }
  }
  //Transfer the cleaned histogram into the original
  for(Int_t xCount=1; xCount<nBinsX; xCount++){
    for(Int_t yCount=1; yCount<nBinsY; yCount++){
      Original->SetBinContent(xCount, yCount,Clone->GetBinContent(xCount, yCount));
    }
  }
  delete Clone;
}

smoothDistribution(TH2F* Original, TH2F* Clone,Int_t scale=1)
{
  Float_t binValue=0.0;
  Int_t xKernelCount,yKernelCount;

  Float_t origIntegral=Original->Integral();
  Int_t NNMin=10;

  Clone->Reset();

  /*
  TH2F* LocalClone1 = (TH2F*)(Original->Clone());
  TH2F* LocalClone2 = (TH2F*)(Original->Clone());

  LocalClone1->Reset();
  LocalClone2->Reset();
  */

  Int_t nBinsX = Original->GetNbinsX();
  Int_t nBinsY = Original->GetNbinsY();

  Int_t nEmptyBinsOld, nEmptyBins=0;
  Int_t nNeighbours=0;
  Float_t locSum=0.0;
  Float_t locSumTemp=0.0;

  //Loop over all bins (2 to nbins-1) in the histogram - if they are NOT empty sqeeze them!
  //Store the results in both Clones;
  for(Int_t xCount=2; xCount<nBinsX-1; xCount++){
    for(Int_t yCount=2; yCount<nBinsY-1; yCount++){
      binValue = (Float_t)(Original->GetBinContent(xCount,yCount));
      if(binValue>0.0){
	for(xKernelCount=-1;xKernelCount<2;xKernelCount++){
	  for(yKernelCount=-1;yKernelCount<2;yKernelCount++){
	    Clone->SetBinContent(xCount+xKernelCount, 
				 yCount+yKernelCount,
				 Clone->GetBinContent(xCount+xKernelCount, yCount+yKernelCount)+
				 (1.0/9.0)*binValue);
	  }
	}
      }
    }
  }
}


smoothDistributionClever(TH2F* Original, TH2F* Clone)
{
  Float_t binValue=0.0;
  Int_t xKernelCount,yKernelCount;

  Float_t origIntegral=Original->Integral();
  Int_t NNMin=10;

  TH2F* LocalClone1 = (TH2F*)(Original->Clone());
  TH2F* LocalClone2 = (TH2F*)(Original->Clone());

  LocalClone1->Reset();
  LocalClone2->Reset();

  Int_t nBinsX = Original->GetNbinsX();
  Int_t nBinsY = Original->GetNbinsY();

  Int_t nEmptyBinsOld, nEmptyBins=0;
  Int_t nNeighbours=0;
  Float_t locSum=0.0;
  Float_t locSumTemp=0.0;

  //Loop over all bins (2 to nbins-1) in the histogram - if they are NOT empty sqeeze them!
  //Store the results in both Clones;
  for(Int_t xCount=2; xCount<nBinsX-1; xCount++){
    for(Int_t yCount=2; yCount<nBinsY-1; yCount++){
      binValue = (Float_t)(Original->GetBinContent(xCount,yCount));
      if(binValue>0.0){
	for(xKernelCount=-1;xKernelCount<2;xKernelCount++){
	  for(yKernelCount=-1;yKernelCount<2;yKernelCount++){
	    LocalClone1->SetBinContent(xCount+xKernelCount, 
				       yCount+yKernelCount,
				       LocalClone1->GetBinContent(xCount+xKernelCount, yCount+yKernelCount)+
				       (1.0/9.0)*binValue);
	    LocalClone2->SetBinContent(xCount+xKernelCount, 
				       yCount+yKernelCount,
				       LocalClone2->GetBinContent(xCount+xKernelCount, yCount+yKernelCount)+
				       (1.0/9.0)*binValue);
	  }
	}
      }
    }
  }


  //Count empty bins in the region we intend to smooth
  for(Int_t xCount=3; xCount<nBinsX-2; xCount++){
    for(Int_t yCount=3; yCount<nBinsY-2; yCount++){
      if(LocalClone1->GetBinContent(xCount, yCount)==0)
	{
	  nEmptyBins++;
	}
    }
    
  }
  
  Int_t iterations = 0;
  //while(nEmptyBins>0){
  while(iterations<5){
    iterations++;
    nEmptyBinsOld=nEmptyBins;
    nEmptyBins=0;
    //Count how many empty bins are still left
    for(Int_t xCount=3; xCount<nBinsX-2; xCount++){
      for(Int_t yCount=3; yCount<nBinsY-2; yCount++){
	if(LocalClone1->GetBinContent(xCount, yCount)==0)
	  {nEmptyBins++;}
      }
      
    }
    cout << nEmptyBins << " left" << endl;    
    if (nEmptyBinsOld==nEmptyBins){
      //reduce the required number of required neighbours by one
      NNMin--;
      cout << "reducing the number of required neighbours to " << NNMin << endl;
    }
    //Loop over all bins
    for(Int_t xCount=3; xCount<nBinsX-2; xCount++){
      for(Int_t yCount=3; yCount<nBinsY-2; yCount++){
	nNeighbours=0;
	locSum=0.0;
	locSumTemp=0.0;
	//count the neighbours with nonzero content and sum these contents
	for(xKernelCount=-2;xKernelCount<3;xKernelCount++){
	  for(yKernelCount=-2;yKernelCount<3;yKernelCount++){
	    if(LocalClone1->GetBinContent(xCount+xKernelCount, yCount+yKernelCount)>0.)
	      {
		nNeighbours++;
		locSum+=LocalClone1->GetBinContent(xCount+xKernelCount, yCount+yKernelCount);
	      }
	  }
	}
	//If the bin had less than 25 neighbours and more than the currently required minimum number of neighbours
	//apply smoothing i.e. set the bin content of the current bin to the average of the surrounding bins.
	if(nNeighbours<25 && nNeighbours>=NNMin)
	  {
	    //store the result in the local clone copy
	    LocalClone2->SetBinContent(xCount, yCount,locSum/25.0);
	  }
	
	
      }
      
    }
    //THe next iteration starts with the new LocalClone2 as the starting point - do the renaming
    delete LocalClone1;
    LocalClone1 = LocalClone2;
    LocalClone2 = (TH2F*)(LocalClone1->Clone());
    
    	
  }
  
  //Store the results in the clone histogram for the outside world
  for(Int_t xCount=1; xCount<nBinsX; xCount++){
    for(Int_t yCount=1; yCount<nBinsY; yCount++){
      Clone->SetBinContent(xCount,yCount,LocalClone2->GetBinContent(xCount,yCount));
    }
  }

  //Now we renormalize the clone such that the yield in the original non-empty bins is not altered!
  Float_t finalIntegral = 0.0;
  for(Int_t xCount=1; xCount<nBinsX; xCount++){
    for(Int_t yCount=1; yCount<nBinsY; yCount++){
      if(Original->GetBinContent(xCount,yCount)){
	finalIntegral+=Clone->GetBinContent(xCount,yCount);
      }
      
    }
  }
  Float_t Ratio = origIntegral/finalIntegral;
  Clone->Scale(Ratio);

  delete LocalClone1;
  delete LocalClone2;

}

void buildRICHPDFs(const char* leaf1="n_rich_patmatrix", 
		   const char* leaf2="n_rich_npads",
		   const Int_t cutting = 1,
		   const Int_t cleaning = 0,
		   const Int_t smoothing = 1,
		   const Int_t computeProbability = 1,
		   const Int_t testrejection = 1,
		   const Char_t* filename="~/hades/hydra/work/SplitPIDNtuplesNov02SimGen2b.root",
		   const Float_t lowerThetaBound=10.0,
		   const Float_t upperThetaBound=90.0,
		   const Char_t* targetdirectory = "~/hades/hydra/work/nov02/Sim/Gen2b/",
		   const Char_t* experiment_ID = "nov02_sim_gen2",
		   const Int_t   selectedSector=0,
		   const Float_t centroidSelection=1,
		   const Int_t savePDFs=1,
		   const Int_t makeCL=1,
		   const Char_t* tracktype="kick",
		   const Char_t* trackcontext="LowRes")
{
  gROOT->Reset();
  //Open the file which contains the ntuples with
  //the output from the correlator, sorted by single,
  //double fake and hadron rings
  TFile* CorrelatorFile = new TFile(filename);
  
  //--------------------------------------------------------------------------------------------------------------  

  //In order to analyse both original and transformed data we first map the 
  //name of the ntuple to a local pointer variable
  TNtuple* SingleNtuple;
  TNtuple* DoubleNtuple;
  TNtuple* FakeNtuple;
  TNtuple* HadronNtuple;

  //This if-statement is true, when we use the observables in the original coordinate system - before the PCA
  if(CorrelatorFile->FindObjectAny("SingleProperties")){
    SingleNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("SingleProperties"));
    DoubleNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("DoubleProperties"));
    FakeNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("FakeProperties"));
    HadronNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("HadronProperties"));
    cout << "Using original data!" << endl;
  }
  //This if-statement is true, when we use the observables in the transformed coordinate system - after the PCA
  if(CorrelatorFile->FindObjectAny("TransformedSingleProperties")){
    SingleNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("TransformedSingleProperties"));
    DoubleNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("TransformedDoubleProperties"));
    FakeNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("TransformedFakeProperties"));
    HadronNtuple = (TNtuple*)(CorrelatorFile->FindObjectAny("TransformedHadronProperties"));
    cout << "Using transformed data!" << endl;
  }
  
  //--------------------------------------------------------------------------------------------------------------  

  //Next we assemble a list of cuts we whish to apply to our data, before we perform any analysis
  //Boundaries in Phi
  Float_t lowerPhiBound = 0.0;
  Float_t upperPhiBound = 360.0;

  //Boundaries in Theta, given in the function call
  char thetaCutString[128]; 
  char phiCutString[128]; 
  //cout << phiCutString<<endl;
  char sectorCutString[16];

  sprintf(thetaCutString,"%s%f%s%","f_track_theta>",lowerThetaBound,"&&f_track_theta<=",upperThetaBound);
  sprintf(phiCutString,"%s%f%s%f","f_track_phi>=",lowerPhiBound,"&&f_track_phi<=",upperPhiBound);  
  sprintf(sectorCutString,"%s%d","n_sector==",selectedSector);
  //cout << phiCutString<<endl;
  //We forsee different possibilities for applying cuts to our sample in phi/theta:
  if(cutting==1)
    {
      sprintf(thetaCutString,"%s%d%s%d","f_track_theta>",lowerThetaBound,"&&f_track_theta<=",upperThetaBound);
    }
  cout << phiCutString<<endl;
  if(cutting==2)
    {
      cout << "Enter lower phi boundary." << endl;
      cin >> lowerPhiBound;
      cout << "Enter upper phi boundary." << endl;
      cin >> upperPhiBound;
      sprintf(phiCutString,"%s%d%s%d","f_track_phi>=",lowerPhiBound,"&&f_track_phi<=",upperPhiBound);

    }
  cout << phiCutString<<endl;
  if(cutting==3)
    {
      //Cut on theta AND phi

      cout << "Enter lower theta boundary." << endl;
      cin >> lowerThetaBound;
      cout << "Enter upper theta boundary." << endl;
      cin >> upperThetaBound;
 
      sprintf(thetaCutString,"%s%d%s%d","f_track_theta>",lowerThetaBound,"&&f_track_theta<=",upperThetaBound);

      cout << "Enter lower phi boundary." << endl;
      cin >> lowerPhiBound;
      cout << "Enter upper phi boundary." << endl;
      cin >> upperPhiBound;

      sprintf(phiCutString,"%s%d%s%d","f_track_phi>=",lowerPhiBound,"&&f_track_phi<=",upperPhiBound);
    }
  //cout << phiCutString<<endl;
  //Here we build the solid-angle element from which we choose our data
  TCut ThetaCut(thetaCutString);
  TCut PhiCut(phiCutString);
  TCut AngularCut = ThetaCut && PhiCut;
  TCut SectorCut(sectorCutString);
  
  //Finally we check whether we are interested in the centroid - if so we use only rings with
  //a meaningful value for the centroid
  
  //We choose positive centroid values
  if(centroidSelection>0)
    TCut centroidCut("f_rich_centroid>=0.0");
  
  //We choose negative centroid values
  if(centroidSelection<0)
    TCut centroidCut("f_rich_centroid<0.0");

  //We use all centroids
  if(centroidSelection==0)
    TCut centroidCut("1");

  
  TString trackString(tracktype);
  if(trackString.Contains("kick"))
    {
      TCut trackingCut("b_kicktrack==1"); //Use only those entries, for which tracking succeeded!
    }
  else if(trackString.Contains("spline"))
    {
      TCut trackingCut("b_splinetrack==1 && (f_tof_tof>-1.0 ||f_shw_sum0>-1)"); //Use only those entries, for which tracking succeeded!
    }
  else
    {
      cout << "Unknown tracking: " << trackString.Data() << endl;
      exit(-1);
    }

  if(centroidSelection==-1){
    TCut TotalCutLeptons = trackingCut && SectorCut && centroidCut;
    TCut TotalCutOthers = trackingCut && SectorCut && centroidCut;
  }
  else
    {
      TCut TotalCutLeptons = AngularCut && trackingCut && SectorCut && centroidCut;
      TCut TotalCutOthers =  AngularCut && trackingCut && SectorCut && centroidCut;
    }

  cout << "We are rejecting the following way" << endl;
  cout << TotalCutLeptons.GetTitle() << endl;
  cout << endl;
  
  cout << "There are " << SingleNtuple->GetEntries() << " single rings before the initial cuts!" << endl;
  cout << "There are " << DoubleNtuple->GetEntries() << " double rings before the initial cuts!" << endl;
  cout << "There are " << FakeNtuple->GetEntries() << " fake rings before the initial cuts!" << endl;
  cout << "There are " << HadronNtuple->GetEntries() << " hadron rings before the initial cuts!" << endl;

  //--------------------------------------------------------------------------------------------------------------  


  //From the pointers we generate a final sample of data  by selecting only those data 
  //out of the ntuple which survive our cuts. From now on we only work on THESE pruned ntuples!
  //Thus we avoid distorting statistics and erroneous normalization
  SingleNtuple->Draw(">>singleRingList",TotalCutLeptons.GetTitle());
  TEventList* singleEventList = (TEventList*)gDirectory->Get("singleRingList");
  SingleNtuple->SetEventList(singleEventList);

  DoubleNtuple->Draw(">>doubleRingList",TotalCutLeptons.GetTitle());
  TEventList* doubleEventList = (TEventList*)gDirectory->Get("doubleRingList");
  DoubleNtuple->SetEventList(doubleEventList);

  FakeNtuple->Draw(">>fakeRingList",TotalCutOthers.GetTitle());
  TEventList* fakeEventList = (TEventList*)gDirectory->Get("fakeRingList");
  FakeNtuple->SetEventList(fakeEventList);

  HadronNtuple->Draw(">>hadronRingList",TotalCutOthers.GetTitle());
  TEventList* hadronEventList = (TEventList*)gDirectory->Get("hadronRingList");
  HadronNtuple->SetEventList(hadronEventList);


  //No you cannot call GetEntries() directly, The tree does NOT automatically reset its entry number when a new
  //eventlist is assigned;
  cout << "There are " << singleEventList->GetN() << " single rings surviving all initial cuts!" << endl;
  cout << "There are " << doubleEventList->GetN() << " double rings surviving all initial cuts!" << endl;
  cout << "There are " << fakeEventList->GetN() << " fake rings surviving all initial cuts!" << endl;
  cout << "There are " << hadronEventList->GetN() << " hadron rings surviving all initial cuts!" << endl;



  //-----------------------------------------------------------------------------------------------------------------
  //No we have a clean sample - next we build names for histograms to store our PDFs!


  //make the strings required to name the histograms according to the observables we choose;
  TString* leafName1String = new TString(leaf1);
  TString* leafName2String = new TString(leaf2);

  //Build name of all histograms where we draw one observable vs the other
  TString* histogramName = new TString(leaf1);
  histogramName->Append("Vs");
  histogramName->Append(leaf2);
  
  //Good rings
  TString* histogramNameSingle = new TString(histogramName->Data());
  TString* histogramNameDouble = new TString(histogramName->Data());
  TString* histogramNameGood   = new TString(histogramName->Data());

  //Bad rings
  TString* histogramNameFake   = new TString(histogramName->Data());
  TString* histogramNameHadron = new TString(histogramName->Data());
  TString* histogramNameBad   = new TString(histogramName->Data());

  histogramNameSingle->Append("Single");
  histogramNameDouble->Append("Double");
  histogramNameGood->Append("Good");

  histogramNameFake->Append("Fake");
  histogramNameHadron->Append("Hadron");
  histogramNameBad->Append("Bad");


  //Clone our histograms for smoothing and cleaning purposes
  TString* cloneHistogramNameSingle = new TString(histogramNameSingle->Data());
  cloneHistogramNameSingle->Append("Clone");
  TString* cloneHistogramNameDouble = new TString(histogramNameDouble->Data());
  cloneHistogramNameDouble->Append("Clone");
  TString* cloneHistogramNameGood   = new TString(histogramNameGood->Data());
  cloneHistogramNameGood->Append("Clone");

  TString* cloneHistogramNameFake   = new TString(histogramNameFake->Data());
  cloneHistogramNameFake->Append("Clone");
  TString* cloneHistogramNameHadron   = new TString(histogramNameHadron->Data());
  cloneHistogramNameHadron->Append("Clone");
  TString* cloneHistogramNameBad   = new TString(histogramNameBad->Data());
  cloneHistogramNameBad->Append("Clone");
  

  //-----------------------------------------------------------------------------------------------------------------
  //We define the binning of the histograms according to the chosen variabls

  Float_t lHB1 = 0.0;
  Float_t lHB2 = 0.0;        //lower Histogram Boundaries
  Float_t uHB1 = 0.0;
  Float_t uHB2 = 0.0;        //upper Histogram Boundary
  Int_t nHB1 = 0;
  Int_t nHB2 = 0;            //number of histogram bins
   
  //For each variable a predefined binning exists. This can be changed by just adapting the numbers here!
  if(leafName1String->Contains("f_rich_ccOne"))
    {
      lHB1=-7.0;
      uHB1=7.0;
      nHB1=70;    
    }
  if(leafName2String->Contains("f_rich_ccOne"))
    {
      lHB2=-7.0;
      uHB2=7.0;
      nHB2=70;
    }

  if(leafName1String->Contains("f_rich_ccTwo"))
    {
      lHB1=-7.0;
      uHB1=4.0;
      nHB1=55;    
    }
  if(leafName2String->Contains("f_rich_ccTwo"))
    {
      lHB2=-7.0;
      uHB2=4.0;
      nHB2=55;
    }
  
  if(leafName1String->Contains("n_rich_patmatrix"))
    {
      uHB1=1800.0;
      nHB1=90;    
    }
  if(leafName2String->Contains("n_rich_patmatrix"))
    {
      uHB2=1800.0;
      nHB2=90;
    }

  if(leafName1String->Contains("n_rich_locmax"))
    {
      uHB1=25.0;
      nHB1=25;    
    }
  if(leafName2String->Contains("n_rich_locmax"))
    {
      uHB2=25.0;
      nHB2=25;
    }
  
  if(leafName1String->Contains("f_rich_phi"))
    {
      uHB1=360.0;
      nHB1=360;    
    }
  if(leafName2String->Contains("f_rich_phi"))
    {
      uHB2=360.0;
      nHB2=360;
    }

  if(leafName1String->Contains("f_rich_theta"))
    {
      lHB1=10.0;
      uHB1=90.0;
      nHB1=80;    
    }
  if(leafName2String->Contains("f_rich_theta"))
    {
      lHB2=10.0;
      uHB2=90.0;
      nHB2=80;
    }
  if(leafName1String->Contains("f_rich_centroid"))
    {
      uHB1=5.0;
      nHB1=50;    
    }
  if(leafName2String->Contains("f_rich_centroid"))
    {
      uHB2=5.0;
      nHB2=50;
    }
  if(leafName1String->Contains("f_rich_avcharge"))
    {
      uHB1=60.0;
      nHB1=60;
    }
  if(leafName2String->Contains("f_rich_avcharge"))
    {
      uHB2=60.0;
      nHB2=60;
    }

  if(leafName2String->Contains("n_rich_npads"))
    {
      uHB2=70.0;
      nHB2=35;
    }

  if(leafName1String->Contains("n_rich_npads"))
    {
      uHB1=70.0;
      nHB1=35;
    }




  //-----------------------------------------------------------------------------------------------------------------
  //Make the histograms for the original and the smeared distributions

  TH2F* leaf1VsLeaf2Single = new TH2F(histogramNameSingle->Data(),histogramNameSingle->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2Double = new TH2F(histogramNameDouble->Data(),histogramNameDouble->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2Good   = new TH2F(histogramNameGood->Data(),histogramNameGood->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);  

  TH2F* leaf1VsLeaf2Fake   = new TH2F(histogramNameFake->Data(),histogramNameFake->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2Hadron   = new TH2F(histogramNameHadron->Data(),histogramNameHadron->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2Bad   = new TH2F(histogramNameBad->Data(),histogramNameBad->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);

  TH2F* leaf1VsLeaf2SingleClone=new TH2F(cloneHistogramNameSingle->Data(),cloneHistogramNameSingle->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2DoubleClone=new TH2F(cloneHistogramNameDouble->Data(),cloneHistogramNameDouble->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2GoodClone   = new TH2F(cloneHistogramNameGood->Data(),cloneHistogramNameGood->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2); 

  TH2F* leaf1VsLeaf2FakeClone   = new TH2F(cloneHistogramNameFake->Data(),cloneHistogramNameFake->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2HadronClone   = new TH2F(cloneHistogramNameHadron->Data(),cloneHistogramNameHadron->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
  TH2F* leaf1VsLeaf2BadClone   = new TH2F(cloneHistogramNameBad->Data(),cloneHistogramNameBad->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);



  //-----------------------------------------------------------------------------------------------------------------
  //Make two canvases to draw the orignal and the smoothed distributions - for later use
  
  TString* distCanvasName = new TString(histogramName->Data());
  distCanvasName->Append("Distribution");
  TCanvas* leaf1VsLeaf2Canvas = new TCanvas(distCanvasName->Data(),distCanvasName->Data());
  distCanvasName->Append("Smoothed");
  TCanvas* leaf1VsLeaf2CloneCanvas = new TCanvas(distCanvasName->Data(),distCanvasName->Data());



  //-----------------------------------------------------------------------------------------------------------------
  //Make the strings specifying what to draw and into which histogram!
  TString* drawStringBase = new TString(leafName2String->Data());
  drawStringBase->Append(":");
  drawStringBase->Append(leaf1);
  drawStringBase->Append(">>"); 
 
  //strings for good rings
  TString* drawStringSingle = new TString(drawStringBase->Data());
  drawStringSingle->Append(histogramNameSingle->Data());
  TString* drawStringDouble = new TString(drawStringBase->Data());
  drawStringDouble->Append(histogramNameDouble->Data());
  TString* drawStringGood = new TString(drawStringBase->Data());
  drawStringGood->Append(histogramNameGood->Data());

  //strings for bad rings
  TString* drawStringFake = new TString(drawStringBase->Data());
  drawStringFake->Append(histogramNameFake->Data());
  TString* drawStringHadron = new TString(drawStringBase->Data());
  drawStringHadron->Append(histogramNameHadron->Data());
  TString* drawStringBad = new TString(drawStringBase->Data());
  drawStringBad->Append(histogramNameBad->Data());



  //-----------------------------------------------------------------------------------------------------------------
  //To generate the distributions we use only half of the data points
  //The other half is used to check the rejection quality with an 
  //independent sample
  if(testrejection){
    Int_t halfNSingles=(Int_t)(SingleNtuple->GetEventList()->GetN()/2);
    Int_t halfNDoubles=(Int_t)(DoubleNtuple->GetEventList()->GetN()/2);
    Int_t halfNFakes=(Int_t)(FakeNtuple->GetEventList()->GetN()/2);
    Int_t halfNHadrons=(Int_t)(HadronNtuple->GetEventList()->GetN()/2);
  }
  else
    {
      Int_t halfNSingles=(Int_t)(SingleNtuple->GetEventList()->GetN());
      Int_t halfNDoubles=(Int_t)(DoubleNtuple->GetEventList()->GetN());
      Int_t halfNFakes=(Int_t)(FakeNtuple->GetEventList()->GetN());
      Int_t halfNHadrons=(Int_t)(HadronNtuple->GetEventList()->GetN());
    }
  //Draw the distributions, giving the TotalCutLeptons object is unneccessary in principle,as the eventlist has allready been
  //made with the TotalCutLeptons object applied to it
  SingleNtuple->Draw(drawStringSingle->Data(),TotalCutLeptons,"",halfNSingles);
  DoubleNtuple->Draw(drawStringDouble->Data(),TotalCutLeptons,"",halfNDoubles);
  FakeNtuple->Draw(drawStringFake->Data(),TotalCutOthers,"",halfNFakes);
  HadronNtuple->Draw(drawStringHadron->Data(),TotalCutOthers,"",halfNHadrons);



  //-----------------------------------------------------------------------------------------------------------------
  //If the user requested cleaning of the histograms, we remove all the lonely entries in the histograms, to avoid outlyers
  //in the distributions - this is questionable, but seems to be neccessary, when smoothing is to be used afterwards
  if(cleaning){
    Clean(leaf1VsLeaf2Single,4);
    Clean(leaf1VsLeaf2Single,1);
    Clean(leaf1VsLeaf2Double,4);
    Clean(leaf1VsLeaf2Double,1);
    Clean(leaf1VsLeaf2Fake,4);
    Clean(leaf1VsLeaf2Fake,1);
    Clean(leaf1VsLeaf2Hadron,4);
    Clean(leaf1VsLeaf2Hadron,1);
  }


  //-----------------------------------------------------------------------------------------------------------------  
  //The disrtibtions are NOT analytically known, so to make them usable in a more general way we smear them out - 
  //also this algorithm probably has room for improvement!
  if(smoothing==0)
    {
      //We create copies of the original histograms so that we can use the
      //cloned histogram pointers later regardless of whether we smoothed 
      //the distributions or not!
      leaf1VsLeaf2SingleClone = (TH2F*)leaf1VsLeaf2Single->Clone();
      leaf1VsLeaf2DoubleClone = (TH2F*)leaf1VsLeaf2Double->Clone();
      leaf1VsLeaf2FakeClone   = (TH2F*)leaf1VsLeaf2Fake->Clone();
      leaf1VsLeaf2HadronClone   = (TH2F*)leaf1VsLeaf2Hadron->Clone();
    }

  
  if(smoothing==1)
    {
      leaf1VsLeaf2Single->Draw("COLZ");
      /*
      smoothDistributionClever(leaf1VsLeaf2Single,leaf1VsLeaf2SingleClone);
      smoothDistributionClever(leaf1VsLeaf2Double,leaf1VsLeaf2DoubleClone);
      smoothDistributionClever(leaf1VsLeaf2Fake,leaf1VsLeaf2FakeClone);
      smoothDistributionClever(leaf1VsLeaf2Hadron,leaf1VsLeaf2HadronClone);
      */
      smoothDistribution(leaf1VsLeaf2Single,leaf1VsLeaf2SingleClone);
      smoothDistribution(leaf1VsLeaf2Double,leaf1VsLeaf2DoubleClone);
      smoothDistribution(leaf1VsLeaf2Fake,leaf1VsLeaf2FakeClone);
      smoothDistribution(leaf1VsLeaf2Hadron,leaf1VsLeaf2HadronClone);

    }



  //-----------------------------------------------------------------------------------------------------------------
  //We store the normalized Probability Density Functions of the good and fake
  //rings for usage in the PID software! 


  if(savePDFs){
    //Here we first need to assemble the name of the file
    //because later this is used to identify the PDF for the hydra-container
    char thetacutstring[50];
    cout << "STORING!" << endl;
    sprintf(thetacutstring,"%s%d%s%d","_",lowerThetaBound,"_to_",upperThetaBound);

    TString* anglecutstring = new TString(thetacutstring);
    //anglecutstring->Append("_degrees");
    cout << "Storing PDF for theta region: " << anglecutstring->Data() << endl;

    TString* outfilename = new TString(targetdirectory);
    /*
    outfilename->Append("leptonpidpar_");

    if(centroidSelection>0)
      {
	outfilename->Append("with_centroid_");
      }
    if(centroidSelection<0)
      {
	outfilename->Append("no_centroid_");
      }
    if(centroidSelection==0)
      {
	outfilename->Append("with_centroid_");
      }
    
    outfilename->Append(experiment_ID);
    outfilename->Append(anglecutstring->Data());
    outfilename->Append("_sector_");
    
    outfilename->Append(secstring);
    */
    char secstring[2];
    sprintf(secstring,"%d",selectedSector);

    outfilename->Append("RichPdfHistograms_");
    outfilename->Append(experiment_ID);
    outfilename->Append("_");
    outfilename->Append(trackcontext);
    outfilename->Append(".root");

    TFile* OutputFile = new TFile(outfilename->Data(),"update");
    TH2F* pdf_leptons = leaf1VsLeaf2SingleClone->Clone();
    pdf_leptons->Add(leaf1VsLeaf2DoubleClone,1.0);
    pdf_leptons->Scale(1.0/pdf_leptons->Integral());

    TH2F* pdf_hadrons =  leaf1VsLeaf2HadronClone->Clone();
    pdf_hadrons->Scale(1.0/pdf_hadrons->Integral());

    TH2F* pdf_fakes =  leaf1VsLeaf2FakeClone->Clone();
    pdf_fakes->Scale(1.0/pdf_fakes->Integral());

    TString* pdf_fake_name = new TString("pdf_fakes_");
    TString* pdf_hadron_name = new TString("pdf_hadrons_");
    TString* pdf_lepton_name = new TString("pdf_leptons_");

    if(centroidSelection>0)
      {
	pdf_fake_name->Append("with_centroid");
	pdf_hadron_name->Append("with_centroid");
	pdf_lepton_name->Append("with_centroid");
      }
    if(centroidSelection==0)
      {
	pdf_fake_name->Append("with_centroid");
	pdf_hadron_name->Append("with_centroid");
	pdf_lepton_name->Append("with_centroid");
      }
    if(centroidSelection<0)
      {
	pdf_fake_name->Append("no_centroid");
	pdf_hadron_name->Append("no_centroid");
	pdf_lepton_name->Append("no_centroid");
      }


    pdf_fake_name->Append(anglecutstring->Data());
    pdf_hadron_name->Append(anglecutstring->Data());
    pdf_lepton_name->Append(anglecutstring->Data());

    pdf_fake_name->Append("_sector_");
    pdf_hadron_name->Append("_sector_");
    pdf_lepton_name->Append("_sector_");
    pdf_fake_name->Append(secstring);
    pdf_hadron_name->Append(secstring);
    pdf_lepton_name->Append(secstring);
    
    pdf_fakes->SetName(pdf_fake_name->Data());
    pdf_fakes->SetTitle(pdf_fake_name->Data());
    pdf_hadrons->SetName(pdf_hadron_name->Data());
    pdf_hadrons->SetTitle(pdf_hadron_name->Data());
    pdf_leptons->SetName(pdf_lepton_name->Data());
    pdf_leptons->SetTitle(pdf_lepton_name->Data());

    
    //We compute the CL-landscape for both histograms
    TH2F* cl_leptons = pdf_leptons->Clone();
    TH2F* cl_hadrons = pdf_hadrons->Clone();
    TH2F* cl_fakes = pdf_fakes->Clone();    

    TString* cl_hadron_name = new TString("cl_hadrons_");
    TString* cl_lepton_name = new TString("cl_leptons_");
    TString* cl_fake_name = new TString("cl_fakes_");

    if(centroidSelection>0)
      {
	cl_fake_name->Append("with_centroid");
	cl_hadron_name->Append("with_centroid");
	cl_lepton_name->Append("with_centroid");
      }
    if(centroidSelection==0)
      {
	cl_fake_name->Append("with_centroid");
	cl_hadron_name->Append("with_centroid");
	cl_lepton_name->Append("with_centroid");
      }
    if(centroidSelection<0)
      {
	cl_fake_name->Append("no_centroid");
	cl_hadron_name->Append("no_centroid");
	cl_lepton_name->Append("no_centroid");
      }
    
    //cl_fake_name->Append(experiment_ID);
    //cl_hadron_name->Append(experiment_ID);
    //cl_lepton_name->Append(experiment_ID);

    cl_fake_name->Append(anglecutstring->Data());
    cl_hadron_name->Append(anglecutstring->Data());
    cl_lepton_name->Append(anglecutstring->Data());
    cl_fake_name->Append("_sector_");
    cl_hadron_name->Append("_sector_");
    cl_lepton_name->Append("_sector_");
    cl_fake_name->Append(secstring);
    cl_hadron_name->Append(secstring);
    cl_lepton_name->Append(secstring);
    
    cl_fakes->SetName(cl_fake_name->Data());
    cl_fakes->SetTitle(cl_fake_name->Data());
    cl_hadrons->SetName(cl_hadron_name->Data());
    cl_hadrons->SetTitle(cl_hadron_name->Data());
    cl_leptons->SetName(cl_lepton_name->Data());
    cl_leptons->SetTitle(cl_lepton_name->Data());

    cl_fakes->Reset();
    cl_leptons->Reset();
    cl_hadrons->Reset();

    Int_t InnerXCount, OuterXCount;
    Int_t InnerYCount, OuterYCount;
    
    Float_t CurrentLeptonBinContent=0.0;
    Float_t CurrentHadronBinContent=0.0;
    Float_t CurrentFakeBinContent=0.0;
    Float_t LeptonConfLevel=0.0;
    Float_t HadronConfLevel=0.0;
    Float_t FakeConfLevel=0.0;

    if(makeCL)
      {        
	//We loop over each bin which we want to compute the CL for -  this is the OUTER loop!
	for(OuterXCount=0;OuterXCount<pdf_leptons->GetNbinsX();OuterXCount++)
	  //for(OuterXCount=95;OuterXCount<105;OuterXCount++)
	  {
	    for(OuterYCount=0; OuterYCount<pdf_leptons->GetNbinsY();OuterYCount++)
	      {
		//Reset the CL and get the current bin!
		LeptonConfLevel=0.0;
		HadronConfLevel=0.0;
		FakeConfLevel=0.0;
		CurrentLeptonBinContent=pdf_leptons->GetBinContent(OuterXCount,OuterYCount);
		CurrentHadronBinContent=pdf_hadrons->GetBinContent(OuterXCount,OuterYCount);
		CurrentFakeBinContent=pdf_fakes->GetBinContent(OuterXCount,OuterYCount);
		//In the inner loop we compute the yield in all bins which have a lower normalized PDF than the currently investigated 
		//bin - this is the CL for the current bin
		for(InnerXCount=0;InnerXCount<pdf_leptons->GetNbinsX();InnerXCount++)
		  {
		    for(InnerYCount=0; InnerYCount<pdf_leptons->GetNbinsY();InnerYCount++)
		      {
			if(pdf_leptons->GetBinContent(InnerXCount,InnerYCount)<CurrentLeptonBinContent)
			  {
			    LeptonConfLevel+=pdf_leptons->GetBinContent(InnerXCount,InnerYCount);
			  }
			if(pdf_hadrons->GetBinContent(InnerXCount,InnerYCount)<CurrentHadronBinContent)
			  {
			    HadronConfLevel+=pdf_hadrons->GetBinContent(InnerXCount,InnerYCount);
			  }
			if(pdf_fakes->GetBinContent(InnerXCount,InnerYCount)<CurrentFakeBinContent)
			  {
			    FakeConfLevel+=pdf_fakes->GetBinContent(InnerXCount,InnerYCount);
			  }
		      }
		  }
		//Store the computed result in the cl_histograms
		cl_leptons->SetBinContent(OuterXCount,OuterYCount,LeptonConfLevel);
		cl_hadrons->SetBinContent(OuterXCount,OuterYCount,HadronConfLevel);
		cl_fakes->SetBinContent(OuterXCount,OuterYCount,FakeConfLevel);
		
	      }
	    cout << OuterXCount << endl;
	  }
      }        
    OutputFile->Write();
    OutputFile->Close();
  }

  leaf1VsLeaf2GoodClone->Add(leaf1VsLeaf2SingleClone, leaf1VsLeaf2DoubleClone,1.0,1.0);
  leaf1VsLeaf2Good->Add(leaf1VsLeaf2Single, leaf1VsLeaf2Double,1.0,1.0);
  
  if(computeProbability){
    
    //Make the strings for the histograms showing the conditional probabilities
    TString* probHistogramNameSingle = new TString("P(Single|");
    TString* probHistogramNameDouble = new TString("P(Double|");
    TString* probHistogramNameGood = new TString("P(Good|");
    TString* probHistogramNameFake = new TString("P(Fake|");
    TString* probHistogramNameHadron = new TString("P(Hadron|");
    
    probHistogramNameSingle->Append(leaf1);
    probHistogramNameSingle->Append("&");
    probHistogramNameSingle->Append(leaf2);
    probHistogramNameSingle->Append(")");

    probHistogramNameDouble->Append(leaf1);
    probHistogramNameDouble->Append("&");
    probHistogramNameDouble->Append(leaf2);
    probHistogramNameDouble->Append(")");

    probHistogramNameGood->Append(leaf1);
    probHistogramNameGood->Append("&");
    probHistogramNameGood->Append(leaf2);
    probHistogramNameGood->Append(")");

    probHistogramNameFake->Append(leaf1);
    probHistogramNameFake->Append("&");
    probHistogramNameFake->Append(leaf2);
    probHistogramNameFake->Append(")");

    probHistogramNameHadron->Append(leaf1);
    probHistogramNameHadron->Append("&");
    probHistogramNameHadron->Append(leaf2);
    probHistogramNameHadron->Append(")");



    TH2F* ProbSingleLeaf1Leaf2= new TH2F(probHistogramNameSingle->Data(),probHistogramNameSingle->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);

    TH2F* ProbDoubleLeaf1Leaf2= new TH2F(probHistogramNameDouble->Data(),probHistogramNameDouble->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);

    TH2F* ProbGoodLeaf1Leaf2= new TH2F(probHistogramNameGood->Data(),probHistogramNameGood->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);

    TH2F* ProbFakeLeaf1Leaf2= new TH2F(probHistogramNameFake->Data(),probHistogramNameFake->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
    
    TH2F* ProbHadronLeaf1Leaf2= new TH2F(probHistogramNameHadron->Data(),probHistogramNameHadron->Data(),nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
    
    Float_t yield_single = 1.0;
    Float_t yield_double = 1.0;
    Float_t yield_fake = 1.0;
    Float_t yield_hadron = 1.0;
    
    //Create the sum histogram
    TH2F* aux = new TH2F("aux","aux",nHB1,lHB1,uHB1,nHB2,lHB2,uHB2);
    aux->Add(leaf1VsLeaf2SingleClone,leaf1VsLeaf2DoubleClone,yield_single,yield_double);
    aux->Add(leaf1VsLeaf2FakeClone, yield_fake);

    //We add also the hadrons to compute the relative probabilities for fakes and hadrons independently
    aux->Add(leaf1VsLeaf2HadronClone, yield_hadron);

    //Compute fake probability
    ProbFakeLeaf1Leaf2->Divide(leaf1VsLeaf2FakeClone,aux);

    //Compute hadron probability
    ProbHadronLeaf1Leaf2->Divide(leaf1VsLeaf2HadronClone,aux);
    
    //Compute single and double probability
    ProbSingleLeaf1Leaf2->Divide(leaf1VsLeaf2SingleClone,aux);    

    ProbDoubleLeaf1Leaf2->Divide(leaf1VsLeaf2DoubleClone,aux);

    //Compute good probability
    ProbGoodLeaf1Leaf2->Divide(leaf1VsLeaf2GoodClone,aux);




    TString* ProbCanvasName = new TString("P(single, double, fake, hadron|");
    ProbCanvasName->Append(leaf1);
    ProbCanvasName->Append("&");
    ProbCanvasName->Append(leaf2);
    ProbCanvasName->Append(")");
    TCanvas* ProbLeaf1Leaf2Canvas = new TCanvas(ProbCanvasName->Data(),ProbCanvasName->Data());

    ProbLeaf1Leaf2Canvas->Divide(2,2);
    ProbLeaf1Leaf2Canvas->cd(1);
    ProbSingleLeaf1Leaf2->Draw("COLZ");
    ProbLeaf1Leaf2Canvas->cd(2);
    ProbDoubleLeaf1Leaf2->Draw("COLZ");
    ProbLeaf1Leaf2Canvas->cd(3);
    ProbGoodLeaf1Leaf2->Draw("COLZ");
    ProbLeaf1Leaf2Canvas->cd(4);
    ProbFakeLeaf1Leaf2->Draw("COLZ");

    }
  
  
  if(testrejection)
    {
      Int_t nSurvivingFakes = 0;
      Int_t nSurvivingHadrons = 0;

      Float_t fracSurvivingFakes = 0.0;
      Float_t fracSurvivingHadrons = 0.0;

      Int_t nRejectedGoodSingle = 0;
      Float_t fracRejectedGoodSingle = 0.0;

      Int_t nRejectedGoodDouble = 0;
      Float_t fracRejectedGoodDouble = 0.0;

      Float_t centroid;
      Float_t single1, single2;
      Float_t double1, double2;
      Float_t fake1, fake2;
      Float_t hadron1, hadron2;
     
      Int_t nSingleRings = SingleNtuple->GetEventList()->GetN();
      Int_t nDoubleRings = DoubleNtuple->GetEventList()->GetN();
      Int_t nFakeRings = FakeNtuple->GetEventList()->GetN();
      Int_t nHadronRings = HadronNtuple->GetEventList()->GetN();

      Int_t halfNSingles=(Int_t)(nSingleRings/2);
      Int_t halfNDoubles=(Int_t)(nDoubleRings/2);
      Int_t halfNFakes=(Int_t)(nFakeRings/2);
      Int_t halfNHadrons=(Int_t)(nHadronRings/2);

      
      cout << "Number of single rings: " << nSingleRings << endl;
      cout << "Number of double rings: " << nDoubleRings << endl;
      cout << "Number of fake rings:   " << nFakeRings << endl;
      cout << "Number of Hadron rings:   " << nHadronRings << endl;
      
      SingleNtuple->SetBranchAddress(leafName1String->Data(),&single1);
      SingleNtuple->SetBranchAddress(leafName2String->Data(),&single2);
      DoubleNtuple->SetBranchAddress(leafName1String->Data(),&double1);
      DoubleNtuple->SetBranchAddress(leafName2String->Data(),&double2);
      FakeNtuple->SetBranchAddress(leafName1String->Data(),&fake1);
      FakeNtuple->SetBranchAddress(leafName2String->Data(),&fake2);
      HadronNtuple->SetBranchAddress(leafName1String->Data(),&hadron1);
      HadronNtuple->SetBranchAddress(leafName2String->Data(),&hadron2);



      //A last check!
      FakeNtuple->SetBranchAddress("f_rich_centroid",&centroid);
      DoubleNtuple->SetBranchAddress("f_rich_centroid",&centroid);
      SingleNtuple->SetBranchAddress("f_rich_centroid",&centroid);
      HadronNtuple->SetBranchAddress("f_rich_centroid",&centroid);

      for(Int_t i = halfNSingles; i<nSingleRings;i++)
	{
	  SingleNtuple->GetEntry(SingleNtuple->GetEventList()->GetEntry(i));
	  if (
	      (centroid<0.0 && centroidSelection>0) ||
	      (centroid>=0.0 && centroidSelection<0)
	      )
	    {
	      cout << "Sipping Single..." << endl;
	      continue;
	    }
	  Int_t binNumber = ProbSingleLeaf1Leaf2->FindBin(single1,single2);
	  if((
	      ProbGoodLeaf1Leaf2->GetBinContent(binNumber)<
	      ProbFakeLeaf1Leaf2->GetBinContent(binNumber)) ||
	     (ProbGoodLeaf1Leaf2->GetBinContent(binNumber)<
	      ProbHadronLeaf1Leaf2->GetBinContent(binNumber)) ) //good probability is NOT the maximum probability!
	    {
	      nRejectedGoodSingle++;
	    }
	  //singles->Fill(single1,single2);
	}

      fracRejectedGoodSingle = (float)nRejectedGoodSingle/(float)halfNSingles;
      cout << "Number of surviving good single rings: " << halfNSingles-nRejectedGoodSingle << endl;
      cout << "Fraction of rejected good (single) rings: " << fracRejectedGoodSingle << endl;
      
      for(Int_t i = halfNDoubles; i<nDoubleRings;i++)
	{
	  DoubleNtuple->GetEntry(DoubleNtuple->GetEventList()->GetEntry(i));
	  if (
	      (centroid<0.0 && centroidSelection>0) ||
	      (centroid>=0.0 && centroidSelection<0)
	      )
	    {
	      cout << "Skipping Double..."<< endl;
	      continue;
	    }
	  Int_t binNumber = ProbDoubleLeaf1Leaf2->FindBin(double1,double2);
	  if((
	      ProbGoodLeaf1Leaf2->GetBinContent(binNumber)<
	      ProbFakeLeaf1Leaf2->GetBinContent(binNumber)) ||
	     (ProbGoodLeaf1Leaf2->GetBinContent(binNumber)<
	      ProbHadronLeaf1Leaf2->GetBinContent(binNumber)) ) //good probability is NOT the maximum probability!
	    {
	      nRejectedGoodDouble++;
	    }
	  //doubles->Fill(double1,double2);
	}
      fracRejectedGoodDouble = (float)nRejectedGoodDouble/(float)halfNDoubles;
      cout << "Number of surviving good doublerings: " << halfNDoubles-nRejectedGoodDouble << endl;
      cout << "Fraction of rejected good (double) rings: " << fracRejectedGoodDouble << endl;
      


      for(Int_t i = halfNFakes; i<nFakeRings;i++)
	{
	  FakeNtuple->GetEntry(FakeNtuple->GetEventList()->GetEntry(i));
	  if (
	      (centroid<0.0 && centroidSelection>0) ||
	      (centroid>=0.0 && centroidSelection<0)
	      )
	    {
	      cout << "Skipping..."<< endl;
	      continue;
	    }
	  
	  Int_t binNumber = ProbFakeLeaf1Leaf2->FindBin(fake1,fake2);
	  if((
	      ProbGoodLeaf1Leaf2->GetBinContent(binNumber)>
	      ProbFakeLeaf1Leaf2->GetBinContent(binNumber)) &&
	     (ProbGoodLeaf1Leaf2->GetBinContent(binNumber)>
	      ProbHadronLeaf1Leaf2->GetBinContent(binNumber)) ) 
	    //good probability is BIGGER than hadron and fake prob. - this will survive!
	    {
	      nSurvivingFakes++;
	    }

	}
      fracSurvivingFakes = (float)nSurvivingFakes/(float)halfNFakes;
      cout << "Number of surviving fakes: " << nSurvivingFakes<< endl;
      cout << "Fraction of surviving fake rings: " << fracSurvivingFakes << endl;      

      for(Int_t i = halfNHadrons; i<nHadronRings;i++)
	{
	  HadronNtuple->GetEntry(HadronNtuple->GetEventList()->GetEntry(i));
	  if (
	      (centroid<0.0 && centroidSelection>0) ||
	      (centroid>=0.0 && centroidSelection<0)
	      )
	    {
	      cout << "Skipping..."<< endl;
	      continue;
	    }
	  Int_t binNumber = ProbHadronLeaf1Leaf2->FindBin(hadron1,hadron2);
	  if((
	      ProbGoodLeaf1Leaf2->GetBinContent(binNumber)>
	      ProbFakeLeaf1Leaf2->GetBinContent(binNumber)) &&
	     (ProbGoodLeaf1Leaf2->GetBinContent(binNumber)>
	      ProbHadronLeaf1Leaf2->GetBinContent(binNumber)) ) 
	    //good probability is BIGGER than hadron and fake prob. - this will survive!
	    {
	      nSurvivingHadrons++;
	    }

	}
      fracSurvivingHadrons = (float)nSurvivingHadrons/(float)halfNHadrons;
      cout << "Number of surviving hadrons: " << nSurvivingHadrons<< endl;
      cout << "Fraction of surviving hadron rings: " << fracSurvivingHadrons << endl;

      
      Float_t finalNumberOfRings = fracSurvivingFakes*halfNFakes + (1.0-fracRejectedGoodSingle)*halfNSingles +
	(1.0-fracRejectedGoodDouble)*halfNDoubles + fracSurvivingHadrons*halfNHadrons;
      Float_t finalContamination = (fracSurvivingFakes*halfNFakes + fracSurvivingHadrons*halfNHadrons) / finalNumberOfRings;
      cout << "Final Contamination of the sample with hadron and fake rings: " << finalContamination << endl;
      
    }
}







