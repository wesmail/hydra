#include "htofcalpar.h"

#include <stdio.h>

Bool_t HTofCalPar::init(void) {
  FILE *fp;
  Int_t s,m,c;
  //  Float_t ol,or;
  Float_t lk,rk,vg,pk,tk;

  for (s=0;s<6;s++)
    for (m=0;m<22;m++)
      for (c=0;c<8;c++) {
	fCells[s][m][c].setLeftK(0.05);
	fCells[s][m][c].setRightK(0.05);
	fCells[s][m][c].setVGroup(16.);
	fCells[s][m][c].setPosK(0.0);
	fCells[s][m][c].setTimK(0.0);
	/*	fCells[s][m][c].setSlopeTimeLeft(0.0249); 
	fCells[s][m][c].setSlopeTimeRight(0.0249);
	fCells[s][m][c].setOffsetTimeLeft(0.0);
	fCells[s][m][c].setOffsetTimeRight(0.0);
	fCells[s][m][c].setSlopeChargeLeft(0.0249);
	fCells[s][m][c].setSlopeChargeRight(0.0249);
	fCells[s][m][c].setOffsetChargeLeft(0.0);
	fCells[s][m][c].setOffsetChargeRight(0.0);*/
      }

  fp = fopen("calib.par","r");
  if(fp) {
    while(!feof(fp)) {
      fscanf(fp,"%d%d%d%f%f%f%f%f",&s,&m,&c,&lk,&rk,&vg,&pk,&tk);
      fCells[s][m][c].setLeftK(lk);
      fCells[s][m][c].setRightK(rk);
      fCells[s][m][c].setVGroup(vg);
      fCells[s][m][c].setPosK(pk);
      fCells[s][m][c].setTimK(tk);
    }
    fclose(fp);
    return kTRUE;
  }
  /*  fp=fopen("cal2.dat","r");
  if (fp) {
    while (!feof(fp)) {
      fscanf(fp,"%i %i %f %f",&m,&c,&ol,&or);
      fCells[0][m][c].setOffsetTimeLeft(ol); 
      fCells[0][m][c].setOffsetTimeRight(or);      
    }
    fclose(fp);
    return kTRUE;
  }*/
  Error("init","File with calibration parameters not found");
  return kFALSE;
}

void HTofCalPar::print() {
  printf("**** Tof Calpar ****\n");
}


void HTofCalPar::printHeader(void) {
  //  printf("Cell Id\tSlopeTL\tOffsetTL\tSlopeTR\tOffsetTR\n");
  printf("==============================================================\n");
}

void HTofCalPar::printPause(void) {
  printf("Press any key to continue");
  getchar();
  printf("\n\n");
  printHeader();
}

void HTofCalPar::printParam(void) {
  /*  Int_t s,m,c,line=0;

  printHeader();
  for (s=0;s<6;s++)
    for (m=0;m<22;m++)
      for (c=0;c<8;c++) {
	line++;
	if (line%20 == 0) printPause();
	printf("(%i,%i,%i):\t%f\t%f\t%f\t%f\n",
	       s,m,c,
	       fCells[s][m][c].getSlopeTimeLeft(),
	       fCells[s][m][c].getOffsetTimeLeft(),
	       fCells[s][m][c].getSlopeTimeRight(),
	       fCells[s][m][c].getOffsetTimeRight());
      }*/
}

ClassImp(HTofCalPar)
ClassImp(HTofCalParCell)







