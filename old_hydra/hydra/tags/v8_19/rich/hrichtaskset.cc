//*-- Author : Dan Magestro
//*-- Modified: 19/11/2001  by D. Magestro

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////////
// HRichTaskSet
//
// This HTaskSet contains the task sets for the Rich detector.
// It can be distinguished between "real" and "simulation" task set by
// passing the corresponding string to the first argument of the make()
// function. Embedding is automatically recognized by return value of
// gHades->getEmbeddingMode(). For embedding ONLY the "real" task set 
// has to be selected, otherwise the program will finish with an error. 
//
// To control the operation of the RICH tasks, HRichTaskSet has following
// switches with preset default values:
//   - inoiseOn   = kTRUE;
//   - iSkipEvent = kFALSE;
//
// The simulation of noise in HRichDigitizer behaves in pure
// simulation differently as compared to the embedding modus.
//    * pure simulation: simulation of both correlated and random noise is ON
//    * embedding:       only simulation of correlated noise is ON
//
// Default values can be changed by passing corresponding strings
// to the second argument of make() function. For more details,
// please read the documentation for this function below.
//
// BEGIN_HTML For more information, see
// <a target="_top" href="http://webdb.gsi.de/pls/hades_webdbs/hanal.hal_posting_query.show_posting_text?p_id=436">
// this analysis logbook entry<a>. END_HTML
//
///////////////////////////////////////////////////////////////////////////////

#include "TObjString.h"

#include "hades.h"
#include "haddef.h"
#include "hdebug.h"
#include "hrichanalysis.h"
#include "hrichanalysissim.h"
#include "hrichdigitizer.h"
#include "hrichtaskset.h"
#include "htask.h"

#include <iomanip>
#include <iostream> 

using namespace std;

ClassImp(HRichTaskSet)

Bool_t HRichTaskSet::inoiseOn   = kTRUE;
Bool_t HRichTaskSet::iSkipEvent = kFALSE;

HRichTaskSet::HRichTaskSet(Text_t name[], Text_t title[]) : HTaskSet(name, title)
{
   // Constructor
}


HRichTaskSet::HRichTaskSet(void) : HTaskSet()
{
   // Default constructor
}


HRichTaskSet::~HRichTaskSet(void)
{
   // Destructor.
}


HTask* 
HRichTaskSet::make(const Char_t *select, Option_t *option)
{
// Returns a pointer to the Rich task or taskset specified by 'select'.
// There are two possibilities to be passed:
//   - "real" for real data analysis and embedding
//   - "simulation" for simulation only
//
// Following options can be passed to the HRichTaskSet to control the
// behavior of the RICH analysis and simulation:
//   - "skipevent" HRichAnalysis task will return kSkipEvent for each
//                 event w/o found ring
//   - "noiseoff"  Turns OFF noise simulation in HRichDigitizer.
// For more information see BEGIN_HTML
// <a target="_top" href="http://forum.gsi.de/index.php?t=rview&th=2193&rid=334">
// this forum entry<a>. END_HTML

         HTaskSet* tasks      = new HTaskSet("Rich", "List of Rich tasks");
   const TString   simulation = "simulation";
   const TString   real       = "real";
         TString   sel        = select;
         TString   opt        = option;

   sel.ToLower();
   if ( 0 == sel.CompareTo(simulation) &&
        0 != gHades->getEmbeddingMode()   )
   {
      cout << "Error:HRichTaskSet::make() , embeddingMode > 0 in simulation NOT ALLOWED!" << endl;
      exit(1);
   }


   parseArguments(opt);
   if ( 0 == sel.CompareTo(simulation) &&
        0 == gHades->getEmbeddingMode()   )
   {
      tasks->add(new HRichDigitizer("rich.digi",  "Rich digitizer", inoiseOn));
      tasks->add(new HRichAnalysisSim("rich.ana", "Ring finder",    iSkipEvent));
   }

   if ( 0 == sel.CompareTo(real) &&
        0 == gHades->getEmbeddingMode() )
   {
      tasks->add(new HRichAnalysis("rich.ana", "Ring finder", iSkipEvent));
   }

   if( 0 == sel.CompareTo(real) &&
       0 != gHades->getEmbeddingMode() )
   {
      tasks->add(new HRichDigitizer("rich.digi",  "Rich digitizer", inoiseOn));
      tasks->add(new HRichAnalysisSim("rich.ana", "Ring finder",    iSkipEvent));
   }
   return tasks;
}

void
HRichTaskSet::parseArguments(TString s1)
{
   const TString skipEvent = "skipevent";
   const TString noiseOff  = "noiseoff";

   TIterator*   myiter     = NULL;
   TObjString*  stemp      = NULL;
   Char_t*      mystring   = NULL;
   Char_t*      buffer     = NULL;
   Int_t        count      = 0;
   TString      argument;
   TList        myarguments;

   s1.ToLower();
   s1.ReplaceAll(" ","");

   if( 0 != s1.Length() )
   {
      mystring = const_cast<Char_t*>(s1.Data());
      
      while( 1 ) // find all token in option string and put them to a list
      {
         if( 0 == count )
         {
            buffer = strtok(mystring, ",");
            stemp  = new TObjString(buffer);
            myarguments.Add(stemp);
         }

         if( ! (buffer=strtok(NULL, ",")) )
            break;
         stemp = new TObjString(buffer);
         myarguments.Add(stemp);
	  
         count++;
      }

      // iterate over the list of arguments and compare it
      // to the known key words.
      myiter = myarguments.MakeIterator();
      myiter->Reset();
      while ( 0 != (stemp=static_cast<TObjString*>(myiter->Next())) )
      {
         argument = stemp->GetString();
         if ( 0 == argument.CompareTo(skipEvent) )
         {
            iSkipEvent = kTRUE;
         }
         else if ( 0 == argument.CompareTo(noiseOff) )
         {
            inoiseOn  = kFALSE;
         }
      }
      delete myiter;
      myiter = NULL;

   }
}
