#include "TArrayI.h"
#include "hevent.h"
#include "heventheader.h"
#include "hdetector.h"
#include "hratreeext.h"
#include "hcategory.h"
#include "hmatrixcategory.h"
#include "hlinearcategory.h"
#include "hlinearcatiter.h"
#include "hlocation.h"
#include "hiterator.h"
#include "hdebug.h"
#include "hades.h"
#include "hypinfodef.h"
#include "hhyplistfiller.h"
#include <cmath> 

using namespace std;

ClassImp(HHypListFiller)

///////////////////////////////////////////////////////////////////////
//HHypListFiller
//
// HHypListFiller reconstructor searches local maxima in CAL level
//
//////////////////////////////////////////////////////////////////////

HHypListFiller::HHypListFiller(Text_t *name,Text_t *title) :
  HReconstructor(name,title)
{
}

HHypListFiller::HHypListFiller()
{
}


HHypListFiller::~HHypListFiller(void) {
}

Bool_t HHypListFiller::SetExitList(Int_t e_list)
{
    exitList = e_list;
    return kTRUE;
}

Bool_t HHypListFiller::init() 
{
    printf("initialization of HHypListFiller \n");

    //m_pContCat=gHades->getCurrentEvent()->getCategory(catTestCont);
    m_pContCat=gHades->getCurrentEvent()->getCategory(catHypList);
    if (!m_pContCat) {
      m_pContCat = HHypList::buildLinearCat("HHypList");


      if (!m_pContCat) return kFALSE;
      else gHades->getCurrentEvent()
                        ->addCategory(catHypList, m_pContCat, "HypList");
    }
	return kTRUE;
}

Int_t HHypListFiller::execute()
{

	Int_t index=0;
	HLocation locDummy;

	HHypList *hyplistcont = NULL;
	//HHypList *testcont = new HHypList(4,3);
	
    hyplistcont = (HHypList*) m_pContCat->getNewSlot(locDummy,&index);

    if ( hyplistcont != 0 ) hyplistcont = new(hyplistcont) HHypList;
    else return ERROR_IDX;

    hyplistcont->setList(exitList);
    hyplistcont->setNcomb(3);
    hyplistcont->setNpart(2);
    hyplistcont->createTables();
    
    hyplistcont->setIdx(0,0,12);
    hyplistcont->setIdx(1,0,13);
    hyplistcont->setIdx(2,0,14);
    hyplistcont->setIdx(0,1,112);
    hyplistcont->setIdx(1,1,113);
    hyplistcont->setIdx(2,1,114);
    
    hyplistcont->setProbComb(0,0.7);
    hyplistcont->setProbComb(1,0.8);
    hyplistcont->setProbComb(2,0.9);
    
    hyplistcont->setProbAlg(0,0.1);
    hyplistcont->setProbAlg(1,0.2);
    hyplistcont->setProbAlg(2,0.3);
    
    hyplistcont->print();
    return index;
}


Bool_t HHypListFiller::finalize()
{
    cout<<" \n\n\n HHypListFiller::finalize() \n\n";
	return kTRUE;
}
