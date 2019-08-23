//*-- Author : RICH team member
//
//_HADES_CLASS_DESCRIPTION 
//////////////////////////////////////////////////////////////////////////////
//
//  HRichCalPar
//
//  Parameter container for calibration data.
//
//////////////////////////////////////////////////////////////////////////////

#include "hades.h"
#include "hparhadasciifileio.h"
#include "hrichcalpar.h"
#include "hrichcalparcell.h"
#include "hrichdetector.h"
#include "hspectrometer.h"

#include <iomanip>
#include <iostream> 

using namespace std;

ClassImp(HRichCalPar)

//----------------------------------------------------------------------------
HRichCalPar::HRichCalPar(const Char_t* name,const Char_t* title,
                         const Char_t* context)
                : HRichParSet(name,title,context) {

  m_pReadParam  = NULL;

  setCellClassName("HRichCalParCell");

  defaultInit();
}
//============================================================================

//----------------------------------------------------------------------------
HRichCalPar::~HRichCalPar()
{ 
  m_ParamsTable.deleteTab();
}
//============================================================================

//----------------------------------------------------------------------------
void HRichCalPar::setCellClassName(const Char_t* pszName)
{
  strncpy(m_szClassName, pszName, sizeof(m_szClassName));
}
//============================================================================

//----------------------------------------------------------------------------
HRichCalParCell* HRichCalPar::getSlot(HLocation &loc)
{
  return (HRichCalParCell*) m_ParamsTable.getSlot(loc);
}
//============================================================================

//----------------------------------------------------------------------------
HRichCalParCell* HRichCalPar::getObject(HLocation &loc)
{
  return (HRichCalParCell*) m_ParamsTable.getObject(loc);
}
//============================================================================

//----------------------------------------------------------------------------
void HRichCalPar::defaultInit()
{
// - allocates memory for the parameter table

   HLocation loc;
   loc.setNIndex(3);

   HRichDetector *pRichDet = (HRichDetector*)gHades->getSetup()->getDetector("Rich");
   if ( NULL == pRichDet )
   {
      Error("defaultInit", "RICH detector not in HADES setup");
      return;
   }

   fSectors = pRichDet->getSectors();
   fRows    = pRichDet->getRows();
   fColumns = pRichDet->getColumns();

   Info("defaultInit", "Configuration: %d - %d - %d\n", fSectors, fRows, fColumns);


   m_ParamsTable.set(3, fSectors, fRows, fColumns);

   m_ParamsTable.setCellClassName(getCellClassName());
   m_ParamsTable.makeObjTable();

   clear();

}
//============================================================================

//----------------------------------------------------------------------------
void HRichCalPar::clear()
{
// - initialize all cells to ZERO

   HLocation loc;
   loc.setNIndex(3);
   HRichCalParCell* pCell = NULL;

   for ( Int_t s = 0; s < fSectors; ++s )
   {
      loc[0] = s;
      for ( Int_t cols = 0; cols < fColumns; ++cols )
      {
         for ( Int_t rows = 0; rows < fRows; ++rows )
         {
            loc[1] = rows;
            loc[2] = cols;	
            pCell = (HRichCalParCell*) m_ParamsTable.getSlot(loc);
            if ( NULL != pCell )
            {
               pCell = new(pCell) HRichCalParCell;
               pCell->setParams(0.,0.,0.);
            }
            else
            {
               Error("clear", "slot not found:  %i %i %i",loc[0],loc[1],loc[2]);
               return;
            }
         }
      }
   }  
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichCalPar::initAscii(HParHadAsciiFileIo* pHadAsciiFile) {

  using namespace HadAsciiAuxiliaryFunctions;

  if ( NULL == pHadAsciiFile )
  {
     Error("initAscii", "Poiner to ascii param file is NULL!!!");
     return kFALSE;
  }

  Bool_t status = kTRUE;
  Char_t sectname[60];
  Int_t iSectors, iRows, iColumns;
  Int_t i, sect, row, col;
  Float_t slope, offset, sigma;
  HLocation loc;
  HRichCalParCell *pCell = NULL;
 
  loc.setNIndex(3);

  // first set everything to zero
  clear();
 
 try {

  HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();

  mydata.SetActiveSection("Rich Calibrater Parameters Setup");

  mydata.SetReadKeyTypes('i', 'i', 'i');
  mydata.ReadRawLine(0,&iSectors,&iRows,&iColumns);

  if( iSectors > fSectors ||
      iRows    > fRows    ||
      iColumns > fColumns  )
    {
      Error("initAscii",
	    "size mismatch of param tables: ASCII %i %i %i <-> HADES setup %i %i %i",
	    iSectors, iRows, iColumns, fSectors, fRows, fColumns);
      return kFALSE;
    }
  
  for(sect = 0; sect < fSectors; sect++) {
   sprintf(sectname, "Rich Calibrater Parameters: Sector %d", sect); 
   mydata.SetActiveSection(sectname);
   mydata.SetReadKeyTypes('i', 'i', 'f', 'f', 'f');

   loc[0] = sect;

   Int_t line = 0;
   for(i = 0; i < mydata.GetRowsNr(); i++) {
    mydata.ReadRawLine(line, &row, &col, &slope, &offset, &sigma);
    loc[1] = row;
    loc[2] = col;

    pCell = (HRichCalParCell*) getSlot(loc);
    if ( NULL != pCell )
      {
	pCell = new(pCell) HRichCalParCell;
	pCell->setSlope(slope);
	pCell->setOffset(offset);
	pCell->setSigma(sigma);
      }
    
    line++;
   }
  }

 } // eof try block

 catch (Bool_t ret)
 {
  status = ret;
 }
 catch (...)
 {
  status = kFALSE;
  ErrorMsg(2,"HRichCalPar::initAscii",1,"Unidentified exception catched.");
 }

 if (status)
 {
    ErrorMsg(0,"HRichCalPar::initAscii",
             1,"Container \'RichCalPar\' has been read from ASCII file.");
 }

  return status;
}
//============================================================================

//----------------------------------------------------------------------------
Bool_t HRichCalPar::writeAscii(HParHadAsciiFileIo* pHadAsciiFile) {

 using namespace HadAsciiAuxiliaryFunctions;

 if ( NULL == pHadAsciiFile )
 {
    Error("writeAscii", "Pointer to ascii param file is NULL!!!");
    return kFALSE;
 }

 Bool_t status = kTRUE;
 Char_t sectname[60];
 Int_t sect, row, col;
 Float_t slope, offset, sigma;
 HLocation loc;

 loc.setNIndex(3);

 try {

  HAsciiRaw &mydata = *pHadAsciiFile->GetRawAscii();

  mydata.SetSeparator('\t');
  mydata.WriteSection("Rich Calibrater Parameters Setup");
  mydata.WriteKeywords(3, "Sector", "Row", "Column");
  mydata.SetWriteKeyTypes('i', 'i', 'i');
  mydata.WriteRawLine(fSectors, fRows, fColumns);
 
  for(sect = 0; sect < fSectors; sect++) {
   sprintf(sectname, "Rich Calibrater Parameters: Sector %d", sect);
   mydata.WriteSection(sectname);
   mydata.WriteKeywords(5, "Row", "Column", "Slope", "Offset", "Sigma");
   mydata.SetWriteKeyTypes('i', 'i', 'f', 'f', 'f');

   loc[0] = sect;
   // order of loop reversed to make this output identical to 
   // output of hrichpedestal
   for(col = 0; col < fColumns; col++)
    for(row = 0; row < fRows; row++) {
     loc[1] = row;
     loc[2] = col;
     slope  = -1;
     offset = -1;
     sigma  = -1;
     if ( NULL != getObject(loc) )
     {
        slope  = ((HRichCalParCell*) getObject(loc))->getSlope();
        offset = ((HRichCalParCell*) getObject(loc))->getOffset();
        sigma  = ((HRichCalParCell*) getObject(loc))->getSigma();
     }
     //if (sigma!=0.)
     //This was commented out because root cannot handle incomplete Objects
     //of type richcalpar. The streamer bombs when they are written to a
     //root file. Therfore we fill things completele before making a root file
     //from them. (Tassilo 08.04.2002)
     mydata.WriteRawLine(row, col, slope, offset, sigma);
    }
  }
 
 } // eof try block

 catch (Bool_t ret)
 {
  status = ret;
 }
 catch (...)
 {
  status = kFALSE;
  ErrorMsg(2,"writeAscii",1,"Unidentified exception catched.");
 }

 if (status)
 {
    ErrorMsg(0,"writeAscii",
             1,"Container \'RichCalPar\' has been written to ASCII file.");
 }

 return status;
}
//============================================================================

//----------------------------------------------------------------------------
void HRichCalPar::PrintCalPar() {
  Int_t i, k, l;
  Float_t slope, offset,sigma;

  HLocation loc;

  loc.setNIndex(3);
  for (i = 0; i < fSectors; i++)
      for (k = 0; k < fRows; k++)
          for (l = 0; l < fColumns; l++) {
             loc[0] = i;
             loc[1] = k;
             loc[2] = l;
	     slope  = -1;
             offset = -1;
	     sigma  = -1;
             if ( NULL != getObject(loc) )
             {
                slope  = ((HRichCalParCell*) getObject(loc))->getSlope();
                offset = ((HRichCalParCell*) getObject(loc))->getOffset();
                sigma  = ((HRichCalParCell*) getObject(loc))->getSigma();
             }
             if ( offset < 0 )
             {
                printf("%d:%d:%d %f %f\n", i, k, l, slope, offset);
             }
          }
    
}
//============================================================================








