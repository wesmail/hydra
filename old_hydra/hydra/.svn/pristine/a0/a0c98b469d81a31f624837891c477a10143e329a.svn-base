// $Id: readPairs.cc,v 1.13 2008-09-18 13:06:49 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-12-11 18:24:26
//
/*************************************************************************
   Purpose: access pair ntuple created from HPairQA

**************************************************************************/
#ifndef __CINT__
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
/////////////////////
#include "TNtuple.h"
#include "TFile.h"
#include "TH1.h"
#include "TH2.h"
#include "TH3.h"
#include "TObjArray.h"
#include "TObjString.h"
/////////////////////
#include "pairs.h"

using namespace std;

void usage(const Char_t *arg) 
{
    cerr << "===> NAME" <<endl;
    cerr <<"\t "<< arg << " - create histograms from pair ntuple" <<endl;
    cerr << "===> SYNOPSIS " << endl;
    cerr << "\t " << arg << endl; 
    cerr << "\t -o, --output-file <outputfile>" << endl; 
    cerr << "\t -i, --input-file  <inputputfiles>" << endl; 
    cerr << "\t [-s, --events <events to process>]" <<endl;
    cerr << "\t [-f, --normalization-factor <normalization factor>]" << endl;
    cerr << "\t -n,  --ntuple-name <ntuple name>" << endl;
#ifdef SIMULATION
    cerr << "\t -g, --generator-options [pluto|elementary|urqmd|evt_mix|pi0_divide|incoherent";
#else // ! SIMULATION
    cerr << "\t -g, --generator-options [exp";
#endif // ! SIMULATION
    cerr << "|cos_pol]"<<endl;
    cerr << "\t [-p, p-eff-matrix <e+ eff. matrix file>]" << endl;
    cerr << "\t [-e, e-eff-matrix <e- eff. matrix file>]" << endl;
    cerr << "\t [-w, --warnings <default:on|off> ]" <<endl;
    cerr << "\t -c, --create" <<endl;
    cerr << "\t -h, --help" <<endl;

    cerr << "=========================================================="<<endl;
    cerr << "===> DESCRIPTION " << endl;
    cerr << "  -o, --output-file is the root file to contain the histograms"<< endl;
    cerr << "  -i, --input-file is a list of root files that contain the pair ntuple. Use whitespace as a seperator. Enclose multiple arguments in quotation marks."<< endl;
    cerr << "  -s, --events is the number of ntuple entries to process" << endl;
    cerr << "  -f, --normalization-factor is the number of events to normalize to" << endl;
    cerr << "  -n, --ntuple-name is the name of the ntuple available" << endl;
    cerr << "  -g, --generator-options is a comma separated list of options" << endl;
    cerr << "\t possible values are:" << endl;
#ifdef SIMULATION
    cerr << "\t    - pluto" << endl;
    cerr << "\t    - elementary" << endl;
    cerr << "\t    - urqmd" << endl;
    cerr << "\t    - evt_mix" << endl;
    cerr << "\t    - pi0_divide" << endl;
    cerr << "\t    - incoherent" << endl;
#else // ! SIMULATION
    cerr << "\t    - exp" << endl;
#endif // ! SIMULATION
    cerr << "\t    - cos_pol" << endl;
    cerr << "  -p, --p-eff-matrix is the root file that contains the ";
    cerr << "efficiency matrix for positrons (non-existence is ";
    cerr << "indicated by a missing .root extension)" << endl;
    cerr << "  -e, --e-eff-matrix is the root file that contains the ";
    cerr << "efficiency matrix for electrons (non-existence is ";
    cerr << "indicated by a missing .root extension)" << endl;
    cerr << "  -w, --warnings switches on (default) or off all warnings during looping over entries" << endl;
    cerr << "  -c, --create enforces the creation of the output file" << endl;
    cerr << "  -h, --help prints this help" << endl;
    exit(1);
}

Int_t main(Int_t argc, Char_t **argv)
{

    // first parse command line arguments
    // c.f. "man 3 getopt"
    Int_t option;
    Int_t digit_optind = 0;
    Int_t option_index = 0;
    
    const Char_t *myname = argv[0];

    // variables to be filled by command line arguments
    TFile* outFile = 0;               // histogram file
    TString outFileName = "";         // histogram file name
    Double_t events = 1.;             // normalization factor
    Int_t nNbEventsToProcess = 0;     // nb of ntuple entries to process
    TString ntname="";                // ntuple name in input file
    TString opt="";                   // evt gen and other options
    TString emEff="";                 // e- eff. matrix file
    TString epEff="";                 // e+ eff. matrix file
    TChain* tup=0;                    // ntuple to process
    TString filelist="";              // list of input files
    Bool_t kWarnings=kTRUE;           // default
    Bool_t kForceCreateOutput=kFALSE; // overwrite output file

    static struct option long_options[] = 
	{
	    {"e-eff-matrix",               1, 0, 'e'},
	    {"events",                     1, 0, 's'},
	    {"input-file",                 1, 0, 'i'},
	    {"normalization-factor",       1, 0, 'f'},
	    {"ntuple-name",                1, 0, 'n'},
	    {"generator-options",          1, 0, 'g'},
	    {"output-file",                1, 0, 'o'},
	    {"p-eff-matrix",               1, 0, 'p'},
	    {"warnings",                   1, 0, 'w'},
	    {"create",                     0, 0, 'c'},
	    {"help",                       0, 0, 'h'},
	    {0, 0, 0, 0}
	};
    
    
    while (1) 
    {
	
 Int_t this_option_optind = optind ? optind : 1;
	
	option = getopt_long (argc, argv, "e:s:i:f:n:g:o:p:w:hc",
			      long_options, &option_index);
	
	if (option == -1) break;
	
	switch (option) 
	{
        case 'h':
	    usage(myname);
	    break;
        case 'c':
	    kForceCreateOutput = kTRUE;
	    break;
        case 'w':
	    printf ("==> warnings \t: ");
	    if (optarg) 
	    {
		TString stmp(optarg);
		if (!stmp.CompareTo("on"))
		{
		    printf (" %s \n", optarg);
		}
		else if (!stmp.CompareTo("off"))
		{
		    kWarnings=kFALSE;
		    printf (" %s \n", optarg);
		    fprintf( stderr,
			     ">>>>> WARNING: All loop warnings switched off!\n" );
		}
		else
		{
		    printf (" %s \n", optarg);
		    fprintf( stderr,
			     ">>>>> ERROR: Illegal argument!\n" );
		}
	    }
	    break;
        case 'o':
	    printf ("==> output \t: ");
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		outFileName = optarg;
	    }
	    break;
        case 's':
	    printf ("==> events \t: ");
	    if (optarg)
	    {
		printf (" %s \n", optarg);
		if (sscanf( optarg, "%d", &nNbEventsToProcess) != 1
		    ||
		    nNbEventsToProcess < 1)
		{
		    fprintf( stderr,
			     ">>>>> ERROR: Illegal number of events requested!\n" );
		    exit( 1 );
		}
	    }
	    break;
        case 'n':
	    printf ("==> ntuple \t:");
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		ntname=optarg;
	    }
	    break;
        case 'g':
	    printf ("==> generator \t:");
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		opt=optarg;
	    }
	    break;
        case 'f':
	    printf ("==> normalization \t:");
	    if (optarg)
	    {
		printf (" %s \n", optarg);

		Char_t *endptr;
		events = strtod(optarg, &endptr);
		if (*endptr) 
		{
		    events = 1.;
		}
		if (events<1.)
		{
		    fprintf( stderr,
			     ">>>>> ERROR: Illegal normalization factor set!\n" );
		    exit( 1 );
		}

	    }
	    break;
        case 'e':
	    printf ("==> e- eff \t:");
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		emEff=optarg;
	    }
	    break;
        case 'p':
	    printf ("==> e+ eff \t:");
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		epEff=optarg;
	    }
	    break;
        case 'i':
	    printf ("==> input \t:");
	    if (optarg) 
	    {
		filelist=optarg;
		printf (" %s \n", optarg);
	    }
	    break;
	case '2':
	    if (digit_optind != 0 && digit_optind != this_option_optind)
		printf ("digits occur in two different argv-elements.\n");
	    digit_optind = this_option_optind;
	    printf ("option %c\n", option);
	    break;
	case '?':
	    usage(myname);
	    break;
	default:
	    printf ("?? getopt returned character code 0%o ??\n", option);
	    usage(myname);
	}
	
    }
    
    
    ///////////////////////////////////////////////////////////////////////
    // print out non-option command line arguments
    if (optind < argc) 
    {
	printf ("non-option ARGV-elements: ");
	while (optind < argc)
	    printf ("%s ", argv[optind++]);
	printf ("\n");
	usage(myname);
    }

    ///////////////////////////////////////////////////////////////////////
    
    // check if we have all params
    if (!outFileName.CompareTo("") )
    {
	fprintf( stderr,
		 ">>>>> ERROR: No output file set!\n" );
	usage(myname);
	exit (1);
    }
    else
    {
	outFile = TFile::Open( outFileName.Data(), 
			       (kForceCreateOutput?"RECREATE":"CREATE") );
	if (!outFile || outFile->IsZombie()) 
	{
	    cerr << ">>>>> ERROR: opening output file (does " << 
		outFileName.Data() << " exist?)." << endl;
	    cerr << ">>>>> Pass \"-c\" argument to force re-creation of output file." << endl;
	    exit(1);
	}
    }
    if (events==1) 
    {
	fprintf( stderr,
		 ">>>>> WARNING: Normalization factor is set to '1'!\n" );
    }
    if (nNbEventsToProcess==0)
    {
	fprintf( stderr,
		 ">>>>> WARNING: All entries in ntuple will be processed!\n" );
    }
    if (!ntname.CompareTo(""))
    {
	fprintf( stderr,
		 ">>>>> ERROR: No ntuple name set!\n" );
	usage(myname);
    }
    if (!opt.CompareTo(""))
    {
	fprintf( stderr,
		 ">>>>> ERROR: No event generator options set!\n" );
	usage(myname);
    }
    else if (!opt.CompareTo("urqmd")) 
    {
	fprintf( stderr,
		 ">>>>> WARNING: URQMD has hard-wired enhancement factors (10x) and only for eta mesons !\n" );
    }
    if (!emEff.CompareTo("")||!epEff.CompareTo(""))
    {
	fprintf( stderr,
		 ">>>>> WARNING: efficiency matrix file not set!\n" );
    }
    if (!tup)
    {
	tup = new TChain(ntname);
	if (!tup) 
	{
	    cerr << ">>>>> No ntuple chain created !!!" <<endl;
	    exit(1);
	}

	TObjArray *arr = filelist.Tokenize(" ");
	cout << endl << "--------------------------------------------" << endl;
	cout << "=====> Now connecting files ... " << endl;
	for (Int_t i=0;i<arr->GetLast()+1;i++)
	{
	    TObjString* o = (TObjString*)(*arr)[i];
	    TString s = o->GetString();
	    tup->Add(s.Data());
	    cout << "=====> file "<<i+1<<" : "<<s.Data()<<endl;

	}
	cout<<"======> Number of connected files: "<<arr->GetLast()+1<<endl;
	if (filelist.Contains("*"))
	    cout <<"======> Wildcard expanded internally!" <<endl;
	cout << endl << "--------------------------------------------" << endl;
	arr->Delete();
	delete arr;
	
    }

    
#if 0
    ///////////////////////////////////////////////////////////////////////
    // old command line parsing
    ///////////////////////////////////////////////////////////////////////

	// need ouput, [options], input [...]
    const Char_t *myname = argv[0];
    argv++; argc--;

	// first argument: output file
	if (!argc) usage(myname);
	TString outFileName(argv[0]);
	TFile* outFile = new TFile(outFileName.Data(),"RECREATE");
	cout<<"Name of output file: "<<outFileName.Data()<<endl;
	argv++; argc--;

	// second argument: option string for pair class
	if (!argc) usage(myname);
	TString opt(argv[0]);
	cout<<"Event generator options: "<<opt.Data()<<endl;
	argv++; argc--;

	// third argument: ntuple name
	if (!argc) usage(myname);
	TString ntname(argv[0]);
	cout<<"Ntuple Name: "<<ntname.Data()<<endl;
	argv++; argc--;

	// forth argument: event number for normalization
	Double_t events = 1;

	if (!argc) usage(myname);
	Char_t *endptr;
	events = strtod(argv[0], &endptr);
	if (*endptr) {
#ifdef SIMULATION
		events = 1.; // change here to have normalisation
#else // ! SIMULATION
#if DS
		events = 1.; // downscaled
#endif // DS
#if LVL2
		events = 1.;
		// downscaled * ds-factor * lvl2-eff.
#endif // LVL2
#endif // ! SIMULATION

	} else {
		argv++; argc--;
	}
	cout<<"Events to normalize to: "<<events<<endl;

	// fifth argument: ep-matrix
	if (!argc) usage(myname);
	TString epEff(argv[0]);
	cout<<"Positrons efficiency matrix: "<<epEff.Data()<<endl;
	argv++; argc--;
	
	// sixth argument: em-matrix
	if (!argc) usage(myname);
	TString emEff(argv[0]);
	cout<<"Electrons efficiency matrix: "<<epEff.Data()<<endl;
	argv++; argc--;

	// seventh argument: number of ntuple entries to process
	Int_t nNbEventsToProcess = 0;

	if (!argc) usage(myname);
	nNbEventsToProcess = strtol(argv[0],&endptr,10);
	if (*endptr)
	{
	    nNbEventsToProcess=0;
	    cout<<"Number of entries to process: ALL"<<endl;
	}
	else
	{
	    cout<<"Number of entries to process: "
		<< nNbEventsToProcess << endl;
	    argv++; argc--;
	}

	// other arguments:  input files to chain
	if (!argc) usage(myname);
	TChain* tup = new TChain(ntname);
	for (Int_t i=0;i<argc;i++)
	{
		if (argv[i])
		{
			tup->Add(argv[i]);
			cout<<argv[i]<<" added to TChain."<<endl;
		}
	}
	cout<<"Number of files provided : "<<argc<<endl;

    ///////////////////////////////////////////////////////////////////////
    // END old command line parsing
    ///////////////////////////////////////////////////////////////////////
#endif




	// START PROCESSING EVENTS 

	///////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////
	// ACCESS NTUPLE

	outFile->cd();

#ifdef SIMULATION
	pairsS c(tup, opt); // access class for dilepton ntuple
#else // ! SIMULATION
	pairsE c(tup, opt); // access class for dilepton ntuple
#endif // ! SIMULATION

	c.setPosiEffFileName(epEff);
	c.setEleEffFileName(emEff);
	c.SetEvents(events);
	c.SetWarnings(kWarnings);
	// "event loop" of access class, "0" means all events in file
	c.Loop(nNbEventsToProcess); 

	
	TObjArray* harr = c.getHistograms();
	if (harr)
	{
	    cout << endl << endl << endl
		 << "--------------------------------------------" <<endl;
	    cout<<"========>  " << harr->GetEntries()
		<<" control histograms created, written to file " 
		<< outFileName.Data() << "!" << endl;
	    outFile->cd();
	    for (Int_t i=0; i<harr->GetEntries(); i++)
	    {
		((TH1*)(*harr)[i] ) -> Write();
#ifndef NDEBUG
		cout<<"saving control histogram "
		    <<((TH1*)(*harr)[i] )->GetName()
		    <<" to file "<<outFile->GetName()<<endl;
#endif // ! NDEBUG
	    }
	    
	}
	else
	{
	    cerr<<"========> ERROR: histogram array is NULL" << endl;
	}
	
	
	return 0;
}
#endif
