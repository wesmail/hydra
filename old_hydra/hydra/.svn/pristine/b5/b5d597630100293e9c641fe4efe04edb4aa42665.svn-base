// @(#)$Id: hpidhitdata.h,v 1.20 2008-09-18 13:06:21 halo Exp $
//*-- Author : Marcin Jaskula 02/03/2003
//  Modified : Marcin Jaskula 14/03/2003
//              tof and mdc variables added
//  Modified : Marcin Jaskula 16/04/2003
//              flags added
//  Refactored : Tassilo Christ 16/10/2003
//           Compatibility with new tracking and hit matching scheme
//           requires redesign. Intended to hold all information from
//           hit level required to do PID. Should most of the users 
//           allow to throw away hit containers.
//  Modified : Stefano Spataro 26/08/2005
//              TOF rod/TOFINO cell included (new classdef)
//  Modified : Tassilo Christ 05/02/2005
//             Upgraded documentation

#ifndef HPidHitData_h
#define HPidHitData_h

////////////////////////////////////////////////////////////////////////////////
//                                                                            //
// HPidHitData                                                                //
//                                                                            //
// Class for storing all interesting data from hit objects matched in         //
// HMetaMatch. Base class of HPidTrackCandidate.                              //
// This class should fulfill 100% of all analysis needs with the datamembers  //
// which are copied from the hit-container level. One can thus throw away     //
// the hit-containers and reduce the DST-size significantly.                  //
//                                                                            //
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------

#include "TObject.h"
#include "TError.h"
#include "piddef.h"
// -----------------------------------------------------------------------------

class HMetaMatch;

// -----------------------------------------------------------------------------

class HPidHitData: public TObject
{
 public:
  
  HPidHitData(void)         { reset();  }
  HPidHitData(const HPidHitData& source);
  
  virtual        ~HPidHitData(void)         { }

  //set all data members to default values
  void            reset(void);     

  //all detectors are set to "OFF" - i.e. there was NO hit in any subsystem
  void            clearFlags(void)                { ResetBit(kALLDETS); } 

  //get information whether a specific detector was active
  UInt_t          getFlags(void) const { return ((TestBits(kALLDETS) & kALLDETS)); }
  
  //Overloads base class function
  virtual void    Clear(Option_t*)          { reset();  }
  
  //Collect information from hits indexed in HMetaMatch into this object
  void            fillFromMetaMatch(HMetaMatch *pMatch, Int_t iMatchIndex);

  //Print data members 
  virtual void    Print(Option_t*) const    { print();  }
  void            print(void) const;
  
  // -------------------------------------------------------------------------

  //we use the bits 14 to 19 of the bit-field in TObject to store our detector information 
  //(The other ones are reserved by the base class! cf. TObject documentation)
  enum
    {
      kRICH        = 0x01<<14,  //bit 14=kTRUE --> RICH had a ring
      kINNERMDC    = 0x02<<14,  //bit 15=kTRUE --> Inner MDCs had a segment
      kOUTERMDC    = 0x04<<14,  //bit 16=kTRUE --> Outer MDCs had a segment
      kTOF         = 0x08<<14,  //bit 17=kTRUE --> TOF had a hit
      kSHOWER      = 0x10<<14,  //bit 18=kTRUE --> Shower had a hit
      kRICHIPU     = 0x20<<14,  //bit 19=kTRUE --> RICH IPU had a ring
      kALLDETS     = kRICH | kINNERMDC | kOUTERMDC | kTOF | kSHOWER | kRICHIPU //All detectors had a hit
    };

  // -------------------------------------------------------------------------  


  //Getters and setters for the above documented flags
  void            setFlagRICH(Bool_t b = kTRUE)   { 
    if(b) SetBit(kRICH);  
    else ResetBit(kRICH);
  }
  Bool_t          getFlagRICH(void) const { return TestBits(kRICH);     }
  

  
  void            setFlagInnerMDC(Bool_t b = kTRUE)    { 
    if(b) SetBit(kINNERMDC);  
    else ResetBit(kINNERMDC);
  }
  Bool_t          getFlagInnerMDC(void) const { return TestBits(kINNERMDC);      }

  
  
  void            setFlagOuterMDC(Bool_t b = kTRUE)    { 
    if(b) SetBit(kOUTERMDC);  
    else ResetBit(kOUTERMDC);
  }
  Bool_t          getFlagOuterMDC(void) const { return TestBits(kOUTERMDC);      }
  

  
  void            setFlagTOF(Bool_t b = kTRUE)    { 
    if(b) SetBit(kTOF);  
    else ResetBit(kTOF);
  }
  Bool_t          getFlagTOF(void) const { return TestBits(kTOF);      }

  
  
  void            setFlagSHOWER(Bool_t b = kTRUE) { 
    if(b) SetBit(kSHOWER);  
    else ResetBit(kSHOWER);
  }
  Bool_t          getFlagSHOWER(void) const { return TestBits(kSHOWER);   }

  
  
  void            setFlagRICHIPU(Bool_t b = kTRUE) { 
    if(b) SetBit(kRICHIPU);  
    else ResetBit(kRICHIPU);
  }
  Bool_t          getFlagRICHIPU(void) const { return TestBits(kRICHIPU);   }
  
  // -------------------------------------------------------------------------


  //Report abgular difference between MDC and RICH in degrees and compensated for sin-theta!!!
  Float_t getDeltaThetaMDCRICH() const;
  Float_t getDeltaPhiMDCRICH()const;

  //Report abgular difference between changed runge kutta direction and RICH angle in degrees and compensated for sin-theta!!!
  Float_t getDeltaThetaRKRICH(Float_t fRKTheta) const;
  Float_t getDeltaPhiRKRICH(Float_t fRKPhi,Float_t fRKTheta) const;

  // -------------------------------------------------------------------------

  //getters and setters will not be documented unless neccessary
  void setSector(Int_t nSec){nSector=nSec;}
  Int_t getSector() const {return nSector;}
  Float_t getTof() const {return (iSystem==0)?fShowerTimeOfFlight:fTOFTimeOfFlight;}
  // -------------------------------------------------------------------------
  
  // all data members represent hit properties and are public at present
  Int_t nSector;

  //System==0 -> Shower, System==1 -> TOF
  Short_t iSystem; 

  // RICH observables - see HRichHit class
  Int_t   nRingPadNr;
  Float_t fRingCentroid;
  Float_t fRichTheta;
  Float_t fRichPhi;
  Int_t   nRingPatMat;
  Int_t   nRingHouTra;
  Int_t   nRingAmplitude;
  Int_t   nRingLocalMax4;
  // -------------------------------------------------------------------------
  
  //MDC inner segment - see HMdcSeg class
  /*
  Int_t   iMdcClsSize0;
  Int_t   iMdcNWires0;
  Int_t   iMdcLevelCls0;
  Int_t   iMdcClsSize1;
  Int_t   iMdcNWires1;
  Int_t   iMdcLevelCls1;
  */ //Variables removed due to inclusion of clusinf index - makes this almost obsolete
  Float_t fInnerMdcChiSquare;
  Float_t fInnerMdcdEdx;
  Float_t fInnerMdcdEdxSigma;



  Float_t fMdcRCoord; //Closest approach to beam axis
  Float_t fMdcZCoord; //z coordinate of this closest approach
  Float_t fMdcTheta;
  Float_t fMdcPhi;
  // -------------------------------------------------------------------------
  
  // MDC outer segment - see HMdcSeg class
  /*
  Int_t   iMdcClsSize2;
  Int_t   iMdcNWires2;
  Int_t   iMdcLevelCls2;
  Int_t   iMdcClsSize3;
  Int_t   iMdcNWires3;
  Int_t   iMdcLevelCls3;
  */ //Variables removed due to inclusion of clusinf index - makes this almost obsolete

  Float_t fOuterMdcChiSquare;
  Float_t fOuterMdcdEdx;
  Float_t fOuterMdcdEdxSigma;
  // -------------------------------------------------------------------------

  //Combined Variables for both segments
  Float_t fCombinedMdcdEdx;
  Float_t fCombinedMdcdEdxSigma;
  
  
  // RICH IPU ring observables - see HRichHitIPU class
  Int_t iIPURingQuality;
  Int_t iIPUVetoQuality;
  // -------------------------------------------------------------------------
  

  //SHOWER/TOF observables - see HShowerHit, HTofHit and HShowerHitTof classes
  Float_t fShowerSum[3];
  Int_t   nShowerClS[3];
  Int_t   nShowerRow;
  Int_t   nShowerCol;
  Float_t fShowerTimeOfFlight;
  Float_t fMetaLocalX;
  Float_t fMetaLocalY;

  Float_t fTOFTimeOfFlight;
  Float_t fTOFLeftAmplitude;
  Float_t fTOFRightAmplitude;
  Float_t fTofEloss;
  Int_t iTofinoMult;
  Int_t nTofClsSize;
  Int_t nMetaCell;
  Int_t nTofCell;
  Int_t nTofModule;

  // -------------------------------------------------------------------------
  
  
  //These numbers represent indices in the hit categories by which the corresponding
  //HRichHit, HRichHitIPU, HMdcSeg, HTofHit etc. can be retrieved
  Short_t iIndRICH;
  Short_t iIndRICHIPU;
  Short_t iIndInnerSeg;
  Short_t iIndOuterSeg;
  Short_t iIndTOF;
  Short_t iIndShower;
  Short_t iIndClusInf0;
  Short_t iIndClusInf1;
  Short_t iIndClusInf2;
  Short_t iIndClusInf3;


  //Index of Match-object in the MetaMatch-category
  Short_t iIndMatch;
  // -------------------------------------------------------------------------

  //kTRUE if for a specific momentum algorithm a ring is in correlation with a the track. 
  //This overruns the bit getFlagRICH() because the answer might be different for each momentum method
  //due to our momentum dependent correlation windows.

  Float_t fDistanceToVertex[MAX_MOM_ALGS];
  Bool_t  hasRingCorrelation[MAX_MOM_ALGS]; 
  Bool_t  hasMetaTrackCorrelation[MAX_MOM_ALGS]; 


  //GETTERS:

  //Int_t   getSector()        {return nSector;}

  Short_t getSystem() const        {return iSystem;}

  Int_t   getRingPadNr() const     {return nRingPadNr;}
  Float_t getRingCentroid() const  {return fRingCentroid;}
  Float_t getRichTheta() const     {return fRichTheta;}
  Float_t getRichPhi() const       {return fRichPhi;}
  Int_t   getRingPatMat() const    {return nRingPatMat;}
  Int_t   getRingHouTra() const    {return nRingHouTra;}
  Int_t   getRingAmplitude() const {return nRingAmplitude;}
  Int_t   getRingLocalMax4() const {return nRingLocalMax4;}
  // -------------------------------------------------------------------------
  


  Int_t   getMdcClsSize(Int_t i) const;
  Int_t   getMdcNWires(Int_t i) const;
  Int_t   getMdcLevelCls(Int_t i) const;  

  Float_t getInnerMdcChiSquare() const {return fInnerMdcChiSquare;}
  Float_t getInnerMdcdEdx() const  {return fInnerMdcdEdx;}
  Float_t getInnerMdcdEdxSigma() const  {return fInnerMdcdEdxSigma;}

  Float_t getMdcRCoord() const     {return fMdcRCoord;}
  Float_t getMdcZCoord() const     {return fMdcZCoord;}
  Float_t getMdcTheta() const      {return fMdcTheta;}
  Float_t getMdcPhi() const        {return fMdcPhi;}
  // -------------------------------------------------------------------------
  
  Float_t getOuterMdcChiSquare() const {return fOuterMdcChiSquare;}
  Float_t getOuterMdcdEdx() const  {return fOuterMdcdEdx;}
  Float_t getOuterMdcdEdxSigma() const  {return fOuterMdcdEdxSigma;}
  // -------------------------------------------------------------------------

  Float_t getCombinedMdcdEdx() const  {return fCombinedMdcdEdx;}
  Float_t getCombinedMdcdEdxSigma() const  {return fCombinedMdcdEdxSigma;}
  
  
  Int_t getIPURingQuality() const  {return iIPURingQuality;}
  Int_t getIPUVetoQuality() const  {return iIPUVetoQuality;}
  // -------------------------------------------------------------------------
  

  Float_t getShowerSum(Int_t index) const {return fShowerSum[index];} 
  Int_t   getShowerClS(Int_t index) const {return nShowerClS[index];}
  Int_t   getShowerRow() const            {return nShowerRow;}
  Int_t   getShowerCol() const            {return nShowerCol;}
  Float_t getShowerTimeOfFlight() const   {return fShowerTimeOfFlight;}
  Float_t getMetaLocalX() const          {return fMetaLocalX;}
  Float_t getMetaLocalY() const          {return fMetaLocalY;}
  Float_t getTOFTimeOfFlight() const      {return fTOFTimeOfFlight;}
  Float_t getTOFLeftAmplitude() const     {return fTOFLeftAmplitude;}
  Float_t getTOFRightAmplitude() const    {return fTOFRightAmplitude;}
  Float_t getTofEloss() const             {return fTofEloss;}
  Int_t getTofinoMult() const             {return iTofinoMult;}
  Int_t getTofClsSize() const             {return nTofClsSize;}
  Int_t getMetaCell() const               {return nMetaCell;}
  Int_t getTofCell() const                {return nTofCell;}
  Int_t getTofModule() const              {return nTofModule;}
  // -------------------------------------------------------------------------
  
  Short_t getIndRICH() const    {return iIndRICH;}
  Short_t getIndRICHIPU() const {return iIndRICHIPU;}
  Short_t getIndInnerSeg() const {return iIndInnerSeg;}
  Short_t getIndOuterSeg() const {return iIndOuterSeg;}
  Short_t getIndTOF() const      {return iIndTOF;}
  Short_t getIndShower() const   {return iIndShower;}
  Short_t getIndMatch() const    {return iIndMatch;}
  Short_t getIndClusInf(Int_t i) const;
  // -------------------------------------------------------------------------

  //kTRUE if for a specific momentum algorithm a ring is in correlation with a the track. 
  //This overruns the bit getFlagRICH() because the answer might be different for each momentum method
  //due to our momentum dependent correlation windows.
  Float_t getDistanceToVertex(Int_t momalg) const {return fDistanceToVertex[momalg];}
  Bool_t  getRingCorrelation(Int_t momalg) const  {return hasRingCorrelation[momalg];} 
  Bool_t  getMetaTrackCorrelation(Int_t momalg) const  {return hasMetaTrackCorrelation[momalg];} 

  

  //SETTERS:

  //void setSector(Int_t o_Sector)                {nSector=o_Sector;}

  void setSystem(Short_t o_System)              {iSystem=o_System;}

  void setRingPadNr(Int_t o_RingPadNr)              {nRingPadNr=o_RingPadNr;}
  void setRingCentroid(Float_t o_RingCentroid)  {fRingCentroid=o_RingCentroid;}
  void setRichTheta(Float_t o_RichTheta)     {fRichTheta=o_RichTheta;}
  void setRichPhi(Float_t o_RichPhi)       {fRichPhi=o_RichPhi;}
  void setRingPatMat(Int_t o_RingPatMat)    {nRingPatMat=o_RingPatMat;}
  void setRingHouTra(Int_t o_RingHouTra)    {nRingHouTra=o_RingHouTra;}
  void setRingAmplitude(Int_t o_RingAmplitude) {nRingAmplitude=o_RingAmplitude;}
  void setRingLocalMax4(Int_t o_RingLocalMax4) {nRingLocalMax4=o_RingLocalMax4;}
  // -------------------------------------------------------------------------
  
  /*
  void setMdcClsSize0(Int_t o_MdcClsSize0)   {iMdcClsSize0=o_MdcClsSize0;}
  void setMdcNWires0(Int_t o_MdcNWires0)    {iMdcNWires0=o_MdcNWires0;}
  void setMdcLevelCls0(Int_t o_MdcLevelCls0)  {iMdcLevelCls0=o_MdcLevelCls0;}
  void setMdcClsSize1(Int_t o_MdcClsSize1)   {iMdcClsSize1=o_MdcClsSize1;}
  void setMdcNWires1(Int_t o_MdcNWires1)    {iMdcNWires1=o_MdcNWires1;}
  void setMdcLevelCls1(Int_t o_MdcLevelCls1)  {iMdcLevelCls1=o_MdcLevelCls1;}
  */
  void setInnerMdcChiSquare(Float_t o_InnerMdcChiSquare) {fInnerMdcChiSquare=o_InnerMdcChiSquare;}
  void setInnerMdcdEdx(Float_t o_InnerMdcdEdx)  {fInnerMdcdEdx=o_InnerMdcdEdx;}
  void setInnerMdcdEdxSigma(Float_t o_InnerMdcdEdxSigma)  {fInnerMdcdEdxSigma=o_InnerMdcdEdxSigma;}

  void setMdcRCoord(Float_t o_MdcRCoord)     {fMdcRCoord=o_MdcRCoord;}
  void setMdcZCoord(Float_t o_MdcZCoord)     {fMdcZCoord=o_MdcZCoord;}
  void setMdcTheta(Float_t o_MdcTheta)      {fMdcTheta=o_MdcTheta;}
  void setMdcPhi(Float_t o_MdcPhi)        {fMdcPhi=o_MdcPhi;}
  // -------------------------------------------------------------------------
  
  /*
  void setMdcClsSize2(Int_t o_MdcClsSize2)   {iMdcClsSize2=o_MdcClsSize2;}
  void setMdcNWires2(Int_t o_MdcNWires2)    {iMdcNWires2=o_MdcNWires2;}
  void setMdcLevelCls2(Int_t o_MdcLevelCls2)  {iMdcLevelCls2=o_MdcLevelCls2;}
  void setMdcClsSize3(Int_t o_MdcClsSize3)   {iMdcClsSize3=o_MdcClsSize3;}
  void setMdcNWires3(Int_t o_MdcNWires3)    {iMdcNWires3=o_MdcNWires3;}
  void setMdcLevelCls3(Int_t o_MdcLevelCls3)  {iMdcLevelCls3=o_MdcLevelCls3;}
  */
  void setOuterMdcChiSquare(Float_t o_OuterMdcChiSquare) {fOuterMdcChiSquare=o_OuterMdcChiSquare;}
  void setOuterMdcdEdx(Float_t o_OuterMdcdEdx)  {fOuterMdcdEdx=o_OuterMdcdEdx;}
  void setOuterMdcdEdxSigma(Float_t o_OuterMdcdEdxSigma)  {fOuterMdcdEdxSigma=o_OuterMdcdEdxSigma;}
  // -------------------------------------------------------------------------

  void setCombinedMdcdEdx(Float_t o_CombinedMdcdEdx)  {fCombinedMdcdEdx=o_CombinedMdcdEdx;}
  void setCombinedMdcdEdxSigma(Float_t o_CombinedMdcdEdxSigma)  {fCombinedMdcdEdxSigma=o_CombinedMdcdEdxSigma;}

  
  void setIPURingQuality(Int_t o_IPURingQuality)  {iIPURingQuality=o_IPURingQuality;}
  void setIPUVetoQuality(Int_t o_IPUVetoQuality)  {iIPUVetoQuality=o_IPUVetoQuality;}
  // -------------------------------------------------------------------------
  

  void setShowerSum(Int_t index, Float_t o_ShowerSum) {fShowerSum[index]=o_ShowerSum;} 
  void setShowerClS(Int_t index, Int_t o_ShowerClS) {nShowerClS[index]=o_ShowerClS;}
  void setShowerRow(Int_t o_ShowerRow)            {nShowerRow=o_ShowerRow;}
  void setShowerCol(Int_t o_ShowerCol)            {nShowerCol=o_ShowerCol;}
  void setShowerTimeOfFlight(Float_t o_ShowerTimeOfFlight)   {fShowerTimeOfFlight=o_ShowerTimeOfFlight;}
  void setMetaLocalX(Float_t o_MetaLocalX)   {fMetaLocalX=o_MetaLocalX;}
  void setMetaLocalY(Float_t o_MetaLocalY)   {fMetaLocalY=o_MetaLocalY;}

  void setTOFTimeOfFlight(Float_t o_TOFTimeOfFlight)      {fTOFTimeOfFlight=o_TOFTimeOfFlight;}
  void setTOFLeftAmplitude(Float_t o_TOFLeftAmplitude)     {fTOFLeftAmplitude=o_TOFLeftAmplitude;}
  void setTOFRightAmplitude(Float_t o_TOFRightAmplitude)    {fTOFRightAmplitude=o_TOFRightAmplitude;}
  void setTofEloss(Float_t o_TofEloss)    {fTofEloss=o_TofEloss;}

  void setTofinoMult(Int_t o_TofinoMult)             {iTofinoMult=o_TofinoMult;}
  void setTofClsSize(Int_t o_TofClsSize)             {nTofClsSize=o_TofClsSize;}
  void setMetaCell(Int_t o_MetaCell)               {nMetaCell=o_MetaCell;}
  void setTofCell(Int_t o_TofCell)                 {nTofCell=o_TofCell;}
  void setTofModule(Int_t o_TofModule)                 {nTofModule=o_TofModule;}
  // -------------------------------------------------------------------------
  
  void setIndRICH(Short_t o_IndRICH)    {iIndRICH=o_IndRICH;}
  void setIndRICHIPU(Short_t o_IndRICHIPU) {iIndRICHIPU=o_IndRICHIPU;}
  void setIndInnerSeg(Short_t o_IndInnerSeg) {iIndInnerSeg=o_IndInnerSeg;}
  void setIndOuterSec(Short_t o_IndOuterSeg) {iIndOuterSeg=o_IndOuterSeg;}
  void setIndTOF(Short_t o_IndTOF)      {iIndTOF=o_IndTOF;}
  void setIndShower(Short_t o_IndShower)   {iIndShower=o_IndShower;}
  void setIndMatch(Short_t o_IndMatch)    {iIndMatch=o_IndMatch;}
  // -------------------------------------------------------------------------

  //kTRUE if for a specific momentum algorithm a ring is in correlation with a the track. 
  //This overruns the bit setFlagRICH() because the answer might be different for each momentum method
  //due to our momentum dependent correlation windows.
  void setDistanceToVertex(Int_t momalg, Float_t o_DistanceToVertex) {fDistanceToVertex[momalg]=o_DistanceToVertex;}
  void setRingCorrelation(Int_t momalg, Float_t o_RingCorrelation)  {hasRingCorrelation[momalg]=o_RingCorrelation;} 
  void setMetaTrackCorrelation(Int_t momalg, Float_t o_MetaTrackCorrelation)  {hasMetaTrackCorrelation[momalg]=o_MetaTrackCorrelation;} 

  


  ClassDef(HPidHitData, 9)  // Store data for HPidParticle from Hits
    };
    
#endif /* HPidHitData_h */
