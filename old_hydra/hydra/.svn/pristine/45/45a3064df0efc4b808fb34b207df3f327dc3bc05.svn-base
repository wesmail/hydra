#ifndef HHYPBLJVPIDLIST_H
#define HHYPBLJVPIDLIST_H

#include "TObject.h"
#include "hcategory.h" 
#include "hgeantkine.h"
#include "hbpidphysicsconstants.h"

  enum {max_t = 6, max_h = 10};
  enum {max_particles = 68, nameLength = 6};
   struct tParticle
    {char    name[nameLength];
     Float_t mass;
     Int_t   charge;
     Int_t   leptonCharge;
     Int_t   baryonCharge;
     Int_t   strange;
    };
    
   struct tPhysProp
    {Int_t particles;
     tParticle P[max_particles];
    };   
   enum {nulla = 0, minima = 1, media = 2, maxima = 3, paranoja = 4};
   enum tPrntLvl         {prnt,noprnt};
   
class HHypBljvPidList : public TObject //HReconstructor 
 {
  private:
      
//   Double_t pi,grad_rad;
//   Double_t gradus,secunda; // in raianus // ,minuta
   
  public:
   HHypBljvPidList ();
  ~HHypBljvPidList (void);
  
   Bool_t init (void);
   void     fillPhysCnst (tPhysProp* Phys, FILE* otp);
   void     prntPhys     (tPhysProp* Phys, FILE* otp);
   void     typePhys     (tPhysProp* Phys           );
//   void     makePhysChargedAndNeutral
//             (tPhysProp* Phys, tPhysProp* PhysCharged, tPhysProp* PhysNeutral);
   void     getPrLvl (Int_t* prLvl, FILE* otp);
   Int_t    nmToIndex (const Char_t* nomen, tPhysProp* Phys);
//   void     vectorPlusVector  (Int_t dim,  Double_t *V1, Double_t *V2, Double_t *V);
//   void     printVector       (FILE* otp, Int_t dim, Double_t *P);
//   Double_t sqr  (Double_t value);
//   Double_t cube (Double_t value);
//   void     vectorMul         (Int_t dim,  Double_t *A,  Double_t *B, Double_t *V);
   ClassDef(HHypBljvPidList,0)
 };

#endif
