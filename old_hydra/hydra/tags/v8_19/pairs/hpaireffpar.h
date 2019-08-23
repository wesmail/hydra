// $Id: hpaireffpar.h,v 1.2 2008-05-09 16:14:32 halo Exp $
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
    HPairEffPar(const Char_t* name="PairEffParameters",
		const Char_t* title="Pair Efficiency Parameters",
		const Char_t* context="PairDefault");
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


