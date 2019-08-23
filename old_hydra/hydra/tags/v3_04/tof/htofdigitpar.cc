#include "htofdigitpar.h"

#include <stdio.h>

Bool_t HTofDigitPar::init(void) {

  FILE *fp;
  Int_t s,m,c;
  Float_t eq, vg, ar, al, trz;

  for (s=0;s<6;s++)
    for (m=0;m<22;m++)
      for (c=0;c<8;c++) {
	fCells[s][m][c].setEffiQuant(0.24);
	fCells[s][m][c].setYieldPhot(11100.0);
	if(m==0) fCells[s][m][c].setHalfLen(118.25);
	if(m==1) fCells[s][m][c].setHalfLen(113.25);
	if(m==2) fCells[s][m][c].setHalfLen(106.75);
	if(m==3) fCells[s][m][c].setHalfLen(98.5);
	if(m==4) fCells[s][m][c].setHalfLen(97.);
	if(m==5) fCells[s][m][c].setHalfLen(89.75);
	if(m==6) fCells[s][m][c].setHalfLen(81.25);
	if(m==7) fCells[s][m][c].setHalfLen(73.75);
	if(m >7) fCells[s][m][c].setHalfLen(0.0);
	fCells[s][m][c].setVelGroup(16.0);
	if(m<=5) fCells[s][m][c].setAngleRef(47.);
	if(m>5 && m<=7) fCells[s][m][c].setAngleRef(38.);
	if(m >7) fCells[s][m][c].setAngleRef(38.0);
	if(m<=5) fCells[s][m][c].setAttenLen(200.);
	if(m >5) fCells[s][m][c].setAttenLen(170.);
	fCells[s][m][c].setTimeResZero(.100);
      }

  fp = fopen("digit.par","r");
  if(fp) {
    while(!feof(fp)) {
      fscanf(fp,"%d%d%d%f%f%f%f%f",&s,&m,&c,&eq,&vg,&ar,&al,&trz);
      fCells[s][m][c].setEffiQuant(eq);
      fCells[s][m][c].setVelGroup(vg);
      fCells[s][m][c].setAngleRef(ar);
      fCells[s][m][c].setAttenLen(al);
      fCells[s][m][c].setTimeResZero(trz);
    }
    fclose(fp);
    return kTRUE;
  }
  Error("init","File with digit parameters not found, default initialization");
  return kTRUE;

}

void HTofDigitPar::print() {

  printf("**** Tof Digitpar ****\n");

}


void HTofDigitPar::printHeader(void) {

  printf("Cell Id\tQuantEf\tHalfLen\tVelGroup\tAngleRef\tAttenLen\tTimeResZero\n");
  printf("==============================================================\n");

}

void HTofDigitPar::printPause(void) {

  printf("Press any key to continue");
  getchar();
  printf("\n\n");
  printHeader();

}

void HTofDigitPar::printParam(void) {

  Int_t s,m,c,line=0;

  printHeader();
  for (s=0;s<6;s++)
    for (m=0;m<22;m++)
      for (c=0;c<8;c++) {
	line++;
	if (line%20 == 0) printPause();
	printf("(%i,%i,%i):\t%f\t%f\t%f\t%f\t%f\t%f\n",
	       s,m,c,
	       fCells[s][m][c].getEffiQuant(),
	       fCells[s][m][c].getHalfLen(),
	       fCells[s][m][c].getVelGroup(),
	       fCells[s][m][c].getAngleRef(),
	       fCells[s][m][c].getAttenLen(),
	       fCells[s][m][c].getTimeResZero());
      }

}

ClassImp(HTofDigitPar)
ClassImp(HTofDigitParCell)











