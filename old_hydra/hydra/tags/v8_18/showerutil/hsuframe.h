#ifndef HSUFrame_H
#define HSUFrame_H
#pragma interface

#include <TGFrame.h>
#include <TGLabel.h>
#include <TGButton.h>
#include <TGSplitter.h>
#include <TG3DLine.h>

class TGMainFrame;

class HSUFrame : public TGMainFrame {
public:
    HSUFrame(const TGWindow *p,UInt_t w, UInt_t h, UInt_t r=0, UInt_t c=2);
    ~HSUFrame();

    Bool_t AddComponent();

    ClassDef(HSUFrame,0) //ROOT extension
private:
    TGVerticalFrame      *m_pVertFrameR;
    TGVerticalFrame      *m_pVertFrameL;
    TGLayoutHints        *m_pLHints;
    TGMatrixLayout       *m_pMLayout;
    TGHorizontal3DLine   *m_pHLine;
    TGHorizontalFrame    *m_pBottomFrame;
    TGTextButton         *m_pOKButton;
    TGTextButton         *m_pCancelButton;

};
#endif
