#ifndef HMDCDEDXHISTS_H
#define HMDCDEDXHISTS_H

#include "hreconstructor.h"
#include "TH2.h"
#include "TH1.h"
#include "TFile.h"
#include "TCanvas.h"
#include "TNtuple.h"

class HCategory;
class HIterator;
class HMdcDeDx;

class HMdcDeDxHists : public HReconstructor {
protected:
  HCategory* trkcandCat;   //! pointer to the HMdcTrkCand data
  HCategory* segCat;       //! pointer to the HMdcSeg data
  Bool_t hasPrinted;       // flag is set if printStatus is called
  HIterator* trkcanditer;  //! iterator on HMdcTrkCand data.
  HCategory* kickCat;      //! pointer to the kicktrack data (fillhists only)
  HIterator* kickiter;     //! iterator on kicktrack  data.  (fillhists only)
  HCategory* splineCat;    //! pointer to the spline track data (fillhists only)
  HCategory* metamatchCat; //! pointer to the meta match data (fillhists only)
  HIterator* metamatchiter;//! iterator on metamatch  data.  (fillhists only)
  HLocation lockick;       //! location in kicktrack category(fillhists only)
  HLocation locseg;        //! location in segment category

  //--------------config---------------------------------
  Bool_t useSpline;        //! use spline fit (kTRUE) or kick plane (kFALSE)
  Bool_t online;           //! switch for online/offline
  Bool_t ntuple;           //! switch ntuple option
  Bool_t debug;            //! switch debug option (variables of HMdcTrkCand are overwritten with p*charge/single_double flag)
  Bool_t cpr;              //! switch option for singles/doubles (HKickTrack->getRingId() will keep single/double flag (1,2) corresponding to Jaro's CPR)
  Int_t mode;              //! switch mode of HMdcDeDX::calcDedX(......,mode,module)
  Int_t module;            //! switch module of HMdcDeDX::calcDedX(......,mode,module)
  Float_t window;          //! window for truncated mean
  Int_t minwires;          //! minimum number of wires
  Bool_t useshifts;        //! switch normalization on/off

  //--------------control hists--------------------------
  TFile* output;
  TH2F* hdedx_p_sec_seg_sys[6][2][2]; //! dedx vs mom for sec/seg/sys
  TH2F* hdedx_p_sec_seg    [6][2];    //! dedx vs mom for sec/seg
  TH2F* hdedx_p_sec_trk_sys[6][2];    //! dedx vs mom for sec/trk/sys
  TH2F* hdedx_p_sec_trk    [6];       //! dedx vs mom for sec/trk

  TH2F* hdedx_p_all_seg_sys[2][2];    //! dedx vs mom for seg/sys
  TH2F* hdedx_p_all_seg    [2];       //! dedx vs mom for seg
  TH2F* hdedx_p_all_trk_sys[2];       //! dedx vs mom for trk/sys
  TH2F* hdedx_p_all_trk;              //! dedx vs mom for trk
  TH1F* hdriftcells[2];               //! histograms for single/double single measurements
  TCanvas* result;                    //! canvas for online option
  TNtuple* celltuple;                 //! ntuple for investigations

  void   makeHists();
  void   initParameters(void);
  void   setParContainers(void);
public:
  HMdcDeDxHists(void);
  HMdcDeDxHists(const Text_t* name,const Text_t* title);
  ~HMdcDeDxHists(void);
  Bool_t init(void);
  Bool_t finalize(void);
  void   printStatus();
  void   setUseSpline(Bool_t ok)       {useSpline=ok;}
  void   setOnline(Bool_t ok)          {online=ok;}
  void   setNTuple(Bool_t ok)          {ntuple=ok;}
  void   setDebug(Bool_t ok)           {debug=ok;}
  void   setCPR  (Bool_t ok)           {cpr=ok;}
  void   setDeDxMethod(Int_t m)        {mode=m;}
  void   setDeDxModule(Int_t m)        {module=m;}
  void   setDeDxWindow(Float_t w)      {window=w;}
  void   setDeDxMinimumWires(Int_t n)  {minwires=n;}
  void   setDeDxUseShifts(Bool_t shift){useshifts=shift;}
  Int_t  execute(void);
  ClassDef(HMdcDeDxHists,1) //  cal1 t2-t1 -> dEdx for Mdc data
};

#endif /* !HMDCDEDXHISTS_H */

