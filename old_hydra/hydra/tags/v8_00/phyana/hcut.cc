//*-- Author : Dan Magestro
//*-- Created: 01/10/01
//*-- Last modified: 07/11/01

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HCut
//
//  This class is the base class for all cut classes. Currently this
//  class doesn't do much itself.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////

#include <time.h>
#include <iostream> 
#include <TNamed.h>
#include "phyanadef.h"
#include "hcut.h"
#include "hades.h"

HCut::HCut() : TNamed() {
// Default constructor
  fNCheck = 0;
  fNGood = 0;

}

HCut::HCut(const Text_t *name, const Text_t *title)
         : TNamed(name, title) {
// Default constructor
  fNCheck = 0;
  fNGood = 0;
}

ClassImp(HCut)
