#include "htriggerhistfillerutil.h"

/// Internal dataobject
/**
 */
TofTypePrivat::TofTypePrivat(void)
{
	clear();
}

TofTypePrivat::TofTypePrivat(Float_t phi, Float_t theta, Float_t timep, Int_t module, Int_t cellp, Int_t sector)
{
	ph = phi;
	th = theta;
	time = timep;
	mod = module;
	cell = cellp;
	det = sector;
}

void TofTypePrivat::clear(void)
{
	ph = 0;
	th = 0;
	time = 0;
	mod = 0;
	cell = 0;
	det = 0;
}

/// Internal dataobject
/**
 */
ThetaCorrPrivat::ThetaCorrPrivat(void)
{
	thetadiff2 = 0;
	anaHit = 0;
	ipuHit = 0;
}

ThetaCorrPrivat::ThetaCorrPrivat(Float_t theta, Int_t ana, Int_t ipu)
{
	thetadiff2 = theta;
	anaHit = ana;
	ipuHit = ipu;
}

///Internal dataobject
/**
 */
CorrPrivat::CorrPrivat(void)
{
	anaPhi = 0;
	anaTheta = 0;
	ipuPhi = 0;
	ipuTheta = 0;
	anaTime = 0;
	ipuTime = 0;
	anaSec = 0;
}

CorrPrivat::CorrPrivat(Int_t aSec, Float_t aPhi, Float_t aTheta, Float_t iPhi, Float_t iTheta, Float_t aTime, Float_t iTime, Int_t aDet)
{
	anaPhi = aPhi;
	anaTheta = aTheta;
	ipuPhi = iPhi;
	ipuTheta = iTheta;
	anaTime = aTime;
	ipuTime = iTime;
	anaSec = aSec;
	anaDet = aDet;
}

CorrShowerCord::CorrShowerCord(void)
{
  Phi = 0;
  Theta = 0;
  MF = 0;
}
CorrShowerCord::CorrShowerCord(Float_t aphi, Float_t atheta, Int_t amf)
{
  Phi = aphi;
  Theta = atheta;
  MF = amf;
}

CorrShowerDiff::CorrShowerDiff(void)
{
  Diff = 0;
  MUSho = 0;
  Shower = 0;
}

CorrShowerDiff::CorrShowerDiff(Float_t adiff, Int_t mushw, Int_t shw)
{
  Diff = adiff;
  MUSho = mushw;
  Shower = shw;
}
