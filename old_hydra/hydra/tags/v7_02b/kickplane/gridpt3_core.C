{
//Core of gridpt macros. Configuration is done previously in other macro
//Takes ntuple from kickResol.C and generates parameters for momentum calc.
  //
/**************************** DECLARATIONS *********************************/
const Float_t binsTheta = (kMaxTheta - kMinTheta) / kGranularity;
const Float_t midPhi    = (kMaxPhi + kMinPhi ) / 2;
const Float_t binsPhi   = (kMaxPhi   - kMinPhi  ) / kGranularity;


HKickGrid gridA(kMinTheta,kMaxTheta,kMinPhi,kMaxPhi,kGranularity,kSimetry);
HKickGrid gridB(kMinTheta,kMaxTheta,kMinPhi,kMaxPhi,kGranularity,kSimetry);
HKickGrid gridC(kMinTheta,kMaxTheta,kMinPhi,kMaxPhi,kGranularity,kSimetry);
HKickGridTrainerCor2 trainer(&gridA,&gridB,&gridC);

Float_t x,y,z,pt,p,d=0,ptf,et1,et2;

Float_t theta,phi;
Int_t binTheta,binPhi;
Double_t S,a,b,c,pcalc,pw,Sf,Sfcalc;
Int_t nData,step;

/**************************** IMPLEMENTATION *******************************/  
TFile inFile(inFileName.Data());
TTree *input=(TTree *)inFile.Get(inputTreeName.Data());

input->SetBranchAddress("x",&x);
input->SetBranchAddress("y",&y);
input->SetBranchAddress("z",&z);
input->SetBranchAddress(bnMomentum.Data(),&p);
input->SetBranchAddress("d",&d);
input->SetBranchAddress("ptf",&ptf);
input->SetBranchAddress("et1",&et1);
input->SetBranchAddress("et2",&et2);
if      (mode == "mdc3")     input->SetBranchAddress("pt",&pt);
else if (mode == "meta")     {
  input->SetBranchAddress("ptg",&pt);
} else {
  cout << "Unknown mode "  << mode.Data() << endl;
  return;
}

nData=input->GetEntries();
step = nData / 20;

//----------- Obtain momentum fit parameters
TH2F multip("multip","multiplicity",(kMaxTheta-kMinTheta)/kGranularity,
	    kMinTheta,kMaxTheta,(kMaxPhi)/kGranularity,0,kMaxPhi);
TH2F multip_in("multip_in","multiplicity_input",
	       (kMaxTheta-kMinTheta)/kGranularity,kMinTheta,kMaxTheta,
	       (kMaxPhi)/kGranularity,0,kMaxPhi);

//First fit with a strong cut in P so to get a correctly even if b,c are not
cout << "Fitting high momenta:               "; cout.flush();
for (int i=0;i<nData; i++) {
  if (i % step == 0) {
    cout << '#';
    cout.flush();
  }
  if (input->GetEntry(i) == 0) cout << "error \n";

  if ( TMath::Sqrt(x*x+y*y+z*z)>1e-6 && TMath::Abs(pt)>0.) {
    theta=TMath::ACos(z/TMath::Sqrt(x*x+y*y+z*z))/TMath::Pi()*180;
    phi=TMath::ATan2(x,y)/TMath::Pi()*180;
    if (phi>kMinPhi && phi<kMaxPhi && p>kInitialPCut)
      if (theta>kMinTheta && theta<kMaxTheta) {
	multip_in->Fill(theta,TMath::Abs(phi));
	if ( TMath::Abs(d)<kMaxDist ) {
	  S = pt / p;	 
	  trainer.addPoint(theta,phi,p,1/S,1);
	}
      }
  }
}
trainer.endInput();
cout << endl;

//Use the previously obtained a to do better fits on b,c
for (Int_t ilow=0; ilow<1; ilow++) {
  cout << "Using pt as a momentum cut. Pass " << ilow << ": ";
  cout.flush();
  trainer.reset();
  multip.Reset();
  for (int i=0;i<nData; i++) {
    if (i % step == 0) {
      cout << '#';
      cout.flush();
    }
    if (input->GetEntry(i) == 0) cout << "error \n";
    if ( TMath::Sqrt(x*x+y*y+z*z)>1e-6 && TMath::Abs(pt)>0.) {
      theta=TMath::ACos(z/TMath::Sqrt(x*x+y*y+z*z))/TMath::Pi()*180;
      phi=TMath::ATan2(x,y)/TMath::Pi()*180;
      
      if (phi>kMinPhi && phi<kMaxPhi)
	if (theta>kMinTheta && theta<kMaxTheta) {

	  S = pt / p;
          a = gridA.getValue(theta,phi);
          if ( TMath::Abs(d)<kMaxDist && p>a) {
	    trainer.addPoint(theta,phi,p,1/S,1.);
	    multip.Fill(theta,TMath::Abs(phi));
	  }

	}
    }
  }
  trainer.endInput();
  cout << endl;
}

//Robustify fitting method: Throw away outliers...
for (Int_t ituk=0;ituk<10;ituk++) {
  cout << "Tukey weights. Pass " << ituk << ":              ";
  
  cout.flush();
  trainer.reset();
  multip.Reset();
  for (int i=0;i<nData; i++) {
    if (i % step == 0) {
      cout << '#';
      cout.flush();
    }
    if (input->GetEntry(i) == 0) cout << "error \n";
    if ( TMath::Sqrt(x*x+y*y+z*z)>1e-6 && TMath::Abs(pt)>0.) {
      theta=TMath::ACos(z/TMath::Sqrt(x*x+y*y+z*z))/TMath::Pi()*180;
      phi=TMath::ATan2(x,y)/TMath::Pi()*180;

      if (phi>kMinPhi && phi<kMaxPhi)
	if (theta>kMinTheta && theta<kMaxTheta) {

	  //P fit
	  S = pt / p;  // 2*sin(deltatheta/2)
	    a = gridA.getValue(theta,phi);
	    b = gridB.getValue(theta,phi);
	    c = gridC.getValue(theta,phi);
	    pcalc = a/S+b*S+c;
	    pw = (p - pcalc) * (p - pcalc) / (p*p*kTukey2);

	    if (pw < 1) {
	      pw = (1 - pw) * (1 - pw); //Tukey weight
	      if ( TMath::Abs(d)<kMaxDist) {
		trainer.addPoint(theta,phi,p,1/S,1./pw);
		multip.Fill(theta,TMath::Abs(phi));
	      }
	    }
	}
    }
  }
  trainer.endInput();
  cout << endl;
}

//Compute residuals
TFile outFile("res.root","RECREATE");
Text_t varlist[]="x:the:phi:pt:pc:d:p:a:b:c:xf:et1:et2";
TNtuple *tree=new TNtuple("Res","Res",varlist);
Float_t vars[18];
Float_t pull1;
Float_t pull2;

cout << "Computing residuals:                ";

cout.flush();
for (int i=0;i<nData; i++) {
  if (i % step == 0) {
    cout << '#';
    cout.flush();
  }
  input->GetEntry(i);
  if (p<0.) cout << "Error" << endl;
  if (TMath::Sqrt(x*x+y*y+z*z)>1e-6 && TMath::Abs(pt)>0.) {
    theta=TMath::ACos(z/TMath::Sqrt(x*x+y*y+z*z))/TMath::Pi()*180;
    phi=TMath::ATan2(x,y)/TMath::Pi()*180;
    if (phi>kMinPhi && phi<kMaxPhi)
      if (theta>kMinTheta && theta<kMaxTheta) {

	S = pt / p;  // 2*sin(deltatheta/2)
	Sf = sin( (et2-et1)/2. );
	  a = gridA.getValue(theta,phi);
	  b = gridB.getValue(theta,phi);
	  c = gridC.getValue(theta,phi);
	  pcalc = a/S + b*S +c;
	tree->Fill(S,theta,phi,pt,pcalc,d,p,a,b,c,Sf,et1,et2);
      }
  }
}    
cout << endl;
outFile.Write();
ofstream as(outParFile.Data());
gridA.print(as);
gridB.print(as);
gridC.print(as);
}
