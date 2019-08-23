#include <iostream.h>
#include <math.h>
//---- Root includes
#include <TCanvas.h>
#include <TColor.h>
#include <TDatime.h>
#include <TFile.h>
#include <TH2.h>
#include <TPaveText.h>
#include <TPostScript.h>
#include <TProfile.h>
#include <TROOT.h>
#include <TString.h>
#include <TStyle.h>
#include <TSystem.h>
//---- Hydra includes
#include "hades.h"
#include "hqaoutputps.h"
#include "hqascalers.h"

//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//
//*-- Author : Dan Magestro (magestro@gsi.de)
//*-- Modified : 24/01/2002
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-//

//////////////////////////////////////////////////////////////////////////////
//
// HQAOutputPS
//
// This class creates a postscript file using TPostScript, and histograms are
// added by iterating over TList(s) of active histograms.
// Histograms must be of type TH1F, TH2F, or TProfile.
//
// Scalers are added with a TList of HQAScaler and HQAScaler6 objects.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/qa/">QA home page</a>.</font></b> END_HTML
//
//////////////////////////////////////////////////////////////////////////////

ClassImp(HQAOutputPS)

HQAOutputPS::HQAOutputPS(TString fname) {
// Default constructor, filename must be supplied
  fnamePS = fname;
  fCanvas = 0;
  fHistPad = 0;
  histCount = 0;
  pageCount = 0;
  nEvent = 0;
  nProcessed = 0;
  nHistPerPage = 6;
  kFIRST = kTRUE;
  kPDF = kTRUE;
}

HQAOutputPS::~HQAOutputPS() {
// Default destructor
  if(fCanvas) closePS();

  if(kPDF) makePDF();
}

void HQAOutputPS::writeHist(TList *histList) {
  // Output histograms to Root file (not really implemented yet)

  TFile *ff = new TFile("ff.root","RECREATE");
  ff->cd();

  TIter next(histList);
  while (TH1 *h = (TH1*) next()) {
    if( !strcmp(h->ClassName(),"TH1F") )
       ((TH1F*)h)->Write();
    else if( !strcmp(h->ClassName(),"TH2F") )
       ((TH2F*)h)->Write();
    else if( !strcmp(h->ClassName(),"TProfile") )
      ((TProfile*)h)->Write();
  }
  cout << "-----------------------------------------" << endl;
  cout << "|  Before the closing the file ff.root  |" << endl;
  cout << "-----------------------------------------" << endl;
  ff->Close();
}

void HQAOutputPS::writeHist(TList *histList1, TList *histList2, TString fnamePS) {
  // Output histograms to Root file (not really implemented yet)
  // modified for 2 TLists to be plotted at ones


  TString histFile = fnamePS(0,fnamePS.Last('.'))+"_hist.root";
  //FILE*  fSC = fopen( histFile, "w");
  //TFile *ff = new TFile("ff.root","RECREATE");
  TFile *ff = new TFile( histFile,"RECREATE");
  ff->cd();

  {
  TIter next(histList1);
  while (TH1 *h = (TH1*) next()) {
    if( !strcmp(h->ClassName(),"TH1F") )
       ((TH1F*)h)->Write();
    else if( !strcmp(h->ClassName(),"TH2F") )
       ((TH2F*)h)->Write();
    else if( !strcmp(h->ClassName(),"TProfile") )
      ((TProfile*)h)->Write();
  }
  }

  {
  TIter next(histList2);
  while (TH1 *h = (TH1*) next()) {
    if( !strcmp(h->ClassName(),"TH1F") )
       ((TH1F*)h)->Write();
    else if( !strcmp(h->ClassName(),"TH2F") )
       ((TH2F*)h)->Write();
    else if( !strcmp(h->ClassName(),"TProfile") )
      ((TProfile*)h)->Write();
  }
  }

  ff->Close();
}


void HQAOutputPS::makeHist(TList *histList) {
// This is the class's main function.  The canvas is defined, and the TList of
// histograms is iterated.  Each histogram is drawn according to its type.

  TString type,typeKeep;

  // Open file if this is the first call to make()
  if(kFIRST) {
    setStyle();
    TVirtualPS *temp=0;

    // This code is ugly, but it gets around a TPad/TPostScript problem and I'm
    //    working on fixing it   (D.Magestro 18/2/02)
    if(gVirtualPS) {
      temp = gVirtualPS;
      gVirtualPS = 0;
    }
    fCanvas = new TCanvas("canvas","HADES diagnostics",200,320);
    if(!gVirtualPS) {
      gVirtualPS = temp;
    }
    Open(fnamePS,111);
  }

  // QA histograms
  TIter next(histList);

  while (TH1 *h = (TH1*) next()) {

    TString opt = next.GetOption();
    opt.ToLower();

    if( opt.Contains("same") ){
      //do not ask for a new panel
    }else{
      //ask for a new panel
      histCount++;
    }


    if(opt.Index("--")>0) {
      type = opt(0,opt.Index("--"));
      if( (type.CompareTo(typeKeep)) ) {
        if(!kFIRST) fCanvas->Update();
        histCount = 1;
        pageCount = 0;
        makeNewPage(type);
        typeKeep = type;
      }
    }

    if(histCount > nHistPerPage || opt.Contains("newpage")) {
      fCanvas->Update();
      makeNewPage(type);
      histCount = 1;
    }

    fHistPad->cd(histCount);

    // Adjust left,right margins for (1,3) division
    if(fHistPad->GetListOfPrimitives()->GetSize()==3) {
      gPad->SetLeftMargin(0.05);
      gPad->SetRightMargin(0.05);
    }

    // Set log scale, if histogram isn't empty and option is set
    if( h->GetEntries() ) {
      if( opt.Contains("logx") )  gPad->SetLogx(1);
      if( opt.Contains("logy") )  gPad->SetLogy(1);
      if( opt.Contains("logz") )  gPad->SetLogz(1);
    }

    h->SetLineWidth(2);

    // Format histograms
    if( opt.Contains("same") ){
      //do no assign any color, it must be specified in the histogramm definition
    }else{
       if( !strcmp(h->ClassName(),"TProfile") )
          h->SetLineColor(1);
       else if( !strcmp(h->ClassName(),"TH1F") )
          h->SetLineColor(4);
    }

    // Format axis titles
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
    h->SetTitleOffset(1.1,"X");
    h->SetTitleOffset(1.1,"Y");
    h->SetTitleSize(.05,"X");
    h->SetTitleSize(.05,"Y");
    h->SetNdivisions(208,"X");
    h->SetNdivisions(208,"Y");

    // Set axis ranges so that variation histogram is full and looks nice
    if( opt.Contains("var") ) {
      Int_t binMax = h->FindBin(nEvent);
      TProfile *prof = (TProfile*) h;
      if(binMax < 200) {
        Float_t xMax = h->GetBinCenter( binMax+1 );
        h->SetAxisRange(0,xMax);
      }
      Float_t min=10000,max=0;
      Float_t cont=0,err=0;
      for(Int_t i=1;i<=binMax;i++) {
        cont=prof->GetBinContent(i);
        err=prof->GetBinError(i);
        if((cont+err)>max) max=cont+err;
        if((cont-err)<min) min=cont-err;
      }
      prof->SetMaximum(max + (max-min));
      prof->SetMinimum(min - (max-min));
      if((min-(max-min))<0) prof->SetMinimum(0);
    }

    // Set options depending on histogram type
    if( !strcmp(h->ClassName(),"TH2F") ) h->SetOption("colz");

    // Overlay options are quiet usefull.
    //if( opt.Contains("same") ) h->SetOption("same");

    // Draw histograms
    if( opt.Contains("same") ){
       h->DrawCopy("same");
    }else{    
       h->DrawCopy();
    }

    // Draw the mean on the profile histogram plots as a horiz. line
    if( !strcmp(h->ClassName(),"TProfile") ) {
      TProfile *prof = (TProfile*) h->Rebin(200,"prof");
      Float_t mean = prof->GetBinContent(1);
      TLine *l = new TLine(0.,mean,nEvent,mean);
      l->SetLineWidth(2);
      l->SetLineColor(4);
      l->Draw();
    }

    if(kFIRST) kFIRST=kFALSE;
  }

}

void HQAOutputPS::makeText(TList *scalerList) {
// Method for generating output for scalers

  TString type,typeKeep;
  Char_t out[200];

  if(fCanvas) fCanvas->Update();

  // Open file if this is the first call to make()
  if(kFIRST) {
    setStyle();
    fCanvas = new TCanvas("canvas","HADES diagnostics",200,320);
    Open(fnamePS,111);
  }

  pageCount = 0;
  nHistPerPage = 1;
  makeNewPage("Scalers");
  fHistPad->cd(1);

  //---- Sectorwise scalers
  Int_t nLines = 0;

  TIter next(scalerList);
  next.Reset();
  while (TNamed *t = (TNamed*) next()) {
    TString opt = next.GetOption();
    opt.ToLower();
    if(opt.Contains("sectorwise")) {
      nLines++;
      if(opt.Index("--")>0) {
        type = opt(0,opt.Index("--"));
        if( (type.CompareTo(typeKeep)) ) {  // Add line between detector types
          typeKeep = type;
          nLines++;
        }
      }
    }
  }
  typeKeep = "";

  // Define text box
  TPaveText *pt = new TPaveText(0.01,0.98-0.02*nLines,0.96,0.98,"b1NDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(10);
  pt->SetTextAlign(13);
  pt->SetTextSize(0.018);
  pt->SetMargin(0.01);
  pt->SetTextFont(102);

  next.Reset();
  while (TNamed *h = (TNamed*) next()) {
    TString opt = next.GetOption();
    opt.ToLower();

    if( opt.Contains("sectorwise") ) {

      HQAScaler6 &scal = *((HQAScaler6*) h);

      if(opt.Index("--")>0) {
        type = opt(0,opt.Index("--"));
        if( (type.CompareTo(typeKeep)) ) {
          sprintf(out,"                                           Sec.0   Sec.1   Sec.2   Sec.3   Sec.4   Sec.5");
          nLines++;
          TText *t = pt->AddText(out);
          t->SetTextColor(4);
          typeKeep = type;
        }
      }

      sprintf(out,"%40s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f",
         scal.GetTitle(),scal[0],scal[1],scal[2],scal[3],scal[4],scal[5]);
      pt->AddText(out);
    }

    if(kFIRST) kFIRST=kFALSE;
  }
  pt->Draw();

}


void HQAOutputPS::saveScal(TList *scalerList, TString fnamePS) {
// Derived from //Method for generating output for scalers
// Used to save scalers into txt-file

  TString type,typeKeep;
  //Char_t out[200];

  TString txtFile = fnamePS(0,fnamePS.Last('.'))+".txt";
  FILE*  fSC = fopen( txtFile, "w");

  //---- Sectorwise scalers
  Int_t nLines = 0;

  TIter next(scalerList);
  next.Reset();
  while (TNamed *t = (TNamed*) next()) {
    TString opt = next.GetOption();
    opt.ToLower();
    if(opt.Contains("sectorwise")) {
      nLines++;
      if(opt.Index("--")>0) {
        type = opt(0,opt.Index("--"));
        if( (type.CompareTo(typeKeep)) ) {  // Add line between detector types
          typeKeep = type;
          nLines++;
        }
      }
    }
  }
  typeKeep = "";

  //cout <<          "                                           Sec.0   Sec.1   Sec.2   Sec.3   Sec.4   Sec.5" << endl;
  fprintf(fSC,"%s","                                           Sec.0   Sec.1   Sec.2   Sec.3   Sec.4   Sec.5\n");


  next.Reset();
  while (TNamed *h = (TNamed*) next()) {
    TString opt = next.GetOption();
    opt.ToLower();

    if( opt.Contains("sectorwise") ) {

      HQAScaler6 &scal = *((HQAScaler6*) h);

      //sprintf(out,"%40s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f",
      //   scal.GetTitle(),scal[0],scal[1],scal[2],scal[3],scal[4],scal[5]);
      //cout << out << endl;
      fprintf(fSC,"%40s %7.2f %7.2f %7.2f %7.2f %7.2f %7.2f \n",
         scal.GetTitle(),scal[0],scal[1],scal[2],scal[3],scal[4],scal[5]);
    }

  }
  fclose( fSC );

}


void HQAOutputPS::closePS() {
// Closes files and deletes the canvas
  fCanvas->Update();

  Close();
  delete fCanvas;
}

void HQAOutputPS::makePDF() {
// Execute ps2pdf GhostScript command on closed PS file, delete PS file

  TString fnamePDF = fnamePS(0,fnamePS.Last('.'))+".pdf";

  TString command = "ps2pdf " + fnamePS + " " + fnamePDF;

  gSystem->Exec(command);

  command = "rm " + fnamePS;

  gSystem->Exec(command);

}


void HQAOutputPS::makeNewPage(TString type) {
// In addition to making a new page in the PS file, this function writes the
// header at the top of each page and defines the histogram pad.

  Char_t pageTitle[120],fileTitle[120],dateTitle[120],eventTitle[120],procTitle[120];

  NewPage();
  pageCount++;

  fCanvas->Clear();

  type.ToUpper();

  if(fnameDST.Contains("/"))
      fnameDST = fnameDST( fnameDST.Last('/')+1,fnameDST.Length() );

  if(type.Contains("SCALER")) sprintf(pageTitle,"Scalers (page %i)",pageCount);
  else sprintf(pageTitle,"%s histograms (page %i)",type.Data(),pageCount);

  sprintf(fileTitle,"%s%s","DST file = ",fnameDST.Data());
  sprintf(eventTitle,"%s%i","Number of events in file= ",(int)nEvent);
  sprintf(procTitle,"Number of QA'd events = %i (%.0f%% sampled)", nProcessed,
         (float) nProcessed/nEvent*100. );

  Char_t dt[25];
  TDatime tt;
  strcpy(dt,tt.AsString());
  sprintf(dateTitle,"%s%s","This page generated ",dt);

  TPaveText *pt = new TPaveText(0.01,0.975,0.99,0.999,"blNDC");
  pt->SetBorderSize(0);
  pt->SetFillColor(10);
  pt->AddText("HADES QA Report");
  pt->Draw();

  TPaveText *pt1 = new TPaveText(0.01,0.926,0.49,0.975,"blNDC");
  pt1->SetBorderSize(0);
  pt1->SetFillColor(10);
  pt1->SetTextAlign(13);
  pt1->SetTextSize(.02);
  pt1->AddText(fileTitle);
  pt1->AddText(eventTitle);
  pt1->AddText(procTitle);
  pt1->Draw();

  TPaveText *pt2 = new TPaveText(0.51,0.926,0.99,0.975,"blNDC");
  pt2->SetBorderSize(0);
  pt2->SetFillColor(10);
  pt2->SetTextAlign(13);
  pt2->SetTextSize(.02);
  pt2->AddText(dateTitle);
  TText *text = pt2->AddText(pageTitle);
  text->SetTextSize(.025);
  pt2->Draw();

  // Divide remaining area of canvas into sub-pads
  fHistPad = new TPad("histPad","Histogram pad",.001,.001,.999,.925);
  if(nHistPerPage == 1) fHistPad->Divide(1,1);
  if(nHistPerPage == 3) fHistPad->Divide(1,3);
  if(nHistPerPage == 4) fHistPad->Divide(2,2);
  if(nHistPerPage == 6) fHistPad->Divide(2,3);
  if(nHistPerPage == 8) fHistPad->Divide(2,4);
  fHistPad->Draw();

}


void HQAOutputPS::setStyle() {
// Set drawing styles for histograms and PostScript file

  gStyle->SetOptStat(0);

  // Add an averaged color between grey and purple
  TColor *col =  (TColor*) (gROOT->GetListOfColors()->At(51));
  col->SetRGB(.636666,.35,.85);

  // Optimized "pretty palette" to distribute colors better
  Int_t colorArray[30] = {10,19,18,17,16,
                          51,52,54,57,60,
                          62,64,66,67,68,
                          70,72,74,80,86,
                          88,89,90,91,92,
                          93,94,96,98,100};

  gStyle->SetPalette(30,colorArray);

  // Histogram title formatting
  gStyle->SetTitleTextColor(4);
  gStyle->SetTitleColor(10);
  gStyle->SetTitleBorderSize(0);
  gStyle->SetTitleX(.2);
  gStyle->SetTitleY(.97);
  gStyle->SetTitleH(.065);
  gStyle->SetTitleW(.6);

  // Canvas and pad formatting
  gStyle->SetPadColor(10);
  gStyle->SetPadBorderMode(0);
  gStyle->SetCanvasColor(10);
  gStyle->SetCanvasBorderMode(0);

  gStyle->SetPaperSize(18.,27.);
}
