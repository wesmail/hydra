#include "hchannellistmaker.h"
//#include "hbpidphysicsconstants.h"
//#include "TFile.h"

ClassImp(HChannelListMaker) 

//------------------------------------------------------------------- 
HChannelListMaker::HChannelListMaker () 
 {
 }

HChannelListMaker::~HChannelListMaker() 
 {
 }
 
Double_t HChannelListMaker::sqr (Double_t value)
{return value*value;} 

Int_t HChannelListMaker::getPid (Char_t* nomen)
{Int_t pid;
 pid = bLib.nmToIndex (nomen,&Phys);
 if (pid == 0) {printf ("getPid: unknown particle '%s'\n",nomen); abort ();}
  else return pid;
}
 
void HChannelListMaker::getDecayList () //(FILE* otp)
{
//  FILE* dec;
//  Bool_t enough,gotten,lineFinished;
//  Int_t decN,prtN;
//  Char_t stroke[120];
//  Char_t nomen[nameLength];
//  Char_t fileName[30];
//  sprintf (fileName,"%s","decayList.txt");
//  if ( (dec = fopen(fileName,"rt")) == NULL ) 
//    {fprintf (stderr,"'%s' file open error \n",fileName); abort();}
//  decPartsNmb = 0;
//  enough = false;
//  fprintf (otp,"\nDecay list\n");
//  while (!enough)
//   {fscanf (dec,"%5s",nomen); 
//    if (nomen[0] == '-') {enough = true; fgets (stroke,80,dec);}
//     else
//      {decPartsNmb++;
//       fscanf (dec,"%f",&decPart[decPartsNmb].cTau);
//       fgets (stroke,80,dec);
//       decPart[decPartsNmb].pID = getPid (nomen);
//       fprintf (otp," %s (cTau = %9.2e) ->\n",nomen,decPart[decPartsNmb].cTau);
//       gotten = false;
//       decN = 0;
//       while (!gotten)
//        {fscanf (dec,"%s",nomen);
//         if (nomen[0] == '-') 
//           {decPart[decPartsNmb].decaysNmb = decN;
//            gotten = true; fgets (stroke,80,dec);
//           }
//          else
//           {decN++; 
//            prtN = 1;
//            decPart[decPartsNmb].decay[decN].pID[prtN] = getPid(nomen);
//            fprintf (otp,"   %s",nomen);
//            lineFinished = false;
//            while (!lineFinished)
//             {fscanf (dec,"%s",nomen);
//              if (nomen[0] == '-')
//                {decPart[decPartsNmb].decay[decN].prong = prtN;
//                 lineFinished = true; fgets (stroke,80,dec);
//                 fprintf (otp,"\n");
//                }
//               else
//                {prtN++;
//                 decPart[decPartsNmb].decay[decN].pID[prtN] = getPid(nomen);
//                 fprintf (otp," %s",nomen);
//                }
//             }
//           }
//        }
//      }
//   }
//  fclose (dec); 
 
 decPartsNmb = 29; // 20; // 
 
 decPart[ 1].pID = getPid ("pi0");
 decPart[ 1].cTau = 25.1e-7;
 decPart[ 1].decaysNmb = 2;
 decPart[ 1].decay[1].prong = 1;
 decPart[ 1].decay[1].pID[1] = getPid ("pi0_"); // pi0_ means pi0 which deyed to -> g g ("stable" pi0)
 decPart[ 1].decay[2].prong = 3;
 decPart[ 1].decay[2].pID[1] = getPid ("e+");
 decPart[ 1].decay[2].pID[2] = getPid ("e-");
 decPart[ 1].decay[2].pID[3] = getPid ("g" );
 
 decPart[ 2].pID = getPid ("eta");
 decPart[ 2].cTau = 0.001;
 decPart[ 2].decaysNmb = 4;
 decPart[ 2].decay[1].prong = 3;
 decPart[ 2].decay[1].pID[1] = getPid ("pi+");
 decPart[ 2].decay[1].pID[2] = getPid ("pi-");
 decPart[ 2].decay[1].pID[3] = getPid ("pi0_");
 decPart[ 2].decay[2].prong = 3;
 decPart[ 2].decay[2].pID[1] = getPid ("pi+");
 decPart[ 2].decay[2].pID[2] = getPid ("pi-");
 decPart[ 2].decay[2].pID[3] = getPid ("g");
 decPart[ 2].decay[3].prong = 3;
 decPart[ 2].decay[3].pID[1] = getPid ("e+");
 decPart[ 2].decay[3].pID[2] = getPid ("e-");
 decPart[ 2].decay[3].pID[3] = getPid ("g");
 decPart[ 2].decay[4].prong = 4;
 decPart[ 2].decay[4].pID[1] = getPid ("pi+");
 decPart[ 2].decay[4].pID[2] = getPid ("pi-");
 decPart[ 2].decay[4].pID[3] = getPid ("e+");
 decPart[ 2].decay[4].pID[4] = getPid ("e-");
 
 decPart[ 3].pID = getPid ("K0S");
 decPart[ 3].cTau = 2.6762;
 decPart[ 3].decaysNmb = 1;
 decPart[ 3].decay[1].prong = 2;
 decPart[ 3].decay[1].pID[1] = getPid ("pi+");
 decPart[ 3].decay[1].pID[2] = getPid ("pi-");
 
 decPart[ 4].pID = getPid ("Lamb");
 decPart[ 4].cTau = 7.89;
 decPart[ 4].decaysNmb = 1;
 decPart[ 4].decay[1].prong = 2;
 decPart[ 4].decay[1].pID[1] = getPid ("p");
 decPart[ 4].decay[1].pID[2] = getPid ("pi-");
 
 decPart[ 5].pID = getPid ("Sig+");
 decPart[ 5].cTau = 2.396;
 decPart[ 5].decaysNmb = 1;
 decPart[ 5].decay[1].prong = 2;
 decPart[ 5].decay[1].pID[1] = getPid ("n");
 decPart[ 5].decay[1].pID[2] = getPid ("pi+");
 
 decPart[ 6].pID = getPid ("Sig0");
 decPart[ 6].cTau = 2.22e-9;
 decPart[ 6].decaysNmb = 1;
 decPart[ 6].decay[1].prong = 3;
 decPart[ 6].decay[1].pID[1] = getPid ("p");
 decPart[ 6].decay[1].pID[2] = getPid ("pi-");
 decPart[ 6].decay[1].pID[3] = getPid ("g");
 
 decPart[ 7].pID = getPid ("Sig-");
 decPart[ 7].cTau = 4.434;
 decPart[ 7].decaysNmb = 1;
 decPart[ 7].decay[1].prong = 2;
 decPart[ 7].decay[1].pID[1] = getPid ("n");
 decPart[ 7].decay[1].pID[2] = getPid ("pi-");
 
 decPart[ 8].pID = getPid ("Xi0");
 decPart[ 8].cTau = 8.71;
 decPart[ 8].decaysNmb = 1;
 decPart[ 8].decay[1].prong = 3;
 decPart[ 8].decay[1].pID[1] = getPid ("p");
 decPart[ 8].decay[1].pID[2] = getPid ("pi-");
 decPart[ 8].decay[1].pID[3] = getPid ("pi0_");
 
 decPart[ 9].pID = getPid ("Xi-");
 decPart[ 9].cTau = 4.91;
 decPart[ 9].decaysNmb = 1;
 decPart[ 9].decay[1].prong = 3;
 decPart[ 9].decay[1].pID[1] = getPid ("p");
 decPart[ 9].decay[1].pID[2] = getPid ("pi-");
 decPart[ 9].decay[1].pID[3] = getPid ("pi-");
 
 decPart[10].pID = getPid ("Omg-");
 decPart[10].cTau = 2.46;
 decPart[10].decaysNmb = 2;
 decPart[10].decay[1].prong = 3;
 decPart[10].decay[1].pID[1] = getPid ("p");
 decPart[10].decay[1].pID[2] = getPid ("pi-");
 decPart[10].decay[1].pID[3] = getPid ("K-");
 decPart[10].decay[2].prong = 4;
 decPart[10].decay[2].pID[1] = getPid ("p");
 decPart[10].decay[2].pID[2] = getPid ("pi-");
 decPart[10].decay[2].pID[3] = getPid ("pi-");
 decPart[10].decay[2].pID[4] = getPid ("pi0_");

 decPart[11].pID = getPid ("~Lamb");
 decPart[11].cTau = 7.89;
 decPart[11].decaysNmb = 1;
 decPart[11].decay[1].prong = 2;
 decPart[11].decay[1].pID[1] = getPid ("~p");
 decPart[11].decay[1].pID[2] = getPid ("pi+");

 decPart[12].pID = getPid ("~Sig-");
 decPart[12].cTau = 4.434;
 decPart[12].decaysNmb = 1;
 decPart[12].decay[1].prong = 2;
 decPart[12].decay[1].pID[1] = getPid ("~n");
 decPart[12].decay[1].pID[2] = getPid ("pi+");

 decPart[13].pID = getPid ("~Sig0");
 decPart[13].cTau = 2.22e-9;
 decPart[13].decaysNmb = 1;
 decPart[13].decay[1].prong = 3;
 decPart[13].decay[1].pID[1] = getPid ("~p");
 decPart[13].decay[1].pID[2] = getPid ("pi+");
 decPart[13].decay[1].pID[3] = getPid ("g");

 decPart[14].pID = getPid ("~Sig+");
 decPart[14].cTau = 2.396;
 decPart[14].decaysNmb = 1;
 decPart[14].decay[1].prong = 2;
 decPart[14].decay[1].pID[1] = getPid ("~n");
 decPart[14].decay[1].pID[2] = getPid ("pi-");

 decPart[15].pID = getPid ("~Xi0");
 decPart[15].cTau = 8.71;
 decPart[15].decaysNmb = 1;
 decPart[15].decay[1].prong = 3;
 decPart[15].decay[1].pID[1] = getPid ("~p");
 decPart[15].decay[1].pID[2] = getPid ("pi+");
 decPart[15].decay[1].pID[3] = getPid ("pi0_");

 decPart[16].pID = getPid ("~Xi-");
 decPart[16].cTau = 4.91;
 decPart[16].decaysNmb = 1;
 decPart[16].decay[1].prong = 3;
 decPart[16].decay[1].pID[1] = getPid ("~p");
 decPart[16].decay[1].pID[2] = getPid ("pi+");
 decPart[16].decay[1].pID[3] = getPid ("pi+");

 decPart[17].pID = getPid ("~Omg-");
 decPart[17].cTau = 2.46;
 decPart[17].decaysNmb = 2;
 decPart[17].decay[1].prong = 3;
 decPart[17].decay[1].pID[1] = getPid ("~p");
 decPart[17].decay[1].pID[2] = getPid ("pi+");
 decPart[17].decay[1].pID[3] = getPid ("K+");
 decPart[17].decay[2].prong = 4;
 decPart[17].decay[2].pID[1] = getPid ("~p");
 decPart[17].decay[2].pID[2] = getPid ("pi+");
 decPart[17].decay[2].pID[3] = getPid ("pi+");
 decPart[17].decay[2].pID[4] = getPid ("pi0_");

 decPart[18].pID = getPid ("rho+");
 decPart[18].cTau = 0.001;
 decPart[18].decaysNmb = 2;
 decPart[18].decay[1].prong = 2;
 decPart[18].decay[1].pID[1] = getPid ("pi+");
 decPart[18].decay[1].pID[2] = getPid ("pi0_");
 decPart[18].decay[2].prong = 2;
 decPart[18].decay[2].pID[1] = getPid ("pi+");
 decPart[18].decay[2].pID[2] = getPid ("g");

 decPart[19].pID = getPid ("rho-");
 decPart[19].cTau = 0.001;
 decPart[19].decaysNmb = 2;
 decPart[19].decay[1].prong = 2;
 decPart[19].decay[1].pID[1] = getPid ("pi-");
 decPart[19].decay[1].pID[2] = getPid ("pi0_");
 decPart[19].decay[2].prong = 2;
 decPart[19].decay[2].pID[1] = getPid ("pi-");
 decPart[19].decay[2].pID[2] = getPid ("g");

 decPart[20].pID = getPid ("rho0");
 decPart[20].cTau = 0.001;
 decPart[20].decaysNmb = 2;
 decPart[20].decay[1].prong = 2;
 decPart[20].decay[1].pID[1] = getPid ("pi+");
 decPart[20].decay[1].pID[2] = getPid ("pi-");
 decPart[20].decay[2].prong = 3;
 decPart[20].decay[2].pID[1] = getPid ("pi+");
 decPart[20].decay[2].pID[2] = getPid ("pi-");
 decPart[20].decay[2].pID[3] = getPid ("g");

 decPart[21].pID = getPid ("w");
 decPart[21].cTau = 0.001;
 decPart[21].decaysNmb = 2;
 decPart[21].decay[1].prong = 3;
 decPart[21].decay[1].pID[1] = getPid ("pi+");
 decPart[21].decay[1].pID[2] = getPid ("pi-");
 decPart[21].decay[1].pID[3] = getPid ("pi0_");
 decPart[21].decay[2].prong = 2;
 decPart[21].decay[2].pID[1] = getPid ("pi+");
 decPart[21].decay[2].pID[2] = getPid ("pi-");

 decPart[22].pID = getPid ("D++");
 decPart[22].cTau = 0.001;
 decPart[22].decaysNmb = 1;
 decPart[22].decay[1].prong = 2;
 decPart[22].decay[1].pID[1] = getPid ("p");
 decPart[22].decay[1].pID[2] = getPid ("pi+");

 decPart[23].pID = getPid ("D+");
 decPart[23].cTau = 0.001;
 decPart[23].decaysNmb = 3;
 decPart[23].decay[1].prong = 2;
 decPart[23].decay[1].pID[1] = getPid ("p");
 decPart[23].decay[1].pID[2] = getPid ("pi0_");
 decPart[23].decay[2].prong = 2;
 decPart[23].decay[2].pID[1] = getPid ("p");
 decPart[23].decay[2].pID[2] = getPid ("g");
 decPart[23].decay[3].prong = 2;
 decPart[23].decay[3].pID[1] = getPid ("n");
 decPart[23].decay[3].pID[2] = getPid ("pi+");

 decPart[24].pID = getPid ("D0");
 decPart[24].cTau = 0.001;
 decPart[24].decaysNmb = 1;
 decPart[24].decay[1].prong = 2;
 decPart[24].decay[1].pID[1] = getPid ("p");
 decPart[24].decay[1].pID[2] = getPid ("pi-");

 decPart[25].pID = getPid ("D-");
 decPart[25].cTau = 0.001;
 decPart[25].decaysNmb = 1;
 decPart[25].decay[1].prong = 2;
 decPart[25].decay[1].pID[1] = getPid ("n");
 decPart[25].decay[1].pID[2] = getPid ("pi-");

 decPart[26].pID = getPid ("NP11+");
 decPart[26].cTau = 0.001;
 decPart[26].decaysNmb = 8;
 decPart[26].decay[1].prong = 2;
 decPart[26].decay[1].pID[1] = getPid ("p");
 decPart[26].decay[1].pID[2] = getPid ("pi0_");
 decPart[26].decay[2].prong = 2;
 decPart[26].decay[2].pID[1] = getPid ("n");
 decPart[26].decay[2].pID[2] = getPid ("pi+");
 decPart[26].decay[3].prong = 3;
 decPart[26].decay[3].pID[1] = getPid ("p");
 decPart[26].decay[3].pID[2] = getPid ("pi+");
 decPart[26].decay[3].pID[3] = getPid ("pi-");
 decPart[26].decay[4].prong = 2;
 decPart[26].decay[4].pID[1] = getPid ("D+");
 decPart[26].decay[4].pID[2] = getPid ("pi0_");
 decPart[26].decay[5].prong = 2;
 decPart[26].decay[5].pID[1] = getPid ("D++");
 decPart[26].decay[5].pID[2] = getPid ("pi-");
 decPart[26].decay[6].prong = 2;
 decPart[26].decay[6].pID[1] = getPid ("D0");
 decPart[26].decay[6].pID[2] = getPid ("pi+");
 decPart[26].decay[7].prong = 2;
 decPart[26].decay[7].pID[1] = getPid ("p");
 decPart[26].decay[7].pID[2] = getPid ("rho0");
 decPart[26].decay[8].prong = 2;
 decPart[26].decay[8].pID[1] = getPid ("n");
 decPart[26].decay[8].pID[2] = getPid ("rho+");
 
 decPart[27].pID = getPid ("ND13+");
 decPart[27].cTau = 0.001;
 decPart[27].decaysNmb = 8;
 decPart[27].decay[1].prong = 2;
 decPart[27].decay[1].pID[1] = getPid ("p");
 decPart[27].decay[1].pID[2] = getPid ("pi0_");
 decPart[27].decay[2].prong = 2;
 decPart[27].decay[2].pID[1] = getPid ("n");
 decPart[27].decay[2].pID[2] = getPid ("pi+");
 decPart[27].decay[3].prong = 3;
 decPart[27].decay[3].pID[1] = getPid ("p");
 decPart[27].decay[3].pID[2] = getPid ("pi+");
 decPart[27].decay[3].pID[3] = getPid ("pi-");
 decPart[27].decay[4].prong = 2;
 decPart[27].decay[4].pID[1] = getPid ("D+");
 decPart[27].decay[4].pID[2] = getPid ("pi0_");
 decPart[27].decay[5].prong = 2;
 decPart[27].decay[5].pID[1] = getPid ("D++");
 decPart[27].decay[5].pID[2] = getPid ("pi-");
 decPart[27].decay[6].prong = 2;
 decPart[27].decay[6].pID[1] = getPid ("D0");
 decPart[27].decay[6].pID[2] = getPid ("pi+");
 decPart[27].decay[7].prong = 2;
 decPart[27].decay[7].pID[1] = getPid ("p");
 decPart[27].decay[7].pID[2] = getPid ("rho0");
 decPart[27].decay[8].prong = 2;
 decPart[27].decay[8].pID[1] = getPid ("n");
 decPart[27].decay[8].pID[2] = getPid ("rho+");
 
 decPart[28].pID = getPid ("NS11+");
 decPart[28].cTau = 0.001;
 decPart[28].decaysNmb = 9;
 decPart[28].decay[1].prong = 2;
 decPart[28].decay[1].pID[1] = getPid ("p");
 decPart[28].decay[1].pID[2] = getPid ("pi0_");
 decPart[28].decay[2].prong = 2;
 decPart[28].decay[2].pID[1] = getPid ("n");
 decPart[28].decay[2].pID[2] = getPid ("pi+");
 decPart[28].decay[3].prong = 3;
 decPart[28].decay[3].pID[1] = getPid ("p");
 decPart[28].decay[3].pID[2] = getPid ("pi+");
 decPart[28].decay[3].pID[3] = getPid ("pi-");
 decPart[28].decay[4].prong = 2;
 decPart[28].decay[4].pID[1] = getPid ("D+");
 decPart[28].decay[4].pID[2] = getPid ("pi0_");
 decPart[28].decay[5].prong = 2;
 decPart[28].decay[5].pID[1] = getPid ("D++");
 decPart[28].decay[5].pID[2] = getPid ("pi-");
 decPart[28].decay[6].prong = 2;
 decPart[28].decay[6].pID[1] = getPid ("D0");
 decPart[28].decay[6].pID[2] = getPid ("pi+");
 decPart[28].decay[7].prong = 2;
 decPart[28].decay[7].pID[1] = getPid ("p");
 decPart[28].decay[7].pID[2] = getPid ("rho0");
 decPart[28].decay[8].prong = 2;
 decPart[28].decay[8].pID[1] = getPid ("n");
 decPart[28].decay[8].pID[2] = getPid ("rho+");
 decPart[28].decay[9].prong = 2;
 decPart[28].decay[9].pID[1] = getPid ("p");
 decPart[28].decay[9].pID[2] = getPid ("eta");
 
}

Bool_t HChannelListMaker::newChannelFound (Double_t newCodeOut)
{Bool_t result;
 Int_t k;
 result = true;
 for (k = 1; k <= num; k++) if (newCodeOut == chList[idx].ch[k].code) result = false;
 return result;
}            

void HChannelListMaker::sortDigit (Int_t n, Int_t* A)
 {Int_t i,j,t;
  for (i = n-1; i >= 1; i--)
   for (j = 1; j <= i; j++)
    if (A[j] > A[j+1]) {t = A[j]; A[j] = A[j+1]; A[j+1] = t;}
 }
 
void HChannelListMaker::addNewChannel (Int_t prong, Int_t* digit, Double_t newCodeOut, 
     tChannelType channelType, Float_t massSum, FILE* otp)
 {Int_t i;
  num++; 
  chList[idx].chNmb = num;
  chList[idx].ch[num].channelType = channelType;
  chList[idx].ch[num].prong       = prong;
  chList[idx].ch[num].code        = newCodeOut;
  for (i = 1; i <= prong; i++) chList[idx].ch[num].pID[i]       = digit[i];
  if (channelType == neutral)  chList[idx].ch[num].pID[prong+1] = neutralIndex;
        
  //output to text file:
  Int_t j;
  fprintf (otp,"%6i %7.3f  ->  ",num,massSum);
  for (i = 1; i <= prong; i++)
   {fprintf (otp,"%s",Phys.P[ndex[i]].name);
    for (j = 1; j <= int(nameLength-strlen(Phys.P[ndex[i]].name)); j++) fprintf (otp," ");
   }
  switch (channelType)
   {case charged: for (j = 1; j <= int(nameLength); j++) fprintf (otp," "); break;
    case neutral: fprintf (otp,"%s",Phys.P[neutralIndex].name);
                  for (j = 1; j <= int(nameLength-strlen(Phys.P[neutralIndex].name)); j++) 
                  fprintf (otp," ");
                  break;
   }
   fprintf (otp,"\n"); 
//   //output to screen:
//   printf ("%6i %7.3f  ->  ",num,massSum);
//   for (i = 1; i <= prong; i++)
//    {
//     printf ("%s",Phys.P[ndex[i]].name);
//     for (j = 1; j <= int(nameLength-strlen(Phys.P[ndex[i]].name)); j++) printf (" ");
//    }
//   if (channelType == neutral) printf ("%s",Phys.P[neutralIndex].name);
//   printf ("\n");
 } 

void HChannelListMaker::copyChString (Int_t idxFrom, Int_t chNFrom, Int_t idxTo, Int_t chNTo)
{Int_t maxP,k;
 maxP = chList[idxFrom].ch[chNFrom].prong;
 chList[idxTo].ch[chNTo].prong = maxP;
 for (k = 1; k <= maxP; k++)
  chList[idxTo].ch[chNTo].pID[k] = chList[idxFrom].ch[chNFrom].pID[k];
}        

void HChannelListMaker::printChString (Char_t* marker, Int_t idx, Int_t chN, FILE* otp)
{Int_t k;
 fprintf (otp,"%3i:  %s ",chN,marker);
 for (k = 1; k <= chList[idx].ch[chN].prong; k++) 
  fprintf (otp," %s",Phys.P[chList[idx].ch[chN].pID[k]].name);
 fprintf (otp,"\n");
}
         
void HChannelListMaker::typeChString (Char_t* marker, Int_t idx, Int_t chN)
{Int_t k;
 printf ("%3i:  %s ",chN,marker);
 for (k = 1; k <= chList[idx].ch[chN].prong; k++) 
  printf (" %s",Phys.P[chList[idx].ch[chN].pID[k]].name);
 printf ("\n");
}
         
void HChannelListMaker::shiftParticleToEnde (Int_t idxNew, Int_t newChN, Int_t pos)
{Int_t i,j,pid,prong;
 j = 0;
 pid   = chList[idxNew].ch[newChN].pID[pos];
 prong = chList[idxNew].ch[newChN].prong;
 for (i = 1; i <= chList[idxNew].ch[newChN].prong; i++)
  if (i != pos)
   {j++;
    chList[idxNew].ch[newChN].pID[j] = chList[idxNew].ch[newChN].pID[i];
   }
 chList[idxNew].ch[newChN].pID[prong] = pid;
}     
       
void HChannelListMaker::printChannelCodes (Int_t idx, Int_t chN, FILE* otp)
{
 Int_t k,maxP;
 fprintf (otp,"%3i: %18.0f -> ",chN,chList[idx].ch[chN].code);
 maxP = chList[idx].ch[chN].prong;
 for (k = 1; k <= maxP; k++)
    fprintf (otp," %s",Phys.P[chList[idx].ch[chN].pID[k]].name);
 if (chList[idx].ch[chN].channelType == neutral) 
  {fprintf (otp," %s",Phys.P[chList[idx].ch[chN].pID[maxP+1]].name);
   if (Phys.P[chList[idx].ch[chN].pID[maxP+1]].charge != 0) fprintf (otp,"'");
  }
 fprintf (otp,"\n");
}   
 
void HChannelListMaker::printChannelList (Int_t idx, FILE* otp)
{
 Int_t i;
 if (chList[idx].chNmb > 0)
   for (i = 1; i <= chList[idx].chNmb; i++) printChannelCodes (idx,i,otp);
  else 
   {
    fprintf (otp,"channel list %i is empty\n",idx);
     printf (    "channel list %i is empty\n",idx);
   }
}

void HChannelListMaker::typeChannelCodes (Int_t idx, Int_t chN)
{Int_t k,maxP;
 printf ("%3i: %18.0f -> ",chN,chList[idx].ch[chN].code);
 maxP = chList[idx].ch[chN].prong;
 for (k = 1; k <= maxP; k++)
    printf (" %s",Phys.P[chList[idx].ch[chN].pID[k]].name);
 if (chList[idx].ch[chN].channelType == neutral) 
  {printf (" %s",Phys.P[chList[idx].ch[chN].pID[maxP+1]].name);
   if (Phys.P[chList[idx].ch[chN].pID[maxP+1]].charge != 0) printf ("'");
  }
 printf ("\n");
}   
 
void HChannelListMaker::typeChannelList (Int_t idx)
{Int_t i;
 if (chList[idx].chNmb > 0)
   for (i = 1; i <= chList[idx].chNmb; i++) typeChannelCodes (idx,i);
  else 
   {
     printf (    "channel list %i is empty\n",idx);
   }
}

void HChannelListMaker::copyChannel (tChannel* chFrom, tChannel* chTo)
{Int_t maxP,i;
 maxP = chFrom->prong;
 if (chFrom->channelType == neutral) maxP++;
 for (i = 1; i <= maxP; i++) chTo->pID[i] = chFrom->pID[i];
 chTo->channelType = chFrom->channelType;
 chTo->prong       = chFrom->prong;
 chTo->code        = chFrom->code;
} 
  
Bool_t HChannelListMaker::validParticle (Int_t pid)
{Bool_t result;
 result = kTRUE;
 if      (strcoll(Phys.P[pid].name,"TC"   ) == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"D0"   ) == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"D++"  ) == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"D+"   ) == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"D-"   ) == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"NP11+") == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"ND13+") == 0) result = kFALSE;
// else if (strcoll(Phys.P[pid].name,"NS11+") == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"pion" ) == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"d"    ) == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"t"    ) == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"alpha") == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"kaon" ) == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"He3"  ) == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"dimu" ) == 0) result = kFALSE;
 else if (strcoll(Phys.P[pid].name,"dilep") == 0) result = kFALSE;
 return result;
}

Int_t HChannelListMaker::getNextCharged (Int_t pid)
{Int_t  result,pidTemp;
 Bool_t endFlag;
 result  = 0;
 endFlag = kFALSE;
 pidTemp = pid;
 while (!endFlag)
  if (pidTemp < Phys.particles)
   {pidTemp++;
    if (Phys.P[pidTemp].charge != 0)
     {result = pidTemp;
      endFlag = kTRUE;
     }
   }
   else endFlag = kTRUE;
 return result;
}

void HChannelListMaker::addPart (Int_t partIndex, Int_t* dim, Int_t* pidList)
{Bool_t found;
 Int_t i;
 found = kFALSE;
 i = 0;
 while (!found && (i < *dim))
  {if (partIndex == pidList[i]) found = kTRUE; 
   i++;
  }
 if (!found)
  {pidList[*dim] = partIndex; (*dim)++;
//    printf (" -> %2i",partIndex);
  }
}

//-------------------------------------------------------------------------------------------
Double_t HChannelListMaker::getChannelCode (Int_t prong, Int_t* digit, Int_t missaPid)
{// digit array should be sorted
 Int_t tNo;
 Double_t chC;
 chC = 0;
 for (tNo = 1; tNo <= prong; tNo++) chC = (chC + digit[tNo])*codeMul; // channel chC code habemus
 if (missaPid != 0) chC = chC + missaPid;
 return chC;
}

Int_t HChannelListMaker::getMissaPid (Double_t chCode)
{return (int)(chCode - ((int)(chCode/codeMul))*codeMul);
}

//-------------------------------------------------------------------------------------------
Bool_t HChannelListMaker::makeChannelList 
    (Int_t maxProng, Int_t beamN, Float_t Ek, Int_t targN,
     Float_t regDist, Bool_t allChargedParticlesAreRegistered,
     Int_t maxChNmb, Int_t* numberOfChannels, Double_t* chCodeList, TString* chLineList,
     Int_t* pNmb, Int_t* posPID, Int_t* nNmb, Int_t* negPID, Int_t* tNmb, Int_t* totPID)
{
 const Float_t dm  = 0.0005;  //GeV, so (neutron.mass - proton.mass) ~ 0.001294 
 const Float_t me  = 0.000511;
 const Float_t mmu = 0.105658;
 Float_t     dme,dmmu;
 Float_t     mb,Eb,pb,mt,Et,pt,ms,Es,ps;
 Float_t     gamma,beta,EbCMS,EtCMS,ECMS;
 Int_t       chargeIn ,leptonIn ,baryonIn ,strangeIn;
 Int_t       lepton,chargeOut,leptonOut,baryonOut,strangeOut;
 Int_t       leptonNeu,baryonNeu,strangeNeu;
 Int_t       electron,muon;
 Float_t     mass,massOut,massNeu;
 Int_t       i,pid,posNum,nextChargedPid,initialChargedPid;
 Bool_t      enough,prongCharged,newCombFound,result; // possible,
 Int_t       digit[limProng];
 Int_t       tempLength,fullLength;
 Int_t       gsiPid[max_particles]; // limit see in hMdcBljvLib
 
 FILE*       otp;
 Char_t*       chListMaker_file_name = "channelListMaker.txt";
 
 result = kTRUE;
 
 if ( (otp = fopen (chListMaker_file_name, "wt")) == NULL )
  {fprintf (stderr,"'%s' open file error \n",chListMaker_file_name); result = kFALSE;}
  else
  {   
   // It is need to use the internal particle table due to manipulation with the 'stable' "pi0_" particle
   bLib.fillPhysCnst(&Phys,otp);
   fullLength = Phys.particles;
//    printf ("fullLength %i\n",fullLength);
//    fullLength = 32;
    fprintf (otp,"Full GSI particle list:\n");
    bLib.prntPhys    (&Phys,otp);

   tempLength = 0;
   for (pid = 1; pid <= fullLength; pid++)
     if (validParticle(pid))
      {tempLength++;
       sprintf (Phys.P[tempLength].name,"%s",     Phys.P[pid].name);
                Phys.P[tempLength].mass         = Phys.P[pid].mass;
                Phys.P[tempLength].charge       = Phys.P[pid].charge;
                Phys.P[tempLength].leptonCharge = Phys.P[pid].leptonCharge;
                Phys.P[tempLength].baryonCharge = Phys.P[pid].baryonCharge;
                Phys.P[tempLength].strange      = Phys.P[pid].strange;
       gsiPid[tempLength] = pid;
      }
   Phys.particles = tempLength;
//    printf ("Phys.particles %i\n",Phys.particles);
   fprintf (otp,"Particle in use list:\n");
   bLib.prntPhys    (&Phys,otp);
//    bLib.typePhys    (&Phys    );
   
   fprintf (otp,"GSI pid's list\n");
    for (pid = 1; pid <= Phys.particles; pid++)
    {
//      printf (    "%3i gsiPid %2i '%s'\n",pid,gsiPid[pid],Phys.P[pid].name);
     fprintf (otp,"%3i gsiPid %2i '%s'\n",pid,gsiPid[pid],Phys.P[pid].name);
    }
   fprintf (otp,"\n");

   mb = HBPidPhysicsConstants::mass(beamN)/1000.0;
   Eb = Ek + mb;
   pb = sqrt(sqr(Eb) - sqr(mb));
   mt = HBPidPhysicsConstants::mass(targN)/1000.0;

   Et = mt;
   pt = 0;

   ps = pb;
   Es = Eb + Et;
   ms = sqrt(sqr(mb) + sqr(mt) + 2*Eb*mt);
   gamma = Es/ms;
   beta  = ps/Es;
   EbCMS = gamma*(Eb - beta*pb);
   EtCMS = gamma*(Et - beta*pt);
   ECMS  = EbCMS + EtCMS;

   chargeIn  = HBPidPhysicsConstants::charge      (beamN) + HBPidPhysicsConstants::charge      (targN);
   leptonIn  = HBPidPhysicsConstants::leptonCharge(beamN) + HBPidPhysicsConstants::leptonCharge(targN);
   baryonIn  = HBPidPhysicsConstants::baryonCharge(beamN) + HBPidPhysicsConstants::baryonCharge(targN);
   strangeIn = HBPidPhysicsConstants::strangeness (beamN) + HBPidPhysicsConstants::strangeness (targN);

   fprintf (otp,"Collision: %s %s ->\n",
       HBPidPhysicsConstants::pid(beamN),HBPidPhysicsConstants::pid(targN));
   fprintf (otp,"beam   particle '%s', mass %f\n",HBPidPhysicsConstants::pid(beamN),mb);
   fprintf (otp,"target particle '%s', mass %f\n",HBPidPhysicsConstants::pid(targN),mt);
   fprintf (otp,"Beam kinetic  energy %f (GeV)\n",Ek);
   fprintf (otp,"Beam momentum        %f (GeV/c)\n",pb);
   fprintf (otp,"CMS system    energy %f (GeV)\n",ECMS);
   fprintf (otp,"System electricity charge %i\n",chargeIn);
   fprintf (otp,"System lepton      charge %i\n",leptonIn);
   fprintf (otp,"System baryon      charge %i\n",baryonIn);
   fprintf (otp,"System strange            %i\n",strangeIn);

//     printf (    "Collision: %s %s ->\n",
//        HBPidPhysicsConstants::pid(beamN),HBPidPhysicsConstants::pid(targN)); // Phys.P[beamN].name,Phys.P[targN].name);
//     printf (    "beam   particle '%s', mass %f\n",HBPidPhysicsConstants::pid(beamN),mb); // Phys.P[beamN].name,mb);
//     printf (    "target particle '%s', mass %f\n",HBPidPhysicsConstants::pid(targN),mt); // Phys.P[targN].name,mt);
//     printf (    "Beam kinetic  energy %f (GeV)\n",Ek);
//     printf (    "Beam momentum        %f (GeV/c)\n",pb);
//     printf (    "CMS system    energy %f (GeV)\n",ECMS);
//     printf (    "System electricity charge %i\n",chargeIn);
//     printf (    "System lepton      charge %i\n",leptonIn);
//     printf (    "System baryon      charge %i\n",baryonIn);
//     printf (    "System strange            %i\n",strangeIn);

   fprintf (otp,"\nChannels possible in vertex:\n");
//     printf ("making channels possible in vertex...\n");

   num = 0; // number of channels
   idx = 0;
   chList[idx].chNmb = 0;
   for (prong = 1; prong <= maxProng; prong++) 
    {fprintf (otp," multiplicity %i\n",prong);
//       printf (    " multiplicity %i\n",prong);
     if      (chargeIn >  0)  posNum = (prong + 2)/2;
     else if (chargeIn == 0)  posNum = (prong    )/2;
     else   /*chargeIn >  0*/ posNum = (prong - 1)/2;
     if ( (prong%2) == (abs(chargeIn)%2) ) 
      {fprintf (otp,"  positive particles %i\n",posNum); 
       // initial particle combination:
       initialChargedPid = getNextCharged (0);
       if (initialChargedPid <= 0) {printf ("no charged particles found\n"); abort ();}
       for (i = 1; i <= prong; i++) ndex[i] = initialChargedPid; // 1;
//        printf ("%s\n",Phys.P[ndex[1]].name);
       enough = false;
       while (!enough)
        {chargeOut  = 0;
         for (i = 1; i <= prong; i++)
          chargeOut = chargeOut + Phys.P[ndex[i]].charge;
         if (chargeIn == chargeOut)
          {leptonOut  = 0;
           baryonOut  = 0;
           strangeOut = 0;
           electron   = 0;
           muon       = 0;
           massOut    = 0;
           prongCharged = true;
           i = 1;
           while ( prongCharged && (i <= prong) )
            if ( ( (i <= posNum) && (Phys.P[ndex[i]].charge > 0) ) || 
                 ( (i >  posNum) && (Phys.P[ndex[i]].charge < 0) )    )
             {
              lepton = Phys.P[ndex[i]].leptonCharge;
              mass   = Phys.P[ndex[i]].mass;
              leptonOut  = leptonOut  + lepton;
              baryonOut  = baryonOut  + Phys.P[ndex[i]].baryonCharge;
              strangeOut = strangeOut + Phys.P[ndex[i]].strange;
              massOut    = massOut    + mass;
              if      (lepton != 0)
               {dme  = fabs(mass - me);
                dmmu = fabs(mass - mmu);
                if (lepton > 0)
                 {if      (dme  < dm) electron++;
                  else if (dmmu < dm) muon++;    
                 }
                 else if (lepton < 0)
                  {if      (dme  < dm) electron--;
                   else if (dmmu < dm) muon--;    
                  }
               }
              i++;
             }
             else prongCharged = false;

           if (prongCharged && (electron == 0) && (muon == 0))
            {for (i = 1; i <= prong; i++) digit[i] = ndex[i];
             sortDigit (prong,digit);
             newCode = getChannelCode (prong,digit,0);
             //is this 0-channel possible?
             if ( (leptonIn  == leptonOut)  &&
                  (baryonIn  == baryonOut)  &&
                  (strangeIn == strangeOut) &&
                  (ECMS      >= massOut)       )
               if (newChannelFound(newCode)) addNewChannel (prong,digit,newCode,charged,massOut,otp);

             for (neutralIndex = 1; neutralIndex <= Phys.particles; neutralIndex++)
              if (Phys.P[neutralIndex].charge == 0) 
               {//is this neutral channel possible?
                leptonNeu  = leptonOut  + Phys.P[neutralIndex].leptonCharge;
                baryonNeu  = baryonOut  + Phys.P[neutralIndex].baryonCharge;
                strangeNeu = strangeOut + Phys.P[neutralIndex].strange;
                massNeu    = massOut    + Phys.P[neutralIndex].mass;
                if ( (leptonIn  == leptonNeu)  &&
                     (baryonIn  == baryonNeu)  &&
                     (strangeIn == strangeNeu) &&
                     (ECMS      >= massNeu)       )
                 {newCodeOut = newCode + neutralIndex;
                  if (newChannelFound(newCodeOut)) addNewChannel (prong,digit,newCodeOut,neutral,massNeu,otp);
                 }
              } 
            }
          }
         //switch to new particle combination: 
         i = prong; newCombFound = kFALSE;
         while ( (i > 0) && !newCombFound )
          {nextChargedPid = getNextCharged (ndex[i]);
           if (nextChargedPid != 0)
            {ndex[i] = nextChargedPid; newCombFound = kTRUE;
//              if (i == 1) printf ("%s\n",Phys.P[ndex[1]].name);
            }
             else
              if (i > 1) {ndex[i] = initialChargedPid; i--;} // carry out // ndex[i] = 1;
               else      {newCombFound = kTRUE; enough = true;}
          }
        }
       fprintf (otp,"--------\n");
//         printf (    "--------\n");
      }
      else {fprintf (otp,"  charge violation\n"); 
//              printf (    "  charge violation\n");
           }
    }
   fprintf (otp,"--------\n");

   fprintf (otp,"\nPrimary possible channels (codes):\n");
   printChannelList (idx,otp);
//     printf (    "\nPrimary possible channels (codes) made (%i)\n",chList[idx].chNmb);
//    typeChannelList (idx);

  // If primary channels are need, program may be stopped.
  // else // all Short_t time particles should be changed with their products

  //pi0
  // It's need to use the internal particle table due to manipulation with the 'stable' "pi0_" particle
   fprintf (otp,"temporary adding 'stable pi0' to particle list:\n"); 
   Int_t pi0Pos,pi0StablePos;
   Phys.particles++;
   pi0StablePos = Phys.particles;
   pi0Pos = getPid ("pi0"); 
   sprintf (Phys.P[pi0StablePos].name,"%s%s",Phys.P[pi0Pos].name,"_");
   Phys.P[pi0StablePos].charge = Phys.P[pi0Pos].charge; // = 0
   // other fields of Phys.P[pi0StablePos] are unimportant here
   
   fprintf (otp,"\nParticle with pi0_ table:\n");
   bLib.prntPhys    (&Phys,otp);

   getDecayList (); // (otp); // decay list should contains only stable particles as decay products
   // for example pi0_ (not pi0)

   Bool_t allChanged,foundDecayed,foundAtLeastOnce,overFlow;
   Int_t  mal,kk,ll,prongNew,chN,newChN,dpN,nd,pID,maxP,neutraNmb,replN;
  // Double_t code;
   fprintf (otp,"\nreplacing Short_t life time particles with stable ones:\n");  
//     printf (    "replacing Short_t life time particles with stable ones...\n");  
  //  here 
  //   1) chList[...].ch[...].prong 
  //      must temporary designate number of ALL particle (rather then charged ones)
  //   2) chList[...].ch[...].channelType and chList[...].ch[...].code are both undefined
   for (chN = 1; chN <= chList[idx].chNmb; chN++)
    if (chList[idx].ch[chN].channelType == neutral) chList[idx].ch[chN].prong++;

   allChanged = false;
   idxNew = 1 - idx;
   mal = 0;
   while (!allChanged)
    {foundAtLeastOnce = false; newChN = 0;
     for (chN = 1; chN <= chList[idx].chNmb; chN++) 
      {
       fprintf (otp,"\n");
       printChString ("==",idx,chN,otp);
//         printf ("\n");
//        typeChString ("==",idx,chN);
       replN = 0; foundDecayed = false; dpN = 0; // to prevent warning
       maxP = chList[idx].ch[chN].prong;
       while (!foundDecayed && (replN < maxP))
        {replN++; pID = chList[idx].ch[chN].pID[replN];
         dpN = 0;
         while (!foundDecayed && (dpN < decPartsNmb)) 
          {dpN++; 
           if ((decPart[dpN].pID == pID) && (decPart[dpN].cTau < regDist))
              foundDecayed = true;
          }
        }
       if (foundDecayed) // having replN - number of replacable Short_t time particle
        {foundAtLeastOnce = true; 
         for (nd = 1; nd <= decPart[dpN].decaysNmb; nd++) // producing decPart[dpN].decaysNmb new channels
           if (newChN < chNmbMaxTmp)
            {
             newChN++; 
             prongNew = replN - 1;
             for (kk = 1; kk <= prongNew; kk++) 
              chList[idxNew].ch[newChN].pID[kk] = chList[idx].ch[chN].pID[kk];
             ll = 0; 
             overFlow = false;
             while (!overFlow && (ll < decPart[dpN].decay[nd].prong))
              {if (prongNew <= maxProng)
                {prongNew++; ll++;
                 pID = decPart[dpN].decay[nd].pID[ll];
                 chList[idxNew].ch[newChN].pID[prongNew] = pID;
                }
               else overFlow = true;
              }
             if (!overFlow)
              {kk = replN;
               while (!overFlow && kk < maxP)
                {if (prongNew <= maxProng)
                  {prongNew++; kk++;
                   pID = chList[idx].ch[chN].pID[kk];
                   chList[idxNew].ch[newChN].pID[prongNew] = pID;
                  }
                  else overFlow = true;
                }
               if (!overFlow) 
                {chList[idxNew].ch[newChN].prong = prongNew;
                 printChString ("->",idxNew,newChN,otp);
//                    typeChString  ("->",idxNew,newChN);
                }
                else newChN--;
              }
             else newChN--;
            }
            else 
             {printf (    "HChannelListMaker: Number of new channels > limit %i\n",chNmbMaxTmp);
              fclose (otp); //abort ();
              allChanged = kTRUE;
              return kFALSE;
             }
        }
        else
         if (newChN < chNmbMaxTmp)  // copy old channel to new one
          {newChN++;
           copyChString (idx,chN,idxNew,newChN);
           printChString ("::",idxNew,newChN,otp);
//            typeChString  ("::",idxNew,newChN);
          }
          else 
           {printf (    "HChannelListMaker: Number of new channels became > limit %i\n",chNmbMaxTmp);
            fclose (otp); //abort ();
            allChanged = kTRUE;
            return kFALSE;
           }
       chList[idxNew].chNmb = newChN;
      }
     idx    = 1 - idx;
     idxNew = 1 - idxNew;
     if (foundAtLeastOnce) 
      {mal++;
       fprintf (otp,"\n #%i replacing of Short_t life time particles with decay products:\n",mal);
//         printf (    "\n #%i replacing of Short_t life time particles with decay products:\n",mal);
      }
      else allChanged = kTRUE;
    }

  //pi0
     fprintf (otp,"\nreplacing pi0_ with pi0:\n"); 
//       printf (    "\nreplacing pi0_ with pi0:\n"); 
     Bool_t replacedFound;
     newChN = 0;
     for (chN = 1; chN <= chList[idx].chNmb; chN++) 
      {
       fprintf (otp,"\n");
       printChString ("==",idx,chN,otp);
//         printf ("\n");
//        typeChString ("==",idx,chN);

       newChN++;
       replacedFound = false;
       for (replN = 1; replN <= chList[idx].ch[chN].prong; replN++)
        {pID = chList[idx].ch[chN].pID[replN];
         if (pID == pi0StablePos) {pID = pi0Pos; replacedFound = true;}
         chList[idxNew].ch[newChN].pID[replN] = pID;
        }
       if (replacedFound) 
        {
         printChString ("->",idxNew,newChN,otp);
//          typeChString  ("->",idxNew,newChN);
        }
        else  
         {
          printChString ("::",idxNew,newChN,otp);
//           typeChString  ("::",idxNew,newChN);
         }
      }
     chList[idxNew].chNmb = chList[idx].chNmb;
     idx    = 1 - idx;
     idxNew = 1 - idxNew;

   fprintf (otp,"deleting pi0_ from particle in use list:\n"); 
//     printf (    "deleting pi0_ from particle in use list:\n"); 
   Phys.particles--;

  // if neutra > 1 then delete channel
  //  else 
  //   1) store channel
  //   2) if neutra = 0 then produce channels with not registered charge particle
   Int_t neutraPos,neutraPID;
   fprintf (otp,"\nextracting multiNeutra and single-particle channels, \n");
//     printf (    "extracting multiNeutra channels...\n");
   if (!allChargedParticlesAreRegistered)
    {fprintf (otp,"producing not registered charge particle channels\n");
//       printf (    "producing not registered charge particle channels...\n");
    }
   newChN = 0;
   for (chN = 1; chN <= chList[idx].chNmb; chN++) 
    {neutraNmb = 0; 
     neutraPos = 0; neutraPID = 0; // to prevent warning
     fprintf (otp,"\n");
     printChString ("==",idx,chN,otp);
//       printf (    "\n");
//      typeChString ("==",idx,chN);
     if (chList[idx].ch[chN].prong > 1) // examples: n p -> d,  n p -> d'
      {
       for (i = 1; i <= chList[idx].ch[chN].prong; i++)
         if (Phys.P[chList[idx].ch[chN].pID[i]].charge == 0) 
           {neutraNmb++; neutraPos = i; neutraPID = chList[idx].ch[chN].pID[neutraPos];}
       if (neutraNmb <= 1)
        {
         if (neutraNmb == 0)
          {
           sortDigit (chList[idx].ch[chN].prong,chList[idx].ch[chN].pID); // sort old channel
           if (chList[idx].ch[chN].prong <= maxProng) 
            {
             newChN++;
             copyChString (idx,chN,idxNew,newChN);
             printChString ("::",idxNew,newChN,otp);
  //         typeChString  ("::",idxNew,newChN);
             chList[idxNew].ch[newChN].channelType = charged;
  //           sortDigit (chList[idxNew].ch[newChN].prong,chList[idxNew].ch[newChN].pID);
             chList[idxNew].ch[newChN].code = getChannelCode (chList[idxNew].ch[newChN].prong,chList[idxNew].ch[newChN].pID,0);
             printChannelCodes (idxNew,newChN,otp);
            }
           if (!allChargedParticlesAreRegistered)
            {fprintf (otp,"producing charge-missing channels...\n");
  //             printf (    "producing charge-missing channels...\n");
             Int_t beforeNonReg,nonRegPos; // zeroChN,
             beforeNonReg = 0; // zeroChN = chN; // zeroChN = newChN;
  //         for (i = 1; i <= chList[idxNew].ch[zeroChN].prong; i++)
  //          {pID = chList[idxNew].ch[zeroChN].pID[i];
             for (i = 1; i <= chList[idx].ch[chN].prong; i++)
              {pID = chList[idx].ch[chN].pID[i];
               if (pID != beforeNonReg)
                {beforeNonReg = pID; nonRegPos = i;
                 newChN++;
                 chList[idxNew].ch[newChN].channelType = neutral; // with charged non registered particle
  //             copyChString (idxNew,zeroChN,idxNew,newChN);
                 copyChString (idx   ,chN   ,idxNew,newChN);
                 shiftParticleToEnde (idxNew,newChN,nonRegPos);
                 chList[idxNew].ch[newChN].prong--;
                 // now sorted
                 chList[idxNew].ch[newChN].code = 
                     getChannelCode (chList[idxNew].ch[newChN].prong,chList[idxNew].ch[newChN].pID,pID);
                 printChannelCodes (idxNew,newChN,otp);
                }
              }
            }
          }

          else
          {
           newChN++;
           copyChString (idx,chN,idxNew,newChN);
           printChString ("::",idxNew,newChN,otp);
  //        typeChString  ("::",idxNew,newChN);

           chList[idxNew].ch[newChN].channelType = neutral;
           // now copied
           shiftParticleToEnde (idxNew,newChN,neutraPos);
           chList[idxNew].ch[newChN].prong--;
           sortDigit (chList[idxNew].ch[newChN].prong,chList[idxNew].ch[newChN].pID);
           chList[idxNew].ch[newChN].code = 
               getChannelCode (chList[idxNew].ch[newChN].prong,chList[idxNew].ch[newChN].pID,neutraPID);

           printChannelCodes (idxNew,newChN,otp);
          } 
        }
        else 
         {
          fprintf (otp,"extracting multi-missing channel...\n");
  //          printf (    "extracting multi-missing channel...\n");
         }
      }
      else 
       {
        fprintf (otp,"extracting single-particle channel...\n");
//          printf (    "extracting single-particle channel...\n");
       }
    }
   chList[idxNew].chNmb = newChN;

   idx    = 1 - idx;
   idxNew = 1 - idxNew;

   fprintf (otp,"\nUnsorted channels:\n");
   printChannelList (idx,otp);
//     printf (    "\nUnsorted channels:\n");
//    typeChannelList (idx);

   Bool_t wasReplacing;
   Int_t K0Spos,K0Lpos;
   fprintf (otp,"Attepmt of replacing K0S (if exists) with K0L:\n");
//     printf (    "Attepmt of replacing K0S (if exists) with K0L:\n");
   K0Spos = getPid ("K0S"); 
   K0Lpos = getPid ("K0L"); 
   wasReplacing = false;
   for (chN = 1; chN <= chList[idx].chNmb; chN++) 
    {copyChannel (&chList[idx].ch[chN],&chList[idxNew].ch[chN]);
     if (chList[idx].ch[chN].channelType == neutral)
      {prong = chList[idx].ch[chN].prong;
       neutraPID = chList[idx].ch[chN].pID[prong+1];
       if (neutraPID == K0Spos) 
        {chList[idxNew].ch[chN].pID[prong+1] = K0Lpos; 
         chList[idxNew].ch[chN].code = (chList[idx].ch[chN].code - K0Spos) + K0Lpos; 
         wasReplacing = true;
        }
      }
    } 
   chList[idxNew].chNmb = chList[idx].chNmb; // was not changed

   idx    = 1 - idx;
   idxNew = 1 - idxNew;

   if (wasReplacing)
    {
     fprintf (otp,"\nChannels with K0S replaced with K0L:\n");
     printChannelList (idx,otp);
//       printf (    "\nChannels with K0S replaced with K0L:\n");
//      typeChannelList (idx);
    }
    else fprintf (otp,"there were no K0S for replacing\n");

   Int_t j;
   // Sorting channels:
  // printf ("sorting channels...\n");
   tChannel chTemp;
   for (i = chList[idx].chNmb-1; i >= 1; i--)
    for (j = 1; j <= i; j++)
     if (chList[idx].ch[j].code > chList[idx].ch[j+1].code)
      {copyChannel (&chList[idx].ch[j+1],&chTemp);
       copyChannel (&chList[idx].ch[j  ],&chList[idx].ch[j+1]);
       copyChannel (&chTemp             ,&chList[idx].ch[j  ]);
      }

   fprintf (otp,"\nSorted channels:\n");
   printChannelList (idx,otp);

   fprintf (otp,"Deleting duplicate channels:\n");
//    printf (    "deleting duplicate channels...\n");
   Double_t oldCode;
   newChN = 0; oldCode = 0.0;
   for (chN = 1; chN <= chList[idx].chNmb; chN++) 
    if (chList[idx].ch[chN].code != oldCode)
     {newChN++; oldCode = chList[idx].ch[chN].code;
      copyChannel (&chList[idx].ch[chN],&chList[idxNew].ch[newChN]);
     }
   chList[idxNew].chNmb = newChN;

   idx    = 1 - idx;
   idxNew = 1 - idxNew;
   fprintf (otp,"\nChannels with internal pid's:\n");
   printChannelList (idx,otp);
//     printf (    "\nChannels with internal pid's:\n");
//    typeChannelList (idx);

  //---------------------------------  
   fprintf (otp,"Replacing pid's with GSI ones...\n");
//     printf (    "Replacing pid's with GSI ones...\n");
//     for (pid = 1; pid <= Phys.particles; pid++)
//     {
//       printf (    "%3i gsiPid %2i '%s'\n",pid,gsiPid[pid],Phys.P[pid].name);
//      fprintf (otp,"%3i gsiPid %2i '%s'\n",pid,gsiPid[pid],Phys.P[pid].name);
//     }
//------------------------------
   bLib.fillPhysCnst(&Phys,otp);
//    printf ("fullLength %i\n",Phys.particles);
//    fprintf (otp,"\nFull list:\n");
//    bLib.prntPhys    (&Phys,otp);
//--------------------------------   
   for (chN = 1; chN <= chList[idx].chNmb; chN++) 
    {chList[idxNew].ch[chN].channelType = chList[idx].ch[chN].channelType;
     prong = chList[idx].ch[chN].prong;
     chList[idxNew].ch[chN].prong = prong;
     for (i = 1; i <= prong; i++)
        chList[idxNew].ch[chN].pID[i] = gsiPid[chList[idx].ch[chN].pID[i]]; // replacing
     if (chList[idx].ch[chN].channelType == neutral)
           neutraPID = gsiPid[chList[idx].ch[chN].pID[prong+1]]; // replacing missing particle pid
      else neutraPID = 0;
     chList[idxNew].ch[chN].pID[prong+1] = neutraPID;
     chList[idxNew].ch[chN].code = 
        getChannelCode (chList[idxNew].ch[chN].prong,chList[idxNew].ch[chN].pID,neutraPID);
    }
   chList[idxNew].chNmb = chList[idx].chNmb;

   idx    = 1 - idx;
   idxNew = 1 - idxNew;
  //----------------------------------------  
   
//---------------------------------------   
   fprintf (otp,"\nChannels:\n");
   printChannelList (idx,otp);
//     printf (    "\nChannels:\n");
//    typeChannelList (idx);

   if (chList[idx].chNmb <= maxChNmb)
    {
  //   printf ("making output parametra...\n"); 
     *numberOfChannels = 0;
     *pNmb = 0;
     *nNmb = 0;
     *tNmb = 0;
     for (chN = 1; chN <= chList[idx].chNmb; chN++)
      {(*numberOfChannels)++;
       chCodeList[*numberOfChannels]= chList[idx].ch[chN].code;
       maxP = chList[idx].ch[chN].prong;
       chLineList[*numberOfChannels] = "";
       for (i = 1; i <= maxP; i++) 
        {pID = chList[idx].ch[chN].pID[i];
         chLineList[*numberOfChannels] = chLineList[*numberOfChannels] + Phys.P[pID].name + " ";
         if      (Phys.P[pID].charge > 0) addPart (pID,pNmb,posPID);
         else if (Phys.P[pID].charge < 0) addPart (pID,nNmb,negPID);
                                          addPart (pID,tNmb,totPID);
        }
       if (chList[idx].ch[chN].channelType == neutral)
        {pID = chList[idx].ch[chN].pID[maxP+1];
         chLineList[*numberOfChannels] = chLineList[*numberOfChannels] + Phys.P[pID].name;
         if (Phys.P[pID].charge != 0) chLineList[*numberOfChannels] = chLineList[*numberOfChannels] + "'";
         if      (Phys.P[pID].charge > 0) addPart (pID,pNmb,posPID);
         else if (Phys.P[pID].charge < 0) addPart (pID,nNmb,negPID);
                                          addPart (pID,tNmb,totPID);
        }
      }
     fprintf (otp,"%2i positive particles in use: ",*pNmb);
     for (i = 0; i < *pNmb; i++)
        fprintf (otp," %s",Phys.P[posPID[i]].name);
     fprintf (otp,"\n");
     fprintf (otp,"%2i negative particles in use: ",*nNmb);
     for (i = 0; i < *nNmb; i++)
        fprintf (otp," %s",Phys.P[negPID[i]].name);
     fprintf (otp,"\n");
     fprintf (otp,"%2i total    particles in use: ",*tNmb);
     for (i = 0; i < *tNmb; i++)
        fprintf (otp," %s",Phys.P[totPID[i]].name);
     fprintf (otp,"\n");

     printf ("%2i positive particles in use: ",*pNmb);
     for (i = 0; i < *pNmb; i++)
        printf (" %s",Phys.P[posPID[i]].name);
     printf ("\n");
     printf ("%2i negative particles in use: ",*nNmb);
     for (i = 0; i < *nNmb; i++)
        printf (" %s",Phys.P[negPID[i]].name);
     printf ("\n");
     printf ("%2i total    particles in use: ",*tNmb);
     for (i = 0; i < *tNmb; i++)
        printf (" %s",Phys.P[totPID[i]].name);
     printf ("\n");
//-----------------------------      
    }
    else 
     {*numberOfChannels = chList[idx].chNmb;
      printf ("hChannelListMaker: number of channel produced (%i) > capacity (%i) of calling program\n",chList[idx].chNmb,maxChNmb);
      result = kFALSE;
     }
//    printf ("channel list maker ended\n");
   fclose (otp);   
  }
 return result;
} 

