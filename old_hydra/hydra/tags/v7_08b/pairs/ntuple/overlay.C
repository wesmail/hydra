{

gROOT->SetBatch();

TFile fe("histsE.root");
TFile fs("histsS.root");
TFile fout("spectra_sim_exp.root","RECREATE");

TIter keylistS(fs.GetListOfKeys());
Int_t nn = 0;
    TKey *keyS=0;
    TH1 *hS=0;
    while ((keyS=(TKey*)keylistS()))
    {
	nn++;
	char* nameS=keyS->GetName();
	cout<<nn<<"  -- Processing : "<<nameS<<endl;
        hS = (TH1*) fs.Get(nameS);
        if (hS)
        {
	    
	    TString ee(nameS);
	    if (ee.Contains("true"))
	    {
		TCanvas* c = new TCanvas(nameS,nameS,600,600);
		TString ss(nameS);
		if (ss.Contains("norm"))
		{
		    hS->SetMinimum(1.e-10);
		    hS->SetMaximum(1.e-5);
		}
		hS->SetMarkerStyle(22);
		hS->SetMarkerColor(4);
		hS->SetLineColor(4);//blue
		c->SetLogy();
		hS->Draw("e1goff");
		fout->cd();
		c->Write();
		delete c;
	    }
	    else
	    {
		TIter keylistE(fe.GetListOfKeys());
		TKey *keyE=0;
		while ((keyE=(TKey*)keylistE()))
		{
		    // compare key names to find match
		    char* nameE=keyE->GetName();
		    if (!strcmp(nameS,nameE))
		    {
			TH1* hE = (TH1*) fe.Get(nameE);
			if (hE)
			{
			    TCanvas* c = new TCanvas(nameE,nameE,600,600);
			    hS->SetMarkerStyle(22);
			    hS->SetMarkerColor(4);
			    hS->SetLineColor(4);//blue
			    hE->SetMarkerStyle(20);
			    hE->SetMarkerColor(2);
			    hE->SetLineColor(2);//red
			    c->SetLogy();
			    TString ss(nameS);
			    if (ss.Contains("norm"))
			    {
				hS->SetMinimum(1.e-10);
				hS->SetMaximum(1.e-5);
				hS->Draw("e1goff");
				hE->Draw("samegoff");
			    }
			    else if(ss.Contains("norm")&&
				    ss.Contains("rap"))
			    {
				hS->SetMinimum(1.e-10);
				hS->SetMaximum(1.e-4);
				hS->Draw("e1goff");
				hE->Draw("samegoff");
			    }
			    else
			    {
				hE->Draw("e1goff");
				hS->Draw("samee1goff");
			    }

			    fout->cd();
			    c->Write();
			    delete c;
			}
		    }
		} // end while over exp histo keys
	    } //endif sim histo only
	    
	} // endif sim histo pointer
	else Error("","no sim histogram for key found");
	    

    } // end of while over keylist simfile
}
