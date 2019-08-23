#include <stdio.h>
#include <iostream.h>
#include <fstream.h>
#include <iomanip.h>
#include <stdlib.h>

#include <TApplication.h>
#include <TCanvas.h>
#include <TError.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <THStack.h>
#include <TLatex.h>
#include <TMath.h>
#include <TObject.h>
#include <TPaveText.h>
#include <TPad.h>
#include <TRFIOFile.h>
#include <TROOT.h>
#include <TStopwatch.h>
#include <TSystem.h>
#include <TString.h>
#include <TStyle.h>
#include <TTree.h>
#include <TUnixSystem.h>

////////////////////////////////////////////////////////////////////////////////
//
// Peter's Tools
// 
//            Author: Peter W.Zumbruch
//           Contact: P.Zumbruch@gsi.de
//           Created: Mar 21, 2002
// Last modification: Aug 13, 2002
// 
////////////////////////////////////////////////////////////////////////////////

#include "ptools.h"

ClassImp(PTools)

TPad* PTools::getDividedSubPad(TCanvas *canv, 
				      Int_t xpads, 
				      Int_t ypads) 
{
  return getDividedSubPad(canv, xpads, ypads, (Char_t*)0, (Char_t*)0, 0, (TPaveText*)0 );
}

TPad* PTools::getDividedSubPad(TCanvas *canv, 
				      Int_t xpads, 
				      Int_t ypads,			       
				      const Char_t * title) 
{
  return getDividedSubPad(canv, xpads, ypads, title, (Char_t*)0, 0, (TPaveText*)0 );
}

TPad* PTools::getDividedSubPad(TCanvas *canv, 
				      Int_t xpads, 
				      Int_t ypads,			       
				      const Char_t * title,
				      const Char_t * filename) 
{
  return getDividedSubPad(canv, xpads, ypads, title, filename, 0, (TPaveText*)0 );
}

TPad* PTools::getDividedSubPad(TCanvas  * canv, 
				      Int_t xpads, 
				      Int_t ypads, 
				      const Char_t * title, 
				      const Char_t * filename, 
				      Int_t nentries)
{
  return getDividedSubPad(canv, xpads, ypads, title, filename, nentries,(TPaveText*)0 );
}

TPad* PTools::getDividedSubPad(TCanvas  * canv,
				      Int_t xpads, 
				      Int_t ypads, 
				      const Char_t * title, 
				      const Char_t * filename,
				      Int_t nentries, 
				      const Char_t * info)
{
  TPaveText *infotext = new TPaveText(0,0,1,1);
  infotext->SetTextSize(10);
  infotext->AddText(info);
  return getDividedSubPad(canv, xpads, ypads, title, filename, nentries, infotext );
};

TPad* PTools::getDividedSubPad(TCanvas  * canv, 
				      Int_t xpads, 
				      Int_t ypads, 
				      const Char_t * title, 
				      const Char_t * filename, 
				      Int_t nentries, 
				      const TString info)
{
  return getDividedSubPad(canv , xpads, ypads, title, filename, nentries, info.Data());
};

TPad* PTools::getDividedSubPad(TCanvas   *mycanvas,
				      Int_t      xpads, 
				      Int_t      ypads, 
				      const Char_t  *titlehead, 
				      const Char_t  *mytextfilename, 
				      Int_t      nentries, 
				      TPaveText *info)
{
  Char_t name[200];
  mycanvas->cd();
  sprintf(name,"%s_header",gPad->GetName());
  mycanvas->SetFillColor(0);
  
  TPad *mycanvas_header = new TPad(name,name,0.01,0.95,0.99,0.99);
  mycanvas_header->Draw();
  mycanvas_header->cd();
  
  printHeader(titlehead,mytextfilename,nentries,mycanvas_header);
  
  mycanvas->cd();
  
  TPad *mycanvas_body;
  
  if (info)
    {
      sprintf(name,"%s_info",gPad->GetName());
      TPad *mycanvas_info = new TPad(name,name,0.80,0.01,0.99,0.94);
      mycanvas_info->Draw();
      mycanvas_info->cd();
      gPad->SetFillStyle(4000);
      
      info->SetX1NDC(0.051);
      info->SetY1NDC(0.02);
      info->SetX2NDC(0.95);
      info->SetY2NDC(0.98);
      info->SetTextSize(0.08);
      info->SetBorderSize(0);
      while (info->GetListOfLines()->LastIndex() < 2*(Int_t)((1/info->GetTextSize())-2)) info->AddText(" ");
      info->SetFillColor(0);
      info->Draw();
      
      mycanvas->cd();
      
      sprintf(name,"%s_body",gPad->GetName());
      
      mycanvas_body = new TPad(name,name,0.01,0.01,0.79,0.94);
    }
  else 
    {
      sprintf(name,"%s_body",gPad->GetName());
      mycanvas_body = new TPad(name,name,0.01,0.01,0.99,0.94);
    }
  
  mycanvas_body->Draw();
  mycanvas_body->cd();
  gPad->SetFillStyle(4000);
  
  if (xpads > 1 || ypads > 1) mycanvas_body->Divide(xpads,ypads);
  
  return mycanvas_body;
}

void PTools::create4Histogramms(TH1F** hists, 
				const Char_t *name, const Char_t *title, const Char_t *xtitle, const Char_t *ytitle,
				Int_t bins,double min,double max)
{
  char myname[4][200],mytitle[4][200];

  sprintf(myname[0] ,"%sSector%i%i",name,25,35);
  sprintf(mytitle[0],"%s for Sector %i",title,5);

  sprintf(myname[1] ,"%sPhi%i%i",name,25,35);
  sprintf(mytitle[1],"%s for %3.1f <#phi < %3.1f",title,25.,35.);

  sprintf(myname[2] ,"%sTheta%i%i",name,50,60);
  sprintf(mytitle[2],"%s for %3.1f <#theta < %3.1f && %3.1f <#phi < %3.1f",title,50.,60.,25.,35.);

  sprintf(myname[3] ,"%sTheta%i%i",name,20,30);
  sprintf(mytitle[3],"%s for %3.1f <#theta < %3.1f && %3.1f <#phi < %3.1f",title,20.,30.,25.,35.);

  for (Int_t myiter=0; myiter<4; myiter++)
    {
      hists[myiter] = new TH1F(myname[myiter],mytitle[myiter],bins,min,max);
      hists[myiter]->SetXTitle(xtitle);
      hists[myiter]->SetYTitle(ytitle);
    }
  return;
}

void PTools::printHeader(const Char_t *titlehead, const Char_t *mytextfilename, Int_t nentries, TPad *parentPad)
{
  if (!parentPad) gPad->cd();
  else parentPad->cd();

  const TDatime daytime;
  Char_t texText[200];

  sprintf(texText,"%s",(titlehead)?titlehead:" ");
  TLatex *tex = new TLatex(0.01,0.15,texText);
  tex->SetTextSize(0.35);
  tex->SetLineWidth(2);
  tex->Draw();

  if (mytextfilename!=0 && nentries!=0)
    {
      sprintf(texText,"%i Events - File %s",nentries,mytextfilename);
    }
  else if (nentries==0 && mytextfilename==0)
    {
      sprintf(texText," ");
    }
  else if (nentries==0 && mytextfilename!=0)
    {
      sprintf(texText,"File %s",mytextfilename);
    }
  else 
    {
      sprintf(texText,"%i Events",nentries);
    }

  TLatex *tex2 = new TLatex(0.01,0.6,texText);
  tex2->SetTextSize(0.35);
  tex2->SetLineWidth(2);
  tex2->Draw();
  
  sprintf(texText,"%s",daytime.AsSQLString());

  TLatex *tex3 = new TLatex(0.85,0.15,texText);
  tex3->SetTextSize(0.35);
  tex3->SetLineWidth(2);
  tex3->Draw();

  return;
}

void PTools::evalStopwatch(TStopwatch *timer, Int_t nevts)
{
  Double_t realtime=(*timer).RealTime();
  Double_t cputime =(*timer).CpuTime();
  Int_t    realHour=(Int_t) ((realtime/3600));
  Int_t    realMin =(Int_t) (((realtime-realHour*3600)/60));
  Int_t    realSec =(Int_t) ((realtime-realHour*3600-realMin*60));
  Int_t    realmSec=(Int_t) (((realtime - (Int_t) (realtime-realHour*3600-realMin*60))*1000));
  Int_t     cpuHour=(Int_t) (( cputime/3600));
  Int_t     cpuMin =(Int_t) ((( cputime- cpuHour*3600)/60));
  Int_t     cpuSec =(Int_t) (( cputime- cpuHour*3600- cpuMin*60));
  Int_t     cpumSec=(Int_t) ((( cputime - (Int_t) ( cputime- cpuHour*3600- cpuMin*60))*1000));
  
  
  printf("------------------------------------------------------\n");
  printf("Events processed: %i\n",nevts);  
  printf("Real time(hh:mm:ss.ms): %02i:%02i:%02i.%03i\n",
	 realHour,realMin,realSec,realmSec);
  printf("Cpu time (hh:mm:ss.ms): %02i:%02i:%02i.%03i\n", 
	 cpuHour, cpuMin, cpuSec, cpumSec);
  if (nevts && cputime >0) printf("cpu  Performance: %f s/ev or %f.2 ev/s \n",(float) ( cputime/nevts),(float) (nevts/ cputime));
  if (nevts && realtime>0) printf("real Performance: %f s/ev or %f.2 ev/s \n",(float) (realtime/nevts),(float) (nevts/realtime));
  gSystem->Exec("cat /proc/cpuinfo | perl -ne 'print if(/cpu MHz/i || /model name/i )'");
  return;
}

void PTools::setTitleArts( TH1* histogramm, const TString xtitle, const TString ytitle, 
			   Int_t fillColor, Int_t titleFont)
{
  setTitleArts( histogramm, xtitle.Data(), ytitle.Data(), fillColor, titleFont);
}

void PTools::setTitleArts( TH1* histogramm, const Char_t *xtitle, const Char_t * ytitle, 
			   Int_t fillColor, Int_t titleFont)
{
  histogramm->SetXTitle(xtitle);
  histogramm->SetYTitle(ytitle);
  if (fillColor >= 0 ) histogramm->SetFillColor(fillColor);
  ((TAxis*)histogramm->GetXaxis())->CenterTitle();
  ((TAxis*)histogramm->GetXaxis())->SetTitleFont(titleFont);
  ((TAxis*)histogramm->GetXaxis())->SetTitleOffset(1.2);
  ((TAxis*)histogramm->GetYaxis())->SetTitleFont(titleFont);
  ((TAxis*)histogramm->GetYaxis())->SetTitleOffset(1.2);
}

void PTools::setTitleArts( THStack* histogrammStack, const TString xtitle, const TString ytitle, 
			   Int_t titleFont)
{
  setTitleArts( histogrammStack, xtitle.Data(), ytitle.Data(), titleFont);
}

void PTools::setTitleArts( THStack* histogrammStack, const Char_t *xtitle, const Char_t * ytitle, 
			   Int_t titleFont)
{
  ((TAxis*)histogrammStack->GetXaxis())->SetTitle(xtitle);
  ((TAxis*)histogrammStack->GetYaxis())->SetTitle(ytitle);
  ((TAxis*)histogrammStack->GetXaxis())->CenterTitle();
  ((TAxis*)histogrammStack->GetXaxis())->SetTitleFont(titleFont);
  ((TAxis*)histogrammStack->GetXaxis())->SetTitleOffset(1.2);
  ((TAxis*)histogrammStack->GetYaxis())->SetTitleFont(titleFont);
  ((TAxis*)histogrammStack->GetYaxis())->SetTitleOffset(1.2);
}


TCanvas* PTools::drawCanvas(const Char_t * name, 
			    const Char_t * title, 
			    TH1 **histarray, 
			    Int_t xsize, 
			    Int_t ysize)
{
#ifndef __CINT__
  static Int_t calls;
#else
  Int_t calls=0;
#endif

  TCanvas *canv = new TCanvas(name,title,0+calls*25,0+calls*25,1000,800);

  canv->Divide(xsize,ysize);

  for (Int_t time=0 ;time  < 6 ; time++)
    {
      canv->cd(time+1);
      if (histarray[time]->GetEntries()>0)
	{
	  histarray[time]->DrawCopy();
	  gPad->SetLogy();
	}
      else delete gPad;
    }
  
  calls++;
  return canv;
}

TCanvas* PTools::draw32Canvas(const Char_t * name, const Char_t * title, TH1F *histarray[])
{
#ifndef __CINT__
  static Int_t calls;
#else
  Int_t calls=0;
#endif

  TCanvas *canv = new TCanvas(name,title,0+calls*25,0+calls*25,1000,800);
  canv->Divide(3,2);

  for (Int_t time=0 ;time  < 6 ; time++)
    {
      canv->cd(time+1);
      if (histarray[time]->GetEntries()>0)
	{
	  histarray[time]->DrawCopy();
	  gPad->SetLogy();
	}
      else delete gPad;
    }
  
  calls++;
  return canv;
}

TCanvas* PTools::draw32CanvasTH2(const Char_t * name, const Char_t * title, TH2F *histarray[], const Char_t * option=0)
{
#ifndef __CINT__
  static Int_t calls;
#else
  Int_t calls=0;
#endif

  TCanvas *canv = new TCanvas(name,title,0+calls*25,0+calls*25,1000,800);
  canv->Divide(3,2);

  for (Int_t time=0 ;time  < 6 ; time++)
    {
      canv->cd(time+1);

      if (histarray[time]->GetEntries()>0)
	{
	  if (!option) histarray[time]->DrawCopy();
	  else histarray[time]->DrawCopy(option);
	  //gPad->SetLogz();
	}
      else {}// delete gPad;
    }
  
  calls++;
  return canv;
}

void PTools::cutEnd(Char_t * infi, Char_t * end)
{
  Char_t textfile[200];
  sprintf(textfile,"tempvar=\"%s\" ; echo ${tempvar%%%s}",infi,end);
  sprintf(infi,"%s",myexec(textfile));
}

char * PTools::myexec(char * in){
  Char_t textfile[200];
  FILE *pipe = gSystem->OpenPipe(in,"r");
  fscanf(pipe,"%s",textfile);
  gSystem->ClosePipe(pipe);
  sprintf(in,"%s",textfile);
  return in;
  ;
}

void PTools::mdb(int called)
{
  static int call;
  if (called==-1)
    {
      call++;
      cerr << "here" << call << endl;
    }
  else cerr << "here" << called << endl;
  return;
}

//Thanks to J.Kempter

TDirectory *PTools::Mkdir(TDirectory *dirOld, 
			  const Char_t *newDirName, 
			  Int_t index=-99, 
			  Int_t precision=1)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
{
  // Function to create subdirectories
  static Char_t sDir[255];
  static Char_t sFmt[10];
  if (index!=-99) 
    {
      sprintf(sFmt, "%%s %%0%1ii", precision);
      sprintf(sDir, sFmt, newDirName, index);
    }
  else 
    {
      sprintf(sFmt, "%%s");
      sprintf(sDir, sFmt, newDirName);
    }
  TDirectory *dirNew;
  if (!dirOld->FindKey(sDir)) 
    {
      dirNew = dirOld->mkdir(sDir);
    }
  else 
    {
      dirNew = (TDirectory *) dirOld->Get(sDir);
    }
  dirOld->cd(sDir);
  return dirNew;
}


TDirectory *PTools::Mkdir(TDirectory *dirOld,
			  const TString newDirName, 
			  Int_t index=-99, 
			  Int_t precision=1)    //! Makes new Dir, changes to Dir, returns pointer to new Dir
{
  return Mkdir(dirOld, newDirName.Data(), index, precision);
}
