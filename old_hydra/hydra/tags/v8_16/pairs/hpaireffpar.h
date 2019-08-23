// $Id: hpaireffpar.h,v 1.1 2005-04-27 18:07:55 eberl Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-04-19 12:40:22
//
// ---------------------------------------------------------------------------
#ifndef HPAIREFFPAR_H
#define HPAIREFFPAR_H
// ---------------------------------------------------------------------------
#include <hparcond.h>
// ---------------------------------------------------------------------------
class HPair;
class HParamList;
// ---------------------------------------------------------------------------
class HPairEffPar: public HParCond {

public:
    HPairEffPar(const char* name="PairEffParameters",
		const char* title="Pair Efficiency Parameters",
		const char* context="PairDefault");
    virtual ~HPairEffPar();
    //  
    Bool_t  init(HParIo*, Int_t*);
    Int_t   write(HParIo*);
    void    putParams(HParamList*);
    Bool_t  getParams(HParamList*);
    void    clear();
    void    printParam(void);
    Bool_t  check(HPair*);

protected:

    ClassDef(HPairEffPar,1)
};


#endif // HPAIREFFPAR_H


