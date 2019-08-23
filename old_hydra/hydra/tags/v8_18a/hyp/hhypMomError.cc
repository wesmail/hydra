//*-- AUTHOR : B. Spruck
//*-- Created: 12/06/2007 by B. Spruck

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypMomError
//
// Temporary solution as Long_t as the Momentum errors for particles
// are not determined correctly by the tracking.
// This is used by all Kinematic Fitting algos!
//
////////////////////////////////////////////////////////////////////////

#include "hhypMomError.h"
#include "TMath.h"

ClassImp (HHypMomError)

HHypMomError HypMomErr;

HHypMomError::HHypMomError ()
 {
   jan04=apr06=apr07=false;
 }

HHypMomError::~HHypMomError ()
 {
 }

Bool_t HHypMomError::init(void)
 {
  return kTRUE;
 }

// Workaround as even static arrays cannot be part of a class
// not sure if this is a bug or a feature

static const Double_t HHypMomError_detrk4[7][5] = {
	{3.04669e-06, 2.92245e-06, 2.88894e-06, 2.54182e-06, 1.79336e-06},
	{3.45534e-06, 3.52212e-06, 3.43443e-06, 3.3966e-06,  3.31397e-06},
	{5.01234e-06, 5.46206e-06, 5.25006e-06, 5.06055e-06, 4.87875e-06},
	{6.29485e-06, 7.83364e-06, 8.64567e-06, 7.44434e-06, 6.44697e-06},
	{5.90814e-06, 7.49372e-06, 8.44064e-06, 7.33158e-06, 5.8983e-06 },
	{5.00201e-06, 6.82347e-06, 8.42634e-06, 6.64071e-06, 4.32283e-06},
	{4.57113e-06, 7.85962e-06, 9.27056e-06, 7.13838e-06, 4.2649e-06 },
};

static const  Double_t HHypMomError_multscrk4[7][5] = {
	{0.00712136, 0.00774792, 0.00745848, 0.0072562, 0.0077318},
	{0.00947987, 0.00919469, 0.00923363, 0.0090577, 0.0088233},
	{0.0104807 , 0.0105168 , 0.0114445 , 0.0109526, 0.0103268},
	{0.0122493 , 0.013979  , 0.0148295 , 0.0142261, 0.0125651},
	{0.0132593 , 0.0151324 , 0.0171913 , 0.0157784, 0.0133762},
	{0.0127184 , 0.0166672 , 0.0197425 , 0.0166028, 0.0127183},
	{0.0121717 , 0.0187349 , 0.0235083 , 0.0183666, 0.0114857}
};

static const  Double_t HHypMomError_detrk3[7][5] = {
	{5.08616e-06, 5.57472e-06, 5.90756e-06, 5.30146e-06, 5.12577e-06},
	{8.64475e-06, 8.44705e-06, 8.58983e-06, 7.7759e-06 , 6.88931e-06},
	{1.23261e-05, 1.42034e-05, 1.55287e-05, 1.31425e-05, 1.10076e-05},
	{1.75029e-05, 2.59217e-05, 2.72391e-05, 2.46139e-05, 1.84151e-05},
	{1.75554e-05, 2.25547e-05, 2.5371e-05 , 2.30453e-05, 1.8036e-05 },
	{1.5159e-05 , 2.14014e-05, 2.37335e-05, 2.17028e-05, 1.56393e-05},
	{1.70025e-05, 2.42703e-05, 2.65468e-05, 2.33554e-05, 1.5726e-05 }
};

static const  Double_t HHypMomError_multscrk3[7][5] = {
	{0.00705671, 0.00707791, 0.00719795, 0.00686656, 0.00468438},
	{0.00791494, 0.00971418, 0.00922887, 0.00921739, 0.00882323},
	{0.0101972 , 0.0108932 , 0.0109951 , 0.0111292 , 0.0103408 },
	{0.0136591 , 0.0147924 , 0.0152897 , 0.0153152 , 0.0135126 },
	{0.014621  , 0.0166669 , 0.0175123 , 0.0176646 , 0.0157295 },
	{0.0146804 , 0.0180094 , 0.0183208 , 0.0172132 , 0.014879  },
	{0.0130367 , 0.018929  , 0.0221153 , 0.0189616 , 0.0147333 }
};

static const  Double_t HHypMomError_detkick[7][5] = {
	{0.000095203, 0.000095204, 0.000104141, 0.000099331, 0.000099331},
	{0.000151284, 0.000148333, 0.000151814, 0.000147953, 0.000143505},
	{0.000197612, 0.000196251, 0.000191317, 0.000195112, 0.000203217},
	{0.000156319, 0.000157184, 0.000163081, 0.000157349, 0.000162052},
	{0.00014439 , 0.000153818, 0.000170083, 0.000160573, 0.000139122},
	{0.000199936, 0.000236463, 0.000262118, 0.000240549, 0.000199932},
	{0.000226558, 0.000296774, 0.00032661 , 0.000292281, 0.000226592}
};

Double_t HHypMomError::getMomError(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec,Bool_t simu)
{
	// Check which Beamtime
	// Only change in Geant is number of Chambers!
	if(jan04){// Jan04
		if(simu){
			return getMomErrGeantJan04( theta,  phi,  mass,  p, sec);
		}else{
			return getMomErrJan04( theta,  phi,  mass,  p, sec);
		}
	}else if(apr07){// Apr07
		if(simu){
			return getMomErrGeantApr07( theta,  phi,  mass,  p, sec);
		}else{
			// In first Approx, Apr07 exp is geant
			return getMomErrGeantApr07( theta,  phi,  mass,  p, sec);
		}
//	}else if(apr06){// Apr06
	}else{
		Error("HHypMomError::","beamtime not defined or no data for beamtime");
		return 0.0;
	}
}

Double_t HHypMomError::getMomErrGeantApr07(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec)
{
	// how many chambers per sec
	// Int_t snr[6]={4,4,4,4,4,4};
	// code
	theta*=TMath::RadToDeg();
   phi*=TMath::RadToDeg();

   if (phi<0.) phi += 360.;                // TLorentzVector uses ATan2(x,y)
   if(theta <= 15 || theta > 85) return -1.0;// Invalid
   Int_t thetaBin = int((theta-15)/10);
   Int_t phiBin = int((phi - int(phi/60)*60)/12);
   Double_t c1, c2, res;

	// case 4
	c1 = HHypMomError_detrk4[thetaBin][phiBin];
	c2 = HHypMomError_multscrk4[thetaBin][phiBin];
	res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));

	return res;
}

Double_t HHypMomError::getMomErrGeantJan04(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec)
{
	// how many chambers per sec
	Int_t snr[6]={4,4,3,4,4,3};
	// code
	theta*=TMath::RadToDeg();
   phi*=TMath::RadToDeg();

	Int_t iSetup;
	iSetup=snr[sec];

   if (phi<0.) phi += 360.;                // TLorentzVector uses ATan2(x,y)
   if(theta <= 15 || theta > 85) return -1.0;// Invalid
   Int_t thetaBin = int((theta-15)/10);
   Int_t phiBin = int((phi - int(phi/60)*60)/12);
   Double_t c1, c2, res;
// 	 c1 = detkick[thetaBin][phiBin];
// 	 res = c1*p*p;
// 	 cout << p << " " << res << " " ;
// 	 c1 = detrk3[thetaBin][phiBin];
// 	 c2 = multscrk3[thetaBin][phiBin];
// 	 res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));
// 	 cout << res << " ";
// 	 c1 = detrk4[thetaBin][phiBin];
// 	 c2 = multscrk4[thetaBin][phiBin];
// 	 res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));
// 	 cout << res << endl;

	switch(iSetup)
	{
		case 2:
			c1 = HHypMomError_detkick[thetaBin][phiBin];
			res = c1*p*p;
			break;

		case 3:
			c1 = HHypMomError_detrk3[thetaBin][phiBin];
			c2 = HHypMomError_multscrk3[thetaBin][phiBin];
			res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));
			break;

		case 4:
			c1 = HHypMomError_detrk4[thetaBin][phiBin];
			c2 = HHypMomError_multscrk4[thetaBin][phiBin];
			res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));
			break;

		default:
			return -1.0;// Invalid
	}

	return res;
}

Double_t HHypMomError::getMomErrJan04(Double_t theta, Double_t phi, Double_t mass, Double_t p,Int_t sec)
{
	// how many chambers per sec
	Int_t snr[6]={4,4,3,4,4,3};
	// code
	theta*=TMath::RadToDeg();
   phi*=TMath::RadToDeg();

	Double_t coeff[6][2] = {{7, 12},{ 5, 8},{ 7, 12},{ 4, 7},{ 4, 7},{ 3, 6}};

	Double_t c1, c2, res, scale;

	if (phi<0.) phi += 360.;
	if(theta <= 15 || theta > 85) return -1.0;// Invalid

	Int_t thetaBin = int((theta-15)/10);
	Int_t phiBin = int((phi - int(phi/60)*60)/12);

//	Int_t sec = int(phi/60) - 1;
//	if(sec < 0) sec = 5;
	Int_t iSetup;
	iSetup=snr[sec];

	scale = (thetaBin < 2 ? coeff[sec][1] : coeff[sec][0]);

	switch(iSetup)
	{
		case 2:
			c1 = HHypMomError_detkick[thetaBin][phiBin];
			res = c1*p*p;
			break;

		case 3:
			c1 = HHypMomError_detrk3[thetaBin][phiBin];
			c1 *= scale;
			c2 = HHypMomError_multscrk3[thetaBin][phiBin];
			res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));
			break;

		case 4:
			c1 = HHypMomError_detrk4[thetaBin][phiBin];
			c1 *= scale;
			c2 = HHypMomError_multscrk4[thetaBin][phiBin];
			res = sqrt(c1*c1*p*p*p*p + c2*c2*(p*p + mass*mass));
			break;

		default:
			return -1.0;// Invalid
	}

	return res;
}

void HHypMomError::getFaktors(
			Double_t &FakErrMomExp,Double_t &FakErrThetaExp,Double_t &FakErrPhiExp,
			Double_t &FakErrMomSim,Double_t &FakErrThetaSim,Double_t &FakErrPhiSim)
{
	// These factors are "hand made" to fit pulls to 1
	// and get confidence level flat
	// one should think about some better managment of these factors,
	// maybe in database
	if(jan04){// Jan04
		FakErrMomSim=0.65;
		FakErrThetaSim=0.95;
		FakErrPhiSim=0.95 ;
		FakErrMomExp=0.7;// war 1.37;
		FakErrThetaExp=1.0; // war 1.11;
		FakErrPhiExp=1.0 ; //war 1.11;
	}else if(apr07){// Apr07 NOT TUNED
		FakErrMomSim=1.37;
		FakErrThetaSim=1.37;
		FakErrPhiSim=1.37;
		FakErrMomExp=1.37;
		FakErrThetaExp=1.37;
		FakErrPhiExp=1.37;
	}else if(apr06){// Apr06 NOT TUNED
		FakErrMomSim=1.37;
		FakErrThetaSim=1.11;
		FakErrPhiSim=1.11;
		FakErrMomExp=1.37;
		FakErrThetaExp=1.11;
		FakErrPhiExp=1.11;
	}else{
		Error("HHypMomError::","beamtime not defined or no data for beamtime");
	}
}
