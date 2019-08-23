void makeHexagon(Float_t cornerDist);
void makeText(Float_t x1,Float_t y1,Float_t x2,Float_t y2,Char_t *text);
void makePageTitle(Char_t fileName[120],Char_t dirName[120],
                      Int_t nEntries, Char_t pageTitle[120]);

void dstDiagnostic(Char_t inputDir[200],Char_t inputFile[200],Char_t outDir[120])
{
gROOT->Reset();
////  This macro automatically generates a mult-page postscript file
////  which performs some basic analysis of a DST file at the Hit level.
////  This macro will evolve/grow as the data levels included in the
////  DST's evolves.
////                            -- D.Magestro, 15.4.2001
gStyle->SetOptStat(0);
gStyle->SetPalette(1,0);
gStyle->SetOptTitle(0);
gStyle->SetPadColor(10);
gStyle->SetCanvasBorderMode(0);
gStyle->SetHistLineWidth(3);
gStyle->SetPadBorderMode(0);
gStyle->SetCanvasColor(10);

//////////////////////////////////////////////////////////////////////////
//----------------------  File and Title Section  ----------------------//
//printf("input %s\n ",inputDir);
//printf("input %s\n ",inputFile);
//strcat(inputDir,inputFile);
//printf("input %s\n ",inputDir);
//Char_t inFile[300];
//sprintf(inFile,"%s%s",inputDir,"_dst.root");
//return 0;
printf("input %s\n ",inputFile);
//return 0;
TFile *file1=new TFile(inputFile);

HTree *tree1 = (HTree *)file1->Get("T");

//--- Build psfile from infile ---//
Char_t fileName[100],cutfile2[100],cutfile3[100],dirName[100],psfile[100],pdffile[100];
Char_t variable[200],cond1[200],plot_this[200],pageTitle[120];
Char_t fileNameps[200];
Char_t *p;
Int_t size;

Int_t i = 0;
while ((fileName[i] = inputFile[i+5])) { i++; }
i = 0;
while ((fileName[i]!='.')) {fileNameps[i]=fileName[i];i++;}
fileNameps[i]='\0';
Int_t nEntries = tree1->GetEntries();

strcat(dirName,"/tmp/");
sprintf(psfile,"%s%s.ps",outDir,fileNameps);

sprintf(pdffile,"%s%s.pdf",outDir,fileNameps);
printf("\n   %s%s\n   %s%s\n   %s%s\n",
       "infile = ",inputFile,"psfile = ",psfile,"pdffile = ",pdffile);

// Open postscript file
TCanvas c2;
TPostScript ps(psfile,111);
ps.Range(18.9,25.2);          // set x,y of printed page

color = (TColor*)(gROOT->GetListOfColors()->At(11));
color->SetRGB(.91,.45,.23);

Int_t markerArray[5]={24,25,26,27,28};
Int_t colorArray[6]={2,3,4,6,11,2};
Char_t *detArray[6]={"RICH","MDC","TOF","TOFINO","SHOWER","KICK"};

Int_t richCol =   colorArray[0];
Int_t mdcCol =    colorArray[1];
Int_t tofCol =    colorArray[2];
Int_t tofinoCol = colorArray[3];
Int_t showerCol = colorArray[4];
Int_t kickCol =   colorArray[5];

TCanvas *window=new TCanvas("window","window",600,800);
window->SetBorderMode(0);

file1->cd();



////////////////////////////////////////////////////////////////////////
//---------------  Text page of diagnostic values  -------------------//
ps->NewPage();
window->Clear();

sprintf(pageTitle,"Diagnostic values from DST file");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Draw();

topPad->cd();

TPaveText *top = new TPaveText(0.05,0.88,0.95,0.99,"blNDC");
top->SetBorderSize(0);
top->SetFillColor(10);
top->SetTextAlign(13);
top->SetTextSize(0.02);
top->ReadFile("diagnosticMessage.txt");
  TText *text= top->AddText("HADES_works!@gsi.de");
  text->SetTextAlign(31);
top->Draw();

// ----- YIELDS BY SECTOR ----- //
TPaveText *ptt = new TPaveText(0.05,0.80,0.95,0.83,"blNDC");
ptt->SetBorderSize(0);
ptt->SetFillColor(10);
ptt->SetTextAlign(22);
ptt->AddText("Total yields by sector");
ptt->Draw();

Int_t col,sec,iDet;
TPad *PadCols[7];
  PadCols[0] = new TPad("PadCols0","PadCols0",.10,.50,.25,.80);
  PadCols[1] = new TPad("PadCols1","PadCols1",.3,.50,.4,.80);
  PadCols[2] = new TPad("PadCols2","PadCols2",.4,.50,.5,.80);
  PadCols[3] = new TPad("PadCols3","PadCols3",.5,.50,.6,.80);
  PadCols[4] = new TPad("PadCols4","PadCols4",.6,.50,.7,.80);
  PadCols[5] = new TPad("PadCols5","PadCols5",.7,.50,.8,.80);
  PadCols[6] = new TPad("PadCols6","PadCols6",.8,.50,.9,.80);
  PadCols[0]->Draw();
  PadCols[1]->Draw();
  PadCols[2]->Draw();
  PadCols[3]->Draw();
  PadCols[4]->Draw();
  PadCols[5]->Draw();
  PadCols[6]->Draw();

TPaveText *pta[7];

for(col=0;col<7;col++) {
  PadCols[col]->cd();
  pta[col] = new TPaveText(0.05,0.05,0.95,0.95,"blNDC");
  pta[col]->SetBorderSize(0);
  pta[col]->SetFillColor(10);
  pta[col]->SetTextAlign(13);
  pta[col]->SetTextSize(0.2);
}

pta[0]->SetTextSize(0.15);
pta[0]->AddText(" ");
Char_t *rowNames[6]={"Rich rings","MDC hits","TOF hits","TofinoCal","Shower hits","Kick tracks"};
PadCols[0]->cd();
for(iDet=0;iDet<6;iDet++) {
   pta[0]->AddText(rowNames[iDet]);
}
pta[0]->Draw();

Char_t *dataMemberArray[6]={
       "HRichHit.fData.nSector",
       "HMdcHit.fData.sector",
       "HTofHit.fData.sector",
       "HTofinoCal.fData.sector",
       "HShowerHit.fData.m_nSector",
       "HKickTrack.fData.sector" };

for(sec=0;sec<6;sec++) {
  PadCols[sec+1]->cd();
  sprintf(plot_this,"Sec. %i",sec);
  TText *text= pta[sec+1]->AddText(plot_this);
  text->SetTextColor(4);
  for(iDet=0;iDet<6;iDet++) {
    sprintf(plot_this,"%s>>h1",dataMemberArray[iDet]);
    sprintf(cond1,"%s == %i",dataMemberArray[iDet],sec);
    TH1F *h1 = new TH1F("h1","",10,-10,100);
    tree1->Draw(plot_this,cond1,"goff");
    sprintf(plot_this,"%i",h1->GetEntries());
    pta[sec+1]->AddText(plot_this);
    h1->Delete();
  }
pta[sec+1]->Draw();
}

topPad->cd();

// ----- YIELDS Per Event ----- //
TPaveText *ptt = new TPaveText(0.05,0.40,0.95,0.43,"blNDC");
ptt->SetBorderSize(0);
ptt->SetFillColor(10);
ptt->SetTextAlign(22);
ptt->AddText("Yield by sector (per event)");
ptt->Draw();

Int_t col,sec,iDet;
TPad *PadCols2[7];
  PadCols2[0] = new TPad("PadCols2_0","PadCols2_0",.10,.10,.25,.40);
  PadCols2[1] = new TPad("PadCols2_1","PadCols2_1",.3,.10,.4,.40);
  PadCols2[2] = new TPad("PadCols2_2","PadCols2_2",.4,.10,.5,.40);
  PadCols2[3] = new TPad("PadCols2_3","PadCols2_3",.5,.10,.6,.40);
  PadCols2[4] = new TPad("PadCols2_4","PadCols2_4",.6,.10,.7,.40);
  PadCols2[5] = new TPad("PadCols2_5","PadCols2_5",.7,.10,.8,.40);
  PadCols2[6] = new TPad("PadCols2_6","PadCols2_6",.8,.10,.9,.40);
  PadCols2[0]->Draw();
  PadCols2[1]->Draw();
  PadCols2[2]->Draw();
  PadCols2[3]->Draw();
  PadCols2[4]->Draw();
  PadCols2[5]->Draw();
  PadCols2[6]->Draw();

TPaveText *pta[7];

for(col=0;col<7;col++) {
  PadCols2[col]->cd();
  pta[col] = new TPaveText(0.05,0.05,0.95,0.95,"blNDC");
  pta[col]->SetBorderSize(0);
  pta[col]->SetFillColor(10);
  pta[col]->SetTextAlign(13);
  pta[col]->SetTextSize(0.2);
}

pta[0]->SetTextSize(0.15);
pta[0]->AddText(" ");
Char_t *rowNames[6]={"Rich rings","MDC hits","TOF hits","TofinoCal","Shower hits","Kick tracks"};
PadCols2[0]->cd();
for(iDet=0;iDet<6;iDet++) {
   pta[0]->AddText(rowNames[iDet]);
}
pta[0]->Draw();

Char_t *dataMemberArray[6]={
       "HRichHit.fData.nSector",
       "HMdcHit.fData.sector",
       "HTofHit.fData.sector",
       "HTofinoCal.fData.sector",
       "HShowerHit.fData.m_nSector",
       "HKickTrack.fData.sector" };

for(sec=0;sec<6;sec++) {
  PadCols2[sec+1]->cd();
  sprintf(plot_this,"Sec. %i",sec);
  TText *text= pta[sec+1]->AddText(plot_this);
  text->SetTextColor(4);
  for(iDet=0;iDet<6;iDet++) {
    sprintf(plot_this,"%s>>h1",dataMemberArray[iDet]);
    sprintf(cond1,"%s == %i",dataMemberArray[iDet],sec);
    TH1F *h1 = new TH1F("h1","",10,-10,100);
    tree1->Draw(plot_this,cond1,"goff");
    sprintf(plot_this,"%6.4f",h1->GetEntries()/tree1->GetEntries());
    if(h1->GetEntries()<1) pta[sec+1]->AddText("--");
    else pta[sec+1]->AddText(plot_this);
    h1->Delete();
  }
pta[sec+1]->Draw();
}



//////////////////////////////////////////////////////////////////////////
//-------------------  Histograms for all detectors  -------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"General histograms for all detectors");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--- Legend for all detectors page ---//
for(Int_t iDet=0;iDet<5;iDet++) {
   TLine *l = new TLine((float) iDet*.18+0.06,.910,(float) iDet*.18+0.10,.910);
   l->SetLineColor(colorArray[iDet]);
   l->SetLineWidth(2);
   l->Draw();

   TPaveText *pt = new TPaveText((float) iDet*.18+0.11,0.902,(float) iDet*.18+0.18,0.918,"blNDC");
   pt->SetBorderSize(0);
   pt->SetFillColor(10);
   pt->SetTextAlign(12);
   pt->SetTextSize(.02);
   pt->AddText(detArray[iDet]);
   pt->Draw();
}

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.900);
topPad->SetBorderMode(0);
topPad->Divide(2,2,0.01,0.01);
topPad->Draw();

/////////////////////////////////////////////////
//---- Pad 1: Hit Multiplicities by Sector ----//

  topPad->cd(1);
  gPad->SetLogy(1);

  Char_t *dataMemberArray[5]={
       "HRichHit.fData.nSector",
       "HMdcHit.fData.sector",
       "HTofHit.fData.sector",
       "HTofinoCal.fData.sector",
       "HShowerHit.fData.m_nSector" };

  for(iDet=0;iDet<5;iDet++) {
    sprintf(variable,"(%s+(%f))",dataMemberArray[iDet],iDet*0.1-0.21);
    sprintf(plot_this,"%s>>h1",variable);

    TH1F *h1 = new TH1F("h1","",120,-.5,5.5);
    if(iDet == 0)
      tree1->Draw(plot_this,"","");
    else
      tree1->Draw(plot_this,"","goff");
    h1->SetLineColor(colorArray[iDet]);
    h1->SetFillColor(colorArray[iDet]);
    h1->SetFillStyle(1001);
    if(iDet == 0) {
      h1->GetXaxis()->SetNdivisions(6);
      h1->SetMaximum((tree1->GetEntries())*10.);
      h1->SetMinimum(.8);
      h1->DrawCopy();
    }
    else
      h1->DrawCopy("same");
    h1->Delete();
  }
  makeText(.15,.92,.85,.98,"Hit Multiplicites by sector");

/////////////////////////////////////////////////
//---- Pad 2: Hit Multiplicities by Event ----//

  topPad->cd(2);
  gPad->SetLogy(1);

  Char_t *dataMemberArray[5]={
       "HRichHit.fNDataObjs",
       "HMdcHit.fNDataObjs",
       "HTofHit.fNDataObjs",
       "HTofinoCal.fNDataObjs",
       "HShowerHit.fNDataObjs" };

  for(iDet=0;iDet<5;iDet++) {
    sprintf(variable,"%s",dataMemberArray[iDet]);
    sprintf(plot_this,"%s>>h1",variable);

    TH1F *h1 = new TH1F("h1","",60,0.,60.);
    if(iDet == 0)
      tree1->Draw(plot_this,"","");
    else
      tree1->Draw(plot_this,"","goff");
    h1->SetLineColor(colorArray[iDet]);
    if(iDet == 0) {
      h1->SetMaximum((tree1->GetEntries())*2);
      h1->SetMinimum(.8);
      h1->DrawCopy();
    }
    else
      h1->DrawCopy("same");
    h1->Delete();
  }
  makeText(.15,.92,.85,.98,"Hit Multiplicites by sector");

/////////////////////////////////////////////////
//---- Pad 3: Hit Multiplicities by Event ----//

  topPad->cd(3);
  gPad->SetLogy(1);

  Char_t *dataMemberArray[5]={
       "HRichHit.fData.fTheta",
       "HMdcSeg.fData.theta*57.295",
       "HTofHit.fData.theta",
       "",
       "HShowerHit.fData.m_fTheta" };

  for(iDet=0;iDet<5;iDet++) {
    if(iDet!=3) {
      sprintf(variable,"%s",dataMemberArray[iDet]);
      sprintf(plot_this,"%s>>h1",variable);

      TH1F *h1 = new TH1F("h1","",100,0.,100.);
      if(iDet == 0)
        tree1->Draw(plot_this,"","");
      else
        tree1->Draw(plot_this,"","goff");
      h1->SetLineColor(colorArray[iDet]);
      if(iDet == 0) {
        h1->SetMaximum((tree1->GetEntries()));
        h1->SetMinimum(.8);
        h1->DrawCopy();
      }
      else
        h1->DrawCopy("same");
      h1->Delete();
    }
  }
  makeText(.15,.92,.85,.98,"Theta by hit");

/////////////////////////////////////////////////
//---- Pad 4: Hit Multiplicities by Event ----//

  topPad->cd(4);
  gPad->SetLogy(1);

  Char_t *dataMemberArray[5]={
       "HRichHit.fData.fPhi",
       "",
       "HTofHit.fData.phi",
       "",
       "HShowerHit.fData.m_fPhi" };

  for(iDet=0;iDet<5;iDet++) {
    if(iDet!=3 && iDet!=1) {
      sprintf(variable,"%s",dataMemberArray[iDet]);
      sprintf(plot_this,"%s>>h1",variable);

      TH1F *h1 = new TH1F("h1","",100,0.,360.);
      if(iDet == 0)
        tree1->Draw(plot_this,"","");
      else
        tree1->Draw(plot_this,"","goff");
      h1->SetLineColor(colorArray[iDet]);
      if(iDet == 0) {
        h1->SetMaximum((tree1->GetEntries()));
        h1->SetMinimum(.8);
        h1->DrawCopy();
      }
      else
        h1->DrawCopy("same");
      h1->Delete();
    }
  }
  makeText(.15,.92,.85,.98,"Phi by hit");

/////////////////////////////////////////////////////////////////////////
//-----------------------  Histograms for RICH  -----------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"RICH diagnostic histograms");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Draw();

Pad1 = new TPad("Pad1","Pad1",.001,.55,.5,.925);
Pad1->Draw();

Pad3 = new TPad("Pad3","Pad3",.5,.55,.999,.925);
Pad3->Divide(1,2,0.01,0.01);
Pad3->Draw();

Pad2 = new TPad("Pad2","Pad2",.001,.001,.999,.55);
Pad2->Divide(4,3,0.01,0.01);
Pad2->Draw();

//--------- RICH Hit scatter plot ---------//
Pad1->cd();
  TH2F *h2 = new TH2F("RingAmplvsTheta","RingAmplvsTheta",105,-1.05,1.05,105,-1.05,1.05);
  tree1->Draw("sin(HRichHit.fData.fTheta/57.296)*cos((HRichHit.fData.fPhi-90)/57.296):sin(HRichHit.fData.fTheta/57.296)*sin((HRichHit.fData.fPhi-90)/57.296)>>RingAmplvsTheta");
  h2->GetZaxis()->SetNdivisions(6);
  h2->GetXaxis()->SetLabelOffset(10000);
  h2->GetYaxis()->SetLabelOffset(10000);
  h2->GetXaxis()->SetTickLength(.00001);
  h2->GetYaxis()->SetTickLength(.00001);
  gPad->SetFrameLineColor(10);
  h2->GetXaxis()->SetAxisColor(10);
  h2->GetYaxis()->SetAxisColor(10);
  h2->DrawCopy("colz");
  h2->Delete();
  lin = new TLine(0,0,-1.05,0);
  lin->SetLineWidth(2);
  lin->Draw();
  cir = new TEllipse(0,0,1.03,1.03);
  cir->SetLineWidth(2);
  cir->Draw();
  TPaveLabel *pl = new TPaveLabel(-0.05,0.4,0.15,0.6,"#phi = 0","b1NDC");
  pl->SetBorderSize(0);
  pl->SetFillStyle(0);
  pl->SetTextSize(0.99);
  pl->SetTextAngle(90);
  pl->Draw();

  makeText(.2,.91,.8,.99,"Hit distribution (view from target)");

Pad3->cd(1);
  TH1F *h1 = new TH1F("Ring_theta","Ring_theta",100,0,100);
  tree1->Draw("HRichHit.fData.fTheta>>Ring_theta");
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->SetLineColor(richCol);
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#theta distribution");

Pad3->cd(2);
  TH1F *h1 = new TH1F("Ring_phi","Ring_phi",100,0,360);
  tree1->Draw("HRichHit.fData.fPhi>>Ring_phi");
  h1->SetLineColor(richCol);
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#phi distribution");

//--------- RICH various plots - Pad 2 ---------//
  TH1F *richHist1d[7];
    richHist1d[0] = new TH1F("Ring_radius","RING radius",80,2,6);
    richHist1d[1] = new TH1F("Centroid","Centroid",100,0,4);
    richHist1d[2] = new TH1F("RingChargeAmpl","Ring Charge Ampl",100,0,1000);
    richHist1d[3] = new TH1F("RingPadNr","Ring Pad Num",35,5,40);
    richHist1d[4] = new TH1F("RingCol","Ring Col",92,0,92);
    richHist1d[5] = new TH1F("RingRow","Ring Row",90,0,90);
    richHist1d[6] = new TH1F("RingLocMax4","Ring Loc Max4",15,0,15);
  Char_t *richHistDataMember[7] = {
    "HRichHit.fData.fRingRadius",
    "HRichHit.fData.fRingCentroid",
    "HRichHit.fData.iRingAmplitude",
    "HRichHit.fData.iRingPadNr",
    "HRichHit.fData.iRingX",
    "HRichHit.fData.iRingY",
    "HRichHit.fData.iRingLocalMax4" };

  for(Int_t i=0;i<7;i++) {
    Pad2->cd(i+1);
    sprintf(plot_this,"%s>>%s",richHistDataMember[i],richHist1d[i]->GetName());
    tree1->Draw(plot_this);
    richHist1d[i]->SetLabelSize(.06,"X");
    richHist1d[i]->SetLabelSize(.06,"Y");
    richHist1d[i]->SetLineColor(richCol);
    richHist1d[i]->DrawCopy();
    makeText(0.1,0.905,0.9,0.999,richHist1d[i]->GetTitle());
    richHist1d[i]->Delete();
  }

  TH2F *richHist2d[5];
    richHist2d[0] = new TH2F("RingQual_HT_PM","Ring Quality: HT vs. PM",100,0,200,100,0,800);
    richHist2d[1] = new TH2F("PatMatvsTheta","#theta vs. PM",100,200,800,90,0,90);
    richHist2d[2] = new TH2F("HouTravsTheta","#theta vs. HT",100,0,200,90,0,90);
    richHist2d[3] = new TH2F("RingAmplvsTheta","#theta vs. ring amplitude",100,0,1000,90,0,90);
    richHist2d[4] = new TH2F("RadiusvsTheta","#theta vs. radius",40,2,6,90,0,90);
  Char_t *richHistDataMember[5] = {
    "HRichHit.fData.iRingPatMat:HRichHit.fData.iRingHouTra",
    "HRichHit.fData.fTheta:HRichHit.fData.iRingPatMat",
    "HRichHit.fData.fTheta:HRichHit.fData.iRingHouTra",
    "HRichHit.fData.fTheta:HRichHit.fData.iRingAmplitude",
    "HRichHit.fData.fTheta:HRichHit.fData.fRingRadius" };

  for(Int_t i=0;i<5;i++) {
    Pad2->cd(i+8);
    sprintf(plot_this,"%s>>%s",richHistDataMember[i],richHist2d[i]->GetName());
    tree1->Draw(plot_this);
    TPaveText *pt = new TPaveText(0.1,0.905,0.9,0.999,"blNDC");
    pt->SetBorderSize(0);
    pt->SetFillColor(10);
    pt->AddText(richHist2d[i]->GetTitle());
    richHist2d[i]->DrawCopy("col");
    richHist2d[i]->Delete();
    pt->Draw();
  }

////////////////////////////////////////////////////////////////////////
//-----------------------  Histograms for MDC  -----------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"MDC diagnostic histograms");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Draw();

Pad1 = new TPad("Pad1","Pad1",.001,.475,.6,.925);
Pad1->Divide(2,2);
Pad1->Draw();

Pad2 = new TPad("Pad2","Pad2",.6,.475,.999,.925);
Pad2->Draw();

Pad4 = new TPad("Pad6","Pad6",.001,.001,.999,.475);
Pad4->Divide(3,2,0.01,0.01);
Pad4->Draw();


//--- This section is for the histograms ---//

Pad4->cd(1);
  gPad->SetLogy(1);
  sprintf(variable,"HMdcSeg.fData.z");
  sprintf(cond1,"%s","HMdcSeg.fData.z > -2000. && HMdcSeg.fData.z < 2000.");
  sprintf(plot_this,"%s>>xy1",variable);

  TH1F *h_xy1 = new TH1F("xy1","",100,-2000.,2000.);
  tree1->Draw(plot_this,cond1,"");
  h_xy1->SetLineColor(mdcCol);
  h_xy1->SetMinimum(.8);
  h_xy1->DrawCopy();
  h_xy1->Delete();
  makeText(0.1,0.905,0.9,0.999,"HMdcSeg:z");

Pad4->cd(2);
  sprintf(variable,"HMdcHit.fNDataObjs");
  sprintf(cond1,"%s","");
  sprintf(plot_this,"%s%s",variable,">>xy1");
  TH1F *h_xy1 = new TH1F("xy1","",60,-0.5,59.5);
  tree1->Draw(plot_this,cond1,"");
  h_xy1->SetLineColor(mdcCol);
  h_xy1->DrawCopy();
  h_xy1->Delete();
  makeText(0.1,0.905,0.9,0.999,"Total hits per event");

Pad4->cd(3);
  sprintf(variable,"HMdcHit.fData.xDir");
  sprintf(cond1,"%s","");
  sprintf(plot_this,"%s%s",variable,">>xy1");
  TH1F *h_xy1 = new TH1F("xy1","",60,-1,1);
  tree1->Draw(plot_this,cond1,"");
  h_xy1->SetLineColor(mdcCol);
  h_xy1->DrawCopy();
  h_xy1->Delete();
  makeText(0.1,0.905,0.9,0.999,"x Dir from hitfinder");

Pad4->cd(4);
  sprintf(variable,"HMdcHit.fData.yDir");
  sprintf(cond1,"%s","");
  sprintf(plot_this,"%s%s",variable,">>xy1");
  TH1F *h_xy1 = new TH1F("xy1","",60,-1,1);
  tree1->Draw(plot_this,cond1,"");
  h_xy1->SetLineColor(mdcCol);
  h_xy1->DrawCopy();
  h_xy1->Delete();
  makeText(0.1,0.905,0.9,0.999,"y Dir from hitfinder");

  TH2F *mdcHit2d[4];
  mdcHit2d[0] = new TH2F("mdcHit2d_0","Plane I (module=0)",100,-1000,1000,100,-1000,1000);
  mdcHit2d[1] = new TH2F("mdcHit2d_1","Plane II (module=1)",100,-1000,1000,100,-1000,1000);
  mdcHit2d[2] = new TH2F("mdcHit2d_2","Plane III (module=2)",100,-2000,2000,100,-2000,2000);
  mdcHit2d[3] = new TH2F("mdcHit2d_3","Plane IV (module=3)",100,-2500,2500,100,-2500,2500);

  TClonesArray *m_pMdcHit = new TClonesArray("HMdcHit");
  TBranch  *b_HMdcHit;
  b_HMdcHit = tree1->GetBranch("HMdcHit.fData");
  b_HMdcHit->SetAddress(&m_pMdcHit);

  tree1->SetBranchStatus("*",1);

  Int_t nentries = tree1->GetEntries();

  Int_t mdchits,row,col,mod,sec;
  Int_t modMult[4];
  Int_t nbytes =0, nb=0;
  Float_t x1,y1,x2,y2,angle;

  for(int i=0;i<nentries-1;i++) {
    modMult[0] = 0;
    modMult[1] = 0;
    modMult[2] = 0;
    modMult[3] = 0;
    m_pMdcHit->Clear();
    nb = b_HMdcHit->GetEntry(i); nbytes += nb;

    mdchits = m_pMdcHit->GetEntries();
    for(int j=0;j<mdchits;j++) {
      HMdcHit *pMdcHit = (HMdcHit*)m_pMdcHit->At(j);
      if(!pMdcHit) continue;

      pMdcHit->getSecMod(sec,mod);
      x1 = pMdcHit->getX();
      y1 = pMdcHit->getY();

      if(mod == 0) y1 = (y1 + 950)*.58;
      if(mod == 1) y1 = (y1 + 950)*.58;
      if(mod == 2) y1 = (y1 + 1800)*.58;
      if(mod == 3) y1 = (y1 + 1900)*.68;

      angle = ((float) sec)*60./57.2967;
      x2 = x1*cos(angle) + y1*sin(angle);
      y2 = -x1*sin(angle) + y1*cos(angle);

      mdcHit2d[mod]->Fill(x2,y2);
      modMult[mod]++;

    }
  }

Float_t HexSize[4] = {1000.,1000.,2000.,2500.};  // Distance from center to corner

for(mod=0;mod<4;mod++) {
Pad1->cd(mod+1);
  gPad->SetFrameLineColor(10);
  gPad->SetLeftMargin(.05);
  gPad->SetRightMargin(.05);
  gPad->SetTopMargin(.08);
  gPad->SetBottomMargin(.02);
  mdcHit2d[mod]->GetXaxis()->SetAxisColor(10);
  mdcHit2d[mod]->GetYaxis()->SetAxisColor(10);
  mdcHit2d[mod]->GetXaxis()->SetLabelOffset(10000);
  mdcHit2d[mod]->GetYaxis()->SetLabelOffset(10000);
  mdcHit2d[mod]->GetXaxis()->SetTickLength(.00001);
  mdcHit2d[mod]->GetYaxis()->SetTickLength(.00001);
  mdcHit2d[mod]->DrawCopy("col");

  makeHexagon(HexSize[mod]);
  makeText(0.1,0.9,0.9,0.98,mdcHit2d[mod]->GetTitle());

}

////////////////////////////////////////////////////////////////////////
//-----------------------  Histograms for TOF  -----------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"TOF + START diagnostic histograms");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Draw();

//--------- Pad definition ---------//
Pad1 = new TPad("Pad1","Pad1",.001,.55,.5,.925);
Pad1->Draw();

Pad3 = new TPad("Pad3","Pad3",.5,.55,.999,.925);
Pad3->Divide(1,2,0.01,0.01);
Pad3->Draw();

Pad2 = new TPad("Pad2","Pad2",.001,.17,.6,.55);
Pad2->Divide(2,2,0.01,0.01);
Pad2->Draw();

Pad4 = new TPad("Pad4","Pad4",.001,.001,.999,.17);
Pad4->SetLeftMargin(0.04);
Pad4->SetRightMargin(0.04);
Pad4->Draw();

Pad5 = new TPad("Pad5","Pad5",.6,.17,.999,.55);
Pad5->Draw();

//--------- TOF Hit scatter plot ---------//
Pad1->cd();
  TH2F *h2 = new TH2F("tofHitDist","tofHitDist",100,-2750,2750,100,-2750,2750);
  tree1->Draw("HTofHit.fData.ylab:(-HTofHit.fData.xlab)>>tofHitDist");
  gPad->SetFrameLineColor(10);
  h2->GetZaxis()->SetNdivisions(6);
  h2->GetXaxis()->SetLabelOffset(10000);
  h2->GetYaxis()->SetLabelOffset(10000);
  h2->GetXaxis()->SetTickLength(.00001);
  h2->GetYaxis()->SetTickLength(.00001);
  h2->GetXaxis()->SetAxisColor(10);
  h2->GetYaxis()->SetAxisColor(10);
  h2->DrawCopy("colz");
  h2->Delete();

  lin = new TLine(0,0,-2850,0);
  lin->SetLineWidth(2);
  lin->Draw();

  makeHexagon(2750);
  makeText(.2,.87,.8,.95,"Hit distribution (view from target)");

  TPaveLabel *pl = new TPaveLabel(-0.05,0.4,0.15,0.6,"#phi = 0","b1NDC");
  pl->SetBorderSize(0);
  pl->SetFillStyle(0);
  pl->SetTextSize(0.99);
  pl->SetTextAngle(90);
  pl->Draw();


Pad3->cd(1);
  TH1F *h1 = new TH1F("Tof_theta","Tof_theta",50,40,90);
  tree1->Draw("HTofHit.fData.theta>>Tof_theta");
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->SetLineColor(tofCol);
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#theta distribution");

Pad3->cd(2);
  TH1F *h1 = new TH1F("Tof_phi","Tof_phi",100,0,360);
  tree1->Draw("HTofHit.fData.phi>>Tof_phi");
  h1->SetLineColor(tofCol);
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#phi distribution");

//--------- tof various plots - Pad 2 ---------//

  TH1F *tofHist1d[4];
    tofHist1d[0] = new TH1F("htof","TOF time of flight",200,0,50);
    tofHist1d[1] = new TH1F("hmulttof","TOF multiplicity",65,-0.5,64.5);
    tofHist1d[2] = new TH1F("hstart","START time of flight",100,-10,10);
    tofHist1d[3] = new TH1F("hpatternstart","START strip pattern",8,0.5,8.5);
  Char_t *tofHistDataMember[4] = {
    "HTofHit.fData.tof",
    "HTofHit.fNDataObjs",
    "HStartCal.fData.time",
    "HStartCal.fData.strip" };

  for(Int_t i=0;i<4;i++) {
    Pad2->cd(i+1);
    sprintf(plot_this,"%s>>%s",tofHistDataMember[i],tofHist1d[i]->GetName());
    tree1->Draw(plot_this);
    tofHist1d[i]->SetLineColor(tofCol);
    tofHist1d[i]->SetLabelSize(.06,"X");
    tofHist1d[i]->SetLabelSize(.06,"Y");
    tofHist1d[i]->DrawCopy();
    makeText(0.1,0.905,0.9,0.999,tofHist1d[i]->GetTitle());
    tofHist1d[i]->Delete();
  }

//--------- tof pattern plot - Pad 4 ---------//
Pad4->cd();
   TH1F *hpatterntof= new TH1F("hpatterntof","pattern tof",384,0.5,384.5);
   tree1->Draw("HTofHit.fData.sector*64+HTofHit.fData.module*8+HTofHit.fData.cell+1>>hpatterntof");
   hpatterntof->SetLineColor(tofCol);
   hpatterntof->SetLabelSize(.06,"X");
   hpatterntof->SetLabelSize(.06,"Y");
   hpatterntof->GetXaxis()->SetNdivisions(555);
   hpatterntof->DrawCopy();
   hpatterntof->Delete();
   makeText(0.1,0.905,0.9,0.999,"TOF rod pattern: sector*64 + module*8 + cell + 1");



/////////////////////////////////////////////////////////////////////////
//--------------------  Histograms for Pre-Shower  --------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"Pre-Shower diagnostic histograms");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Draw();

Pad1 = new TPad("Pad1","Pad1",.001,.55,.5,.925);
Pad1->Draw();

Pad3 = new TPad("Pad3","Pad3",.5,.55,.999,.925);
Pad3->Divide(1,2,0.01,0.01);
Pad3->Draw();

Pad2 = new TPad("Pad2","Pad2",.001,.001,.999,.55);
Pad2->Divide(3,4,0.01,0.01);
Pad2->Draw();

//--------- SHOWER Hit scatter plot ---------//
Pad1->cd();
  TH2F *h2 = new TH2F("showerHitDist","showerHitDist",100,-2100,2100,100,-2100,2100);
  tree1->Draw("HShowerHit.fData.m_fLabY:(-HShowerHit.fData.m_fLabX)>>showerHitDist");
  gPad->SetFrameLineColor(10);
  h2->GetZaxis()->SetNdivisions(6);
  h2->GetXaxis()->SetLabelOffset(10000);
  h2->GetYaxis()->SetLabelOffset(10000);
  h2->GetXaxis()->SetTickLength(.00001);
  h2->GetYaxis()->SetTickLength(.00001);
  h2->GetXaxis()->SetAxisColor(10);
  h2->GetYaxis()->SetAxisColor(10);
  h2->DrawCopy("colz");
  h2->Delete();

  lin = new TLine(0,0,-2250,0);
  lin->SetLineWidth(2);
  lin->Draw();

  makeHexagon(2090.);

  TPaveLabel *pl = new TPaveLabel(-0.05,0.4,0.15,0.6,"#phi = 0","b1NDC");
  pl->SetBorderSize(0);
  pl->SetFillStyle(0);
  pl->SetTextSize(0.99);
  pl->SetTextAngle(90);
  pl->Draw();
  makeText(.2,.87,.8,.95,"Hit distribution (view from target)");

Pad3->cd(1);
  TH1F *h1 = new TH1F("Shower_theta","Shower_theta",50,5,55);
  tree1->Draw("HShowerHit.fData.m_fTheta>>Shower_theta");
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->SetLineColor(showerCol);
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#theta distribution");

Pad3->cd(2);
  TH1F *h1 = new TH1F("Shower_phi","Shower_phi",100,0,360);
  tree1->Draw("HShowerHit.fData.m_fPhi>>Shower_phi");
  h1->SetLineColor(showerCol);
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#phi distribution");

//--------- pre-shower various plots - Pad 2 ---------//

  TClonesArray *m_pShowerHit = new TClonesArray("HShowerHit");
  TBranch  *b_HShowerHit;
  b_HShowerHit = tree1->GetBranch("HShowerHit.fData");
  b_HShowerHit->SetAddress(&m_pShowerHit);

  TH2F *showerHist2d[10];
    showerHist2d[0] = new TH2F("showLMAvg1" ,"Average Charge on Loc. max. mod1 ",32,0,32,32,0,32);
    showerHist2d[1] = new TH2F("showLMAvg2" ,"Average Charge on Loc. max. mod2 ",32,0,32,32,0,32);
    showerHist2d[2] = new TH2F("showLMAvg3" ,"Average Charge on Loc. max. mod3 ",32,0,32,32,0,32);
    showerHist2d[3] = new TH2F("showSum1" ,"Average Sum around Loc. max. mod1 ",32,0,32,32,0,32);
    showerHist2d[4] = new TH2F("showSum2" ,"Average Sum around Loc. max. mod2 ",32,0,32,32,0,32);
    showerHist2d[5] = new TH2F("showSum3" ,"Average Sum around Loc. max. mod3 ",32,0,32,32,0,32);
    showerHist2d[6] = new TH2F("showLMFreq1" ,"Fire Frequency per entries  mod1 ",32,0,32,32,0,32);
    showerHist2d[7] = new TH2F("showLMFreq2" ,"Fire Frequency per entries  mod2 ",32,0,32,32,0,32);
    showerHist2d[8] = new TH2F("showLMFreq3" ,"Fire Frequency per entries  mod3 ",32,0,32,32,0,32);
    showerHist2d[9] = new TH2F("showSecMod" ,"Modules vs Sectors",11,-0.25,5.25,5,-0.25,2.25);

  tree1->SetBranchStatus("*",1);

  Int_t nentries = tree1->GetEntries();
  Int_t tempent = nentries;
  if(tempent>1000) nentries = 1000;
  else nentries = tempent;

  Int_t showhits,row,col,mod;
  Int_t nbytes =0, nb=0;

  for(int i=0;i<nentries;i++) {
    m_pShowerHit->Clear();
    nb = b_HShowerHit->GetEntry(i); nbytes += nb;

    showhits = m_pShowerHit->GetEntries();
    for(int j=0;j<showhits;j++) {
      HShowerHit *pShowerHit = (HShowerHit*)m_pShowerHit->At(j);
      if(!pShowerHit) continue;

      row = pShowerHit->getRow();
      col = pShowerHit->getCol();
      mod = pShowerHit->getModule();
      showSecMod->Fill(pShowerHit->getSector(),mod);

      showerHist2d[mod]->Fill(row,col,pShowerHit->getCharge());    // showLMAvgX
      showerHist2d[mod+3]->Fill(row,col,pShowerHit->getSum(mod));  // showSumX
      showerHist2d[mod+6]->Fill(row,col);                          // showLMFFreqX

    }
  }

  for(Int_t i=0;i<3;i++) {
    showerHist2d[i]->Divide(showerHist2d[i+6]);
    showerHist2d[i+3]->Divide(showerHist2d[i+6]);
    showerHist2d[i+6]->Scale(1./nentries);
  }

  for(Int_t i=0;i<10;i++) {
    Pad2->cd(i+1);
    showerHist2d[i]->SetLabelSize(.06,"X");
    showerHist2d[i]->SetLabelSize(.06,"Y");
    showerHist2d[i]->DrawCopy("col");
    makeText(0.1,0.905,0.9,0.999,showerHist2d[i]->GetTitle());
    showerHist2d[i]->Delete();
  }


/////////////////////////////////////////////////////////////////////////
//----------------------  Histograms for TOFINO  ----------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"TOFINO + Shower/TOFINO histograms");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad","Top Pad",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Draw();

Pad1 = new TPad("Pad1","Pad1",.001,.55,.5,.925);
Pad1->Draw();

Pad3 = new TPad("Pad3","Pad3",.5,.55,.999,.925);
Pad3->Divide(1,2,0.01,0.01);
Pad3->Draw();

Pad2 = new TPad("Pad2","Pad2",.001,.001,.999,.55);
Pad2->Divide(3,4,0.01,0.01);
Pad2->Draw();

//--- This section is for the histograms ---//
Pad1->cd();
  TH2F *h2 = new TH2F("showerHitDist","showerHitDist",100,-2050,2050,100,-2050,2050);
  tree1->Draw("HShowerHitTof.fData.m_fLabY:(-HShowerHitTof.fData.m_fLabX)>>showerHitDist");
  gPad->SetFrameLineColor(10);
  h2->GetZaxis()->SetNdivisions(6);
  h2->GetXaxis()->SetLabelOffset(10000);
  h2->GetYaxis()->SetLabelOffset(10000);
  h2->GetXaxis()->SetTickLength(.00001);
  h2->GetYaxis()->SetTickLength(.00001);
  h2->GetXaxis()->SetAxisColor(10);
  h2->GetYaxis()->SetAxisColor(10);
  h2->DrawCopy("colz");
  h2->Delete();

  lin = new TLine(0,0,-2150,0);
  lin->SetLineWidth(2);
  lin->Draw();

  makeHexagon(2050.);

  TPaveLabel *pl = new TPaveLabel(-0.05,0.4,0.15,0.6,"#phi = 0","b1NDC");
  pl->SetBorderSize(0);
  pl->SetFillStyle(0);
  pl->SetTextSize(0.99);
  pl->SetTextAngle(90);
  pl->Draw();
  makeText(.1,.87,.85,.95,"Tofino/Shower Hits (in HShowerHitTof)");

Pad3->cd(1);
  TH1F *h1 = new TH1F("Shower_theta","Shower_theta",50,5,55);
  tree1->Draw("HShowerHitTof.fData.m_fTheta>>Shower_theta");
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->SetLineColor(tofinoCol);
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#theta distribution (HShowerHitTof)");

Pad3->cd(2);
  TH1F *h1 = new TH1F("Shower_phi","Shower_phi",100,0,360);
  tree1->Draw("HShowerHitTof.fData.m_fPhi>>Shower_phi");
  h1->SetLineColor(tofinoCol);
  h1->SetLabelSize(.06,"X");
  h1->SetLabelSize(.06,"Y");
  h1->DrawCopy();
  h1->Delete();
  makeText(0.1,0.901,0.9,0.999,"#phi distribution (HShowerHitTof)");

  TH1F *tofinoHist1d[5];
    tofinoHist1d[0] = new TH1F("tofinoPattern","TOFINO pattern",24,-0.5,23.5);
    tofinoHist1d[1] = new TH1F("tofinoTdc","TOFINO tdc",100,-10,150);
    tofinoHist1d[2] = new TH1F("tofinoTof","TOFINO time of flight (HShowerHitTof)",100,0,100);
    tofinoHist1d[3] = new TH1F("tofinoMult","TOFINO multiplicity",51,-.5,50.5);
    tofinoHist1d[4] = new TH1F("tofinoHitMult","HShowerHitTof multiplicity",51,-.5,50.5);
  Char_t *tofinoHistDataMember[5] = {
    "(HTofinoCal.fData.cell+HTofinoCal.fData.sector*4)",
    "HTofinoCal.fData.timeCal",
    "HShowerHitTof.fData.m_fTof",
    "HTofinoCal.fNDataObjs",
    "HShowerHitTof.fNDataObjs" };
  for(Int_t i=0;i<5;i++) {
    Pad2->cd(i+1);
    sprintf(plot_this,"%s>>%s",tofinoHistDataMember[i],tofinoHist1d[i]->GetName());
    tree1->Draw(plot_this);
    tofinoHist1d[i]->SetLineColor(tofinoCol);
    tofinoHist1d[i]->DrawCopy();
    tofinoHist1d[i]->SetLabelSize(.06,"X");
    tofinoHist1d[i]->SetLabelSize(.06,"Y");
    makeText(0.1,0.905,0.9,0.999,tofinoHist1d[i]->GetTitle());
    tofinoHist1d[i]->Delete();
  }

  Int_t sec;
  TH2F *tofinoHist2d[6];
  for(sec=0;sec<6;sec++) {
  Pad2->cd(sec+7);
    gPad->SetRightMargin(0.15);
    sprintf(variable,"TofinoCal-ShowerHitTof Corr: Sec. %i",sec);
    sprintf(plot_this,"tofinoShowerCorrSec%i",sec);
    tofinoHist2d[sec] = new TH2F(plot_this,variable,4,-0.5,3.5,4,-0.5,3.5);

    sprintf(cond1,"HTofinoCal.fData.sector==HShowerHitTof.fData.m_nSector && HShowerHitTof.fData.m_nSector == %i",sec);
    sprintf(plot_this,"%s>>%s","HTofinoCal.fData.cell:HShowerHitTof.fData.m_nTofinoCell",tofinoHist2d[sec]->GetName());
    tree1->Draw(plot_this,cond1);
    tofinoHist2d[sec]->DrawCopy("colz");
    tofinoHist2d[sec]->SetMinimum(1.);
    tofinoHist2d[sec]->SetLabelSize(.06,"X");
    tofinoHist2d[sec]->SetLabelSize(.06,"Y");
    makeText(0.1,0.905,0.9,0.999,tofinoHist2d[sec]->GetTitle());
    tofinoHist2d[sec]->Delete();
  }



////////////////////////////////////////////////////////////////////////
//--------------------  Histograms for KickPlane  --------------------//
window->Update();
ps->NewPage();
window->Clear();

sprintf(pageTitle,"Kick plane diagnostic histograms");
makePageTitle(fileName,dirName,nEntries,pageTitle);

//--------- Pad definition ---------//
topPad = new TPad("topPad1","Top Pad2",.001,.001,.999,.925);
topPad->SetBorderMode(0);
topPad->Divide(1,2,0.01,0.01);
topPad->Draw();

//--- This section is for the histograms ---//

  TH1F *kickHist1d[6];
    kickHist1d[0] = new TH1F("mass_mod0","Mass (sector 0)",100,0,3000);
    kickHist1d[1] = new TH1F("mass_mod1","Mass (sector 1)",100,0,3000);
    kickHist1d[2] = new TH1F("mass_mod2","Mass (sector 2)",100,0,3000);
    kickHist1d[3] = new TH1F("mass_mod3","Mass (sector 3)",100,0,3000);
    kickHist1d[4] = new TH1F("mass_mod4","Mass (sector 4)",100,0,3000);
    kickHist1d[5] = new TH1F("mass_mod5","Mass (sector 5)",100,0,3000);

  TClonesArray *m_pKickTrack = new TClonesArray("HKickTrack");
  TBranch  *b_HKickTrack;
  b_HKickTrack = tree1->GetBranch("HKickTrack.fData");
  b_HKickTrack->SetAddress(&m_pKickTrack);

  tree1->SetBranchStatus("*",1);

  Int_t nentries = tree1->GetEntries();
  Int_t tempent = nentries;
  if(tempent>10000) nentries = 10000;
  else nentries = tempent;

  Int_t kickhits,row,col,mod,sec,mass;
  Int_t modMult[4];
  Int_t nbytes =0, nb=0;

  for(int i=0;i<nentries;i++) {
    m_pKickTrack->Clear();
    nb = b_HKickTrack->GetEntry(i); nbytes += nb;

    kickhits = m_pKickTrack->GetEntries();
    for(int j=0;j<kickhits;j++) {
      HKickTrack *pKickTrack = (HKickTrack*)m_pKickTrack->At(j);
      if(!pKickTrack) continue;

      sec = pKickTrack->getSector();
      mass = sqrt(pKickTrack->getMass());
      if(mass<3000) kickHist1d[sec]->Fill(mass);

    }
  }

  topPad->cd(1);

  for(sec=0;sec<6;sec++) {
    kickHist1d[sec]->SetLineColor(colorArray[sec]);
    if(sec == 0) kickHist1d[sec]->DrawCopy();
    if(sec != 0) kickHist1d[sec]->DrawCopy("same");
    kickHist1d[sec]->Delete();
  }
  makeText(0.1,0.905,0.9,0.96,"Mass distribution");



/////////////////////////////////////////////////////////////////////////
//-------------  Write psfile and pdffile & close infile  -------------//

printf("\n   Writing ps file: %s\n",psfile);
ps.Close();

Char_t command[150];
sprintf(command,"%s%s%s%s","ps2pdf ",psfile," ",pdffile);
printf("   Writing pdf file: %s\n\n",pdffile);
system(command);

file1->Close();
}

/////////////////////////////////////////////////////////////////////////
//---------------------------------------------------------------------//
//----------  Function to fill title pades at top of page  ------------//
//---------------------------------------------------------------------//
/////////////////////////////////////////////////////////////////////////

void makePageTitle(Char_t fileName[120],Char_t dirName[120],
                      Int_t nEntries, Char_t pageTitle[120]) {
  Char_t fileTitle[120],dateTitle[120],dirTitle[120],entriesTitle[120];

  sprintf(fileTitle,"%s%s","DST file = ",fileName);
  sprintf(dirTitle,"%s%s","DST dir = ",dirName);
  sprintf(entriesTitle,"%s%i","Number of events = ",nEntries);

  Char_t dt[25];
  TDatime tt;
  strcpy(dt,tt.AsString());
  sprintf(dateTitle,"%s%s","This file generated ",dt);

  TPaveText *pt = new TPaveText(0.01,0.975,0.99,0.999,"blNDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(10);
  pt->AddText("DST diagnostic histograms");
  pt->Draw();

  TPaveText *pt = new TPaveText(0.01,0.926,0.49,0.975,"blNDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(10);
  pt->SetTextAlign(13);
  pt->SetTextSize(.02);
  pt->AddText(fileTitle);
  pt->AddText(dirTitle);
  pt->AddText(entriesTitle);
  pt->Draw();

  TPaveText *pt = new TPaveText(0.51,0.926,0.99,0.975,"blNDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(10);
  pt->SetTextAlign(13);
  pt->SetTextSize(.020);
  pt->AddText(dateTitle);
  TText *text = pt->AddText(pageTitle);
  text->SetTextSize(.025);
  pt->Draw();
}

void makeHexagon(Float_t cornerDist) {
  lin = new TLine(-cornerDist,0,-cornerDist/2.0,cornerDist/1.1544);
  lin->SetLineWidth(2);
  lin->Draw();
  lin = new TLine(-cornerDist/2.0,cornerDist/1.1544,cornerDist/2.0,cornerDist/1.1544);
  lin->SetLineWidth(2);
  lin->Draw();
  lin = new TLine(cornerDist/2.0,cornerDist/1.1544,cornerDist,0);
  lin->SetLineWidth(2);
  lin->Draw();
  lin = new TLine(cornerDist,0,cornerDist/2.0,-cornerDist/1.1544);
  lin->SetLineWidth(2);
  lin->Draw();
  lin = new TLine(cornerDist/2.0,-cornerDist/1.1544,-cornerDist/2.0,-cornerDist/1.1544);
  lin->SetLineWidth(2);
  lin->Draw();
  lin = new TLine(-cornerDist/2.0,-cornerDist/1.1544,-cornerDist,0);
  lin->SetLineWidth(2);
  lin->Draw();
}

void makeText(Float_t x1,Float_t y1,Float_t x2,Float_t y2,Char_t *text) {
  TPaveText *pt = new TPaveText(x1,y1,x2,y2,"blNDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(10);
  pt->AddText(text);
  pt->Draw();
}
