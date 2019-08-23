#ifndef HRICHMODULEGEOM_H
#define HRICHMODULEGEOM_H
#pragma interface

#include <TObject.h>
#include <TObjArray.h>
#include "hrichparset.h"
#include "hrichmodulegeom.h"
#include "hrichpad.h"
#include "hrichwire.h"
#include "hrichframe.h"

class HRichModuleGeom: public HRichParSet
{
  public:
    HRichModuleGeom();
    virtual ~HRichModuleGeom();

    void reset();
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);

    void setModuleID(int id){m_nModuleID = id;}
    int getModuleID(){return m_nModuleID;}

    HRichFrame* getFrame(){return m_pFrame;}
    HRichWireTab* getWires(){return m_pWires;}
    HRichPadTab* getPads(){return m_pPads;}
    void setFrame(HRichFrame* pFrame){ m_pFrame = pFrame;}
    void setWires(HRichWireTab* pWires){ m_pWires = pWires;}
    void setPads(HRichPadTab* pPads){ m_pPads = pPads;}

public:
  float m_fCenterX, m_fCenterY, m_fCenterZ;

private:
  int m_nModuleID;

  HRichFrame* m_pFrame;  //It's referation to frame[m_nModuleID]
  HRichWireTab* m_pWires;  //it's similar
  HRichPadTab* m_pPads;    //it's similar

  ClassDef(HRichModuleGeom, 1)
};

#endif // HRICHMODULEGEOM_H
