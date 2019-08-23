// @(#)$Id: hpidcontfact.h,v 1.3 2006-08-16 16:13:35 halo Exp $
//*-- Author : Marcin Jaskula 09/10/2002

#ifndef HPIDCONTFACT_H
#define HPIDCONTFACT_H

////////////////////////////////////////////////////////////////////////////////
//
// HPidContFact
//
////////////////////////////////////////////////////////////////////////////////

#include "hcontfact.h"

// -----------------------------------------------------------------------------

class HParSet;
class HContainer;

// -----------------------------------------------------------------------------

class HPidContFact : public HContFact
{
private:
  void setAllContainers(void);
  void setContexts(HContainer *pCont,const char* pDefContName);

 public:

  HPidContFact(void);
  ~HPidContFact() {}
  HParSet* createContainer(HContainer *);
  
    // -------------------------------------------------------------------------

    ClassDef(HPidContFact,0) // Factory for parameter containers in libPid
};

#endif  /* !HPIDCONTFACT_H */
