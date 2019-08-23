#ifndef HMDCWIREINFO_H
#define HMDCWIREINFO_H
using namespace std;
#include <iostream>
#include <cstdlib>
#include <TObject.h>

class HMdcSeg;
class HMdcHit;
class HMdcClusInf;
class HMdcClusFit;
class HMdcWireFit;
class HMdcCal1;
class HKickTrack;
class HMdcClus;

class HMdcWireInfo : public TObject {
protected:
    Int_t sector;         // sector number
    Int_t module;         // module number
    Int_t layer;          // layer number
    Int_t cell;           // cell number
    Int_t slots[5][8];    // number of slots * 8 index
    Int_t nslots;         // number of slots
    Int_t list[8]; //!
public:
    enum ids {hit,clusinf,clusfit,wirefit,clus,seg,kick,pid};

    HMdcWireInfo();
    ~HMdcWireInfo();
    void clear(){sector=module=layer=cell=-1;nslots=0;for(Int_t i=0;i<5;i++){for(Int_t j=0;j<8;j++){slots[i][j]=-1;}}}
    void setModule(const Int_t m) { module = m; }
    void setLayer(const Int_t l)  { layer = l; }
    void setCell(const Int_t c)   { cell = c; }
    void setAddress(const Int_t s,const Int_t m,const Int_t l,const Int_t c) { sector=s;module=m;layer=l;cell=c;}
    Int_t getSector(void) const { return sector; }
    Int_t getModule(void) const { return module; }
    Int_t getLayer(void) const  { return layer; }
    Int_t getCell(void) const   { return cell; }
    Int_t getNSlots(){return nslots;}
    void getAddress(Int_t& s,Int_t& m,Int_t& l,Int_t& c) { s=sector;m=module;l=layer;c=cell;}
    void setList()
    {
        if(nslots<5){
            for(Int_t i=0;i<8;i++){slots[nslots][i]=list[i];}nslots++;
        }else{ Warning("setList()","Maximum number of slots exceeded!");}
    }
    Int_t* getList(){return &list[0];}
    void   setListIndex(ids id,Int_t index){ list[id]=index; }
     Int_t getIndex(ids id,Int_t slot )
    {
        if(slot<5){ return slots[slot][id];
        } else { Warning("getIndex()","Slot number out of range!");return -1;}
    }
    HKickTrack*  getKickTrack(Int_t slot=0);
    HMdcSeg*     getMdcSeg(Int_t slot=0);
    HMdcHit*     getMdcHit(Int_t slot=0);
    HMdcClusInf* getMdcClusInf(Int_t slot=0);
    HMdcClusFit* getMdcClusFit(Int_t slot=0);
    HMdcWireFit* getMdcWireFit(Int_t slot=0);
    HMdcClus*    getMdcClus(Int_t slot=0);
    //getPid(Int_t slot=0);
    HMdcCal1*    getMdcCal1(Int_t slot=0);
    ClassDef(HMdcWireInfo,1) // information on usage of a wire
};

#endif  /* HMDCWIREINFO_H */

