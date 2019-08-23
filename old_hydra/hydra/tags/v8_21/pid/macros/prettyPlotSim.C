void setHistOptions(TH1F *ph1)
{
  ph1->GetXaxis()->SetTitleOffset(0.95);
  ph1->GetXaxis()->SetTitleSize(0.04);
  ph1->GetXaxis()->SetLabelSize(0.04);
  ph1->GetYaxis()->SetTitleOffset(1.2);
  ph1->GetYaxis()->SetTitleSize(0.04);
  ph1->GetYaxis()->SetLabelSize(0.04);
  //ph1->GetXaxis()->CenterTitle();
  ph1->SetLineWidth(2);
}


void setHistOptions(TH2F *ph2)
{
  ph2->GetXaxis()->SetTitleOffset(0.95);
  ph2->GetXaxis()->SetTitleSize(0.04);
  ph2->GetXaxis()->SetLabelSize(0.04);
  ph2->GetYaxis()->SetTitleOffset(1.2);
  ph2->GetYaxis()->SetTitleSize(0.04);
  ph2->GetYaxis()->SetLabelSize(0.04);
  ph2->GetZaxis()->SetTitleOffset(0.95);
  ph2->GetZaxis()->SetTitleSize(0.04);
  ph2->GetZaxis()->SetLabelSize(0.04);
  //ph2->GetXaxis()->CenterTitle();
  ph2->SetLineWidth(2);

}

TLegend * getLegend( Float_t minXCor,Float_t minYCorr, Float_t maxXCor, Float_t maxYCor)
{
  TLegend *leg = new TLegend(minXCor,minYCorr,maxXCor,maxYCor);
  leg->SetFillColor(10);
  leg->SetTextSize(0.04);
  leg->SetBorderSize(1);
  return leg;
}

TLatex * getLatexText(Double_t x, Double_t y, char* text)
{
  TLatex *t2 = new TLatex(x,y,text);
  t2->SetTextSize(0.06);
  t2->SetNDC(kTRUE);
  t2->SetX(0.1);
  t2->SetY(0.93);
  return t2;
}

void setOptions()
{
gStyle->SetCanvasColor(10);
gStyle->SetPadColor(10);
gStyle->SetOptTitle(0);
gStyle->SetOptStat(00000000);
//TGaxis::SetMaxDigits(2);
}

CocktailNoCuts(Char_t* inputfile) //Plots Cocktail composition before any cut is applied
{
  setOptions();
  TFile* ptrlow = TFile::Open(inputfile,"READ");
  
  
  TCanvas* Cocktail = new TCanvas("Cocktail composition","Cocktail before any cuts");
  Cocktail->cd();

  Cocktail->SetGridx();
  Cocktail->SetGridy();
  Cocktail->SetLogy();

  TH1F* FullSpectrum = hmass_cut0_true0_norm->Clone();
  TH1F* Pi0Dalitz = hmass_cut0_true1_norm->Clone();
  TH1F* Conversion = hmass_cut0_true2_norm->Clone();
  TH1F* EtaDalitz = hmass_cut0_true3_norm->Clone();
  TH1F* DeltaDalitz = hmass_cut0_true4_norm->Clone();
  TH1F* RhoDirect = hmass_cut0_true5_norm->Clone();
  TH1F* OmegaDalitz = hmass_cut0_true6_norm->Clone();
  TH1F* OmegaDirect = hmass_cut0_true7_norm->Clone();
  
  setHistOptions(FullSpectrum);
  FullSpectrum->SetTitle("");
  FullSpectrum->SetLineColor(2);
  FullSpectrum->SetXTitle("Invariant Mass [MeV/c^{2}]");
  FullSpectrum->SetYTitle("counts /1MeV/c^{2}/LVL1");
  FullSpectrum->SetAxisRange(1.e-12,1.e-4,"Y");
  FullSpectrum->Draw("le");


  setHistOptions(Pi0Dalitz);
  Pi0Dalitz->SetTitle("");
  Pi0Dalitz->SetLineColor(3);
  Pi0Dalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  Pi0Dalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  Pi0Dalitz->Draw("same,le");
  
      
  setHistOptions(Conversion);
  Conversion->SetTitle("");
  Conversion->SetLineColor(4);
  Conversion->SetXTitle("Invariant Mass [MeV/c^{2}]");
  Conversion->SetYTitle("counts /1MeV/c^{2}/LVL1");
  Conversion->Draw("same,le");

      
  setHistOptions(EtaDalitz);
  EtaDalitz->SetTitle("");
  EtaDalitz->SetLineColor(5);
  EtaDalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  EtaDalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  EtaDalitz->Draw("same,le");

      
  setHistOptions(DeltaDalitz);
  DeltaDalitz->SetTitle("");
  DeltaDalitz->SetLineColor(6);
  DeltaDalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  DeltaDalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  DeltaDalitz->Draw("same,le");

      
  setHistOptions(RhoDirect);
  RhoDirect->SetTitle("");
  RhoDirect->SetLineColor(7);
  RhoDirect->SetXTitle("Invariant Mass [MeV/c^{2}]");
  RhoDirect->SetYTitle("counts /1MeV/c^{2}/LVL1");
  RhoDirect->Draw("same,le");

      
  setHistOptions(OmegaDalitz);
  OmegaDalitz->SetTitle("");
  OmegaDalitz->SetLineColor(8);
  OmegaDalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  OmegaDalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  OmegaDalitz->Draw("same,le");

      
  setHistOptions(OmegaDirect);
  OmegaDirect->SetTitle("");
  OmegaDirect->SetLineColor(9);
  OmegaDirect->SetXTitle("Invariant Mass [MeV/c^{2}]");
  OmegaDirect->SetYTitle("counts /1MeV/c^{2}/LVL1");
  OmegaDirect->Draw("same,le");


  
  TLegend *leg3 = getLegend(0.6,0.6,0.89,0.89);
  leg3->AddEntry(FullSpectrum,"All pairs","le");
  leg3->AddEntry(Pi0Dalitz,"Pi0 - Dalitz","le");
  leg3->AddEntry(Conversion,"Conversion","le");
  leg3->AddEntry(EtaDalitz,"Eta - Dalitz","le");
  leg3->AddEntry(DeltaDalitz,"Delta - Dalitz","le");
  leg3->AddEntry(RhoDirect,"Rho - Direct","le");
  leg3->AddEntry(OmegaDalitz,"Omega - Dalitz","le");
  leg3->AddEntry(OmegaDirect,"Omega - Direct","le");
  leg3->SetHeader("Cocktail composition");
  leg3->Draw("same");

  TLatex* t = getLatexText(0.85,1.0,"Cocktail composition without pair cuts ");
  t->Draw();
  
  Cocktail->SaveAs("CocktailNoCuts.root");
  Cocktail->SaveAs("CocktailNoCuts.pdf");
  Cocktail->SaveAs("CocktailNoCuts.ps");
}

CocktailAllCuts(Char_t* inputfile) //Plots Cocktail composition after all cuts are applied
{
  setOptions();
  TFile* ptrlow = TFile::Open(inputfile,"READ");
  
  
  TCanvas* Cocktail = new TCanvas("Cocktail composition","Cocktail after all pair cuts");
  Cocktail->cd();

  Cocktail->SetGridx();
  Cocktail->SetGridy();
  Cocktail->SetLogy();

  TH1F* FullSpectrum = hmass_cut9_true0_norm->Clone();
  TH1F* Pi0Dalitz = hmass_cut9_true1_norm->Clone();
  TH1F* Conversion = hmass_cut9_true2_norm->Clone();
  TH1F* EtaDalitz = hmass_cut9_true3_norm->Clone();
  TH1F* DeltaDalitz = hmass_cut9_true4_norm->Clone();
  TH1F* RhoDirect = hmass_cut9_true5_norm->Clone();
  TH1F* OmegaDalitz = hmass_cut9_true6_norm->Clone();
  TH1F* OmegaDirect = hmass_cut9_true7_norm->Clone();
  

  setHistOptions(FullSpectrum);
  FullSpectrum->SetTitle("");
  FullSpectrum->SetLineColor(2);
  FullSpectrum->SetXTitle("Invariant Mass [MeV/c^{2}]");
  FullSpectrum->SetYTitle("counts /1MeV/c^{2}/LVL1");
  FullSpectrum->SetAxisRange(1.e-12,1.e-4,"Y");
  FullSpectrum->Draw("le");

      
  setHistOptions(Pi0Dalitz);
  Pi0Dalitz->SetTitle("");
  Pi0Dalitz->SetLineColor(3);
  Pi0Dalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  Pi0Dalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  Pi0Dalitz->Draw("same,le");
  
      
  setHistOptions(Conversion);
  Conversion->SetTitle("");
  Conversion->SetLineColor(4);
  Conversion->SetXTitle("Invariant Mass [MeV/c^{2}]");
  Conversion->SetYTitle("counts /1MeV/c^{2}/LVL1");
  Conversion->Draw("same,le");

      
  setHistOptions(EtaDalitz);
  EtaDalitz->SetTitle("");
  EtaDalitz->SetLineColor(5);
  EtaDalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  EtaDalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  EtaDalitz->Draw("same,le");

      
  setHistOptions(DeltaDalitz);
  DeltaDalitz->SetTitle("");
  DeltaDalitz->SetLineColor(6);
  DeltaDalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  DeltaDalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  DeltaDalitz->Draw("same,le");

      
  setHistOptions(RhoDirect);
  RhoDirect->SetTitle("");
  RhoDirect->SetLineColor(7);
  RhoDirect->SetXTitle("Invariant Mass [MeV/c^{2}]");
  RhoDirect->SetYTitle("counts /1MeV/c^{2}/LVL1");
  RhoDirect->Draw("same,le");

      
  setHistOptions(OmegaDalitz);
  OmegaDalitz->SetTitle("");
  OmegaDalitz->SetLineColor(8);
  OmegaDalitz->SetXTitle("Invariant Mass [MeV/c^{2}]");
  OmegaDalitz->SetYTitle("counts /1MeV/c^{2}/LVL1");
  OmegaDalitz->Draw("same,le");

      
  setHistOptions(OmegaDirect);
  OmegaDirect->SetTitle("");
  OmegaDirect->SetLineColor(9);
  OmegaDirect->SetXTitle("Invariant Mass [MeV/c^{2}]");
  OmegaDirect->SetYTitle("counts /1MeV/c^{2}/LVL1");
  OmegaDirect->Draw("same,le");


  
  TLegend* leg3 = getLegend(0.6,0.6,0.89,0.89);
  leg3->AddEntry(FullSpectrum,"All pairs","le");
  leg3->AddEntry(Pi0Dalitz,"Pi0 - Dalitz","le");
  leg3->AddEntry(Conversion,"Conversion","le");
  leg3->AddEntry(EtaDalitz,"Eta - Dalitz","le");
  leg3->AddEntry(DeltaDalitz,"Delta - Dalitz","le");
  leg3->AddEntry(RhoDirect,"Rho - Direct","le");
  leg3->AddEntry(OmegaDalitz,"Omega - Dalitz","le");
  leg3->AddEntry(OmegaDirect,"Omega - Direct","le");
  leg3->SetHeader("Cocktail composition");
  leg3->Draw("same");

  TLatex* t = getLatexText(0.85,1.0,"Cocktail composition after all pair cuts ");
  t->Draw();

  
  Cocktail->SaveAs("CocktailAllCuts.root");
  Cocktail->SaveAs("CocktailAllCuts.pdf");
  Cocktail->SaveAs("CocktailAllCuts.ps");
}


NegativeLSReduction(Char_t* inputfile) //Plots Ratio of -- signal with cuts to the one without cut
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* negativelsbackground =  new TCanvas("NegativeLsBackgroundReduction","Reduction of negative like sign background");
  negativelsbackground->cd();
  negativelsbackground->SetGridx();
  negativelsbackground->SetGridy();

  // -- Like sign combinatorial background
  TH1F* base = hmass_cut0_pol0_norm->Clone();
  hmass_cut0_pol0_norm->SetTitle("");
  hmass_cut1_pol0_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_pol0_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_pol0_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol0_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  
  setHistOptions(hmass_cut0_pol0_norm);
  setHistOptions(hmass_cut1_pol0_norm);
  setHistOptions(hmass_cut5_pol0_norm);
  setHistOptions(hmass_cut7_pol0_norm);
  setHistOptions(hmass_cut9_pol0_norm);

  hmass_cut1_pol0_norm->SetTitle("");
  hmass_cut1_pol0_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut1_pol0_norm->SetYTitle("Yield [counts /1MeV/c^{2}/LVL1] ");
  
  //hmass_cut0_pol0_norm->Draw("lp"); //opening angle cut

  hmass_cut1_pol0_norm->Divide(base);
  hmass_cut1_pol0_norm->Draw(""); //opening angle cut

  hmass_cut5_pol0_norm->Divide(base);
  hmass_cut5_pol0_norm->Draw("same,l"); //opening angle + no same hit cut

  hmass_cut7_pol0_norm->Divide(base);
  hmass_cut7_pol0_norm->Draw("same,l"); //opening angle + no same hit cut + recursive rejection

  hmass_cut9_pol0_norm->Divide(base);
  hmass_cut9_pol0_norm->Draw("same,l"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.12,0.7,0.89,0.89);
  //leg1->AddEntry(hmass_cut0_pol2_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_pol0_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_pol0_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_pol0_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_pol0_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Like sign -- CB");
  leg1->Draw();
  
  TLatex* t = getLatexText(0.85,1.0,"Reduction of e^{-}e^{-}");
  t->Draw();

  negativelsbackground->SaveAs("reductionnegativelsbackground_aug04.root");
  negativelsbackground->SaveAs("reductionnegativelsbackground_aug04.pdf");
  negativelsbackground->SaveAs("reductionnegativelsbackground_aug04.ps");

}

UnlikeSignReduction(Char_t* inputfile) //Plots Ratio of -+ signal with cuts to the one without cut
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* unlsbackground =  new TCanvas("UnlikesignBackgroundReduction","Reduction of unlike sign background");
  unlsbackground->cd();
  unlsbackground->SetGridx();
  unlsbackground->SetGridy();

  // -- Like sign combinatorial background
  TH1F* base = hmass_cut0_pol1_norm->Clone();
  hmass_cut0_pol1_norm->SetTitle("");
  hmass_cut1_pol1_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_pol1_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_pol1_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol1_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition

  setHistOptions(hmass_cut0_pol1_norm);
  setHistOptions(hmass_cut1_pol1_norm);
  setHistOptions(hmass_cut5_pol1_norm);
  setHistOptions(hmass_cut7_pol1_norm);
  setHistOptions(hmass_cut9_pol1_norm);

  hmass_cut1_pol1_norm->SetTitle("");
  hmass_cut1_pol1_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut1_pol1_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  
  //hmass_cut0_pol1_norm->Draw("lp"); //opening angle cut

  hmass_cut1_pol1_norm->Divide(base);
  hmass_cut1_pol1_norm->Draw(""); //opening angle cut

  hmass_cut5_pol1_norm->Divide(base);
  hmass_cut5_pol1_norm->Draw("same,l"); //opening angle + no same hit cut

  hmass_cut7_pol1_norm->Divide(base);
  hmass_cut7_pol1_norm->Draw("same,l"); //opening angle + no same hit cut + recursive rejection

  hmass_cut9_pol1_norm->Divide(base);
  hmass_cut9_pol1_norm->Draw("same,l"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.2,0.89,0.4);
  //leg1->AddEntry(hmass_cut0_pol2_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_pol1_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_pol1_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_pol1_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_pol1_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Unlike sign reduction");
  leg1->Draw();

  TLatex* t = getLatexText(0.85,1.0,"Reduction of e^{+}e^{-}");
  t->Draw();

  unlsbackground->SaveAs("reductionunlikesignpairs_aug04.root");
  unlsbackground->SaveAs("reductionunlikesignpairs_aug04.pdf");
  unlsbackground->SaveAs("reductionunlikesignpairs_aug04.ps");

}

PositiveLSReduction(Char_t* inputfile) //Plots Ratio of ++ signal with cuts to the one without cut
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* positivelsbackground =  new TCanvas("PositiveLsBackgroundReduction","Reduction of positive like sign background");
  positivelsbackground->cd();
  positivelsbackground->SetGridx();
  positivelsbackground->SetGridy();

  // -- Like sign combinatorial background
  TH1F* base = hmass_cut0_pol2_norm->Clone();
  hmass_cut0_pol2_norm->SetTitle("");
  hmass_cut1_pol2_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_pol2_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_pol2_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol2_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition

  setHistOptions(hmass_cut0_pol2_norm);
  setHistOptions(hmass_cut1_pol2_norm);
  setHistOptions(hmass_cut5_pol2_norm);
  setHistOptions(hmass_cut7_pol2_norm);
  setHistOptions(hmass_cut9_pol2_norm);

  hmass_cut1_pol2_norm->SetTitle("");
  hmass_cut1_pol2_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut1_pol2_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  
  //hmass_cut0_pol2_norm->Draw("lp"); //opening angle cut

  hmass_cut1_pol2_norm->Divide(base);
  hmass_cut1_pol2_norm->Draw("l"); //opening angle cut

  hmass_cut5_pol2_norm->Divide(base);
  hmass_cut5_pol2_norm->Draw("same,l"); //opening angle + no same hit cut

  hmass_cut7_pol2_norm->Divide(base);
  hmass_cut7_pol2_norm->Draw("same,l"); //opening angle + no same hit cut + recursive rejection

  hmass_cut9_pol2_norm->Divide(base);
  hmass_cut9_pol2_norm->Draw("same,l"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.4,0.6,0.89,0.89);
  //leg1->AddEntry(hmass_cut0_pol2_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_pol2_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_pol2_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_pol2_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_pol2_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Like sign ++ CB");
  leg1->Draw();
  
  TLatex* t = getLatexText(0.85,1.0,"Reduction of e^{+}e^{+}");
  t->Draw();

  positivelsbackground->SaveAs("reductionpositivelsbackground_aug04.root");
  positivelsbackground->SaveAs("reductionpositivelsbackground_aug04.pdf");
  positivelsbackground->SaveAs("reductionpositivelsbackground_aug04.ps");

}


ReconstructedVsTrueNoCuts(Char_t* inputfile) //Plots the true signal before any cuts and the reconstructed signal before any cuts
{
  setOptions();
  TFile* ptrlow = TFile::Open(inputfile,"READ");
  
  
  TCanvas* ReconstructedVsTrue = new TCanvas("ReconstructedVsTrue","True pair content vs reconstructed Pair content before any cuts");
  ReconstructedVsTrue->cd();

  ReconstructedVsTrue->SetGridx();
  ReconstructedVsTrue->SetGridy();
  ReconstructedVsTrue->SetLogy();
  
  TH1F* TrueSpectrum = hmass_cut0_true0_norm->Clone();
  setHistOptions(TrueSpectrum);
  TrueSpectrum->SetTitle("");
  TrueSpectrum->SetLineColor(2);
  TrueSpectrum->SetXTitle("Invariant Mass [MeV/c^{2}]");
  TrueSpectrum->SetYTitle("counts /1MeV/c^{2}/LVL1");
  TrueSpectrum->Draw("le");
  
  TH1F* ReconstructedSpectrum = hmass_cut0_back_0_sig_norm->Clone();
  setHistOptions(ReconstructedSpectrum);
  ReconstructedSpectrum->SetTitle("");
  ReconstructedSpectrum->SetLineColor(4);
  ReconstructedSpectrum->Draw("le,same");
  
  TLegend* leg3 = getLegend(0.3,0.6,0.89,0.89);
  leg3->AddEntry(ReconstructedSpectrum,"Reconstructed pair signal without pair-cuts","le");
  leg3->AddEntry(TrueSpectrum,"True pair signal withut pair-cuts","le");
  leg3->SetHeader("Reconstructed vs true pair signal - without cuts");
  leg3->Draw("same");
  
  TLatex* t = getLatexText(0.85,1.0,"Reconstructed vs True Signal no cuts");
  t->Draw();

  ReconstructedVsTrue->SaveAs("ReconstructedVsTrueNoCuts.root");
  ReconstructedVsTrue->SaveAs("ReconstructedVsTrueNoCuts.pdf");
  ReconstructedVsTrue->SaveAs("ReconstructedVsTrueNoCuts.ps");

}

ReconstructedVsTrueAllCuts(Char_t* inputfile) //Plots the true signal after all cuts and the reconstructed signal after all cuts
{
  setOptions();
  TFile* ptrlow = TFile::Open(inputfile,"READ");
  
  
  TCanvas* ReconstructedVsTrue = new TCanvas("ReconstructedVsTrue","True pair content vs reconstructed Pair content after all cuts");
  ReconstructedVsTrue->cd();

  ReconstructedVsTrue->SetGridx();
  ReconstructedVsTrue->SetGridy();
  ReconstructedVsTrue->SetLogy();

  TH1F* TrueSpectrum = hmass_cut9_true0_norm->Clone();
  setHistOptions(TrueSpectrum);
  TrueSpectrum->SetTitle("");
  TrueSpectrum->SetLineColor(2);
  TrueSpectrum->SetXTitle("Invariant Mass [MeV/c^{2}]");
  TrueSpectrum->SetYTitle("counts /1MeV/c^{2}/LVL1");
  TrueSpectrum->Draw("le");
  
  TH1F* ReconstructedSpectrum = hmass_cut9_back_0_sig_norm->Clone();
  setHistOptions(ReconstructedSpectrum);
  ReconstructedSpectrum->SetTitle("");
  ReconstructedSpectrum->SetLineColor(4);
  ReconstructedSpectrum->Draw("le,same");
  
  TLegend* leg3 = getLegend(0.4,0.6,0.89,0.89);
  leg3->AddEntry(ReconstructedSpectrum,"Reconstructed pair signal after all pair-cuts","le");
  leg3->AddEntry(TrueSpectrum,"True pair signal after all pair-cuts","le");
  leg3->SetHeader("Reconstructed vs true pair signal - after all cuts");
  leg3->Draw("same");

  TLatex* t = getLatexText(0.85,1.0,"Reconstructed vs True Signal all cuts");
  t->Draw();

  ReconstructedVsTrue->SaveAs("ReconstructedVsTrueAllCuts.root");
  ReconstructedVsTrue->SaveAs("ReconstructedVsTrueAllCuts.pdf");
  ReconstructedVsTrue->SaveAs("ReconstructedVsTrueAllCuts.ps");

}

NegativeLSBackgroundDevelopment(Char_t* inputfile) //Plots the change in the -- background with increasing cuts
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* negativelsbackground =  new TCanvas("LikeSignBackground --","Like Sign Combinatorial background (--) aug04");
  negativelsbackground->cd();
  negativelsbackground->SetGridx();
  negativelsbackground->SetGridy();
  negativelsbackground->SetLogy();
  
  // -- Like sign combinatorial background
  hmass_cut0_pol0_norm->SetTitle("");
  hmass_cut1_pol0_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_pol0_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_pol0_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol0_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  setHistOptions(hmass_cut0_pol0_norm);
  setHistOptions(hmass_cut1_pol0_norm);
  setHistOptions(hmass_cut5_pol0_norm);
  setHistOptions(hmass_cut7_pol0_norm);
  setHistOptions(hmass_cut9_pol0_norm);

  hmass_cut0_pol0_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_pol0_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_pol0_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_pol0_norm->Draw("lp"); //opening angle cut
  hmass_cut1_pol0_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_pol0_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_pol0_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol0_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_pol0_norm,"no cut","le");
  leg1->AddEntry(hmass_cut1_pol0_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_pol0_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_pol0_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_pol0_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Like sign -- CB");
  leg1->Draw();
  
  TLatex* t = getLatexText(0.85,1.0,"e^{-}e^{-}");
  t->Draw();

  negativelsbackground->SaveAs("negativelsbackground_aug04.root");
  negativelsbackground->SaveAs("negativelsbackground_aug04.pdf");
  negativelsbackground->SaveAs("negativelsbackground_aug04.ps");

}

PositiveLSBackgroundDevelopment(Char_t* inputfile) //Plots the change in the ++ background with increasing cuts
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* positivelsbackground =  new TCanvas("LikeSignBackground ++","Like Sign Combinatorial background (++) aug04");
  positivelsbackground->cd();
  positivelsbackground->SetGridx();
  positivelsbackground->SetGridy();
  positivelsbackground->SetLogy();

  // -- Like sign combinatorial background
  hmass_cut0_pol2_norm->SetTitle("");
  hmass_cut1_pol2_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_pol2_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_pol2_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol2_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  
  setHistOptions(hmass_cut0_pol2_norm);
  setHistOptions(hmass_cut1_pol2_norm);
  setHistOptions(hmass_cut5_pol2_norm);
  setHistOptions(hmass_cut7_pol2_norm);
  setHistOptions(hmass_cut9_pol2_norm);

  hmass_cut0_pol2_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_pol2_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_pol2_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_pol2_norm->Draw("lp"); //opening angle cut
  hmass_cut1_pol2_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_pol2_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_pol2_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol2_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_pol2_norm,"no cut","le");
  leg1->AddEntry(hmass_cut1_pol2_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_pol2_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_pol2_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_pol2_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Like sign ++ CB");
  leg1->Draw();

  TLatex* t = getLatexText(0.85,1.0,"e^{+}e^{+}");
  t->Draw();

  positivelsbackground->SaveAs("positivelsbackground_aug04.root");
  positivelsbackground->SaveAs("positivelsbackground_aug04.pdf");
  positivelsbackground->SaveAs("positivelsbackground_aug04.ps");

}

UnlikeSignSpectrumDevelopment(Char_t* inputfile) //Plots the change in the -+ pair signal with increasing cuts
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* unlikesignpairs =  new TCanvas("UnlikeSignPairs -+","Unlike Sign Pair Combinations aug04");
  unlikesignpairs->cd();
  unlikesignpairs->SetGridx();
  unlikesignpairs->SetGridy();
  unlikesignpairs->SetLogy();

  // Unlike sign pair spectrum
  hmass_cut0_pol1_norm->SetTitle("");
  hmass_cut1_pol1_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_pol1_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_pol1_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol1_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition

  setHistOptions(hmass_cut0_pol1_norm);
  setHistOptions(hmass_cut1_pol1_norm);
  setHistOptions(hmass_cut5_pol1_norm);
  setHistOptions(hmass_cut7_pol1_norm);
  setHistOptions(hmass_cut9_pol1_norm);

  hmass_cut0_pol1_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_pol1_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_pol1_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_pol1_norm->Draw("lp"); //opening angle cut
  hmass_cut1_pol1_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_pol1_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_pol1_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_pol1_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_pol1_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_pol1_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_pol1_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_pol1_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_pol1_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Unlike sign Pair-Spectrum");
  leg1->Draw();
  
  TLatex* t = getLatexText(0.85,1.0,"e^{+}e^{-}");
  t->Draw();

  unlikesignpairs->SaveAs("unlikesignpairs_aug04.root");
  unlikesignpairs->SaveAs("unlikesignpairs_aug04.pdf");
  unlikesignpairs->SaveAs("unlikesignpairs_aug04.ps");

}

GeometricMeanDevelopment(Char_t* inputfile) //Plots the development of the geometric mean background with increasing cuts

{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* unlikesignpairs =  new TCanvas("GeometricMean","Geometric Mean of ++/-- CB aug04");
  unlikesignpairs->cd();
  unlikesignpairs->SetGridx();
  unlikesignpairs->SetGridy();
  unlikesignpairs->SetLogy();

  //Geometric mean of ++/-- background
  hmass_cut0_back_0_norm->SetTitle("");
  hmass_cut1_back_0_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_back_0_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_back_0_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_0_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition

  setHistOptions(hmass_cut0_back_0_norm);
  setHistOptions(hmass_cut1_back_0_norm);
  setHistOptions(hmass_cut5_back_0_norm);
  setHistOptions(hmass_cut7_back_0_norm);
  setHistOptions(hmass_cut9_back_0_norm);

  hmass_cut0_back_0_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_back_0_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_back_0_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_back_0_norm->Draw("lp"); //opening angle cut
  hmass_cut1_back_0_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_back_0_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_back_0_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_0_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_back_0_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_back_0_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_back_0_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_back_0_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_back_0_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Geometric mean of ++/-- background");
  leg1->Draw();
  
  TLatex* t = getLatexText(0.85,1.0,"geometric mean background development");
  t->Draw();

  unlikesignpairs->SaveAs("geometricmean_aug04.root");
  unlikesignpairs->SaveAs("geometricmean_aug04.pdf");
  unlikesignpairs->SaveAs("geometricmean_aug04.ps");

}


ArithmeticMeanDevelopment(Char_t* inputfile) //Plots the development of the arithmetic mean background with increasing cuts

{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* unlikesignpairs =  new TCanvas("ArithmeticMean","Arithmetic Mean of ++/-- CB aug04");
  unlikesignpairs->cd();
  unlikesignpairs->SetGridx();
  unlikesignpairs->SetGridy();
  unlikesignpairs->SetLogy();

  //Arithmetic mean of ++/-- background
  hmass_cut0_back_1_norm->SetTitle("");
  hmass_cut1_back_1_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_back_1_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_back_1_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_1_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition

  setHistOptions(hmass_cut0_back_1_norm);
  setHistOptions(hmass_cut1_back_1_norm);
  setHistOptions(hmass_cut5_back_1_norm);
  setHistOptions(hmass_cut7_back_1_norm);
  setHistOptions(hmass_cut9_back_1_norm);

  hmass_cut0_back_1_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_back_1_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_back_1_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_back_1_norm->Draw("lp"); //opening angle cut
  hmass_cut1_back_1_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_back_1_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_back_1_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_1_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_back_1_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_back_1_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_back_1_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_back_1_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_back_1_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Arithmetic mean of ++/-- background");
  leg1->Draw();

  TLatex* t = getLatexText(0.85,1.0,"arithmetic mean background development");
  t->Draw();
  
  unlikesignpairs->SaveAs("arithmeticmean_aug04.root");
  unlikesignpairs->SaveAs("arithmeticmean_aug04.pdf");
  unlikesignpairs->SaveAs("arithmeticmean_aug04.ps");

}

//Plots the development of the net signal after subtraction of arithmetic mean background with increasing cuts
NetSignalDevelopmentArithmetic(Char_t* inputfile) 
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* unlikesignpairs =  new TCanvas("NetSignal","Net Signal after subtraction of arithmetic CB aug04");
  unlikesignpairs->cd();
  unlikesignpairs->SetGridx();
  unlikesignpairs->SetGridy();
  unlikesignpairs->SetLogy();

  //Net signal after subtraction of arithmetic-mean CB
  hmass_cut0_back_1_sig_norm->SetTitle("");
  hmass_cut1_back_1_sig_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_back_1_sig_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_back_1_sig_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_1_sig_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition

  setHistOptions(hmass_cut0_back_1_sig_norm);
  setHistOptions(hmass_cut1_back_1_sig_norm);
  setHistOptions(hmass_cut5_back_1_sig_norm);
  setHistOptions(hmass_cut7_back_1_sig_norm);
  setHistOptions(hmass_cut9_back_1_sig_norm);

  hmass_cut0_back_1_sig_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_back_1_sig_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_back_1_sig_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_back_1_sig_norm->Draw("lp"); //opening angle cut
  hmass_cut1_back_1_sig_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_back_1_sig_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_back_1_sig_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_1_sig_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.2,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_back_1_sig_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_back_1_sig_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_back_1_sig_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_back_1_sig_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_back_1_sig_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Net signal after subtraction of arithmetic-mean CB");
  leg1->Draw();

  TLatex* t = getLatexText(0.85,1.0,"Signal norm. arithm. background substr.");
  t->Draw();


  unlikesignpairs->SaveAs("netsignal_arithmetic_aug04.root");
  unlikesignpairs->SaveAs("netsignal_arithmetic_aug04.pdf");
  unlikesignpairs->SaveAs("netsignal_arithmetic_aug04.ps");
}

//Plots the development of the net signal after subtraction of geometric mean background with increasing cuts
NetSignalDevelopmentGeometric(Char_t* inputfile)
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");
  TCanvas* unlikesignpairs =  new TCanvas("NetSignal","Net Signal after subtraction of geometric-mean CB aug04");
  unlikesignpairs->cd();
  unlikesignpairs->SetGridx();
  unlikesignpairs->SetGridy();
  unlikesignpairs->SetLogy();

  //Net signal after subtraction of geometric-mean CB
  hmass_cut0_back_1_sig_norm->SetTitle("");
  hmass_cut1_back_1_sig_norm->SetLineColor(2); //opening angle cut
  hmass_cut5_back_1_sig_norm->SetLineColor(4); //opening angle + no same hit cut
  hmass_cut7_back_1_sig_norm->SetLineColor(6); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_1_sig_norm->SetLineColor(8); //opening angle + no same hit cut + recursive rejection + no cp candidate condition


  setHistOptions(hmass_cut0_back_1_sig_norm);
  setHistOptions(hmass_cut1_back_1_sig_norm);
  setHistOptions(hmass_cut5_back_1_sig_norm);
  setHistOptions(hmass_cut7_back_1_sig_norm);
  setHistOptions(hmass_cut9_back_1_sig_norm);


  hmass_cut0_back_1_sig_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_back_1_sig_norm->SetYTitle("counts /1MeV/c^{2}/LVL1 ");
  hmass_cut0_back_1_sig_norm->SetAxisRange(1.e-12,1.e-4,"Y");

  hmass_cut0_back_1_sig_norm->Draw("lp"); //opening angle cut
  hmass_cut1_back_1_sig_norm->Draw("same,lp"); //opening angle cut

  hmass_cut5_back_1_sig_norm->Draw("same,lp"); //opening angle + no same hit cut
  hmass_cut7_back_1_sig_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection
  hmass_cut9_back_1_sig_norm->Draw("same,lp"); //opening angle + no same hit cut + recursive rejection + no cp candidate condition
  

  TLegend* leg1 = getLegend(0.4,0.6,0.89,0.89);
  leg1->AddEntry(hmass_cut0_back_1_sig_norm,"N0 cut","le");
  leg1->AddEntry(hmass_cut1_back_1_sig_norm,"op-ang","le");
  leg1->AddEntry(hmass_cut5_back_1_sig_norm,"op-ang & no-shared-hit","le");
  leg1->AddEntry(hmass_cut7_back_1_sig_norm,"op-ang & no-shared-hit + recursion","le");
  leg1->AddEntry(hmass_cut9_back_1_sig_norm,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");

  leg1->SetHeader("Net signal after subtraction of geometric-mean CB");
  leg1->Draw();
  
  TLatex* t = getLatexText(0.85,1.0,"Signal norm. geom. background substr.");
  t->Draw();
  unlikesignpairs->SaveAs("netsignal_geometric_aug04.root");
  unlikesignpairs->SaveAs("netsignal_geometric_aug04.pdf");
  unlikesignpairs->SaveAs("netsignal_geometric_aug04.ps");
}

//Plots the development of the S/B with increasing cuts
SignalToBackgroundDevelopmentLog(Char_t* inputfile)
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");

  TCanvas* LepReduction = new TCanvas("SignalToBackground","Signal/Background with increasing cuts");
  LepReduction->cd(1);
  LepReduction->SetLogy();
  LepReduction->SetGridy();
  LepReduction->SetGridx();

  //Signal to background after successive cuts
  hmass_cut0_back_0_sig_norm->SetTitle("");
  hmass_cut0_back_0_sig_norm->SetMaximum(400);
  
  hmass_cut1_back_0_sig_norm->SetLineColor(2);
  hmass_cut5_back_0_sig_norm->SetLineColor(4);
  hmass_cut7_back_0_sig_norm->SetLineColor(6);
  hmass_cut9_back_0_sig_norm->SetLineColor(8);


  setHistOptions(hmass_cut0_back_0_sig_norm);
  setHistOptions(hmass_cut1_back_0_sig_norm);
  setHistOptions(hmass_cut5_back_0_sig_norm);
  setHistOptions(hmass_cut7_back_0_sig_norm);
  setHistOptions(hmass_cut9_back_0_sig_norm);

  hmass_cut0_back_0_sig_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_back_0_sig_norm->SetYTitle("Signal/Background");

  
  TH1F* s_o_b_0 = (TH1F*)hmass_cut0_back_0_sig_norm->Clone();
  TH1F* s_o_b_1 = (TH1F*)hmass_cut1_back_0_sig_norm->Clone();
  TH1F* s_o_b_5 = (TH1F*)hmass_cut5_back_0_sig_norm->Clone();
  TH1F* s_o_b_7 = (TH1F*)hmass_cut7_back_0_sig_norm->Clone();
  TH1F* s_o_b_9 = (TH1F*)hmass_cut9_back_0_sig_norm->Clone();

  s_o_b_0->Divide(hmass_cut0_back_0_norm);
  s_o_b_1->Divide(hmass_cut1_back_0_norm);
  s_o_b_5->Divide(hmass_cut5_back_0_norm);
  s_o_b_7->Divide(hmass_cut7_back_0_norm);
  s_o_b_9->Divide(hmass_cut9_back_0_norm);
  
  TLegend* leg3 = getLegend(0.2,0.7,0.89,0.89);
  leg3->AddEntry(s_o_b_0,"No cuts applied","le");
  leg3->AddEntry(s_o_b_1,"op-ang","le");
  leg3->AddEntry(s_o_b_5,"op-ang & no-shared-hit","le");
  leg3->AddEntry(s_o_b_7,"op-ang & no-shared-hit + recursion","le");
  leg3->AddEntry(s_o_b_9,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");
  leg3->SetHeader("Signal to Background as function of applied cut");

  TLatex* t = getLatexText(0.85,1.0,"Signal/Background");

  s_o_b_0->Draw("le");
  s_o_b_1->Draw("le,same");
  s_o_b_5->Draw("le,same");
  s_o_b_7->Draw("le,same");
  s_o_b_9->Draw("le,same");
  t->Draw();
  leg3->Draw();

  LepReduction->SaveAs("SignalToBackground.root");
  LepReduction->SaveAs("SignalToBackground.pdf");
  LepReduction->SaveAs("SignalToBackground.ps");
}

SignalToBackgroundDevelopment(Char_t* inputfile)
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");

  TCanvas* LepReduction = new TCanvas("SignalToBackground","Signal/Background with increasing cuts");
  LepReduction->cd(1);
  LepReduction->SetGridy();
  LepReduction->SetGridx();

  //Signal to background after successive cuts
  hmass_cut0_back_0_sig_norm->SetTitle("");
  hmass_cut0_back_0_sig_norm->SetAxisRange(-10,200,"Y");
  
  hmass_cut1_back_0_sig_norm->SetLineColor(2);
  hmass_cut5_back_0_sig_norm->SetLineColor(4);
  hmass_cut7_back_0_sig_norm->SetLineColor(6);
  hmass_cut9_back_0_sig_norm->SetLineColor(8);


  setHistOptions(hmass_cut0_back_0_sig_norm);
  setHistOptions(hmass_cut1_back_0_sig_norm);
  setHistOptions(hmass_cut5_back_0_sig_norm);
  setHistOptions(hmass_cut7_back_0_sig_norm);
  setHistOptions(hmass_cut9_back_0_sig_norm);

  hmass_cut0_back_0_sig_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut0_back_0_sig_norm->SetYTitle("Signal/Background");

  
  TH1F* s_o_b_0 = (TH1F*)hmass_cut0_back_0_sig_norm->Clone();
  TH1F* s_o_b_1 = (TH1F*)hmass_cut1_back_0_sig_norm->Clone();
  TH1F* s_o_b_5 = (TH1F*)hmass_cut5_back_0_sig_norm->Clone();
  TH1F* s_o_b_7 = (TH1F*)hmass_cut7_back_0_sig_norm->Clone();
  TH1F* s_o_b_9 = (TH1F*)hmass_cut9_back_0_sig_norm->Clone();

  s_o_b_0->Divide(hmass_cut0_back_0_norm);
  s_o_b_1->Divide(hmass_cut1_back_0_norm);
  s_o_b_5->Divide(hmass_cut5_back_0_norm);
  s_o_b_7->Divide(hmass_cut7_back_0_norm);
  s_o_b_9->Divide(hmass_cut9_back_0_norm);
  
  TLegend* leg3 = getLegend(0.2,0.7,0.89,0.89);
  leg3->AddEntry(s_o_b_0,"No cuts applied","le");
  leg3->AddEntry(s_o_b_1,"op-ang","le");
  leg3->AddEntry(s_o_b_5,"op-ang & no-shared-hit","le");
  leg3->AddEntry(s_o_b_7,"op-ang & no-shared-hit + recursion","le");
  leg3->AddEntry(s_o_b_9,"op-ang & no-shared-hit + recursion + no-cp-candidate","le");
  leg3->SetHeader("Signal to Background as function of applied cut");

  TLatex* t = getLatexText(0.85,1.0,"Signal/Background");

  s_o_b_0->Draw("le");
  s_o_b_1->Draw("le,same");
  s_o_b_5->Draw("le,same");
  s_o_b_7->Draw("le,same");
  s_o_b_9->Draw("le,same");
  t->Draw();
  leg3->Draw();

  LepReduction->SaveAs("SignalToBackground.root");
  LepReduction->SaveAs("SignalToBackground.pdf");
  LepReduction->SaveAs("SignalToBackground.ps");
}


//Plots Signal before bg-subtraction, signal after geometric bg subtraction and geometric background into one picture
MostWanted(Char_t* inputfile) 
{
  setOptions();
  TFile* ptr = TFile::Open(inputfile,"READ");

  TCanvas* signal =  new TCanvas("signal","Pair-Signal Aug04");
  signal->cd();
  signal->SetGridx();
  signal->SetGridy();
  signal->SetLogy();

  
  //Unlike sign pair spectrum
  hmass_cut9_pol1_norm->SetTitle("Unlike - Sign Pairs and Combinatorial Background (after all cuts)");
  hmass_cut9_pol1_norm->SetLineColor(2);
  setHistOptions( hmass_cut9_pol1_norm);
  hmass_cut9_pol1_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut9_pol1_norm->SetYTitle("counts /1MeV/c^{2}/LVL1");

      

  //Invariant mass spectrum after CB-subtraction
      hmass_cut9_back_0_sig_norm->SetLineWidth(2);
  setHistOptions(hmass_cut9_back_0_sig_norm);
  hmass_cut9_back_0_sig_norm->SetLineColor(4);
  hmass_cut9_back_0_sig_norm->SetTitle("Dilepton mass signal after geometric CB subtraction");
  hmass_cut9_back_0_sig_norm->SetXTitle("Invariant Mass [MeV/c^{2}]");
  hmass_cut9_back_0_sig_norm->SetYTitle("counts /1MeV/c^{2}/LVL1");

      
  setHistOptions( hmass_cut9_back_0_norm);
  hmass_cut9_back_0_norm->SetLineColor(1);

  hmass_cut9_back_0_norm->SetMaximum(1.0E-6);
  hmass_cut9_back_0_norm->Draw("");
  hmass_cut9_back_0_sig_norm->Draw("same");
  hmass_cut9_pol1_norm->Draw("same");
  
  TLegend* leg3 = getLegend(0.4,0.7,0.89,0.89);


  leg3->AddEntry(hmass_cut9_pol1_norm,"unlike sign pair signal e^{-}e^{-}","l");
  leg3->AddEntry(hmass_cut9_back_0_sig_norm,"dilepton mass signal","l");
  leg3->AddEntry(hmass_cut9_back_0_norm,"geometric ls-background","l");
  leg3->SetHeader("Unlike Sign Signal");
  leg3->Draw();
  
  signal->SaveAs("signal_aug04.root");
  signal->SaveAs("signal_aug04.pdf");
  signal->SaveAs("signal_aug04.ps");
}



MakeAllExpSpectra(Char_t* inputfile)
{
  NegativeLSBackgroundDevelopment(inputfile);
  NegativeLSReduction(inputfile);

  PositiveLSBackgroundDevelopment(inputfile);
  PositiveLSReduction(inputfile);

  UnlikeSignSpectrumDevelopment(inputfile);
  UnlikeSignReduction(inputfile);

  GeometricMeanDevelopment(inputfile);
  ArithmeticMeanDevelopment(inputfile);

  NetSignalDevelopmentGeometric(inputfile);
  NetSignalDevelopmentArithmetic(inputfile);

  SignalToBackgroundDevelopmentLog(inputfile);
  SignalToBackgroundDevelopment(inputfile);

  MostWanted(inputfile);
}

MakeAllSimSpectra(Char_t* inputfile)
{
  MakeAllExpSpectra(inputfile);
  ReconstructedVsTrueNoCuts;
  ReconstructedVsTrueAllCuts;
  CocktailNoCuts(inputfile);
  CocktailAllCuts(inputfile);
  
}
