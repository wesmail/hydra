#ifndef HMDCUNPACKER_H
#define HMDCUNPACKER_H

#include "hldunpack.h"
#include "TArrayI.h"
#include "hmdcevreadout.h"

class HMdcDataWord : public TObject{
private:
    enum { m1=0x1, m2=0x3, m3=0x7, m4=0xf, m11=0x7ff}; 
    Int_t sam;
    Int_t samP;
    Int_t roc;
    Int_t rocP;
    Int_t mbo;
    Int_t tdc;
    Int_t chan;
    Int_t hit;
    Int_t time;


public:
    HMdcDataWord(){clear();}
    ~HMdcDataWord(){;}

    Bool_t decode( const UInt_t data);
    inline void clear() {sam=samP=roc=rocP=mbo=tdc=chan=hit=time=-1;}
    inline Int_t getSam(){ return sam;}
    inline Int_t getSamP(){ return samP;}
    inline Int_t getRoc(){ return roc;}
    inline Int_t getRocP(){ return rocP;}
    inline Int_t getBus(){ return mbo;}
    inline Int_t getTdc(){ return tdc;}
    inline Int_t getChan(){ return chan;}
    inline Int_t getHit(){ return hit;}
    inline Int_t getTime(){ return time;}


ClassDef(HMdcDataWord, 1)	// a mdc data word  
};


class HMdcUnpacker: public HldUnpack {
protected:
    HMdcDataWord *w; //!

    HMdcEvReadout *pReadout;//Lookup up table 
    Int_t  subEvtId;
public:
    // Range for MDC subevents is 200 - 299
    HMdcUnpacker(Int_t id = 1) { subEvtId = id; 
                               w = new HMdcDataWord(); 
                               pReadout = NULL;}
    HMdcUnpacker(const HMdcUnpacker& mdc) { subEvtId = mdc.subEvtId; 
                                            w=mdc.w;}

    Int_t getSubEvtId() const { return subEvtId; }
    Bool_t init();
    Int_t execute();
   
    Int_t getTime() const { return w->getTime();}
    Int_t getMbo(const Int_t nsamN, const Int_t nroc, 
             const Int_t nrocP, const Int_t nmbo) const;
    Int_t getTdc(const Int_t chip, const Int_t chan) const {
	return 8*chip + chan;
    }

protected:
    Int_t fill();
    void print();

    ClassDef(HMdcUnpacker, 1)	// unpack MDC data 
};

#endif /* ! HMDCUNPACKER_H */

