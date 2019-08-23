#ifndef HMDCGEANTNTUPLEREADER_H
#define HMDCGEANTNTUPLEREADER_H

#include "hmdcgeantreader.h"
#include "TTree.h"

class HMdcLayerGeomPar;
class HMdcGeantCell;

class HMdcGeantNtupleReader : public HMdcGeantReader {
private:
  TTree *tr[24];                        // Root trees
  struct mdc_tuple{
    Int_t idevt, nhit, mdctrk[100];
    Float_t mdcx[100][6],mdcy[100][6],mdctheta[100][6];
    Float_t mdcphi[100][6],mdctof[100][6];
  } mdc;                                //! MDC GEANT ntuple
public:
  HMdcGeantNtupleReader(void);
  ~HMdcGeantNtupleReader(void) {}
  Bool_t init(void);
  Bool_t execute(void);
  ClassDef(HMdcGeantNtupleReader,1) // MDC HGeant Ntuple reader for converted ntuple files
};

#endif /* !HMDCGEANTNTUPLEREADER_H */


