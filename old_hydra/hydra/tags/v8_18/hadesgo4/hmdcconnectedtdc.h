#ifndef HMDCCONNECTEDTDC_H
#define HMDCCONNECTEDTDC_H

#include "TNamed.h"
/** Class to hold numbers for MDC Online Display.
 */
class HMdcLookupGeom;
class HMdcLookupRaw;
class HMdcRawStruct;
class HMdcGeomStruct;

class HMdcConnectedTdc : public TNamed {
private:
Int_t raw[6][4][16][96][2];
Int_t cal[6][4][6][210][2];
Int_t rawStruct [6][4][16][2];
Int_t geomStruct[6][4][6];
Int_t mdcSetup[6][4];
public:
    HMdcConnectedTdc(const Char_t*,const Char_t*);
    HMdcConnectedTdc();
    ~HMdcConnectedTdc();
    void reset()
    {
	for(Int_t s=0;s<6;s++){
	    for(Int_t m=0;m<4;m++){
		mdcSetup[s][m]=0;
		for(Int_t mb=0;mb<16;mb++){
                    rawStruct[s][m][mb][0]=-2;
		    rawStruct[s][m][mb][1]=-2;
		    for(Int_t t=0;t<96;t++){
			raw[s][m][mb][t][0]=-2;
                        raw[s][m][mb][t][1]=-2;

		    }
		}
                for(Int_t l=0;l<6;l++){
		    geomStruct[s][m][l]=-2;
		    geomStruct[s][m][l]=-2;
		    for(Int_t c=0;c<210;c++){
			cal[s][m][l][c][0]=-2;
                        cal[s][m][l][c][1]=-2;
		    }
		}

	    }
	}
    }
    void fill();
    void printRaw();
    void printCal();
    void printRawStruct();
    void printGeomStruct();
    void getCalAddress(Int_t s,Int_t m,Int_t mb,Int_t t,Int_t* l ,Int_t* c){*l =raw[s][m][mb][t][0];*c=raw[s][m][mb][t][1];}
    void getRawAddress(Int_t s,Int_t m,Int_t  l,Int_t c,Int_t* mb,Int_t* t){*mb=cal[s][m][l][c][0]; *t=cal[s][m][l][c][1];}
    Int_t getNTdcs (Int_t s,Int_t m,Int_t mb) {return rawStruct [s][m][mb][0];}
    Int_t getNCells(Int_t s,Int_t m,Int_t l ) {return geomStruct[s][m][l];}
    Int_t getMboName(Int_t s,Int_t m,Int_t mb){return rawStruct [s][m][mb][1];}
    Int_t getModule(Int_t s,Int_t m){return mdcSetup[s][m];}
    ClassDef(HMdcConnectedTdc,1)
};
#endif  /*!HMDCCONNECTEDTDC_H*/
