//*-- Author: Alberica Toia
//*-- Modified: Joern Wuestenfeld 05/19/2005

#ifndef HRICHIPUMAP_H
#define HRICHIPUMAP_H

#include "hreconstructor.h" 

class HCategory;
class HIterator;
class HIterator;
class HRichGeometryPar;
class HParSet;
// ***************************************************************************

class HRichIPUMap : public HReconstructor
{
// ***************************************************************************
  protected:
    HParSet *fpGeomPar;    //! Pointer to RICH geometry parameters
    HCategory *pRich;      //! Pointer to RICH hard hit category
    HIterator * iterRich ; //! Iterator over RICH category

  public:
    HRichIPUMap(void);
    HRichIPUMap(Text_t *name, Text_t *title);
    ~HRichIPUMap(void);

    Bool_t init(void);
    Bool_t finalize(void);
    Int_t execute(void);

    HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); }
    HParSet* getGeomPar() { return fpGeomPar; }
    void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }

    ClassDef(HRichIPUMap,1) // RICH IPU map
  };

//============================================================================

#endif  // HRICHIPUMAP_H




