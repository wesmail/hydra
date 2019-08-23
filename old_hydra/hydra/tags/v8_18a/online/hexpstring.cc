//*-- Author : Dmitry Vasiliev (vassili@lns.infn.it)
/* 
 * Copyright P.Finocchiaro and D.Vasiliev (INFN-LNS, Catania) HADES collaboration
*/
using namespace std;
#include "hexpstring.h"
#include <TGListBox.h>
#include <TFormula.h>
#include <ctype.h>
#include <stdlib.h>

#include <iostream> 
#include <iomanip>

//-------------------------------------------------------------------------
//
// File name:       hexpstring.cc
// Subject:         Expression string for a condition.
// Author:          D.Vasiliev (INFN-LNS, Catania) "vassili@lns.infn.it"
// Date:            March 2000
//
//-------------------------------------------------------------------------
//
// Description:
//
// This file contains the source code for the member functions of 
// the class HExpString.
// This is a helper class which keeps the expression in the form of a string
// and all the related information necessary for evaluation, like for example
// parameters indexes. It is also used by the subpanels for local and
// global conditions, since it contains the helper functions for 
// the interactive definition of an expression by means of GUI.
//
//-------------------------------------------------------------------------

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////////
// This is a helper class which keeps the expression in the form of a string
// and all the related information necessary for evaluation, like for example
// parameters indexes. It is also used by the subpanels for local and
// global conditions, since it contains the helper functions for 
// the interactive definition of an expression by means of GUI.
///////////////////////////////////////////////////////////////////////////
ClassImp(HExpString)

const Char_t* butnames[] = { "0", "1", "2", "3", "4", "5", "6", "7", "8",
			   "9", ".", " ", "(", ")", "/", "*", "+", "-", "ln",
			   "exp", "^", "sqrt", "sin", "cos", "tan", "asin",
			   "acos", "atan", "&&", "||", "!=", "==", ">", 
			   "<", ">=", "<=" };


//---------------------------------------------------------------------
//******  void HExpString::makeExp()
//
// This function updates the string according to the indexes of 
// the string elements which are stored in array "index".
//
//---------------------------------------------------------------------

void HExpString::makeExp() {
//
// This function updates the string according to the indexes of 
// the string elements which are stored in array "index".
//

  Char_t tmp[500] = "";
  Char_t tmp2[5] = "";
  sprintf(exp,"%s",tmp);
  for(Int_t i=0; i<(cursor+1); i++) {
    sprintf(tmp,"%s",exp);
    if(index[i] <= 35) sprintf(exp,"%s%s",tmp,butnames[index[i]]);
    else {
      sprintf(tmp2,"%s%i%s","[",index[i]-36,"]");
      sprintf(exp,"%s%s",tmp,tmp2);
    }
  }
  if(cursor > -1) kClear = kFALSE;
  else kClear = kTRUE;
  kValid = kFALSE;
}


//---------------------------------------------------------------------
//******  void HExpString::addItem(Int_t idx)
//
// This function adds a new index to the array of element indexes and
// calls makeExp() to update the string.
//
//---------------------------------------------------------------------

void HExpString::addItem(Int_t idx) {
//
// This function adds a new index to the array of element indexes and
// class makeExp() to update the string.
//

  if(idx < 0) return;
  index[++cursor] = idx;
  if(idx > 35) paridx[++parcur] = idx - 36;
  makeExp();

}


//---------------------------------------------------------------------
//******  void HExpString::addString(const Char_t* strng)
//
// This function scans the string and divides it into subelements 
// the indexes of which will be stored in the arrays "index" and "paridx".
// This function is mainly used by the GUI.
//
//---------------------------------------------------------------------

void HExpString::addString(const Char_t* strng) {
//
// This function scans the string and divides it into subelements 
// the indexes of which will be stored in the arrays "index" and "paridx".
// This function is mainly used by the GUI.
//

  if(!strng) return;
  Char_t temp[5];
  Int_t i = -1;
  Int_t count = 0;

  while(*strng) {

    if(strchr("0",*strng)) { addItem(0);  ++strng; continue; }
    if(strchr("1",*strng)) { addItem(1);  ++strng; continue; }
    if(strchr("2",*strng)) { addItem(2);  ++strng; continue; }
    if(strchr("3",*strng)) { addItem(3);  ++strng; continue; }
    if(strchr("4",*strng)) { addItem(4);  ++strng; continue; }
    if(strchr("5",*strng)) { addItem(5);  ++strng; continue; }
    if(strchr("6",*strng)) { addItem(6);  ++strng; continue; }
    if(strchr("7",*strng)) { addItem(7);  ++strng; continue; }
    if(strchr("8",*strng)) { addItem(8);  ++strng; continue; }
    if(strchr("9",*strng)) { addItem(9);  ++strng; continue; }
    if(strchr(".",*strng)) { addItem(10); ++strng; continue; }
    if(isspace(*strng))    { addItem(11); ++strng; continue; }
    if(strchr("(",*strng)) { addItem(12); ++strng; continue; }
    if(strchr(")",*strng)) { addItem(13); ++strng; continue; }
    if(strchr("/",*strng)) { addItem(14); ++strng; continue; }
    if(strchr("*",*strng)) { addItem(15); ++strng; continue; }
    if(strchr("+",*strng)) { addItem(16); ++strng; continue; }
    if(strchr("-",*strng)) { addItem(17); ++strng; continue; }
    if(strchr("^",*strng)) { addItem(20); ++strng; continue; }
    
    if(isalpha(*strng)) {
      //      strncpy(temp,strng,3);

      //      printf("%c\n",*strng);

      if(strchr("l",*strng)) { addItem(18); strng+=2; continue; } // ln
      if(strchr("e",*strng)) { addItem(19); strng+=3; continue; } // exp
      if(strchr("c",*strng)) { addItem(23); strng+=3; continue; } // cos
      if(strchr("t",*strng)) { addItem(24); strng+=3; continue; } // tan
      if(strchr("s",*strng)) { // sin or sqrt
	++strng;
	if(strchr("i",*strng)) { addItem(22); strng+=2; continue; } // sin
	if(strchr("q",*strng)) { addItem(21); strng+=3; continue; } // sqrt
      }
      if(strchr("a",*strng)) { // asin, acos or atan
	++strng;
	if(strchr("s",*strng)) { addItem(25); strng+=3; continue; } // asin
	if(strchr("c",*strng)) { addItem(26); strng+=3; continue; } // acos
	if(strchr("t",*strng)) { addItem(27); strng+=3; continue; } // atan
      }
      
      /*      if(!strcmp(temp,"ln"))  { addItem(18); strng+=2; continue; }
      if(!strcmp(temp,"exp")) { addItem(19); strng+=3; continue; }
      if(!strcmp(temp,"sqr")) { addItem(21); strng+=4; continue; }
      if(!strcmp(temp,"sin")) { addItem(22); strng+=3; continue; }
      if(!strcmp(temp,"cos")) { addItem(23); strng+=3; continue; }
      if(!strcmp(temp,"tan")) { addItem(24); strng+=3; continue; }
      if(!strcmp(temp,"asi")) { addItem(25); strng+=4; continue; }
      if(!strcmp(temp,"aco")) { addItem(26); strng+=4; continue; }
      if(!strcmp(temp,"ata")) { addItem(27); strng+=4; continue; }*/
    }
    if(strchr("[",*strng)) {
      count = 0;
      ++strng;
      while(!strchr("]",*strng)) { count++; ++strng; if(count>4) break; }
      strncpy(temp,strng-count,4);
      i = atoi(temp);
      addItem(i+36);
      ++strng;
      continue;
    }
    ++strng;
  }

}


//---------------------------------------------------------------------
//******  void HExpString::undo()
//
// This function deletes the index of the last element from the array "index".
// All spaces are skipped.
//
//---------------------------------------------------------------------

void HExpString::undo() {
//
// This function deletes the index of the last element from the array "index".
// All spaces are skipped.
//

  while(index[cursor] == 11) cursor--; // space
  if(index[cursor--] > 35) parcur--;
  while(index[cursor] == 11) cursor--; // space
  if(cursor < -1) cursor = -1;
  if(parcur < -1) parcur = -1;
  makeExp();
}


//---------------------------------------------------------------------
//******  void HExpString::updateList(TGListBox *p)
//
// This function puts the content of the string to the list box p.
//
//---------------------------------------------------------------------

void HExpString::updateList(TGListBox *p) {
//
// This function puts the content of the string to the list box p.
//

  if(!p) return;
  p->RemoveEntry(1);
  p->AddEntry(exp, 1);
  p->MapSubwindows();
  p->Layout();
}


//---------------------------------------------------------------------
//******  void HExpString::copy(HExpString &hes)
//
// This function makes a copy of this object and stores it in the new
// object "hes".
//
//---------------------------------------------------------------------

void HExpString::copy(HExpString &hes) {
//
// This function makes a copy of this object and stores it in the new
// object "hes".
//

  hes.cursor = cursor;
  hes.parcur = parcur;
  hes.kClear = kClear;
  hes.kValid = kValid; 
  for(Int_t i=0;i<500;i++) hes.index[i] = index[i];
  for(Int_t i=0;i<500;i++) hes.paridx[i] = paridx[i];
  sprintf(hes.exp,"%s",exp);
}


//---------------------------------------------------------------------
//******  void HExpString::check()
//
// This function checks the validity of the mathematical expression.
//
//---------------------------------------------------------------------

void HExpString::check() {
//
// This function checks the validity of the mathematical expression.
//

  kValid = kTRUE;
  TFormula *formula = new TFormula("formula",exp);
  if(!formula) {
    kValid = kFALSE;
    return;
  }
  if(formula->Compile()) {
    kValid = kFALSE;
    return;
  }
  delete formula;

}


//---------------------------------------------------------------------
//******  void HExpString::checkParam()
//
// This function checks the validity of the expression when it serves
// for definition of a new parameter.
//
//---------------------------------------------------------------------

void HExpString::checkParam() {
//
// This function checks the validity of the expression when it serves
// for definition of a new parameter.
//

  for(Int_t i=0; i<cursor; i++) {
    if(index[i] > 27 && index[i] < 36) {
      kValid = kFALSE;
      return;
    }
  }
  return check();

}







