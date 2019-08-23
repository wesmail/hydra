#ifndef HMDCWIRESTAT_H
#define HMDCWIRESTAT_H
#include "TArrayF.h"
#include "TH2.h"
#include "TCanvas.h"
#include "hparcond.h"

class HParamList;

class HMdcWireStat : public HParCond {
protected:
    TH2F* mb        [6][4]; // mapping sector,module,layer,wire to mother board
    TH2F* tdc       [6][4]; // mapping sector,module,layer,wire to tdc
    TH2F* statusmap [6][4]; // status of the wire: 1=working,-1=not connected, -3=no correct offset, -7=dead, -10=init
    TH2F* efficiency[6][4]; // efficiency of the wire: 0-1
    TH2F* offset    [6][4]; // offset of the wire in ns (for simulation)

public:
    HMdcWireStat(const Char_t* name ="MdcWireStat",
                 const Char_t* title  ="Mdc lookup table for wire stat",
                 const Char_t* context="MdcWireStatProduction");
    ~HMdcWireStat();

    Bool_t  init(HParIo*, Int_t*);
    Int_t   write(HParIo*);
    Int_t   getStatus    (Int_t sec,Int_t mod,Int_t lay,Int_t cell)            {return (Int_t)statusmap[sec][mod]->GetBinContent(cell+1,lay+1);}
    void    setStatus    (Int_t sec,Int_t mod,Int_t lay,Int_t cell,Int_t stat) {statusmap[sec][mod]->SetBinContent(cell+1,lay+1,stat);}
    Float_t getEfficiency(Int_t sec,Int_t mod,Int_t lay,Int_t cell)            {return (Float_t)efficiency[sec][mod]->GetBinContent(cell+1,lay+1);}
    void    setEfficiency(Int_t sec,Int_t mod,Int_t lay,Int_t cell,Float_t eff){efficiency[sec][mod]->SetBinContent(cell+1,lay+1,eff);}
    Float_t getOffset    (Int_t sec,Int_t mod,Int_t lay,Int_t cell)            {return (Float_t)offset[sec][mod]->GetBinContent(cell+1,lay+1);}
    void    setOffset    (Int_t sec,Int_t mod,Int_t lay,Int_t cell,Float_t off){offset[sec][mod]->SetBinContent(cell+1,lay+1,off);}
    Int_t   getMbo       (Int_t sec,Int_t mod,Int_t lay,Int_t cell)            {return (Int_t)mb[sec][mod]->GetBinContent(cell+1,lay+1);}
    void    setMbo       (Int_t sec,Int_t mod,Int_t lay,Int_t cell,Int_t mbo)  {mb[sec][mod]->SetBinContent(cell+1,lay+1,mbo);}
    Int_t   getTdc       (Int_t sec,Int_t mod,Int_t lay,Int_t cell)            {return (Int_t)tdc[sec][mod]->GetBinContent(cell+1,lay+1);}
    void    setTdc       (Int_t sec,Int_t mod,Int_t lay,Int_t cell,Int_t t)    {tdc[sec][mod]->SetBinContent(cell+1,lay+1,t);}
    Int_t   getDeadWires();
    Int_t   getDeadWires(Int_t,Int_t);
    TCanvas* plotStatusMbVsTdc();
    TCanvas* plotEfficiencyMbVsTdc();
    TCanvas* plotStatus(Int_t,Int_t);
    TCanvas* plotEfficiency(Int_t,Int_t);
    TCanvas* plotOffset(Int_t,Int_t);
    void    printStats();
    void    printStatsSum();
    void    putParams(HParamList*);
    Bool_t  getParams(HParamList*);
    void    clear();
    void    unsetDirectory();
    void    printParam(void);
    ClassDef(HMdcWireStat,2) //  MDC lookup table (wire stat, noise)
};
#endif  /*!HMDCWIRESTAT_H*/
