#include "hmdcconnectedtdc.h"
#include "hmdclookupgeom.h"
#include "hmdclookupraw.h"
#include "hmdcrawstruct.h"
#include "hmdcgeomstruct.h"
#include "hmdcdetector.h"
#include "hades.h"
#include "hruntimedb.h"
#include "hdetector.h"
#include "hspectrometer.h"
#include <stdlib.h>

ClassImp(HMdcConnectedTdc)

HMdcConnectedTdc::HMdcConnectedTdc(const char* name,const char* title)
    :TNamed(name,title)
{
   reset();
}
HMdcConnectedTdc::HMdcConnectedTdc()
{
}
HMdcConnectedTdc::~HMdcConnectedTdc(){}
void HMdcConnectedTdc::fill()
{
    HDetector *mdcDet=0;
    mdcDet = gHades->getSetup()->getDetector("Mdc");
    if (!mdcDet){
	Error("HMdcConnectedTdc::fill()","Mdc-Detector setup (gHades->getSetup()->getDetector(\"Mdc\")) missing.");
        exit(1);
    }
    else
    {
        HMdcLookupGeom* lookup=0;
	lookup=(HMdcLookupGeom*)gHades->getRuntimeDb()->findContainer("MdcLookupGeom");
	if(!lookup){Error("HMdcConnectedTdc::fill()","HMdcLookupGeom not initialized!");exit(1);}

	HMdcLookupRaw* lookupraw=0;
	lookupraw=(HMdcLookupRaw*)gHades->getRuntimeDb()->findContainer("MdcLookupRaw");
	if(!lookupraw){Error("HMdcConnectedTdc::fill()","HMdcLookupRaw not initialized!");exit(1);}

	HMdcRawStruct* rawstru=0;
	rawstru=(HMdcRawStruct*)gHades->getRuntimeDb()->findContainer("MdcRawStruct");
	if(!rawstru){Error("HMdcConnectedTdc::fill()","HMdcRawStruct not initialized!");exit(1);}

	HMdcGeomStruct* geomstru=0;
	geomstru=(HMdcGeomStruct*)gHades->getRuntimeDb()->findContainer("MdcGeomStruct");
	if(!geomstru){Error("HMdcConnectedTdc::fill()","HMdcGeomStruct not initialized!");exit(1);}
        Int_t mboname;
	for(Int_t s=0;s<6;s++){
	    for(Int_t m=0;m<4;m++){
		if (!mdcDet->getModule(s, m)) continue;
		mdcSetup[s][m]=1;
		for(Int_t mb=0;mb<16;mb++){
		    sscanf((*rawstru)[s][m][mb].GetName(),"%i",&mboname);
		    rawStruct[s][m][mb][0]=(*rawstru)[s][m][mb].getNTdcs();
		    rawStruct[s][m][mb][1]=mboname;
                  
		    HMdcLookupMoth& moth=(*lookup)[s][m][mb];
		    for(Int_t t=0;t<moth.getSize();t++){
			HMdcLookupChan& chan=(*lookup)[s][m][mb][t];
			raw[s][m][mb][t][0]=chan.getNLayer();
			raw[s][m][mb][t][1]=chan.getNCell();
		    }
		}
		for(Int_t l=0;l<6;l++){
                    HMdcLookupLayer& lay=(*lookupraw)[s][m][l];
		    for(Int_t c=0;c<lay.getSize();c++){

			geomStruct[s][m][l]=(*geomstru)[s][m][l];

			HMdcLookupCell& cell=(*lookupraw)[s][m][l][c];
			cal[s][m][l][c][0]=cell.getNMoth();
			cal[s][m][l][c][1]=cell.getNChan();
		    }
		}
	    }
	}
    }
}
void HMdcConnectedTdc::printRaw()
{
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    for(Int_t mb=0;mb<16;mb++){
		for(Int_t t=0;t<96;t++){
		    printf("s %i m %i mb %2i tdc %2i lay %i cell %3i\n",s,m,mb,t,raw[s][m][mb][t][0],raw[s][m][mb][t][1]);
		}
	    }
	}
    }
}
void HMdcConnectedTdc::printCal()
{
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    for(Int_t l=0;l<6;l++){
		for(Int_t c=0;c<210;c++){
		    printf("s %i m %i l %i c %3i mb %2i tdc %2i\n",s,m,l,c,cal[s][m][l][c][0],cal[s][m][l][c][1]);
		}
	    }
	}
    }
}
void HMdcConnectedTdc::printGeomStruct()
{
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    for(Int_t l=0;l<6;l++){
		    printf("s %i m %i l %i c %3i\n",s,m,l,geomStruct[s][m][l]);
	    }
	}
    }
}
void HMdcConnectedTdc::printRawStruct()
{
    for(Int_t s=0;s<6;s++){
	for(Int_t m=0;m<4;m++){
	    for(Int_t mb=0;mb<16;mb++){
		    printf("s %i m %i mb %2i mbo name %3i tdc %2i\n",s,m,mb,rawStruct[s][m][mb][1],rawStruct[s][m][mb][0]);
	    }
	}
    }
}

