/**
 * @file   hydralogon.C
 * @author Simon Lang
 * @date   Thu May 11 09:31:40 2006
 * 
 * @brief  hydralogin.C:
 *         A Root macro to make available all Hydra libraries in CINT.
 *         This macro can be listed in .rootrc
 * 
 * $Id: rootlogon.C,v 1.24 2007-06-26 16:24:37 halo Exp $
 *
 */


/** 
 * This function searches LD_LIBRARY_PATH to find each file of a 
 * list of dynamic libraries.
 * The default list of libraries is hard-coded here, and an additional 
 * list, or even a complete new list can be provided as arguments.
 * 
 * @param additional_libs All libraries in this space separated list are
 *        loaded after the default libraries.
 * @param load_from_second_location All libraries listed in this space 
 *        separated list are not loaded from the first location found in
 *        LD_LIBRARY_PATH, if they exist in another one, too.
 * @param personal_lib_list Overrides the default (hard-coded) list of
 *        libraries.
 * @param test_mode If this is true, then no libraries are loaded after
 *        the location and load order is shown.
 */
void rootlogon(TString additional_libs           = "",
	       TString load_from_second_location = "",
	       TString personal_lib_list         = "",
	       Bool_t  test_mode                 = kFALSE)
{
   TString common_libs;

   // space seperated list and order of common libraries to be loaded
   common_libs  = "Hydra ";
   common_libs += "RFIOtsm ";
   common_libs += "Start ";
   common_libs += "Hodo ";
   common_libs += "Trigger ";
   common_libs += "Rich ";
   common_libs += "Mdc ";
   common_libs += "Tof ";
   common_libs += "Tofino ";
   common_libs += "Shower ";
   common_libs += "ShowerTofino ";
   common_libs += "Rpc ";
   common_libs += "Wall ";
   common_libs += "MdcTrackD ";
   common_libs += "MdcTrackS ";
   common_libs += "MdcTrackG ";
   common_libs += "Kick ";
   common_libs += "ShowerUtil ";
   common_libs += "PidUtil ";   
   common_libs += "Pid ";
   common_libs += "Hyp ";
   common_libs += "MdcPid ";
   common_libs += "PhyAna ";
   common_libs += "Pairs ";   
   common_libs += "QA ";
   common_libs += "Dst ";
   common_libs += "Tools ";
   common_libs += "Ora ";
   common_libs += "OraUtil ";
   common_libs += "TriggerUtil ";
   common_libs += "RichUtil ";
   common_libs += "MdcUtil ";
   common_libs += "TofUtil ";
   common_libs += "Simulation ";
   common_libs += "OraSim ";
   common_libs += "MdcGarfield ";
   common_libs += "Revt ";
   common_libs += "Online ";  
   common_libs += "HadesGo4 ";  
   common_libs += "Alignment ";


   ////// here we go ...


   TString full_path;
   TString truncated_path;
   TString dir;
   TString file;
   TString filename;
   TString lib_name;
   Char_t* path      = NULL;
   Char_t* path_dir  = NULL;
   Char_t* libs      = NULL;
   Char_t* lib       = NULL;
   Bool_t  load_libs = kFALSE;
   Int_t   max_name  = 0;
   Bool_t  error     = kFALSE;

   // retrieve and clean DLL load path
   full_path = gSystem->GetDynamicPath();
   full_path.ReplaceAll( "::", ":" );
   full_path.Strip( TString::kBoth, ':' );

   // print path with one directory per line
   cout << "\nDLL Load Path Directories (in the Order of Precedence):" << endl;
   path     = StrDup( full_path );
   path_dir = strtok( path, ":" );
   while (path_dir != NULL)
   {
      cout << "   " << path_dir << endl;
      path_dir = strtok( NULL, ":" );
   }
   delete[] path;

   // handle personal list of libraries
   if (!personal_lib_list.IsNull())
   {
      common_libs = personal_lib_list;
   }
   common_libs.Prepend( " " );
   common_libs.Append( " " );

   // add a library from list of additional ones to common library list in
   // case it is not already a part of it - this check does not work if one
   // uses explicit paths for a library
   libs = StrDup( additional_libs.Data() );
   lib  = strtok( libs, " " );
   while (lib != NULL)
   {
      lib_name  = " ";
      lib_name += lib;
      lib_name += " ";
      if (!common_libs.Contains( lib_name ))
      {
	 common_libs += " ";
	 common_libs += lib;
      }
      lib = strtok( NULL, " " );
   }
   delete[] libs;
   
   // find the longest filename
   libs = StrDup( common_libs.Data() );
   lib  = strtok( libs, " " );
   while (lib != NULL)
   {
      max_name = max_name < strlen( lib )  ?  strlen( lib )  :  max_name; 
      lib = strtok( NULL, " " );
   }
   delete[] libs;

   load_from_second_location.Prepend( " " );
   load_from_second_location.Append( " " );
   cout << "\nList and Order of loaded DLLs and their Locations:" << endl;

  ProcessLibs:

   // get first library name
   libs = StrDup( common_libs.Data() );
   lib  = strtok( libs, " " );

   while (lib != NULL)
   {
      filename = lib;
      if (!filename.EndsWith( ".so" ))
      {
	 filename  = "lib";
	 filename += lib;
	 filename += ".so";
      }

      // determine full filename (of the first file location within path)
      file = gSystem->Which( full_path.Data(), filename, kReadPermission );
      if (file.IsNull())
      {
	 if (!load_libs)
	 {
	    cerr << "  ERROR: " << filename << " not found!" << endl;
	    error = kTRUE;

	    // proceed with next library
	    lib = strtok( NULL, " " );
	    continue;
	 }
	 return;
      }

      // check, if the library should be loaded from a second path location
      lib_name  = " ";
      lib_name += lib;
      lib_name += " ";
      if (load_from_second_location.Contains( lib_name ))
      {
	 // remove first location from path ...
	 dir             = ":";
	 dir            += gSystem->DirName( file.Data() );
	 dir            += ":";
	 truncated_path  = ":";
	 truncated_path += full_path;
	 truncated_path += ":";
	 truncated_path.ReplaceAll( dir, ":" );

	 // ... and search again, for a second location
	 filename =
	    gSystem->Which( truncated_path.Data(), filename, kReadPermission );
	 if (!filename.IsNull())
	 {
	    file = filename;
	 }
      }

      if (load_libs)
      {
	 // load library ...
	 if(gSystem->Load( file.Data() ) != 0)
	 {
	    cout << "ERROR: Failed to load " << lib  << " library!" << endl;
	    return;
	 }
      }
      else
      {
	 // ... or just print the location
	 cout << "  ";
	 cout.setf( ios::left );
	 cout.width( max_name + 2 ); 
	 cout << lib_name << ": " << gSystem->DirName( file ) << endl;
      }

      // proceed with next library
      lib = strtok( NULL, " " );
   }

   delete[] libs;

   // if everything is fine, process all libraries again, but now load them
   cout << endl;
   if (!load_libs  &&  !error  &&  !test_mode)
   {
      load_libs = kTRUE;
      goto ProcessLibs;
   }
   if ((!load_libs  &&  error)  ||  test_mode)
   {
      cout << "No libraries loaded!\n" << endl;
      return;
   }


   ////// ACLiC settings


   full_path = gSystem->Getenv( "MYHADDIR" );
   if (!full_path.IsNull())
   {
      full_path += "/include";
      if (!gSystem->AccessPathName( full_path.Data() ))
      {
	 full_path.Prepend( ".include " );
	 gROOT->ProcessLine( full_path.Data(), NULL );
         // gSystem->AddIncludePath( full_path.Data() ); somehow does not work!
      }
   }

   full_path = gSystem->Getenv( "HADDIR" );
   if (!full_path.IsNull())
   {
      full_path += "/include";
      if (!gSystem->AccessPathName( full_path.Data() ))
      {
	 full_path.Prepend( ".include " );
	 gROOT->ProcessLine( full_path.Data(), NULL );
         // gSystem->AddIncludePath( full_path.Data() ); somehow does not work!
      }
   }

   gSystem->SetFlagsOpt( "-O2" );

   gSystem->SetMakeSharedLib(
      "cd $BuildDir; "
      "g++ -c $Opt -pipe -Wall -fPIC -pthread $IncludePath $SourceFiles; "
      "g++ $ObjectFiles -shared -Wl,-soname,$LibName.so "
      "-O $LinkedLibs -o $SharedLib" );


   ////// CINT/RINT settings


   // use spectral color palette for histograms
   gStyle->SetPalette( 1 );
}
