//*-- AUTHOR : D Gonzalez-Diaz
//*-- created: 14/06/2006

using namespace std;
#include "hrpctaskset.h"
#include "hdebug.h"
#include "haddef.h"
#include "hrpcdigitizer.h"
#include <iostream> 
#include <iomanip>

///////////////////////////////////////////////////////////////////////////////
// HRpcTaskSet
//
// This HRpcSet contains the tasks for the Rpc detector
//
//
///////////////////////////////////////////////////////////////////////////////

HRpcTaskSet::HRpcTaskSet(Text_t name[],Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HRpcTaskSet::HRpcTaskSet(void) : HTaskSet() {
  // Default constructor
}


HRpcTaskSet::~HRpcTaskSet(void) {
  // Destructor.
}


HTask* HRpcTaskSet::make(const char *select, Option_t *option) {
  // Returns a pointer to the Rpc task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Rpc","List of Rpc tasks");
  TString simulation="simulation";
  TString real ="real";

  TString sel = select;
  TString opt = option;
  sel.ToLower();

  if(sel.CompareTo(simulation)==0)
    {
      tasks->add( new HRpcDigitizer("rpc.digi","rpc.digi") );
    }

  return tasks;
}

ClassImp(HRpcTaskSet)










