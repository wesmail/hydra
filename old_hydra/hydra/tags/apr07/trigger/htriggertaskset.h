//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 03/31/2005
//*-- Version: $Id: htriggertaskset.h,v 1.4 2005-07-15 09:00:25 wuestenf Exp $

#ifndef HTRIGGERTASKSET_H
#define HTRIGGERTASKSET_H

#include "htaskset.h"

class HTriggerTaskSet : public HTaskSet
{
protected:
  Bool_t isReal;        //!
  Bool_t isExp;         //!
  Bool_t isSimulation;  //!
  Bool_t doFilter;      //!

private :
  void parseArguments(TString s1);
  
public:
  HTriggerTaskSet(void);
  HTriggerTaskSet(Text_t name[],Text_t title[]);
  ~HTriggerTaskSet(void);

  HTask *make(const char *select="", Option_t *option="");

  ClassDef(HTriggerTaskSet,1) // Set of trigger tasks
};

#endif /* !HTRIGGERTASKSET_H */
