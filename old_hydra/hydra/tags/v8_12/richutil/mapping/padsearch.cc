// $Id: padsearch.cc,v 1.1 2005-11-08 13:36:30 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-11-08 14:34:14
//
// Algorithm by: Soenke Schoeder <soenke.schroeder@physik.tu-muenchen.de>
// Last update by Soenke Schroeder: 01/04/09 15:34:51
/*************************************************************************
   Purpose: calculate the polar angle theta for the RICH pad centers
   Input: pad corner coordinates in padplane coos
   All coordinates are given in meters
**************************************************************************/
#ifndef __CINT__
#include <iostream>
#include <istream>
#include <fstream>
#include <stdlib.h>
#include "TMath.h"
#include "TH2.h"
#include "TFile.h"
#include "TObjArray.h"
#include "TSystem.h"
using namespace std;

void print_usage()
{
    cout << "usage: padsearch FILE_input FILE_output <z-ccord>" << endl;
}

void print_hint()
{
    cout<< "z-coord not given, set to default z = 0.073 [m]" << endl;
}

void read3Corners(ifstream& ifs, Double_t& dx, Double_t& dy)
{
    Double_t x1, x2, x3, y1, y2, y3;
    x1=x2=x3=y1=y2=y3=0.;
    ifs>>x1>>y1;
    ifs>>x2>>y2;
    ifs>>x3>>y3;
    dx=(x1+x2+x3)/3000.;
    dy=(y1+y2+y3)/3000.;
}

void read4Corners(ifstream& ifs, Double_t& dx, Double_t& dy)
{
	Double_t x1, x2, x3, x4, y1, y2, y3, y4;
	x1=x2=x3=x4=y1=y2=y3=y4=0.;
	ifs>>x1>>y1;
	ifs>>x2>>y2;
	ifs>>x3>>y3;
	ifs>>x4>>y4;
	Double_t a=TMath::Sqrt(TMath::Power(x1-x2,2)+TMath::Power(y1-y2,2));
	Double_t b=TMath::Sqrt(TMath::Power(x2-x3,2)+TMath::Power(y2-y3,2));
	Double_t c=TMath::Sqrt(TMath::Power(x3-x1,2)+TMath::Power(y3-y1,2));
	Double_t s=(a+b+c)/2.;
	Double_t S1=TMath::Sqrt(s*(s-a)*(s-b)*(s-c));
	Double_t Mx1=(x1+x2+x3)/3000.;
	Double_t My1=(y1+y2+y3)/3000.;
	a=TMath::Sqrt(TMath::Power(x1-x3,2)+TMath::Power(y1-y3,2));
	b=TMath::Sqrt(TMath::Power(x3-x4,2)+TMath::Power(y3-y4,2));
	c=TMath::Sqrt(TMath::Power(x4-x1,2)+TMath::Power(y4-y1,2));
	s=(a+b+c)/2.;
	Double_t S2=TMath::Sqrt(s*(s-a)*(s-b)*(s-c));
	Double_t Mx2=(x1+x3+x4)/3000.;
	Double_t My2=(y1+y3+y4)/3000.;
	dx=(S1*Mx1+S2*Mx2)/(S1+S2);
	dy=(S1*My1+S2*My2)/(S1+S2);
}

void read5Corners(ifstream& ifs, Double_t& dx, Double_t& dy)
{
	Double_t x1, x2, x3, x4, x5, y1, y2, y3, y4, y5;
	x1=x2=x3=x4=x5=y1=y2=y3=y4=y5=0.;
	ifs>>x1>>y1;
	ifs>>x2>>y2;
	ifs>>x3>>y3;
	ifs>>x4>>y4;
	ifs>>x5>>y5;
	Double_t a=TMath::Sqrt(TMath::Power(x1-x2,2)+TMath::Power(y1-y2,2));
	Double_t b=TMath::Sqrt(TMath::Power(x2-x5,2)+TMath::Power(y2-y5,2));
	Double_t c=TMath::Sqrt(TMath::Power(x5-x1,2)+TMath::Power(y5-y1,2));
	Double_t s=(a+b+c)/2.;
	Double_t S1=TMath::Sqrt(s*(s-a)*(s-b)*(s-c));
	Double_t Mx1=(x1+x2+x5)/3000.;
	Double_t My1=(y1+y2+y5)/3000.;
	a=TMath::Sqrt(TMath::Power(x2-x3,2)+TMath::Power(y2-y3,2));
	b=TMath::Sqrt(TMath::Power(x3-x5,2)+TMath::Power(y3-y5,2));
	c=TMath::Sqrt(TMath::Power(x5-x2,2)+TMath::Power(y5-y2,2));
	s=(a+b+c)/2.;
	Double_t S2=TMath::Sqrt(s*(s-a)*(s-b)*(s-c));
	Double_t Mx2=(x2+x3+x5)/3000.;
	Double_t My2=(y2+y3+y5)/3000.;
	a=TMath::Sqrt(TMath::Power(x3-x4,2)+TMath::Power(y3-y4,2));
	b=TMath::Sqrt(TMath::Power(x4-x5,2)+TMath::Power(y4-y5,2));
	c=TMath::Sqrt(TMath::Power(x5-x3,2)+TMath::Power(y5-y3,2));
	s=(a+b+c)/2.;
	Double_t S3=TMath::Sqrt(s*(s-a)*(s-b)*(s-c));
	Double_t Mx3=(x3+x4+x5)/3000.;
	Double_t My3=(y3+y4+y5)/3000.;
	dx=(S1*Mx1+S2*Mx2+S3*Mx3)/(S1+S2+S3);
	dy=(S1*My1+S2*My2+S3*My3)/(S1+S2+S3);

} 
TH2D* createLookupTable(Char_t* cFilename, Double_t ty, Double_t tz)
{

ifstream fin(cFilename,ios::in);
Char_t* fname = new Char_t[30];
sprintf(fname,"padtable_y_%2.3f_z_%2.3f",ty,tz);
ofstream fout(fname,ios::out);
Double_t deg2rad = TMath::DegToRad();
Double_t rad2deg = TMath::RadToDeg();
Double_t alpha0  = 20. * deg2rad;
Double_t R       = 0.871;
Double_t mz      = -0.42;

TH2D *h = new TH2D(fname,fname,96,0,96,96,0,96);
TH2D *hphi = 0;
 Bool_t kPhiRef = kFALSE;
 if (tz - 0.073 < 1.e-4) kPhiRef = kTRUE;
 if (kPhiRef)     hphi = new TH2D("phi","phi",96,0,96,96,0,96);
 

 for (Int_t i=1;i<96;i++)
     for (Int_t j=1;j<96;j++)
	 h->SetBinContent(h->GetBin(i,j),-1.);
     

 Int_t corners = 0;
 Int_t padno = 0;
 Int_t nrPads = 4850;

for (Int_t i=0; i<nrPads; i++) {
    fin>>corners>>padno;
    Int_t col=padno/100;
    Int_t row=padno%100;
    Double_t dx,dy;
    dx=dy=0.;
    switch (corners)
    {
    case 3:
	read3Corners(fin,dx,dy);
	break;
    case 4:
	read4Corners(fin,dx,dy);
	break;
    case 5:
	read5Corners(fin,dx,dy);
	break;
    default:
	cout << "invalid number of pad corners, exiting ...." << endl;
	exit(1);
    }

    Double_t alpha=TMath::ATan(TMath::Tan(alpha0)*TMath::Cos(TMath::ATan(dx/dy)));
    //phi=TMath::ATan(dx/(dy*TMath::Cos(alpha0)));
    Double_t phi=TMath::ATan(dx/(dy*TMath::Cos(alpha0)))+TMath::Pi()/2.;
    Double_t dr=TMath::Sqrt(TMath::Power(dx,2)+TMath::Power(dy,2));
    Double_t theta1=0.;// starting point 1
    Double_t theta2=90.*deg2rad;// starting point 2
    Double_t dr1=0.;
    Double_t theta=0.;
    do {
	//cout<<tz<<"   "<<TMath::Abs(dr-dr1)<<endl;
	theta=(theta1+theta2)/2.;
	Double_t tantheta  = TMath::Tan(theta);
	Double_t tantheta2 = TMath::Power(tantheta,2);
	Double_t rz=1./2.*(
		  -2.  * ty * tantheta  + 
      		   2.  * tz * tantheta2 +
		   2.  * mz             +
		   2.  * TMath::Sqrt(
				     -2.                * mz                 * tantheta  * ty + 
				     2.                 * mz                 * tantheta2 * tz - 
				     tantheta2          * TMath::Power(mz,2)                  +
				     tantheta2          * TMath::Power(R,2)                   - 
				     TMath::Power(ty,2)                                       -  
				     tantheta2          * TMath::Power(tz,2)                  +
				     2.                 * ty                 * tantheta  * tz +
				     TMath::Power(R,2)
				    )
		  )    /   (TMath::Power(tantheta,2) + 1. );

	Double_t ry = ty + (rz-tz) * tantheta;
	Double_t tanalpha = TMath::Tan(alpha);
	Double_t Drzmz2   = TMath::Power(rz-mz,2);
	Double_t ry2      = TMath::Power(ry,2);
	Double_t uz = tz - 2. * ry      * (-ty * (rz-mz) + (tz-mz) * ry) / (Drzmz2 + ry2);
	Double_t uy = ty + 2. * (rz-mz) * (-ty * (rz-mz) + (tz-mz) * ry) / (Drzmz2 + ry2);
	Double_t hz = rz -      (uz-rz) * (ry  * tanalpha + rz) / (uz - rz + (uy - ry) * tanalpha);
	Double_t hy = ry -      (uy-ry) * (ry  * tanalpha + rz) / (uz - rz + (uy - ry) * tanalpha);
	dr1 = TMath::Sqrt(TMath::Power(hz,2) + TMath::Power(hy,2));
	if (dr>dr1) {
	    theta1=theta;
	}
	else {
	    theta2=theta;
	}
    } while (TMath::Abs(dr-dr1)>1.e-9);

    fout<<col*100+row<<" "<<theta*rad2deg<<" "<<phi*rad2deg<<endl;
    h->SetBinContent(h->GetBin(col,row),theta*rad2deg);

    if (kPhiRef) hphi->SetBinContent(hphi->GetBin(col,row),phi*rad2deg);

    
}
fin.close();
fout.close();
 TString sFname(fname);
 sFname.Prepend("gzip ");
 gSystem->Exec(sFname.Data());
 delete fname;
 if (kPhiRef)
 {
     TFile fphi("phiref.root","RECREATE");
     hphi->Write();
     fphi.Close();
     delete hphi;
 }
 return h;
}

int main(int argc, char **argv)
{
    /*------------------------------------------------------------------------------------
      photon emission point (ty,tz) [m]: in the used coos the point (0.,0.073) is the 
      nominal target point inside the RICH.

      This routine is supposed to calculate a table (2D histogram) of 
      lepton emission angles (only polar angles) for each 
      hit pad (ring center) of the padplane.

      Thus it is possible to create one table for each possible emission point (ty,tz)
      of liquid or segmented target.

      The differences of the table relative to the reference table for (0.,0.073)
      which is in ORACLE can then be parameterized as a function of theta. 
      A phi dependency is negligible.

      c.f. http://www.e12.physik.tu-muenchen.de/~teberl/RICH_optics/
    -------------------------------------------------------------------------------------*/

    //    Double_t ty      = 0.;                       //default=0.
    Double_t tz      = 0.073;                    //default=0.073
    Char_t* FILEIN   = 0;
    Char_t* FILEOUT  = 0;

    if (argc < 3)
    {
	print_usage();
	exit(1);
    }
    if (argc == 3)
    {
	print_usage();
	print_hint();
	FILEIN = argv[1];
	FILEOUT = argv[2];
    }
    if (argc == 4)
    {
	FILEIN = argv[1];
	FILEOUT = argv[2];
	tz = atof(argv[3]);
    }
    if (argc > 4)
    {
	print_usage();
	exit(1);
    }

    // Array with histograms, each histogram is a polar angle lookup table

    for (Int_t cally=0; cally<=100; cally++)
    {
	TObjArray *t = new TObjArray(100);
	for (Int_t callz=0; callz<=300; callz++)
	{
	    Double_t ty = -0.05 + 0.001 * cally;
	    Double_t tz = -0.1  + 0.001 * callz;
	    t->Add((TH2D*)createLookupTable(FILEIN,ty,tz));
	}
	Char_t* fname = new Char_t[10];
	sprintf(fname,"_y=%2.3f",-0.05 + 0.001 * cally);
	TString sFILEOUT(FILEOUT);
	sFILEOUT+=fname;
	sFILEOUT+=".root";
	delete fname;

	// write table histograms to ROOT file
	TFile frout(sFILEOUT.Data(),"RECREATE");
	for (Int_t table=0; table<(t->GetLast()+1); table++)
	{
	    ((TH2D*)(*t)[table])->Write();
	    delete (TH2D*)(*t)[table];
	}
	delete t;
	frout.Close();
    }

    // create only reference histo
    //    t->Add((TH2D*)createLookupTable(FILEIN,0.,0.073));

}


#endif
    //  fout<<""<<endl;
//      fout<<"Pad "<<padno<<" x (col) y (row) position = "<<col<<" "<<row<<endl;
//      fout<<"Pad "<<padno<<" linked pads nr = 1 "<<padno<<endl;
//      fout<<"Pad "<<padno<<" theta in LAB = "<<theta*180./TMath::Pi()<<endl;
//      //fout<<"Pad "<<padno<<" phi in LAB = "<<phi*180./TMath::Pi()<<endl;
//      fout<<"Pad "<<padno<<" corners = "<<corners<<endl;
//      fout<<"\\ "<<x1<<" "<<y1<<endl;
//      fout<<"\\ "<<x2<<" "<<y2<<endl;
//      fout<<"\\ "<<x3<<" "<<y3<<endl;
//      if (corners==4) {
//  	fout<<"\\ "<<x4<<" "<<y4<<endl;
//      }
//      if (corners==5) {
//  	fout<<"\\ "<<x4<<" "<<y4<<endl;
//  	fout<<"\\ "<<x5<<" "<<y5<<endl;
//      }
//      fout<<""<<endl;
//      fout<<"# ----------------------------------------------------------------------"<<endl;
