#ifndef HRPCCLUSTER_H
#define HRPCCLUSTER_H

#include "TObject.h"

class HRpcCluster : public TObject {
 protected:
	
  Float_t tof;             // Time of flight [ns]
  Float_t charge;          // Charge [pC]
  Float_t xmod;            // X coordinate in Module system [mm]
  Float_t ymod;            // Y coordinate in Module system [mm]
  Float_t zmod;            // Z coordinate in Module system [mm]
  Float_t xsec;            // X coordinate in Sector system [mm]
  Float_t ysec;            // Y coordinate in Sector system [mm]
  Float_t zsec;            // Z coordinate in Sector system [mm]

  Float_t sigma_x;         // Sigma of x   [mm]
  Float_t sigma_y;         // Sigma of y   [mm]
  Float_t sigma_z;         // Sigma of z   [mm]
  Float_t sigma_tof;       // Sigma of tof [ps]

  Int_t   address;         // Address in the category.

  Short_t detID1;          // Detector ID of the first  cell in the cluster
  Short_t detID2;          // Detector ID of the second cell in the cluster

  Short_t type;            // 1: Single hit, 2: Two-cell cluster

 public:
  HRpcCluster(void);
  ~HRpcCluster(void);

  // Functions getVariable	  
  Float_t getTof()             { return  tof;      }
  Float_t getCharge()          { return  charge;   }
  Float_t getXSec()            { return  xsec;     }
  Float_t getYSec()            { return  ysec;     }
  Float_t getZSec()            { return  zsec;     }
  Float_t getXMod()            { return  xmod;     }
  Float_t getYMod()            { return  ymod;     }
  Float_t getZMod()            { return  zmod;     }
  Float_t getXRMS()            { return  sigma_x;  }
  Float_t getYRMS()            { return  sigma_y;  }
  Float_t getZRMS()            { return  sigma_z;  }
  Float_t getTOFRMS()          { return  sigma_tof;}
  Int_t   getSector()          { return  address/1000;          }
  Int_t   getIndex()           { return  address - address/1000;}
  Int_t   getClusterType()     { return  Int_t(type);}

  Int_t   getSector1()  const  { if(detID1<0) return -1; return (detID1>>9) & 7;  }
  Int_t   getColumn1()  const  { if(detID1<0) return -1; return (detID1>>6) & 7;  }
  Int_t   getCell1()    const  { if(detID1<0) return -1; return  detID1     & 63; }

  Int_t   getSector2()  const  { if(detID2<0) return -1; return (detID2>>9) & 7;  }
  Int_t   getColumn2()  const  { if(detID2<0) return -1; return (detID2>>6) & 7;  }
  Int_t   getCell2()    const  { if(detID2<0) return -1; return  detID2     & 63; }

  Int_t   getNLocationIndex()  { return 2;         } 

  inline Int_t   getLocationIndex(Int_t i);

  // Functions setVariable	  

  void setClusterType(Short_t atype)  { type = atype; }

  inline void setDetID1(Int_t sec,Int_t col,Int_t cell);
  void setDetID1(Short_t aDetID1)                        { detID1 = aDetID1;  }
  inline void setDetID2(Int_t sec,Int_t col,Int_t cell);
  void setDetID2(Short_t aDetID2)                        { detID2 = aDetID2;  }

  inline  void setCluster(Float_t atof,Float_t acharge, Float_t axmod,Float_t aymod, Float_t azmod);
  inline  void setRSEC(Float_t axsec, Float_t aysec, Float_t azsec);
  inline  void setRMS(Float_t asigma_tof, Float_t asigma_x, Float_t asigma_y, Float_t asigma_z);
  inline  void setAddress(Int_t asector, Int_t aindex);

  ClassDef(HRpcCluster,1) //RPC cluster data class
};


inline void HRpcCluster::setCluster(Float_t atof,Float_t acharge, Float_t axmod,
				    Float_t aymod, Float_t azmod) {
  tof    = atof;
  charge = acharge;
  xmod   = axmod;
  ymod   = aymod;
  zmod   = azmod;
}

inline void HRpcCluster::setRSEC(Float_t axsec, Float_t aysec, Float_t azsec) {
  xsec   = axsec;
  ysec   = aysec;
  zsec   = azsec;
}

inline void HRpcCluster::setRMS(Float_t asigma_tof, Float_t asigma_x, Float_t asigma_y, Float_t asigma_z) {
  sigma_tof = asigma_tof;
  sigma_x   = asigma_x;
  sigma_y   = asigma_y;
  sigma_z   = asigma_z;
}

inline void HRpcCluster::setAddress(Int_t asector,Int_t aindex) {
  address = asector*1000 + aindex;
}

inline void HRpcCluster::setDetID1(Int_t sec,Int_t col,Int_t cell)      { 
  if (sec<0 || col <0 || cell<0) detID1=-1;
  else detID1 = (sec<<9) + (col<<6) + cell; 
}

inline void HRpcCluster::setDetID2(Int_t sec,Int_t col,Int_t cell)      { 
  if (sec<0 || col <0 || cell<0) detID2=-1;
  else detID2 = (sec<<9) + (col<<6) + cell; 
}
    
inline Int_t HRpcCluster::getLocationIndex(Int_t i) {
  switch (i) {
  case 0 : return getSector(); break;
  case 1 : return getIndex(); break;
  }
  return -1;
}


#endif /* !HRPCCLUSTER_H */
