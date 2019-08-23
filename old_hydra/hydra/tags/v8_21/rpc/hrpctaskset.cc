//*-- Author  : D. Gonzalez-Diaz
//*-- Created : 14/06/2006
//*-- Modified: 24/08/2007 by P.Cabanelas

//_HADES_CLASS_DESCRIPTION
///////////////////////////////////////////////////////////////////////////////
//
// HRpcTaskSet
//
// This HRpcSet contains the tasks for the Rpc detector
//
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hrpctaskset.h"
#include "hdebug.h"
#include "haddef.h"
#include "hrpcdigitizer.h"
#include "hrpccalibrater.h"
#include "hrpchitf.h"
#include "hrpcclusterf.h"
#include <iostream> 
#include <iomanip>
#include "TObjString.h"

HRpcTaskSet::HRpcTaskSet(const Text_t name[],const Text_t title[]) : HTaskSet(name,title) {
  // Constructor
}


HRpcTaskSet::HRpcTaskSet(void) : HTaskSet() {
  // Default constructor
}


HRpcTaskSet::~HRpcTaskSet(void) {
  // Destructor.
}


HTask* HRpcTaskSet::make(const Char_t *select,const Option_t *option) {
  // Returns a pointer to the Rpc task or taskset specified by 'select'

  HTaskSet *tasks = new HTaskSet("Rpc","List of Rpc tasks");
  TString simulation="simulation";
  TString real ="real"; 

  TString sel = select;
  TString opt = option;
  
  sel.ToLower();
  opt.ToLower();

  if(sel.CompareTo(simulation)==0)
    {
      tasks->add( new HRpcDigitizer("rpc.digi","rpc.digi") );
      tasks->add( new HRpcHitF("rpc.hitf","rpc.hitf") );
      tasks->add( new HRpcClusterF("rpc.clusterf","rpc.clusterf") );
    }

  if(sel.CompareTo(real)==0) 
    {
      tasks->add( new HRpcCalibrater("rpc.cal","rpc.cal") );
      tasks->add( new HRpcHitF("rpc.hitf","rpc.hitf") );
      tasks->add( new HRpcClusterF("rpc.clusterf","rpc.clusterf") );
    }

  return tasks;
}

ClassImp(HRpcTaskSet)










