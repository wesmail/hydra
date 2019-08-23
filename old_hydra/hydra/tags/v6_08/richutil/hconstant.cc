// File: hconstant.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 02/05/13 11:20:19
//
#include "hconstant.h"
#include <TMath.h>
#include <iostream.h>
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





