// @(#)$Id: hpidphysicsconstants.cc,v 1.7 2008-05-09 16:08:01 halo Exp $
//*-- Author : Dan Magestro
//*-- Created: 03/09/01
//*-- Copy into Pid lib and changing the class name by Marcin Jaskula
//    15/10/02
//*-- Modified     : 24/02/03 by Marcin Jaskula
//                   new ids for fakes, get rid of TObject
//*-- Last modified: 24/02/03

//_HADES_CLASS_DESCRIPTION 
////////////////////////////////////////////////////////////////////////////////
//
//  HPidPhysicsConstants
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
#include "hpidphysicsconstants.h"

ClassImp(HPidPhysicsConstants)
// -----------------------------------------------------------------------------

const Int_t nPidPart = 53;  // number of particles stored permanently

// -----------------------------------------------------------------------------

// particle naming convention (adopted from Pluto++)
const Char_t *cPidName[nPidPart] = {
    "dummy","g","e+", "e-","nu", "mu+", "mu-", "pi0", "pi+",
    "pi-", "K0L", "K+", "K-", "n", "p", "anti_p",
    "K0S", "eta", "Lambda", "Sigma+", "Sigma0",
    "Sigma-", "Xi0", "Xi-", "Omega", "anti_n",
    "anti_Lambda", "anti_Sigma-", "anti_Sigma0",
    "anti_Sigma+","anti_Xi0", "anti_Xi+",
    "anti_Omega+","TC", "D0", "D++", "D+", "D-", "NP11+",
    "ND13+", "NS11+", "rho0", "rho+", "rho-",
    "pion", "d", "t", "alpha", "kaon", "He3",
    "dimuon", "dilepton","w"
};

Double_t fPidMass[nPidPart]={
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
Int_t fPidCharge[nPidPart]={
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

// -----------------------------------------------------------------------------

// artificial particles
const Int_t   nArtPidPart = 2;
const Char_t *cArtPidName[nArtPidPart]   = { "fake-", "fake+" };
const Int_t   fArtPidCharge[nArtPidPart] = { -1,  1 };
const Int_t   fArtPidMass[nArtPidPart]   = { -1, -1 };

// -----------------------------------------------------------------------------

#define ARTIFICIAL_PID_OFFSET 500

// -----------------------------------------------------------------------------

// new artificial particles
const Int_t   nArtPidPartNew = 4;
const Char_t *cArtPidNameNew[nArtPidPartNew]   = { "artificial+", "artificial-",
                                                    "fake+", "fake-" };
const Int_t   fArtPidChargeNew[nArtPidPartNew] = {  1, -1,  1, -1 };
const Int_t   fArtPidMassNew[nArtPidPartNew]   = { -1, -1, -1, -1 };

// -----------------------------------------------------------------------------

//ClassImp(HPidPhysicsConstants)

// -----------------------------------------------------------------------------

Int_t HPidPhysicsConstants::pid(const Char_t *n)
{
// Return integer id given a particle's name

Int_t i;

    if( ! n)
        return 0;

    for(i = 0; i < nPidPart && strcmp(n, (Char_t*)cPidName[i]); i++)
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

    ::Error("HPidPhysicsConstants::pid",
                "%s is not a valid particle name\n",n);

    return 0;
}

// -----------------------------------------------------------------------------

const Char_t *HPidPhysicsConstants::pid(Short_t pid)
{
// Return name of the particle

    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return cArtPidName[pid - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
            return cPidName[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return cArtPidNameNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HPidPhysicsConstants::pid", "No particle with id=%d", pid);

    return NULL;
}

// -----------------------------------------------------------------------------

Int_t HPidPhysicsConstants::charge(const Int_t pid)
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
            return fPidCharge[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return fArtPidChargeNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HPidPhysicsConstants::charge", "No particle with id=%d", pid);

    return 0;
}

// -----------------------------------------------------------------------------

Float_t HPidPhysicsConstants::mass(const Int_t pid)
{
// Return mass of the particle

    if(pid >= ARTIFICIAL_PID_OFFSET)
    {
        if(pid < ARTIFICIAL_PID_OFFSET + nArtPidPart)
            return fArtPidMass[pid - ARTIFICIAL_PID_OFFSET];
    }
    else
    {
        if((pid >= 0) && (pid < nPidPart))
            return fPidMass[pid];
        else
        {
            if(pid >= -nArtPidPartNew)
                return fArtPidMassNew[nArtPidPartNew + pid];
        }
    }

    ::Error("HPidPhysicsConstants::mass", "No particle with id=%d", pid);

    return -1;
}

// -----------------------------------------------------------------------------

Bool_t HPidPhysicsConstants::isFake(Int_t iId)
{
// checks wheater the particle defined by its id is a fake

    return ((iId >= ARTIFICIAL_PID_OFFSET)
                || (iId == fakePos()) || (iId == fakeNeg()));
}

// -----------------------------------------------------------------------------

Bool_t HPidPhysicsConstants::isArtificial(Int_t iId)
{
// checks wheater the particle defined by its id is an artificial one

    return ((iId == artificialPos()) || (iId == artificialNeg()));
}

// -----------------------------------------------------------------------------

UInt_t HPidPhysicsConstants::getProcess(HGeantKine *pKine, UInt_t iMask)
{
    return 1;
}
