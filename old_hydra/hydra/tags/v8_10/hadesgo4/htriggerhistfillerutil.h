#ifndef HTIGGERHISTFILLERUTIL_H
#define HTIGGERHISTFILLERUTIL_H

#include "TObject.h"

class TofTypePrivat : public TObject
{
 protected:
	Float_t ph;       //!<
	Float_t th;       //!<
	Float_t time;     //!<
	Int_t mod;        //!<
	Int_t cell;       //!<
	Int_t det;        //!<

 public:
		TofTypePrivat(void);
		TofTypePrivat(Float_t phi, Float_t theta, Float_t timep, Int_t module, Int_t cellp, Int_t sector);
		~TofTypePrivat(void){};

		void clear(void);

		Float_t getPhi(void){return ph;};
		Float_t getTheta(void){return th;};
		Float_t getTime(void){return time;};
		Int_t getModule(void){return mod;};
		Int_t getCell(void){return cell;};
		Int_t getSector(void){return det;};

		void setPhi(Float_t val){ph = val;};
		void setTheta(Float_t val){th = val;};
		void setTime(Float_t val){time = val;};
		void setModule(Int_t val){mod = val;};
		void setCell(Int_t val){cell = val;};
		void setSector(Int_t val){det = val;};
};

class ThetaCorrPrivat : public TObject
{
 protected:
	Float_t thetadiff2;     //!<
	Int_t anaHit;           //!<
	Int_t ipuHit;           //!<

 public:
	ThetaCorrPrivat(void);
	ThetaCorrPrivat(Float_t theta, Int_t ana, Int_t ipu);
	~ThetaCorrPrivat(void){};

	Float_t getThetaDiff2(void){return thetadiff2;};
	Int_t   getAnaHit(void){return anaHit;};
	Int_t   getIpuHit(void){return ipuHit;};
};


class CorrPrivat : public TObject
{
 protected:
	Int_t   anaSec;
	Float_t anaPhi;
	Float_t anaTheta;
	Float_t ipuPhi;
	Float_t ipuTheta;
	Float_t anaTime;
	Float_t ipuTime;
	Int_t   anaDet;

 public:
	CorrPrivat(void);
	CorrPrivat(Int_t aSec, Float_t aPhi, Float_t aTheta, Float_t iPhi, Float_t iTheta, Float_t aTime, Float_t iTime, Int_t aDet);
	~CorrPrivat(void){};

	Float_t getPhiAna(void){return anaPhi;};
	Float_t getPhiIPU(void){return ipuPhi;};
	Float_t getThetaAna(void){return anaTheta;};
	Float_t getThetaIPU(void){return ipuTheta;};
	Float_t getTimeAna(void){return anaTime;};
	Float_t getTimeIPU(void){return ipuTime;};
	Float_t getDetector(void){return anaDet;};
};

class CorrShowerCord : public TObject
{
  protected:
    Float_t Phi;
	  Float_t Theta;
	  Int_t MF;

  public:
    CorrShowerCord(void);
    CorrShowerCord(Float_t aphi, Float_t atheta, Int_t amf);
    ~CorrShowerCord(void){};

    Float_t getPhi(void){return Phi;};
    Float_t getTheta(void){return Theta;};
    Int_t   getMF(void){return MF;};
    void    setMF(Int_t val){MF = val;};
};

class CorrShowerDiff : public TObject
{
  protected:
    Float_t Diff;
	  Int_t MUSho;
	  Int_t Shower;

  public:
    CorrShowerDiff(void);
    CorrShowerDiff(Float_t adiff, Int_t mushw, Int_t shw);
    ~CorrShowerDiff(void){};

    Float_t getDiff(void){return Diff;};
    Int_t getMU(void){return MUSho;};
    Int_t getShower(void){return Shower;};
};

#endif

