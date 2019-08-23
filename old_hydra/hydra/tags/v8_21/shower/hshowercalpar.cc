#pragma implementation
using namespace std;
#include <iostream> 
#include <iomanip>
#include "hades.h"
#include "hparhadasciifileio.h"
#include "hspectrometer.h"
#include "hshowerparset.h"
#include "hshowerdetector.h"
#include "hshowercalpar.h"
#include "hshowercalparcell.h"

#include "TH2.h"

ClassImp(HShowerCalPar)

//_HADES_CLASS_DESCRIPTION 
///////////////////////////////////////////////////////////////////////
// HShowerCalPar
//
// HShowerCalPar is used to manage of all parameters for calibration
// This class contains structure HObjTable, which contains all
// HShowerCalParCell objects. One HShowerCalParCell object contains
// calibration parameters of one pad: slope and offset
//
//////////////////////////////////////////////////////////////////////

HShowerCalPar::HShowerCalPar(const Char_t* name,const Char_t* title,
                             const Char_t* context)
              : HShowerParSet(name,title,context) {
  m_bWriteForElectronic = kFALSE;
  setSetup(6, 3, 32, 32); //default setup
  setCellClassName("HShowerCalParCell");
  defaultInit();
}

HShowerCalPar::~HShowerCalPar()
{
  m_ParamsTable.deleteTab();
}

void HShowerCalPar::setSetup(Int_t nSectors, Int_t nModules,
   Int_t  nRows, Int_t nColumns)
{
//set sizes of internal structure
  m_nSectors = nSectors;
  m_nModules = nModules;
  m_nRows = nRows;
  m_nColumns = nColumns;
}

void HShowerCalPar::setCellClassName(const Char_t* pszName)
//set name of class of calibration parameter container for one pad
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}

Char_t* HShowerCalPar::getCellClassName()
{
//return  name of class of calibration parameter container for one pad

  return m_szClassName;
}


HShowerCalParCell*& HShowerCalPar::getSlot(HLocation &loc)
{
  //return references address to place new object at loc position
  return (HShowerCalParCell*&) m_ParamsTable.getSlot(loc);
}

HShowerCalParCell* HShowerCalPar::getObject(HLocation &loc)
{
  //return pointer to object, which contains parameter for pad
  //at loc position
  return (HShowerCalParCell*) m_ParamsTable.getObject(loc);
}

Float_t HShowerCalPar::getSlope(HLocation &loc)
{
  //return slope for the pad at postion loc
  return getObject(loc)->getSlope();
}

Float_t HShowerCalPar::getOffset(HLocation &loc)
{
  //return offset for the pad at postion loc
  return getObject(loc)->getOffset();
}

Float_t HShowerCalPar::getThreshold(HLocation &loc)
{
  //return Threshold for the pad at postion loc
  return getObject(loc)->getThreshold();
}

Float_t HShowerCalPar::getGain(HLocation &loc)
{
  //return Gain for the pad at postion loc
  return getObject(loc)->getGain();
}

void HShowerCalPar::setSlope(HLocation &loc, Float_t fSlope)
{
  //set slope for the pad at postion loc
  getObject(loc)->setSlope(fSlope);
}

void HShowerCalPar::setOffset(HLocation &loc, Float_t fOffset)
{
  //set offset for the pad at postion loc
  getObject(loc)->setOffset(fOffset);
}

void HShowerCalPar::setThreshold(HLocation &loc, Float_t fThreshold)
{
  //set Threshold for the pad at postion loc
  getObject(loc)->setThreshold(fThreshold);
}

void HShowerCalPar::setGain(HLocation &loc, Float_t fGain)
{
  //set Gain for the pad at postion loc
  getObject(loc)->setGain(fGain);
}

Bool_t HShowerCalPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {
//reading and initializing  calibration parameters from ascii file (HAsciiRaw format)
  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  Char_t sectname[60];
  Int_t sect, mod, row, col, row1, col1;
  Float_t slope, offset, fThreshold, fGain;
  HLocation loc;
  HShowerCalParCell *pCell;
  Bool_t bForElectronic;

  loc.setNIndex(4);

  try {
     HAsciiRaw* pHAscii = pHadAsciiFile->GetRawAscii();
     HAsciiRaw &mydata = *pHAscii;

     mydata.SetActiveSection("Shower Calibrater Parameters Setup");

     mydata.SetReadKeyTypes('i', 'i', 'i', 'i');
     mydata.ReadRawLine(0,&m_nSectors,&m_nModules,&m_nRows,&m_nColumns);

     setSetup(m_nSectors, m_nModules, m_nRows, m_nColumns);
     m_ParamsTable.set(4, m_nSectors, m_nModules, m_nRows, m_nColumns);
     m_ParamsTable.setCellClassName(getCellClassName());
     m_ParamsTable.makeObjTable();

     for(sect = 0; sect < m_nSectors; sect++)
       for(mod = 0; mod < m_nModules; mod++) {
          sprintf(sectname,
               "Shower Calibrater Parameters: Sector %d Module %d",
                                                             sect, mod);
          if (!mydata.SetActiveSection(sectname))
             return kFALSE;

          bForElectronic = (mydata.GetKeys() == 4);

          if(bForElectronic)
              mydata.SetReadKeyTypes('i', 'i', 'f', 'f');
          else
              mydata.SetReadKeyTypes('i', 'i', 'f', 'f', 'f', 'f');

          loc[0] = sect;
          loc[1] = mod;

          Int_t line = 0;
          for(row = 0; row < m_nRows; row++)
            for(col = 0; col < m_nColumns; col++) {
               loc[2] = row;
               loc[3] = col;

               if(bForElectronic)
               {
                   mydata.ReadRawLine(line, &row1, &col1,
                            &slope, &fThreshold);

                   offset = fThreshold;
                   fGain  = 1.0f;
               }
               else
               {
                   mydata.ReadRawLine(line, &row1, &col1,
                            &slope, &offset, &fThreshold, &fGain);
               }

               if(row!=row1 || col!=col1) {
                   printf("error");
                   return kFALSE;
               }

               pCell = (HShowerCalParCell*) getSlot(loc);
               if (pCell) {
                    pCell = new(pCell) HShowerCalParCell;
                    pCell->setSlope(slope);
                    pCell->setThreshold(fThreshold);
                    pCell->setOffset(offset);
                    pCell->setGain(fGain);
               }
               line++;
          }
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}

Bool_t HShowerCalPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile)
{
//writing calibration parameters into ascii file (HAsciiRaw format)

  Bool_t status = kTRUE;
  if (!pHadAsciiFile) return kFALSE;

  Char_t sectname[60];
  Int_t sect, mod, row, col;
  Float_t slope, offset, fThreshold, fGain;
  HLocation loc;

  loc.setNIndex(4);

  try {
     HAsciiRaw* pHAscii = pHadAsciiFile->GetRawAscii();
     HAsciiRaw &mydata = *pHAscii;

     mydata.SetSeparator('\t');
     mydata.WriteSection("Shower Calibrater Parameters Setup");
     mydata.WriteKeywords(4, "Sector", "Module", "Row", "Column");
     mydata.SetWriteKeyTypes('i', 'i', 'i', 'i');
     mydata.WriteRawLine(m_nSectors, m_nModules, m_nRows, m_nColumns);

     for(sect = 0; sect < m_nSectors; sect++)
       for(mod = 0; mod < m_nModules; mod++) {
          sprintf(sectname, "Shower Calibrater Parameters: Sector %d Module %d",
                                                             sect, mod);
          mydata.WriteSection(sectname);

          if(m_bWriteForElectronic)
          {
              mydata.WriteKeywords(4, "Row", "Column", "Slope", "Threshold");
              mydata.SetWriteKeyTypes('i', 'i', 'f', 'f');
          }
          else
          {
              mydata.WriteKeywords(6, "Row", "Column", "Slope", "Offset",
                                    "Threshold", "Gain");
              mydata.SetWriteKeyTypes('i', 'i', 'f', 'f', 'f', 'f');
          }

          loc[0] = sect;
          loc[1] = mod;

          for(row = 0; row < m_nRows; row++)
            for(col = 0; col < m_nColumns; col++) {
               loc[2] = row;
               loc[3] = col;

               slope      = getSlope(loc);
               fThreshold = getThreshold(loc);

               if(m_bWriteForElectronic)
               {
                   mydata.WriteRawLine(row, col, slope * getGain(loc),
                            fThreshold);
               }
               else
               {
                   offset = getOffset(loc);
                   fGain  = getGain(loc);
                   mydata.WriteRawLine(row, col, slope, offset, fThreshold,
                            fGain);
               }
          }
     }
  }
  catch (Bool_t ret) {
    status = ret;
  }

  return status;
}


Bool_t HShowerCalPar::defaultInit()
{
  //initialization of calibration parameters for full setup
  //of pre-SHOWER detector. Default calibration using
  //values of slope==1 and offset==0

  printf("initialization of ShowerCalPar\n");
  Int_t sect, mod, row, col;

  HShowerDetector *pShowerDet = (HShowerDetector*)gHades->getSetup()
                                                  ->getDetector("Shower");
  if (!pShowerDet) return kFALSE;

  m_nSectors = pShowerDet->getShowerSectors();
  m_nModules = pShowerDet->getShowerModules();
  m_nRows = pShowerDet->getRows();
  m_nColumns = pShowerDet->getColumns();

  printf("%d - %d - %d - %d\n", m_nSectors, m_nModules, m_nRows, m_nColumns);
  m_ParamsTable.set(4, m_nSectors, m_nModules, m_nRows, m_nColumns);

  m_ParamsTable.setCellClassName(getCellClassName());
  m_ParamsTable.makeObjTable();

  HLocation loc;
  HShowerCalParCell *pCell;
  loc.setNIndex(4);

  for(sect = 0; sect < m_nSectors; sect++)
    for(mod = 0; mod < m_nModules; mod++)
      for(row = 0; row < m_nRows; row++)
         for(col = 0; col < m_nColumns; col++) {
           loc[0] = sect;
           loc[1] = mod;
           loc[2] = row;
           loc[3] = col;

           pCell = (HShowerCalParCell*) getSlot(loc);
           if (pCell) {
                 pCell = new(pCell) HShowerCalParCell;
                 pCell->setSlope(1.0);
                 pCell->setOffset(0.0);
           }

            //printf("XX : ");
           //pCell->print();
         }

  printf("Shower calpar default initialization\n");
  return kTRUE;
}

void HShowerCalPar::Print() {
  Int_t i, j, k, l;
  Float_t slope, offset,threshold,gain;

  HLocation loc;

  loc.setNIndex(4);
  for (i = 0; i < m_nSectors; i++)
     for (j = 0; j < m_nModules; j++)
        for (k = 0; k < m_nRows; k++)
           for (l = 0; l < m_nColumns; l++) {
             loc[0] = i;
             loc[1] = j;
             loc[2] = k;
             loc[3] = l;
             slope = getSlope(loc);
             offset = getOffset(loc);
                         threshold = getThreshold(loc);
                         gain = getGain(loc);
             if (offset < 0)
             printf("%d:%d:%d:%d %f %f %f %f\n", i, j, k, l, slope, offset,threshold,gain);
           }
}

// -----------------------------------------------------------------------------

void HShowerCalPar::changeValues(Int_t iVar, Int_t iOpr, Float_t f)
{
HLocation          loc;
Int_t                iSec, iMod, iRow, iCol;
Float_t            fVals[4];
HShowerCalParCell *pCell;

    loc.setNIndex(4);
    for(iSec = 0; iSec < m_nSectors; iSec++)
    {
        loc[0] = iSec;

        for(iMod = 0; iMod < m_nModules; iMod++)
        {
            loc[1] = iMod;

            for(iRow = 0; iRow < m_nRows; iRow++)
            {
                loc[2] = iRow;

                for(iCol = 0; iCol < m_nColumns; iCol++)
                {
                    loc[3] = iCol;

                    if((pCell = (HShowerCalParCell *) getSlot(loc)) == NULL)
                        continue;

                    fVals[HSCPV_SLOPE]     = pCell->getSlope();
                    fVals[HSCPV_OFFSET]    = pCell->getOffset();
                    fVals[HSCPV_THRESHOLD] = pCell->getThreshold();
                    fVals[HSCPV_GAIN]      = pCell->getGain();

                    switch(iOpr)
                    {
                        case HSCPM_SET:
                            fVals[iVar] = f;
                            break;

                        case HSCPM_ADD:
                            fVals[iVar] += f;
                            break;

                        default:
                            fVals[iVar] *= f;
                            break;
                    }

                    pCell->setParams(fVals[HSCPV_SLOPE], fVals[HSCPV_OFFSET],
                                fVals[HSCPV_THRESHOLD], fVals[HSCPV_GAIN]);
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------

TH2F* HShowerCalPar::getHist(Int_t iSector, Int_t iModule,
                enum_HSCPM_Var enType)
{
// Create a new histogram with a calibartion values for the selected chamber.
// User must delete the histogram himself

    // check parameters
    if((iSector < 0) || (iSector >= m_nSectors))
    {
        Error("getHist", "Sector %d out of bounds [0, %d]",
                    iSector, m_nSectors - 1);

        return NULL;
    }

    if((iModule < 0) || (iModule >= m_nModules))
    {
        Error("getHist", "Module %d out of bounds [0, %d]",
                    iModule, m_nModules - 1);

        return NULL;
    }

    if(m_nRows <= 0)
    {
        Error("getHist", "m_nRows = %d", m_nRows);
        return NULL;
    }

    if(m_nColumns <= 0)
    {
        Error("getHist", "m_nColumns = %d", m_nColumns);
        return NULL;
    }

TH2F *h;
Char_t  sT[100];
Char_t  sN[100];

    // prepare the names
    strcpy(sT, "Shower calibration params: ");
    strcpy(sN, "hShwCal_");
    switch(enType)
    {
        case HSCPV_SLOPE:
            strcat(sT, "slopes");
            strcat(sN, "slope");
            break;

        case HSCPV_OFFSET:
            strcat(sT, "offsets");
            strcat(sN, "off");
            break;

        case HSCPV_THRESHOLD:
            strcat(sT, "thresholds");
            strcat(sN, "thr");
            break;

        case HSCPV_GAIN:
            strcat(sT, "gains");
            strcat(sN, "gain");
            break;

        default:
            Error("getHist", "Wrong option enType=%d", enType);
            return NULL;
    }

    sprintf(sT + strlen(sT), " sec: %d mod: %d", iSector, iModule);
    sprintf(sN + strlen(sN), "_%d_%d", iSector, iModule);

    // create histogram
    if((h = new TH2F(sN, sT, m_nColumns, 0, m_nColumns, m_nRows, 0, m_nRows))
            == NULL)
    {
        Error("getHist", "Cannot create histogram");
        return NULL;
    }

    // fill histogram
HLocation hLoc;
Float_t   f;

    hLoc.setNIndex(4);
    hLoc[0] = iSector;
    hLoc[1] = iModule;

    for(hLoc[2] = 0; hLoc[2] < m_nRows; hLoc[2]++)
    {
        for(hLoc[3] = 0; hLoc[3] < m_nColumns; hLoc[3]++)
        {
            switch(enType)
            {
                case HSCPV_SLOPE:
                    f = getSlope(hLoc);
                    break;

                case HSCPV_OFFSET:
                    f = getOffset(hLoc);
                    break;

                case HSCPV_THRESHOLD:
                    f = getThreshold(hLoc);
                    break;

                default:
                    f = getGain(hLoc);
                    break;
            }

            h->Fill(hLoc[3], hLoc[2], f);
        }
    }

    return h;
}
