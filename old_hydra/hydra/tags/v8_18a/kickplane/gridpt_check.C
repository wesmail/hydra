//Provides conveniencie functions to check momentum parameterization both
//on a global and bin per bin basis

TNtuple *tuple;
TCanvas *canvas;

TF1 *pkick;

void plot_tails(Float_t tailcut=0.1,Float_t minp=100.) {
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

  sprintf(cond,"abs((p-pc)/p)>%.4f && p>%.1f && abs(x)<2",tailcut,minp);
  tuple->Draw("the:abs(phi)>>tails",cond,"goff");

  sprintf(cond,"p>%.1f && abs(x)<2",minp);
  tuple->Draw("the:abs(phi)>>multip",cond,"goffcolz");

  histo->Divide(mul);
  histo->Draw("colz");
}

void plot_bin(Float_t theta,Float_t phi,Float_t gran=0.25) {
  Text_t cond[256];
  TH2F *hxf = 0;
  TH2F *hxfc = 0;
  canvas->Clear();

  canvas->Divide(2,2);

  sprintf(cond,"abs(the-%.2f)<%.2f && abs(abs(phi)-%.2f)<%.2f && p>100 && abs(x)<2",
	  theta,gran,phi,gran);
  tuple->Draw("p:x>>hxf",cond,"goff");
  tuple->Draw("pc:x>>hxfc",cond,"goff");

  hxf = (TH2F *)gROOT->FindObject("hxf");
  hxfc = (TH2F *)gROOT->FindObject("hxfc");

  canvas->cd(1);
  hxfc->SetMarkerColor(kRed);
  hxfc->SetMarkerStyle(kCircle);
  hxf->SetMarkerStyle(kOpenSquare);
  hxf->Draw();
  hxfc->Draw("same");

  canvas->cd(2);
  tuple->Draw("p:x>>pxf",cond,"prof");

  canvas->cd(3);
  tuple->Draw("pc:x>>pxfc",cond,"prof"); 

  canvas->cd(4);
  sprintf(cond,"%s && abs((p-pc)/p)<1",cond);
  tuple->Draw("(p-pc)/p:x>>residual",cond,"prof");
}

void scan_bin(Float_t theta,Float_t phi,Float_t gran=0.25) {
  Text_t cond[256];
  sprintf(cond,"abs(the-%.2f)<%.2f && abs(abs(phi)-%.2f)<%.2f && p>100",
	  theta,gran,phi,gran);
  tuple->Scan("a:b:c",cond);
}

void gridpt_check(void) {
  TFile *f = new TFile("res.root");
  tuple = (TNtuple *)gFile->Get("Res");

  canvas = new TCanvas("check","check",800,600);

  pkick = new TF1("pkick","[0]/x + [1]*x + [2]");
  pkick->SetParameter(0,10);
  pkick->SetParameter(1,1);
  pkick->SetParameter(2,1);
  TF1 *pkicks = new TF1("pkicksimple","[0]/x + [1]*2*x*sqrt(1-x) + [2]");
  pkick->SetParameter(0,10);
  pkick->SetParameter(1,1);
  pkick->SetParameter(2,1);
}
