{
printf("\n");
gStyle->SetCanvasColor(0);
gStyle->SetTitleColor(0);
gStyle->SetPadColor(0);
gStyle->SetStatColor(0);
if (gSystem->Load("./libmdcgui.so")==0) {
  printf("Shared library libguidemo.so loaded\n");
  gui = new mdcGUI(gClient->GetRoot(),1,1);
} else {
  printf("Unable to load libguidemo.so\n");
} 
}








