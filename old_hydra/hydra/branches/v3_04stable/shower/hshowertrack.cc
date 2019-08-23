//###################################################
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

#pragma implementation
#include "hshowertrack.h"
ClassImp(HShowerTrack)

////////// **********ShowerTrack defintion ****************/////////////77

int HShowerTrack::clear()
{
   m_lEventNr = -1;
   m_nAddress = -1;
   m_nTrack = -1;
 
   return 1;
}



