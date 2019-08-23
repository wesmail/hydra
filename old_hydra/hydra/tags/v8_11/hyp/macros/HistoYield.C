#include <iostream.h>
#include <math.h>
#include <TH3.h>
#include <TH2.h>
#include <TCanvas.h>

#define GRAN_MAX 20

#include "HistoFit.C"



class HistoYield
{
public:
  HistoYield(TH3F * , TH3F *, TH3F * model = NULL);
  HistoYield(TH2F * , TH2F *, TH2F * model = NULL);
  void Project(Int_t, Int_t, Int_t, Int_t, Double_t sc=1);
  void DeletePhaseSpaceBin(Int_t,Int_t);
  void SetPhaseSpace(TH2F *);
  void ExtraMin(Int_t, Int_t,Float_t);
  void ExtraMax(Int_t, Int_t,Float_t);
  void ExtraAxis(Float_t,Float_t);
  void Fit(Float_t, Float_t, Float_t, Float_t, Int_t, Int_t grad=4, Int_t cerror=0, Int_t d=1);
  void Display(Int_t,Int_t);
  void SetTitles(char*,char*);
  void Scaling(Float_t);
  void Write(char *, Int_t opt=0);
  void SetSysError(Int_t,Int_t,Float_t);
  void PrintStat(void);
  TH2F *histo_yield,*histo_counts,*histo_counts_alt,*histo_corr;
  TH1D *histo_yield1,*histo_counts1,*histo_counts1_alt,*histo_corr1;
  TH1F *mini_data[GRAN_MAX][GRAN_MAX];
  TH1F *mini_sim[GRAN_MAX][GRAN_MAX];
  TH1F *mini_model[GRAN_MAX][GRAN_MAX];
  Float_t raw[GRAN_MAX][GRAN_MAX];
    
private:
  Float_t chi2[GRAN_MAX][GRAN_MAX],ndf[GRAN_MAX][GRAN_MAX];
  Bool_t phase_flag;
  Float_t sys_error[GRAN_MAX][GRAN_MAX];
  Float_t extra_min[GRAN_MAX][GRAN_MAX];
  Float_t extra_max[GRAN_MAX][GRAN_MAX];
  Float_t extra_axis_min,extra_axis_max;
  Float_t dx(Int_t);
  Float_t dy(Int_t);
  TH3F *histo_sim,*histo_data,*histo_model;
  TH2F *histo_sim2,*histo_data2,*histo_model2;
  Float_t min_x,max_x,min_y,max_y;
  Int_t x_bins, y_bins;
  Double_t simd[GRAN_MAX][GRAN_MAX];
  Double_t modeld[GRAN_MAX][GRAN_MAX];
  Float_t yield[GRAN_MAX][GRAN_MAX];
  Int_t project_flag;
  Int_t grid_x,grid_y,step_x,step_y;
  char name[100],title[100];
  Int_t sheets_x,sheets_y;
  char * xtitle, * ytitle;
  Float_t scaling;

  DataPoint data;
};


void HistoYield::ExtraAxis(Float_t a,Float_t b){
  extra_axis_min=a;extra_axis_max=b;
}

void HistoYield::Write(char * name, Int_t opt) {
  FILE * file;
  if ( (file = fopen(name,"w")) == NULL) {
    cout << "Cannot open " << name << endl;
    exit(1);
  }
  for (int i=1;i<=histo_yield1->GetNbinsX();i++) {
    fprintf(file,"%f,%f\n",histo_yield1->GetBinContent(i),histo_yield1->GetBinError(i));
  }
  for (int i=1;i<=histo_corr1->GetNbinsX();i++) {
    fprintf(file,"%f,%f\n",histo_corr1->GetBinContent(i),histo_corr1->GetBinError(i));
  }
  for (int i=1;i<=histo_counts1->GetNbinsX();i++) {
    fprintf(file,"%f,%f\n",histo_counts1->GetBinContent(i),histo_counts1->GetBinError(i));
  }

  fclose (file);   
}

void HistoYield::SetSysError(Int_t x,Int_t y,Float_t e){
  sys_error[x][y]=e;
}

void HistoYield::Scaling(Float_t my_scaling){
  scaling= my_scaling;
};

void HistoYield::ExtraMin(Int_t x, Int_t y,Float_t m)
{
  extra_min[x][y]=m;
};

void HistoYield::ExtraMax(Int_t x, Int_t y,Float_t m)
{
  extra_max[x][y]=m;
};

void HistoYield::SetTitles(char*x,char*y)
{
  xtitle=x;ytitle=y;
};

void HistoYield::DeletePhaseSpaceBin(Int_t x,Int_t y){simd[x][y]=0;};

void HistoYield::SetPhaseSpace(TH2F * phasespace)
{
  cout << "Extracting phase space distrubution... " ;
  phase_flag=1;
  Float_t phase_min_x = phasespace->GetXaxis()->GetXmin();
  Float_t phase_min_y = phasespace->GetYaxis()->GetXmin();
  Float_t phase_max_x = phasespace->GetXaxis()->GetXmax();
  Float_t phase_max_y = phasespace->GetYaxis()->GetXmax();
  
  if ((phase_min_x != min_x) ||
      (phase_min_y != min_y) ||
      (phase_max_x != max_x) ||
      (phase_max_y != max_y)){cout << "WARNING: Borders differ!" << endl; }
  
  Int_t phase_x_bins = phasespace->GetNbinsX();
  Int_t phase_y_bins = phasespace->GetNbinsY();
  
  Int_t phase_step_x = Int_t(phase_x_bins/grid_x);
  Int_t phase_step_y = Int_t(phase_y_bins/grid_y);
  
  for(int ix=0; ix<grid_x; ix++)
    {
      for(int iy=0; iy<grid_y; iy++)
	{
	  simd[ix][iy] = 0;
	  for(int jx=0; jx<phase_step_x; jx++)
	    {
	      for(int jy=0; jy<phase_step_y; jy++)
		{
		  Int_t mybin = phasespace->GetBin(((ix*phase_step_x)+1+jx),((iy*phase_step_y)+1+jy));
		  simd[ix][iy] += phasespace->GetBinContent(mybin);
		}
	    }
	}
    }
  
  cout << "done" << endl;
}

void  HistoYield::PrintStat(void) {
 for(int ix=0; ix<grid_x; ix++)
    {
      for(int iy=0; iy<grid_y; iy++)
	{
	  cout << ix << "," << iy << ": Chi2=" << chi2[ix][iy] << ", ndf=" << ndf[ix][iy] << " -> " << chi2[ix][iy]/ndf[ix][iy] << endl;
	}
    }

}

void HistoYield::Display(Int_t op, Int_t simflag)
{

  if (extra_axis_min !=0) {
    TH2D * frame = new TH2D("frame","frame",1, extra_axis_min,extra_axis_max,1,extra_axis_min,extra_axis_max);
    frame->GetXaxis()->SetTitleOffset(1.1);
    frame->GetYaxis()->SetTitleOffset(1.1);
    frame->GetXaxis()->SetTitleSize(.06);
    frame->GetYaxis()->SetTitleSize(.06);  
    frame->GetYaxis()->SetTitleFont(42);
    frame->GetXaxis()->SetTitleFont(42);
  }

  //if (op != 0){
    histo_yield->Scale(1/histo_yield->Integral());
    histo_corr->Scale(1/histo_corr->Integral());
    histo_counts->Scale(1/histo_counts->Integral());
    
    
    histo_yield1  = histo_yield->ProjectionX("histo_yield1",0,1000,"E");
    histo_yield1->Sumw2();
    histo_counts1 = histo_counts->ProjectionX("histo_counts1",0,1000);
    if (histo_model || histo_model2) {
      histo_counts_alt->Scale(1/histo_counts_alt->Integral());
      histo_counts1_alt = histo_counts_alt->ProjectionX("histo_counts1_alt",0,1000);
      cout << "display model" << endl;
    }
    else
      histo_counts1_alt = NULL;
    histo_corr1 =histo_corr->ProjectionX("histo_corr1",0,1000,"E");
    //}
    histo_yield1->GetXaxis()->SetTitleOffset(1.1);
    histo_yield1->GetYaxis()->SetTitleOffset(1.1);
    histo_yield1->GetXaxis()->SetTitleSize(.06);
    histo_yield1->GetYaxis()->SetTitleSize(.06);  
    histo_yield1->GetYaxis()->SetTitleFont(42);
    histo_yield1->GetXaxis()->SetTitleFont(42);


    histo_corr1->SetMarkerStyle(0);
    histo_yield1->SetMarkerStyle(0);
    histo_corr1->SetMarkerSize(1.5);
    histo_yield1->SetMarkerSize(1.5);
    histo_yield1->SetLineWidth(2);
    histo_corr1->SetLineWidth(2);
    histo_yield->GetXaxis()->SetTitleOffset(1.1);
    histo_yield->GetYaxis()->SetTitleOffset(1.1);
    histo_yield->GetXaxis()->SetTitleSize(.06);
    histo_yield->GetYaxis()->SetTitleSize(.06); 
    histo_yield->GetYaxis()->SetTitleFont(42);
    histo_yield->GetXaxis()->SetTitleFont(42);
    histo_corr->GetYaxis()->SetTitleFont(42);
    histo_corr->GetXaxis()->SetTitleFont(42);

    Float_t binwidth=histo_yield1->GetXaxis()->GetBinWidth(1);
    //Float_t binwidth=1;
    if (scaling)
      {
	histo_corr1->Scale(scaling/binwidth);
	histo_yield1->Scale(scaling/binwidth);
	histo_counts1->Scale(scaling/binwidth);
	if (histo_counts1_alt) histo_counts1_alt->Scale(scaling/binwidth);
      }

  TCanvas *c5 = new TCanvas("c5","10x10 dalitz /yield",100,100,400,400);
  c5->cd();
  c5->SetFillColor(0);
  if (op == 0)
    histo_yield->Draw("box");
  else {
    histo_yield1->Draw("E0");
    histo_counts1->SetLineWidth(2);
    histo_counts1->Draw("Csame");

    if (histo_counts1_alt) {
      cout << "same" << endl;
      histo_counts1_alt->SetLineStyle(2);
      histo_counts1_alt->SetLineWidth(2);
      histo_counts1_alt->Draw("Csame");      
    }
  }
  TCanvas *c6 = new TCanvas("c6","10x10 dalitz /sim",100,100,400,400);
  c6->cd();
  c6->SetFillColor(0);
  if (op == 0)
    histo_counts->Draw("box");
  else
    histo_counts1->Draw("E0");
  
  TCanvas *c7 = new TCanvas("c7","10x10 dalitz /corr",100,100,400,400);
  c7->cd();
  c7->SetFillColor(0);
  if ((xtitle) && (op==0)) {
    histo_corr->SetXTitle(xtitle);
    if (extra_axis_min !=0) frame->SetXTitle(xtitle);
    histo_yield->SetXTitle(xtitle); }
  if ((xtitle) && (op==1)) {
    histo_corr1->SetXTitle(xtitle);
    histo_yield1->SetXTitle(xtitle); }

  if ((ytitle) && (op==0)) {
    histo_corr->SetYTitle(ytitle);
    if (extra_axis_min !=0) frame->SetYTitle(ytitle);
    histo_yield->SetYTitle(ytitle); }

  if ((ytitle) && (op==1)) {
    histo_corr1->SetYTitle(ytitle);
    histo_yield1->SetYTitle(ytitle); }

  c5->SetBorderMode(0);
  c6->SetBorderMode(0);
  c7->SetBorderMode(0);
  c5->SetLeftMargin(0.15);
  c6->SetLeftMargin(0.15);
  c7->SetLeftMargin(0.15);
  c5->SetBottomMargin(0.15);
  c6->SetBottomMargin(0.15);
  c7->SetBottomMargin(0.15);
  c5->SetTopMargin(0.05);
  c6->SetTopMargin(0.05);
  c7->SetTopMargin(0.05);
    c5->SetRightMargin(0.05);
    c6->SetRightMargin(0.05);
    c7->SetRightMargin(0.05);
    if (op == 0) {
      histo_corr->GetXaxis()->SetTitleOffset(1.1);
      histo_corr->GetYaxis()->SetTitleOffset(1.1);
      histo_corr->GetXaxis()->SetTitleSize(.06);
      histo_corr->GetYaxis()->SetTitleSize(.06);
      if (extra_axis_min !=0) {frame->Draw();
      histo_corr->Draw("boxsame");} else
      histo_corr->Draw("box"); }
    else {
      histo_corr1->GetXaxis()->SetTitleOffset(1.1);
      histo_corr1->GetYaxis()->SetTitleOffset(1.1);
      histo_corr1->GetXaxis()->SetTitleSize(.06);
      histo_corr1->GetYaxis()->SetTitleSize(.06);
      histo_corr1->GetYaxis()->SetTitleFont(42);
      histo_corr1->GetXaxis()->SetTitleFont(42);

      histo_corr1->Draw("E0");}
    
    if (simflag == 1)
    {
      histo_counts->SetLineStyle(2);
      histo_counts1->SetLineStyle(2);
      histo_counts1->SetLineWidth(2);
      histo_counts1->SetMarkerStyle(0);
      c5->cd();
      if (op == 0)
	histo_counts->Draw("same");
      else
	histo_counts1->Draw("sameH");
      c7->cd();
      if (op == 0)
	histo_counts->Draw("same");
      else
	histo_counts1->Draw("sameH");
    }

}

void HistoYield::Fit(Float_t fit_min, Float_t fit_max, Float_t pic_min, Float_t pic_max, Int_t start, Int_t grad, Int_t cerror, Int_t d)
{
  int rows=1;
  int numx,numy;
  char dump[10000];
  if (grid_y != 1) {numx=grid_x/sheets_x;  numy=grid_y/sheets_y;} //2d plot
  else  {numx=grid_x;  numy=1;rows=sheets_x;sheets_x=1;}
  //cout << grid_y << endl; exit(1);
  TCanvas *c1;
  int my_pad; 
  FILE * conf;
  conf = fopen("data.vector","w");
  c1 = new TCanvas("cx00","0:0",0,0,800,800);
  HistoFit(mini_sim[0][0],mini_data[0][0],fit_min,fit_max,start,pic_min,pic_max,d,grad);
  //exit(1);

  c1->Clear();
  c1->Divide(numx/rows,numy*rows,0.001,0.001,0);

  for (int sx=0; sx<sheets_x;sx++)
    for (int sy=0; sy<sheets_y;sy++)
      {
	sprintf(name,"cx%u%u",sx,sy);
	sprintf(title,"%u:%u",sx,sy);
	if ((sx!=0) || (sy!=0)){
	  c1 = new TCanvas(name,title,0,0,800,800);
	  c1->Divide(numx,numy,0.001,0.001,0);}
	my_pad=1;
	for (int iy=sy*numy;iy<(sy+1)*numy;iy++)
	  for (int ix=sx*numx;ix<(sx+1)*numx;ix++)
	    {
	      cout << "going to fit: " << ix << ":" << iy << endl;
	      c1->cd(my_pad);
	      gPad->SetFillColor(0);
	      my_pad++;
	      Int_t corr_i=ix;Int_t corr_j=(grid_y-1)-iy; 
	      mini_data[corr_i][corr_j]->Draw();
	      Float_t my_fit_min=fit_min;
	      Float_t my_fit_max=fit_max;
	      if (extra_min[ix][iy] != 0)  {my_fit_min=extra_min[ix][iy];}
	      if (extra_max[ix][iy] != 0)  my_fit_max=extra_max[ix][iy];
	      
	      data = HistoFit(mini_sim[corr_i][corr_j],mini_data[corr_i][corr_j],my_fit_min,my_fit_max,start,pic_min,pic_max,d,grad,cerror);
	      yield[corr_i][corr_j] = fabs(data.data);
	      raw[corr_i][corr_j] = fabs(data.data);
	      chi2[corr_i][corr_j] =  fabs(data.chi2);
	      ndf[corr_i][corr_j] =  fabs(data.ndf);
	      gPad->Update();
	      gPad->Modified();
	      gPad->Paint();

	      Int_t mybin = histo_yield->GetBin(1+corr_i,corr_j+1);

	      histo_yield->SetBinContent(mybin,yield[corr_i][corr_j]);
	      
	      if (data.data!=0) histo_yield->SetBinError(mybin,data.data*(sqrt( (data.error/data.data)*(data.error/data.data) + sys_error[corr_i][corr_j]* sys_error[corr_i][corr_j] )));
	      else histo_yield->SetBinError(mybin,0);
	      cout << "setting " << yield[corr_i][corr_j] << ":"<<data.error<< endl;

	      if (!phase_flag) {//get phase space from the simulations 
		simd[corr_i][corr_j] = mini_sim[corr_i][corr_j]->Integral();
		cout << "get from simu..." << endl;
	      }
	      histo_counts->SetBinContent(mybin,(Stat_t)simd[corr_i][corr_j]);
	      histo_counts->SetBinError(mybin,1);
	      
	      if (histo_model2 || histo_model)
		{
		  cout << "using model" << endl;
		  modeld[corr_i][corr_j] = mini_model[corr_i][corr_j]->Integral();
		  histo_counts_alt->SetBinContent(mybin,(Stat_t)modeld[corr_i][corr_j]);
		}
	      

	      if ((simd[corr_i][corr_j] != 0) && (data.data !=0))
		{
		  cout << "histo_corr: " << yield[corr_i][corr_j] << "/" << simd[corr_i][corr_j] << endl;
		  histo_corr->SetBinContent(mybin,yield[corr_i][corr_j]/Float_t(simd[corr_i][corr_j]));
		  cout << data.error<<":" << data.data<<endl;
		  Float_t rerror1=histo_yield->GetBinError(mybin)/histo_yield->GetBinContent(mybin);
		  Float_t rerror2=(1/(sqrt(simd[corr_i][corr_j])));
		  cout << rerror1<<":" << rerror2<<endl;
		  Float_t error=(rerror1+rerror2)*(yield[corr_i][corr_j]/Float_t(simd[corr_i][corr_j]));
		  histo_corr->SetBinError(mybin,error);
		  cout << "YIELD: " << yield[corr_i][corr_j]/Float_t(simd[corr_i][corr_j]) << " ERROR: " << error << endl;
		  fprintf(conf,"%f\n",yield[corr_i][corr_j]/Float_t(simd[corr_i][corr_j]));
		}
              else
                fprintf(conf,"0.0\n");

	      /*
	      histo_yield->Fill(dx(corr_i),dy(corr_j),Stat_t(yield[corr_i][corr_j]));
	      histo_counts->Fill(dx(corr_i),dy(corr_j),Stat_t(simd[corr_i][corr_j]));
	      if (simd[corr_i][corr_j] != 0)
		histo_corr->Fill(dx(corr_i),dy(corr_j),Stat_t(yield[corr_i][corr_j]/Float_t(simd[corr_i][corr_j])));
	      */
	    }
	
      }

cout << dump;
  
}

void HistoYield::Project(Int_t my_grid_x, Int_t my_grid_y, Int_t my_sheets_x , Int_t my_sheets_y , Double_t sc)
{
  if ((my_grid_x<1) || (my_grid_y<1))
    {cout << "Illegal grid number!" << endl; exit(1);}
  if ((my_sheets_x<1) || (my_sheets_y<1))
    {cout << "Illegal sheet number!" << endl; exit(1);}
  sheets_x=my_sheets_x;sheets_y=my_sheets_y;
  
  
  grid_x=my_grid_x;grid_y=my_grid_y;
  cout << "Projecting Mini Histograms..." << endl;
  
  step_x = Int_t(x_bins/grid_x);
  step_y = Int_t(y_bins/grid_y);

  for(Int_t x=0; x<grid_x; x++)
    for(Int_t y=0; y<grid_y; y++)
      {
	cout << "PROJECTING: " << x*step_x+1 << ":" << (x+1)*step_x << ":" << y*step_y+1 << ":" << (y+1)*step_y<<endl;
	sprintf(name,"data_%u%u",x,y);
	if (histo_data) {
	  mini_data[x][y] = (TH1F*)histo_data->ProjectionZ(name,x*step_x+1,(x+1)*step_x,y*step_y+1,(y+1)*step_y,"E");
	  mini_data[x][y]->SetXTitle(histo_data->GetTitle());	  
	}
	else {
	  mini_data[x][y] = (TH1F*)histo_data2->ProjectionY(name,x*step_x+1,(x+1)*step_x,"E");
	  mini_data[x][y]->SetXTitle(histo_data2->GetTitle());
	}
	sprintf(name,"sim_%u%u",x,y);
	if (histo_sim)
	  mini_sim[x][y] = (TH1F*)histo_sim->ProjectionZ(name,x*step_x+1,(x+1)*step_x,y*step_y+1,(y+1)*step_y,"E");
	else {
	  mini_sim[x][y] = (TH1F*)histo_sim2->ProjectionY(name,x*step_x+1,(x+1)*step_x,"E");
	  if (histo_model2) {
	    cout << "got model" << endl;
	    sprintf(name,"model_%u%u",x,y);
	    mini_model[x][y] = (TH1F*)histo_model2->ProjectionY(name,x*step_x+1,(x+1)*step_x,"E");
	  }
	}

	mini_data[x][y]->SetTitleSize(0.06);
	mini_sim[x][y]->Scale(sc);
	mini_data[x][y]->Scale(sc);
      }
  
  histo_yield = new TH2F("histo_yield","Yield",grid_x,min_x,max_x,grid_y,min_y,max_y);
  histo_yield->Sumw2();
  histo_counts = new TH2F("histo_counts","Phasespace",grid_x,min_x,max_x,grid_y,min_y,max_y);
  histo_counts_alt = new TH2F("histo_counts_alt","Phasespace",grid_x,min_x,max_x,grid_y,min_y,max_y);
  //histo_counts->Sumw2();
  histo_corr = new TH2F("histo_corr","Yield /phase",grid_x,min_x,max_x,grid_y,min_y,max_y);
  histo_corr->Sumw2();
}

Float_t HistoYield::dx(Int_t bin)
{
  return min_x+(Float_t(bin)+.5)*((max_x- min_x)/grid_x);
}
Float_t HistoYield::dy(Int_t bin)
{
  return min_y+(Float_t(bin)+.5)*((max_y- min_y)/grid_y);
}

HistoYield::HistoYield(TH2F * my_histo_sim, TH2F * my_histo_data, TH2F * my_histo_model)
{
  histo_model=NULL;
  histo_model2=NULL;

  project_flag = 0;
  sheets_x=2;sheets_y=2;
  extra_axis_min=0;extra_axis_max=0;
  cout << "Testing the axis... " ;
  Float_t sim_min_x = my_histo_sim->GetXaxis()->GetXmin();
  Float_t sim_max_x = my_histo_sim->GetXaxis()->GetXmax();

  Float_t data_min_x = my_histo_data->GetXaxis()->GetXmin();
  Float_t data_max_x = my_histo_data->GetXaxis()->GetXmax();

  if ((sim_min_x != data_min_x) ||
      (sim_max_x != data_max_x)){cout << "Borders differ!" << endl; exit(1);}
    
  Int_t sim_x_bins = my_histo_sim->GetNbinsX();
  Int_t data_x_bins = my_histo_data->GetNbinsX();

  if ((sim_x_bins != data_x_bins)){cout << "Bin number differ!" << endl; exit(1);}

  cout << "done" << endl;

  min_x=data_min_x;
  max_x=data_max_x;
  x_bins=data_x_bins; 
  y_bins=1;

  cout << "Got " << x_bins << ":" << y_bins << endl;

  histo_sim2=my_histo_sim;histo_data2=my_histo_data;
  histo_model2=my_histo_model;
  histo_sim=NULL;
  histo_data=NULL;
}

HistoYield::HistoYield(TH3F * my_histo_sim, TH3F * my_histo_data, TH3F * my_histo_model)
{
  histo_model=NULL;
  histo_model2=NULL;

  
  project_flag = 0;
  sheets_x=2;sheets_y=2;
  extra_axis_min=0;extra_axis_max=0;
  cout << "Testing the axis... " ;
  Float_t sim_min_x = my_histo_sim->GetXaxis()->GetXmin();
  Float_t sim_min_y = my_histo_sim->GetYaxis()->GetXmin();
  Float_t sim_max_x = my_histo_sim->GetXaxis()->GetXmax();
  Float_t sim_max_y = my_histo_sim->GetYaxis()->GetXmax();

  Float_t data_min_x = my_histo_data->GetXaxis()->GetXmin();
  Float_t data_min_y = my_histo_data->GetYaxis()->GetXmin();
  Float_t data_max_x = my_histo_data->GetXaxis()->GetXmax();
  Float_t data_max_y = my_histo_data->GetYaxis()->GetXmax();

  if ((sim_min_x != data_min_x) ||
      (sim_min_y != data_min_y) ||
      (sim_max_x != data_max_x) ||
      (sim_max_y != data_max_y)){cout << "Borders differ!" << endl; exit(1);}
    
  Int_t sim_x_bins = my_histo_sim->GetNbinsX();
  Int_t sim_y_bins = my_histo_sim->GetNbinsY();
  Int_t data_x_bins = my_histo_data->GetNbinsX();
  Int_t data_y_bins = my_histo_data->GetNbinsY();

  if ((sim_x_bins != data_x_bins) ||
      (sim_y_bins != data_y_bins)){cout << "Bin number differ!" << endl; exit(1);}

  cout << "done" << endl;

  min_x=data_min_x;min_y=data_min_y;
  max_x=data_max_x;max_y=data_max_y;
  x_bins=data_x_bins; y_bins=data_y_bins;

  cout << "Got " << x_bins << ":" << y_bins << endl;

  histo_sim=my_histo_sim;histo_data=my_histo_data;
  histo_model=my_histo_model;

}
