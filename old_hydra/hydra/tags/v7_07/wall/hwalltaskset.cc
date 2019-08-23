//*-- AUTHOR : Ilse Koenig, Tomasz Wojcik, Wolfgang Koenig
//*-- Created : 19/01/2005

///////////////////////////////////////////////////////////////////////////////
//
// HWallTaskSet
//
// This HTaskSet contains the tasks for the Forward Wall
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hwalltaskset.h"
#include "hdebug.h"
#include "haddef.h"
#include <iostream> 
#include <iomanip>

HTask* HWallTaskSet::make(const char* select, Option_t* option) {
  // Returns a pointer to the task or taskset specified by 'select'
  HTaskSet *tasks = new HTaskSet("Wall","List of Forward Wall tasks");
  TString sel = select;
  TString opt = option;
  sel.ToLower();
  opt.ToLower();
  // actually no task defined
  return tasks;
}

ClassImp(HWallTaskSet)










