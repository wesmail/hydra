//*-- AUTHOR : Ilse Koenig
//*-- Created : 09/05/2005

///////////////////////////////////////////////////////////////////////////////
//
// HHodoTaskSet
//
// This HTaskSet contains the tasks for the Pion Hodoscopes
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hhodotaskset.h"
#include "hdebug.h"
#include "haddef.h"
#include <iostream> 
#include <iomanip>

HTask* HHodoTaskSet::make(const char* select, Option_t* option) {
  // Returns a pointer to the task or taskset specified by 'select'
  HTaskSet *tasks = new HTaskSet("Hodo","List of Hodoscopes tasks");
  TString sel = select;
  TString opt = option;
  sel.ToLower();
  opt.ToLower();
  // actually no task defined
  return tasks;
}

ClassImp(HHodoTaskSet)










