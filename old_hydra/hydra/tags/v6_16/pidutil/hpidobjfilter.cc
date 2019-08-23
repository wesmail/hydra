// $Id: hpidobjfilter.cc,v 1.1 2003-02-26 08:53:11 eberl Exp $
// Last update by Thomas Eberl: 03/02/26 09:20:05
//
#include "hpidobjfilter.h"
#include "hlinearcategory.h"
#include "hevent.h"
#include "hspectrometer.h"
#include "hdetector.h"
#include "hrichdetector.h"
#include "hcategory.h"
#include "hiterator.h"
#include "hades.h"
#include "richdef.h"
#include "hruntimedb.h"
#include "hpidcutparticle.h"
#include "hpidfilter.h"
#include "hpidfl.h"
#include "piddef.h"
#include <iostream> 
ClassImp(HPidObjFilter)

HPidObjFilter::HPidObjFilter(Text_t *name,Text_t *title, char *swt) :
  HReconstructor(name,title)
{
    // set a string that switches on the requested filter 
    pFilterSwitch = new TString(swt);
}

HPidObjFilter::HPidObjFilter()
{

}

HPidObjFilter::~HPidObjFilter(void) 
{
 
}

Bool_t HPidObjFilter::init() 
{
    if (gHades) 
    {
    }    
    return kTRUE;
   
}

Bool_t HPidObjFilter::finalize() 
{
    return kTRUE;
}

Int_t HPidObjFilter::execute()
{

    // everybody can extend this execute func with his own filters
    if(pFilterSwitch->Contains("PID_PARTICLE_LEPTON"))
    {
	HPidCutParticle *pparticlecut = new HPidCutParticle("particlecut","default");
	pparticlecut->switchTo("lepton");
	HPidFilter pidfilter;
	pidfilter.initCuts(pparticlecut);
	((HLinearCategory*)HPidFL::getCategory(catPidPart))->filter(pidfilter);
	// save the cut with its inner state in the output file later
    }

    Int_t kReturnValue=0;//proceed normally

    // if evt does not contain an object of the filtered kind
    // after filtering, skip the event completely ??!!

    //    if (isEmptyEvt()) kReturnValue = kSkipEvent;

    return kReturnValue;
}

