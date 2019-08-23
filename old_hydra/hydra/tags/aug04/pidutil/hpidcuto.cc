// $Id: hpidcuto.cc,v 1.1 2003-02-26 08:51:22 eberl Exp $
// Last update by Thomas Eberl: 03/02/25 14:18:32
// analogous to Dan's class in phyana

/////////////////////////////////////////////////////////////////////////
//
//  HPidCutO
//
//  This class is the base class for all cut classes. Currently this
//  class doesn't do much itself.
//
/////////////////////////////////////////////////////////////////////////
using namespace std;
#include <iostream>
#include <iomanip>
#include <TNamed.h>
#include "hpidcuto.h"


HPidCutO::HPidCutO() : TNamed() {
// Default constructor
  fNCheck = 0;
  fNGood = 0;

}

HPidCutO::HPidCutO(const Text_t *name, const Text_t *title)
         : TNamed(name, title) {
// Default constructor
  fNCheck = 0;
  fNGood = 0;
}

ClassImp(HPidCutO)
