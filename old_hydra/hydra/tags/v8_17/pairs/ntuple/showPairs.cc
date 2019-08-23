// $Id: showPairs.cc,v 1.2 2008-05-09 16:14:32 halo Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2006-11-24 13:08:49
//
/*************************************************************************
   Purpose: access pair ntuple created from HPairQA
            show single entries and full events

**************************************************************************/
#ifndef __CINT__
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
/////////////////////
#include <TNtuple.h>
#include <TFile.h>
#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <TObjArray.h>
#include <TObjString.h>
//#include <TUnixSystem.h>
/////////////////////
#include "pairs.h"

using namespace std;

void usage(const Char_t *arg) {

    cerr << "===> NAME" <<endl;
    cerr <<"\t "<< arg << " - create histograms from pair ntuple" <<endl;
    cerr << "===> SYNOPSIS " << endl;
    cerr << "\t " << arg << endl; 
    cerr << "\t -i, --input-file  <inputputfiles>" << endl; 
    cerr << "\t [-s, --events <events to process>]" <<endl;
    cerr << "\t -n,  --ntuple-name <ntuple name>" << endl;
#ifdef SIMULATION
    cerr << "\t -g, --generator-options [pluto|elementary|urqmd|evt_mix|pi0_divide|incoherent";
#else // ! SIMULATION
    cerr << "\t -g, --generator-options [exp";
#endif // ! SIMULATION
    cerr << "|cos_pol]"<<endl;
    cerr << "\t [-w, --warnings <default:on|off> ]" <<endl;
    cerr << "\t -h, --help" <<endl;

    cerr << "=========================================================="<<endl;
    cerr << "===> DESCRIPTION " << endl;
    cerr << "  -i, --input-file is a list of root files that contain the pair ntuple. Use whitespace as a seperator. Enclose multiple arguments in quotation marks."<< endl;
    cerr << "  -s, --events is the number of ntuple entries to process" << endl;
    cerr << "  -g, --generator-options is a comma separated list of options" << endl;
    cerr << "\t possible values are:" << endl;
    cerr << "  -n, --ntuple-name is the name of the ntuple available" << endl;
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
    cerr << "  -w, --warnings switches on (default) or off all warnings during looping over entries" << endl;
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

    Long64_t nNbEventToProcess = 0;     // ntuple entry to process
    Long64_t nNbCollisionToProcess = 0; // events to process
    Long64_t nRun = -1;                // run number
    TString ntname="";                // ntuple name in input file
    TString opt="";                   // evt gen and other options
    TChain* tup=0;                    // ntuple to process
    TString filelist="";              // list of input files
    Bool_t kWarnings=kTRUE;           // default

    static struct option long_options[] = 
	{
	    {"entry",                      1, 0, 'e'},
	    {"collision",                  1, 0, 'c'},
	    {"run",                        1, 0, 'r'},
	    {"input-file",                 1, 0, 'i'},
	    {"ntuple-name",                1, 0, 'n'},
	    {"warnings",                   1, 0, 'w'},
	    {"help",                       0, 0, 'h'},
	    {0, 0, 0, 0}
	};
    
    
    while (1) 
    {
	
 Int_t this_option_optind = optind ? optind : 1;
	
	option = getopt_long (argc, argv, "e:c:r:i:n:w:h",
			      long_options, &option_index);
	
	if (option == -1) break;
	
	switch (option) 
	{
        case 'h':
	    usage(myname);
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
        case 'e':
	    printf ("==> event \t: ");
	    if (optarg)
	    {
		printf (" %s \n", optarg);
		Char_t*endptr;
		nNbEventToProcess=strtol( optarg, &endptr,10);
		if (*endptr) 
		{
		   nNbEventToProcess  = -1;
		}

		if(nNbEventToProcess < 1)
		{
		    fprintf( stderr,
			     ">>>>> ERROR: Illegal entry number requested!\n" );
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
        case 'c':
	    printf ("==> collision \t:");
#ifdef FORMATBEFORE_AUG06
	    cerr << "This is not possible for this ntuple format!" << endl;
	    exit (1);
#endif	    
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		Char_t*endptr;
		nNbCollisionToProcess=strtol( optarg, &endptr,10);
		if (*endptr) 
		{
		   nNbCollisionToProcess  = -1;
		}

		if(nNbCollisionToProcess < 1)
		{
		    fprintf( stderr,
			     ">>>>> ERROR: Illegal entry number requested!\n" );
		    exit( 1 );
		}
	    }
	    break;
        case 'r':
	    printf ("==> run \t:");
#ifdef FORMATBEFORE_AUG06
	    cerr << "This is not possible for this ntuple format!" << endl;
	    exit (1);
#endif	    
	    if (optarg) 
	    {
		printf (" %s \n", optarg);
		Char_t *endptr;
		nRun = strtol(optarg,&endptr,10);
#ifndef SIMULATION

		if (nRun < 1)
		{
		    fprintf( stderr,
			     ">>>>> ERROR: Illegal run number requested!\n" );
		    exit( 1 );
		}
#endif // ! SIMULATION
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
    
if (!ntname.CompareTo(""))
{
    fprintf( stderr,
	     ">>>>> ERROR: No ntuple name set!\n" );
    usage(myname);
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

    




#ifdef SIMULATION
pairsS c(tup, opt); // access class for dilepton ntuple
#else // ! SIMULATION
pairsE c(tup, opt); // access class for dilepton ntuple
#endif // ! SIMULATION

c.SetWarnings(kWarnings);

c.Show(nNbEventToProcess,nNbCollisionToProcess,nRun);

return 0;
}
#endif
