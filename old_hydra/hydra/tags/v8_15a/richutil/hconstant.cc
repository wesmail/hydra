// $Id: hconstant.cc,v 1.6 2006-08-12 12:53:41 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:18:55
//
using namespace std;
#include "hconstant.h"
#include <TMath.h>
#include <iostream> 
#include <iomanip>
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////
// HConstants: HADES physics/maths constants reference class
//
///////////////////////////////////////////////////////////

ClassImp(HConstant)

HConstant::HConstant()
{

}
HConstant::~HConstant()
{

}
Double_t HConstant::rad2deg()
{
    const Double_t rad2deg=180./TMath::Pi();
    return rad2deg;
}




