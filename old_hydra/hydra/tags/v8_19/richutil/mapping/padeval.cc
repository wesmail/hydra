// $Id: padeval.cc,v 1.2 2008-05-09 16:16:33 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-11-08 14:34:27
//
/*************************************************************************
   Purpose: calculate min and max value of theta for each pad
            
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

 
Int_t main(Int_t argc, Char_t **argv)
{
    TFile fin("/home/teberl/hades/richpads/output/padtables.root");
    
    TH2D* hdefault = (TH2D*) fin.Get("padtable_z_0.073");

    TObjArray *t = new TObjArray(300);
    for (Int_t call = 0; call <= 2; call++)
    {
	Double_t zero = 0.073;
	Double_t tz = zero + 0.01 + 0.001 * call;
	if (tz == 0.073) continue;
	Char_t* fname = new Char_t[20];
	sprintf(fname,"padtable_z_%2.3f",tz);
	TH2D* h = (TH2D*)fin.Get(fname);
	t->Add(h);
    }

    TH2D hmin("hmin","hmin",96,0,96,96,0,96);
    TH2D hmax("hmax","hmax",96,0,96,96,0,96);
    TH2D hmin_z("hmin_z","hmin_z",96,0,96,96,0,96);
    TH2D hmax_z("hmax_z","hmax_z",96,0,96,96,0,96);

    TH3D h3d("h3d","h3d",60,0.033,0.093,96,0.,96.,96,0.,96.);
    for (Int_t table=0; table<(t->GetLast()+1); table++)
    {
	TH2D *htmp = 0;
	htmp = (TH2D*) hdefault->Clone("diff");
	htmp->Add(((TH2D*)(*t)[table]),-1.);

	TString hn(((TH2D*)(*t)[table])->GetName());
	hn.Remove(0,hn.Length()-5);
	Float_t z = atof(hn.Data());
	//		cout<<z<<endl;
	for (Int_t i=1;i<96;i++)
	{
	    for (Int_t j=1;j<96;j++)
	    {
		h3d.SetBinContent(h3d.GetBin(table,i,j),
                ((TH2D*)(*t)[table])->GetBinContent(((TH2D*)(*t)[table])->GetBin(i,j)));
		if (table==0)
		{
		    hmin.SetBinContent(hmin.GetBin(i,j),
				       htmp->GetBinContent(htmp->GetBin(i,j))
				       );
		    hmax.SetBinContent(hmax.GetBin(i,j),
				       htmp->GetBinContent(htmp->GetBin(i,j))
				       );
		    
		    hmin_z.SetBinContent(hmin_z.GetBin(i,j),z);
		    hmax_z.SetBinContent(hmax_z.GetBin(i,j),z);
		}
		else
		{
		    Double_t newval = htmp->GetBinContent(htmp->GetBin(i,j));
		    Double_t maxval = hmax.GetBinContent(hmax.GetBin(i,j));
		    Double_t minval = hmin.GetBinContent(hmin.GetBin(i,j));
		    if (newval > maxval) 
		    {
			hmax.SetBinContent(hmax.GetBin(i,j),newval);
			hmax_z.SetBinContent(hmax_z.GetBin(i,j),z);
		   }
		    if (newval < minval) 
		    {
			hmin.SetBinContent(hmin.GetBin(i,j),newval);
			hmin_z.SetBinContent(hmin_z.GetBin(i,j),z);
		    }

		}
	    }
	}
	delete htmp;
    }


    TH2D hdiff("hdiff","hdiff",96,0,96,96,0,96);
    for (Int_t i=1;i<96;i++)
    {
	for (Int_t j=1;j<96;j++)
	{
	    hdiff.SetBinContent(hdiff.GetBin(i,j),
				TMath::Abs(hmax.GetBinContent(hmax.GetBin(i,j))-
					   hmin.GetBinContent(hmin.GetBin(i,j)))
				);
	}
	
    }

    cout.setf(ios::fixed);
    cout.precision(2);

    cout<<"x  y theta  absdiff  z_min  downshift  z_max  upshift"<<endl;


    // count different theta values
    Double_t thetatmp=0.;
    Int_t ncount = 0;
    Double_t theta[95*95];
    Double_t diff[95*95];
    Double_t min[95*95];
    Double_t max[95*95];
    Int_t ihelp[95*95+1];
    for (Int_t i=1;i<96;i++)
    {
	for (Int_t j=1;j<96;j++)
	{
	    ihelp[ncount] =  0;
	    theta[ncount] =  hdefault->GetBinContent(hdefault->GetBin(i,j));
	    diff[ncount]  =  hdiff.GetBinContent(hdiff.GetBin(i,j));
	    min[ncount]   =  hmin.GetBinContent(hmin.GetBin(i,j));
	    max[ncount]   =  hmax.GetBinContent(hmax.GetBin(i,j));
	    ncount++;
	}
    }

    TMath::Sort(95*95, theta, ihelp, kFALSE);

    ncount = 0;
    for (Int_t k=0; k<95*95; k++) 
    {
	Double_t thetatmp2 = theta[ihelp[k]];
	if (thetatmp2 < 0 ) continue;
	if (TMath::Abs(thetatmp2-thetatmp) > 1.e-1 )
	{
	    ncount++;
	    thetatmp = thetatmp2;
	    //	    cout<<thetatmp<<endl;
	}
    }


    Double_t* ntheta = new Double_t[ncount];
    Double_t* ndiff = new Double_t[ncount];
    Double_t* nmin = new Double_t[ncount];
    Double_t* nmax = new Double_t[ncount];
    Int_t* nihelp = new Int_t[ncount+1];
    
    thetatmp=0.;
    ncount=0;
    for (Int_t k=0; k<95*95; k++) 
    {

	//cout<<theta[ihelp[k]]<<endl;
	Double_t thetatmp2 = theta[ihelp[k]];
	if (thetatmp2 < 0 ) continue;
	if (TMath::Abs(thetatmp2-thetatmp) > 1.e-1 )
	{
	    nihelp[ncount] = ihelp[k];
	    ntheta[ncount] = theta[ihelp[k]];
	    ndiff[ncount] = diff[ihelp[k]];
	    nmin[ncount] = min[ihelp[k]];
	    nmax[ncount] = max[ihelp[k]];
	    thetatmp = thetatmp2;
	    //	    cout<<ncount<<" "<<ntheta[ncount]<<endl;
	    ncount++;
	}
    }




    cout<<"Number of different theta values is "<<ncount<<endl;

    
    for (Int_t i=1;i<96;i++)
    {
	for (Int_t j=1;j<96;j++)
	{
	    
	    if (hdefault->GetBinContent(hdefault->GetBin(i,j)) > -1)
	    {
		cout<<i<<"    "<<j<<"   [ "<<
		    hdefault->GetBinContent(hdefault->GetBin(i,j))+
		    hmin.GetBinContent(hmin.GetBin(i,j))<<" < "<<
		    hdefault->GetBinContent(hdefault->GetBin(i,j))<<" < "<<
		    hdefault->GetBinContent(hdefault->GetBin(i,j))+
		    hmax.GetBinContent(hmax.GetBin(i,j))<<" ] | "<<

		    hdiff.GetBinContent(hdiff.GetBin(i,j))<<" | "<<

		    (hmax_z.GetBinContent(hmax_z.GetBin(i,j))-0.073)*1000.<<"  "<<
		    hmax.GetBinContent(hmax.GetBin(i,j))<<" | "<<

		    (hmin_z.GetBinContent(hmin_z.GetBin(i,j))-0.073)*1000.<<"  "<<
		    hmin.GetBinContent(hmin.GetBin(i,j))<<endl;

	    }
	}
	
    }
    
    TGraph gdiff(ncount,ntheta,ndiff);
    gdiff.SetName("gdiff");
    gdiff.SetTitle("gdiff");
    TGraph gmin(ncount,ntheta,nmin);
    gmin.SetName("gmin");
    gmin.SetTitle("gmin");
    TGraph gmax(ncount,ntheta,nmax);
    gmax.SetName("gmax");
    gmax.SetTitle("gmax");

    TFile fout("diffout.root","RECREATE");
    hmin.Write();
    hmax.Write();
    hmin_z.Write();
    hmax_z.Write();
    hdiff.Write();
    h3d.Write();
    gdiff.Write();
    gmin.Write();
    gmax.Write();
    fout.Close();
}
#endif
