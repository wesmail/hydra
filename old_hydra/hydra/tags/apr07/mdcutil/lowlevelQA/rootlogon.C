// The main idea of this file is to be loaded instead of usual rootlogon.C
// This file does not load some libraries which used to crach in a batch ROOT mode
// (usually any GUI oriented libraries has to be commented out)
// Depending on you settings you may either modify you normal rootlogon.C
// to get rid of these "bad" libraries, or you will copy this rootlogin
// to the directory from there you batch job will be launched, so that root
// will load libraries from this file...
// Looking into root outprintings in batch-output file be sure that you have
// really loaded this file and not your usuall one, defined in ~/.rootrc 

char* whichMacro(char * macro) 
{
  char *mac; 
  mac = gSystem->Which(gROOT->GetMacroPath(),macro,kReadPermission); 
  if (!mac) 
    {
      delete [] mac ;
      return 0;
    }
  char *macrodir = new char[100];
  strcpy(macrodir,mac);
  delete [] mac;
  return macrodir;
}

void rootlogon()
{
  gStyle->SetPalette(1); 
  printf("\n*****************************************************\n");
  printf("*****************************************************\n");
  printf("***  Loading rootlogon.C for BATCH calculations   ***\n");
  printf("***           SOME LIBRARIES COMMENTED OUT        ***\n");
  printf("*****************************************************\n");
  printf("*****************************************************\n\n");
  
  Char_t *libFiles[200] = {
    "libRFIO.so",
    "libProof.so",
    "libTreePlayer.so",
    "libPhysics.so"
  };

  Char_t *libHydraFiles[200] = {
    "$HADDIR_BASE_LIB/libHydra.so",
    "$HADDIR_TOF_LIB/libTof.so",
    //"$HADDIR_TOFUTIL_LIB/libTofUtil.so",
    //"$HADDIR_MDC_LIB/libMdc.so",
    "/u/sadovski/dev/lib/libMdc.so",
    "$HADDIR_MDCUTIL_LIB/libMdcUtil.so",
    "$HADDIR_EVTSERVER_LIB/libRevt.so",
    "$HADDIR_SHOWER_LIB/libShower.so",
    //"$HADDIR_SHOWERUTIL_LIB/libShowerUtil.so",
    "$HADDIR_TOFINO_LIB/libTofino.so",
    //"$HADDIR_MDCGARFIELD_LIB/libMdcGarfield.so",
    "$HADDIR_MDCTRACKD_LIB/libMdcTrackD.so",
    "$HADDIR_MDCTRACKS_LIB/libMdcTrackS.so",
    "$HADDIR_START_LIB/libStart.so",
    "$HADDIR_SHOWERTOFINO_LIB/libShowerTofino.so",
    "$HADDIR_RICH_LIB/libRich.so",
    "$HADDIR_RICHUTIL_LIB/libRichUtil.so",
    "$HADDIR_TRIGGER_LIB/libTrigger.so",
    "$HADDIR_QA_LIB/libQA.so",
    "$HADDIR_PHYANA_LIB/libPhyAna.so",
    "$HADDIR_KICKPLANE_LIB/libKick.so",
    //"$HADDIR_PID_LIB/libPid.so",
    "$HADDIR_ORA_LIB/libOra.so",
    "$HADDIR_ORAUTIL_LIB/libOraUtil.so",
    //"$HADDIR_HADESGO4_LIB/libHadesGo4.so"
  };

  Char_t *libPrivateFiles[200] = {
    //     "$HADDIR_MDCDEV_LIB/libMdcDev.so" // i.e.
  };

  printf("\nHALO rootlogon ... loaded from: \"%s\".\n\n...loading shared libraries\n\n",whichMacro("rootlogon.C"));

  for(Int_t i=0;libFiles[i]!=0;i++) 
    {
      char *path = gSystem->ExpandPathName(libFiles[i]);
      if (gSystem->Load(path)==0) printf("Library %s loaded\n",path);
      else Error("Load Library","Unable to load %s",path);
      path =NULL;
  }
 
  for(Int_t i=0;libHydraFiles[i]!=0;i++) 
    {
      char *path = gSystem->ExpandPathName(libHydraFiles[i]);
      if (!gSystem->AccessPathName(path,kReadPermission))
	{
	  if(gSystem->Load(path)==0) printf("Library %s loaded\n",path);
	  else Error("Load Library","Unable to load %s",path);
	}
      else Error("Load Library","Unable to load %s",path);
      path =NULL;
    }

  for(Int_t i=0;libPrivateFiles[i]!=0;i++) 
    {
      char *path = gSystem->ExpandPathName(libPrivateFiles[i]);
      if (!gSystem->AccessPathName(path,kReadPermission))
	{
	  if(gSystem->Load(path)==0) printf("Library %s loaded\n",path);
	  else Error("Load Library","Unable to load %s",path);
	}
      else Error("Load Library","Unable to load %s",path);
      path =NULL;
    }
  
  if (whichMacro("analfunc.C")) if (gROOT->LoadMacro("analfunc.C")==0) printf("\n\"Analysis\" macro loaded\n");
  else printf("\nUnable to load \"Analysis\" macro\n");
  
  printf("\nWelcome to the Hydra Root environment\n\n");
  
}







  
