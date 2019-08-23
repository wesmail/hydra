#include "hlineparser.h"
#include <string.h>
#include "TObjString.h"

//*-- Author : Manuel Sanchez
//*-- Modified : 9/11/1998 by Manuel Sanchez

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////
//HLineParser
//
// This is a utility class. It allows to have a single line with variables 
//inside so you can give values to those variables and get a string 
//corresponding to the original text but with numbers instead of variables.
//
// The variables are declared as numbers (variable number) between % symbols.
//For example: "S%0%.M%1%" is interpreted as two varibles: "%0%" and "%1%".
//////////////////////////////////////////////////////

HLineParser::HLineParser(void) {
  //Default constructor
}

HLineParser::~HLineParser(void) {
  //Destructor
  fStrings.Delete();
}

Bool_t HLineParser::parse(Text_t *line) {
  //Parses the text given in "line". For example if line="S%0%.M%1%" this
  //function will identify the literals "S", ".M" and the variables "%0%" and
  //"%1".
  Text_t string[255];
  Text_t *token;
  Int_t nVars=0,nStrings=0;
  strcpy(string,line);

  fStrings.Delete();
  fVars.Reset();

  token=strtok(string,"%");
  while (token!=NULL) {
    nStrings++;
    token=strtok(NULL,"%");
    nVars++;
    if (token) token=strtok(NULL,"%");
  }
  fVars.Set(nVars);
  fStrings.Expand(nStrings);

  strcpy(string,line);
  nStrings=0; nVars=0;
  token=strtok(string,"%");
  while (token!=NULL) {
    fStrings.Add(new TObjString(token));
    token=strtok(NULL,"%");
    sscanf(token,"%i",&fVars[nVars]);
    nVars++;
    if (token) token=strtok(NULL,"%");
  }
  return kTRUE;
}

Bool_t HLineParser::eval(TArrayI *vals,Text_t *string) {
  //Puts in "string" a text resulting of substituting all the variables in
  //the parsed text (see parse()) by its values given in "vals". Following
  //with the example in parse(), if now we set vals={1,3} the eval function
  //will set: string="S1.M3"
  TIter nextString(&fStrings);
  Int_t varN=0;
  TObjString *st;
  Text_t buffer[255];

  //if (vals->fN<maxvar) return kFALSE;

  strcpy(string,"");
  while ( (st=(TObjString *)nextString())!=NULL) {
    strcat(string,st->GetName());
    if (varN<fVars.fN) {
      sprintf(buffer,"%i",vals->At(fVars[varN]));
      varN++;
      strcat(string,buffer);
    }
  }
  return kTRUE;
}


ClassImp(HLineParser)
