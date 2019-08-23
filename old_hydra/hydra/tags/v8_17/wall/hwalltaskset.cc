//*-- AUTHOR : Ilse Koenig, Tomasz Wojcik, Wolfgang Koenig
//*-- Created : 19/01/2005

//Modified 24/08/2005 Filip Krizek
// Modified by M.Golubeva 01.11.2006

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
#include "hwalltrbunpacker.h"//FK//
#include "hwalldigitizer.h"//FK//
#include "hwallcalibrater.h"
#include "hwallonehitf.h"
#include "hwallhitf.h"
#include "hwallhitfsim.h" //FK//
#include "hdebug.h"
#include "haddef.h"
#include <iostream> 
#include <iomanip>

HTask* HWallTaskSet::make(const Char_t* select, Option_t* option) {
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
  tasks->add( new HWallHitFSim("wall.hitfs","wall.hitfs"));
  }
  
  if(sel.CompareTo(real)==0){  	 	
    tasks->add( new HWallCalibrater("wall.cal","wall.cal") );
    tasks->add( new HWallOneHitF("wall.onehitf","wall.onehitf")); 
    tasks->add( new HWallHitF("wall.hitf","wall.hitf")); 
  }


  // actually no task defined
  return tasks;
}

ClassImp(HWallTaskSet)










