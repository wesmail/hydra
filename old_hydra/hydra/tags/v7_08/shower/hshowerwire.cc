using namespace std;
#include <iostream> 
#include <iomanip>
#include <stdlib.h>
#include <math.h>
#include "hshowerwire.h"
#include "hshowergeantwire.h"

ClassImp(HShowerWire)
ClassImp(HShowerWireTab)

/////////////////////////////////////////////////////////////
// HShowerWire
// Class describe coordinates one wire 
//
// HShowerWireTab
// Table of local coordinates of wires
//
/////////////////////////////////////////////////////////////

HShowerWire::HShowerWire() {
   nWireNr = 0;
   fYwir = 0.;
} // eof constructor

HShowerWire::~HShowerWire() {
} // eof destructor

HShowerWireTab::HShowerWireTab()
{
//creating empty table
   reset();
}

HShowerWireTab::~HShowerWireTab()
{
   reset();
}

Bool_t HShowerWireTab::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing wires table from ascii file (HAsciiKey format)
  Bool_t status = kTRUE;

  if (!pHadAsciiFile)
     return kFALSE;

  char buf[80];

  try {

     sprintf(buf, "Shower Module %d - Wires Coordinates", m_nModuleID);
     printf("%s initializating ...", buf);
   
     HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
     HAsciiKey &mydata = *pHAscii;

     if(!mydata.SetActiveSection(buf))
        return kFALSE;

     int nrWires = mydata.ReadInt("Wires");
     HShowerWire* pWire;
   
     setDistWire(mydata.ReadFloat("Distance"));
   
     for (int i=0; i<nrWires; i++) {
       pWire = new HShowerWire;
       pWire->setNrWire(i);
       pWire->setYWire(mydata.ReadFloat("Wires Coordinates",i));
       addWire(pWire);
     }

     printf("\tdone\n");
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}


Bool_t HShowerWireTab::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//writing wires table into ascii file (HAsciiKey format)
	Bool_t status = kTRUE;
	if (!pHadAsciiFile)
		return kFALSE;
	
	char buf[80];
	try {
		sprintf(buf, "Shower Module %d - Wires Coordinates", m_nModuleID);
		HAsciiKey* pHAscii = pHadAsciiFile->GetKeyAscii();
		HAsciiKey &mydata = *pHAscii;
		mydata.WriteSection(buf);
		mydata.WriteInt("Wires",getWireNr());
		mydata.WriteFloat("Distance",getDistWire());	
		TArrayF temp(getWireNr());
		mydata.SetNumInLine(8);
		for (int i=0; i<getWireNr(); i++) {
			HShowerWire* pWire = getWire(i);
			temp[i] = pWire->getYWire();
		}
		mydata.WriteFloat("Wires Coordinates*",&temp);
	}	
	catch (Bool_t ret) {
    	status = ret;
  	}

	return status;
}


void HShowerWireTab::reset()
{
//clearing data 
   fDistWire = 0.0;

   nWires = 0;
   m_nModuleID = -1;
   m_WiresArr.Delete();

}


HShowerWire* HShowerWireTab::getWire(Int_t n) {
//return information about one wire
   if ((n < 0) || (n >= nWires))
      return NULL;

   return (HShowerWire*)m_WiresArr.At(n);
}

Int_t HShowerWireTab::addWire(HShowerWire* pWire) {
//add new wire information 
  m_WiresArr.Add(pWire);
  return nWires++;
}

Int_t HShowerWireTab::setWire(HShowerWire* pWire, Int_t n) {
//set wire information at n pos
   if ((n < 0) || (n >= nWires))
      return 0;

   delete m_WiresArr.At(n);
   m_WiresArr.AddAt(pWire, n);

   return 1;

}

Int_t HShowerWireTab::lookupWire(float y)
{
  //return number of wire at position y
  //
  int nWireNr = -1;;
  float Ywir;
  for(int j = 0; j < nWires; j++)
  {	 
     Ywir = getWire(j)->getYWire();
     if ((y >= Ywir - fDistWire) && (y < Ywir + fDistWire))
     {
        nWireNr = j;
        break;
     }
  }

  return nWireNr;
}

