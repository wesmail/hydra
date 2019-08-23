#include "hrtsavgolmanager.h"

HRtSavGolManager::HRtSavGolManager(Int_t nLeft,Int_t nRight,
    Int_t ld,Int_t m) {
  fMaxLeft = nLeft + 1;
  fMaxRight = nRight + 1;
  fM = m;

  //size is max number of points - polinomial degree. Because fitting
  //cannot be done with less points than m. The factor 2 stands for
  //the cases of the points leading to the left or de right, that is
  //nl>nr or nr>nl
  fSize = fMaxLeft*fMaxRight;
  fTable = new HRtVector* [fSize];
  HRtVector *v;

  for (int i=0;i<fMaxLeft;i++)
    for (int j=0;j<fMaxRight;j++) {
      if (i+j>=m) {
	v = new HRtVector(i+j+1);
	fSavGol.calcCoefficients(*v,i,j,ld,m);
      } else v = 0;
      fTable[i*fMaxRight + j] = v;
    }
}

void HRtSavGolManager::print(void) {
  const HRtVector *v;

  for (int nl=0;nl<fMaxLeft;nl++) {
    for (int nr=0;nr<fMaxRight;nr++) {
      v = getCoefficients(nl,nr);
      printf("| %5i  | %5i  | %5i  ",fM,nl,nr);
      if (v) {
	for (int i=0;i<v->getSize();i++) {
	  printf("| %5.3f  ",v->at(i));
	}
      }
      printf("|\n");
    }
  }


}

HRtSavGolManager::~HRtSavGolManager(void) {
  delete[] fTable;
}

ClassImp(HRtSavGolManager);
