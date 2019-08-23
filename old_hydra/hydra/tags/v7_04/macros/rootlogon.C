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
  
  Char_t *libFiles[200] = {
    "libRFIO.so",
    "libProof.so",
    "libTreePlayer.so",
    "libPhysics.so"
  };

  Char_t *libHydraFiles[200] = {
    "$HADDIR_BASE_LIB/libHydra.so",
    "$HADDIR_SIMULATION_LIB/libSimulation.so",
    "$HADDIR_TOF_LIB/libTof.so",
    "$HADDIR_TOFUTIL_LIB/libTofUtil.so",
    "$HADDIR_MDC_LIB/libMdc.so",
    "$HADDIR_EVTSERVER_LIB/libRevt.so",
    "$HADDIR_SHOWER_LIB/libShower.so",
    "$HADDIR_SHOWERUTIL_LIB/libShowerUtil.so",
    "$HADDIR_TOFINO_LIB/libTofino.so",
    "$HADDIR_MDCGARFIELD_LIB/libMdcGarfield.so",
    "$HADDIR_MDCTRACKD_LIB/libMdcTrackD.so",
    "$HADDIR_MDCTRACKS_LIB/libMdcTrackS.so",
    "$HADDIR_MDCTRACKG_LIB/libMdcTrackG.so",
    "$HADDIR_MDCUTIL_LIB/libMdcUtil.so",
    "$HADDIR_START_LIB/libStart.so",
    "$HADDIR_SHOWERTOFINO_LIB/libShowerTofino.so",
    "$HADDIR_RICH_LIB/libRich.so",
    "$HADDIR_RICHUTIL_LIB/libRichUtil.so",
    "$HADDIR_TRIGGER_LIB/libTrigger.so",
    "$HADDIR_TRIGGERUTIL_LIB/libTriggerUtil.so",
    "$HADDIR_QA_LIB/libQA.so",
    "$HADDIR_PHYANA_LIB/libPhyAna.so",
    "$HADDIR_KICKPLANE_LIB/libKick.so",
    "$HADDIR_PID_LIB/libPid.so",
    "$HADDIR_PID_LIB/libPidUtil.so",
    "$HADDIR_ORA_LIB/libOra.so",
    "$HADDIR_ORASIM_LIB/libOraSim.so",
    "$HADDIR_ORAUTIL_LIB/libOraUtil.so",
    "$HADDIR_HADESGO4_LIB/libHadesGo4.so",
    "$HADDIR_MDCPID_LIB/libMdcPid.so",
    "$HADDIR_TOOLS_LIB/libTools.so"
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







  
