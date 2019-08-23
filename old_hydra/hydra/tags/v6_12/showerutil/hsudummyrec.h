// @(#)$Id: hsudummyrec.h,v 1.1 2002-12-01 02:12:14 jaskula Exp $
//*-- Author : Marcin Jaskula 02/11/2002

#ifndef HSUDummyRec_H
#define HSUDummyRec_H

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HSUDummyRec                                                                //
//                                                                            //
// Dummy reconstructor which calls user defined functions                     //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

#include "hreconstructor.h"
#include "haddef.h"

#include <TList.h>
#include <TFile.h>

// -----------------------------------------------------------------------------

class HCategory;
class HIterator;
class TFile;

// -----------------------------------------------------------------------------

class HSUDummyRec : public HReconstructor
{
public:
                        HSUDummyRec(Bool_t bVerbose = kTRUE);
                        HSUDummyRec(Text_t name[], Text_t title[],
                                    Bool_t bVerbose = kTRUE);
                       ~HSUDummyRec(void);

    // -------------------------------------------------------------------------

    void                setVerbose(Bool_t b = kTRUE)      { bVerbose = b;      }
    Bool_t              getVerbose(void) const            { return bVerbose;   }

    // -------------------------------------------------------------------------

    static HCategory*   getCategory(Cat_t cat, Bool_t bRaport = kTRUE);
    static HCategory*   getCategory(const char* pName, Bool_t bRaport = kTRUE);

    HIterator*          getIterator(Cat_t cat, Bool_t bDelete = kTRUE);
    HIterator*          getIterator(const char* pName, Bool_t bDelete = kTRUE);

    // -------------------------------------------------------------------------

    void                addObjectToWrite(TObject *pObj, Bool_t bDelete = kTRUE);

    Bool_t              removeObject(TObject *pObj);
    Bool_t              removeIterator(HIterator *pI);

    // -------------------------------------------------------------------------

    Int_t               writeObjects(TFile *pFile);
    Int_t               writeObjects(const char *pFileName,
                                Bool_t bRecreate = kTRUE);

    // -------------------------------------------------------------------------

    TFile*              getOutFile(void) const          { return pOutFile;     }
    Bool_t              setOutFile(TFile *pFile, Bool_t bCloseOld = kFALSE);
    TFile*              openOutFile(const char *pName, Bool_t bRecr = kTRUE);
    Bool_t              writeAndCloseOutFile(void);

    // -------------------------------------------------------------------------

    virtual void        Print(Option_t* option) const   { print();             }
    virtual void        print(void) const;

    // -------------------------------------------------------------------------

    virtual Int_t       execute(void)                   { return 0;            }
    virtual Bool_t      init(void)                      { return kTRUE;        }
    virtual Bool_t      finalize(void)                  { return kTRUE;        }
    virtual Bool_t      reinit(void);

    // -------------------------------------------------------------------------

    Bool_t              getInitOk(void)                 { return bInitOk;      }
    void                setInitOk(Bool_t b = kTRUE)     { bInitOk = b;         }

    // -------------------------------------------------------------------------

protected:

    TList               lToDelete;          // list of allocated objects
    TList               lToWrite;           // list of objects for writing

    Bool_t              bVerbose;           // report any warnings

    Bool_t              bInitOk;            // flag if init was ok

    TFile              *pOutFile;           // output file

    // -------------------------------------------------------------------------

    void                setDefault(void);

    // -------------------------------------------------------------------------

    ClassDef(HSUDummyRec, 0)  // Dummy reconstructor
};

// -----------------------------------------------------------------------------

class HSUIteratorObject : public TObject
{
public:

                HSUIteratorObject(void)          : pIter(NULL)  {}
                HSUIteratorObject(HIterator *pI) : pIter(pI)    {}
               ~HSUIteratorObject(void);

    // -------------------------------------------------------------------------

    HIterator*  get(void) const             { return pIter; }
    void        set(HIterator *pI)          { pIter = pI;   }

public:
    HIterator *pIter;

    // -------------------------------------------------------------------------

    ClassDef(HSUIteratorObject, 0)  // Wrapper HIterator -> TObject
};

// -----------------------------------------------------------------------------

#endif //HSUDummyRec_H
