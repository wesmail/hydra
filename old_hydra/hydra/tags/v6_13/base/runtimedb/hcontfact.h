#ifndef HCONTFACT_H
#define HCONTFACT_H

#include "TList.h"
#include "TNamed.h"

class HParSet;

class HContainer : public TNamed {
private:
  HContainer();
protected:
  TList* contexts;       // available contexts for this parameter container
  TString actualContext; // actual context set by the user
public:
  HContainer( const char*, const char*, const char*);
  ~HContainer();
  void addContext(const char*);
  Bool_t setActualContext(const char* c);
  const char* getDefaultContext();
  const char* getActualContext() { return actualContext.Data(); }
  void print();
  TString getConcatName();
  const char* getContext();
  ClassDef(HContainer,0) // class for list elements in class HContFact
};

class HContFact : public TNamed {
protected:
  TList* containers;   // all parameter containers managed by this factory
public:
  HContFact();
  virtual ~HContFact();
  Bool_t addContext(const char* name);
  void print();
  HParSet* getContainer(const char*);
  virtual HParSet* createContainer(HContainer*) {return 0;}
protected:
  const char* getActualContext(const char* name) {
    return ((HContainer*)containers->FindObject(name))->getActualContext();
  }
  ClassDef(HContFact,0) // base class of all factories for parameter containers
};

#endif  /* !HCONTFACT_H */
