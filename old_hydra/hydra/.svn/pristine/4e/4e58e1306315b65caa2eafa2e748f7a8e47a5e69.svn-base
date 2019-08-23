#ifndef __HShowerModuleGeom_H
#define __HShowerModuleGeom_H
#pragma interface

#include "TObject.h"
#include "TObjArray.h"
#include "hshowerparset.h"
#include "hshowerpad.h"
#include "hshowerwire.h"
#include "hshowerframe.h"
#include "hgeomtransform.h"

class HShowerModuleGeom: public HShowerParSet
{
  public:
    HShowerModuleGeom();
    virtual ~HShowerModuleGeom();

    void reset();
    virtual Bool_t initAscii(HParHadAsciiFileIo* pHadAsciiFile);
    virtual Bool_t writeAscii(HParHadAsciiFileIo* pHadAsciiFile);

    void setModuleID(Int_t id){m_nModuleID = id;}
    Int_t getModuleID(){return m_nModuleID;}

    HShowerFrame* getFrame(){return m_pFrame;}
    HShowerWireTab* getWires(){return m_pWires;}
    HShowerPadTab* getPads(){return m_pPads;}
    void setFrame(HShowerFrame* pFrame){ m_pFrame = pFrame;}
    void setWires(HShowerWireTab* pWires){ m_pWires = pWires;}
    void setPads(HShowerPadTab* pPads){ m_pPads = pPads;}

    const HGeomTransform& getTransform(){return transform;}
    void setTransform(const HGeomTransform& t){transform = t;}

    void transformModule(HGeomTransform& t);
    void transformVector(HGeomTransform& t);
public:
  HGeomTransform transform;

private:
  Int_t m_nModuleID;

  HShowerFrame* m_pFrame;  //!It's referation to frame[m_nModuleID]
  HShowerWireTab* m_pWires;  //!it's similar
  HShowerPadTab* m_pPads;    //!it's similar

  ClassDef(HShowerModuleGeom, 1)
};

#endif
