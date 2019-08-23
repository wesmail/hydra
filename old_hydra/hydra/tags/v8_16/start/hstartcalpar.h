#ifndef HSTARTCALPAR_H
#define HSTARTCALPAR_H

#include "TObject.h"
#include "TObjArray.h"
#include "hparset.h"

class HStartCalParChan : public TObject {
protected:
    Float_t slope;  // TDC width
    Float_t offset; // timing offset
	Float_t slopeL; //left side TDC width
	Float_t slopeR; //right side  TDC width
	Float_t slopeM;	//witdh of TDC channel for hardare meantime
	Float_t vGroup; //group velocity 
	Float_t posOffset; //position offset
public:
    HStartCalParChan() { clear(); }
    ~HStartCalParChan() {;}
    Float_t getSlope()  	{ return slope;  	}
    Float_t getSlopeL() 	{ return slopeL; 	}
    Float_t getSlopeR() 	{ return slopeR; 	}
    Float_t getSlopeM() 	{ return slopeM; 	}
	Float_t getVGroup() 	{ return vGroup; 	}
	Float_t getPosOffset()	{ return posOffset; }
    Float_t getOffset() 	{ return offset; 	}

    void fill(Float_t s,Float_t o) {
      slope=s;
      offset=o;
    }
	void fill(Float_t *s) {
		slope=s[0];
		offset=s[1];
		slopeL=s[2];
		slopeR=s[3];
		slopeM=s[4];
		vGroup=s[5];
		posOffset=s[6];
		
		}
	
    void fill(HStartCalParChan& r) {
		slope 		= r.getSlope();
		slopeL		= r.getSlopeL();
		slopeR		= r.getSlopeR();
		slopeM		= r.getSlopeM();
		vGroup		= r.getVGroup();
		posOffset	= r.getPosOffset();
		offset		= r.getOffset();
    }
    void setSlope(Float_t s) { slope=s; }
    void setSlopeL(Float_t s) { slopeL=s; }
    void setSlopeR(Float_t s) { slopeR=s; }
    void setSlopeM(Float_t s) { slopeM=s; }
	void setVGroup(Float_t v) { vGroup=v; }
	void setPosOffset(Float_t o) { posOffset=o; }
    void setOffset(Float_t o) { offset=o; }
    void clear() {
		slope=0.F;
		slopeL=0.F;
		slopeR=0.F;
		slopeM=0.F;
		vGroup=0.F;
		posOffset=0.F;
		offset=0.F;
    }
    ClassDef(HStartCalParChan,2) // Chan level of the START calibration parameters 
};

class HStartCalParMod: public TObject {
protected:
  TObjArray* array;  // pointer array containing HStartCalParChan objects
public:
  HStartCalParMod(Int_t n=8);
  ~HStartCalParMod();
  HStartCalParChan& operator[](Int_t i) {
      return *static_cast<HStartCalParChan*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  ClassDef(HStartCalParMod,1) // Module level of Start Calibration parameters
};

class HStartCalPar : public HParSet {
protected:
  TObjArray* array;     // array of pointers of type HStartCalParMod 
public:
  HStartCalPar(const char* name="StartCalPar",
               const char* title="Calibration parameters for Start",
               const char* context="StartCalProduction");
  ~HStartCalPar();
  HStartCalParMod& operator[](Int_t i) {
    return *static_cast<HStartCalParMod*>((*array)[i]);
  }
  Int_t getSize() { return array->GetEntries(); }
  Bool_t init(HParIo* input,Int_t* set);
  Int_t write(HParIo* output);
  void clear();
  void printParam();
  void readline(const char*, Int_t*);
  void putAsciiHeader(TString&);
  Bool_t writeline(char*, Int_t, Int_t);
  ClassDef(HStartCalPar,1) // Container for the START calibration parameters
};

#endif  /*!HSTARTCALPAR_H*/









