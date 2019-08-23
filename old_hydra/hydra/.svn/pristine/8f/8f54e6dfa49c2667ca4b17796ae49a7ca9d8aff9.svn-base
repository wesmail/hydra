#ifndef HTOFINODIGITPAR_H
#define HTOFINODIGITPAR_H
#include "TArrayF.h"
#include "TArrayI.h"
#include "hparcond.h"
#include "hlocation.h"
#include "htofinocalpar.h"
#include "htofinocalparcell.h"



class HParamList;

class HTofinoDigitPar : public HParCond {
protected:
    TArrayF m_fOffset       ;  // offset parameter for calibration of the cell
    TArrayF m_fSlope        ;  // slope parameter for  calibration of the cell
    TArrayF m_fDeltaSlope   ;  // error in slope parameter
    TArrayF m_fDeltaOffset  ;  // error in offset parameter
    TArrayF m_fCFDThreshold ;  // CFD Threshold (chan.) for the cell
    TArrayF m_fADCThreshold ;  // ADC Threshold (chan.) for the cell
    TArrayF m_fADCPedestal  ;  // ADC Pedestal (chan.) for the cell
    TArrayF m_fElossParam0  ;  // K and A0-A2 parameters for (Eloss = K*(ADC-ADCPedestal) + A0+A1*Row+A2*Row*Row) calculation
    TArrayF m_fElossParam1  ;  // K and A0-A2 parameters for (Eloss = K*(ADC-ADCPedestal) + A0+A1*Row+A2*Row*Row) calculation
    TArrayF m_fElossParam2  ;  // K and A0-A2 parameters for (Eloss = K*(ADC-ADCPedestal) + A0+A1*Row+A2*Row*Row) calculation
    TArrayF m_fElossParam3  ;  // K and A0-A2 parameters for (Eloss = K*(ADC-ADCPedestal) + A0+A1*Row+A2*Row*Row) calculation
    TArrayF m_fResolParam0  ;  // parameters for tofino resolution calculation
    TArrayF m_fResolParam1  ;  // parameters for tofino resolution calculation
    TArrayF m_fResolParam2  ;  // parameters for tofino resolution calculation
    TArrayF m_fDistParam0   ;  // parameters for distance calculation
    TArrayF m_fDistParam1   ;  // parameters for distance calculation
    TArrayF m_fDistParam2   ;  // parameters for distance calculation
    TArrayF m_fDistParam3   ;  // parameters for distance calculation
    TArrayF m_fMinLength    ;  // min distance from target to cell
    TArrayI m_nDistParamType;  // type of parameters for distance calculation
    TArrayF m_fTimeChannel  ;  // time in ns per tdc channel
    TArrayF m_fCoordPos     ;  // y position of the coordinate system in the cell

    //----------------------------------------------------------------------------
    // copied functions from HTofinoCalParCell
    Float_t calcDistance(Int_t nSector, Int_t nCell,Float_t row)
    {
        Int_t ind=nSector*4+nCell;
	if(m_nDistParamType[ind]== 2) {
	    if(row < 7) return (m_fCoordPos[ind] - (m_fDistParam0[ind] + m_fDistParam1[ind]*row));
	    else return(m_fCoordPos[ind] - (m_fDistParam2[ind] + m_fDistParam3[ind]*row));
	}
	else {
	    return m_fDistParam0[ind] + row*(m_fDistParam1[ind]+row*m_fDistParam2[ind]);
	}
    }
    Float_t calcDistPMT(Int_t nSector, Int_t nCell,Float_t y)
    {
	return (m_fCoordPos[nSector*4+nCell] - y);
    }
    Float_t calcTimeResol(Int_t nSector, Int_t nCell,Float_t y)
    {
	Int_t ind=nSector*4+nCell;
	return m_fResolParam0[ind] + y*(m_fResolParam1[ind]+y*m_fResolParam2[ind]);
    }
    //----------------------------------------------------------------------------

public:
    HTofinoDigitPar(const char* name ="TofinoDigitPar",
	      const char* title  ="digitisation parameters for Tofino",
	      const char* context="TofinoDigitParProduction");
    ~HTofinoDigitPar();

    Float_t getOffset        (HLocation &loc) { return m_fOffset       [loc[0]*4+loc[1]];}
    Float_t getSlope         (HLocation &loc) { return m_fSlope        [loc[0]*4+loc[1]];}
    Float_t getDeltaSlope    (HLocation &loc) { return m_fDeltaSlope   [loc[0]*4+loc[1]];}
    Float_t getDeltaOffset   (HLocation &loc) { return m_fDeltaOffset  [loc[0]*4+loc[1]];}
    Float_t getCFDThreshold  (HLocation &loc) { return m_fCFDThreshold [loc[0]*4+loc[1]];}
    Float_t getADCThreshold  (HLocation &loc) { return m_fADCThreshold [loc[0]*4+loc[1]];}
    Float_t getADCPedestal   (HLocation &loc) { return m_fADCPedestal  [loc[0]*4+loc[1]];}
    Float_t getCoordPosition (HLocation &loc) { return m_fCoordPos     [loc[0]*4+loc[1]];}
    Float_t getMinLength     (HLocation &loc) { return m_fMinLength    [loc[0]*4+loc[1]];}
    Float_t getTimeChannel   (HLocation &loc) { return m_fTimeChannel  [loc[0]*4+loc[1]];}
    Int_t   getNDistParamType(HLocation &loc) { return m_nDistParamType[loc[0]*4+loc[1]];}
    void    getElossParams   (HLocation &loc, Float_t *eloss)
    {
	eloss[0]=m_fElossParam0[loc[0]*4+loc[1]];
        eloss[1]=m_fElossParam1[loc[0]*4+loc[1]];
        eloss[2]=m_fElossParam2[loc[0]*4+loc[1]];
        eloss[3]=m_fElossParam3[loc[0]*4+loc[1]];
    }
    void    getResolParams   (HLocation &loc, Float_t *resol)
    {
	resol[0]=m_fResolParam0[loc[0]*4+loc[1]];
	resol[1]=m_fResolParam1[loc[0]*4+loc[1]];
	resol[2]=m_fResolParam2[loc[0]*4+loc[1]];
    }
    void    getDistParams    (HLocation &loc, Float_t *dist)
    {
	dist[0]=m_fDistParam0[loc[0]*4+loc[1]];
        dist[1]=m_fDistParam1[loc[0]*4+loc[1]];
        dist[2]=m_fDistParam2[loc[0]*4+loc[1]];
        dist[3]=m_fDistParam3[loc[0]*4+loc[1]];
    }

    void setOffset        (HLocation &loc, Float_t fOffset)       { m_fOffset       [loc[0]*4+loc[1]]=fOffset;}
    void setSlope         (HLocation &loc, Float_t fSlope)        { m_fSlope        [loc[0]*4+loc[1]]=fSlope ;}
    void setDeltaSlope    (HLocation &loc, Float_t fDeltaSlope)   { m_fDeltaSlope   [loc[0]*4+loc[1]]=fDeltaSlope;}
    void setDeltaOffset   (HLocation &loc, Float_t fDeltaOffset)  { m_fDeltaOffset  [loc[0]*4+loc[1]]=fDeltaOffset;}
    void setCFDThreshold  (HLocation &loc, Float_t fCFDThreshold) { m_fCFDThreshold [loc[0]*4+loc[1]]=fCFDThreshold;}
    void setADCThreshold  (HLocation &loc, Float_t fADCThreshold) { m_fADCThreshold [loc[0]*4+loc[1]]=fADCThreshold;}
    void setADCPedestal   (HLocation &loc, Float_t fADCPedestal)  { m_fADCPedestal  [loc[0]*4+loc[1]]=fADCPedestal;}
    void setCoordPosition (HLocation &loc, Float_t fCoordPos)     { m_fCoordPos     [loc[0]*4+loc[1]]=fCoordPos;}
    void setNDistParamType(HLocation &loc,Int_t n)                { m_nDistParamType[loc[0]*4+loc[1]]=n;}
    void setMinLength     (HLocation &loc, Float_t fLen)          { m_fMinLength    [loc[0]*4+loc[1]]=fLen;}
    void setTimeChannel   (HLocation &loc, Float_t timechannel)   { m_fTimeChannel  [loc[0]*4+loc[1]]=timechannel;}
    void setElossParams   (HLocation &loc, Float_t *eloss)
    {
	m_fElossParam0[loc[0]*4+loc[1]]=eloss[0];
	m_fElossParam1[loc[0]*4+loc[1]]=eloss[1];
	m_fElossParam2[loc[0]*4+loc[1]]=eloss[2];
	m_fElossParam3[loc[0]*4+loc[1]]=eloss[3];
    }
    void    setResolParams(HLocation &loc, Float_t *resol)
    {
	m_fResolParam0[loc[0]*4+loc[1]]=resol[0];
	m_fResolParam1[loc[0]*4+loc[1]]=resol[1];
	m_fResolParam2[loc[0]*4+loc[1]]=resol[2];
    }
    void    setDistParams (HLocation &loc, Float_t *dist)
    {
	m_fDistParam0[loc[0]*4+loc[1]]=dist[0];
        m_fDistParam1[loc[0]*4+loc[1]]=dist[1];
        m_fDistParam2[loc[0]*4+loc[1]]=dist[2];
        m_fDistParam3[loc[0]*4+loc[1]]=dist[3];
    }
    Float_t calcDriftTime         (Int_t nSector, Int_t nCell, Float_t fY);
    Float_t calcLightPropTime     (Int_t nSector, Int_t nCell, Float_t fY);
    Float_t calcLightPropTimeFull (Int_t nSector, Int_t nCell, Float_t fY);
    Float_t calcDeltaLightPropTime(Int_t nSector, Int_t nCell, Float_t fTime);
    Float_t channelToTime         (Int_t nSector, Int_t nCell, Float_t fTime);
    Float_t timeToChannel         (Int_t nSector, Int_t nCell, Float_t fTime);


    //-----------------------------------------
    void copyCell(HLocation& local,HTofinoCalParCell* cell)
    {

	Float_t eloss[4]={0};
        Float_t resol[3]={0};
        Float_t dist [4]={0};
        setOffset            (local,cell->getOffset()            );
	setSlope             (local,cell->getSlope()             );
	setDeltaSlope        (local,cell->getDeltaSlope()        );
	setDeltaOffset       (local,cell->getDeltaOffset()       );
	setCFDThreshold      (local,cell->getCFDThreshold()      );
	setADCThreshold      (local,cell->getADCThreshold()      );
	setADCPedestal       (local,cell->getADCPedestal()       );
	setTimeChannel       (local,cell->getTimeChannel()       );
	setCoordPosition     (local,cell->getCoordPosition()     );
	setMinLength         (local,cell->getMinLength()         );
	setNDistParamType    (local,cell->getDistanceParamsType());

	cell->getElossParams (eloss);
        setElossParams(local, eloss);
	cell->getResolParams(resol);
	setResolParams(local,resol);
	cell->getDistanceParams(dist);
	setDistParams(local,dist);
    }
    void copyTofinoCalPar(HTofinoCalPar* pCal)
    {
	HLocation local;
	local.set(2,0,0);
	for(Int_t s=0;s<6;s++){
	    for(Int_t c=0;c<4;c++){
		local[0]=s;
		local[1]=c;
		HTofinoCalParCell* cell=(HTofinoCalParCell*)pCal->getObject(local);
		copyCell(local,cell);
	    }
	}
    }
    //-----------------------------------------
    Bool_t  init(HParIo*, Int_t*);
    Int_t   write(HParIo*);
    void    putParams(HParamList*);
    Bool_t  getParams(HParamList*);
    void    clear();
    void    printParam(void);
    ClassDef(HTofinoDigitPar,1) // Container for the Tofino Digitizer parameters
};
#endif  /*!HTofinoDIGITPAR_H*/
