// File: hconstant.cc
//
// Author: Thomas Eberl <Thomas.Eberl@physik.tu-muenchen.de>
// Last update by Thomas Eberl: 01/08/30 13:05:58
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
