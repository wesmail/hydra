//*-- AUTHOR : Ilse Koenig
//*-- Created : 06/12/2001

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
//  HParCond
//
//  Base class for all parameter containers containing conditions
//
//  The following functions must be implemented by the derived class:
//
//    void putParams(HParamList*)    Fills all persistent data members into
//                                   the list for write.
//
//    Bool_t getParams(HParamList*)  Fills all persistent data members from
//                                   the list after reading. The function
//                                   returns false, when a data member is
//                                   not in the list.
//
///////////////////////////////////////////////////////////////////////////
using namespace std;
#include "hparcond.h"
#include "hparamlist.h"
#include <iostream> 
#include <iomanip>

ClassImp(HParCond)

void HParCond::printParams() {
  // prints information about the container
  cout<<"---------------------------------------------"<<'\n';
  cout<<"-----  "<<GetName()<<"  -----"<<'\n';
  if (!paramContext.IsNull()) cout<<"--  Context/Purpose:  "<<paramContext<<'\n';
  if (!author.IsNull()) cout<<"--  Author:           "<<author<<'\n';
  if (!description.IsNull()) cout<<"--  Description:      "<<description<<'\n';
  cout<<"---------------------------------------------"<<'\n';
  HParamList* condList = new HParamList;
  putParams(condList);
  condList->print();
  delete condList;
} 
 
