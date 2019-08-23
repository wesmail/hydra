{
//Core macro to calculate kickplane azimuthal deflection parameters.
//Input is an ntuple from kickResol.C
  
/**************************** DECLARATIONS *********************************/
HKickGrid gridA(kMinTheta,kMaxTheta,kMinPhi,kMaxPhi,kGranularity,kSimetry);
HKickGrid gridB(kMinTheta,kMaxTheta,kMinPhi,kMaxPhi,kGranularity,kSimetry);
HKickGrid gridC(kMinTheta,kMaxTheta,kMinPhi,kMaxPhi,kGranularity,kSimetry);
HKickGridTrainerPol2 trainer(&gridA,&gridB,&gridC);

Float_t theta,phi;
Float_t a,b,c,pcalc,pw,Sf,p,Sfcalc,pt,x;
Int_t nData,step;

/**************************** IMPLEMENTATION *******************************/  
TFile inFile(inFileName.Data());
TTree *input=(TTree *)inFile.Get(inputTreeName.Data());

input->SetBranchAddress("the",&theta);
input->SetBranchAddress("phi",&phi);
input->SetBranchAddress("pc",&pcalc);
input->SetBranchAddress("p",&p);
input->SetBranchAddress("a",&pt);
input->SetBranchAddress("xf",&Sf);
input->SetBranchAddress("x",&x);

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

  Sf = (phi<=0.)?Sf:-Sf;
  if (fabs(x)<2 && pcalc>kInitialPCut && p>kInitialPCut && fabs((p-pcalc)/p)<kMaxPError) {
    multip_in->Fill(theta,fabs(phi));
    trainer.addPoint(theta,phi,pcalc*Sf,Sf,1);
  }
}
trainer.endInput();
cout << endl;

//Robustify fitting method: Throw away outliers...
Int_t count=0;
for (Int_t ituk=0;ituk<3;ituk++) {
  count=0;
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

    // P fit
    Sf = (phi<=0.)?Sf:-Sf;
    a = gridA.getValue(theta,phi);
    b = gridB.getValue(theta,phi);
    c = gridC.getValue(theta,phi);
    if (fabs(Sf)>1e-6 && p>kInitialPCut && pcalc>kInitialPCut && fabs((p-pcalc)/p)<kMaxPError && fabs(x)<2) {
      Sfcalc = a + b*Sf + c*Sf*Sf;
      pw = (pcalc*Sf - Sfcalc) * (pcalc*Sf - Sfcalc) / (36);
	//(pcalc*pcalc*Sf*Sf*kTukey2);
      
      if (pw < 1) {
	pw = (1 - pw) * (1 - pw); //Tukey weight
	trainer.addPoint(theta,phi,pcalc*Sf,Sf,1./pw);
	multip.Fill(theta,TMath::Abs(phi));
      }
    }
  }
  trainer.endInput();
  cout << endl;
}

//Compute residuals
TFile outFile("resmatch.root","RECREATE");
Text_t varlist[]="the:phi:p:pc:a:b:c:xf:xfc";
TNtuple *tree=new TNtuple("Resm","Resm",varlist);
Float_t vars[18];
Float_t pull1;
Float_t pull2;


trainer.finalize();
cout << "Computing residuals:                ";
cout.flush();

for (int i=0;i<nData; i++) {
  if (i % step == 0) {
    cout << '#';
    cout.flush();
  }
  input->GetEntry(i);

  Sf = (phi<0.)?Sf:-Sf;
  a = gridA.getValue(theta,phi);
  b = gridB.getValue(theta,phi);
  c = gridC.getValue(theta,phi);
//   if (fabs(pcalc)>1e-6)
//     Sfcalc = (a+b*Sf+c*Sf*Sf)/pcalc; 
//   else
//     Sfcalc = 1.;
//   if (fabs(theta-50)<2.5 && fabs(phi+20)<2.5) 
//     cout << a << ":" << b << ":" << c << endl;
  Sfcalc = c*( pcalc - a - sqrt((pcalc-a)*(pcalc-a) - b));
  if (fabs(x)<2)
    tree->Fill(theta,phi,p,pcalc,a,b,c,Sf,Sfcalc);
}    
cout << endl;
outFile.Write();

ofstream as(outParFile.Data());
gridA.print(as);
gridB.print(as);
gridC.print(as);
}
