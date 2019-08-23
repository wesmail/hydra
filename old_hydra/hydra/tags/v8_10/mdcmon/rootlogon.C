{
printf("\n");
//gROOT->Macro("/u/nekhaev/anal/Hydra/cur/macros/rootlogon.C");
gStyle->SetCanvasColor(0);
gStyle->SetTitleColor(0);
gStyle->SetPadColor(0);
gStyle->SetStatColor(0);
if (gSystem->Load("libPhysics.so")!=0) {
  printf("Shared library libPhysics.so could not load\n");
}
if (gSystem->Load("libProof.so")!=0) {
  printf("Shared library libProof.so could not load\n");
}
if (gSystem->Load("libTreePlayer.so")!=0) {
  printf("Shared library libTreePlayer.so could not load\n");
}
if (gSystem->Load("libHydra.so")==0) {
  printf("Shared library libHydra.so loaded\n");
} else {
  printf("Unable to load libHydra.so\n");
}
if (gSystem->Load("libMdc.so")==0) {
  //   if (gSystem->Load("/u/halo/anal/pro/lib/libMdc.so")==0) {
  printf("Shared library libMdc.so loaded\n");
} else {
  printf("Unable to load libMdc.so\n");
} 
// if (gSystem->Load("libOra.so")==0) {
//   printf("Shared library libOra.so loaded\n");
// } else {
//   printf("Unable to load libOra.so\n");
// } 
if (gSystem->Load("./libmdcmaingui.so")==0) {
  printf("Shared library libmdcmaingui.so loaded\n");
  gui = new mdcMainGUI(gClient->GetRoot(),1,1);
} else {
  printf("Unable to load libmdcmaingui.so\n");
} 
}








