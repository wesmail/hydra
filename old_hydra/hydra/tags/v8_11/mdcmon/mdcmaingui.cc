#if !defined S_ISDIR
#define S_ISDIR(m) (((m)&(0170000)) == (0040000))
#endif
#include "mdcmaingui.h"
#include "mdcgui.h"

ClassImp(mdcMainGUI)
ClassImp(HldMsgBox)
ClassImp(HldFileSearch)
  
  mdcMainGUI::mdcMainGUI(const TGWindow *p, UInt_t w, UInt_t h) : 
    TGMainFrame(p, w, h) {
  
  pMdcMain = new TGCompositeFrame(this,1800,1400,kVerticalFrame);
  AddFrame(pMdcMain, new TGLayoutHints(kLHintsNormal));

  pMDC = new TGGroupFrame(pMdcMain," Choose MDC module: ",kHorizontalFrame);
  pMdcMain->AddFrame(pMDC, new TGLayoutHints(kLHintsNormal,5,0,5,0));
  pInput = new TGGroupFrame(pMdcMain," Input: ",kHorizontalFrame);
  pInput->SetLayoutManager(new TGMatrixLayout(pInput,0,3,15));
  pMdcMain->AddFrame(pInput, new TGLayoutHints(kLHintsNormal,5,0,20,0));

  pSector = new TGCompositeFrame(pMDC,400,50,kHorizontalFrame);
  pSector->SetLayoutManager(new TGMatrixLayout(pSector,0,1));
  pMDC->AddFrame(pSector, new TGLayoutHints(kLHintsNormal, 25, 15, 5, 0 ));

  pModule = new TGCompositeFrame(pMDC,400,50,kHorizontalFrame);
  pModule->SetLayoutManager(new TGMatrixLayout(pModule,0,1));
  pMDC->AddFrame(pModule, new TGLayoutHints(kLHintsNormal, 50, 25, 5, 0 ));


  // Restore parameters from last run

  sprintf(saveFile,"%s%s",getenv("HADDIR"),"/mdcmon/param/saveparam.txt");	
  ifstream *in = new ifstream(saveFile);
  if(access(saveFile,F_OK)) {  // file doesn't exist
    in->close();
    char line[100];
    secNum = 2;
    modNum = 1;
    evtNum = 1000;
    ofstream *out = new ofstream(saveFile);
    if(access(saveFile,F_OK)) { 
      cout <<"Cannot open file" << endl;
    }
    sprintf(line,"%i\n", secNum); 
    out->write(line,strlen(line));
    sprintf(line,"%i\n", modNum); 
    out->write(line,strlen(line));
    sprintf(line,"%i\n", evtNum); 
    out->write(line,strlen(line));
    sprintf(line,"%s\n", "/d/hades/mdc/pulser/"); 
    out->write(line,strlen(line));
    sprintf(line,"%s\n", "puls_weiss_nix_130300.hld"); 
    out->write(line,strlen(line));
    out->close();
  }
  else {
    in->getline(buf,100);
    sscanf(buf,"%i",&secNum);
    in->getline(buf,100);
    sscanf(buf,"%i",&modNum);
    in->getline(buf,100);
    sscanf(buf,"%i",&evtNum);
    in->getline(buf,100);
    sscanf(buf,"%s",hldDir);
    in->getline(buf,100);
    sscanf(buf,"%s",hldFile);
    in->close();
  }

  // Creates combos for sector&module numbers 

  pSector->AddFrame(new TGLabel(pSector, new TGHotString("Sector: ")),
		    new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  pComboSector = new TGComboBox(pSector,M_SECTORMAIN);
  pComboSector->Associate(this);
  for (i=0;i<6;i++) {
    sprintf(tmp,"%i",i+1);
    pComboSector->AddEntry(tmp,i);
  }
  pComboSector->Resize(70,20);
  pComboSector->Select(secNum);
  pSector->AddFrame(pComboSector);
  
  pModule->AddFrame(new TGLabel(pModule, new TGHotString("Module: ")),
		    new TGLayoutHints(kLHintsNormal, 0, 0, 0, 0));
  pComboModule = new TGComboBox(pModule,M_MODULEMAIN);
  pComboModule->Associate(this);
  for (i=0;i<4;i++) {
    sprintf(tmp,"%i",i+1);
    pComboModule->AddEntry(tmp,i);
  }
  pComboModule->Resize(70,20);
  pComboModule->Select(modNum);
  pModule->AddFrame(pComboModule);
  
  pOnlineButton = new TGTextButton(pInput, "Online", M_ONLINE);
  pOnlineButton->Associate(this);
  pOnlineButton->SetToolTipText("Connect to DAQ server");
  pInput->AddFrame(pOnlineButton, new TGLayoutHints(kLHintsNormal,10, 0, 10, 20));
  pOnlineButton->Resize(80, 22);

  pHldButton = new TGTextButton(pInput, "Hld Files", M_HLDFILE);
  pHldButton->Associate(this);
  pHldButton->Resize(80, 22);
  pHldButton->SetToolTipText("Choose Hld file");
  pInput->AddFrame(pHldButton, new TGLayoutHints(kLHintsNormal,10, 0, 10, 20));

  pCloseButton = new TGTextButton(pInput, "Close Panel", M_CLOSE);
  pCloseButton->Associate(this);
  pCloseButton->Resize(80, 22);
  pCloseButton->SetToolTipText("Close GUI Interface");
  pInput->AddFrame(pCloseButton, new TGLayoutHints(kLHintsNormal,10, 0, 10, 20));

  MapSubwindows();
  SetWindowName("MDC Monitoring Version 3_00 ");
  Resize(GetDefaultSize());
  //  Resize(300,180);
  MapWindow();
}

mdcMainGUI::~mdcMainGUI() {
 
  delete pCloseButton;
  delete pHldButton;
  delete pOnlineButton;
  delete pModule;
  delete pSector;
  delete pInput;
  delete pMDC;
  delete pMdcMain;
}


HldMsgBox::HldMsgBox(const TGWindow *p, UInt_t w, UInt_t h, Int_t secnum, Int_t modnum, UInt_t options) :
  TGMainFrame(p, w, h) {
  gHldMsgBox = this;
  secNum = secnum;
  modNum = modnum;
  char tmp[20];
  readParam();
  pHldMain = new TGCompositeFrame(this, 400, 400, kVerticalFrame);
  AddFrame(pHldMain, new TGLayoutHints(kLHintsNormal));
  pHldMsg = new TGCompositeFrame(pHldMain, 400, 100);
  //  pHldMsg = new TGCompositeFrame(pHldMain, 400, 100, kHorizontalFrame);
  //  pHldMsg->SetLayoutManager(new TGMatrixLayout(pHldMsg,2,2));
  pHldMain->AddFrame(pHldMsg, new TGLayoutHints(kLHintsNormal,5,0,20,0));
  pHldBrow = new TGCompositeFrame(pHldMain, 400, 100, kHorizontalFrame);
  pHldMain->AddFrame(pHldBrow, new TGLayoutHints(kLHintsNormal,5,0,0,0));

  pHldBut = new TGCompositeFrame(pHldMain, 400,100, kHorizontalFrame);
  pHldBut->SetLayoutManager(new TGMatrixLayout(pHldBut,0,3,30));
  pHldMain->AddFrame(pHldBut, new TGLayoutHints(kLHintsNormal,0,0,0,0));

  pMsg1   = new TGLabel(pHldMsg, new TGString("Number of events:"));
  pHldMsg->AddFrame(pMsg1,new TGLayoutHints(kLHintsNormal, 0, 10, 0, 0));
  pEvtNum = new TGTextEntry(pHldMsg, pEvtNumBuf = new TGTextBuffer(100));
  sprintf(tmp,"%i",evtNum);
  pEvtNumBuf->AddText(0, tmp);
  pEvtNum->Resize(50, pEvtNum->GetDefaultHeight());
  pHldMsg->AddFrame(pEvtNum,new TGLayoutHints(kLHintsNormal, 100, 10, 10, 0));

  pMsg2   = new TGLabel(pHldMsg, new TGString("Name of Hld File:"));
  pHldMsg->AddFrame(pMsg2,new TGLayoutHints(kLHintsNormal, 0, 10, 10, 0));
  pHldFileName = new TGTextEntry(pHldMsg, pHldFileBuf = new TGTextBuffer(100));
  pHldFileBuf->AddText(0, DirFile);
  pHldFileName->Resize(300, pHldFileName->GetDefaultHeight());
  pHldMsg->AddFrame(pHldFileName,new TGLayoutHints(kLHintsNormal, 0, 10, 10, 0));

  pBrowseButton = new TGTextButton(pHldBrow, "&Browse...", M_HLDBROWSE);
  pBrowseButton->Associate(this);
  pBrowseButton->SetToolTipText("Browse ...");
  pHldBrow->AddFrame(pBrowseButton, new TGLayoutHints(kLHintsNormal,230, 0, 10, 0));
  pBrowseButton->Resize(80, 22);

  pExecuteButton = new TGTextButton(pHldBut, "&Execute", M_HLDEXECUTE);
  pExecuteButton->Associate(this);
  pExecuteButton->Resize(80, 22);
  pExecuteButton->SetToolTipText("Start Event Loop ");
  pHldBut->AddFrame(pExecuteButton, new TGLayoutHints(kLHintsNormal,0, 0, 0, 0));
 
  pClearButton = new TGTextButton(pHldBut, "C&lear", M_HLDCLEAR);
  pClearButton->Associate(this);
  pClearButton->Resize(80, 22);
  pClearButton->SetToolTipText("Clear Hld Msg Box");
  pHldBut->AddFrame(pClearButton, new TGLayoutHints(kLHintsNormal,0, 0, 0, 0));
 
  pCloseButton = new TGTextButton(pHldBut, "&Close", M_HLDCLOSE);
  pCloseButton->Associate(this);
  pCloseButton->Resize(80, 22);
  pCloseButton->SetToolTipText("Close Hld Msg Box");
  pHldBut->AddFrame(pCloseButton, new TGLayoutHints(kLHintsNormal,0, 0, 0, 0));

  MapSubwindows();
  //  Resize(400,300);
  Resize(GetDefaultSize());
  SetWindowName("Choose Hld File");
  MapWindow();
  //  fClient->WaitFor(this);

} 

HldMsgBox::~HldMsgBox () {
  delete pCloseButton;
  delete pClearButton;
  delete pExecuteButton;
  delete pHldBut;
  delete pHldBrow;
  delete pHldMsg;
  delete pHldMain;
}

void HldMsgBox::closeWindow() { 
  delete this;
}

void HldMsgBox::closeWindowHld() { 
  delete this;
}

void HldMsgBox::fileDirDefine() {
  int i,j,fileSize;
  fileSize = 0;      //size of fileName string
  sprintf(DirFile,"%s",pHldFileBuf->GetString());
  for(i=strlen(DirFile);i--;) {
    if(DirFile[i]=='/') break;
    fileSize++;
  }
  
  for(j=0;j<(int)(strlen(DirFile)-fileSize);j++) dirName[j]=DirFile[j];
  dirName[j]='\0';
  for(j=(int)(strlen(DirFile)-fileSize);j<(int)strlen(DirFile);j++) { 
    fileName[j-strlen(DirFile)+fileSize]=DirFile[j];
  }
  fileName[j-strlen(DirFile)+fileSize]='\0';
  
}

void HldMsgBox::readParam() {
  char buf[200];
  sprintf(saveFile,"%s%s",getenv("HADDIR"),"/mdcmon/param/saveparam.txt");	
  ifstream *in = new ifstream(saveFile);
  if(access(saveFile,F_OK)) { 
    cout <<"Cannot open file" << endl;
  }
  else {
    in->getline(buf,100);
    sscanf(buf,"%i",&secNumOld); // Zateraet novie znacheniya!!!
    in->getline(buf,100);
    sscanf(buf,"%i",&modNumOld);
    in->getline(buf,100);
    sscanf(buf,"%i",&evtNumOld);
    evtNum = evtNumOld;
    in->getline(buf,100);
    sscanf(buf,"%s",hldDirOld);
    in->getline(buf,100);
    sscanf(buf,"%s",hldFileOld);
    //    sprintf(hldDirOld,"%s",hldDir);
    //    sprintf(hldFileOld,"%s",hldFile);
    sprintf(DirFile,"%s%s",hldDirOld,hldFileOld);
    in->close();
  }
}

void HldMsgBox::updateParam() {
  char buf[200];
  sprintf(saveFile,"%s%s",getenv("HADDIR"),"/mdcmon/param/param.C");	
  ofstream *out = new ofstream(saveFile);
  if(access(saveFile,F_OK)) { 
    cout <<"Cannot open file" << endl;
  }
  else {
    sprintf(buf,"%s\n","{ gROOT->LoadMacro(\"monit.C\"); ");
    out->write(buf,strlen(buf));
    sprintf(buf,"%s%i%s%i%s%i%s%s%s%s%s%s\n","gROOT->ProcessLine(\"mdcmonhld(",secNum,",",modNum,",",evtNum,",\\\"",hldDir,"\\\"",",\\\"",hldFile,"\\\")\");");
    out->write(buf,strlen(buf));
    sprintf(buf,"%s\n","gROOT->ProcessLine(\".q\"); } ");
    out->write(buf,strlen(buf));
    out->close();
  }

}

int HldMsgBox::saveParam() {
  // save paramaters in the file
  fileDirDefine();
  hldDir = dirName;
  hldFile = fileName;
    int retval; //for class TGMsgBox
    int buttons;//used to construct message panel when Close button is clicked
    char tmp[100];
     if(secNum==secNumOld&&modNum==modNumOld&&evtNum==evtNumOld&&(!strcmp(hldFile,hldFileOld))&&(!strcmp(hldDir,hldDirOld))) {
       //      sprintf(tmp,"%s%s%s","/tmp/",getenv("USER"),"/mdcmon.map");	
      sprintf(tmp,"%s%s","/tmp/","mdcmon.map");	
       if(!access(tmp,F_OK)) { 
	 retval = 0;
	 buttons = 0;
	 buttons |= kMBOk;
	 buttons |= kMBNo;
	 new TGMsgBox(fClient->GetRoot(), this, "Confirm action",
		      "You have this map file already! Analyse again?",
		      kMBIconQuestion,buttons,&retval);
	 if(retval==2) return 1;
       }
     }
     else { 
       ofstream *out = new ofstream(saveFile);
       if(access(saveFile,F_OK)) { 
	 cout <<"Cannot open file" << endl;
	 return 1;
       }
       char line[100];
       sprintf(line,"%i\n", secNum); 
       out->write(line,strlen(line));
       sprintf(line,"%i\n", modNum); 
       out->write(line,strlen(line));
       sprintf(line,"%i\n", evtNum); 
       out->write(line,strlen(line));
       sprintf(line,"%s\n", hldDir); 
       out->write(line,strlen(line));
       sprintf(line,"%s\n", hldFile); 
       out->write(line,strlen(line));
       out->close();
     }
    return 0;
}



Bool_t HldMsgBox::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {
  char command[200], tmp[100];
  int flag; 
  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      switch(parm1) {	
      case M_HLDEXECUTE:
	evtNum = (atoi)(pEvtNumBuf->GetString());
	flag = saveParam();
	if(! flag) {
 
	  sprintf(command,"%s%s%s","chmod u+x ", getenv("HADDIR"),"/mdcmon/param/startxterm");
	  if(access(command,X_OK)) gSystem->Exec(command);

	  sprintf(tmp,"%s%s",getenv("HADDIR"),"/mdcmon/param");
	  gSystem->ChangeDirectory(tmp);
	  sprintf(command,"%s%s%s","xterm -iconic -e ", getenv("HADDIR"), "/mdcmon/param/startxterm &" );
	  updateParam();
	  gSystem->Exec(command);
	}
	// closeWindowHld();
	// closeWindow();
	new mdcGUI(gClient->GetRoot(),gClient->GetRoot(), 400, 200, secNum, modNum);
	//	delete this;
	break;
	case M_HLDCLEAR:
	  pHldFileBuf->Clear();
	  fClient->NeedRedraw(pHldFileName);
	  pEvtNumBuf->Clear();
	  fClient->NeedRedraw(pEvtNum);
	  break;	  
	case M_HLDCLOSE:
	  closeWindow();
	  gROOT->ProcessLine("mdcMainGUI* gui = new mdcMainGUI(gClient->GetRoot(),1,1);");
	  break;
      case M_HLDBROWSE:
	fileDirDefine();
	new HldFileSearch(fClient->GetRoot(), this, 400, 200, DirFile, dirName);
	break;
      }
    }
  }
  return kTRUE;
}


void mdcMainGUI::closeWindow() { 
  delete this;
}


Bool_t mdcMainGUI::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2) {
  int retval;  //for class TGMsgBox
  int buttons; //used to construct message panel when Close button is clicked 
  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:
      switch(parm1) {	
      case M_ONLINE:
	retval = 0;
	buttons = 0;
	buttons |= kMBClose;
	new TGMsgBox(fClient->GetRoot(), this, "Warning message",
		     "Connection to DAQ not emplemented yet",
		     kMBIconQuestion,buttons,&retval);
	//	closeWindow();
	break;

      case M_HLDFILE:
	HldMsgBox *hldmsg;
	 hldmsg = new HldMsgBox(fClient->GetRoot(), 400, 200, secNum, modNum);
	closeWindow();
	break;
	
      case M_CLOSEMAIN:
	retval = 0;
	buttons = 0;
	buttons |= kMBYes;
	buttons |= kMBNo;
	new TGMsgBox(fClient->GetRoot(), this, "Confirm action",
		     "Close MDC Monitoring panel?",
		     kMBIconQuestion,buttons,&retval);
	if(retval == 1) closeWindow();
	break;
      default:
	break;      
      }
    }      
 
  case kCM_COMBOBOX:
    switch(parm1) {
    case M_SECTORMAIN:
      secNum = parm2;
     break;
    case M_MODULEMAIN:
      modNum = parm2;
     break;
    default:
      break;
    }
  }
  return kTRUE;
}

HldFileSearch::HldFileSearch(const TGWindow *p, const TGWindow *main,
                       UInt_t w, UInt_t h, Char_t *dirFile, Char_t *dirName,UInt_t options) :
  TGTransientFrame(p, main, w, h, options) {
  gSystem->ChangeDirectory(dirName);
  pHldFileSearchMain = new TGCompositeFrame(this, 400, 400, kVerticalFrame);
  AddFrame(pHldFileSearchMain, new TGLayoutHints(kLHintsNormal));

  pHldFileSearchBut = new TGCompositeFrame(this, 400, 100, kHorizontalFrame);
  pHldFileSearchMain->AddFrame(pHldFileSearchBut, new TGLayoutHints(kLHintsNormal,5,0,5,0));
  pHldFileSearchBrow = new TGCompositeFrame(this, 400, 100, kHorizontalFrame);
  pHldFileSearchMain->AddFrame(pHldFileSearchBrow, new TGLayoutHints(kLHintsNormal,5,0,5,0));
  //  pHldFileSearchStart = new TGCompositeFrame(this, 100, 20, kVerticalFrame);
  pHldFileSearchStart = new TGCompositeFrame(this, 400, 100, kHorizontalFrame);
  pHldFileSearchMain->AddFrame(pHldFileSearchStart, new TGLayoutHints(kLHintsNormal,130,0,5,0));

  pDir = new TGListBox(pHldFileSearchBut,-1);
  //  fDir->Associate(this);
  pDir->Resize(400,20);
  //  sprintf(tmp,"%s",gSystem->WorkingDirectory());
  pDir->AddEntry(dirFile,1);
  pHldFileSearchBut->AddFrame(pDir, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,0,5,0));

  pPcdup = fClient->GetPicture("tb_uplevel.xpm");
  pCdup = new TGPictureButton(pHldFileSearchBut,pPcdup,M_CDUP);
  pCdup->SetToolTipText("Up One Level");
  pCdup->Associate(this);
  pHldFileSearchBut->AddFrame(pCdup, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


  pPlist = fClient->GetPicture("tb_list.xpm");
  pListMode = new TGPictureButton(pHldFileSearchBut,pPlist,M_LIST_MODE);
  pListMode->SetToolTipText("List Mode");
  pListMode->Associate(this);
  pListMode->SetState(kButtonUp);
  pListMode->AllowStayDown(kTRUE);
  pHldFileSearchBut->AddFrame(pListMode, new TGLayoutHints(kLHintsLeft|kLHintsTop,5,5,5,5));


  pPdetail = fClient->GetPicture("tb_details.xpm");
  pDetailMode = new TGPictureButton(pHldFileSearchBut,pPdetail,M_DETAIL_MODE);
  pDetailMode->SetToolTipText("Details Mode");
  pDetailMode->Associate(this);
  pDetailMode->SetState(kButtonEngaged);
  pDetailMode->AllowStayDown(kTRUE);
  pHldFileSearchBut->AddFrame(pDetailMode, new TGLayoutHints(kLHintsLeft|kLHintsTop,
						5,5,5,5));

  pFileView = new TGListView(pHldFileSearchBrow,540,180);
  pFileCont = new TGFileContainer(pFileView->GetViewPort(),100,100,
				  kVerticalFrame,fgWhitePixel);
  pFileCont->Associate(this);
  pFileView->GetViewPort()->SetBackgroundColor(fgWhitePixel);
  pFileView->SetContainer(pFileCont);
  pFileCont->SetFilter("*");
  pFileCont->ChangeDirectory(".");
  pFileView->SetViewMode(kLVDetails);
  pFileCont->Sort(kSortByName);
  pHldFileSearchBrow->AddFrame(pFileView, new TGLayoutHints(kLHintsLeft|kLHintsTop,
                                               2,2,2,2));

  pOKButton = new TGTextButton(pHldFileSearchStart, "    &O'k    ", M_OK);
  pOKButton->Associate(this);
  pOKButton->SetToolTipText("O'k button");
  pHldFileSearchStart->
    AddFrame(pOKButton, new TGLayoutHints(kLHintsLeft | kLHintsExpandX,50,5,5,5));
  pOKButton->Resize(80, 22);

  pCloseButton = new TGTextButton(pHldFileSearchStart, "&Close", M_CL);
  pCloseButton->Associate(this);
  pCloseButton->SetToolTipText("Close Search ...");
  pHldFileSearchStart->
    AddFrame(pCloseButton,new TGLayoutHints(kLHintsLeft | kLHintsExpandX,50,5,5,5)); 
  pCloseButton->Resize(80, 22);

  MapSubwindows();
  //  Resize(400,300);
  Resize(GetDefaultSize());
  SetWindowName("Search for hld file");
  MapWindow();
  fClient->WaitFor(this);
}

HldFileSearch::~HldFileSearch() {
  delete pHldFileSearchMain;
}

void HldFileSearch::closeWindow() { 
  delete this;
}

Bool_t HldFileSearch::ProcessMessage(Long_t msg, Long_t parm1, Long_t) {

  int retval;  //for class TGMsgBox
  int buttons; //used to construct message panel when Close button is clicked 
  TGFileItem *f;
  void *p = NULL;

  switch(GET_MSG(msg)) {
  case kC_COMMAND:
    switch(GET_SUBMSG(msg)) {
    case kCM_BUTTON:    
      switch(parm1) {
      case M_CDUP:
      gSystem->ChangeDirectory("..");
      sprintf(tmp,"%s",gSystem->WorkingDirectory());
      pDir->RemoveEntry(1);
      pDir->AddEntry(tmp,1);
      pDir->MapSubwindows();
      pDir->Layout();
      pFileCont->ChangeDirectory(tmp);
      pFileCont->DisplayDirectory();
      break;
	
      case M_LIST_MODE:

	// "List" mode of file view

	pFileView->SetViewMode(kLVList);
	pDetailMode->SetState(kButtonUp);
	break;
	
      case M_DETAIL_MODE:
	
	// "Details" mode of file view
	
	pFileView->SetViewMode(kLVDetails);
	pListMode->SetState(kButtonUp);
	break;
      case M_CL:
      closeWindow();
      break;

      case M_OK:
 	if (pFileCont->NumSelected() == 1) {
	  f = (TGFileItem *) pFileCont->GetNextSelected(&p);
	  sprintf(tmp,"%s%s%s",gSystem->WorkingDirectory(),"/",
		  f->GetItemName()->GetString());
	  gHldMsgBox->HldMsgBox::pHldFileBuf->Clear();
	  gHldMsgBox->HldMsgBox::pHldFileBuf->AddText(0,tmp);
	  fClient->NeedRedraw(gHldMsgBox->HldMsgBox::pHldFileName);
	  sprintf(gHldMsgBox->HldMsgBox::dirName,"%s",gSystem->WorkingDirectory());
	  sprintf(gHldMsgBox->HldMsgBox::fileName,"%s",f->GetItemName()->GetString());
	  closeWindow();
	}
	else {
	  retval = 0;
	  buttons = 0;
	  buttons |= kMBOk;
	  new TGMsgBox(fClient->GetRoot(), this, "Confirm action",
		       "Select file before, pls...",
		       kMBIconQuestion,buttons,&retval);
	}
	break;
      default:
	break;

      }
    }
  case kC_CONTAINER:
    switch(GET_SUBMSG(msg)) {
      
    case kCT_ITEMDBLCLICK:

      if (parm1 == kButton1) {
	if (pFileCont->NumSelected() == 1) {
	  f = (TGFileItem *) pFileCont->GetNextSelected(&p);
	  if (S_ISDIR(f->GetType())) {
	    gSystem->ChangeDirectory(f->GetItemName()->GetString());
	    pFileCont->ChangeDirectory(f->GetItemName()->GetString());
	    pFileCont->DisplayDirectory();
 	    sprintf(tmp,"%s",gSystem->WorkingDirectory());
	    pDir->RemoveEntry(1);
	    pDir->AddEntry(tmp,1);
	    //	    fClient->NeedRedraw(pDir);    
	    pDir->MapSubwindows();
	    pDir->Layout();
	  } 
	  else {
	    pDir->Clear();
	    sprintf(tmp,"%s%s%s",gSystem->WorkingDirectory(),"/",
		    f->GetItemName()->GetString());
	    pDir->RemoveEntry(1);
	    pDir->AddEntry(tmp,1);
	    pDir->MapSubwindows();
	    pDir->Layout();
	    // Clowes windows after doublclick
	    /*	  sprintf(tmp,"%s%s%s",gSystem->WorkingDirectory(),"/",
		  f->GetItemName()->GetString());
		  gHldMsgBox->HldMsgBox::pHldFileBuf->Clear();
		  gHldMsgBox->HldMsgBox::pHldFileBuf->AddText(0,tmp);
		  fClient->NeedRedraw(gHldMsgBox->HldMsgBox::pHldFileName);
		  closeWindow();*/
	    // Perevod O'k knopki v aktivnoe polozhenie
	  }
	}
      }
    }
  }
  return kTRUE;
}
  
HldMsgBox *gHldMsgBox;
