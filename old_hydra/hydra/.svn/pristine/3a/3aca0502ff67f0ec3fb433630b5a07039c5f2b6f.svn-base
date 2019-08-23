#ifndef HSHOWERDETECTOR_H
#define HSHOWERDETECTOR_H

#pragma interface

#include "hdetector.h" 
#include "haddef.h"
//#include "hparset.h"
#include <TArrayI.h>

class HParIo;
class HTask;
class HTaskSet;
class HParSet;

class HShowerDetector : public HDetector {
private:
  Int_t maxRows;
  Int_t maxColumns;

  HParSet *m_pGeometry; //!

  void getMaxValues(Int_t *, Int_t *);

protected:
  HCategory *buildLinearCat(Text_t *classname);
  HCategory *buildMatrixCat(Text_t *classname,Float_t fillRate, Int_t nLevels);
  HCategory *buildMatrixCat(Text_t *classname,Float_t fillRate, Int_t nLevels, Int_t* sizes);
public:
  HShowerDetector(void);
  ~HShowerDetector(void);

  void activateParIo(HParIo* io);
  Bool_t write(HParIo* io);

  HTask *buildTask(Text_t *name,Option_t *opt="");
  HCategory *buildCategory(Cat_t cat);

  Bool_t init(Text_t* level="raw");
  Int_t getShowerSectors();
  Int_t getShowerModules();
  Int_t getRows(){return maxRows;}
  Int_t getColumns(){return maxColumns;}

  HParSet* getGeometry(){return m_pGeometry;}
  void setGeometry(HParSet* pGeometry){m_pGeometry= pGeometry;}


  ClassDef(HShowerDetector,1)  // SHOWER detector class
};


#endif /* !HSHOWERDETECTOR_H */
