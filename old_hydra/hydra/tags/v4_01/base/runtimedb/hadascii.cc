// (hadascii.cc)
// ****************************************************************************
//  Module: HAscii
//  Version: 0.32
// ****************************************************************************


#include <TDatime.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream.h>
#include <fstream.h>
#include <string.h>
#include "hadascii.h"




// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//
// StringFunctions namespace definitions
//


//----------------------------------------------------------------------------
namespace StringFunctions {

// Additional components of StringFunctions namespace: 
// exit in case of error. They are in .cc file because of root
// dictionary including headers. 

 Int_t fiExit;
   // error flag, if (default) value is 1 program stops if error happens
}
//============================================================================

//----------------------------------------------------------------------------
Int_t StringFunctions::GetExit() {

// The function calling exit from program in case of any critical
// situation. 

 if (fiExit) {
  cout << "Program exited.\n";
  exit(1);
 } 
return fiExit;
}
//============================================================================

//----------------------------------------------------------------------------
void StringFunctions::SetExit(Int_t state) {

// It sets the behavior of program in case of critical errors.
// By default (1) program exits. 0 as argument means to force program to go on.

 if (state) {
  fiExit = 1;
 } else {
  fiExit = 0;
 }
}
//============================================================================

// --------------------- string and char operations ---------------------

//----------------------------------------------------------------------------
Char_t* StringFunctions::strtrunc(Char_t* nameout, const Char_t* namein, Char_t x) {

// It copies string namein to nameout but truncating all x characters. If x not specified
// by default all white spaces (spaces and tabulations) will be omitted.

 if (namein == NULL) return nameout;
 Int_t i = -1, j = 0;
 do {
  i++;
  if (x == ' ') {
   if (namein[i] != ' ' && namein[i] != '\t') nameout[j++] = namein[i];
  } else {
   if (namein[i] != x) nameout[j++] = namein[i];
  }
 } while (namein[i]);
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t StringFunctions::strempty(const Char_t* namein, Char_t x) {

// It returns true if string namein has not any other character
// besides x (and if x is omitted it means white space).

 if (namein == NULL) return kTRUE;
 Int_t i = -1;
 do {
  i++;
  if (x == ' ') {
   if (namein[i] && namein[i] != ' ' && namein[i] != '\t') return kFALSE;
  } else {
   if (namein[i] && namein[i] != x) return kFALSE;
  }
 } while (namein[i]);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t StringFunctions::strnextchar(const Char_t* namein, Int_t pos, Char_t x) {

// It returns position of next appearance of character x in string namein
// starting counting from position pos.

 if (namein == NULL) return 0;
 if (pos > strlength(namein)) {
  cerr << "###### HadAscii ###### Error in <StringFunctions::strnextchar> ######\n"
       << "# Input string length shorter than position argument!               #\n"
       << "#####################################################################\n";
  GetExit();
  return 0;
 }
 Int_t i = pos + 1;
 if (x == ' ') {
  while (namein[i] && (namein[i] != ' ' && namein[i] != '\t' && namein[i] != ',')) { i++; };
 } else {
  while (namein[i] && namein[i] != x) { i++; };
 }
return (i - pos); 
}
//============================================================================

//----------------------------------------------------------------------------
Char_t StringFunctions::strfirstchar(const Char_t* namein, Char_t x) {

// It returns first character in string namein which is different from
// character x (and if x is not specified all white spaces at the beginning
// are omitted).

 if (namein == NULL) return '\0';
 Int_t i = 0;
 if (x == ' ') {
  while (namein[i] && (namein[i] == ' ' || namein[i] == '\t')) { i++; };
 } else {
  while (namein[i] && namein[i] == x) { i++; };
 }
return namein[i];
}
//============================================================================

//----------------------------------------------------------------------------
Char_t StringFunctions::strlastchar(const Char_t* namein, Char_t x) {

// It returns last character in string namein which is different from
// character x (and if x is not specified all white spaces counting from
// the end of the string are omitted).

 if (namein == NULL) return '\0';
 Int_t i = strlen(namein) - 1;
 if (x == ' ') {
  while (namein[i] && (namein[i] == ' ' || namein[i] == '\t')) { i--; }
 } else {
  while (namein[i] && namein[i] == x) { i--; }
 }
return namein[i];
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strafter(Char_t* nameout, const Char_t* namein, Char_t x) {

// It copies substring of namein to nameout omitting all characters x
// if there are any at the beginning (especially i.e. white spaces).

 if (namein == NULL) return nameout;
 Int_t i = 0, j = 0;
 while (namein[i] && namein[i] != x) i++; 
 if (namein[i] && namein[i] == x) i++;
 while ((nameout[j++] = namein[i++]));
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strbefore(Char_t* nameout, const Char_t* namein, Char_t x) {

// It copies substring of namein to nameout untill the first appearance of character x.

 if (namein == NULL) return nameout;
 Int_t i = 0;
 while (namein[i] && namein[i] != x) {
  nameout[i] = namein[i];
  i++;
 }
 nameout[i] = '\0';
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strunit(Char_t* nameout, Char_t* namein) {

// According to data format it copies substring of namein to nameout
// untill it finds delimiter i.e. white space or comma. All white spaces
// at the beginning are skipped and substring copied is replaced in
// nameout by spaces - it means that function destroys namein.

 if (namein == NULL) return nameout;
 Int_t i = 0, j = 0;
 while (namein[i] && (namein[i] == ' ' || 
        namein[i] == ',' || namein[i] == '\t')) namein[i++] = ' ';
 while (namein[i] && namein[i] != ' ' && 
        namein[i] != ',' && namein[i] != '\t') {
  nameout[j++] = namein[i++];
  namein[i-1] = ' ';
 }
 if (namein[i] == ',' || namein[i] == '\t') namein[i] = ' ';
 nameout[j] = '\0';
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strunit(Char_t* nameout, Char_t* namein, Char_t x) {

// It copies substring of namein to nameout untill it finds character x.
// All white spaces at the beginning are skipped and substring copied is replaced
// in nameout by spaces - it means that function destroys namein.

 if (namein == NULL) return nameout;
 Int_t i = 0, j = 0;
 while (namein[i] && (namein[i] == ' ' || 
        namein[i] == '\t')) namein[i++] = ' ';
 while (namein[i] && namein[i] != x) {
  nameout[j++] = namein[i];
  namein[i++] = ' ';
 }
 if (namein[i] == x) namein[i] = ' ';
 nameout[j] = '\0';
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t StringFunctions::strlength(const Char_t* namein) {

// String length calculation (termination '\0' character is not
// taken into account.

 if (namein == NULL) return 0;
 if (!strchr(namein,'\0')) {
  cerr << "###### HadAscii ###### Error in <StringFunctions::strlength> ######\n"
       << "# Argument is not a string terminater by \'\\0\'!                    #\n"
       << "###################################################################\n";
  GetExit();
  return 0;
 }
return (Int_t)(strlen(namein));
} 
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strcopy(Char_t* nameout, const Char_t* namein) {

// Copying string namein to nameout.

 if (namein == NULL) return nameout;
 if (!strchr(namein,'\0')) {
  cerr << "###### HadAscii ###### Error in <StringFunctions::strcopy> ######\n"
       << "# Argument is not a string terminater by \'\\0\'!                  #\n"
       << "#################################################################\n";
  GetExit();
  return nameout;
 }
return strcpy(nameout,namein);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strtolower(Char_t* nameout, const Char_t* namein) {

// Copying namein to nameout and changing all characters to low.

 if (namein == NULL) return nameout;
 Int_t i = 0;
 if (!strchr(namein,'\0')) {
  cerr << "###### HadAscii ###### Error in <StringFunctions::strtolower> ######\n"
       << "# Argument is not a string terminater by \'\\0\'!                     #\n"
       << "####################################################################\n";
  GetExit();
  return nameout;
 }
 while (namein[i]) {
  if (namein[i] >= 65 && namein[i] <= 90) nameout[i] = namein[i] + 32;
   else nameout[i] = namein[i];
  i++;
 }
 nameout[i] = '\0';
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t StringFunctions::strcompare(const Char_t* nameout, const Char_t* namein) {

// Comparison of strings nameout and namein. If strings are the same 0 is returned.

 if (nameout && namein==NULL) return 1;
  else if (nameout==NULL && namein) return -1;
   else if (nameout==NULL && namein==NULL) return 0;
return strcmp(nameout,namein);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strconcat(Char_t* nameout, const Char_t* namein) {

// String namein concatenated to nameout.

 if (namein == NULL) return nameout;
 if (!strchr(namein,'\0')) {
  cerr << "###### HadAscii ###### Error in <StringFunctions::strconcat> ######\n"
       << "# Argument is not a string terminater by \'\\0\'!                    #\n"
       << "###################################################################\n";
  GetExit();
  return nameout;
 }
return strcat(nameout,namein);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* StringFunctions::strnconcat(Char_t* nameout, const Char_t* namein, Int_t begin, Int_t end) {

// Substring of namein (starting at begin and ending at end position) concatenated to nameout.

 if (namein == NULL) return nameout;
 Int_t i, j = 0;
 j = strlength(nameout);
 for (i = begin; i < end; i++) 
  if (namein[i]) {
   nameout[j] = namein[i];
   j++;
  } else break;
 nameout[j] = '\0';
return nameout;
} 
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiSectionRaw 
//
// Internal class used only in case of reading data in "raw" format.
// It contains information of the structure of raw data like number
// of columns, keys (keywords) names, as well as all the data banks.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiSectionRaw)

//----------------------------------------------------------------------------
HAsciiSectionRaw::HAsciiSectionRaw() 
{
// Default constructor.

 fKeys = 0;
 fElements = 0;
 fpKeyTypeId = NULL;
 fpDataString = NULL;
 fpDataInt = NULL;
 fpDataFloat = NULL;
 fpDataChar = NULL;
 fpDataStr = NULL;
 fpTempString = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionRaw::~HAsciiSectionRaw() 
{
// Destructor. It deletes allocated memory if it was not deleted earlier.
// Therefore remember always to put your pointer to NULL after deleting!

 Reset();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionRaw::HAsciiSectionRaw(const HAsciiSectionRaw& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiSectionRaw::HAsciiSectionRaw> ######\n"
      << "# Object cannot be copied!                                                #\n"
      << "###########################################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionRaw& HAsciiSectionRaw::operator=(const HAsciiSectionRaw& source) {
 using namespace StringFunctions;
 if (this != &source) {
  cerr << "###### HadAscii ###### Error in <HAsciiSectionRaw::operator=> ######\n"
       << "# Object cannot be initialised by assignment!                      #\n"
       << "####################################################################\n";
  GetExit();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionRaw::Reset() 
{
// It deletes allocated memory if it was not deleted earlier and resets data.

 Int_t i, j;
 if (fpKeyTypeId) {
  delete [] fpKeyTypeId;
  fpKeyTypeId = NULL;
 }
 if (fpKeyNames) {
  for (i = 0; i < fKeys; i++)
   if (fpKeyNames[i]) delete [] fpKeyNames[i];
  delete [] fpKeyNames;
  fpKeyNames = NULL;
 }
 if (fpDataString) {
  for (i = 0; i < fKeys; i++)
   if (fpDataString[i]) delete [] fpDataString[i];
  delete [] fpDataString;
  fpDataString = NULL;
 }
 if (fpDataInt) {
  for (i = 0; i < fKeys; i++)
   if (fpDataInt[i]) delete [] fpDataInt[i];
  delete [] fpDataInt;
  fpDataInt = NULL;
 }
 if (fpDataFloat) {
  for (i = 0; i < fKeys; i++)
   if (fpDataFloat[i]) delete [] fpDataFloat[i];
  delete [] fpDataFloat;
  fpDataFloat = NULL;
 }
 if (fpDataChar) {
  for (i = 0; i < fKeys; i++)
   if (fpDataChar[i]) delete [] fpDataChar[i];
  delete [] fpDataChar;
  fpDataChar = NULL;
 }
 if (fpDataStr) {
  for (i = 0; i < fKeys; i++) 
   if (fpDataStr[i]) {
    for (j = 0; j < fElements; j++)
     if (fpDataStr[i][j]) {
      delete [] fpDataStr[i][j];
      fpDataStr[i][j] = NULL;
     }
    delete [] fpDataStr[i];
    fpDataStr[i] = NULL;
   }
  delete [] fpDataStr;
  fpDataStr = NULL; 
 }
 if (fpTempString) {
  delete [] fpTempString;
  fpTempString = NULL;
 }
 fKeys = 0;
 fElements = 0;
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiSection
//
// It is container for [section] parameters:
// name, state, begin (position in bytes), end, commentaries and
// pointer to HAsciiSectionRaw containing "raw data" information.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiSection)

//----------------------------------------------------------------------------
HAsciiSection::HAsciiSection() 
{
// Default constructor.

 fSectionState = 0;
 fSectionBegin = 0;
 fSectionEnd = 0;
 fSectionName[0] = '\0';
 fpSectionComment = NULL;
 fpRawData = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection::~HAsciiSection() 
{
// Destructor. It deletes allocated memory if it was not deleted earlier.
// Therefore remember always to put your pointer to NULL after deleting!

 Reset();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection::HAsciiSection(const HAsciiSection& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiSection::HAsciiSection> ######\n"
      << "# Object cannot be copied!                                          #\n"
      << "#####################################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection& HAsciiSection::operator=(const HAsciiSection& source) {
 using namespace StringFunctions;
 if (this != &source) { 
  cerr << "###### HadAscii ###### Error in <HAsciiSection::operator=> ######\n"
       << "# Object cannot be initialised by assignment!                   #\n"
       << "#################################################################\n";
  GetExit();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Short_t HAsciiSection::GetSectionState() 
{
// It returns present state of section.

 return fSectionState;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiSection::GetSectionName() 
{
// It returns the name of section (what is in [ ] brackets).

 return fSectionName;
}
//============================================================================

//----------------------------------------------------------------------------
Long_t HAsciiSection::GetSectionBegin() 
{
// It returns begin position of section in bytes (in given file).

 return fSectionBegin;
}
//============================================================================

//----------------------------------------------------------------------------
Long_t HAsciiSection::GetSectionEnd() 
{
// It returns end position of section in bytes (in given file).

 return fSectionEnd;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiSection::GetSectionComment() 
{
// It returns commentary to the section if there was any.

 return fpSectionComment;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionRaw* HAsciiSection::GetSectionRaw() 
{
// It returns pointer to HAsciiSectionRaw class containg raw data information.

 return fpRawData;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSection::SetSectionState(Short_t val) 
{
// It sets section state (so you can switch it off).

 fSectionState = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSection::SetSectionName(const Char_t* name) 
{
// It sets section name.

 using namespace StringFunctions;
 strcopy(fSectionName,name);
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSection::SetSectionBegin(Long_t val) 
{
// It sets section begin. Section begins after closing ] bracket of section name.

 fSectionBegin = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSection::SetSectionEnd(Long_t val) 
{
// It sets section end. Section ends at the opening [ bracket of the next
// section name or at the enf of file.

 fSectionEnd = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSection::SetSectionComment(const Char_t* name) 
{
// It sets section commentary.

 using namespace StringFunctions;
 fpSectionComment = new Char_t[strlength(name) + 1];
 strcopy(fpSectionComment,name);
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSection::Reset() 
{
// It deletes allocated memory if it was not deleted earlier and resets data. 

 fSectionState = 0;
 fSectionName[0] = '\0';
 fSectionBegin = fSectionEnd = 0;
 if (fpSectionComment) {
  delete [] fpSectionComment;
  fpSectionComment = NULL;
 }
 if (fpRawData) {
  delete fpRawData;
  fpRawData = NULL;
 }
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiSectionList
//
// Implementation of singly linked list which nodes contain
// information about sections.
// 
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiSectionList)

//----------------------------------------------------------------------------
HAsciiSectionList::HAsciiSectionList() : fLock(0),fpFirstNode(NULL),fpLastNode(NULL),fpThisNode(NULL) 
{
// Default contructor.

 fpTemp = NULL;
 fpOpenNode = new SectionNode * [10];
 for (Int_t i = 0; i < 10; i++) fpOpenNode[i] = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionList::HAsciiSectionList(SectionNode* init) : fLock(0),
        fpFirstNode(init),fpLastNode(init),fpThisNode(init) 
{
// Constructor for initialising section list with given node.

 fpTemp = NULL;
 fpOpenNode = new SectionNode * [10];
 for (Int_t i = 0; i < 10; i++) fpOpenNode[i] = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionList::~HAsciiSectionList() 
{
// Destructor removes section list and deletes all objects in nodes.

 Reset();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionList::HAsciiSectionList(const HAsciiSectionList& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiSectionList::HAsciiSectionList> ######\n"
      << "# Object cannot be copied!                                                  #\n"
      << "#############################################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionList& 
        HAsciiSectionList::operator=(const HAsciiSectionList& source) {
 using namespace StringFunctions;
 if (this != &source) { 
  cerr << "###### HadAscii ###### Error in <HAsciiSectionList::operator=> ######\n"
       << "# Object cannot be initialised by assignment!                       #\n"
       << "#####################################################################\n";
  GetExit();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection* HAsciiSectionList::ThisNodeSection() 
{
// It returns pointer to section of node pointed by fpThisNode (current
// list pointer) or NULL if fpThisNode is not set.

 if (fpThisNode) return &(fpThisNode->fSection); 
 else return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection* HAsciiSectionList::ToNodeSection(const Char_t* name) 
{
// It returns pointer to section of given name and sets current fpThisNode 
// to this node. If no name given, first node is returned.

 using namespace StringFunctions;
 if (NodeNumber()) {
  ToFirstNode();
  if (strlength(name)==0 || strempty(name)) {
   cerr << "###### HadAscii ###### Warning in <HAsciiSectionList::ToNodeSection> ######\n"
        << "# Section name for node not specified - pointer to the first section      #\n"
        << "# in the section list will be returned!                                   #\n"
        << "###########################################################################\n";
   return &(fpThisNode->fSection);
  }
  while (fpThisNode && 
         strcompare(name,ThisNodeSection()->GetSectionName()) != 0) {
   ToNextNode();
  }
  if (fpThisNode) return &(fpThisNode->fSection);
  cerr << "###### HadAscii ###### Error in <HAsciiSectionList::ToNodeSection> ######\n"
       << "# Section [" << name << "] not found!\n"
       << "#########################################################################\n";
  GetExit();
 } else { 
  cerr << "###### HadAscii ###### Error in <HAsciiSectionList::ToNodeSection> ######\n"
       << "# None section in the list!                                             #\n"
       << "#########################################################################\n";
  GetExit();
 }
 fpThisNode = NULL;
return NULL;
}   
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiSectionList::NodeNumber() 
{
// It  returns number of nodes = number of sections in list.

 Int_t i = 0;
 StoreThisNode();
 fpThisNode = fpFirstNode;
 while (fpThisNode) {
  i++;
  fpThisNode = fpThisNode->fpNextNode;
 }
 RestoreThisNode();
return i;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::ToFirstNode() 
{
// It sets current pointer fpThisNode to the first node.

 fpThisNode = fpFirstNode; 
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::ToLastNode() 
{
// It sets fpThisNode to the end of list (last node).

 fpThisNode = fpLastNode; 
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::AfterLastNode() 
{
// It sets fpThisNode to NULL what means position 'after last node in the list'.

 fpThisNode = NULL; 
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::InsertSection() 
{
// It creates a new node (with section) in position of list
// pointed by fpThisNode and assigns NULL values.

 SectionNode *pNewNode = new SectionNode;
 if (fpThisNode != NULL) {
  if (fpThisNode == fpFirstNode) {
   InsertFirstNode(pNewNode);
  } else {
   InsertNode(pNewNode);
  }
 } else {
  InsertLastNode(pNewNode);
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::InsertFirstNode(SectionNode* newnode) 
{
// It inserts node at the beginning of list.

 newnode->fpNextNode = fpThisNode;
 fpFirstNode = newnode;
 fpThisNode = newnode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::InsertNode(SectionNode* newnode) 
{
// It inserts node somewhere in the middle of list.

 PreviousNode(fpThisNode)->fpNextNode = newnode;
 newnode->fpNextNode = fpThisNode;
 fpThisNode = newnode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::InsertLastNode(SectionNode* newnode) 
{
// It inserts node at the end of list.

 if (!fpFirstNode) {
  fpFirstNode = newnode;
 } else {
  fpLastNode->fpNextNode = newnode;
 }
 fpLastNode = newnode;
 fpThisNode = newnode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::RemoveSection() 
{
// It removes node pointed by fpThisNode from section list.

 if (!fpThisNode) return;
 if (fpThisNode == fpFirstNode) {
  RemoveFirstNode();
 } else {
  if (fpThisNode == fpLastNode) {
   RemoveLastNode();
  } else {
   RemoveNode();
  }
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::RemoveFirstNode() 
{
// It removes node if it is the first node in the list.

 fpFirstNode = fpFirstNode->fpNextNode;
 delete fpThisNode;
 fpThisNode = fpFirstNode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::RemoveLastNode() 
{
// It removes node if it is the last node in the list.

 SectionNode *node;
 node = PreviousNode(fpThisNode);
 node->fpNextNode = NULL;
 fpLastNode = node;
 delete fpThisNode;
 fpThisNode = node;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::RemoveNode() 
{
// It removes node which is in the middle of the list.

 SectionNode *node;
 node = PreviousNode(fpThisNode);
 node->fpNextNode = fpThisNode->fpNextNode;
 delete fpThisNode;
 fpThisNode = node;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::ToNode(Int_t n) 
{
// It goes to node number n, nodes are counted from 0 (1st node) etc.

 ToFirstNode();
 for (Int_t i = 0; i < n; i++) {
  ToNextNode();
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::StoreThisNode() 
{
// It stores in temporary stack the value of fpThisNode. Used if operations
// on the list in nested loops are performed and the value od current pointer
// needs to be conserved.

 if (fLock < 10) {
  fpOpenNode[fLock++] = fpThisNode;
 } else {
  fpTemp = fpOpenNode;
  fpOpenNode = new SectionNode * [fLock];
  for (Int_t i = 0; i < fLock; i++) fpOpenNode[i] = fpTemp[i];
  fpOpenNode[fLock++] = fpThisNode;
  delete [] fpTemp;
  fpTemp = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::RestoreThisNode() 
{
// It restores the last stored value of fpThisNode.

 using namespace StringFunctions;
 if (fLock) {
  if (fLock < 10) {
   fpThisNode = fpOpenNode[--fLock];
   fpOpenNode[fLock] = NULL;
  } else {
   fpThisNode = fpOpenNode[--fLock];
   fpTemp = fpOpenNode;
   fpOpenNode = new SectionNode * [fLock];
   for (Int_t i = 0; i < fLock; i++) fpOpenNode[i] = fpTemp[i];
   delete [] fpTemp;
   fpTemp = NULL;
  }
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiSectionList::RestoreThisNode> ######\n"
       << "# Internal error: fpOpenNode has no fpThisNode value stored!              #\n"
       << "###########################################################################\n";
  GetExit();
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiSectionList::Reset() 
{
// It resets all data (deletes list with its objects).

 SectionNode *node;
 for (fpThisNode = fpFirstNode; fpThisNode; ) {
  node = fpThisNode->fpNextNode;
  delete fpThisNode;
  fpThisNode = node;
 }
 fpThisNode = NULL;
 fpFirstNode = NULL;
 fpLastNode = NULL;
 if (fpOpenNode) {
  delete [] fpOpenNode;
  fpOpenNode = NULL;
 }
 if (fpTemp) {
  delete [] fpTemp;
  fpTemp = NULL;
 }
 fLock = 0;
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiFile
//
// Container for file parameters:
// name, size, list of sections, different flags etc.
// It is used in a class defining file node.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiFile)

//----------------------------------------------------------------------------
HAsciiFile::HAsciiFile() : fFileReadState(0), fFileMode(0), fFileChange(0),
                             fFileSize(0), fPosNow(0), fPosition(0),
                             fSectionList(NULL) 
{
// Default constructor.

 fFileName[0] = '\0';
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFile::HAsciiFile(const HAsciiFile& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiFile::HAsciiFile> ######\n"
      << "# Object cannot be copied!                                    #\n"
      << "###############################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFile& HAsciiFile::operator=(const HAsciiFile& source) {
 using namespace StringFunctions;
 if (this != &source) {
  cerr << "###### HadAscii ###### Error in <HAsciiFile::operator=> ######\n"
       << "# Object cannot be initialised by assignment!                #\n"
       << "##############################################################\n";
  GetExit();
 }
return *this;
} 
//============================================================================

//----------------------------------------------------------------------------
Short_t HAsciiFile::GetFileState() 
{
// It returns file state, if file opened, it is 1.

 return fFileReadState;
}
//============================================================================

//----------------------------------------------------------------------------
Short_t HAsciiFile::GetFileMode() 
{
// It is file mode which can be in fact only 1 (read-mode).

 return fFileMode;
}
//============================================================================

//----------------------------------------------------------------------------
Short_t HAsciiFile::GetFileChange() 
{
// It returns flag informing if the file was already scanned for sections.

 return fFileChange;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiFile::GetFileName() 
{
// It returns file name.

 return fFileName;
}
//============================================================================

//----------------------------------------------------------------------------
Long_t HAsciiFile::GetFileSize() 
{
// Here we have file size.

 return fFileSize;
}
//============================================================================

//----------------------------------------------------------------------------
Long_t HAsciiFile::GetPosNow() 
{
// Auxiliary function with read position.

 return fPosNow;
}
//============================================================================

//----------------------------------------------------------------------------
Long_t HAsciiFile::GetPosition() 
{
// Auxiliary function used in read functions.

 return fPosition;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiFile::OpenFile(const Char_t* filename) 
{
// It opens file - this function is called from another method in HAscii class.

 using namespace StringFunctions;
 fStreamFileRead.open(filename, ios::in);

 if (fStreamFileRead.fail()) {
  cerr << "###### HadAscii ###### Error in <HAsciiFile::OpenFile> ######\n"
       << "# File \'" << filename << "\' not found!\n"
       << "#############################################################\n";
  fStreamFileRead.clear(fStreamFileRead.rdstate() & ~ios::badbit);
  GetExit();
  return kFALSE;
 }

 SetFileState(1);
 SetFileMode(1);
 SetFileChange(1);
 SetFileName(filename);
 SetFileSize();
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiFile::CloseFile() 
{
// It closes file and deletes section list.

 using namespace StringFunctions;
 fStreamFileRead.close();

 if (fStreamFileRead.fail()) {
  cerr << "###### HadAscii ###### Error in <HAsciiFile::CloseFile> ######\n"
       << "# Unexpected error during closing file \'" << GetFileName() << "\'\n"
       << "##############################################################\n";
  fStreamFileRead.clear(fStreamFileRead.rdstate() & ~ios::badbit);
  Reset();
  GetExit();
  return kFALSE;
 }

// cout << "File \'" << GetFileName() << "\' closed.\n";
Reset();
return kTRUE;
}
//============================================================================
  
//----------------------------------------------------------------------------
void HAsciiFile::SetFileState(Short_t val) 
{
// Setting file state.

 fFileReadState = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::SetFileMode(Short_t val) 
{
// Setting file mode (it is obsolete; only 1 available because of read-mode, 
// write-mode solved in another way).

 fFileMode = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::SetFileChange(Short_t val) 
{
// Setting file change flag (if 1 then file should be scanned for sections).
 fFileChange = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::SetFileName(const Char_t* nam) 
{
// Setting file name.

 using namespace StringFunctions;
 strcopy(fFileName,nam);
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::SetFileSize() 
{
// Setting file size.

 fStreamFileRead.seekg(0,ios::end);
 fFileSize = fStreamFileRead.tellg();
 fStreamFileRead.seekg(0);
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::SetPosNow(Long_t val) 
{
// Setting current position during reading.

 fPosNow = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::SetPosition(Long_t val) 
{
// Setting position (auxiliary function).

 fPosition = val;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::Reset() 
{
// Resetting all data and section list (with deleting nodes).

 fFileReadState = fFileMode = fFileChange = 0;
 fFileName[0] = '\0';
 fFileSize = fPosNow = fPosition = 0;
 fSectionList.Reset();
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiFile::ReadSectionComment(Int_t begin, Int_t end) 
{
// It returns commentary for section (if there is any).
// This function is called only from InitSectionList().

 using namespace StringFunctions;
 Int_t iStreamPos = 0;
 Char_t *pData = NULL;
 Char_t *pDataTemp = NULL;
 Char_t cLine[1024], cData[1024];
 cLine[0] = '\0';
 cData[0] = '\0';
 iStreamPos = fStreamFileRead.tellg();
 fPosition = fPosNow;
 fStreamFileRead.seekg(begin);

 do {
  fPosNow = fStreamFileRead.tellg();
  if (fPosNow < end)  
   if (fStreamFileRead.peek() == '\n') {
    fStreamFileRead.ignore(1);
    fPosNow = fStreamFileRead.tellg();
   }
  if (fPosNow < end) {
   cLine[0] = '\0';
   cData[0] = '\0';
   if ((end - fPosNow) >= 1023) fStreamFileRead.get(cLine,1023);
    else fStreamFileRead.get(cLine,(end - fPosNow + 1));
   if (fStreamFileRead.fail()) {
    fStreamFileRead.clear(fStreamFileRead.rdstate() & ~ios::failbit);
    cerr << "###### HadAscii ###### Error in <HAsciiFile::ReadSectionComment> ######\n"
         << "# In file \'" << GetFileName() << "\' unexpected error\n"
         << "# during searching commentaries for section!                          #\n"
         << "#######################################################################\n";
    GetExit();
    if (pData) delete [] pData;
    fPosNow = fPosition;
    fStreamFileRead.seekg(iStreamPos);
    return NULL;
   }
   if (strfirstchar(cLine) == '#' ||
       strlength(cLine) == 0 || 
       strempty(cLine)) {
    // ignore this line
   } else if (strfirstchar(cLine) == ';') {
    if (pData) {
     pDataTemp = pData;
    }
    strafter(cData,cLine,';');
    pData = new Char_t[strlength(pDataTemp) + strlength(cData) + 1];
    pData[0] = '\0';
    if (pDataTemp) {
     strcopy(pData,pDataTemp);
     strconcat(pData,cData);
     delete [] pDataTemp;
     pDataTemp = NULL;
    } else {
     strcopy(pData,cData);
    }
   } else {
    fPosNow = fPosition;
    fStreamFileRead.seekg(iStreamPos);
    return pData;
   }
  }
 } while (fPosNow < end);

fPosNow = fPosition;
fStreamFileRead.seekg(iStreamPos);
return pData;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFile::InitSectionList() 
{
// It reads  all sections in file and creates section list with section parameters.

 using namespace StringFunctions;
 Long_t lSecBegin = 0, lSecEnd = 0;
 Char_t cSecName[128];
 Char_t cData[1024], cLine[1024];
 cSecName[0] = '\0';
 cData[0] = '\0';
 cLine[0] = '\0';

 if (fFileReadState && fFileChange) {
  if (fSectionList.NodeNumber()) fSectionList.Reset();
  fStreamFileRead.seekg(0);
  fPosNow = fStreamFileRead.tellg();
  do {
   if (fPosNow < fFileSize)
    if (fStreamFileRead.peek() == '\n') {
     fStreamFileRead.ignore(1);
     fPosNow = fStreamFileRead.tellg();
    }
   if (fPosNow < fFileSize) {
    if ((fFileSize - fPosNow) >= 1023) fStreamFileRead.get(cLine,1023);
     else fStreamFileRead.get(cLine,(fFileSize - fPosNow + 1));
    if (fStreamFileRead.fail()) {
     fStreamFileRead.clear(fStreamFileRead.rdstate() & ~ios::failbit);
     cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
          << "# In file \'" << GetFileName() << "\' unexpected error\n"
          << "# during searching section name!                                   #\n"
          << "####################################################################\n";
     GetExit();
     return;
    }
    if (strfirstchar(cLine) == '[') {

     if (lSecBegin) {
      strbefore(cData,cLine,'[');
      lSecEnd = fPosNow + strlength(cData);
      if (lSecEnd <= lSecBegin) {
       cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
            << "# Internal error: Section begin <= section end.                    #\n"
            << "# Please report it to the author!                                  #\n"
            << "####################################################################\n";
       GetExit();
       return;
      } 
      fSectionList.AfterLastNode();
      fSectionList.InsertSection();
      fSectionList.ThisNodeSection()->SetSectionName(cSecName);
      fSectionList.ThisNodeSection()->SetSectionBegin(lSecBegin);
      fSectionList.ThisNodeSection()->SetSectionEnd(lSecEnd);
      fSectionList.ThisNodeSection()->SetSectionComment(
                                   ReadSectionComment(lSecBegin,lSecEnd));
      lSecBegin = lSecEnd = 0;
      cSecName[0] = '\0';
      cData[0] = '\0';
     }
     
     if (strlastchar(cLine) == ']') {
      strbefore(cData,cLine,']'); 
      lSecBegin = fPosNow + strlength(cData) + 1;
      cLine[0] = '\0';
      strafter(cLine,cData,'[');
      cData[0] = '\0';
      strtrunc(cData,cLine);
      if (strlength(cData) < 128) {
       strcopy(cSecName,cData);
      } else {
       cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
            << "# In file \'" << GetFileName() << "\' section name\n"
            << "# [" << cData << "...\n"
            << "# longer than 127 characters!                                      #\n"
            << "####################################################################\n";
       GetExit();
       lSecBegin = 0;
       return;
      }
     } else {
      fPosNow = fStreamFileRead.tellg();
      if (fPosNow >= fFileSize) {
       cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
            << "# End of file \'" << GetFileName() << "\' reached during\n"
            << "# reading section name!                                            #\n"
            << "####################################################################\n";
       GetExit();
       fFileChange = 0;
       return;
      } else {
       cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
            << "# In file \'" << GetFileName() << "\' section name\n"
            << "# [" << cLine << " not terminated by \"]\"!\n"
            << "####################################################################\n";
       GetExit();
       return;
      }
     }
    } 
    fPosNow = fStreamFileRead.tellg();
    if (fPosNow < fFileSize)
     if (fStreamFileRead.peek() == '\n') {
      fStreamFileRead.ignore(1);
      fPosNow = fStreamFileRead.tellg();
     }
    if (fPosNow >= fFileSize) {

     if (lSecBegin) {
      lSecEnd = fFileSize;
      if (lSecEnd <= lSecBegin) {
       cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
            << "# Internal error: Section begin <= section end.                    #\n" 
            << "# Please report it to the author!                                  #\n"
            << "####################################################################\n";
       GetExit();
       return;
      } 
      fSectionList.AfterLastNode();
      fSectionList.InsertSection();
      fSectionList.ThisNodeSection()->SetSectionName(cSecName);
      fSectionList.ThisNodeSection()->SetSectionBegin(lSecBegin);
      fSectionList.ThisNodeSection()->SetSectionEnd(lSecEnd);
      fSectionList.ThisNodeSection()->SetSectionComment(
                                    ReadSectionComment(lSecBegin,lSecEnd));
     }
     fFileChange = 0;
     return;

    }
    cData[0] = '\0';
    cLine[0] = '\0';

   } else {

    if (lSecBegin) {
     lSecEnd = fFileSize;
     if (lSecEnd <= lSecBegin) {
      cerr << "###### HadAscii ###### Error in <HAsciiFile::InitSectionList> ######\n"
           << "# Internal error: Section begin <= section end.                    #\n"
           << "# Please report it to the author!                                  #\n"
           << "####################################################################\n";
      GetExit();
      return;
     }
     fSectionList.AfterLastNode();
     fSectionList.InsertSection();
     fSectionList.ThisNodeSection()->SetSectionName(cSecName);
     fSectionList.ThisNodeSection()->SetSectionBegin(lSecBegin);
     fSectionList.ThisNodeSection()->SetSectionEnd(lSecEnd);
     fSectionList.ThisNodeSection()->SetSectionComment(
                                    ReadSectionComment(lSecBegin,lSecEnd));
    }
    fFileChange = 0;
    return;

   }
  } while (fPosNow < fFileSize);
 }

 fFileChange = 0;
} 
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiFileList
//
// Implementation of singly linked list which nodes contain
// informations about files.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiFileList)

//----------------------------------------------------------------------------
HAsciiFileList::HAsciiFileList() : fLock(0),fpFirstNode(NULL),
                                       fpLastNode(NULL),fpThisNode(NULL) 
{
// Default constructor.

 fpTemp = NULL;
 fpOpenNode = new FileNode * [10];
 for (Int_t i = 0; i < 10; i++) fpOpenNode[i] = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFileList::~HAsciiFileList() 
{
// Destructor. It deletes file list releasing memory allocated by them.

 FileNode *node;
 for (fpThisNode = fpFirstNode; fpThisNode; ) {
  node = fpThisNode->fpNextNode;
  delete fpThisNode;
  fpThisNode = node;
 }
 if (fpOpenNode) {
  delete [] fpOpenNode;
  fpOpenNode = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFileList::HAsciiFileList(const HAsciiFileList& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiFileList::HAsciiFileList> ######\n"
      << "# Object cannot be copied!                                            #\n"
      << "#######################################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFileList& HAsciiFileList::operator=(const HAsciiFileList& source) {
 using namespace StringFunctions;
 if (this != &source) {
  cerr << "###### HadAscii ###### Error in <HAsciiFileList::operator=> ######\n"
       << "# Object cannot be initialised by assignment!                    #\n"
       << "##################################################################\n";
  GetExit();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFile* HAsciiFileList::ThisNodeFile() 
{
// Returning file of current node (pointed by fpThisNode).

 if (fpThisNode) return &(fpThisNode->file); 
 else return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFile* HAsciiFileList::ToNodeFile(const Char_t* name) 
{
// It sets current list pointer fpThisNode to node with file of given name.

 using namespace StringFunctions;
 if (NodeNumber()) {
  ToFirstNode();
  if (strlength(name)==0 || strempty(name)) {
   // this warning is not ok in case of not parametrised closing
   // cerr << "Warning in <HAsciiFileList::ToNodeFile>: "
   //     << "File name for node not specified - pointer to the 1st file "
   //     << "in the file list will be returned!\n";
   return &(fpThisNode->file);
  }
  while (fpThisNode && strcompare(name,ThisNodeFile()->GetFileName()) != 0) {
   ToNextNode();
  }
  if (fpThisNode) return &(fpThisNode->file);
  cerr << "###### HadAscii ###### Error in <HAsciiFileList::ToNodeFile> ######\n"
       << "# File \'" << name << "\' not opened!\n"
       << "###################################################################\n";
  GetExit();
 } else if (strempty(name) == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiFileList::ToNodeFile> ######\n"
       << "# File \'" << name << "\' not opened!\n"
       << "###################################################################\n";
  // GetExit();
 }
 fpThisNode = NULL;
return NULL;
}   
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiFileList::NodeNumber() 
{
// Number of nodes (files) in the list.

 Int_t i = 0;
 StoreThisNode();
 fpThisNode = fpFirstNode;
 while (fpThisNode) {
  i++;
  fpThisNode = fpThisNode->fpNextNode;
 }
 RestoreThisNode();
return i;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::ToFirstNode() 
{
// Current node set to the first.

 fpThisNode = fpFirstNode; 
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::ToLastNode() 
{
// Current node set to the last node.

 fpThisNode = fpLastNode; 
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::AfterLastNode() 
{
// Current node set to NULL (it means after the last node).

 fpThisNode = NULL; 
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::InsertFile() 
{
// It creates new node with place for file and puts into list at the position
// of the current fpThisNode pointer.

 FileNode *pNewNode = new FileNode;
 if (fpThisNode != NULL) {
  if (fpThisNode == fpFirstNode) {
   InsertFirstNode(pNewNode);
  } else {
   InsertNode(pNewNode);
  }
 } else {
  InsertLastNode(pNewNode);
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::InsertFirstNode(FileNode* newnode) 
{
// It inserts node at the first position of the list.

 newnode->fpNextNode = fpThisNode;
 fpFirstNode = newnode;
 fpThisNode = newnode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::InsertNode(FileNode* newnode) 
{
// It inserts node into list somwhere in the place pointed by fpThisNode.

 PreviousNode(fpThisNode)->fpNextNode = newnode;
 newnode->fpNextNode = fpThisNode;
 fpThisNode = newnode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::InsertLastNode(FileNode* newnode) 
{
// It inserts node at the last list position.

 if (!fpFirstNode) {
  fpFirstNode = newnode;
 } else {
  fpLastNode->fpNextNode = newnode;
 }
 fpLastNode = newnode;
 fpThisNode = newnode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::RemoveFile() 
{
// It removes file (it means node) from the list pointed by current pointer fpThisNode.

 if (!fpThisNode) return;
 if (fpThisNode == fpFirstNode) {
  RemoveFirstNode();
 } else {
  if (fpThisNode == fpLastNode) {
   RemoveLastNode();
  } else {
   RemoveNode();
  }
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::RemoveFirstNode() 
{
// Removing first node in the list.

 fpFirstNode = fpFirstNode->fpNextNode;
 delete fpThisNode;
 fpThisNode = fpFirstNode;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::RemoveLastNode() 
{
// Removing last node in the list.

 FileNode *node;
 node = PreviousNode(fpThisNode);
 node->fpNextNode = NULL;
 fpLastNode = node;
 delete fpThisNode;
 fpThisNode = node;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::RemoveNode() 
{
// Removing node pointed by current list pointer fpThisNode.

 FileNode *node;
 node = PreviousNode(fpThisNode);
 node->fpNextNode = fpThisNode->fpNextNode;
 delete fpThisNode;
 fpThisNode = node;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::ToNode(Int_t n) 
{
// Setting current list pointer fpThisNode to node number n.

 ToFirstNode();
 for (Int_t i = 0; i < n; i++) {
  ToNextNode();
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::StoreThisNode() 
{
// It stores in temporary stack the value of fpThisNode. Used if operations
// on the list in nested loops are performed and the value od current pointer
// needs to be conserved.

 if (fLock < 10) {
  fpOpenNode[fLock++] = fpThisNode;
 } else { 
  fpTemp = fpOpenNode;
  fpOpenNode = new FileNode * [fLock];
  for (Int_t i = 0; i < fLock; i++) fpOpenNode[i] = fpTemp[i];
  fpOpenNode[fLock++] = fpThisNode;
  delete [] fpTemp;
  fpTemp = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiFileList::RestoreThisNode() 
{
// It restores the last stored value of fpThisNode.

 using namespace StringFunctions;
 if (fLock) {
  if (fLock < 10) {
   fpThisNode = fpOpenNode[--fLock];
   fpOpenNode[fLock] = NULL;
  } else {
   fpThisNode = fpOpenNode[--fLock];
   fpTemp = fpOpenNode;
   fpOpenNode = new FileNode * [fLock];
   for (Int_t i = 0; i < fLock; i++) fpOpenNode[i] = fpTemp[i];
   delete [] fpTemp;
   fpTemp = NULL;
  }
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiFileList::RestoreThisNode> ######\n"
       << "# Internal error: fpOpenNode has no fpThisNode value stored!           #\n"
       << "########################################################################\n";
  GetExit();
 }
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAscii
//
// Base class for ASCII operations, opening and closing files,
// setting active section, reading data and commentaries.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAscii)

//----------------------------------------------------------------------------
HAscii::HAscii() : fDataLineLength(0),fDataCounter(0),
                   fDataBank(0) 
{
// Default constructor. It resets all data and sets pointers to NULL.

 using namespace StringFunctions;
 SetExit(1);
 fReadAllPosition = 0;
 fKeyLine[0] = '\0';
 for (Int_t i=0; i<1024; i++) {
  fDataInt[i] = 0;
  fDataFloat[i] = 0.;
  fDataChar[i] = '\0';
  fDataString[i] = '\0';
 }
 fpRemarkLine = NULL;
 fpDataLine = NULL;
 fpCommentLine = NULL;
 fpActiveSection = NULL;
 fpActiveFile = NULL;
 fpDataInt = NULL;
 fpDataFloat = NULL;
 fpDataChar = NULL;
 fpDataString = NULL;

 fSeparator = ' ';
 fNumInLine = 16;
 Reset();
}
//============================================================================

//----------------------------------------------------------------------------
HAscii::~HAscii() 
{
// Destructor releases memory allocated dynamically.

 CloseFile();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }
 if (fpDataLine) {
  delete [] fpDataLine;
  fpDataLine = NULL;
 }
 if (fpCommentLine) {
  delete [] fpCommentLine;
  fpCommentLine = NULL;
 }
 if (fpActiveSection) {
  delete [] fpActiveSection;
  fpActiveSection = NULL;
 }
 if (fpActiveFile) {
  delete [] fpActiveFile;
  fpActiveFile = NULL;
 }
 if (fpDataInt) {
  delete [] fpDataInt;
  fpDataInt = NULL;
 }
 if (fpDataFloat) {
  delete [] fpDataFloat;
  fpDataFloat = NULL;
 }
 if (fpDataChar) {
  delete [] fpDataChar;
  fpDataChar = NULL;
 }
 if (fpDataString) {
  delete [] fpDataString;
  fpDataString = NULL;
 }
} 
//============================================================================

//----------------------------------------------------------------------------
HAscii::HAscii(const HAscii& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAscii::HAscii> ######\n"
      << "# Object cannot be copied!                            #\n"
      << "#######################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAscii& HAscii::operator=(const HAscii& source) {
 using namespace StringFunctions;
 if (this != &source) {
  cerr << "###### HadAscii ###### Error in <HAscii::operator=> ######\n"
       << "# Object cannot be initialised by assignment!            #\n"
       << "##########################################################\n";
 GetExit();
 }
return *this;
}
//============================================================================


//----------------------------------------------------------------------------
Char_t* HAscii::CorrectName(Char_t* name) 
{
// Function called while opening file error happened to correct file name.

 using namespace StringFunctions;
 Char_t choice;
 Char_t tempname[150];
 tempname[0] = '\0';

 do {
  cout << "Do you want to correct\\abort? [y\\n]: ";
  choice = '\0';
  cin >> choice;
  if (choice!=121 && choice!=89 && choice!=110 && choice!=78)
   cout << "Incorrect character!\n";
 } while (choice!=89 && choice!=121 && choice!=78 && choice!=110);
 if (choice==89 || choice==121) {
  *tempname = '\0';
  cout << "Write valid file name: ";
  cin >> tempname;
  *name = '\0';
  strtrunc(name,tempname);
 } else {
  cout << "Program terminated by user.\n";
  exit(1);
 }
return name;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::CheckFileList(HAsciiFileList& list, Char_t* name) 
{
// It checks if file of given name is not already opened.

 using namespace StringFunctions;
 Int_t iFlag;
 list.StoreThisNode();
 if (list.NodeNumber()) {
  list.ToFirstNode();
  do {
   for (iFlag=0; !iFlag; )
    if (strcompare(name,list.ThisNodeFile()->GetFileName())==0) {
     cerr << "###### HadAscii ###### Error in <HAscii::CheckFileList> ######\n"
          << "# File \'" << name << "\' already opened!\n"
          << "##############################################################\n";
     CorrectName(name);
     list.ToFirstNode();
    } else {
     iFlag = 1;
    }
  } while (list.ToNextNode());
 }
 list.RestoreThisNode();
return name;
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::WriteError() 
{
// It checks if the error has happened during write opetarion.

 using namespace StringFunctions;
 if (fStreamFileWrite.fail()) {
  cerr << "###### HadAscii ###### Error in <HAscii::WriteError> ######\n"
       << "# Error while writing data to file \'" << fFileWriteName << "\'\n"
       << "# (maybe disk full?)!                                     #\n"
       << "###########################################################\n";
  fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
  fStreamFileWrite.close();
  GetExit();
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::Reset() 
{
// Resetting data of file (in write mode).

 fFileWriteState = 0;
 fFileWriteName[0] = '\0';
 fSeparator = ' ';
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::SetSeparator(Char_t character) 
{
// Setting separator for data units (only space, tabulation '\t' or comma allowed).

 if (character != ' ' && character != '\t' && character != ',') {
  cerr << "###### HadAscii ###### Warning in <HAscii::SetSeparator> ######\n"
       << "# Incorrect separator as argument: space, tabulation (\\t) or  #\n"
       << "# comma allowed (now set to default = space).                  #\n"
       << "###############################################################\n";
  character = ' ';
  return;
 }
 fSeparator = character;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAscii::GetSeparator() 
{
// Getting current separator character for data units.

return fSeparator;
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::CloseOpenedFile() 
{
// Checking if file in write mode is opened (and closing it if yes)
// in case you want to open a new file in write mode.

 using namespace StringFunctions;
 if (fFileWriteState > 0) {
  cerr << "###### HadAscii ###### Warning in <HAscii::CloseOpenedFile> ######\n"
       << "# There is already file \'" << fFileWriteName << "\'"
       << "# opened in write mode. It will be closed now.                   #\n"
       << "##################################################################\n";
  fStreamFileWrite << endl;
  WriteError();
  fStreamFileWrite.close();
  if (fStreamFileWrite.fail()) {
   cerr << "###### HadAscii ###### Error in <HAscii::CloseOpenedFile> ######\n"
        << "# Unexpected error during closing file \'" << fFileWriteName << "\'\n"
        << "################################################################\n";
   fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
   GetExit();
  }
  Reset();
 }
}
//============================================================================

//----------------------------------------------------------------------------
fstream& HAscii::OpenFile(const Char_t* filename, const Char_t* mode) 
{
// Opening ASCII file with given 'filename' and 'mode'.
//
//  Modes available: 
//  "read" or "in" 
//   - read mode (it is default). Unlimited number of files
//     can be opened in read mode.
//  "new" or "out" or "create" or "write" 	
//   - write mode. A new file is created but only if there is 
//     no file with the same 'filename' existing. Only one file
//     can be opened in write mode at the same time.
//  "update" 	
//   - opens existing file for writing at the end of it (to append data).
//  "recreate" or "overwrite"	
//   - creates a new file or replaces existing in write mode.

 using namespace StringFunctions;
 Int_t dataOK = 0, opentime = 0;
 Int_t fFileReadState = 0;
 TDatime timenow;

 Char_t digitchar[9];
 Char_t tempname[150];
 Char_t openline[100];
 digitchar[0] = '\0';
 tempname[0] = '\0';
 openline[0] = '\0';
 strtrunc(tempname,filename);

 Char_t cMode[10];
 Char_t cModeRaw[20];
 cMode[0] = '\0';
 cModeRaw[0] = '\0';

 strtrunc(cModeRaw,mode);
 strtolower(cMode,cModeRaw);
 
 if (!strcompare(cMode,"read") || 
     !strcompare(cMode,"in")) {

  // function OpenFile from HAsciiFile will be called and 
  // file will be added to a list of files

  // file will be opened in read mode
  fFileReadState = 1;

 } else if (!strcompare(cMode,"new") || 
            !strcompare(cMode,"create") || 
            !strcompare(cMode,"write") || 
            !strcompare(cMode,"out")) {

  CloseOpenedFile();
  // creates new file in write mode (if there is no file
  // of that name already existing)

  fFileWriteState = 1;

 } else if (!strcompare(cMode,"update")) {

  CloseOpenedFile();
  // opens existing file in write mode to append data
  // at the end of file
  fFileWriteState = 2;

 } else if (!strcompare(cMode,"recreate") || 
            !strcompare(cMode,"overwrite")) {

  CloseOpenedFile();
  // creates new file (or replaces existing) in write mode
  fFileWriteState = 3;

 } else {
  cerr << "###### HadAscii ###### Error in <HAscii::OpenFile> ######\n"
       << "# File mode \'" << cMode << "\' not known!\n"
       << "# File \'" << tempname << "\' will be opened in \'read\' mode.\n"
       << "#########################################################\n";
  fFileReadState = 1;
 }

 if (fFileReadState) {
  // prepares place in file list
  CheckFileList(fFileList,tempname);
  fFileList.AfterLastNode();
  fFileList.InsertFile();
 }

 for (dataOK=0; !dataOK; ) {

  if (fFileReadState) 
   if (!fFileList.ThisNodeFile()->OpenFile(tempname)) {
    CorrectName(tempname);
    CheckFileList(fFileList,tempname);
   } else {
    dataOK = 1;
   }

  if (fFileReadState==0 && fFileWriteState) 
   switch (fFileWriteState) {
    case 1: fStreamFileWrite.open(tempname, ios::out | ios::noreplace);
            break;
    case 2: fStreamFileWrite.open(tempname, ios::out | ios::app);
            break;
    case 3: fStreamFileWrite.open(tempname, ios::out | ios::trunc);
            break;
   }

  if (fFileReadState && dataOK) {
   // file opened in read mode successfully
   return fFileList.ThisNodeFile()->fStreamFileRead;
  }

  if (fFileReadState==0 && fFileWriteState)
   if (fStreamFileWrite.fail()) {
    cerr << "###### HadAscii ###### Error in <HAscii::OpenFile> ######\n"
         << "# File \'" << tempname << "\' cannot be opened for writing!\n"
         << "#########################################################\n";
    fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
    CorrectName(tempname);
   } else {
    dataOK = 1;
    fFileWriteName[0] = '\0';
    strcopy(fFileWriteName,tempname);
    
    if (fFileWriteState == 2) strconcat(openline,"\n# ASCII file ");
    else strconcat(openline,"# ASCII file ");
    if (fFileWriteState == 2) strconcat(openline,"appended ");
    else strconcat(openline,"created ");

    opentime = timenow.GetDate();
    sprintf(digitchar,"%d",opentime/10000);
    strconcat(openline,digitchar);
    strconcat(openline,"/");
    sprintf(digitchar,"%d",(opentime%10000)/100);
    strconcat(openline,digitchar);
    strconcat(openline,"/");
    sprintf(digitchar,"%d",opentime%100);
    strconcat(openline,digitchar);
    strconcat(openline," (");

    opentime = timenow.GetTime();
    if (opentime/10000 < 10) strconcat(openline,"0");
    sprintf(digitchar,"%d",opentime/10000);
    strconcat(openline,digitchar);
    strconcat(openline,":");
    if ((opentime%10000)/100 < 10) strconcat(openline,"0");
    sprintf(digitchar,"%d",(opentime%10000)/100);
    strconcat(openline,digitchar);
    strconcat(openline,":");
    if (opentime%100 < 10) strconcat(openline,"0");
    sprintf(digitchar,"%d",opentime%100);
    strconcat(openline,digitchar);
    strconcat(openline,")  ");
    strconcat(openline,"\n\n");
 
    fStreamFileWrite << openline;
    WriteError();
    // file opened in write mode successfully
    return fStreamFileWrite;
   }
 } // end of checking loop

 cerr << "###### HadAscii ###### Error in <HAscii::OpenFile> ######\n"
      << "# Function achieved the end not returning proper        #\n"
      << "# fstream object. Please report it to the author!       #\n"
      << "#########################################################\n";
 GetExit();
return fStreamFileWrite; // this should never happen but compiler wants it...!
} 
//============================================================================

//----------------------------------------------------------------------------
void HAscii::OpenFileList(const Char_t* filename) 
{
// It opens and reads ASCII file with the list of files and opens all of them.
// The file with file list must be in the following syntax:
// section - 'ASCIIFileList'
// keyword - 'Files', followed by string containing all file names (and
// optionally mode), separated by commas.
//
// Example of file containing list of files to be opened:
//
//  [ASCII File List]
//  ; you may put commentaries, as well
//
//  Files = mdcinit.txt, richpads.txt read,
//  \ showercal_data.dat,
//  \ tofcal.txt read , /scratch/tofini.dat 

 using namespace StringFunctions;
 Int_t iLong = 0;
 Char_t *pData = NULL;
 Char_t cFileList[160];
 cFileList[0] = '\0';
 Char_t fFileNameList[128];
 fFileNameList[0] = '\0';
 Char_t cFile[160];
 cFile[0] = '\0';
 Char_t fFileName[128];
 fFileName[0] = '\0';
 Char_t cFileMode[32];
 cFileMode[0] = '\0';

 strcopy(cFileList,filename);
 strtrunc(fFileNameList,cFileList);
 OpenFile(fFileNameList);
 SetActiveSection("ASCII File List");
 iLong = strlength(ReadString("Files"));
 if (iLong == 0) {
  cerr << "###### HadAscii ###### Error in <HAscii::OpenFileList> ######\n"
       << "# File list in file \'" << fFileNameList << "\' is empty!\n"
       << "#############################################################\n";
  GetExit();
  ResetKeyData();
  ResetBankData();
  CloseFile(fFileNameList);
  return;
 } 
 pData = new Char_t[iLong + 1];
 pData[0] = '\0';
 strcopy(pData,ReadString());
 if (strfirstchar(pData) == '\0') {
  cerr << "###### HadAscii ###### Error in <HAscii::OpenFileList> ######\n"
       << "# File list in file \'" << fFileNameList << "\' is empty!\n"
       << "#############################################################\n";
  GetExit();
  ResetKeyData();
  ResetBankData();
  delete [] pData;
  pData = NULL;
  CloseFile(fFileNameList);
  return;    
 }
 do {
  strunit(cFile,pData,','); 
  if (strlength(cFile)) {
   strunit(fFileName,cFile);
   strunit(cFileMode,cFile);
   if (strlength(cFileMode) > 0) OpenFile(fFileName,cFileMode);
   else OpenFile(fFileName);
   fFileName[0] = '\0';
   cFileMode[0] = '\0';
  }
 } while (strlength(cFile) > 0); 
 delete [] pData;
 pData = NULL;
 CloseFile(fFileNameList);
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::CloseFile(const Char_t* filename) 
{
// Closing file of given filename or all opened files if no filename specified.

 using namespace StringFunctions;
 Char_t tempname[150];
 tempname[0] = '\0';
 strtrunc(tempname,filename);

 if (fFileWriteState) {
  if (!strlength(tempname) || !strcompare(tempname,fFileWriteName)) {
   fStreamFileWrite << endl;
   WriteError();
   fStreamFileWrite.close();
   fFileWriteState = 0;
   fFileWriteName[0] = '\0';
   if (fStreamFileWrite.fail()) {
    cerr << "###### HadAscii ###### Error in <HAscii::CloseFile> ######\n"
         << "# Unexpected error during closing file \'" << tempname << "\'\n"
         << "##########################################################\n";
    fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
    GetExit();
    return;
   }
  }
 }

 if (fpActiveFile != NULL && fpActiveSection != NULL) 
  if (!strlength(tempname) || !strcompare(tempname,fpActiveFile->GetFileName())) {
   fpActiveFile = NULL;
   fpActiveSection = NULL;
  } 
 do {
  fFileList.ToNodeFile(tempname);
  if (fFileList.ThisNode()) fFileList.ThisNodeFile()->CloseFile();
  fFileList.RemoveFile();

 } while (!tempname && fFileList.NodeNumber());
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFile* HAscii::GetActiveFile() 
{
// Pointer to active file (that file which contains active section).

 return fpActiveFile;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection* HAscii::GetActiveSection() 
{
// Pointer to active section.

 return fpActiveSection;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckUnambiguitySection() 
{
// It checks if a new section name is  unambiguous with relation 
// to previously stored while initializing next section list.

 using namespace StringFunctions;
 Int_t i,j,k,l;
 Int_t iAmbiguityFlag = 0;

 HAsciiFile *pFileNode = NULL;
 HAsciiSection *pSectionNode = NULL;

 fFileList.StoreThisNode();
 if (fFileList.ThisNode()) fFileList.ThisNodeFile()->fSectionList.StoreThisNode();

 for (i = 0; i < fFileList.NodeNumber(); i++) {
  fFileList.ToNode(i);
  if (fFileList.ThisNode()) pFileNode = fFileList.ThisNodeFile();
   else { pFileNode = NULL; continue; }

  for (j = 0; j < pFileNode->fSectionList.NodeNumber(); j++) {
   pFileNode->fSectionList.ToNode(j);
   if (pFileNode->fSectionList.ThisNode()) 
    pSectionNode = pFileNode->fSectionList.ThisNodeSection();
   else { pSectionNode = NULL; continue; }

    for (l = j+1; l < pFileNode->fSectionList.NodeNumber(); l++) {
     pFileNode->fSectionList.ToNode(l);
     if (!pFileNode->fSectionList.ThisNode()) continue;

     if (!strcompare(pSectionNode->GetSectionName(),
          pFileNode->fSectionList.ThisNodeSection()->GetSectionName())) {

      cerr << "###### HadAscii ###### Error in <HAscii::CheckUnambiguitySection> ######\n"
           << "# Section name [" << pSectionNode->GetSectionName() << "] "
           << "# occurs in file: \'" << pFileNode->GetFileName() << "\'\n"
           << "# more than once.                                                      #\n"
           << "########################################################################\n";
      iAmbiguityFlag = 1;
     }

    }
  }
 }

 for (i = 0; i < fFileList.NodeNumber(); i++) {
  fFileList.ToNode(i);
  if (fFileList.ThisNode()) pFileNode = fFileList.ThisNodeFile();
   else { pFileNode = NULL; continue; }

  for (j = 0; j < pFileNode->fSectionList.NodeNumber(); j++) {
   pFileNode->fSectionList.ToNode(j);
   if (pFileNode->fSectionList.ThisNode()) 
    pSectionNode = pFileNode->fSectionList.ThisNodeSection();
   else { pSectionNode = NULL; continue; }

   for (k = i+1; k < fFileList.NodeNumber(); k++) {
    fFileList.ToNode(k);
    if (!fFileList.ThisNode()) continue;

    for (l = 0; l < fFileList.ThisNodeFile()->fSectionList.NodeNumber(); l++) {
     fFileList.ThisNodeFile()->fSectionList.ToNode(l);
     if (!fFileList.ThisNodeFile()->fSectionList.ThisNode()) continue;

     if (i != k || j != l) {

       if (!strcompare(pSectionNode->GetSectionName(),
            fFileList.ThisNodeFile()->fSectionList.ThisNodeSection()->
            GetSectionName())) {
         cerr << "###### HadAscii ###### Error in <HAscii::CheckUnambiguitySection> ######\n"
              << "# Section name [" << pSectionNode->GetSectionName() << "]\n"
              << "# repeats in files: \'" << pFileNode->GetFileName() << "\'\n"
              << "# and \'" << fFileList.ThisNodeFile()->GetFileName() << "\'.\n"
              << "########################################################################\n";
        iAmbiguityFlag = 1;
       }
     }

    }
   }

  }
 }

 if (iAmbiguityFlag) {
  cerr << "# Section names in all opened files must be unambiguous! #\n"
       << "# Correct it and run again.                              #\n"
       << "##########################################################\n";
  GetExit();
  return kFALSE;
 }

 fFileList.RestoreThisNode();
 if (fFileList.ThisNode()) fFileList.ThisNodeFile()->
                          fSectionList.RestoreThisNode();
return kTRUE;
} 
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection* HAscii::CheckActiveSection() 
{
// Additional checking and if necessary reinitialization of section list, now not used.

 fFileList.ToFirstNode();
 while (fFileList.ThisNode()) {
  if (fFileList.ThisNodeFile()->GetFileState() &&
      fFileList.ThisNodeFile()->GetFileChange()) {
   fFileList.ThisNodeFile()->InitSectionList();
   CheckUnambiguitySection();
  }
  fFileList.ThisNodeFile()->fSectionList.ToFirstNode();
  while (fFileList.ThisNodeFile()->fSectionList.ThisNode()) {
   if (fFileList.ThisNodeFile()->fSectionList.ThisNodeSection()->
       GetSectionState()) {
    fpActiveFile = fFileList.ThisNodeFile();
    return (fpActiveSection = fFileList.ThisNodeFile()->
            fSectionList.ThisNodeSection());
   }
   fFileList.ThisNodeFile()->fSectionList.ToNextNode();
  }
  fFileList.ToNextNode();
 }
 fpActiveFile = NULL;
return (fpActiveSection = NULL);
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection* HAscii::SetActiveSection(const Char_t* name) 
{
// Function for section selection. Before reading any data active
// section has to be chosen.

 using namespace StringFunctions;
 Char_t cSecName[128];
 cSecName[0] = '\0';

 strtrunc(cSecName,name);
 if (fpActiveSection && !strcompare(cSecName,
     fpActiveSection->GetSectionName())) {
  if (fReadAllPosition == 0) {
   cerr << "###### HadAscii ###### Warning in <HAscii::SetActiveSection> ######\n"
        << "# Section [" << cSecName << "] is already selected as active.\n"
        << "###################################################################\n";
  } else fReadAllPosition = 0;
  return fpActiveSection;
 }

 fFileList.ToFirstNode();
 while (fFileList.ThisNode()) {
  if (fFileList.ThisNodeFile()->GetFileChange()) { 
   fFileList.ThisNodeFile()->InitSectionList();
   CheckUnambiguitySection();
  }
  fFileList.ThisNodeFile()->fSectionList.ToFirstNode();
  while (fFileList.ThisNodeFile()->fSectionList.ThisNode()) {
   if (!strcompare(cSecName,fFileList.ThisNodeFile()->
        fSectionList.ThisNodeSection()->GetSectionName())) {
    if (fpActiveSection) fpActiveSection->SetSectionState(0);
    fFileList.ThisNodeFile()->fSectionList.ThisNodeSection()->
             SetSectionState(1);
    fpActiveFile = fFileList.ThisNodeFile();
    fReadAllPosition = 0;
    return (fpActiveSection = fFileList.ThisNodeFile()->
            fSectionList.ThisNodeSection());
   }
   fFileList.ThisNodeFile()->fSectionList.ToNextNode();
  }
  fFileList.ToNextNode();
 }

 cerr << "###### HadAscii ###### Warning in <HAscii::SetActiveSection> ######\n"
      << "# No section [" << cSecName << "] in any opened ASCII file\n"
      << "# for this runtime database input.                                #\n"
      << "###################################################################\n";
 // Because of runtime databes two inputs it will not stop it this case.
 // GetExit();
 fpActiveFile = NULL;
 fReadAllPosition = 0;
return (fpActiveSection = NULL);
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::ResetKeyData() 
{
// It resets variables holding keyword information
// like name, length, data string, commentary string.

 fKeyLine[0] = '\0';
 fDataLineLength = 0;
 if (fpDataLine) {
  delete [] fpDataLine;
  fpDataLine = NULL;
 }
 if (fpCommentLine) {
  delete [] fpCommentLine;
  fpCommentLine = NULL;
 }
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::ResetBankData() 
{
// Resetting data bank and bank id.

 using namespace StringFunctions;
 Int_t i;
 fDataCounter = 0;
 switch (fDataBank) {
   case 1: for (i=0; i<1024; i++) fDataInt[i] = 0;
           break;
   case 11: if (fpDataInt) {
             delete [] fpDataInt;
             fpDataInt = NULL;
            }
            break;
   case 2: for (i=0; i<1024; i++) fDataFloat[i] = 0.;
           break;
   case 22: if (fpDataFloat) {
             delete [] fpDataFloat;
             fpDataFloat = NULL;
            }
            break;
   case 3: for (i=0; i<1024; i++) fDataChar[i] = '\0';
           break;
   case 33: if (fpDataChar) {
             delete [] fpDataChar;
             fpDataChar = NULL;
            }
            break;
   case 4: fDataString[0] = '\0';
           break;
   case 44: if (fpDataString) {
             delete [] fpDataString;
             fpDataString = NULL;
            }
            break;
   default: if (fpDataInt || fpDataFloat || fpDataChar || 
                fpDataString || fDataInt[0] || fDataFloat[0] || 
                fDataChar[0] || fDataString[0]) {
             cerr << "###### HadAscii ###### Error in <HAscii::ResetBankData> ######\n"
                  << "# Internal error: Inconsistency in data bank management.     #\n"
                  << "# Please report it to the author!                            #\n"
                  << "##############################################################\n";
             GetExit();
            }
 }
 fDataBank = 0;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckSection() 
{
// Checking if section is selected.

 using namespace StringFunctions;
 if (!fpActiveSection) {
  cerr << "###### HadAscii ###### Error in <HAscii::CheckSection> ######\n"
       << "# None section selected!                                    #\n"
       << "#############################################################\n";
  GetExit();
  return kFALSE;
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckData() 
{
// Checking if keyword just read has any data.

 using namespace StringFunctions;
 if (!CheckSection()) return kFALSE;
 if (!fDataCounter) {
  cerr << "###### HadAscii ###### Error in <HAscii::CheckData> ######\n"
       << "# Keyword \'" << fKeyLine << "\' in section\n"
       << "# [" << fpActiveSection->GetSectionName() << "] has no data!\n"
       << "##########################################################\n";
  GetExit();
 } else return kTRUE;
return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckDataType(Char_t typ) 
{
// It checks whether the call for next data unit of keyword just read
// is of the same type as while keyword reading (this is when
// calling for data without keyword specified but with type
// specified and then it is assumed to read data of last keyword read.

 using namespace StringFunctions;
 if (typ >= 'A' && typ <= 'Z') typ += 32;
 if (typ != 'i' && typ != 'f' && typ != 'c' && typ != 's') {
  cerr << "###### HadAscii ###### Error in <HAscii::CheckDataType> ######\n"
       << "# Data type \'" << typ << "\' not supported (only:\n"
       << "# i - Int_t, f - Float_t, c - Char_t, s - string)!           #\n"
       << "##############################################################\n";
  GetExit();
  return kFALSE;
 }
 if ((typ == 'i' && (fDataBank != 1 && fDataBank != 11)) ||
     (typ == 'f' && (fDataBank != 2 && fDataBank != 22)) ||
     (typ == 'c' && (fDataBank != 3 && fDataBank != 33)) ||
     (typ == 's' && (fDataBank != 4 && fDataBank != 44))) {
  cerr << "###### HadAscii ###### Error in <HAscii::CheckDataType> ######\n"
       << "# Type mismatch: data of keyword \'" << fKeyLine << "\'\n"
       << "# have been started to be read as \'";
  if (fDataBank==1 || fDataBank==11) cerr << "Int_t\'!                   #\n";
   else if (fDataBank==2 || fDataBank==22) cerr << "Float_t\'!                 #\n";
    else if (fDataBank==3 || fDataBank==33) cerr << "Char_t\'!                  #\n";
     else if (fDataBank==4 || fDataBank==44) cerr << "string\'!                  #\n";
  cerr << "##############################################################\n";
  GetExit();
  return kFALSE;
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckDataPosition(Int_t pos) 
{
// Checking if pos is not larger than number of data units;
// in case of string there is always only 1 unit.

 using namespace StringFunctions;
 if (pos >= fDataCounter) {
  cerr << "###### HadAscii ###### Error in <HAscii::CheckDataPosition> ######\n"
       << "# Position (" << pos << ") out of data range:\n"
       << "# keyword \'" << fKeyLine << "\' contains " << fDataCounter << "\n"
       << "# data units of type \'";
  if (fDataBank==1 || fDataBank==11) cerr << "Int_t\'!                                    #\n";
   else if (fDataBank==2 || fDataBank==22) cerr << "Float_t\'!                                  #\n";
    else if (fDataBank==3 || fDataBank==33) cerr << "Char_t\'!                                   #\n";
     else if (fDataBank==4 || fDataBank==44) cerr << "string\'!                                   #\n";
  cerr << "##################################################################\n";
  GetExit();
  return kFALSE;
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::GetActiveKeyword() 
{
// It returns keyword just read (active keyword).

 return fKeyLine;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::GetCommentKeyword() 
{
// Returning of commentary to the last read keyword if there was any.

 return fpCommentLine;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadString() 
{
// Getting string data from data bank if they are present there.

 if (!CheckData() || !CheckDataType('s'))  return NULL;
 if (fDataBank==4) return fDataString; else return fpDataString;
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadString(const Char_t* key) 
{
// Reading data of given keyword key as string.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return NULL;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToString()) {
   cerr << "###### HadAscii ###### Error in <HAscii::ReadString> ######\n"
        << "# Reading of \'string\' data aborted!                       #\n"
        << "###########################################################\n";
   GetExit();
   return NULL;
  }
 }
return ReadString();
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadString(const Char_t* sec, const Char_t* key) 
{
// Reading data of given keyword key in section sec as string.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToString()) {
   cerr << "###### HadAscii ###### Error in <HAscii::ReadString> ######\n"
        << "# Reading of \'string\' data aborted!                       #\n"
        << "###########################################################\n";
   GetExit();
   return NULL;
  }
 }
return ReadString();
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadRemark() 
{
// It returns commentary to the active section if there is any.

 if (CheckSection()) return fpActiveSection->GetSectionComment();
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
void HAscii::SetNumInLine(Int_t num) 
{
// Setting number of digits (characters) in one line to which
// writing of ASCII line will be formatted. Default is 16.

 if (num < 1) {
  cerr << "###### HadAscii ###### Warning in <HAscii::SetNumInLine> ######\n"
       << "# Number of digits (characters) in line must be               #\n"
       << "# of a positive value (now set to default = 16).              #\n"
       << "###############################################################\n";
  fNumInLine = 16;
 } else fNumInLine = num;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAscii::GetNumInLine() 
{
// Number of units (digits, characters) to which written an ASCII line
// will be formatted.

 return fNumInLine;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckOpenedFile() 
{
// Checking if file in write mode is opened.

 if (!fFileWriteState) {
  cerr << "###### HadAscii ###### Warning in <HAscii::CheckOpenedFile> ######\n"
       << "# Writing to file not possible: no file opened!                  #\n"
       << "##################################################################\n";
  return kFALSE;
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::IsReadFile() 
{
// Returns kTRUE if any file in read mode is opened.

if (fFileList.NodeNumber() > 0) return kTRUE;
return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::IsWriteFile() 
{
// Returns kTRUE if a file in write mode is currently opened.

if (fFileWriteState > 0) return kTRUE;
return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::WriteLine(const Char_t* text, Int_t linelen, Int_t keylen) 
{
// Writing ASCII line text with given length linelen taking into 
// consideration that the first line may follow keyword of given length keylen.

 using namespace StringFunctions;
 Int_t i = 0, j = 0, linestart = 0, linecount = 0, textlen;
 Char_t* lineout = NULL;
 textlen = strlength(text);

 if (keylen >= linelen) {
  // if keyword length > line length we go to the next line at once
  fStreamFileWrite << "\n";
  WriteError();
  linecount++;
  keylen = 0;
 }

 if (textlen <= linelen - keylen) {
  if (linecount) fStreamFileWrite << "\\";
  // fStreamFileWrite << text << "\n";
  fStreamFileWrite << text;
  WriteError();
  return kTRUE;
 } else {

   do {
    i = strnextchar(text,j);

    if (i + j - linestart <= linelen - keylen) {
    // substring length optimisation
     j += i;
     i = 0;
    }

    if ((i + j - linestart > linelen - keylen) ||
        (j >= textlen)) {

     // substring will be written to file

     if (j == linestart) {
     // substring too long

      if (i > 1023) {
      // substring longer than upper limit of line length
      // it will not be written (put some separators in it)
       cerr << "###### HadAscii ###### Error in <HAscii::WriteLine> ######\n"
            << "# Line without any separator is too long                 #\n"
            << "# (more than 1023 characters)!                           #\n"
            << "##########################################################\n";
       GetExit();
       return kFALSE;
      } else {
      // substring too long but it will be written
       cerr << "###### HadAscii ###### Error in <HAscii::WriteLine> ######\n"
            << "# Substring of line without separator longer             #\n"
            << "# than given limit.                                      #\n"
            << "##########################################################\n";
       if (linecount == 0) {
        lineout = new Char_t[i+2];
        lineout[0] = '\0';
        keylen = 0;
       } else {
        lineout = new Char_t[i+4];
        lineout[0] = '\0';
        // every 2-nd line needs to start from '\'
        strconcat(lineout,"\\");
       }
       strnconcat(lineout,text,linestart,linestart+i);
       // fStreamFileWrite << lineout << "\n";
       fStreamFileWrite << lineout;
       WriteError();
       linecount++;
       linestart += i;
       i = 0;
       j = linestart;
       delete [] lineout;
       lineout = NULL;
      }

     } else {
     // substring length ok, it will be written

      if (linecount == 0) {
       lineout = new Char_t[j - linestart + 2];
       lineout[0] = '\0';
       keylen = 0;
      } else {
       lineout = new Char_t[j - linestart + 4];
       lineout[0] = '\0';
       // every 2-nd line needs to start from '\'
       strconcat(lineout,"\\");
      }
      strnconcat(lineout,text,linestart,j);
      // fStreamFileWrite << lineout << "\n";
      fStreamFileWrite << lineout;
      WriteError();
      linecount++;
      linestart = j;
      i = 0;
      delete [] lineout;
      lineout = NULL;
     } // eof writing

    }
   } while (j < textlen);

 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::WriteRemark(const Char_t* text, Char_t sign, Int_t linelen) 
{
// Writing remark text starting line from sing # or ; (default) 
// with a line of length linelen.

 if (sign != ';' && sign != '#') {
  cerr << "###### HadAscii ###### Warning in <HAscii::WriteRemark> ######\n"
       << "# Unrecognised character \'" << sign << "\'. Commentary lines               #\n"
       << "# may begin only with \';\' or \'#\' (now set to default = ;).   #\n"
       << "##############################################################\n";
  sign = ';';
 }

 if (CheckOpenedFile()) {
  fStreamFileWrite << "\n" << sign << " ";
  WriteError();
  WriteLine(text,linelen,2);
 } else return kFALSE;

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::WriteSection(const Char_t* name) 
{
// Writing section name, brackets [ ] are added automatically.

 using namespace StringFunctions;
 if (strlength(name) > 127) {
  cerr << "###### HadAscii ###### Error in <HAscii::WriteSection> ######\n"
       << "# Section name to be written is too long                    #\n"
       << "# (more than 127 characters)!                               #\n"
       << "#############################################################\n";
  return kFALSE;
 }
 if (CheckOpenedFile()) {
  fStreamFileWrite << "\n \n [ " << name << " ]\n";
  WriteError();
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CopySection(const Char_t* sec) 
{
// Copying (writing) section sec if there is any. 

 using namespace StringFunctions;
 Long_t lPos = 0, lSecBegin = 0, lSecEnd = 0;
 Char_t c;
 Char_t cSecName[128];
 cSecName[0] = '\0';

 strtrunc(cSecName,sec);
 if (fpActiveSection && !strcompare(cSecName,
     fpActiveSection->GetSectionName())) {
 } else SetActiveSection(sec);

 WriteSection(sec);

 lPos = lSecBegin = fpActiveSection->GetSectionBegin();
 lSecEnd = fpActiveSection->GetSectionEnd();

 fpActiveFile->fStreamFileRead.seekg(lSecBegin);
 if (lPos < lSecEnd)
  if (fpActiveFile->fStreamFileRead.peek() == '\n') {
   fpActiveFile->fStreamFileRead.ignore(1);
   lPos = fpActiveFile->fStreamFileRead.tellg();
  }

 if (lPos < lSecEnd)
  do {
   fpActiveFile->fStreamFileRead.get(c);
   if (fpActiveFile->fStreamFileRead.fail()) {
    fpActiveFile->fStreamFileRead.clear(fpActiveFile->fStreamFileRead.rdstate() &
                                      ~ios::failbit);
    cerr << "###### HadAscii ###### Error in <HAscii::CopySection> ######\n"
         << "# In file \'" << fpActiveFile->GetFileName() << "\' unexpected\n"
         << "# error during copying  section [" << fpActiveSection->GetSectionName() << "]\n"
         << "############################################################\n";
    GetExit();
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    return kFALSE;
   }
   fStreamFileWrite.put(c);
   WriteError();
   lPos = fpActiveFile->fStreamFileRead.tellg();
  } while (lPos < lSecEnd);

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::IsNextKeyword()
{
// Testing if in given section starting from position fReadAllPosition 
// there is any keyword to be read. It is used in case of sequential data 
// reading to control the loop and to stop if there are no more keywords 
// to be read (i.e. we achieved the end of the section). 

 using namespace StringFunctions;
 Long_t lSecBegin = 0, lSecEnd = 0, lPos = 0;
 Char_t cLine[1024], cKey[256];
 cLine[0] = '\0';
 cKey[0] = '\0';
 Bool_t fNextKey = kFALSE;
 
 if (!fpActiveSection || !fpActiveFile) {
  cerr << "###### HadAscii ###### Error in <HAscii::IsNextKeyword> ######\n"
       << "# None section selected!                                     #\n"
       << "##############################################################\n";
  GetExit();
  return kFALSE;
 }

 lPos = lSecBegin = fpActiveSection->GetSectionBegin();
 lSecEnd = fpActiveSection->GetSectionEnd();

 if (fReadAllPosition != 0) lPos = fReadAllPosition;
 fpActiveFile->fStreamFileRead.seekg(lPos);
 
 do {
  if (lPos < lSecEnd)
   if (fpActiveFile->fStreamFileRead.peek() == '\n') 
    fpActiveFile->fStreamFileRead.ignore(1);
   
  lPos = fpActiveFile->fStreamFileRead.tellg();
  if (lPos < lSecEnd) {
   if ((lSecEnd - lPos) >= 1023) fpActiveFile->fStreamFileRead.get(cLine,1023);
    else fpActiveFile->fStreamFileRead.get(cLine,(lSecEnd - lPos + 1));
   if (fpActiveFile->fStreamFileRead.fail()) {
    fpActiveFile->fStreamFileRead.clear(fpActiveFile->fStreamFileRead.rdstate() &
                                      ~ios::failbit);
    cerr << "###### HadAscii ###### Error in <HAscii::IsNextKeyword> ######\n"
         << "# In file \'" << fpActiveFile->GetFileName() << "\' unexpected\n"
         << "# error during reading in section [" << fpActiveSection->GetSectionName() << "]\n"
         << "##############################################################\n";
    GetExit();
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    return fNextKey;
   }

   if (strfirstchar(cLine) == ';' ||
       strfirstchar(cLine) == '#' ||
       strfirstchar(cLine) == '\\' ||
       strfirstchar(cLine) == '=' ||
       !strchr(cLine,'=') ||
       strlength(cLine) == 0 ||
       strempty(cLine)) {
    cLine[0] = '\0';
   } else {
    strbefore(cKey,cLine,'=');
    if (cKey != NULL && strempty(cKey) == kFALSE)
     fNextKey = kTRUE;
   }

   lPos = fpActiveFile->fStreamFileRead.tellg();
   if (lPos >= lSecEnd) return fNextKey;
  } else {
   return fNextKey;
  }
 } while (lPos < lSecEnd && fNextKey == kFALSE);

return fNextKey;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadKeywordLine(const Char_t* name) 
{
// Reading keyword name and keyword data as string from ASCII file.
// If keyword is not specified the sequential reading is done. Reading 
// starts then from position fReadAllPosition and every next call of this
// function makes reading of next keyword (with data) until the end of section.
// If you want to read the section sequentially again you nedd to set it
// again as active section (and then fReadAllPosition is resetted).

 using namespace StringFunctions;
 Int_t i;
 Long_t lSecBegin = 0, lSecEnd = 0;
 Long_t lPos = 0;
 Char_t cName[128], cLine[1024], cKey[128], cKeyUnpacked[256], cData[1024];
 Char_t *pData = NULL;
 cName[0] = '\0';
 cLine[0] = '\0';
 cKey[0] = '\0';
 cKeyUnpacked[0] = '\0';
 cData[0] = '\0';
 Bool_t fReadAll = kFALSE;

 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 if (!fpActiveSection || !fpActiveFile) {
  cerr << "###### HadAscii ###### Error in <HAscii::ReadKeywordLine> ######\n"
       << "# None section selected!                                       #\n"
       << "################################################################\n";
  GetExit();
  return NULL;
 }
 strtrunc(cName,name);
 if (name==NULL || strempty(cName)) fReadAll = kTRUE;

 lPos = lSecBegin = fpActiveSection->GetSectionBegin();
 lSecEnd = fpActiveSection->GetSectionEnd();

 if (fReadAll) {
  if (fReadAllPosition == 0) fReadAllPosition = lSecBegin;
  fpActiveFile->fStreamFileRead.seekg(fReadAllPosition);
 } else fpActiveFile->fStreamFileRead.seekg(lSecBegin);

 do {
  if (lPos < lSecEnd)
   if (fpActiveFile->fStreamFileRead.peek() == '\n') 
    fpActiveFile->fStreamFileRead.ignore(1);
  lPos = fpActiveFile->fStreamFileRead.tellg();
  if (fReadAll) fReadAllPosition = lPos;
  if (lPos < lSecEnd) {
   if ((lSecEnd - lPos) >= 1023) fpActiveFile->fStreamFileRead.get(cLine,1023);
    else fpActiveFile->fStreamFileRead.get(cLine,(lSecEnd - lPos + 1));
   if (fpActiveFile->fStreamFileRead.fail()) {
    fpActiveFile->fStreamFileRead.clear(fpActiveFile->fStreamFileRead.rdstate() & 
                                      ~ios::failbit);
    cerr << "###### HadAscii ###### Error in <HAscii::ReadKeywordLine> ######\n"
         << "# In file \'" << fpActiveFile->GetFileName() << "\' unexpected\n"
         << "# error during reading in section [" << fpActiveSection->GetSectionName() << "]\n"
         << "################################################################\n";
    GetExit();
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    return NULL;
   }

   if (strfirstchar(cLine) == ';' || 
       strfirstchar(cLine) == '#' || 
       strfirstchar(cLine) == '\\' ||
       strfirstchar(cLine) == '=' ||
       !strchr(cLine,'=') || 
       strlength(cLine) == 0 ||
       strempty(cLine)) {
    cLine[0] = '\0';
   } else {
    strbefore(cKeyUnpacked,cLine,'=');
    strtrunc(cKey,cKeyUnpacked);
    if (fReadAll || !strcompare(cName,cKey)) {
     strcopy(fKeyLine,cKey); // copying keyword
     strafter(cData,cLine,'=');
     fDataLineLength = strlength(cData) + 1;
     fpDataLine = new Char_t[fDataLineLength];
     strcopy(fpDataLine,cData); // copying data

     do {
      lPos = fpActiveFile->fStreamFileRead.tellg();
      if (fReadAll) fReadAllPosition = lPos;
      if (lPos < lSecEnd)
       if (fpActiveFile->fStreamFileRead.peek() == '\n') {
        fpActiveFile->fStreamFileRead.ignore(1);
        lPos = fpActiveFile->fStreamFileRead.tellg();
       }
      if (fReadAll) fReadAllPosition = lPos;
      if (lPos < lSecEnd) {
       cLine[0] = '\0';
       cData[0] = '\0';
       if ((lSecEnd - lPos) >= 1023) fpActiveFile->fStreamFileRead.get(cLine,1023);
        else fpActiveFile->fStreamFileRead.get(cLine,(lSecEnd - lPos + 1));
       if (fpActiveFile->fStreamFileRead.fail()) {
        fpActiveFile->fStreamFileRead.clear(fpActiveFile->
                     fStreamFileRead.rdstate() & ~ios::failbit);
        cerr << "###### HadAscii ###### Error in <HAscii::ReadKeywordLine> ######\n"
             << "# In file \'" << fpActiveFile->GetFileName() << "\' unexpected\n"
             << "# error during reading data for keyword \'" << fKeyLine << "\'\n"
             << "# in section [" << fpActiveSection->GetSectionName() << "]\n"
             << "################################################################\n";
        GetExit();
        fpActiveFile = NULL;
        fpActiveSection = NULL;
        if (fpDataLine) {
         delete [] fpDataLine;
         fpDataLine = NULL;
        }
        fDataLineLength = 0; 
        for (i=0; i<128; i++) fKeyLine[i] = '\0';
        return NULL;
       }
       if (strfirstchar(cLine) == '#' ||
           strlength(cLine) == 0 ||
           strempty(cLine)) {
       // ignore it
       } else if(strfirstchar(cLine) == ';') {
        if (fpCommentLine) {
         pData = fpCommentLine;
        }
        strafter(cData,cLine,';');
        fpCommentLine = new Char_t[strlength(pData) + strlength(cData) + 1];
        fpCommentLine[0] = '\0';
        if (pData) {
         strcopy(fpCommentLine,pData);
         strconcat(fpCommentLine,cData);
         delete [] pData;
         pData = NULL;
        } else {
         strcopy(fpCommentLine,cData);
        }
       } else if (strfirstchar(cLine) == '\\') {
        pData = fpDataLine;
        strafter(cData,cLine,'\\');
        fDataLineLength = strlength(pData) + strlength(cData) + 1;
        fpDataLine = new Char_t[fDataLineLength];
        fpDataLine[0] = '\0';
        strcopy(fpDataLine,pData);
        strconcat(fpDataLine,cData);
        delete [] pData;
        pData = NULL;
       } else return fpDataLine;
      }
     } while (lPos < lSecEnd);
     return fpDataLine;

    }
   }

   lPos = fpActiveFile->fStreamFileRead.tellg();
   if (fReadAll) fReadAllPosition = lPos;
   if (lPos >= lSecEnd) {
    cerr << "###### HadAscii ###### Error in <HAscii::ReadKeywordLine> ######\n"
         << "# In section [" << fpActiveSection->GetSectionName() << "]\n"
         << "# in file \'" << fpActiveFile->GetFileName() << "\'\n"
         << "# keyword \'" << cName << "\' not found!\n"
         << "################################################################\n";
    GetExit();
    return NULL;
   }
   cLine[0] = '\0';
   cKey[0] = '\0';
   cKeyUnpacked[0] = '\0';
  } else {
   cerr << "###### HadAscii ###### Error in <HAscii::ReadKeywordLine> ######\n"
        << "# In section [" << fpActiveSection->GetSectionName() << "]\n"
        << "# in file \'" << fpActiveFile->GetFileName() << "\'\n"
        << "# keyword \'" << cName << "\' not found!\n"
        << "################################################################\n";
   GetExit();
   return NULL;
  }
 } while (lPos < lSecEnd);

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t* HAscii::ConvertToInt() 
{
// Conversion of string of data stored in fpDataLine to units of Int_t type.

 using namespace StringFunctions;
 Int_t i = 0, j = 0;
 Int_t iBox = 0;
 Int_t *pDataIntTemp = NULL;
 Char_t cUnit[30];
 cUnit[0] = '\0';

 if (fpDataLine) 
  while (strfirstchar(fpDataLine)) {
   strunit(cUnit,fpDataLine);
   j = 0;
   while (cUnit[j]) {

    if (cUnit[0]=='0' || ((cUnit[0]=='-' || cUnit[0]=='+') && 
        cUnit[1]=='0')) {
     if (!((cUnit[j] >= '0' && cUnit[j] <= '7') || 
         cUnit[j]=='-' || cUnit[j]=='+')) {
      cerr << "###### HadAscii ###### Error in <HAscii::ConvertToInt> ######\n"
           << "# Keyword \'" << fKeyLine << "\' in section [" << fpActiveSection->GetSectionName() << "]\n"
           << "# in file \'" << fpActiveFile->GetFileName() << "\' contains a digit in\n"
           << "# octal system but with characters out of octal system!\n"
           << "#############################################################\n";
      GetExit();
      ResetKeyData();
      ResetBankData();
      return NULL;
     }
    } else if ((cUnit[0]=='0' && (cUnit[1]=='x' || cUnit[1]=='X')) ||
               ((cUnit[0]=='-' || cUnit[0]=='+') && 
                (cUnit[1]=='0' && (cUnit[2]=='x' || cUnit[2]=='X')))) {
     if (!((cUnit[j] >= '0' && cUnit[j] <= '9') || cUnit[j]=='a' || 
         cUnit[j]=='A' ||
           cUnit[j]=='b' || cUnit[j]=='B' || cUnit[j]=='c' || cUnit[j]=='C' ||
           cUnit[j]=='d' || cUnit[j]=='D' || cUnit[j]=='e' || cUnit[j]=='E' ||
           cUnit[j]=='x' || cUnit[j]=='X' || cUnit[j]=='-' || cUnit[j]=='+')) {
      cerr << "###### HadAscii ###### Error in <HAscii::ConvertToInt> ######\n"
           << "# Keyword \'" << fKeyLine << "\' in section [" << fpActiveSection->GetSectionName() << "]\n"
           << "# in file \'" << fpActiveFile->GetFileName() << "\' contains a digit\n"
           << "# in hexadecimal system but with characters                 #\n"
           << "# out of hexadecimal system!                                #\n"
           << "#############################################################\n";
      GetExit();
      ResetKeyData();
      ResetBankData();
      return NULL;
     }
    } else if (!((cUnit[j] >= '0' && cUnit[j] <= '9') || 
               cUnit[j]=='e' || cUnit[j]=='E' ||
          cUnit[j]=='.' || cUnit[j]=='-' || cUnit[j]=='+')) {
     cerr << "###### HadAscii ###### Error in <HAscii::ConvertToInt> ######\n"
          << "# Keyword \'" << fKeyLine << "\' in section [" << fpActiveSection->GetSectionName() << "]\n"
          << "# in file \'" << fpActiveFile->GetFileName() << "\' contains a digit\n"
          << "# with characters out of decimal system!                    #\n"
          << "#############################################################\n";
     GetExit();
     ResetKeyData();
     ResetBankData();
     return NULL;
    }

    j++;
   }

   switch (i / 1024) {
    case 0: if (fDataBank != 1) fDataBank = 1;
            if (strchr(cUnit,'.')) {
             fDataInt[i++] = (Int_t)atof(cUnit);
            } else {
             fDataInt[i++] = strtol(cUnit,NULL,0);
            }
            fDataCounter++;
            cUnit[0] = '\0';
            break;
    case 1: if (fDataBank != 11) {
             fDataBank = 11;
             iBox = 1;
             fpDataInt = new Int_t[2048];
             for (j = 0; j < 1024; j++) { fpDataInt[j] = fDataInt[j]; 
                                          fDataInt[j] = 0; }
             for (j = 1024; j < 2048; j++) fpDataInt[j] = 0;
            }
            fpDataInt[i++] = atoi(cUnit);
            fDataCounter++;
            cUnit[0] = '\0';
            break;
   default: if (iBox < i/1024) {
             pDataIntTemp = fpDataInt;
             fpDataInt = new Int_t[1024*(1+i/1024)];
             for (j = 0; j < 1024*(1+iBox); j++) fpDataInt[j] = pDataIntTemp[j];
             for (j = 1024*(1+iBox); j < 1024*(1+i/1024); j++) fpDataInt[j] = 0;
             iBox = i/1024;
             delete [] pDataIntTemp;
             pDataIntTemp = NULL;
            }
            fpDataInt[i++] = atoi(cUnit);
            fDataCounter++;
            cUnit[0] = '\0';
            break;
   }

  }

if (fDataBank == 1) return fDataInt; 
 else if (fDataBank == 11) return fpDataInt;
  else return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t* HAscii::ConvertToFloat() 
{
// Conversion of string of data stored in fpDataLine to units of Float_t type.

 using namespace StringFunctions;
 Int_t i = 0, j = 0;
 Int_t iBox = 0;
 Float_t *pDataFloatTemp = NULL;
 Char_t cUnit[30];
 cUnit[0] = '\0';

 if (fpDataLine)
  while (strfirstchar(fpDataLine)) {
   strunit(cUnit,fpDataLine);
   j = 0;
   while (cUnit[j]) {
    if (!((cUnit[j] >= '0' && cUnit[j] <= '9') || cUnit[j]=='e' || 
        cUnit[j]=='E' ||
          cUnit[j]=='.' || cUnit[j]=='-' || cUnit[j]=='+')
       ) {
     cerr << "###### HadAscii ###### Error in <HAscii::ConvertToFloat> ######\n"
          << "# Keyword \'" << fKeyLine << "\' in section [" << fpActiveSection->GetSectionName() << "]\n"
          << "# in file \'" << fpActiveFile->GetFileName() << "\' contains\n"
          << "# non-digit characters!                                       #\n"
          << "###############################################################\n";
     GetExit();
     ResetKeyData();
     ResetBankData();
     return NULL;
    }
    j++;
   }

   switch (i / 1024) {
    case 0: if (fDataBank != 2) fDataBank = 2;
            fDataFloat[i++] = atof(cUnit);
            fDataCounter++;
            cUnit[0] = '\0';
            break;
    case 1: if (fDataBank != 22) {
             fDataBank = 22;
             iBox = 1;
             fpDataFloat = new Float_t[2048];
             for (j = 0; j < 1024; j++) { fpDataFloat[j] = fDataFloat[j]; 
                                          fDataFloat[j] = 0.; }
             for (j = 1024; j < 2048; j++) fpDataFloat[j] = 0.;
            }
            fpDataFloat[i++] = atof(cUnit);
            fDataCounter++;
            cUnit[0] = '\0';
            break;
   default: if (iBox < i/1024) {
             pDataFloatTemp = fpDataFloat;
             fpDataFloat = new Float_t[1024*(1+i/1024)];
             for (j = 0; j < 1024*(1+iBox); j++) 
              fpDataFloat[j] = pDataFloatTemp[j];
             for (j = 1024*(1+iBox); j < 1024*(1+i/1024); j++) 
              fpDataFloat[j] = 0.;
             iBox = i/1024;
             delete [] pDataFloatTemp;
             pDataFloatTemp = NULL;
            }
            fpDataFloat[i++] = atof(cUnit);
            fDataCounter++;
            cUnit[0] = '\0';
            break;
   }

  }

if (fDataBank == 2) return fDataFloat; 
 else if (fDataBank == 22) return fpDataFloat;
  else return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ConvertToChar() 
{
// Conversion of string of data stored in fpDataLine to units of Char_t type.
// Char_t unit is a single Char_t separated in string by white space or comma.

 using namespace StringFunctions;
 Int_t i = 0, j = 0;
 Int_t iBox = 0;
 Char_t *pDataCharTemp = NULL;
 Char_t cUnit[30];
 cUnit[0] = '\0';

 if (fpDataLine)
  while (strfirstchar(fpDataLine)) {
   strunit(cUnit,fpDataLine);
   if (strlength(cUnit) > 1) {
    cerr << "###### HadAscii ###### Error in <HAscii::ConvertToChar> ######\n"
         << "# Keyword \'" << fKeyLine << "\' in section [" << fpActiveSection->GetSectionName() << "]\n"
         << "# in file \'" << fpActiveFile->GetFileName() << "\' contains\n"
         << "# strings longer than 1 character!                           #\n"
         << "##############################################################\n";
    GetExit();
    ResetKeyData();
    ResetBankData();
    return NULL;
   }

   switch (i / 1024) {
    case 0: if (fDataBank != 3) fDataBank = 3;
            fDataChar[i++] = cUnit[0];
            fDataCounter++;
            cUnit[0] = '\0';
            break;
    case 1: if (fDataBank != 33) {
             fDataBank = 33;
             iBox = 1;
             fpDataChar = new Char_t[2048];
             for (j = 0; j < 1024; j++) { fpDataChar[j] = fDataChar[j]; 
                                          fDataChar[j] = '\0'; }
             for (j = 1024; j < 2048; j++) fpDataChar[j] = '\0';
            }
            fpDataChar[i++] = cUnit[0];
            fDataCounter++;
            cUnit[0] = '\0';
            break;
   default: if (iBox < i/1024) {
             pDataCharTemp = fpDataChar;
             fpDataChar = new Char_t[1024*(1+i/1024)];
             for (j = 0; j < 1024*(1+iBox); j++) 
              fpDataChar[j] = pDataCharTemp[j];
             for (j = 1024*(1+iBox); j < 1024*(1+i/1024); j++) 
              fpDataChar[j] = '\0';
             iBox = i/1024;
             delete [] pDataCharTemp;
             pDataCharTemp = NULL;
            }
   }

  }

if (fDataBank == 3) return fDataChar; 
 else if (fDataBank == 33) return fpDataChar;
  else return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ConvertToString() 
{
// Conversion of string of data stored in fpDataLine to string unit.
// If string begins and ends with quotation marks they are removed.
// String unit is a set of characters terminated by '\0'. 

 using namespace StringFunctions;
 Int_t i = 0, j = 0;
 Char_t *pDLine = NULL;
 if (fpDataLine)
  if (strlength(fpDataLine) < 1024) {
   if (fDataBank != 4) fDataBank = 4;
   if (strfirstchar(fpDataLine)=='\"' && strlastchar(fpDataLine)=='\"') {
    pDLine = new Char_t[strlength(fpDataLine) + 1];
    pDLine[0] = '\0';
    strafter(pDLine,fpDataLine,'\"');
    i = strlength(pDLine) - 1;
    while (pDLine[i] != '\"') { i--; }
    j = 0;
    while (j<i) {
     fDataString[j] = pDLine[j]; 
     j++;
    }
    fDataString[j] = '\0';
    delete [] pDLine;
    pDLine = NULL;
   } else {
    strcopy(fDataString,fpDataLine);
   }
   fDataCounter++;
   return fDataString;
  } else {
   if (fDataBank != 44) fDataBank = 44;
   if (strfirstchar(fpDataLine)=='\"' && strlastchar(fpDataLine)=='\"') {
    pDLine = new Char_t[strlength(fpDataLine) + 1];
    pDLine[0] = '\0';
    strafter(pDLine,fpDataLine,'\"');
    i = strlength(pDLine) - 1;
    while (pDLine[i] != '\"') { i--; }
    j = 0;
    while (j<i) {
     fDataString[j] = pDLine[j];
     j++;
    }
    fDataString[j] = '\0';
    delete [] pDLine;
    pDLine = NULL;
   } else {
    fpDataString = new Char_t[strlength(fpDataLine) + 1];
    fpDataString[0] = '\0';
    strcopy(fpDataString,fpDataLine);
   }
   fDataCounter++;
   return fpDataString;
  }
return NULL;
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiKey
//
// Class inherited from HAscii for formatted reading/writing ASCII files.
// The format of data is similar to M$ Windows .ini files (data divided
// into [sections] and described by keywords= (see: documentation).
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiKey)

//----------------------------------------------------------------------------
HAsciiKey::HAsciiKey() : HAscii() 
{
// Default constructor. It does nothigh.

}
//============================================================================

//----------------------------------------------------------------------------
HAsciiKey::~HAsciiKey() 
{
// Destructor.

}
//============================================================================

//----------------------------------------------------------------------------
HAsciiKey::HAsciiKey(const HAsciiKey& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiKey::HAsciiKey> ######\n"
      << "# Object cannot be copied!                                  #\n"
      << "#############################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiKey& HAsciiKey::operator=(const HAsciiKey& source) {
 using namespace StringFunctions;
 if (this != &source) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::operator=> ######\n"
       << "# Object cannot be initialised by assignment!               #\n"
       << "#############################################################\n";
  GetExit();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
void* HAsciiKey::Read() 
{
// Reading data from data bank (so previously converted to given type)
// returning void pointer to begin of them.

 if (!CheckData()) return NULL;
 switch (fDataBank) {
   case 1: return (void*)fDataInt;
   case 11: return (void*)fpDataInt;
   case 2: return (void*)fDataFloat;
   case 22: return (void*)fpDataFloat;
   case 3: return (void*)fDataChar;
   case 33: return (void*)fpDataChar;
   case 4: return (void*)fDataString;
   case 44: return (void*)fpDataString;
 }
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
void* HAsciiKey::Read(Char_t typ) 
{
// Reading data from keyword previously read, converting to given type typ
// and returning void pointer to begin of them.

 if (!CheckData()) return NULL;
 if (!CheckDataType(typ)) return NULL;
 if (typ >= 'A' && typ <= 'Z') typ += 32;

 switch (typ) {
   case 'i': if (fDataBank==1) return (void*)fDataInt;
              else return (void*)fpDataInt;
   case 'f': if (fDataBank==2) return (void*)fDataFloat;
              else return (void*)fpDataFloat;
   case 'c': if (fDataBank==3) return (void*)fDataChar;
              else return (void*)fpDataChar;
   case 's': if (fDataBank==4) return (void*)fDataString;
              else return (void*)fpDataString;
 }
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiKey::ReadInt(Int_t pos) 
{
// Reading Int_t number at the position pos (from data bank) for keyword just read.

 if (!CheckData() || !CheckDataType('i') ||
     !CheckDataPosition(pos)) return 0;
 if (fDataBank==1) return fDataInt[pos]; else return fpDataInt[pos];
return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiKey::ReadFloat(Int_t pos) 
{
// Readnig Float_t number at the position pos (from data bank) for keyword just read.

 if (!CheckData() || !CheckDataType('f') ||
     !CheckDataPosition(pos)) return 0.;
 if (fDataBank==2) return fDataFloat[pos]; else return fpDataFloat[pos];
return 0.0;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiKey::ReadChar(Int_t pos) 
{
// Reading Char_t character at the position pos (from data bank) for keyword just read. 

 if (!CheckData() || !CheckDataType('c') ||
     !CheckDataPosition(pos)) return '\0';
 if (fDataBank==3) return fDataChar[pos]; else return fpDataChar[pos];
return '\0';
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiKey::ReadInt(const Char_t* key, Int_t pos) 
{
// Reading Int_t number at the position pos in keyword key.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return 0;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToInt()) {
   cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadInt> ######\n"
        << "# Reading of \'Int_t\' data aborted!                        #\n"
        << "###########################################################\n";
   GetExit();
   return 0;
  }
 }
return ReadInt(pos); 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiKey::ReadInt(const Char_t* sec, const Char_t* key, Int_t pos) 
{
// Reading Int_t number at the position pos in keyword key in section sec.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToInt()) {
   cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadInt> ######\n"
        << "# Reading of \'Int_t\' data aborted!                        #\n"
        << "###########################################################\n";
   GetExit();
   return 0;
  }
 }
return ReadInt(pos);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(Int_t* putin, const Char_t* key) 
{
// Reading the whole contents of keyword key as Int_t units and copying it 
// to place (array) pointed by putin.

 Int_t i;
 ReadInt(key,0);
 for (i = 0; i < fDataCounter; i++) putin[i] = ReadInt(i);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(TArrayI* putin, const Char_t* key) 
{
// Reading the whole contents of keyword key as Int_t units and copying it 
// to TArrayI structure pointed by putin.

 return ReadInt(&putin[0], key);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(Int_t* putin, const Char_t* sec, const Char_t* key) 
{
// Reading the whole contents of keyword key in section sec as Int_t units
// and copying it to place (array) pointed by putin.

 Int_t i;
 ReadInt(sec,key,0);
 for (i = 0; i < fDataCounter; i++) putin[i] = ReadInt(i);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(TArrayI* putin, const Char_t* sec, const Char_t* key) 
{
// Reading the whole contents of keyword key in section sec as Int_t units
// and copying it to TArrayI structure pointed by putin.

 return ReadInt(&putin[0], sec, key);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(Int_t* putin, const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key as Int_t units
// and copying to putin.

 using namespace StringFunctions;
 if (number <= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadInt> ######\n"
       << "# Argument number = " << number << " must have positive value!\n"
       << "###########################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadInt(key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadInt(i);
 } else {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadInt> ######\n"
       << "# Argument number = " << number << " exceeds number\n"
       << "# of Int_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << "# in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "#############################################################\n";
  for (i = 0; i < fDataCounter; i++) putin[i] = ReadInt(i);
 } 
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(TArrayI* putin, const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key as Int_t units
// and copying to TArrayI structure pointed by putin.

 return ReadInt(&putin[0], key, number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(Int_t* putin, const Char_t* sec, 
                         const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key in section sec
// as Int_t units and copying to putin.

 using namespace StringFunctions;
 if (number <= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadInt> ######\n"
       << "# Argument number = " << number << " must have positive value!\n"
       << "###########################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadInt(sec,key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadInt(i);
 } else {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadInt> ######\n"
       << "# Argument number = " << number << " exceeds number\n"
       << "# of Int_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << "# in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "#############################################################\n";
  for (i = 0; i < fDataCounter; i++) putin[i] = ReadInt(i);
 }  
return kTRUE; 
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(TArrayI* putin, const Char_t* sec, 
                           const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key in section sec
// as Int_t units and copying to TArrayI structure pointed by putin.

 return ReadInt(&putin[0], sec, key, number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(Int_t* putin, const Char_t* key, Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword 
// as Int_t units and copying to putin.

 using namespace StringFunctions;
 if (nrfrom < 0 || number < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadInt> ######\n"
       << "# Argument nrfrom = " << nrfrom << " and number = " << number << "\n"
       << "# must have positive value!                               #\n"
       << "###########################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadInt(key,0);
 if ((nrfrom + number) > fDataCounter) {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadInt> ######\n"
       << "# End value nrfrom + number = " << nrfrom+number << "\n"
       << "# exceeds number of Int_t data units (" << fDataCounter << ")\n"
       << "# of keyword \'" << key << "\' in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "#############################################################\n";
  for (i = nrfrom; i < fDataCounter; i++) putin[i] = ReadInt(i);
 } else {
  for (i = nrfrom; i < nrfrom + number; i++) putin[i] = ReadInt(i);
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(TArrayI* putin, const Char_t* key, 
                           Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword 
// as Int_t units and copying to TArrayI structure pointed by putin.

 return ReadInt(&putin[0], key, nrfrom, number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(Int_t* putin, const Char_t* sec, const Char_t* key, 
                         Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword 
// in section sec as Int_t units and copying to putin.

 using namespace StringFunctions;
 if (nrfrom < 0 || number < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadInt> ######\n"
       << "# Argument nrfrom = " << nrfrom << " and number = " << number << "\n"
       << "# must have positive value!                               #\n"
       << "###########################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadInt(sec,key,0);
 if ((nrfrom + number) > fDataCounter) {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadInt> ######\n"
       << "# End value nrfrom + number = " << nrfrom+number << " exceeds\n"
       << "# number of Int_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << "# in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "#############################################################\n";
  for (i = nrfrom; i < fDataCounter; i++) putin[i] = ReadInt(i);
 } else {
  for (i = nrfrom; i < nrfrom + number; i++) putin[i] = ReadInt(i);
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadInt(TArrayI* putin, const Char_t* sec, 
                           const Char_t* key, Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword
// in section sec as Int_t units and copying to TArrayI structure pointed by putin.

 return ReadInt(&putin[0], sec, key, nrfrom, number);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiKey::ReadFloat(const Char_t* key, Int_t pos) 
{
// Reading Float_t number at the position pos in keyword key.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return 0.;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToFloat()) {
   cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadFloat> ######\n"
        << "# Reading of \'Float_t\' data aborted!                        #\n"
        << "#############################################################\n";
   GetExit();
   return 0.;
  }
 }
return ReadFloat(pos);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiKey::ReadFloat(const Char_t* sec, const Char_t* key, Int_t pos) 
{
// Reading Float_t number at the position pos in keyword key in section sec.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToFloat()) {
   cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadFloat> ######\n"
        << "# Reading of \'Float_t\' data aborted!                        #\n"
        << "#############################################################\n";
   GetExit();
   return 0.;
  }
 }
return ReadFloat(pos);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(Float_t* putin, const Char_t* key) 
{
// Reading the whole contents of keyword key as Float_t units and copying it
// to place (array) pointed by putin.

 Int_t i;
 ReadFloat(key,0);
 for (i = 0; i < fDataCounter; i++) putin[i] = ReadFloat(i);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(TArrayF* putin, const Char_t* key) 
{
// Reading the whole contents of keyword key as Float_t units and copying it
// to TArrayF structure pointed by putin.

 return ReadFloat(&putin[0], key);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(Float_t* putin, const Char_t* sec, const Char_t* key) 
{
// Reading the whole contents of keyword key in section sec as Float_t units
// and copying it to place (array) pointed by putin.

 Int_t i;
 ReadFloat(sec,key,0);
 for (i = 0; i < fDataCounter; i++) putin[i] = ReadFloat(i);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(TArrayF* putin, const Char_t* sec, 
                             const Char_t* key) 
{
// Reading the whole contents of keyword key in section sec as Float_t units
// and copying it to TArrayF structure pointed by putin.

 return ReadFloat(&putin[0], sec, key);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(Float_t* putin, const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key as Float_t units
// and copying to putin.

 using namespace StringFunctions;
 if (number <= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadFloat> ######\n"
       << "# Argument number = " << number << " must have positive value!\n"
       << "#############################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadFloat(key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadFloat(i);
 } else {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadFloat> ######\n"
       << "# Argument number = " << number << " exceeds number\n"
       << "# of Float_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << "# in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "###############################################################\n";
  for (i = 0; i < fDataCounter; i++) putin[i] = ReadFloat(i);
 } 
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(TArrayF* putin, const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key as Float_t units
// and copying to TArrayF structure pointed by putin.

 return ReadFloat(&putin[0], key, number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(Float_t* putin, const Char_t* sec, 
                           const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key in section sec
// as Float_t units and copying to putin.

 using namespace StringFunctions;
 if (number <= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadFloat> ######\n"
       << "# Argument number = " << number << " must have positive value!\n"
       << "#############################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadFloat(sec,key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadFloat(i);
 } else {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadFloat> ######\n"
       << "# Argument number = " << number << " exceeds number\n"
       << "# of Float_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << "# in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "###############################################################\n";
  for (i = 0; i < fDataCounter; i++) putin[i] = ReadFloat(i);
 }  
return kTRUE; 
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(TArrayF* putin, const Char_t* sec, 
                             const Char_t* key, Int_t number) 
{
// Reading the given number of units from keyword key in section sec
// as Float_t units and copying to TArrayF structure pointed by putin.

 return ReadFloat(&putin[0], sec, key, number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(Float_t* putin, const Char_t* key, 
                           Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword
// as Float_t units and copying to putin.

 using namespace StringFunctions;
 if (nrfrom < 0 || number < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadFloat> ######\n"
       << "# Argument nrfrom = " << nrfrom << " and number = " << number << "\n"
       << "# must have positive value!                                 #\n"
       << "#############################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadFloat(key,0);
 if ((nrfrom + number) > fDataCounter) {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadFloat> ######\n"
       << "# End value nrfrom + number = " << nrfrom+number << " exceeds number\n"
       << "# of Float_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << "# in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "###############################################################\n";
  for (i = nrfrom; i < fDataCounter; i++) putin[i] = ReadFloat(i);
 } else {
  for (i = nrfrom; i < nrfrom + number; i++) putin[i] = ReadFloat(i);
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(TArrayF* putin, const Char_t* key, 
                             Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword
// as Float_t units and copying to TArrayF structure pointed by putin.

 return ReadFloat(&putin[0], key, nrfrom, number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(Float_t* putin, const Char_t* sec, 
                           const Char_t* key, Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword
// in section sec as Float_t units and copying to putin.

 using namespace StringFunctions;
 if (nrfrom < 0 || number < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadFloat> ######\n"
       << "# Argument nrfrom = " << nrfrom << " and number = " << number << "\n"
       << "# must have positive value!                                 #\n"
       << "#############################################################\n";
  GetExit();
  return kFALSE;
 }
 Int_t i;
 ReadFloat(sec,key,0);
 if ((nrfrom + number) > fDataCounter) {
  cerr << "###### HadAscii ###### Warning in <HAsciiKey::ReadFloat> ######\n"
       << "# End value nrfrom + number = " << nrfrom+number << " exceeds number\n"
       << " of Float_t data units (" << fDataCounter << ") of keyword \'" << key << "\'\n"
       << " in file \'" << fpActiveFile->GetFileName() << "\'!\n"
       << "###############################################################\n";
  for (i = nrfrom; i < fDataCounter; i++) putin[i] = ReadFloat(i);
 } else {
  for (i = nrfrom; i < nrfrom + number; i++) putin[i] = ReadFloat(i);
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::ReadFloat(TArrayF* putin, const Char_t* sec, 
                             const Char_t* key, Int_t nrfrom, Int_t number) 
{
// Reading the given number of units starting from position nrfrom from keyword
// in section sec as Float_t units and copying to TArrayF structure pointed by putin.

 return ReadFloat(&putin[0], sec, key, nrfrom, number);
}
//============================================================================

//----------------------------------------------------------------------------
void* HAsciiKey::Read(const Char_t* key, Char_t typ) 
{
// Returning void pointer to the beginning of data in keyword key of type typ.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (typ >= 'A' && typ <= 'Z') typ += 32;
 if (!CheckSection()) return NULL;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  switch (typ) {
    case 'i': if (!ConvertToInt()) { 
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'Int_t\' data aborted!                     #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
    case 'f': if (!ConvertToFloat()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'Float_t\' data aborted!                   #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
    case 'c': if (!ConvertToChar()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'Char_t\' data aborted!                    #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
    case 's': if (!ConvertToString()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'string\' data aborted!                    #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
     default: cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                   << "# Data type \'" << typ << "\' not supported! Supported are:          #\n"
                   << "# i - Int_t, f - Float_t, c - Char_t, s - string.      #\n"
                   << "########################################################\n";
              GetExit();
              return NULL;
  }
 }
return Read();
}
//============================================================================

//----------------------------------------------------------------------------
void* HAsciiKey::Read(const Char_t* sec, const Char_t* key, Char_t typ) 
{
// Returning void pointer to the beginning of data in keyword key of type typ
// in section sec.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (typ >= 'A' && typ <= 'Z') typ += 32;
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  switch (typ) {
    case 'i': if (!ConvertToInt()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'Int_t\' data aborted!                     #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
    case 'f': if (!ConvertToFloat()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'Float_t\' data aborted!                   #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
    case 'c': if (!ConvertToChar()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'Char_t\' data aborted!                    #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
    case 's': if (!ConvertToString()) {
               cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                    << "# Reading of \'string\' data aborted!                    #\n"
                    << "########################################################\n";
               GetExit();
               return NULL;
              }
              break;
     default: cerr << "###### HadAscii ###### Error in <HAsciiKey::Read> ######\n"
                   << "# Data type \'" << typ << "\' not supported! Supported are:          #\n"
                   << "# i - Int_t, f - Float_t, c - Char_t, s - string.      #\n"
                   << "########################################################\n";
              GetExit();
              return NULL;
  }
 }
return Read();
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiKey::ReadChar(const Char_t* key, Int_t pos) 
{
// Reading Char_t character at the position pos in keyword key.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return '\0';
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToChar()) {
   cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadChar> ######\n"
        << "# Reading of \'Char_t\' data aborted!                        #\n"
        << "############################################################\n";
   GetExit();
   return '\0';
  }
 }
return ReadChar(pos);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiKey::ReadChar(const Char_t* sec, const Char_t* key, Int_t pos) 
{
// Reading Char_t character at the position pos in keyword key in section sec.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (!ConvertToChar()) {
   cerr << "###### HadAscii ###### Error in <HAsciiKey::ReadChar> ######\n"
        << "# Reading of \'Char_t\' data aborted!                        #\n"
        << "############################################################\n";
   GetExit();
   return '\0';
  }
 }
return ReadChar(pos);
}
//============================================================================

//-----------------------------------------------------------------------------
Char_t* HAsciiKey::ReadRemark() 
{
// It returns commentary to the active section if there is any.

 if (CheckSection()) return fpActiveSection->GetSectionComment();
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiKey::ReadRemark(const Char_t* key) 
{
// It returns commentary to the keyword key if there is any.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return NULL;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (strlength(fpCommentLine)) fpRemarkLine = 
   new Char_t[strlength(fpCommentLine) + 1];
  fpRemarkLine[0] = '\0';
  strcopy(fpRemarkLine,fpCommentLine);
  ResetKeyData();
  return fpRemarkLine;
 } else if (strlength(fpCommentLine)) return fpCommentLine;
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiKey::ReadRemark(const Char_t* sec, const Char_t* key) 
{
// It returns commentary to the keyword key in section sec if there is any.

 using namespace StringFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  if (strlength(fpCommentLine)) fpRemarkLine = 
   new Char_t[strlength(fpCommentLine) + 1];
  fpRemarkLine[0] = '\0';
  strcopy(fpRemarkLine,fpCommentLine);
  ResetKeyData();
  return fpRemarkLine;
 } else if (strlength(fpCommentLine)) return fpCommentLine;
return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteKeyword(const Char_t* name) 
{
// Writing keyword, equal sign = is added automatically.
// If you want to break line just after the keyword, put '*' at the 
// end of it, i.e. WriteKeyword("RICH module*") will cause 
// RICH module =  and the new line after it. All '*' characters
// in keyword name are truncated!

 using namespace StringFunctions;
 if (strlength(name) > 127) {
  cerr << "###### HadAscii ###### Error in <HAsciiKey::WriteKeyword> ######\n"
       << "# Keyword name to be written is too long                       #\n"
       << "# (more than 127 characters)!                                  #\n"
       << "################################################################\n";
  return kFALSE;
 }
 if (CheckOpenedFile()) {
  if (strlastchar(name) == '*') {
   Char_t fTemp[128];
   strtrunc(fTemp, name, '*');
   fStreamFileWrite << "\n" << fTemp << " = \n\\ ";
  } else {
   fStreamFileWrite << "\n" << name << " = ";
  }
  WriteError();
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(Int_t num) 
{
// Writing one Int_t digit.

 if (CheckOpenedFile()) {
  fStreamFileWrite << num << GetSeparator();
  WriteError();
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(const Char_t* key, Int_t num) 
{
// Writing keyword key and one Int_t digit.

 WriteKeyword(key);
 WriteInt(num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteNInt(Int_t arg, ...) 
{
// Writing Int_t digits. The number of them is the first argument arg.
// Please be careful - in this function there is no type control so you
// have to put exactly Int_t variables and not the other.

 Int_t i;
 if (CheckOpenedFile()) {
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << va_arg(ap,Int_t) << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
  va_end(ap);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteNInt(const Char_t* key, Int_t arg, ...) 
{
// Writing keyword key and Int_t digits. The number of them is in argument arg.

 WriteKeyword(key);
 Int_t i;
 if (CheckOpenedFile()) {
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << va_arg(ap,Int_t) << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
  va_end(ap);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(Int_t arg, Int_t* num) 
{
// Writing Int_t digits from place pointed by num. 
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << num[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(Int_t arg, Int_t begin, Int_t* num) 
{
// Writing Int_t digits from place pointed by num starting from position begin.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = begin; i < arg+begin; i++) {
   fStreamFileWrite << num[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(const Char_t* key, Int_t arg, Int_t* num) 
{
// Writing keyword and Int_t digits from place pointed by num.
// The number of written digits is arg.

 WriteKeyword(key);
 WriteInt(arg,num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(const Char_t* key, Int_t arg, Int_t begin, Int_t* num) 
{
// Writing keyword and Int_t digits from place pointed by num starting from position begin.
// The number of written digits is arg.

 WriteKeyword(key);
 WriteInt(arg,begin,num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(TArrayI* data) 
{
// Writing all Int_t digits from TArrayI pointed by data.

 Int_t i, size = data->GetSize();
 if (CheckOpenedFile()) {
  for (i = 0; i < size; i++) {
   fStreamFileWrite << (*data)[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < size) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(Int_t arg, TArrayI* data) 
{
// Writing Int_t digits from TArrayI pointed by data.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << (*data)[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(Int_t arg, Int_t begin, TArrayI* data) 
{
// Writing Int_t digits from TArrayI pointed by data starting from position begin.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = begin; i < arg+begin; i++) {
   fStreamFileWrite << (*data)[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(const Char_t* key, TArrayI* data) 
{
// Writing keyword key and all Int_t digits from TArrayI pointed by data.

 WriteKeyword(key);
 WriteInt(data);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(const Char_t* key, Int_t arg, TArrayI* data) 
{
// Writing keyword key and Int_t digits from TArrayI pointed by data.
// The number of written digits is arg.

 WriteKeyword(key);
 WriteInt(arg,data);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteInt(const Char_t* key, Int_t arg, Int_t begin, TArrayI* data) 
{
// Writing keyword key and Int_t digits from TArrayI pointed by data starting 
// from position begin. The number of written digits is arg.

 WriteKeyword(key);
 WriteInt(arg,begin,data);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(Float_t num) 
{
// Writing one Float_t digit.

 if (CheckOpenedFile()) {
  fStreamFileWrite << num << GetSeparator();
  WriteError();
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(const Char_t* key, Float_t num) 
{
// Writing keyword key and one Float_t digit.

 WriteKeyword(key);
 WriteFloat(num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteNFloat(Int_t arg, ...) 
{
// Writing Float_t digits. The number of them is the first argument arg.
// Please be careful - in this function there is no type control so you
// have to put exactly Float_t variables and not the other.

 Int_t i;
 if (CheckOpenedFile()) {
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << va_arg(ap,Double_t) << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
  va_end(ap);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteNFloat(const Char_t* key, Int_t arg, ...) 
{
// Writing keyword key and Float_t digits. The number of them is in argument arg.

 WriteKeyword(key);
 Int_t i;
 if (CheckOpenedFile()) {
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << va_arg(ap,Double_t) << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
  va_end(ap);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(Int_t arg, Float_t* num) 
{
// Writing Float_t digits from place pointed by num. 
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << num[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(Int_t arg, Int_t begin, Float_t* num) 
{
// Writing Float_t digits from place pointed by num starting from position begin.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = begin; i < arg+begin; i++) {
   fStreamFileWrite << num[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(const Char_t* key, Int_t arg, Float_t* num) 
{
// Writing keyword and Float_t digits from place pointed by num.
// The number of written digits is arg.

 WriteKeyword(key);
 WriteFloat(arg,num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(const Char_t* key, Int_t arg, Int_t begin, Float_t* num) 
{
// Writing keyword and Float_t digits from place pointed by num starting from
// position begin. The number of written digits is arg.

 WriteKeyword(key);
 WriteFloat(arg,begin,num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(TArrayF* data) 
{
// Writing all Float_t digits from TArrayF pointed by data.

 Int_t i, size = data->GetSize();
 if (CheckOpenedFile()) {
  for (i = 0; i < size; i++) {
   fStreamFileWrite << (*data)[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < size) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(Int_t arg, TArrayF* data) 
{
// Writing Float_t digits from TArrayF pointed by data.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << (*data)[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(Int_t arg, Int_t begin, TArrayF* data) 
{
// Writing Float_t digits from TArrayF pointed by data starting from position begin.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = begin; i < arg+begin; i++) {
   fStreamFileWrite << (*data)[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(const Char_t* key, TArrayF* data) 
{
// Writing keyword key and all Float_t digits from TArrayF pointed by data.

 WriteKeyword(key);
 WriteFloat(data);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(const Char_t* key, Int_t arg, TArrayF* data) 
{
// Writing keyword key and Float_t digits from TArrayF pointed by data.
// The number of written digits is arg.

 WriteKeyword(key);
 WriteFloat(arg,data);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteFloat(const Char_t* key, Int_t arg, Int_t begin, TArrayF* data) 
{
// Writing keyword key and Float_t digits from TArrayF pointed by data starting 
// from position begin. The number of written digits is arg.

 WriteKeyword(key);
 WriteFloat(arg,begin,data);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteChar(Char_t num) 
{
// Writing one Char_t character.

 if (CheckOpenedFile()) {
  fStreamFileWrite << num << GetSeparator();
  WriteError();
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteChar(const Char_t* key, Char_t num) 
{
// Writing keyword key and one Char_t character.

 WriteKeyword(key);
 WriteChar(num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteNChar(Int_t arg, ...) 
{
// Writing Char_t characters. The number of them is the first argument arg.
// Please be careful - in this function there is no type control so you
// have to put exactly Char_t variables and not the other.

 Int_t i;
 if (CheckOpenedFile()) {
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << va_arg(ap,Char_t) << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
  va_end(ap);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteNChar(const Char_t* key, Int_t arg, ...) 
{
// Writing keyword key and Char_t characters. The number of them is in argument arg.

 WriteKeyword(key);
 Int_t i;
 if (CheckOpenedFile()) {
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << va_arg(ap,Char_t) << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
  va_end(ap);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteChar(Int_t arg, Char_t* num) 
{
// Writing Char_t characters from place pointed by num. 
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = 0; i < arg; i++) {
   fStreamFileWrite << num[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteChar(Int_t arg, Int_t begin, Char_t* num) 
{
// Writing Char_t characters from place pointed by num starting from position begin.
// The number of written digits is arg.

 Int_t i;
 if (CheckOpenedFile()) {
  for (i = begin; i < arg+begin; i++) {
   fStreamFileWrite << num[i] << GetSeparator();
   WriteError();
   if (i && !((i+1) % fNumInLine) && i+1 < arg) {
    fStreamFileWrite << "\n\\ ";
    WriteError();
   }
  }
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteChar(const Char_t* key, Int_t arg, Char_t* num) 
{
// Writing keyword and Char_t characters from place pointed by num.
// The number of written digits is arg.

 WriteKeyword(key);
 WriteChar(arg,num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteChar(const Char_t* key, Int_t arg, Int_t begin, Char_t* num) 
{
// Writing keyword and Char_t characters from place pointed by num starting from
// position begin. The number of written digits is arg.

 WriteKeyword(key);
 WriteChar(arg,begin,num);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteString(const Char_t* text, Int_t linelen, Int_t keylen) 
{
// Writing string text in line of length linelen taking into account
// that the first line may be with keyword of length keylen.

 if (CheckOpenedFile()) {
  WriteLine(text,linelen,keylen);
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::WriteString(const Char_t* key, const Char_t* text, Int_t linelen) 
{
// Writing keyword key and string text in line of length linelen.

 using namespace StringFunctions;
 WriteKeyword(key);
 WriteLine(text,linelen,strlength(key));
return kTRUE;
} 
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::CopyKeyword(const Char_t* key) 
{
// Copying keyword key to a new file (if there is any in active section).

 using namespace StringFunctions;
 if (ReadKeywordLine(key)) {
  WriteKeyword(fKeyLine);
  WriteLine(fpDataLine,76,strlength(fKeyLine));
 } else return kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::CopyKeyword(const Char_t* sec, const Char_t* key) 
{
// Copying keyword key from section sec to a new file.

 using namespace StringFunctions;
 Char_t cSecName[128];
 cSecName[0] = '\0';

 strtrunc(cSecName,sec);
 if (fpActiveSection && !strcompare(cSecName,
     fpActiveSection->GetSectionName())) {
 } else SetActiveSection(sec);

 if (ReadKeywordLine(key)) {
  WriteKeyword(fKeyLine);
  WriteLine(fpDataLine,76,strlength(fKeyLine));
 } else return kFALSE;
return kTRUE;
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 99/07/31 by Witold Przygoda

// ***************************************************************************
//////////////////////////////////////////////////////////////////////////////
//
// HAsciiRaw
//
// Class inherited from HAscii for reading/writing ASCII files in raw format.
// The data are just columns of digits (or characters), every column has its 
// keyword at the top. Data block starts from [section] name.
//
//////////////////////////////////////////////////////////////////////////////
// ***************************************************************************

 ClassImp(HAsciiRaw)

//----------------------------------------------------------------------------
HAsciiRaw::HAsciiRaw() : HAscii() 
{
// Default constructor.

 fSection = kFALSE;
 fKeywords = kFALSE;
 fData = kFALSE;
 fCounter = 0;
 fKeys = 0;
 fpKeyTypeId = NULL;
 fpKeyNames = NULL;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiRaw::~HAsciiRaw() 
{
// Destructor, releases memory and resets data.

 Reset();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiRaw::HAsciiRaw(const HAsciiRaw& source) {
 using namespace StringFunctions;
 cerr << "###### HadAscii ###### Error in <HAsciiRaw::HAsciiRaw> ######\n"
      << "# Object cannot be copied!                                  #\n"
      << "#############################################################\n";
 GetExit();
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiRaw& HAsciiRaw::operator=(const HAsciiRaw& source) {
 using namespace StringFunctions;
 if (this != &source) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::operator=> ######\n"
       << "# Object cannot be initialised by assignment!               #\n"
       << "#############################################################\n";
  GetExit();
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::GetKeys() 
{
// Number of keywords (columns of data).

return fKeys;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::GetKeyTypeId(Int_t n) 
{
// Type of data for column n. -1 means type is not defined, 
// 0 - Int_t, 1 - Float_t, 2 - Char_t.

 using namespace StringFunctions;
 if (n < 0 || n > fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::GetKeyTypeId> ######\n"
       << "# Argument (column number) out of range!                       #\n"
       << "################################################################\n";
  GetExit();
 }

return fpKeyTypeId[n];
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::GetKeyName(Int_t n) 
{
// Keyword name for column n.

 using namespace StringFunctions;
 if (n < 0 || n > fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::GetKeyName> ######\n"
       << "# Argument (column number) out of range!                     #\n"
       << "##############################################################\n";
  GetExit();
 }

return fpKeyNames[n];
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection* HAsciiRaw::SetActiveSection(const Char_t* name) 
{
// Setting active section. During activation keywords are read and stored
// and all raw data are scanned and stored as string variables containing
// the subsequent columns of data.

 using namespace StringFunctions;
 Char_t cSecName[128];
 cSecName[0] = '\0';

 strtrunc(cSecName,name);
 if (fpActiveSection && !strcompare(cSecName,
     fpActiveSection->GetSectionName())) {
  cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetActiveSection> ######\n"
       << "# Section [" << cSecName << "] is already active.\n"
       << "######################################################################\n";
  return fpActiveSection;
 }

 fFileList.ToFirstNode();
 while (fFileList.ThisNode()) {
  if (fFileList.ThisNodeFile()->GetFileChange()) {
   fFileList.ThisNodeFile()->InitSectionList();
   CheckUnambiguitySection();
  }
  fFileList.ThisNodeFile()->fSectionList.ToFirstNode();
  while (fFileList.ThisNodeFile()->fSectionList.ThisNode()) {
   if (!strcompare(cSecName,fFileList.ThisNodeFile()->
        fSectionList.ThisNodeSection()->GetSectionName())) {
    if (fpActiveSection) fpActiveSection->SetSectionState(0);
    fFileList.ThisNodeFile()->fSectionList.ThisNodeSection()->
             SetSectionState(1);
    fpActiveFile = fFileList.ThisNodeFile();
    fpActiveSection = fFileList.ThisNodeFile()->fSectionList.ThisNodeSection();
    if (fpActiveSection->GetSectionRaw()==NULL) 
     ReadSectionRaw();
    return fpActiveSection;
   }
   fFileList.ThisNodeFile()->fSectionList.ToNextNode();
  }
  fFileList.ToNextNode();
 }

 cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetActiveSection> ######\n"
      << "# No section [" << cSecName << "] in any opened ASCII file\n"
      << "# for this runtime database input.                                   #\n"
      << "######################################################################\n";
 // Because of runtime databes two inputs it will not stop it this case.
 // GetExit();
 fpActiveFile = NULL;
return (fpActiveSection = NULL);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadSectionRaw() 
{
// It is called only from SetActiveSection. The function reads the whole 
// section - kewords and column data (automatically assigned to proper
// keyword). The control of number of data in line with respect to number 
// of keywords is performed.
 
 using namespace StringFunctions;
 Int_t i, j, k, l, lSecSize = 0, lCountLine = 0;
 Bool_t bCondition;
 Bool_t bKeys = kFALSE;
 Long_t lSecBegin = 0, lSecEnd = 0;
 Long_t lPos = 0;
 Char_t cName[128], cLine[1024], cKey[128], cData[1024];
 cName[0] = '\0';
 cLine[0] = '\0';
 cKey[0] = '\0';
 cData[0] = '\0';

 lPos = lSecBegin = fpActiveSection->GetSectionBegin();
 lSecEnd = fpActiveSection->GetSectionEnd();

 fpActiveFile->fStreamFileRead.seekg(lSecBegin);

 do {

  if (lPos < lSecEnd)
   if (fpActiveFile->fStreamFileRead.peek() == '\n')
    fpActiveFile->fStreamFileRead.ignore(1);
  lPos = fpActiveFile->fStreamFileRead.tellg();

  if (lPos < lSecEnd) {

   if ((lSecEnd - lPos) >= 1023) fpActiveFile->fStreamFileRead.get(cLine,1023);
    else fpActiveFile->fStreamFileRead.get(cLine,(lSecEnd - lPos + 1));
   if (fpActiveFile->fStreamFileRead.fail()) {
    fpActiveFile->fStreamFileRead.clear(fpActiveFile->fStreamFileRead.rdstate() &
                                      ~ios::failbit);
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadSectionRaw> ######\n"
         << "# In file \'" << fpActiveFile->GetFileName() << "\' unexpected error\n"
         << "# during reading in section [" << fpActiveSection->GetSectionName() << "].\n"
         << "##################################################################\n";
    GetExit();
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    return kFALSE;
   }

   if (strfirstchar(cLine) == ';' ||
       strfirstchar(cLine) == '#' ||
       strfirstchar(cLine) == '\\' ||
       strlength(cLine) == 0 ||
       strempty(cLine)) {

    cLine[0] = '\0';
    if (bKeys == kTRUE && strfirstchar(cLine) == '\\')
     cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadSectionRaw> ######\n"
          << "# Line with \'\\\' at the beginning detected. Remember that data      #\n"
          << "# in \'raw\' format cannot be continued in the next line.            #\n"
          << "# This line will be ignored!                                       #\n"
          << "####################################################################\n";

   } else {

    // first reading keys then all data

    if (bKeys == kFALSE) {
     // strcopy(fKeyLine,"RawData");
     cData[0] = '\0';
     strcopy(cData,cLine);
     i = 0;
      	
     do {
      bCondition = kFALSE;
      cKey[0] = '\0';
      strunit(cKey,cData);
      
      if (strlength(cKey) > 0) {
       i++;
       bCondition = kTRUE;
      }
     } while (bCondition);

     if (i > 0) {

      // here allocation of raw data part of section
      fpActiveSection->fpRawData = new HAsciiSectionRaw;

      fpActiveSection->fpRawData->fKeys = i;
      fpActiveSection->fpRawData->fElements = 0;
      fpActiveSection->fpRawData->fpKeyTypeId = new Int_t[i];

      fpActiveSection->fpRawData->fpKeyNames = new (Char_t*)[i];
      fpActiveSection->fpRawData->fpDataString = new (Char_t*)[i];
      fpActiveSection->fpRawData->fpDataInt = new (Int_t*)[i];
      fpActiveSection->fpRawData->fpDataFloat = new (Float_t*)[i];
      fpActiveSection->fpRawData->fpDataChar = new (Char_t*)[i];
      fpActiveSection->fpRawData->fpDataStr = new (Char_t**)[i];

      cData[0] = '\0';
      strcopy(cData,cLine);
      lSecSize = lSecEnd - fpActiveFile->fStreamFileRead.tellg();


      for (j = 0; j < i; j++) {
       fpActiveSection->fpRawData->fpKeyTypeId[j] = -1;

       fpActiveSection->fpRawData->fpKeyNames[j] = new Char_t[128];
       fpActiveSection->fpRawData->fpKeyNames[j][0] = '\0';
       strunit(fpActiveSection->fpRawData->fpKeyNames[j],cData);

       // control of keys unambiguity
       for (k = 0; k < j+1; k++) 
        for (l = 0; l < k; l++) {
         if (strcompare(fpActiveSection->fpRawData->fpKeyNames[k],
                        fpActiveSection->fpRawData->fpKeyNames[l]) == 0) {
          cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadSectionRaw> ######\n"
               << "# Keyword names in raw data must be unambiguous!                 #\n"
               << "##################################################################\n";
          GetExit();
         }
       }

       fpActiveSection->fpRawData->fpDataString[j] = new Char_t[lSecSize];
       fpActiveSection->fpRawData->fpDataString[j][0] = '\0';

       fpActiveSection->fpRawData->fpDataInt[j] = NULL;
       fpActiveSection->fpRawData->fpDataFloat[j] = NULL;
       fpActiveSection->fpRawData->fpDataChar[j] = NULL;
       fpActiveSection->fpRawData->fpDataStr[j] = NULL;
      }

      bKeys = kTRUE;

     } else {
      cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadSectionRaw> ######\n"
           << "# In section [" << fpActiveSection->GetSectionName() << "]\n"
           << "# of raw data in file \'" << fpActiveFile->GetFileName() << "\'\n"
           << "# there are no keys describing columns of data!                  #\n"
           << "##################################################################\n";
      GetExit();
     }

    } else {
             
      bCondition = kTRUE;
      cData[0] = '\0';
      strcopy(cData,cLine);
      for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
       cKey[0] = '\0';
       strunit(cKey,cData);
       if (strempty(cKey)) {
        cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadSectionRaw> ######\n" 
             << "# Missing data (there are: " << i << "; there should be: " << fpActiveSection->fpRawData->fKeys << ") in row " << lCountLine << ".\n"
             << "# Zeros (0) will be put here - need manual correction!             #\n"
             << "####################################################################\n";
        bCondition = kFALSE;
       }
      }
      if (strempty(cData) == kFALSE) {
       cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadSectionRaw> ######\n"
            << "# Too many data units in row " << lCountLine << ".\n"
            << "# Zeros (0) will be put here - need manual correction!             #\n"
            << "####################################################################\n";
       bCondition = kFALSE;
      }

 
      if (bCondition) {
       for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
        cKey[0] = '\0';
        strunit(cKey,cLine);
        strconcat(fpActiveSection->fpRawData->fpDataString[i],cKey);
        strconcat(fpActiveSection->fpRawData->fpDataString[i]," ");
       }  
      } else {
       for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
        strconcat(fpActiveSection->fpRawData->fpDataString[i],"0 ");
       }
      }

      lCountLine++;

    } // eof condition on keys 

   }

   lPos = fpActiveFile->fStreamFileRead.tellg();
   if (lPos >= lSecEnd && bKeys == kFALSE) {
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadSectionRaw> ######\n"
         << "# In section [" << fpActiveSection->GetSectionName() << "] of raw data\n"
         << "# in file \'" << fpActiveFile->GetFileName() << "\' there are no keys\n"
         << "# describing columns of data!                                    #\n"
         << "##################################################################\n";
    GetExit();
    return kFALSE;
   }
   cLine[0] = '\0';

  } else if (bKeys==kFALSE) {

   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadSectionRaw> ######\n"
        << "# In section [" << fpActiveSection->GetSectionName() << "] of raw data\n"
        << "# in file \'" << fpActiveFile->GetFileName() << "\' there are no keys\n"
        << "# describing columns of data!                                    #\n"
        << "##################################################################\n";

   GetExit();
   return kFALSE;
  }
 } while (lPos < lSecEnd);

 fpActiveSection->fpRawData->fElements = lCountLine;

// at the end remastering

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
  j = strlength(fpActiveSection->fpRawData->fpDataString[i]);
  if (j > 0) {
   fpActiveSection->fpRawData->fpTempString = new Char_t[j+1];
   fpActiveSection->fpRawData->fpTempString[0] = '\0';
   strcopy(fpActiveSection->fpRawData->fpTempString,
           fpActiveSection->fpRawData->fpDataString[i]);
   delete [] fpActiveSection->fpRawData->fpDataString[i];
   fpActiveSection->fpRawData->fpDataString[i] = new Char_t[j+1];
   fpActiveSection->fpRawData->fpDataString[i][0] = '\0';
   strcopy(fpActiveSection->fpRawData->fpDataString[i],
           fpActiveSection->fpRawData->fpTempString);
   delete [] fpActiveSection->fpRawData->fpTempString;
   fpActiveSection->fpRawData->fpTempString = NULL;
  }
 }

 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

return kTRUE;
} 
//============================================================================

//----------------------------------------------------------------------------
Long_t HAsciiRaw::GetRowsNr() 
{ 
// Number of data units in column of raw data.

return fpActiveSection->fpRawData->fElements;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(Int_t col, Int_t row) 
{
// Reading one Int_t digit from column col in position row.

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "###########################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  if (!ConvertToInt()) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
        << "# Conversion of raw data column " << col << " to \'Int_t\' aborted!\n"
        << "###########################################################\n";
   GetExit();
   return 0;
  }

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
        << "# Number of recognised integers is different than number  #\n"
        << "# of units read in column " << col << "!\n"
        << "###########################################################\n";
   GetExit();
  }

  // memory allocation for integers
  fpActiveSection->fpRawData->fpDataInt[col] = 
	new Int_t[fpActiveSection->fpRawData->fElements];
  
  if (fDataBank==1) {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataInt[col][i] = fDataInt[i];
  } else {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataInt[col][i] = fpDataInt[i];
  }
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 0;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 0) {
  return (fpActiveSection->fpRawData->fpDataInt[col][row]);
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "###########################################################\n";
  GetExit();
 }
 
return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(const Char_t* keyname, Int_t row) 
{
// Reading one Int_t digit from column keyname in position row.

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) 
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "###########################################################\n";
  GetExit();
 }

return ReadInt(column,row); 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(Int_t colcond, Int_t rowvalue, Int_t col) 
{
// Reading one Int_t digit from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++) 
  if (ReadInt(colcond,i) == rowvalue) return ReadInt(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadInt> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "#############################################################\n";

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(Int_t colcond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Int_t digit from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadInt(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadInt> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "#############################################################\n";

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(const Char_t* keycond, Int_t rowvalue, Int_t col) 
{
// Reading one Int_t digit from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadInt(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadInt> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "#############################################################\n";

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(const Char_t* keycond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Int_t digit from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadInt(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadInt> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "#############################################################\n";

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(Int_t* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Int_t digits from column col starting from position begin. 
// Digits are put into Int_t array pointed by putin. 

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "###########################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  if (!ConvertToInt()) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
        << "# Conversion of raw data column " << col << " to \'Int_t\' aborted!\n"
        << "###########################################################\n";
   GetExit();
   return 0;
  }

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
        << "# Number of recognised integers is different than number  #\n" 
        << "# of units read in column " << col << "!\n"
        << "###########################################################\n";
   GetExit();
  }

  // memory allocation for integers
  fpActiveSection->fpRawData->fpDataInt[col] =
        new Int_t[fpActiveSection->fpRawData->fElements];

  if (fDataBank==1) {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataInt[col][i] = fDataInt[i];
  } else {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataInt[col][i] = fpDataInt[i];
  }
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 0;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 0) {
  if (begin+number > fpActiveSection->fpRawData->fElements) 
   number = fpActiveSection->fpRawData->fElements - begin;
  for (i = begin; i < begin+number; i++)
   putin[i] = fpActiveSection->fpRawData->fpDataInt[col][i];
  return kTRUE;
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "###########################################################\n";
  GetExit();
 }

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(Int_t* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Int_t digits from column keyname starting from position begin. 
// Digits are put into Int_t array pointed by putin. 

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++)
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadInt> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "###########################################################\n";
  GetExit();
 }

return ReadInt(putin,column,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(Int_t* putin, Int_t col, Int_t number) 
{
// Reading number of Int_t digits from column col. If number = 0 all data are read.
// Digits are put into Int_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadInt(putin,col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(Int_t* putin, const Char_t* keyname, Int_t number) 
{
// Reading number of Int_t digits from column keyname. If number = 0 all data are read.
// Digits are put into Int_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadInt(putin,keyname,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(TArrayI* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Int_t digits from column col starting from position begin.
// Digits are put into TArrayI pointed by putin.

return ReadInt(&putin[0],col,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(TArrayI* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Int_t digits from column keyname starting from position begin.
// Digits are put into TArrayI pointed by putin.

return ReadInt(&putin[0],keyname,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(TArrayI* putin, Int_t col, Int_t number) 
{
// Reading number of Int_t digits from column col. If number = 0 all data are read.
// Digits are put into TArrayI pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadInt(&putin[0],col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(TArrayI* putin, const Char_t* keyname, Int_t number) 
{
// Reading number of Int_t digits from column keyname. If number = 0 all data are read.
// Digits are put into TArrayI pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadInt(&putin[0],keyname,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(Int_t col, Int_t row) 
{
// Reading one Float_t digit from column col in position row.

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "#############################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  if (!ConvertToFloat()) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
        << "# Conversion of raw data column " << col << " to \'Float_t\' aborted!\n"
        << "#############################################################\n";
   GetExit();
   return 0.;
  }

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
        << "# Number of recognised floats is different than number      #\n"
        << "# of units read in column " << col << "!\n"
        << "#############################################################\n";
   GetExit();
  }

  // memory allocation for floats
  fpActiveSection->fpRawData->fpDataFloat[col] =
        new Float_t[fpActiveSection->fpRawData->fElements];

  if (fDataBank==2) {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataFloat[col][i] = fDataFloat[i];
  } else {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataFloat[col][i] = fpDataFloat[i];
  }
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 1;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 1) {
  return (fpActiveSection->fpRawData->fpDataFloat[col][row]);
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "#############################################################\n";
  GetExit();
 }

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(const Char_t* keyname, Int_t row) 
{
// Reading one Float_t digit from column keyname in position row.

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++)
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "#############################################################\n";
  GetExit();
 }

return ReadFloat(column,row);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(Int_t colcond, Int_t rowvalue, Int_t col) 
{
// Reading one Float_t digit from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadFloat(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadFloat> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "###############################################################\n";

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(Int_t colcond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Float_t digit from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadFloat(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadFloat> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "###############################################################\n";

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(const Char_t* keycond, Int_t rowvalue, Int_t col) 
{
// Reading one Float_t digit from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadFloat(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadFloat> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "###############################################################\n";

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(const Char_t* keycond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Float_t digit from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadFloat(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadFloat> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "###############################################################\n";

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(Float_t* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Float_t digits from column col starting from position begin.
// Digits are put into Int_t array pointed by putin.

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "#############################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  if (!ConvertToFloat()) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
        << "# Conversion of raw data column " << col << " to \'Float_t\' aborted!\n"
        << "#############################################################\n";
   GetExit();
   return 0;
  }

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
        << "# Number of recognised floats is different than number      #\n"
        << "# of units read in column " << col << "!\n"
        << "#############################################################\n";
   GetExit();
  }

  // memory allocation for floats
  fpActiveSection->fpRawData->fpDataFloat[col] =
        new Float_t[fpActiveSection->fpRawData->fElements];

  if (fDataBank==2) {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataFloat[col][i] = fDataFloat[i];
  } else {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataFloat[col][i] = fpDataFloat[i];
  }
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 1;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 1) {
  if (begin+number > fpActiveSection->fpRawData->fElements)
   number = fpActiveSection->fpRawData->fElements - begin;
  for (i = begin; i < begin+number; i++)
   putin[i] = fpActiveSection->fpRawData->fpDataFloat[col][i];
  return kTRUE;
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "#############################################################\n";
  GetExit();
 }

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(Float_t* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Float_t digits from column keyname starting from position begin.
// Digits are put into Int_t array pointed by putin.

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++)
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadFloat> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "#############################################################\n";
  GetExit();
 }

return ReadFloat(putin,column,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(Float_t* putin, Int_t col, Int_t number) 
{
// Reading number of Float_t digits from column col. If number = 0 all data are read.
// Digits are put into Int_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadFloat(putin,col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(Float_t* putin, const Char_t* keyname, Int_t number) 
{
// Reading number of Float_t digits from column keyname. If number = 0 all data are read.
// Digits are put into Int_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadFloat(putin,keyname,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(TArrayF* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Float_t digits from column col starting from position begin.
// Digits are put into TArrayI pointed by putin.

return ReadFloat(&putin[0],col,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(TArrayF* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Float_t digits from column keyname starting from position begin.
// Digits are put into TArrayI pointed by putin.

return ReadFloat(&putin[0],keyname,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(TArrayF* putin, Int_t col, Int_t number) 
{
// Reading number of Float_t digits from column col. If number = 0 all data are read.
// Digits are put into TArrayI pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadFloat(&putin[0],col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(TArrayF* putin, const Char_t* keyname, Int_t number) 
{
// Reading number of Float_t digits from column keyname. If number = 0 all data are read.
// Digits are put into TArrayI pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadFloat(&putin[0],keyname,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(Int_t col, Int_t row) 
{
// Reading one Char_t character from column col in position row.

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "############################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  if (!ConvertToChar()) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
        << "# Conversion of raw data column " << col << " to \'Char_t\' aborted!\n"
        << "############################################################\n";
   GetExit();
   return 0;
  }

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
        << "# Number of recognised characters is different than number #\n"
        << "# of units read in column " << col << "!\n"
        << "############################################################\n";
   GetExit();
  }

  // memory allocation for characters
  fpActiveSection->fpRawData->fpDataChar[col] =
        new Char_t[fpActiveSection->fpRawData->fElements];

  if (fDataBank==3) {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataChar[col][i] = fDataChar[i];
  } else {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataChar[col][i] = fpDataChar[i];
  }
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 2;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 2) {
  return (fpActiveSection->fpRawData->fpDataChar[col][row]);
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "############################################################\n";
  GetExit();
 }

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(const Char_t* keyname, Int_t row) 
{
// Reading one Char_t character from column keyname in position row.

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++)
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "############################################################\n";
  GetExit();
 }

return ReadChar(column,row);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(Int_t colcond, Int_t rowvalue, Int_t col) 
{
// Reading one Char_t character from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadChar(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadChar> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "##############################################################\n";

return '\0';
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(Int_t colcond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Char_t character from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadChar(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadChar> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "##############################################################\n";

return '\0';
}
//============================================================================

//----------------------------------------------------------------------------
// Function not available because of confusion with another function.
// Char_t HAsciiRaw::ReadChar(const Char_t* keycond, Int_t rowvalue, Int_t col) 
// {
// Reading one Char_t character from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.
// 
//  for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
//   if (ReadInt(keycond,i) == rowvalue) return ReadChar(col,i);
//    else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadChar> ######\n"
//              << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
//              << "##############################################################\n";
// 
// return '\0';
// }
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(const Char_t* keycond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Char_t character digit from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadChar(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadChar> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "##############################################################\n";

return '\0';
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Char_t characters from column col starting from position begin.
// Digits are put into Char_t array pointed by putin.

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "############################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  if (!ConvertToChar()) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
        << "# Conversion of raw data column " << col << " to \'Char_t\' aborted!\n"
        << "############################################################\n";
   GetExit();
   return 0;
  }

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
        << "# Number of recognised characters is different than number #\n"
        << "# of units read in column " << col << "!\n"
        << "############################################################\n";
   GetExit();
  }

  // memory allocation for characters
  fpActiveSection->fpRawData->fpDataChar[col] =
        new Char_t[fpActiveSection->fpRawData->fElements];

  if (fDataBank==3) {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataChar[col][i] = fDataChar[i];
  } else {
   for (i = 0; i < fDataCounter; i++)
    fpActiveSection->fpRawData->fpDataChar[col][i] = fpDataChar[i];
  }
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 2;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 2) {
  if (begin+number > fpActiveSection->fpRawData->fElements)
   number = fpActiveSection->fpRawData->fElements - begin;
  for (i = begin; i < begin+number; i++)
   putin[i] = fpActiveSection->fpRawData->fpDataChar[col][i];
  return kTRUE;
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "############################################################\n";
  GetExit();
 }

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Char_t characters from column keyname starting from position begin.
// Digits are put into Char_t array pointed by putin.

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++)
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "############################################################\n";
  GetExit();
 }

return ReadChar(putin,column,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, Int_t col, Int_t number = 0) 
{
// Reading number of Char_t characters from column col. If number = 0 all data are read.
// Digits are put into Char_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadChar(putin,col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, const Char_t* keyname, Int_t number = 0) 
{
// Reading number of Char_t characters from column keyname. If number = 0 all data are read.
// Digits are put into Char_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadChar(putin,keyname,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(TArrayC* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Char_t characters from column col starting from position begin.
// Digits are put into TArrayC pointed by putin.

return ReadChar(&putin[0],col,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(TArrayC* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Char_t characters from column keyname starting from position begin.
// Digits are put into TArrayC pointed by putin.

return ReadChar(&putin[0],keyname,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(TArrayC* putin, Int_t col, Int_t number = 0) 
{
// Reading number of Char_t characters from column col. If number = 0 all data are read.
// Digits are put into TArrayC pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadChar(&putin[0],col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(TArrayC* putin, const Char_t* keyname, Int_t number = 0) 
{
// Reading number of Char_t characters from column keyname. If number = 0 all data are read.
// Digits are put into TArrayC pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadChar(&putin[0],keyname,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(Int_t col, Int_t row)
{
// Reading string of characters from column col in position row.

 using namespace StringFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i,j,k;
 Char_t buf[32];

 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadChar> ######\n"
       << "# Column number out of range (" << fpActiveSection->fpRawData->fKeys << " column(s) recognised).\n"
       << "############################################################\n";
  GetExit();
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);


  for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
   fpActiveSection->fpRawData->fpDataStr[i] =
         new (Char_t*)[fpActiveSection->fpRawData->fElements];
   for (j = 0; j < fpActiveSection->fpRawData->fElements; j++)
    fpActiveSection->fpRawData->fpDataStr[i][j] = NULL;
  }

  for (i = 0; i < fpActiveSection->fpRawData->fElements; i++) {
   strunit(buf,fpDataLine);
   if (strempty(buf) == kFALSE) {
    k = strlength(buf);
    fpActiveSection->fpRawData->fpDataStr[col][i] = new Char_t[k+1];
    strcopy(fpActiveSection->fpRawData->fpDataStr[col][i], buf);
   } else {
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadString> ######\n"
         << "# Missing data in column " << col << "!\n"
         << "##############################################################\n";
    GetExit();
   }
  }
  if (strempty(fpDataLine) == kFALSE) {
   cerr << "###### HadAscii ###### Warnirng in <HAsciiRaw::ReadString> ######\n"
        << "# Too many data units in column " << col << "!\n"
        << "#################################################################\n";
  }

  fpActiveSection->fpRawData->fpKeyTypeId[col] = 3;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 3) {
  return (fpActiveSection->fpRawData->fpDataStr[col][row]);
 } else {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadString> ######\n"
       << "# Column " << col << " was converted earlier to another type!\n"
       << "##############################################################\n";
  GetExit();
 }

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(const Char_t* keyname, Int_t row)
{
// Reading string from column keyname in position row.

 using namespace StringFunctions;
 Int_t i, column = -1;
 Char_t cName[128];
 cName[0] = '\0';
 strtrunc(cName,keyname);

 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++)
  if (strcompare(cName,fpActiveSection->fpRawData->fpKeyNames[i])==0) {
   column = i;
   break;
  }

 if (column < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadString> ######\n"
       << "# No keyword \'" << cName << "\' describing any column of data!\n"
       << "##############################################################\n";
  GetExit();
 }

return ReadString(column,row);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(Int_t colcond, Int_t rowvalue, Int_t col)
{
// Reading string from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadString(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadString> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "################################################################\n";

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(Int_t colcond, Int_t rowvalue, const Char_t* key)
{
// Reading string from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadString(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadString> ######\n"
             << "# No value " << rowvalue << " in column \'" << colcond << "\'.\n"
             << "################################################################\n";

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(const Char_t* keycond, Int_t rowvalue, Int_t col)
{
// Reading string from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadString(col,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadString> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "################################################################\n";

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(const Char_t* keycond, Int_t rowvalue, const Char_t* key)
{
// Reading sitrng from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadString(key,i);
   else cerr << "###### HadAscii ###### Warning in <HAsciiRaw::ReadString> ######\n"
             << "# No value " << rowvalue << " in column \'" << keycond << "\'.\n"
             << "################################################################\n";

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteSection(const Char_t* name) 
{
// Writing section name. Brackets [ ] are added automatically.

 using namespace StringFunctions;
 if (strlength(name) > 127) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteSection> ######\n"
       << "# Section name to be written is too long                       #\n"
       << "# (more than 127 characters)!\n"
       << "################################################################\n";
  return kFALSE;
 }
 if (CheckOpenedFile()) {
  fStreamFileWrite << "\n[ " << name << " ]\n";
  WriteError();
 } else return kFALSE;
 fSection = kTRUE;
 fKeywords = kFALSE;
 fData = kFALSE;

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::RecogniseKeys(const Char_t* name, Int_t num) 
{
// Analysis of string line containing keywords, counting them and writing
// their names into fpKeyNames array.

 using namespace StringFunctions;
 if (strempty(name)) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::RecogniseKeys> ######\n"
       << "# No keywords in this variable!                                 #\n"
       << "#################################################################\n";
  return kFALSE;
 }

 Int_t i;
 Char_t *fpTemp = NULL;
 Bool_t bCondition;
 Char_t cKey[128];

 fKeys = 0;
 fpTemp = new Char_t[strlength(name)+1];
 strcopy(fpTemp,name);

 if (num == 0) {
  do {
   bCondition = kFALSE;
   strunit(cKey,fpTemp);
   if (strlength(cKey) > 0) {
    fKeys++;
    bCondition = kTRUE;
   }
  } while (bCondition);
  strcopy(fpTemp,name);
 } else fKeys = num;

 fpKeyTypeId = new Int_t[fKeys];
 fpKeyNames = new (Char_t*)[fKeys];
 for (i = 0; i < fKeys; i++) {
  fpKeyTypeId[i] = -1;
  fpKeyNames[i] = new Char_t[128];
  strunit(cKey,fpTemp);
  strcopy(fpKeyNames[i],cKey);
 }
 
 delete [] fpTemp;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteKeywords(const Char_t* name) 
{
// Writing keywords line.

 using namespace StringFunctions;
 if (fKeywords == kTRUE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# Keywords have been already written.                           #\n"
       << "# You have to define next section name!                         #\n"
       << "#################################################################\n";
  return kFALSE;
 }
 if (fSection == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# No section name written - keywords must follow a new section! #\n"
       << "#################################################################\n";
  return kFALSE;
 }
 if (strlength(name) > 1023) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# Keywords line to be written is too long                       #\n"
       << "# (more than 1023 characters)!                                  #\n"
       << "#################################################################\n";
  return kFALSE;
 }
 if (CheckOpenedFile()) {
  fStreamFileWrite << "\n" << name << "\n";
  WriteError();
 } else return kFALSE;
 RecogniseKeys(name);
 fKeywords = kTRUE;
 fSection = kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteKeywords(Int_t arg, ...) 
{
// Writing keywords, number of keywords arg and the keywords themselves (as strings)
// are the arguments.

 using namespace StringFunctions;
 if (fKeywords == kTRUE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# Keywords have been already written.                           #\n"
       << "# You have to define next section name!                         #\n"
       << "#################################################################\n";
  return kFALSE;
 }
 if (fSection == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# No section name written - keywords must follow a new section! #\n"
       << "#################################################################\n";
  return kFALSE;
 }
 if (arg <= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# Wrong function argument - number of keywords                  #\n"
       << "# must be of positive value!                                    #\n"
       << "#################################################################\n";
  return kFALSE;
 }

 Int_t i;
 fKeys = arg;
 fpKeyTypeId = new Int_t[arg];
 fpKeyNames = new (Char_t*)[arg];

 if (CheckOpenedFile()) {
  fStreamFileWrite << "\n";
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fpKeyTypeId[i] = -1;
   fpKeyNames[i] = new Char_t[128];
   strcopy(fpKeyNames[i],va_arg(ap,Char_t*));
   fStreamFileWrite << fpKeyNames[i] << GetSeparator();
   WriteError();
  }
  va_end(ap);
  fStreamFileWrite << "\n";
 } else return kFALSE;
 fKeywords = kTRUE;
 fSection = kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteKeywords(const Char_t* sec, Int_t arg, ...) 
{
// Writing section sec and keywords (number of keywords is arg).

 using namespace StringFunctions;
 WriteSection(sec);

 if (arg <= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteKeywords> ######\n"
       << "# Wrong function argument - number of keywords                  #\n"
       << "# must be of positive value!                                    #\n"
       << "#################################################################\n";
  return kFALSE;
 }

 Int_t i;
 fKeys = arg;
 fpKeyTypeId = new Int_t[arg];
 fpKeyNames = new (Char_t*)[arg];

 if (CheckOpenedFile()) {
  fStreamFileWrite << "\n";
  va_list ap;
  va_start(ap,arg);
  for (i = 0; i < arg; i++) {
   fpKeyTypeId[i] = -1;
   fpKeyNames[i] = new Char_t[128];
   strcopy(fpKeyNames[i],va_arg(ap,Char_t*));
   fStreamFileWrite << fpKeyNames[i] << GetSeparator();
   WriteError();
  }
  va_end(ap);
  fStreamFileWrite << "\n";
 } else return kFALSE;
 fKeywords = kTRUE;
 fSection = kFALSE;
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiRaw::SetReadKeyType(Int_t col, Int_t type) 
{
// This is auxiliary function used only in HParHadAsciiFileIo to have access
// to private data member of this class.

 using namespace StringFunctions;
 if (fpActiveSection->fpRawData->fKeys == 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyType> ######\n"
       << "# Setting keywords types not possible - no keywords read!        #\n"
       << "##################################################################\n";
  GetExit();
  return;
 }
 if (col > fpActiveSection->fpRawData->fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyType> ######\n"
       << "# Arguemnt column out of range (larger than number of keywords)! #\n"
       << "##################################################################\n";
  GetExit();
  return;
 }
 if (type < 0 || type > 3) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyType> ######\n"
       << "# Arguemnt type out of range (0 - 3)!                            #\n"
       << "##################################################################\n";
  GetExit();
  return;
 }

 fpActiveSection->fpRawData->fpKeyTypeId[col] = type;
}
//============================================================================ 

//----------------------------------------------------------------------------
void HAsciiRaw::SetWriteKeyType(Int_t col, Int_t type)
{
// This is auxiliary function used only in HParHadAsciiFileIo to have access
// to private data member of this class.

 using namespace StringFunctions;
 if (fKeys == 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetWriteKeyType> ######\n"
       << "# Setting keywords types not possible - no keywords written!      #\n"
       << "###################################################################\n";
  GetExit();
  return;
 }
 if (col > fKeys) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetWriteKeyType> ######\n"
       << "# Arguemnt column out of range (larger than number of keywords)!  #\n"
       << "###################################################################\n";
  GetExit();
  return;
 }
 if (type < 0 || type > 3) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetWriteKeyType> ######\n"
       << "# Arguemnt type out of range (0 - 3)!                             #\n"
       << "###################################################################\n";
  GetExit();
  return;
 }

 fpKeyTypeId[col] = type;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiRaw::SetWriteKeyTypes(Char_t key1, ...) 
{
// Defining type of data for subsequent columns (described by keywords) to be written.
// Allowed type of data are: 'i' - Int_t, 'f' - Float_t, 'c' - Char_t and 's' - string (Char_t*).
// You can force changing type of column data calling this function.

 using namespace StringFunctions;
  if (fKeys == 0) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetWriteKeyTypes> ######\n"
        << "# Setting keywords types not possible - no keywords written!       #\n"
        << "####################################################################\n";
   GetExit();
  }
  Char_t base;
  if (key1 < 97) key1 += 32;
  switch (key1) {
   case 'i': if (fpKeyTypeId[0] > 0) 
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                              #\n"
                   << "######################################################################\n";
             fpKeyTypeId[0] = 0;
             break;
   case 'f': if (fpKeyTypeId[0] > 0) 
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                              #\n"
                   << "######################################################################\n";
             fpKeyTypeId[0] = 1;
             break;
   case 'c': if (fpKeyTypeId[0] > 0) 
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                              #\n"
                   << "######################################################################\n";
             fpKeyTypeId[0] = 2;
             break;
   case 's': if (fpKeyTypeId[0] > 0)
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                              #\n"
                   << "######################################################################\n";
             fpKeyTypeId[0] = 3;
             break;
    default: cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                  << "# Type \'" << key1 << "\' not supported (only: i,f,c,s)!                          #\n"
                  << "####################################################################\n";
             GetExit();
             return;
  }

  va_list ap;
  va_start(ap,key1);
  for (Int_t i = 1; i < fKeys; i++) {
   base = (Char_t)va_arg(ap,Int_t);
   if (base < 97) base += 32;
   
   switch (base) {
    case 'i': if (fpKeyTypeId[i] > 0) 
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "######################################################################\n";
              fpKeyTypeId[i] = 0;
              break;
    case 'f': if (fpKeyTypeId[i] > 0) 
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "######################################################################\n";
              fpKeyTypeId[i] = 1;
              break;
    case 'c': if (fpKeyTypeId[i] > 0) 
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "######################################################################\n";
              fpKeyTypeId[i] = 2;
              break;
    case 's': if (fpKeyTypeId[i] > 0)
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "######################################################################\n";
              fpKeyTypeId[i] = 3;
              break;
     default: cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetWriteKeyTypes> ######\n"
                   << "# Type \'" << base << "\' not supported (only: i,f,c,s)!                          #\n"
                   << "####################################################################\n";
              GetExit();
              return;
   }
  }
  va_end(ap);

}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiRaw::SetReadKeyTypes(Char_t key1, ...)
{
// Defining type of data read for subsequent columns (described by keywords).
// Active section must be selected first. 
// Allowed type of data are: 'i' - Int_t, 'f' - Float_t, 'c' - Char_t and 
// 's' - string (Char_t*).
// You can force changing type of column data calling this function.

 using namespace StringFunctions;
  if (fpActiveSection == NULL) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyTypes> ######\n"
        << "# There is no active section selected!                            #\n"
        << "###################################################################\n";
   GetExit();
   return;
  }
  if (fpActiveSection->fpRawData->fKeys == 0) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyTypes> ######\n"
        << "# Setting keywords types not possible - no keywords read!         #\n"
        << "###################################################################\n";
   GetExit();
   return;
  }
  Char_t base;
  if (key1 < 97) key1 += 32;
  switch (key1) {
   case 'i': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                             #\n"
                   << "#####################################################################\n";
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadInt(0,0);
             break;
   case 'f': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                             #\n"
                   << "#####################################################################\n";
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadFloat(0,0);
             break;
   case 'c': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                             #\n"
                   << "#####################################################################\n";
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadChar(0,0);
             break;
   case 's': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                   << "# Change of type for 1. column of data!                             #\n"
                   << "#####################################################################\n";
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadString(0,0);
             break;
    default: cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyTypes> ######\n"
                  << "# Type \'" << key1 << "\' not supported (only: i,f,c,s)!                         #\n"
                  << "###################################################################\n";
             GetExit();
             return;
  }

  va_list ap;
  va_start(ap,key1);
  for (Int_t i = 1; i < fpActiveSection->fpRawData->fKeys; i++) {
   base = (Char_t)va_arg(ap,Int_t);
   if (base < 97) base += 32;
  
   switch (base) {
    case 'i': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0)
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "#####################################################################\n";
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadInt(i,0);
              break;
    case 'f': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0)
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "#####################################################################\n";
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadFloat(i,0);
              break;
    case 'c': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0)
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "#####################################################################\n";
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadChar(i,0);
              break;
    case 's': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0)
               cerr << "###### HadAscii ###### Warning in <HAsciiRaw::SetReadKeyTypes> ######\n"
                    << "# Change of type for " << i+1 << ". column of data!\n"
                    << "#####################################################################\n";
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadString(i,0);
              break;
     default: cerr << "###### HadAscii ###### Error in <HAsciiRaw::SetReadKeyTypes> ######\n"
                   << "# Type \'" << base << "\' not supported (only: i,f,c,s)!                         #\n"
                   << "###################################################################\n";
              GetExit();
              return;
   }
  }
  va_end(ap);

}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadRawLine(Int_t row, ...)
{
// Reading of given row of raw data. The first argument is the 'row' number
// and then pointers to variables of proper type should follow. It means
// that the sequence of variables should be exactly of the same type as
// declared for types of columns of data. For example:
// ReadRawLine(5, &a, &b, &c);
// where a, b, c, are variables of proper type.

 using namespace StringFunctions;
 Int_t i;
 if (fpActiveSection == NULL) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadRawLine> ######\n"
       << "# There is no active section selected!                        #\n"
       << "###############################################################\n";
  GetExit();
  return kFALSE;
 }
 if (fpActiveSection->fpRawData->fKeys == 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadRawLine> ######\n"
       << "# Reading data not possible - no keywords read!               #\n"
       << "###############################################################\n";
  GetExit();
  return kFALSE;
 }

 Int_t *pInt;
 Float_t *pFloat;
 Char_t *pChar;

 va_list ap;
 va_start(ap,row);
 for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
  if (fpActiveSection->fpRawData->fpKeyTypeId[i] == 0) {
   pInt = va_arg(ap,Int_t*);
   *pInt = ReadInt(i,row);
  } else if (fpActiveSection->fpRawData->fpKeyTypeId[i] == 1) {
   pFloat = (Float_t*)va_arg(ap,Double_t*);
   *pFloat = ReadFloat(i,row);
  } else if (fpActiveSection->fpRawData->fpKeyTypeId[i] == 2) {
   pChar = va_arg(ap,Char_t*);
   *pChar = ReadChar(i,row);
  } else if (fpActiveSection->fpRawData->fpKeyTypeId[i] == 3) {
   strcopy(va_arg(ap,Char_t*),ReadString(i,row));
  } else if (fpActiveSection->fpRawData->fpKeyTypeId[i] < 0) {
   cerr << "###### HadAscii ###### Error in <HAsciiRaw::ReadRawLine> ######\n"
        << "# Data type for " << i+1 << ". column not specified!\n"
        << "###############################################################\n";
   GetExit();
   return kFALSE;
  }
 }
 va_end(ap);

return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteRawLine(Int_t first, ...) 
{
// Writing line of data. The number and the type of arguments must be in agreement
// with the number of columns (keywords) and their declared types. There is no type
// control and user is fully responsible to put right arguments. (This function
// is used in case of the first argument of Int_t type).

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Writing data not possible - keywords must be written first!  #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }
 if (fpKeyTypeId[0] > 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Specified data type of 1. column is different                #\n"
       << "# than of variable to be written!                              #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "#Data type for 1. column not specified!                        #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }

 Int_t i;
 Double_t fDigit;
 if (CheckOpenedFile()) {
  if (fCounter != 0) {
   fCounter = 0;
   fStreamFileWrite << endl;
  }
  fStreamFileWrite << first << GetSeparator();
  WriteError();

  va_list ap;
  va_start(ap,first);
  for (i = 1; i < fKeys; i++) {
   if (fpKeyTypeId[i] == 0 || fpKeyTypeId[i] == 2) {
    fStreamFileWrite << va_arg(ap,Int_t) << GetSeparator();
   } else if (fpKeyTypeId[i] == 1) {
    fDigit = va_arg(ap,Double_t);
    if (fDigit == 0.0) fStreamFileWrite << "0" << GetSeparator();
     else fStreamFileWrite << fDigit << GetSeparator();
   } else if (fpKeyTypeId[i] == 3) {
    fStreamFileWrite << va_arg(ap,Char_t*) << GetSeparator();
   } else if (fpKeyTypeId[i] < 0) {
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
         << "# Data type for " << i+1 << ". column not specified!\n"
         << "################################################################\n";
    GetExit();
    return kFALSE;
   } 
   WriteError();
  }
  fStreamFileWrite << "\n";
  WriteError();
  va_end(ap);
 }

 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteRawLine(Float_t first, ...) 
{
// Writing line of data. The number and the type of arguments must be in agreement
// with the number of columns (keywords) and their declared types. There is no type
// control and user is fully responsible to put right arguments. (This function
// is used in case of the first argument of Float_t type).

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Writing data not possible - keywords must be written first!  #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }
 if (fpKeyTypeId[0] != 1 && fpKeyTypeId[0] >= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Specified data type of 1. column is different                #\n"
       << "# than of variable to be written!                              #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Data type for 1. column not specified!                       #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }
 
 Int_t i;
 Double_t fDigit;
 if (CheckOpenedFile()) {
  if (fCounter != 0) {
   fCounter = 0;
   fStreamFileWrite << endl;
  }
  fStreamFileWrite << first << GetSeparator();
  WriteError();
  
  va_list ap; 
  va_start(ap,first); 
  for (i = 1; i < fKeys; i++) {
   if (fpKeyTypeId[i] == 0 || fpKeyTypeId[i] == 2) {
    fStreamFileWrite << va_arg(ap,Int_t) << GetSeparator();
   } else if (fpKeyTypeId[i] == 1) {
    fDigit = va_arg(ap,Double_t);
    if (fDigit == 0.0) fStreamFileWrite << "0" << GetSeparator();
     else fStreamFileWrite << fDigit << GetSeparator();
   } else if (fpKeyTypeId[i] == 3) {
    fStreamFileWrite << va_arg(ap,Char_t*) << GetSeparator();
   } else if (fpKeyTypeId[i] < 0) {
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
         << "# Data type for " << i+1 << ". column not specified!\n"
         << "################################################################\n";
    GetExit();
    return kFALSE;
   }
   WriteError();
  }
  fStreamFileWrite << "\n";
  WriteError();
  va_end(ap);
 } 
 
 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteRawLine(Char_t first, ...) 
{
// Writing line of data. The number and the type of arguments must be in agreement
// with the number of columns (keywords) and their declared types. There is no type
// control and user is fully responsible to put right arguments. (This function
// is used in case of the first argument of Char_t type).

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Writing data not possible - keywords must be written first!  #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }
 if (fpKeyTypeId[0] != 2 && fpKeyTypeId[0] >= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Specified data type of 1. column is different                #\n"
       << "# than of variable to be written!                              #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Data type for 1. column not specified!                       #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }
 
 Int_t i;
 Double_t fDigit;
 if (CheckOpenedFile()) {
  if (fCounter != 0) {
   fCounter = 0;
   fStreamFileWrite << endl;
  }
  fStreamFileWrite << first << GetSeparator();
  WriteError();
  
  va_list ap; 
  va_start(ap,first); 
  for (i = 1; i < fKeys; i++) {
   if (fpKeyTypeId[i] == 0 || fpKeyTypeId[i] == 2) {
    fStreamFileWrite << va_arg(ap,Int_t) << GetSeparator();
   } else if (fpKeyTypeId[i] == 1) {
    fDigit = va_arg(ap,Double_t);
    if (fDigit == 0.0) fStreamFileWrite << "0" << GetSeparator();
     else fStreamFileWrite << fDigit << GetSeparator();
   } else if (fpKeyTypeId[i] == 3) {
    fStreamFileWrite << va_arg(ap,Char_t*) << GetSeparator();
   } else if (fpKeyTypeId[i] < 0) {
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
         << "# Data type for " << i+1 << ". column not specified!\n"
         << "################################################################\n";
    GetExit();
    return kFALSE;
   }
   WriteError();
  }
  fStreamFileWrite << "\n";
  WriteError();
  va_end(ap);
 } 
 
 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteRawLine(const Char_t* first, ...)
{
// Writing line of data. The number and the type of arguments must be in agreement
// with the number of columns (keywords) and their declared types. There is no type
// control and user is fully responsible to put right arguments. (This function
// is used in case of the first argument of Char_t* type /string/).

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Writing data not possible - keywords must be written first!  #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }
 if (fpKeyTypeId[0] != 3 && fpKeyTypeId[0] >= 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Specified data type of 1. column is different                #\n"
       << "# than of variable to be written!                              #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
       << "# Data type for 1. column not specified!                       #\n"
       << "################################################################\n";
  GetExit();
  return kFALSE;
 }

 Int_t i;
 Double_t fDigit;
 if (CheckOpenedFile()) {
  if (fCounter != 0) {
   fCounter = 0;
   fStreamFileWrite << endl;
  }
  fStreamFileWrite << first << GetSeparator();
  WriteError();

  va_list ap;
  va_start(ap,first);
  for (i = 1; i < fKeys; i++) {
   if (fpKeyTypeId[i] == 0 || fpKeyTypeId[i] == 2) {
    fStreamFileWrite << va_arg(ap,Int_t) << GetSeparator();
   } else if (fpKeyTypeId[i] == 1) {
    fDigit = va_arg(ap,Double_t);
    if (fDigit == 0.0) fStreamFileWrite << "0" << GetSeparator();
     else fStreamFileWrite << fDigit << GetSeparator();
   } else if (fpKeyTypeId[i] == 3) {
    fStreamFileWrite << va_arg(ap,Char_t*) << GetSeparator();
   } else if (fpKeyTypeId[i] < 0) {
    cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteRawLine> ######\n"
         << "# Data type for " << i+1 << ". column not specified!\n"
         << "################################################################\n";
    GetExit();
    return kFALSE;
   }
   WriteError();
  }
  fStreamFileWrite << "\n";
  WriteError();
  va_end(ap);
 }

 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteInt(Int_t num) 
{
// Writing one Int_t digit. Number of units in line as well as their type
// compared to number of keywords and types is controlled so break-line
// is automatically done. If the type for given column of data was not
// previously declared, it will be Int_t since now.

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteInt> #########\n"
       << "# Writing data not possible - keywords must be written first! #\n"
       << "###############################################################\n";
  return kFALSE;
  GetExit();
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 0;
 if (fpKeyTypeId[fCounter] != 0) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteInt> ######\n"
       << "# Specified data type of " << fCounter+1 << ". column is different\n"
       << "# than of variable to be written!                          #\n"
       << "############################################################\n";
  return kFALSE;
  GetExit();
 }
 if (CheckOpenedFile()) {
  if (fCounter+1 == fKeys) fStreamFileWrite << num << "\n";
   else fStreamFileWrite << num << GetSeparator();
  WriteError();
  fCounter++;
 }
 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteFloat(Float_t num) 
{
// Writing one Float_t digit. Number of units in line as well as their type
// compared to number of keywords and types is controlled so break-line
// is automatically done. If the type for given column of data was not
// previously declared, it will be Float_t since now.

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteFloat> #######\n"
       << "# Writing data not possible - keywords must be written first! #\n"
       << "###############################################################\n";
  return kFALSE;
  GetExit();
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 1;
 if (fpKeyTypeId[fCounter] != 1) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteFloat> #######\n"
       << "# Specified data type of " << fCounter+1 << ". column is different\n"
       << "# than of variable to be written!                             #\n"
       << "###############################################################\n";
  return kFALSE;
  GetExit();
 }
 if (CheckOpenedFile()) {
  if (fCounter+1 == fKeys) {
   if (num == 0.0) fStreamFileWrite << "0\n";
    else fStreamFileWrite << num << "\n";
  } else {
   if (num == 0.0) fStreamFileWrite << "0" << GetSeparator();
    else fStreamFileWrite << num << GetSeparator();
  }
  WriteError();
  fCounter++;
 }
 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteChar(Char_t num) 
{
// Writing one Char_t digit. Number of units in line as well as their type
// compared to number of keywords and types is controlled so break-line
// is automatically done. If the type for given column of data was not
// previously declared, it will be Char_t since now.

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteChar> ########\n"
       << "# Writing data not possible - keywords must be written first! #\n"
       << "###############################################################\n";
  return kFALSE;
  GetExit();
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 2;
 if (fpKeyTypeId[fCounter] != 2) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteChar> ########\n"
       << "# Specified data type of " << fCounter+1 << ". column is different\n"
       << "# than of variable to be written!                             #\n"
       << "###############################################################\n";
  return kFALSE;
  GetExit();
 }
 if (CheckOpenedFile()) {
  if (fCounter+1 == fKeys) fStreamFileWrite << num << "\n";
   else fStreamFileWrite << num << GetSeparator();
  WriteError();
  fCounter++;
 }
 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteString(const Char_t* num)
{
// Writing a string. Number of units in line as well as their type
// compared to number of keywords and types is controlled so break-line
// is automatically done. If the type for given column of data was not
// previously declared, it will be Char_t* (string) since now.

 using namespace StringFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteChar> ########\n"
       << "# Writing data not possible - keywords must be written first! #\n"
       << "###############################################################\n";
  return kFALSE;
  GetExit();
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 3;
 if (fpKeyTypeId[fCounter] != 3) {
  cerr << "###### HadAscii ###### Error in <HAsciiRaw::WriteString> ########\n"
       << "# Specified data type of " << fCounter+1 << ". column is different\n"
       << "# than of variable to be written!                               #\n"
       << "#################################################################\n";
  return kFALSE;
  GetExit();
 }
 if (CheckOpenedFile()) {
  if (fCounter+1 == fKeys) fStreamFileWrite << num << "\n";
   else fStreamFileWrite << num << GetSeparator();
  WriteError();
  fCounter++;
 }
 fKeywords = kFALSE;
 fData = kTRUE;

 return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
void HAsciiRaw::Reset() 
{
// Resetting all data and releasing memory.

 fSection = kFALSE;
 fKeywords = kFALSE;
 fData = kFALSE;
 fCounter = 0;
 
 if (fpKeyTypeId) {
  delete [] fpKeyTypeId;
  fpKeyTypeId = NULL;
 }
 if (fpKeyNames) {
  for (Int_t i = 0; i < fKeys; i++) {
   if (fpKeyNames[i]) delete [] fpKeyNames[i];
  }
  delete [] fpKeyNames;
  fpKeyNames = NULL;
 }
  fKeys = 0;
}
//============================================================================



