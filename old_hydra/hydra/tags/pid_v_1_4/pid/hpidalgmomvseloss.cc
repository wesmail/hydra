//*-- Author : Dusan Zovinec 13/03/2003
//*-- Modified : Dusan Zovinec 13/06/2003 (background function changed into Landau)

//////////////////////////////////////////////////////////////////////////////////
//
// HPidAlgMomVsEloss
//
// The PID algorithm uses the correlations between energy loss and momentum
// data from kick plain. Energy loss PDF is approximation of the Vavilov function
// (based on NIM in Phys.Res. B47 (1990) 215-223) with both the Gaussian and
// Landau limits depending on the distinguishing parameter kappa:
//    0.00  < kappa < 0.02     -  Landau PDF
//    0.02 <= kappa < 12.0     -  aproximation of Vavilov PDF
//    12.0 <= kappa < infinity -  Gaussian PDF
// PID algorithm calculates the values of the above PDF functions
// and appropriate confidence coefficients.
//
//////////////////////////////////////////////////////////////////////////////////

#pragma implementation

#include "hpidalgmomvseloss.h"
#include "hpidtrackcand.h"
#include "hpidreconstructor.h"

#include "hades.h"
#include "hruntimedb.h"

#include "hpidalgmomvselosspar.h"
#include "hpidelossmpvshiftpar.h"
#include "hpidphysicsconstants.h"

#include <hkicktrack.h>

// -----------------------------------------------------------------------------

ClassImp(HPidAlgMomVsEloss)

// -----------------------------------------------------------------------------

HPidAlgMomVsEloss::HPidAlgMomVsEloss(void) : HPidAlgorithm("MomVsEloss", algMomVsEloss){
  // Default constructor
}

// -----------------------------------------------------------------------------

HPidAlgMomVsEloss::HPidAlgMomVsEloss(Float_t fWeight) : HPidAlgorithm("MomVsEloss", algMomVsEloss, fWeight){
  // Default constructor with weight
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsEloss::init(void){

  // Get pointer to the parameter's container
  if((pParams = (HPidAlgMomVsElossPar *)gHades->getRuntimeDb()->getContainer(PIDALGMOMVSELOSSPAR_NAME)) == NULL){
    Error("init", "Cannot get parameters: %s", PIDALGMOMVSELOSSPAR_NAME);
    return kFALSE;
  }

  if((pMPVShiftParam = (HPidElossMPVShiftPar *)gHades->getRuntimeDb()->getContainer(PIDELOSSMPVSHIFTPAR_NAME)) == NULL){
    Error("init", "Cannot get parameters: %s", PIDELOSSMPVSHIFTPAR_NAME);
    return kFALSE;
  }

  // Chebyshev coefficients
  dChebCo[0][0][0]= 0.25850874;     dChebCo[0][0][1]= 0.43142617;    dChebCo[0][0][2]= 0.25225964;    dChebCo[0][0][3]= 1.2593243;     dChebCo[0][0][4]=-0.024864376;    dChebCo[0][0][5]= 0.035855696;    dChebCo[0][0][6]= 10.234692;    dChebCo[0][0][7]= 21.487522;
  dChebCo[0][1][0]= 0.032477982;    dChebCo[0][1][1]= 0.040797543;   dChebCo[0][1][2]= 0.064820468;   dChebCo[0][1][3]=-0.20374501;    dChebCo[0][1][4]=-0.0010368495;   dChebCo[0][1][5]=-0.027542114;    dChebCo[0][1][6]=-3.5619655;    dChebCo[0][1][7]=-11.825253;
  dChebCo[0][2][0]=-0.0059020496;   dChebCo[0][2][1]=-0.0091490215;  dChebCo[0][2][2]=-0.023615759;   dChebCo[0][2][3]= 0.095055662;   dChebCo[0][2][4]= 0.0014330117;   dChebCo[0][2][5]= 0.012631023;    dChebCo[0][2][6]= 0.69387764;   dChebCo[0][2][7]= 4.3133087;
  dChebCo[0][3][0]= 0.0;            dChebCo[0][3][1]= 0.0;           dChebCo[0][3][2]= 0.0;           dChebCo[0][3][3]=-0.020771531;   dChebCo[0][3][4]= 0.0002005273;   dChebCo[0][3][5]=-0.0030188807;   dChebCo[0][3][6]=-0.14047599;   dChebCo[0][3][7]=-1.4500543;
  dChebCo[0][4][0]= 0.024880692;    dChebCo[0][4][1]= 0.042127077;   dChebCo[0][4][2]= 0.023834176;   dChebCo[0][4][3]=-0.046865180;   dChebCo[0][4][4]= 0.0018751903;   dChebCo[0][4][5]=-0.00084479939;  dChebCo[0][4][6]=-1.9952390;    dChebCo[0][4][7]=-3.4343169;
  dChebCo[0][5][0]= 0.0047404356;   dChebCo[0][5][1]= 0.0073167928;  dChebCo[0][5][2]= 0.0021624675;  dChebCo[0][5][3]=-0.0077222986;  dChebCo[0][5][4]= 0.0012668869;   dChebCo[0][5][5]= 0.0;            dChebCo[0][5][6]=-0.45679694;   dChebCo[0][5][7]=-1.1063164;
  dChebCo[0][6][0]=-0.0007444513;   dChebCo[0][6][1]=-0.0014026047;  dChebCo[0][6][2]=-0.0026865597;  dChebCo[0][6][3]= 0.0032241039;  dChebCo[0][6][4]= 0.00048736023;  dChebCo[0][6][5]= 0.00045675843;  dChebCo[0][6][6]= 0.0;          dChebCo[0][6][7]=-0.21000819;
  dChebCo[0][7][0]= 0.0073225731;   dChebCo[0][7][1]= 0.016195241;   dChebCo[0][7][2]=-0.0054891384;  dChebCo[0][7][3]= 0.008988292;   dChebCo[0][7][4]= 0.0034850854;   dChebCo[0][7][5]=-0.0069836141;   dChebCo[0][7][6]= 0.50505298;   dChebCo[0][7][7]= 1.7891643;
  dChebCo[0][8][0]= 0.0;            dChebCo[0][8][1]= 0.0024714789;  dChebCo[0][8][2]= 0.0039800522;  dChebCo[0][8][3]=-0.0067167236;  dChebCo[0][8][4]= 0.0;            dChebCo[0][8][5]= 0.0039876546;   dChebCo[0][8][6]= 0.0;          dChebCo[0][8][7]=-0.89601916;
  dChebCo[0][9][0]= 0.0011668284;   dChebCo[0][9][1]= 0.0020751278;  dChebCo[0][9][2]= 0.0048447456;  dChebCo[0][9][3]=-0.013049241;   dChebCo[0][9][4]=-0.00036597173;  dChebCo[0][9][5]=-0.0036055679;   dChebCo[0][9][6]= 0.0;          dChebCo[0][9][7]= 0.39120793;
  dChebCo[0][10][0]= 0.0;           dChebCo[0][10][1]= 0.0;          dChebCo[0][10][2]=-0.0089439554; dChebCo[0][10][3]= 0.018786468;  dChebCo[0][10][4]= 0.0019372124;  dChebCo[0][10][5]= 0.0;           dChebCo[0][10][6]= 0.0;         dChebCo[0][10][7]= 0.73410606;
  dChebCo[0][11][0]=-0.0015727318;  dChebCo[0][11][1]=-0.0025141668; dChebCo[0][11][2]=-0.0062756944; dChebCo[0][11][3]= 0.014484097;  dChebCo[0][11][4]= 0.00070761825; dChebCo[0][11][5]= 0.0015298434;  dChebCo[0][11][6]= 0.0;         dChebCo[0][11][7]= 0.0;
  dChebCo[0][12][0]=-0.0011210142;  dChebCo[0][12][1]=-0.0014064022; dChebCo[0][12][2]=-0.0024655436; dChebCo[0][12][3]= 0.0;          dChebCo[0][12][4]= 0.00046898375; dChebCo[0][12][5]= 0.0019247256;  dChebCo[0][12][6]= 0.0;         dChebCo[0][12][7]=-0.32454506;

  dChebCo[1][0][0]= 0.27827257;     dChebCo[1][0][1]= 0.41421789;    dChebCo[1][0][2]= 0.20191059;    dChebCo[1][0][3]= 1.3206085;     dChebCo[1][0][4]= 0.016435245;    dChebCo[1][0][5]= 0.033432409;    dChebCo[1][0][6]= 5.4529572;    dChebCo[1][0][7]= 9.3841352;
  dChebCo[1][1][0]=-0.0014227603;   dChebCo[1][1][1]=-0.030061649;   dChebCo[1][1][2]=-0.046831422;   dChebCo[1][1][3]= 0.10036618;    dChebCo[1][1][4]= 0.0360514;      dChebCo[1][1][5]= 0.0060583916;   dChebCo[1][1][6]=-0.90906096;   dChebCo[1][1][7]=-1.6276904;
  dChebCo[1][2][0]= 0.0024848327;   dChebCo[1][2][1]= 0.0052249697;  dChebCo[1][2][2]= 0.0096777473;  dChebCo[1][2][3]=-0.022015201;   dChebCo[1][2][4]= 0.002303652;    dChebCo[1][2][5]=-0.0023381379;   dChebCo[1][2][6]= 0.086122438;  dChebCo[1][2][7]= 0.16571423;
  dChebCo[1][3][0]= 0.0;            dChebCo[1][3][1]= 0.0;           dChebCo[1][3][2]=-0.0017995317;  dChebCo[1][3][3]= 0.0061667091;  dChebCo[1][3][4]=-0.00061666343;  dChebCo[1][3][5]= 0.00083846081;  dChebCo[1][3][6]= 0.0;          dChebCo[1][3][7]= 0.0;
  dChebCo[1][4][0]= 0.045091424;    dChebCo[1][4][1]= 0.12693873;    dChebCo[1][4][2]= 0.053921588;   dChebCo[1][4][3]=-0.14986093;    dChebCo[1][4][4]=-0.010775802;    dChebCo[1][4][5]=-0.013346861;    dChebCo[1][4][6]=-1.2218009;    dChebCo[1][4][7]=-1.8160479;
  dChebCo[1][5][0]= 0.0080559636;   dChebCo[1][5][1]= 0.022999801;   dChebCo[1][5][2]= 0.0035068740;  dChebCo[1][5][3]=-0.012720568;   dChebCo[1][5][4]= 0.0051476061;   dChebCo[1][5][5]=-0.0017402116;   dChebCo[1][5][6]=-0.32324120;   dChebCo[1][5][7]=-0.50919193;
  dChebCo[1][6][0]=-0.0038974523;   dChebCo[1][6][1]=-0.0086792801;  dChebCo[1][6][2]=-0.012621494;   dChebCo[1][6][3]= 0.024972042;   dChebCo[1][6][4]= 0.0056856517;   dChebCo[1][6][5]= 0.0021052496;   dChebCo[1][6][6]=-0.027373591;  dChebCo[1][6][7]=-0.051384654;
  dChebCo[1][7][0]= 0.0;            dChebCo[1][7][1]= 0.031875584;   dChebCo[1][7][2]=-0.0054996531;  dChebCo[1][7][3]=-0.0097751962;  dChebCo[1][7][4]=-0.013438433;    dChebCo[1][7][5]= 0.0015528195;   dChebCo[1][7][6]= 0.12173464;   dChebCo[1][7][7]= 0.21413992;
  dChebCo[1][8][0]=-0.0030634124;   dChebCo[1][8][1]=-0.0061757928;  dChebCo[1][8][2]=-0.0090029985;  dChebCo[1][8][3]= 0.026087455;   dChebCo[1][8][4]= 0.0;            dChebCo[1][8][5]= 0.002190067;    dChebCo[1][8][6]= 0.0;          dChebCo[1][8][7]= 0.0;
  dChebCo[1][9][0]= 0.00075633702;  dChebCo[1][9][1]= 0.0;           dChebCo[1][9][2]= 0.0034958743;  dChebCo[1][9][3]=-0.011399062;   dChebCo[1][9][4]= 0.0;            dChebCo[1][9][5]=-0.0013202847;   dChebCo[1][9][6]= 0.0;          dChebCo[1][9][7]= 0.0;
  dChebCo[1][10][0]= 0.0054730726;  dChebCo[1][10][1]= 0.019716857;  dChebCo[1][10][2]= 0.018513506;  dChebCo[1][10][3]=-0.048282515;  dChebCo[1][10][4]=-0.0025421507;  dChebCo[1][10][5]=-0.0045124157;  dChebCo[1][10][6]= 0.040917471; dChebCo[1][10][7]= 0.066596366;
  dChebCo[1][11][0]= 0.0019792507;  dChebCo[1][11][1]= 0.0032596742; dChebCo[1][11][2]= 0.0068332334; dChebCo[1][11][3]=-0.0098552378; dChebCo[1][11][4]= 0.0020169108;  dChebCo[1][11][5]=-0.0015629454;  dChebCo[1][11][6]= 0.0;         dChebCo[1][11][7]= 0.0;
  dChebCo[1][12][0]= 0.0;           dChebCo[1][12][1]= 0.0;          dChebCo[1][12][2]=-0.0012940502; dChebCo[1][12][3]= 0.0;          dChebCo[1][12][4]=-0.0015144931;  dChebCo[1][12][5]= 0.00022499176; dChebCo[1][12][6]= 0.0;         dChebCo[1][12][7]= 0.0;

  dChebCo[2][0][0]= 0.29712948;     dChebCo[2][0][1]= 0.40882632;    dChebCo[2][0][2]= 0.16861629;    dChebCo[2][0][3]= 1.3493897;     dChebCo[2][0][4]= 0.10264949;     dChebCo[2][0][5]= 0.029568177;    dChebCo[2][0][6]=0.0;           dChebCo[2][0][7]= 6.6184654;
  dChebCo[2][1][0]= 0.0097572934;   dChebCo[2][1][1]= 0.014474912;   dChebCo[2][1][2]= 0.0;           dChebCo[2][1][3]=-0.0026863185;  dChebCo[2][1][4]= 0.032738857;    dChebCo[2][1][5]=-0.001630006;    dChebCo[2][1][6]=0.0;           dChebCo[2][1][7]=-0.73866379;
  dChebCo[2][2][0]= 0.0;            dChebCo[2][2][1]= 0.0025023704;  dChebCo[2][2][2]= 0.0036317285;  dChebCo[2][2][3]=-0.003521604;   dChebCo[2][2][4]= 0.0;            dChebCo[2][2][5]=-0.00021119745;  dChebCo[2][2][6]=0.0;           dChebCo[2][2][7]= 0.044693973;
  dChebCo[2][3][0]=-0.0015291686;   dChebCo[2][3][1]=-0.0037707379;  dChebCo[2][3][2]=-0.0043657818;  dChebCo[2][3][3]= 0.024434909;   dChebCo[2][3][4]= 0.0043608779;   dChebCo[2][3][5]= 0.0023599053;   dChebCo[2][3][6]=0.0;           dChebCo[2][3][7]= 0.0;
  dChebCo[2][4][0]= 0.035707399;    dChebCo[2][4][1]= 0.18719727;    dChebCo[2][4][2]= 0.030144338;   dChebCo[2][4][3]=-0.083447911;   dChebCo[2][4][4]=-0.043097757;    dChebCo[2][4][5]=-0.0048515387;   dChebCo[2][4][6]=0.0;           dChebCo[2][4][7]=-1.4540925;
  dChebCo[2][5][0]= 0.0096221631;   dChebCo[2][5][1]= 0.056954987;   dChebCo[2][5][2]= 0.013891826;   dChebCo[2][5][3]=-0.04806136;    dChebCo[2][5][4]=-0.0022647176;   dChebCo[2][5][5]=-0.0040797531;   dChebCo[2][5][6]=0.0;           dChebCo[2][5][7]=-0.39529833;
  dChebCo[2][6][0]=-0.0018402821;   dChebCo[2][6][1]= 0.0;           dChebCo[2][6][2]=-0.0058030495;  dChebCo[2][6][3]= 0.0076473951;  dChebCo[2][6][4]= 0.009453129;    dChebCo[2][6][5]= 0.00040403265;  dChebCo[2][6][6]=0.0;           dChebCo[2][6][7]=-0.044293243;
  dChebCo[2][7][0]=-0.0049821585;   dChebCo[2][7][1]= 0.023020158;   dChebCo[2][7][2]=-0.0038717547;  dChebCo[2][7][3]= 0.02449443;    dChebCo[2][7][4]=-0.012442571;    dChebCo[2][7][5]= 0.0018200105;   dChebCo[2][7][6]=0.0;           dChebCo[2][7][7]= 0.088741049;
  dChebCo[2][8][0]= 0.0018831112;   dChebCo[2][8][1]= 0.0050574313;  dChebCo[2][8][2]= 0.0085359607;  dChebCo[2][8][3]=-0.0162092;     dChebCo[2][8][4]=-0.0032283517;   dChebCo[2][8][5]=-0.0014346306;   dChebCo[2][8][6]=0.0;           dChebCo[2][8][7]= 0.0;
  dChebCo[2][9][0]= 0.0043541673;   dChebCo[2][9][1]= 0.009455014;   dChebCo[2][9][2]= 0.014507659;   dChebCo[2][9][3]=-0.037768479;   dChebCo[2][9][4]=-0.0075640352;   dChebCo[2][9][5]=-0.0039165276;   dChebCo[2][9][6]=0.0;           dChebCo[2][9][7]= 0.0;
  dChebCo[2][10][0]= 0.0020301312;  dChebCo[2][10][1]= 0.019300232;  dChebCo[2][10][2]= 0.0082387775; dChebCo[2][10][3]=-0.047890063;  dChebCo[2][10][4]=-0.0088293329;  dChebCo[2][10][5]=-0.0037432073;  dChebCo[2][10][6]=0.0;          dChebCo[2][10][7]= 0.0;
  dChebCo[2][11][0]=-0.0018723311;  dChebCo[2][11][1]= 0.0;          dChebCo[2][11][2]=-0.010116105;  dChebCo[2][11][3]= 0.017778596;  dChebCo[2][11][4]= 0.0052537299;  dChebCo[2][11][5]= 0.001995038;   dChebCo[2][11][6]=0.0;          dChebCo[2][11][7]= 0.0;
  dChebCo[2][12][0]=-0.00073403108; dChebCo[2][12][1]= 0.0;          dChebCo[2][12][2]=-0.005513567;  dChebCo[2][12][3]= 0.013179324;  dChebCo[2][12][4]= 0.0013340546;  dChebCo[2][12][5]= 0.0012222675;  dChebCo[2][12][6]=0.0;          dChebCo[2][12][7]= 0.0;

  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsEloss::reinit(void){

  // Dummy method

  return kTRUE;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsEloss::finalize(void){

  // Dummy method

  return kTRUE;
}

Bool_t HPidAlgMomVsEloss::calculatePDF(HPidTrackCand *pTrack, Float_t afReturn[], Short_t &nCatIndex){
  // Calculate PDF from data stroed in histograms in HPidAlgMomVsElossPar
  HKickTrack *pKickTrack;

  if(pTrack->getKickTrackId() < 0) return kTRUE; // not a full track
  if((pKickTrack = pTrack->getKickTrack()) == NULL){
    Error("calculatePDF", "Cannot get HKickTrack for the track: %d", pTrack->getKickTrackId());
    return kFALSE;
  }

  static  Double_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());

  Float_t fCharge = pKickTrack->getCharge();
  Int_t   iSys    = pKickTrack->getSystem();
  Float_t fMom    = pKickTrack->getP();
  Float_t fTheta  = TMath::RadToDeg() * pKickTrack->getTheta();
  Float_t fEloss   = pKickTrack->getMetaEloss();
  Int_t   iId;
  Float_t fParams[4];
  Float_t fMPVShiftParam;
  Int_t   i, k;

  //printf("eloss: track\n");
  for(i = 0; i < pRec->particlesNumber(); i++){
    iId = pRec->getParticleId(i);
    //printf("eloss: pid %d of current track\n",iId);
    // compare the particle charge
    if(fCharge != HPidPhysicsConstants::charge(iId)){
      afReturn[i] = 0.0f;
      continue;
    }

    if( ! HPidPhysicsConstants::isFake(iId)){

      // eloss fit
      for(k = 0; k < 4; k++) fParams[k] = pParams->getParameterValue(iId, 4 * iSys + k, fMom, fTheta);
      //if(iId == 11) printf("parameters: %f %f %f %f\n", fParams[0], fParams[1], fParams[2],fParams[3]);
      if((fParams[2] <= 0.0f) || (fParams[2] >= 1.0f)){
        if((fParams[2] < 0.0f) || (fParams[2] >= 1.0f)) Error("execute", "Wrong beta value for eloss distr. %d %d %f %f = %f", iId, 4 * iSys + 2, fMom, fTheta, fParams[2]);
        afReturn[i] = -1.0f;
        continue;
      }
      if(fParams[3] <= 0.0f){
        if(fParams[3] < 0.0f) Error("execute", "Wrong xi value for eloss distr. %d %d %f %f = %f", iId, 4 * iSys + 3, fMom, fTheta, fParams[3]);
        afReturn[i] = -1.0f;
        continue;
      }

      fMPVShiftParam = pMPVShiftParam->getParameterValue(iId, iSys, fMom, fTheta);

      if((fEloss>0.8) && (fEloss<10.0)){
        afReturn[i] = calculatePDF(fEloss,fMPVShiftParam,fParams);
      } else {
        afReturn[i] = -1.0;
      }
    } else {
      // landau fit for the background functions
      if((fEloss>0.8) && (fEloss<10.0)){
        for(k = 0; k < 3; k++) fParams[k] = pParams->getParameterValue(iId, 4 * iSys + k, fMom, fTheta);
        //printf("parameters:%d %f %f %f\n", iId, fParams[0], fParams[1], fParams[2]);
        Double_t mpv=fParams[1]+fParams[2]*0.225;
        if(fParams[2]>0.0) afReturn[i] =(1.0/fParams[2])*TMath::Landau(fEloss,mpv,fParams[2]);
        else afReturn[i] = 0.0;
        //afReturn[i] = fParams[0] + fParams[1] * fEloss + fParams[2] * fEloss * fEloss;
        if(afReturn[i] < 0.0f) afReturn[i] = 0.0f;
      } else {
        afReturn[i] = -1.0f;
      }
    }
    //printf("eloss:val = %f\n",afReturn[i]);
  }
  return kTRUE;
}


Float_t HPidAlgMomVsEloss::calculatePDF(Float_t fEloss, Float_t fMPVShiftParam, Float_t *fParams){
  // Body of the energy loss PDF calculation

  static  Double_t Euler    = 0.57721566490;
  Float_t elosspdf=0.0;
  Float_t beta2=(fParams[2]*fParams[2]);

  // Landau limit of the energy loss probability distribution function.
  if(fParams[1] <0.02){
    Double_t mpv=fParams[0]+fParams[3]*0.225;
    elosspdf=(1.0/fParams[3])*TMath::Landau(fEloss,mpv,fParams[3]);
  }

  // Aproximation of the Vavilov aproximation of the
  // energy loss probability distribution function.
  if((fParams[1]>=0.02) && (fParams[1]<12.00)){
    Double_t lambdaMPV = fMPVShiftParam;
    Double_t lambda;

    // Moyal approximation of Vavilov distribution function
    if((fParams[1]>=0.02) && (fParams[1]<0.29)){
      Double_t kappaMin=0.12, kappaMax=0.2, betaMin=0.05, betaMax=1.0;
      Double_t lambdaLB=-3.03, a[8];
      Int_t  kappa_index=0;

      // Chebyshev polynomials
      if((fParams[1] >= 0.02) && (fParams[1] < 0.12)){
        kappaMin = 0.02;
        kappaMax = 0.112;
      }
      if((fParams[1] >= 0.12) && (fParams[1] < 0.22)){
        kappaMin = 0.12;
        kappaMax = 0.2;
      }
      if((fParams[1] >= 0.22) && (fParams[1] < 0.29)){
        kappaMin = 0.22;
        kappaMax = 0.3;
      }

      Double_t kappaMean = 1.0 + 2.0*((fParams[1] - kappaMax)/(kappaMax - kappaMin));
      Double_t betaMean = 1.0 + 2.0*((fParams[2] - betaMax)/(betaMax - betaMin));
      Double_t T0K = 1.0;
      Double_t T1K = kappaMean;
      Double_t T2K = 2.0*kappaMean*kappaMean - 1.0;
      Double_t T3K = (4.0*kappaMean*kappaMean*kappaMean) - (3.0*kappaMean);

      Double_t T0B = 1.0;
      Double_t T1B = betaMean;
      Double_t T2B = 2.0*betaMean*betaMean - 1.0;
      Double_t T3B = (4.0*betaMean*betaMean*betaMean) - (3.0*betaMean);

      if((fParams[1]>=0.02) && (fParams[1]<0.12)){
        kappa_index = 0;
        lambdaLB = -3.03;
      }
      if((fParams[1]>=0.12) && (fParams[1]<0.22)){
        kappa_index = 1;
        lambdaLB = -3.04;
      }
      if((fParams[1]>=0.22) && (fParams[1]<0.29)){
        kappa_index = 2;
        lambdaLB = -3.05;
      }

      for(Int_t j=0; j<8; j++){
        a[j]=(dChebCo[kappa_index][0][j]*T0K*T0B)+
             (dChebCo[kappa_index][1][j]*T1K*T0B)+
             (dChebCo[kappa_index][2][j]*T2K*T0B)+
             (dChebCo[kappa_index][3][j]*T3K*T0B)+
             (dChebCo[kappa_index][4][j]*T0K*T1B)+
             (dChebCo[kappa_index][5][j]*T0K*T2B)+
             (dChebCo[kappa_index][6][j]*T0K*T3B)+
             (dChebCo[kappa_index][7][j]*T1K*T1B)+
             (dChebCo[kappa_index][8][j]*T2K*T1B)+
             (dChebCo[kappa_index][9][j]*T3K*T1B)+
             (dChebCo[kappa_index][10][j]*T1K*T2B)+
             (dChebCo[kappa_index][11][j]*T1K*T3B)+
             (dChebCo[kappa_index][12][j]*T2K*T2B);
      }

      lambda = (fEloss-fParams[0])/fParams[3] + lambdaMPV;
      elosspdf = 0.0;
      if((fParams[1]>=0.02) && (fParams[1]<0.22)){
        if((lambda>=lambdaLB) && (lambda<a[6])){
          elosspdf = (1.0/fParams[3])*(a[0]*(TMath::Exp((-a[1]*(lambda+(a[4]*lambda*lambda))) - a[2]*(TMath::Exp(-a[3]*(lambda+(a[5]*lambda*lambda)))))));
        }
        if((lambda>=a[6]) && (lambda<=a[7])){
          Double_t vav95=(a[0]*(TMath::Exp((-a[1]*(a[6]+(a[4]*a[6]*a[6]))) - a[2]*(TMath::Exp(-a[3]*(a[6]+(a[5]*a[6]*a[6])))))));
          Double_t aconst = 1.0/TMath::Log(a[7]/a[6]);
          Double_t bconst = (a[7]-a[6])/(a[7]*a[6]);
          Double_t Dconst = a[6]*a[6]*(vav95-((aconst*0.045)/(a[6])))*(1.0/(1.0-(a[6]*aconst*bconst)));
          Double_t dconst = aconst*((0.045/Dconst)-bconst);
          elosspdf = (1.0/fParams[3])*Dconst*((1.0/(lambda*lambda))+(dconst/lambda));
        }
      }
      if((fParams[1]>=0.22) && (fParams[1]<0.29)){
        if((lambda>=lambdaLB) && (lambda<a[7])){
          elosspdf = (1.0/fParams[3])*(a[0]*(TMath::Exp((-a[1]*(lambda+(a[4]*lambda*lambda))) - a[2]*(TMath::Exp(-a[3]*(lambda+(a[5]*lambda*lambda)))))));
        }
      }
    }

    // Edgeworth expansion aproximation
    // of the Vavilov distribution function.
    if((fParams[1] >= 0.29) && (fParams[1] < 12.0)){
      Double_t A0 = -(0.0322*beta2) - 0.0743;
      Double_t A1 = -(0.0135*beta2) - 0.0488;
      Double_t B0 = -(0.2453*beta2) + 0.0701;
      Double_t B1 = -(1.6921*beta2) + 8.3656;
      Double_t C0 = -(0.5561*beta2) - 3.1579;
      Double_t C1 = -(0.7327*beta2) - 3.5226;
      Double_t lambdaL = (A0*fParams[1]) + (B0/(TMath::Sqrt(fParams[1]))) + C0;
      Double_t lambdaR = (A1*fParams[1]) + (B1/(TMath::Sqrt(fParams[1]))) + C1;

      Double_t mean=(Euler-beta2-1.0-TMath::Log(fParams[1]));
      lambda = (fEloss-fParams[0])/fParams[3]+lambdaMPV;
      Double_t delta= (lambda-mean);

      Double_t kappa2=(fParams[1]*fParams[1]);
      Double_t kappa3=(kappa2*fParams[1]);
      Double_t kappa4=(kappa2*kappa2);

      Double_t alfa2=(1.0/fParams[1])*(1.0-(beta2/2.0));
      Double_t alfa3=(1.0/kappa2)*(0.5-(beta2/3.0));
      Double_t alfa4=(1.0/kappa3)*((1.0/3.0)-(beta2/4.0));
      Double_t alfa5=(1.0/kappa4)*(0.25-(beta2/5.0));

      Double_t mu2 = alfa2;
      Double_t mu3 = alfa3;
      Double_t mu4 = (3.0*alfa2*alfa2) + alfa4;
      Double_t mu5 = (10.0*alfa2*alfa3) + alfa5;

      Double_t sigma=TMath::Sqrt(mu2);
      Double_t sigma2=mu2;
      Double_t sigma3=sigma2*sigma;
      Double_t sigma4=sigma2*sigma2;
      Double_t sigma5=sigma3*sigma2;

      Double_t ratio=(delta/sigma);
      Double_t ratio2=ratio*ratio;
      Double_t norm = (1.0/(sigma*(TMath::Sqrt(2.0*TMath::Pi()))))*(TMath::Exp(-1.0*ratio2/2.0));

      Double_t H0 = 1.0;
      Double_t H1 = ratio;
      Double_t H2 = (ratio*H1) - (H0);
      Double_t H3 = (ratio*H2) - (2.0*H1);
      Double_t H4 = (ratio*H3) - (3.0*H2);
      Double_t H5 = (ratio*H4) - (4.0*H3);
      Double_t H6 = (ratio*H5) - (5.0*H4);
      Double_t H7 = (ratio*H6) - (6.0*H5);
      Double_t H8 = (ratio*H7) - (7.0*H6);
      Double_t H9 = (ratio*H8) - (8.0*H7);

      Double_t member1 = 1.0;
      Double_t member2 = (1.0/6.0)*(mu3/sigma3)*H3;
      Double_t member3 = (1.0/24.0)*((mu4/sigma4)-3.0)*H4;
      Double_t member4 = (1.0/120.0)*((mu5/sigma5)-(10.0*(mu3/sigma3)))*H5;
      Double_t member5 = (1.0/72.0)*((mu3/sigma3)*(mu3/sigma3))*H6;
      Double_t member6 = (1.0/144.0)*((mu3/sigma3)*((mu4/sigma4)-3.0))*H7;
      Double_t member7 = (1.0/1296.0)*((mu3/sigma3)*(mu3/sigma3)*(mu3/sigma3))*H9;

      elosspdf = (1.0/fParams[3])*norm*(member1+member2+member3+member4+member5+member6+member7);
      if ((lambda<lambdaL) || (lambda>lambdaR) || (elosspdf<0.0)) elosspdf=0.0;
    }
  }

  // Gaussian limit of the energy loss probability
  // density function.
  if(fParams[1] >= 12.0){
    Double_t lambda=(fEloss-fParams[0])/fParams[3];
    Double_t sigma=TMath::Sqrt((1.0/fParams[1])*(1.0-(beta2/2.0)));
    Double_t ratio2=lambda*lambda/(sigma*sigma);
    elosspdf = (1.0/(fParams[3]*sigma*(TMath::Sqrt(2.0*TMath::Pi()))))*(TMath::Exp(-ratio2/2.0));
  }
  return elosspdf;
}

// -----------------------------------------------------------------------------

Bool_t HPidAlgMomVsEloss::calculateCL(HPidTrackCand *pTrack, Float_t afReturn[], Short_t &nCatIndex){
  // This method calculates confidence coefficients numerically.
  // Each confidence coefficient is calculated as an integral probability of the PDF
  // correponding to the input value fEloss.
  // The following condition has been chosen to define confidence coefficient:
  // Range of the integral probability is as narrow as possible at a given
  // input value fEloss.

  HKickTrack *pKickTrack;

  if(pTrack->getKickTrackId() < 0) return kTRUE; // not a full track
  if((pKickTrack = pTrack->getKickTrack()) == NULL){
    Error("calculatePDF", "Cannot get HKickTrack for the track: %d", pTrack->getKickTrackId());
    return kFALSE;
  }

  static  Double_t SQRT_2PI = TMath::Sqrt(2.0 * TMath::Pi());

  Float_t fCharge = pKickTrack->getCharge();
  Int_t   iSys    = pKickTrack->getSystem();
  Float_t fMom    = pKickTrack->getP();
  Float_t fTheta  = TMath::RadToDeg() * pKickTrack->getTheta();
  Float_t fEloss   = pKickTrack->getMetaEloss();
  Int_t   iId;
  Float_t fParams[4];
  Float_t fMPVShiftParam;
  Int_t   i, k;

  Int_t flagAF=0;
  Double_t dEloss, eloss, eloss1=1.0, eloss2=1.0, eloss21, elossR;
  Double_t elossP1=1.0, elossP2=1.0, elossP=1.0, elossPR=1.0, sum, conflev;

  for(i = 0; i < pRec->particlesNumber(); i++){
    iId = pRec->getParticleId(i);

    // do not calculate CL for fakes
    if(HPidPhysicsConstants::isFake(iId)){
      afReturn[i] = -1.0f;
      continue;
    }

    // compare the particle charge
    if(fCharge != HPidPhysicsConstants::charge(iId)){
      afReturn[i] = -2.0f;
      continue;
    }

    // eloss fit
    for(k = 0; k < 4; k++) fParams[k] = pParams->getParameterValue(iId, 4 * iSys + k, fMom, fTheta);
    //printf("parameters: %f %f %f \n", fParams[0], fParams[1], fParams[2]);
    if((fParams[2] <= 0.0f) || (fParams[2] >= 1.0f)){
      if((fParams[2] < 0.0f) || (fParams[2] >= 1.0f)) Error("execute", "Wrong beta value for eloss distr. %d %d %f %f = %f", iId, 4 * iSys + 2, fMom, fTheta, fParams[2]);
      afReturn[i] = 0.0f;
      continue;
    }
    if(fParams[3] <= 0.0f){
      if(fParams[3] < 0.0f) Error("execute", "Wrong xi value for eloss distr. %d %d %f %f = %f", iId, 4 * iSys + 3, fMom, fTheta, fParams[3]);
      afReturn[i] = 0.0f;
      continue;
    }

    fMPVShiftParam = pMPVShiftParam->getParameterValue(iId, iSys, fMom, fTheta);


    // body of the confidence level calculation
    // search the second bound of the confidence level interval
    if(fEloss == fParams[0]){
      conflev = 0.0;
    } else {
      if(fEloss > fParams[0]){
        flagAF = 0;
        eloss2=fEloss;
        elossP2=calculatePDF(eloss2,fMPVShiftParam,fParams);
        eloss1=fParams[0];
        dEloss = 0.1;
        for(Int_t j = 1; j < 10000; j++){
          eloss1 -= dEloss;
          elossP1=calculatePDF(eloss1,fMPVShiftParam,fParams);
          if(TMath::Abs(elossP1 - elossP2) < 0.005){
            break;
          } else {
            if(elossP1<elossP2) flagAF = 1;
            if(flagAF == 1){
              if(elossP1>elossP2){
                dEloss = dEloss/2.0;
              } else {
                dEloss = -dEloss/2.0;
              }
            }
          }
        }
      }
      if(fEloss < fParams[0]){
        flagAF = 0;
        eloss1=fEloss;
        elossP1=calculatePDF(eloss1,fMPVShiftParam,fParams);
        eloss2=fParams[0];
        dEloss = 0.1;
        for(Int_t j = 1; j < 10000; j++){
          eloss2 += dEloss;
          elossP2=calculatePDF(eloss2,fMPVShiftParam,fParams);
          if(TMath::Abs(elossP1 - elossP2) < 0.005){
            break;
          } else {
            if(elossP2<elossP1) flagAF = 1;
            if(flagAF == 1){
              if(elossP2>elossP1){
                dEloss = dEloss/2.0;
              } else {
                dEloss = -dEloss/2.0;
              }
            }
          }
        }
      }
      // integral calculation
      sum=0.0;
      for(Int_t j=0; j< Int_t((eloss2-eloss1)/0.01); j++){
        eloss  = (eloss1+0.005)+(Float_t(j)*0.01);
        elossP=calculatePDF(eloss,fMPVShiftParam,fParams);
        sum += (elossP*0.01);
      }

      eloss21 = eloss1+(Float_t(Int_t((eloss2-eloss1)/0.01))*0.01);
      elossR = (eloss21+eloss2)/2.0;
      elossPR=calculatePDF(elossR,fMPVShiftParam,fParams);
      sum+= (elossPR*(eloss2-eloss21));
      conflev = sum;
    }
    afReturn[i] = conflev;
    //printf("conf. lev. = %f\n\n", conflev);
  }

  return kTRUE;
}

// -----------------------------------------------------------------------------

void HPidAlgMomVsEloss::print(void) const
{
// Empty function for printing info of the algorithm

    printf("\tMomVsEloss algorithm\n");
}
