#include "hhypbljvpidlist.h"
#include <TMath.h>
#include <TFile.h> 

ClassImp (HHypBljvPidList)

void HHypBljvPidList::fillPhysCnst (tPhysProp* Phys, FILE* otp)
 {Int_t    i;
  Char_t     sysPrtName[20];
  Float_t m; //Double_t m;
  Bool_t   endFlag;
  //printf ("HHypBljvPidList::fillPhysCnst:\n");
  endFlag = false;
  i = 0;
  while (!endFlag)
   {i++; 
    m = HBPidPhysicsConstants::mass(i);
    if (m >= 0)
     {Phys->P[i].mass         = m/1000.0; // GeV/c2 
      Phys->P[i].charge       = HBPidPhysicsConstants::charge      (i);
      Phys->P[i].leptonCharge = HBPidPhysicsConstants::leptonCharge(i);
      Phys->P[i].baryonCharge = HBPidPhysicsConstants::baryonCharge(i);
      Phys->P[i].strange      = HBPidPhysicsConstants::strangeness (i);
      sprintf (sysPrtName,"%s",HBPidPhysicsConstants::pid(i));
      if       (strcoll(sysPrtName,"anti_p"     ) == 0) sprintf (Phys->P[i].name,"%s","~p"   );
       else if (strcoll(sysPrtName,"Lambda"     ) == 0) sprintf (Phys->P[i].name,"%s","Lamb" );
       else if (strcoll(sysPrtName,"Sigma+"     ) == 0) sprintf (Phys->P[i].name,"%s","Sig+" );
       else if (strcoll(sysPrtName,"Sigma0"     ) == 0) sprintf (Phys->P[i].name,"%s","Sig0" );
       else if (strcoll(sysPrtName,"Sigma-"     ) == 0) sprintf (Phys->P[i].name,"%s","Sig-" );
       else if (strcoll(sysPrtName,"Omega"      ) == 0) sprintf (Phys->P[i].name,"%s","Omg-" );
       else if (strcoll(sysPrtName,"anti_n"     ) == 0) sprintf (Phys->P[i].name,"%s","~n"   );
       else if (strcoll(sysPrtName,"anti_Lambda") == 0) sprintf (Phys->P[i].name,"%s","~Lamb");
       else if (strcoll(sysPrtName,"anti_Sigma-") == 0) sprintf (Phys->P[i].name,"%s","~Sig-");
       else if (strcoll(sysPrtName,"anti_Sigma0") == 0) sprintf (Phys->P[i].name,"%s","~Sig0");
       else if (strcoll(sysPrtName,"anti_Sigma+") == 0) sprintf (Phys->P[i].name,"%s","~Sig+");
       else if (strcoll(sysPrtName,"anti_Xi0"   ) == 0) sprintf (Phys->P[i].name,"%s","~Xi0" );
       else if (strcoll(sysPrtName,"anti_Xi-"   ) == 0) sprintf (Phys->P[i].name,"%s","~Xi-" );
       else if (strcoll(sysPrtName,"anti_Omega-") == 0) sprintf (Phys->P[i].name,"%s","~Omg-");
       else if (strcoll(sysPrtName,"dimuon"     ) == 0) sprintf (Phys->P[i].name,"%s","dimu" );
       else if (strcoll(sysPrtName,"dilepton"   ) == 0) sprintf (Phys->P[i].name,"%s","dilep");
       else                                             sprintf (Phys->P[i].name,"%s",sysPrtName);
     }
      else {i--; endFlag = true;}
   }
  Phys->particles = i;
 }

// void HHypBljvPidList::makePhysChargedAndNeutral 
//     (tPhysProp* Phys, tPhysProp* PhysCharged,tPhysProp* PhysNeutral)
// {Int_t i,j,k;
//  PhysCharged->particles = 0; // dummy particle with index 0
//  PhysNeutral->particles = 0; // dummy particle with index 0
//  for (i = 1; i <= Phys->particles; i++)
//    if (Phys->P[i].charge != 0)
//     {PhysCharged->particles++;
//      j = PhysCharged->particles;
//      sprintf (PhysCharged->P[j].name,"%s",Phys->P[i].name);
//      PhysCharged->P[j].mass         = Phys->P[i].mass;
//      PhysCharged->P[j].charge       = Phys->P[i].charge;
//      PhysCharged->P[j].leptonCharge = Phys->P[i].leptonCharge;
//      PhysCharged->P[j].baryonCharge = Phys->P[i].baryonCharge;
//      PhysCharged->P[j].strange      = Phys->P[i].strange;
//     }
//     else
//     {PhysNeutral->particles++;
//      k = PhysNeutral->particles;
//      sprintf (PhysNeutral->P[k].name,"%s",Phys->P[i].name);
//      PhysNeutral->P[k].mass         = Phys->P[i].mass;
//      PhysNeutral->P[k].charge       = Phys->P[i].charge;
//      PhysNeutral->P[k].leptonCharge = Phys->P[i].leptonCharge;
//      PhysNeutral->P[k].baryonCharge = Phys->P[i].baryonCharge;
//      PhysNeutral->P[k].strange      = Phys->P[i].strange;
//     }
// }

void HHypBljvPidList::getPrLvl (Int_t* prLvl, FILE* otp)
 {Bool_t gotten;
  Int_t t;
  printf ("Enter print level:\n");
  printf ("nulla    %i (general report)\n",nulla);
  printf ("minima   %i (+ event loop)\n",minima);
  printf ("media    %i (+ particle combination loop)\n",media);
  printf ("maxima   %i (+ iteratio loop)\n",maxima);
  printf ("paranoja %i (+ minimization step watch)\n",paranoja);
  gotten = false;
  while (!gotten)
   {scanf  ("%i",&t);
    if ( (nulla <= t) && (t <= paranoja+1) ) gotten = true;
   }
  *prLvl = t; 
   printf (    "print level %i\n",*prLvl); 
  fprintf (otp,"print level %i\n",*prLvl); 
 }
  
void HHypBljvPidList::prntPhys (tPhysProp* Phys, FILE* otp)
 {Int_t n,l,i;
  fprintf (otp,"\nParticle property list\n");
  fprintf (otp,"## name    mass    charge lepton baryon  S\n");
  for (n = 1; n < Phys->particles+1; n++)
   {l = nameLength - strlen(Phys->P[n].name);
    fprintf (otp,"%2i %s",n,Phys->P[n].name);
    for (i = 0; i < l; i++) fprintf (otp," ");
    fprintf (otp,"%9.6f %5i %5i %5i %5i\n",Phys->P[n].mass,Phys->P[n].charge,
             Phys->P[n].leptonCharge,Phys->P[n].baryonCharge,Phys->P[n].strange);
   }
  fprintf (otp,"\n"); 
 }

void HHypBljvPidList::typePhys (tPhysProp* Phys)
 {Int_t n,l,i;
  printf ("\n## name    mass    charge lepton baryon  S\n");
  for (n = 1; n < Phys->particles+1; n++)
   {l = nameLength - strlen(Phys->P[n].name);
    printf ("%2i %s",n,Phys->P[n].name);
    for (i = 0; i < l; i++) printf (" ");
    printf ("%9.6f %5i %5i %5i %5i\n",Phys->P[n].mass,Phys->P[n].charge,
             Phys->P[n].leptonCharge,Phys->P[n].baryonCharge,Phys->P[n].strange);
   }
  printf ("\n"); 
 }

Int_t HHypBljvPidList::nmToIndex (Char_t* nomen, tPhysProp* Phys)
 {Int_t  i;
  Bool_t found;
  i = 0;
  found = false;
  //printf ("nomen '%s'\n",nomen);
  while (!found && (i < Phys->particles) )
   {i++;
    //printf ("index %i name '%s'\n",i,Phys->P[i].name);
    if (strcoll(Phys->P[i].name,nomen) == 0) found = true;
   }
  if (found)
   {//printf ("nomen '%s' -> index %i\n",nomen,i); 
   }
   else
    {i = 0;
     printf ("nmToIndex: unexpected particle name '%s'\n",nomen); 
//     abort();
    } 
  return i;
 }
 
HHypBljvPidList::HHypBljvPidList () 
 {//printf ("bLib.Construktor (empty):\n");
  // initia(); 
  //printf ("bLib.End of Construktor\n");
 }
    
HHypBljvPidList::~HHypBljvPidList () 
 {//printf ("Destruktor:\n");
  //if (iterMdcSeg) delete iterMdcSeg;
  //printf ("end of destruktor\n");
 }

Bool_t HHypBljvPidList::init(void) 
 {
  //printf (    "bLib.init: ");
//   pi = TMath::Pi(); printf ("pi %f  ",pi);
//   grad_rad = 180/pi;
//   gradus   = pi/180; 
  //minuta   = gradus/60; 
//   secunda  = gradus/3600;
  //printf ("end of bLib.init\n");
  return kTRUE;
 }
 
// void HHypBljvPidList::vectorMul (Int_t dim, Double_t *A, Double_t *B, Double_t *V)
//  {if (dim != 3) 
//    {printf ("dimension %i for vector multiplication != 3",dim); abort();}
//    else
//     {V[0] =  (A[1]*B[2] - A[2]*B[1]);
//      V[1] = -(A[0]*B[2] - A[2]*B[0]);
//      V[2] =  (A[0]*B[1] - A[1]*B[0]);
//     }
//  }
 
// void HHypBljvPidList::vectorPlusVector (Int_t dim, Double_t *V1, Double_t *V2, Double_t *V)
// {Int_t i;
//  for (i = 0; i < dim; i++) V[i] = V1[i] + V2[i];
// }
// void HHypBljvPidList::printVector (FILE* otp, Int_t dim, Double_t *P)
//  {Int_t i;
//   for (i = 0; i < dim; i++)
//    {if ( (0 < fabs(P[i])) && (fabs(P[i]) < 1.0e-6) ) 
//           fprintf (otp," %12.2e",P[i]);
//      else fprintf (otp," %12.6f",P[i]);
//    } 
//  }
 
// Double_t HHypBljvPidList::sqr (Double_t value)
// {return value*value;} 
  
// Double_t HHypBljvPidList::cube(Double_t value)
// {return value*value*value;} 

