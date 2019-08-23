//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////
//*-- AUTHOR : J. Markert
////////////////////////////////////////////////////////////////////////////
// HTofinoDigitPar
//
// Container class for Tofino digitization parameters
//
////////////////////////////////////////////////////////////////////////////
#include "htofinodigitpar.h"
#include "hpario.h"
#include "hdetpario.h"
#include "hmessagemgr.h"
#include "hparamlist.h"
#include "hmessagemgr.h"
#include <stdlib.h>

ClassImp(HTofinoDigitPar)

HTofinoDigitPar::HTofinoDigitPar(const char* name,const char* title,
                       const char* context)
    : HParCond(name,title,context)
{
    //
    strcpy(detName,"Tofino");

    m_fOffset       .Set(24);
    m_fSlope        .Set(24);
    m_fDeltaSlope   .Set(24);
    m_fDeltaOffset  .Set(24);
    m_fCFDThreshold .Set(24);
    m_fADCThreshold .Set(24);
    m_fADCPedestal  .Set(24);
    m_fElossParam0  .Set(24);
    m_fElossParam1  .Set(24);
    m_fElossParam2  .Set(24);
    m_fElossParam3  .Set(24);
    m_fResolParam0  .Set(24);
    m_fResolParam1  .Set(24);
    m_fResolParam2  .Set(24);
    m_fDistParam0   .Set(24);
    m_fDistParam1   .Set(24);
    m_fDistParam2   .Set(24);
    m_fDistParam3   .Set(24);
    m_fMinLength    .Set(24);
    m_nDistParamType.Set(24);
    m_fTimeChannel  .Set(24);
    m_fCoordPos     .Set(24);

    clear();
}
HTofinoDigitPar::~HTofinoDigitPar()
{
  // destructor
}
void HTofinoDigitPar::clear()
{

    m_fOffset.       Reset(0);
    m_fSlope.        Reset(0);
    m_fDeltaSlope.   Reset(0);
    m_fDeltaOffset.  Reset(0);
    m_fCFDThreshold. Reset(0);
    m_fADCThreshold. Reset(0);
    m_fADCPedestal.  Reset(0);
    m_fElossParam0.  Reset(0);
    m_fElossParam1.  Reset(0);
    m_fElossParam2.  Reset(0);
    m_fElossParam3.  Reset(0);
    m_fResolParam0.  Reset(0);
    m_fResolParam1.  Reset(0);
    m_fResolParam2.  Reset(0);
    m_fDistParam0.   Reset(0);
    m_fDistParam1.   Reset(0);
    m_fDistParam2.   Reset(0);
    m_fDistParam3.   Reset(0);
    m_fMinLength.    Reset(0);
    m_nDistParamType.Reset(0);
    m_fTimeChannel.  Reset(0);
    m_fCoordPos.     Reset(0);

    status=kFALSE;
    resetInputVersions();
    changed=kFALSE;
}
void HTofinoDigitPar::printParam(void)
{
    // prints the parameters of HTofinoDigitPar to the screen.
    SEPERATOR_msg("#",60);
    INFO_msg(10,HMessageMgr::DET_TOFINO,"HTofinoDigitPar:");
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    cout<<"sector "<<s<<" cell "<<m<<" -------------------------------"<<endl;
            cout<<"\t Offset          "<<m_fOffset[s*4+m]<<endl;
            cout<<"\t Slope           "<<m_fSlope [s*4+m]<<endl;
            cout<<"\t DeltaSlope      "<<m_fDeltaSlope   [s*4+m]<<endl;
            cout<<"\t DeltaOffset     "<<m_fDeltaOffset  [s*4+m]<<endl;
            cout<<"\t CDFThreshold    "<<m_fCFDThreshold [s*4+m]<<endl;
            cout<<"\t ADCThreshold    "<<m_fADCThreshold [s*4+m]<<endl;
            cout<<"\t ADCPedestal     "<<m_fADCPedestal  [s*4+m]<<endl;
	    cout<<"\t ElossParam      "
		<<m_fElossParam0[s*4+m]<<" "
		<<m_fElossParam1[s*4+m]<<" "
		<<m_fElossParam2[s*4+m]<<" "
		<<m_fElossParam3[s*4+m]<<endl;
	    cout<<"\t ResolParam      "
		<<m_fResolParam0[s*4+m]<<" "
		<<m_fResolParam1[s*4+m]<<" "
		<<m_fResolParam2[s*4+m]<<endl;
	    cout<<"\t DistParam       "
		<<m_fDistParam0[s*4+m]<<" "
		<<m_fDistParam1[s*4+m]<<" "
		<<m_fDistParam2[s*4+m]<<" "
		<<m_fDistParam3[s*4+m]<<endl;

            cout<<"\t MinLength       "<<m_fMinLength    [s*4+m]<<endl;
            cout<<"\t nDistParamType  "<<m_nDistParamType[s*4+m]<<endl;
            cout<<"\t TimeChannel     "<<m_fTimeChannel  [s*4+m]<<endl;
            cout<<"\t CoordPos        "<<m_fCoordPos     [s*4+m]<<endl;
           
	}
    }
    SEPERATOR_msg("#",60);
}
Bool_t HTofinoDigitPar::init(HParIo* inp,Int_t* set)
{
  // intitializes the container from an input
  HDetParIo* input=inp->getDetParIo("HTofinoParIo");
  if (input) return (input->init(this,set));
  return kFALSE;
}
Int_t HTofinoDigitPar::write(HParIo* output)
{
  // writes the container to an output
  HDetParIo* out=output->getDetParIo("HTofinoParIo");
  if (out) return out->write(this);
  return -1;
}
void HTofinoDigitPar::putParams(HParamList* l)
{
    // Puts all params of HTofinoDigitPar to the parameter list of
    // HParamList (which ist used by the io);
    if (!l) return;
    l->add("m_fOffset"       ,m_fOffset       ,6);
    l->add("m_fSlope"        ,m_fSlope        ,6);
    l->add("m_fDeltaSlope"   ,m_fDeltaSlope   ,5);
    l->add("m_fDeltaOffset"  ,m_fDeltaOffset  ,3);
    l->add("m_fCFDThreshold" ,m_fCFDThreshold ,1);
    l->add("m_fADCThreshold" ,m_fADCThreshold ,1);
    l->add("m_fADCPedestal"  ,m_fADCPedestal  ,1);
    l->add("m_fElossParam0"  ,m_fElossParam0  ,5);
    l->add("m_fElossParam1"  ,m_fElossParam1  ,3);
    l->add("m_fElossParam2"  ,m_fElossParam2  ,5);
    l->add("m_fElossParam3"  ,m_fElossParam3  ,6);
    l->add("m_fResolParam0"  ,m_fResolParam0  ,2);
    l->add("m_fResolParam1"  ,m_fResolParam1  ,6);
    l->add("m_fResolParam2"  ,m_fResolParam2  ,10);
    l->add("m_fDistParam0"   ,m_fDistParam0   ,3);
    l->add("m_fDistParam1"   ,m_fDistParam1   ,3);
    l->add("m_fDistParam2"   ,m_fDistParam2   ,3);
    l->add("m_fDistParam3"   ,m_fDistParam3   ,4);
    l->add("m_fMinLength"    ,m_fMinLength    ,3);
    l->add("m_nDistParamType",m_nDistParamType  );
    l->add("m_fTimeChannel"  ,m_fTimeChannel  ,3);
    l->add("m_fCoordPos"     ,m_fCoordPos     ,3);

}
Bool_t HTofinoDigitPar::getParams(HParamList* l)
{
    if(!l) return kFALSE;
    if(!( l->fill("m_fOffset"       ,&m_fOffset       ))) return kFALSE;
    if(!( l->fill("m_fSlope"        ,&m_fSlope        ))) return kFALSE;
    if(!( l->fill("m_fDeltaSlope"   ,&m_fDeltaSlope   ))) return kFALSE;
    if(!( l->fill("m_fDeltaOffset"  ,&m_fDeltaOffset  ))) return kFALSE;
    if(!( l->fill("m_fCFDThreshold" ,&m_fCFDThreshold ))) return kFALSE;
    if(!( l->fill("m_fADCThreshold" ,&m_fADCThreshold ))) return kFALSE;
    if(!( l->fill("m_fADCPedestal"  ,&m_fADCPedestal  ))) return kFALSE;
    if(!( l->fill("m_fElossParam0"  ,&m_fElossParam0  ))) return kFALSE;
    if(!( l->fill("m_fElossParam1"  ,&m_fElossParam1  ))) return kFALSE;
    if(!( l->fill("m_fElossParam2"  ,&m_fElossParam2  ))) return kFALSE;
    if(!( l->fill("m_fElossParam3"  ,&m_fElossParam3  ))) return kFALSE;
    if(!( l->fill("m_fResolParam0"  ,&m_fResolParam0  ))) return kFALSE;
    if(!( l->fill("m_fResolParam1"  ,&m_fResolParam1  ))) return kFALSE;
    if(!( l->fill("m_fResolParam2"  ,&m_fResolParam2  ))) return kFALSE;
    if(!( l->fill("m_fDistParam0"   ,&m_fDistParam0   ))) return kFALSE;
    if(!( l->fill("m_fDistParam1"   ,&m_fDistParam1   ))) return kFALSE;
    if(!( l->fill("m_fDistParam2"   ,&m_fDistParam2   ))) return kFALSE;
    if(!( l->fill("m_fDistParam3"   ,&m_fDistParam3   ))) return kFALSE;
    if(!( l->fill("m_fMinLength"    ,&m_fMinLength    ))) return kFALSE;
    if(!( l->fill("m_nDistParamType",&m_nDistParamType))) return kFALSE;
    if(!( l->fill("m_fTimeChannel"  ,&m_fTimeChannel  ))) return kFALSE;
    if(!( l->fill("m_fCoordPos"     ,&m_fCoordPos     ))) return kFALSE;

    return kTRUE;
}
Float_t HTofinoDigitPar::calcDriftTime(Int_t nSector, Int_t nCell, Float_t fRow)
{
    Int_t   ind   = nSector*4+nCell;
    Float_t fDist = calcDistance(nSector,nCell,fRow);
    Float_t fTime = fDist*m_fSlope[ind];
    if(fTime <= 0) {
	cout<<"cell,type,dist.par " <<nCell <<" "
	    <<m_nDistParamType[ind] <<" "
	    <<m_fDistParam0[ind]    <<" "
	    <<m_fDistParam1[ind]    <<" "
	    <<m_fDistParam2[ind]    <<" "
	    <<m_fDistParam3[ind]    <<endl;
	cout <<"fRow,fDist,fTime "  <<fRow <<" " <<fDist <<" " <<fTime <<endl;
    }
    return fTime;
}

Float_t HTofinoDigitPar::calcLightPropTime(Int_t nSector, Int_t nCell, Float_t fY)
{
    Float_t fDist = calcDistPMT(nSector,nCell,fY);
    Float_t fTime = fDist*m_fSlope[nSector*4+nCell];
    return fTime;
}
Float_t HTofinoDigitPar::calcLightPropTimeFull(Int_t nSector, Int_t nCell, Float_t fTime)
{
    return m_fOffset[nSector*4+nCell] + fTime;
}
Float_t HTofinoDigitPar::calcDeltaLightPropTime(Int_t nSector, Int_t nCell, Float_t fY)
{
    Float_t fDist = calcDistPMT(nSector,nCell,fY);
    return calcTimeResol(nSector,nCell,fDist);
}
Float_t HTofinoDigitPar::channelToTime(Int_t nSector, Int_t nCell, Float_t fTime)
{
    return m_fTimeChannel[nSector*4+nCell]*fTime;
}
Float_t HTofinoDigitPar::timeToChannel(Int_t nSector, Int_t nCell, Float_t fTime)
{
    return fTime/m_fTimeChannel[nSector*4+nCell];
}

