#include "hlocateddataobject.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 9/11/1998

////////////////////////////////
//HLocatedDataObject
//
// This is a data object which also stores it's location in the event structure
// This location is retrieved in a standard way through the functions 
//getNLocationIndex() (returns the number of indexes needed to locate the
//data object) and getLocationIndex(Int_t n) (which returns the value of the
//index number "n").
/////////////////////////////////

ClassImp(HLocatedDataObject)
