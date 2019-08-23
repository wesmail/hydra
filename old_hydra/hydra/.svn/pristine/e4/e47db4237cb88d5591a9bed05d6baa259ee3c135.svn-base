/**
 * @file   hdstproduction.h
 * @author Simon Lang
 * @date   Fri May  5 15:24:58 2006
 * 
 * @brief  Base class of DST Production Classes
 * 
 * $Id: hdstproduction.h,v 1.1.1.1 2006-09-08 07:39:37 halo Exp $
 *
 */

#ifndef HDSTPRODUCTION_H
#define HDSTPRODUCTION_H


// Root include files
#include "TObject.h"
// no forward declaration here, since derived classes might not use TString
#include "TString.h"

// Hydra include files
#include "haddef.h"
#include "showerdef.h"
#include "hstartdef.h"
#include "tofinodef.h"
#include "tofdef.h"
#include "richdef.h"
#include "hmdcdef.h"

// forward declarations
class HTaskSet;


class HDstProduction : public TObject
{
   protected:
      // we use a large buffer for Root type output trees
      static const Int_t TREE_BUFFER_SIZE;

      struct DstConfiguration_t
      {
	    Int_t triggerModules[1];   // we have just one trigger system
	    Int_t startModules[6];     // start, veto, ?, ?, ?, ? detector
	    Int_t richModules[1];      // whole RICH detector is one module
	    Int_t mdcModules[6][4];    // 6 chambers (sectors) per plane
	    Int_t showerModules[3];    // 3 cell layers in shower
	    Int_t tofModules[22];      // 22 x 8 stripes assemble our TOF
	    Int_t tofinoModules[6][1]; // all channel together per sector
	    Int_t rpcModules[1];       // yet unused
	    Int_t wallModules[1];      // yet unused
	    
	    Int_t mdcTestLayer[6][4];  // MDC wire layers used for tracking
	    Int_t mdcTestLevel[4];

	    TString dstId;
	    TString expId;
	    TString inputUrl;
	    TString outputDir;
	    TString qaOutputDir;
	    TString notPersistentCategories;
	    TString firstParameterSource;
	    TString secondParameterSource;
	    TString parameterOutputFile;

	    Int_t referenceRunId;
	    Int_t startWithEvent;
	    Int_t qaEventInterval;
	    Int_t eventCounterInterval;
	    Int_t processNumEvents;
      };
      DstConfiguration_t dstCfg;

      HTaskSet* masterTaskSet;

   public:
      HDstProduction();
      ~HDstProduction();

      Int_t   getReferenceRunId()       { return dstCfg.referenceRunId;       }
      Int_t   getStartWithEvent()       { return dstCfg.startWithEvent;       }
      Int_t   getProcessNumEvents()     { return dstCfg.processNumEvents;     }
      Int_t   getQaEventInterval()      { return dstCfg.qaEventInterval;      }
      Int_t   getEventCounterInterval() { return dstCfg.eventCounterInterval; }
      TString getDstId()                { return dstCfg.dstId;                }
      TString getInputFile()            { return dstCfg.inputUrl;             }
      TString getOutputDir()            { return dstCfg.outputDir;            }
      TString getQaOutputDir()          { return dstCfg.qaOutputDir;          }
      TString getParameterOutputFile()  { return dstCfg.parameterOutputFile;  }
      TString getNotPersistentCategories()
      {
	 return dstCfg.notPersistentCategories;
      }
      TString getFirstParameterSource()
      {
	 return dstCfg.firstParameterSource;
      }
      TString getSecondParameterSource()
      {
	 return dstCfg.secondParameterSource;
      }
      
      void configureDstProduction(int argc, char** argv);
      void printConfiguration();
      void setupSpectrometer();
      void setupParameterSources();
      void setupParameterOutput();
      void setupAllParameterOutput();
      void setupOutput();
      void initialize();
      void produceDst();
      void finalizeProduction();
      void terminateProduction( Int_t exit_code );


   ClassDef( HDstProduction, 0 ) // Base class of DST Production Classes
};


#endif   // HDSTPRODUCTION_H
