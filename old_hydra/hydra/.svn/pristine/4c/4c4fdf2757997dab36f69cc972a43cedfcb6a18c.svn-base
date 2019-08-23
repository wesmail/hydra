//*-- Authors:
// ....
// Alexander Belyaev

//_HADES_CLASS_DESCRIPTION
////////////////////////////////////////////////////////////////////////
//
// HHypMultiChannelFiller
//
// Please add some statements here
//
////////////////////////////////////////////////////////////////////////

#include "hhypmultichannelfiller.h"
//======================================
#include "hruntimedb.h"
#include "hiterator.h"
//#include "hcategory.h"
//#include "heventheader.h"
//#include "hbpidphysicsconstants.h" // <--- Ingo: hbpdiphysicsconstants is used
                                     //      not   hpdiphysicsconstants
                                     // So, because leptonCharge, baryonCharge and strangness are needed
#include "hbasetrack.h"
// #include "hsymmat.h"
//======================================
using namespace std;

ClassImp(HHypMultiChannelFiller)

//=======================================

Double_t HHypMultiChannelFiller::sqr (Double_t x)
{return x*x;
}

void HHypMultiChannelFiller::prologue ()
{char    fileName[150];
 sprintf (fileName,"hypMChfiller.txt");
 if ( (otp = fopen (fileName, "wt")) == NULL )
  {fprintf (stderr,"'%s' open file error \n",fileName); abort ();}
 time (&tempus);
 fprintf (otp,"Multi channel filler 2007. %s\n",ctime(&tempus));
  printf (    "Multi channel filler 2007. %s",ctime(&tempus));
}

void HHypMultiChannelFiller::printParticlePropertyTable (FILE* otp) //, Int_t nParticles)
{Int_t i;
 Bool_t endOfTable;
 Char_t nomen[nameLength];
 fprintf (otp,"##   name  mass     charge lepton baryon S\n");
//  for (i = 1; i <= nParticles; i++)
  i = 1;
  endOfTable = kFALSE;
  while (!endOfTable)
   if (HBPidPhysicsConstants::mass(i) >= 0)
    {
     if      (strcoll(HBPidPhysicsConstants::pid(i),"anti_p"     ) == 0) sprintf (nomen,"%s","~p");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"Lambda"     ) == 0) sprintf (nomen,"%s","Lamb");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"Sigma+"     ) == 0) sprintf (nomen,"%s","Sig+");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"Sigma0"     ) == 0) sprintf (nomen,"%s","Sig0");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"Sigma-"     ) == 0) sprintf (nomen,"%s","Sig-");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"Omega"      ) == 0) sprintf (nomen,"%s","Omg-");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_n"     ) == 0) sprintf (nomen,"%s","~n");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Lambda") == 0) sprintf (nomen,"%s","~Lamb");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Sigma-") == 0) sprintf (nomen,"%s","~Sig-");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Sigma0") == 0) sprintf (nomen,"%s","~Sig0");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Sigma+") == 0) sprintf (nomen,"%s","~Sig+");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Xi0"   ) == 0) sprintf (nomen,"%s","~Xi0");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Xi-"   ) == 0) sprintf (nomen,"%s","~Xi-");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"anti_Omega-") == 0) sprintf (nomen,"%s","~Omg-");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"dimuon"     ) == 0) sprintf (nomen,"%s","dimu");
     else if (strcoll(HBPidPhysicsConstants::pid(i),"dilepton"   ) == 0) sprintf (nomen,"%s","dile");
     else sprintf (nomen,"%s",HBPidPhysicsConstants::pid(i));
     fprintf (otp,"%2i %6s %8.6f %5i %5i %5i %5i\n",i,nomen,
                      HBPidPhysicsConstants::mass(i)/1000.0,
                      HBPidPhysicsConstants::charge(i),
                      HBPidPhysicsConstants::leptonCharge(i),
                      HBPidPhysicsConstants::baryonCharge(i),
                      HBPidPhysicsConstants::strangeness(i)   );
     i++;
    }
    else endOfTable = kTRUE;
}

void HHypMultiChannelFiller::setBeamPID (Int_t pid)
{//printf ("HHypMultiChannelFiller::setBeamPID %i\n",pid);
   beamIndex = pid;
}

void HHypMultiChannelFiller::setTargetPID (Int_t pid)
{//printf ("HHypMultiChannelFiller::setTargetPID %i\n",pid);
  trgtIndex = pid;
}

void HHypMultiChannelFiller::setBeam (TLorentzVector* beamVector)
{Double_t p,m;
// printf ("HHypMultiChannelFiller::setBeam \n");
 m = HBPidPhysicsConstants::mass(beamIndex)/1000.0;
 p = (beamVector->P())/1000.0;
 maxMeasuredMomentum = p*(1.0 + 0.12*3); // 0.12 ~ mom resolution
 Ek = sqrt(sqr(p) + sqr(m)) - m;
 printf ("p %f m %f Ek %f\n",p,m,Ek);
}

Bool_t HHypMultiChannelFiller::intro () //-----------------------------------------------------------------------
{
 Char_t    ch;
 Bool_t  gotten;
 Float_t delBeamDp;
 Int_t   i; //,nParticles;
 Float_t regDist;
 Bool_t  allChargedParticlesAreRegistered,result;

// printf ("intro:\n");
 result = kTRUE;

 pi   = TMath::Pi();
 pi2  = pi*2;
 pi_2 = pi/2;
 gradDivRad = 180.0/pi;
 thetaMin = 15.0/gradDivRad; // 0.157; // ~ 9 gradus

 minMom = 1.0e-4; // GeV/c
 maxVelo = 1.5; //1.2; // // maximum for calculated speed of light

 theLimErr  = 0.1; //0.004;// Rad
 phiLimErr  = 0.2; //0.008;// Rad

 pNmb  = 0;
 nNmb  = 0;
 tNmb  = 0;
 chNmb = 0;

 // masses in Gev/c2
 dm             = 0.0005; // to difference masses in standard table; so (neutron.mass - proton.mass) ~ 0.001294
 mBetweenMuAndE = 0.01;   // to difference muones and electrones

// nParticles = 32;
 printParticlePropertyTable (otp); //, nParticles);
//-------------------------------------
 pi0        = HBPidPhysicsConstants::pid ("pi0");
 proton     = HBPidPhysicsConstants::pid ("p" );
 electron   = HBPidPhysicsConstants::pid ("e-");
 positron   = HBPidPhysicsConstants::pid ("e+");
 muMinus    = HBPidPhysicsConstants::pid ("mu-");
 muPlus     = HBPidPhysicsConstants::pid ("mu+");
 neutrino   = HBPidPhysicsConstants::pid ("nu");
 piMinus    = HBPidPhysicsConstants::pid ("pi-");
 piPlus     = HBPidPhysicsConstants::pid ("pi+");
//-------------------------------------

//  beamIndex = proton;
//  trgtIndex = proton;
//  Ek        =  2.2;    // ideal kinetic energy of beam particle, GeV
 //printf ("Enter beam particle ideal kinetic energy:\n");
 //scanf  ("%e",&Ek);
 printf ("beam pid %i, trgt pid %i, Ek %f\n",beamIndex,trgtIndex,Ek);

 delBeamDp =  0.0003; // relative beam momentum measure error; from Jrslmv, for beam delBeamDp = 0.5%
 regDist   = 50.0;    // (sm) ~ distance from vertex to first registrating device (mdc0)
 allChargedParticlesAreRegistered = kFALSE; // any charged particle (and neutral of course) may be not registered

 Float_t mb,Eb;
 mb = HBPidPhysicsConstants::mass(beamIndex)/1000.0;
 Eb = Ek + mb;
 beamMomIdeal = sqrt(sqr(Eb) - sqr(mb));
 beamDp       = beamMomIdeal*delBeamDp;

 fprintf (otp,"Collision particles:\n");
 fprintf (otp," beam   particle '%s'\n",HBPidPhysicsConstants::pid(beamIndex));
 fprintf (otp," target particle '%s'\n",HBPidPhysicsConstants::pid(trgtIndex));
 fprintf (otp," beam momentum   %8.6f (GeV/c)\n",beamMomIdeal);

 trckInputNo        = 0; // input tracks
 overflowTrNo       = 0;
 trNo               = 0; // number of all accepted tracks in accepted events
 evInputNo          = 0; // input events
 badRecsNmb         = 0;

 delPhiTrNo               = 0;
 delThetaTrNo             = 0;
 phiOutOfAcceptanceTrNo   = 0;
 thetaOutOfAcceptanceTrNo = 0;
 noPolarityTrNo           = 0;
 wrongVeloTrNo            = 0;
 wrongMomTrNo             = 0;

 evNo               = 0; // number of filled events
 noCombinations     = 0;
 proFit             = 0; // events processed by lagFit

 printf ("Enter number of 1-st event to be printed:\n");
// scanf ("%i",&evFirstNo);
evFirstNo=0;
 printf ("Enter number of last event to be printed:\n");
// scanf ("%i",&evLastNo);
evLastNo=0;
 if (evFirstNo > evLastNo) {evFirstNo =   0; evLastNo  = 100;}
  printf (    "1-st printed event # %6i\n",evFirstNo);
 fprintf (otp,"1-st printed event # %6i\n",evFirstNo);
  printf (    "Last printed event # %6i\n",evLastNo);
 fprintf (otp,"Last printed event # %6i\n",evLastNo);

 tableContentType = pressed;
 ch = 'p';
 //printf ("Table content type: pressed (p) or not (n)?\n");
 gotten = false;
 while (!gotten)
  {//scanf ("%c",&ch);
   if      (ch == 'p') {gotten = true; tableContentType =    pressed;}
   else if (ch == 'n') {gotten = true; tableContentType = notPressed;}
  }
 if (chLM.makeChannelList (max_t,beamIndex,Ek,trgtIndex,regDist,
        allChargedParticlesAreRegistered,chNmbMax,
        &chNmb,chCodeList,chLineList,&pNmb,posPID,&nNmb,negPID,&tNmb,totPID)  )
  {fprintf (otp,"\nChannel list:\n");
   for (i = 1; i <= chNmb; i++) fprintf (otp,"%3i: %18.0f %s\n",i,chCodeList[i],chLineList[i].Data());

//   chLM.printInUseParticles (otp);

//    fprintf (otp,"%2i positive particles in use: ",pNmb);
//    for (i = 0; i < pNmb; i++) fprintf (otp," %s",HBPidPhysicsConstants::pid(posPID[i]));
//    fprintf (otp,"\n");
//    fprintf (otp,"%2i negative particles in use: ",nNmb);
//    for (i = 0; i < nNmb; i++) fprintf (otp," %s",HBPidPhysicsConstants::pid(negPID[i]));
//    fprintf (otp,"\n");
//    fprintf (otp,"%2i total    particles in use: ",tNmb);
//    for (i = 0; i < tNmb; i++) fprintf (otp," %s",HBPidPhysicsConstants::pid(totPID[i]));
//    fprintf (otp,"\n");

 if ((pNmb > max_h) || (nNmb > max_h)) // max_h see in hHypBljvPidList
  {fprintf (otp,"Number of positive charged particles in use > max hypotesa number (%i)\n",max_h);
    printf (    "Number of positive charged particles in use > max hypotesa number (%i)\n",max_h);
   fclose (otp);
   result = kFALSE;
  }


  }
  else
  {fprintf (otp,"number of channels produced (%i) > limit (%i)\n",chNmb,chNmbMax);
    printf (    "number of channels produced (%i) > limit (%i)\n",chNmb,chNmbMax);
   fclose (otp);
   result = kFALSE;
  }
// printf ("end of intro\n");
 return result;
}

//------------------------------------------------------------------

void HHypMultiChannelFiller::getECMS ()
{Float_t mb,pb,Eb,mt,pt,Et,ps,ms,gamma,beta,EbCMS,EtCMS,beamMomLim;
 mb = HBPidPhysicsConstants::mass(beamIndex)/1000.0;
 mt = HBPidPhysicsConstants::mass(trgtIndex)/1000.0;
 pt = 0;
 pb = beamMomIdeal;
 Es = sqrt(mb*mb + pb*pb) + mt;
 fprintf (otp,"sys: beamMom in LAB %10.7f\n",pb);
 fprintf (otp,"     beam   mass    %10.7f\n",mb);
 fprintf (otp,"     target mass    %10.7f\n",mt);
 fprintf (otp,"     sysE    in LAB %10.7f\n",Es);
 beamMomLim = pb + beamDp*3.0;
 pb = beamMomLim;
 Eb = sqrt(sqr(mb) + sqr(pb));
 Et = sqrt(sqr(mt) + sqr(pt));
 ps = pb;
 EsLim = Eb + Et;
 ms = sqrt(sqr(EsLim) - sqr(ps));
 gamma = EsLim/ms;
 beta  = ps/EsLim;
 EbCMS = gamma*(Eb - beta*pb);
 EtCMS = gamma*(Et - beta*pt);
 ECMS  = EbCMS + EtCMS;
 fprintf (otp,"\nLorentz energy conversion:\n");
 fprintf (otp,"mb %f pb %f Eb %f\n",mb,pb,Eb);
 fprintf (otp,"mt %f pt %f Et %f\n",mt,pt,Et);
 fprintf (otp,"ms %f ps %f Es %f\n",ms,ps,EsLim);
 fprintf (otp,"gamma %f beta %f\n",gamma,beta);
 fprintf (otp,"Eb %f Et %f E %f\n",EbCMS,EtCMS,ECMS);
 fprintf (otp,"Mean system energy %f (GeV)\n",Es);
}

void HHypMultiChannelFiller::getSysPar ()
{Int_t nPosMin,nNegMin;
 getECMS ();
 chargeIn  = HBPidPhysicsConstants::charge(beamIndex) + HBPidPhysicsConstants::charge(trgtIndex);
 if      (chargeIn > 0) {nPosMin = chargeIn; nNegMin =  0;       }
 else if (chargeIn < 0) {nPosMin = 0;        nNegMin = -chargeIn;}
 else   /*chargeIn = 0*/{nPosMin = 1;        nNegMin =  1;       }

// maximumTracks = maxT;
// if (chargeIn%2 != maxT%2) maximumTracks--;

 leptonIn  = HBPidPhysicsConstants::leptonCharge(beamIndex) + HBPidPhysicsConstants::leptonCharge(trgtIndex);
 electronIn = 0;
 muonIn     = 0;
 baryonIn  = HBPidPhysicsConstants::baryonCharge(beamIndex) + HBPidPhysicsConstants::baryonCharge(trgtIndex);
 fprintf (otp,"\n");
 fprintf (otp,"Collision: %s %s ->\n",
     HBPidPhysicsConstants::pid(beamIndex),HBPidPhysicsConstants::pid(trgtIndex));
 fprintf (otp,"System electricity charge   %i\n",chargeIn);
 fprintf (otp,"System lepton      charge   %i\n",leptonIn);
 fprintf (otp,"System baryon      charge   %i\n",baryonIn);
 fprintf (otp,"Maximum multiplicity        %i\n",max_t);
 fprintf (otp," Normal minimum of positive particles %i\n",nPosMin);
 fprintf (otp," Normal minimum of negative particles %i\n",nNegMin);
 fprintf (otp,"\n");
}

//======================================

 HHypMultiChannelFiller::HHypMultiChannelFiller(const Text_t *inputDir,const Text_t *inputFileName):
     HReconstructor(inputDir,inputFileName)
{//printf ("HHypMultiChannelFiller.HReconstructor\n");
  nPlus  = 0;
  nMinus = 0;
  missing_array  = 0;
  pid_real_array = 0;
  tid_real_array = 0; // <--- Ingo
  numberOfParticles = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
  inDir       = (TString)inputDir;
  inFile      = (TString)inputFileName + ".root";
  inFileName  = (TString)inputFileName;
  prologue ();
}

//-----------------------------------------------------------------
HHypMultiChannelFiller::HHypMultiChannelFiller()
{//printf ("HHypMultiChannelFiller.Constructor");
  nPlus  = 0;
  nMinus = 0;
  missing_array  = 0;
  pid_real_array = 0;
  tid_real_array = 0; // <--- Ingo
  numberOfParticles = 0;
  particleArrayPlus.Set(0);
  particleArrayMinus.Set(0);
  prologue ();
}

//-----------------------------------------------------------------
HHypMultiChannelFiller::~HHypMultiChannelFiller(void)
{ finalize ();
}

//-----------------------------------------------------------------
Bool_t HHypMultiChannelFiller::SetExitList(Int_t e_list)
{ exitList = e_list;
  return kTRUE;
}

//-----------------------------------------------------------------
Bool_t HHypMultiChannelFiller::IngoInit()
{ // Purpose:
  // 1.) Calculate the number of possible combinations // made earlier, in getPidTrackCandEvent()
  // 2.) Prepare the pid table with all permutations is created.
  // 3.) init HypComb, HypList and PidTrackCand Containers

//  generateNumberOfCombinations(); // Ingo: this may be done in execute, after getPidTrackCandEvent();
//  create_pid_table(); // this may be done in execute, after multiChannelCombinations ();

  //---------- Initialization of HHypComb Container -----------------------
  m_pContCatComb = gHades->getCurrentEvent()->getCategory(catHypComb);
  if (!m_pContCatComb)
   {m_pContCatComb = HHypComb::buildLinearCat("HHypComb");
    if (!m_pContCatComb)     return kFALSE;
    // else
    gHades->getCurrentEvent()->addCategory(catHypComb, m_pContCatComb,"HypComb");
   }
  //---------- Initialization of HHypList Container -----------------------
  m_pContCatList = gHades->getCurrentEvent()->getCategory(catHypList);
  if (!m_pContCatList)
   {m_pContCatList = HHypList::buildLinearCat("HHypList");

    if (!m_pContCatList)  return kFALSE;
    // else
    gHades->getCurrentEvent()->addCategory(catHypList, m_pContCatList,"HypList");
   }
  //---------- Initialization of HPidTrackCand Container -----------------------
  m_pContItPart  = NULL;        // Iterator // see init
  m_pContCatPart = NULL;        // Category
  if ((m_pContCatPart = gHades->getCurrentEvent()->getCategory(catPidTrackCand)) == NULL)
   {Error("init", "Cannot get catPidTrackCand category");
    return kFALSE;
   }
  m_pContItPart = (HIterator *) m_pContCatPart->MakeIterator();
  //-----------------------------------------------------------------------
 return kTRUE;
}

Bool_t HHypMultiChannelFiller::init()
{ Bool_t result;
//  printf ("HHypMultiChannelFiller.init:\n");
  result = kTRUE;
  if (!IngoInit ()) {Error("init","The IngoInit fake"); result = kFALSE;}
//  printf ("end of init:\n");
  return result;
}                               // End Bool_t HHypMultiChannelFiller::init()

Bool_t HHypMultiChannelFiller::reinit ()
{Bool_t result;
//  printf ("HHypMultiChannelFiller.reinit:\n");
 result = intro ();
 if (result) getSysPar ();
//  printf ("end of reinit:\n");
 return result;
}

//==============================================

void HHypMultiChannelFiller::printMatrix (FILE* otp, const Char_t* name, Int_t nLin, Int_t nCol, Double_t *P)
 {Int_t i,j,n;
  fprintf (otp,"%s (%i,%i)\n",name,nLin,nCol);
  for (i = 0; i < nLin; i++)
   {for (j = 0; j < nCol; j++)
     {n = i*nCol + j;
      if (P[n] == 0) fprintf (otp,"  0       ");
       else if ( (fabs(P[n]) >= 100.0) || (fabs(P[n]) < 1.0e-6) ) fprintf (otp,"%10.2e",P[n]);
             else fprintf (otp,"%10.6f",P[n]);
     }
    fprintf (otp,"\n");
   }
 }

void HHypMultiChannelFiller::print_cov (Double_t* cov)
{Int_t i;
  fprintf (otp,"           cov (3,3): ");
  for (i = 0; i < 9; i++)
   if (cov[i] == 0) fprintf (otp,"  0       ");
     else
      if ((1.0e-6 < fabs(cov[i])) && (fabs(cov[i]) <= 100.0))
         fprintf (otp,"%10.6f",cov[i]);
    else fprintf (otp,"%10.2e",cov[i]);
  fprintf (otp,"\n");
}

void HHypMultiChannelFiller::storeHypothesis (Int_t tNo, Int_t pNmb)
{T[tNo].nh++;
 T[tNo].pid[T[tNo].nh] = pNmb;
 if (printFlag)
   fprintf (otp," %s ",HBPidPhysicsConstants::pid(pNmb));
}

void HHypMultiChannelFiller::storeSignHypotheses (Int_t Num, Int_t* PID, Int_t tNo,
    Bool_t isRICH, Float_t massEstim, Int_t electron_or_positron)
{Int_t pNo;
 T[tNo].nh = 0; //till filling track bank with fit banks
 for (pNo = 0; pNo < Num; pNo++)
  {if (HBPidPhysicsConstants::leptonCharge(PID[pNo]) == 0) // hadroni
     storeHypothesis (tNo,PID[pNo]);
    else
     if (PID[pNo] == electron_or_positron)
      {if (isRICH) storeHypothesis (tNo,PID[pNo]);
      }
       else // muon
        if (massEstim > mBetweenMuAndE) // mass seems as muon mass
             storeHypothesis (tNo,PID[pNo]);
  }
}


void HHypMultiChannelFiller::getPidTrackCandEvent (Bool_t* overflow)
{Bool_t    isRICH,cutFlag;
 Int_t     sign,trID,system; // i,j,
 Double_t  beta,massEstim;
 Float_t   p,theta,phi,theErr,phiErr,RKChiSquare; // ,angErr
//  HSymMat   covPnt;
//  Double_t  covFull[6*6];
//  Double_t  cov    [3*3];
//  printf ("getPidTrackCandEvent:\n");
 HBaseTrack*    b;
 counterMinus = 0;
 counterPlus  = 0;
 counter      = 0;
//------------
 HPidTrackCand   *pidPart = NULL;
//  printf ("pidPart = NULL\n");
 m_pContItPart->Reset();
//  printf ("m_pContItPart->Reset()\n");
Int_t idxCounter=-1;
 while (((pidPart = (HPidTrackCand *) m_pContItPart->Next()) != NULL) && (counter < max_t)) //10))
  {
	idxCounter++;// Workaround for getting index of HPidTrackCand
//    printf (" track %i\n",counter);
   system = (int)pidPart->getHitData()->getSystem();
   beta   = pidPart->getTrackData()->getBeta(4);
   isRICH = pidPart->getHitData()->getFlagRICH();
   p      = pidPart->getTrackData()->getMomenta(ALG_RUNGEKUTTA)/1000.0; // GeV/c

   phi         = (pidPart->getHitData()->fMdcPhi)/gradDivRad;
   theta       = (pidPart->getHitData()->fMdcTheta)/gradDivRad;

//    covPnt = pidPart->getTrackData()->getCovariance(4);
//    if (evFirstNo)
//     {for (i = 0; i < 6; i++)
//        for (j = 0; j < 6; j++) covFull[i*6+j] =  covPnt.getElement(i,j);
//      printMatrix (otp,"covFull",6,6,covFull);
//     }
//    for (i = 2; i < 5; i++)
//     for (j = 2; j < 5; j++) cov[(i-2)*3+(j-2)] = covPnt.getElement(i,j);
//    if (evFirstNo) print_cov (cov);
//    theErr = sqrt(cov[4]);
//    phiErr = sqrt(cov[8]);
//    angErr = phiErr*sin(theta);

   b=(HBaseTrack *)(pidPart->getTrackData()->getBaseTrack(pidPart->getTrackData()->getBestMomAlg()));
   theErr=b->getErrTheta();
   phiErr=b->getErrPhi();

   RKChiSquare =  pidPart->getTrackData()->fRKChiSquare;

   massEstim = 0; // to prevent warning
    if ((1.0e-9 < beta) && (beta <= 1.0)) massEstim = p*sqrt(1.0 - sqr(beta))/beta; // GeV/c2
   trID = idxCounter;//pidPart->getIndex();
   sign = pidPart->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
   Charge[counter] = sign;
   if      (Charge[counter] ==  1) counterPlus++;
   else if (Charge[counter] == -1) counterMinus++;
   counter++;
   if (printFlag)
    {fprintf (otp," nt %i",nt+1);
     fprintf (otp," sign %2i mom %6.3f theta %9.6f+/-%9.6f phi %9.6f+/-%9.6f ",sign,p,theta,theErr,phi,phiErr);
     fprintf (otp," beta %6.3f",beta);
     if ((1.0e-16 < beta) && (beta <= 1.0)) fprintf (otp," ~mass %9.6f",massEstim);
      else fprintf (otp,"                ");
     if (isRICH) fprintf (otp," is RICH");
      else fprintf (otp,"        ");
    }

   cutFlag = kFALSE;
   if ((p < minMom) || (maxMeasuredMomentum < p)) // beamMom + delM*p
    {cutFlag = kTRUE;
     wrongMomTrNo++;
      if (printFlag) fprintf (otp,"  cut: mom %f out of (0, %f)",p,maxMeasuredMomentum);
    }
    else
     if ((beta <= 0) || (maxVelo< beta))  //due to calculation errors maxVelo may be > 1
      {cutFlag = kTRUE;
       wrongVeloTrNo++;
       if (printFlag) fprintf (otp,"  cut: beta %f out of (0, %3.1f]",beta,maxVelo);
      }
      else
       if (sign == 0)
        {cutFlag = kTRUE;
         noPolarityTrNo++;
          if (printFlag) fprintf (otp,"  cut: polarity == 0");
        }
        else
         if (RKChiSquare > 10000) // ANAR
          {cutFlag = kTRUE;
           // ???
           if (printFlag) fprintf (otp,"  cut: PidTrackData->fRKChiSquare > 10000");
          }
          else
           if ((theta < thetaMin) || (pi_2 < theta))
            {cutFlag = kTRUE;
             thetaOutOfAcceptanceTrNo++;
             if (printFlag) fprintf (otp,"  cut: theta %f out of acceptance (%f, %f)",theta,thetaMin,pi_2);
            }
            else
             if ((phi < 0) || (pi2 < phi))
              {cutFlag = kTRUE;
               phiOutOfAcceptanceTrNo++;
                if (printFlag) fprintf (otp,"  cut: phi %f out of [0, 2*pi)",phi);
              }
              else
               if (theLimErr < theErr)
                {cutFlag = kTRUE;
                 delThetaTrNo++;
                  if (printFlag) fprintf (otp,"  cut: del(theta) %f > lim = %f",theErr,theLimErr);
                }
                else
                 if (phiLimErr < phiErr)
                  {cutFlag = kTRUE;
                   delPhiTrNo++;
                    if (printFlag) fprintf (otp,"  cut: del(phi) %f > lim = %f",phiErr,phiLimErr);
                  }

   if (printFlag) fprintf (otp,"\n");
//   if (!cutFlag)
    if (nt < max_t) //maximumTracks)
     {nt++;
      T[nt].chargeSign = sign;
      T[nt].isRICH     = isRICH;
      T[nt].trID       = trID;
      if (printFlag) fprintf (otp,"  hyps: ");
      if      (sign > 0) storeSignHypotheses (pNmb,posPID,nt,isRICH,massEstim,positron);
      else if (sign < 0) storeSignHypotheses (nNmb,negPID,nt,isRICH,massEstim,electron);
      else {fprintf (otp,"storeTrack: charged particle sign = 0\n"); fclose(otp); abort ();}
      if (printFlag) fprintf (otp,"\n");
     }
     else
      {*overflow = true; overflowTrNo++;
       if (printFlag)
         fprintf (otp,"  cut: number of track hypothesa > max_t = %i\n",max_t);
//       fprintf (otp,"  cut: number of track hypothesa > maximumTracks = %i\n",maximumTracks);
      }
//    if ((minMom < p) && (p < maxMeasuredMomentum)) // beamMom + delM*p
//     {
//      if ((0 < beta) && (beta <= maxVelo))  //due to calculation errors maxVelo may be > 1
//       {
//        if (sign != 0)
//         {
//          if (RKChiSquare <= 10000) // ANAR
//           {
//            if ((thetaMin < theta) && (theta < pi_2))
//             {
//              if ((0 <= phi) && (phi < pi2))
//               {
//                if (theErr < theLimErr)
//                 {
//                  if (phiErr < phiLimErr)
// //                  if (angErr < phiLimErr)
//                   {
//
//                    if (nt < max_t) //maximumTracks)
//                     {nt++;
//                      T[nt].chargeSign = sign;
//                      T[nt].isRICH     = isRICH;
//                      T[nt].trID       = trID;
//                      if (printFlag) fprintf (otp,"  hyps: ");
//                      if      (sign > 0) storeSignHypotheses (pNmb,posPID,nt,isRICH,massEstim,positron);
//                      else if (sign < 0) storeSignHypotheses (nNmb,negPID,nt,isRICH,massEstim,electron);
//                      else {fprintf (otp,"storeTrack: charged particle sign = 0\n"); fclose(otp); abort ();}
//                      if (printFlag) fprintf (otp,"\n");
//                     }
//                     else
//                      {*overflow = true; overflowTrNo++;
//                       if (printFlag)
//                         fprintf (otp,"  cut: number of track hypothesa > max_t = %i\n",max_t);
////                       fprintf (otp,"  cut: number of track hypothesa > maximumTracks = %i\n",maximumTracks);
//                      }
//
//                   }
//                   else
//                    {delPhiTrNo++;
//                     if (printFlag)
//                       fprintf (otp,"  cut: del(phi) %f > lim = %f\n",phiErr,phiLimErr);
//                    }
//                 }
//                 else
//                  {delThetaTrNo++;
//                   if (printFlag)
//                     fprintf (otp,"  cut: del(theta) %f > lim = %f\n",theErr,theLimErr);
//                  }
//               }
//               else
//                {phiOutOfAcceptanceTrNo++;
//                 if (printFlag)
//                   fprintf (otp,"  cut: phi %f out of [0, 2*pi)\n",phi);
//                }
//             }
//             else
//              {thetaOutOfAcceptanceTrNo++;
//               if (printFlag)
//                 fprintf (otp,"  cut: theta %f out of acceptance (%f, %f)\n",theta,thetaMin,pi_2);
//              }
//           }
//           else
//            { // ???
//             if (printFlag)
//              fprintf (otp,"  cut: PidTrackData->fRKChiSquare > 10000 \n");
//            }
//         }
//         else
//          {noPolarityTrNo++;
//           if (printFlag)
//             fprintf (otp,"  cut: polarity == 0\n");
//          }
//       }
//       else
//        {wrongVeloTrNo++;
//         if (printFlag)
//          fprintf (otp,"  cut: beta %f out of (0, %3.1f]\n",beta,maxVelo);
//        }
//     }
//     else
//      {wrongMomTrNo++;
//       if (printFlag)
//        fprintf (otp,"  cut: mom %f out of (0, %f)\n",p,maxMeasuredMomentum);
//      }
   trckInputNo++;
  }
 if ((nt <= 0) && (printFlag) )
  fprintf (otp," empty event\n");
}

//==============================================
Int_t HHypMultiChannelFiller::execute()
{ Int_t   i,result=ERROR_IDX;
  Bool_t  overflow;
  if (evInputNo == 0)
   {fprintf (otp,"\nExecute\n");
//     fprintf (otp," (MAX_USER_VALUES        = %4i)\n",MAX_USER_VALUES);
//     fprintf (otp," (ERROR_IDX              = %4i)\n",ERROR_IDX);
//     fprintf (otp," (FILLER_VALID_PIDTRACKS = %4i)\n",FILLER_VALID_PIDTRACKS);
//     fprintf (otp," (FILLER_MISSING_PID     = %4i)\n",FILLER_MISSING_PID);
    printf (    "\nExecute: \n");
   }

//   HEvent* ev = gHades->getCurrentEvent();
//   evHdr = ev->getHeader();
//   eventRunNumber = evHdr->getEventRunNumber();
//   eventSeqNumber = evHdr->getEventSeqNumber();

  printFlag = ((evFirstNo      <= evInputNo) && (evInputNo      <= evLastNo))
//      ||      ((eventRunNumber == 364651847) &&
//         ( 713067 - 10 <= eventSeqNumber) && (eventSeqNumber <= 713067 + 10))
                              ;
//  printFlag = kFALSE;
  if (printFlag)
  {
    fprintf (otp,"\n=== event === %d (%d)\n",evInputNo,proFit);
//     fprintf (otp,"\n=== event === %d (%d)  runNmb %i seqNmb %i\n",
//           evInputNo,proFit,eventRunNumber,eventSeqNumber);
//     printf (      "=== event === %d (%d)\n",evInputNo,proFit);
  }

  nt           = 0;
  overflow = false;
  getPidTrackCandEvent  (&overflow); // getPidTrackCandEvent (&overflow); //
//   printf ("event gotten\n");
  if (!overflow  && (nt > 0))
   {numberOfParticles    = nt; // <-- Ingo: maybe 'numOfTracks' name is more correct then numberOfParticles?
    if (printFlag)
     {fprintf (otp," accepted: %i-th event of %i tracks; ",evNo+1,nt);
      fprintf (otp,"trID/sign/nHyp[/RICH]: ");
      for (i = 1; i <= nt; i++)
       {fprintf (otp,"   %i/",T[i].trID);
        if (T[i].chargeSign > 0) fprintf (otp,"+");
        fprintf (otp,"%i/%i",T[i].chargeSign,T[i].nh);
        if (T[i].isRICH) fprintf (otp,"/R");
       }
      fprintf (otp,"\n");
     }
    multiChannelCombinations (); // <<======================
    if (numberOfCombinations <= 0)
     {noCombinations++;
      if (printFlag)
        fprintf (otp," There are no possible particle combination\n");
     }
     else
     {proFit++;
      // numberOfCombinations is defined here (and now it is > 0)
//       if (nt >= max_t) printf (    "execute (%i): nt %i nc %i\n",evInputNo,nt,numberOfCombinations);
      create_pid_table();     // <---------------------------
      result = IngoExecute(); // <---------------------------
     }
    trNo = trNo + nt;// full number of all tracks in all events
    evNo++;          // full number of all processed events
   }
   else
    {badRecsNmb++;
     if (printFlag)
      {if (overflow) fprintf (otp," Track overflow");
        else         fprintf (otp," No tracks");
       fprintf (otp,", event %7i",evInputNo);
       fprintf (otp,"\n");
      }
    }
  evInputNo++;
//   if (printFlag)
//     printf ("=== event end === %d\n",evInputNo);
  return result;
}

//================================================
Int_t HHypMultiChannelFiller::IngoExecute ()
{ Int_t result = ERROR_IDX;
  HLocation locDummy;
  Int_t index_comb = 0;
  Int_t index_list = 0;
  Int_t numpidtracks[MAX_USER_VALUES]; // = 500 (too small?...) see hypinfodef.h
//========= Counting Number of ch. positive and negative particles END =====
      // Number of particles in event must be number of particles INCLUDING fakes
//       fprintf (otp,"counterPlus %i nPlus %i counterMinus %i nMinus %i\n",counterPlus,nPlus,counterMinus,nMinus);
//      if (counterPlus == nPlus && counterMinus == nMinus)
//       {
      //=========================== Create HHypComb Container ================
      HHypComb       *hypcomb = NULL;
      hypcomb = (HHypComb *) m_pContCatComb->getNewSlot(locDummy, &index_comb);
      if (hypcomb != 0)
       {hypcomb = new(hypcomb) HHypComb(numberOfCombinations, numberOfParticles);
        //========================== Fill PID Table & Idx Table in HHypComb =================
        fill_pid_table(hypcomb);    // just copy from pid_real_array & from tid_real_array
//       //========================== Fill Idx Table in HHypComb =================
//       fill_pid_idx(hypcomb);                                               //  <---- Ingo!
      //========================== Fill HHypComb: fprobcomb ===================
        if (fill_pid_fprob(hypcomb, numpidtracks))
         {//=========================== Create HHypList Container ==================
          HHypList       *hyplist = NULL;
          hyplist = (HHypList *) m_pContCatList->getNewSlot(locDummy, &index_list);
          if (hyplist != 0)
           {
				hyplist = new(hyplist) HHypList(index_comb);
				result=index_list;
            //=========================== Create HHypList Container END ==============
            hyplist->setListId(exitList);
//          if (printFlag)
//           {
//            printf ("IngoExecute: for... (Icomb < MAX_USER_VALUES) %i\n",numberOfCombinations);
//            printf ("MAX_USER_VALUES %i FILLER_VALID_PIDTRACKS %i FILLER_MISSING_PID %i\n",
//                MAX_USER_VALUES,FILLER_VALID_PIDTRACKS,FILLER_MISSING_PID);
//           }
            for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++)
             {//this is to keep PIDTRACK INFO
              if (Icomb < MAX_USER_VALUES)
                hyplist->setUserValue(FILLER_VALID_PIDTRACKS, numpidtracks[Icomb],Icomb);
//            if ( (printFlag) && (Icomb >= 1000))
//             printf ("FILLER_MISSING_PID %i missing_array[Icomb] %i Icomb %i\n",
//                      FILLER_MISSING_PID,missing_array[Icomb],Icomb);
              hyplist->setUserValue(FILLER_MISSING_PID,missing_array[Icomb],Icomb);
             }
           }
           else {result = ERROR_IDX; printf ("hyplist == 0\n");}
         }
         else {result = ERROR_IDX; printf ("!fill_pid_fprob\n");}
       }
       else {result = ERROR_IDX; printf ("hypcomb == 0\n");}

//       }                             //if( counterPlus==nPlus && counterMinus==nMinus )
//       else
//        {
//         result ERROR_IDX;
//        }                             //if( counterPlus==nPlus && counterMinus==nMinus )

  if ((result != 0) && (printFlag) )
   fprintf (otp,"IngoExecute: result %i\n",result);
//   printf ("IngoExecute: result %i\n",result);
  return result;
}


//==================================================
void HHypMultiChannelFiller::sortTrackSubSets (Int_t nt, Int_t prong, Int_t* trackNo, Bool_t* overflow)
{Int_t i,j,k;
 Bool_t carryMade;
 //switch to next track subset:
 i = prong; j = 0; carryMade = false;
 while (!carryMade)
  if (trackNo[i] >= nt - j)
   if (i > 1) {i--; j++;}
    else    {carryMade = true; *overflow = true;}
   else
    {trackNo[i]++;
     for (k = i+1; k <= prong; k++) trackNo[k] = trackNo[k-1] + 1;
     carryMade = true;
    }
}

Bool_t HHypMultiChannelFiller::existSuchChannel (Double_t chC)
{Bool_t existFlag,made;
 Int_t nCh,nMin,nMax;
//  fprintf (otp,"chNmb %i chC %18.0f\n",chNmb,chC);
 existFlag = kFALSE;
 made      = kFALSE;
 nMin =       0;
 nMax = chNmb-1;
 while (!made) // binary search
 {nCh = nMin + (nMax - nMin)/2;
//   fprintf (otp,"chCodeList[%i] %18.0f\n",nCh,chCodeList[nCh]);
  if (chC == chCodeList[nCh])
    {made = kTRUE; existFlag = kTRUE;}
   else if (chC < chCodeList[nCh])
           if (nCh < nMax) nMax = nCh;
            else           made = kTRUE;
         else
           if (nMin < nCh) nMin = nCh;
            else if (nMin < nMax) nMin = nMax;
                  else            made = kTRUE;
 }
 return existFlag;
}

void HHypMultiChannelFiller::printComb
    (tChain* pChain, Int_t prong, Int_t* trackNo,Int_t* partIndex, Int_t missa)
{Int_t    i,j;
 fprintf (otp,"%4i ",numberOfCombinations);
 for (j   = 1; j   <= nt; j++  )
  {fprintf (otp," ");
   if (pChain->pidCharged[j] != 0)
    {fprintf (otp,"%2i %s",pChain->tidCharged[j],HBPidPhysicsConstants::pid(pChain->pidCharged[j]));
     for (i = 0; i < nameLength-int(strlen(HBPidPhysicsConstants::pid(pChain->pidCharged[j]))); i++) fprintf (otp," ");
    }
    else fprintf (otp,"________ ");
  }
 if (missa != 0)
  {if (tableContentType == pressed) fprintf (otp,"  ");
   fprintf (otp," %s",HBPidPhysicsConstants::pid(missa));
   if (HBPidPhysicsConstants::charge(missa) != 0) fprintf (otp,"'");
  }
 fprintf (otp,"\n");
}

void HHypMultiChannelFiller::getCombination (Int_t prong, Int_t* trackNo,Int_t* partIndex, Int_t missa )
{tChain* pChainCurrent;
 Int_t    tNo,j;
 numberOfCombinations++;
 pChainCurrent = new tChain;
 pChainCurrent->next = NULL;
 for (j = 1; j <= nt; j++)
  {pChainCurrent->pidCharged[j] =  0; // default fake pid
   // see hHypKine: if (HPidPhysicsConstants::pid(nPid) != NULL)
   pChainCurrent->tidCharged[j] = -1; // default fake track number
   // see hHypKine: if ((0 <= nTrck) && (nTrck <= nPart))
  }
 switch (tableContentType)
  {case notPressed:
    for (tNo = 1; tNo <= prong; tNo++)
     {pChainCurrent->pidCharged[trackNo[tNo]] = partIndex[tNo];
      pChainCurrent->tidCharged[trackNo[tNo]] = T[trackNo[tNo]].trID;
     }
    break;
   case pressed:
    j = 0;
    for (tNo = 1; tNo <= prong; tNo++)
     if (HBPidPhysicsConstants::charge(partIndex[tNo]) > 0)
      {j++;
       pChainCurrent->pidCharged[j] = partIndex[tNo];
       pChainCurrent->tidCharged[j] = T[trackNo[tNo]].trID;
      }
    for (tNo = 1; tNo <= prong; tNo++)
     if (HBPidPhysicsConstants::charge(partIndex[tNo]) < 0)
      {j++;
       pChainCurrent->pidCharged[j] = partIndex[tNo];
       pChainCurrent->tidCharged[j] = T[trackNo[tNo]].trID;
      }
    break;
  }
 if (missa != 0) pChainCurrent->pidMissing = missa;
  else           pChainCurrent->pidMissing = 0; // a channel without missing particle
 if (printFlag)
                                       printComb (pChainCurrent,prong,trackNo,partIndex,missa);
 if (pChainBegin == NULL) pChainBegin     = pChainCurrent;
  else                    pChainEnd->next = pChainCurrent;
 pChainEnd   = pChainCurrent;
}

// Int_t HHypMultiChannelFiller::getChNo (Double_t chCode)
// {Int_t i,result;
//  Bool_t found;
//  result = -1;
//  i      =  0;
//  found  = false;
//  while (!found && (i < chNmb))
//   {//printf ("%2i chCodeList %18.0f\n",i,chCodeList[i]);
//    if (chCodeList[i] == chCode) {found = true; result = i;}
//    i++;
//   }
//  return result;
// }

void HHypMultiChannelFiller::sortParticleCombinations (Int_t prong, Int_t* trackNo, Int_t chargeOut)
{Bool_t allHypsChecked,convergated,needSearchNextComb;
 Int_t tNo,hypIndex[max_t+1],partIndex[max_t+1];
 Int_t pID,missa,missaCnt;
 Int_t digit[max_t+1];
 Double_t chC,chCPlusMissa;
 Int_t    leptCharge;
 Int_t baryonOut,leptonOut,delCharge,delBaryon,delLepton,baryonSum,leptonSum,chargeSum;
 Int_t electronOut,muonOut,delElectron,delMuon,electronSum,muonSum;
 Float_t massOut,massSum;
 for (tNo = 1; tNo <= prong; tNo++) hypIndex[tNo] = 1;
 convergated    = false;
 allHypsChecked = false;
 while (!allHypsChecked)
  {  baryonOut = 0;
     leptonOut = 0;
   electronOut = 0;
       muonOut = 0;
   massOut     = 0.0;
   for (tNo = 1; tNo <= prong; tNo++)
    {
     pID = T[trackNo[tNo]].pid[hypIndex[tNo]];
     baryonOut  = baryonOut + HBPidPhysicsConstants::baryonCharge(pID);
     leptCharge =             HBPidPhysicsConstants::leptonCharge(pID);
     leptonOut  = leptonOut + leptCharge;
     if (leptCharge > 0)
      {if      (pID == electron) electronOut++;
       else if (pID == muMinus )     muonOut++;
       // else neutrino
      }
     else if (leptCharge < 0)
      {if      (pID == positron) electronOut--;
       else if (pID == muPlus  )     muonOut--;
       // else ~neutrino
      }
     massOut   = massOut   + HBPidPhysicsConstants::mass(pID)/1000.0;
     partIndex[tNo] = pID;
     digit    [tNo] = pID;
    }
   delBaryon   = baryonIn   - baryonOut;
   delLepton   = leptonIn   - leptonOut;
   delElectron = electronIn - electronOut;
   delMuon     = muonIn     - muonOut;
   delCharge   = chargeIn   - chargeOut;
   // single (~)baryon or (~)lepton may fly forward, out of acceptance:
   if (abs(delBaryon) <= 1)
    {if ((abs(delLepton) <= 1) && (delLepton == (delElectron + delMuon)))
      {if (massOut < ECMS)
        {sort(prong,digit);
         chC = chLM.getChannelCode (prong,digit,0);
         if (prong > 1)
          if (delBaryon == 0)
            if ((delLepton == 0) && (delElectron == 0) && (delMuon == 0))
              if (delCharge == 0)
                if (existSuchChannel(chC)) // in channel list
                     getCombination (prong,trackNo,partIndex,0); // <--------
//                    else fprintf (otp,"charged:   %18.0f\n",chC);
           missaCnt = 0;
           while (missaCnt < tNmb)
            {missa = totPID[missaCnt];
//              fprintf (otp,"missa %s: ",HBPidPhysicsConstants::pid(missa));
             if ((HBPidPhysicsConstants::charge(missa) == delCharge) && (missa != neutrino))
              {baryonSum  = baryonOut +  HBPidPhysicsConstants::baryonCharge(missa);
               leptCharge =              HBPidPhysicsConstants::leptonCharge(missa);
               leptonSum = leptonOut + leptCharge;
               electronSum = electronOut;
               muonSum     = muonOut;
               if (leptCharge > 0)
                {if      (missa == electron) electronSum++;
                 else if (missa == muMinus ) muonSum++;
                // else neutrino
                }
                else if (leptCharge < 0)
                {if      (missa == positron) electronSum--;
                 else if (missa == muPlus  ) muonSum--;
                 // else ~neutrino
                }
               chargeSum   = chargeOut + HBPidPhysicsConstants::charge(missa);
               massSum     = massOut   + HBPidPhysicsConstants::mass(missa)/1000.0;
               delBaryon   = baryonIn   - baryonSum;
               delLepton   = leptonIn   - leptonSum;
               delElectron = electronIn - electronSum;
               delMuon     = muonIn     - muonSum;
               if (delBaryon == 0)
                {if (delLepton == 0)
                  {if (chargeIn  == chargeSum)
                    {if ((delElectron == 0) && (delMuon == 0))
                      {if (massSum < ECMS)
                        {chCPlusMissa = chC + missa;
                         if (existSuchChannel(chCPlusMissa)) // in channel list
                          {getCombination (prong,trackNo,partIndex,missa); // <------
                          }
//                         else fprintf (otp,"neutral: %2i %18.0f\n",missa,chCPlusMissa);
                        }
                      }
                    }
                  }
                }
              }
             missaCnt++;
            }
        }
      }
    }
   //switch to next particle combination:
   tNo = prong;
   needSearchNextComb = true;
   while (needSearchNextComb && (tNo > 0))
    if (hypIndex[tNo] < T[trackNo[tNo]].nh)
         {hypIndex[tNo]++; needSearchNextComb = false;}
     else
      if (tNo > 1) {hypIndex[tNo] = 1; tNo--;} // carry
       else {needSearchNextComb = false; allHypsChecked = true;}
  }
}

void HHypMultiChannelFiller::multiChannelCombinations ()
{Int_t prong; // number of tracks in track subset
 Int_t i;     // number of track into subset
 Int_t chargeOut,delCharge;
 Int_t trackNo[max_t+1];
 Bool_t overflow;
 numberOfCombinations   = 0;
 if (printFlag)
  fprintf (otp," Combinations to be fitted:\n");
 pChainBegin = NULL;
 pChainEnd   = NULL;
 for (prong = 1; prong <= nt; prong++)
  {for (i = 1; i <= prong; i++) trackNo[i] = i; // all trackNo[i] should be different
   overflow = false;
   while (!overflow)
    {chargeOut = 0;
     for (i = 1; i <= prong; i++) chargeOut = chargeOut + T[trackNo[i]].chargeSign;
     delCharge = chargeIn - chargeOut;
     // single charged particle may fly forward, out of acceptance:
     if (abs(delCharge) <= 1) sortParticleCombinations (prong,trackNo,chargeOut);
     sortTrackSubSets (nt,prong,trackNo,&overflow);
    }
  }
}

void HHypMultiChannelFiller::sort (Int_t n, Int_t* a)
{Int_t i,j,t;
 for (i = n-1;i >= 1; i--)
   for (j = 1; j <= i; j++)
     if (a[j] > a[j+1]) {t = a[j]; a[j] = a[j+1]; a[j+1] = t;}
}

void HHypMultiChannelFiller::IngoFinalize ()
{
  if (pid_real_array)
   {         // if init was called this is ALWAYS true
    for (Int_t i = 0; i < numberOfCombinations; i++)
     {
      delete[]pid_real_array[i];
      delete[]tid_real_array[i]; // <---- Ingo
     }
    delete[]pid_real_array;
    delete[]tid_real_array; // <---- Ingo
   }
  if (missing_array)
    delete missing_array;
}

Bool_t HHypMultiChannelFiller::finalize()
{
  fprintf (otp,"\n====== Finalize =======\n");
  IngoFinalize ();
  report();
  fclose (otp);
  return kTRUE;
}                               // End HHypMultiChannelFiller::finalize()

//============================================
void HHypMultiChannelFiller::report ()
{
 fprintf (otp,"General channel filler report:\n");
  printf (    "General channel filler report:\n");
 fprintf (otp,"%9i 100.000 input tracks\n",trckInputNo);
 fprintf (otp,"%9i %7.3f overflow\n",overflowTrNo,overflowTrNo*100.0/trckInputNo);

 fprintf (otp,"%9i %7.3f moment out of (%f, %f) GeV/c\n",wrongMomTrNo,wrongMomTrNo*100.0/trckInputNo,minMom,maxMeasuredMomentum);
 fprintf (otp,"%9i %7.3f beta   out of (%f, %f)\n",wrongVeloTrNo,wrongVeloTrNo*100.0/trckInputNo,0.0,maxVelo);
 fprintf (otp,"%9i %7.3f no polarity\n",noPolarityTrNo,noPolarityTrNo*100.0/trckInputNo);
 fprintf (otp,"%9i %7.3f theta  out of (%f, %f) Rad\n",thetaOutOfAcceptanceTrNo,thetaOutOfAcceptanceTrNo*100.0/trckInputNo,thetaMin,pi_2);
 fprintf (otp,"%9i %7.3f phi    out of [0, 2*pi) Rad\n",phiOutOfAcceptanceTrNo,phiOutOfAcceptanceTrNo*100.0/trckInputNo);
 fprintf (otp,"%9i %7.3f delTheta > %f Rad\n",delThetaTrNo,delThetaTrNo*100.0/trckInputNo,theLimErr);
 fprintf (otp,"%9i %7.3f delPhi   > %f Rad\n",delPhiTrNo,delPhiTrNo*100.0/trckInputNo,phiLimErr);

 fprintf (otp,"%9i %7.3f tracks accepted in accepted events\n",trNo,trNo*100.0/trckInputNo);
 fprintf (otp,"\n");
 fprintf (otp,"%9i 100.000 input events\n",evInputNo);
  printf (    "%9i input events\n",evInputNo);
 if (evInputNo > 0)
  {fprintf (otp,"%9i %7.3f empty/overflowed events\n",badRecsNmb,badRecsNmb*100.0/evInputNo);
   fprintf (otp,"%9i %7.3f accepted events\n",evNo,evNo*100.0/evInputNo);
   fprintf (otp,"%9i %7.3f no combinations\n",noCombinations,noCombinations*100.0/evInputNo);
   fprintf (otp,"%9i %7.3f pro fit\n",proFit,proFit*100.0/evInputNo);

    printf ("%9i empty/overflowed events\n",badRecsNmb);
    printf ("%9i accepted events\n",evNo);
    printf ("%9i no combinations\n",noCombinations);
    printf ("%9i pro fit\n",proFit);
  }
 time (&tempus);
 fprintf (otp,"End. %s\n",ctime(&tempus));
  printf (    "End. %s\n",ctime(&tempus));
}


//=============================================
// -----------------------------------------------------------------------
// ----------       Private methods --------------------------------------
// -----------------------------------------------------------------------

void HHypMultiChannelFiller::create_pid_table(void)
{Int_t i,j;
 tChain* pChainTemp;
 tChain* pChainCurrent;
  // This function create all possible combinations
  // For each combination, wich is one line in the final pid
  // table, the array of negative and postive particles have to be set
  // up. In addition we need one (=exactly one) particle which will be
  // the missing particle

  // Input: nPlus and nMinus (Set from outside)
  // Beam energy -> ToDo
  if (printFlag)
     fprintf (otp,"numberOfCombinations %i numberOfParticles (tracks) %i\n",
              numberOfCombinations,numberOfParticles);
  pid_real_array = new Int_t *[numberOfCombinations];
  tid_real_array = new Int_t *[numberOfCombinations]; //  <--- Ingo!
  for (i = 0; i < numberOfCombinations; i++)
   {
    pid_real_array[i] = new Int_t[numberOfParticles];
    tid_real_array[i] = new Int_t[numberOfParticles]; //  <--- Ingo!
   }
  missing_array = new Int_t[numberOfCombinations];

  // Alexander: Please add your algorithm here#
  // For each combination, the positive particles should appear FIRST
  // e.g. +++-
  // or ++++--
  // The next lines are JUST for testing
//   for (i = 0; i < numberOfCombinations; i++)
//    {pid_real_array[i][0] = HPidPhysicsConstants::pid("p");
//     pid_real_array[i][1] = HPidPhysicsConstants::pid("p");
//     pid_real_array[i][2] = HPidPhysicsConstants::pid("pi+");
//     pid_real_array[i][3] = HPidPhysicsConstants::pid("pi-");
//      missing_array[i]    = HPidPhysicsConstants::pid("pi0");
//    }

  pChainCurrent = pChainBegin;
  i = 0;
  while (pChainCurrent != NULL)
   {//Ingo:
    //Particles appear (temporary?) in the order of input of tracks (positive ones may be NOT first)
    //(the order may be changed if it is really need):
    for (j = 0; j < numberOfParticles; j++)
     {
      pid_real_array[i][j] = pChainCurrent->pidCharged[j+1];
      tid_real_array[i][j] = pChainCurrent->tidCharged[j+1]; // <--- Ingo!
     }
    missing_array[i] = pChainCurrent->pidMissing;
    i++;
    pChainTemp    = pChainCurrent;
    pChainCurrent = pChainCurrent->next;
    delete pChainTemp;
   }
// to check filled arrays:
//   if (printFlag)
//    {fprintf (otp,"{----- numberOfParticles %i ---\n",numberOfParticles);
//     for (i = 0; i < numberOfCombinations; i++)
//      {fprintf (otp,"%3i: ",i);
//       for (j = 0; j < numberOfParticles; j++) fprintf (otp,"(%2i %2i) ",tid_real_array[i][j],pid_real_array[i][j]);
//       fprintf (otp,"   %2i\n",missing_array[i]);
//      }
//     fprintf (otp,"}----- numberOfCombinations %i ---\n",numberOfCombinations);
//    }
}

void HHypMultiChannelFiller::fill_pid_table(HHypComb * hypcomb)
{
  for (Int_t i = 0; i < numberOfCombinations; i++)
   {
    for (Int_t j = 0; j < numberOfParticles; j++)
     {
      hypcomb->setPid(i, j, pid_real_array[i][j]);

      hypcomb->setIdxPidTrackCand(i, j, tid_real_array[i][j]); // <---- Ingo!

     }
//    hyplist->setUserValue(FILLER_MISSING_PID,missing_array[i],i);
   }
}

// void HHypMultiChannelFiller::fill_pid_idx(HHypComb * hypcomb)
// { Int_t sign;
//   Int_t pPos;
//   Int_t idx = 0;                // idx nnumber of particle in contPid
//   Int_t          *iPosPlus;     // position of positive Particle in REAL Idx table
//   Int_t          *iPosMinus;    // position of negative Particle in REAL Idx table
//   Int_t          *iPosPlusF;    // position of positive Particle in FULL Idx table
//   Int_t          *iPosMinusF;   // position of negative Particle in FULL Idx table
//   HPidTrackCand   *pidPart = NULL;
//
//   iPosPlus   = new Int_t[numberOfCombinations];
//   iPosMinus  = new Int_t[numberOfCombinations];
//   iPosPlusF  = new Int_t[numberOfCombinations];
//   iPosMinusF = new Int_t[numberOfCombinations];
//
//   for (Int_t i = 0; i < numberOfCombinations; i++) {
//     iPosPlus  [i] = 0;
//     iPosMinus [i] = 0;
//     iPosPlusF [i] = 0;
//     iPosMinusF[i] = 0;
//   }
//
// //   if (printFlag) fprintf (otp,"fill_pid_idx:\n");
//   m_pContItPart->Reset();
//   while (  (pidPart = (HPidTrackCand *) m_pContItPart->Next()) != NULL   && (idx < max_t))
//    {sign = pidPart->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
// #if 0
//     cout << "-------------" << endl;
//     cout << " 2: " << pidPart->getWeightForPID( 2) << endl;
//     cout << " 3: " << pidPart->getWeightForPID( 3) << endl;
//     cout << " 8: " << pidPart->getWeightForPID( 8) << endl;
//     cout << " 9: " << pidPart->getWeightForPID( 9) << endl;
//     cout << "14: " << pidPart->getWeightForPID(14) << endl;
// #endif
//     if (sign == -1) // set Idx for negative particle
//      for (Int_t i = 0; i < numberOfCombinations; i++)
//       {pPos = (numberOfParticles - 1) - iPosMinus[i];
//        hypcomb->setIdxPidTrackCand(i, pPos, idx);
// //        if (printFlag)
// //          fprintf (otp,"i %4i pPos %i idx %i iPosMinus[%4i] %4i\n",i,pPos,idx,i,iPosMinus[i]);
//        iPosMinus[i]++;
//       }
//      else
//       for (Int_t i = 0; i < numberOfCombinations; i++)
//        {pPos = iPosPlus[i];
//         hypcomb->setIdxPidTrackCand(i, pPos, idx);
// //         if (printFlag)
// //           fprintf (otp,"i %4i pPos %i idx %i iPosPlus [%4i] %4i\n",i,pPos,idx,i,iPosPlus[i]);
//         iPosPlus[i]++;
//        }
//
// //     for (Int_t i = 0; i < numberOfCombinations; i++)
// //      {//sign = pidPart->getTrackData()->getPolarity(ALG_RUNGEKUTTA);
// //       if (sign == -1) //(pidPart->getTrackData()->getPolarity(ALG_RUNGEKUTTA) == -1)
// //        { // set Idx for negative particle
// //         hypcomb->setIdxPidTrackCand(i, (numberOfParticles - 1) - iPosMinus[i], idx);
// //         iPosMinus[i]++;
// //        }
// //         else if (sign == +1) // (pidPart->getTrackData()->getPolarity(ALG_RUNGEKUTTA) == 1)
// //          {   // set Idx for positive particle
// //           hypcomb->setIdxPidTrackCand(i, iPosPlus[i], idx);
// //           iPosPlus[i]++;
// //          }
// //      }
//
//     idx++;
//    }                             // end while
//
//   delete[]iPosMinus;
//   delete[]iPosPlus;
//   delete[]iPosMinusF;
//   delete[]iPosPlusF;
// }

Bool_t HHypMultiChannelFiller::fill_pid_fprob(HHypComb * hypcomb, Int_t * numpidtracks)
{ //HPidTrackCand   *PidPart    = NULL;
  //HCategory      *pidpartCat = gHades->getCurrentEvent()->getCategory(catPidTrackCand);
  for (Int_t Icomb = 0; Icomb < numberOfCombinations; Icomb++)
   {Float_t ProbComb = 1.0;
    if (Icomb < MAX_USER_VALUES) numpidtracks[Icomb] = 0;
   /* for (Int_t Ipart = 0; Ipart < numberOfParticles; Ipart++)
     {Int_t Pid = hypcomb->getPid       (Icomb, Ipart);

      if (Pid > 0)
       {
        Int_t Idx = hypcomb->getIdxPidTrackCand(Icomb, Ipart);

        PidPart = (HPidTrackCand *) pidpartCat->getObject(Idx);
        if (PidPart == NULL)
         {
          Error("fill_pid_fprob","[ERROR] HHypMultiChannelFiller::execute() setProbComb");
          if (printFlag) printf ("Pid %2i Idx %i\n",Pid,Idx);
          return kFALSE;
         }
        Float_t Prob = PidPart->getWeightForPID(Pid);
        Float_t Pid_from_PID = PidPart->getPid();
        //cout << "Our Pid " << Pid << " Prob " << Prob << " Pid_from_PID " << Pid_from_PID << endl;

        // To distinguish this to cases (below)
        // Pid_from_PID=-3  Prob(p)=-1  Prob(pi+)=-1     Prob(pi-)=-1   Charge=-1
        // Pid_from_PID= 8  Prob(p)=-1  Prob(pi+)=0.999  Prob(pi-)=-1   Charge= 1
        if      (Prob == -1.0 && Pid_from_PID < 0) { Prob = 1.0;  }
        else if (Prob == -1.0 && Pid_from_PID > 0) { Prob = 0.0;  }
        else if (Icomb < MAX_USER_VALUES)          { numpidtracks[Icomb]++; }

        ProbComb *= Prob;
       }

     }*/
    //std::cout << "setting prob " << ProbComb << std::endl;
    if (ProbComb < 0) ProbComb = 0.0;           //lower than zero is very strange
    hypcomb->setProbComb(Icomb, ProbComb);
   }                             // end for
  return kTRUE;
}


//====================================================================

// Bool_t HHypMultiChannelFiller::generateNumberOfCombinations()
// { // computes number of combinations and number of pos/neg particle combination
//   // This function is normally called by init.
//   //
//   // This should be done by the MultiChannel program here
//   numberOfParticles = nMinus + nPlus;
//   fprintf (otp,"generateNumbOfComb: numberOfParticles %i nMinus %i nPlus %i\n",
//                 numberOfParticles,nMinus,nPlus);
//   if (numberOfParticles > max_t) //10)
//    {Error("generateNumberOfCombinations",
//           "HHypMultiChannelFiller: we do not support more than %i particles in one reaction",max_t);
//     numberOfCombinations = 0;
//     //              numberOfPosCombinations=0;// not needed
//     //              numberOfNegCombinations=0;// not needed
//     return kFALSE;
//    }
// //  Int_t sort;
//   numberOfCombinations = 123;   //THIS HAS TO BE CHANGED
//   fprintf (otp,"numberOfCombinations %i\n",numberOfCombinations);
//   return kTRUE;
// }                               // End Bool_t HHypMultiChannelFiller::generateNumberOfCombinations()

