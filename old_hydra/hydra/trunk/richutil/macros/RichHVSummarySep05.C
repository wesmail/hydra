void ana(fstream &file, const TString &partitionName, const TString &start, const TString &end) {
#define EXIT_SUCCESS  0
#define EXIT_FAILURE  1
#define MAX_FILES 400

  gSystem->Load("libOraUtil.so");

  // Nominal voltages for SEP05 experiment
  const Float_t nomHV[6] = {2475, 2490, 2475, 2410, 2475, 2475};

  Int_t option = 1; // 0= all periods
                    // 1 = only hld-files
                    // 2 = apply hldfile filter

  TGraphErrors *graph = 0;
  HOraSlowChannel *chan = 0;
  Double_t xn[6][MAX_FILES];
  Double_t yn[6][MAX_FILES];
  Double_t en[6][MAX_FILES];
  Double_t *x;
  Double_t *y;
  Double_t *e;

  Char_t title[100];
  Char_t name[100]; 
  Char_t xtitle[100];
  Char_t ytitle[100];
  Int_t  counter[6];
  Int_t  nFiles = 0;
  Int_t  nData = 0;

  TH2F *histhv[6];
  TH1F *histhv1dim[6];

  TString hldFile = "";
  TString asciiFile = "summary_";
  asciiFile.Append(start);
  asciiFile.Append(".txt");
  TString rootFile = "Pictures/validation_";
  rootFile.Append(start);
  rootFile.Append(".root");
  TString epsFile = "Pictures/validation_";
  epsFile.Append(start);
  epsFile.Append(".eps");

  TFile f(rootFile.Data(), "RECREATE");

  TCanvas* canvas = new TCanvas("c1", "RICH HV", 200, 10, 1200, 800);
  canvas->Divide(3, 2);
  TCanvas *canv1 = new TCanvas("canv1", "RichHVHist", 200, 50, 1200, 800);
  canv1->Divide(3,2);
  TCanvas *canv2 = new TCanvas("canv2", "RichHVHist", 200, 90, 1200, 800);
  canv2->Divide(3, 2);

  HOraSlowManager *slowManager = new HOraSlowManager;
  HOraSlowPartition *partition = slowManager->setPartition(partitionName, start, end);
  TList &channels = slowManager->getChannels();
  char buf[80];

  for(Int_t sector = 0; sector < 6; sector++) {
    sprintf(buf,"HAD:RICH:HV:PS:VMON_C%i", sector);
    slowManager->addChannel(buf);
  }

  Bool_t rc = slowManager->readSummary();
  if (!rc) {
    delete slowManager;
    exit(EXIT_FAILURE);
  }

  slowManager->writeSummaryToAscii(asciiFile.Data(), option);

  for(Int_t sector = 0; sector < 6; sector++) {
    counter[sector] = 0;

    sprintf(title, "RichHV sector %i", sector + 1);
    sprintf(name, "richHV2Dsector%i", sector + 1);
    sprintf(xtitle, "FileNr");
    sprintf(ytitle, "HV [V]");
    histhv[sector] = new TH2F(name, title, MAX_FILES, 0, MAX_FILES, 1500, 0, 3000);
    histhv[sector]->SetXTitle(xtitle);
    histhv[sector]->SetYTitle(ytitle);

    sprintf(name, "richHVsector%i", sector + 1);
    sprintf(xtitle, "HV [V]");
    sprintf(ytitle, "Counts");
    histhv1dim[sector] = new TH1F(name, title, 1500, 0, 3000);
    histhv1dim[sector]->SetXTitle(xtitle);
    histhv1dim[sector]->SetYTitle(ytitle);

    for(Int_t i = 0; i < MAX_FILES; i++) {
      yn[sector][i] = 0;
      xn[sector][i] = 0;
      en[sector][i] = 0;
    }

    canvas->cd(sector + 1);
    chan = (HOraSlowChannel *)(channels.At(sector));

    if(chan) {
      graph = chan->getRunSumMeanSigmaGraph(option);

      if(graph) {
	graph->Draw("AP");
	nFiles = graph->GetN();
	x = graph->GetX();
	y = graph->GetY();
	e = graph->GetEY();

	for(Int_t i = 0; i < nFiles; i++) {
	  histhv[sector]->Fill(x[i], y[i]);
	  histhv1dim[sector]->Fill(y[i]);
	  yn[sector][i] = y[i];
	  xn[sector][i] = x[i];
	  en[sector][i] = e[i];
	}
      }
    }

    canv1->cd(sector + 1);
    histhv[sector]->SetMarkerColor(2);
    histhv[sector]->SetMarkerStyle(kFullDotMedium);
    histhv[sector]->DrawCopy();

    canv2->cd(sector + 1);
    histhv1dim[sector]->SetLineWidth(3);
    histhv1dim[sector]->DrawCopy();

    Int_t max = histhv1dim[sector]->GetBinCenter(histhv1dim[sector]->GetMaximumBin());
  
    cout << "==================\t" << sector + 1 << "\t================== " << endl;
    for(Int_t i = 0; i < nFiles; i++) {
      Float_t hvdiff = yn[sector][i] - nomHV[sector];
      nData = ((HOraSlowChanRunSum *) chan->getRunSumData()->At(xn[sector][i]))->getNData();

      hldFile = partition->getPeriod(i)->getFilename();
      if(hldFile.Length() > 4 && hldFile.BeginsWith("be") && yn[sector][i] > -100 
	 && (TMath::Abs(hvdiff) > 20 || nData > 5)) {
  	if((sector != 3 && ((en[sector][i] > 50 || yn[sector][i] < 2430) && nData > 2 || yn[sector][i] < 2400)) || 
	   (sector == 3 && (yn[sector][i] < 2380 || en[sector][i] > 20))) {

	  counter[sector]++;
	  chan = (HOraSlowChannel *)(channels.At(sector));

	  if(chan) {
	    cout << xn[sector][i] << "\t" << hldFile.Sizeof() 
		 <<  ".\thvdiff = " << hvdiff << "\t" << yn[sector][i]
		 << "\t+++++++  nData = " << nData << endl;
	    file << hldFile << "\tsec" << sector + 1 << endl;
	  }
	}
      }
    }
  }

// Print trip summary and write histos to file --------------------------------------
  cout << endl;
  gSystem->Exec("echo -en \"\\033[0;34m\"");
  cout << "===============================" << endl;
  cout << "Trip summary for day " << start << endl;
  cout << "===============================" << endl;
  for(Int_t sector = 0; sector < 6; sector++) {
    cout << sector + 1 << ". sector: " << counter[sector] << endl;
  }
  cout << "=============================== " << endl;
  gSystem->Exec("echo -en \"\\033[0;30m\"");

  f.cd();
  canvas->Write();
  canv1->Write();
  canv2->Write();
  f.Close();

  canvas->SaveAs(epsFile.Data());
}

void RichHVSummarySep05(void) {
  fstream file("./tmp.txt", ios::out);
  TString start = "";
  TString end = "";

  ana(file, "online", start, end);
  file.close();
}

void RichHVSummarySep05(TString partitionName = "online",
		   TString start = "", TString end = "") {
//////////////////////////////////////////////////////////////////////////////
//
// partitionName      online or experiment name as it is in oracle
// start              start date of analysis (oracle format, see below)
// end                stop date of analysis  (oracle format, see below)
//
// Dates must be specified in the format yyyy-mm-dd hh:mi:ss
//
//////////////////////////////////////////////////////////////////////////////

  TString fileName = "./tmp.txt";
  TString execStr = "";

  if(start.Length() != 0) {
    fileName = "validation_";
    fileName.Append(start.Data());
    fileName.Append(".txt.unsort");
  }

  fstream file(fileName, ios::out);

  ana(file, partitionName, start, end);
  file.close();

  cout << endl;
  gSystem->Exec("echo -en \"\\033[0;31m\"");
  cout << "=============================== " << endl;
  cout << "Sorting file: " << fileName.Data() << endl;

  execStr = "sort " + fileName + " > ";
  fileName.Remove(fileName.Length() - 7);
  execStr.Append(fileName.Data());
  cout << "Executing shell commando: \"" << execStr.Data() << "\"" << endl;
  gSystem->Exec(execStr.Data());
  gSystem->Exec("echo -en \"\\033[0;30m\"");
}
