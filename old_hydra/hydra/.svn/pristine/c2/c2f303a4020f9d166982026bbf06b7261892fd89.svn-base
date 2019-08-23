#ifndef HMDCUNPACKER_H
#define HMDCUNPACKER_H

#include "hldunpack.h"
#include "TArrayI.h"
#include "hmdcevreadout.h"
#include "TTree.h"
#include "TNtuple.h"
#include "TString.h"
#include "hmdcdef.h"

// this part should be for the future
// set in base logger class 
#ifdef TRACEON
ofstream TRACEFILE__("mdc_unpacker_error.log");
#define log TRACEFILE__
#endif

class HMdcDataWord : public TObject{

private:
    enum { m1=0x1, m2=0x3, m3=0x7, m4=0xf, m11=0x7ff, m30=0x3fffffff}; 

    //sub-header
    Int_t evNb; 
    Int_t rocNb;
    Int_t trigType;
    Int_t sizeStatus;
    Int_t subevtSize;
 
    //Mdc data-word
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
    Bool_t subHeader(const UInt_t* data);
    void clear();

    // selectors sub-header
    Int_t getEvNb(){ return evNb;} 
    Int_t getTrigType(){ return trigType;}
    Int_t getSizeStatus(){ return sizeStatus;}
    Int_t getSubEvtSize(){ return subevtSize;}
    Int_t getRocNb(){ return rocNb;}
  
    // selectors data-word
    Int_t getSam(){ return sam;}
    Int_t getSamP(){ return samP;}
    Int_t getRoc(){ return roc;}
    Int_t getRocP(){ return rocP;}
    Int_t getBus(){ return mbo;}
    Int_t getTdc(){ return tdc;}
    Int_t getChan(){ return chan;}
    Int_t getHit(){ return hit;}
    Int_t getTime(){ return time;}
    void dump(); 


ClassDef(HMdcDataWord, 1)	// a mdc data word  
};


class HMdcUnpacker: public HldUnpack {
protected:
    static HMdcDataWord *w; //!
    static TTree* packTree;

    HMdcEvReadout *pReadout;//Lookup up table 
    Int_t  subEvtId;
    Int_t tdcMode; // tdcMode defines the mode the tdc has taken data:
    // The TDC can be operated in 2 different modes:
    // mode 0 (default) : trigger on leading and trailing edge
    // mode 1           : trigger on 2 leading edges:

    Bool_t debug;  
    Bool_t extraDebug;

    Bool_t standalone; // If MDCs are not run via the common daq system, but in a 
    // standalone mode,
    // the event trig type is not provided. 
    // In case of calibration triggers which are indicated in addition
    // by the roc header information, the trigger type is set 
    // to calibration event.

    TFile *pFile;
    TFile *ntupleOutputFile;
    TNtuple *ntuple;
    TFile *ntupleOutputFile2;
    TNtuple *ntuple2;

    TString pFileName;  
    TString ntupleOutputFileName;
    TString ntupleOutputFile2Name;

public:
    // Range for MDC subevents is 200 - 299

    HMdcUnpacker(Int_t id = 1,Bool_t dbg=kFALSE, Bool_t exdbg=kFALSE ) 
      { 
	subEvtId = id; 
	w = new HMdcDataWord(); 
	pReadout = NULL;
	debug=dbg;
	pFile=NULL;
	extraDebug=exdbg;
	standalone=kFALSE;
	tdcMode=0;
      }

    HMdcUnpacker(const HMdcUnpacker& mdc) 
      { 
	subEvtId = mdc.subEvtId; 
	w=mdc.w;
      }

    ~HMdcUnpacker()
      {  
	if(w) delete w; w=0;
	if(debug)
	  {
	    if(packTree) 
	      {
		delete packTree; 
		packTree=0;  
	      }
	  }
      } 

    Int_t getSubEvtId() const { return subEvtId; }
    Bool_t init();
    Int_t execute();
    Bool_t finalize();
   
    Int_t getTime() const { return w->getTime();}
    Int_t getTdc(const Int_t chip, const Int_t chan) const {
	return 8*chip + chan;
    }

    void SetErrorOutputFileName(Char_t*);
    void SetNtupleOutputFileName(Char_t*);
    void SetNtupleOutputFile2Name(Char_t*);

    
    void setStandalone(Bool_t b){standalone = b;}
    Bool_t getStandalone(void) {return standalone;}

    void setTdcMode(Int_t);
    Int_t getTdcMode(void);

protected:
    Int_t fill();
    void print();

    ClassDef(HMdcUnpacker, 1)	// unpack MDC data 
};

#endif /* ! HMDCUNPACKER_H */

