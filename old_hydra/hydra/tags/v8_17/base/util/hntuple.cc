// Author: Witold Przygoda, przygoda@if.uj.edu.pl, last modifed on: 1 May 2007

#include <iostream>
#include <stdexcept>
#include <string>
#include "hntuple.h"

using namespace std;

ClassImp(HNtuple)
// ---------------------------------------------------------------------------------
// Dummy constructor
// ---------------------------------------------------------------------------------
HNtuple::HNtuple() : name(0), title(0), bufsize(0), ptrNt(0), isNtuple(kFALSE), varArrayN(0), varArray(0)
{
}

// ---------------------------------------------------------------------------------
// This is ntuple constructor with "lazy"-delayed construction
// ntuple is created only after first fill attempt based on 
// variables which have been set 
// ---------------------------------------------------------------------------------
HNtuple::HNtuple(const Char_t* name, const Char_t* title, Int_t bufsize) : 
	name(name), title(title), bufsize(bufsize), ptrNt(0), isNtuple(kFALSE), varArrayN(0), varArray(0)
{
}

// ---------------------------------------------------------------------------------
// Basic ntuple constructor with exactly the same parameters as in the case of ROOT TNtuple
// ---------------------------------------------------------------------------------
HNtuple::HNtuple(const Char_t* name, const Char_t* title, const Char_t* varlist, Int_t bufsize) :
	name(name), title(title), bufsize(bufsize), isNtuple(kFALSE), varArrayN(0), varArray(0)
{
   ptrNt = new TNtuple(name, title, varlist, bufsize);
   setMap(varlist, isNtuple);
}

// ---------------------------------------------------------------------------------
void HNtuple::setMap(const Char_t* vlist, Bool_t& kPair)
{
   Int_t i = 0;
   varList = vlist;
   std::string sName = "";
   std::string::iterator sIter;
   for (sIter = varList.begin(); sIter != varList.end(); ++sIter)
   {
       if (*sIter != ':')
       {
          sName += *sIter;
       }
       else
       {
          // create a new pair ntuple variable - value
          vKeyOrder.insert(make_pair(sName, i++));
          if (kPair==kFALSE) vKeyValue.insert(make_pair(sName, 0.));
          sName.clear();
       }
   }
          // create the last new pair ntuple variable - value
          vKeyOrder.insert(make_pair(sName, i++));
          if (kPair==kFALSE) vKeyValue.insert(make_pair(sName, 0.));
          sName.clear();

   // create Float_t array based on variables string and number of ':' separators
   varArrayN = 1 + count_if(varList.begin(), varList.end(), std::bind2nd(std::equal_to<char>(), ':'));
   varArray = new Float_t[varArrayN];
   kPair = kTRUE;
}

// ---------------------------------------------------------------------------------
HNtuple::~HNtuple()
{
   if (ptrNt != 0) delete ptrNt;
   if (varArray != 0) delete [] varArray;
}


// ---------------------------------------------------------------------------------
Int_t HNtuple::Write(const Char_t* name, Int_t option, Int_t bufsize) 
{
   return ptrNt->Write(name, option, bufsize); 
}

// ---------------------------------------------------------------------------------
Float_t & HNtuple::operator[](const std::string& key)
{
   if (isNtuple)
   {
      mIter = vKeyValue.find(key);
      if (mIter != vKeyValue.end())
      {
         return mIter->second;
      }
      string out_error = "An unknown variable name >>  ";
      out_error += key;
      out_error += "  << in HNtuple tried to be assigned";
      throw std::invalid_argument(out_error);
//       throw std::invalid_argument("An unknown variable name in HNtuple tried to be assigned");
   }

return vKeyValue[key]; 
}

// ---------------------------------------------------------------------------------
const Float_t& HNtuple::operator[](const std::string& key) const
{
   std::map<std::string, Float_t>::const_iterator mcIter = vKeyValue.find(key);
   if (mcIter != vKeyValue.end())
   {
      return (mcIter->second); 
   }
   string out_error = "An unknown variable name ";
   out_error += key;
   out_error += " in HNtuple tried to be assigned";
   throw std::invalid_argument(out_error);
//    throw std::invalid_argument("An unknown variable name in HNtuple tried to be assigned");
}

// ---------------------------------------------------------------------------------
Bool_t HNtuple::setDefaults(Float_t ii)
{
   if (isNtuple==kTRUE)
   {
      for (mIter = vKeyValue.begin(); mIter != vKeyValue.end(); ++mIter)
      {
        (*mIter).second = ii;
      }
      return kTRUE;
   }
   return kFALSE;
}

// ---------------------------------------------------------------------------------
Int_t HNtuple::fill()
{
// This function is similar to Fill(...) from NTuple class besides
// the fact that is has no parameter and is small "f" :)

   Int_t i;
   if (isNtuple==kTRUE)
   {
      for (i = 0; i < varArrayN; ++i) varArray[i] = 0.;
      for (mIter = vKeyValue.begin(); mIter != vKeyValue.end(); ++mIter)
      {
        varArray[ vKeyOrder[(*mIter).first] ] = (*mIter).second;
        // reset of map array
        (*mIter).second = 0.;
      }
   }
   else
   {
      // ntuple not booked yet, we create it here based on variables 
      // set with the function setVal
      
      std::string vList;
      for (mIter = vKeyValue.begin(); mIter != vKeyValue.end(); ++mIter)
      {
         vList += (*mIter).first + ":";
      }
      vList.erase(vList.find_last_of(":"),1);

      //-------- here a part of NTuple Ctor
      ptrNt = new TNtuple(name, title, vList.c_str(), bufsize);
      isNtuple = kTRUE;
      setMap(vList.c_str(), isNtuple);
	 //-------- fill
         for (i = 0; i < varArrayN; ++i) varArray[i] = 0.;
         for (mIter = vKeyValue.begin(); mIter != vKeyValue.end(); ++mIter)
         {
           varArray[ vKeyOrder[(*mIter).first] ] = (*mIter).second;
           // reset of map array
           (*mIter).second = 0.;
         }
   }

   // filling the ROOT ntuple
   return ptrNt->Fill(varArray);
}
// ****************************************************************************



