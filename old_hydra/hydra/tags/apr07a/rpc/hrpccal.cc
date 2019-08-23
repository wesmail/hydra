#pragma implementation
#include "hrpccal.h"
#include <TDirectory.h>
#include <TTree.h>

//*-- Author : D. Gonzalez-Diaz 08/06/2006
////////////////////////////
//HRpcCal
//
// This class contains RPC cal data
//
////////////////////////////

ClassImp(HRpcCal)

  void  HRpcCal::doit(void) {
    TTree *tree=(TTree *)gDirectory->Get("T");
    tree->Draw("HRpcCal.fData.fLeftT>>newhist"); //Really?
}

HRpcCal::HRpcCal(void) {
     rightT= leftT=0.0;
     rightTot= leftTot=0.0;
}

HRpcCal::~HRpcCal(void) {
}

