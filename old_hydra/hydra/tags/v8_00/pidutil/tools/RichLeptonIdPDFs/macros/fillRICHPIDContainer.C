//@(#)$Id: fillRICHPIDContainer.C,v 1.1 2005-05-13 08:57:42 christ Exp $
//The macro reads hitogram from files, puts them into containers
//and stores in the RTDB for all available PID algorithms
// *** Modified: 25/04/2003
// Fake ids changed to -1 (fake-) and -2 (fake+)
// In the input files the corresponding histograms should have the "old" names
// 500 for fake- and 501 for fake+

//Here we copy the components of the RICH pattern - eigenvectors required by the
//ring recognition algorithm for 4 and 5 D transformation into the eigenspace:
//These numbers are the result from the PCAs.
//5D:
/*
static Double_t g5DSigmaValuesSEC0[] = {
0,0,0,0,0};
static Double_t g5DMeanValuesSEC0[] = {
0,0,0,0,0};
static Double_t g5DEigenVectorsSEC0[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


static Double_t g5DSigmaValuesSEC1[] = {
0,0,0,0,0};
static Double_t g5DMeanValuesSEC1[] = {
0,0,0,0,0};
static Double_t g5DEigenVectorsSEC1[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g5DSigmaValuesSEC2[] = {
0,0,0,0,0};
static Double_t g5DMeanValuesSEC2[] = {
0,0,0,0,0};
static Double_t g5DEigenVectorsSEC2[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g5DSigmaValuesSEC3[] = {
0,0,0,0,0};
static Double_t g5DMeanValuesSEC3[] = {
0,0,0,0,0};
static Double_t g5DEigenVectorsSEC3[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


static Double_t g5DSigmaValuesSEC4[] = {
0,0,0,0,0};
static Double_t g5DMeanValuesSEC4[] = {
0,0,0,0,0};
static Double_t g5DEigenVectorsSEC4[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g5DSigmaValuesSEC5[] = {
0,0,0,0,0};
static Double_t g5DMeanValuesSEC5[] = {
0,0,0,0,0};
static Double_t g5DEigenVectorsSEC5[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};


//4D:
static Double_t g4DMeanValuesSEC0[] = {
,0,0,0,0};
static Double_t g4DSigmaValuesSEC0[] = {
,0,0,0,0};
static Double_t g4DEigenVectorsSEC0[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g4DMeanValuesSEC1[] = {
0,0,0,0};
static Double_t g4DSigmaValuesSEC1[] = {
0,0,0,0};
static Double_t g4DEigenVectorsSEC1[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g4DMeanValuesSEC2[] = {
0,0,0,0};
static Double_t g4DSigmaValuesSEC2[] = {
0,0,0,0};
static Double_t g4DEigenVectorsSEC2[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g4DMeanValuesSEC3[] = {
0,0,0,0};
static Double_t g4DSigmaValuesSEC3[] = {
0,0,0,0};
static Double_t g4DEigenVectorsSEC3[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g4DMeanValuesSEC4[] = {
0,0,0,0};
static Double_t g4DSigmaValuesSEC4[] = {
0,0,0,0};
static Double_t g4DEigenVectorsSEC4[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

static Double_t g4DMeanValuesSEC5[] = {
0,0,0,0};
static Double_t g4DSigmaValuesSEC5[] = {
0,0,0,0};
static Double_t g4DEigenVectorsSEC5[] = {
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

*/

// -----------------------------------------------------------------------------

Int_t  aIds[] = { 2, 3, 8, 9, 11, 14, 45, -1, -2 };

void fillRICHPIDContainer(Char_t* pdfInputFile, Char_t* normalizationInputFile, Char_t* sOutputFile, Int_t refRunId)
{


  

  Hades myHades;
  HRuntimeDb *rtdb = gHades->getRuntimeDb();
  
  // -------------------------------------------------------------------------
  // HPidRingPropertiesPar

  HParRootFileIo *in = new HParRootFileIo;
  in->open("~/hades/projects/current/PDFParameters/PDF-creation/macros/MakeContainers/EmptyRichPIDParameters.root");
  rtdb->setFirstInput(in);

  HPidAlgRingPropertiesPar *pRingProperties
    = (HPidAlgRingPropertiesPar *) rtdb->getContainer("PidAlgRingPropertiesPar");
  
  HParRootFileIo *out = new HParRootFileIo;
  
  out->open(sOutputFile, "RECREATE");
  rtdb->setOutput(out);
  
  rtdb->initContainers(refRunId);

  TFile* rich_pdfs= new TFile(pdfInputFile,"read");
  addRichPdfs(pRingProperties);
  cout << "added ring pdfs" << endl;

  TFile* rich_normalization= new TFile(normalizationInputFile,"read");
  addRichNormalization(pRingProperties);
  
  pRingProperties->print();
  
  // -------------------------------------------------------------------------
  // write the parameters to RTDB output
 
  pRingProperties->setChanged();
  //rtdb->setContainersStatic();
  
  /*  for(Int_t i = 0; i < sizeof(iRunIds) / sizeof(Int_t); i++)
      rtdb->initContainers(iRunIds[i],3);*/
  
  rtdb->print();
  
  //rtdb->saveOutput();
  //rtdb->closeOutput();
  //delete gHades;
}

// -----------------------------------------------------------------------------

void addRichNormalization(HPidAlgRingPropertiesPar *pRingProperties)
{
  Int_t offset=0;
  char s[100];
  TH1* pH;
  char secchar[2];

  const TString MomDistFakeBase("MomDistFakeRatio_sector_");
  const TString MomDistLepBase("MomDistLepRatio_sector_");
  const TString MomDistProtonBase("MomDistProtonRatio_sector_");
  const TString MomDistPiPlusBase("MomDistPiPlusRatio_sector_");
  const TString MomDistPiMinusBase("MomDistPiMinusRatio_sector_");

  for(Int_t sector=0;sector<6;sector++)
    {
      sprintf(secchar,"%d",sector);
      TString* MomDistFakeRatioString=new TString(MomDistFakeBase);
      MomDistFakeRatioString->Append(secchar);
      TString* MomDistLepRatioString=new TString(MomDistLepBase);
      MomDistLepRatioString->Append(secchar);
      TString* MomDistProtonRatioString=new TString(MomDistProtonBase);
      MomDistProtonRatioString->Append(secchar);
      TString* MomDistPiPlusRatioString=new TString(MomDistPiPlusBase);
      MomDistPiPlusRatioString->Append(secchar);
      TString* MomDistPiMinusRatioString=new TString(MomDistPiMinusBase);
      MomDistPiMinusRatioString->Append(secchar);

      for(Int_t i = 0; i < sizeof(aIds) / sizeof(Int_t); i++) //loop over all species
	{
	  
	  if(aIds[i]<=3 && aIds[i]>1) //normalization for electrons/positrons
	    {
	      cout << "Searching for: " << MomDistLepRatioString->Data() << endl;
	      pH =	(TH1*)gDirectory->Get(MomDistLepRatioString->Data());
	      cout << pH << endl;
	      pRingProperties->addHistogram(aIds[i],6,sector, pH);
	    }
	  if(aIds[i]==8)
	    {
	      pH =	(TH1*)gDirectory->Get(MomDistPiPlusRatioString->Data());
	      cout << pH << endl;
	      pRingProperties->addHistogram(aIds[i],6,sector, pH);
	    }
	  if(aIds[i]==9)
	    {
	      pH =	(TH1*)gDirectory->Get(MomDistPiMinusRatioString->Data());
	      cout << pH << endl;
	      pRingProperties->addHistogram(aIds[i],6,sector, pH);
	    }
	  if(aIds[i]>9) //protons!
	    {
	      pH =	(TH1*)gDirectory->Get(MomDistProtonRatioString->Data());
	      cout << pH << endl;
	      pRingProperties->addHistogram(aIds[i],6,sector, pH);
	    }
	  if(aIds[i]<0) //fakes 
	    {
	      pH =	(TH1*)gDirectory->Get(MomDistFakeRatioString->Data());
	      cout << pH << endl;
	      pRingProperties->addHistogram(aIds[i],6,sector, pH);
	    }
	}

    }
}

void addRichPdfs(HPidAlgRingPropertiesPar *pRingProperties)
{
  cout << "starting to add RICH stuff" << endl;
  Int_t i, anglecount;
  Int_t lowerangle, upperangle;

  char  s[100];
  TH1  *pH;
  Int_t offset=0;
  char thetacutstring[50];
  TString* anglecutstring=0;

  char sectorchar[16];
  TString* sectorstring=0;


  //loop over all sectors
  for(Int_t isector=0;isector<6;isector++)
    {
      sprintf(sectorchar,"%s%d","_sector_",isector);
      sectorstring = new TString(sectorchar);

      //loop over all species 
      for(i = 0; i < sizeof(aIds) / sizeof(Int_t); i++)
	{
	  cout << "PID is : " << aIds[i] << endl;
	  
	  if(aIds[i]<=3 && aIds[i]>1) //we are creating pdfs for electrons and positrons!
	    {
	      for(anglecount=0;anglecount<3;anglecount++)
		{
		  lowerangle = 10*(2*anglecount+1); 
		  upperangle=lowerangle+20;
		  if(lowerangle==50) 
		    {
		      upperangle+=20;
		    }
		  sprintf(thetacutstring,"%s%d%s%d","_",lowerangle,"_to_",upperangle);
		  anglecutstring = new TString(thetacutstring);

		  sprintf(s,"%s%s%s","pdf_leptons_with_centroid",anglecutstring->Data(),sectorstring->Data());
		  cout << "searching for " << s<< endl;
		  offset=anglecount;
		  pH = (TH1 *) gDirectory->Get(s);
		  cout << pH;
		  pRingProperties->addHistogram(aIds[i],offset, isector,pH);
		  cout <<" adding a histogram at offset" << offset << endl;

		  sprintf(s,"%s%s%s","pdf_leptons_no_centroid",anglecutstring->Data(),sectorstring->Data());
		  cout << "searching for " << s<< endl;
		  offset=3+anglecount;
		  pH = (TH1 *) gDirectory->Get(s);
		  cout << pH;
		  pRingProperties->addHistogram(aIds[i],offset,isector,pH);
		  delete anglecutstring;
		}
	      cout << "done with PID: " << aIds[i] << endl;
	    }
	  
	  if(aIds[i]>3) //hadrons!
	    {
	      for(anglecount = 0; anglecount<3;anglecount++)
		{
		  lowerangle = 10*(2*anglecount+1); 
		  upperangle=lowerangle+20;
		  if(lowerangle==50)
		    {
		      upperangle+=20;
		    }

		  sprintf(thetacutstring,"%s%d%s%d","_",lowerangle,"_to_",upperangle);
		  anglecutstring = new TString(thetacutstring);
		  
		  sprintf(s,"%s%s%s","pdf_hadrons_with_centroid",anglecutstring->Data(),sectorstring->Data());
		  cout << "searching for " << s<< endl;
		  offset=anglecount;
		  pH = (TH1 *) gDirectory->Get(s);
		  cout << pH;
		  pRingProperties->addHistogram(aIds[i],offset,isector,pH);

		  sprintf(s,"%s%s%s","pdf_hadrons_no_centroid",anglecutstring->Data(),sectorstring->Data());
		  cout << "searching for " << s<< endl;
		  offset=3+anglecount;
		  pH = (TH1 *) gDirectory->Get(s);
		  pRingProperties->addHistogram(aIds[i],offset,isector, pH);
		  delete anglecutstring;
		}
	    }
	  
	  if(aIds[i]<0) //fakes!
	    {
	      for(anglecount = 0; anglecount<3;anglecount++)
		{
		  lowerangle = 10*(2*anglecount+1); 
		  upperangle=lowerangle+20;
		  if(lowerangle==50)
		    {
		      upperangle+=20;
		    }

		  sprintf(thetacutstring,"%s%d%s%d","_",lowerangle,"_to_",upperangle);
		  anglecutstring = new TString(thetacutstring);
		  sprintf(s,"%s%s%s","pdf_fakes_with_centroid",anglecutstring->Data(),sectorstring->Data());
		  cout << "searching for " << s<< endl;
		  offset=anglecount;
		  pH = (TH1 *) gDirectory->Get(s);
		  cout << pH;
		  pRingProperties->addHistogram(aIds[i],offset,isector,pH);

		  sprintf(s,"%s%s%s","pdf_fakes_no_centroid",anglecutstring->Data(),sectorstring->Data());
		  cout << "searching for " << s<< endl;
		  offset=3+anglecount;
		  pH = (TH1 *) gDirectory->Get(s);
		  pRingProperties->addHistogram(aIds[i],offset,isector, pH);
		  delete anglecutstring;
		}
	    }
	}
    }
}
