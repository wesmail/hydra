// $Id: hconstant.cc,v 1.5 2003-01-24 18:24:10 halo Exp $
// Last update by Thomas Eberl: 02/09/25 17:18:55
//
using namespace std;
#include "hconstant.h"
#include <TMath.h>
#include <iostream> 
#include <iomanip>
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





