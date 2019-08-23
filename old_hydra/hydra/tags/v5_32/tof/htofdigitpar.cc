#include "htofdigitpar.h"

#include <stdio.h>
//*-- Author : D.Vasiliev
//*-- Modified: 30/11/2000 by R. Holzmann

/////////////////////////////////////////////////////////////////
//
//  HTofDigitPar container contains digitization parameters
//  read from file digit.par (if not existent, defaults are used)
//  
//  All lengths are in mm, group velocity in mm/ns
//  
/////////////////////////////////////////////////////////////////

Bool_t HTofDigitPar::init(void) {

  FILE *fp;
  Int_t s,m,c;
  Float_t eq, vg, ar, al, trz;

  for (s=0;s<6;s++)
    for (m=0;m<22;m++)
      for (c=0;c<8;c++) {
	fCells[s][m][c].setEffiQuant(0.24);
	fCells[s][m][c].setYieldPhot(11100.0);
	if(m==0) fCells[s][m][c].setHalfLen(1108.25);
	if(m==1) fCells[s][m][c].setHalfLen(1103.25);
	if(m==2) fCells[s][m][c].setHalfLen(1006.75);
	if(m==3) fCells[s][m][c].setHalfLen(980.5);
	if(m==4) fCells[s][m][c].setHalfLen(970.0);
	if(m==5) fCells[s][m][c].setHalfLen(890.75);
	if(m==6) fCells[s][m][c].setHalfLen(810.25);
	if(m==7) fCells[s][m][c].setHalfLen(730.75);
	if(m >7) fCells[s][m][c].setHalfLen(0.0);
	fCells[s][m][c].setVelGroup(160.0);
	if(m<=5) fCells[s][m][c].setAngleRef(47.);
	if(m>5 && m<=7) fCells[s][m][c].setAngleRef(38.);
	if(m >7) fCells[s][m][c].setAngleRef(38.0);
	if(m<=5) fCells[s][m][c].setAttenLen(2000.);
	if(m >5) fCells[s][m][c].setAttenLen(1700.);
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











