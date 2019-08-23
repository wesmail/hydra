//*-- AUTHOR : Ilse Koenig
//*-- Created : 28/01/2009

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
//
//  HParCond
//
//  Base class for all condition-stype parameter containers
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

#include "hparcond.h"
#include "hparamlist.h"
#include <iostream> 
#include <iomanip>

ClassImp(HParCond)

void HParCond::printParams() {
  // prints information about the container
  std::cout<<"\n---------------------------------------------\n";
  std::cout<<"-----  "<<GetName()<<"  -----\n";
  if (!paramContext.IsNull()) std::cout<<"--  Context/Purpose:  "<<paramContext<<'\n';
  if (!author.IsNull())       std::cout<<"--  Author:           "<<author<<'\n';
  if (!description.IsNull())  std::cout<<"--  Description:      "<<description<<'\n';
  std::cout<<"---------------------------------------------\n";
  HParamList* condList = new HParamList;
  putParams(condList);
  condList->print();
  std::cout<<"---------------------------------------------\n\n";
  delete condList;
} 
 
