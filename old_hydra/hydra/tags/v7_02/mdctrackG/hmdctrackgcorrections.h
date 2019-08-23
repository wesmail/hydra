#ifndef HMDCTRACKGCORRECTIONS_H
#define HMDCTRACKGCORRECTIONS_H
#include "TString.h"
#include "TObject.h"
#include "TNamed.h"

//CORRECTION PARAMETERS FOR MOMENTUM RECONSTRUCTION

class HMdcTrackGCorrections:public TNamed
{
friend class HMdcTrackGSpline;
public:
Double_t phiI[16];
Double_t tetaI[34];
Double_t precI[60];
Double_t corrE[34][16][60];
Double_t corrP[34][16][60];
Double_t corrE1[34][16][60];
Double_t corrP1[34][16][60];

public:
  HMdcTrackGCorrections(const char* name="",const char* title="");
  ~HMdcTrackGCorrections();
  void init(TString,TString,TString,TString);
  void clear();
  Double_t getphiI(Int_t );
  ClassDef(HMdcTrackGCorrections,2)

};


#endif
