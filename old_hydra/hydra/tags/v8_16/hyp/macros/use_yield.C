{
#include "HistoYield.C"

  gStyle->SetPalette(8,0);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetFrameBorderMode(0);

  gStyle->SetOptStat(0);
  //gStyle->SetOptFit(11);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);
  gStyle->SetPadBorderMode(0);
  gStyle->SetPadBorderSize(0);
  gStyle->SetPadLeftMargin(0.2);
  gStyle->SetPadBottomMargin(0.2);
  gStyle->SetPadRightMargin(0.2);
  gStyle->SetPadTopMargin(0.2);

  gStyle->SetCanvasBorderMode(0);
  gStyle->SetCanvasBorderSize(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
      
Int_t nicefig=0;
//nicefig=1;
//nicefig=2; //one fig

float nicescale=1;



Int_t version =0; //eta hadronic
//version =1; //eta ang in dalitz
//version =2; //eta ang decay in dalitz
//version =10; //pp hadronic

TFile * dat;
if ((version ==0) || (version == 10))
  dat = new TFile("out.root");
else
dat = new TFile("dalitz_out.root");

Int_t rebin =4;

if (version == 2){
  modth3a->RebinY(rebin);datath3a->RebinY(rebin);simth3a->RebinY(rebin);
}

HistoYield* bla;
if (version ==0) {
  bla = new HistoYield(simth3,datath3,modth3);
}
else if (version ==1) {
  bla = new HistoYield(modth3,datath3);
}
else if (version ==2){
  bla = new HistoYield(simth3a,datath3a,modth3a);
}
else if (version ==10){
  bla = new HistoYield(simth3a,datath3a,modth3a);
}

if ((version ==0) || (version == 10)) {
  if (!nicefig)
    bla->Project(10,1,5,1,1);
  else if (nicefig == 1)
    bla->Project(4,1,2,1);
  else if (nicefig == 2)
    bla->Project(1,1,1,1);
}
else if (version ==1) {
  if (!nicefig)
    bla->Project(4,1,2,1);
  else if (nicefig == 1)
    bla->Project(4,1,2,1);
  else if (nicefig == 2)
    bla->Project(1,1,1,1);
} 
else if (version ==2) {
  bla->Project(4,1,2,1);
  //bla->Project(1,1,1,1);
} 
else
bla->Project(4,1,2,1);
gSpaceMin=220000;  
gSpaceMax=380000; 

//if (version == 1) 
//{gSpaceMin=250000;gSpaceMax=340000;}

if ((version ==0 ) || (version == 10)) {
  //bla->Fit(200000,600000,100000,800000,100000000,5,0,2);
  if (!nicefig)
    bla->Fit(150000,600000,100000,800000,100000000,7,0,1);
  else if (nicefig==1)
    bla->Fit(170000,550000,100000,800000,100000000,7,0,1);
  else if (nicefig==2)
    bla->Fit(200000,550000,100000,800000,100000000,7,0,1);
}
else if (version ==1){
  bla->Fit(100000,450000,100000,800000,100000000,2,0,1);
  //bla->Fit(100000,450000,100000,800000,100000000,2,0,2);
} else {
  bla->Fit(200000,400000,100000,800000,100000000,2,0,1);
}

bla->Display(1,0);

TH1D * h1 = bla->histo_yield1;
TH1D * h2 = bla->histo_corr1;

if (version == 10)
  h1->SetXTitle("cos(#theta_{p}^{pp})");
else if (version == 2)
  h1->SetXTitle("cos(#alpha)");
else
h1->SetXTitle("cos(#theta_{#eta}^{CM})");

  h1->SetYTitle("2#pi*d#sigma/d#Omega [arb.u.]");
  h1->SetTitleOffset(1.1);
  h1->SetTitleSize(0.06);
  h1->GetYaxis()->SetTitleOffset(1.1);
  h1->GetYaxis()->SetTitleSize(0.06);
  h1->SetMarkerColor(2);
  h1->SetMarkerStyle(20);
  h1->SetMarkerSize(2);
  h1->SetLineColor(2);
  h1->SetLineWidth(2);

 //now the label
  Float_t y_pos = 0.06;
  Float_t x_pos = -0.4;
  Float_t posdiff = 0.015;
  Float_t y_postext = -0.002;
  Float_t x_postext = 0.1;
Float_t diffline = 0.1;

if (version == 1) {
  y_pos = 0.1;
  posdiff = 0.04;
}
if (version == 2) {
  x_pos = 0.5;
  posdiff = 0.02;
  y_pos = 0.4;
}

if (!nicefig) {
  
  c5.cd();
  
  Float_t *dum = 0;
  TGraph *graph = new TGraph(1,dum,dum);
  graph->SetMarkerColor(2);
  graph->SetMarkerStyle(20);
  graph->SetMarkerSize(2);
  graph->SetPoint(0,x_pos,y_pos);
  graph->Draw("LP");
  
  TText * hello = new TText(x_pos+x_postext,y_pos+y_postext,"exp");
  hello->Draw();
  
  if (version == 1) {
    TText * hello1 = new TText(x_pos+x_postext,y_pos-posdiff+y_postext,"N*(1535), anisotrop.");
    hello1->Draw();
  }
  else if (version == 2) {
    TText * hello1 = new TText(x_pos+x_postext,y_pos-posdiff+y_postext,"isotrop.");
    hello1->Draw();
    TText * hello2 = new TText(x_pos+x_postext,y_pos-posdiff*2+y_postext,"anisotrop.");
    hello2->Draw();
  } else {
    TText * hello1 = new TText(x_pos+x_postext,y_pos-posdiff+y_postext,"N*(1535), isotrop.");
    hello1->Draw();
    TText * hello2 = new TText(x_pos+x_postext,y_pos-posdiff*2+y_postext,"N*(1535), anisotrop.");
    hello2->Draw();
    TText * hello3 = new TText(-.8,y_pos-posdiff*3+y_postext,"not corrected for acceptance");
    hello3->Draw();
    TText * hello4 = new TText(-.8,y_pos-posdiff*3.5+y_postext,"and reconstruction efficiency");
    hello4->Draw();
  }

  TLine line(x_pos-diffline,y_pos-posdiff,x_pos+diffline,y_pos-posdiff);
  line.SetLineWidth(2);
  line.Draw("same");
  


  if (version !=1) {
  TLine line2(x_pos-diffline,y_pos-posdiff*2,x_pos+diffline,y_pos-posdiff*2);
  line2.SetLineWidth(2);
  line2.SetLineStyle(2);
  line2.Draw("same");
  }
  
  c7.cd();
  TF1 * legendre = new TF1("legendre",draw_legendre,-1,1,2);

  if (version == 2)
    h2->Fit("legendre","B","",0,1);
  else
    h2->Fit("legendre","B","",-1,1);
  print_percent(legendre,2);

  if (version == 2)
    h2->SetXTitle("cos(#alpha)");
  else
    h2->SetXTitle("cos(#theta_{#eta}^{CM})");


  h2->SetYTitle("Normalized Yields");
  h2->SetTitleOffset(1.1);
  h2->SetTitleSize(0.06);
  h2->GetYaxis()->SetTitleOffset(1.1);
  h2->GetYaxis()->SetTitleSize(0.06);
  h2->SetMarkerColor(2);
  h2->SetMarkerStyle(20);
  h2->SetMarkerSize(2);
  h2->SetLineColor(2);
  h2->SetLineWidth(2);


}

if (nicefig == 1) {

  cx00->SetFillColor(0);
  cx00->Draw();
  for (int i=1;i<5;i++)
    {
      cx00->cd(i);
      gPad->SetBorderMode(0);
      gPad->SetBorderSize(0);
      gPad->SetTickx(1);
      gPad->SetTicky(1);
    }

  TH2F * data_00 = ((TH2F*)gROOT->FindObject("data_00"));
  TH2F * data_10 = ((TH2F*)gROOT->FindObject("data_10"));
  TH2F * data_20 = ((TH2F*)gROOT->FindObject("data_20"));
  TH2F * data_30 = ((TH2F*)gROOT->FindObject("data_30"));


  //data_00->Scale(nicescale);data_10->Scale(nicescale);data_20->Scale(nicescale);data_30->Scale(nicescale);
  ScaleAll(data_00,nicescale,1);ScaleAll(data_10,nicescale,1);ScaleAll(data_20,nicescale,1);ScaleAll(data_30,nicescale,1);

  //Stat_t setmax=10000;


  Stat_t setmax=2000*nicescale;
  data_00->SetMaximum(setmax);
  data_00->SetMinimum(0.001);
  data_00->GetYaxis()->SetTitleOffset(1.05);
  data_00->GetYaxis()->SetTitleSize(0.1);
  data_00->GetYaxis()->SetTitleFont(42);
  data_00->GetYaxis()->SetLabelSize(0.08);
  data_00->GetYaxis()->SetNdivisions(505);
  data_00->GetXaxis()->SetNdivisions(505);
  data_00->SetYTitle("yield [arb. units]");

  data_30->SetMaximum(setmax);
  data_30->GetXaxis()->SetTitleOffset(0.95);
  data_30->GetXaxis()->SetTitleSize(0.1);
  data_30->GetXaxis()->SetTitleFont(42);
  data_30->GetXaxis()->SetLabelSize(0.08);
  data_30->GetXaxis()->SetNdivisions(505);
  data_30->GetYaxis()->SetNdivisions(505);
  data_30->SetXTitle("(M_{pp}^{miss})^{2} [GeV^{2}/c^{4}]");

  data_20->SetXTitle(" ");
  data_00->SetXTitle(" "); data_10->SetXTitle(" ");
  data_20->GetYaxis()->SetLabelSize(0.08);
  data_20->GetXaxis()->SetLabelSize(0.08);
  data_20->GetXaxis()->SetNdivisions(505);
  data_20->GetYaxis()->SetNdivisions(505);

  data_00->GetXaxis()->SetLabelSize(0.08);
  data_10->GetXaxis()->SetNdivisions(505);
  data_10->GetYaxis()->SetNdivisions(505);
  data_10->GetXaxis()->SetLabelSize(0);
  data_10->GetYaxis()->SetLabelSize(0);


  data_30->GetYaxis()->SetLabelSize(0);


  float nice_margin=.21;

  cx00->cd(1);
  gPad->SetBottomMargin(0.0000001);
  gPad->SetLeftMargin(nice_margin);
  gPad->SetRightMargin(0.0000001);
  gPad->SetTopMargin(nice_margin);
  gPad->Draw();
  TPaveText hello0(0.22,0.66,0.95,0.77,"brNDC");
  hello0.SetFillColor(0);
  hello0.SetBorderSize(2);
  hello0.AddText("0 < cos #theta_{#eta}^{c.m.} < 0.25   ");
  hello0.Draw();

#if 0
  if (energy==3) 
    TText num(.1,.9,"c.)");
  if (energy==2) 
    TText num(.1,.9,"b.)");
  if (energy==1) 
    TText num(.1,.9,"a.)");


  num.SetNDC();
  num.SetTextSize(0.09);
  num.SetTextFont(132);
  num.Draw();

#endif

  cx00->cd(2);
  data_10->SetMaximum(setmax);
  gPad->SetBottomMargin(0.000452791);
  gPad->SetRightMargin(nice_margin);
  gPad->SetLeftMargin(0.0000001);
  gPad->SetTopMargin(nice_margin);
  gPad->Draw();   
  TPaveText hello1(0.22-.2,0.66,0.95-.2,0.77,"brNDC");
  hello1.SetFillColor(0);
  hello1.SetBorderSize(2);
  hello1.AddText("0.25 < cos #theta_{#eta}^{c.m.} < 0.5");
  hello1.Draw();
  cx00->cd(3);
  data_20->SetMaximum(setmax);
  gPad->SetTopMargin(0.000452791);
  gPad->SetRightMargin(0.0000001);
  gPad->SetLeftMargin(nice_margin);
  gPad->SetBottomMargin(nice_margin);
  gPad->Draw(); 
  TPaveText hello2(0.22,0.66+.2,0.95,0.77+.2,"brNDC");
  hello2.AddText("0.5 < cos #theta_{#eta}^{c.m.} < 0.75");
  hello2.SetFillColor(0);
  hello2.SetBorderSize(2);
  hello2.Draw();
  cx00->cd(4);
  data_30->SetMaximum(setmax);
  gPad->SetTopMargin(0.000452791);
  gPad->SetLeftMargin(0.0000001);
  gPad->SetRightMargin(nice_margin);
  gPad->SetBottomMargin(nice_margin);
  gPad->Draw();   
  TPaveText hello3(0.22-.2,0.66+.2,0.95-.2,0.77+.2,"brNDC");
  hello3.AddText("0.75 < cos #theta_{#eta}^{c.m.} < 1   ");
  hello3.SetFillColor(0);
  hello3.SetBorderSize(2);
  hello3.Draw();



  }





}
