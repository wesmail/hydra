//Checker for HKickMatchPar. That is azimuthal deflection parameters

TNtuple *tuple;
TCanvas *canvas;

void plot_tails(Float_t tailcut=0.01,Float_t minp=100) {
  TH2F *histo = (TH2F *)gROOT->FindObject("tails");
  TH2F *mul = (TH2F *)gROOT->FindObject("multip");

  if (!histo) histo = new TH2F("tails","Angular distribution of tails",
			       60,-0.25,29.75,140,14.75,84.75);
  if (!mul) mul = new TH2F("multip","Multiplicity",
			   60,-0.25,29.75,140,14.75,84.75);
  canvas->Clear();
  Text_t cond[256];

  histo->SetXTitle("abs( #varphi )");
  histo->SetYTitle("#theta");
  mul->SetXTitle("abs( #varphi )");
  mul->SetYTitle("#theta");

  sprintf(cond,"abs(xf-xfc)>%.4f && pc>%f",tailcut,minp);
  tuple->Draw("the:abs(phi)>>tails",cond,"goff");

  sprintf(cond,"pc>%f",minp);
  tuple->Draw("the:abs(phi)>>multip",cond,"goffcolz");

  histo->Divide(mul);
  histo->Draw("colz");
}

void plot_binx(Float_t theta,Float_t phi,Float_t gran=0.25) {
  Text_t cond[256];
  TH2F *hxf = 0;
  TH2F *hxfc = 0;
  canvas->Clear();

  canvas->Divide(2,2);

  sprintf(cond,"abs(the-%.2f)<%.2f && abs(abs(phi)-%.2f)<%.2f && p>100",
	  theta,gran,phi,gran);
  tuple->Draw("xf:pc>>hxxf",cond,"goff");
  tuple->Draw("xfc:pc>>hxxfc",cond,"goff");

  hxf = (TH2F *)gROOT->FindObject("hxxf");
  hxfc = (TH2F *)gROOT->FindObject("hxxfc");

  canvas->cd(1);
  hxfc->SetMarkerColor(kRed);
  hxfc->SetMarkerStyle(kCircle);
  hxf->SetMarkerStyle(kOpenSquare);
  hxf->Draw();
  hxfc->Draw("same");

  canvas->cd(2);
  tuple->Draw("xf:pc>>pxf",cond,"prof");

  canvas->cd(3);
  tuple->Draw("xfc:pc>>pxfc",cond,"prof");

  canvas->cd(4);
  tuple->Draw("xf-xfc:pc>>resix",cond,"");  
}

void plot_bin(Float_t theta,Float_t phi,Float_t gran=0.25) {
  Text_t cond[256];
  TH2F *hxf = 0;
  TH2F *hxfc = 0;
  canvas->Clear();

  canvas->Divide(2,2);

  sprintf(cond,"abs(the-%.2f)<%.2f && abs(abs(phi)-%.2f)<%.2f && p>100",
	  theta,gran,phi,gran);
  tuple->Draw("pc*xf:xf>>hxf",cond,"goff");
  tuple->Draw("pc*xfc:xf>>hxfc",cond,"goff");

  hxf = (TH2F *)gROOT->FindObject("hxf");
  hxfc = (TH2F *)gROOT->FindObject("hxfc");

  canvas->cd(1);
  hxfc->SetMarkerColor(kRed);
  hxfc->SetMarkerStyle(kCircle);
  hxf->SetMarkerStyle(kOpenSquare);
  hxf->Draw();
  hxfc->Draw("same");

  canvas->cd(2);
  tuple->Draw("pc*xf:xf>>pxf",cond,"prof");

  canvas->cd(3);
  tuple->Draw("pc*xfc:xfc>>pxfc",cond,"prof");

  canvas->cd(4);
  tuple->Draw("xf-xfc:xf>>resi",cond,"prof");  
}

void matchpar_check(void) {
  TFile *f = new TFile("resmatch.root");
  tuple = (TNtuple *)gFile->Get("Resm");

  canvas = new TCanvas("check","check",800,600);
}
