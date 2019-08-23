// (hadascii.cc)
// ****************************************************************************
//  Module: HAscii
//  Version: 0.35
// ****************************************************************************

using namespace std;
#include <TDatime.h>
#include <stdlib.h>
#include <stdarg.h>
#include <iostream> 
#include <iomanip>
#include <fstream> 
#include <string.h>
#include "hadascii.h"




// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2001/08/20 by Witold Przygoda

// ***************************************************************************
//
// HadAsciiAuxiliaryFunctions namespace definitions
//


//----------------------------------------------------------------------------
void HadAsciiAuxiliaryFunctions::ErrorMsg(Int_t status, Char_t* name, Int_t arg, ...) {

// Basic function for printing info / warning / error messages. 
// Text is automatically formatted.
// status: 0 - INFO, 1 - WARNING, 2 - ERROR; 
// name: location of the news (eg class::function name); 
// arg: number of arguments; 
// all arguments _must_ be of Char_t* type

Char_t buf[4096];
Char_t buffer[4096];
Char_t buffer2[4096];
Char_t buffer3[4096];
Int_t breakline[40];
Int_t filler, underliner, precounter, counter, pos, posenter, stop, i,j,k,l,m;

for (i=0; i<40; i++) breakline[i] = 0;
buf[0] = '\0';
buffer[0] = '\0';
buffer2[0] = '\0';
buffer3[0] = '\0';

va_list ap;
va_start(ap,arg);
for (i = 0; i < arg; i++) {
 strconcat(buf, va_arg(ap,Char_t*));
}
va_end(ap);

strtrunc(buffer,buf,'\n');
buf[0] = '\0';
strcopy(buf,buffer);
buffer[0] = '\0';
strtrunc(buffer,buf,'\r');
buf[0] = '\0';
strcopy(buf,buffer);
buffer[0] = '\0';
strskipwhite(buffer,buf,'$');
buf[0] = '\0';
strcopy(buf,buffer);
buffer[0] = '\0';

switch (status) {
 case 0: strcopy(buffer2, " | INFO: ");
         break;
 case 1: strcopy(buffer2, " | WARNING: ");
         break;
 case 2: strcopy(buffer2, " | ERROR: ");
         break;
 default: strcopy(buffer2, " | ");
}
strconcat(buffer2,name);
strconcat(buffer2," |");

underliner = strlength(buffer2) - 2;
filler = 75 - underliner;
while (underliner > 0) {
 strconcat(buffer,"-");
 underliner--;
}
strconcat(buffer,"-'");
while (filler > 0) {
 strconcat(buffer2," ");
 strconcat(buffer," ");
 filler--;
}
strconcat(buffer2," |");
strconcat(buffer," |");
buffer[0] = ' ';
buffer[1] = '|';

i = 0;
j = strlength(buf);
precounter = counter = 0;
while (counter < j) {
 pos = strnextchar(buf, counter);
 posenter = strnextchar(buf, counter, '$');
 if (posenter < pos) pos = posenter;
 if (pos < 74) {
  if (counter + pos <= precounter + 74) {
   counter += pos;
   if (counter == j || posenter==pos) breakline[i++] = precounter = counter;
  } else {
   breakline[i] = precounter = counter;
   counter += pos;
   i++;
  }
 } else {
  breakline[i] = precounter = counter;
  counter += pos;
  i++;
 }
}

strreplace(buffer3,buf,'$');
buf[0] = '\0';
strcopy(buf,buffer3);
buffer3[0] = '\0';

i = k = 0;
cout << "\n .----------------------------------------------------------------------------.\n";
cout << buffer2 << endl;
cout << buffer << endl;
do {
 stop = (i == 0) ? 0 : breakline[i-1];
 for (j = 0; j < breakline[i]-stop; j++) {
  buffer[j] = buf[k];
  k++;
 }
 buffer[j] = '\0';
 l = m = 0;
 while (buffer[l] == ' ' || buffer[l] == '\t') l++;
 while ((buffer2[m++] = buffer[l++]));
 buffer[0] = '\0';
 strconcat(buffer, " | ");
 strconcat(buffer, buffer2);

 filler = 77 - strlength(buffer);
 while (filler > 0) {
  strconcat(buffer," ");
  filler--;
 }
 strconcat(buffer," |");

 cout << buffer << endl;
 i++;
} while (breakline[i] != 0);
cout << " `----------------------------------------------------------------------------'\n\n";
}
//============================================================================


// --------------------- string and char operations ---------------------

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strtrunc(Char_t* nameout, const Char_t* namein, Char_t x) {

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
Char_t* HadAsciiAuxiliaryFunctions::strreplace(Char_t* nameout, const Char_t* namein, 
                                               Char_t x) {

// Internally used by ErrorMsg.
// It copies string namein to nameout replacing all x characters with ' '. 
  
 Int_t i = -1, j = 0;
 do {
  i++;
  if (namein[i] != x) nameout[j++] = namein[i];
   else nameout[j++] = ' ';
 } while (namein[i]);
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HadAsciiAuxiliaryFunctions::strempty(const Char_t* namein, Char_t x) {

// It returns true if string namein has not any other character
// besides x (and if x is omitted it means white space).

 if (namein == NULL) return kTRUE;
 Int_t i = -1;
 do {
  i++;
  if (x == ' ') {
   if (namein[i] && namein[i] != ' ' && namein[i] != '\t' &&
       namein[i] != '\r' && namein[i] != '\n') return kFALSE;
  } else {
   if (namein[i] && namein[i] != x) return kFALSE;
  }
 } while (namein[i]);
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HadAsciiAuxiliaryFunctions::strnextchar(const Char_t* namein, Int_t pos, Char_t x) {

// It returns position of next appearance of character x in string namein
// starting counting from position pos.

 if (namein == NULL) return 0;
 if (pos > strlength(namein)) {
  ErrorMsg(2,"HadAsciiAuxiliaryFunctions::strnextchar",
   1,"Internal error: input string length shorter than position argument.");
  throw (Bool_t) kFALSE;
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
Int_t HadAsciiAuxiliaryFunctions::strfirstchar(const Char_t* namein, Int_t startpos) 
{
// It returns position of first character in string namein after startpos 
// which is different from character x (and if x is not specified all 
// white spaces at the beginning are omitted).

 Int_t i = startpos;
  while (namein[i] && (namein[i] == ' ' || namein[i] == '\t')) { i++; };
  if (namein[i]=='\0') return -1;
   else
return i;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HadAsciiAuxiliaryFunctions::strfirstchar(const Char_t* namein, Char_t x) {

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
Char_t HadAsciiAuxiliaryFunctions::strlastchar(const Char_t* namein, Char_t x) {

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
Char_t* HadAsciiAuxiliaryFunctions::strafter(Char_t* nameout, const Char_t* namein, Char_t x) {

// It copies substring of namein to nameout after first appearance of character x.
// White spaces until x and x itself are skipped.

 if (namein == NULL) return nameout;
 Int_t i = 0, j = 0;
 while (namein[i] && namein[i] != x) i++; 
 if (namein[i] && namein[i] == x) i++;
 while ((nameout[j++] = namein[i++]));
return nameout;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strbefore(Char_t* nameout, const Char_t* namein, Char_t x) {

// It copies substring of namein to nameout until the first appearance of character x.

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
void HadAsciiAuxiliaryFunctions::strskipwhite(Char_t* nameout, Char_t* namein, 
                                               Char_t x) {

// Internal function used by ErrorMsg. It rewrites namein to nameout
// but after encountering a character of break line 'x' it skips
// all white spaces until the next character non-white occurs.

 Int_t i = 0, j = 0;
 Bool_t skip = kFALSE;

 while (namein[i]) {
  if (namein[i] == x) skip = kTRUE; 
   else if (namein[i] != ' ' && namein[i] != '\t') skip = kFALSE;
  if (skip && (namein[i] == ' ' || namein[i] == '\t')) i++;
   else nameout[j++] = namein[i++];
 }
 nameout[j] = '\0';
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HadAsciiAuxiliaryFunctions::strunit(Char_t* nameout, Char_t* namein, 
                                          Int_t startpos) { 
// It copies substring of namein to nameout until it finds character x.
// All white spaces at the beginning are skipped and substring copied is replaced
// in nameout by spaces - it means that function destroys namein.
// This version should be used in recursive usage of 'namein' analysis
// and it returns the end position of the operation which is start position
// for the next operation.

 Int_t i = startpos, j = 0;
 while (namein[i] && (namein[i] == ' ' ||
        namein[i] == ',' || namein[i] == '\t')) namein[i++] = ' ';
 while (namein[i] && namein[i] != ' ' &&
        namein[i] != ',' && namein[i] != '\t') {
  nameout[j++] = namein[i++];
  namein[i-1] = ' ';
 }
 if (namein[i] == ',' || namein[i] == '\t') namein[i] = ' ';
 nameout[j] = '\0';
return i;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strunit(Char_t* nameout, Char_t* namein) {

// According to data format it copies substring of namein to nameout
// until it finds delimiter i.e. white space or comma. All white spaces
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
Char_t* HadAsciiAuxiliaryFunctions::strunit(Char_t* nameout, Char_t* namein, Char_t x) {

// It copies substring of namein to nameout until it finds character x.
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
Int_t HadAsciiAuxiliaryFunctions::strlength(const Char_t* namein) {

// String length calculation (termination '\0' character is not
// taken into account.

 if (namein == NULL) return 0;
 if (!strchr(namein,'\0')) {
  ErrorMsg(2,"HadAsciiAuxiliaryFunctions::strlength",
   1,"Internal error: argument is not a string terminater by \'\\0\'.");
  throw (Bool_t) kFALSE;
 }
return (Int_t)(strlen(namein));
} 
//============================================================================

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strcopy(Char_t* nameout, const Char_t* namein) {

// Copying string namein to nameout.

 if (namein == NULL) return nameout;
 if (!strchr(namein,'\0')) {
  ErrorMsg(2,"HadAsciiAuxiliaryFunctions::strcopy",
   1,"Internal error: argument is not a string terminater by \'\\0\'.");
  throw (Bool_t) kFALSE;
 }
return strcpy(nameout,namein);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strtolower(Char_t* nameout, const Char_t* namein) {

// Copying namein to nameout and changing all characters to low.

 if (namein == NULL) return nameout;
 Int_t i = 0;
 if (!strchr(namein,'\0')) {
  ErrorMsg(2,"HadAsciiAuxiliaryFunctions::strtolower",
   1,"Internal error: argument is not a string terminater by \'\\0\'.");
  throw (Bool_t) kFALSE;
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
Int_t HadAsciiAuxiliaryFunctions::strcompare(const Char_t* nameout, const Char_t* namein) {

// Comparison of strings nameout and namein. If strings are the same 0 is returned.

 if (nameout && namein==NULL) return 1;
  else if (nameout==NULL && namein) return -1;
   else if (nameout==NULL && namein==NULL) return 0;
return strcmp(nameout,namein);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strconcat(Char_t* nameout, const Char_t* namein) {

// String namein concatenated to nameout.

 if (namein == NULL) return nameout;
 if (!strchr(namein,'\0')) {
  ErrorMsg(2,"HadAsciiAuxiliaryFunctions::strconcat",
   1,"Internal error: argument is not a string terminater by \'\\0\'.");
  throw (Bool_t) kFALSE;
 }
return strcat(nameout,namein);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HadAsciiAuxiliaryFunctions::strnconcat(Char_t* nameout, const Char_t* namein, Int_t begin, Int_t end) {

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
//*-- Modified : 2001/08/20 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiSectionRaw::HAsciiSectionRaw",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionRaw& HAsciiSectionRaw::operator=(const HAsciiSectionRaw& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) {
  ErrorMsg(2,"HAsciiSectionRaw::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
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
//*-- Modified : 2000/02/11 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiSection::HAsciiSection",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSection& HAsciiSection::operator=(const HAsciiSection& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) { 
  ErrorMsg(2,"HAsciiSection::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
//*-- Modified : 2000/02/11 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiSectionList::HAsciiSectionList",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiSectionList& 
        HAsciiSectionList::operator=(const HAsciiSectionList& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) { 
  ErrorMsg(2,"HAsciiSectionList::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (NodeNumber()) {
  ToFirstNode();
  if (strlength(name)==0 || strempty(name)) {
   ErrorMsg(1,"HAsciiSectionList::ToNodeSection",
    1,"Section name not specified. Ptr to first section in list will be returned.");
   return &(fpThisNode->fSection);
  }
  while (fpThisNode && 
         strcompare(name,ThisNodeSection()->GetSectionName()) != 0) {
   ToNextNode();
  }
  if (fpThisNode) return &(fpThisNode->fSection);
  ErrorMsg(2,"HAsciiSectionList::ToNodeSection",
   3,"Section [",name,"] not found.");
  throw (Bool_t) kFALSE;
 } else { 
  ErrorMsg(2,"HAsciiSectionList::ToNodeSection",
   1,"None section in the list.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiSectionList::RestoreThisNode",
   1,"Internal error: fpOpenNode has no fpThisNode value stored.");
  throw (Bool_t) kFALSE;
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
//*-- Modified : 2000/02/11 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiFile::HAsciiFile",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFile& HAsciiFile::operator=(const HAsciiFile& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) {
  ErrorMsg(2,"HAsciiFile::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 fStreamFileRead.open(filename, ios::in);

 if (fStreamFileRead.fail()) {
  ErrorMsg(2,"HAsciiFile::OpenFile",
   3,"File \'",filename,"\' not found.");
  fStreamFileRead.clear(fStreamFileRead.rdstate() & ~ios::badbit);
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 fStreamFileRead.close();

 if (fStreamFileRead.fail()) {
  ErrorMsg(2,"HAsciiFile::CloseFile",
   3,"Unexpected error during closing file \'",GetFileName(),"\'.");
  fStreamFileRead.clear(fStreamFileRead.rdstate() & ~ios::badbit);
  Reset();
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
    if (pData) delete [] pData;
    fPosNow = fPosition;
    fStreamFileRead.seekg(iStreamPos);
    ErrorMsg(2,"HAsciiFile::ReadSectionComment",
     3,"In file \'",GetFileName(),"\' unexpected error during  \
searching commentaries for section.");
    throw (Bool_t) kFALSE;
   }
   if (strlastchar(cLine)=='\r') cLine[strlength(cLine)-1] = ' ';
   // this is in case of DOS files since '\r' is included in the line!

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

 using namespace HadAsciiAuxiliaryFunctions;
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
     ErrorMsg(2,"HAsciiFile::InitSectionList",
      3,"In file \'",GetFileName(),"\' unexpected error  \
during searching section name.");
     throw (Bool_t) kFALSE;
    }
    if (strlastchar(cLine)=='\r') cLine[strlength(cLine)-1] = ' ';
    // this is in case of DOS files since '\r' is included in the line!

    if (strfirstchar(cLine) == '[') {

     if (lSecBegin) {
      strbefore(cData,cLine,'[');
      lSecEnd = fPosNow + strlength(cData);
      if (lSecEnd <= lSecBegin) {
       ErrorMsg(2,"HAsciiFile::InitSectionList",
        1,"Internal error: section begin <= section end - please report it to the author.");
       throw (Bool_t) kFALSE;
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
       lSecBegin = 0;
       ErrorMsg(2,"HAsciiFile::InitSectionList",
        5,"In file \'",GetFileName(),"\' section name [",cData,"...\' longer than 127 characters.");
       throw (Bool_t) kFALSE;
      }
     } else {
      fPosNow = fStreamFileRead.tellg();
      if (fPosNow >= fFileSize) {
       fFileChange = 0;
       ErrorMsg(2,"HAsciiFile::InitSectionList",
        3,"End of file \'",GetFileName(),"\' reached during reading section name.");
       throw (Bool_t) kFALSE;
      } else {
       ErrorMsg(2,"HAsciiFile::InitSectionList",
        5,"In file \'",GetFileName(),"\' section name [",cLine," not terminated by \"]\".");
       throw (Bool_t) kFALSE;
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
       ErrorMsg(2,"HAsciiFile::InitSectionList",
        1,"Internal error: section begin <= section end. Please report it to the author.");
       throw (Bool_t) kFALSE;
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
      ErrorMsg(2,"HAsciiFile::InitSectionList",
       1,"Internal error: section begin <= section end. Please report it to the author.");
      throw (Bool_t) kFALSE;
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
//*-- Modified : 2000/02/11 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiFileList::HAsciiFileList",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiFileList& HAsciiFileList::operator=(const HAsciiFileList& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) {
  ErrorMsg(2,"HAsciiFileList::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiFileList::ToNodeFile",
   3,"File \'",name,"\' not opened.");
  throw (Bool_t) kFALSE;
 } else if (strempty(name) == kFALSE) {
  ErrorMsg(2,"HAsciiFileList::ToNodeFile",
   3,"File \'",name,"\' not opened.");
  // throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiFileList::RestoreThisNode",
   1,"Internal error: fpOpenNode has no fpThisNode value stored.");
  throw (Bool_t) kFALSE;
 }
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2000/02/11 by Witold Przygoda

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

 using namespace HadAsciiAuxiliaryFunctions;
 fReadAllPosition = 0;
 fKeyLine[0] = '\0';
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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAscii::HAscii",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAscii& HAscii::operator=(const HAscii& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) {
  ErrorMsg(2,"HAscii::operator=",
   1,"Object cannot be initialised by assignment.");
 throw (Bool_t) kFALSE;
 }
return *this;
}
//============================================================================


//----------------------------------------------------------------------------
Char_t* HAscii::CorrectName(Char_t* name) 
{
// Function called while opening file error happened to correct file name.

 using namespace HadAsciiAuxiliaryFunctions;
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

 using namespace HadAsciiAuxiliaryFunctions;
 Int_t iFlag;
 list.StoreThisNode();
 if (list.NodeNumber()) {
  list.ToFirstNode();
  do {
   for (iFlag=0; !iFlag; )
    if (strcompare(name,list.ThisNodeFile()->GetFileName())==0) {
     ErrorMsg(1,"HAscii::CheckFileList",
      3,"File \'",name,"\' already opened.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fStreamFileWrite.fail()) {
  ErrorMsg(2,"HAscii::WriteError",
   3,"Error while writing data to file \'",fFileWriteName,"\' (maybe disk full?).");
  fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
  fStreamFileWrite.close();
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (character != ' ' && character != '\t' && character != ',') {
  ErrorMsg(1,"HAscii::SetSeparator",
   1,"Incorrect separator as argument: space, tabulation (\\t) or  \
comma allowed (now set to default - space).");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fFileWriteState > 0) {
  ErrorMsg(1,"HAscii::CloseOpenedFile",
   3,"There is already file \'",fFileWriteName,"\' opened in write mode. It will be closed now.");
  fStreamFileWrite << endl;
  WriteError();
  fStreamFileWrite.close();
  if (fStreamFileWrite.fail()) {
   ErrorMsg(2,"HAscii::CloseOpenedFile",
    3,"Unexpected error during closing file \'",fFileWriteName,"\'.");
   fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
   throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(1,"HAscii::OpenFile",
   5,"File mode \'",cMode,"\' not known. File \'",tempname,"\' will be opened in \'read\' mode.");
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
# warning "open ios::noreplace not anylonger supported in gcc 3.2!!! \n \
    some work has to be done here."
    case 1: //fStreamFileWrite.open(tempname, ios::out | ios::noreplace);
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
    ErrorMsg(2,"HAscii::OpenFile",
     3,"File \'",tempname,"\' cannot be opened for writing.");
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

 ErrorMsg(2,"HAscii::OpenFile",
  1,"Function achieved the end not returning proper fstream object.  \
Please report it to the author.");
 throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
  ResetKeyData();
  ResetBankData();
  CloseFile(fFileNameList);
  ErrorMsg(2,"HAscii::OpenFileList",
   3,"File list in file \'",fFileNameList,"\' is empty.");
  throw (Bool_t) kFALSE;
 } 
 pData = new Char_t[iLong + 1];
 pData[0] = '\0';
 strcopy(pData,ReadString());
 if (strfirstchar(pData) == '\0') {
  ResetKeyData();
  ResetBankData();
  delete [] pData;
  pData = NULL;
  CloseFile(fFileNameList);
  ErrorMsg(2,"HAscii::OpenFileList",
   3,"File list in file \'",fFileNameList,"\' is empty.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
    ErrorMsg(2,"HAscii::CloseFile",
     3,"Unexpected error during closing file \'",tempname,"\'.");
    fStreamFileWrite.clear(fStreamFileWrite.rdstate() & ~ios::badbit);
    throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
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

      ErrorMsg(2,"HAscii::CheckUnambiguitySection",
       5,"Section name [",pSectionNode->GetSectionName(),"] occurs in  \
file: \'",pFileNode->GetFileName(),"\' more than once.");
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
         ErrorMsg(2,"HAscii::CheckUnambiguitySection",
          7,"Section name [",pSectionNode->GetSectionName(),"] repeats in files: \'",
pFileNode->GetFileName(),"\' and \'",fFileList.ThisNodeFile()->GetFileName(),"\'.");
        iAmbiguityFlag = 1;
       }
     }

    }
   }

  }
 }

 if (iAmbiguityFlag) {
  ErrorMsg(2,"information",
   1,"Section names in all opened files must be unambiguous. Correct it and run again.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cSecName[128];
 cSecName[0] = '\0';

 strtrunc(cSecName,name);
 if (fpActiveSection && !strcompare(cSecName,
     fpActiveSection->GetSectionName())) {
  if (fReadAllPosition == 0) {
   ErrorMsg(1,"HAscii::SetActiveSection",
    3,"Section [",cSecName,"] is already selected as active.");
  } else fReadAllPosition = 0;
  return fpActiveSection;
 }

 fKeyLine[0] = '\0'; // bug fix by WP 19 Nov 2000
 

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

 ErrorMsg(1,"HAscii::SetActiveSection",
  3,"No section [",cSecName,"] in any opened ASCII file for this runtime database input.");
 fpActiveFile = NULL;
 fReadAllPosition = 0;
 throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 fDataCounter = 0;
 switch (fDataBank) {
   case 1: if (fpDataInt) {
            delete [] fpDataInt;
            fpDataInt = NULL;
           }
           break;
   case 2: if (fpDataFloat) {
            delete [] fpDataFloat;
            fpDataFloat = NULL;
           }
           break;
   case 3: if (fpDataChar) {
            delete [] fpDataChar;
            fpDataChar = NULL;
           }
           break;
   case 4: if (fpDataString) {
            delete [] fpDataString;
            fpDataString = NULL;
           }
           break;
   default: if (fpDataInt || fpDataFloat || fpDataChar || fpDataString) {
             ErrorMsg(2,"HAscii::ResetBankData",
              1,"Internal error: inconsistency in data bank management.  \
Please report it to the author.");
             throw (Bool_t) kFALSE;
            }
 }
 fDataBank = 0;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckSection() 
{
// Checking if section is selected.

 using namespace HadAsciiAuxiliaryFunctions;
 if (!fpActiveSection) {
  ErrorMsg(2,"HAscii::CheckSection",
   1,"None section selected.");
  throw (Bool_t) kFALSE;
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckData() 
{
// Checking if keyword just read has any data.

 using namespace HadAsciiAuxiliaryFunctions;
 if (!CheckSection()) return kFALSE;
 if (!fDataCounter) {
  ErrorMsg(2,"HAscii::CheckData",
   5,"Keyword \'",fKeyLine,"\' in section [",fpActiveSection->GetSectionName(),
"] has no data.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (typ >= 'A' && typ <= 'Z') typ += 32;
 if (typ != 'i' && typ != 'f' && typ != 'c' && typ != 's') {
  char buf[2];
  buf[0] = typ;
  buf[1] = '\0';
  ErrorMsg(2,"HAscii::CheckDataType",
   3,"Data type \'",buf,"\' not supported (only: i - Int_t, f - Float_t,  \
c - Char_t, s - string).");
  throw (Bool_t) kFALSE;
 }
 if ((typ == 'i' && fDataBank != 1) ||
     (typ == 'f' && fDataBank != 2) ||
     (typ == 'c' && fDataBank != 3) ||
     (typ == 's' && fDataBank != 4)) {
  Char_t tempbuf[10];
  tempbuf[0] = '\0';
  if (fDataBank==1) strcopy(tempbuf,"Int_t");
   else if (fDataBank==2) strcopy(tempbuf,"Float_t");
    else if (fDataBank==3) strcopy(tempbuf,"Char_t");
     else if (fDataBank==4) strcopy(tempbuf,"string");
  ErrorMsg(2,"HAscii::CheckDataType",
   5,"Type mismatch: data of keyword \'",fKeyLine,"\' have been started to be read as \'",
tempbuf,"\'.");
  throw (Bool_t) kFALSE;
 }
return kTRUE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAscii::CheckDataPosition(Int_t pos) 
{
// Checking if pos is not larger than number of data units;
// in case of string there is always only 1 unit.

 using namespace HadAsciiAuxiliaryFunctions;
 if (pos >= fDataCounter) {
  Char_t tempbuf[10];
  tempbuf[0] = '\0';
  if (fDataBank==1) strcopy(tempbuf,"Int_t");
   else if (fDataBank==2) strcopy(tempbuf,"Float_t");
    else if (fDataBank==3) strcopy(tempbuf,"Char_t");
     else if (fDataBank==4) strcopy(tempbuf,"string");
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",pos);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(2,"HAscii::CheckDataPosition",
   9,"Position (",buf,") out of data range: keyword \'",fKeyLine,"\' contains ",
buf2," data units of type \'",tempbuf,"\'.");
  throw (Bool_t) kFALSE;
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
return fpDataString;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadString(const Char_t* key) 
{
// Reading data of given keyword key as string.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return NULL;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToString();
 }
return ReadString();
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ReadString(const Char_t* sec, const Char_t* key) 
{
// Reading data of given keyword key in section sec as string.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToString();
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (num < 1) {
  ErrorMsg(1,"HAscii::SetNumInLine",
   1,"Number of digits (characters) in line must be  \
a positive value (now set to default = 16).");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (!fFileWriteState) {
  ErrorMsg(1,"HAscii::CheckOpenedFile",
   1,"Writing to file not possible: no file opened.");
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

 using namespace HadAsciiAuxiliaryFunctions;
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
       ErrorMsg(2,"HAscii::WriteLine",
        1,"Line without any separator is too long (more than 1023 characters).");
       throw (Bool_t) kFALSE;
      } else {
      // substring too long but it will be written
       ErrorMsg(2,"HAscii::WriteLine",
        1,"Substring of line without separator longer than given limit.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (sign != ';' && sign != '#') {
  Char_t buf[2];
  buf[0] = sign;
  buf[1] = '\0';
  ErrorMsg(1,"HAscii::WriteRemark",
   3,"Unrecognized character \'",buf,"\'. Commentary lines may begin \
only with \';\' or \'#\' (now set to default = ;).");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (strlength(name) > 127) {
  ErrorMsg(2,"HAscii::WriteSection",
   1,"Section name to be written is too long (more than 127 characters).");
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

 using namespace HadAsciiAuxiliaryFunctions;
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
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    ErrorMsg(2,"HAscii::CopySection",
     5,"In file \'",fpActiveFile->GetFileName(),"\' unexpected error during \
copying  section [",fpActiveSection->GetSectionName(),"].");
    throw (Bool_t) kFALSE;
   }
   if (c=='\r') c = ' ';
   // this is in case of DOS files since '\r' is included in the line!

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

 using namespace HadAsciiAuxiliaryFunctions;
 Long_t lSecBegin = 0, lSecEnd = 0, lPos = 0;
 Char_t cLine[1024], cKey[256];
 cLine[0] = '\0';
 cKey[0] = '\0';
 Bool_t fNextKey = kFALSE;
 
 if (!fpActiveSection || !fpActiveFile) {
  ErrorMsg(2,"HAscii::IsNextKeyword",
   1,"None section selected.");
  throw (Bool_t) kFALSE;
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
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    ErrorMsg(2,"HAscii::IsNextKeyword",
     5,"In file \'",fpActiveFile->GetFileName(),"\' unexpected \
error during reading in section [",fpActiveSection->GetSectionName(),"].");
    throw (Bool_t) kFALSE;
   }
   if (strlastchar(cLine)=='\r') cLine[strlength(cLine)-1] = ' ';
   // this is in case of DOS files since '\r' is included in the line!

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

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAscii::ReadKeywordLine",
   1,"None section selected.");
  throw (Bool_t) kFALSE;
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
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    ErrorMsg(2,"HAscii::ReadKeywordLine",
     5,"In file \'",fpActiveFile->GetFileName(),"\' unexpected \
error during reading in section [",fpActiveSection->GetSectionName(),"].");
    throw (Bool_t) kFALSE;
   }
   if (strlastchar(cLine)=='\r') cLine[strlength(cLine)-1] = ' ';
   // this is in case of DOS files since '\r' is included in the line!

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
        fpActiveFile = NULL;
        fpActiveSection = NULL;
        ErrorMsg(2,"HAscii::ReadKeywordLine",
         7,"In file \'",fpActiveFile->GetFileName(),"\' unexpected \
error during reading data for keyword \'",fKeyLine,"\'in \
section [",fpActiveSection->GetSectionName(),"].");
        throw (Bool_t) kFALSE;
        if (fpDataLine) {
         delete [] fpDataLine;
         fpDataLine = NULL;
        }
        fDataLineLength = 0; 
        for (i=0; i<128; i++) fKeyLine[i] = '\0';
        return NULL;
       }
       if (strlastchar(cLine)=='\r') cLine[strlength(cLine)-1] = ' ';
       // this is in case of DOS files since '\r' is included in the line!

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
    if (strempty(cName)) {
     ErrorMsg(2,"HAscii::ReadKeywordLine", 
      5,"In section [",fpActiveSection->GetSectionName(),"] in file \'", 
fpActiveFile->GetFileName(),"\' next line with any keyword not found (probably the end of section has been reached during sequential reading).");
    } else {
     ErrorMsg(2,"HAscii::ReadKeywordLine",
      7,"In section [",fpActiveSection->GetSectionName(),"] in file \'",
fpActiveFile->GetFileName(),"\' keyword \'",cName,"\' not found.");
    }
    throw (Bool_t) kFALSE;
   }
   cLine[0] = '\0';
   cKey[0] = '\0';
   cKeyUnpacked[0] = '\0';
  } else {
    if (strempty(cName)) {
     ErrorMsg(2,"HAscii::ReadKeywordLine",
      5,"In section [",fpActiveSection->GetSectionName(),"] in file \'", 
fpActiveFile->GetFileName(),"\' next line with any keyword not found (probably the end of section has been reached during sequential reading).");
    } else {
     ErrorMsg(2,"HAscii::ReadKeywordLine",
      7,"In section [",fpActiveSection->GetSectionName(),"] in file \'", 
fpActiveFile->GetFileName(),"\' keyword \'",cName,"\' not found.");
    }
   throw (Bool_t) kFALSE;
  }
 } while (lPos < lSecEnd);

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t* HAscii::ConvertToInt() 
{
// Conversion of string of data stored in fpDataLine to units of Int_t type.

 using namespace HadAsciiAuxiliaryFunctions;
 Int_t i = 0, j = 0;
 Int_t iPos = 0;
 Int_t fMaxDataLength = strlength(fpDataLine)/2 + 1;
 
 Char_t cUnit[30];
 cUnit[0] = '\0';

 if (fDataBank != 1) fDataBank = 1;
 fpDataInt = new Int_t[fMaxDataLength];
 for (i=0; i<fMaxDataLength; i++) fpDataInt[i]=0;

 i = 0;
 if (fpDataLine) 
  while (strfirstchar(fpDataLine,iPos)>=0) {
   iPos = strunit(cUnit,fpDataLine,iPos);
   j = 0;
   while (cUnit[j]) {

    if (cUnit[0]=='0' || ((cUnit[0]=='-' || cUnit[0]=='+') && 
        cUnit[1]=='0')) {
     if (!((cUnit[j] >= '0' && cUnit[j] <= '7') || 
         cUnit[j]=='-' || cUnit[j]=='+')) {
      ResetKeyData();
      ResetBankData();
      ErrorMsg(2,"HAscii::ConvertToInt",
       7,"Keyword \'",fKeyLine,"\' in section [",
fpActiveSection->GetSectionName(),"] in file \'",fpActiveFile->GetFileName(),
"\' contains a digit in octal system but with characters out of octal system.");
      throw (Bool_t) kFALSE;
     }
    } else if ((cUnit[0]=='0' && (cUnit[1]=='x' || cUnit[1]=='X')) ||
               ((cUnit[0]=='-' || cUnit[0]=='+') && 
                (cUnit[1]=='0' && (cUnit[2]=='x' || cUnit[2]=='X')))) {
     if (!((cUnit[j] >= '0' && cUnit[j] <= '9') || cUnit[j]=='a' || 
         cUnit[j]=='A' ||
           cUnit[j]=='b' || cUnit[j]=='B' || cUnit[j]=='c' || cUnit[j]=='C' ||
           cUnit[j]=='d' || cUnit[j]=='D' || cUnit[j]=='e' || cUnit[j]=='E' ||
           cUnit[j]=='x' || cUnit[j]=='X' || cUnit[j]=='-' || cUnit[j]=='+')) {
      ResetKeyData();
      ResetBankData();
      ErrorMsg(2,"HAscii::ConvertToInt",
       7,"Keyword \'",fKeyLine,"\' in section [",
fpActiveSection->GetSectionName(),"] in file \'",fpActiveFile->GetFileName(),
"\' contains a digit in hexadecimal system but with characters out of hexadecimal system.");
      throw (Bool_t) kFALSE;
     }
    } else if (!((cUnit[j] >= '0' && cUnit[j] <= '9') || 
               cUnit[j]=='e' || cUnit[j]=='E' ||
          cUnit[j]=='.' || cUnit[j]=='-' || cUnit[j]=='+')) {
     ResetKeyData();
     ResetBankData();
     ErrorMsg(2,"HAscii::ConvertToInt",
      7,"Keyword \'",fKeyLine,"\' in section [",fpActiveSection->GetSectionName(),
"] in file \'",fpActiveFile->GetFileName(),"\' contains a digit with characters \
out of decimal system.");
     throw (Bool_t) kFALSE;
    }

    j++;
   }

            if (strchr(cUnit,'.')) {
             fpDataInt[i++] = (Int_t)atof(cUnit);
            } else {
             fpDataInt[i++] = strtol(cUnit,NULL,0);
            }
            fDataCounter++;
            cUnit[0] = '\0';
  }

return fpDataInt;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t* HAscii::ConvertToFloat() 
{
// Conversion of string of data stored in fpDataLine to units of Float_t type.

 using namespace HadAsciiAuxiliaryFunctions;
 Int_t i = 0, j = 0;
 Int_t iPos = 0;
 Int_t fMaxDataLength = strlength(fpDataLine)/2 + 1;
 
 Char_t cUnit[30];
 cUnit[0] = '\0';

 if (fDataBank != 2) fDataBank = 2;
 fpDataFloat = new Float_t[fMaxDataLength];
 for (i=0; i<fMaxDataLength; i++) fpDataFloat[i]=0.;

 i = 0;
 if (fpDataLine)
  while (strfirstchar(fpDataLine,iPos)>=0) {
   iPos = strunit(cUnit,fpDataLine,iPos);
   j = 0;
   while (cUnit[j]) {
    if (!((cUnit[j] >= '0' && cUnit[j] <= '9') || cUnit[j]=='e' || 
        cUnit[j]=='E' ||
          cUnit[j]=='.' || cUnit[j]=='-' || cUnit[j]=='+')
       ) {
     ResetKeyData();
     ResetBankData();
     ErrorMsg(2,"HAscii::ConvertToFloat",
      7,"Keyword \'",fKeyLine,"\' in section [",fpActiveSection->GetSectionName(),
"] in file \'",fpActiveFile->GetFileName(),"\' contains non-digit characters.");
     throw (Bool_t) kFALSE;
    }
    j++;
   }

            fpDataFloat[i++] = atof(cUnit);
            fDataCounter++;
            cUnit[0] = '\0';

  }

return fpDataFloat;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ConvertToChar() 
{
// Conversion of string of data stored in fpDataLine to units of Char_t type.
// Char_t unit is a single Char_t separated in string by white space or comma.

 using namespace HadAsciiAuxiliaryFunctions;
 Int_t i = 0;
 Int_t iPos = 0;
 Int_t fMaxDataLength = strlength(fpDataLine)/2 + 1;
 
 Char_t cUnit[30];
 cUnit[0] = '\0';

 if (fDataBank != 3) fDataBank = 3;
 fpDataChar = new Char_t[fMaxDataLength];
 for (i=0; i<fMaxDataLength; i++) fpDataChar[i]='\0';

 i = 0;
 if (fpDataLine)
  while (strfirstchar(fpDataLine,iPos)>=0) {
   iPos = strunit(cUnit,fpDataLine,iPos);
   if (strlength(cUnit) > 1) {
    ResetKeyData();
    ResetBankData();
    ErrorMsg(2,"HAscii::ConvertToChar",
     7,"Keyword \'",fKeyLine,"\' in section [",fpActiveSection->GetSectionName(),
"] in file \'",fpActiveFile->GetFileName(),"\' contains strings longer than 1 character.");
    throw (Bool_t) kFALSE;
   }

            fpDataChar[i++] = cUnit[0];
            fDataCounter++;
            cUnit[0] = '\0';
  }

return fpDataChar;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAscii::ConvertToString() 
{
// Conversion of string of data stored in fpDataLine to string unit.
// If string begins and ends with quotation marks they are removed.
// String unit is a set of characters terminated by '\0'. 

 using namespace HadAsciiAuxiliaryFunctions;
 Int_t i = 0, j = 0;
 Char_t *pDLine = NULL;
 if (fDataBank != 4) fDataBank = 4;
 fpDataString = new Char_t[strlength(fpDataLine) + 1];

 if (fpDataLine)
   if (strfirstchar(fpDataLine)=='\"' && strlastchar(fpDataLine)=='\"') {
    pDLine = new Char_t[strlength(fpDataLine) + 1];
    pDLine[0] = '\0';
    strafter(pDLine,fpDataLine,'\"');
    i = strlength(pDLine) - 1;
    while (pDLine[i] != '\"') { i--; }
    j = 0;
    while (j<i) {
     fpDataString[j] = pDLine[j];
     j++;
    }
    fpDataString[j] = '\0';
    delete [] pDLine;
    pDLine = NULL;
   } else {
    fpDataString[0] = '\0';
    strcopy(fpDataString,fpDataLine);
   }
   fDataCounter++; // always 1 as there is only one string possible

return fpDataString;
}
//============================================================================


// $$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$


//*-- Author : Witold Przygoda (przygoda@psja1.if.uj.edu.pl)
//*-- Modified : 2001/08/20 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiKey::HAsciiKey",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiKey& HAsciiKey::operator=(const HAsciiKey& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) {
  ErrorMsg(2,"HAsciiKey::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
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
   case 1: return (void*)fpDataInt;
   case 2: return (void*)fpDataFloat;
   case 3: return (void*)fpDataChar;
   case 4: return (void*)fpDataString;
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
   case 'i': return (void*)fpDataInt;
   case 'f': return (void*)fpDataFloat;
   case 'c': return (void*)fpDataChar;
   case 's': return (void*)fpDataString;
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
return fpDataInt[pos];
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiKey::ReadFloat(Int_t pos) 
{
// Readnig Float_t number at the position pos (from data bank) for keyword just read.

 if (!CheckData() || !CheckDataType('f') ||
     !CheckDataPosition(pos)) return 0.;
return fpDataFloat[pos];
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiKey::ReadChar(Int_t pos) 
{
// Reading Char_t character at the position pos (from data bank) for keyword just read. 

 if (!CheckData() || !CheckDataType('c') ||
     !CheckDataPosition(pos)) return '\0';
return fpDataChar[pos];
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiKey::ReadInt(const Char_t* key, Int_t pos) 
{
// Reading Int_t number at the position pos in keyword key.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return 0;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToInt();
 }
return ReadInt(pos); 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiKey::ReadInt(const Char_t* sec, const Char_t* key, Int_t pos) 
{
// Reading Int_t number at the position pos in keyword key in section sec.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToInt();
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (number <= 0) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",number);
  ErrorMsg(2,"HAsciiKey::ReadInt",
   3,"Argument number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadInt(key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadInt(i);
 } else {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadInt",
   9,"Argument number = ",buf," exceeds number of Int_t data units (",
buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (number <= 0) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",number);
  ErrorMsg(2,"HAsciiKey::ReadInt",
   3,"Argument number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadInt(sec,key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadInt(i);
 } else {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadInt",
   9,"Argument number = ",buf," exceeds number of Int_t data units (",
buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (nrfrom < 0 || number < 0) {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",nrfrom);
  ErrorMsg(2,"HAsciiKey::ReadInt",
   5,"Argument nrfrom = ",buf2," and number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadInt(key,0);
 if ((nrfrom + number) > fDataCounter) {
  Int_t it = nrfrom+number;
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",it);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadInt", 
   9,"End value nrfrom + number = ",buf," exceeds number of Int_t data units (",
   buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");

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

 using namespace HadAsciiAuxiliaryFunctions;
 if (nrfrom < 0 || number < 0) {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",nrfrom);
  ErrorMsg(2,"HAsciiKey::ReadInt",
   5,"Argument nrfrom = ",buf2," and number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadInt(sec,key,0);
 if ((nrfrom + number) > fDataCounter) {
  Int_t it = nrfrom+number;
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",it);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadInt",
   9,"End value nrfrom + number = ",buf," exceeds \
number of Int_t data units (",buf2,") of keyword \'",
key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return 0.;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToFloat();
 }
return ReadFloat(pos);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiKey::ReadFloat(const Char_t* sec, const Char_t* key, Int_t pos) 
{
// Reading Float_t number at the position pos in keyword key in section sec.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToFloat();
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (number <= 0) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",number);
  ErrorMsg(2,"HAsciiKey::ReadFloat",
   3,"Argument number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadFloat(key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadFloat(i);
 } else {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadFloat",
   9,"Argument number = ",buf," exceeds number of Float_t data units (",
buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (number <= 0) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",number);
  ErrorMsg(2,"HAsciiKey::ReadFloat",
   3,"Argument number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadFloat(sec,key,0);
 if (number <= fDataCounter) {
  for (i = 0; i < number; i++) putin[i] = ReadFloat(i);
 } else {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadFloat",
   9,"Argument number = ",buf," exceeds number of Float_t data units (",
buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (nrfrom < 0 || number < 0) {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",nrfrom);
  ErrorMsg(2,"HAsciiKey::ReadFloat",
   5,"Argument nrfrom = ",buf2," and number = ",buf," must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadFloat(key,0);
 if ((nrfrom + number) > fDataCounter) {
  Int_t it = nrfrom+number;
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",it);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadFloat",
   9,"End value nrfrom + number = ",buf," exceeds number of Float_t data units (",
buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (nrfrom < 0 || number < 0) {
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",number);
  sprintf(buf2,"%d",nrfrom);
  ErrorMsg(2,"HAsciiKey::ReadFloat",
   5,"Argument nrfrom = ",buf2," and number = ",buf,
" must be a positive value.");
  throw (Bool_t) kFALSE;
 }
 Int_t i;
 ReadFloat(sec,key,0);
 if ((nrfrom + number) > fDataCounter) {
  Int_t it = nrfrom+number;
  Char_t buf[11];
  Char_t buf2[11];
  buf[0] = '\0';
  buf2[0] = '\0';
  sprintf(buf,"%d",it);
  sprintf(buf2,"%d",fDataCounter);
  ErrorMsg(1,"HAsciiKey::ReadFloat",
   9,"End value nrfrom + number = ",buf," exceeds number of Float_t data units (",
buf2,") of keyword \'",key,"\' in file \'",fpActiveFile->GetFileName(),"\'.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (typ >= 'A' && typ <= 'Z') typ += 32;
 if (!CheckSection()) return NULL;
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  switch (typ) {
    case 'i': ConvertToInt();
              break;
    case 'f': ConvertToFloat();
              break;
    case 'c': ConvertToChar();
              break;
    case 's': ConvertToString();
              break;
     default: Char_t buf[2];
              buf[0] = typ;
              buf[0] = '\0';
              ErrorMsg(2,"HAsciiKey::Read",
               3,"Data type \'",buf,"\' not supported. Supported are: \
i - Int_t, f - Float_t, c - Char_t, s - string.");
              throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (typ >= 'A' && typ <= 'Z') typ += 32;
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  switch (typ) {
    case 'i': ConvertToInt();
              break;
    case 'f': ConvertToFloat();
              break;
    case 'c': ConvertToChar();
              break;
    case 's': ConvertToString();
              break;
     default: Char_t buf[2];
              buf[0] = typ;
              buf[1] = '\0';
              ErrorMsg(2,"HAsciiKey::Read",
               3,"Data type \'",buf,"\' not supported. Supproted are: \
i - Int_t, f - Float_t, c - Char_t, s - string.");
              throw (Bool_t) kFALSE;
  }
 }
return Read();
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiKey::ReadChar(const Char_t* key, Int_t pos) 
{
// Reading Char_t character at the position pos in keyword key.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 if (!CheckSection()) return '\0';
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToChar();
 }
return ReadChar(pos);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiKey::ReadChar(const Char_t* sec, const Char_t* key, Int_t pos) 
{
// Reading Char_t character at the position pos in keyword key in section sec.

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cKey[128];
 cKey[0] = '\0';
 SetActiveSection(sec);
 strtrunc(cKey,key);
 if (strcompare(cKey,fKeyLine) != 0) {
  if (strfirstchar(cKey) == '*') ReadKeywordLine();
  else ReadKeywordLine(key);
  ConvertToChar();
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

 using namespace HadAsciiAuxiliaryFunctions;
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

 using namespace HadAsciiAuxiliaryFunctions;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (strlength(name) > 127) {
  ErrorMsg(2,"HAsciiKey::WriteKeyword",
   1,"Keyword name to be written is too long (more than 127 characters).");
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
   //fStreamFileWrite << va_arg(ap,Char_t) << GetSeparator();
   fStreamFileWrite << va_arg(ap,int) << GetSeparator();
   // compiler fix (WP 06.2001)
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
   //fStreamFileWrite << va_arg(ap,Char_t) << GetSeparator();
   fStreamFileWrite << va_arg(ap,int) << GetSeparator();
   // compiler fix (WP 06.2001)
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

 using namespace HadAsciiAuxiliaryFunctions;
 WriteKeyword(key);
 WriteLine(text,linelen,strlength(key));
return kTRUE;
} 
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiKey::CopyKeyword(const Char_t* key) 
{
// Copying keyword key to a new file (if there is any in active section).

 using namespace HadAsciiAuxiliaryFunctions;
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

 using namespace HadAsciiAuxiliaryFunctions;
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
//*-- Modified : 2001/08/20 by Witold Przygoda

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
 using namespace HadAsciiAuxiliaryFunctions;
 ErrorMsg(2,"HAsciiRaw::HAsciiRaw",
  1,"Object cannot be copied.");
 throw (Bool_t) kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
HAsciiRaw& HAsciiRaw::operator=(const HAsciiRaw& source) {
 using namespace HadAsciiAuxiliaryFunctions;
 if (this != &source) {
  ErrorMsg(2,"HAsciiRaw::operator=",
   1,"Object cannot be initialised by assignment.");
  throw (Bool_t) kFALSE;
 }
return *this;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::GetKeys() 
{
// Number of keywords (columns of data).

return fpActiveSection->fpRawData->fKeys;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::GetKeyTypeId(Int_t n) 
{
// Type of data for column n. -1 means type is not defined, 
// 0 - Int_t, 1 - Float_t, 2 - Char_t.

 using namespace HadAsciiAuxiliaryFunctions;
 if (n < 0 || n > fKeys) {
  ErrorMsg(2,"HAsciiRaw::GetKeyTypeId",
   1,"Argument (column number) out of range.");
  throw (Bool_t) kFALSE;
 }

return fpKeyTypeId[n];
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::GetKeyName(Int_t n) 
{
// Keyword name for column n.

 using namespace HadAsciiAuxiliaryFunctions;
 if (n < 0 || n > fKeys) {
  ErrorMsg(2,"HAsciiRaw::GetKeyName",
   1,"Argument (column number) out of range.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 Char_t cSecName[128];
 cSecName[0] = '\0';

 strtrunc(cSecName,name);
 if (fpActiveSection && !strcompare(cSecName,
     fpActiveSection->GetSectionName())) {
  ErrorMsg(1,"HAsciiRaw::SetActiveSection",
   3,"Section [",cSecName,"] is already active.");
  return fpActiveSection;
 }

 fKeyLine[0] = '\0'; // bug fix by WP 19 Nov 2000

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

 ErrorMsg(1,"HAsciiRaw::SetActiveSection",
  3,"No section [",cSecName,"] in any opened ASCII file for this runtime database input.");
 fpActiveFile = NULL;
 throw (Bool_t) kFALSE;
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
 
 using namespace HadAsciiAuxiliaryFunctions;
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
    fpActiveFile = NULL;
    fpActiveSection = NULL;
    ErrorMsg(2,"HAsciiRaw::ReadSectionRaw",
     5,"In file \'",fpActiveFile->GetFileName(),"\' unexpected error \
during reading in section [",fpActiveSection->GetSectionName(),"].");
    throw (Bool_t) kFALSE;
   }
   if (strlastchar(cLine)=='\r') cLine[strlength(cLine)-1] = ' ';
   // this is in case of DOS files since '\r' is included in the line!

   if (strfirstchar(cLine) == ';' ||
       strfirstchar(cLine) == '#' ||
       strfirstchar(cLine) == '\\' ||
       strlength(cLine) == 0 ||
       strempty(cLine)) {

    cLine[0] = '\0';
    if (bKeys == kTRUE && strfirstchar(cLine) == '\\')
     ErrorMsg(1,"HAsciiRaw::ReadSectionRaw",
      1,"Line with \'\\\' at the beginning detected. Remember that data \
in \'raw\' format cannot be continued in the next line. This line will be ignored.");

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
          ErrorMsg(2,"HAsciiRaw::ReadSectionRaw",
           1,"Keyword names in raw data must be unambiguous.");
          throw (Bool_t) kFALSE;
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
      ErrorMsg(2,"HAsciiRaw::ReadSectionRaw",
       5,"In section [",fpActiveSection->GetSectionName(),"] of raw data in file \'",
fpActiveFile->GetFileName(),"\' there are no keys describing columns of data.");
      throw (Bool_t) kFALSE;
     }

    } else {
             
      bCondition = kTRUE;
      cData[0] = '\0';
      strcopy(cData,cLine);
      for (i = 0; i < fpActiveSection->fpRawData->fKeys; i++) {
       cKey[0] = '\0';
       strunit(cKey,cData);
       if (strempty(cKey)) {
         Char_t buf[11];
         Char_t buf2[11];
         Char_t buf3[11];
         buf[0] = '\0';
         buf2[0] = '\0';
         buf3[0] = '\0';
         sprintf(buf,"%d",i);
         sprintf(buf2,"%d",fpActiveSection->fpRawData->fKeys);
         sprintf(buf3,"%d",lCountLine);
        ErrorMsg(1,"HAsciiRaw::ReadSectionRaw",
         7,"Missing data (there are: ",buf,"; there should be: ",
buf2,") in row ",buf3,". Zeros (0) will be put here \
- need manual correction.");
        bCondition = kFALSE;
       }
      }
      if (strempty(cData) == kFALSE) {
       Char_t buf[11];
       buf[0] = '\0';
       sprintf(buf,"%d",lCountLine);
       ErrorMsg(1,"HAsciiRaw::ReadSectionRaw",
        3,"Too many data units in row ",buf,". Zeros (0) will be put here \
- need manual correction.");
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
    ErrorMsg(2,"HAsciiRaw::ReadSectionRaw",
     5,"In section [",fpActiveSection->GetSectionName(),"] of raw data in file \'",
fpActiveFile->GetFileName(),"\' there are no keys describing columns of data.");
    throw (Bool_t) kFALSE;
   }
   cLine[0] = '\0';

  } else if (bKeys==kFALSE) {

   ErrorMsg(2,"HAsciiRaw::ReadSectionRaw",
    5,"In section [",fpActiveSection->GetSectionName(),"] of raw data in file \'",
fpActiveFile->GetFileName(),"\' there are no keys describing columns of data.");

   throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(2,"HAsciiRaw::ReadInt",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  ConvertToInt();

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(2,"HAsciiRaw::ReadInt",
    3,"Number of recognised integers is different than number of units \
read in column ",buf,".");
   throw (Bool_t) kFALSE;
  }

  // memory allocation for integers
  fpActiveSection->fpRawData->fpDataInt[col] = 
	new Int_t[fpActiveSection->fpRawData->fElements];
  
  for (i = 0; i < fDataCounter; i++)
   fpActiveSection->fpRawData->fpDataInt[col][i] = fpDataInt[i];
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 0;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 0) {
  return (fpActiveSection->fpRawData->fpDataInt[col][row]);
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadInt",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }
 
return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(const Char_t* keyname, Int_t row) 
{
// Reading one Int_t digit from column keyname in position row.

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiRaw::ReadInt",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
 }

return ReadInt(column,row); 
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(Int_t colcond, Int_t rowvalue, Int_t col) 
{
// Reading one Int_t digit from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++) 
  if (ReadInt(colcond,i) == rowvalue) return ReadInt(col,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadInt",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(Int_t colcond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Int_t digit from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadInt(key,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadInt",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(const Char_t* keycond, Int_t rowvalue, Int_t col) 
{
// Reading one Int_t digit from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadInt(col,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadInt",
     5,"No value ",buf," in column \'",keycond,"\'.");
   }

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Int_t HAsciiRaw::ReadInt(const Char_t* keycond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Int_t digit from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadInt(key,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadInt",
     5,"No value ",buf," in column \'",keycond,"\'.");
   }

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(Int_t* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Int_t digits from column col starting from position begin. 
// Digits are put into Int_t array pointed by putin. 

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(2,"HAsciiRaw::ReadInt",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  ConvertToInt(); 

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(2,"HAsciiRaw::ReadInt",
    3,"Number of recognised integers is different than number of units \
read in column ",buf,".");
   throw (Bool_t) kFALSE;
  }

  // memory allocation for integers
  fpActiveSection->fpRawData->fpDataInt[col] =
        new Int_t[fpActiveSection->fpRawData->fElements];

  for (i = 0; i < fDataCounter; i++)
   fpActiveSection->fpRawData->fpDataInt[col][i] = fpDataInt[i];
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 0;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 0) {
  if (begin+number > fpActiveSection->fpRawData->fElements) 
   number = fpActiveSection->fpRawData->fElements - begin;
  for (i = begin; i < begin+number; i++)
   putin[i] = fpActiveSection->fpRawData->fpDataInt[col][i];
  return kTRUE;
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadInt",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadInt(Int_t* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Int_t digits from column keyname starting from position begin. 
// Digits are put into Int_t array pointed by putin. 

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiRaw::ReadInt",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(2,"HAsciiRaw::ReadFloat",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  ConvertToFloat();

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(2,"HAsciiRaw::ReadFloat",
    3,"Number of recognised floats is different than number \
of units read in column ",buf,".");
   throw (Bool_t) kFALSE;
  }

  // memory allocation for floats
  fpActiveSection->fpRawData->fpDataFloat[col] =
        new Float_t[fpActiveSection->fpRawData->fElements];

  for (i = 0; i < fDataCounter; i++)
   fpActiveSection->fpRawData->fpDataFloat[col][i] = fpDataFloat[i];
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 1;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 1) {
  return (fpActiveSection->fpRawData->fpDataFloat[col][row]);
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadFloat",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(const Char_t* keyname, Int_t row) 
{
// Reading one Float_t digit from column keyname in position row.

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiRaw::ReadFloat",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
 }

return ReadFloat(column,row);
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(Int_t colcond, Int_t rowvalue, Int_t col) 
{
// Reading one Float_t digit from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadFloat(col,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadFloat",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(Int_t colcond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Float_t digit from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadFloat(key,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadFloat",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(const Char_t* keycond, Int_t rowvalue, Int_t col) 
{
// Reading one Float_t digit from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadFloat(col,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadFloat",
     5,"No value ",buf," in column \'",keycond,"\'.");
   }

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Float_t HAsciiRaw::ReadFloat(const Char_t* keycond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Float_t digit from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadFloat(key,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadFloat",
     5,"No value ",buf," in column \'",keycond,"\'.");
   }

return 0.;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(Float_t* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Float_t digits from column col starting from position begin.
// Digits are put into Int_t array pointed by putin.

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(2,"HAsciiRaw::ReadFloat",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  ConvertToFloat();

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(2,"HAsciiRaw::ReadFloat",
    3,"Number of recognised floats is different than number \
of units read in column ",buf,".");
   throw (Bool_t) kFALSE;
  }

  // memory allocation for floats
  fpActiveSection->fpRawData->fpDataFloat[col] =
        new Float_t[fpActiveSection->fpRawData->fElements];

  for (i = 0; i < fDataCounter; i++)
   fpActiveSection->fpRawData->fpDataFloat[col][i] = fpDataFloat[i];
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 1;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 1) {
  if (begin+number > fpActiveSection->fpRawData->fElements)
   number = fpActiveSection->fpRawData->fElements - begin;
  for (i = begin; i < begin+number; i++)
   putin[i] = fpActiveSection->fpRawData->fpDataFloat[col][i];
  return kTRUE;
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadFloat",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadFloat(Float_t* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Float_t digits from column keyname starting from position begin.
// Digits are put into Int_t array pointed by putin.

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(3,"HAsciiRaw::ReadFloat",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(2,"HAsciiRaw::ReadChar",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  ConvertToChar();

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(2,"HAsciiRaw::ReadChar",
    3,"Number of recognised characters is different than number \
of units read in column ",buf,".");
   throw (Bool_t) kFALSE;
  }

  // memory allocation for characters
  fpActiveSection->fpRawData->fpDataChar[col] =
        new Char_t[fpActiveSection->fpRawData->fElements];

  for (i = 0; i < fDataCounter; i++)
   fpActiveSection->fpRawData->fpDataChar[col][i] = fpDataChar[i];
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 2;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 2) {
  return (fpActiveSection->fpRawData->fpDataChar[col][row]);
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadChar",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }

return 0;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(const Char_t* keyname, Int_t row) 
{
// Reading one Char_t character from column keyname in position row.

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiRaw::ReadChar",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
 }

return ReadChar(column,row);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(Int_t colcond, Int_t rowvalue, Int_t col) 
{
// Reading one Char_t character from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadChar(col,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadChar",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return '\0';
}
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(Int_t colcond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Char_t character from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadChar(key,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadChar",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

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
//    else {
//     Char_t buf[11];
//     buf[0] = '\0';
//     sprintf(buf,"%d",rowvalue);
//     ErrorMsg(1,"HAsciiRaw::ReadChar",
//      5,"No value ",buf," in column \'",keycond,"\'.");
//    }
// 
// return '\0';
// }
//============================================================================

//----------------------------------------------------------------------------
Char_t HAsciiRaw::ReadChar(const Char_t* keycond, Int_t rowvalue, const Char_t* key) 
{
// Reading one Char_t character digit from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadChar(key,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadChar",
     5,"No value ",rowvalue," in column \'",keycond,"\'.");
   }

return '\0';
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, Int_t col, Int_t begin, Int_t number) 
{
// Reading number of Char_t characters from column col starting from position begin.
// Digits are put into Char_t array pointed by putin.

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i;

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(2,"HAsciiRaw::ReadChar",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] < 0) {
  fDataLineLength = strlength(fpActiveSection->fpRawData->fpDataString[col]);
  fpDataLine = new Char_t[fDataLineLength+1];
  fpDataLine[0] = '\0';
  strcopy(fpDataLine,fpActiveSection->fpRawData->fpDataString[col]);
  strcopy(fKeyLine,fpActiveSection->fpRawData->fpKeyNames[col]);

  ConvertToChar();

  if (fDataCounter != fpActiveSection->fpRawData->fElements) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(2,"HAsciiRaw::ReadChar",
    3,"Number of recognised characters is different than number \
of units read in column ",buf,".");
   throw (Bool_t) kFALSE;
  }

  // memory allocation for characters
  fpActiveSection->fpRawData->fpDataChar[col] =
        new Char_t[fpActiveSection->fpRawData->fElements];

  for (i = 0; i < fDataCounter; i++)
   fpActiveSection->fpRawData->fpDataChar[col][i] = fpDataChar[i];
  fpActiveSection->fpRawData->fpKeyTypeId[col] = 2;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 2) {
  if (begin+number > fpActiveSection->fpRawData->fElements)
   number = fpActiveSection->fpRawData->fElements - begin;
  for (i = begin; i < begin+number; i++)
   putin[i] = fpActiveSection->fpRawData->fpDataChar[col][i];
  return kTRUE;
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadChar",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }

return kFALSE;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, const Char_t* keyname, Int_t begin, Int_t number) 
{
// Reading number of Char_t characters from column keyname starting from position begin.
// Digits are put into Char_t array pointed by putin.

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiRaw::ReadChar",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
 }

return ReadChar(putin,column,begin,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, Int_t col, Int_t number)
{
// Reading number of Char_t characters from column col. If number = 0 all data are read.
// Digits are put into Char_t array pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadChar(putin,col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(Char_t* putin, const Char_t* keyname, Int_t number)
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
Bool_t HAsciiRaw::ReadChar(TArrayC* putin, Int_t col, Int_t number)
{
// Reading number of Char_t characters from column col. If number = 0 all data are read.
// Digits are put into TArrayC pointed by putin.

 if (number == 0) number = fpActiveSection->fpRawData->fElements;

return ReadChar(&putin[0],col,0,number);
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::ReadChar(TArrayC* putin, const Char_t* keyname, Int_t number)
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

 using namespace HadAsciiAuxiliaryFunctions;
 ResetKeyData();
 ResetBankData();
 if (fpRemarkLine) {
  delete [] fpRemarkLine;
  fpRemarkLine = NULL;
 }

 Int_t i,j,k;
 Char_t buf[32];

 if (col > fpActiveSection->fpRawData->fKeys) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fpActiveSection->fpRawData->fKeys);
  ErrorMsg(3,"HAsciiRaw::ReadChar",
   3,"Column number out of range (",buf," column(s) recognised).");
  throw (Bool_t) kFALSE;
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
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",col);
    ErrorMsg(2,"HAsciiRaw::ReadString",
     3,"Missing data in column ",buf,".");
    throw (Bool_t) kFALSE;
   }
  }
  if (strempty(fpDataLine) == kFALSE) {
   Char_t buf[11];
   buf[0] = '\0';
   sprintf(buf,"%d",col);
   ErrorMsg(1,"HAsciiRaw::ReadString",
    3,"Too many data units in column ",buf,".");
  }

  fpActiveSection->fpRawData->fpKeyTypeId[col] = 3;
 }

 if (fpActiveSection->fpRawData->fpKeyTypeId[col] == 3) {
  return (fpActiveSection->fpRawData->fpDataStr[col][row]);
 } else {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",col);
  ErrorMsg(2,"HAsciiRaw::ReadString",
   3,"Column ",buf," was converted earlier to another type.");
  throw (Bool_t) kFALSE;
 }

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(const Char_t* keyname, Int_t row)
{
// Reading string from column keyname in position row.

 using namespace HadAsciiAuxiliaryFunctions;
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
  ErrorMsg(2,"HAsciiRaw::ReadString",
   3,"No keyword \'",cName,"\' describing any column of data.");
  throw (Bool_t) kFALSE;
 }

return ReadString(column,row);
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(Int_t colcond, Int_t rowvalue, Int_t col)
{
// Reading string from column col in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadString(col,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadString",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(Int_t colcond, Int_t rowvalue, const Char_t* key)
{
// Reading string from column key in the position described
// by the condition: in column colcond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(colcond,i) == rowvalue) return ReadString(key,i);
   else {
    Char_t buf[11];
    Char_t buf2[11];
    buf[0] = '\0';
    buf2[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    sprintf(buf2,"%d",colcond);
    ErrorMsg(1,"HAsciiRaw::ReadString",
     5,"No value ",buf," in column \'",buf2,"\'.");
   }

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(const Char_t* keycond, Int_t rowvalue, Int_t col)
{
// Reading string from column col in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadString(col,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadString",
     5,"No value ",buf," in column \'",keycond,"\'.");
   }

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Char_t* HAsciiRaw::ReadString(const Char_t* keycond, Int_t rowvalue, const Char_t* key)
{
// Reading sitrng from column key in the position described
// by the condition: in column keycond find digit of value = rowvalue.

 using namespace HadAsciiAuxiliaryFunctions;
 for (Int_t i = 0; i < fpActiveSection->fpRawData->fElements; i++)
  if (ReadInt(keycond,i) == rowvalue) return ReadString(key,i);
   else {
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",rowvalue);
    ErrorMsg(1,"HAsciiRaw::ReadString",
     5,"No value ",buf," in column \'",keycond,"\'.");
   }

return NULL;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HAsciiRaw::WriteSection(const Char_t* name) 
{
// Writing section name. Brackets [ ] are added automatically.

 using namespace HadAsciiAuxiliaryFunctions;
 if (strlength(name) > 127) {
  ErrorMsg(2,"HAsciiRaw::WriteSection",
   1,"Section name to be written is too long (more than 127 characters).");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (strempty(name)) {
  ErrorMsg(2,"HAsciiRaw::RecogniseKeys",
   1,"No keywords in this variable.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kTRUE) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"Keywords have been already written. You have to define next section name.");
  return kFALSE;
 }
 if (fSection == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"No section name written - keywords must follow a new section.");
  return kFALSE;
 }
 if (strlength(name) > 1023) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"Keywords line to be written is too long (more than 1023 characters).");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kTRUE) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"Keywords have been already written. You have to define next section name.");
  return kFALSE;
 }
 if (fSection == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"No section name written - keywords must follow a new section.");
  return kFALSE;
 }
 if (arg <= 0) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"Wrong function argument - number of keywords must be of positive value.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 WriteSection(sec);

 if (arg <= 0) {
  ErrorMsg(2,"HAsciiRaw::WriteKeywords",
   1,"Wrong function argument - number of keywords must be a positive value.");
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fpActiveSection->fpRawData->fKeys == 0) {
  ErrorMsg(2,"HAsciiRaw::SetReadKeyType",
   1,"Setting keywords types not possible - no keywords read.");
  throw (Bool_t) kFALSE;
 }
 if (col > fpActiveSection->fpRawData->fKeys) {
  ErrorMsg(2,"HAsciiRaw::SetReadKeyType",
   1,"Arguemnt column out of range (larger than number of keywords).");
  throw (Bool_t) kFALSE;
 }
 if (type < 0 || type > 3) {
  ErrorMsg(2,"HAsciiRaw::SetReadKeyType",
   1,"Arguemnt type out of range (0 - 3).");
  throw (Bool_t) kFALSE;
 }

 fpActiveSection->fpRawData->fpKeyTypeId[col] = type;
}
//============================================================================ 

//----------------------------------------------------------------------------
void HAsciiRaw::SetWriteKeyType(Int_t col, Int_t type)
{
// This is auxiliary function used only in HParHadAsciiFileIo to have access
// to private data member of this class.

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeys == 0) {
  ErrorMsg(2,"HAsciiRaw::SetWriteKeyType",
   1,"Setting keywords types not possible - no keywords written.");
  throw (Bool_t) kFALSE;
 }
 if (col > fKeys) {
  ErrorMsg(2,"HAsciiRaw::SetWriteKeyType",
   1,"Arguemnt column out of range (larger than number of keywords).");
  throw (Bool_t) kFALSE;
 }
 if (type < 0 || type > 3) {
  ErrorMsg(2,"HAsciiRaw::SetWriteKeyType",
   1,"Arguemnt type out of range (0 - 3)");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
  if (fKeys == 0) {
   ErrorMsg(2,"HAsciiRaw::SetWriteKeyTypes",
    1,"Setting keywords types not possible - no keywords written.");
   throw (Bool_t) kFALSE;
  }
  Char_t base;
  if (key1 < 97) key1 += 32;
  switch (key1) {
   case 'i': if (fpKeyTypeId[0] > 0) 
              ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
               1,"Change of type for 1. column of data.");
             fpKeyTypeId[0] = 0;
             break;
   case 'f': if (fpKeyTypeId[0] > 0) 
              ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
               1,"Change of type for 1. column of data.");
             fpKeyTypeId[0] = 1;
             break;
   case 'c': if (fpKeyTypeId[0] > 0) 
              ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
               1,"Change of type for 1. column of data.");
             fpKeyTypeId[0] = 2;
             break;
   case 's': if (fpKeyTypeId[0] > 0)
              ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
               1,"Change of type for 1. column of data.");
             fpKeyTypeId[0] = 3;
             break;
    default: Char_t buf[2];
             buf[0] = key1;
             buf[1] = '\0';
             ErrorMsg(2,"HAsciiRaw::SetWriteKeyTypes",
              3,"Type \'",buf,"\' not supported (only: i,f,c,s).");
             throw (Bool_t) kFALSE;
  }

  va_list ap;
  va_start(ap,key1);
  for (Int_t i = 1; i < fKeys; i++) {
   base = (Char_t)va_arg(ap,Int_t);
   if (base < 97) base += 32;
   
   switch (base) {
    case 'i': if (fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpKeyTypeId[i] = 0;
              break;
    case 'f': if (fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpKeyTypeId[i] = 1;
              break;
    case 'c': if (fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpKeyTypeId[i] = 2;
              break;
    case 's': if (fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetWriteKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpKeyTypeId[i] = 3;
              break;
     default: Char_t buf[2];
              buf[0] = base;
              buf[1] = '\0';
              ErrorMsg(2,"HAsciiRaw::SetWriteKeyTypes",
               3,"Type \'",buf,"\' not supported (only: i,f,c,s).");
              throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
  if (fpActiveSection == NULL) {
   ErrorMsg(2,"HAsciiRaw::SetReadKeyTypes",
    1,"There is no active section selected.");
   throw (Bool_t) kFALSE;
  }
  if (fpActiveSection->fpRawData->fKeys == 0) {
   ErrorMsg(2,"HAsciiRaw::SetReadKeyTypes",
    1,"Setting keywords types not possible - no keywords read.");
   throw (Bool_t) kFALSE;
  }
  Char_t base;
  if (key1 < 97) key1 += 32;
  switch (key1) {
   case 'i': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
               1,"Change of type for 1. column of data.");
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadInt(0,0);
             break;
   case 'f': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
               1,"Change of type for 1. column of data.");
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadFloat(0,0);
             break;
   case 'c': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
               1,"Change of type for 1. column of data.");
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadChar(0,0);
             break;
   case 's': if (fpActiveSection->fpRawData->fpKeyTypeId[0] > 0)
              ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
               1,"Change of type for 1. column of data.");
             fpActiveSection->fpRawData->fpKeyTypeId[0] = -1;
             ReadString(0,0);
             break;
    default: Char_t buf[2];
             buf[0] = key1;
             buf[1] = '\0';
             ErrorMsg(2,"HAsciiRaw::SetReadKeyTypes",
              3,"Type \'",buf,"\' not supported (only: i,f,c,s).");
             throw (Bool_t) kFALSE;
  }

  va_list ap;
  va_start(ap,key1);
  for (Int_t i = 1; i < fpActiveSection->fpRawData->fKeys; i++) {
   base = (Char_t)va_arg(ap,Int_t);
   if (base < 97) base += 32;
  
   switch (base) {
    case 'i': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadInt(i,0);
              break;
    case 'f': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadFloat(i,0);
              break;
    case 'c': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadChar(i,0);
              break;
    case 's': if (fpActiveSection->fpRawData->fpKeyTypeId[i] > 0) {
               Char_t buf[11];
               buf[0] = '\0';
               sprintf(buf,"%d",i+1);
               ErrorMsg(1,"HAsciiRaw::SetReadKeyTypes",
                3,"Change of type for ",buf,". column of data.");
              }
              fpActiveSection->fpRawData->fpKeyTypeId[i] = -1;
              ReadString(i,0);
              break;
     default: Char_t buf[11];
              buf[0] = base;
              buf[1] = '\0';
              ErrorMsg(2,"HAsciiRaw::SetReadKeyTypes",
               3,"Type \'",base,"\' not supported (only: i,f,c,s).");
              throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 Int_t i;
 if (fpActiveSection == NULL) {
  ErrorMsg(2,"HAsciiRaw::ReadRawLine",
   1,"There is no active section selected.");
  throw (Bool_t) kFALSE;
 }
 if (fpActiveSection->fpRawData->fKeys == 0) {
  ErrorMsg(2,"HAsciiRaw::ReadRawLine",
   1,"Reading data not possible - no keywords read.");
  throw (Bool_t) kFALSE;
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
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",i+1);
    ErrorMsg(2,"HAsciiRaw::ReadRawLine",
     3,"Data type for ",buf,". column not specified.");
   throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }
 if (fpKeyTypeId[0] > 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Specified data type of 1. column is different than of variable to be written.");
  throw (Bool_t) kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Data type for 1. column not specified.");
  throw (Bool_t) kFALSE;
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
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",i+1);
    ErrorMsg(2,"HAsciiRaw::WriteRawLine",
     3,"Data type for ",buf,". column not specified.");
    throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }
 if (fpKeyTypeId[0] != 1 && fpKeyTypeId[0] >= 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Specified data type of 1. column is different than of variable to be written.");
  throw (Bool_t) kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Data type for 1. column not specified.");
  throw (Bool_t) kFALSE;
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
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",i+1);
    ErrorMsg(2,"HAsciiRaw::WriteRawLine",
     3,"Data type for ",buf,". column not specified.");
    throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }
 if (fpKeyTypeId[0] != 2 && fpKeyTypeId[0] >= 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Specified data type of 1. column is different than of variable to be written.");
  throw (Bool_t) kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Data type for 1. column not specified.");
  throw (Bool_t) kFALSE;
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
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",i+1);
    ErrorMsg(2,"HAsciiRaw::WriteRawLine",
     3,"Data type for ",buf,". column not specified.");
    throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }
 if (fpKeyTypeId[0] != 3 && fpKeyTypeId[0] >= 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Specified data type of 1. column is different than of variable to be written.");
  throw (Bool_t) kFALSE;
 } else if (fpKeyTypeId[0] < 0) {
  ErrorMsg(2,"HAsciiRaw::WriteRawLine",
   1,"Data type for 1. column not specified.");
  throw (Bool_t) kFALSE;
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
    Char_t buf[11];
    buf[0] = '\0';
    sprintf(buf,"%d",i+1);
    ErrorMsg(2,"HAsciiRaw::WriteRawLine",
     3,"Data type for ",buf,". column not specified.");
    throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteInt",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 0;
 if (fpKeyTypeId[fCounter] != 0) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fCounter+1);
  ErrorMsg(2,"HAsciiRaw::WriteInt",
   3,"Specified data type of ",buf,". column is different \
than of variable to be written.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteFloat",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 1;
 if (fpKeyTypeId[fCounter] != 1) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fCounter+1);
  ErrorMsg(2,"HAsciiRaw::WriteFloat",
   3,"Specified data type of ",buf,". column is different \
than of variable to be written.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteChar",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 2;
 if (fpKeyTypeId[fCounter] != 2) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fCounter+1);
  ErrorMsg(2,"HAsciiRaw::WriteChar",
   3,"Specified data type of ",buf,". column is differenr \
than of variable to be written.");
  throw (Bool_t) kFALSE;
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

 using namespace HadAsciiAuxiliaryFunctions;
 if (fKeywords == kFALSE && fData == kFALSE) {
  ErrorMsg(2,"HAsciiRaw::WriteChar",
   1,"Writing data not possible - keywords must be written first.");
  throw (Bool_t) kFALSE;
 }

 if (fCounter == fKeys) fCounter = 0;
 if (fpKeyTypeId[fCounter] < 0) fpKeyTypeId[fCounter] = 3;
 if (fpKeyTypeId[fCounter] != 3) {
  Char_t buf[11];
  buf[0] = '\0';
  sprintf(buf,"%d",fCounter+1);
  ErrorMsg(2,"HAsciiRaw::WriteString",
   3,"Specified data type of ",buf,". column is different \
than of variable to be written.");
  throw (Bool_t) kFALSE;
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



