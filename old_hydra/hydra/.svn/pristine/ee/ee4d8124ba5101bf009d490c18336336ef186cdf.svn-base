// $Id: hpairfl.h,v 1.7 2005-06-23 17:10:16 jacek Exp $
// Author: Thomas.Eberl@ph.tum.de, last modified : 2005-05-23 00:33:23
//
// ---------------------------------------------------------------------------
#ifndef HPAIRFL_H
#define HPAIRFL_H
// ---------------------------------------------------------------------------
#include <TObject.h>
// ---------------------------------------------------------------------------
#include <haddef.h>
// ---------------------------------------------------------------------------
class TFile;
class TH1;
class TH1F;
class TLorentzVector;
class TObjArray;
// ---------------------------------------------------------------------------
class HCategory;
class HIterator;
class HLinearCategory;
class HGeantKine;
// ---------------------------------------------------------------------------
class HPidParticle;
// ---------------------------------------------------------------------------

class HPairFL : public TObject {


public:

    HPairFL();
    ~HPairFL();

    static HCategory* buildLinearCategory(Cat_t, Text_t* catName="HNewLinearCategory");
    static HCategory* getCategory(Cat_t,Bool_t bWarning=kFALSE);
    static void saveHistos(TFile*,TObjArray*);
    static void printBitField(const Int_t);
    static void printBitField(const Char_t);
    static void setBit(UChar_t, Int_t&);
    static void clearBit(UChar_t, Int_t&);
    static Int_t getBit(UChar_t, const Int_t);
    static void setBit(UInt_t, Int_t&);
    static void clearBit(UInt_t, Int_t&);
    static Int_t getBit(UInt_t, const Int_t);
    static Int_t countPositiveBits(const Int_t);
    static Float_t calcInvMass(HPidParticle*, HPidParticle*);
    static Float_t calcInvMass(TLorentzVector*, TLorentzVector*);
    static HGeantKine* findParent(HGeantKine*,HCategory*);
    static HGeantKine* findParent(HGeantKine*,HIterator*);
    static void dumpKine(HGeantKine*);
    /*
    static TH1F* createVarBinHisto(TH1*, Float_t*, Int_t nbins, Bool_t kNorm=kTRUE, Bool_t kErr=kTRUE);
    static TH1F* getSameEventCombinatorialBackground(TH1F*, TH1F*, Int_t typ);
    static TH1F* subtractHistogram(TH1F*, TH1F*);
    static void printHisto(TH1F*);
    */
// ---------------------------------------------------------------------------

    ClassDef(HPairFL,0)
};
#endif // HPAIRFL_H
