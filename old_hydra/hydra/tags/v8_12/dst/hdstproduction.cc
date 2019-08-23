/**
 * @file   hdstproduction.cc
 * @author Simon Lang
 * @date   Fri May  5 16:16:39 2006
 * 
 * @brief  Base class of DST Production Classes
 * 
 * $Id: hdstproduction.cc,v 1.20 2007-02-28 15:01:11 slang Exp $
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
#include "TUrl.h"

// Hydra header files
#include "hdstproduction.h"
#include "hdstutilities.h"

#include "hades.h"
#include "hevent.h"
#include "hcategory.h"
#include "htaskset.h"
#include "hrootsource.h"
#include "htool.h"

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

// we use a large buffer for Root type output trees
const Int_t HDstProduction::TREE_BUFFER_SIZE = 8000;



//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
//  Base class of DST Production Classes.
//
//  This is not an abstract base class, however it is not intended to create
//  instances of this class. Instead, create an object of one of the classes
//  derived from here. Therefore, the constructor of this class is protected.
//
////////////////////////////////////////////////////////////////////////////////
ClassImp(HDstProduction)



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
      }
      dstCfg.startModules[i]     = 0;
      dstCfg.tofinoModules[i][0] = 0;
   }

   // these flags are altered by using modules of the related detectors
   dstCfg.triggerIsUsed = kFALSE;
   dstCfg.startIsUsed   = kFALSE;
   dstCfg.richIsUsed    = kFALSE;
   dstCfg.mdcIsUsed     = kFALSE;
   dstCfg.showerIsUsed  = kFALSE;
   dstCfg.tofIsUsed     = kFALSE;
   dstCfg.tofinoIsUsed  = kFALSE;
   dstCfg.rpcIsUsed     = kFALSE;
   dstCfg.wallIsUsed    = kFALSE;
    
   dstCfg.dstId                   = "";
   dstCfg.expId                   = "";
   dstCfg.inputUrl                = "";
   dstCfg.embeddingInput          = "";
   dstCfg.outputDir               = "";
   dstCfg.outputFile              = "";
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
   dstCfg.createControlNTuple     = kFALSE;

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



void HDstProduction::configure(int argc, char** argv)
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

   int    steps_left   = 2;
   int    option       = 0;
   int    option_index = 0;
   int    current_argc = 0;
   char** current_argv = NULL;
   int    file_argc    = 0;
   char** file_argv    = NULL;
   int    print_help   = 0;

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
      { "exp-id",                    required_argument, NULL, 32009 },
      { "start-with-event",          required_argument, NULL, 'e'   },
      { "event-counter-interval",    required_argument, NULL, 'E'   },
      { "input-url",                 required_argument, NULL, 'i'   },
      { "embedding-input",           required_argument, NULL, 'I'   },
      { "output-dir",                required_argument, NULL, 'o'   },
      { "output-file",               required_argument, NULL, 'O'   },
      { "qa-output-dir",             required_argument, NULL, 'q'   },
      { "qa-event-interval",         required_argument, NULL, 'Q'   },
      { "not-persistent-categories", required_argument, NULL, 'c'   },
      { "first-parameter-source",    required_argument, NULL, '1'   },
      { "second-parameter-source",   required_argument, NULL, '2'   },
      { "parameter-output-file",     required_argument, NULL, 'p'   },
      { "reference-run-id",          required_argument, NULL, 'r'   },
      { "num-events",                required_argument, NULL, 'n'   },
      { "create-control-ntuple",     optional_argument, NULL, 't'   },
      { "help",                      no_argument,       NULL, 'h'   },
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
	    terminate( 1 );
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
			    "1:2:c:e:f:hi:I:n:o:O:p:q:r:s:t:",
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
	       Error( "configure", "Illegal trigger modules definition!" );
	       terminate( 1 );
	    }
	    dstCfg.triggerIsUsed = kTRUE;
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
	       Error( "configure", "Illegal start detectors definition!" );
	       terminate( 1 );
	    }
	    dstCfg.startIsUsed = kTRUE;
	    break;
	 case 32003:
	    if (sscanf( optarg, "%d", &dstCfg.richModules[0] ) != 1)
	    {
	       Error( "configure", "Illegal RICH modules definition!" );
	       terminate( 1 );
	    }
	    dstCfg.richIsUsed = kTRUE;
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
	       Error( "configure", "Illegal MDC modules definition!" );
	       terminate( 1 );
	    }
	    dstCfg.mdcIsUsed = kTRUE;
	    break;
	 case 32005:
	    if (sscanf( optarg, "%d %d %d",
			&dstCfg.showerModules[0],
			&dstCfg.showerModules[1],
			&dstCfg.showerModules[2] ) != 3)
	    {
	       Error( "configure", "Illegal shower modules definition!");
	       terminate( 1 );
	    }
	    dstCfg.showerIsUsed = kTRUE;
	    break;
	 case 32006:
	    if (sscanf( optarg,
			"%d %d %d %d %d %d %d %d %d %d %d "
			"%d %d %d %d %d %d %d %d %d %d %d",
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
	       Error( "configure", "Illegal TOF modules definition!" );
	       terminate( 1 );
	    }
	    dstCfg.tofIsUsed = kTRUE;
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
	       Error( "configure", "Illegal TOFino modules definition!" );
	       terminate( 1 );
	    }
	    dstCfg.tofinoIsUsed = kTRUE;
	    break;
	 case 32008:
	    dstCfg.dstId = optarg;
	    break;
	 case 32009:
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
	       Error( "configure", "Illegal start event defined!" );
	       terminate( 1 );
	    }
	    break;
	 case 'E':
	    if (sscanf( optarg, "%d", &dstCfg.eventCounterInterval ) != 1
	       ||
	       dstCfg.eventCounterInterval < 1)
	    {
	       Error( "configure", "Illegal counter interval defined!" );
	       terminate( 1 );
	    }
	    break;
	 case 'h':
	    print_help = 1;
	    break;
	 case 'i':
	    dstCfg.inputUrl += " ";
	    dstCfg.inputUrl += optarg;
	    break;
	 case 'I':
	    dstCfg.embeddingInput = optarg;
	    break;
	 case 'n':
	    if (sscanf( optarg, "%d", &dstCfg.processNumEvents ) != 1
	       ||
	       dstCfg.processNumEvents < 0)
	    {
	       Error( "configure", "Illegal number of events defined!" );
	       terminate( 1 );
	    }
	    break;
	 case 'o':
	    dstCfg.outputDir = optarg;
	    dstCfg.outputDir.Remove( TString::kTrailing, '/' );
	    break;
	 case 'O':
	    dstCfg.outputFile = optarg;
	    break;
	 case 'p':
	    dstCfg.parameterOutputFile = optarg;
	    break;
	 case 'q':
	    dstCfg.qaOutputDir = optarg;
	    dstCfg.outputDir.Remove( TString::kTrailing, '/' );
	    break;
	 case 'Q':
	    if (sscanf( optarg, "%d", &dstCfg.qaEventInterval ) != 1
	       ||
	       dstCfg.qaEventInterval < 1)
	    {
	       Error( "configure", "Illegal QA event interval defined!" );
	       terminate( 1 );
	    }
	    break;
	 case 'r':
	    if (sscanf( optarg, "%d", &dstCfg.referenceRunId ) != 1)
	    {
	       Error( "configure", "Illegal reference run ID defined!" );
	       terminate( 1 );
	    }
	    break;
	 case 't':
	    dstCfg.createControlNTuple = kTRUE;
	    break;
	 case '?':
	    Error( "configure", "Found unkown or ambiguous option!" );
	    terminate( 1 );
	    break;
	 default:
	    Error( "configure", "Found illegal option!" );
	    terminate( 1 );
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

   if (print_help)
   {
      printConfiguration();
      printHelp();
      terminate( 0 );
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
      Error( "setupSpectrometer", "Failed to initialize Spectrometer!" );
      terminate( 3 );
   }

   if (dstCfg.richIsUsed)
   {
      spectrometer->addDetector( new HRichDetector );
      for (Int_t i = 0; i < 6; i++)
      {
	 spectrometer->getDetector( "Rich" )->
	    setModules( i, dstCfg.richModules );
      }
   }
   if (dstCfg.mdcIsUsed)
   {
      spectrometer->addDetector( new HMdcDetector );
      for (Int_t i = 0; i < 6; i++)
      {
	 spectrometer->getDetector( "Mdc" )->
	    setModules( i, dstCfg.mdcModules[i] );
      }
   }
   if (dstCfg.tofIsUsed)
   {
      spectrometer->addDetector( new HTofDetector );
      for (Int_t i = 0; i < 6; i++)
      {
	 spectrometer->getDetector( "Tof" )->
	    setModules( i, dstCfg.tofModules );
      }
   }
   if (dstCfg.tofinoIsUsed)
   {
      spectrometer->addDetector( new HTofinoDetector );
      for (Int_t i = 0; i < 6; i++)
      {
	 spectrometer->getDetector( "Tofino" )->
	    setModules( i, dstCfg.tofinoModules[i] );
      }
   }
   if (dstCfg.showerIsUsed)
   {
      spectrometer->addDetector( new HShowerDetector );
      for (Int_t i = 0; i < 6; i++)
      {
	 spectrometer->getDetector( "Shower" )->
	    setModules( i, dstCfg.showerModules );
      }
   }
   if (dstCfg.startIsUsed)
   {
      spectrometer->addDetector( new HStartDetector );
      spectrometer->getDetector( "Start" )->setModules(-1, dstCfg.startModules);
   }
   if (dstCfg.triggerIsUsed)
   {
      spectrometer->addDetector( new HTBoxDetector    );
      spectrometer->addDetector( new HTriggerDetector );
      spectrometer->getDetector( "TBox" )->setModules(-1, dstCfg.triggerModules);
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
	 Error( "setupParameterSources", "Could not open DB connection!" );
	 terminate( 3 );
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
	 Error( "setupParameterSources",
		"Could not open Root parameter input file!" );
	 terminate( 3 );
      }
      runtime_db->setFirstInput( input );
   }
   else if (dstCfg.firstParameterSource.CompareTo( "none",
						   TString::kIgnoreCase ) == 0)
   {
      Error( "setupParameterSources", "First parameter source not defined!" );
      terminate( 1 );
   }
   else
   {
      HParAsciiFileIo* input = new HParAsciiFileIo();
      if (!input->open(const_cast<Text_t*>(dstCfg.firstParameterSource.Data()),
		       "in" ))
      {
	 Error( "setupParameterSources",
		"Could not open ASCII parameter input file!" );
	 terminate( 3 );
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
	 Error( "setupParameterSources", "Could not open DB connection!" );
	 terminate( 3 );
      }
      input->setHistoryDate( "now" );
      runtime_db->setSecondInput( input );
   }
   else if (dstCfg.secondParameterSource.EndsWith( ".root",
						   TString::kIgnoreCase ))
   {
      HParRootFileIo* input = new HParRootFileIo();
      if (!input->open(
	     const_cast<Text_t*>(dstCfg.secondParameterSource.Data()),"READ"))
      {
	 Error( "setupParameterSources",
		"Could not open Root parameter input file!" );
	 terminate( 3 );
      }
      runtime_db->setSecondInput( input );
   }
   else if (dstCfg.secondParameterSource.CompareTo(
	       "none", TString::kIgnoreCase ) == 0)
   {
      // in this case, do nothing
   }
   else
   {
      HParAsciiFileIo* input = new HParAsciiFileIo();
      if (!input->open(
	     const_cast<Text_t*>(dstCfg.secondParameterSource.Data()), "in" ))
      {
	 Error( "setupParameterSources",
		"Could not open ASCII parameter input file!" );;
	 terminate( 3 );
      }
      runtime_db->setSecondInput( input );
   }
}



void HDstProduction::setupRootInput()
{
   // Setup data input file: This function determines and opens
   // one or more .root input files of analysed data. All necessary
   // information are provided by dstCfg structure.

   HRootSource* source      = NULL;
   TObjArray*   files       = NULL;
   TIterator*   file        = NULL;
   TObjString*  filename    = NULL;
   TObjArray*   local_files = NULL; 
   TIterator*   local_file  = NULL;
   TFile*       root_file   = NULL;
   
   TString      directory;
   
   files = dstCfg.inputUrl.Tokenize( " \t" );
   file  = files->MakeIterator();
   file->Reset();
   while ((filename = (TObjString*)file->Next()))
   {
      TUrl url( filename->GetString().Data(), kTRUE );

      if (!source)
      {
	 source = new HRootSource;
	 source->setDirectory(
	    const_cast<Text_t*>(gSystem->DirName( url.GetFile() )) );
	 directory = gSystem->DirName( url.GetFile() );
      }
      if (strcmp( url.GetProtocol(), "file" ) == 0)
      {
	 local_files = HTool::glob( url.GetFile() );
	 if (!local_files)
	 {
	    Error( "setupRootInput", "Input file(s) do not exist!" );
	    terminate( 2 );
	 }
	 local_file = local_files->MakeIterator();
	 while ((filename = (TObjString*)local_file->Next()))
	 {
	    if (directory.CompareTo(
		   gSystem->DirName( filename->GetString().Data() ) ) != 0)
	    {
	       Error( "setupRootInput", "Only one input directory allowed!" );
	       terminate( 2 );
	    }

	    root_file = TFile::Open( filename->GetString().Data(), "READ" );
	    if (!root_file->IsOpen()  ||  root_file->IsZombie()  ||
		root_file->GetSize() < 8000)
	    {
	       Error( "setupRootInput", Form(
			 "Broken/invalid .root input file '%s'!",
			 filename->GetString().Data() ) );
	       terminate( 2 );
	    }
	    delete root_file;

	    if (!source->addFile(
		   const_cast<Text_t*>(gSystem->BaseName(
					  filename->GetString().Data() )) ))
	    {
	       Error( "setupRootInput", Form(
			 "Adding input file '%s' failed!",
			 filename->GetString().Data() ) );
	       terminate( 2 );
	    }
	 }
	 delete local_file;
	 delete local_files;
      }
      else
      {
	 Error( "setupRootInput", "Input URL must refer to a local file!" );
	 terminate( 2 );
      }
   }
   delete file;

   gHades->setDataSource( source );
}



void HDstProduction::setupOutput()
{
   // Setup data output: This function opens a .root output file, those
   // name is derived from the input filename and initializes the output
   // tree 'T' in this file. 
   // CAUTION: This function must be called AFTER initialize(), otherwise
   // the output is corrupted or the program might crash.
   
   TObjString* category_name = NULL;
   TObjArray*  categories    = NULL;
   TIterator*  element       = NULL;
   HCategory*  category      = NULL;
   TString     name;

   // check given output locations
   if (gSystem->AccessPathName( dstCfg.outputDir.Data(), kWritePermission ))
   {
      Error( "setupOutput", "Ouput directory '%s' not writable!",
	     dstCfg.outputDir.Data() );
      terminate( 2 );
   }
   if (dstCfg.qaOutputDir.IsNull())
   {
      dstCfg.qaOutputDir = dstCfg.outputDir;
   }
   else if (gSystem->AccessPathName( dstCfg.qaOutputDir.Data(),
				     kWritePermission ))
   {
      Error( "setupOutput", "QA output directory '%s' not writable!",
	     dstCfg.qaOutputDir.Data() );
      terminate( 2 );
   }

   categories = dstCfg.notPersistentCategories.Tokenize( " \t" );
   element    = categories->MakeIterator();
   element->Reset();
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
	 Error( "setupOutput", "Unknown output category '%s' defined!",
		name.Data() );
	 terminate( 2 );
      }

      if(category)
      {
	 category->setPersistency( kFALSE );
      }
      else
      {
	 Warning( "setupOutput", "Output category '%s' does not exist!",
		  name.Data() );
      }
   }
   delete categories;
   delete element;

   // we use a large output tree buffer
   gHades->setTreeBufferSize( TREE_BUFFER_SIZE );

   // derive output file name from input file name (which might be a list,
   // separated by blanks), if it was not set e.g. by the task list before
   if (dstCfg.outputFile.IsNull())
   {
      dstCfg.outputFile = dstCfg.inputUrl.Data();
      Ssiz_t filename = dstCfg.outputFile.Index( ' ', 1, 1, TString::kExact );
      if (filename != kNPOS)
      {
	 dstCfg.outputFile.Resize( filename );
      }
      if (dstCfg.outputFile.EndsWith( ".root" ))
      {
	 dstCfg.outputFile.ReplaceAll( ".root", 5, "_dst.root", 9 );
      }
      else if (dstCfg.outputFile.EndsWith( ".hld" ))
      {
         dstCfg.outputFile.ReplaceAll( ".hld",  4, "_dst.root", 9 );
      }
      else
      {
	 dstCfg.outputFile += "_dst.root";
      }
   }
   dstCfg.outputFile = gSystem->BaseName( dstCfg.outputFile.Data() );
   dstCfg.outputFile.Prepend( "/" );
   dstCfg.outputFile.Prepend( dstCfg.outputDir );

   // open and inititialize a compressed data output file (level 2)
   gHades->setOutputFile( const_cast<Text_t*>(dstCfg.outputFile.Data()),
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
   // gHades after the event loop was executed, thus do:
   //    setupAllParameterOutput();
   //    initialize();
   //    runEventLoop();
   //    finalize();

   if (dstCfg.expId.IsNull())
   {
      Error( "setupAllParameterOutput", "Experiment ID not set!" );
      terminate( 2 );
   }
   
   if (dstCfg.parameterOutputFile.IsNull())
   {
      Error( "setupAllParameterOutput",
	     "Parameter output file not defined!" );
      terminate( 2 );
   }

   if (!dstCfg.parameterOutputFile.EndsWith( ".root", TString::kIgnoreCase ))
   {
      Error( "setupAllParameterOutput",
	     "Since the output is a Root file, it must end on .root!" );
      terminate( 2 );
   }

   if (!gHades->getRuntimeDb()->makeParamFile(
	  const_cast<Text_t*>(dstCfg.parameterOutputFile.Data()), 
	  const_cast<Text_t*>(dstCfg.expId.Data()) ))
   {
      Error( "setupAllParameterOutput",
	     "Could not open .root parameter output file!" );
      terminate( 3 );
   }
}



void HDstProduction::setupParameterOutput()
{
   // Dump the used parameter to an .root output file.
   // This function expects a filename ending on '.root' in dstCfg parameter
   // structure, if the output should be written to a Root file.
   // Actually to dump the parameter to file one has to destroy the object
   // gHades after the event loop was executed, thus do:
   //    setupAllParameterOutput();
   //    initialize();
   //    runEventLoop();
   //    finalize();

   if (dstCfg.parameterOutputFile.IsNull())
   {
      Error( "setupParameterOutput", "Parameter output file not defined!" );
      terminate( 2 );
   }

   if (!dstCfg.parameterOutputFile.EndsWith( ".root", TString::kIgnoreCase ))
   {
      Error( "setupParameterOutput",
	     "Since the output is a Root file, it must end on .root!" );
      terminate( 2 );
      
   }

   HParRootFileIo* output = new HParRootFileIo();
   if (!output->open(
	  const_cast<Text_t*>(dstCfg.parameterOutputFile.Data()),
	  "RECREATE" ))
   {
      Error( "setupParameterOutput",
	     "Could not open .root parameter output file!" );
      terminate( 3 );
   }
   gHades->getRuntimeDb()->setOutput( output );
}



void HDstProduction::printConfiguration()
{
   // Just print the content of the dstCfg structure.
   
   TObjArray*  inputs    = NULL;
   TIterator*  input     = NULL;
   TObjString* inputname = NULL;
   TObjArray*  files     = NULL;
   TIterator*  file      = NULL;
   TObjString* filename  = NULL;

   cout << endl;

   cout << "==> DST Production ID ...............: "
	<< (dstCfg.dstId.IsNull()  ?  "-unset-"  :  dstCfg.dstId) << endl;

   cout << "==> Experiment ID ...................: "
	<< (dstCfg.expId.IsNull()  ?  "-unset-"  :  dstCfg.expId) << endl;

   cout << "==> Reference Run ID ................: ";
   if (dstCfg.referenceRunId)
   {
      cout << dstCfg.referenceRunId << endl;
   }
   else
   {
      cout << "-unset-" << endl;
   }

   cout << "==> First Parameter Source ..........: "
	<< dstCfg.firstParameterSource << endl;

   cout << "==> Second Parameter Source .........: "
	<< dstCfg.secondParameterSource << endl;

   cout << "==> Parameter Output File ...........: "
	<< (dstCfg.parameterOutputFile.IsNull()  ?  "-unset-"  :
	    dstCfg.parameterOutputFile) << endl;

   cout << "==> Write file with control NTuple ..: "
	<< (dstCfg.createControlNTuple  ?  "yes"  :  "no") << endl;

   cout << "==> Number of Events to be processed : ";
   if (dstCfg.processNumEvents)
   {
      cout << dstCfg.processNumEvents << endl;
   }
   else
   {
	cout << "all" << endl;
   }

   cout << "==> First Event to be processed .....: "
	<< dstCfg.startWithEvent << endl;

   if (dstCfg.inputUrl.IsNull())
   {
      cout << "==> Input URL .......................: -unset-" << endl;
   }
   else
   {
      inputs = dstCfg.inputUrl.Tokenize( " \t" );
      input  = inputs->MakeIterator();
      while ((inputname = (TObjString*)input->Next()))
      {
	 files = HTool::glob( inputname->GetString() );
	 if (files)
	 {
	    file  = files->MakeIterator();
	    while ((filename = (TObjString*)file->Next()))
	    {
	       cout << "==> Input URL .......................: "
		    << filename->GetString() << endl;
	    }
	    delete files;
	    delete file;
	 }
      }
      delete inputs;
      delete input;
   }

   cout << "==> Output Directory ................: "
	<< (dstCfg.outputDir.IsNull()  ?  "-unset-"  :  dstCfg.outputDir)
	<< endl;

   cout << "==> Output Filename .................: "
	<< (dstCfg.outputFile.IsNull()  ?
	    "-unset: derived from input filename-"  :
	    dstCfg.outputFile)
	<< endl;

   cout << "==> Output Directory of QA Reports ..: "
	<< (dstCfg.qaOutputDir.IsNull()  ?
	    "-unset: using data output directory-"  :  dstCfg.qaOutputDir)
	<< endl;

   cout << "==> Trigger Modules .................: ";
   if (dstCfg.triggerIsUsed)
   {
      cout << dstCfg.triggerModules[0] << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> Start Modules ...................: ";
   if (dstCfg.startIsUsed)
   {
      cout << dstCfg.startModules[0] << " "
	   << dstCfg.startModules[1] << " "
	   << dstCfg.startModules[2] << " "
	   << dstCfg.startModules[3] << " "
	   << dstCfg.startModules[4] << " "
	   << dstCfg.startModules[5] << " "
	   << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> RICH Modules ....................: ";
   if (dstCfg.richIsUsed)
   {
      cout << dstCfg.richModules[0] << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> MDC Modules .....................: ";
   if (dstCfg.mdcIsUsed)
   {
      for (UInt_t i = 0; i < 4; i++)
      {
	 for (UInt_t j = 0;  j < 6; j++)
	 {
	    cout << dstCfg.mdcModules[j][i] << " ";
	 }
	 cout << " ";
      }
      cout << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> Shower Modules ..................: ";
   if (dstCfg.showerIsUsed)
   {
      cout << dstCfg.showerModules[0] << " "
	   << dstCfg.showerModules[1] << " "
	   << dstCfg.showerModules[2] << " "
	   << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> TOF Modules .....................: ";
   if (dstCfg.tofIsUsed)
   {
      for (UInt_t i = 0; i < sizeof(dstCfg.tofModules) / sizeof(Int_t); i++)
      {
	 cout << dstCfg.tofModules[i] << " ";
      }
      cout << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> TOFino Modules ..................: ";
   if (dstCfg.tofinoIsUsed)
   {
      for (UInt_t i = 0; i < 6; i++)
      {
	 cout << dstCfg.tofinoModules[i][0] << " ";
      }
      cout << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> RPC Modules .....................: ";
   if (dstCfg.rpcIsUsed)
   {
      cout << dstCfg.rpcModules[0] << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> Wall Modules ....................: ";
   if (dstCfg.wallIsUsed)
   {
      cout << dstCfg.wallModules[0] << endl;
   }
   else
   {
      cout << "-unused-" << endl;
   }

   cout << "==> Not persistent Categories .......: "
	<< (dstCfg.notPersistentCategories.IsNull()  ?  "none"  :
	    dstCfg.notPersistentCategories) << endl;
   cout << endl;
}



void HDstProduction::initialize()
{
   // Just initialize the analysis run.

   // initialize the anlysis run
   if (!gHades->init())
   {
      Error( "initialize", "Initialization failed!" );
      terminate( 3 );
   }
}



void HDstProduction::runEventLoop()
{
   // Run the event loop an print the timing information of the 
   // executed tasks, if available.
   // The number of events to be processed and other parameter
   // are provided by the dstCfg structure.
   
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
   cout << endl;
   gHades->getTaskSet( "real" )->printTimer();
   cout << endl;
   gHades->getTaskSet( "simulation" )->printTimer();
   cout << endl;
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



void HDstProduction::finalize()
{
   // Close all IO sources and exit the program successfully.
   
   terminate( 0 );
}



void HDstProduction::terminate(Int_t exit_code)
{
   // Close all IO sources and exit the program with an error code.
   
   if (gHades)
   {
      delete gHades;
   }

   // this quits Root, too
   exit( exit_code );
}



void HDstProduction::printHelp()
{
   // Just print the help on the command line options.

   cout << "\n"
	<< " Use the following long option flags on cammand line, in your configuration\n"
	<< " file or as aguments while calling the macro within CINT. The short options\n"
	<< " might be used on cmmand line, only!\n"
	<< "\n"
	<< " Only long options:\n"
	<< "   --trigger-modules        Array of used trigger modules\n"
	<< "   --start-detector-modules Array of used start modules\n"
	<< "   --rich-modules           Array of used RICH modules\n"
	<< "   --mdc-modules            Array of used MDC modules\n"
	<< "   --shower-modules         Array of used shower modules\n"
	<< "   --tof-modules            Array of used TOF modules\n"
	<< "   --tofino-modules         Array of used TOFino modules\n"
	<< "   --dst-id                 DST type description\n"
	<< "   --exp-id                 Experiment ID (like used in Oracle)\n"
	<< "\n"
	<< " Short and long options:\n"
	<< "   -1, --first-parameter-source    First parameter source\n"
	<< "   -2, --second-parameter-source   Second parameter source\n"
	<< "   -c, --not-persistent-categories List of not persistent categories (will\n"
	<< "                                   not be written to the output ROOT file)\n"
	<< "   -e, --start-with-event          Event to start the analysis from\n"
	<< "   -E, --event-counter-interval    Event counter interval\n"
	<< "   -f, --cfg-file                  Configuration file (All the configuration\n"
	<< "                                   can be stored in this file, command line\n"
	<< "                                   options overwrite the options in this file)\n"
	<< "   -i, --input-url                 Input for the analysis (file on the disk or\n"
	<< "                                   robot, access via NFS or rootd = future GSI\n"
	<< "                                   access scheme). One can define several input\n"
	<< "                                   files. In this case use the option for each\n"
	<< "                                   file in the order they should be used.\n"
	<< "   -I, --embedded-input            Second input source for \"embedded\" mode.\n"
	<< "                                   Here, the same rules are valid as for\n"
	<< "                                   normal input files.\n"
	<< "   -n, --num-events                Number of events to analyse.\n"
	<< "   -o, --output-dir                Directory, where the output is stored.\n"
	<< "   -O, --output-file               Name of the output file.\n"
	<< "   -p, --parameter-output-file     Name of the parameter output root file for\n"
	<< "                                   the case if some/all parameters will be\n"
	<< "                                   written from parameter sources to a single\n"
	<< "                                   root file\n"
	<< "   -q, --qa-output-dir             Directory, where the output of QA task is\n"
	<< "                                   stored\n"
	<< "   -Q, --qa-event-interval         QA event interval\n"
	<< "   -r, --reference-run-id          Reference runID (needed mainly for\n"
	<< "                                   simulation)\n"
	<< "   -t, --create-control-ntuple     Turn on the creation of the control nTuple\n"
	<< endl;
}
