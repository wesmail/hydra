//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Last modified: 06/05/05

//_HADES_CLASS_DESCRIPTION 
/////////////////////////////////////////////////////////////////////////
//
//  HPhysicsConstants
//
//  This class contains look-up information for particle properties.  The
//  design is based on a scheme written by Marios Kargalis in his PData.h
//  class for Pluto++.  See PData.h class documentation for references.
//
//  Note: Particle Id's are reserved for 'pion' (44) and 'kaon' (48), which
//        can be either + or -. This is useful for graphical cuts on all
//        particles regardless of sign; see HCutHadronId.
//
// BEGIN_HTML<b><font color="maroon">For more information, see the <a href="http://www-hades.gsi.de/computing/phyana/">PhyAna documentation<a>.</font></b> END_HTML
//
/////////////////////////////////////////////////////////////////////////

#include <math.h>
#include "TMath.h"
#include "hphysicsconstants.h"

const Int_t nPart = 53;  // number of particles stored permanently

// particle naming convention (adopted from Pluto++)
const Char_t *cName[nPart] = {
  "dummy","g","e+", "e-","nu", "mu+", "mu-", "pi0", "pi+",
  "pi-", "K0L", "K+", "K-", "n", "p", "anti_p",
  "K0S", "eta", "Lambda", "Sigma+", "Sigma0",
  "Sigma-", "Xi0", "Xi-", "Omega", "anti_n",
  "anti_Lambda", "anti_Sigma-", "anti_Sigma0",
  "anti_Sigma+","anti_Xi0", "anti_Xi+",
  "anti_Omega+","TC", "D0", "D++", "D+", "D-", "NP11+",
  "ND13+", "NS11+", "rho0", "rho+", "rho-",
  "pion", "d", "t", "alpha", "kaon", "He3",
  "dimuon", "dilepton","w"};

Double_t fMass[nPart]={
  /* 0: dummy     */  0.0,           /* 1: Photon    */  0.0,
  /* 2: Positron  */  0.51099906,    /* 3: Electron  */  0.51099906,
  /* 4: Neutrino  */  0.0,           /* 5: mu+       */  105.658389,
  /* 6: mu-       */  105.658389,    /* 7: pi0       */  134.9764,
  /* 8: pi+       */  139.56995,     /* 9: pi-       */  139.56995,
  /*10: K0 Long_t   */  497.672,       /*11: K+        */  493.677,
  /*12: K-        */  493.677,       /*13: Neutron   */  939.56563,
  /*14: Proton    */  938.27231,     /*15: Antiproton*/  938.27231,
  /*16: K0 Short_t  */  497.672,       /*17: Eta       */  547.45,
  /*18: Lambda    */  1115.684,      /*19: Sigma+    */  1189.37,
  /*20: Sigma0    */  1192.55,       /*21: Sigma-    */  1197.436,
  /*22: Xi0       */  1314.9,        /*23: Xi-       */  1321.32,
  /*24: Omega     */  1672.45,       /*25: Antineutrn*/  939.56563,
  /*26: Antilambda*/  1115.684,      /*27: Antisigma-*/  1197.436,
  /*28: Antisigma0*/  1192.55,       /*29: Antisigma+*/  1189.37,
  /*30: Antixi0   */  1314.9,        /*31: Antixi+   */  1321.32,
  /*32: Antiomega+*/  1672.45,       /*33: TC        */  0.0,
  /*34: Delta0    */  1232,          /*35: Delta++   */  1232,
  /*36: Delta+    */  1232,          /*37: Delta-    */  1232,
  /*38: NP11+     */  1440,          /*39: ND13+     */  1520,
  /*40: NS11+     */  1535,          /*41: rho0      */  769.9,
  /*42: rho+      */  769.9,         /*43: rho-      */  769.9,
  /*44: PION      */  139.56995,     /*45: Deuteron  */  1875.613,
  /*46: Tritium   */  2809.25,       /*47: Alpha     */  3727.417,
  /*48: KAON      */  493.677,       /*49: He3       */  2809.23,
  /*50: dimuon    */  211.31678,     /*51: dilepton  */   1.022,
  /*52: omega     */  781.94
};

// Particle charge
Int_t fCharge[nPart]={
  /* 0: dummy     */  0,        /* 1: Photon    */  0,
  /* 2: Positron  */  1,        /* 3: Electron  */ -1,
  /* 4: Neutrino  */  0,        /* 5: mu+       */  1,
  /* 6: mu-       */ -1,        /* 7: pi0       */  0,
  /* 8: pi+       */  1,        /* 9: pi-       */ -1,
  /*10: K0 Long_t   */  0,        /*11: K+        */  1,
  /*12: K-        */ -1,        /*13: Neutron   */  0,
  /*14: Proton    */  1,        /*15: Antiproton*/ -1,
  /*16: K0 Short_t  */  0,        /*17: eta       */  0,
  /*18: Lambda    */  0,        /*19: Sigma+    */  1,
  /*20: Sigma0    */  0,        /*21: Sigma-    */ -1,
  /*22: Xi0       */  0,        /*23: Xi-       */ -1,
  /*24: Omega-    */ -1,        /*25: Antineutrn*/  0,
  /*26: Antilambda*/  0,        /*27: Antisigma-*/ -1,
  /*28: Antisigma0*/  0,        /*29: Antisigma+*/  1,
  /*30: Antixi0   */  0,        /*31: Antixi+   */  1,
  /*32: Antiomega+*/  1,        /*33: TC        */  0,
  /*34: Delta0    */  0,        /*35: Delta++   */  2,
  /*36: Delta+    */  1,        /*37: Delta-    */ -1,
  /*38: NP11+     */  1,        /*39: ND13+     */  1,
  /*40: NS11+     */  1,        /*41: rho0      */  0,
  /*42: rho+      */  1,        /*43: rho-      */ -1,
  /*44: PION      */  0,        /*45: Deuteron  */  1,
  /*46: Tritium   */  1,        /*47: Alpha     */  2,
  /*48: KAON      */  0,        /*49: He3       */  2,
  /*50: dimuon    */  0,        /*51: dilepton  */  0,
  /*52: omega     */  0
};


Double_t *HPhysicsConstants::PMass=(Double_t*)fMass;
Int_t *HPhysicsConstants::PCharge=(Int_t*)fCharge;
Char_t **HPhysicsConstants::PName = (Char_t**)cName;

Int_t HPhysicsConstants::pid(const Char_t *n) {
// returns integer id given a particle's name

  Int_t i;
  if (!n) return 0;
  for(i=0; i<nPart && strcmp(n,(Char_t*)cName[i]) ; i++) { }
  if (i==nPart) {
    printf("%s is not a valid particle name\n",n);
    return 0;
  }
  return i;
}

const Char_t *HPhysicsConstants::pid(const Int_t &id) {
  if (id>0 && id<nPart) return PName[id];
  else return PName[0];
}

Int_t HPhysicsConstants::charge(const Int_t &id) {
  if (id>0 && id<nPart) return PCharge[id];
  else return -100;
}

Double_t HPhysicsConstants::mass(const Int_t &id) {
  if (id>0 && id<nPart) return PMass[id];
  else return -100.;
}

ClassImp(HPhysicsConstants)
