//*-- Author : Jochen Markert 18.07.2007

#ifndef  HPIDTRACKCLEANER_H
#define  HPIDTRACKCLEANER_H

#include "hreconstructor.h"
#include "hcategory.h"
#include "hpidtracksorter.h"

#include "TString.h"

class HPidTrackCleaner : public HReconstructor {

protected:


    HPidTrackSorter sorter;             //! for doing the final task of flagging
    HCategory* pPidTrackCandCat;        //! HPidTrackCand category

    void   clear(void);
public:
    HPidTrackCleaner(void);
    HPidTrackCleaner(Text_t *name,Text_t *title);
    ~HPidTrackCleaner(void);
    Bool_t init      (void);
    Int_t  execute   (void);
    Bool_t finalize  (void);
    static void   setDebug       (void)        { HPidTrackSorter::setDebug();}
    static void   setPrintLevel  (Int_t level) { HPidTrackSorter::setPrintLevel(level);}
    void          setOutputFile(TString filename = "");
    ClassDef(HPidTrackCleaner,0);
};
#endif /* !HPIDTRACKCLEANER_H */







