//*-- Author: Joern Wuestenfeld 07/07/2005
//*-- Modified:
//*-- Version: $Id: hmuemueventfilter.h,v 1.1 2005-07-15 09:00:24 wuestenf Exp $

#ifndef HMUEMUEVENTFILTER_H
#define HMUEMUEVENTFILTER_h

#include "hreconstructor.h"

class HEventHeader;

class HMUEmuEventFilter : public HReconstructor
{
  protected:
    HEventHeader *eventHeader;

  public:
    HMUEmuEventFilter(Text_t *name, Text_t *title);
    HMUEmuEventFilter(void);
    virtual ~HMUEmuEventFilter(void);

    virtual Bool_t init(void);
    virtual Bool_t finalize(void);
    virtual Bool_t reinit(void);
    virtual Int_t execute(void);

    ClassDef(HMUEmuEventFilter,0)
};

#endif
