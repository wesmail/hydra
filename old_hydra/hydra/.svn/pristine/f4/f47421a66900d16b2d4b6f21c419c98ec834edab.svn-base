{

gStyle->SetPalette(1); 
gStyle->SetPadColor(19);

// "libRFIO.so",
 Char_t *libFiles[80] = {
   "libProof.so",
   "libTreePlayer.so",
   "libPhysics.so",
   "$HADDIR_BASE_LIB/libHydra.so",
   "$HADDIR_TOF_LIB/libTof.so",
   "$HADDIR_MDC_LIB/libMdc.so",
   "$HADDIR_MDCUTIL_LIB/libMdcUtil.so",
   "$HADDIR_EVTSERVER_LIB/libRevt.so",
   "$HADDIR_SHOWER_LIB/libShower.so",
   "$HADDIR_SHOWERUTIL_LIB/libShowerUtil.so",
   "$HADDIR_TOFINO_LIB/libTofino.so",
   "$HADDIR_MDCTRACKD_LIB/libMdcTrackD.so",
   "$HADDIR_MDCTRACKS_LIB/libMdcTrackS.so",
   "$HADDIR_START_LIB/libStart.so",
   "$HADDIR_SHOWERTOFINO_LIB/libShowerTofino.so",
   "$HADDIR_RICH_LIB/libRich.so",
   "$HADDIR_RICHUTIL_LIB/libRichUtil.so",
   "$HADDIR_TRIGGER_LIB/libTrigger.so",
   "$HADDIR_KICKPLANE_LIB/libKick.so",
   "$HADDIR_ORA_LIB/libOra.so",
   "$HADDIR_PHYANA_LIB/libPhyAna.so",
   "$HADDIR_ALIGNMENT_LIB/libAlignment.so",
   "$HADDIR_QA_LIB/libQA.so"
// "$HADDIR_ONLINE_LIB/libOnline.so",
 };

 printf("\nrootlogon\nloading shared libraries...\n\n");
  for(Int_t i=0;libFiles[i]!=0;i++) {
    char *path = gSystem->ExpandPathName(libFiles[i]);
    if (gSystem->Load(path)==0) printf("Library %s loaded\n",path);
   else printf("Unable to load %s\n",path);
    path =NULL;
 }
 
 if (gROOT->LoadMacro("analfunc.C")==0) {
   printf("\n\"Analysis\" macro loaded\n");
 } else {
   printf("\nUnable to load \"Analysis\" macro\n");
 } 

 printf("\nWelcome to the Hydra Root environment\n\n");
}








