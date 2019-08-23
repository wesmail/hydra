#ifndef HRTDATA_H
#define HRTDATA_H

#include "hparset.h"

class HParIo;
class HRtGrid;

class HRtData : public HParSet {
  public:
    HRtData(const Char_t *name="RtData",
	const Char_t *title="Reference trajectories",
	const Char_t *context="RtProduction");
    virtual ~HRtData(void);
    Bool_t init(HParIo *inp, Int_t *set);
    Int_t write(HParIo *io);
    HRtGrid *getGrid(Int_t pol) { return (pol>0)?fGrid[0]:fGrid[1]; }
    void setGrids(HRtGrid *pos,HRtGrid *neg) { fGrid[0]=pos; fGrid[1]=neg;}
    void clear(void) { fGrid[0]=0; fGrid[1]=0; }
    void stealData(HRtData &o);
  protected:
    HRtGrid *fGrid[2]; //Data grids for e+ e-
  public:
    ClassDef(HRtData,1) //Reference trajectories 
};

#endif 
