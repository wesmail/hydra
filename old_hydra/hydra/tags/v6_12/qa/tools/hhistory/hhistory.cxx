// @(#)root/test:$Name: not supported by cvs2svn $:$Id: hhistory.cxx,v 1.1 2002-12-11 11:02:07 sadovski Exp $
// Author: Fons Rademakers   07/03/98
// Author: Modified by A.Sadovski FOR HADES-DST-QA

#include <stdlib.h>
#include "stdio.h"
#include "fstream.h"
#include "iostream.h"


#include <TROOT.h>
#include <TApplication.h>
#include <TVirtualX.h>

#include <TGListBox.h>
#include <TGClient.h>
#include <TGFrame.h>
#include <TGIcon.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGTextEntry.h>
#include <TGNumberEntry.h>
#include <TGMsgBox.h>
#include <TGMenu.h>
#include <TGCanvas.h>
#include <TGComboBox.h>
#include <TGTab.h>
#include <TGSlider.h>
#include <TGDoubleSlider.h>
#include <TGFileDialog.h>
#include <TGTextEdit.h>
#include <TGShutter.h>
#include <TGProgressBar.h>
#include <TGColorSelect.h>
#include <TRootEmbeddedCanvas.h>
#include <TCanvas.h>
#include <TH1.h>
#include <TH2.h>
#include <TRandom.h>
#include <TSystem.h>
#include <TEnv.h>
#include <TObjString.h>
#include <TFile.h>
#include <TKey.h>
#include <TRegexp.h>
#include <TText.h>
#include <TStyle.h>


#define iMaxHistogramNumber  500 //maximal number of histograms we can read from file
#define iMaxAnalFilesNumber  999 //maximal number of files to can be analised. May be better to use vector here...?

enum ETestCommandIdentifiers {
   M_FILE_OPEN,
   M_FILE_SAVE,
   M_FILE_SAVEAS,
   M_FILE_PRINT,
   M_FILE_PRINTSETUP,
   M_FILE_EXIT,

   M_TEST_DLG,
   M_TEST_MSGBOX,
   M_TEST_SLIDER,
   M_TEST_SHUTTER,
   M_TEST_PROGRESS,
   M_TEST_NUMBERENTRY,
   M_TEST_NEWMENU,

   M_HELP_CONTENTS,
   M_HELP_SEARCH,
   M_HELP_ABOUT,

   M_CASCADE_1,
   M_CASCADE_2,
   M_CASCADE_3,

   M_NEW_REMOVEMENU,

   VId1,
   HId1,
   VId2,
   HId2,

   VSId1,
   HSId1,
   VSId2,
   HSId2,

   ColorSel,

   FILE_LOAD_QaRootDir,
   FILE_LOAD_FileHistMap,
   FILE_SAVE_FileHistMap,
   FILE_LOAD_SUPPRESS_DIR_READ,
   SELECT_QA_ROOT_Histogram,
   SELECT_QA_ROOT_FileNumber,
   SELECT_QA_ROOT_isGoodHistogram,
   SELECT_QA_ROOT_isWarningHistogram,
   SELECT_QA_ROOT_isBadHistogram,
   SHOW_good_HistFiles,
   SHOW_warn_HistFiles,
   SHOW_bad_HistFiles,
   HIST_PLAYER_Play,
   HIST_PLAYER_Step_f,
   HIST_PLAYER_Pause,
   HIST_PLAYER_Step_b,
   HIST_PLAYER_Stop,
   HIST_PLAYER_DELAY_TIME_Slider
   
};

const char *mapfiletypes[] = { "File/Hist MAP files",    "*.map",
//t.k root gliuchit s refresh  "All files",              "*"    ,
                                0,                        0      };


  //class ListBoxMemoryHelper {
  //  private:
  //    Int_t id[iMaxAnalFilesNumber];
  //    Int_t iPos;
  //  public:
  //    ListBoxMemoryHelper(){
  //      iPos=0;
  //    }
  //   ~ListBoxMemoryHelper(){}
  //
  //    void AddLast(){
  //    }
  //};


  struct Stroka {
    Char_t st[92]; //tipically ROOTclass name
    Char_t tp[30]; //tipically ROOTclass type
  };

  class StArray {
    //Purpose: to keep an array of histogram names, which has to be alive
    //even after the destruction of objects themself

    private:
      static const Int_t maxLetters = 64;
      static const Int_t maxSize  = iMaxHistogramNumber; //maximal number of strings 
      Stroka data[maxSize];
      Int_t iPos;

    public:
       StArray(){
         Init();
       }

      ~StArray(){
       }

       void Init(){
         iPos = 0;         //initial position of array
       }

       void AddLast(Char_t *str, Char_t *typ){
          if(iPos<maxSize){
             //data[iPos].st = str;
             sprintf(data[iPos].st, "%s", str);
             sprintf(data[iPos].tp, "%s", typ);
             //version_1.00// << "StArray::str = " <<data[iPos].st << " type: " << data[iPos].tp << endl;
             iPos++;
          }else{
             //version_1.00// << "Array StArray is overflown!!!!"<< endl;
          }
       }

       Int_t GetSize(){
          return iPos;
       }
 
       Char_t* At(Int_t iP){
          if(iP>=0 && iP<iPos){
            return (Char_t*)data[iP].st;
          }else{
            cout << "Stroka:: out of range request!!!" << endl;
            return 0;
          }
       }

       Char_t* TypeAt(Int_t iP){
          if(iP>=0 && iP<iPos){
            return (Char_t*)data[iP].tp;
          }else{
            cout << "Stroka:: out of range request!!!" << endl;
            return 0;
          }
       }
  };

//-------------------------------------------------------------------

class MineField {
  private:
    //This is a inner map of good/warning/bad status of file-histogram
    //used to store and show user current status of histogram for any of
    //loaded files. In current realisation is limited by fixed array width
    //which has to be improved in future...?
    short int iFHfm[iMaxAnalFilesNumber][iMaxHistogramNumber];
    //Char_t *fMapFile;
    Char_t fMapFile[200];

  public:
    MineField(){ Init(); }
   ~MineField(){}

    void Init(){
      //fMapFile = "qaFileHisto.map";
      sprintf(fMapFile,"%s","qaFileHisto.map");
      for(   Int_t ix=0; ix<iMaxAnalFilesNumber; ix++){
         for(Int_t iy=0; iy<iMaxHistogramNumber; iy++){
            iFHfm[ix][iy]=0;
         }
      }
    }
   
    void SetMapFileName(Char_t *newMapFileName){
       //fMapFile = newMapFileName;
       sprintf(fMapFile,"%s", newMapFileName);
       printf("  ======== Important ========\n");
       printf("    Selecting  %s\n", fMapFile);
       printf("    to be MAP file for work\n");
       printf("  ===========================\n");
    }

    Int_t GetField(Int_t ix, Int_t iy){ 
      if(ix>=0 && ix<iMaxAnalFilesNumber){
         if(iy>=0 && iy<iMaxHistogramNumber){
           return (int) iFHfm[ix][iy]; 
         }else{
            printHistNumOverflow(iy);
         }
      }else{
         printFileNumOverflow(ix);
      }
      return -1; //error code, here it means overflow...
    }

    void SetField(Int_t ix, Int_t iy, short int iFval){ 
      if(ix>=0 && ix<iMaxAnalFilesNumber){
         if(iy>=0 && iy<iMaxHistogramNumber){
            iFHfm[ix][iy] = iFval; 
         }else{
            printHistNumOverflow(iy);
         }
      }else{
         printFileNumOverflow(ix);
      }
    }

    Bool_t writeFieldIntoFile(){
       Bool_t iOK;
       FILE *fieldFile;
       fieldFile = fopen( (Char_t*)fMapFile , "w");
       //version_1.00// << "Can open file?= "<<(int)fieldFile << endl;
       printf("  ======== Important ========\n");
       printf("    Saving  %s\n", fMapFile);
       printf("    to be MAP file for work\n");
       printf("  ===========================\n");
       if( (int)fieldFile != 0 ){
         //version_1.00// << "qaFileHisto.map Can be opened" << endl;
         fprintf(fieldFile,"%s","FORMAT:[colomns=fileNumber]:[raws=histogramNumber]\n");
         for(Int_t iy=0; iy<iMaxHistogramNumber; iy++){
         for(Int_t ix=0; ix<iMaxAnalFilesNumber; ix++){
            fprintf(fieldFile,"%i ", iFHfm[ix][iy]);
         }
         fprintf(fieldFile,"\n");
         }
         fclose(fieldFile);
         iOK=kTRUE;
       }else{
         printf("  ============================ Sorry =============================\n");
         printf("  qaFileHisto.map cannot be opened... May be it's write protected?\n");
         printf("  ---------------                                 ---------------\n");
         iOK=kFALSE;
       }
       //delete fieldFile;
       return iOK;
    }

    Bool_t readFieldFromFile(){
       Bool_t iOK;
       int iValue;
       Char_t TitleString[300];
       FILE *fieldFile;

       //version_1.00// << "Loadind map from" << fMapFile << endl;

       fieldFile = fopen( (Char_t*)fMapFile , "r");
       //version_1.00// << "Can open file?= "<<(int)fieldFile << endl;
       if( (int)fieldFile != 0 ){
         ////version_1.00// << fieldFile <<" Can be opened" << endl;
         //printf("%s Can be opened.",fieldFile);
         fscanf(fieldFile,"%s",TitleString); //here we do not need it
         //version_1.00// << "TitleString=" << TitleString << endl;
         for(Int_t iy=0; iy<iMaxHistogramNumber; iy++){
         for(Int_t ix=0; ix<iMaxAnalFilesNumber; ix++){
            fscanf(fieldFile,"%i", &iValue);
            //if(iy==0){if(ix<5){
            //  cout <<" iValue= "<<iValue << endl;
            //}}
            iFHfm[ix][iy] = iValue;
            //
            //if(iy==0){if(ix<5){cout <<" val= "<<iFHfm[ix][iy] << endl;}}
            //
         }
         //sprintf(fieldFile,"\n");
         }
         fclose(fieldFile);
         iOK=kTRUE;
       }else{
         printf("  =========================== Comment ============================\n");
         printf("  New project: %s does not exist or read protected...\n", fMapFile);
         printf("  ================================================================\n");
         iOK=kFALSE;
       }
       //delete fieldFile;
       return iOK;
    }


    void printFileNumOverflow(Int_t ix){
       printf("   *--- Significant Problem: ----------------------*\n");
       printf("   |-                                             -|\n");
       printf("   |- Impossible  to  adress  so  many  files     -|\n");
       printf("   |- due to insufficient of memory allocated     -|\n");
       printf("   |- for the FILE .vs. HISTOGRAMM   field...     -|\n");
       printf("   |- Please contact A.Sadovski@fz-rossendorf.de  -|\n");
       printf("   |- or correct the value of iMaxAnalFilesNumber -|\n");
       printf("   *-----------------------------------------------*\n");
       printf("    program assume:  iMaxAnalFilesNumber < %i\n", iMaxAnalFilesNumber);
       printf("    But You are trying to read %i files !\n", ix);
    }

    void printHistNumOverflow(Int_t iy){
       printf("   *--- Significant Problem: ----------------------*\n");
       printf("   |-                                             -|\n");
       printf("   |- Impossible to adress so many histograms     -|\n");
       printf("   |- due to insufficient of memory allocated     -|\n");
       printf("   |- for the FILE .vs. HISTOGRAMM   field...     -|\n");
       printf("   |- Please contact A.Sadovski@fz-rossendorf.de  -|\n");
       printf("   |- or correct the value of iMaxHistogramNumber -|\n");
       printf("   *-----------------------------------------------*\n");
       printf("    program assume:  iMaxHistogramNumber < %i\n", iMaxHistogramNumber);
       printf("    But You are trying to read %i histograms !\n", iy);
    }

};


//-------------------------------------------------------------------

class TestMainFrame : public TGMainFrame {

private:
   //TGCompositeFrame   *fStatusFrame;
   //asd2//TGCanvas           *fCanvasWindow;

   TGMenuBar          *fMenuBar;
   TGPopupMenu        *fMenuHelp;
   TGLayoutHints      *fMenuBarLayout, *fMenuBarItemLayout, *fMenuBarHelpLayout;

   //-asd--//
   TGLayoutHints *fLayout;
   TGTextButton  *fButtonFHgood, *fButtonFHbad, *fButtonFHwarn; //file-histogramm good, warning and bad marker button
   TGTextButton  *fButton1, *fButton2, *fButton3, *fButton4, *fButton5;
   TGButton      *fOkButton, *fCancelButton, *fStartB, *fStepfB, *fPauseB, *fStepbB, *fStopB;
   TGLabel       *fSliderLabel;
   TGLabel       *fHistosLabel, *frFilesLabel;
   TGHSlider     *fHslider1;

   //
   //TGCompositeFrame    *fContainer;
   TGCompositeFrame    *fFrame1, *fF1, *fF2, *fF3, *fF5, *fF6, *fF7;
   TGCompositeFrame    *fF4A; //, *fF4B;
   TGGroupFrame        *fF4, *fF4F;
   TGCheckButton       *fBtnShowGood, *fBtnShowWarn, *fBtnShowBad;

   TGLayoutHints       *fL1, *fL2, *fL3, *fL4, *fL4A;
   TGTab               *fTab;   
   TGTextEntry         *fTxt1, *fTxt2, *fTxt3;
   TGButton            *fBtn1, *fBtn2, *fChk1, *fChk2, *fRad1, *fRad2;
   TGComboBox          *fCombo;
   TGComboBox          *fFileCombo;
   TGCheckButton       *fCheckMulti;
   TGListBox           *fListBox;
   TGListBox           *fListBoxFile;

   TRootEmbeddedCanvas *fEc1, *fEc2, *fColoredPlatz;
   Int_t                fFirstEntry;  //for histogramm
   Int_t                fLastEntry;   //for histogramm
   Int_t                fFirstEntryFile; //for fileNumber
   Int_t                fLastEntryFile;  //for fileNumber
   Bool_t               fFillHistos;
   TH1F                *fHpx;
   TH2F                *fHpxpy;
   TList               *fCleanup;

   //Some date for ROOT applications
   Bool_t FileSuppresDirRead;
   Int_t iFL; //root file enumerator
   const Char_t* DST_HIST_ROOT_fileDir;
   const Char_t* DST_HIST_ROOT_fileSpecification;
   TList*  rootFileName;
   StArray histograName;

   Int_t  selectedHistogram_num;
   Char_t selectedHistogram[100];
   Char_t selectedHistogramType[100];
   Int_t iFileCurrPos;  //to remember the current position of the displayed file 
                        //-- if button continue will come to strart not from very 
                        //begining but from the current position

   Int_t fixedUpdateRate;         //should be fixed (constant) tipical rate
   Int_t tUpdateHistoDelayTime;   //rate adjustement

   MineField fhmf; //[iMaxAnalFilesNumber][iMaxHistogramNumber] good/warning/bad flag storage

   Bool_t showHF_g, showHF_w, showHF_b;
   ULong_t green, yellow, blue, red, gray;


public:
   TestMainFrame(const TGWindow *p, UInt_t w, UInt_t h);
   virtual ~TestMainFrame();

   virtual void CloseWindow();
   virtual Bool_t ProcessMessage(Long_t msg, Long_t parm1, Long_t);

   void   placeRootFileNamesIntoListBox();
   void replaceRootFileNamesIntoListBox();

   void PlaySelectedHistoAlongTheFileNumber(Int_t ind);
   void MarkAndCommentButton(Char_t* msg);
   void saveThisConfiguration();
   void indicateByColorLight();
};


TestMainFrame::TestMainFrame(const TGWindow *p, UInt_t w, UInt_t h)
      : TGMainFrame(p, w, h)
{

   //Working class variables initialisation
   FileSuppresDirRead = kFALSE;
   rootFileName = new TList();
   iFileCurrPos = 0;
   fixedUpdateRate = 40000; //fixed
   tUpdateHistoDelayTime = 100000;
   showHF_g=kTRUE; //
   showHF_w=kTRUE; // Initial settings of checkBox show all histograms
   showHF_b=kTRUE; //
   //End of Working class variables init...

   //Colors initialisation:
   fClient->GetColorByName("yellow", yellow);
   fClient->GetColorByName("green" , green);
   fClient->GetColorByName("red"   , red);
   fClient->GetColorByName("blue"  , blue);
   fClient->GetColorByName("gray"  , gray);


   //asd2//fCanvasWindow = new TGCanvas(this, 600, 240);
   //asd2//fCanvasWindow->Resize(200,100);
   //asd3//fContainer = new TGCompositeFrame(this, 60, 20, kHorizontalFrame | kSunkenFrame); 
   //asd2//fCanvasWindow->SetContainer(fContainer);

   // Used to store GUI elements that need to be deleted in the ctor.
   fCleanup = new TList;

   //// Create status frame containing a button and a text entry widget
   ////fStatusFrame = new TGCompositeFrame(this, 60, 20, kHorizontalFrame | kSunkenFrame);
   //fStatusFrame = new TGHorizontalFrame(this, 160, 20, kFixedWidth);

   //fOkButton = new TGTextButton(fStatusFrame, "&Ok", 1);
   //fOkButton->Associate(this);
   //fCancelButton = new TGTextButton(fStatusFrame, "&Cancel", 2);
   //fCancelButton->Associate(this);

   //fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 2, 2, 2, 2);
   //fL2 = new TGLayoutHints(kLHintsBottom | kLHintsRight, 2, 2, 5, 1);

   //fCanvasWindow->AddFrame(fOkButton, fL1);
   //fCanvasWindow->AddFrame(fCancelButton, fL1);

   //fCanvasWindow->Resize(150, fOkButton->GetDefaultHeight());
   //AddFrame(fCanvasWindow, fL2);

   //--------- create Tab widget and some composite frames for Tab testing

   fTab = new TGTab(this, 300, 300);
   fL3 = new TGLayoutHints(kLHintsTop | kLHintsLeft, 5, 5, 5, 5);

   TGCompositeFrame *tf;
   //?
   tf = new TGCompositeFrame(fClient->GetRoot(), 500, 300, kVerticalFrame);
   //?

   //###### Tab 1 ######//
   TGTextButton *bt;
   TGTextButton *btOpenFHfieldFile;
   TGTextButton *btSaveFHfieldFile;

   tf = fTab->AddTab("LoadDir...");
   fF1 = new TGCompositeFrame(tf, 60, 40, kVerticalFrame);
   fF1->AddFrame(fTxt1 = new TGTextEntry(fF1, new TGTextBuffer(300)), fL3);
   fF1->AddFrame(fTxt2 = new TGTextEntry(fF1, new TGTextBuffer(100)), fL3);
   fF1->AddFrame(bt = new TGTextButton(fF1, "&LOAD ROOT FILES", FILE_LOAD_QaRootDir), fL3);
   bt->Associate(this);

   fF1->AddFrame(fChk1 = new TGCheckButton(fF1, "Freeze the List of files", FILE_LOAD_SUPPRESS_DIR_READ), fL3);
   fChk1->Associate(this);

   fF1->AddFrame(fTxt3 = new TGTextEntry(fF1, new TGTextBuffer(100)), fL3);
   fF1->AddFrame(btOpenFHfieldFile = new TGTextButton(fF1, "LOAD &MAP FILE", FILE_LOAD_FileHistMap), fL3);
   btOpenFHfieldFile->Associate(this);

   fF1->AddFrame(btSaveFHfieldFile = new TGTextButton(fF1, "SAVE MAP FILE", FILE_SAVE_FileHistMap), fL3);
   btSaveFHfieldFile->Associate(this);

   Char_t fString[300];
   FILE *CFGfile;
   CFGfile = fopen( "qaSetup.cfg" , "r");
   Int_t iCanOpenFile = (int)CFGfile;
   ////version_1.00// << iCanOpenFile << endl;
   if( iCanOpenFile != 0 ){
     //version_1.00// << "qaSetup.cfg Can be opened" << endl;
     fscanf(CFGfile,"%s", fString);
     fTxt1->SetText(fString);
     fscanf(CFGfile,"%s", fString);
     fTxt2->SetText(fString);
     fscanf(CFGfile,"%s", fString);
     fTxt3->SetText(fString);
     fclose(CFGfile);
   }else{
     //version_1.00// << "qaSetup.cfg absent, we'll create one..." << endl;
   }

   tf->AddFrame(fF1, fL3);
   fTxt1->Resize(300, fTxt1->GetDefaultHeight());
   fTxt2->Resize(100, fTxt2->GetDefaultHeight());
   fTxt3->Resize(100, fTxt3->GetDefaultHeight());

   //######### TAB 4 #########//
   tf = fTab->AddTab("Hi/File");
   tf->SetLayoutManager(new TGHorizontalLayout(tf));

   //fF4B = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   //Char_t  buff_HistosTitle[100], buff_rFilesTitle[100];
   //sprintf(buff_HistosTitle,"%s" ,"  Histogram Names                                       ");
   //sprintf(buff_rFilesTitle,"%s" ,"  ROOT files List");
   //fHistosLabel = new TGLabel( fF4B, buff_HistosTitle );
   //frFilesLabel = new TGLabel( fF4B, buff_rFilesTitle );
   //fF4B->AddFrame(fHistosLabel, fL3 );
   //fF4B->AddFrame(frFilesLabel, fL3 );
   //tf->AddFrame(fF4B, fL3);   

   fF4 = new TGGroupFrame(tf, "Histograms", kVerticalFrame);
   //fF4 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   //fF4->AddFrame(bt = new TGTextButton(fF4, "&Bad", SELECT_QA_ROOT_isBadHistogram), fL3);
   //bt->Associate(this);
   //fCleanup->Add(bt);
   //// fF4->AddFrame(bt = new TGTextButton(fF4, "Remove &Entry", 91), fL3);
   //// bt->Associate(this);
   //// fCleanup->Add(bt);
   ////////////////////////////////////
   //Here we add new List of histograms
   //
   fF4->AddFrame(fListBox = new TGListBox(fF4, SELECT_QA_ROOT_Histogram), fL3);
   fListBox->Associate(this);
   //fF4->AddFrame(fCheckMulti = new TGCheckButton(fF4, "&Mutli Selectable", 92), fL3);
   //fCheckMulti->Associate(this);
   tf->AddFrame(fF4, fL3);
   fFirstEntry = 0;
   fLastEntry  = 0;
   ////------------------------------------------------------
   ////here we take all histograms from already created TList
   ////version_1.00// << "take all histograms afrom the TList" << endl;
   ////version_1.00// << "Nhistograms = " << histograName->GetSize() << endl;
   //for(Int_t i=0; i<histograName->GetSize(); i++){
   //   //version_1.00// << "histogramm TList: " << ((TObjString*) histograName->At(i))->GetString() << endl;
   //   fListBox->AddEntry(((TObjString*) histograName->At(i))->GetString(), i);
   //}
   //fFirstEntry = 0;
   //fLastEntry  = histograName->GetSize();
   ////version_1.00// << "end of taking and menu forming" << endl;
   ////end of checking if all histograms are settled in the TList
   ////-------------------------------------------------------
   fListBox->Resize(260, 420);


   fF4F = new TGGroupFrame(tf, "Files", kVerticalFrame);
   tf->AddFrame(fF4F, fL3);
   ////////////////////////////////////////////////////////////////////////////
   //Here we add list of root-files to allow posiibility to start from each one
   //
   fF4F->AddFrame(fListBoxFile = new TGListBox(fF4F, SELECT_QA_ROOT_FileNumber), fL3);
   //for (Int_t i=0; i < 20; ++i) {
   //   char tmp[20];
   //   sprintf(tmp, "FileNane_xx0009999000199%i.RoOt", i+1);
   //   fListBoxFile->AddEntry(tmp, i+1);
   //}
   fFirstEntryFile = 1;
   fLastEntryFile  = 0;
   fListBoxFile->Associate(this);
   fListBoxFile->Resize(260, 420);
   fF4F->AddFrame(fBtnShowGood = new TGCheckButton(fF4F, "Show good files for the histogram", SHOW_good_HistFiles), fL1);
   fF4F->AddFrame(fBtnShowWarn = new TGCheckButton(fF4F, "Show ???? files for the histogram", SHOW_warn_HistFiles), fL1);
   fF4F->AddFrame(fBtnShowBad  = new TGCheckButton(fF4F, "Show bad  files for the histogram", SHOW_bad_HistFiles ), fL1);
   fBtnShowGood->SetState( (EButtonState) 1);
   fBtnShowWarn->SetState( (EButtonState) 1);
   fBtnShowBad->SetState(  (EButtonState) 1);
   fBtnShowGood->Associate(this);   
   fBtnShowWarn->Associate(this);
   fBtnShowBad->Associate(this);
   
   //fF4A = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   //fF4A->AddFrame(fButtonFHbad = new TGTextButton(fF4A, "BadHst", SELECT_QA_ROOT_isBadHistogram), fL3);
   //fButtonFHbad->Associate(this);
   //fF4A->AddFrame(fButtonFHwarn = new TGTextButton(fF4A, "Warning", SELECT_QA_ROOT_isWarningHistogram), fL3);
   //fButtonFHwarn->Associate(this);
   //tf->AddFrame(fF4A, fL3);

   //####### end TAB 4 ########//



   //######### TAB 3 ###########//
   //-------------- embedded canvas demo
   fFillHistos = kFALSE;
   fHpx   = 0;
   fHpxpy = 0;
   tf = fTab->AddTab("HH-Player");
   fF3 = new TGCompositeFrame(tf, 60, 20, kHorizontalFrame);
   fStartB = new TGTextButton(fF3, "&Play", HIST_PLAYER_Play);
   fStepbB = new TGTextButton(fF3, "&<"   , HIST_PLAYER_Step_b);
   fPauseB = new TGTextButton(fF3, "&||"  , HIST_PLAYER_Pause);
   fStepfB = new TGTextButton(fF3, "&>"   , HIST_PLAYER_Step_f);
   fStopB  = new TGTextButton(fF3, "&Stop", HIST_PLAYER_Stop);
   fStartB->Associate(this);
   fStepbB->Associate(this);
   fPauseB->Associate(this);
   fStepfB->Associate(this);
   fStopB->Associate(this);
   fF3->AddFrame(fStartB, fL3);
   fF3->AddFrame(fStepbB, fL3);
   fF3->AddFrame(fPauseB, fL3);
   fF3->AddFrame(fStepfB, fL3);
   fF3->AddFrame(fStopB , fL3);


   //fFileCombo = new TGComboBox(fF3, 88);
   //fFileCombo->Associate(this);
   //fFileCombo->Resize(200, 20);
   //for (Int_t i = 0; i < 20; i++) {
   //   char tmp[20];
   //   sprintf(tmp, "FileNumber %i", i+1);
   //   fFileCombo->AddEntry(tmp, i+1);
   //}
   //fF3->AddFrame(fFileCombo, fL3);

   Char_t buff[30];
   sprintf(buff,"%s" ,"    Delay");
   fSliderLabel = new TGLabel( fF3, buff );
   fF3->AddFrame(fSliderLabel, fL3 );

   fHslider1 = new TGHSlider(fF3, 150, kSlider1 | kScaleBoth, HIST_PLAYER_DELAY_TIME_Slider);
   fHslider1->Associate(this);
   fHslider1->SetPosition(Int_t(tUpdateHistoDelayTime/fixedUpdateRate));
   fHslider1->SetRange(1,20);
   fF3->AddFrame(fHslider1,fL3);
   fF3->AddFrame(fButtonFHgood = new TGTextButton(fF3, "&Good", SELECT_QA_ROOT_isGoodHistogram)   , fL3);
   fF3->AddFrame(fButtonFHwarn = new TGTextButton(fF3, "&???" , SELECT_QA_ROOT_isWarningHistogram), fL3);
   fF3->AddFrame(fButtonFHbad  = new TGTextButton(fF3, "&Bad" , SELECT_QA_ROOT_isBadHistogram)    , fL3);
   fButtonFHgood->Associate(this);
   fButtonFHwarn->Associate(this);
   fButtonFHbad->Associate( this);
   // paint good button into green
   //ULong_t green;
   fClient->GetColorByName("lightgreen", green);
   fButtonFHgood->ChangeBackground(green);

   // paint warning button into yellow
   //ULong_t yellow;
   fClient->GetColorByName("Orange", yellow);
   fButtonFHwarn->ChangeBackground(yellow);

   // paint warning button into red
   //ULong_t red;
   fClient->GetColorByName("Magenta", red);
   fButtonFHbad->ChangeBackground(red);

   fColoredPlatz = new TRootEmbeddedCanvas("ColoredPlatz", fF3, 25, 25);
   fColoredPlatz->GetCanvas()->SetBorderMode(0);

   fF3->AddFrame(fColoredPlatz , fL3); //just will indicat the color
   fF3->AddFrame(fButtonFHgood , fL3);
   fF3->AddFrame(fButtonFHwarn , fL3);
   fF3->AddFrame(fButtonFHbad  , fL3);


   fF5 = new TGCompositeFrame(tf, 60, 60, kHorizontalFrame);

   fL4 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX |
                           kLHintsExpandY, 5, 5, 5, 5);
   fEc1 = new TRootEmbeddedCanvas("ec1", fF5, 500, 500);
   fF5->AddFrame(fEc1, fL4);
   //fEc2 = new TRootEmbeddedCanvas("ec2", fF5, 100, 100);
   //fF5->AddFrame(fEc2, fL4);

   tf->AddFrame(fF3, fL3);
   tf->AddFrame(fF5, fL4);

   fEc1->GetCanvas()->SetBorderMode(0);
   //fEc2->GetCanvas()->SetBorderMode(0);

   // make tab yellow
   //ULong_t yellow;
   fClient->GetColorByName("yellow", yellow);
   TGTabElement *tabel = fTab->GetTabTab(2);
   tabel->ChangeBackground(yellow);
   //-------------- end embedded canvas demo
   //####### end TAB 3 ##########//


   ////###### Tab 2 ######//
   //tf = fTab->AddTab("Tab2");
   //fL1 = new TGLayoutHints(kLHintsTop | kLHintsLeft | kLHintsExpandX, 200, 2, 2, 2);
   //fF2 = new TGCompositeFrame(tf, 60, 20, kVerticalFrame);
   //fF2->AddFrame(fBtn1 = new TGTextButton(fF2, "&Button 1", 0), fL1);
   //fF2->AddFrame(fBtn2 = new TGTextButton(fF2, "B&utton 2", 0), fL1);
   //fF2->AddFrame(fChk1 = new TGCheckButton(fF2, "C&heck 1", 0), fL1);
   //fF2->AddFrame(fChk2 = new TGCheckButton(fF2, "Chec&k 2", 0), fL1);
   //fF2->AddFrame(fRad1 = new TGRadioButton(fF2, "&Radio 1", 81), fL1);
   //fF2->AddFrame(fRad2 = new TGRadioButton(fF2, "R&adio 2", 82), fL1);
   //fCombo = new TGComboBox(fF2, 88);
   //fF2->AddFrame(fCombo, fL3);
   //tf->AddFrame(fF2, fL3);
   //int i;
   //for (i = 0; i < 20; i++) {
   //   char tmp[20];
   //
   //   sprintf(tmp, "Entry %i", i+1);
   //   fCombo->AddEntry(tmp, i+1);
   //}
   //fCombo->Resize(150, 20);
   //fBtn1->Associate(this);
   //fBtn2->Associate(this);
   //fChk1->Associate(this);
   //fChk2->Associate(this);
   //fRad1->Associate(this);
   //fRad2->Associate(this);
   ////######## end TAB 2 ########//
   //
   //
   //
   ////########### tab 5 ##########
   //tf = fTab->AddTab("Tab 5");
   //tf->SetLayoutManager(new TGHorizontalLayout(tf));
   //
   //fF6 = new TGGroupFrame(tf, "Options", kVerticalFrame);
   //tf->AddFrame(fF6, fL3);
   //
   //// 2 column, n rows
   //fF6->SetLayoutManager(new TGMatrixLayout(fF6, 0, 2, 10));
   //char buff[100];
   //int j;
   //for (j = 0; j < 4; j++) {
   //   sprintf(buff, "Module %i", j+1);
   //   fF6->AddFrame(new TGLabel(fF6, new TGHotString(buff)));
   //
   //   TGTextBuffer *tbuf = new TGTextBuffer(10);
   //   tbuf->AddText(0, "0.0");
   //
   //   TGTextEntry  *tent = new TGTextEntry(fF6, tbuf);
   //   tent->Resize(50, tent->GetDefaultHeight());
   //   tent->SetFont("-adobe-courier-bold-r-*-*-14-*-*-*-*-*-iso8859-1");
   //   fCleanup->Add(tent);
   //   fF6->AddFrame(tent);
   //}
   //fF6->Resize(fF6->GetDefaultSize());
   //
   //// another matrix with text and buttons
   //fF7 = new TGGroupFrame(tf, "Tab Handling", kVerticalFrame);
   //tf->AddFrame(fF7, fL3);
   //
   //fF7->SetLayoutManager(new TGMatrixLayout(fF7, 0, 1, 10));
   //
   //fF7->AddFrame(bt = new TGTextButton(fF7, "Remove Tab", 101));
   //bt->Associate(this);
   //bt->Resize(90, bt->GetDefaultHeight());
   //fCleanup->Add(bt);
   //
   //fF7->AddFrame(bt = new TGTextButton(fF7, "Add Tab", 103));
   //bt->Associate(this);
   //bt->Resize(90, bt->GetDefaultHeight());
   //fCleanup->Add(bt);
   //
   //fF7->AddFrame(bt = new TGTextButton(fF7, "Remove Tab 5", 102));
   //bt->Associate(this);
   //bt->Resize(90, bt->GetDefaultHeight());
   //fCleanup->Add(bt);
   //
   //fF7->Resize(fF6->GetDefaultSize());
   //
   ////--- end of last tab
   
   TGLayoutHints *fL5 = new TGLayoutHints(kLHintsBottom | kLHintsExpandX |
                                          kLHintsExpandY, 2, 2, 5, 1);
   AddFrame(fTab, fL5);

   MapSubwindows();
   Resize(GetDefaultSize());

   SetWindowName("DST QA-viewer");
   SetIconName("DST QA-viewer");

   //MapSubwindows();


   MapWindow();
}

TestMainFrame::~TestMainFrame()
{
   // Delete all created widgets.

   //delete fStatusFrame;
   //asd2//delete fCanvasWindow;

   delete fMenuBarLayout;
   delete fMenuBarItemLayout;
   delete fMenuBarHelpLayout;

   delete fMenuBar;
   delete fMenuHelp;
}

void TestMainFrame::CloseWindow()
{
   // Got close message for this MainFrame. Terminate the application
   // or returns from the TApplication event loop (depending on the
   // argument specified in TApplication::Run()).

   gApplication->Terminate(0);
}

Bool_t TestMainFrame::ProcessMessage(Long_t msg, Long_t parm1, Long_t parm2)
{
   // Handle messages send to the TestMainFrame object. E.g. all menu button
   // messages.

   switch (GET_MSG(msg)) {

      case kC_COMMAND:
         ////version_1.00// << "Command" << endl;

         switch (GET_SUBMSG(msg)) {

            case kCM_TAB:
              //printf("Button was pressed, id = %ld\n", parm1);
              if(parm1==2){
                ////we need to plot current histogram
                /////
                ////version_1.00// << "hist="<<selectedHistogram_num<<" F="<<iFileCurrPos <<endl;
                ////
                //// Taken from  HIST_PLAYER_Step_b button as it does identical business
                //fFillHistos = kTRUE;
                //PlaySelectedHistoAlongTheFileNumber(-1);
                //fFillHistos = kFALSE;
                ////
                ////
              }
              break;

            case kCM_BUTTON:
              //printf("Button was pressed, id = %ld\n", parm1);
                switch(parm1){

                  case FILE_LOAD_QaRootDir:
                    //version_1.00// << "FILE_LOAD_QaRootDir" << endl;
                    ////version_1.00// << fTxt1->GetText() << endl;
                    DST_HIST_ROOT_fileDir = fTxt1->GetText();
                    //version_1.00// << DST_HIST_ROOT_fileDir << endl;

                    ////version_1.00// << fTxt2->GetText() << endl;
                    DST_HIST_ROOT_fileSpecification = fTxt2->GetText();
                    //version_1.00// << DST_HIST_ROOT_fileSpecification << endl;

                    fhmf.SetMapFileName((Char_t*) fTxt3->GetText());

                    saveThisConfiguration();

                    //Before the opening the root files
                    //we cheeck if our TList is not empty, which is
                    //usually the case then user run this menu more
                    //then the first time
                    if(rootFileName->GetSize()>0){
                      //version_1.00// << "TList--rootFileName is not empty " << rootFileName->GetSize() << endl;
                      //---------------------------------------------
                      //Some checking that do we have...............
                      for(Int_t i=0; i<rootFileName->GetSize(); i++){
                         ////version_1.00// << "From Last session: "<<((TObjString*) rootFileName->At(i))->GetString() << endl;
                         delete (((TObjString*) rootFileName->At(i)));
                      }
                      ////version_1.00// << "now we have deleted them" << endl;
                      
                      //for(Int_t i=0; i<rootFileName->GetSize(); i++){
                      //   //version_1.00// << "After deletion: "<<((TObjString*) rootFileName->At(i))->GetString() << endl;
                      //}
                    
                    
                      rootFileName->Clear();
                      //version_1.00// << "TList--rootFileName after deletion: " << rootFileName->GetSize() << endl;
                    } //---------------------------------------------
                    
                   
                    //Now we will open the first root file from directory
                    //and try to get the list of histograms in it
                    Char_t  listToFileCommand[300];
                    sprintf(listToFileCommand,"ls -l %s/%s > files.txt",DST_HIST_ROOT_fileDir, DST_HIST_ROOT_fileSpecification );
                    if(!FileSuppresDirRead){
                      gSystem->Exec(listToFileCommand);
                    }
                    Char_t ffString[300];
                    iFL=0;
                    Int_t iFileSize;
                    FILE *fSC;
                    fSC = fopen( "files.txt" , "r");
                    while( fscanf(fSC,"%s", ffString) != EOF ){
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%s", ffString);
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%s", ffString);
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%s", ffString);
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%i", &iFileSize);
                       //version_1.00// << "\n" << iFileSize << endl;
                       fscanf(fSC,"%s", ffString);
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%s", ffString);
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%s", ffString);
                       ////version_1.00// << "\n" << ffString << endl;
                       fscanf(fSC,"%s", ffString);
                       //version_1.00// << "\n" << ffString << endl;

                       //Now we will work under this root file to get histogramm

                       if(iFileSize>100){
                         //Here we suppress empty files
                         TObjString *flnm = new TObjString(ffString);                       
                         //version_1.00// << "flnm=" << flnm->GetString() << endl;
                         rootFileName->AddLast( flnm );
                         iFL++;
                       }

                       ////version_1.00// << "DST-QA root file loaded: " << rootFileName[iFL] << endl;
                       //printf("DST-QA-root file: %s", rootFileName[iFL]);
                       //delete flnm;
                    }
                    {//---------------------------------------------
                      //Some checking that do we have...............
                      for(Int_t i=0; i<iFL; i++){
                         //version_1.00// << ((TObjString*) rootFileName->At(i))->GetString() << endl;
                      }
                      //version_1.00// << "--end-of-checking--TOTAL NUMBER is="<<iFL << endl;
                      //version_1.00// << "                    TOTAL SIZE is ="<<rootFileName->GetSize() << endl;
                    }//---------------------------------------------

                    //Here we open a first file from a list and
                    //read names of all histograms
                    {//---------------------------------------------
                      //Some checking that do we have...............
                      for(Int_t i=0; i<1/*iFL*/; i++){
                         //version_1.00// << "Attempt to get histogram names from file: " <<((TObjString*) rootFileName->At(i))->GetString() << endl;
                         TFile *f1=NULL;
                         const Char_t *ttFile = ((TObjString*) rootFileName->At(i))->GetString() ;
                         //version_1.00// << "ttFile=" << ttFile << endl;
                         f1 = new TFile( ttFile, "READONLY", "",1 );
                         f1->cd();

                         //////////////////////////////////////////////////////////////if (gDirectory) gDirectory->ls();
                         //------reading-fistograms-from-a-file------------------------------------------------

                         //here we check if TList for histograms is not empty
                         //version_1.00// << "number of hists in TList = " << histograName.GetSize() << endl;
                         if( histograName.GetSize() != 0){
                            //that tipically means we have already red histogram list
                            //and we skip this read for any next time as it is not needed
                            //version_1.00// << "!! Histogram list is already prepared !!" << endl;
                         }else{
                            //in case it was not filled before (not yet red)
                            //we will start of extraction procedure and read it
                         
                            //TCanvas c1;
                            TString reg = "*";
                            TRegexp re(reg, kTRUE);
                            TKey *key; 
                            TIter next(gDirectory->GetListOfKeys());
                            while ((key = (TKey *) next())) {
                               //TString s = key->GetName();
                               ////version_1.00// << "KeyName" << key->GetName() << endl;
                               // //if (s.Index(re) == kNPOS) continue;
                               //key->ls();                 //*-* Loop on all the keys
                               if( ((TObject*)key->ReadObj())->InheritsFrom("TH1") || ((TObject*)key->ReadObj())->InheritsFrom("TCanvas")){
                                   //((TH1*)key->ReadObj())->DrawCopy();
                                   //c1.Update();
                                   const Char_t * histName = ((TObject*)key->ReadObj())->GetName();
                                   //version_1.00// << "Accepted histogram= "<< histName << endl;
                                   //version_1.00// << "Accepted histogram= "<< (Char_t*)(((TObject*)key->ReadObj())->GetName()) << endl;
                         
                                   if(strlen(histName)){ //to suppress adding "zero" names
                                      //--now we put this histogram in the TList
                                      Char_t hType[30];
                                      if(((TObject*)key->ReadObj())->InheritsFrom("TH1")){
                                         sprintf(hType,"%s","TH1");
                                      }
                                      if(((TObject*)key->ReadObj())->InheritsFrom("TCanvas")){
                                         sprintf(hType,"%s","TCanvas");
                                      }

                                      //version_1.00// << "Selected object Type:"<< hType << endl;
                                      //histograName.AddLast( (Char_t*)((TObject*)key->ReadObj())->GetName() );
                                      histograName.AddLast( (Char_t*)histName, (Char_t*)hType );
                                      //--end of  putting histogram in the TList
                                   }
                         
                               }
                            }
                            //------------------------------------------------------
                            //here we check if all histograms are settled in the TList
                            //version_1.00// << "check if all histograms are settled in the TList" << endl;
                            //TCanvas c2;
                            for(Int_t i=0; i<histograName.GetSize(); i++){
                               ////version_1.00// << "histogramm TList: " << ((TObjString*) histograName->At(i))->GetString() << endl;
                               //version_1.00// << "histogramm TList: " << histograName.At(i) << " type: "<< histograName.TypeAt(i)  <<endl;
                               //((TH1*) histograName->At(i))->DrawCopy();
                               //c2.Update();
                            }
                            //c2.Close();
                            //version_1.00// << "end of cheecking" << endl;
                            //end of checking if all histograms are settled in the TList
                            //-------------------------------------------------------
                         
                         
                            //Here we fill histogram names into the LixtBox
                            //------------------------------------------------------
                            //here we take all histograms from already created TList
                            //version_1.00// << "ON BUTTON: take all histograms afrom the TList" << endl;
                            //version_1.00// << "ON BUTTON: Nhistograms = " << histograName.GetSize() << endl;
                            //fListBox->Clear();
                            //fListBox->RemoveEntries();
                            for(Int_t i=0; i<histograName.GetSize(); i++){
                               ////version_1.00// << "ON BUTTON:histogramm TList: " << ((TObjString*) histograName->At(i))->GetString() << endl;
                               //version_1.00// << "ON BUTTON:histogramm TList: " << histograName.At(i) << endl;
                               //fListBox->AddEntry(((TObjString*) histograName->At(i))->GetString(), i);
                               fLastEntry++;
                               fListBox->AddEntry( histograName.At(i), fLastEntry);
                            }
                            fFirstEntry = 1;
                            //version_1.00// << "end of taking and menu forming" << endl;
                            //-------------------------------------------------------
                         
                            //-//fListBox->RemoveEntry(fFirstEntry);
                            //-//fLastEntry++;
                            //+//sprintf(tmp, "Entry %i", fLastEntry);
                            //+//fListBox->AddEntry(tmp, fLastEntry);
                            //+//fListBox->MapSubwindows();
                            //+//fListBox->Layout();
                            fListBox->MapSubwindows();
                            fListBox->Layout();


                            placeRootFileNamesIntoListBox();

                         }
                         //------reading-fistograms-from-a-file------------------------------------------------

                      }
                    }//---------------------------------------------

                    break;

                  case FILE_LOAD_FileHistMap:
                    //{
                    //  static TString dir(".");
                    // TGFileInfo fi;
                    //  fi.fFileTypes = mapfiletypes;
                    //  fi.fIniDir    = StrDup(dir.Data());
                    //  printf("fIniDir = %s\n", fi.fIniDir);
                    //  new TGFileDialog(gClient->GetRoot(), fF1, kFDOpen, &fi);
                    //  printf("Open file: %s (dir: %s)\n", fi.fFilename, fi.fIniDir);
                    //  dir = fi.fIniDir;
                    //  ////version_1.00// << fi.fFilename << endl;
                    //  fhmf.SetMapFileName((Char_t*)fi.fFilename);
                    //}

                    fhmf.SetMapFileName((Char_t*) fTxt3->GetText());
                    saveThisConfiguration();
                    fhmf.readFieldFromFile();                    
                    break;

                  case FILE_SAVE_FileHistMap:
                    fhmf.SetMapFileName((Char_t*) fTxt3->GetText());
                    if(strlen(fTxt3->GetText())){
                      fhmf.writeFieldIntoFile();
                    }else{
                      //version_1.00// << "Can not write into empty file" << endl;
                      //version_1.00// << "Use qaFileHisto.map by default" << endl;
                      fhmf.SetMapFileName("qaFileHisto.map");
                      fhmf.writeFieldIntoFile();
                    }
                    break;


                  case SELECT_QA_ROOT_isGoodHistogram:
                    ////version_1.00// << "iFileCurrPos=" << iFileCurrPos << "selectedHistogram_num"<<selectedHistogram_num<<endl;
                    if(rootFileName->GetSize()>0){
                      ////version_1.00// << "Histogramm is good, please report!!! "<< endl;
                      fhmf.SetField(iFileCurrPos, selectedHistogram_num, 0);
                      indicateByColorLight();
                      MarkAndCommentButton("good:");
                    }
                    break;

                  case SELECT_QA_ROOT_isWarningHistogram:
                    ////version_1.00// << "Histogramm is warning, please report!!! "<< endl;
                    if(rootFileName->GetSize()>0){
                      fhmf.SetField(iFileCurrPos, selectedHistogram_num, 1);
                      indicateByColorLight();
                      MarkAndCommentButton("warn:");
                    }
                    break;

                  case SELECT_QA_ROOT_isBadHistogram:
                    ////version_1.00// << "Histogramm is bad, please report!!! "<< endl;
                    if(rootFileName->GetSize()>0){
                      fhmf.SetField(iFileCurrPos, selectedHistogram_num, 2);
                      indicateByColorLight();
                      MarkAndCommentButton("bad!:");
                    }
                    break;

                  case HIST_PLAYER_Play:
                    //version_1.00// << "PLAY button activated"<< endl;
                    fFillHistos = kTRUE;
                    PlaySelectedHistoAlongTheFileNumber(0);
                    break;

                  case HIST_PLAYER_Step_f:
                    //version_1.00// << "STEP FORWARD button activated"<< endl;
                    fFillHistos = kTRUE;
                    PlaySelectedHistoAlongTheFileNumber(1);
                    fFillHistos = kFALSE;
                    break;

                  case HIST_PLAYER_Pause:
                    //version_1.00// << "PAUSE button activated"<< endl;
                    fFillHistos = kFALSE;
                    break;

                  case HIST_PLAYER_Step_b:
                    //version_1.00// << "STEP BACKWARD button activated"<< endl;
                    fFillHistos = kTRUE;
                    PlaySelectedHistoAlongTheFileNumber(-1);
                    fFillHistos = kFALSE;
                    break;

                  case HIST_PLAYER_Stop:
                    fFillHistos = kFALSE;
                    iFileCurrPos = 0;
                    //version_1.00// << "STOP button activated"<< endl;
                    break;

                  default:
                    break;
                }
              break;

            case kCM_MENUSELECT:
               //printf("Pointer over menu entry, id=%ld\n", parm1);
               break;

            case kCM_CHECKBUTTON:
               switch (parm1){
                 case FILE_LOAD_SUPPRESS_DIR_READ:
                   //printf("Check button pressed, id=%ld id2=%ld\n", parm1, parm2);
                   if(FileSuppresDirRead == kFALSE){
                      FileSuppresDirRead=kTRUE;
                      //version_1.00// << "Selecting this ON you suppress rereading the directory" << endl;
                      //version_1.00// << "so what you can edit file \"files.txt\" and to delete" << endl;
                      //version_1.00// << "some undesirible strings (with file names)..." << endl;
                   }else{
                      FileSuppresDirRead=kFALSE;
                      //version_1.00// << "To allow rereading the QA-Directory" << endl;
                      //version_1.00// << "will regenerate the list of files" << endl;
                      //version_1.00// << "only if you push LOAD button afterwards." << endl;
                   }
                   break;

                 case SHOW_good_HistFiles:
                   if(showHF_g){showHF_g=kFALSE;
                   }else{       showHF_g=kTRUE;}
                   replaceRootFileNamesIntoListBox();
                   break;

                 case SHOW_warn_HistFiles:
                   if(showHF_w){showHF_w=kFALSE;
                   }else{       showHF_w=kTRUE;}
                   replaceRootFileNamesIntoListBox();
                   break;

                 case SHOW_bad_HistFiles:
                   if(showHF_b){showHF_b=kFALSE;
                   }else{       showHF_b=kTRUE;}
                   replaceRootFileNamesIntoListBox();
                   break;

               }
               break;

            case kCM_LISTBOX:
              //printf("TListBox pressed id = %ld iList = %ld\n", parm1, parm2);

              switch (parm1){
                 case SELECT_QA_ROOT_Histogram:
                   //version_1.00// << "The histogram selected: " << histograName.At(parm2-1) << endl;
                   selectedHistogram_num = parm2-1;
                   sprintf(selectedHistogram    ,"%s", histograName.At(    parm2-1) );
                   //version_1.00// << "The histogram selected: " << selectedHistogram << endl;
                   sprintf(selectedHistogramType,"%s", histograName.TypeAt(parm2-1) );
                   //version_1.00// << "The histogram selected type of: "<< selectedHistogramType << endl;

                   //if(iFileCurrPos==0){iFileCurrPos = 1;} //lets assume it if we just started

                   replaceRootFileNamesIntoListBox();

                   //we need to plot current histogram
                   //
                   //version_1.00// << "hist="<<selectedHistogram_num<<" F="<<iFileCurrPos <<endl;
                   //
                   // Taken from  HIST_PLAYER_Step_b button as it does identical business
                   fFillHistos = kTRUE;
                   iFileCurrPos++;
                   PlaySelectedHistoAlongTheFileNumber(-1);
                   fFillHistos = kFALSE;
                   //

                   break;

                 case SELECT_QA_ROOT_FileNumber:
                   //version_1.00// << "The current ROOT file Number is selected: "<<parm2 << endl;
                   iFileCurrPos = parm2;
                   //
                   // Taken from  HIST_PLAYER_Step_b button as it does identical business
                   fFillHistos = kTRUE;
                   PlaySelectedHistoAlongTheFileNumber(-1);
                   fFillHistos = kFALSE;
                   //
                   //
                   break;

                 default:
                   break;
              }

              break;
           
            case kCM_MENU:
               switch (parm1) {



                  case M_FILE_SAVE:
                     printf("M_FILE_SAVE\n");
                     break;

                  case M_FILE_PRINT:
                     printf("M_FILE_PRINT\n");
                     printf("Hiding itself, select \"Print Setup...\" to enable again\n");
                     break;

                  case M_FILE_PRINTSETUP:
                     printf("M_FILE_PRINTSETUP\n");
                     printf("Enabling \"Print\"\n");
                     break;

                  case M_FILE_EXIT:
                     CloseWindow();   // this also terminates theApp
                     break;

                  case M_TEST_MSGBOX:
                     //version_1.00// << "case M_TEST_MSGBOX" << endl;
                     break;

                  case M_TEST_SLIDER:
                     //version_1.00// << "case M_TEST_SLIDER" << endl;
                     break;

                  case M_TEST_NUMBERENTRY:
                     //version_1.00// << "case M_TEST_NUMBERENTRY" << endl;
                     break;

                  case M_NEW_REMOVEMENU:
                     {
                        fMenuBar->RemovePopup("New 1");
                        fMenuBar->RemovePopup("New 2");
                        fMenuBar->Layout();
                     }
                     break;

                  default:
                     break;
               }
            default:
               break;
         }

      case kC_HSLIDER:
         ////version_1.00// << "Slider" << endl;
         switch (GET_SUBMSG(msg)) {
            case kSL_POS:
              //printf("Slider update time tuning, id = %ld, %ld\n", parm1, parm2);
              tUpdateHistoDelayTime = fixedUpdateRate*parm2;
              break;

            default:
              break;
         }
         break;

      default:
         break;
   }
   return kTRUE;
}

void TestMainFrame::placeRootFileNamesIntoListBox(){
   //Here we fill file names into the LixtBox
   //------------------------------------------------------
   //version_1.00// << "Now RootfileNames are filled into ListBox" << endl;
   for(Int_t i=0; i<rootFileName->GetSize(); i++){
      fLastEntryFile++;
      //version_1.00// << "Add: "<< ((TObjString*) rootFileName->At(i))->GetString() << endl;
      TString theFileNameToListBox( ((TObjString*) rootFileName->At(i))->GetString()  );
      //theFileNameToListBox  = theFileNameToListBox( theFileNameToListBox.Last('/')+1,theFileNameToListBox.Last('.')-theFileNameToListBox.Last('/')-1 ); 
      theFileNameToListBox  = theFileNameToListBox( theFileNameToListBox.Last('/')+1,theFileNameToListBox.Length()-theFileNameToListBox.Last('/')-1 ); 

      //theFileNameToListBox = "[x]  "+theFileNameToListBox(0,theFileNameToListBox.Length());

      //version_1.00// << theFileNameToListBox << endl;
      //fListBoxFile->AddEntry( ((TObjString*) rootFileName->At(i))->GetString() , fLastEntryFile);
      fListBoxFile->AddEntry( theFileNameToListBox , fLastEntryFile);
   }
   fFirstEntryFile = 1;
   fListBoxFile->MapSubwindows();
   fListBoxFile->Layout();
   //-------------------------------------------------------
}


void TestMainFrame::replaceRootFileNamesIntoListBox(){
   //Here we fill file names into the LixtBox
   //------------------------------------------------------
   //version_1.00// << "Now RootfileNames are REfilled into ListBox" << endl;

   Bool_t showThis;
   fListBoxFile->RemoveEntries(0, rootFileName->GetSize());
   fLastEntryFile=0;
   for(Int_t i=0; i<rootFileName->GetSize(); i++){
      fLastEntryFile++;
      ////version_1.00// << "Add: "<< ((TObjString*) rootFileName->At(i))->GetString() << endl;
      TString theFileNameToListBox( ((TObjString*) rootFileName->At(i))->GetString()  );
      theFileNameToListBox  = theFileNameToListBox( theFileNameToListBox.Last('/')+1,theFileNameToListBox.Length()-theFileNameToListBox.Last('/')-1 ); 

      //theFileNameToListBox = "[x]  "+theFileNameToListBox(0,theFileNameToListBox.Length());

      showThis=kFALSE; //initially we do not show anything
      switch( fhmf.GetField(i,selectedHistogram_num) ){
         case 0:
           theFileNameToListBox = "[-ok-]  "+theFileNameToListBox(0,theFileNameToListBox.Length());
           if(showHF_g){
              showThis=kTRUE;
           }
           break;
         case 1:
           theFileNameToListBox = "[····?···]  "+theFileNameToListBox(0,theFileNameToListBox.Length());
           if(showHF_w){
              showThis=kTRUE;
           }
           break;
         case 2:
           theFileNameToListBox = "#####  "+theFileNameToListBox(0,theFileNameToListBox.Length());
           if(showHF_b){
              showThis=kTRUE;
           }
           break;
         default:
           theFileNameToListBox = "     "+theFileNameToListBox(0,theFileNameToListBox.Length());
           showThis=kTRUE;
           break;
      }


      ////version_1.00// << theFileNameToListBox << endl;
      if(showThis){
        fListBoxFile->AddEntry( theFileNameToListBox , fLastEntryFile);
      }else{
        fListBoxFile->AddEntry( "+" , fLastEntryFile);
      }

   }
   fFirstEntryFile = 1;
   fListBoxFile->Select(iFileCurrPos+1, kTRUE);
   fListBoxFile->MapSubwindows();
   fListBoxFile->Layout();
   //-------------------------------------------------------
}

void TestMainFrame::saveThisConfiguration(){
   //Using file as a temporary storage of used configuration
   FILE *CFGfile;
   CFGfile = fopen( "qaSetup.cfg" , "w");
   fprintf(CFGfile,"%s\n",fTxt1->GetText());
   fprintf(CFGfile,"%s\n",fTxt2->GetText());
   fprintf(CFGfile,"%s\n",fTxt3->GetText());
   fclose(CFGfile);
}

void TestMainFrame::PlaySelectedHistoAlongTheFileNumber(Int_t ind=0){
   ////version_1.00// << "<< PlaySelectedHistoAlongTheFileNumber() >>"<< endl;
   //const int kUPDATE = 10;
   static int cnt;
   cnt = 0;   


   TText txt;
   Int_t iStart_from = iFileCurrPos;
   Int_t iFinish_at  = rootFileName->GetSize();
   Int_t iPlusPlus=1;

   if(ind == 1 && iStart_from<rootFileName->GetSize()-1){
     //
     //if we have still some place to increment we allow it
     //
     iStart_from=iStart_from+1; iFinish_at=iStart_from+1; iPlusPlus= 1;
   }else{
     if(ind == 1){
       //here someone tries to look on the file after the last one
       //we better not allow him to do this
       return;
     }
   }

   if(ind ==-1 && iStart_from>0){
     //
     //if we still have some place to decrement we allow it
     //
     iStart_from=iStart_from-1; iFinish_at=iStart_from-1; iPlusPlus=-1;
   }else{
     if(ind == -1){
       //here one tries to go back, then we are in the 0 position
       //we do not allow this
       return;
     }
   }

   //version_1.00// << "iStart_from="<<iStart_from <<" iFinish_at="<<iFinish_at << " iPlusPlus="<<iPlusPlus<<endl;

   ////version_1.00// << endl;
   //Int_t i=iStart_from;
   ////version_1.00// <<"for(Int_t i=" << iStart_from << "; " << i*iPlusPlus << "<" << iFinish_at*iPlusPlus <<"; i=i+"<<iPlusPlus<<"){...}"<<endl;
   ////version_1.00// << endl;

   TCanvas *c1 = fEc1->GetCanvas();

   for(Int_t i=iStart_from; i*iPlusPlus<iFinish_at*iPlusPlus; i=i+iPlusPlus){
      //version_1.00// << "iStart_from="<<iStart_from <<" iFinish_at="<<iFinish_at << " iPlusPlus="<<iPlusPlus<<endl;
      if(!fFillHistos) break; //to stop while updating

      cnt++;

      TFile *f1=NULL;
      TString thisFile( ((TObjString*) rootFileName->At(i))->GetString() );
      f1 = new TFile( thisFile, "READONLY", "",1 );


      gStyle->SetPalette(1);
      TString strHistTyp(selectedHistogramType);
      if( strHistTyp.Contains("TH1") ){
         //version_1.00// << "InheritsFrom(TH1) :" <<selectedHistogram << endl;
         //gROOT->SetStyle("Plain");
         TH1    *h;
         c1->cd();
         h = (TH1*) f1->Get(selectedHistogram);
         h->GetXaxis()->SetTitleOffset(0.8);
         h->DrawCopy();
         thisFile  = thisFile( thisFile.Last('/')+1,thisFile.Last('.')-thisFile.Last('/')-1 ); 
         txt.DrawTextNDC( 0.2, 0.85, thisFile);
         c1->Update();
         iFileCurrPos = i;
         fListBoxFile->Select(i+1, kTRUE); //i+1 becouse I use back function to redraw the filename selected in a ListBoxFile
         //fListBoxFile->MapSubwindows();
         //fListBoxFile->Layout();

         indicateByColorLight();

         //version_1.00// << "file=" << i << endl;

         if(ind!=0){ 
            //that means non standart play but ">>" or "<<"
            //we want buttons update canvas twice faster
            for(Int_t it=0; it<tUpdateHistoDelayTime/2; it++){
              gSystem->ProcessEvents();  // handle GUI events
            }
         }else{
            //update canvas in a normal mode
            //for usual play operation
            for(Int_t it=0; it<tUpdateHistoDelayTime; it++){
              gSystem->ProcessEvents();  // handle GUI events
            }
         }
         //gSystem->Sleep(1500);
         delete h;
      }

      if( strHistTyp.Contains("TCanvas") ){
         TCanvas *tc;
         //version_1.00// << "InheritsFrom(TCanvas)" << endl;
         tc = (TCanvas*) f1->Get(selectedHistogram);
         //tc->DrawClone();
         //c1=tc;
         c1->Update();
         gSystem->ProcessEvents();  // handle GUI events
         //gSystem->Sleep(3000);
         delete tc;
      }

      f1->Close();
      delete f1;
   }

}


void TestMainFrame::indicateByColorLight(){
  Int_t color;
  switch(fhmf.GetField(iFileCurrPos,selectedHistogram_num)){
    case 0:
      color = 3;
      break;
    case 1:
      color = 5;
      break;
    case 2:
      color = 2;
      break;
    default:
      color = 11;
      break;
  }

  ////version_1.00// << "we change color="<< color << endl;
  //TText   tTransparentText;
  TCanvas *colCanvas = fColoredPlatz->GetCanvas();
  colCanvas->cd();
  colCanvas->SetFillColor( color );
  //tTransparentText.DrawTextNDC(0.5, 0.5 , ".");
  colCanvas->Modified();
  colCanvas->Update();
  //delete tTransparentText;
  //delete colCanvas;
  ////version_1.00// << "we change color4" << endl;
}

void TestMainFrame::MarkAndCommentButton(Char_t* msg){
   Char_t* ffString = 0;
   FILE *reportFile;
   reportFile = fopen( "qaReport.txt" , "append");
   Int_t iCanOpenFile = (int)reportFile;

   if(   iCanOpenFile != 0 ){
     //version_1.00// << "Report file can be opened" << endl;
     while( fscanf(reportFile,"%s", ffString) != EOF ){
       ////version_1.00// << "\n" << ffString << endl;
       fscanf(reportFile,"%s", ffString);
       ////version_1.00// << "\n" << ffString << endl;
     }
     TString thisBadFile( ((TObjString*) rootFileName->At(iFileCurrPos))->GetString() );
     thisBadFile = thisBadFile( thisBadFile.Last('/')+1,thisBadFile.Length()-thisBadFile.Last('/')-1 ); 
     ////version_1.00// << "bad: file= "<<thisBadFile<<"  hist= "<<selectedHistogram<<endl;

     Char_t strFile[200];
     //strFile = thisBadFile.Data();
     sprintf(strFile,"%s",thisBadFile.Data());
     fprintf(reportFile,"%s file=%s hist=%s\n" , msg, strFile ,selectedHistogram );

     fclose( reportFile);
   }else{
     //version_1.00// << "Strange: can not open report file..." << endl;
   }
}



//------------------------------------------------------------------------------

//---- Main program ------------------------------------------------------------

int main(int argc, char **argv)
{
   TApplication theApp("App", &argc, argv);

   if (gROOT->IsBatch()) {
      fprintf(stderr, "%s: cannot run in batch mode\n", argv[0]);
      return 1;
   }

   TestMainFrame mainWindow(gClient->GetRoot(), 400, 220);

   theApp.Run();

   return 0;
}
