#ifndef HLINEPARSER_H
#define HLINEPARSER_H

#include <TObject.h>
#include <TObjArray.h>
#include <TArrayI.h>

class HLineParser : public TObject {
private:
  TObjArray fStrings;
  TArrayI fVars;
public:
  HLineParser(void);
  ~HLineParser(void);
  Bool_t parse(Text_t *line);
  Bool_t eval(TArrayI *vals,Text_t *string);
  ClassDef(HLineParser,1) //Utility parser
};

#endif /* !HLINEPARSER_H */
