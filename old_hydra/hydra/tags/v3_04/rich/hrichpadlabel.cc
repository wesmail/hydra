#include "hrichpadgraph.h"
#include "hrichpadlabel.h"
#include <iostream.h>

ClassImp(HRichPadLabel)



HRichPadLabel::HRichPadLabel() {
 iGraphArray = 128;
 iLabelGraphConnection.Set(iGraphArray*iGraphArray);
}

HRichPadLabel::HRichPadLabel(Int_t padsx, Int_t padsy) {
 if (padsx > padsy) iGraphArray = padsx;
 else iGraphArray = padsy;
 iLabelGraphConnection.Set(iGraphArray*iGraphArray);
}

HRichPadLabel::~HRichPadLabel() {}

HRichPadLabel::HRichPadLabel(const HRichPadLabel& source) {}

HRichPadLabel& HRichPadLabel::operator=(const HRichPadLabel& source) {
 if (this != &source) {}

return *this;
}


void HRichPadLabel::Reset() { iLabelGraphConnection.Reset(); }


Int_t HRichPadLabel::LabelPads(HRichAnalysis *showMe, Int_t padDistance) {

Int_t i,k,l,m,n,xNow,yNow,xPad,yPad;

iLabelGraphConnection.Reset();

for (i = 0; i < showMe->GetPadNr(); i++)
 if (showMe->GetPad(i)->getAmplitude() > 0) {
  showMe->GetPad(i)->setLock(1);
  for (k = 0; k < 2*padDistance+1; k++)
   for (l = 0; l < 2*padDistance+1; l++)
    if (!(l == padDistance && k == padDistance))
     if (!showMe->IsOut(i,l-padDistance,k-padDistance))
      if (showMe->GetPad(i+l-padDistance + (showMe->GetPadsXNr())*(k-padDistance))
           ->getAmplitude() > 0 &&
          showMe->GetPad(i+l-padDistance + (showMe->GetPadsXNr())*(k-padDistance))
           ->getLock() == 0) {
       xNow = i % showMe->GetPadsXNr();
       yNow = i / showMe->GetPadsXNr();
       xPad = xNow + l - padDistance;
       yPad = yNow + k - padDistance;
       for (n=((k>=padDistance) ? yNow:yPad); n<=((k<=padDistance) ? yNow:yPad); n++)
        for (m=((l>=padDistance) ? xNow:xPad); m<=((l<=padDistance) ? xNow:xPad); m++)
         if (((HRichGeometry*)showMe->getGeomPar())
              ->fPads.getPad(m+(showMe->GetPadsXNr())*n)->getPadActive())
              showMe->GetPad(m+(showMe->GetPadsXNr())*n)->setLabel(-1);
      }
 }

//   cout << endl << "-----------LABEL------------" << endl;;
//   for (int kk = 0; kk < 4096; kk++) {
//    if ((kk % 64) == 0) cout << endl;
//    if (showMe->GetPad(kk)->getLabel() < 0) cout << "o";
//     else cout << ".";
//   }
//   cout << endl;


Int_t padL, padD, marker = 0;
for (i = 0; i < showMe->GetPadNr(); i++)
 if (showMe->GetPad(i)->getLabel() < 0) {
  padL = padD = 0;
  if (!showMe->IsOut(i-1) && (i % showMe->GetPadsXNr()) &&
      showMe->GetPad(i-1)->getLabel() > 0) 
         padL = showMe->GetPad(i-1)->getLabel();
  if (!showMe->IsOut(i-(showMe->GetPadsXNr())) &&
      showMe->GetPad(i-(showMe->GetPadsXNr()))->getLabel() > 0)
         padD = showMe->GetPad(i-(showMe->GetPadsXNr()))->getLabel();

  if (!padL && !padD) {
   showMe->GetPad(i)->setLabel(++marker);
  }
  else {

   if (padL && padD) {

    if (padL != padD && !iLabelGraphConnection[padL-1 + iGraphArray*(padD-1)]) {
     iLabelGraphConnection[padL-1 + iGraphArray*(padD-1)] = 1;
     iLabelGraphConnection[padD-1 + iGraphArray*(padL-1)] = 1;
    }
    if (padL < padD) {
     if (padL < marker) showMe->GetPad(i)->setLabel(padL);
     else showMe->GetPad(i)->setLabel(marker);
    }
    else {
     if (padD < marker) showMe->GetPad(i)->setLabel(padD);
     else showMe->GetPad(i)->setLabel(marker);
    }
   }

   if (padL && !padD) {
    if (padL < marker) showMe->GetPad(i)->setLabel(padL);
    else showMe->GetPad(i)->setLabel(marker);
   }

   if (!padL && padD) {
    if (padD < marker) showMe->GetPad(i)->setLabel(padD);
    else showMe->GetPad(i)->setLabel(marker);
   }

  }
 }
// end of 'for' loop


//   cout << endl << "-----------LABEL------------" << endl;;
//   for (int kk = 0; kk < 4096; kk++) {
//    if ((kk % 64) == 0) cout << endl;
//    if (showMe->GetPad(kk)->getLabel() < 0) cout << "o";
//     else if (showMe->GetPad(kk)->getLabel() > 0) cout << showMe->GetPad(kk)->getLabel(); 
//     else cout << ".";
//   }
//   cout << endl;



HRichPadGraph *pClusterList = NULL;
if (marker) pClusterList = new HRichPadGraph[marker];
for (i = 0; i < marker; i++) pClusterList[i].InitGraph(i+1,marker);

for (k = 0; k < marker; k++)
 for (m = 0; m < marker; m++)
  if (m < k && iLabelGraphConnection[m + iGraphArray*k]) {
   pClusterList[m].AddNode(pClusterList+k);
   pClusterList[k].AddNode(pClusterList+m);
  }



k = 0;
for (i = 0; i < marker; i++)
 if (pClusterList[i].MarkNodes(k)) k++;


HRichLabel *pAreaData = NULL;
if (k) pAreaData = new HRichLabel[k];
HRichLabel::iLabelNr = k;

if (pAreaData != NULL && pClusterList != NULL)
 for (i = 0; i < showMe->GetPadNr(); i++)
  if (showMe->GetPad(i)->getLabel() > 0) {
   m = i % showMe->GetPadsXNr();
   n = i / showMe->GetPadsXNr();
   if (pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iLeftX >= m)
      pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iLeftX = m;
   if (pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iRightX <= m)
      pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iRightX = m;
   if (pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iLowerY >= n)
      pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iLowerY = n;
   if (pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iUpperY <= n)
      pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iUpperY = n;
   if (showMe->GetPad(i)->getAmplitude() > 0)
      pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iFiredPadsNr++;
   pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iLabeledPadsNr++;
   pAreaData[pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel-1].iSignature =
            pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel;
   showMe->GetPad(i)->setLabel(pClusterList[showMe->GetPad(i)->getLabel()-1].iLabel);
  }

if (pClusterList) {
 delete [] pClusterList;
 pClusterList = NULL;

#ifdef HRICH_DEBUGMODE0
 cout << "RICH DEBUG MODE: pointer \'pClusterList\' (type \'class "
         "HRichPadGraph\') deleted.\n";
#endif
}

//  now grouping section - deciding if a given label may be accepted

for (i = 0; i < k; i++)
 if (pAreaData[i].iRightX - pAreaData[i].iLeftX < padDistance ||
     pAreaData[i].iUpperY - pAreaData[i].iLowerY < padDistance) 
       pAreaData[i].Reset();

if (showMe->pLabelArea) {
 delete [] showMe->pLabelArea;
 showMe->pLabelArea = NULL;
}
showMe->iLabelNr = 0;
if (HRichLabel::iLabelNr) {
 showMe->iLabelNr = HRichLabel::iLabelNr;      
 showMe->pLabelArea = new HRichLabel[HRichLabel::iLabelNr];
} 
 

cout << "Temporary debug: Accepted labels: " << HRichLabel::iLabelNr << endl;

m = 0;
for (i = 0; i < k; i++)
 if (pAreaData[i].iSignature) showMe->pLabelArea[m++] = pAreaData[i];

#ifdef HRICH_DEBUGMODE
 cout << "RICH DEBUG MODE: LabelPads divides pads into following areas:\n";
 for (i = 0; i < HRichLabel::iLabelNr; i++)
  cout << "Label nr " << i+1 << " (" << showMe->pLabelArea[i].iSignature
       << ") | left = " << showMe->pLabelArea[i].iLeftX << " | right = "
       << showMe->pLabelArea[i].iRightX << " (dx = "
       << showMe->pLabelArea[i].iRightX-showMe->pLabelArea[i].iLeftX+1 << ") | bottom = "
       << showMe->pLabelArea[i].iLowerY << " | top = " << showMe->pLabelArea[i].iUpperY
       << " (dy = "
       << showMe->pLabelArea[i].iUpperY-showMe->pLabelArea[i].iLowerY+1 << ") | fired = "
       << showMe->pLabelArea[i].iFiredPadsNr << "\n";
#endif

if (pAreaData) {
 delete [] pAreaData;
 pAreaData = NULL;

#ifdef HRICH_DEBUGMODE0
 cout << "RICH DEBUG MODE: pointer \'pAreaData\' (type \'class "
         "HRichLabel\') deleted.\n";
#endif
}

return (showMe->iLabelNr = HRichLabel::iLabelNr);

} // eof LabelPads



Int_t HRichPadLabel::Execute(HRichAnalysis *giveMe) {
 HRichAnalysisPar *analParams = giveMe->getParams();
 Int_t iLabelNr = 0;

 if (analParams->isActiveLabelPads) 
   iLabelNr += LabelPads(giveMe,analParams->iLabeledPadsDistance);

return iLabelNr;
}



