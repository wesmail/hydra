//*-- AUTHOR : Ilse Koenig, Tomasz Wojcik, Wolfgang Koenig
//*-- Created : 19/01/2005

//Modified 24/08/2005 Filip Krizek
//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HWallTaskSet
//
// This HTaskSet contains the tasks for the Forward Wall
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hwalltaskset.h"
#include "hwalldigitizer.h"//FK//
#include "hwallhitf.h"
#include "hwallhitfsim.h" //FK//
#include "hdebug.h"
#include "haddef.h"
#include <iostream> 
#include <iomanip>

HTask* HWallTaskSet::make(const char* select, Option_t* option) {
  // Returns a pointer to the task or taskset specified by 'select'
  HTaskSet *tasks = new HTaskSet("Wall","List of Forward Wall tasks");
  TString simulation="simulation"; //FK//
  TString real ="real";            //FK//
 

  TString sel = select;
  TString opt = option;
  sel.ToLower();
  opt.ToLower();

  if(sel.CompareTo(simulation)==0){
     tasks->add(new HWallDigitizer("wall.digi","wall.digi"));
     //tasks->add( new HWallHitF("wall.hitf","wall.hitf"));
     tasks->add( new HWallHitFSim("wall.hitfs","wall.hitfs"));
  }
  if(sel.CompareTo(real)==0){  	 	
     tasks->add( new HWallHitF("wall.hitf","wall.hitf"));
     //tasks->add( new HWallHitFSim("wall.hitfs","wall.hitfs"));  	
  }


  // actually no task defined
  return tasks;
}

ClassImp(HWallTaskSet)










