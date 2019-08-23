// @(#)$Id: hbpidphysicsconstants.cc,v 1.2 2008-05-09 16:52:44 halo Exp $
//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Copy into Pid lib and changing the class name by Marcin Jaskula
//    15/10/02
//*-- Modified     : 24/02/03 by Marcin Jaskula
//                   new ids for fakes, get rid of TObject
//*-- Last modified: 24/02/03

//*-- Modified     : 03/11/06 by Alexander Belyaev
//                   leptonCharge, baryonCharge and strangeness are added

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
//  HBPidPhysicsConstants
//  modified HPhysicsConstants
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
// The class was copied from Dan's HPidPhysicsConstants to get it in pid library
// and do not make the user to include phyana lib into the calculations
//
////////////////////////////////////////////////////////////////////////////////

#include <math.h>
#include <TMath.h>
#include <TError.h>
#include "hbpidphysicsconstants.h"

ClassImp(HBPidPhysicsConstants)
// -----------------------------------------------------------------------------

const Int_t nPidPart = 53;  // number of particles stored permanently

// -----------------------------------------------------------------------------

// particle naming convention (adopted from Pluto++)  
const Char_t *cBPidName[nPidPart] = {
   /* 0: dummy     */  "dummy",      /* 1: Photon    */  "g",
   /* 2: Positron  */  "e+",         /* 3: Electron  */  "e-",
   /* 4: Neutrino  */  "nu",         /* 5: mu+       */  "mu+", 
   /* 6: mu-       */  "mu-",        /* 7: pi0       */  "pi0", 
   /* 8: pi+       */  "pi+",        /* 9: pi-       */  "pi-",
   /*10: K0 Long_t   */  "K0L",        /*11: K+        */  "K+",
   /*12: K-        */  "K-",         /*13: Neutron   */  "n", 
   /*14: Proton    */  "p",          /*15: Antiproton*/  "anti_p",
   /*16: K0 Short_t  */  "K0S",        /*17: Eta       */  "eta",
   /*18: Lambda    */  "Lambda",     /*19: Sigma+    */  "Sigma+",
   /*20: Sigma0    */  "Sigma0",     /*21: Sigma-    */  "Sigma-",
   /*22: Xi0       */  "Xi0",        /*23: Xi-       */  "Xi-",
   /*24: Omega     */  "Omega",      /*25: Antineutrn*/  "anti_n",
   /*26: Antilambda*/  "anti_Lambda",/*27: Antisigma-*/  "anti_Sigma-",
   /*28: Antisigma0*/  "anti_Sigma0",/*29: Antisigma+*/  "anti_Sigma+",
   /*30: Antixi0   */  "anti_Xi0",   /*31: Antixi+   */  "anti_Xi-",    // "anti_Xi-"    ???
   /*32: Antiomega+*/  "anti_Omega-",/*33: TC        */  "TC",          // "anti_Omega-" ???
   /*34: Delta0    */  "D0",         /*35: Delta++   */  "D++",
   /*36: Delta+    */  "D+",         /*37: Delta-    */  "D-", 
   /*38: NP11+     */  "NP11+",      /*39: ND13+     */  "ND13+", 
   /*40: NS11+     */  "NS11+",      /*41: rho0      */  "rho0",
   /*42: rho+      */  "rho+",       /*43: rho-      */  "rho-",
   /*44: PION      */  "pion",       /*45: Deuteron  */  "d",
   /*46: Tritium   */  "t",          /*47: Alpha     */  "alpha",
   /*48: KAON      */  "kaon",       /*49: He3       */  "He3",
   /*50: dimuon    */  "dimuon",     /*51: dilepton  */  "dilepton",
   /*52: omega     */  "w"
};

Double_t fBPidMass[nPidPart]={
  /* 0: dummy     */   0.0,        /* 1: Photon    */   0.0,
  /* 2: Positron  */   0.51099906, /* 3: Electron  */   0.51099906,
  /* 4: Neutrino  */   0.0,        /* 5: mu+       */ 105.658389,
  /* 6: mu-       */ 105.658389,   /* 7: pi0       */ 134.9764,
  /* 8: pi+       */ 139.56995,    /* 9: pi-       */ 139.56995,
  /*10: K0 Long_t   */ 497.672,      /*11: K+        */ 493.677,
  /*12: K-        */ 493.677,      /*13: Neutron   */ 939.56563,
  /*14: Proton    */ 938.27231,    /*15: Antiproton*/ 938.27231,
  /*16: K0 Short_t  */ 497.672,      /*17: Eta       */ 547.45,
  /*18: Lambda    */1115.684,      /*19: Sigma+    */1189.37,
  /*20: Sigma0    */1192.55,       /*21: Sigma-    */1197.436,
  /*22: Xi0       */1314.9,        /*23: Xi-       */1321.32,
  /*24: Omega     */1672.45,       /*25: Antineutrn*/ 939.56563,
  /*26: Antilambda*/1115.684,      /*27: Antisigma-*/1197.436,
  /*28: Antisigma0*/1192.55,       /*29: Antisigma+*/1189.37,
  /*30: Antixi0   */1314.9,        /*31: Antixi-   */1321.32, // Antixi-    ???
  /*32: Antiomega-*/1672.45,       /*33: TC        */   0.0,  // Antiomega- ???
  /*34: Delta0    */1232.0,        /*35: Delta++   */1232.0,
  /*36: Delta+    */1232.0,        /*37: Delta-    */1232.0,
  /*38: NP11+     */1440.0,        /*39: ND13+     */1520.0,
  /*40: NS11+     */1535.0,        /*41: rho0      */ 769.9,
  /*42: rho+      */ 769.9,        /*43: rho-      */ 769.9,
  /*44: PION      */ 139.56995,    /*45: Deuteron  */1875.613,
  /*46: Tritium   */2809.25,       /*47: Alpha     */3727.417,
  /*48: KAON      */ 493.677,      /*49: He3       */2809.23,
  /*50: dimuon    */ 211.31678,    /*51: dilepton  */   1.022,
  /*52: omega     */ 781.94
};

// Particle charge
Int_t fBPidCharge[nPidPart]={
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
  /*26: Antilambda*/  0,        /*27: Antisigma-*/ +1, // +1         ???
  /*28: Antisigma0*/  0,        /*29: Antisigma+*/ -1, // -1         ???
  /*30: Antixi0   */  0,        /*31: Antixi-   */  1, // Antixi-    ???
  /*32: Antiomega-*/  1,        /*33: TC        */  0, // Antiomega- ???
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

// Particle lepton charge
Int_t fPidLeptonCharge[nPidPart]={
  /* 0: dummy     */  0,        /* 1: Photon    */  0,
  /* 2: Positron  */ -1,        /* 3: Electron  */ +1,
  /* 4: Neutrino  */ +1,        /* 5: mu+       */ -1,
  /* 6: mu-       */ +1,        /* 7: pi0       */  0,
  /* 8: pi+       */  0,        /* 9: pi-       */  0,
  /*10: K0 Long_t   */  0,        /*11: K+        */  0,
  /*12: K-        */  0,        /*13: Neutron   */  0,
  /*14: Proton    */  0,        /*15: Antiproton*/  0,
  /*16: K0 Short_t  */  0,        /*17: eta       */  0,
  /*18: Lambda    */  0,        /*19: Sigma+    */  0,
  /*20: Sigma0    */  0,        /*21: Sigma-    */  0,
  /*22: Xi0       */  0,        /*23: Xi-       */  0,
  /*24: Omega-    */  0,        /*25: Antineutrn*/  0,
  /*26: Antilambda*/  0,        /*27: Antisigma-*/  0,
  /*28: Antisigma0*/  0,        /*29: Antisigma+*/  0,
  /*30: Antixi0   */  0,        /*31: Antixi-   */  0, // Antixi- ???
  /*32: Antiomega-*/  0,        /*33: TC        */  0, // Antiomega- ???
  /*34: Delta0    */  0,        /*35: Delta++   */  0,
  /*36: Delta+    */  0,        /*37: Delta-    */  0,
  /*38: NP11+     */  0,        /*39: ND13+     */  0,
  /*40: NS11+     */  0,        /*41: rho0      */  0,
  /*42: rho+      */  0,        /*43: rho-      */  0,
  /*44: PION      */  0,        /*45: Deuteron  */  0,
  /*46: Tritium   */  0,        /*47: Alpha     */  0,
  /*48: KAON      */  0,        /*49: He3       */  0,
  /*50: dimuon    */  0,        /*51: dilepton  */  0,
  /*52: omega     */  0
};

// Particle baryon charge
Int_t fPidBaryonCharge[nPidPart]={
  /* 0: dummy     */  0,        /* 1: Photon    */  0,
  /* 2: Positron  */  0,        /* 3: Electron  */  0,
  /* 4: Neutrino  */  0,        /* 5: mu+       */  0,
  /* 6: mu-       */  0,        /* 7: pi0       */  0,
  /* 8: pi+       */  0,        /* 9: pi-       */  0,
  /*10: K0 Long_t   */  0,        /*11: K+        */  0,
  /*12: K-        */  0,        /*13: Neutron   */ +1,
  /*14: Proton    */ +1,        /*15: Antiproton*/ -1,
  /*16: K0 Short_t  */  0,        /*17: eta       */  0,
  /*18: Lambda    */ +1,        /*19: Sigma+    */ +1,
  /*20: Sigma0    */ +1,        /*21: Sigma-    */ +1,
  /*22: Xi0       */ +1,        /*23: Xi-       */ +1,
  /*24: Omega-    */ +1,        /*25: Antineutrn*/ -1,
  /*26: Antilambda*/ -1,        /*27: Antisigma-*/ -1,
  /*28: Antisigma0*/ -1,        /*29: Antisigma+*/ -1,
  /*30: Antixi0   */ -1,        /*31: Antixi-   */ -1, // Antixi-    ???
  /*32: Antiomega-*/ -1,        /*33: TC        */  0, // Antiomega- ???
  /*34: Delta0    */ +1,        /*35: Delta++   */ +1,
  /*36: Delta+    */ +1,        /*37: Delta-    */ +1,
  /*38: NP11+     */ +1,        /*39: ND13+     */ +1,
  /*40: NS11+     */ +1,        /*41: rho0      */  0,
  /*42: rho+      */  0,        /*43: rho-      */  0,
  /*44: PION      */  0,        /*45: Deuteron  */ +2,
  /*46: Tritium   */ +3,        /*47: Alpha     */ +4,
  /*48: KAON      */  0,        /*49: He3       */ +3,
  /*50: dimuon    */  0,        /*51: dilepton  */  0,
  /*52: omega     */  0
};

// Particle strangeness
Int_t fPidStrangeness[nPidPart]={
  /* 0: dummy     */  0,        /* 1: Photon    */  0,
  /* 2: Positron  */  0,        /* 3: Electron  */  0,
  /* 4: Neutrino  */  0,        /* 5: mu+       */  0,
  /* 6: mu-       */  0,        /* 7: pi0       */  0,
  /* 8: pi+       */  0,        /* 9: pi-       */  0,
  /*10: K0 Long_t   */ +1,        /*11: K+        */ +1,
  /*12: K-        */ -1,        /*13: Neutron   */  0,
  /*14: Proton    */  0,        /*15: Antiproton*/  0,
  /*16: K0 Short_t  */ +1,        /*17: eta       */  0,
  /*18: Lambda    */ -1,        /*19: Sigma+    */ -1,
  /*20: Sigma0    */ -1,        /*21: Sigma-    */ -1,
  /*22: Xi0       */ -2,        /*23: Xi-       */ -2,
  /*24: Omega-    */ -3,        /*25: Antineutrn*/  0,
  /*26: Antilambda*/ +1,        /*27: Antisigma-*/ +1,
  /*28: Antisigma0*/ +1,        /*29: Antisigma+*/ +1,
  /*30: Antixi0   */ +2,        /*31: Antixi-   */ +2, // Antixi-    ???
  /*32: Antiomega-*/ +3,        /*33: TC        */  0, // Antiomega- ???
  /*34: Delta0    */  0,        /*35: Delta++   */  0,
  /*36: Delta+    */  0,        /*37: Delta-    */  0,
  /*38: NP11+     */  0,        /*39: ND13+     */  0,
  /*40: NS11+     */  0,        /*41: rho0      */  0,
  /*42: rho+      */  0,        /*43: rho-      */  0,
  /*44: PION      */  0,        /*45: Deuteron  */  0,
  /*46: Tritium   */  0,        /*47: Alpha     */  0,
  /*48: KAON      */  0,        /*49: He3       */  0,
  /*50: dimuon    */  0,        /*51: dilepton  */  0,
  /*52: omega     */  0
};

// -----------------------------------------------------------------------------

// artificial particles
const Int_t   nArtPidPart = 2;
const Char_t *cArtPidName[nArtPidPart]   = { "fake-", "fake+" };
const Int_t   fArtPidCharge[nArtPidPart]       = { -1,  1 };
const Int_t   fArtPidLeptonCharge[nArtPidPart] = { -1,  1 };
const Int_t   fArtPidBaryonCharge[nArtPidPart] = { -1,  1 };
const Int_t   fArtPidStrangeness[nArtPidPart]  = { -1,  1 };
const Int_t   fArtPidMass[nArtPidPart]         = { -1, -1 }; // why integers?

// -----------------------------------------------------------------------------

#define ARTIFICIAL_PID_OFFSET 500

// -----------------------------------------------------------------------------

// new artificial particles
const Int_t   nArtPidPartNew = 4;
const Char_t *cArtPidNameNew[nArtPidPartNew]   = { "artificial+", "artificial-",
                                                    "fake+", "fake-" };
const Int_t   fArtPidChargeNew[nArtPidPartNew]       = {  1, -1,  1, -1 };
const Int_t   fArtPidLeptonChargeNew[nArtPidPartNew] = {  1, -1,  1, -1 };
const Int_t   fArtPidBaryonChargeNew[nArtPidPartNew] = {  1, -1,  1, -1 };
const Int_t   fArtPidStrangenessNew[nArtPidPartNew]  = {  1, -1,  1, -1 };
const Int_t   fArtPidMassNew[nArtPidPartNew]         = { -1, -1, -1, -1 }; // why integers?

// -----------------------------------------------------------------------------

//ClassImp(HBPidPhysicsConstants)

// -----------------------------------------------------------------------------

Int_t HBPidPhysicsConstants::pid(const Char_t *n)
{
// Return integer id given a particle's name

Int_t i;

    if( ! n)
        return 0;

    for(i = 0; i < nPidPart && strcmp(n, (Char_t*)cBPidName[i]); i++)
        ;

    if(i < nPidPart)
        return i;

    for(i = 0; i < nArtPidPartNew && strcmp(n, (Char_t*)cArtPidNameNew[i]); i++)
        ;

    if(i < nArtPidPartNew)
        return i - nArtPidPartNew;  // negative ids

    for(i = 0; i < nArtPidPart && strcmp(n, (Char_t*)cArtPidName[i]); i++)
        ;

    if(i < nArtPidPart)
        return ARTIFICIAL_PID_OFFSET + i;

    ::Error("HbPidPhysicsConstants::pid",
                "%s is not a valid particle name\n",n);

    return 0;
}

// -----------------------------------------------------------------------------

const Char_t *HBPidPhysicsConstants::pid(Short_t pidN)
{
// Return name of the particle

    if(pidN >= ARTIFICIAL_PID_OFFSET)
    {
        if(pidN < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return cArtPidName[pidN - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pidN >= 0) && (pidN < nPidPart))
           return cBPidName[pidN];
        else
        {
            if(pidN >= -nArtPidPartNew)
                return cArtPidNameNew[nArtPidPartNew + pidN];
        }
    }

    ::Error("HBPidPhysicsConstants::pid", "No particle with id=%d", pidN);

    return NULL;
}

// -----------------------------------------------------------------------------

Int_t HBPidPhysicsConstants::charge(const Int_t pid)
{
// Return charge of the particle

    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return fArtPidCharge[pid - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
            return fBPidCharge[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return fArtPidChargeNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HBPidPhysicsConstants::charge", "No particle with id=%d", pid);

    return 0;
}

// -----------------------------------------------------------------------------

Int_t HBPidPhysicsConstants::leptonCharge(const Int_t pid)
{
// Return leptonCharge of the particle

    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return fArtPidLeptonCharge[pid - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
            return fPidLeptonCharge[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return fArtPidLeptonChargeNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HBPidPhysicsConstants::leptonCharge", "No particle with id=%d", pid);

    return 0;
}

// -----------------------------------------------------------------------------

Int_t HBPidPhysicsConstants::baryonCharge(const Int_t pid)
{
// Return baryonCharge of the particle

    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return fArtPidBaryonCharge[pid - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
            return fPidBaryonCharge[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return fArtPidBaryonChargeNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HBPidPhysicsConstants::baryonCharge", "No particle with id=%d", pid);

    return 0;
}

// -----------------------------------------------------------------------------

Int_t HBPidPhysicsConstants::strangeness(const Int_t pid)
{
// Return strangeness of the particle

    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return fArtPidStrangeness[pid - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
            return fPidStrangeness[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return fArtPidStrangenessNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HBPidPhysicsConstants::strangeness", "No particle with id=%d", pid);

    return 0;
}

// -----------------------------------------------------------------------------

Float_t HBPidPhysicsConstants::mass(const Int_t pid)
{
// Return mass of the particle
    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
           {return fArtPidMass[pid - ARTIFICIAL_PID_OFFSET]; //why integer?
           }
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
           return fBPidMass[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
               return -1.0; //fArtPidMassNew[nArtPidPartNew + pid]; //why integer?
        }
    }

    ::Error("HBPidPhysicsConstants::mass", "No particle with id=%d", pid);

    return -1.0; //-1 //why integer?
}

// -----------------------------------------------------------------------------

Bool_t HBPidPhysicsConstants::isFake(Int_t iId)
{
// checks wheater the particle defined by its id is a fake

    return ((iId >= ARTIFICIAL_PID_OFFSET)
                || (iId == fakePos()) || (iId == fakeNeg()));
}

// -----------------------------------------------------------------------------

Bool_t HBPidPhysicsConstants::isArtificial(Int_t iId)
{
// checks wheater the particle defined by its id is an artificial one

    return ((iId == artificialPos()) || (iId == artificialNeg()));
}

// -----------------------------------------------------------------------------

UInt_t HBPidPhysicsConstants::getProcess(HGeantKine *pKine, UInt_t iMask)
{
    return 1;
}
