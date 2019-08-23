#include <TF1.h>
#include <TH1.h>
#include <iostream.h>

TH1F * gSim, *gData, *gSimArray[1000];
Int_t gFuncCounter;
Int_t gSimFlag;
Int_t gGrad,gShift;

Double_t gSpaceMin,gSpaceMax;

Float_t percent0,percent1,percent2,percent0e;


class DataPoint
{
public:
  Float_t data;
  Float_t error;
  Float_t chi2;
  Float_t ndf;
};

Double_t rel_error(Double_t par,Double_t error) {
  return 1;
  
}

Double_t f_sig(Double_t *x, Double_t * par)
{
  if (gSimFlag != 0)
    gSim = gSimArray[Int_t(par[1])];
  
  Double_t val=0;
  Double_t xx =x[0];   //get the x-axis value
  Int_t i1 = gSim->GetXaxis()->FindBin(xx);
  Double_t x1 = gSim->GetXaxis()->GetBinCenter(i1);
  Int_t i0 = i1-1;Int_t i2=i1+1;
  Double_t x0 = gSim->GetXaxis()->GetBinCenter(i0);
  Double_t x2 = gSim->GetXaxis()->GetBinCenter(i2);
  Double_t y0 = gSim->GetBinContent(i0+gShift);
  Double_t y1 = gSim->GetBinContent(i1+gShift);
  Double_t y2 = gSim->GetBinContent(i2+gShift);

  //linear interpolation
  if (xx > x1)
    val = y1+(y2-y1)*((xx-x1)/(x2-x1));
  else
    val = y0+(y1-y0)*((xx-x0)/(x1-x0));

  Double_t integral =gSim->Integral();

  //cout << integral << ":" << val << endl;
  //Double_t integral = 1;
  if (integral != 0)
    return val*(par[0]/integral); 
  //return val*(par[0]);
  else
    return 0;
} 


Double_t f_under_space(Double_t *x, Double_t * par)
{
  Double_t c = x[0];
  Double_t val=0;
  if ((x[0]>gSpaceMin) && (x[0]<gSpaceMax)) return gData->GetBinContent(gData->FindBin(x[0]));
  if (gGrad == 7) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c +  par[4]*c*c*c*c +  par[5]*c*c*c*c*c  +  par[6]*c*c*c*c*c*c;
  if (gGrad == 6) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c +  par[4]*c*c*c*c +  par[5]*c*c*c*c*c;
  if (gGrad == 5) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c +  par[4]*c*c*c*c;
  if (gGrad == 4) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c;
  if (gGrad == 3) val = par[0] + par[1]*c + par[2]*c*c;
  if (gGrad == 2) val = par[0] + par[1]*c;
  if (gGrad == 1) val = par[0];
  return val;  
}

Double_t f_under(Double_t *x, Double_t * par)
{
  Double_t c = x[0];
  Double_t val=0;
  if (gGrad == 7) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c +  par[4]*c*c*c*c +  par[5]*c*c*c*c*c +  par[6]*c*c*c*c*c*c;
  if (gGrad == 6) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c +  par[4]*c*c*c*c +  par[5]*c*c*c*c*c;
  if (gGrad == 5) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c +  par[4]*c*c*c*c;
  if (gGrad == 4) val = par[0] + par[1]*c + par[2]*c*c +  par[3]*c*c*c;
  if (gGrad == 3) val = par[0] + par[1]*c + par[2]*c*c;
  if (gGrad == 2) val = par[0] + par[1]*c;
  if (gGrad == 1) val = par[0];
  return val;  
}

Double_t f_sig_under(Double_t *x, Double_t * par) 
{
  return f_sig(x,par) + f_under(x,&par[2]);
}


void ScaleAll(TH1 * hist, Double_t sc, Int_t exclude=100) {
  Double_t par[20],parm,parx;
  hist->Scale(sc);
  TList * list = hist->GetListOfFunctions();
  TIter next(list);
  TF1 *obj;
  while (obj = (TF1*)next()){                                
    obj->GetParameters(par);
    cout << "found " << obj->GetName() << endl;
    for (int i=0;i<10;i++) {
      //cout << par[i] << endl;
      //try to figure out if scaling is allowed...
      if (i!=exclude) par[i] = sc*par[i];
      obj->GetParLimits(i,parm,parx);
      //cout << "-- parx" <<  parx << endl;
      if ((i==exclude) && (parx==0)) {
        par[i] = sc*par[i];
        cout << "scale!!!" << endl;
      }
    }
    obj->SetParameters(par);
  }
}

Double_t draw_legendre(Double_t *x, Double_t * par){
  return par[0]+par[1]*.5*(3*x[0]*x[0]-1)+par[2]*(.1/.8)*(35*pow(x[0],4)-30*pow(x[0],2)+3);
}

void print_percent(TF1 * func,int num, int start=0) {
  float w=0,dw=0;
  for (int i=start;i<num;i++){
    w+=func->GetParameter(i);
    if (func->GetParameter(i))
      dw+=(func->GetParError(i)/func->GetParameter(i))*(func->GetParError(i)/func->GetParameter(i));
  }

  w=func->GetParameter(0);


  for (int i=start;i<num;i++) {
    cout << "Parameter p" << i << ": " << func->GetParameter(i)/w << " +/- (s)"  << func->GetParError(i)/w  
<< endl;
  }

  percent0=func->GetParameter(0+start)/w;
  percent1=func->GetParameter(1+start)/w;
  percent2=func->GetParameter(2+start)/w;

  if ((num-start) == 2) {
    percent0e=fabs(percent1)*(fabs(func->GetParError(0+start)/func->GetParameter(0+start))+fabs(func->GetPar
Error(1+start)/func->GetParameter(1+start)));
      

    cout << "Mix Error: " <<  percent0e
         << endl;
  }else if ((num-start) == 3){
    percent0e=(fabs(func->GetParameter(1)+func->GetParameter(2))*func->GetParError(0))/((func->GetParameter(
0)+func->GetParameter(1)+func->GetParameter(2))*(func->GetParameter(0)+func->GetParameter(1)+func->GetParame
ter(2))) +
      (fabs(func->GetParameter(0))*(func->GetParError(1)+func->GetParError(2)))/((func->GetParameter(0)+func
->GetParameter(1)+func->GetParameter(2))*(func->GetParameter(0)+func->GetParameter(1)+func->GetParameter(2))
);
    
  }


}



DataPoint HistoFit(TH1F * my_sim, TH1F * my_data,Double_t min,Double_t max, Double_t yconst, Double_t lo, Double_t hi, Int_t d=1, Int_t grad=4, Int_t shift=0)
{
  //Fitting signal plus background with signal shape from simulation
  //Parameters:
  //TH1F * my_sim:    Simulation signal shape
  //                  If d==2, use only under_space fit. 
  //TH1F * my_data:   Data which has to be fitted
  //                  Both histograms have to have the same axis parameters
  //Double_t min,max: Fitting Range
  //Double_t yconst:  Max signal yield
  //Double_t lo, hi:  Axis range for the display
  //Int_t d:          Some stupid flag...
  //                  =0: Release Fit
  //                  =2: Only under_space fit
  //                  =1: ??
  //                  =3: Stop after Signal fitting
  //Int_t grad:       Order of the polynomial (up to 6)
  //Int_t shift:      If you think that the signal is shifted by some channels....
  //
  //Return value: A self defined datapoint
  //
  //Globals:
  //gSpaceMin,gSpaceMax: Keep out range for the first fitting step
  
  DataPoint datapoint;
  gGrad=grad;

  gSimFlag = 0;

  cout << gFuncCounter << endl;
  //Is needed for fitting more than one histo per page

  gData = my_data;
  gSim = my_sim;
  gShift=shift;
  gFuncCounter++;

  gData->SetAxisRange(min,max);
  if (d) gData->Draw(); 
  
  TF1 * under_space = new TF1("under_space",f_under_space,min,max,grad);

  //gData->Fit("under_space","LM");
  gData->Fit("under_space");

  Double_t par_u[7],integral=0,error=0;
  under_space->GetParameters(par_u);
  TF1 * under = new TF1("under",f_under,min,max,grad);
  under->SetParameters(par_u);

  if ((d==2) || (d==102)) {
    //only background sub.
    //my_sim->Sumw2();
    // scan bin entries
    Double_t tmp = my_sim->Integral();
    cout << "sim: " << tmp << endl;
    for (int i=1; i<my_data->GetNbinsX();i++) {
      if (d==102) { //reshape simulated form
	if ((float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i))) > 0) {
	  my_sim->SetBinContent(i,float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i)));
	  my_sim->SetBinError(i,my_data->GetBinError(i));
	}
      }
      if ((my_data->GetBinCenter(i) > gSpaceMin) && (my_data->GetBinCenter(i) < gSpaceMax)) {
	//integral += my_sim->GetBinContent(i);
	integral += float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i));
	//error += my_sim->GetBinError(i);
	error += my_data->GetBinError(i);
      }
    }
    
    if (d==102 && tmp) my_sim->Scale(tmp/my_sim->Integral()); //scale to the same integral
    else {
      cout << "Integral: " << integral << endl;
      datapoint.data = integral;
      datapoint.error = error;
      
      gData->SetAxisRange(lo,hi);
      gData->Draw("e1");
      
      char my_name[100];
      under_space->GetParameters(par_u);
      under->SetParameters(par_u);
      under->SetLineWidth(4);
      under->SetLineStyle(2);
      gSimArray[gFuncCounter] = gSim;
      cout << gFuncCounter << endl;
      sprintf(my_name,"under_%u",gFuncCounter);
      under->SetName(my_name);
      gData->GetListOfFunctions()->AddLast(under);
      
      return datapoint;
    }
  }
 
  TF1 * sig_under = new TF1("sig_under",f_sig_under,min,max,grad+2);
  sig_under->SetLineWidth(4);
  
  sig_under->SetParameters
    (yconst,gFuncCounter,par_u[0],par_u[1],par_u[2],par_u[3],par_u[4],par_u[5],par_u[6],par_u[7]);
  sig_under->SetParLimits(0,0,yconst);
  sig_under->SetParLimits(1,gFuncCounter,gFuncCounter);

  //fix background
  Double_t par_s[10];
  sig_under->GetParameters(par_s);
  sig_under->SetParLimits(2,par_s[2],par_s[2]);
  sig_under->SetParLimits(3,par_s[3],par_s[3]);
  sig_under->SetParLimits(4,par_s[4],par_s[4]);
  sig_under->SetParLimits(5,par_s[5],par_s[5]);
  sig_under->SetParLimits(6,par_s[6],par_s[6]);  
  sig_under->SetParLimits(7,par_s[7],par_s[7]);  
  //fit signal
  gData->Fit("sig_under","LM");   
  //return datapoint;
  
  sig_under->GetParameters(par_s);
  sig_under->SetParLimits(2,0,0);
  sig_under->SetParLimits(3,0,0);
  sig_under->SetParLimits(4,0,0);
  sig_under->SetParLimits(5,0,0);
  sig_under->SetParLimits(6,0,0); 
  sig_under->SetParLimits(7,0,0);

  if (d!=3) {
    //free background

    gData->Fit("sig_under","LM");   
    sig_under->GetParameters(par_s);
  }
 


  gData->SetAxisRange(lo,hi);
  if (d) gData->Draw("e1");

  if (d == 101) {
    cout << "rescale sim histo" << endl; 
    //take integral from difference, not from integral
    Double_t tmp = my_sim->Integral();
    for (int i=1; i<my_data->GetNbinsX();i++) {
      if ((my_data->GetBinCenter(i) > gSpaceMin) && (my_data->GetBinCenter(i) < gSpaceMax)) {
	if ((float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i))) > 0) {
	  my_sim->SetBinContent(i,float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i)));
	  my_sim->SetBinError(i,my_data->GetBinError(i));
	}
	cout << float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i))<< endl;
	
	//integral += my_sim->GetBinContent(i);
	integral += float(my_data->GetBinContent(i))-under->Eval(my_data->GetBinCenter(i));
	//error += my_sim->GetBinError(i);
	error += my_data->GetBinError(i);	
	
      }
    }
    if (tmp)
      my_sim->Scale(tmp/my_sim->Integral()); //scale to the same integral
    cout << "Integral: " << integral << endl;
      //fix background
    Double_t par_s[10];
    sig_under->GetParameters(par_s);
    sig_under->SetParLimits(2,par_s[2],par_s[2]);
    sig_under->SetParLimits(3,par_s[3],par_s[3]);
    sig_under->SetParLimits(4,par_s[4],par_s[4]);
    sig_under->SetParLimits(5,par_s[5],par_s[5]);
    sig_under->SetParLimits(6,par_s[6],par_s[6]);  
    sig_under->SetParLimits(7,par_s[7],par_s[7]);  
    
    gData->Fit("sig_under","LM"); 

    sig_under->GetParameters(par_s);
  }

  char my_name[100];
  under->SetParameters(&par_s[2]);
  
  under->SetLineWidth(4);
  under->SetLineStyle(2);
  gSimArray[gFuncCounter] = gSim;
  cout << gFuncCounter << endl;
  sprintf(my_name,"under_%u",gFuncCounter);
  under->SetName(my_name);
  gData->GetListOfFunctions()->AddLast(under);

  TF1 * sig = new TF1("sig",f_sig,min,max,2);
  
  sig->SetParameters(par_s);
  sig->SetParLimits(1,1,1); //scaling forbidden
  
  sig->SetLineWidth(4);
  sig->SetLineStyle(3);
  sig_under->SetLineWidth(4);

  sprintf(my_name,"sig_%u",gFuncCounter);
  sig->SetName(my_name);
  gData->GetListOfFunctions()->AddLast(sig);
  //if (d) sig->Draw("same");
  //float binwidth=my_sim->GetXaxis()->GetBinWidth(2);
  gSimFlag = 1;


  datapoint.data = par_s[0];

  cout << "yield: " << par_s[0] << endl;
  cout << "Integral: " << sig->Integral(min, max) << endl;

  if (sig_under->GetNDF() !=0)
    datapoint.error = sig_under->GetParError(0)*(sqrt(sig_under->GetChisquare()/sig_under->GetNDF()));
  else
    datapoint.error = 0;
  if (d!=1)
    datapoint.error = sig_under->GetParError(0);
  
  
  datapoint.chi2 = sig_under->GetChisquare();
  datapoint.ndf = sig_under->GetNDF();


  if (datapoint.chi2 ==0) datapoint.data=0;

  //**********************

  return datapoint;
 
}



void fit_test(void)
{

  gSpaceMin=0.4;
  gSpaceMax=0.6;


  TF1 *signal = new TF1("signal", "xgaus+[3]*x*(x-0.5)");

  signal->SetParameters(1, 0.5, 0.03, 2);
  //signal->SetParameters(1, 0.5, 0.03, 0);

  TF1 *simu = new TF1("simu", "xgaus");
  simu->SetParameters(1, 0.5, 0.03);
  
  
  TH1F *data = new TH1F("data", "Signal und Untergrund", 50,0,1);
  data->Sumw2();
  TH1F *sim = new TH1F("sim", "Nur Signal", 50,0,1);
  sim->Sumw2();
  
  data->FillRandom("signal", 100000);  
  sim->FillRandom("simu", 10000);
  
  
  signal->SetParLimits(1,0.5,0.5);
  signal->SetParameter(2,0.1);
  signal->SetParLimits(2,0.1,0.1);
  
  DataPoint dat = HistoFit(sim,data,0.1,.9,10000000, 0, 1.,  3   ,6,0);

  cout << "data:" << dat.data << " error:" << dat.error << " chi2:" << 
    dat.chi2 << " ndf:" << dat.ndf << endl;
   
  //HistoFit(TH1F * my_sim, TH1F * my_data,Double_t min,Double_t max, Double_t yconst, Double_t lo, Double_t hi, Int_t d=1, Int_t grad=4, Int_t shift=0)
}










