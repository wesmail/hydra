//###################################################
//#
//#       
//#                      FRAMEWORK
//#
//#       Authors:                           W.Koenig
//#       adoption to framework & ROOT       W.Schoen
//#    
//#       last mod. W.Schoen 19.1.98
// ###################################################

#pragma implementation
#include "hshowerdigidetpar.h"
ClassImp(HShowerDigiDetPar)

////////// ********** HShowerDigiDetPar defintion ****************/////////////77
HShowerDigiDetPar::HShowerDigiDetPar() {
  m_pReadParam = NULL;
}

HShowerDigiDetPar::~HShowerDigiDetPar()
{
  clear(); 
}

void HShowerDigiDetPar::setHadIoAscii(HadIoAscii* pReadParam)
{
  m_pReadParam = pReadParam;
}

HadIoAscii* HShowerDigiDetPar::getHadIoAscii()
{
  return m_pReadParam;
}

Bool_t HShowerDigiDetPar::init()
{
 char buf[60], name[20], *b;

 if (!m_pReadParam)
   return kFALSE;

 HadIoAscii &mydata = *m_pReadParam;
 HadIoAscii *pTmpRP;

 mydata.echo = 0;

  mydata.open();
  fThickDet = mydata.getFloatData("thickDet");
  fFwhmResponse = mydata.getFloatData("fwhmResponse");
  nMatrixRange = mydata.getIntData("matrixRange");
  fBoxSize = mydata.getFloatData("boxSize");
  fPlaneDist = mydata.getFloatData("planeDist");
  nPadRange = mydata.getIntData("padRange");
  fFactorInd = mydata.getFloatData("factorInd");
  nNumElec = mydata.getIntData("numElec");
  setModules(mydata.getIntData("modules"));
  mydata.getStringData("modDesc", buf, sizeof(buf));
  mydata.close();

  b = buf;
  for(int i = 0; i < getModules(); i++) {
    if (i) b = NULL;
    strcpy(name, strtok(b, "*"));

    pTmpRP = new HadIoAscii;
    pTmpRP->setFileName(name);

    frames[i].setIO(pTmpRP);
    frames[i].init();
    wires[i].setIO(pTmpRP);
    wires[i].init();
    pads[i].setIO(pTmpRP);
    pads[i].setFrame(&frames[i]);
    pads[i].init();

    delete pTmpRP;
  }
 
  initChargeMatrix();

  return 1;
}

void HShowerDigiDetPar::clear()
{
detNr = 0;
fFwhmResponse = 0.;
fThickDet = 0.;
fFactorInd = 0.;
if (chargeMatrix) {
 delete [] chargeMatrix;
 chargeMatrix = NULL;
}

} //end of destructor

float HShowerDigiDetPar::calcCharge(float charge, float dist, 
  float Xd, float Yd, float Xu , float Yu) {

const float twoPI = 6.28318530718;

return ((fFactorInd*charge/twoPI)*(atan(Xd*Yd/(dist*sqrt(dist*dist+Xd*Xd+Yd*Yd)))
 - atan(Xd*Yu/(dist*sqrt(dist*dist+Xd*Xd+Yu*Yu))) 
 + atan(Xu*Yu/(dist*sqrt(dist*dist+Xu*Xu+Yu*Yu))) 
 - atan(Xu*Yd/(dist*sqrt(dist*dist+Xu*Xu+Yd*Yd)))));
} // eof

void HShowerDigiDetPar::initChargeMatrix() {
  int i,j;
  float cornerX = -1*(nMatrixRange*fBoxSize + 0.5*fBoxSize);
  float cornerY = cornerX;

  chargeMatrix = new float[(2*nMatrixRange+1)*(2*nMatrixRange+1)];
  for (j=0; j<2*nMatrixRange+1; j++)
   for (i=0; i<2*nMatrixRange+1; i++)
        chargeMatrix[i+j*(2*nMatrixRange+1)] = calcCharge(0.1, fPlaneDist,
        cornerX+i*fBoxSize, cornerY+j*fBoxSize,
        cornerX+(i+1)*fBoxSize, cornerY+(j+1)*fBoxSize);
}


