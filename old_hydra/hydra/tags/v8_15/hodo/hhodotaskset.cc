//*-- AUTHOR : Ilse Koenig
//*-- Created : 09/05/2005
//*-- Modified: 30.3.2006 B. Spruck

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
//
// HHodoTaskSet
//
// This HTaskSet contains the tasks for the (now only Pion) Hodoscopes
// by default added will be the HodoCalibrator task
// 
// Options:
// (All needs as a parameter the number of a Hodo module they should use)
// 
// "fiber=[0-3]"   - hit finder for fiber hodoscopes, add HHodoHitFFiber task
// "diamond=[0-3]" - hit finder for diamond, add HHodoHitFDiamond task
// "pionbar=[0-3]" - *NOT USED YET* hit finder for pion bar hodoscopes
//
// "startcopymod=[0-3]" - adds a task which copies hit information to start hit category
//
///////////////////////////////////////////////////////////////////////////////

using namespace std;
#include "hhodotaskset.h"
#include "hhodocalibrater.h"
#include "hhodohitffiber.h"
#include "hhodohitfdiamond.h"
#include "hhodohit2start.h"
#include "hdebug.h"
#include "haddef.h"
#include <iostream>
#include <iomanip>
#include "TObjString.h"

HTask* HHodoTaskSet::make(const char *sel, Option_t* opt)
{
  // Returns a pointer to the task or taskset specified by 'select'
  HTaskSet *tasks = new HTaskSet("Hodo","List of Hodoscopes tasks");
  TString option=opt;
  TString select=sel;
  
  select.ToLower();
  option.ToLower();

  if(option.Contains("apr07")) option+=",fiber=0,diamond=1,diamond=2,diamond=3,startcopymod=1";
  
  if( select.Contains("sim") ) {
    Error("HodoTaskSet::make","No Simulation Task existing for HODO");
  }
  if( select.Contains("real") ) {
    // Calibrator for all Modules
    tasks->add( new HHodoCalibrater("hodo.cal","Hodo cal") );

    // But! Different HitFinders for different modules
	 for(Int_t i=0; i<10; i++){
		TString tmp;
		tmp="fiber=";
		tmp+=i;
		if(option.Contains(tmp)){
        HHodoHitFFiber *t;
        t=new HHodoHitFFiber("hodo.hitffiber","Hodo hitffiber");
        t->set_mod(i);
		  tasks->add(t);
      }
		tmp="diamond=";
		tmp+=i;
		if(option.Contains(tmp)){
        HHodoHitFDiamond *t;
        t=new HHodoHitFDiamond("hodo.hitfdiamond","Hodo hitfdiamond");
        t->set_mod(i);
		  tasks->add(t);
      }
/*    if(option.Contains("pionbars")){
      tasks->add(new HHodoHitFPionBar("hodo.hitfpionbar","Hodo hitfpionbar"););
    }*/
	 }

    // And only ONE module can be copied to start Hit
    if(option.Contains("startcopymod")){
      Int_t m=-1;
      if(option.Contains("startcopymod=0")) m=0;
      if(option.Contains("startcopymod=1")) m=1;
      if(option.Contains("startcopymod=2")) m=2;
      if(option.Contains("startcopymod=3")) m=3;
      if(option.Contains("startcopymod=4")) m=4;
      if(option.Contains("startcopymod=5")) m=5;
      if(option.Contains("startcopymod=6")) m=6;
      if(option.Contains("startcopymod=7")) m=7;
      if(option.Contains("startcopymod=8")) m=8;
      if(option.Contains("startcopymod=9")) m=9;
	 
#warning "Has to be optimized"
		if(m==-1){
			cerr << "startcopymod needs =value [0-9] " 
<< endl;
		}else{
      	HHodoHit2Start *t=new HHodoHit2Start("hodo.hit2start","Hodo hit2start");
      	t->set_mod(m);
      	tasks->add(t);
		}
    }
  }
  return tasks;
}

ClassImp(HHodoTaskSet);
