#pragma implementation
#include "hsuframe.h"
ClassImp(HSUFrame)

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// HSUFrame
//
// HSUFrame
//
//////////////////////////////////////////////////////////////////////

HSUFrame::HSUFrame(const TGWindow *p,UInt_t w, UInt_t h, UInt_t r,UInt_t c)
        : TGMainFrame(p,w,h,kMainFrame| kFixedSize|kSunkenFrame| kHorizontalFrame){

    m_pMLayout = new TGMatrixLayout(this,r,c);
    this->SetLayoutManager(m_pMLayout);
    m_pVertFrameL      = new TGVerticalFrame(this,w/2,h, kChildFrame,23);
    m_pVertFrameR      = new TGVerticalFrame(this,w/2,h, kChildFrame,20);
    m_pBottomFrame     = new TGHorizontalFrame(this,w,40, kChildFrame,30);
    m_pOKButton        = new TGTextButton(m_pBottomFrame, "  OK  ", 0);
    m_pCancelButton    = new TGTextButton(m_pBottomFrame, "CANCEL", 1);

    this->AddFrame(m_pVertFrameL,new TGLayoutHints(kLHintsTop));
    this->AddFrame(m_pVertFrameR,new TGLayoutHints(kLHintsTop));

    m_pBottomFrame->AddFrame(m_pOKButton);
    m_pBottomFrame->AddFrame(m_pCancelButton );
    this->AddFrame(m_pBottomFrame,new TGLayoutHints(kLHintsBottom));

    m_pLHints    = new TGLayoutHints(kLHintsLeft | kLHintsExpandY,0,1,5,5);

    this->Layout();
    this->MapSubwindows();
    this->MapWindow();
}

HSUFrame::~HSUFrame(){
    delete m_pMLayout;
    delete m_pVertFrameL;
    delete m_pVertFrameR;
    delete m_pLHints;
    delete m_pOKButton;
    delete m_pCancelButton;
}

Bool_t HSUFrame::AddComponent(){

    return kTRUE;
}
