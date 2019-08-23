#ifndef HRICHIPUMAP_H
#define HRICHIPUMAP_H

#include <TObject.h>
#include <TArrayI.h> 
#include "hreconstructor.h" 
#include "hmatrixcatiter.h"
#include "hrichhitIPU.h"
#include "hrichpad.h"
#include "hrichgeometrypar.h"

class HCategory;
class HIterator;
class HSpectrometer;
class HRuntimeDb;
// ***************************************************************************

class HRichIPUMap : public HReconstructor {

// ***************************************************************************
 protected:
    HParSet *fpGeomPar;//!

 public:
    HCategory *pRich; 
    
    HRichIPUMap();
    HRichIPUMap(Text_t *name, Text_t *title);
    ~HRichIPUMap();
    
    HIterator * iterRich ; //!
    
    Bool_t init();
    Bool_t finalize();
    Int_t execute();
 
    HRichGeometryPar* getGeometryPar() { return ((HRichGeometryPar*)fpGeomPar); }
    HParSet* getGeomPar() { return fpGeomPar; }
    void setGeomPar(HParSet* pPar) { fpGeomPar = pPar; }

    ClassDef(HRichIPUMap,1) 
  };
	
//============================================================================
 
#endif  // HRICHIPUMAP_H
             




