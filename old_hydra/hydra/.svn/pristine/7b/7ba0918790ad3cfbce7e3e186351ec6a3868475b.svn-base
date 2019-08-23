// (hadascii.h)
// ****************************************************************************
//  Module: HAscii 
//  Files: hadascii.h, hadascii.cc - source, hadascii.html - documentation
//  Version: 0.35
//  Last changes: 20 August 2001
//  Author: Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//  ---------------------------------------------------------------------------
//  Description: set of utility classes which allow to read ASCII files
//  both in "key" format - similar to M$ Windows *.ini files and in "raw"
//  format which is columns of digits. Unlimited number of
//  files can be opened, data in a file divided in [sections] and specified
//  by keywords. Supported type of data: Int_t, Float_t, Char_t, string. 
//  Commentaries in any place of a file allowed. 
//  Writing to files possible. For details: see User's Guide.
//  ---------------------------------------------------------------------------
//  Destination: provided for internal usage by HADES collaboration software
//  developers / users. Alternative (to basic: Oracle database & ROOT files) 
//  way of reading of all kind of data for analysis, digitization, simulation 
//  packages (calibration data, detector geometry, program parameters etc.).
// ****************************************************************************

#ifndef HADASCII_H
#define HADASCII_H

#include "TObject.h"
#include "TArrayI.h"
#include "TArrayF.h"
#include "TArrayC.h"
#include "TError.h"

#include <fstream> 


namespace HadAsciiAuxiliaryFunctions {

// --------------------- string and Char_t operations ---------------------

 void ErrorMsg(Int_t status, const Char_t* name, Int_t arg, ...);
   // message printed in case of an warning or error, 
   // just before exception is thrown
 Char_t* strtrunc(Char_t* nameout, const Char_t* namein, Char_t x = ' ');
   // copies string namein truncating all characters x 
   // if x = ' ' (space) it truncates spaces and tabs
 Char_t* strreplace(Char_t* nameout, const Char_t* namein, Char_t x);
   // copies string namein replacing all characters x with ' '
 Bool_t strempty(const Char_t* namein, Char_t x = ' ');
 Int_t strnextchar(const Char_t* namein, Int_t pos = 0, Char_t x = ' ');
 Int_t strfirstchar(const Char_t* namein, Int_t startpos);
 Char_t strfirstchar(const Char_t* namein, Char_t x = ' ');
   // returns first Char_t of string namein ignoring characters x 
   // if x = ' ' (space) it returns simply first non white space character
 Char_t strlastchar(const Char_t* namein, Char_t x = ' ');
   // returns last Char_t of string namein ignoring characters x
   // if x = ' ' (space) it returns simply last non-whitespace character
 Char_t* strafter(Char_t* nameout, const Char_t* namein, Char_t x = ' ');
   // copies part of string namein after first appearance of Char_t x
 Char_t* strbefore(Char_t* nameout, const Char_t* namein, Char_t x = ' ');
   // copies part of string namein before first appearance of Char_t x
 void strskipwhite(Char_t* nameout, Char_t* namein, Char_t x);
   // skips white spaces after 'x' character occurence
 Int_t strunit(Char_t* nameout, Char_t* namein, Int_t startpos);
   // copies part of string namein until white space or ',' comma
   // truncating white spaces or commas before that part of string
   // and replaces all copied characters with ' ' space
   // It starts from startpos end returns endpos, which can
   // be used for the next call of this function on the same string.
 Char_t* strunit(Char_t* nameout, Char_t* namein);
   // copies part of string namein until white space or ',' comma
   // truncating white spaces or commas before that part of string 
   // and replaces all copied characters with ' ' space
 Char_t* strunit(Char_t* nameout, Char_t* namein, Char_t x);
   // copies part of string namein untill Char_t x
   // truncating whitespaces or commas before that part of string 
 Int_t strlength(const Char_t* namein);
   // returns length of string namein (excluding termination '\0' character)
 Char_t* strcopy(Char_t* nameout, const Char_t* namein);
   // copies string namein
 Char_t* strtolower(Char_t* nameout, const Char_t* namein);
   // copies string namein changing all characters to lowercase
 Int_t strcompare(const Char_t* nameout, const Char_t* namein);
   // returns 0 if strings namein and nameout are equal
   // 1 if nameout > namein and -1 if nameout < namein
 Char_t* strconcat(Char_t* nameout, const Char_t* namein);
   // concatenates string namein to end of nameout
 Char_t* strnconcat(Char_t* nameout, const Char_t* namein, Int_t begin, Int_t end);

}



// ***************************************************************************

 class HAsciiSectionRaw : public TObject {

// ***************************************************************************

  public:  

   HAsciiSectionRaw();
   virtual ~HAsciiSectionRaw();

   HAsciiSectionRaw(const HAsciiSectionRaw& source);
   HAsciiSectionRaw& operator=(const HAsciiSectionRaw& source);

   void Reset();

   Int_t fKeys; // - number of keywords identifying columns (number of columns)
   Int_t *fpKeyTypeId; // - array of type of data in given column (-1 - no type
                       // specified; 0 - Int_t; 1 - Float_t; 2 - Char_t; 3 - string)
   Char_t **fpKeyNames; // - array of (size fkeys) array of keywords names

   Int_t fElements; // - number of elements in one column
   Char_t **fpDataString; // - string of data (one column)
  
   Int_t **fpDataInt; // - array of (size fkeys) array of integers
   Float_t **fpDataFloat; // - array of (size fkeys) array of floats
   Char_t **fpDataChar; // - array of (size fkeys) array of chars
   Char_t ***fpDataStr; // - array of (size fkeys) array of pointers to Char_t
 
   Char_t *fpTempString; // - auxiliary variable

   friend std::ostream& operator<< (std::ostream& stream, HAsciiSectionRaw& section) {
    stream << "Number of keywords (columns): " << section.fKeys << std::endl;
    if (section.fKeys > 0) {
     stream << "Keywords parameters: \n";
     for (Int_t i = 0; i < section.fKeys; i++) {
      stream << "name: " << section.fpKeyNames[i] << " type: ";
      switch (section.fpKeyTypeId[i]) {
       case -1: stream << "not specified \n";
                break;
       case 0: stream << "Int_t \n";
               break;
       case 1: stream << "Float_t \n";
               break;
       case 2: stream << "Char_t \n";
               break;
       case 3: stream << "string (Char_t*) \n";
               break;
      }
     }
     stream << "Size (nr of elements in column): " << section.fElements << std::endl;
    }
    return stream;
   }

  ClassDef(HAsciiSectionRaw,0) // Used only in case of reading data in "raw" format.

 };

//============================================================================



// ***************************************************************************

 class HAsciiSection : public TObject {

// ***************************************************************************

 private:

  Short_t fSectionState;
    // - state of section: active (1) or inactive (0)
  Char_t fSectionName[128];
    // - section name - the contents of what is in [ ] brackets
  Long_t fSectionBegin;
    // - section begin position in bytes for given file
  Long_t fSectionEnd;
    // - section end position in bytes for given file
  Char_t *fpSectionComment;
    // - section commentaries 

 public:

  HAsciiSection();
  virtual ~HAsciiSection();

  HAsciiSection(const HAsciiSection& source);
  HAsciiSection& operator=(const HAsciiSection& source);

  HAsciiSectionRaw* fpRawData; // - activated while reading raw data

  Short_t GetSectionState();
  Char_t* GetSectionName();
  Long_t GetSectionBegin();
  Long_t GetSectionEnd();
  Char_t* GetSectionComment();
  HAsciiSectionRaw* GetSectionRaw();

  void SetSectionState(Short_t val);
  void SetSectionName(const Char_t* name);
  void SetSectionBegin(Long_t val);
  void SetSectionEnd(Long_t val);
  void SetSectionComment(const Char_t* name);
  void Reset();

  friend std::ostream& operator<< (std::ostream& stream, HAsciiSection& section) {
   stream << "Section: [" << section.GetSectionName() << "]  State: ";
   if (section.GetSectionState()) stream << " active.\n"; 
   else stream << " not active.\n";
   stream << "Begin: " << section.GetSectionBegin() << 
   " End: " << section.GetSectionEnd() << std::endl;
   stream << "Raw data: ";
   if (section.GetSectionRaw()) {
    stream << " present.\n";
    stream << section.GetSectionRaw() << std::endl;
   }
   else stream << " not present.\n";
   return stream;
  }

  ClassDef(HAsciiSection,0)

 }; 

//============================================================================



// ***************************************************************************

 class HAsciiSectionList : public TObject {

// ***************************************************************************

 public:
 // compiler fix (WP 06.2001)

  struct SectionNode {

   // Definition of node of HAsciiSectionList containing
   // information about section in object HAsciiSection.

   HAsciiSection fSection; // - section information
   SectionNode *fpNextNode; // - pointer to next node in section list
 
   SectionNode() : fSection(), fpNextNode(NULL) {}
   virtual ~SectionNode() {}
  };

 private:

  Int_t fLock; // - auxiliaty variable (see fpOpenNode remark)
  SectionNode **fpTemp; // - auxiliary variable (see fpOpenNode remark)
  SectionNode **fpOpenNode;
    // - variable(s) used for storing and restoring fpThisNode pointer
    // in methods operating on section list which may by called nested

  SectionNode *fpFirstNode; // - first node in section list
  SectionNode *fpLastNode; // - last node in section list
  SectionNode *fpThisNode; // - current node in section list

  SectionNode* PreviousNode(SectionNode* refnode) {
   // It returns pointer to node prior to referenced node.
   SectionNode *node;
   for(node = fpFirstNode; node->fpNextNode != refnode; node = node->fpNextNode);
   return node;
  }

  void InsertFirstNode(SectionNode* newnode);
  void InsertNode(SectionNode* newnode);
  void InsertLastNode(SectionNode* newnode);
  void RemoveFirstNode();
  void RemoveNode();
  void RemoveLastNode();

 public:

  HAsciiSectionList();
  HAsciiSectionList(SectionNode* init);
  virtual ~HAsciiSectionList();

  HAsciiSectionList(const HAsciiSectionList& source);
  HAsciiSectionList& operator=(const HAsciiSectionList& source);

  void InsertSection();
  void RemoveSection();

  HAsciiSection* ThisNodeSection();
  HAsciiSection* ToNodeSection(const Char_t* name = NULL);
  SectionNode* ThisNode() { return fpThisNode; }
  Int_t NodeNumber();
  void ToFirstNode();
  void ToLastNode();
  void AfterLastNode();
  SectionNode* ToNextNode() { 
   // It sets fpThisNode on next node.
   if (fpThisNode) return (fpThisNode = fpThisNode->fpNextNode);
   else return fpThisNode;
  }
  void ToNode(Int_t n);
  void StoreThisNode();
  void RestoreThisNode();
  void Reset();

  friend std::ostream& operator<< (std::ostream& stream, HAsciiSectionList& list) {
   HAsciiSectionList::SectionNode *node = list.fpFirstNode;
   stream << "Section list (nodes):\n";
   for (Int_t i = 0; node; i++, node = node->fpNextNode) {
    stream << i << ". " << node->fSection;
   }
   stream << std::endl;
   return stream;
  }

  ClassDef(HAsciiSectionList,0)

 };

//============================================================================



// ***************************************************************************

 class HAsciiFile : public TObject {

// ***************************************************************************

 private:

  Short_t fFileReadState;
    // - file status, now if file is opened it is set to 1
  Short_t fFileMode;
    // - mode in which file is opened, it is only 1 (read) here
  Short_t fFileChange;
    // - flag informing if list of sections was initialized 
    // if 1 list should be initialized before reading data
  Char_t fFileName[128]; // - file name
  Long_t fFileSize; // - file size
  Long_t fPosNow; // - indicator of current position while reading file
  Long_t fPosition; // - auxiliary storage variable

  Char_t* ReadSectionComment(Int_t begin, Int_t end);

 public:

  std::fstream fStreamFileRead; // - stream object handling file reading
  HAsciiSectionList fSectionList; // - list of sections for given file

  HAsciiFile();
  virtual ~HAsciiFile() {}

  HAsciiFile(const HAsciiFile& source);
  HAsciiFile& operator=(const HAsciiFile& source);

  Bool_t OpenFile(const Char_t* filename);
  Bool_t CloseFile();
  void InitSectionList();

  Short_t GetFileState();
  Short_t GetFileMode();
  Short_t GetFileChange();
  Char_t* GetFileName();
  Long_t GetFileSize();
  Long_t GetPosNow();
  Long_t GetPosition();

  void SetFileState(Short_t val);
  void SetFileMode(Short_t val);
  void SetFileChange(Short_t val);
  void SetFileName(const Char_t* nam);
  void SetFileSize();
  void SetPosNow(Long_t val);
  void SetPosition(Long_t val); 
  void Reset();

  friend std::ostream& operator<< (std::ostream& stream, HAsciiFile& file) {
   stream << "File: " << file.GetFileName() << 
             " Size: " << file.GetFileSize() << std::endl;
   return stream;
  }

  ClassDef(HAsciiFile,0)

}; 

//============================================================================



// ***************************************************************************

 class HAsciiFileList : public TObject {

// ***************************************************************************

 private:

  struct FileNode {

   // Definition of node of HAsciiFileList containing
   // information about section in object HAsciiFile.

   HAsciiFile file;
   FileNode *fpNextNode;

   FileNode() : fpNextNode(NULL) {}
   ~FileNode() {}
  };

  Int_t fLock; // - auxiliaty variable (see fpOpenNode remark)
  FileNode **fpTemp; // - auxiliary variable (see fpOpenNode remark)
  FileNode **fpOpenNode;
    // - variable(s) used for storing and restoring fpThisNode pointer
    // in methods operating on section list which may by called nested

  FileNode *fpFirstNode; // - first node in file list
  FileNode *fpLastNode; // - last node in file list
  FileNode *fpThisNode; // - current node in file list

  FileNode* PreviousNode(FileNode* refnode) {
   // Returning pointer to node before the node given as argument.
   FileNode *node;
   for(node = fpFirstNode; node->fpNextNode != refnode; node = node->fpNextNode);
   return node;
  }
  void InsertFirstNode(FileNode* newnode);
  void InsertNode(FileNode* newnode);
  void InsertLastNode(FileNode* newnode);
  void RemoveFirstNode();
  void RemoveNode();
  void RemoveLastNode();

 public:

  HAsciiFileList();
  virtual ~HAsciiFileList();

  HAsciiFileList(const HAsciiFileList& source);
  HAsciiFileList& operator=(const HAsciiFileList& source);

  void InsertFile();
  void RemoveFile();

  HAsciiFile* ThisNodeFile();
  HAsciiFile* ToNodeFile(const Char_t* name = NULL);
  FileNode* ThisNode() { return fpThisNode; }
  Int_t NodeNumber();
  void ToFirstNode();
  void ToLastNode();
  void AfterLastNode();
  FileNode* ToNextNode() { 
   if (fpThisNode) return (fpThisNode = fpThisNode->fpNextNode);
   else return fpThisNode;
  }
  void ToNode(Int_t n);
  void StoreThisNode();
  void RestoreThisNode();

  friend std::ostream& operator<< (std::ostream& stream, HAsciiFileList& list) {
   HAsciiFileList::FileNode *node = list.fpFirstNode;
   stream << "File list (nodes):\n";
   for (Int_t i = 0; node; i++, node = node->fpNextNode) {
    stream << i << ". " << node->file;
   }
   stream << std::endl;
   return stream;
  }

  ClassDef(HAsciiFileList,0)

}; 

//============================================================================



// ***************************************************************************

 class HAscii : public TObject {

// ***************************************************************************

 private:

  Long_t fReadAllPosition; // - position during reading sequentially all data

 protected:

  HAsciiFileList fFileList; // - list of files and every file has its list of sections

  Char_t fSeparator; // - character used as separator (space, tab or comma)
  Char_t fKeyLine[128]; // - keyword name
  Int_t fDataLineLength; // - length of string containing data 
  Char_t *fpDataLine; // - string with data as dynamically allocated array
  Char_t *fpCommentLine; // - string with commentary line
  Char_t *fpRemarkLine;
    // - auxiliary pointer for string with commentary line
    // used in case of reading commentary only and not keyword itself

  Int_t fDataCounter; // - number of data units of given type for keyword just read
  Int_t fDataBank; // - data bank id specifying type of data

  Int_t *fpDataInt; 
    // - dynamically created container for data of Int_t type
  Float_t *fpDataFloat;
    // - dynamically created container for data of Float_t type
  Char_t *fpDataChar;
    // - dynamically created container for data of Char_t type
  Char_t *fpDataString;
    // - dynamically created container for string (set of characters terminated by '\0')

  Char_t* CorrectName(Char_t* name);
  Char_t* CheckFileList(HAsciiFileList& list, Char_t* name);
  Bool_t CheckSection();
  Bool_t CheckData();
  Bool_t CheckDataType(Char_t typ);
  Bool_t CheckDataPosition(Int_t pos);
  Bool_t CheckUnambiguitySection();

  Char_t* ReadKeywordLine(const Char_t* name = NULL);
  Int_t* ConvertToInt();
  Float_t* ConvertToFloat();
  Char_t* ConvertToChar();
  Char_t* ConvertToString();
  HAsciiSection* CheckActiveSection();

  void ResetKeyData();
  void ResetBankData();

  HAsciiSection* fpActiveSection; // - auxiliary pointer to active section
  HAsciiFile* fpActiveFile; // - auxiliary pointer to file containing active section


  Int_t fFileWriteState; // - writing file status, if file opened it is 1, 2 or 3
  Char_t fFileWriteName[128]; // - file name opened in write mode

  Int_t fNumInLine; // - number of units (digits, characters) in line to be written

  void CloseOpenedFile();
  void Reset();

 public:

  Bool_t CheckOpenedFile();
  void WriteError();
  void SetNumInLine(Int_t num = 16);
  Int_t GetNumInLine();


 public:

  HAscii();
  virtual ~HAscii();

  HAscii(const HAscii& source);
  HAscii& operator=(const HAscii& source);

  std::fstream fStreamFileWrite; // - stream object handling file writing


  std::fstream& OpenFile(const Char_t* filename, const Char_t* mode = "read"); 
  void OpenFileList(const Char_t* filename); 
  void CloseFile(const Char_t* filename = NULL); 

  HAsciiSection* GetActiveSection();
  HAsciiSection* SetActiveSection(const Char_t* name);
  HAsciiFile* GetActiveFile();
  Char_t* GetActiveKeyword();
  Char_t* GetCommentKeyword();
  Bool_t IsReadFile();
  Bool_t IsWriteFile();
  Bool_t IsNextKeyword();

  void SetSeparator(Char_t character = ' ');
  Char_t GetSeparator();

  Char_t* ReadString();
  Char_t* ReadString(const Char_t* key);
  Char_t* ReadString(const Char_t* sec, const Char_t* key);

  Char_t* ReadRemark();

  Bool_t WriteLine(const Char_t* text, Int_t linelen = 76, Int_t keylen = 0);
  Bool_t WriteRemark(const Char_t* text, Char_t sign = ';', Int_t linelen = 76);
  Bool_t WriteSection(const Char_t* name);

  Bool_t CopySection(const Char_t* sec);

  ClassDef(HAscii,0)

}; 

//============================================================================



// ***************************************************************************

 class HAsciiKey : public HAscii {

// ***************************************************************************

  public:

  HAsciiKey();
  virtual ~HAsciiKey();

  HAsciiKey(const HAsciiKey& source);
  HAsciiKey& operator=(const HAsciiKey& source);

  Bool_t WriteKeyword(const Char_t* name);

  Bool_t WriteInt(Int_t num);
  Bool_t WriteInt(const Char_t* key, Int_t num);
  Bool_t WriteNInt(Int_t arg, ...);
  Bool_t WriteNInt(const Char_t* key, Int_t arg, ...);
  Bool_t WriteInt(Int_t arg, Int_t* num);
  Bool_t WriteInt(Int_t arg, Int_t begin, Int_t* num);
  Bool_t WriteInt(const Char_t* key, Int_t arg, Int_t* num);
  Bool_t WriteInt(const Char_t* key, Int_t arg, Int_t begin, Int_t* num);
  Bool_t WriteInt(TArrayI* data);
  Bool_t WriteInt(Int_t arg, TArrayI* data);
  Bool_t WriteInt(Int_t arg, Int_t begin, TArrayI* data);
  Bool_t WriteInt(const Char_t* key, TArrayI* data);
  Bool_t WriteInt(const Char_t* key, Int_t arg, TArrayI* data);
  Bool_t WriteInt(const Char_t* key, Int_t arg, Int_t begin, TArrayI* data);

  Bool_t WriteFloat(Float_t num);
  Bool_t WriteFloat(const Char_t* key, Float_t num);
  Bool_t WriteNFloat(Int_t arg, ...);
  Bool_t WriteNFloat(const Char_t* key, Int_t arg, ...);
  Bool_t WriteFloat(Int_t arg, Float_t* num);
  Bool_t WriteFloat(Int_t arg, Int_t begin, Float_t* num);
  Bool_t WriteFloat(const Char_t* key, Int_t arg, Float_t* num);
  Bool_t WriteFloat(const Char_t* key, Int_t arg, Int_t begin, Float_t* num);
  Bool_t WriteFloat(TArrayF* data);
  Bool_t WriteFloat(Int_t arg, TArrayF* data);
  Bool_t WriteFloat(Int_t arg, Int_t begin, TArrayF* data);
  Bool_t WriteFloat(const Char_t* key, TArrayF* data);
  Bool_t WriteFloat(const Char_t* key, Int_t arg, TArrayF* data);
  Bool_t WriteFloat(const Char_t* key, Int_t arg, Int_t begin, TArrayF* data);

  Bool_t WriteChar(Char_t num);
  Bool_t WriteChar(const Char_t* key, Char_t num);
  Bool_t WriteNChar(Int_t arg, ...);
  Bool_t WriteNChar(const Char_t* key, Int_t arg, ...);
  Bool_t WriteChar(Int_t arg, Char_t* num);
  Bool_t WriteChar(Int_t arg, Int_t begin, Char_t* num);
  Bool_t WriteChar(const Char_t* key, Int_t arg, Char_t* num);
  Bool_t WriteChar(const Char_t* key, Int_t arg, Int_t begin, Char_t* num);

  Bool_t WriteString(const Char_t* text, Int_t linelen = 76, Int_t keylen = 15);
  Bool_t WriteString(const Char_t* key, const Char_t* text, Int_t linelen = 76);

  Bool_t CopyKeyword(const Char_t* key);
  Bool_t CopyKeyword(const Char_t* sec, const Char_t* key);



  void* Read();
  void* Read(Char_t typ);
  void* Read(const Char_t* key, Char_t typ);
  void* Read(const Char_t* sec, const Char_t* key, Char_t typ);

  Int_t ReadInt(Int_t pos = 0);
  Int_t ReadInt(const Char_t* key, Int_t pos = 0);
  Int_t ReadInt(const Char_t* sec, const Char_t* key, Int_t pos = 0);

  Bool_t ReadInt(Int_t* putin, const Char_t* key);
  Bool_t ReadInt(Int_t* putin, const Char_t* sec, const Char_t* key);
  Bool_t ReadInt(Int_t* putin, const Char_t* key, Int_t number);
  Bool_t ReadInt(Int_t* putin, const Char_t* sec, const Char_t* key, Int_t number);
  Bool_t ReadInt(Int_t* putin, const Char_t* key, Int_t nrfrom, Int_t number);
  Bool_t ReadInt(Int_t* putin, const Char_t* sec, const Char_t* key,
                   Int_t nrfrom, Int_t number);

  Bool_t ReadInt(TArrayI* putin, const Char_t* key);
  Bool_t ReadInt(TArrayI* putin, const Char_t* sec, const Char_t* key);
  Bool_t ReadInt(TArrayI* putin, const Char_t* key, Int_t number);
  Bool_t ReadInt(TArrayI* putin, const Char_t* sec, const Char_t* key, Int_t number);
  Bool_t ReadInt(TArrayI* putin, const Char_t* key, Int_t nrfrom, Int_t number);
  Bool_t ReadInt(TArrayI* putin, const Char_t* sec, const Char_t* key,
                   Int_t nrfrom, Int_t number);


  Float_t ReadFloat(Int_t pos = 0);
  Float_t ReadFloat(const Char_t* key, Int_t pos = 0);
  Float_t ReadFloat(const Char_t* sec, const Char_t* key, Int_t pos = 0);

  Bool_t ReadFloat(Float_t* putin, const Char_t* key);
  Bool_t ReadFloat(Float_t* putin, const Char_t* sec, const Char_t* key);
  Bool_t ReadFloat(Float_t* putin, const Char_t* key, Int_t number);
  Bool_t ReadFloat(Float_t* putin, const Char_t* sec, const Char_t* key, Int_t number);
  Bool_t ReadFloat(Float_t* putin, const Char_t* key, Int_t nrfrom, Int_t number);
  Bool_t ReadFloat(Float_t* putin, const Char_t* sec, const Char_t* key,
                   Int_t nrfrom, Int_t number);

  Bool_t ReadFloat(TArrayF* putin, const Char_t* key);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* sec, const Char_t* key);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* key, Int_t number);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* sec, const Char_t* key, Int_t number);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* key, Int_t nrfrom, Int_t number);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* sec, const Char_t* key,
                     Int_t nrfrom, Int_t number);


  Char_t ReadChar(Int_t pos = 0);
  Char_t ReadChar(const Char_t* key, Int_t pos = 0);
  Char_t ReadChar(const Char_t* sec, const Char_t* key, Int_t pos = 0);


  Char_t* ReadRemark();
  Char_t* ReadRemark(const Char_t* key);
  Char_t* ReadRemark(const Char_t* sec, const Char_t* key);


  friend std::ostream& operator<< (std::ostream& stream, HAsciiKey& file) {
   using namespace HadAsciiAuxiliaryFunctions;
   stream << "HAsciiKey: \n" << file.fFileList;
   stream << "Active section: ";
   if (file.GetActiveSection()) {
    stream << file.GetActiveSection()->GetSectionName() << std::endl;
   } else {
    stream << "none " << std::endl;
   }
   stream << "Active keyword: ";
   if (strlength(file.GetActiveKeyword())) {
    stream << file.GetActiveKeyword() << std::endl;
   } else {
    stream << "none " << std::endl;
   }
   stream << "Commentaries to keyword: ";
   if (strlength(file.GetCommentKeyword())) {
    stream << file.GetCommentKeyword() << std::endl;
   } else {
    stream << "none \n" << std::endl;
   }
   return stream;
  }

  ClassDef(HAsciiKey,0)

}; 

//============================================================================


// ***************************************************************************

 class HAsciiRaw : public HAscii {

// ***************************************************************************

 private:

  Int_t fKeys; // - number of keywords

  Bool_t RecogniseKeys(const Char_t* name, Int_t num = 0);
  Bool_t ReadSectionRaw();
  void Reset();

  Bool_t fSection; // - logical variable, true after writing section
  Bool_t fKeywords; // - logical variable, true after writing keywords 
  Bool_t fData; // - logical variable, true after writing 1st and next lines of data

  Int_t fCounter; // - auxiliary variable, number of digitis written in given line

 public:

  Int_t *fpKeyTypeId; // - array of type of data in given column
  Char_t **fpKeyNames; // - array (of size fKeys) of array of keyword names

 public:

  HAsciiRaw();
  virtual ~HAsciiRaw();

  HAsciiRaw(const HAsciiRaw& source);
  HAsciiRaw& operator=(const HAsciiRaw& source);

  Int_t GetKeys();
  Int_t GetKeyTypeId(Int_t n = 0);
  Char_t* GetKeyName(Int_t n = 0);
  Long_t GetRowsNr();

  void SetReadKeyTypes(Char_t key1, ...);
  void SetWriteKeyTypes(Char_t key1, ...);
  void SetReadKeyType(Int_t col, Int_t type);
  void SetWriteKeyType(Int_t col, Int_t type);

  Bool_t GetKeyword() { return fKeywords; }
  Bool_t GetSection() { return fSection; }
  Bool_t GetData() { return fData; }
  Int_t GetCounter() { return fCounter; }
  void SetCounter(Int_t num) { fCounter = num; }
  void SetKeyword(Bool_t val) { fKeywords = val; }
  void SetSection(Bool_t val) { fSection = val; }
  void SetData(Bool_t val) { fData = val; }
  void SetKeyNr(Int_t num) { fKeys = num; }


  HAsciiSection* SetActiveSection(const Char_t* name);
  Bool_t WriteSection(const Char_t* name);

  Bool_t WriteKeywords(const Char_t* name);
  Bool_t WriteKeywords(Int_t arg, ...);
  Bool_t WriteKeywords(const Char_t* sec, Int_t arg, ...);

  Bool_t WriteRawLine(Int_t first, ...);
  Bool_t WriteRawLine(Float_t first, ...);
  Bool_t WriteRawLine(Char_t first, ...);
  Bool_t WriteRawLine(const Char_t* first, ...);

  Bool_t WriteInt(Int_t num);
  Bool_t WriteFloat(Float_t num);
  Bool_t WriteChar(Char_t num);
  Bool_t WriteString(const Char_t* num);

  Bool_t ReadRawLine(Int_t row, ...);

  Int_t ReadInt(Int_t col, Int_t raw);
  Int_t ReadInt(const Char_t* keyname, Int_t raw);

  Int_t ReadInt(Int_t colcond, Int_t rawvalue, Int_t col);
  Int_t ReadInt(Int_t colcond, Int_t rawvalue, const Char_t* key);
  Int_t ReadInt(const Char_t* keycond, Int_t rawvalue, Int_t col);
  Int_t ReadInt(const Char_t* keycond, Int_t rawvalue, const Char_t* key);

  Bool_t ReadInt(Int_t* putin, Int_t col, Int_t number = 0);
  Bool_t ReadInt(Int_t* putin, const Char_t* keyname, Int_t number = 0);
  Bool_t ReadInt(Int_t* putin, Int_t col, Int_t begin, Int_t number);
  Bool_t ReadInt(Int_t* putin, const Char_t* keyname, Int_t begin, Int_t number);

  Bool_t ReadInt(TArrayI* putin, Int_t col, Int_t number = 0);
  Bool_t ReadInt(TArrayI* putin, const Char_t* keyname, Int_t number = 0);
  Bool_t ReadInt(TArrayI* putin, Int_t col, Int_t begin, Int_t number);
  Bool_t ReadInt(TArrayI* putin, const Char_t* keyname, Int_t begin, Int_t number);


  Float_t ReadFloat(Int_t col, Int_t raw);
  Float_t ReadFloat(const Char_t* keyname, Int_t raw);

  Float_t ReadFloat(Int_t colcond, Int_t rawvalue, Int_t col);
  Float_t ReadFloat(Int_t colcond, Int_t rawvalue, const Char_t* key);
  Float_t ReadFloat(const Char_t* keycond, Int_t rawvalue, Int_t col);
  Float_t ReadFloat(const Char_t* keycond, Int_t rawvalue, const Char_t* key);

  Bool_t ReadFloat(Float_t* putin, Int_t col, Int_t number = 0);
  Bool_t ReadFloat(Float_t* putin, const Char_t* keyname, Int_t number = 0);
  Bool_t ReadFloat(Float_t* putin, Int_t col, Int_t begin, Int_t number);
  Bool_t ReadFloat(Float_t* putin, const Char_t* keyname, Int_t begin, Int_t number);
  
  Bool_t ReadFloat(TArrayF* putin, Int_t col, Int_t number = 0);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* keyname, Int_t number = 0);
  Bool_t ReadFloat(TArrayF* putin, Int_t col, Int_t begin, Int_t number);
  Bool_t ReadFloat(TArrayF* putin, const Char_t* keyname, Int_t begin, Int_t number);


  Char_t ReadChar(Int_t col, Int_t raw);
  Char_t ReadChar(const Char_t* keyname, Int_t raw);

  Char_t ReadChar(Int_t colcond, Int_t rawvalue, Int_t col);
  Char_t ReadChar(Int_t colcond, Int_t rawvalue, const Char_t* key);
  // Char_t ReadChar(const Char_t* keycond, Int_t rawvalue, Int_t col);
  // this no more available because of confusion with the 2. function below
  Char_t ReadChar(const Char_t* keycond, Int_t rawvalue, const Char_t* key);

  Bool_t ReadChar(Char_t* putin, Int_t col, Int_t number = 0);
  Bool_t ReadChar(Char_t* putin, const Char_t* keyname, Int_t number = 0);
  Bool_t ReadChar(Char_t* putin, Int_t col, Int_t begin, Int_t number);
  Bool_t ReadChar(Char_t* putin, const Char_t* keyname, Int_t begin, Int_t number);

  Bool_t ReadChar(TArrayC* putin, Int_t col, Int_t number = 0);
  Bool_t ReadChar(TArrayC* putin, const Char_t* keyname, Int_t number = 0);
  Bool_t ReadChar(TArrayC* putin, Int_t col, Int_t begin, Int_t number);
  Bool_t ReadChar(TArrayC* putin, const Char_t* keyname, Int_t begin, Int_t number);


  Char_t* ReadString(Int_t col, Int_t row);
  Char_t* ReadString(const Char_t* keyname, Int_t row);

  Char_t* ReadString(Int_t colcond, Int_t rowvalue, Int_t col);
  Char_t* ReadString(Int_t colcond, Int_t rowvalue, const Char_t* key);
  Char_t* ReadString(const Char_t* keycond, Int_t rowvalue, Int_t col);
  Char_t* ReadString(const Char_t* keycond, Int_t rowvalue, const Char_t* key);
  

  ClassDef(HAsciiRaw,0)

};

//============================================================================



#endif //  HADASCII_H
