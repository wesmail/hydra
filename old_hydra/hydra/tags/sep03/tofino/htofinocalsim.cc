#include "htofinocalsim.h"

// Created 26.06.00 by R. Holzmann
// Modified 05.10.00 by M. Golubeva

ClassImp(HTofinoCalSim)

    HTofinoCalSim::HTofinoCalSim(void) { 
      clear();
    }

    HTofinoCalSim::~HTofinoCalSim(void) {;}

    void HTofinoCalSim::clear(void) {
      HTofinoCal::clear();
      nHits = 0;
      for(Int_t i=0;i<MAXHITS;i++) {
	nTrack[i] = -1;
	fGeaTof[i] = -1;
      }
    }

 
