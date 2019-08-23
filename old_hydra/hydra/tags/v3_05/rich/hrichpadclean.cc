#include <iostream.h>
#include "hrichpadclean.h"

ClassImp(HRichPadClean)


HRichPadClean::HRichPadClean() {
 iCount = 0;
 iTempCluster.Set(100);
}

HRichPadClean::~HRichPadClean() {}

HRichPadClean::HRichPadClean(const HRichPadClean& source) {
 iCount = source.iCount;
 iTempCluster = source.iTempCluster;
}

HRichPadClean& HRichPadClean::operator=(const HRichPadClean& source) {
 if (this != &source) {
  iCount = source.iCount;
  iTempCluster = source.iTempCluster;
 }
return *this;
}


Int_t HRichPadClean::CleanAlonePad(HRichAnalysis *showMe, Int_t border, Int_t lowerThr) {

Int_t i,k,l;
iCount = 0;

if (!border) {
  cerr << "Argument \'border\' in function CleanAlonePad must be larger than 0!\n";
  exit(1);
}

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: CleanAlonePad cleans the following pads: \n";
#endif

for (i = 0; i < showMe->GetPadNr(); i++)
  if (showMe->GetPad(i) && showMe->GetPad(i)->getAmplitude() > 0) {
    for (k = 0; k < 2*border+1; k++)
      for (l = 0; l < 2*border+1; l++)
        if (!(l == border && k == border))
          if (!showMe->IsOut(i,l-border,k-border) && 
              showMe->GetPad(i+l-border + (showMe->GetPadsXNr())*(k-border)))
            if (showMe->GetPad(i+l-border + (showMe->GetPadsXNr())*(k-border))
                 ->getAmplitude() > 0) goto nextPad;

    if (showMe->GetPad(i)->getAmplitude() < lowerThr) {
      showMe->GetPad(i)->setAmplitude(0);
      iCount++;

#ifdef HRICH_DEBUGMODE
 Int_t tempX = i % showMe->GetPadsXNr();
 Int_t tempY = i / showMe->GetPadsXNr();
 cout << " (" << tempX << "," << tempY << ")... ";
#endif

    }

  nextPad: continue;
  }

#ifdef HRICH_DEBUGMODE
 if (!iCount) cout << "None. \n" << "RICH DEBUG MODE: Total number of "
                      "pads cleaned by CleanAlonePad = 0\n";
 else cout << "\nRICH DEBUG MODE: Total number of pads cleaned by "
              "CleanAlonePad = " << iCount << "\n";
#endif

return iCount;
}


void HRichPadClean::DeletePulse(HRichAnalysis *showYou, Int_t border, Int_t nowPad) {

Int_t i,j;
TArrayI iTempMatrix((2*border+1)*(2*border+1));

showYou->GetPad(nowPad)->setAmplitude(0);
iCount++;

#ifdef HRICH_DEBUGMODE
 Int_t iTempX = nowPad % showYou->GetPadsXNr();
 Int_t iTempY = nowPad / showYou->GetPadsXNr();
 cout << " (" << iTempX << "," << iTempY << ")... ";
#endif


for (i = 0; i < 2*border+1; i++)
 for (j = 0; j < 2*border+1; j++)
  if (!showYou->IsOut(nowPad,j-border,i-border))
   if(!(i == border && j == border))
    if (showYou->GetPad(nowPad + j-border + (showYou->GetPadsXNr())*(i-border)) && 
        showYou->GetPad(nowPad + j-border + (showYou->GetPadsXNr())*(i-border))
         ->getAmplitude() > 0) {
     showYou->GetPad(nowPad + j-border + (showYou->GetPadsXNr())*(i-border))
      ->setAmplitude(0);
     iTempMatrix[i + (2*border+1)*j] = 1;
    }

for (i = 0; i < 2*border+1; i++)
 for (j = 0; j < 2*border+1; j++)
  if (iTempMatrix[i + (2*border+1)*j] > 0) 
   DeletePulse(showYou, border, nowPad + j-border + (showYou->GetPadsXNr())*(i-border));
}


Int_t HRichPadClean::CleanHighPulse(HRichAnalysis *showMe, Int_t border, Int_t upperThr) {
 
Int_t iCountBefore = 0;
Int_t i,j;

iCount = 0;

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: DeletePulse cleans the following pads: \n";
#endif

j = 0;
for (i = 0; i < showMe->GetPadNr(); i++)
  if (showMe->GetPad(i) && showMe->GetPad(i)->getAmplitude() > upperThr) {
   iCountBefore = iCount;
   DeletePulse(showMe,border,i);
   if ((iCount-iCountBefore) > 0 && j < 100) {
    // showMe->iClustersCleaned[j] = iCount-iCountBefore;
    iTempCluster[j] = iCount-iCountBefore;
    showMe->iClusterCleanedNr = ++j;
   }
  }

#ifdef HRICH_DEBUGMODE
 if (!iCount) cout << "None. \n" << "RICH DEBUG MODE: Total number "
                      "of pads cleaned by CleanHighPulse = 0\n";
 else cout << "\nRICH DEBUG MODE: Total number of pads cleaned "
              "by CleanHighPulse = " << iCount << "\n";
#endif

showMe->iClustersCleaned.Set(showMe->iClusterCleanedNr);
for (i = 0; i < showMe->iClusterCleanedNr; i++) showMe->iClustersCleaned[i] = iTempCluster[i];
return iCount;
}


Int_t HRichPadClean::Execute(HRichAnalysis *giveMe) {

HRichAnalysisPar *analParams = giveMe->getParams(); 

Int_t iReducedNr = 0;
giveMe->iPadCleanedNr = 0;

if (analParams->isActiveCleanAlonePad) 
 iReducedNr += CleanAlonePad(giveMe,analParams->iCleanAlonePadBorder,
                             analParams->iCleanAlonePadLowerThreshold);
if (analParams->isActiveCleanHighPulse) 
 iReducedNr += CleanHighPulse(giveMe,analParams->iCleanHighPulseBorder,
                              analParams->iCleanHighPulseUpperThreshold);

return (giveMe->iPadCleanedNr = iReducedNr);
}


