{
///////////////////////////////////////////////////////////
// This file has been automatically generated 
// (Mon Dec 11 16:30:37 2000 by ROOT version2.25/03)
// from HTree T/T.2
// found on file: /d/hades/koenig/xx00319054346RingWK.root
///////////////////////////////////////////////////////////


//Reset ROOT and connect tree file
   gROOT->Reset();
//   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/d/hades/koenig/xx00319054346RingWK.root");
   TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("/d/hades/koenig/testWK.root");
   if (!f) {
//      f = new TFile("/d/hades/koenig/xx00319054346RingWK.root");
      f = new TFile("/d/hades/koenig/testWK.root");
   }
   HTree *T = (HTree*)gDirectory->Get("T");

   if(!T) {cout << "No tree in file!" << endl; exit();}

   HRecEvent* fEvent = (HRecEvent*)f->Get("Event");
   if(fEvent) HEventHeader* fHead = fEvent->getHeader();
   T->SetBranchAddress("Event",&fEvent);

   f->cd("dirRich"); 
   HRichHit * ptlHRichHit = new HRichHit();
   HLinearCategory* catlHRichHit = (HLinearCategory*) gDirectory->Get("HRichHit");
   TBranch* brlHRichHit = T->GetBranch("HRichHit");
   if(brlHRichHit) brlHRichHit->SetAddress(&catlHRichHit);

//   HRichCal* ptmHRichCal = new HRichCal();
//   HMatrixCategory* catmHRichCal = (HMatrixCategory*) gDirectory->Get("HRichCal");
//   TBranch* brmHRichCal = T->GetBranch("HRichCal");
//   if(brmHRichCal) brmHRichCal->SetAddress(&catmHRichCal);

   HRichHitHeader* ptlHRichHitHeader = new HRichHitHeader();
   HLinearCategory* catlHRichHitHeader = (HLinearCategory*) gDirectory->Get("HRichHitHeader");
   TBranch* brlHRichHitHeader = T->GetBranch("HRichHitHeader");
   if(brlHRichHitHeader) brlHRichHitHeader->SetAddress(&catlHRichHitHeader);

   f->cd();

//     This is the loop skeleton
//     To read only selected branches, Insert statements like:
// T->SetBranchStatus("*",0);  // disable all branches
// T->SetBranchStatus("branchname",1);  // activate branchname

   Int_t nentries = T->GetEntries();

   Int_t nbytes = 0;
   Float_t theta, phi;
   HRichHit * hit1;
   HRichHit * hit2;
   Float_t factor=1.0F/57.2958F;
   Float_t phi1, phi2, theta1, theta2;
   TH1F * opang = new TH1F("opang","pair angles",91,0,180);
   TH1F * centroid = new TH1F("centroid","close pair centroids",41,0,4.1);
   TH1F * meanAmpl = new TH1F("meanAmpl","mean amplitudes",101,0,100);

   for (Int_t i=0; i<nentries;i++) {
      nbytes += brlHRichHit->GetEntry(i);
      
      if (catlHRichHit->getEntries() >= 2) {
	hit1 = (HRichHit *)catlHRichHit->getObject(0);
	hit2 = (HRichHit *)catlHRichHit->getObject(1);
        phi=hit1->fPhi;
	if(phi<60.0F) phi=30.0F-(phi-30.0F)*phiCorr;
	else if(phi<120.0F) phi=90.0F-(phi-90.0F)*phiCorr;
	else if(phi<180.0F) phi=150.0F-(phi-150.0F)*phiCorr;
	else if(phi<240.0F) phi=210.0F-(phi-210.0F)*phiCorr;
	else if(phi<300.0F) phi=270.0F-(phi-270.0F)*phiCorr;
	else phi=330.0F-(phi-330.0F)*phiCorr;
        phi1=phi*factor;
        phi=hit1->fPhi;
	if(phi<60.0F) phi=30.0F-(phi-30.0F)*phiCorr;
	else if(phi<120.0F) phi=90.0F-(phi-90.0F)*phiCorr;
	else if(phi<180.0F) phi=150.0F-(phi-150.0F)*phiCorr;
	else if(phi<240.0F) phi=210.0F-(phi-210.0F)*phiCorr;
	else if(phi<300.0F) phi=270.0F-(phi-270.0F)*phiCorr;
	else phi=330.0F-(phi-330.0F)*phiCorr;
        phi2=phi*factor;
        theta1=(hit1->fTheta+2.0F)*factor;
        theta2=(hit2->fTheta+2.0F)*factor;
        theta=acos(sin(theta1)*sin(theta2)*
	     (cos(phi1)*cos(phi2)+sin(phi1)*sin(phi2))+
	     cos(theta1)*cos(theta2))*57.2958;
	opang->Fill(theta);
	if(theta<8) {
	    centroid->Fill(hit1.centroid);
	    centroid->Fill(hit2->centroid);
	    meanAmpl->Fill(hit1->iRingAmplitude/hit1->iRingPadNr);
	    meanAmpl->Fill(hit2->iRingAmplitude/hit2->iRingPadNr);
            cout<<hit1->centroid<<", "<<hit2->centroid<<endl;
	}
      };

      catlHRichHit->Clear();
   }
}
