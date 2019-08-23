/**
 * @file   hdstproduction.cc
 * @author Simon Lang
 * @date   Fri May  5 16:16:39 2006
 * 
 * @brief  Base class of DST Production Classes
 * 
 * $Id: hdstproduction.cc,v 1.1.1.1 2006-09-08 07:39:37 halo Exp $
 *
 */

// STL header files
#include <iostream>
#include <cassert>

extern "C" {
#  ifndef _GNU_SOURCE
#     define _GNU_SOURCE
#  endif
#  include <getopt.h>
}

// Root header files
#include "TString.h"
#include "TSystem.h"

#include "TApplication.h"

// Hydra header files
#include "hdstproduction.h"
#include "hdstutilities.h"

#include "hades.h"
#include "hspectrometer.h"
#include "hrichdetector.h"
#include "hmdcdetector.h"
#include "htofdetector.h"
#include "htofinodetector.h"
#include "hshowerdetector.h"
#include "hstartdetector.h"
#include "htboxdetector.h"
#include "htriggerdetector.h"

#include "hruntimedb.h"
#include "hparoraio.h"
#include "hparrootfileio.h"
#include "hparasciifileio.h"

#include "hevent.h"
#include "hcategory.h"
#include "htaskset.h"

#include "haddef.h"
#include "hgeantdef.h"
#include "hododef.h"
#include "hypinfodef.h"
#include "kickdef.h"
#include "hmdcdef.h"
#include "hmdctrackddef.h"
#include "hmdctrackgdef.h"
#include "mdcsdef.h"
#include "horadef.h"
#include "horasimdef.h"
#include "pairsdef.h"
#include "phyanadef.h"
#include "piddef.h"
#include "richdef.h"
#include "richutildef.h"
#include "showerdef.h"
#include "showertofinodef.h"
#include "simulationdef.h"
#include "hstartdef.h"
#include "tofdef.h"
#include "triggerinfodef.h"
#include "walldef.h"
#include "rpcdef.h"
#include "tofinodef.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////
//
//  Base class of DST Production Classes.
//
//  This is not an abstract base class, however it is not intended to create
//  instances of this class. Instead, create an object of one of the classes
//  derived from here. Therefore, the constructor of this class is protected.
//
//_____________________________________________________________________________

// this macro is used to implement Root's class features (e.g. the streamer)
ClassImp(HDstProduction)

// we use a large buffer for Root type output trees
const Int_t HDstProduction::TREE_BUFFER_SIZE = 8000;


HDstProduction::HDstProduction()
{
   dstCfg.richModules[0]    = 0;
   dstCfg.showerModules[0]  = 0;
   dstCfg.showerModules[1]  = 0;
   dstCfg.showerModules[2]  = 0;
   dstCfg.triggerModules[0] = 0;
   dstCfg.rpcModules[0]     = 0;
   dstCfg.wallModules[0]    = 0;

   for (UInt_t i = 0; i < 22; i++)
   {
      dstCfg.tofModules[i] = 0;
   }

   for (UInt_t i = 0; i < 6; i++)
   {
      for (UInt_t j = 0; j < 4; j++)
      {
	 dstCfg.mdcModules[i][j]   = 0;
	 dstCfg.mdcTestLayer[i][j] = 0;
      }
      dstCfg.startModules[i]     = 0;
      dstCfg.tofinoModules[i][0] = 0;
   }

   for (UInt_t i = 0; i < 4; i++)
   {
      dstCfg.mdcTestLevel[i] = 0;
   }

   dstCfg.dstId                   = "<unnamed>";
   dstCfg.expId                   = "";
   dstCfg.inputUrl                = "";
   dstCfg.outputDir               = "";
   dstCfg.qaOutputDir             = "";
   dstCfg.notPersistentCategories = "";
   dstCfg.firstParameterSource    = "none";
   dstCfg.secondParameterSource   = "none";
   dstCfg.parameterOutputFile     = "";
   dstCfg.referenceRunId          = 0;
   dstCfg.processNumEvents        = 0;
   dstCfg.startWithEvent          = 0;
   dstCfg.eventCounterInterval    = 100;
   dstCfg.qaEventInterval         = 50;

   masterTaskSet = NULL;

   // set up Root - inhibit graphical output in batch mode
   gROOT->SetBatch();

   // set up singleton analysis instance - gHades is set by the constructor of
   // class Hades
   gHades = NULL;
   new Hades;
}



HDstProduction::~HDstProduction()
{
   if (gHades)
   {
      delete gHades;
   }
}



void HDstProduction::configureDstProduction(int argc, char** argv)
{
   // First this function checks on the command line, weather a 
   // configuration file was specified. Then these information
   // are evaluated, before the command line is search again for
   // other option.
   //
   // This means, that the program parameter are evaluated in the
   // following order of precedence (low to high):
   //   1. Default parameter from constructor
   //   2. Parameter read from configuration file
   //   3. Parameter read from command line
   //
   // Since a lot of C library functions are called here, this
   // function is written in plain C (except the access to dstCfg
   // structure)

   int    steps_left   = 2;
   int    option       = 0;
   int    option_index = 0;
   int    current_argc = 0;
   char** current_argv = NULL;
   int    file_argc    = 0;
   char** file_argv    = NULL;

   // options allowed on command line and in the config file: their long
   // name and possibly the corresponding short (single character) form
   const struct option dst_options[] = {
      { "cfg-file",                  required_argument, NULL, 'f'   },
      { "trigger-modules",           required_argument, NULL, 32001 },
      { "start-detector-modules",    required_argument, NULL, 32002 },
      { "rich-modules",              required_argument, NULL, 32003 },
      { "mdc-modules",               required_argument, NULL, 32004 },
      { "shower-modules",            required_argument, NULL, 32005 },
      { "tof-modules",               required_argument, NULL, 32006 },
      { "tofino-modules",            required_argument, NULL, 32007 },
      { "dst-id",                    required_argument, NULL, 32008 },
      { "mdc-test-layer",            required_argument, NULL, 32009 },
      { "mdc-test-level",            required_argument, NULL, 32010 },
      { "exp-id",                    required_argument, NULL, 32011 },
      { "start-with-event",          required_argument, NULL, 'e'   },
      { "event-counter-interval",    required_argument, NULL, 'E'   },
      { "input-url",                 required_argument, NULL, 'i'   },
      { "output-dir",                required_argument, NULL, 'o'   },
      { "qa-output-dir",             required_argument, NULL, 'q'   },
      { "qa-event-interval",         required_argument, NULL, 'Q'   },
      { "not-persistent-categories", required_argument, NULL, 'c'   },
      { "first-parameter-source",    required_argument, NULL, '1'   },
      { "second-parameter-source",   required_argument, NULL, '2'   },
      { "parameter-output-file",     required_argument, NULL, 'p'   },
      { "reference-run-id",          required_argument, NULL, 'r'   },
      { "num-events",                required_argument, NULL, 'n'   },
      { 0, 0, 0, 0 }
   };

   // first, try to find and parse an optional configuration file
   for (current_argc = 0; current_argc < argc - 1; current_argc++)
   {
      if (strcmp( argv[current_argc], "-f"         ) == 0  ||
	  strcmp( argv[current_argc], "--cfg-file" ) == 0)
      {
	 if (!HDstUtilities::readCmdLineArgsFromFile(
		argv[current_argc + 1], &file_argc, &file_argv ))
	 {
	    terminateProduction( 1 );
	 }
	 break;
      }
   }

   // first, evaluate the parameter from file (from the beginning), then 
   // from command line (see code around "goto" statement below, too)
   current_argc = file_argc;
   current_argv = file_argv;

  EvalOptions:

   while (1)
   {
      option = getopt_long( current_argc, current_argv,
			    "1:2:c:f:i:n:o:p:q:r:s:",
			    dst_options, &option_index );
      if (option == -1)
      {
	 break;
      }

      switch (option)
      {
	 case 32001:
	    if (sscanf( optarg, "%d", &dstCfg.triggerModules[0] ) != 1)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal trigger modules definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32002:
	    if (sscanf( optarg, "%d %d %d %d %d %d",
			&dstCfg.startModules[0],
			&dstCfg.startModules[1],
			&dstCfg.startModules[2],
			&dstCfg.startModules[3],
			&dstCfg.startModules[4],
			&dstCfg.startModules[5] ) != 6)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal start detectors definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32003:
	    if (sscanf( optarg, "%d", &dstCfg.richModules[0] ) != 1)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal RICH modules definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32004:
	    if (sscanf( optarg,
			"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			&dstCfg.mdcModules[0][0],
			&dstCfg.mdcModules[1][0],
			&dstCfg.mdcModules[2][0],
			&dstCfg.mdcModules[3][0],
			&dstCfg.mdcModules[4][0],
			&dstCfg.mdcModules[5][0],
			&dstCfg.mdcModules[0][1],
			&dstCfg.mdcModules[1][1],
			&dstCfg.mdcModules[2][1],
			&dstCfg.mdcModules[3][1],
			&dstCfg.mdcModules[4][1],
			&dstCfg.mdcModules[5][1],
			&dstCfg.mdcModules[0][2],
			&dstCfg.mdcModules[1][2],
			&dstCfg.mdcModules[2][2],
			&dstCfg.mdcModules[3][2],
			&dstCfg.mdcModules[4][2],
			&dstCfg.mdcModules[5][2],
			&dstCfg.mdcModules[0][3],
			&dstCfg.mdcModules[1][3],
			&dstCfg.mdcModules[2][3],
			&dstCfg.mdcModules[3][3],
			&dstCfg.mdcModules[4][3],
			&dstCfg.mdcModules[5][3] ) != 24)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal MDC modules definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32005:
	    if (sscanf( optarg, "%d %d %d",
			&dstCfg.showerModules[0],
			&dstCfg.showerModules[1],
			&dstCfg.showerModules[2] ) != 3)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal shower modules definition!\n");
	       terminateProduction( 1 );
	    }
	    break;
	 case 32006:
	    if (sscanf( optarg,
			"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			&dstCfg.tofModules[0],
			&dstCfg.tofModules[1],
			&dstCfg.tofModules[2],
			&dstCfg.tofModules[3],
			&dstCfg.tofModules[4],
			&dstCfg.tofModules[5],
			&dstCfg.tofModules[6],
			&dstCfg.tofModules[7],
			&dstCfg.tofModules[8],
			&dstCfg.tofModules[9],
			&dstCfg.tofModules[10],
			&dstCfg.tofModules[11],
			&dstCfg.tofModules[12],
			&dstCfg.tofModules[13],
			&dstCfg.tofModules[14],
			&dstCfg.tofModules[15],
			&dstCfg.tofModules[16],
			&dstCfg.tofModules[17],
			&dstCfg.tofModules[18],
			&dstCfg.tofModules[19],
			&dstCfg.tofModules[20],
			&dstCfg.tofModules[21] ) != 22)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal TOF modules definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32007:
	    if (sscanf( optarg,
			"%d %d %d %d %d %d",
			&dstCfg.tofinoModules[0][0],
			&dstCfg.tofinoModules[1][0],
			&dstCfg.tofinoModules[2][0],
			&dstCfg.tofinoModules[3][0],
			&dstCfg.tofinoModules[4][0],
			&dstCfg.tofinoModules[5][0] ) != 6)
	    {
	       fprintf( stderr,
			"==>  ERROR: Illegal TOFino modules definition!\n");
	       terminateProduction( 1 );
	    }
	    break;
	 case 32008:
	    dstCfg.dstId = optarg;
	    break;
	 case 32009:
	    if (sscanf( optarg,
			"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
			&dstCfg.mdcTestLayer[0][0],
			&dstCfg.mdcTestLayer[1][0],
			&dstCfg.mdcTestLayer[2][0],
			&dstCfg.mdcTestLayer[3][0],
			&dstCfg.mdcTestLayer[4][0],
			&dstCfg.mdcTestLayer[5][0],
			&dstCfg.mdcTestLayer[0][1],
			&dstCfg.mdcTestLayer[1][1],
			&dstCfg.mdcTestLayer[2][1],
			&dstCfg.mdcTestLayer[3][1],
			&dstCfg.mdcTestLayer[4][1],
			&dstCfg.mdcTestLayer[5][1],
			&dstCfg.mdcTestLayer[0][2],
			&dstCfg.mdcTestLayer[1][2],
			&dstCfg.mdcTestLayer[2][2],
			&dstCfg.mdcTestLayer[3][2],
			&dstCfg.mdcTestLayer[4][2],
			&dstCfg.mdcTestLayer[5][2],
			&dstCfg.mdcTestLayer[0][3],
			&dstCfg.mdcTestLayer[1][3],
			&dstCfg.mdcTestLayer[2][3],
			&dstCfg.mdcTestLayer[3][3],
			&dstCfg.mdcTestLayer[4][3],
			&dstCfg.mdcTestLayer[5][3] ) != 24)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal MDC test leyer definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32010:
	    if (sscanf( optarg, "%d %d %d %d",
			&dstCfg.mdcTestLevel[0],
			&dstCfg.mdcTestLevel[1],
			&dstCfg.mdcTestLevel[2],
			&dstCfg.mdcTestLevel[3] ) != 4)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal MDC test level definition!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 32011:
	    dstCfg.expId = optarg;
	    dstCfg.expId.ToLower();
	    break;
	 case '1':
	    dstCfg.firstParameterSource = optarg;
	    break;
	 case '2':
	    dstCfg.secondParameterSource = optarg;
	    break;
	 case 'c':
	    dstCfg.notPersistentCategories = optarg;
	    break;
	 case 'f':
	    // configuration file is already evaluated
	    break;
	 case 'e':
	    if (sscanf( optarg, "%d", &dstCfg.startWithEvent ) != 1)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal start event defined!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 'E':
	    if (sscanf( optarg, "%d", &dstCfg.eventCounterInterval ) != 1
	       ||
	       dstCfg.eventCounterInterval < 1)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal counter interval defined!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 'i':
	    dstCfg.inputUrl = optarg;
	    break;
	 case 'n':
	    if (sscanf( optarg, "%d", &dstCfg.processNumEvents ) != 1
	       ||
	       dstCfg.processNumEvents < 0)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal number of events defined!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 'o':
	    dstCfg.outputDir = optarg;
	    break;
	 case 'p':
	    dstCfg.parameterOutputFile = optarg;
	    break;
	 case 'q':
	    dstCfg.qaOutputDir = optarg;
	    break;
	 case 'Q':
	    if (sscanf( optarg, "%d", &dstCfg.qaEventInterval ) != 1
	       ||
	       dstCfg.qaEventInterval < 1)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal QA event interval defined!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case 'r':
	    if (sscanf( optarg, "%d", &dstCfg.referenceRunId ) != 1)
	    {
	       fprintf( stderr,
			"==> ERROR: Illegal reference run ID defined!\n" );
	       terminateProduction( 1 );
	    }
	    break;
	 case '?':
	    fprintf( stderr, "==> ERROR: Found ambiguous options!\n" );
	    terminateProduction( 1 );
	    break;
	 default:
	    fprintf( stderr, "==> ERROR: Found illegal option!\n" );
	    terminateProduction( 1 );
      }
   }

   // first, evaluate the parameter from file, then from command line, while
   // skipping the program name (see code around "goto label" above, too)
   if (--steps_left)
   {
      optind       = 1;
      current_argc = argc;
      current_argv = argv;
      goto EvalOptions;
   }

   while (file_argc > 0)
   {
      free( file_argv[--file_argc] );
   }
   if (file_argv != NULL)
   {
      free( file_argv );
   }
}



void HDstProduction::setupSpectrometer()
{
   // Setup the spectormeter. This functions creates the setup of the
   // spectrometer using the related module parameters of the dstCfg
   // structure.

   HSpectrometer* spectrometer = NULL;

   // set up the spectrometer
   if (!(spectrometer = gHades->getSetup()))
   {
      cerr << "==> ERROR: Failed to initialize Spectrometer!" << endl;
      terminateProduction( 3 );
   }
   spectrometer->addDetector( new HRichDetector    );
   spectrometer->addDetector( new HMdcDetector     );
   spectrometer->addDetector( new HTofDetector     );
   spectrometer->addDetector( new HTofinoDetector  );
   spectrometer->addDetector( new HShowerDetector  );
   spectrometer->addDetector( new HStartDetector   );
   spectrometer->addDetector( new HTBoxDetector    );
   spectrometer->addDetector( new HTriggerDetector );

   spectrometer->getDetector( "Start" )->setModules( -1, dstCfg.startModules );
   spectrometer->getDetector( "TBox"  )->setModules(  0, dstCfg.triggerModules);

   // initialize all 6 sectors of the spectrometer
   for (Int_t i = 0; i < 6; i++)
   {
      spectrometer->getDetector( "Shower" )->
	 setModules( i, dstCfg.showerModules );
      spectrometer->getDetector( "Tof" )->
	 setModules( i, dstCfg.tofModules );
      spectrometer->getDetector( "Tofino" )->
	 setModules( i, dstCfg.tofinoModules[i] );
      spectrometer->getDetector( "Mdc" )->
	 setModules( i, dstCfg.mdcModules[i] );
      spectrometer->getDetector( "Rich" )->
	 setModules( i, dstCfg.richModules );
   }
}



void HDstProduction::setupParameterSources()
{
   // Setup and open parameter sources. The function uses the related variables
   // of the dstCfg structure to decide which source is taken.

   HRuntimeDb* runtime_db = gHades->getRuntimeDb();

   // assign first parameter source
   if (dstCfg.firstParameterSource.CompareTo( "oracle",
					      TString::kIgnoreCase ) == 0)
   {
      HParOraIo* input = new HParOraIo();
      if (!input->open())
      {
	 cerr << "==> ERROR: Could not open DB connection!" << endl;
	 terminateProduction( 3 );
      }
      input->setHistoryDate( "now" );
      runtime_db->setFirstInput( input );
   }
   else if (dstCfg.firstParameterSource.EndsWith( ".root",
						  TString::kIgnoreCase ))
   {
      HParRootFileIo* input = new HParRootFileIo();
      if (!input->open(const_cast<Text_t*>(dstCfg.firstParameterSource.Data()),
		       "READ" ))
      {
	 cerr << "==> ERROR: Could not open Root parameter input file!" <<endl;
	 terminateProduction( 3 );
      }
      runtime_db->setFirstInput( input );
   }
   else if (dstCfg.firstParameterSource.CompareTo( "none",
						   TString::kIgnoreCase ) == 0)
   {
      cerr << "==> ERROR: First parameter source not defined!" << endl;
      terminateProduction( 1 );
   }
   else
   {
      HParAsciiFileIo* input = new HParAsciiFileIo();
      if (!input->open(const_cast<Text_t*>(dstCfg.firstParameterSource.Data()),
		       "in" ))
      {
	 cerr << "==> ERROR: Could not open ASCII parameter input file!"<<endl;
	 terminateProduction( 3 );
      }
      runtime_db->setFirstInput( input );
   }

   // assign second parameter source
   if (dstCfg.secondParameterSource.CompareTo( "oracle",
					       TString::kIgnoreCase ) == 0)
   {
      HParOraIo* input = new HParOraIo();
      if (!input->open())
      {
	 cerr << "==> ERROR: Could not open DB connection!" << endl;
	 terminateProduction( 3 );
      }
      input->setHistoryDate( "now" );
      runtime_db->setSecondInput( input );
   }
   else if (dstCfg.secondParameterSource.EndsWith( ".root",
						   TString::kIgnoreCase ))
   {
      HParRootFileIo* input = new HParRootFileIo();
      if (!input->open(
	     const_cast<Text_t*>(dstCfg.secondParameterSource.Data()), "READ"))
      {
	 cerr << "==> ERROR: Could not open Root parameter input file!" <<endl;
	 terminateProduction( 3 );
      }
      runtime_db->setSecondInput( input );
   }
   else if (dstCfg.secondParameterSource.CompareTo( "none",
						    TString::kIgnoreCase ) == 0)
   {
      // in this case, do nothing
   }
   else
   {
      HParAsciiFileIo* input = new HParAsciiFileIo();
      if (!input->open(
	     const_cast<Text_t*>(dstCfg.secondParameterSource.Data()), "in" ))
      {
	 cerr << "==> ERROR: Could not open ASCII parameter input file!"<<endl;
	 terminateProduction( 3 );
      }
      runtime_db->setSecondInput( input );
   }
}



void HDstProduction::setupOutput()
{
   TObjString* category_name = NULL;
   TObjArray*  categories    = NULL;
   TIterator*  element       = NULL;
   HCategory*  category      = NULL;
   TString     name;
   TString     output_file;

   // check given output locations
   if (gSystem->AccessPathName( dstCfg.outputDir.Data(), kWritePermission ))
   {
      cerr << "==> ERROR: Ouput directory '" << dstCfg.outputDir
	   << "' not writable!" << endl;
      terminateProduction( 2 );
   }

   if (gSystem->AccessPathName( dstCfg.qaOutputDir.Data(), kWritePermission ))
   {
      cerr << "==> ERROR: QA output directory '" << dstCfg.qaOutputDir
	   << "' not writable!" << endl;
      terminateProduction( 2 );
   }

   // set non-persistent categories (from the first event) - which are not
   // written to the Root output file
   categories = dstCfg.notPersistentCategories.Tokenize( " \t" );
   element    = categories->MakeIterator();
   while ((category_name = dynamic_cast<TObjString*>(element->Next())))
   {
      name = category_name->GetString();
      if (name.CompareTo( "catGeantHeader", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catGeantHeader );
      }
      else if (name.CompareTo( "catGeantKine", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catGeantKine );
      }
      else if (name.CompareTo( "catMdcGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcGeantRaw );
      }
      else if (name.CompareTo( "catRichGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichGeantRaw );
      }
      else if (name.CompareTo( "catShowerGeantRaw", TString::kIgnoreCase ) ==0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerGeantRaw);
      }
      else if (name.CompareTo( "catTofGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofGeantRaw );
      }
      else if (name.CompareTo( "catTofGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofGeantRaw );
      }
      else if (name.CompareTo( "catStartGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catStartGeantRaw );
      }
      else if (name.CompareTo( "catWallGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catWallGeantRaw );
      }
      else if (name.CompareTo( "catRpcGeantRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRpcGeantRaw );
      }
      else if (name.CompareTo( "catMdcRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcRaw );
      }
      else if (name.CompareTo( "catMdcHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcHit );
      }
      else if (name.CompareTo( "catMdcCal1", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcCal1 );
      }
      else if (name.CompareTo( "catMdcCal2", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcCal2 );
      }
      else if (name.CompareTo( "catMdcGeantCell", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcGeantCell );
      }
      else if (name.CompareTo( "catMdcSeg", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcSeg );
      }
      else if (name.CompareTo( "catMdcTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcTrack );
      }
      else if (name.CompareTo( "catMdcRawEventHeader", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcRawEventHeader );
      }
      else if (name.CompareTo( "catMdcTrkCand", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcTrkCand );
      }
      else if (name.CompareTo( "catMdcDataWord", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcDataWord );
      }
      else if (name.CompareTo( "catMdcSlope", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcSlope );
      }
      else if (name.CompareTo( "catMdcRawCor", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcRawCor );
      }
      else if (name.CompareTo( "catMdcHitIdeal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcHitIdeal );
      }
      else if (name.CompareTo( "catMdcHitIdeal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcHitIdeal );
      }
      else if (name.CompareTo( "catMdcSegIdeal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcSegIdeal );
      }
      else if (name.CompareTo( "catMdcTrkCandIdeal", TString::kIgnoreCase )==0)
      {
	 category = gHades->getCurrentEvent()->getCategory(catMdcTrkCandIdeal);
      }
      else if (name.CompareTo( "catMdcClus", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcClus );
      }
      else if (name.CompareTo( "catMdcClusFit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcClusFit );
      }
      else if (name.CompareTo( "catMdcWireFit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcWireFit );
      }
      else if (name.CompareTo( "catMdcClusInf", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcClusInf );
      }
      else if (name.CompareTo( "catBaseTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catBaseTrack );
      }
      else if (name.CompareTo( "catSplineTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catSplineTrack );
      }
      else if (name.CompareTo( "catMetaMatch", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMetaMatch );
      }
      else if (name.CompareTo( "catKickTrack123B", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catKickTrack123B );
      }
      else if (name.CompareTo( "catRKTrackB", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRKTrackB );
      }
      else if (name.CompareTo( "catHodoRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHodoRaw );
      }
      else if (name.CompareTo( "catHodoCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHodoCal );
      }
      else if (name.CompareTo( "catHodoHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHodoHit );
      }
      else if (name.CompareTo( "catStartRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catStartRaw );
      }
      else if (name.CompareTo( "catStartCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catStartCal );
      }
      else if (name.CompareTo( "catStartHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catStartHit );
      }
      else if (name.CompareTo( "catTBox", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTBox );
      }
      else if (name.CompareTo( "catTBoxChan", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTBoxChan );
      }
      else if (name.CompareTo( "catTBoxChanSum", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTBoxChanSum );
      }
      else if (name.CompareTo( "catHypList", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHypList );
      }
      else if (name.CompareTo( "catHypComb", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHypComb );
      }
      else if (name.CompareTo( "catHypKine", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHypKine );
      }
      else if (name.CompareTo( "catKickTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catKickTrack );
      }
      else if (name.CompareTo( "catKickMdcTrk", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catKickMdcTrk );
      }
      else if (name.CompareTo( "catKickMetaSeg", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catKickMetaSeg );
      }
      else if (name.CompareTo( "catKickTrackB", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catKickTrackB );
      }
      else if (name.CompareTo( "catMdcCalHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcCalHit );
      }
      else if (name.CompareTo( "catMdcSegment", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcSegment );
      }
      else if (name.CompareTo( "catMdcSimSegment", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMdcSimSegment );
      }
      else if (name.CompareTo( "catPairsHeader", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPairsHeader );
      }
      else if (name.CompareTo( "catPairsHeader", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPairsHeader );
      }
      else if (name.CompareTo( "catParticle", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catParticle );
      }
      else if (name.CompareTo( "catParticleSrc", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catParticleSrc );
      }
      else if (name.CompareTo( "catDilepton", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catDilepton );
      }
      else if (name.CompareTo( "catPidTrackCand", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPidTrackCand );
      }
      else if (name.CompareTo( "catPidCandidate", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPidCandidate );
      }
      else if (name.CompareTo( "catPidPart", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPidPart );
      }
      else if (name.CompareTo( "catPidDilepton", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPidDilepton );
      }
      else if (name.CompareTo( "catPidDynamic", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPidDynamic );
      }
      else if (name.CompareTo( "catPidPoorCandidate", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catPidPoorCandidate );
      }
      else if (name.CompareTo( "catRichRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichRaw );
      }
      else if (name.CompareTo( "catRichHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichHit );
      }
      else if (name.CompareTo( "catRichCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichCal );
      }
      else if (name.CompareTo( "catRichTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichTrack );
      }
      else if (name.CompareTo( "catRichLocal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichLocal );
      }
      else if (name.CompareTo( "catRichCalTemp", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichCalTemp );
      }
      else if (name.CompareTo( "catRichPhotClus", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichPhotClus );
      }
      else if (name.CompareTo( "catRichHitHdr", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichHitHdr );
      }
      else if (name.CompareTo( "catRichPID", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichPID );
      }
      else if (name.CompareTo( "catRichPhotonHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichPhotonHit );
      }
      else if (name.CompareTo( "catRichPhotonHitHeader", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichPhotonHitHeader );
      }
      else if (name.CompareTo( "catRichHitFit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichHitFit );
      }
      else if (name.CompareTo( "catMatchHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchHit );
      }
      else if (name.CompareTo( "catMatchHitHeader", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchHitHeader );
      }
      else if (name.CompareTo( "catRichHitSim", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichHitSim );
      }
      else if (name.CompareTo( "catDiMatchHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catDiMatchHit );
      }
      else if (name.CompareTo( "catRichLocalMaxHeader", TString::kIgnoreCase ) ==0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichLocalMaxHeader);
      }
      else if (name.CompareTo( "catRichDirClus", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRichDirClus );
      }
      else if (name.CompareTo( "catRpcRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRpcRaw );
      }
      else if (name.CompareTo( "catRpcCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRpcCal );
      }
      else if (name.CompareTo( "catRpcHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catRpcHit );
      }
      else if (name.CompareTo( "catShowerRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerRaw );
      }
      else if (name.CompareTo( "catShowerHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerHit );
      }
      else if (name.CompareTo( "catShowerCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerCal );
      }
      else if (name.CompareTo( "catShowerGeantWire", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerGeantWire );
      }
      else if (name.CompareTo( "catShowerPID", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerPID );
      }
      else if (name.CompareTo( "catShowerTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerTrack );
      }
      else if (name.CompareTo( "catShowerRawMatr", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerRawMatr );
      }
      else if (name.CompareTo( "catShowerHitHdr", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerHitHdr );
      }
      else if (name.CompareTo( "catShowerHitTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerHitTrack );
      }
      else if (name.CompareTo( "catShowerPIDTrack", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerPIDTrack );
      }
      else if (name.CompareTo( "catShowerHitTof", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerHitTof );
      }
      else if (name.CompareTo( "catShowerHitTofTrack", TString::kIgnoreCase ) ==0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerHitTofTrack);
      }
      else if (name.CompareTo( "catShowerTofPID", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerTofPID );
      }
      else if (name.CompareTo( "catShowerTofPIDTrack", TString::kIgnoreCase ) ==0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerTofPIDTrack);
      }
      else if (name.CompareTo( "catTofRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofRaw );
      }
      else if (name.CompareTo( "catTofHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofHit );
      }
      else if (name.CompareTo( "catTofCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofCal );
      }
      else if (name.CompareTo( "catTofCluster", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofCluster );
      }
      else if (name.CompareTo( "catTofinoRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofinoRaw );
      }
      else if (name.CompareTo( "catTofinoCal", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catTofinoCal );
      }
      else if (name.CompareTo( "catMatchURich", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchURich );
      }
      else if (name.CompareTo( "catMatchUShower", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchUShower );
      }
      else if (name.CompareTo( "catMatchUTof", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchUTof );
      }
      else if (name.CompareTo( "catMatchULeptons", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchULeptons );
      }
      else if (name.CompareTo( "catMatchUScalers", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchUScalers );
      }
      else if (name.CompareTo( "catMatchUDiLeptons", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMatchUDiLeptons );
      }
      else if (name.CompareTo( "catMUEMULeptons", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMUEMULeptons );
      }
      else if (name.CompareTo( "catMUEMUDiLeptons", TString::kIgnoreCase ) ==0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catMUEMUDiLeptons);
      }
      else if (name.CompareTo( "catHardRichHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catHardRichHit );
      }
      else if (name.CompareTo( "catShowerCalIPU", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory(catShowerCalIPU  );
      }
      else if (name.CompareTo( "catShowerHitIPU", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catShowerHitIPU );
      }
      else if (name.CompareTo( "catWallRaw", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catWallRaw );
      }
      else if (name.CompareTo( "catWallHit", TString::kIgnoreCase ) == 0)
      {
	 category = gHades->getCurrentEvent()->getCategory( catWallHit );
      }
      else
      {
	 cerr << "==> ERROR: Unknown output category '" << name << "' defined!"
	      << endl;
	 terminateProduction( 2 );
      }

      if(category)
      {
	 category->setPersistency( kFALSE );
      }
      else
      {
	 cerr << "==> ERROR: Output category '" << name << "' does not exist!"
	      << endl;
	 terminateProduction( 3 );
      }
   }
   delete categories;

   // we use a large output tree buffer
   gHades->setTreeBufferSize( TREE_BUFFER_SIZE );

   // derive output file name from input file name
   output_file = gSystem->BaseName( dstCfg.inputUrl.Data() );
   output_file.ReplaceAll( ".root", 5, "_dst.root", 9 );
   output_file.ReplaceAll( ".hld",  4, "_dst.root", 9 );
   output_file.Prepend( "/" );
   output_file.Prepend( dstCfg.outputDir );

   // open and inititialize a compressed data output file
   gHades->setOutputFile( const_cast<Text_t*>(output_file.Data()),
			  "RECREATE",
			  const_cast<Text_t*>(dstCfg.dstId.Data()),
			  2 );

   // create output tree of categories
   gHades->makeTree();
}



void HDstProduction::setupAllParameterOutput()
{
   // Dump all parameter of an experiment to an .root output file.
   // This function expects a filename ending on '.root' in dstCfg parameter
   // structure, if the output should be written to a Root file.
   // Actually to dump the parameter to file one has to destroy the object
   // gHades after this function, gHades->init() and gHades->eventLoop(1) was
   // called.

   if (dstCfg.expId.IsNull())
   {
      cerr << "==> ERROR: Experiment ID not set!" << endl;
      terminateProduction( 2 );
   }
   
   if (dstCfg.parameterOutputFile.IsNull())
   {
      cerr << "==> ERROR: Parameter output file not defined!" << endl;
      terminateProduction( 2 );
   }

   if (!dstCfg.parameterOutputFile.EndsWith( ".root", TString::kIgnoreCase ))
   {
      cerr << "==> ERROR: "
	   << "Since the output is a Root file, it must end on .root!"
	   << endl;
      terminateProduction( 2 );
   }

   if (!gHades->getRuntimeDb()->makeParamFile(
	  const_cast<Text_t*>(dstCfg.parameterOutputFile.Data()), 
	  const_cast<Text_t*>(dstCfg.expId.Data()) ))
   {
      cerr << "==> ERROR: Could not open .root parameter output file!" << endl;
      terminateProduction( 3 );
   }
}



void HDstProduction::setupParameterOutput()
{
   // Dump the used parameter to an .root output file.
   // This function expects a filename ending on '.root' in dstCfg parameter
   // structure, if the output should be written to a Root file.
   // Actually to dump the parameter to file one has to destroy the object
   // gHades after this function, gHades->init() and gHades->eventLoop() was
   // called.

   if (dstCfg.parameterOutputFile.IsNull())
   {
      cerr << "==> ERROR: Parameter output file not defined!" << endl;
      terminateProduction( 2 );
   }

   if (!dstCfg.parameterOutputFile.EndsWith( ".root", TString::kIgnoreCase ))
   {
      cerr << "==> ERROR: Since the output is a Root file, it must end on .root!"
	   << endl;
      terminateProduction( 2 );
      
   }

   HParRootFileIo* output = new HParRootFileIo();
   if (!output->open(
	  const_cast<Text_t*>(dstCfg.parameterOutputFile.Data()),
	  "RECREATE" ))
   {
      cerr << "==> ERROR: Could not open .root parameter output file!"
	   << endl;
      terminateProduction( 3 );
   }
   gHades->getRuntimeDb()->setOutput( output );
}



void HDstProduction::printConfiguration()
{
   cout << endl;

   cout << "==> DST Production ID ...............: "
	<< dstCfg.dstId << endl;

   cout << "==> Reference Run ID ................: "
	<< dstCfg.referenceRunId << endl;

   cout << "==> First Parameter Source ..........: "
	<< dstCfg.firstParameterSource << endl;

   cout << "==> Second Parameter Source .........: "
	<< dstCfg.secondParameterSource << endl;

   cout << "==> Parameter Output File ...........: "
	<< dstCfg.parameterOutputFile << endl;

   cout << "==> Number of Events to be processed : "
	<< dstCfg.processNumEvents << endl;

   cout << "==> First Event to be processed .....: "
	<< dstCfg.startWithEvent << endl;

   cout << "==> Input URL .......................: "
	<< dstCfg.inputUrl << endl;

   cout << "==> Output Directory ................: "
	<< dstCfg.outputDir << endl;

   cout << "==> Output Directory of QA Reports ..: "
	<< dstCfg.qaOutputDir << endl;

   cout << "==> Trigger Modules .................: "
	<< dstCfg.triggerModules[0] << endl;

   cout << "==> Start Modules ...................: "
	<< dstCfg.startModules[0] << " "
	<< dstCfg.startModules[1] << " "
	<< dstCfg.startModules[2] << " "
	<< dstCfg.startModules[3] << " "
	<< dstCfg.startModules[4] << " "
	<< dstCfg.startModules[5] << " "
	<< endl;

   cout << "==> RICH Modules ....................: "
	<< dstCfg.richModules[0] << endl;

   cout << "==> MDC Modules .....................: ";
   for (UInt_t i = 0; i < 4; i++)
   {
      for (UInt_t j = 0;  j < 6; j++)
      {
	 cout << dstCfg.mdcModules[j][i] << " ";
      }
      cout << " ";
   }
   cout << endl;

   cout << "==> Shower Modules ..................: "
	<< dstCfg.showerModules[0] << " "
	<< dstCfg.showerModules[1] << " "
	<< dstCfg.showerModules[2] << " "
	<< endl;

   cout << "==> TOF Modules .....................: ";
   for (UInt_t i = 0; i < sizeof(dstCfg.tofModules) / sizeof(Int_t); i++)
   {
      cout << dstCfg.tofModules[i] << " ";
   }
   cout << endl;

   cout << "==> TOFino Modules ..................: ";
   for (UInt_t i = 0; i < 6; i++)
   {
      cout << dstCfg.tofinoModules[i][0] << " ";
   }
   cout << endl;

   cout << "==> RPC Modules .....................: "
	<< dstCfg.rpcModules[0] << endl;

   cout << "==> Wall Modules ....................: "
	<< dstCfg.wallModules[0] << endl;

   cout << "==> MDC Test Layers .................: ";
   for (UInt_t i = 0; i < 4; i++)
   {
      for (UInt_t j = 0;  j < 6; j++)
      {
	 cout << dstCfg.mdcTestLayer[j][i] << " ";
      }
      cout << " ";
   }
   cout << endl;

   cout << "==> MDC Test Level ..................: ";
   for (UInt_t i = 0; i < 4; i++)
   {
      cout << dstCfg.mdcTestLevel[i] << " ";
   }
   cout << endl;

   cout << "==> Not persistent Categories .......: "
	<< dstCfg.notPersistentCategories << endl;
   cout << endl;
}



void HDstProduction::initialize()
{
   // initialize the anlysis run
   if (!gHades->init())
   {
      cerr << "==> ERROR: Initialization failed!" << endl;
      terminateProduction( 3 );
   }
}



void HDstProduction::produceDst()
{
   Int_t      num_events = 0;
   TStopwatch timer;

   // produce DST by looping over input events
   timer.Reset();
   timer.Start();
   gHades->makeCounter( dstCfg.eventCounterInterval );
   if (dstCfg.processNumEvents < 1)
   {
      num_events = gHades->eventLoop();
   }
   else
   {
      num_events = gHades->eventLoop( dstCfg.processNumEvents,
				      dstCfg.startWithEvent );
   }
   timer.Stop();

   // print some statistics on what was done
   masterTaskSet->printTimer();
   cout << "\n"
	<< "==> Events processed : " << num_events       << "\n"
	<< "==> Needed Time .....: " << timer.RealTime() << " seconds\n"
	<< "==> Needed CPU Time .: " << timer.CpuTime()  << " seconds\n";
   if (num_events)
   {
      cout << "==> Performance .....: "
	   << num_events / timer.RealTime() << " events per second\n"
	   << "==> CPU Performance .: " << timer.CpuTime() / num_events
	   << " CPU seconds per event\n";
   }
   cout << endl;
}



void HDstProduction::finalizeProduction()
{
   terminateProduction( 0 );
}



void HDstProduction::terminateProduction(Int_t exit_code)
{
   if (gHades)
   {
      delete gHades;
   }

   // this quits Root, too
   exit( exit_code );
}
