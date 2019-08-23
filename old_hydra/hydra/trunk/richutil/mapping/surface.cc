// $Id: surface.cc,v 1.2 2008-05-09 16:16:33 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-11-08 14:34:49
//
/*************************************************************************
   Purpose: calculate the hypersurface theta, delta_theta, z
            
   Input: pad table histograms
   All coordinates are given in meters
**************************************************************************/
#ifndef __CINT__
#include <iostream>
#include <istream>
#include <fstream>
#include <stdlib.h>
#include "TMath.h"
#include "TH2.h"
#include "TH3.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TString.h"
#include "TGraph.h"
using namespace std;


Double_t* getPolarAngleAveragedShift(const TH2D* h, const TH2D* hdef)
{
    const Int_t iThetaBins = 79;
    Double_t* arr = new Double_t[iThetaBins];
    
    for (Int_t iTheta = 0; iTheta < iThetaBins; iTheta++)
    {
	Double_t dThetaCurr = iTheta + 10.;
	Double_t dThetaSum = 0.;
	Int_t iThetaSumCnt = 0;
	for (Int_t i = 1; i < 96; i++)
	{
	    for (Int_t j = 1; j < 96; j++)
	    {
		if (hdef->GetBinContent(hdef->GetBin(i,j)) > dThetaCurr &&
		    hdef->GetBinContent(hdef->GetBin(i,j)) < dThetaCurr+1.)
		{
		    dThetaSum += (h->GetBinContent(h->GetBin(i,j)) -
				  hdef->GetBinContent(hdef->GetBin(i,j)));
		    iThetaSumCnt++;
		}
	    }
	}
	arr[iTheta] = dThetaSum / (Double_t) iThetaSumCnt;
    }

    //    for (Int_t i=0; i<iThetaBins; i++) cout<<i<<"  "<<arr[i]<<endl;
    return arr;

}

Double_t* getAzimuthalAngleAveragedShift(const TH2D* h, const TH2D* hdef, const TH2D* hphi)
{
    const Int_t iAzimBins = 59;
    Double_t* arr = new Double_t[iAzimBins];
    for (Int_t iAzim = 0; iAzim < iAzimBins; iAzim++) arr[iAzim]=0.;     
    for (Int_t iAzim = 2; iAzim < iAzimBins; iAzim++)
    {
	Double_t dAzimCurr = iAzim + (Double_t)iAzimBins;
	Double_t dAzimSum = 0.; // sum of pol angle shifts
	Int_t iAzimSumCnt = 0;
	for (Int_t i = 1; i < 96; i++)
	{
	    for (Int_t j = 1; j < 96; j++)
	    {
		// average over phi angle
		if (hphi->GetBinContent(hphi->GetBin(i,j)) > dAzimCurr &&
		    hphi->GetBinContent(hphi->GetBin(i,j)) < dAzimCurr+1.)
		{
		    dAzimSum += (h->GetBinContent(h->GetBin(i,j)) -
				  hdef->GetBinContent(hdef->GetBin(i,j)));
		    iAzimSumCnt++;
		}
	    }
	}
	arr[iAzim] = dAzimSum / (Double_t) iAzimSumCnt; // arith mean of polar shifts
    }

    //    for (Int_t i=2; i<iAzimBins; i++) cout<<i<<"  "<<arr[i]<<endl;
    return arr;

}

Int_t main(Int_t argc, Char_t **argv)
{
    Float_t ty = atof(argv[1])/1000.;
    Char_t* finname = new Char_t[70];
    sprintf(finname,"/home/teberl/d/RICH/mapping/Tables_y_z/polarangles_y=%2.3f.root",ty);
    TFile fin(finname);
    Char_t* fnamett = new Char_t[30];
    sprintf(fnamett,"padtable_y_%2.3f_z_0.073",ty);
    TH2D* hdefault = (TH2D*) fin.Get(fnamett);

    Int_t n_vertices = atoi(argv[3]);
    cout<<atof(argv[2])<<endl;
    Float_t fStartZ = (atof(argv[2])+73.)/1000.;
    cout<<fStartZ<< " " << n_vertices<<endl;
    Float_t fBinningZ = 0.001; //[m]
    Int_t iThetaBins = 79;
    Int_t iAzimBins = 55;

    //    Float_t ty = 0.050;
    TObjArray *t = new TObjArray(300);
    for (Int_t call=0; call<=n_vertices; call++)
    {
	Double_t tz = fStartZ + fBinningZ * call;
	if (tz == 0.073) continue;
	Char_t* fname = new Char_t[30];
	sprintf(fname,"padtable_y_%2.3f_z_%2.3f",ty,tz);
	TH2D* h = (TH2D*)fin.Get(fname);
	t->Add(h);
    }

    TFile phiin("/home/teberl/d/RICH/mapping/Tables_y_z/phiref.root");
    TH2D* hphi = (TH2D*) phiin.Get("phi");

    fStartZ= fStartZ*1000. - 73.;
    fBinningZ = fBinningZ*1000.;
    TH2D hsurf("hsurf","hsurf",n_vertices,fStartZ,fStartZ+n_vertices*fBinningZ,
	                       iThetaBins,10.,89.);
    TH2D hsurfphi("hsurfphi","hsurfphi",n_vertices,fStartZ,fStartZ+n_vertices*fBinningZ,
	                       iAzimBins,3.,58.);
    hsurf.Print();
//     Double_t* arr = 0;
//     Double_t* arrp = 0;
    for (Int_t table=0; table<(t->GetLast()+1); table++)
    {
	Double_t* arr = getPolarAngleAveragedShift( (TH2D*)(*t)[table], hdefault );
	Double_t* arrp = getAzimuthalAngleAveragedShift( (TH2D*)(*t)[table], hdefault, hphi );

	TString hn(((TH2D*)(*t)[table])->GetName());
	hn.Remove(0,hn.Length()-5);
	Float_t z = (atof(hn.Data()) - 0.073) * 1000.;

	//	cout<<z<<endl;
	for(Int_t i=0; i<iThetaBins; i++)
	{
	    Int_t k = hsurf.FindBin(z,i+10.);
	    hsurf.SetBinContent(k,arr[i]);
	}
	for(Int_t i=3; i<iAzimBins+4; i++)
	{
	    Int_t k = hsurfphi.FindBin(z,i+0.);
	    hsurfphi.SetBinContent(k,arrp[i]);
	}
	delete arr;
	delete arrp;
    }

    Char_t* foutname = new Char_t[70];
    sprintf(foutname,"/home/teberl/d/RICH/mapping/Tables_y_z/surface_y=%2.3f.root",ty);
    TFile fout(foutname,"RECREATE");
    hsurf.Write();
    hsurfphi.Write();
    fout.Close();
    return 0;
}

#endif
