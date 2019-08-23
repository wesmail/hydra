#include "hmdcconfig.h"

ClassImp(HMdcConfig)

HMdcConfig::HMdcConfig(const char* name,const char* title)
    :TNamed(name,title)
{
    setScale(1000);
}
HMdcConfig::HMdcConfig()
{
  setScale(1000);
}
HMdcConfig::~HMdcConfig(){}
